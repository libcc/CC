# hmap.h 文档

## 文件概述
`hmap.h` 是一个实现哈希表（Hash Map）的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 哈希表结构
定义了 `_cc_hmap_t` 结构体，用于存储哈希表数据：
- `limit` - 哈希表容量。
- `count` - 当前元素数量。
- `slots` - 槽位数组。
- `equals_func` - 键比较函数。
- `hash_func` - 哈希函数。

### 2. 哈希表操作
提供了以下函数：
- `_cc_alloc_hmap` - 初始化哈希表。
- `_cc_free_hmap` - 释放哈希表。
- `_cc_hmap_push` - 插入键值对。
- `_cc_hmap_find` - 查找键值对。
- `_cc_hmap_pop` - 删除键值对。

## 使用示例

### 示例 1：使用哈希表
```c
#include <libcc/hmap.h>
#include <stdio.h>

int main() {
    _cc_hmap_t map;
    if (_cc_alloc_hmap(&map, 16, nullptr, nullptr)) {
        _cc_hmap_push(&map, (uintptr_t)"key", (uintptr_t)"value");
        uintptr_t value = _cc_hmap_find(&map, (uintptr_t)"key");
        printf("Value: %s\n", (char_t*)value);
        _cc_free_hmap(&map);
    }
    return 0;
}
```

## 注意事项
1. **线程安全**：需要外部同步机制保证线程安全。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。