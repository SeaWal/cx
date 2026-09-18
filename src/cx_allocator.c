#include <stdlib.h>

#include "cx_allocator.h"
#include "cx_core.h"

void* cx_alloc(cx_allocator* allocator, size_t size, size_t align) {
    if(allocator == NULL || allocator->alloc == NULL) {
        return NULL;
    }

    return allocator->alloc(allocator->ctx, size, align);
}

void cx_dealloc(cx_allocator* allocator, void* ptr, size_t size, size_t align) {
    if(allocator == NULL || allocator->dealloc == NULL) {
        return;
    }

    allocator->dealloc(allocator->ctx, ptr, size, align);
}

static void* cx_general_alloc(void* ctx, size_t size, size_t align) {
    CX_UNUSED(ctx);
    CX_UNUSED(align);
    return malloc(size);
}

static void cx_general_dealloc(void* ctx, void* ptr, size_t size, size_t align) {
    CX_UNUSED(ctx);
    CX_UNUSED(size);
    CX_UNUSED(align);
    free(ptr);
}

CX_API cx_allocator cx_general_allocator(void) {
    return (cx_allocator) {
        .alloc = cx_general_alloc,
        .dealloc = cx_general_dealloc,
        .ctx = NULL,
    };
}

static bool is_power_of_two(size_t value) {
    return value != 0 && (value & (value - 1)) == 0;
}

static bool align_forward(size_t value, size_t align, size_t* result) {
    if(!is_power_of_two(align)) {
        return false;
    }

    size_t mask = align - 1;

    if(value > SIZE_MAX - mask) {
        return false;
    }

    *result = (value + mask) & ~mask;
    return true;
}

static void* cx_arena_alloc(void* ctx, size_t size, size_t align) {
    cx_arena* arena = ctx;
    if(arena == NULL || size == 0) {
        return NULL;
    }

    size_t position;
    if(!align_forward(arena->pos, align, &position)) {
        return NULL;
    }

    if(position > arena->capacity) {
        return NULL;
    }

    if(size > arena->capacity - position) {
        return NULL;
    }

    void* ptr = arena->buffer + position;

    arena->pos = position + size;

    return ptr;
}

// dealloc not defined for individual deallocations with arena
static void cx_arena_dealloc(void* ctx, void* ptr, size_t size, size_t align) {
    CX_UNUSED(ctx);
    CX_UNUSED(ptr);
    CX_UNUSED(size);
    CX_UNUSED(align);
}

CX_API bool cx_arena_init(cx_arena* arena, size_t capacity) {
    if(arena == NULL || capacity == 0) {
        return false;
    }

    u8* buffer = malloc(capacity);
    if(buffer == NULL) {
        return false;
    }

    arena->buffer = buffer;
    arena->capacity = capacity;
    arena->pos = 0;

    arena->allocator = (cx_allocator) {
        .alloc = cx_arena_alloc,
        .dealloc = cx_arena_dealloc,
        .ctx = arena,
    };

    return true;
}

CX_API cx_allocator cx_arena_allocator(cx_arena* arena) {
    if(arena == NULL) {
        return (cx_allocator) { 0 };
    }

    return arena->allocator;
}

CX_API void cx_arena_reset(cx_arena* arena) {
    if(arena == NULL) {
        return;
    }

    arena->pos = 0;
}


CX_API void cx_arena_destroy(cx_arena* arena) {
    if(arena == NULL) {
        return;
    }

    free(arena->buffer);
    arena->buffer = NULL;
    arena->capacity = 0;
    arena->pos = 0;
    arena->allocator = (cx_allocator) { 0 };
}