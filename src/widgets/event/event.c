/*
 * Copyright libcc.cn@gmail.com. and other libcc contributors.
 * All rights reserved.org>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:

 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/
#include <libcc/alloc.h>
#include <libcc/thread.h>
#include <libcc/queue.h>
#include "event.c.h"

#if defined(__CC_LINUX__)
#include <sys/resource.h>
#endif

#define _CC_MAX_STEP_           64

static struct {
    _cc_atomic32_t async_limit;
    _cc_atomic32_t refcount;
    _cc_atomic32_t slot_refcount;
    int32_t slot_length;
    int32_t slot_limit;
    _cc_queue_iterator_t idles;

    _cc_async_event_t **async;
    _cc_event_t **slots;
} g = {0};

_CC_API_PRIVATE(int32_t) _get_max_limit(void) {
#if defined(__CC_LINUX__) || defined(__CC_APPLE__)
    struct rlimit limit;
    //1048576
    if (getrlimit(RLIMIT_NOFILE, &limit) == 0) {
        //printf("rlim_cur =%lld,rlim_max =%lld\n",limit.rlim_cur,limit.rlim_max);
        return (int32_t)(limit.rlim_cur > 0x0FFFFF ? 0x0FFFFF : limit.rlim_cur);
    }
#endif
    return 0x0FFFFF;
}

_CC_API_PRIVATE(_cc_event_t*) _cc_reserve_event(uint16_t baseid) {
    _cc_queue_iterator_t *lnk;
    _cc_event_t *e;
    do {
        lnk = _cc_queue_sync_pop(&g.idles);

        if (lnk != &g.idles && lnk != nullptr) {
            e = _cc_upcast(lnk, _cc_event_t, lnk);
            break;
        }

        if (_cc_atomic32_cas(&g.slot_refcount, 0, 1)) {
            int32_t i,j;
            int32_t expand_length = g.slot_length + _CC_MAX_STEP_;
            _cc_event_t *data;

            if (g.slot_limit <= g.slot_length) {
                _cc_logger_error(_T("The maximum number of event supported by the RLIMIT_NOFILE is %d"), g.slot_limit);
                return nullptr;
            }

            /*If the allocation fails, it directly aborts, so there is no need to check whether the application is successful, which is meaningless.*/
            g.slots = (_cc_event_t **)_cc_realloc(g.slots, sizeof(_cc_event_t*) * expand_length);
            data = (_cc_event_t *)_cc_calloc(sizeof(_cc_event_t), _CC_MAX_STEP_);

            for (i = g.slot_length, j = 0; j < _CC_MAX_STEP_; ++i,++j) {
                _cc_event_t *e = data + j;
                g.slots[i] = e;
                e->ident = i;
                _cc_queue_sync_push(&g.idles, (_cc_queue_iterator_t*)(&e->lnk));
            }
            g.slot_length = expand_length;
            //g.slot_refcount = 0;
            _cc_atomic32_set(&g.slot_refcount, 0);
        } else {
            _cc_sleep(0);
        }
    } while(1);

    e->ident = (uint32_t)(baseid << 20) | (e->ident & 0x0FFFFF);
    return e;
}

/**/
_CC_API_PUBLIC(_cc_async_event_t*) _cc_get_async_event(void) {
    _cc_async_event_t *async = nullptr;
    static uint16_t index = 0;
    int32_t i;
    for (i = 0; i < (int32_t)g.async_limit; i++,index++) {
        async = g.async[index % g.async_limit];
        if (async && async->running != 0) {
            break;
        }
    }

#if 0
    _cc_async_event_t *n;
    int32_t i, count;

    async = nullptr;
    if (g.async_limit > 1) {
        i = (int32_t)(rand() % g.async_limit);
        count = (int32_t)(g.async_limit + i);
    } else {
        i = 0;
        count = (int32_t)g.async_limit;
    }

    for (; i < count; i++) {
        n = (_cc_async_event_t *)g.async[i % g.async_limit];
        if (n == nullptr || n->running == 0) {
            continue;
        }

        if (async == nullptr || n->processed < async->processed) {
            async = n;
        }
    }
#endif
    _cc_assert(async != nullptr);
    return async;
}

/**/
_CC_API_PUBLIC(_cc_event_t*) _cc_get_event_by_id(uint32_t ident) {
	int32_t index = (int32_t)(ident & 0x0FFFFF);
    if (g.slot_length <= index) {
        return nullptr;
    }
#ifdef _CC_DEBUG_
    {
        _cc_event_t *e = g.slots[index];
        _cc_assert(e != nullptr);
        if (e->ident != ident) {
            _cc_logger_error(_T("event id:%d is deleted"), ident);
            return nullptr;
        }
        return e;
    }
#else
    return g.slots[index];
#endif
    
}

