# sha1.c 文档

## 文件概述
`sha1.c` 是一个实现 SHA-1（Secure Hash Algorithm 1）哈希算法的库文件，提供对输入数据的哈希计算功能。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. SHA-1 算法实现
- **SHA-1 原理**：
  - 输入任意长度的数据，输出固定长度（160 位）的哈希值。
  - 适用于数据完整性校验，但安全性较低，不推荐用于安全敏感场景。

### 2. 核心函数
- **函数**: `_cc_sha1_init`
  - 初始化 SHA-1 上下文，设置初始哈希值。
  - 参数：
    - `ctx`: SHA-1 上下文结构体指针。

- **函数**: `_cc_sha1_process`
  - 处理 64 字节的数据块，更新哈希状态。
  - 参数：
    - `ctx`: SHA-1 上下文结构体指针。
    - `data`: 64 字节的数据块。

- **函数**: `_cc_sha1_update`
  - 更新哈希计算，输入任意长度的数据。
  - 参数：
    - `ctx`: SHA-1 上下文结构体指针。
    - `input`: 输入数据。
    - `ilen`: 输入数据长度。

- **函数**: `_cc_sha1_final`
  - 完成哈希计算，输出最终的哈希值。
  - 参数：
    - `ctx`: SHA-1 上下文结构体指针。
    - `output`: 输出缓冲区（20 字节）。

- **函数**: `_cc_sha1`
  - 封装 SHA-1 计算的完整流程（初始化、更新、完成）。
  - 参数：
    - `input`: 输入数据。
    - `ilen`: 输入数据长度。
    - `output`: 输出缓冲区（十六进制字符串）。

### 3. 文件哈希计算
- **函数**: `_cc_sha1_fp`
  - 计算文件的 SHA-1 哈希值。
  - 参数：
    - `fp`: 文件指针。
    - `output`: 输出缓冲区（十六进制字符串）。
  - 返回值：成功返回 `true`，失败返回 `false`。

- **函数**: `_cc_sha1file`
  - 计算指定文件的 SHA-1 哈希值。
  - 参数：
    - `filename`: 文件路径。
    - `output`: 输出缓冲区（十六进制字符串）。
  - 返回值：成功返回 `true`，失败返回 `false`。

## 使用示例

### 示例 1：计算字符串的 SHA-1 哈希值
```c
#include <libcc/crypto/sha.h>

int main() {
    const byte_t input[] = "Hello, SHA-1!";
    tchar_t output[41]; // SHA-1 输出为 40 字符（20 字节）

    _cc_sha1(input, strlen(input), output);
    printf("SHA-1: %s\n", output);

    return 0;
}
```

### 示例 2：计算文件的 SHA-1 哈希值
```c
#include <libcc/crypto/sha.h>

int main() {
    tchar_t output[41]; // SHA-1 输出为 40 字符（20 字节）

    if (_cc_sha1file("/path/to/file", output)) {
        printf("File SHA-1: %s\n", output);
    }

    return 0;
}
```

## 注意事项
1. **安全性**：SHA-1 算法已被证明存在安全漏洞，不适用于密码存储或数字签名等安全敏感场景。
2. **性能**：SHA-1 算法计算速度快，适合非安全敏感场景的哈希需求。
3. **内存管理**：调用者需确保输入数据的生命周期与哈希计算过程一致。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。