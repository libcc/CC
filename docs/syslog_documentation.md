# syslog.h 文档

## 文件概述
`syslog.h` 是一个实现系统日志功能的头文件，支持日志级别和远程日志发送。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

## 主要功能

### 1. 日志级别
定义了以下日志级别：
- `_CC_LOG_LEVEL_EMERG_` - 紧急
- `_CC_LOG_LEVEL_ALERT_` - 警报
- `_CC_LOG_LEVEL_CRIT_` - 严重
- `_CC_LOG_LEVEL_ERROR_` - 错误
- `_CC_LOG_LEVEL_WARNING_` - 警告
- `_CC_LOG_LEVEL_NOTICE_` - 通知
- `_CC_LOG_LEVEL_INFO_` - 信息
- `_CC_LOG_LEVEL_DEBUG_` - 调试

### 2. 日志函数
提供了以下函数：
- `_cc_open_syslog` - 打开系统日志。
- `_cc_close_syslog` - 关闭系统日志。
- `_cc_syslogA` - 发送 ASCII 日志。
- `_cc_syslogW` - 发送宽字符日志。

## 使用示例

### 示例 1：发送系统日志
```c
#include <libcc/syslog.h>

int main() {
    _cc_open_syslog(_CC_LOG_FACILITY_USER_, "myapp", "127.0.0.1", 514);
    _cc_syslogA(_CC_LOG_LEVEL_INFO_, "Application started", 18);
    _cc_close_syslog();
    return 0;
}
```

## 注意事项
1. **线程安全**：函数是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。