# linux.h 文档

## 文件概述
`linux.h` 是一个针对 Linux 平台的头文件，用于定义 Linux 特有的功能和宏。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. Linux 平台支持
提供了 Linux 平台特有的宏和函数定义。

### 2. 依赖关系
依赖于 `unix.h`，继承 Unix 平台的通用功能。

## 使用示例

### 示例 1：Linux 平台检测
```c
#include <libcc/core/linux.h>

int main() {
    #ifdef __CC_LINUX__
    printf("Running on Linux\n");
    #endif
    return 0;
}
```

## 注意事项
1. **平台限制**：仅适用于 Linux 平台。
2. **线程安全**：宏和函数是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。