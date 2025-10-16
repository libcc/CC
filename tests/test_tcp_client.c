#include <stdio.h>
#include <locale.h>
#include <libcc/event.h>

//A simple example program which connects to a server

static bool_t _do_event_handler(_cc_async_event_t *async, _cc_event_t *e, const uint32_t which) {
    if (which & _CC_EVENT_CONNECT_) {
        return true;
    } else if (which & _CC_EVENT_CLOSED_) {
        _cc_logger_info(_T("closed event %d"), e->ident);
        return false;
    } else if (which & _CC_EVENT_READABLE_) {

    }

    if (which & _CC_EVENT_WRITABLE_) {
        
    }

    if (which & _CC_EVENT_TIMEOUT_) {
        _cc_logger_info(_T("timeout event %d"), e->ident);
        return false;
    }

    return true;
}

bool_t _connect_server(const tchar_t *host, uint16_t port) {
    struct sockaddr_in sa;
    _cc_event_t *event;
    _cc_async_event_t *async = _cc_get_async_event();
    _cc_assert(async != NULL);

    event = _cc_event_alloc(async, _CC_EVENT_CONNECT_|_CC_EVENT_TIMEOUT_|_CC_EVENT_READABLE_);
    _cc_assert(event != NULL);
    if (event == nullptr) {
        return false;
    }

    event->timeout = 10000;
    event->callback = _do_event_handler;

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

    _connect_server(_T("time-nw.nist.gov"), 13);

    while((c = getchar()) != 'q') {
        _cc_sleep(100);
    }
    _cc_free_async_event();
    return 0;
}
