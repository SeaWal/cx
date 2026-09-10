#ifndef CX_CORE_H
#define CX_CORE_H


#if defined(__GNUC__) || defined(__clang__)
  #define CX_API __attribute__((visibility("default")))
#else
  #define CX_API
#endif

#ifndef CX_UNUSED
#define CX_UNUSED(x) ((void)(x))
#endif


#endif // CX_CORE_H