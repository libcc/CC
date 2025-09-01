# tchar.h 文档

## 文件概述
`tchar.h` 是一个实现跨平台字符处理的头文件，支持 Unicode 和多字节字符集。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 字符类型
定义了以下类型：
- `tchar_t` - 平台相关字符类型。
- `_TCHAR` - 兼容 Windows 的字符类型。

### 2. 字符串函数
提供了跨平台的字符串处理函数宏，如：
- `_tprintf` - 格式化输出。
- `_tcscpy` - 字符串拷贝。
- `_tcslen` - 字符串长度。

## 使用示例

### 示例 1：使用跨平台字符串
```c
#include <libcc/tchar.h>
#include <stdio.h>

int main() {
    tchar_t str[] = _T("Hello, world!");
    _tprintf(_T("String: %s\n"), str);
    return 0;
}
```

## 注意事项
1. **跨平台兼容性**：根据 `_CC_UNICODE_` 宏自动选择字符集。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。