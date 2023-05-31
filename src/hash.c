#include "hash.h"

size_t hash(const char* str) {
  size_t hash = 0;

  for (size_t i = 0; i < strlen(str); i++) {
    hash = str[i] + (hash << 6) + (hash << 16) - hash;
  }

  return hash;
}