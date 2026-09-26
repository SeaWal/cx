#include <stdint.h>

#include "utils.h"

bool cx_checked_mul(size_t a, size_t b, size_t* result) {
    if(a != 0 && b > SIZE_MAX / a) {
        return false;
    }

    *result = a * b;
    return true;
}