#include <stdlib.h>
#include <string.h>

#include "cx_allocator.h"
#include "cx_core.h"
#include "cx_types.h"
#include "cx_vector.h"

#define CX_VECTOR_INITIAL_CAPACITY 8
#define CX_VECTOR_GROWTH_FACTOR 2

struct cx_vector {
    cx_allocator allocator;
    u8* data;
    size_t length;
    size_t capacity;
    size_t elem_size;
    size_t elem_align;
};

// TODO: move to util file?
/*
 * Multiplies two size_t values while checking for overflow.
 *
 * @param a First value.
 * @param b Second value.
 * @param result Pointer to output value.
 * @return true on success, false if the multiplication would overflow.
 */
static bool cx_checked_mul(size_t a, size_t b, size_t* result) {
    if(a != 0 && b > SIZE_MAX / a) {
        return false;
    }

    *result = a * b;
    return true;
}

/*
 * Returns a suitable larger capacity for a vector.
 *
 * The capacity is doubled until it can hold the required number of
 * elements. If doubling would overflow, the required capacity is used.
 *
 * @param vector Vector whose current capacity is used as the starting point.
 * @param required Minimum required capacity.
 * @return New capacity.
 */
static size_t cx_vector_grow_capacity(const cx_vector* vector, size_t required) {
    size_t cap = vector->capacity;

    if(cap == 0) {
        cap = CX_VECTOR_INITIAL_CAPACITY;
    }

    while(cap < required) {
        if(cap > SIZE_MAX / CX_VECTOR_GROWTH_FACTOR) {
            return required;
        }

        cap *= CX_VECTOR_GROWTH_FACTOR;
    }

    return cap;
}

/*
 * Resizes the vector's backing storage.
 *
 * @param vector Vector to resize.
 * @param new_capacity New capacity in elements.
 * @return true on success, false on allocation failure or size overflow.
 */
static bool cx_vector_resize_storage(cx_vector* vector, size_t new_capacity) {
    size_t old_size = vector->capacity * vector->elem_size;
    
    size_t new_size;
    if (!cx_checked_mul(new_capacity, vector->elem_size, &new_size)) {
        return false;
    }

    u8* data = cx_realloc(
        &vector->allocator,
        vector->data,
        old_size,
        new_size,
        vector->elem_align
    );

    if (data == NULL) {
        return false;
    }

    vector->data = data;
    vector->capacity = new_capacity;

    return true;
}

CX_API cx_vector* cx_vector_create_raw(cx_allocator* allocator, size_t element_size, size_t element_align) {
    if(allocator == NULL || element_size == 0 || element_align == 0) {
        return NULL;
    }

    cx_vector* vector = cx_alloc(allocator, sizeof(cx_vector), _Alignof(cx_vector));

    if(vector == NULL) {
        return NULL;
    }

    *vector = (cx_vector) {
        .allocator = *allocator,
        .data = NULL,
        .length = 0,
        .capacity = 0,
        .elem_size = element_size,
        .elem_align = element_align,
    };

    return vector;
}

CX_API void cx_vector_destroy(cx_vector* vector) {
    if(vector == NULL) {
        return;
    }

    if(vector->data != NULL) {
        size_t data_size = vector->capacity * vector->elem_size;
        cx_dealloc(&vector->allocator, vector->data, data_size, vector->elem_align);
    }

    cx_dealloc(&vector->allocator, vector, sizeof(cx_vector), _Alignof(cx_vector));
}

CX_API size_t cx_vector_length(const cx_vector* vector) {
    if(vector == NULL) {
        return 0;
    }

    return vector->length;
}

CX_API size_t cx_vector_capacity(const cx_vector* vector) {
    if(vector == NULL) {
        return 0;
    }

    return vector->capacity;
}

CX_API bool cx_vector_is_empty(const cx_vector* vector) {
    return vector == NULL || vector->length == 0;
}

