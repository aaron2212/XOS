#ifndef _STDINT_H
#define _STDINT_H

/* limits of types */
#define INTMAX_MIN -2147483647  // -((2**31)-1)
#define INTMAX_MAX  2147483647  // (2**31)-1
#define UINTMAX_MAX NNN

#define SIZE_MAX 	4294967295 	// (2**32)-1

/* signed integral types */
typedef char  int8_t;
typedef short int16_t;
typedef int   int32_t;
typedef long  int64_t;
typedef long  intmax_t;

/* unsigned integral types */
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef unsigned long  uint64_t;
typedef unsigned long  uintmax_t;

#endif
