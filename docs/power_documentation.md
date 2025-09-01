# power.h 文档

## 文件概述
`power.h` 是一个提供电源状态查询功能的头文件，用于获取系统电源的当前状态（如电池电量、充电状态等）。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 电源状态枚举
定义了 `_CC_POWER_STATE_ENUM_` 枚举，表示系统的电源状态：
- `_CC_POWERSTATE_UNKNOWN_`：无法确定电源状态。
- `_CC_POWERSTATE_ON_BATTERY_`：未插电，正在使用电池供电。
- `_CC_POWERSTATE_NO_BATTERY_`：已插电，但没有电池可用。
- `_CC_POWERSTATE_CHARGING_`：已插电，正在充电。
- `_CC_POWERSTATE_CHARGED_`：已插电，电池已充满。

### 2. 电源信息查询
- **函数**: `_cc_get_power_info`
  - 获取当前电源的详细信息。
  - 参数：
    - `secs`：指向存储电池剩余时间（秒）的指针，若为 `nullptr` 则忽略。
    - `pct`：指向存储电池剩余电量百分比（0-100）的指针，若为 `nullptr` 则忽略。
  - 返回值：当前电源状态（`_CC_POWER_STATE_ENUM_` 枚举值）。

## 使用示例

### 示例 1：查询电源状态
```c
#include <libcc/power.h>
#include <stdio.h>

int main() {
    int32_t secs;
    byte_t pct;
    _CC_POWER_STATE_ENUM_ state = _cc_get_power_info(&secs, &pct);

    switch (state) {
        case _CC_POWERSTATE_ON_BATTERY_:
            printf("On battery: %d%% remaining (%d seconds)\n", pct, secs);
            break;
        case _CC_POWERSTATE_CHARGING_:
            printf("Charging: %d%% remaining\n", pct);
            break;
        case _CC_POWERSTATE_CHARGED_:
            printf("Battery fully charged\n");
            break;
        default:
            printf("Unknown power state\n");
    }
    return 0;
}
```

## 注意事项
1. **跨平台兼容性**：该功能在不同平台上的实现可能有所不同，某些平台可能不支持所有状态。
2. **返回值**：若无法获取电池信息（如未插电或无电池），`secs` 和 `pct` 将返回 `-1`。
3. **线程安全**：函数 `_cc_get_power_info` 是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。