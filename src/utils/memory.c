#include <utils/memory.h>

#include <stdio.h>
#include <string.h>

#include <utils/types.h>

static Allocation* allocations = null;
static bool trace = false;

void start_memtrace(const char* msg) {
  if (msg)
    printf("[MEMTRACE:START]  %s\n", msg);

  trace = true;
}

void stop_memtrace(const char* msg) {
  if (msg)
    printf("[MEMTRACE:END]  %s\n", msg);

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

void* _trace_realloc(void* ptr,
                     void* oldptr,
                     size_t size,
                     const char* file,
                     int line) {
  if (trace) {
    Allocation* a = allocations;

    while (a) {
      if (a->ptr == oldptr) {
        a->ptr = ptr;
        a->size = size;
        a->file = file;
        a->line = line;
        break;
      }

      a = a->next;
    }
  }

  return ptr;
}

void _trace_free(void* ptr) {
  if (trace) {
    Allocation* a = allocations;
    Allocation* prev = null;

    while (a) {
      if (a->ptr == ptr) {
        if (prev)
          prev->next = a->next;
        else
          allocations = a->next;

        free(a);
        break;
      }

      prev = a;
      a = a->next;
    }
  }

  free(ptr);
}

void memory_report(bool short_file) {
  if (!trace)
    return;

  Allocation* a = allocations;

  printf("Memory trace report: ");

  if (!a) {
    printf("No memory leaks!\n");
    return;
  }

  printf("\n");

  const char* file_label_space =
      short_file ? "           " : "                          ";

  printf(
      "Memory leaks detected\n\n"
      "|    pointer     "
      "|  size  "
      "| %sfile%s "
      "|\n",
      file_label_space, file_label_space);
  while (a) {
    printf("| %p | %6zu ", a->ptr, a->size);
    char* file = (char*)a->file + (short_file ? strlen(__ROOT_DIR__) + 1 : 0);
    char location[100] = {0};
    sprintf(location, "%s:%d", file, a->line);
    printf(short_file ? "| %-26s |\n" : "| %-56s |\n", location);

    a = a->next;
  }
}