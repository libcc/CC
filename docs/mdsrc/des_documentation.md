# des.c 文档

## 文件概述
`des.c` 是一个实现 DES（Data Encryption Standard）加密算法的库文件，提供对称加密功能。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. DES 算法实现
- **DES 原理**：
  - 使用 56 位密钥对 64 位数据进行加密或解密。
  - 包含初始置换、16 轮 Feistel 网络、最终置换等步骤。

### 2. 核心函数
- **函数**: `_cc_des_init`
  - 初始化 DES 上下文。
  - 参数：
    - `ctx`: DES 上下文结构体指针。

- **函数**: `_cc_des_key_set_parity`
  - 设置密钥的奇偶校验位。
  - 参数：
    - `key`: 8 字节的 DES 密钥。

- **函数**: `_cc_des_key_check_key_parity`
  - 检查密钥的奇偶校验位是否正确。
  - 参数：
    - `key`: 8 字节的 DES 密钥。
  - 返回值：校验通过返回 `true`，否则返回 `false`。

- **函数**: `_cc_des_key_check_weak`
  - 检查是否为弱密钥。
  - 参数：
    - `key`: 8 字节的 DES 密钥。
  - 返回值：是弱密钥返回 `true`，否则返回 `false`。

- **函数**: `_cc_des_setkey`
  - 生成 DES 的子密钥。
  - 参数：
    - `SK`: 子密钥数组（32 个 32 位整数）。
    - `key`: 8 字节的 DES 密钥。

- **函数**: `_cc_des_setkey_enc`
  - 设置加密密钥。
  - 参数：
    - `ctx`: DES 上下文结构体指针。
    - `key`: 8 字节的 DES 密钥。

### 3. 三重 DES (3DES) 支持
- **函数**: `_cc_des3_init`
  - 初始化 3DES 上下文。
  - 参数：
    - `ctx`: 3DES 上下文结构体指针。

## 使用示例

### 示例 1：DES 加密
```c
#include <libcc/crypto/des.h>

int main() {
    _cc_des_t ctx;
    byte_t key[_CC_DES_KEY_SIZE_] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    byte_t input[8] = "HelloDES";
    byte_t output[8];

    _cc_des_init(&ctx);
    _cc_des_setkey_enc(&ctx, key);
    // 假设有加密函数 _cc_des_crypt
    // _cc_des_crypt(&ctx, input, output);

    return 0;
}
```

### 示例 2：检查弱密钥
```c
#include <libcc/crypto/des.h>

int main() {
    byte_t key[_CC_DES_KEY_SIZE_] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};

    if (_cc_des_key_check_weak(key)) {
        printf("Warning: Weak key detected!\n");
    }

    return 0;
}
```

## 注意事项
1. **安全性**：DES 算法密钥长度较短，已不再适用于高安全性场景，建议使用 AES 或其他现代加密算法。
2. **密钥管理**：确保密钥的奇偶校验位正确，避免使用弱密钥。
3. **性能**：DES 算法计算速度快，适合低安全性需求的场景。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。