#include "string.h"

bool streq(const char* s1, const char* s2) {
  return strcmp(s1, s2) == 0;
}

bool strneq(const char* s1, const char* s2, size_t n) {
  return strncmp(s1, s2, n) == 0;
}

bool strdiff(const char* s1, const char* s2) {
  return !streq(s1, s2);
}

bool strstarts(const char* str, const char* start) {
  size_t len = strlen(start);

  return strneq(str, start, len);
}

bool strcasestarts(const char* str, const char* start) {
  size_t len = strlen(start);

  return strncaseeq(str, start, len);
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
}

bool strcaseeq(const char* s1, const char* s2) {
  return strcasecmp(s1, s2) == 0;
}

bool strncaseeq(const char* s1, const char* s2, size_t n) {
  return strncasecmp(s1, s2, n) == 0;
}

char* strcasestr(const char* haystack, const char* needle) {
  size_t len = strlen(needle);

  while (*haystack) {
    if (strncasecmp(haystack, needle, len) == 0)
      return (char*)haystack;

    haystack++;
  }

  return null;
}