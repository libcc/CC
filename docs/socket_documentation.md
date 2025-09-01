# socket.h 文档

## 文件概述
`socket.h` 是一个实现跨平台套接字编程的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 套接字操作
提供了以下函数：
- `_cc_socket` - 创建套接字。
- `_cc_close_socket` - 关闭套接字。
- `_cc_set_socket_nonblock` - 设置非阻塞模式。
- `_cc_set_socket_nodelay` - 设置 TCP_NODELAY。

### 2. 网络地址转换
提供了以下函数：
- `_cc_inet_pton` - 字符串转网络地址。
- `_cc_inet_ntop` - 网络地址转字符串。

## 使用示例

### 示例 1：创建套接字
```c
#include <libcc/socket/socket.h>
#include <stdio.h>

int main() {
    _cc_socket_t sock = _cc_socket(AF_INET, SOCK_STREAM, 0);
    if (sock != _CC_INVALID_SOCKET_) {
        printf("Socket created\n");
        _cc_close_socket(sock);
    }
    return 0;
}
```

## 注意事项
1. **跨平台兼容性**：支持 Windows 和 Unix 平台。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。