#include <stdio.h>
#include <locale.h>
#include <libcc/widgets/event.h>

//A simple example program which connects to a daytime server and prints the response:
//

static bool_t onConnected(_cc_async_event_t *async, _cc_event_t *e);
static bool_t onDisconnect(_cc_async_event_t *async, _cc_event_t *e);
static bool_t onRead(_cc_async_event_t *async, _cc_event_t *e);
static bool_t onWrite(_cc_async_event_t *async, _cc_event_t *e);
static bool_t onTimeout(_cc_async_event_t *async, _cc_event_t *e);

static bool_t doEvent(_cc_async_event_t *async, _cc_event_t *e, const uint32_t which) {
    if (which & _CC_EVENT_CONNECTED_) {
        return onConnected(async, e);
    } else if (which & _CC_EVENT_DISCONNECT_) {
        return onDisconnect(async, e);
    } else if (which & _CC_EVENT_READABLE_) {
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

static bool_t onConnected(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_logger_info(_T("connect to server!"));
    return true;
}

static bool_t onDisconnect(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_logger_debug(_T("%d onDisconnect."), e->ident);
    return true;
}

static bool_t onRead(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_logger_debug(_T("%d onRead."), e->ident);
    //time_t ulTime = 0;
    byte_t buf[1024];
    size_t length = _cc_recv(e->fd, (byte_t*)&buf, 1024);
    if (length <= 0) {
        _tprintf(_T("TCP close %d\n"), e->fd);
        return false;
    }
    buf[length] = 0;
    _cc_logger_info(_T("%d,%s"), length, buf);
    return true;
}

static bool_t onWrite(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_logger_debug(_T("%d onWrite."), e->ident);
    return _cc_event_sendbuf(e) < 0;
}

static bool_t onTimeout(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_logger_debug(_T("%d onTimeout."), e->ident);
    return false;
}

bool_t addConnectListener(const tchar_t *host, uint16_t port) {
    struct sockaddr_in sa;
    _cc_event_t *event;
    _cc_async_event_t *async = _cc_get_async_event();
    _cc_assert(async != NULL);

    event = _cc_event_alloc(async, _CC_EVENT_CONNECT_|_CC_EVENT_TIMEOUT_);
    _cc_assert(event != NULL);
    if (event == nullptr) {
        return false;
    }

    event->timeout = 10000;
    event->callback = doEvent;

    _cc_inet_ipv4_addr(&sa, host, port);
    _cc_logger_info(_T("connect to %s:%d!"),host,port);
    if (!_cc_tcp_connect(async, event, (_cc_sockaddr_t *)&sa, sizeof(struct sockaddr_in))) {
        _cc_free_event(async, event);
        return false;
    }
    return true;
}

int main (int argc, char * const argv[]) {
    int c;
    _cc_alloc_async_event(0, nullptr);

    addConnectListener(_T("time-nw.nist.gov"), 13);

    while((c = getchar()) != 'q') {
        _cc_sleep(100);
    }
    _cc_free_async_event();
    return 0;
}