/**/
_CC_API_PUBLIC(_cc_async_event_t*) _cc_get_async_event_by_id(uint32_t ident) {
    int16_t i = (ident >> 20) & 0x0FFF;
    if (g.async_limit <= i) {
        _cc_logger_error(_T("async_event id:%d is unregistered!"), ident);
        return nullptr;
    }
    return (_cc_async_event_t *)g.async[i];
}

/**/
_CC_API_PUBLIC(_cc_event_t*) _cc_event_alloc(_cc_async_event_t *async, const uint32_t flags) {
    _cc_event_t *e = _cc_reserve_event(async->ident);
    if (_cc_unlikely(e == nullptr)) {
        return nullptr;
    }

    e->marks = _CC_EVENT_UNKNOWN_;
    e->flags = flags;
    e->fd = _CC_INVALID_SOCKET_;
    e->args = nullptr;
    e->callback = nullptr;
    e->buffer = nullptr;
    e->expire = 0;
    e->timeout = 0;

    if (_CC_EVENT_IS_SOCKET(flags)) {
        e->flags |= _CC_EVENT_DESC_SOCKET_;
    }

    if (_CC_ISSET_BIT(_CC_EVENT_BUFFER_, flags)) {
        e->buffer = _cc_alloc_event_buffer();
    }

#ifdef _CC_EVENT_USE_IOCP_
    e->accept_fd = _CC_INVALID_SOCKET_;
#endif

    _cc_list_iterator_cleanup(&e->lnk);
    return e;
}

/**/
_CC_API_PUBLIC(void) _cc_free_event(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_socket_t fd;

    if (e->buffer) {
        _cc_free_event_buffer(e->buffer);
        e->buffer = nullptr;
    }

    if (!_cc_list_iterator_empty(&e->lnk)) {
        _cc_list_iterator_remove(&e->lnk);
    }

    fd = e->fd;

    e->ident = (e->ident & 0xFFFFF);
    e->fd = _CC_INVALID_SOCKET_;
    e->flags = _CC_EVENT_UNKNOWN_;
    e->marks = _CC_EVENT_UNKNOWN_;

    if (fd != _CC_INVALID_SOCKET_ && fd != 0) {
        _cc_close_socket(fd);
    }
    _cc_queue_sync_push(&g.idles, (_cc_queue_iterator_t*)(&e->lnk));
}

/*
_CC_API_PUBLIC(void) _cc_print_cycle_processed(void) {
    uint32_t i;
    for (i = 0; i < g.cycles.length; i++) {
        _cc_async_event_t *async = (_cc_async_event_t*)g.async[i];
        if (async) {
            printf("%d: %d, ", i, async->processed);
        }
    }
    putchar('\n');
}
*/
/**/
_CC_API_PUBLIC(bool_t) _register_async_event(_cc_async_event_t *async) {
    int32_t i, j;
    int32_t async_limit;
    _cc_assert(async != nullptr);

    if (_cc_atomic32_inc_ref(&g.refcount)) {
        _cc_event_t *data;
        _cc_queue_iterator_cleanup(&g.idles);
        /*If the allocation fails, it directly aborts, so there is no need to check whether the application is successful, which is meaningless.*/
        g.slots = (_cc_event_t **)_cc_calloc(sizeof(_cc_event_t*), _CC_MAX_STEP_);
        data = (_cc_event_t *)_cc_calloc(sizeof(_cc_event_t), _CC_MAX_STEP_);

        for (i = 0; i < _CC_MAX_STEP_; i++) {
            _cc_event_t *e = (data + i);
            g.slots[i] = e;
            e->ident = i;
            _cc_queue_iterator_push(&g.idles, (_cc_queue_iterator_t*)(&e->lnk));
        }
        
        g.slot_limit = _get_max_limit();
        g.slot_length = _CC_MAX_STEP_;
        g.slot_refcount = 0;
        g.async_limit = 0;
        g.async = _cc_calloc(0xFFF, sizeof(_cc_async_event_t*));
    }

    while (g.async == nullptr) {
        _cc_sleep(10);
    }

    if (g.async_limit >= 0xFFF) {
        async_limit = 0xFFFF;
        for (i = 0; i < g.async_limit; i++) {
            if (_cc_atomic_cas((_cc_atomic_t*)&g.async[i], 0, (intptr_t)async)) {
                async_limit = i;
                break;
            }
        }
        if (async_limit == 0xFFFF) {
            _cc_logger_error(_T("The maximum number of events supported by asynchronous events is %d"), g.async_limit);
            return false;
        }
    } else {
        async_limit = _cc_atomic32_inc(&g.async_limit);
        g.async[async_limit] = async;
    }

    async->changes = _cc_alloc_array(_CC_MAX_CHANGE_EVENTS_);
    async->processed = 0;
    async->running = 0;
    async->timer = 0;
    async->diff = 0;
    async->tick = _cc_get_ticks();
    async->ident = (uint16_t)async_limit & 0xFFF;
#ifdef _CC_EVENT_USE_MUTEX_
    async->lock = _cc_alloc_mutex();
#else
    _cc_lock_init(&async->lock);
#endif

    for (i = 0; i < _CC_TIMEOUT_NEAR_; i++) {
        _cc_list_iterator_cleanup(&async->nears[i]);
    }

    for (i = 0; i < _CC_TIMEOUT_MAX_LEVEL_; i++) {
        for (j = 0; j < _CC_TIMEOUT_LEVEL_; j++) {
            _cc_list_iterator_cleanup(&async->level[i][j]);
        }
    }

    _cc_list_iterator_cleanup(&async->pending);
    _cc_list_iterator_cleanup(&async->no_timer);

    async->running = 1;
    return true;
}

