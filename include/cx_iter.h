#ifndef CX_ITER_H
#define CX_ITER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cx_core.h"
#include "cx_types.h"

/*
 * Generic iterator used to traverse elements of a sequence.
 *
 * Iterators borrow their underlying container or sequence. The
 * underlying object and any required allocator must remain valid
 * until the iterator is destroyed.
 *
 * Treat iterators as move-only: do not copy an iterator by value.
 * Destroy each iterator exactly once.
 *
 * Callers should use the public cx_iter_* functions rather than
 * accessing the context or function pointers directly.
 */
typedef struct cx_iter {
    void* ctx;

    bool (*next)(struct cx_iter* iter);
    const void* (*get)(const struct cx_iter* iter);
    void (*destroy)(struct cx_iter* iter);
} cx_iter;

/*
 * Advances the iterator to the next element.
 *
 * @param iter Iterator to advance.
 * @return true if the iterator was advanced to an element,
 *         false if the end of the sequence was reached.
 */
CX_API bool cx_iter_next(cx_iter* iter);

/*
 * Returns the current element.
 * 
 * The returned pointer is non-owning and its lifetime depends on
 * the underlying container.
 * 
 * @param iter Iterator whose current element should be returned.
 * @return Pointer to the current element, or NULL if the iterator
 *         does not currently refer to an element.
 *
 */
CX_API const void* cx_iter_get(const cx_iter* iter);

/**
 * Releases resources owned by the iterator.
 *
 * The iterator must not be used after destruction.
 * 
 * @param iter Iterator to destroy. May be NULL.
 */
CX_API void cx_iter_destroy(cx_iter* iter);

#ifdef __cplusplus
}
#endif

#endif // CX_ITER_H
