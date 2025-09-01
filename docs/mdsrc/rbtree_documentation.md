# rbtree.c 文档

## 文件概述
`rbtree.c` 是一个实现红黑树（Red-Black Tree）数据结构的库文件，提供了红黑树的插入、删除和平衡操作。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 红黑树的基本操作
- **红黑树性质**：
  - 节点是红色或黑色。
  - 根节点是黑色。
  - 所有叶子节点（`nullptr`）是黑色。
  - 每个红色节点的两个子节点都是黑色。
  - 从任一节点到其每个叶子的所有路径都包含相同数目的黑色节点。

### 2. 插入操作
- **函数**: `_cc_rbtree_insert_color`
  - 将节点插入红黑树，并调整树的结构以保持红黑树的性质。
  - 支持三种情况的调整（Case 1、Case 2、Case 3）。

### 3. 删除操作
- **函数**: `_rb_erase_color`
  - 删除节点后调整树的结构以保持红黑树的性质。
  - 支持四种情况的调整（Case 1、Case 2、Case 3、Case 4）。

### 4. 辅助函数
- **函数**: `_rb_rotate_set_parents`
  - 执行旋转操作（左旋或右旋），并更新父节点和颜色信息。
- **函数**: `_rb_set_parent_color`
  - 设置节点的父节点和颜色。

## 使用示例

### 示例 1：插入节点
```c
#include <libcc/rbtree.h>

int main() {
    _cc_rbtree_t tree = {0};
    _cc_rbtree_iterator_t node = {0};

    // 插入节点并调整颜色
    _cc_rbtree_insert_color(&tree, &node);

    return 0;
}
```

### 示例 2：删除节点
```c
#include <libcc/rbtree.h>

int main() {
    _cc_rbtree_t tree = {0};
    _cc_rbtree_iterator_t node = {0};

    // 假设节点已插入
    _cc_rbtree_insert_color(&tree, &node);

    // 删除节点并调整颜色
    _rb_erase_color(_rb_parent(&node), &tree);

    return 0;
}
```

## 注意事项
1. **线程安全**：红黑树的操作不是线程安全的，多线程环境下需要外部同步。
2. **性能**：红黑树的插入、删除和查找操作的时间复杂度为 O(log n)。
3. **内存管理**：调用者需确保节点的生命周期与树的操作一致。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。