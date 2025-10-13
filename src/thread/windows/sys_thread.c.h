#ifndef _C_CC_SYS_WINDOWS_THREAD_C_H_INCLUDED_
#define _C_CC_SYS_WINDOWS_THREAD_C_H_INCLUDED_

#include <libcc/alloc.h>
#include <libcc/platform/windows.h>
#include <libcc/logger.h>
#include <libcc/thread.h>

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__CC_WIN32_CE__) && (__CC_WIN32_CE__ < 300)

typedef struct _SYNCH_HANDLE_STRUCTURE {
    HANDLE hEvent;
    HANDLE hMutex;
    HANDLE hSemph;
    LONG MaxCount;
    volatile LONG CurCount;
    LPCTSTR lpName;
} SYNCH_HANDLE_STRUCTURE, *SYNCHHANDLE;

#define SYNCH_HANDLE_SIZE sizeof(SYNCH_HANDLE_STRUCTURE)

/* Error codes - all must have bit 29 set */
#define SYNCH_ERROR 0X20000000 /* EXERCISE - REFINE THE ERROR NUMBERS */

extern SYNCHHANDLE CreateSemaphoreCE(LPSECURITY_ATTRIBUTES, LONG, LONG, LPCTSTR);

extern BOOL ReleaseSemaphoreCE(SYNCHHANDLE, LONG, LPLONG);
extern DWORD WaitForSemaphoreCE(SYNCHHANDLE, DWORD);

extern BOOL CloseSynchHandle(SYNCHHANDLE);

#endif /*__CC_WIN32_CE__*/

#define _CC_WINDOWS_FORCE_MUTEX_CRITICAL_SECTIONS_ 0
#define _CC_WINDOWS_SUPPORTED_CONDITION_ 1

typedef _cc_mutex_t *(*pfnCreateMutex)(void);
typedef bool_t (*pfnLockMutex)(_cc_mutex_t *);
typedef int (*pfnTryLockMutex)(_cc_mutex_t *);
typedef bool_t (*pfnUnlockMutex)(_cc_mutex_t *);
typedef void (*pfnDestroyMutex)(_cc_mutex_t *);

typedef enum {
    _CC_MUTEX_INVALID_ = 0,
    _CC_MUTEX_SRW_,
    _CC_MUTEX_CS_
} _enum_mutex_type_t;

typedef struct _cc_mutex_impl {
    pfnCreateMutex Create;
    pfnDestroyMutex Destroy;
    pfnLockMutex Lock;
    pfnTryLockMutex TryLock;
    pfnUnlockMutex Unlock;
    /* */
    _enum_mutex_type_t Type;
} _cc_mutex_impl_t;

struct _cc_mutex_cs {
    CRITICAL_SECTION cs;
};

#ifndef SRWLOCK_INIT
#define SRWLOCK_INIT                                                                                                   \
    { 0 }
typedef struct _SRWLOCK {
    PVOID Ptr;
} SRWLOCK, *PSRWLOCK;
#endif

struct _cc_mutex_srw {
    SRWLOCK srw;
    DWORD count;
    DWORD owner;
};

struct _cc_semaphore {
#if defined(__CC_WIN32_CE__) && (__CC_WIN32_CE__ < 300)
    SYNCHHANDLE ident;
#else
    HANDLE ident;
#endif
    LONG count;
};

/**/
struct _cc_condition {
#ifdef _CC_WINDOWS_SUPPORTED_CONDITION_
    CONDITION_VARIABLE cond_var;
#else
    HANDLE cond_var;
#endif
};

extern _cc_mutex_impl_t _cc_mutex_impl_active;
/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /*_C_CC_SYS_WINDOWS_THREAD_C_H_INCLUDED_*/
