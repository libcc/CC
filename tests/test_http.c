#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <libcc/widgets/widgets.h>

typedef struct _http {
    byte_t status;
    int64_t payload;
    _cc_http_request_header_t *request;
    _cc_buf_t buffer;
} _http_t;

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

    _http_t *http = _cc_malloc(sizeof(_http_t));
    http->status = _CC_HTTP_STATUS_HEADER_;
    http->request = nullptr;
    http->payload = 0;
    _cc_alloc_buf(&http->buffer, 1024);

    event->fd = fd;
    event->callback = e->callback;
    event->timeout = e->timeout;
    event->args = http;

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
    return false;
}

_CC_API_PRIVATE(void) bad_request(_cc_event_t *e) {
    _cc_String_t body = _cc_String("<P>Your browser sent a bad request, such as a POST without a Content-Length.</P>");

    _cc_event_writef(e,"HTTP/1.1 400 BAD REQUEST\r\nConnection: close;\r\nContent-type: text/html\r\nContent-Length: %d\r\n\r\n", body.length);
    _cc_event_send(e,(byte_t*)body.data,body.length);
}
#if 0
_CC_API_PRIVATE(void) unimplemented(_cc_event_t *e) {
    _cc_String_t body = _cc_String("<HTML><HEAD><TITLE>Method Not Implemented</TITLE></HEAD><BODY><p>HTTP request method not supported.</p></BODY></HTML>");
    
    _cc_event_writef(e,"HTTP/1.1 501 Method Not Implemented\r\nConnection: close;\r\nContent-type: text/html\r\nContent-Length: %d\r\n\r\n", body.length);
    _cc_event_send(e,(byte_t*)body.data,body.length);
}
#endif
_CC_API_PRIVATE(void) request_ok(_cc_event_t *e) {
    _cc_String_t body = _cc_String("<HTML><HEAD><TITLE>Welcome to HTTP</TITLE></HEAD><BODY><p>If you see this page, the web server is successfully</p></BODY></HTML>");
    
    _cc_event_writef(e,"HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nContent-type: text/html\r\nContent-Length: %d\r\n\r\n", body.length);
    _cc_event_send(e,(byte_t*)body.data,body.length);
}

_CC_API_PRIVATE(int64_t) _get_content_length(_cc_rbtree_t *headers) {
    const _cc_http_header_t *data = _cc_http_header_find(headers, _T("Content-Length"));
    return data ? _ttoi(data->value) : 0;
}

static bool_t onRead(_cc_async_event_t *async, _cc_event_t *e) {
    _http_t *http = (_http_t*)e->args;
    _cc_event_rbuf_t *rbuf = &e->buffer->r;
    if (http->status == _CC_HTTP_STATUS_ESTABLISHED_) {
        return false;
    } else  if (http->status == _CC_HTTP_STATUS_HEADER_) {
        http->status = _cc_http_header_parser((_cc_http_header_fn_t)_cc_http_alloc_request_header, (pvoid_t *)&http->request, rbuf);
        /**/
        if (http->status == _CC_HTTP_STATUS_HEADER_) {
            return true;
        } else if (http->status != _CC_HTTP_STATUS_PAYLOAD_) {
            bad_request(e);
            return false;
        }

        http->payload = _get_content_length(&http->request->headers);
        if (http->payload == 0) {
            http->status = _CC_HTTP_STATUS_ESTABLISHED_;
        }

        if (http->buffer.bytes == nullptr && http->payload > 0) {
            _cc_alloc_buf(&http->buffer, (size_t)http->payload);
        }
    } 

    if (http->status == _CC_HTTP_STATUS_PAYLOAD_) {
        _cc_buf_append(&http->buffer, rbuf->bytes, rbuf->length);
        if (http->buffer.length >= http->payload) {
            http->status = _CC_HTTP_STATUS_ESTABLISHED_;
        }
        rbuf->length = 0;
    }

    if (http->status == _CC_HTTP_STATUS_ESTABLISHED_) {
        request_ok(e);
        http->status = _CC_HTTP_STATUS_HEADER_;

        _cc_logger_info("http:%s %s %s",http->request->method,http->request->script,http->request->protocol);

        if (_tcsicmp(http->request->method, _T("POST")) == 0) {
            _cc_logger_info("RAW:%.*s",http->buffer.length, http->buffer.bytes);
        }
        
        _cc_http_free_request_header(&http->request);
    }

    return true;
}

static bool_t onWrite(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_logger_debug(_T("%d onWrite."), e->ident);
    return true;
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

    event->timeout = 60000;
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

    addListener(nullptr, 8080);

    while((c = getchar()) != 'q') {
        _cc_sleep(100);
    }
    _cc_free_async_event();
    return 0;
}