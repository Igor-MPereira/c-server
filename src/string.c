#include "string.h"

bool streq(const char* s1, const char* s2) {
  return strcmp(s1, s2) == 0;
}

bool strneq(const char* s1, const char* s2, size_t n) {
  return strncmp(s1, s2, n) == 0;
}

void strrep(char* str, const char* toReplace, const char replaceWith) {
  size_t len = strlen(str);
  size_t toReplaceLen = strlen(toReplace);

  for (size_t i = 0; i < len; i++)
    for (size_t j = 0; j < toReplaceLen; j++)
      if (str[i] == toReplace[j]) {
        str[i] = replaceWith;
        break;
      }

  return str;
}