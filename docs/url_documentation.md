# url.h 文档

## 文件概述
`url.h` 是一个实现 URL 解析和编码的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. URL 结构
定义了 `_cc_url_t` 结构体，用于存储 URL 各部分：
- `scheme` - 协议。
- `host` - 主机。
- `port` - 端口。
- `path` - 路径。
- `query` - 查询参数。
- `fragment` - 片段。

### 2. URL 操作
提供了以下函数：
- `_cc_alloc_url` - 解析 URL。
- `_cc_free_url` - 释放 URL。
- `_cc_url_encode` - URL 编码。
- `_cc_url_decode` - URL 解码。

## 使用示例

### 示例 1：解析 URL
```c
#include <libcc/url.h>
#include <stdio.h>

int main() {
    _cc_url_t url;
    if (_cc_alloc_url(&url, "http://example.com/path?query=1#frag")) {
        printf("Host: %s, Port: %u\n", url.host, url.port);
        _cc_free_url(&url);
    }
    return 0;
}
```

## 注意事项
1. **线程安全**：函数是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。