#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef float f32;
typedef double f64;
typedef long double f128;

typedef int8_t byte;
typedef uint8_t ubyte;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define null 0
#define null_ptr ((void*)0)

#define array_count(array) (sizeof(array) / sizeof(array[0]))

#define kB(value) ((value)*1024LL)
#define mB(value) (kB(value) * 1024LL)

#define assert(expression, message, ...)                                      \
  if (!(expression)) {                                                        \
    printf("Assertion failed: %s, line %d, file %s\n", #expression, __LINE__, \
           __FILE__);                                                         \
    printf("Message: " message "\n", __VA_ARGS__);                            \
    *(int*)0 = 0;                                                             \
  }

#define assert_static(expression)                \
  do {                                           \
    enum { assert_static__ = 1 / (expression) }; \
  } while (0)

#define assert_not_null(expression) assert(expression != null)
#define assert_null(expression) assert(expression == null)
#define assert_equal(expression, value) assert(expression == value)
#define assert_not_equal(expression, value) assert(expression != value)
#define assert_greater(expression, value) assert(expression > value)
#define assert_greater_equal(expression, value) assert(expression >= value)
#define assert_less(expression, value) assert(expression < value)
#define assert_less_equal(expression, value) assert(expression <= value)
#define assert_fail(message, ...) assert(false, message, __VA_ARGS__)
#define assert_unreachable() assert_fail("Unreachable code")
#define assert_not_implemented() assert_fail("Not implemented")