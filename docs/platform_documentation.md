# platform.h 文档

## 文件概述
`platform.h` 是一个定义平台相关宏的头文件，用于检测操作系统和平台类型。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 平台检测
支持检测以下平台：
- Windows (32/64位)
- Linux
- macOS/iOS
- Android
- FreeBSD/NetBSD/OpenBSD

### 2. 宏定义
定义了以下宏：
- `__CC_WINDOWS__` - Windows 平台。
- `__CC_LINUX__` - Linux 平台。
- `__CC_MACOSX__` - macOS 平台。
- `__CC_ANDROID__` - Android 平台。

## 使用示例

### 示例 1：平台检测
```c
#include <libcc/core/platform.h>
#include <stdio.h>

int main() {
    #ifdef __CC_WINDOWS__
    printf("Running on Windows\n");
    #elif defined(__CC_LINUX__)
    printf("Running on Linux\n");
    #elif defined(__CC_MACOSX__)
    printf("Running on macOS\n");
    #endif
    return 0;
}
```

## 注意事项
1. **跨平台兼容性**：确保在不同平台上正确使用宏。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。