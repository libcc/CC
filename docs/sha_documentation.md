# sha.h 文档

## 文件概述
`sha.h` 是一个实现 SHA 系列哈希算法（包括 SHA-1、SHA-224、SHA-256、SHA-384、SHA-512 和 SHA-3）的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. SHA-1 哈希
- **上下文结构**: `_cc_sha1_t`
- **函数**:
  - `_cc_sha1_init` - 初始化 SHA-1 上下文。
  - `_cc_sha1_update` - 更新 SHA-1 计算。
  - `_cc_sha1_final` - 完成 SHA-1 计算并生成摘要。
  - `_cc_sha1` - 计算输入缓冲区的 SHA-1 哈希。
  - `_cc_sha1_fp` - 计算文件的 SHA-1 哈希。
  - `_cc_sha1file` - 通过文件名计算文件的 SHA-1 哈希。

### 2. SHA-256 哈希
- **上下文结构**: `_cc_sha256_t`
- **函数**:
  - `_cc_sha256_init` - 初始化 SHA-256 上下文。
  - `_cc_sha256_update` - 更新 SHA-256 计算。
  - `_cc_sha256_final` - 完成 SHA-256 计算并生成摘要。
  - `_cc_sha256` - 计算输入缓冲区的 SHA-256 哈希。
  - `_cc_sha256_fp` - 计算文件的 SHA-256 哈希。
  - `_cc_sha256file` - 通过文件名计算文件的 SHA-256 哈希。

### 3. SHA-512 哈希
- **上下文结构**: `_cc_sha512_t`
- **函数**:
  - `_cc_sha512_init` - 初始化 SHA-512 上下文。
  - `_cc_sha512_update` - 更新 SHA-512 计算。
  - `_cc_sha512_final` - 完成 SHA-512 计算并生成摘要。
  - `_cc_sha512` - 计算输入缓冲区的 SHA-512 哈希。
  - `_cc_sha512_fp` - 计算文件的 SHA-512 哈希。
  - `_cc_sha512file` - 通过文件名计算文件的 SHA-512 哈希。

### 4. SHA-3 哈希
- **上下文结构**: `_cc_sha3_t`
- **函数**:
  - `_cc_sha3_init` - 初始化 SHA-3 上下文。
  - `_cc_sha3_update` - 更新 SHA-3 计算。
  - `_cc_sha3_final` - 完成 SHA-3 计算并生成摘要。

## 使用示例

### 示例 1：计算字符串的 SHA-256 哈希
```c
#include <libcc/crypto/sha.h>
#include <stdio.h>

int main() {
    const byte_t data[] = "Hello, SHA-256!";
    tchar_t hash[65]; // 64 字符 + 空终止符
    _cc_sha256(data, sizeof(data) - 1, hash, false);
    printf("SHA-256 Hash: %s\n", hash);
    return 0;
}
```

### 示例 2：计算文件的 SHA-512 哈希
```c
#include <libcc/crypto/sha.h>
#include <stdio.h>

int main() {
    tchar_t hash[129]; // 128 字符 + 空终止符
    if (_cc_sha512file(_T("example.txt"), hash, false)) {
        printf("File SHA-512 Hash: %s\n", hash);
    } else {
        printf("Failed to compute hash.\n");
    }
    return 0;
}
```

## 注意事项
1. **安全性**：SHA-1 已被证明存在安全漏洞，不建议用于安全敏感的场景。
2. **缓冲区大小**：确保输出缓冲区足够大以容纳哈希结果。
3. **线程安全**：所有哈希函数都是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。