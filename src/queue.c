#include <libcc/atomic.h>
#include <libcc/queue.h>

/**/
_CC_API_PUBLIC(void) _cc_queue_sync_push(_cc_queue_iterator_t *head, _cc_queue_iterator_t *lnk) {
    _cc_queue_iterator_t *next;
    do {
        next = head->next;
        lnk->next = next;
    } while (!_cc_atomic64_cas((_cc_atomic64_t *)&head->next, (uintptr_t)lnk->next, (uintptr_t)lnk));
}

/**/
_CC_API_PUBLIC(_cc_queue_iterator_t *) _cc_queue_sync_pop(_cc_queue_iterator_t *head) {
    _cc_queue_iterator_t *lnk;

    do {
        lnk = head->next;
    } while (!_cc_atomic64_cas((_cc_atomic64_t *)&head->next, (uintptr_t)lnk, (uintptr_t)lnk->next));

    return lnk;
}

/* Return the element at the specified zero-based index
 * where 0 is the head, 1 is the element next to head
 * and so on. If the index is out of range nullptr is returned. */
_CC_API_PUBLIC(_cc_queue_iterator_t *) _cc_queue_iterator_index(_cc_queue_iterator_t *head, long index) {
    _cc_queue_iterator_t *n;
    if (index < 0) {
        return nullptr;
    }

    n = head->next;
    while (index-- && n)
        n = n->next;

    return n;
}
