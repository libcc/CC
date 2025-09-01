# xxtea.c 文档

## 文件概述
`xxtea.c` 是一个实现 XXTEA（Corrected Block TEA）加密算法的库文件，提供对称加密和解密功能。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. XXTEA 算法实现
- **XXTEA 原理**：
  - 一种分组加密算法，适用于小型数据块的加密和解密。
  - 使用 128 位密钥，支持任意长度的数据块。

### 2. 核心函数
- **函数**: `_cc_xxtea_encrypt`
  - 对输入数据进行 XXTEA 加密。
  - 参数：
    - `data`: 输入数据。
    - `len`: 输入数据长度。
    - `key`: 128 位密钥。
    - `output_length`: 输出数据长度（通过指针返回）。
  - 返回值：加密后的数据（需手动释放内存）。

- **函数**: `_cc_xxtea_decrypt`
  - 对输入数据进行 XXTEA 解密。
  - 参数：
    - `data`: 输入数据。
    - `len`: 输入数据长度。
    - `key`: 128 位密钥。
    - `output_length`: 输出数据长度（通过指针返回）。
  - 返回值：解密后的数据（需手动释放内存）。

## 使用示例

### 示例 1：加密数据
```c
#include <libcc/crypto/xxtea.h>

int main() {
    const byte_t data[] = "Hello, XXTEA!";
    const byte_t key[] = "1234567890123456"; // 128-bit key
    size_t output_len;
    byte_t *encrypted = _cc_xxtea_encrypt(data, sizeof(data) - 1, key, &output_len);

    if (encrypted) {
        printf("Encrypted data length: %zu\n", output_len);
        _cc_free(encrypted);
    }

    return 0;
}
```

### 示例 2：解密数据
```c
#include <libcc/crypto/xxtea.h>

int main() {
    const byte_t encrypted_data[] = { /* Encrypted bytes */ };
    const byte_t key[] = "1234567890123456"; // 128-bit key
    size_t output_len;
    byte_t *decrypted = _cc_xxtea_decrypt(encrypted_data, sizeof(encrypted_data), key, &output_len);

    if (decrypted) {
        printf("Decrypted data: %s\n", decrypted);
        _cc_free(decrypted);
    }

    return 0;
}
```

## 注意事项
1. **密钥管理**：确保密钥长度为 128 位（16 字节），否则会补零或截断。
2. **内存管理**：加密和解密函数返回的数据需手动调用 `_cc_free` 释放内存。
3. **安全性**：XXTEA 算法适用于小型数据块，但不适用于高安全性场景。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。