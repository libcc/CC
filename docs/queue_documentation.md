# queue.h 文档

## 文件概述
`queue.h` 是一个实现队列数据结构的头文件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 队列结构
定义了 `_cc_queue_iterator_t` 结构体，用于存储队列节点。

### 2. 队列操作
提供了以下函数：
- `_cc_queue_iterator_push` - 入队。
- `_cc_queue_iterator_pop` - 出队。
- `_cc_queue_sync_push` - 线程安全入队。
- `_cc_queue_sync_pop` - 线程安全出队。

## 使用示例

### 示例 1：使用队列
```c
#include <libcc/queue.h>
#include <stdio.h>

int main() {
    _cc_queue_iterator_t head;
    _cc_queue_iterator_cleanup(&head);
    
    _cc_queue_iterator_t node;
    _cc_queue_iterator_push(&head, &node);
    
    _cc_queue_iterator_t* item = _cc_queue_iterator_pop(&head);
    printf("Popped item: %p\n", item);
    return 0;
}
```

## 注意事项
1. **线程安全**：需要外部同步或使用 `_cc_queue_sync_*` 函数。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。