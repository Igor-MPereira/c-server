#pragma once

#include <string.h>

/// @brief Hashes a string using sdbm algorithm.
/// http://www.cse.yorku.ca/~oz/hash.html
/// @param str string to be hashed
/// @return unsigned integer hash value
int hash(const char* str);