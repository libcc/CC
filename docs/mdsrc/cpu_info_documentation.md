# cpu_info.c 文档

## 文件概述
`cpu_info.c` 是一个实现 CPU 核心数查询功能的库文件，提供对系统 CPU 信息的获取支持。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. CPU 核心数查询
- **跨平台支持**：支持 Linux、Windows、macOS 等操作系统。
- **缓存机制**：首次查询后会缓存结果，避免重复查询。

### 2. 核心函数
- **函数**: `_cc_get_cpu_cores`
  - 获取当前系统的 CPU 核心数。
  - 返回值：CPU 核心数。

## 使用示例

### 示例 1：获取 CPU 核心数
```c
#include <libcc/core/cpu_info.h>

int main() {
    int cores = _cc_get_cpu_cores();
    printf("CPU Cores: %d\n", cores);
    return 0;
}
```

## 注意事项
1. **缓存机制**：首次调用 `_cc_get_cpu_cores` 时会查询系统信息并缓存结果，后续调用直接返回缓存值。
2. **线程安全**：函数内部使用静态变量缓存结果，多线程环境下需自行加锁。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。