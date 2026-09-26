#ifndef CX_UTILS_H
#define CX_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

/*
 * Multiplies two size_t values while checking for overflow.
 *
 * @param a First value.
 * @param b Second value.
 * @param result Pointer to output value.
 * @return true on success, false if the multiplication would overflow.
 */
bool cx_checked_mul(size_t a, size_t b, size_t* result) ;

#ifdef __cplusplus
}
#endif


#endif // CX_UTILS_H
