# hmac.h 文档

## 文件概述
`hmac.h` 是一个实现 HMAC（基于哈希的消息认证码）的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. HMAC 计算
提供了以下函数：
- `_cc_hmac_init` - 初始化 HMAC 上下文。
- `_cc_hmac_update` - 更新 HMAC 计算。
- `_cc_hmac_final` - 完成 HMAC 计算并输出结果。

### 2. 辅助函数
提供了 `_cc_hmac` 函数，用于快速计算数据的 HMAC 值。

## 使用示例

### 示例 1：计算 HMAC-SHA1
```c
#include <libcc/crypto/hmac.h>
#include <stdio.h>

int main() {
    uint8_t key[] = "secret";
    uint8_t data[] = "Hello, HMAC!";
    uint8_t hmac[20]; // SHA1 输出长度为 20 字节
    _cc_hmac(hmac, key, 6, data, 12, _CC_HMAC_SHA1_);
    printf("HMAC-SHA1: ");
    for (int i = 0; i < 20; i++) {
        printf("%02x", hmac[i]);
    }
    printf("\n");
    return 0;
}
```

## 注意事项
1. **密钥管理**：确保密钥安全存储。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。