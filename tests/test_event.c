#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <libcc/widgets/event.h>
#include <libcc/widgets/timeout.h>

static uint16_t port = 3000;
// 自定义测试宏
#define TEST_CASE(name) printf("Running test: %s\n", #name); name();
#define ASSERT(cond) assert(cond)

void test_accept(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_socket_t fd;
    _cc_event_t *event;
    struct sockaddr_in remote_addr = {0};
    _cc_socklen_t remote_addr_len = sizeof(struct sockaddr_in);
    _cc_async_event_t *async2 = _cc_get_async_event();

    fd = _cc_event_accept(async, e, &remote_addr, &remote_addr_len);
    if (fd == _CC_INVALID_SOCKET_) {
        _cc_logger_debug(_T("thread %d accept fail %s."), _cc_get_thread_id(nullptr),
                         _cc_last_error(_cc_last_errno()));
        return ;
    }

    event = _cc_event_alloc(async2, _CC_EVENT_TIMEOUT_ | _CC_EVENT_READABLE_ | _CC_EVENT_BUFFER_);
    if (event == nullptr) {
        _cc_close_socket(fd);
        return ;
    }

    _cc_set_socket_nonblock(fd, 1);

    event->fd = fd;
    event->callback = e->callback;
    event->timeout = e->timeout;

    if (async2->attach(async2, event) == false) {
        _cc_logger_debug(_T("thread %d add socket (%d) event fial."), _cc_get_thread_id(nullptr), fd);
        _cc_free_event(async2, event);
        return ;
    }
    _cc_logger_debug(_T("%d accept."), event->ident);
}
static int times = 0;
static bool_t test_event_callback(_cc_async_event_t *async, _cc_event_t *e, const uint16_t which) {
    if (which & _CC_EVENT_ACCEPT_) {
        test_accept(async,e);

        return true;
    } else if (which & _CC_EVENT_CONNECTED_) {
        _cc_logger_debug(_T("%d connected."), e->ident);
        return true;
    }

	if (which & _CC_EVENT_DISCONNECT_) {
        _cc_logger_debug(_T("%d disconnect."), e->ident);
        return false;
    }

    if (which & _CC_EVENT_READABLE_) {
        byte_t buf[1024 * 16];
        int32_t length;

        length = _cc_recv(e->fd, buf, _cc_countof(buf));
        if (length <= 0) {
            _cc_logger_debug(_T("%d close."), e->ident);
            return false;
        }
        if (_strnicmp((char_t*)buf, "ping", 5) == 0){
            if (_cc_send(e->fd, (byte_t*)"pong", 5) < 0) {
                _cc_logger_debug(_T("%d send pong fail."), e->ident);
                return false;
            }
        } else if (_strnicmp((char_t*)buf, "close", 5) == 0){
            _cc_logger_debug(_T("%d client close."), e->ident);
            return false;
        }
        buf[length] = 0;
        _cc_logger_debug("%d: %.*s",e->ident, length, buf);
    }

    if (which & _CC_EVENT_WRITABLE_) {
        _cc_logger_debug(_T("%d writeable."), e->ident);
        _CC_UNSET_BIT(_CC_EVENT_WRITABLE_, e->flags);
    }

    if (which & _CC_EVENT_TIMEOUT_) {
        _cc_logger_debug(_T("%d timeout."), e->ident);
        if (times++ > 10) {
            if (_cc_send(e->fd, (byte_t*)"close", 5) < 0) {
                _cc_logger_debug(_T("%d send close fail."), e->ident);
                return false;
            }
        } else if (_cc_send(e->fd, (byte_t*)"ping", 5) < 0) {
            _cc_logger_debug(_T("%d send ping fail."), e->ident);
            return false;
        }
    }
    return true;
}
static bool_t test_event_timeout_callback(_cc_async_event_t *async, _cc_event_t *e, const uint16_t which) {
    if (which & _CC_EVENT_TIMEOUT_) {
        _cc_logger_debug(_T("%d timer timeout."), e->ident);
        return false;
    }

    if (which & _CC_EVENT_DISCONNECT_) {
        _cc_logger_debug(_T("%d destroy timeout."), e->ident);
    }       
    return false;
}

// 测试缓冲区分配
void test_buffer_allocation() {
    _cc_event_buffer_t *buffer = _cc_alloc_event_buffer();
    ASSERT(buffer != NULL);
    _cc_free_event_buffer(buffer);
}

// 测试读缓冲区分配
void test_read_buffer_allocation() {
    _cc_event_rbuf_t rbuf = {0};
    _cc_alloc_event_rbuf(&rbuf, 1024);
    ASSERT(rbuf.limit == 1024);
    ASSERT(rbuf.bytes != NULL);
}

// 测试写缓冲区分配
void test_write_buffer_allocation() {
    _cc_event_wbuf_t wbuf = {0};
    _cc_alloc_event_wbuf(&wbuf, 1024);
    ASSERT(wbuf.limit == 1024);
    ASSERT(wbuf.bytes != NULL);
}

// 测试事件超时
void test_event_timeout() {
    _cc_event_t *event;
    _cc_async_event_t *async = _cc_get_async_event();
    ASSERT(async != NULL);

    event = _cc_add_event_timeout(async, 6000, test_event_timeout_callback, nullptr);
    ASSERT(event != NULL);
}

void test_event_tcp_listen() {
    struct sockaddr_in sa;
    _cc_event_t *event;
    _cc_async_event_t *async = _cc_get_async_event();

    event = _cc_event_alloc(async, _CC_EVENT_ACCEPT_);
    ASSERT(event != NULL);
    if (event == nullptr) {
        return;
    }

    event->timeout = 10000;
    event->callback = test_event_callback;

    _cc_inet_ipv4_addr(&sa, nullptr, port);
    if (!_cc_tcp_listen(async, event, (_cc_sockaddr_t *)&sa, sizeof(struct sockaddr_in))) {
        _cc_free_event(async, event);    
        ASSERT(FALSE);
        return ;
    }
}

void test_event_tcp_connect() {
    struct sockaddr_in sa;
    _cc_event_t *event;
    _cc_async_event_t *async = _cc_get_async_event();
    ASSERT(async != NULL);

    event = _cc_event_alloc(async, _CC_EVENT_CONNECT_|_CC_EVENT_TIMEOUT_);
    ASSERT(event != NULL);
    if (event == nullptr) {
        return;
    }

    event->timeout = 6000;
    event->callback = test_event_callback;

    _cc_inet_ipv4_addr(&sa, "127.0.0.1", port);
    if (!_cc_tcp_connect(async, event, (_cc_sockaddr_t *)&sa, sizeof(struct sockaddr_in))) {
        _cc_free_event(async, event);
        return;
    }
}
int main() {
    int c;
    _cc_alloc_async_event(0, nullptr);

    TEST_CASE(test_buffer_allocation);
    TEST_CASE(test_read_buffer_allocation);
    TEST_CASE(test_write_buffer_allocation);
    TEST_CASE(test_event_tcp_listen);
    TEST_CASE(test_event_tcp_connect);
    TEST_CASE(test_event_timeout);

    while((c = getchar()) != 'q') {
        _cc_sleep(100);
    }

    printf("All tests passed!\n");
    _cc_free_async_event();
    return 0;
}