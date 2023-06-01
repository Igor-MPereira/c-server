#pragma once

#include "headers.h"

typedef struct {
  int status_code;
  char status_text[16];
  Headers* headers;
  char* body;
} Response;

/// @brief Create a new response.
/// @return A pointer to the new response.
Response* response_new();

/// @brief Destroy and free the memory of the response.
/// @param response
void response_destroy(Response* response);

/// @brief Print the response.
/// @param response The response.
void response_print(Response* response);

/// @brief Parse a response from a string.
/// @param response The response.
/// @param string The string.
void response_parse(Response* response, char* string);

/// @brief Stringify a response to a string.
/// @param response The response.
/// @param string The string.
/// @param size The size of the string.
/// @return The string.
char* response_stringify(Response* response, char** string, size_t* size);

#define SIZEOF_VERSION 9
#define SIZEOF_STATUS_CODE 4
#define SIZEOF_NEWLINE 2

size_t response_size(Response* response, char** headers);

void response_set_body(Response* response, char* body);