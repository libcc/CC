# UTF.c 文档

## 文件概述
`UTF.c` 是一个用于处理 Unicode 字符编码转换的库文件，提供了多种 UTF-8、UTF-16 和 UTF-32 之间的转换函数。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. UTF-16 到 UTF-8 的转换
- **函数**: `_cc_convert_utf16_literal_to_utf8`
  - 将 UTF-16 编码的字符串字面量转换为 UTF-8 编码。
  - 支持处理 UTF-16 代理对（Surrogate Pairs）。

- **函数**: `_cc_utf16_to_utf8`
  - 将 UTF-16 编码的字符串转换为 UTF-8 编码。

### 2. UTF-8 到 UTF-16 的转换
- **函数**: `_cc_utf8_to_utf16`
  - 将 UTF-8 编码的字符串转换为 UTF-16 编码。

### 3. UTF-32 到 UTF-16 的转换
- **函数**: `_cc_utf32_to_utf16`
  - 将 UTF-32 编码的字符串转换为 UTF-16 编码。

### 4. UTF-16 到 UTF-32 的转换
- **函数**: `_cc_utf16_to_utf32`
  - 将 UTF-16 编码的字符串转换为 UTF-32 编码。

### 5. UTF-32 到 UTF-8 的转换
- **函数**: `_cc_utf32_to_utf8`
  - 将 UTF-32 编码的字符串转换为 UTF-8 编码。

### 6. UTF-8 到 UTF-32 的转换
- **函数**: `_cc_utf8_to_utf32`
  - 将 UTF-8 编码的字符串转换为 UTF-32 编码。

### 7. Unicode 码点到 UTF-8/UTF-16 的转换
- **函数**: `_cc_unicode_to_utf8`
  - 将 Unicode 码点转换为 UTF-8 编码。

- **函数**: `_cc_unicode_to_utf16`
  - 将 Unicode 码点转换为 UTF-16 编码。

### 8. UTF-8 到 Unicode 码点的转换
- **函数**: `_cc_utf8_to_unicode`
  - 将 UTF-8 编码的字符转换为 Unicode 码点。

## 使用示例

### 示例 1：UTF-16 到 UTF-8 的转换
```c
const tchar_t *utf16_input = "\\u0041\\u0042\\u4e2d";
tchar_t utf8_output[32];
int32_t result = _cc_convert_utf16_literal_to_utf8(&utf16_input, utf16_input + strlen(utf16_input), utf8_output, sizeof(utf8_output));
```

### 示例 2：UTF-8 到 UTF-16 的转换
```c
const uint8_t *utf8_input = "ABC中文";
uint16_t utf16_output[32];
int32_t result = _cc_utf8_to_utf16(utf8_input, utf8_input + strlen(utf8_input), utf16_output, utf16_output + sizeof(utf16_output));
```

## 注意事项
1. **输入验证**：所有函数均对输入进行了严格的验证，确保不会处理无效的编码序列。
2. **缓冲区大小**：调用者需确保输出缓冲区足够大，以避免溢出。
3. **性能**：函数设计为高效实现，适合频繁调用。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。