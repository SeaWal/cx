#ifndef CX_SLICE_H
#define CX_SLICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cx_core.h"
#include "cx_types.h"

typedef struct cx_slice {
    const u8* data;
    size_t length;
    size_t elem_size;
} cx_slice;

/*
 * Creates a non-owning, read-only slice over existing memory.
 *
 * The slice does not allocate or free the referenced memory.
 * The referenced memory must remain valid for the lifetime of the slice.
 *
 * @param data Pointer to the first element.
 * @param length Number of elements.
 * @param elem_size Size of each element in bytes.
 * @return A slice describing the memory.
 */
CX_API cx_slice cx_slice_create(const u8* data, size_t length, size_t elem_size);

/*
 * Returns whether the slice contains no elements.
 *
 * @param slice Slice to inspect.
 * @return true if the slice is empty, false otherwise.
 */
CX_API bool cx_slice_is_empty(const cx_slice* slice);

/*
 * Returns the element at the specified index.
 *
 * @param slice Slice to access.
 * @param index Index of element to get.
 * @return Pointer to the element, or NULL if the index is out of bounds.
 */
CX_API const u8* cx_slice_get(const cx_slice* slice, size_t index);

/*
 * Creates a non-owning subslice.
 *
 * The resulting slice refers to the same underlying memory as the
 * original slice.
 *
 * @param slice Source slice.
 * @param start Index of starting element.
 * @param length Number of elements in the subslice.
 * @return Subslice, or an empty slice if the range is invalid.
 */
CX_API cx_slice cx_slice_subslice(const cx_slice* slice, size_t start, size_t length);

#ifdef __cplusplus
}
#endif

#endif // CX_SLICE_H
