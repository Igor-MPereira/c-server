#pragma once

#include <utils/types.h>

// PARSING UTILS

bool is_ws(char c);
char skip_ws(char** s, size_t* position);
char peek(char** s);
char next(char** s, size_t* position);

// MEMORY UTILS

void json_object_free(JsonObject* object);
void json_array_free(JsonArray* array);