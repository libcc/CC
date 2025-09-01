# event.h 文档

## 文件概述
`event.h` 是一个实现 UI 事件系统的头文件，用于处理用户交互事件。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 事件类型
定义了以下事件类型：
- `_CC_EVENT_MOUSE_` - 鼠标事件
- `_CC_EVENT_KEYBOARD_` - 键盘事件
- `_CC_EVENT_TOUCH_` - 触摸事件

### 2. 事件处理
提供了以下函数：
- `_cc_event_register` - 注册事件处理器
- `_cc_event_unregister` - 注销事件处理器
- `_cc_event_dispatch` - 分发事件

## 使用示例

### 示例 1：处理鼠标点击事件
```c
#include <libcc/widgets/event.h>
#include <stdio.h>

void on_click(_cc_event_t* event) {
    printf("Click at (%d, %d)\n", event->mouse.x, event->mouse.y);
}

int main() {
    _cc_event_register(_CC_EVENT_MOUSE_, _CC_MOUSE_CLICK_, on_click);
    // 进入事件循环
    return 0;
}
```

## 注意事项
1. **线程安全**：事件处理器应在主线程调用。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。