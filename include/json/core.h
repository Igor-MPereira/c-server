#pragma once

#include <json/exception.h>
#include <utils/types.h>

typedef enum JsonType {
  JSON_TYPE_OBJECT,
  JSON_TYPE_ARRAY,
  JSON_TYPE_STRING,
  JSON_TYPE_NUMBER,
  JSON_TYPE_BOOLEAN,
  JSON_TYPE_NULL
} JsonType;

#define OBJECT OBJECT
#define ARRAY ARRAY
#define STRING STRING
#define NUMBER NUMBER
#define BOOLEAN BOOLEAN

#define json_t(X) JSON_TYPE_##X

typedef struct Json Json;
typedef struct JsonObject JsonObject;

struct JsonObject {
  char* key;
  Json* value;
  JsonObject* next;
};

typedef struct JsonArray {
  Json** items;
  size_t length;
} JsonArray;

typedef union JsonValue {
  JsonObject* object;
  JsonArray* array;
  char* string;
  f64 number;
  bool boolean;
} JsonValue;

struct Json {
  JsonType type;
  JsonValue value;
};

Json* json_new();
void json_free(Json* json);

void json_parse(Json* json, char* s);

void json_str(char** dest, Json* json);
void json_num(f64* dest, Json* json);
void json_bool(bool* dest, Json* json);
void json_obj(JsonObject** dest, Json* json);
void json_arr(JsonArray** dest, Json* json);

void json_get(Json* dest, Json* json, char* key);
void json_at(Json* dest, Json* json, size_t index);

void json_get_str(char** dest, Json* json, char* key);
void json_get_num(f64* dest, Json* json, char* key);
void json_get_bool(bool* dest, Json* json, char* key);
void json_get_obj(JsonObject** dest, Json* json, char* key);
void json_get_arr(JsonArray** dest, Json* json, char* key);

void json_str_at(char** dest, Json* json, size_t index);
void json_num_at(f64* dest, Json* json, size_t index);
void json_bool_at(bool* dest, Json* json, size_t index);
void json_obj_at(JsonObject** dest, Json* json, size_t index);
void json_arr_at(JsonArray** dest, Json* json, size_t index);