#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <libcc/widgets/event.h>
#include <libcc/widgets/timeout.h>

class TCP {
    TCP() {

    }
    virtual ~TCP() {

    }
public:
    virtual bool_t onAccept(_cc_async_event_t *async, _cc_event_t *e) {
        return true;
    }
    virtual bool_t onConnect(_cc_async_event_t *async, _cc_event_t *e) {
        return true;
    }
    virtual bool_t onDisconnect(_cc_async_event_t *async, _cc_event_t *e) = 0;
    virtual bool_t onRead(_cc_async_event_t *async, _cc_event_t *e) = 0;
    virtual bool_t onWrite(_cc_async_event_t *async, _cc_event_t *e) = 0;
    virtual bool_t onTimeout(_cc_async_event_t *async, _cc_event_t *e) = 0;
    
    static bool_t callback(_cc_async_event_t *async, _cc_event_t *e, const uint16_t which) {
        TCP *tcp = (TCP *)e->args;
        if (which & _CC_EVENT_ACCEPT_) {
            tcp->onAccept(async,e);
            return true;
        } else if (which & _CC_EVENT_CONNECTED_) {
            return tcp->onConnect(async, e);
        }
        if (which & _CC_EVENT_DISCONNECT_) {
            return tcp->onDisconnect(async, e);
        }
        if (which & _CC_EVENT_READABLE_) {
            if (!tcp->onRead(async, e)) {
                return false;
            }
        }
        if (which & _CC_EVENT_WRITABLE_) {
            if (!tcp->onWrite(async, e)) {
                return false;
            }
        }
        if (which & _CC_EVENT_TIMEOUT_) {
            if (!tcp->onTimeout(async, e)) {
                return false;
            }
        }
        return true;
    }
};

class TCPServer:public TCP {
private:
    _cc_event_t *event;

public:
    TCPServer() {
        event = nullptr;
    }
    TCPServer(const tchar_t *host, uint16_t port) {
        event = nullptr;
        this->listen(host,port);
    }
    virtual ~TCPServer() {

    }

private:
    bool_t listen(const tchar_t *host, uint16_t port) {
        struct sockaddr_in sa;
        _cc_async_event_t *async = _cc_get_async_event();
        event = _cc_event_alloc(async, _CC_EVENT_ACCEPT_);
        assert(event != NULL);
        if (event == nullptr) {
            return false;
        }

        event->timeout = 10000;
        event->callback = &TCP::callback;

        _cc_inet_ipv4_addr(&sa, host, port);
        if (!_cc_tcp_listen(async, event, (_cc_sockaddr_t *)&sa, sizeof(struct sockaddr_in))) {
            _cc_free_event(async, event);    
            assert(FALSE);
            return false;
        }
        return true;
    }

    virtual bool_t onAccept(_cc_async_event_t *async, _cc_event_t *e) {
        _cc_socket_t fd;
        _cc_event_t *event;
        struct sockaddr_in remote_addr = {0};
        _cc_socklen_t remote_addr_len = sizeof(struct sockaddr_in);
        _cc_async_event_t *async2 = _cc_get_async_event();

        fd = _cc_event_accept(async, e, &remote_addr, &remote_addr_len);
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
    virtual bool_t onDisconnect(_cc_async_event_t *async, _cc_event_t *e) {
        return true;
    }
    virtual bool_t onRead(_cc_async_event_t *async, _cc_event_t *e) {
        return true;
    }
    virtual bool_t onWrite(_cc_async_event_t *async, _cc_event_t *e) {
        return true;
    }
    virtual bool_t onTimeout(_cc_async_event_t *async, _cc_event_t *e) {
        return true;
    }
};

class TCPClient:public TCP {
private:
    _cc_event_t *event;
public:
    TCPClient() {
        event = nullptr;
    }
    TCPClient(const tchar_t *host, uint16_t port) {

    }
    virtual ~TCPClient() {

    }
    virtual bool_t onConnect(_cc_async_event_t *async, _cc_event_t *e) {
        return true;
    }
    virtual bool_t onDisconnect(_cc_async_event_t *async, _cc_event_t *e) {
        return true;
    }
    virtual bool_t onRead(_cc_async_event_t *async, _cc_event_t *e) {
        return true;
    }
    virtual bool_t onWrite(_cc_async_event_t *async, _cc_event_t *e) {
        return true;
    }
    virtual bool_t onTimeout(_cc_async_event_t *async, _cc_event_t *e) {
        return true;
    }

    bool_t connect(const tchar_t *host, uint16_t port) {
        struct sockaddr_in sa;
        _cc_event_t *event;
        _cc_async_event_t *async = _cc_get_async_event();
        assert(async != NULL);

        event = _cc_event_alloc(async, _CC_EVENT_CONNECT_|_CC_EVENT_TIMEOUT_);
        assert(event != NULL);
        if (event == nullptr) {
            return false;
        }

        event->timeout = 6000;
        event->callback = &TCP::callback;

        _cc_inet_ipv4_addr(&sa, host, port);
        if (!_cc_tcp_connect(async, event, (_cc_sockaddr_t *)&sa, sizeof(struct sockaddr_in))) {
            _cc_free_event(async, event);
            return false;
        }
        return true;
    }
};

int main() {
    int c;
    _cc_install_async_event(0, nullptr);

    TCPServer* tcp = new TCPServer(nullptr, 3000);

    while((c = getchar()) != 'q') {
        _cc_sleep(100);
    }

    delete tcp;
    printf("All tests passed!\n");
    _cc_uninstall_async_event();
    return 0;
}