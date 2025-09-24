#include <libcc.h>
#include <libcc/widgets/widgets.h>
#include <stdio.h>

_cc_OpenSSL_t *openSSL;

typedef struct _WebSocket {
    uint8_t status;
    uint8_t handshake;
    _cc_http_request_header_t *request;
    _cc_buf_t buffer;
    _cc_io_buffer_t *io;
    int64_t payload;
    char_t websocket_key[256];
} _WebSocket_t;

/**/
_CC_API_PRIVATE(void) _WebSocketSecKey(const tchar_t *sec_websocket_key, _WebSocket_t *ws) {
    _cc_sha1_t ctx;
    size_t length;
    byte_t results[1024];
    byte_t sha1_results[_CC_SHA1_DIGEST_LENGTH_];

    length = _snprintf((char_t*)results, _cc_countof(results), "%s258EAFA5-E914-47DA-95CA-C5AB0DC85B11", sec_websocket_key);

    _cc_logger_debug("Sec-WebSocket-Key: %s",sec_websocket_key);
    _cc_sha1_init(&ctx);
    _cc_sha1_update(&ctx, results, length);
    _cc_sha1_final(&ctx, sha1_results);
    _cc_base64_encode(sha1_results, _CC_SHA1_DIGEST_LENGTH_, ws->websocket_key, _cc_countof(ws->websocket_key));
}

/**/
_CC_API_PRIVATE(bool_t) _WebSocketResponseHeader(_cc_event_t *e, _WebSocket_t *ws) {
    _cc_io_buffer_t *io = (_cc_io_buffer_t*)ws->io;
    io->w.off = _snprintf((char*)io->w.bytes, io->w.limit,
                        "HTTP/1.1 101 Switching Protocols\r\n"
                        "Connection: Upgrade\r\n"
                        "Upgrade: websocket\r\n"
                        "Sec-WebSocket-Protool: chat\r\n"
                        "Sec-WebSocket-Accept: %s\r\n\r\n",
                        ws->websocket_key);

    //printf("send: %.*s\n",io->w.off, io->w.bytes);
    return _cc_io_buffer_flush(e, io);
}

_CC_API_PRIVATE(int64_t) _WebSocketGetContentLength(_cc_rbtree_t *headers) {
    const _cc_http_header_t *data = _cc_http_header_find(headers, _T("Content-Length"));
    return data ? _ttoi(data->value) : 0;
}

/**/
_CC_API_PRIVATE(bool_t) _Heartbeat(_cc_event_t *e, byte_t oc) {
    _WebSocket_t *ws = (_WebSocket_t*)e->data;
    _cc_io_buffer_t *io = (_cc_io_buffer_t*)ws->io;
    byte_t buf[2];
    buf[0] = 0x80 | oc;
    buf[1] = 0;
    return (_cc_io_buffer_send(e, io, buf, 2) > 0);
}

/**/
_CC_API_PRIVATE(void) _WebSocketFree(_WebSocket_t *ws) {
    if (ws->request) {
        _cc_http_free_request_header(&ws->request);
    }
    _cc_free_buf(&ws->buffer);
    if (ws->io) {
        _cc_free_io_buffer(ws->io);
    }
    _cc_free(ws);
}

/**/
_CC_API_PRIVATE(bool_t) _WebSocketData(_cc_event_t *e) {
    _WebSocket_t *ws = (_WebSocket_t*)e->data;
    _cc_io_buffer_t *io = (_cc_io_buffer_t*)ws->io;
    _CCWSHeader_t header = {0};

    header.bytes = io->r.bytes;
    header.length = io->r.off;
    header.offset = 0;

    do {
        if (_CCWSRead(&header) == WS_DATA_OK) {
            if (header.payload > 0 && header.mask) {
                //Get the complete packet
                _CCWSMask(header.bytes + header.offset, header.payload, header.mask);
            }
            //You can handle the packet
            //fn(header, header.bytes + header.offset, header.payload);
            switch (header.opc) {
                case WS_OP_PING:
                    _Heartbeat(e, WS_OP_PONG);
                    break;
                case WS_OP_PONG:
                    break;
                case WS_OP_BINARY:
                case WS_OP_JSON:
                case WS_OP_XML:
                case WS_OP_TXT:
                    _tprintf("WS:%.*s\n",(int)header.payload, header.bytes + header.offset);
                    break;
                case WS_OP_DISCONNECT:
                    return false;
            }
            header.offset += header.payload;
        } else {
            if (header.payload > io->r.limit) {
                _tprintf("big data fail\n");
                return false;
                /*
                //If you want big data.
                //This is just a simple example, you can optimize it
                _cc_alloc_event_rbuf(r,header.payload + _WS_MAX_HEADER_)
                header.bytes = io->r.bytes;
                header.length = io->r.off;
                */
            }
            break;
        }
    } while(header.length > header.offset);

    if (header.offset > 0 && header.length >= header.offset) {
        header.length -= header.offset;
        memmove(io->r.bytes, io->r.bytes + header.offset, header.length);
        io->r.off = (int32_t)header.length;
    }
    return true;
}

