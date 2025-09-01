# alloc.h 文档

## 文件概述
`alloc.h` 是一个实现内存分配功能的头文件，提供安全的内存管理接口。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 内存分配
提供了以下函数：
- `_cc_malloc` - 分配内存。
- `_cc_calloc` - 分配并清零内存。
- `_cc_realloc` - 重新分配内存。
- `_cc_free` - 释放内存。

### 2. 字符串复制
提供了以下函数：
- `_cc_strdupA` - 复制 ASCII 字符串。
- `_cc_strdupW` - 复制宽字符串。

## 使用示例

### 示例 1：安全内存分配
```c
#include <libcc/alloc.h>
#include <stdio.h>

int main() {
    char_t* str = _cc_strdupA("Hello");
    if (str) {
        printf("%s\n", str);
        _cc_free(str);
    }
    return 0;
}
```

## 注意事项
1. **内存泄漏**：确保配对使用分配和释放函数。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。