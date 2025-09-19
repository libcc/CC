****
# libcc

![platform](https://img.shields.io/badge/platform-linux%20%7C%20windows%20%7C%20macos-blue)
<br>

`libcc`是跨平台，多线程，轻量级的C语言库，提供了更简单的接口和更丰富的协议。提供的函数功能包括：字符串、日志、双向/单向链表、哈希表、网络通信、JSON、XML、INI配置文件读取、AES、DES、MD2、MD4、MD5、base16/base58/base64编码/解码、url编码/解码、时间轮计时器等等。详细信息请参阅C文件
```C
//跨平台，超轻量，易扩展，框架代码如下：
#define CALL(fn, ...) fn(__VA_ARGS__)

//使用示例：输出hello World!
CALL(printf, "Hello World!\n");
/*
 * 开一个玩笑:)！
 * 
 * 只是出于对C的热爱，累积一份属于自己的C代码
 */
```

## ✨ 特征

- 跨平台（Linux, Windows, MacOS, FreeBSD, Android, iOS）
- 高性能事件循环（网络IO事件、定时器事件）
- TCP/UDP服务端/客户端/代理
- TCP支持心跳、转发、拆包、多线程安全write和close等特性

## ⌛️ 安装教程

见[REANDME.build.md](REANDME.build.md)

libcc提供了以下构建方式:

```shell
#1、通过Makefile:编译 (Linux,freeBSD,macOS)
cd yourdirname/libcc/build
make path

./build.sh debug
# Windows 下MSYS2环境 执行
./build.cmd debug

#2、通过Visual Studio编译 (Windows)
proj.Win/libcc.vcxproj

#3、通过Android JNI编译
#打开 .bash_profile 配置 $NDK_ROOT = (Android NDK目录)
cd proj.Android/JNI
./build_NDK.sh

#4、通过Xcode 编译(macOS,iOS)
proj.OSX/cc.xcodeproj
proj.IOS/cc.xcodeproj

#5、ubuntu 编译
sudo apt-get install libmysqlclient-dev
sudo apt-get install libsqlite3-dev 
sudo apt-get install libjpeg-dev
sudo apt-get install libpng-dev

#6、FreeBSD 安装

```

## ⚡️ 入门与体验

```shell
# 下载编译
git clone https://github.com/huai2001/CC.git
cd CC/build
make path
./build.sh
```
#### Support
Email: [libcc.cn@gmail.com](mailto:libcc.cn@gmail.com)

## ✨ TCP Server
```C
static int times = 0;
static int c = 0;
static uint16_t port = 3000;

void test_accept(_cc_async_event_t *async, _cc_event_t *e) {
    _cc_socket_t fd;
    _cc_event_t *event;
    struct sockaddr_in remote_addr = {0};
    _cc_socklen_t remote_addr_len = sizeof(struct sockaddr_in);
    _cc_async_event_t *async2 = _cc_get_async_event();

    fd = _cc_event_accept(async, e, (_cc_sockaddr_t *)&remote_addr, &remote_addr_len);
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

static bool_t test_callback(_cc_async_event_t *async, _cc_event_t *e, const uint32_t which) {
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
        _cc_event_rbuf_t *rbuf = &e->buffer->r;
        if (!_cc_event_recv(e)) {
            return false;
        }
        if (_strnicmp((char_t*)rbuf->bytes, "ping", 5) == 0){
            if (_cc_send(e->fd, (byte_t*)"pong", 5) < 0) {
                _cc_logger_debug(_T("%d send pong fail."), e->ident);
                return false;
            }
        } else if (_strnicmp((char_t*)rbuf->bytes, "close", 5) == 0){
            _cc_logger_debug(_T("%d client close."), e->ident);
            return false;
        }
        rbuf->bytes[rbuf->length] = 0;
        _cc_logger_debug("%d: %.*s",e->ident, rbuf->length, rbuf->bytes);
        rbuf->length = 0;
    }

    if (which & _CC_EVENT_WRITABLE_) {
        _cc_logger_debug(_T("%d writeable."), e->ident);
        return _cc_event_sendbuf(e) < 0;
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

void _test_listen() {
    struct sockaddr_in sa;
    _cc_event_t *event;
    _cc_async_event_t *async = _cc_get_async_event();

    event = _cc_event_alloc(async, _CC_EVENT_ACCEPT_);
    assert(event != NULL);
    if (event == nullptr) {
        return;
    }

    event->timeout = 60000;
    event->callback = test_callback;

    _cc_inet_ipv4_addr(&sa, nullptr, port);
    if (!_cc_tcp_listen(async, event, (_cc_sockaddr_t *)&sa, sizeof(struct sockaddr_in))) {
        _cc_free_event(async, event);    
        assert(false);
    }
}

int main() {
    int i;
    _cc_alloc_async_event(0, nullptr);
    _test_listen();
    while((c = getchar()) != 'q') {
        _cc_sleep(100);
    }
    _cc_free_async_event();
    return 0;
}
```
## ✨ TCP Client
```C
static int times = 0;
static int c = 0;
static uint16_t port = 3000;

static bool_t test_callback(_cc_async_event_t *async, _cc_event_t *e, const uint32_t which) {
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
        _cc_event_rbuf_t *rbuf = &e->buffer->r;
        if (!_cc_event_recv(e)) {
            return false;
        }
        if (_strnicmp((char_t*)rbuf->bytes, "ping", 5) == 0){
            if (_cc_send(e->fd, (byte_t*)"pong", 5) < 0) {
                _cc_logger_debug(_T("%d send pong fail."), e->ident);
                return false;
            }
        } else if (_strnicmp((char_t*)rbuf->bytes, "close", 5) == 0){
            _cc_logger_debug(_T("%d client close."), e->ident);
            return false;
        }
        rbuf->bytes[rbuf->length] = 0;
        _cc_logger_debug("%d: %.*s",e->ident, rbuf->length, rbuf->bytes);
        rbuf->length = 0;
    }

    if (which & _CC_EVENT_WRITABLE_) {
        _cc_logger_debug(_T("%d writeable."), e->ident);
        return _cc_event_sendbuf(e) < 0;
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

void test_tcp_connect() {
    struct sockaddr_in sa;
    _cc_event_t *event;
    _cc_async_event_t *async = _cc_get_async_event();
    assert(async != NULL);

    event = _cc_event_alloc(async, _CC_EVENT_CONNECT_|_CC_EVENT_TIMEOUT_|_CC_EVENT_BUFFER_);
    assert(event != NULL);
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
    int i;
    _cc_alloc_async_event(0, nullptr);

    test_tcp_connect();
    while((c = getchar()) != 'q') {
        _cc_sleep(100);
    }

    _cc_free_async_event();
    return 0;
}
```
功能函数
------