_CC_API_PRIVATE(void) _event_link_free(_cc_async_event_t *async, _cc_list_iterator_t *head) {
    _cc_list_iterator_t *next;
    _cc_list_iterator_t *curr;
    _cc_event_t *e;

    next = head->next;
    _cc_list_iterator_cleanup(head);

    while (_cc_likely(next != head)) {
        curr = next;
        next = next->next;

        e = _cc_upcast(curr, _cc_event_t, lnk);
        if (e->callback && ((e->flags & _CC_EVENT_DISCONNECT_) == 0)) {
            e->callback(async, e, _CC_EVENT_DISCONNECT_);
        }
        _cc_free_event(async, e);
    }
}

/**/
_CC_API_PUBLIC(bool_t) _unregister_async_event(_cc_async_event_t *async) {
    int32_t i, j;
    _cc_assert(async != nullptr);

    _event_lock(async);
    async->running = 0;
    _cc_array_for_each(_cc_event_t, e, i, async->changes, {
        _cc_list_iterator_swap(&async->pending, &e->lnk);
    });
    _cc_free_array(async->changes);
    _event_unlock(async);
    
    for (i = 0; i < _CC_TIMEOUT_NEAR_; i++) {
        _event_link_free(async, &async->nears[i]);
    }

    for (i = 0; i < _CC_TIMEOUT_MAX_LEVEL_; i++) {
        for (j = 0; j < _CC_TIMEOUT_LEVEL_; j++) {
            _event_link_free(async, &async->level[i][j]);
        }
    }
    _event_link_free(async, &async->no_timer);
    _event_link_free(async, &async->pending);

    if (_cc_atomic32_dec_ref(&g.refcount)) {;
        //
        for (i = 0; i < g.slot_length; i += _CC_MAX_STEP_) {
            _cc_free(g.slots[i]);
        }
        _cc_free(g.slots);
        _cc_free(g.async);
        _cc_queue_iterator_cleanup(&g.idles);
        g.slot_length = 0;
        g.slots = nullptr;
        g.async = nullptr;
    } else {
        g.async[async->ident] = 0;
    }
    return true;
}

/**/
_CC_API_PUBLIC(bool_t) _valid_event(_cc_async_event_t *async, _cc_event_t *e) {
    return (((e->ident >> 20) & 0xFFF) == async->ident);
}

/**/
_CC_API_PUBLIC(uint32_t) _valid_connected(_cc_event_t *e, uint32_t which) {
    if (_valid_event_fd(e)) {
        _CC_MODIFY_BIT(_CC_EVENT_READABLE_, _CC_EVENT_CONNECT_, e->flags);
        return which;
    }
    
    return _CC_EVENT_DISCONNECT_;
}

/**/
_CC_API_PUBLIC(bool_t) _event_callback(_cc_async_event_t *async, _cc_event_t *e, uint32_t which) {
    /**/
    async->processed++;
#ifndef _CC_EVENT_USE_IOCP_
    if (e->buffer && (e->flags & _CC_EVENT_BUFFER_)) {
        if ( (which & _CC_EVENT_READABLE_) ) {
            if (!_cc_event_recv(e)) {
                which = _CC_EVENT_DISCONNECT_;
            }
        } else if ( (which & _CC_EVENT_WRITABLE_) ) {
            if (_cc_event_sendbuf(e) < 0) {
                which = _CC_EVENT_DISCONNECT_;
            }
        }
    }
#endif
    
    _cc_list_iterator_swap(&async->pending, &e->lnk);

    /**/
    if (e->callback && e->callback(async, e, which)) {
        if ((e->flags & _CC_EVENT_DISCONNECT_) == 0) {
            return true;
        }
    }
    /**/
    if ((e->flags & _CC_EVENT_WRITABLE_) == 0 && (which & _CC_EVENT_DISCONNECT_) == 0) {
        e->callback(async, e, _CC_EVENT_DISCONNECT_);
    }

    /*force disconnect*/
    _CC_MODIFY_BIT(_CC_EVENT_DISCONNECT_, _CC_EVENT_READABLE_|_CC_EVENT_ACCEPT_, e->flags);
    return false;
}

