#pragma once

#include <json/core/functions.h>
#include <json/core/types.h>
#include <json/exception.h>

Json* json_new();
void json_free(Json* json);

void json_parse(Json* json, char* s);
