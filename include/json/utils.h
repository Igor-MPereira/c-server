#pragma once

#include <utils/types.h>

bool isspace(char c);

char skip_ws(char** s, size_t* position);

char peek(char* s);

char next(char** s, size_t* position);