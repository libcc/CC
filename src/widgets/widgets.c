#include <libcc/widgets/widgets.h>

/**/
_CC_API_PUBLIC(void) _cc_widget_open_syslog(byte_t facility, const tchar_t *ip, const uint16_t port) {
    _cc_open_syslog(facility, "libcc.widgets.dll", ip, port);
}

/**/
_CC_API_PUBLIC(void) _cc_widget_close_syslog(void) {
    _cc_close_syslog();
}