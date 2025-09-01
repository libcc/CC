# string.h 文档

## 文件概述
`string.h` 是一个实现字符串操作的头文件，提供扩展的字符串处理功能。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 字符串结构
定义了 `_cc_AString_t` 和 `_cc_WString_t` 结构体，用于存储字符串数据。

### 2. 字符串操作
提供了以下函数：
- `_cc_splitA` - 分割 ASCII 字符串。
- `_cc_splitW` - 分割宽字符串。
- `_cc_trimA_copy` - 修剪 ASCII 字符串。
- `_cc_trimW_copy` - 修剪宽字符串。

## 使用示例

### 示例 1：分割字符串
```c
#include <libcc/string.h>
#include <stdio.h>

int main() {
    _cc_AString_t parts[10];
    int count = _cc_splitA(parts, 10, "a,b,c", [](char_t* s, int32_t) {
        return *s == ',' ? 1 : 0;
    });
    printf("Split into %d parts\n", count);
    return 0;
}
```

## 注意事项
1. **线程安全**：函数是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。