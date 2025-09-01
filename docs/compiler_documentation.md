# compiler.h 文档

## 文件概述
`compiler.h` 是一个定义编译器相关宏和功能的头文件，用于处理不同编译器和平台的兼容性问题。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 编译器检测
支持检测以下编译器：
- GCC/Clang
- Microsoft Visual C++ (MSVC)
- Intel C++ Compiler

### 2. 平台检测
支持检测以下平台：
- Windows (32/64位)
- Linux
- macOS/iOS
- Android
- FreeBSD/NetBSD/OpenBSD

### 3. 功能宏
提供以下功能宏：
- `_CC_FORCE_INLINE_` - 强制内联函数。
- `_cc_likely`/`_cc_unlikely` - 分支预测优化。
- `_cc_alignas` - 内存对齐。
- `_cc_sync_synchronize` - 内存屏障。

### 4. 标准版本检测
支持检测 C/C++ 标准版本（如 C11、C++17 等）。

## 使用示例

### 示例 1：强制内联函数
```c
#include <libcc/core/compiler.h>

_CC_FORCE_INLINE_ int add(int a, int b) {
    return a + b;
}

int main() {
    printf("Sum: %d\n", add(2, 3));
    return 0;
}
```

## 注意事项
1. **跨平台兼容性**：确保在不同编译器和平台上正确使用宏。
2. **线程安全**：宏和函数是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。