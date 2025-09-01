# rbtree.h 文档

## 文件概述
`rbtree.h` 是一个实现红黑树（Red-Black Tree）的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 红黑树结构
定义了 `_cc_rbtree_t` 和 `_cc_rbtree_iterator_t` 结构体，用于存储红黑树数据。

### 2. 红黑树操作
提供了以下函数：
- `_cc_rbtree_insert_color` - 插入节点并着色。
- `_cc_rbtree_erase` - 删除节点。
- `_cc_rbtree_first` - 获取第一个节点。
- `_cc_rbtree_last` - 获取最后一个节点。
- `_cc_rbtree_next` - 获取下一个节点。
- `_cc_rbtree_prev` - 获取前一个节点。

## 使用示例

### 示例 1：使用红黑树
```c
#include <libcc/rbtree.h>
#include <stdio.h>

int main() {
    _cc_rbtree_t tree;
    _CC_RB_INIT_ROOT(&tree);
    
    _cc_rbtree_iterator_t node;
    _cc_rbtree_node_init(&node);
    _cc_rbtree_insert_color(&tree, &node);
    
    _cc_rbtree_iterator_t *iter = _cc_rbtree_first(&tree);
    printf("First node: %p\n", iter);
    return 0;
}
```

## 注意事项
1. **线程安全**：需要外部同步机制保证线程安全。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。