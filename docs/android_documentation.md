# android.h 文档

## 文件概述
`android.h` 是一个针对 Android 平台的头文件，用于定义 Android 特有的功能和宏。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. Android 平台支持
提供了 Android 平台特有的宏和函数定义。

### 2. JNI 接口
支持以下功能：
- 文件操作
- 系统信息
- 权限管理

## 使用示例

### 示例 1：Android 平台检测
```c
#include <libcc/core/android.h>
#include <stdio.h>

int main() {
    #ifdef __CC_ANDROID__
    printf("Running on Android\n");
    #endif
    return 0;
}
```

## 注意事项
1. **平台限制**：仅适用于 Android 平台。
2. **线程安全**：宏和函数是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。