#pragma once

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <utils/io.h>
#include <utils/types.h>

// create allocators and dealocators to keep track of memory usage and leaks
// using a static linked list of allocations
typedef struct Allocation {
  void* ptr;
  size_t size;
  const char* file;
  int line;
  struct Allocation* next;
} Allocation;

static Allocation* allocations = null;
static bool trace = false;

void start_memtrace() {
  trace = true;
}

void stop_memtrace() {
  trace = false;
}

void* _trace_alloc(void* ptr, size_t size, const char* file, int line) {
  if (trace) {
    Allocation* a = malloc(sizeof(Allocation));
    a->ptr = ptr;
    a->size = size;
    a->file = file;
    a->line = line;
    a->next = allocations;
    allocations = a;
  }

  return ptr;
}

void _trace_free(void* ptr) {
  if (trace) {
    Allocation* a = allocations;
    Allocation* prev = null;
    while (a != null) {
      if (a->ptr == ptr) {
        if (!prev)
          allocations = a->next;
        else
          prev->next = a->next;
        free(a);
        break;
      }
      prev = a;
      a = a->next;
    }
  }

  free(ptr);
}

void memory_report() {
  if (!trace)
    return;

  Allocation* a = allocations;
  printf("Memory trace report: ");

  if (!a)
    printf("No memory leaks!");

  printf("\n");

  printf(
      "Memory leak detected "
      "|      pointer       "
      "|    size    "
      "|            file             "
      "| line |\n");
  while (a) {
    printf(
        "                     "
        "| %p "
        "| %10zu "
        "| %28s "
        "| %4d |\n",
        a->ptr, a->size, a->file, a->line);

    a = a->next;
  }
}

#define memalloc(size) _trace_alloc(malloc(size), size, __FILE__, __LINE__)
#define memcalloc(count, size) \
  _trace_alloc(calloc(count, size), size, __FILE__, __LINE__)
#define memrealloc(ptr, size) \
  _trace_alloc(realloc(ptr, size), size, __FILE__, __LINE__)
#define memfree(ptr) _trace_free(ptr)
