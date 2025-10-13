#include <libcc/alloc.h>
#include <libcc/platform.h>
#include <libcc/platform/android.h>
#include <sys/errno.h>

_CC_API_PUBLIC(void) _cc_get_preferred_languages(tchar_t *buf, size_t length) {
    Android_JNI_GetLocale(buf, length);
}