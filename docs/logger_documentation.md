# logger.h 文档

## 文件概述
`logger.h` 是一个实现日志记录功能的头文件，支持多级别日志输出。该文件是 `libcc` 项目的一部分，遵循开源许可证（见文件头部注释）。

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
- `_cc_loggerA` - ASCII 日志输出。
- `_cc_loggerW` - 宽字符日志输出。
- `_cc_logger_format` - 格式化日志输出。

## 使用示例

### 示例 1：记录日志
```c
#include <libcc/logger.h>

int main() {
    const tchar_t *logger_info = "Application started";
    _cc_static_logger(_CC_LOG_LEVEL_INFO_, _T("Application started"));
    _cc_logger_error(_T("Application started %s"), _T("Ok"));
    _cc_logger(_CC_LOG_LEVEL_INFO_, logger_info);
    return 0;
}
```

## 注意事项
1. **线程安全**：函数是线程安全的。

## 许可证
该文件遵循开源许可证，详细信息请参考文件头部的注释。