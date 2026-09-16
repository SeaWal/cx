#ifndef CX_ALLOCATOR_H
#define CX_ALLOCATOR_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "cx_core.h"
#include "cx_types.h"

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
     * @param ptr   Pointer to the memory block.
     * @param size  Size of the block (for debugging/tracking).
     * @param align Alignment of the block (for debugging/tracking).
     */
    void  (*dealloc)(void* ptr, size_t size, size_t align);
};

/*
 * Creates the default system allocator.
 * For simple, general-purpose use-cases.
 * 
 * @return Allocator wrapper around malloc/free. 
 */
CX_API cx_allocator cx_general_allocator(void);


/*
 * Arena allocator.
 *
 * Allocates memory linearly from a buffer.
 * Individual allocations can't be deallocated.
 * Memory is reclaimed when the arena is destroyed.
 */
typedef struct cx_arena cx_arena;

struct cx_arena {
    cx_allocator allocator;

    u8* buffer;
    size_t capacity;
    size_t pos;
};

/*
 * Initialises an arena with the specified capacity.
 * 
 * The arena allocates its buffer from the heap.
 * 
 * @param arena Arena to initialise.
 * @param capacity Size of the buffer in bytes.
 * @return true on success, false on failure to allocate.
 */
CX_API bool cx_arena_init(cx_arena* arena, size_t capacity);

/*
 * Returns the allocator interface for the given arena.
 * 
 * @param arena The arena to create an allocator for.
 * @return Allocator backed by the arena.
 */
CX_API cx_allocator cx_arena_allocator(cx_arena* arena);

/*
 * Resets the arena.
 * 
 * All memory previously allocated from the arena becomes invalid.
 * 
 * @param arena The arena to reset.
 */
CX_API void cx_arena_reset(cx_arena* arena);

/*
 * Destroys the arena.
 *
 * Releases the underlying buffer and invalidates all allocations
 * made from the arena.
 * 
 * @param arena The arena to destroy.
 */
CX_API void cx_arena_destroy(cx_arena* arena);

#ifdef __cplusplus
}
#endif

#endif // CX_ALLOCATOR_H
