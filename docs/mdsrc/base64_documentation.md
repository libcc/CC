# base64.c 文档

## 文件概述
`base64.c` 是一个实现 Base64 编码和解码功能的库文件，提供对二进制数据的编码和解码支持。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. Base64 算法实现
- **Base64 原理**：
  - 将二进制数据转换为可打印的 ASCII 字符，便于传输和存储。
  - 使用 64 个字符（A-Z, a-z, 0-9, +, /）和填充字符 `=`。

### 2. 核心函数
- **函数**: `_cc_base64_encode`
  - 对输入数据进行 Base64 编码。
  - 参数：
    - `input`: 输入数据（二进制）。
    - `length`: 输入数据长度。
    - `output`: 输出缓冲区（编码后的字符串）。
    - `output_length`: 输出缓冲区长度。
  - 返回值：编码后的字符串长度。

- **函数**: `_cc_base64_decode`
  - 对输入数据进行 Base64 解码。
  - 参数：
    - `input`: 输入数据（Base64 字符串）。
    - `length`: 输入数据长度。
    - `output`: 输出缓冲区（解码后的二进制数据）。
    - `output_length`: 输出缓冲区长度。
  - 返回值：解码后的二进制数据长度。

### 3. 辅助数据结构
- **常量**: `base64_table`
  - Base64 编码使用的字符表。

- **常量**: `base64_reverse_table`
  - Base64 解码使用的反向字符表。

## 使用示例

### 示例 1：Base64 编码
```c
#include <libcc/crypto/base64.h>

int main() {
    const byte_t input[] = "Hello, Base64!";
    tchar_t output[64]; // 足够大的缓冲区
    size_t encoded_len = _cc_base64_encode(input, strlen(input), output, sizeof(output));

    if (encoded_len > 0) {
        printf("Base64 Encoded: %s\n", output);
    }

    return 0;
}
```

### 示例 2：Base64 解码
```c
#include <libcc/crypto/base64.h>

int main() {
    const tchar_t input[] = "SGVsbG8sIEJhc2U2NCE=";
    byte_t output[64]; // 足够大的缓冲区
    size_t decoded_len = _cc_base64_decode(input, strlen(input), output, sizeof(output));

    if (decoded_len > 0) {
        printf("Base64 Decoded: %.*s\n", (int)decoded_len, output);
    }

    return 0;
}
```

## 注意事项
1. **缓冲区大小**：调用者需确保输出缓冲区足够大，以容纳编码或解码后的数据。
2. **输入验证**：解码时需验证输入是否为有效的 Base64 字符串。
3. **性能**：Base64 编码和解码操作均为线性时间复杂度，适合高频使用。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。