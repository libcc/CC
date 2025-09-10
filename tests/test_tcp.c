#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <libcc/widgets/event.h>
#include <libcc/widgets/timeout.h>

static bool_t onAccept(_cc_async_event_t *async, _cc_event_t *e);
static bool_t onDisconnect(_cc_async_event_t *async, _cc_event_t *e);
static bool_t onRead(_cc_async_event_t *async, _cc_event_t *e);
static bool_t onWrite(_cc_async_event_t *async, _cc_event_t *e);
static bool_t onTimeout(_cc_async_event_t *async, _cc_event_t *e);

static bool_t doEvent(_cc_async_event_t *async, _cc_event_t *e, const uint32_t which) {
    if (which & _CC_EVENT_ACCEPT_) {
        onAccept(async,e);
        return true;
    } else if (which & _CC_EVENT_DISCONNECT_) {
        return onDisconnect(async, e);
    }
    if (which & _CC_EVENT_READABLE_) {
        if (!onRead(async, e)) {
            return false;
        }
    }
    if (which & _CC_EVENT_WRITABLE_) {
        if (!onWrite(async, e)) {
            return false;
        }
    }
    if (which & _CC_EVENT_TIMEOUT_) {
        if (!onTimeout(async, e)) {
            return false;
        }
    }
    return true;
}

static bool_t onAccept(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_socket_t fd;
    _cc_event_t *event;
    struct sockaddr_in remote_addr = {0};
    _cc_socklen_t remote_addr_len = sizeof(struct sockaddr_in);
    _cc_async_event_t *async2 = _cc_get_async_event();

    fd = _cc_event_accept(async, e, (_cc_sockaddr_t *)&remote_addr, &remote_addr_len);
    if (fd == _CC_INVALID_SOCKET_) {
        _cc_logger_debug(_T("thread %d accept fail %s."), _cc_get_thread_id(nullptr),
                         _cc_last_error(_cc_last_errno()));
        return false;
    }

    event = _cc_event_alloc(async2, _CC_EVENT_TIMEOUT_ | _CC_EVENT_READABLE_ | _CC_EVENT_BUFFER_);
    if (event == nullptr) {
        _cc_close_socket(fd);
        return false;
    }

    _cc_set_socket_nonblock(fd, 1);

    event->fd = fd;
    event->callback = e->callback;
    event->timeout = e->timeout;

    if (async2->attach(async2, event) == false) {
        _cc_logger_debug(_T("thread %d add socket (%d) event fial."), _cc_get_thread_id(nullptr), fd);
        _cc_free_event(async2, event);
        return false;
    }
    _cc_logger_debug(_T("%d accept."), event->ident);
    return true;
}

static bool_t onDisconnect(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_logger_debug(_T("%d onDisconnect."), e->ident);
    return true;
}
static bool_t onRead(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_logger_debug(_T("%d onRead."), e->ident);
    return true;
}
static bool_t onWrite(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_logger_debug(_T("%d onWrite."), e->ident);
    return false;
}
static bool_t onTimeout(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_logger_debug(_T("%d onTimeout."), e->ident);
    return false;
}

bool_t addListener(const tchar_t *host, uint16_t port) {
    struct sockaddr_in sa;
    _cc_async_event_t *async = _cc_get_async_event();
    _cc_event_t *event = _cc_event_alloc(async, _CC_EVENT_ACCEPT_);
    _cc_assert(async != NULL);
    _cc_assert(event != NULL);
    if (event == nullptr) {
        return false;
    }

    event->timeout = 10000;
    event->callback = doEvent;

    _cc_inet_ipv4_addr(&sa, host, port);
    if (!_cc_tcp_listen(async, event, (_cc_sockaddr_t *)&sa, sizeof(struct sockaddr_in))) {
        _cc_free_event(async, event);    
        _cc_assert(FALSE);
        return false;
    }
    return true;
}

int main() {
    int c;
    _cc_alloc_async_event(0, nullptr);

    addListener(nullptr, 3000);

    while((c = getchar()) != 'q') {
        _cc_sleep(100);
    }
    _cc_free_async_event();
    return 0;
}