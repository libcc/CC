# power.c 文档

## 文件概述
`power.c` 是一个用于获取系统电源状态的库文件，提供了查询电源状态（如电池电量、剩余时间等）的功能。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 获取电源信息
- **函数**: `_cc_get_power_info`
  - 查询当前系统的电源状态，包括电源类型（如电池、交流电等）、剩余电量百分比和剩余时间（秒）。
  - 参数说明：
    - `seconds`: 输出参数，返回电池剩余时间（秒）。如果为 `-1`，表示无法获取或无效。
    - `percent`: 输出参数，返回电池剩余电量百分比（0-100）。如果为 `-1`，表示无法获取或无效。
  - 返回值：
    - 返回 `_CC_POWER_STATE_ENUM_` 枚举值，表示当前的电源状态（如 `_CC_POWERSTATE_UNKNOWN_`、`_CC_POWERSTATE_ON_BATTERY_` 等）。

- **内部函数**: `_cc_get_sys_power_info`
  - 平台相关的实现，用于实际获取电源信息。

## 使用示例

### 示例：查询电源状态
```c
#include <libcc/power.h>

int main() {
    int32_t seconds;
    byte_t percent;
    _CC_POWER_STATE_ENUM_ state = _cc_get_power_info(&seconds, &percent);

    printf("Power State: %d\n", state);
    printf("Battery Percent: %d%%\n", percent);
    printf("Remaining Time: %d seconds\n", seconds);

    return 0;
}
```

## 注意事项
1. **平台依赖性**：部分功能依赖于操作系统提供的接口，可能在不同平台上的行为有所不同。
2. **返回值处理**：调用者需检查返回值和输出参数的有效性（如 `-1` 表示无效）。
3. **内存管理**：无需手动释放资源，所有内存由函数内部管理。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。