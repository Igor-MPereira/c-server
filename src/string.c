#include "string.h"

/// @brief Compare two strings.
/// @param s1 The first string.
/// @param s2 The second string.
/// @return true if the strings are equal, false otherwise.
bool streq(const char* s1, const char* s2) {
  return strcmp(s1, s2) == 0;
}

/// @brief Compare two strings up to n characters.
/// @param s1 The first string.
/// @param s2 The second string.
/// @param n The number of characters to compare.
/// @return true if the strings are equal, false otherwise.
bool strneq(const char* s1, const char* s2, size_t n) {
  return strncmp(s1, s2, n) == 0;
}

/// @brief Replace all occurrences of a character in a string.
/// @param str The string to modify.
/// @param toReplace The character to replace.
/// @param replaceWith The character to replace with.
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

/// @brief Case insensitive string comparison.
/// @param s1 The first string.
/// @param s2 The second string.
/// @return true if the strings are equal, false otherwise.
bool strcaseeq(const char* s1, const char* s2) {
  return strcasecmp(s1, s2) == 0;
}

/// @brief Case insensitive string comparison up to n characters.
/// @param s1  The first string.
/// @param s2  The second string.
/// @param n  The number of characters to compare.
/// @return true if the strings are equal, false otherwise.
bool strncaseeq(const char* s1, const char* s2, size_t n) {
  return strncasecmp(s1, s2, n) == 0;
}

/// @brief Case insensitive string search.
/// @param haystack The string to search in.
/// @param needle The string to search for.
/// @return A pointer to the first occurrence of needle in haystack, or null if
/// not found.
char* strcasestr(const char* haystack, const char* needle) {
  size_t len = strlen(needle);

  while (*haystack) {
    if (strncasecmp(haystack, needle, len) == 0)
      return (char*)haystack;

    haystack++;
  }

  return null;
}