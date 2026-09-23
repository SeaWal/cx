#ifndef CX_VECTOR_H
#define CX_VECTOR 

#ifdef __cplusplus
extern "C" {
#endif

#include "cx_allocator.h"
#include "cx_core.h"
#include "cx_types.h"

typedef struct cx_vector cx_vector;

/*
 * Creates an empty vector for elements of the specified type.
 *
 * @param allocator Allocator to use.
 * @param type Element type stored by the vector.
 * @return Pointer to the vector, or NULL on failure.
 */
#define cx_vector_create(allocator, type) \
    cx_vector_create_raw(allocator, sizeof(type), _Alignof(type))

/*
 * Creates an empty vector using explicit element metadata.
 *
 * @param allocator Allocator to use.
 * @param element_size Size of each element in bytes.
 * @param element_align  Alignment of each element.
 * @return Pointer to the vector, or NULL on failure.
 */
CX_API cx_vector* cx_vector_create_raw(
    cx_allocator* allocator, 
    size_t element_size, 
    size_t element_align
);

/*
 * Destroys a vector and releases its allocated memory.
 *
 * @param vector Vector to destroy.
 */
CX_API void cx_vector_destroy(cx_vector* vector);

/*
 * Returns the number of elements stored in the vector.
 *
 * @param vector Vector to query.
 * @return Number of elements.
 */
CX_API size_t cx_vector_length(const cx_vector* vector);

/*
 * Returns the number of elements the vector can currently hold.
 *
 * @param vector Vector to query.
 * @return Current capacity.
 */
CX_API size_t cx_vector_capacity(const cx_vector* vector);

/*
 * Returns whether the vector contains no elements.
 *
 * @param vector Vector to query.
 * @return true if empty, false otherwise.
 */
CX_API bool cx_vector_is_empty(const cx_vector* vector);

/*
 * Adds an element to the end of the vector.
 *
 * The specified type should match the type used to create the vector.
 *
 * @param vector Vector to modify.
 * @param type Element type.
 * @param value Value to copy into the vector.
 * @return true on success, false on failure.
 */
#define cx_vector_push(vector, type, value) \
    cx_vector_push_ptr((vector), &(type){ (value) })

/*
 * Adds an element to the end of the vector by copying from a pointer.
 *
 * @param vector Vector to modify.
 * @param value Pointer to the element to copy.
 * @return true on success, false on failure.
 */
CX_API void cx_vector_push_ptr(cx_vector* vector, const void* value);

/*
 * Returns a pointer to the element at the specified index.
 *
 * @param vector Vector to access.
 * @param index Index of element to get.
 * @return Pointer to the element, or NULL if out of bounds.
 */
CX_API void* cx_vector_get(cx_vector* vector, size_t index);

/*
 * Returns a const pointer to the element at the specified index.
 *
 * @param vector Vector to access.
 * @param index Index of element to get.
 * @return Pointer to the element, or NULL if out of bounds.
 */
CX_API const void* cx_vector_get_const(const cx_vector* vector, size_t index);

/*
 * Removes all elements from the vector.
 *
 * Allocated storage is retained.
 *
 * @param vector Vector to clear.
 */
CX_API void cx_vector_clear(cx_vector* vector);

/*
 * Ensures the vector can hold at least the specified number of elements.
 *
 * @param vector Vector to modify.
 * @param capacity Minimum required capacity.
 * @return true on success, false on allocation failure.
 */
CX_API bool cx_vector_reserve(cx_vector* vector, size_t capacity);

/*
 * Reduces the vector's capacity to its current length.
 *
 * @param vector Vector to modify.
 * @return true on success, false on allocation failure.
 */
CX_API bool cx_vector_shrink_to_fit(cx_vector* vector);

/*
 * Removes the last element from the vector.
 *
 * If out_element is not NULL, the removed element is copied into it.
 *
 * @param vector Vector to pop from.
 * @param out_element Destination for the removed element, or NULL.
 */
CX_API void cx_vector_pop(cx_vector* vector, void* out_value)

/*
 * Removes the element at the specified index.
 *
 * Elements after the removed element are shifted toward the beginning
 * of the vector.
 *
 * @param vector Vector to modify.
 * @param index Index of the element to remove.
 * @return true on success, false if the index is out of bounds.
 */
CX_API bool cx_vector_remove(cx_vector* vector, size_t index);

/*
 * Inserts an element of the specified type at the specified index.
 *
 * Elements at and after the index are shifted toward the end of the
 * vector. Inserting at the current length appends the element.
 *
 * @param vector Vector to modify.
 * @param type Element type.
 * @param index Index to inser the element.
 * @param value Value to copy into the vector.
 * @return true on success, false if the index is invalid or allocation fails.
 */
#define cx_vector_insert(vector, type, index, value) \
    cx_vector_insert_ptr( (vector), (index), &(type){ (value) })

/*
 * Inserts an element at the specified index by copying from a pointer.
 *
 * Elements at and after the index are shifted toward the end of the
 * vector. Inserting at the current length appends the element.
 *
 * @param vector Vector to modify.
 * @param index Index to insert the element.
 * @param value Pointer to the element to copy into the vector.
 * @return true on success, false if the index is invalid or allocation fails.
 */
CX_API bool cx_vector_insert_ptr(cx_vector* vector, size_t index, const void* value);


#ifdef __cplusplus
}
#endif


#endif // CX_VECTOR_H