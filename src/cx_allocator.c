#include <stdlib.h>

#include "cx_allocator.h"
#include "cx_core.h"

static void* cx_general_alloc(size_t size, size_t align) {
    CX_UNUSED(align);
    return malloc(size);
}

static void cx_general_dealloc(void* ptr, size_t size, size_t align) {
    CX_UNUSED(size);
    CX_UNUSED(align);
    free(ptr);
}

CX_API cx_allocator cx_general_allocator(void) {
    return (cx_allocator) {
        .alloc = cx_general_alloc,
        .dealloc = cx_general_dealloc,
    };
}
