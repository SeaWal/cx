#ifndef CX_SLICE_H
#define CX_SLICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cx_core.h"
#include "cx_types.h"

/*
 * Opaque iterator used to traverse elements of a container.
 *
 * Iterators are non-owning. The container being iterated must remain
 * valid for the lifetime of the iterator.
 *
 * An iterator should be treated as a temporary traversal object and
 * should not be copied after iteration has begun.
 * 
 * The context and function pointers are implementation details of the
 * iterator and should not normally be accessed directly by callers.
 */
typedef struct cx_iter {
    void* ctx;

    bool (*next)(struct cx_iter* iter);
    const void* (*get)(const struct cx_iter* iter);
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
 * @param iter Iterator whose current element should be returned.
 * @return Pointer to the current element, or NULL if the iterator
 *         does not currently refer to an element.
 *
 */
CX_API const void* cx_iter_get(const cx_iter* iter);

#ifdef __cplusplus
}
#endif

#endif // CX_SLICE_H
