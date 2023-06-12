#pragma once

#include <json/core.h>

void json_parse(Json* json, char** s, size_t* position);
void json_parse_object(JsonObject** json, char** s, size_t* position);
void json_parse_array(Json* json, char** s, size_t* position);
void json_parse_string(char** dest, char** s, size_t* position);
void json_parse_number(Json* json, char** s, size_t* position);
void json_parse_boolean(Json* json, char** s, size_t* position);
void json_parse_null(Json* json, char** s, size_t* position);