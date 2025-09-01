# list.c 文档

## 文件概述
`list.c` 是一个实现双向链表功能的库文件，提供对链表的操作支持。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 链表操作
- **链表遍历**：支持正向和反向遍历链表。
- **链表排序**：提供稳定的插入排序算法。
- **链表合并**：支持将两个链表合并为一个链表。

### 2. 核心函数
- **函数**: `_cc_list_iterator_index`
  - 根据索引获取链表中的元素。
  - 参数：
    - `head`: 链表头指针。
    - `index`: 元素索引（支持负数，表示从尾部开始计数）。
  - 返回值：指向元素的指针，如果索引越界则返回 `nullptr`。

- **函数**: `_cc_list_iterator_append`
  - 将一个链表追加到另一个链表的指定位置。
  - 参数：
    - `head`: 目标链表头指针。
    - `add`: 要追加的链表头指针。

- **函数**: `_cc_list_iterator_sort`
  - 对链表进行稳定的插入排序。
  - 参数：
    - `lnk`: 链表头指针。
    - `_cmp`: 比较函数指针，用于定义排序规则。

## 使用示例

### 示例 1：遍历链表
```c
#include <libcc/list.h>

int main() {
    _cc_list_iterator_t *head = _cc_list_iterator_new();
    _cc_list_iterator_t *node;
    int i;

    // 添加元素到链表
    for (i = 0; i < 10; i++) {
        node = _cc_list_iterator_new();
        _cc_list_iterator_append(head, node);
    }

    // 遍历链表
    for (node = head->next; node != head; node = node->next) {
        printf("Node: %p\n", node);
    }

    return 0;
}
```

### 示例 2：链表排序
```c
#include <libcc/list.h>

int32_t compare(const _cc_list_iterator_t *a, const _cc_list_iterator_t *b) {
    // 定义排序规则
    return (a->data < b->data) ? -1 : 1;
}

int main() {
    _cc_list_iterator_t *head = _cc_list_iterator_new();
    _cc_list_iterator_t *node;
    int i;

    // 添加元素到链表
    for (i = 0; i < 10; i++) {
        node = _cc_list_iterator_new();
        node->data = rand();
        _cc_list_iterator_append(head, node);
    }

    // 排序链表
    _cc_list_iterator_sort(head, compare);

    return 0;
}
```

## 注意事项
1. **内存管理**：调用者需确保链表的生命周期与操作过程一致。
2. **线程安全**：链表操作不是线程安全的，多线程环境下需自行加锁。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。