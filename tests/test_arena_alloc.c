#include <stdio.h>
#include "cx_allocator.h"

static bool arena_is_fully_init(cx_arena* arena, size_t init_cap) {
    if(arena == NULL
        || arena->buffer == NULL
        || arena->capacity != init_cap
        || arena->pos != 0) {
        return false;
    }
    
    return true;
}

static bool arena_is_destroyed(cx_arena* arena) {
    if(arena->buffer != NULL
        || arena->capacity != 0
        || arena->pos != 0) {
            return false;
        }

    return true;
}

static int test_arena_basic_alloc(void) {
    cx_arena arena;

    // try initialise the arena
    if(!cx_arena_init(&arena, 1024)) {
        printf("FAIL: failed to initialise arena.\n");
        return 1;
    }

    if(!arena_is_fully_init(&arena, 1024)) {
        cx_arena_destroy(&arena);
        return 1;
    }

    // get the allocator
    cx_allocator al = cx_arena_allocator(&arena);

    if(al.alloc == NULL || al.dealloc == NULL) {
        printf("FAIL: arena allocator alloc/dealloc are NULL.\n");
        cx_arena_destroy(&arena);
        return 1;
    }

    // test basic allocation
    int* value = cx_alloc(&al, sizeof(int), _Alignof(int));

    if (value == NULL) {
        printf("FAIL: arena allocation returned NULL\n");
        cx_arena_destroy(&arena);
        return 1;
    }

    // test memory is useable
    *value = 42;

    if (*value != 42) {
        printf("FAIL: arena memory is not usable.\n");
        cx_arena_destroy(&arena);
        return 1;
    }

    // test alignment is correct
    if ((uintptr_t)value % _Alignof(int) != 0) {
        printf("FAIL: allocation is incorrectly aligned\n");
        cx_arena_destroy(&arena);
        return 1;
    }

    cx_arena_destroy(&arena);
    if(!arena_is_destroyed(&arena)) {
        return 1;
    }
    
    // success
    return 0;
}


static int test_arena_multiple_alloc(void) {
    cx_arena arena;

    // try initialise the arena
    if(!cx_arena_init(&arena, 1024)) {
        printf("FAIL: failed to initialise arena.\n");
        return 1;
    }

    if(!arena_is_fully_init(&arena, 1024)) {
        cx_arena_destroy(&arena);
        return 1;
    }

    // get the allocator
    cx_allocator al = cx_arena_allocator(&arena);

    // test multiple allocations
    int* first = cx_alloc(&al, sizeof(int), _Alignof(int));
    int* second = cx_alloc(&al, sizeof(int), _Alignof(int));

    if (first == NULL || second == NULL) {
        printf("FAIL: multiple arena allocations failed\n");
        cx_arena_destroy(&arena);
        return 1;
    }

    if (first == second) {
        printf("FAIL: separate arena allocations returned the same pointer\n");
        cx_arena_destroy(&arena);
        return 1;
    }

    cx_arena_destroy(&arena);
    if(!arena_is_destroyed(&arena)) {
        return 1;
    }
    
    // success
    return 0;
}

static int test_arena_small_capacity(void) {
    cx_arena small_arena;

    if (!cx_arena_init(&small_arena, 16)) {
        printf("FAIL: small arena initialization failed\n");
        return 1;
    }

    if(!arena_is_fully_init(&small_arena, 16)) {
        cx_arena_destroy(&small_arena);
        return 1;
    }

    cx_allocator al = cx_arena_allocator(&small_arena);

    // test alloc a block same size as arena cap
    void* block = cx_alloc(&al, 16, 1);
    
    if(block == NULL) {
        printf("FAIL: allocation matching capacity failed\n");
        cx_arena_destroy(&small_arena);
        return 1;
    }

    // test overflow capacity
    void* overflow = cx_alloc(&al, 1, 1);

    if(overflow != NULL) {
        printf("FAIL: arena allowed allocation beyond capacity.\n");
        cx_arena_destroy(&small_arena);
        return 1;
    }

    cx_arena_destroy(&small_arena);
    if(!arena_is_destroyed(&small_arena)) {
        return 1;
    }
    
    // success
    return 0;
}


int main(void) {
    if(test_arena_basic_alloc() != 0) {
        return 1;
    }

    if(test_arena_multiple_alloc() != 0) {
        return 1;
    }

    if(test_arena_small_capacity() != 0) {
        return 1;
    }
}
