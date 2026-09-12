#ifndef CX_TYPES_H
#define CX_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;

typedef float     f32;
typedef double    f64;

typedef bool      bool;

typedef size_t    size_t;
typedef size_t    usize;

typedef ptrdiff_t ptrdiff_t;
typedef ptrdiff_t isize;

typedef uint8_t   byte;

#ifdef __cplusplus
}
#endif

#endif // CX_TYPES_H
