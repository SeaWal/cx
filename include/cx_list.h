#ifndef CX_LIST_H
#define CX_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#include "cx_allocator.h"
#include "cx_core.h"
#include "cx_iter.h"

/**
 * A generic doubly linked list.
 *
 * Elements are stored by copying their bytes into list-owned storage.
 * 
 * Memory for the list is allocated by the `cx_allocator` provided by
 * the caller.
 * 
 * The list does not manage resources referenced by pointers contained
 * within an element.
 *
 * The list owns its nodes but does not own its allocator. The allocator
 * must remain valid until the list is destroyed.
 *
 * Indexed access takes O(n) time.
 * The list is not thread-safe.
 */
typedef struct cx_list cx_list;

/**
 * Creates an empty doubly linked list.
 *
 * @param allocator Allocator used for list and node allocations.
 * @param elem_size Size of each element in bytes. Must be greater than zero.
 * @param elem_align Required alignment of each element, typically
 *                   obtained with _Alignof(type).
 *
 * @return Pointer to the new list, or NULL if arguments are invalid or
 *         memory allocation fails.
 */
CX_API cx_list* cx_list_create_raw(
    cx_allocator* allocator,
    size_t elem_size,
    size_t elem_align
);

/**
 * Creates a list for elements of the specified type.
 *
 * @param allocator Allocator used for list and node allocations.
 * @param type Element type.
 *
 * @return Pointer to the new list, or NULL if creation fails.
 */
#define cx_list_create(allocator, type) \
    cx_list_create_raw((allocator), sizeof(type), _Alignof(type))

/**
 * Destroys a list and releases all memory owned by it.
 *
 * Passing NULL has no effect.
 *
 * @param list List to destroy.
 *
 * Pointers to elements and iterators referring to the list become invalid
 * after destruction.
 */
CX_API void cx_list_destroy(cx_list* list);

/**
 * Returns the number of elements in a list.
 *
 * @param list List to query.
 *
 * @return Number of elements, or zero if list is NULL.
 */
CX_API size_t cx_list_length(const cx_list* list);

/**
 * Checks whether a list is empty.
 *
 * A NULL list is considered empty.
 *
 * @param list List to query.
 *
 * @return true if the list is empty or NULL, false otherwise.
 */
CX_API bool cx_list_is_empty(const cx_list* list);

/**
 * Removes all elements from a list while keeping it usable.
 *
 * Passing NULL has no effect.
 *
 * @param list List to clear.
 */
CX_API void cx_list_clear(cx_list* list);

/**
 * Appends a copy of an element to the end of a list.
 *
 * @param list List to modify.
 * @param value Pointer to the element bytes to copy. Must not be NULL.
 *
 * @return true if insertion succeeds, false if an argument is invalid or
 *         memory allocation fails.
 *
 * On failure, the list remains unchanged.
 */
CX_API bool cx_list_push_back_ptr(cx_list* list, const void* value);

/**
 * Prepends a copy of an element to the beginning of a list.
 *
 * @param list List to modify.
 * @param value Pointer to the element bytes to copy. Must not be NULL.
 *
 * @return true if insertion succeeds, false if an argument is invalid or
 *         memory allocation fails.
 *
 * On failure, the list remains unchanged.
 */
CX_API bool cx_list_push_front_ptr(cx_list* list, const void* value);

/**
 * Appends a typed value to a list.
 *
 * @param list List to modify.
 * @param type Element type.
 * @param value Value to copy into the list.
 *
 * @return true if insertion succeeds, false otherwise.
 */
#define cx_list_push_back(list, type, value) \
    cx_list_push_back_ptr((list), &(type){(value)})

/**
 * Prepends a typed value to a list.
 *
 * @param list List to modify.
 * @param type Element type.
 * @param value Value to copy into the list.
 *
 * @return true if insertion succeeds, false otherwise.
 */
#define cx_list_push_front(list, type, value) \
    cx_list_push_front_ptr((list), &(type){(value)})

/**
 * Returns a mutable pointer to an element.
 *
 * @param list List to query.
 * @param index Zero-based index of the element.
 *
 * @return Pointer to the element, or NULL if list is NULL or index is
 *         out of bounds.
 *
 * The pointer is borrowed and becomes invalid when its node is removed,
 * or when the list is cleared or destroyed.
 */
CX_API void* cx_list_get(cx_list* list, size_t index);

/**
 * Returns a const pointer to an element.
 *
 * @param list List to query.
 * @param index Zero-based index of the element.
 *
 * @return Const pointer to the element, or NULL if list is NULL or index
 *         is out of bounds.
 *
 * The pointer follows the same lifetime rules as cx_list_get().
 */
CX_API const void* cx_list_get_const(const cx_list* list, size_t index);

/**
 * Removes an element from a list.
 *
 * @param list List to modify.
 * @param index Zero-based index of the element to remove.
 *
 * @return true if an element was removed, false if list is NULL or index
 *         is out of bounds.
 *
 * Pointers to the removed element become invalid.
 */
CX_API bool cx_list_remove(cx_list* list, size_t index);

/**
 * Creates a forward iterator over a list.
 *
 * @param list List to iterate over.
 *
 * @return Generic iterator, or an empty iterator if list is NULL or
 *         iterator creation fails.
 *
 * The iterator borrows the list. The list and its allocator must remain
 * valid until the iterator is destroyed.
 *
 * Structural modifications during iteration are not supported.
 * The caller remains responsible for destroying the iterator with
 * cx_iter_destroy().
 * Iterators must not be copied and independently destroyed.
 */
CX_API cx_iter cx_list_iter(const cx_list* list);

#ifdef __cplusplus
}
#endif

#endif
