# base64.h 文档

## 文件概述
`base64.h` 是一个实现 Base64 编码和解码的头文件，用于数据的 Base64 转换。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. Base64 编码
- **函数**: `_cc_base64_encode`
  - 将输入缓冲区编码为 Base64 格式。
  - 参数：
    - `input` - 输入数据缓冲区。
    - `length` - 输入数据的长度。
    - `output` - 存储编码结果的缓冲区。
    - `output_length` - 输出缓冲区的长度。
  - 返回值：编码后的数据长度。

### 2. Base64 解码
- **函数**: `_cc_base64_decode`
  - 将 Base64 格式的数据解码为原始数据。
  - 参数：
    - `input` - 输入 Base64 数据缓冲区。
    - `length` - 输入数据的长度。
    - `output` - 存储解码结果的缓冲区。
    - `output_length` - 输出缓冲区的长度。
  - 返回值：解码后的数据长度。

## 使用示例

### 示例 1：Base64 编码
```c
#include <libcc/crypto/base64.h>
#include <stdio.h>

int main() {
    const byte_t data[] = "Hello, Base64!";
    tchar_t encoded[100];
    size_t encoded_len = _cc_base64_encode(data, sizeof(data) - 1, encoded, sizeof(encoded));
    printf("Base64 Encoded: %s\n", encoded);
    return 0;
}
```

### 示例 2：Base64 解码
```c
#include <libcc/crypto/base64.h>
#include <stdio.h>

int main() {
    const tchar_t encoded[] = "SGVsbG8sIEJhc2U2NCE=";
    byte_t decoded[100];
    size_t decoded_len = _cc_base64_decode(encoded, sizeof(encoded) - 1, decoded, sizeof(decoded));
    printf("Base64 Decoded: %s\n", decoded);
    return 0;
}
```

## 注意事项
1. **缓冲区大小**：确保输出缓冲区足够大以容纳编码或解码后的数据。
2. **线程安全**：函数 `_cc_base64_encode` 和 `_cc_base64_decode` 是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。