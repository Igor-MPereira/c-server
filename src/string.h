#pragma once

#include <string.h>
#include "types.h"

/// @brief Compares two strings.
/// @param s1 first string
/// @param s2 second string
/// @return true if the strings are equal, false otherwise
bool streq(const char* s1, const char* s2);

/// @brief Compares two strings up to n characters.
/// @param s1 first string
/// @param s2 second string
/// @param n number of characters to compare
/// @return true if the substrings are equal, false otherwise
bool strneq(const char* s1, const char* s2, size_t n);

/// @brief Replaces an array of characters in a string.
/// @param str string to be modified
/// @param toReplace list of characters to be replaced
/// @param replaceWith character to replace with
void strrep(char* str, const char* toReplace, const char replaceWith);

/// @brief Compares two strings ignoring case.
/// @param s1 first string
/// @param s2 second string
/// @return true if the strings are equal, false otherwise
bool strcaseeq(const char* s1, const char* s2);

/// @brief Compares two strings up to n characters ignoring case.
/// @param s1 first string
/// @param s2 second string
/// @param n number of characters to compare
/// @return true if the substrings are equal, false otherwise
bool strncaseeq(const char* s1, const char* s2, size_t n);

// if it's MSVC we need to implement strcasecmp and strncasecmp using _stricmp
// and _strnicmp
#ifdef _MSC_VER
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

#ifdef _WIN32

char* strcasestr(const char* haystack, const char* needle);

#endif
