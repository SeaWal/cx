#ifndef CX_ALGORITHM_H
#define CX_ALGORITHM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cx_core.h"
#include "cx_types.h"
#include "cx_iter.h"

/**
 * Finds the first element equal to a target value.
 * 
 * The returned pointer is borrowed from the underlying sequence.
 * The caller remains responsible for destroying the iterator.
 * 
 * @param iter Iterator to search. The iterator is consumed.
 * @param equals Returns true when an element equals the target.
 * @param target Value to search for.
 *
 * @return Pointer to the matching element, or NULL if not found.
 */
CX_API const void* cx_find(
    cx_iter* iter,
    bool (*equals)(const void* element, const void* target),
    const void* target
);


/**
 * Finds the first element satisfying a predicate.
 * 
 * The returned pointer is borrowed from the underlying sequence.
 * The caller remains responsible for destroying the iterator.
 * 
 * @param iter Iterator to search. The iterator is consumed.
 * @param predicate Returns true when an element satisfies the condition.
 * @param context User-defined data passed to the predicate.
 *
 * @return Pointer to the matching element, or NULL if not found.
 */
CX_API const void* cx_find_if(
    cx_iter* iter,
    bool (*predicate)(const void* element, void* context),
    void* context
);

#ifdef __cplusplus
}
#endif

#endif // CX_ALGORITHM_H
