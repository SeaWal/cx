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
     * @param ctx Allocator-specific context data.
     * @param size Size in bytes.
     * @param align Alignment in bytes.
     * @return Pointer to allocated memory, or NULL on failure.
     */
    void* (*alloc)(void* ctx, size_t size, size_t align);

    /*
     * Deallocates a previously allocated block of memory
     * 
     * @param ctx Allocator-specific context data.
     * @param ptr Pointer to the memory block.
     * @param size Size of the block (for debugging/tracking).
     * @param align Alignment of the block (for debugging/tracking).
     */
    void  (*dealloc)(void* ctx, void* ptr, size_t size, size_t align);

    /*
    * Reallocates a previously allocated block of memory.
    *
    * The contents of the original block are preserved up to the smaller
    * of old_size and new_size.
    *
    * If reallocation fails, the original allocation remains valid.
    *
    * @param ctx Allocator-specific context data.
    * @param ptr Pointer to the existing allocation, or NULL.
    * @param old_size Size of the existing allocation in bytes.
    * @param new_size Size of the requested allocation in bytes.
    * @param align Required alignment of the allocation.
    * @return Pointer to the resized allocation, or NULL on failure.
    */
    void* (*realloc)(void* ctx, void* ptr, size_t old_size, size_t new_size, size_t align);

    /*
     * Allocato-specific context data.
     *
     * Used by allocator to access any internal state.
     */
    void* ctx;
};

/*
 * Generic interface for allocation in cx.
 *
 * Calls the alloc function of the given allocator
 * like `allocator->alloc(allocator->ctx, size, align)`
 * 
 * @param allocator The allocator to use for getting memory.
 * @param size Size of the block in bytes.
 * @param align Alignment of the block.
 * @return Pointer to allocated memory, or NULL on failure.
 */
CX_API void* cx_alloc(cx_allocator* allocator, size_t size, size_t align);

/*
 * Generic interface for deallocation in cx.
 *
 * Calls the dealloc function of the given allocator
 * like `allocator->dealloc(allocator->ctx, ptr, size, align)`
 * 
 * @param ptr Pointer to the memory block.
 * @param size Size of the block (for debugging/tracking).
 * @param align Alignment of the block (for debugging/tracking).
 */
CX_API void cx_dealloc(cx_allocator* allocator, void* ptr, size_t size, size_t align);

/*
 * Reallocates a previously allocated block of memory.
 *
 * The contents of the original block are preserved up to the smaller
 * of old_size and new_size.
 *
 * If reallocation fails, the original allocation remains valid.
 *
 * Passing NULL as ptr behaves like cx_alloc().
 *
 * Passing zero as new_size releases the allocation and returns NULL.
 *
 * @param allocator The allocator to use.
 * @param ptr Pointer to the existing allocation, or NULL.
 * @param old_size Size of the old allocation in bytes.
 * @param new_size Size of the new allocation in bytes.
 * @param align  Alignment of the allocation.
 * @return Pointer to the resized allocation, or NULL on failure.
 */

CX_API void* cx_realloc(cx_allocator* allocator, void* ptr, size_t old_size, size_t new_size, size_t align);

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
