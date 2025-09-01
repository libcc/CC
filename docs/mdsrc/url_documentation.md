# url.c 文档

## 文件概述
`url.c` 是一个用于解析和处理 URL 的库文件，提供了 URL 的解析、编码和解码功能。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. URL 解析
- **函数**: `_cc_parse_url`
  - 解析给定的 URL 字符串，提取其各个组成部分（如协议、主机、端口、路径等）。
  - 支持多种协议（HTTP、HTTPS、FTP 等）。

- **函数**: `_parser_url`
  - 内部函数，用于实际解析 URL 的逻辑。

### 2. URL 编码和解码
- **函数**: `_cc_url_encode`
  - 对字符串进行 URL 编码，将特殊字符转换为 `%` 后跟十六进制表示的格式。

- **函数**: `_cc_url_decode`
  - 对 URL 编码的字符串进行解码，还原为原始字符串。

### 3. URL 资源释放
- **函数**: `_cc_free_url`
  - 释放由 `_cc_parse_url` 解析出的 URL 结构体占用的内存。

## 使用示例

### 示例 1：解析 URL
```c
_cc_url_t url;
if (_cc_parse_url(&url, "https://example.com:8080/path?query=value#fragment")) {
    printf("Scheme: %s\n", url.scheme.value);
    printf("Host: %s\n", url.host);
    printf("Port: %d\n", url.port);
    printf("Path: %s\n", url.path);
    printf("Query: %s\n", url.query);
    printf("Fragment: %s\n", url.fragment);
    _cc_free_url(&url);
}
```

### 示例 2：URL 编码和解码
```c
tchar_t encoded[256];
tchar_t decoded[256];

// 编码
_cc_url_encode("Hello World!", 12, encoded, sizeof(encoded));
printf("Encoded: %s\n", encoded);

// 解码
_cc_url_decode(encoded, strlen(encoded), decoded, sizeof(decoded));
printf("Decoded: %s\n", decoded);
```

## 注意事项
1. **输入验证**：所有函数均对输入进行了严格的验证，确保不会处理无效的 URL 或字符串。
2. **内存管理**：调用者需负责释放由 `_cc_parse_url` 和 `_cc_url_encode` 分配的内存。
3. **性能**：函数设计为高效实现，适合频繁调用。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。