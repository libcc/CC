#ifndef _C_CC_SKIP_LIST_H_INCLUDED_
#define _C_CC_SKIP_LIST_H_INCLUDED_

#include "list.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/**/
typedef struct _cc_skiplist {
    int level;
    _cc_list_iterator_t link[0];
} _cc_skiplist_t;

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#endif /*_C_CC_SKIP_LIST_H_INCLUDED_*/
