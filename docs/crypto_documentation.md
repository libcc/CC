# crypto.h 文档

## 文件概述
`crypto.h` 是一个汇总了 `libcc` 项目中所有加密相关功能的头文件。它包含了 AES、DES、Base16、Base58、Base64、MD2、MD4、MD5、HMAC 和 SHA 系列算法的头文件引用。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 头文件引用
`crypto.h` 引用了以下加密相关的头文件：
- `aes.h` - AES 加密算法。
- `des.h` - DES 加密算法。
- `base16.h` - Base16 编码/解码。
- `base58.h` - Base58 编码/解码。
- `base64.h` - Base64 编码/解码。
- `md2.h` - MD2 哈希算法。
- `md4.h` - MD4 哈希算法。
- `md5.h` - MD5 哈希算法。
- `hmac.h` - HMAC 消息认证码。
- `sha.h` - SHA 系列哈希算法。

## 使用示例

### 示例 1：使用 AES 加密
```c
#include <libcc/crypto/crypto.h>
#include <stdio.h>

int main() {
    _cc_aes_t ctx;
    byte_t key[16] = "1234567890123456";
    byte_t input[16] = "Hello, AES!";
    byte_t output[16];

    _cc_aes_init(&ctx);
    _cc_aes_setkey_enc(&ctx, key, 128);
    _cc_aes_encrypt(&ctx, input, output);

    printf("AES Encrypted: %s\n", output);
    return 0;
}
```

### 示例 2：使用 SHA-256 哈希
```c
#include <libcc/crypto/crypto.h>
#include <stdio.h>

int main() {
    const byte_t data[] = "Hello, SHA-256!";
    tchar_t hash[65]; // 64 字符 + 空终止符
    _cc_sha256(data, sizeof(data) - 1, hash, false);
    printf("SHA-256 Hash: %s\n", hash);
    return 0;
}
```

## 注意事项
1. **功能依赖**：`crypto.h` 仅提供头文件引用，具体功能实现需要包含对应的头文件。
2. **线程安全**：各算法的线程安全性请参考对应的头文件文档。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。