# thread.h 文档

## 文件概述
`thread.h` 是一个实现线程管理的头文件，支持线程创建、优先级设置和同步。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 线程结构
定义了 `_cc_thread_t` 结构体，用于存储线程信息：
- `status` - 线程状态。
- `thread_id` - 线程 ID。
- `stacksize` - 栈大小。
- `callback` - 线程函数。
- `user_args` - 用户参数。

### 2. 线程操作
提供了以下函数：
- `_cc_thread` - 创建线程。
- `_cc_thread_with_stacksize` - 创建指定栈大小的线程。
- `_cc_wait_thread` - 等待线程结束。
- `_cc_detach_thread` - 分离线程。
- `_cc_thread_priority` - 设置线程优先级。

## 使用示例

### 示例 1：创建线程
```c
#include <libcc/thread.h>
#include <stdio.h>

int32_t thread_func(_cc_thread_t *thread, pvoid_t args) {
    printf("Thread running\n");
    return 0;
}

int main() {
    _cc_thread_t *thread = _cc_thread(thread_func, "mythread", nullptr);
    _cc_wait_thread(thread, nullptr);
    return 0;
}
```

## 注意事项
1. **线程安全**：需要合理管理线程生命周期。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。