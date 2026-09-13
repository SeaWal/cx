#ifndef CX_ALLOCATOR_H
#define CX_ALLOCATOR_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "cx_core.h"

/*
* Allocator interface for custom memory management.
* Provides hooks for allocation/deallocation of memory with
* alignment support.
*/
typedef struct cx_allocator cx_allocator;

struct cx_allocator {
    /*
     * Allocates a block of memory
     *
     * @param size  Size in bytes.
     * @param align Alignment in bytes.
     * @return Pointer to allocated memory, or NULL on failure.
     */
    void* (*alloc)(size_t size, size_t align);

    /*
     * Deallocates a previously allocated block of memory
     * 
     * @param ptr  Pointer to the memory block.
     * @param size Size of the block (for debugging/tracking).
     * @param align Alignment of the block (for debugging/tracking).
     */
    void  (*dealloc)(void* ptr, size_t size, size_t align);
};

/*
 * Creates the default system allocator.
 * For simple, general-purpose use-cases.
 * 
 * @return  Allocator wrapper around malloc/free.
 * 
 */
CX_API cx_allocator cx_general_allocator(void);

#ifdef __cplusplus
}
#endif

#endif // CX_ALLOCATOR_H