CX_API bool cx_vector_push_ptr(cx_vector* vector, const void* value) {
    if (vector == NULL || value == NULL) {
        return false;
    }

    if(vector->length == vector->capacity) {
        if(vector->length == SIZE_MAX) {
            return false;
        }

        size_t required = vector->length + 1;
        size_t new_capacity = cx_vector_grow_capacity(vector, required);

        if(!cx_vector_resize_storage(vector, new_capacity)) {
            return false;
        }
    }

    size_t offset;
    if(!cx_checked_mul(vector->length, vector->elem_size, &offset)) {
        return false;
    }

    memcpy(vector->data + offset, value, vector->elem_size);
    vector->length++;

    return true;
}

CX_API void* cx_vector_get(cx_vector* vector, size_t index) {
    if(vector == NULL || index >= vector->length) {
        return NULL;
    }

    size_t offset;
    if(!cx_checked_mul(index, vector->elem_size, &offset)) {
        return NULL;
    }

    return vector->data + offset;
}

CX_API const void* cx_vector_get_const(const cx_vector* vector, size_t index) {
    if (vector == NULL || index >= vector->length) {
        return NULL;
    }

    size_t offset;

    if (!cx_checked_mul(index, vector->elem_size, &offset)) {
        return NULL;
    }

    return vector->data + offset;
}

CX_API void cx_vector_clear(cx_vector* vector) {
    if(vector == NULL) {
        return;
    }

    vector->length = 0;
}

CX_API bool cx_vector_reserve(cx_vector* vector, size_t capacity) {
    if(vector == NULL) {
        return false;
    }

    // vector already has enough capacity
    if(capacity <= vector->capacity) {
        return true;
    }

    return cx_vector_resize_storage(vector, capacity);
}

CX_API bool cx_vector_shrink_to_fit(cx_vector* vector) {
    if(vector == NULL) {
        return false;
    }

    // vector already small enough
    if(vector->length == vector->capacity) {
        return true;
    }

    return cx_vector_resize_storage(vector, vector->length);
}

CX_API bool cx_vector_pop(cx_vector* vector, void* out_value) {
    if(vector == NULL || vector->length == 0) {
        return false;
    }

    size_t index = vector->length - 1;
    size_t offset;
    if(!cx_checked_mul(index, vector->elem_size, &offset)) {
        return false;
    }

    if(out_value != NULL) {
        memcpy(out_value, vector->data + offset, vector->elem_size);
    }

    vector->length--;
    return true;
}

CX_API bool cx_vector_remove(cx_vector* vector, size_t index) {
    if (vector == NULL || index >= vector->length) {
        return false;
    }

    size_t offset;
    if (!cx_checked_mul(index, vector->elem_size, &offset)) {
        return false;
    }

    size_t elements_after = vector->length - index - 1;

    if (elements_after > 0) {
        size_t bytes_after;

        if (!cx_checked_mul(elements_after, vector->elem_size, &bytes_after)) {
            return false;
        }

        memmove(
            vector->data + offset,
            vector->data + offset + vector->elem_size,
            bytes_after
        );
    }

    vector->length--;

    return true;
}

CX_API bool cx_vector_insert_ptr(cx_vector* vector, size_t index, const void* value) {
    if (vector == NULL || value == NULL || index > vector->length) {
        return false;
    }

    if (vector->length == SIZE_MAX) {
        return false;
    }

    size_t required = vector->length + 1;

    if (required > vector->capacity) {
        size_t new_capacity = cx_vector_grow_capacity(vector, required);

        if (!cx_vector_resize_storage(vector, new_capacity)) {
            return false;
        }
    }

    size_t offset;
    if (!cx_checked_mul(index, vector->elem_size, &offset)) {
        return false;
    }

    size_t elements_after = vector->length - index;

    if (elements_after > 0) {
        size_t bytes_after;

        if (!cx_checked_mul(elements_after, vector->elem_size, &bytes_after)) {
            return false;
        }

        memmove(
            vector->data + offset + vector->elem_size,
            vector->data + offset,
            bytes_after
        );
    }

    memcpy(vector->data + offset, value, vector->elem_size);

    vector->length++;

    return true;
}
