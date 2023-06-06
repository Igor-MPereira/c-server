#include <utils/hash.h>

#include <string.h>

u32 hash(const char* str) {
  u32 hash = 0;
  size_t len = strlen(str);

  for (size_t i = 0; i < len; i++)
    hash = str[i] + (hash << 6) + (hash << 16) - hash;

  return hash;
}