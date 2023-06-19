#pragma once

#include <json/core/types.h>

void json_str(char* dest, Json* json);
void json_pstr(char** dest, Json* json);

void json_num(f64* dest, Json* json);
void json_pnum(f64** dest, Json* json);
void json_int(i32* dest, Json* json);
void json_pint(i32** dest, Json* json);

void json_bool(bool* dest, Json* json);
void json_pbool(bool** dest, Json* json);

void json_obj(JsonObject* dest, Json* json);
void json_pobj(JsonObject** dest, Json* json);

void json_arr(JsonArray* dest, Json* json);
void json_parr(JsonArray** dest, Json* json);

void json_get(Json* dest, Json* json, char* key);

void json_get_str(char** dest, Json* json, char* key);
void json_get_num(f64* dest, Json* json, char* key);
void json_get_bool(bool* dest, Json* json, char* key);
void json_get_obj(JsonObject** dest, Json* json, char* key);
void json_get_arr(JsonArray** dest, Json* json, char* key);

void json_length(Json* json, size_t* dest);

void json_at(Json* dest, Json* json, size_t index);

void json_str_at(char** dest, Json* json, size_t index);
void json_num_at(f64* dest, Json* json, size_t index);
void json_bool_at(bool* dest, Json* json, size_t index);
void json_obj_at(JsonObject** dest, Json* json, size_t index);
void json_arr_at(JsonArray** dest, Json* json, size_t index);