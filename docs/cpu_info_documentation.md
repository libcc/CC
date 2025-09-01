# cpu_info.h 文档

## 文件概述
`cpu_info.h` 是一个定义 CPU 相关信息的头文件，用于获取 CPU 核心数等信息。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. CPU 核心数
提供了函数 `_cc_get_cpu_cores`，用于获取当前系统的 CPU 核心数。

### 2. 全局变量
定义了全局变量 `_cc_cpu_cores`，存储 CPU 核心数。

## 使用示例

### 示例 1：获取 CPU 核心数
```c
#include <libcc/core/cpu_info.h>
#include <stdio.h>

int main() {
    int cores = _cc_get_cpu_cores();
    printf("CPU cores: %d\n", cores);
    return 0;
}
```

## 注意事项
1. **线程安全**：函数 `_cc_get_cpu_cores` 是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。