#pragma once

#include "types.h"
#define HEADER_MAX_SIZE kB(4)
typedef struct Header {
  char* key;
  char* value;
  struct Header* next;
  struct Header* prev;
} Header;

typedef struct {
  Header* first;
  Header* last;
} Headers;

/// @brief Create a new headers.
/// @return A pointer to the new headers.
Headers* headers_new();

/// @brief Destroy and free the memory of the headers.
/// @param headers
void headers_destroy(Headers* headers);

/// @brief Add a new header to the headers.
/// @param headers The headers.
/// @param key The key of the header.
/// @param value The value of the header.
void headers_add(Headers* headers, const char* key, const char* value);

/// @brief Remove a header from the headers.
/// @param headers The headers.
/// @param key The key of the header.
void headers_remove(Headers* headers, char* key);

/// @brief Get a header from the headers.
/// @param headers The headers.
/// @param key The key of the header.
/// @return The value of the header.
char* headers_get(Headers* headers, char* key);

/// @brief Check if the headers has a header.
/// @param headers The headers.
/// @param key The key of the header.
/// @return true if the headers has the header, false otherwise.
bool headers_has(Headers* headers, char* key);

/// @brief Set a header in the headers.
/// @param headers The headers.
/// @param key The key of the header.
/// @param value The value of the header.
void headers_set(Headers* headers, char* key, char* value);

/// @brief Print the headers.
/// @param headers The headers.
void headers_print(Headers* headers);

/// @brief Print single header.
/// @param header The header.
void header_print(Header* header);

/// @brief Stringify the headers to a string.
/// @param headers The headers.
/// @return The string.
char* headers_stringify(Headers* headers);

/// @brief Parse a string to headers.
/// @param headers The headers.
/// @param ctx The string.
void headers_parse(Headers* headers, char** ctx);

const char* mime_type(const char* path);