# aes.h 文档

## 文件概述
`aes.h` 是一个实现 AES（高级加密标准）算法的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. AES 加密/解密
提供了以下函数：
- `_cc_aes_set_key` - 设置 AES 密钥。
- `_cc_aes_encrypt` - AES 加密。
- `_cc_aes_decrypt` - AES 解密。

### 2. 加密模式
支持以下模式：
- ECB（电子密码本模式）
- CBC（密码块链接模式）

## 使用示例

### 示例 1：AES-256-CBC 加密
```c
#include <libcc/crypto/aes.h>
#include <stdio.h>

int main() {
    uint8_t key[32] = {0}; // 256-bit key
    uint8_t iv[16] = {0};  // Initialization vector
    uint8_t plaintext[16] = "Hello, AES!";
    uint8_t ciphertext[16];
    uint8_t decrypted[16];
    _cc_aes_ctx_t ctx;
    _cc_aes_set_key(&ctx, key, 256);
    _cc_aes_cbc_encrypt(&ctx, iv, plaintext, ciphertext, 16);
    _cc_aes_cbc_decrypt(&ctx, iv, ciphertext, decrypted, 16);
    printf("Decrypted: %s\n", decrypted);
    return 0;
}
```

## 注意事项
1. **密钥安全**：确保密钥和 IV 安全存储。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。