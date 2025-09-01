# hmac.c 文档

## 文件概述
`hmac.c` 是一个实现 HMAC（Hash-based Message Authentication Code）算法的库文件，支持多种哈希算法（如 MD5、SHA1、SHA224、SHA256、SHA384、SHA512）。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. HMAC 算法实现
- **HMAC 原理**：
  - 使用密钥和哈希函数生成消息认证码。
  - 支持多种哈希算法，通过 `type` 参数指定。

### 2. 核心函数
- **函数**: `_cc_hmac_alloc`
  - 分配并初始化 HMAC 上下文，根据指定的哈希算法类型（如 `_CC_HMAC_MD5_`、`_CC_HMAC_SHA256_` 等）。
  - 返回 `_cc_hmac_t` 结构体指针。

- **函数**: `_cc_hmac_init`
  - 初始化 HMAC 上下文，处理密钥（如果密钥长度超过块长度，则先哈希密钥）。
  - 参数：
    - `key`: 密钥数据。
    - `length`: 密钥长度。

- **函数**: `_cc_hmac_update`
  - 更新 HMAC 计算，输入消息数据。
  - 参数：
    - `input`: 消息数据。
    - `length`: 消息长度。

- **函数**: `_cc_hmac_final`
  - 完成 HMAC 计算，输出认证码。
  - 参数：
    - `output`: 输出缓冲区。
    - `length`: 输出缓冲区长度。
  - 返回值：实际输出的认证码长度。

- **函数**: `_cc_hmac`
  - 封装 HMAC 计算的完整流程（分配、初始化、更新、完成、释放）。
  - 参数：
    - `type`: 哈希算法类型。
    - `input`: 消息数据。
    - `ilen`: 消息长度。
    - `key`: 密钥数据。
    - `key_length`: 密钥长度。
    - `output`: 输出缓冲区（十六进制字符串）。
  - 返回值：实际输出的认证码长度。

### 3. 辅助函数
- **函数**: `_hmac_init_block`
  - 初始化 HMAC 的块数据（填充密钥和常量）。

## 使用示例

### 示例 1：计算 HMAC-SHA256
```c
#include <libcc/crypto/hmac.h>

int main() {
    const byte_t key[] = "secret_key";
    const byte_t message[] = "Hello, HMAC!";
    tchar_t output[65]; // SHA256 输出为 64 字符（32 字节）

    int result = _cc_hmac(_CC_HMAC_SHA256_, message, strlen(message), key, strlen(key), output);
    if (result > 0) {
        printf("HMAC: %s\n", output);
    }

    return 0;
}
```

### 示例 2：分步计算 HMAC
```c
#include <libcc/crypto/hmac.h>

int main() {
    const byte_t key[] = "secret_key";
    const byte_t message[] = "Hello, HMAC!";
    byte_t digest[32]; // SHA256 输出为 32 字节

    _cc_hmac_t *ctx = _cc_hmac_alloc(_CC_HMAC_SHA256_);
    if (ctx) {
        _cc_hmac_init(ctx, key, strlen(key));
        _cc_hmac_update(ctx, message, strlen(message));
        int result = _cc_hmac_final(ctx, digest, sizeof(digest));
        if (result > 0) {
            printf("HMAC calculated successfully.\n");
        }
        _cc_hmac_free(ctx);
    }

    return 0;
}
```

## 注意事项
1. **密钥安全**：密钥应保密，避免硬编码或明文存储。
2. **哈希算法选择**：根据安全需求选择合适的哈希算法（如 SHA256 或 SHA512）。
3. **内存管理**：使用 `_cc_hmac_alloc` 和 `_cc_hmac_free` 管理 HMAC 上下文的内存。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。