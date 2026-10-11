#ifndef CX_ALGORITHM_H
#define CX_ALGORITHM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cx_core.h"
#include "cx_types.h"
#include "cx_iter.h"

/**
 * Finds the first element matching a predicate.
 *
 * The returned pointer is borrowed from the underlying sequence.
 * The caller remains responsible for destroying the iterator.
 * 
 * @param iter Iterator to search. The iterator is consumed.
 * @param predicate Function that returns true for a matching element.
 * @param context User-defined data passed to the predicate.
 *
 * @return Pointer to the matching element, or NULL if no match exists.
 */
CX_API const void* cx_find(
    cx_iter* iter,
    bool (*predicate)(const void* element, void* context),
    void* context
);


#ifdef __cplusplus
}
#endif

#endif // CX_ALGORITHM_H
