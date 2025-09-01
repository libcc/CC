# list.h 文档

## 文件概述
`list.h` 是一个实现双向链表的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 链表节点
定义了 `_cc_list_iterator_t` 结构体，用于存储链表节点：
- `prev` - 前驱节点。
- `next` - 后继节点。

### 2. 链表操作
提供了以下函数：
- `_cc_list_iterator_insert` - 插入节点。
- `_cc_list_iterator_remove` - 删除节点。
- `_cc_list_iterator_push_front` - 头部插入。
- `_cc_list_iterator_push_back` - 尾部插入。
- `_cc_list_iterator_pop` - 弹出节点。

## 使用示例

### 示例 1：使用双向链表
```c
#include <libcc/list.h>
#include <stdio.h>

int main() {
    _cc_list_iterator_t head;
    _cc_list_iterator_cleanup(&head);
    
    _cc_list_iterator_t node1, node2;
    _cc_list_iterator_push_back(&head, &node1);
    _cc_list_iterator_push_back(&head, &node2);
    
    _cc_list_iterator_t *iter;
    _cc_list_iterator_for(iter, &head) {
        printf("Node at %p\n", iter);
    }
    return 0;
}
```

## 注意事项
1. **线程安全**：需要外部同步机制保证线程安全。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。