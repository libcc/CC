# loadso.h 文档

## 文件概述
`loadso.h` 是一个实现动态库加载功能的头文件，支持跨平台的动态库加载和符号查找。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 动态库加载
提供了以下函数：
- `_cc_load_object` - 加载动态库。
- `_cc_load_function` - 查找动态库中的符号。
- `_cc_unload_object` - 卸载动态库。

## 使用示例

### 示例 1：加载动态库
```c
#include <libcc/loadso.h>
#include <stdio.h>

int main() {
    pvoid_t handle = _cc_load_object(_T("libexample.so"));
    if (handle) {
        printf("Library loaded\n");
        _cc_unload_object(handle);
    }
    return 0;
}
```

## 注意事项
1. **跨平台兼容性**：支持 Windows 和 Unix 平台。
2. **线程安全**：函数是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。