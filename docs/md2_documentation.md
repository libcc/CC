# md2.h 文档

## 文件概述
`md2.h` 是一个实现 MD2 哈希算法的头文件，用于生成数据的 MD2 校验和。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. MD2 上下文结构
定义了 `_cc_md2_t` 结构体，用于存储 MD2 计算的中间状态和数据块：
- `cksum[16]`：数据块的校验和。
- `state[48]`：中间摘要状态。
- `buffer[16]`：正在处理的数据块。
- `left`：缓冲区中的数据量。

### 2. MD2 哈希计算函数
- **初始化上下文**：`_cc_md2_init`
  - 初始化 MD2 上下文结构。
  - 参数：`ctx` - 待初始化的上下文。

- **更新数据块**：`_cc_md2_update`
  - 处理输入数据块并更新上下文状态。
  - 参数：
    - `ctx` - MD2 上下文。
    - `input` - 输入数据缓冲区。
    - `ilen` - 输入数据的长度。

- **生成最终摘要**：`_cc_md2_final`
  - 完成 MD2 计算并生成最终的校验和。
  - 参数：
    - `ctx` - MD2 上下文。
    - `output` - 存储校验和的缓冲区（16 字节）。

### 3. 文件哈希计算
- **文件指针哈希**：`_cc_md2_fp`
  - 计算文件的 MD2 校验和。
  - 参数：
    - `fp` - 文件指针。
    - `output` - 存储校验和的缓冲区。
  - 返回值：成功返回 `true`，失败返回 `false`。

- **文件名哈希**：`_cc_md2file`
  - 通过文件名计算文件的 MD2 校验和。
  - 参数：
    - `filename` - 文件名。
    - `output` - 存储校验和的缓冲区。
  - 返回值：成功返回 `true`，失败返回 `false`。

- **缓冲区哈希**：`_cc_md2`
  - 计算输入缓冲区的 MD2 校验和。
  - 参数：
    - `input` - 输入数据缓冲区。
    - `ilen` - 输入数据的长度。
    - `output` - 存储校验和的缓冲区。

## 使用示例

### 示例 1：计算字符串的 MD2 哈希
```c
#include <libcc/crypto/md2.h>
#include <stdio.h>

int main() {
    const byte_t data[] = "Hello, MD2!";
    tchar_t hash[33]; // 32 字符 + 空终止符
    _cc_md2(data, sizeof(data) - 1, hash);
    printf("MD2 Hash: %s\n", hash);
    return 0;
}
```

### 示例 2：计算文件的 MD2 哈希
```c
#include <libcc/crypto/md2.h>
#include <stdio.h>

int main() {
    tchar_t hash[33]; // 32 字符 + 空终止符
    if (_cc_md2file(_T("example.txt"), hash)) {
        printf("File MD2 Hash: %s\n", hash);
    } else {
        printf("Failed to compute hash.\n");
    }
    return 0;
}
```

## 注意事项
1. **安全性**：MD2 算法已被证明存在安全漏洞，不建议用于安全敏感的场景。
2. **缓冲区大小**：`output` 缓冲区必须至少为 16 字节（或 32 字符的十六进制字符串）。
3. **线程安全**：函数 `_cc_md2_init`、`_cc_md2_update` 和 `_cc_md2_final` 是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。