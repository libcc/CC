# buf.h 文档

## 文件概述
`buf.h` 是一个实现缓冲区操作的头文件，支持动态缓冲区管理和数据读写。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 缓冲区结构
定义了 `_cc_buf_t` 结构体，用于存储缓冲区数据：
- `limit` - 缓冲区容量。
- `length` - 当前数据长度。
- `bytes` - 数据指针。

### 2. 缓冲区操作
提供了以下函数：
- `_cc_alloc_buf` - 初始化缓冲区。
- `_cc_free_buf` - 释放缓冲区。
- `_cc_buf_expand` - 扩展缓冲区。
- `_cc_buf_append` - 追加数据。
- `_cc_buf_stringify` - 转换为字符串。

## 使用示例

### 示例 1：使用缓冲区
```c
#include <libcc/buf.h>
#include <stdio.h>

int main() {
    _cc_buf_t buf;
    if (_cc_alloc_buf(&buf, 1024)) {
        _cc_buf_append(&buf, "Hello", 5);
        printf("Buffer: %.*s\n", (int)buf.length, buf.bytes);
        _cc_free_buf(&buf);
    }
    return 0;
}
```

## 注意事项
1. **线程安全**：需要外部同步机制保证线程安全。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。