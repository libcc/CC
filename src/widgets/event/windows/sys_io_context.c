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
#include "sys_socket_c.h"

#ifdef _CC_EVENT_USE_IOCP_

_CC_API_PUBLIC(void) _io_context_init(_cc_async_event_priv_t *priv) {
    _cc_list_iterator_cleanup(&priv->io_active);
    _cc_list_iterator_cleanup(&priv->io_idle);
    priv->frees = 0;
}

_CC_API_PUBLIC(void) _io_context_quit(_cc_async_event_priv_t *priv) {
    _cc_list_iterator_for_each(it, &priv->io_active, {
        _cc_free(_cc_upcast(it, _io_context_t, lnk));
    });
    _cc_list_iterator_for_each(it, &priv->io_idle, {
        _cc_free(_cc_upcast(it, _io_context_t, lnk));
    });
    _cc_list_iterator_cleanup(&priv->io_active);
    _cc_list_iterator_cleanup(&priv->io_idle);
}

_CC_API_PUBLIC(_io_context_t*) _io_context_alloc(_cc_async_event_priv_t *priv, _cc_event_t *e) {
    _io_context_t *io_context;
    _cc_list_iterator_t *lnk = _cc_list_iterator_pop(&priv->io_idle);

    if (lnk == &priv->io_idle) {
        io_context = (_io_context_t *)_cc_malloc(sizeof(_io_context_t));
        lnk = &(io_context->lnk);
    } else {
        io_context = _cc_upcast(lnk, _io_context_t, lnk);
        priv->frees--;
    }
	
    bzero(io_context, sizeof(_io_context_t));
    io_context->fd = _CC_INVALID_SOCKET_;
    io_context->e = e;
	io_context->number_of_bytes = 0;
    io_context->ident = e->ident;
    
    _cc_list_iterator_push(&priv->io_active, &(io_context->lnk));

    return io_context;
}

_CC_API_PUBLIC(void) _io_context_free(_cc_async_event_priv_t *priv, _io_context_t *io_context) {

    if (io_context->fd != _CC_INVALID_SOCKET_) {
        _cc_close_socket(io_context->fd);
        io_context->fd = _CC_INVALID_SOCKET_;
    }
    
    if (priv->frees >= _CC_MAX_CHANGE_EVENTS_) {
        _cc_list_iterator_remove(&io_context->lnk);
        _cc_free(io_context);
        return;
    }

    _cc_list_iterator_swap(&priv->io_idle, &io_context->lnk);
    priv->frees++;
    return;
}

#endif