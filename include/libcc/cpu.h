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

#ifndef _C_CC_CPU_INFO_H_INCLUDED_
#define _C_CC_CPU_INFO_H_INCLUDED_

#include "types.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/**/
extern int _cc_cpu_cores;

/**
 *  @brief This function returns the number of CPU cores available.
 */
_CC_API_PUBLIC(int) _cc_get_cpu_cores(void);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /* _C_CC_CPU_INFO_H_INCLUDED_ */

