# UTF.h 文档

## 文件概述
`UTF.h` 是一个实现 UTF-8、UTF-16 和 UTF-32 编码转换的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. Unicode 编码转换
支持以下转换函数：
- `_cc_unicode_to_utf8` - Unicode 转 UTF-8。
- `_cc_utf8_to_unicode` - UTF-8 转 Unicode。
- `_cc_unicode_to_utf16` - Unicode 转 UTF-16。
- `_cc_utf8_to_utf16` - UTF-8 转 UTF-16。
- `_cc_utf16_to_utf8` - UTF-16 转 UTF-8。
- `_cc_utf8_to_utf32` - UTF-8 转 UTF-32。
- `_cc_utf32_to_utf8` - UTF-32 转 UTF-8。
- `_cc_utf16_to_utf32` - UTF-16 转 UTF-32。
- `_cc_utf32_to_utf16` - UTF-32 转 UTF-16。

### 2. GBK 编码转换
支持以下转换函数：
- `_cc_utf8_to_gbk` - UTF-8 转 GBK。
- `_cc_gbk_to_utf8` - GBK 转 UTF-8。

## 使用示例

### 示例 1：UTF-8 转 UTF-16
```c
#include <libcc/UTF.h>
#include <stdio.h>

int main() {
    const uint8_t utf8[] = "Hello, 世界!";
    uint16_t utf16[100];
    int32_t len = _cc_utf8_to_utf16(utf8, utf8 + sizeof(utf8), utf16, utf16 + 100);
    printf("UTF-16 length: %d\n", len);
    return 0;
}
```

## 注意事项
1. **线程安全**：所有函数是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。