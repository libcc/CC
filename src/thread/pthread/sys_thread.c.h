#ifndef _C_CC_SYS_PTHREAD_C_H_INCLUDED_
#define _C_CC_SYS_PTHREAD_C_H_INCLUDED_

#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#include <libcc/alloc.h>
#include <libcc/thread.h>
#include <libcc/time.h>
#include <semaphore.h>

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif
#if !_CC_THREAD_PTHREAD_RECURSIVE_MUTEX_ && !_CC_THREAD_PTHREAD_RECURSIVE_MUTEX_NP_
#define _CC_FAKE_RECURSIVE_MUTEX_ 1
#endif
struct _cc_mutex {
    pthread_mutex_t ident;
#if _CC_FAKE_RECURSIVE_MUTEX_
    int32_t recursive;
    pthread_t owner;
#endif
};

struct _cc_semaphore {
    sem_t sem;
};

struct _cc_condition {
    pthread_cond_t cond_var;
};

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /*_C_CC_SYS_PTHREAD_C_H_INCLUDED_*/
