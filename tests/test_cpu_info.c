#include <assert.h>
#include <stdio.h>

#include <libcc/platform.h>

#ifdef _CC_HAVE_SYSCONF_
#include <unistd.h>
#endif

int main() {
    _cc_get_cpu_cores();
#if defined(_SC_NPROCESSORS_ONLN)
    printf("SC_NPROCESSORS_ONLN: %d\n", _cc_cpu_cores);
#endif

#ifdef __CC_WINDOWS__
    printf("Windows: %d\n", _cc_cpu_cores);
#endif

#if defined(__CC_MACOSX__) || defined(__CC_IPHONEOS__)
    printf("MacOSX: %d\n", _cc_cpu_cores);
#endif
#ifdef __CC_OS2__
    printf("OS/2: %d\n", _cc_cpu_cores);
#endif

    printf("All tests passed! cpu cores:%d\n", _cc_cpu_cores);

    return 0;
}