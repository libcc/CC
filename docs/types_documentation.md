# types.h 文档

## 文件概述
`types.h` 是一个定义基本数据类型的头文件，用于跨平台兼容性。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 基本数据类型
定义了以下类型：
- `char_t` - 字符类型。
- `byte_t` - 字节类型。
- `bool_t` - 布尔类型。
- `float32_t` - 32 位浮点数。
- `float64_t` - 64 位浮点数。
- `pvoid_t` - 指针类型。

### 2. 整数类型
定义了固定大小的整数类型：
- `int8_t`/`uint8_t` - 8 位整数。
- `int16_t`/`uint16_t` - 16 位整数。
- `int32_t`/`uint32_t` - 32 位整数。
- `int64_t`/`uint64_t` - 64 位整数。

### 3. Unicode 支持
通过 `_CC_UNICODE_` 宏支持 Unicode 字符串。

## 使用示例

### 示例 1：使用基本类型
```c
#include <libcc/types.h>
#include <stdio.h>

int main() {
    int32_t num = 42;
    printf("Number: %d\n", num);
    return 0;
}
```

## 注意事项
1. **跨平台兼容性**：确保在不同平台上类型大小一致。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。