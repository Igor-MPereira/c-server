#pragma once
/*
  AVOID INCLUDING THIS FILE DIRECTLY. THIS IS ONLY USED INTERNALLY.
  USE `json_parse` from `json/core.h` INSTEAD.
*/

#include <json/core.h>

void __json_parse(Json* json, char** s, size_t* position, bool is_root);
void __json_parse_object(JsonObject** json, char** s, size_t* position);
void __json_parse_array(Json* json, char** s, size_t* position, bool is_root);
void __json_parse_string(char** dest, char** s, size_t* position, bool is_root);
void __json_parse_number(f64* dest, char** s, size_t* position, bool is_root);
void __json_parse_boolean(Json* json, char** s, size_t* position, bool is_root);
void __json_parse_null(Json* json, char** s, size_t* position, bool is_root);