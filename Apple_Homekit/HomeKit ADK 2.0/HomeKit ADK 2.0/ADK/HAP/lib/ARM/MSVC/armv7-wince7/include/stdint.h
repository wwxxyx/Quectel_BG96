#ifndef STDINT_H
#define STDINT_H

#include <limits.h>

// See here: https://msdn.microsoft.com/en-us/library/mt764276.aspx?f=255&MSPPError=-2147217396
// https://msdn.microsoft.com/en-us/library/29dh1w7z.aspx
typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#define INT8_MIN   SCHAR_MIN
#define INT8_MAX   SCHAR_MAX
#define UINT8_MAX  UCHAR_MAX
#define INT16_MIN  SHRT_MIN
#define INT16_MAX  SHRT_MAX
#define UINT16_MAX USHRT_MAX
#define INT32_MIN  INT_MIN
#define INT32_MAX  INT_MAX
#define UINT32_MAX UINT_MAX
#define INT64_MIN  LLONG_MIN
#define INT64_MAX  LLONG_MAX
#define UINT64_MAX ULLONG_MAX

#ifndef SIZE_MAX
    #ifndef _WIN64
        #define SIZE_MAX UINT32_MAX
    #else
        #define SIZE_MAX UINT64_MAX
    #endif
#endif

#endif
