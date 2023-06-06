#pragma once

#define array_count(array) (sizeof(array) / sizeof(array[0]))

#define kB(value) ((value)*1024LL)
#define mB(value) (kB(value) * 1024LL)
#define gB(value) (mB(value) * 1024LL)

#define REQ_MAXSIZE kB(16)