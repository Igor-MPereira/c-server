#pragma once

#define array_count(array) (sizeof(array) / sizeof(array[0]))

#define kB(value) ((value)*1024LL)
#define mB(value) (kB(value) * 1024LL)
#define gB(value) (mB(value) * 1024LL)

#define REQ_MAXSIZE kB(16)

#if !defined(__UNUSED_PARAM)
#define __UNUSED_PARAM(P) P __attribute__((unused))
#endif  // __UNUSED_PARAM

#define UNUSED(P) __UNUSED_PARAM(P)
