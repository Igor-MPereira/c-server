#pragma once

#include <stdlib.h>

#include <utils/types.h>

typedef struct Allocation Allocation;
struct Allocation {
  void* ptr;
  size_t size;
  const char* file;
  int line;
  Allocation* next;
};

void start_memtrace(const char* msg);
void stop_memtrace(const char* msg);

void* _trace_alloc(void* ptr, size_t size, const char* file, int line);
void* _trace_realloc(void* ptr,
                     void* oldptr,
                     size_t size,
                     const char* file,
                     int line);
void _trace_free(void* ptr);

void memory_report(bool short_filename);

#define memalloc(size) _trace_alloc(malloc(size), size, __FILE__, __LINE__)
#define memcalloc(count, size) \
  _trace_alloc(calloc(count, size), size, __FILE__, __LINE__)
#define memrealloc(ptr, size) \
  _trace_realloc(realloc(ptr, size), ptr, size, __FILE__, __LINE__)
#define memfree(ptr) _trace_free(ptr)
