/*
From Anton Zhiyanov's "Simplified GCC/Clang" implementation of the "defer" concept

See: https://antonz.org/defer-in-c/


Example usage:

#include <cx/cx_defer>

int main() {
    int* p = malloc(sizeof(int));
    if(!p) return 1;
    cx_defer(free, p);
}
*/

#ifndef CX_DEFER_H
#define CX_DEFER_H

#ifdef __cplusplus
extern "C" {
#endif

#define _DEFER_CONCAT(a, b) a##b
#define _DEFER_NAME(a, b) _DEFER_CONCAT(a, b)

// Deferred function and its argument.
struct _defer_ctx {
    void (*fn)(void*);
    void* arg;
};

// Calls the deferred function with its argument.
static inline void _defer_cleanup(struct _defer_ctx* ctx) {
    if (ctx->fn) ctx->fn(ctx->arg);
}

// Create a deferred function call for the current scope.
#define cx_defer(fn, ptr)                                      \
    struct _defer_ctx _DEFER_NAME(_defer_var_, __COUNTER__) \
        __attribute__((cleanup(_defer_cleanup))) =          \
            {(void (*)(void*))(fn), (void*)(ptr)}

#ifdef __cplusplus
}
#endif


#endif // CX_DEFER_H