static bool_t network_event_callback(_cc_async_event_t *async, _cc_event_t *e, const uint32_t which) {
    _WebSocket_t *ws = (_WebSocket_t*)e->data;
    if (which & _CC_EVENT_ACCEPT_) {
        _cc_event_t *event;
        _cc_socket_t fd;
        uint32_t timeout = e->timeout;
        uint32_t flags = _CC_EVENT_TIMEOUT_;
        struct sockaddr_in remote_addr = {0};
        _cc_socklen_t remote_addr_len = sizeof(struct sockaddr_in);

        fd = async->accept(async, e, (_cc_sockaddr_t*)&remote_addr, &remote_addr_len);
        if (fd == _CC_INVALID_SOCKET_) {
            _cc_logger_error(_T("accept fail %s."), _cc_last_error(_cc_last_errno()));
            return true;
        }

        _cc_set_socket_nonblock(fd, 1);

        ws = (_WebSocket_t*)_cc_malloc(sizeof(_WebSocket_t));
        ws->buffer.length = 0;
        ws->buffer.limit = 0;
        ws->buffer.bytes = nullptr;
        ws->request = nullptr;
        ws->payload = 0;
        ws->io = _cc_alloc_io_buffer(_CC_IO_BUFFER_SIZE_);
        ws->status = _CC_HTTP_STATUS_HEADER_;
        ws->io->ssl = _SSL_accept(openSSL, fd, &ws->handshake);

        if (ws->handshake == _CC_SSL_HS_ERROR_) {
            _cc_logger_error(_T("SSL accept fail %s."), _cc_last_error(_cc_last_errno()));
            _WebSocketFree(ws);
            _cc_close_socket(fd);
            return true;
        } else if (ws->handshake == _CC_SSL_HS_ESTABLISHED_) {
            flags |= _CC_EVENT_READABLE_;
        } else {
            //wait for handshake
            timeout = 1000;
        }

        event = _cc_event_alloc(async, flags);
        if (event == nullptr) {
            _cc_close_socket(fd);
            _WebSocketFree(ws);
            return true;
        }

        event->fd = fd;
        event->callback = e->callback;
        event->timeout = timeout;
        event->data = (uintptr_t)ws;

        if (async->attach(async, event) == false) {
            _cc_logger_debug(_T("thread %d add socket (%d) event fial."), _cc_get_thread_id(nullptr), fd);
            _cc_free_event(async, event);
            _WebSocketFree(ws);
            return true;
        }

        {
            struct sockaddr_in *remote_ip = (struct sockaddr_in *)&remote_addr;
            byte_t *ip_addr = (byte_t *)&remote_ip->sin_addr.s_addr;
            _cc_logger_debug(_T("TCP accept [%d,%d,%d,%d] fd:%d"), ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3], fd);
        }

        return true;
    }

    if (which & _CC_EVENT_CLOSED_) {
        _cc_logger_debug(_T("%d disconnect to client."), e->fd);
        if (e->data) {
            _WebSocketFree((_WebSocket_t*)e->data);
        }
        return false;
    }

    if (which & _CC_EVENT_READABLE_) {
        _cc_io_buffer_t *io = (_cc_io_buffer_t*)ws->io;
        int32_t off = _cc_io_buffer_read(e, io);
        if (off < 0) {
            _cc_logger_debug(_T("read fail %s."), _cc_last_error(_cc_last_errno()));
            return false;
        } else if (off == 0) {
            return true;
        }

        if (ws->status == _CC_HTTP_STATUS_ESTABLISHED_) {
            if(_WebSocketData(e)) {
                return true;
            }
            return false;
        }

        if (ws->status == _CC_HTTP_STATUS_HEADER_) {
            const _cc_http_header_t *connection, *upgrade, *sec_websocket_key;
            ws->status = _cc_http_header_parser((_cc_http_header_fn_t)_cc_http_alloc_request_header, (pvoid_t *)&ws->request, io->r.bytes, &io->r.off);
            /**/
            if (ws->status != _CC_HTTP_STATUS_PAYLOAD_) {
                return ws->status == _CC_HTTP_STATUS_HEADER_;
            }

            ws->payload = _WebSocketGetContentLength(&ws->request->headers);
            if (ws->payload == 0) {
                ws->status = _CC_HTTP_STATUS_ESTABLISHED_;
            }

            if (ws->buffer.bytes == nullptr && ws->payload > 0) {
                _cc_alloc_buf(&ws->buffer, (size_t)ws->payload);
            }
            connection = _cc_http_header_find(&ws->request->headers,_T("Connection"));
            upgrade = _cc_http_header_find(&ws->request->headers, _T("Upgrade"));
            sec_websocket_key = _cc_http_header_find(&ws->request->headers, _T("Sec-WebSocket-Key"));
            if (connection == nullptr || upgrade == nullptr || sec_websocket_key == nullptr) {
                return false;
            }
            if (_tcsicmp("Upgrade",connection->value) != 0 || _tcsicmp("websocket",upgrade->value) != 0) {
                return false;
            }
            _WebSocketSecKey(sec_websocket_key->value, ws);
        } 

        if (ws->status == _CC_HTTP_STATUS_PAYLOAD_) {
            _cc_buf_append(&ws->buffer, io->r.bytes, io->r.off);
            if (ws->buffer.length >= ws->payload) {
                ws->status = _CC_HTTP_STATUS_ESTABLISHED_;
            }
            io->r.off = 0;
        }

        if (ws->status == _CC_HTTP_STATUS_ESTABLISHED_) {
            return _WebSocketResponseHeader(e,ws);
        }

        return true;
    }
 
    if (which & _CC_EVENT_WRITABLE_) {

    }

    if (which & _CC_EVENT_TIMEOUT_) {
        if (ws->handshake != _CC_SSL_HS_ESTABLISHED_) {
            ws->handshake = _SSL_do_handshake(ws->io->ssl);
            _cc_logger_debug(_T("SSL do handshake %d"), ws->handshake);
            if (ws->handshake == _CC_SSL_HS_ESTABLISHED_) {
                e->flags |= _CC_EVENT_READABLE_;
                e->timeout = 60000;
            }
            return true;
        }
        if (_Heartbeat(e, WS_OP_PONG)) {
            return true;
        }
        _cc_logger_debug(_T("TCP timeout %d"), e->fd);
        return false;
    }

    return true;
}
int main(int argc, char *const argv[]) {
    // char c = 0;
    struct sockaddr_in sa;
    _cc_async_event_t async;
    _cc_event_t *e;
    uint16_t port = 5500;

    _cc_install_socket();
    openSSL = _SSL_init(false);
    if (openSSL == nullptr) {
        return 1;
    }

    //_SSL_setup(openSSL, "./ssl/ws.libcc.cn_bundle.crt", "./ssl/ws.libcc.cn.key",nullptr);
    _SSL_setup(openSSL, "/opt/homebrew/etc/nginx/ssl/ws.libcc.cn_bundle.crt", "/opt/homebrew/etc/nginx/ssl/ws.libcc.cn.key",nullptr);

    if (_cc_register_poller(&async) == false) {
        return 1;
    }
    e = _cc_event_alloc(&async, _CC_EVENT_ACCEPT_);
    if (e == nullptr) {
        async.free(&async);
        return -1;
    }
    e->callback = network_event_callback;
    e->timeout = 60000;

    _cc_inet_ipv4_addr(&sa, nullptr, port);
    _cc_tcp_listen(&async, e, (_cc_sockaddr_t *)&sa, sizeof(struct sockaddr_in));
    _cc_logger_debug(_T("listen port: %d"), port);

    while (1) {
        // while((c = getchar()) != 'q') {
        async.wait(&async, 100);
    }

    async.free(&async);
    return 0;
}