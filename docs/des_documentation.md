# des.h 文档

## 文件概述
`des.h` 是一个实现 DES（数据加密标准）和 3DES 算法的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. DES 加密/解密
提供了以下函数：
- `_cc_des_set_key` - 设置 DES 密钥。
- `_cc_des_encrypt` - DES 加密。
- `_cc_des_decrypt` - DES 解密。

### 2. 3DES 支持
支持三重 DES 加密（3DES）。

## 使用示例

### 示例 1：DES 加密
```c
#include <libcc/crypto/des.h>
#include <stdio.h>

int main() {
    uint8_t key[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    uint8_t plaintext[8] = "HelloDES";
    uint8_t ciphertext[8];
    uint8_t decrypted[8];
    _cc_des_ctx_t ctx;
    _cc_des_set_key(&ctx, key);
    _cc_des_encrypt(&ctx, plaintext, ciphertext);
    _cc_des_decrypt(&ctx, ciphertext, decrypted);
    printf("Decrypted: %s\n", decrypted);
    return 0;
}
```

## 注意事项
1. **安全性**：DES 已被证明不安全，建议使用 AES。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。