/**/
_CC_API_PUBLIC(bool_t) _reset_event(_cc_async_event_t *async, _cc_event_t *e) {
    bool_t results = true;
    if (async->running == 0) {
        return false;
    }

    if (_CC_ISSET_BIT(_CC_EVENT_CHANGING_, e->flags) == 0) {
        _event_lock(async);
        _cc_array_push(&async->changes, (uintptr_t)e);
        _CC_SET_BIT(_CC_EVENT_CHANGING_, e->flags);
        _event_unlock(async);
    }

    return results;
}

/**/
_CC_API_PUBLIC(void) _reset_event_timeout(_cc_async_event_t *async, _cc_event_t *e) {
    if (_CC_ISSET_BIT(_CC_EVENT_TIMEOUT_, e->flags)) {
        e->expire = async->timer + e->timeout;
        _add_event_timeout(async, e);
    } else {
        _cc_list_iterator_swap(&async->no_timer, &e->lnk);
    }
}

/**/
_CC_API_PUBLIC(void) _reset_event_pending(_cc_async_event_t *async, void (*_reset)(_cc_async_event_t *, _cc_event_t *)) {
    _cc_list_iterator_t *head;
    _cc_list_iterator_t *next;
    _cc_list_iterator_t *curr;
    size_t length = _cc_array_length(async->changes);

    if (length > 0) {
        size_t i;
        _cc_event_t *e;
        _event_lock(async);
        for (i = 0; i < length; i++) {
            e = ((_cc_event_t*)*((uintptr_t*)(async->changes) + i));
            _CC_UNSET_BIT(_CC_EVENT_CHANGING_, e->flags);
            _cc_list_iterator_swap(&async->pending, &e->lnk);
        }
        _cc_array_cleanup(async->changes);
        _event_unlock(async);
    }

    head = &async->pending;
    next = head->next;
    _cc_list_iterator_cleanup(&async->pending);

    while (_cc_likely(next != head)) {
        curr = next;
        next = next->next;
        _reset(async, _cc_upcast(curr, _cc_event_t, lnk));
    }
}

/**/
_CC_API_PUBLIC(bool_t) _disconnect_event(_cc_async_event_t *async, _cc_event_t *e) {
    /**/
    if (e->flags & (_CC_EVENT_DESC_SOCKET_ | _CC_EVENT_DESC_FILE_)) {
        _cc_shutdown_socket(e->fd, _CC_SHUT_RD_);
    }
    
    _CC_MODIFY_BIT(_CC_EVENT_DISCONNECT_, _CC_EVENT_READABLE_, e->flags);

    return async->reset(async, e);
}

/**/
_CC_API_PUBLIC(bool_t) _valid_event_fd(_cc_event_t *e) {
    int results = 0;
    socklen_t len = sizeof(results);

    if (_cc_unlikely(e->fd == _CC_INVALID_SOCKET_)) {
        return false;
    }

    if (getsockopt(e->fd, SOL_SOCKET, SO_ERROR, (char *)&results, &len) != 0) {
        results = _cc_last_errno();
        _cc_logger_error(_T("Socket Error:%d, %s"), results, _cc_last_error(results));
        return false;
    }

    if (results != 0) {
        _cc_logger_error(_T("Socket Error:%d, %s"), results, _cc_last_error(results));
        return false;
    }

    return true;
}

/**/
_CC_API_PUBLIC(bool_t) _cc_event_attach(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_assert(async != nullptr && async->attach != nullptr);
    return async->attach(async, e);
}

/**/
_CC_API_PUBLIC(bool_t) _cc_event_connect(_cc_async_event_t *async, _cc_event_t *e, const _cc_sockaddr_t *sa, const _cc_socklen_t sa_len) {
    _cc_assert(async != nullptr && async->connect != nullptr && sa != nullptr);
    return async->connect(async, e, sa, sa_len);
}

/**/
_CC_API_PUBLIC(_cc_socket_t) _cc_event_accept(_cc_async_event_t *async, _cc_event_t *e, _cc_sockaddr_t *sa, _cc_socklen_t *sa_len) {
    _cc_assert(async != nullptr && async->accept != nullptr && e != nullptr);
    return async->accept(async, e, sa, sa_len);
}

/**/
_CC_API_PUBLIC(bool_t) _cc_event_wait(_cc_async_event_t *async, uint32_t timeout) {
    _cc_assert(async != nullptr && async->wait != nullptr);
    return async->wait(async, timeout);
}
