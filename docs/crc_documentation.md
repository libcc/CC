# crc.h 文档

## 文件概述
`crc.h` 是一个实现 CRC（循环冗余校验）和哈希算法的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. CRC 计算
支持以下 CRC 算法：
- `_cc_crc8` - 8 位 CRC。
- `_cc_crc16` - 16 位 CRC。
- `_cc_crc32` - 32 位 CRC。
- `_cc_crc64` - 64 位 CRC。

### 2. 哈希算法
支持以下哈希算法：
- `_cc_hash_rotating` - 旋转哈希。
- `_cc_hash` - 简单哈希。
- `_cc_hash_fnv1_32` - FNV-1 32 位哈希。
- `_cc_hash_fnv1a_32` - FNV-1a 32 位哈希。
- `_cc_hash_fnv1_64` - FNV-1 64 位哈希。
- `_cc_hash_fnv1a_64` - FNV-1a 64 位哈希。

## 使用示例

### 示例 1：计算 CRC32
```c
#include <libcc/crc.h>
#include <stdio.h>

int main() {
    const byte_t data[] = "Hello, CRC!";
    uint32_t crc = _cc_crc32(data, sizeof(data) - 1);
    printf("CRC32: 0x%08X\n", crc);
    return 0;
}
```

## 注意事项
1. **线程安全**：所有函数是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。