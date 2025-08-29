/*
 * Copyright libcc.cn@gmail.com. and other libcc contributors.
 * All rights reserved.org>
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:

 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/
#include <libcc/generic.h>
#include <libcc/string.h>

#ifdef _CC_HAVE_SYSCONF_
#include <unistd.h>
#endif

#ifdef __CC_MACOSX__
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/task_info.h>
#include <mach/mach_types.h>
#include <mach/mach.h>
#elif defined(__CC_OPENBSD__) && defined(__powerpc__)
#include <sys/param.h>
#include <machine/cpu.h>
#endif

/**/
int _cc_cpu_cores = 0;

/**/
_CC_API_PUBLIC(int) _cc_get_cpu_cores(void) {
#if defined(_SC_NPROCESSORS_ONLN)
    if (_cc_cpu_cores <= 0) {
        _cc_cpu_cores = (int)sysconf(_SC_NPROCESSORS_ONLN);
    }
#endif

#ifdef __CC_WINDOWS__
    if (_cc_cpu_cores <= 0) {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        _cc_cpu_cores = info.dwNumberOfProcessors;
    }
#endif

#if defined(__CC_MACOSX__) || defined(__CC_IPHONEOS__)
    if (_cc_cpu_cores <= 0) {
        size_t size = sizeof(_cc_cpu_cores);
        sysctlbyname("hw.ncpu", &_cc_cpu_cores, &size, nullptr, 0);
    }
#endif
#ifdef __CC_OS2__
    if (_cc_cpu_cores <= 0) {
        DosQuerySysInfo(QSV_NUMPROCESSORS, QSV_NUMPROCESSORS,
                        &_cc_cpu_cores, sizeof(_cc_cpu_cores) );
    }
#endif
    /* There has to be at least 1, right? :) */
    if (_cc_cpu_cores <= 0) {
        _cc_cpu_cores = 1;
    }
    return _cc_cpu_cores;
}