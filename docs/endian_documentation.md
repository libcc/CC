# endian.h 文档

## 文件概述
`endian.h` 是一个处理字节序（Endianness）的头文件，支持大端和小端字节序的转换。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 字节序检测
提供了函数 `_cc_isbig`，用于检测当前系统的字节序。

### 2. 字节序转换
支持以下转换函数：
- `_cc_swap16` - 16 位字节序转换。
- `_cc_swap32` - 32 位字节序转换。
- `_cc_swap64` - 64 位字节序转换。
- `_cc_swap_float` - 浮点数字节序转换。

### 3. 宏定义
定义了以下宏：
- `_CC_LIL_ENDIAN_` - 小端字节序。
- `_CC_BIG_ENDIAN_` - 大端字节序。

## 使用示例

### 示例 1：字节序转换
```c
#include <libcc/endian.h>
#include <stdio.h>

int main() {
    uint32_t num = 0x12345678;
    uint32_t swapped = _cc_swap32(num);
    printf("Original: 0x%08X, Swapped: 0x%08X\n", num, swapped);
    return 0;
}
```

## 注意事项
1. **跨平台兼容性**：支持不同字节序的系统。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。