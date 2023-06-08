#pragma once

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
#define NULL NULL

#define json_t(X) JSON_TYPE_##X

typedef struct Json Json;

typedef struct JsonObject {
  char* key;
  Json* value;
} JsonObject;

typedef struct JsonArray {
  Json** items;
  size_t length;
} JsonArray;

typedef union JsonValue {
  JsonObject* object;
  JsonArray* array;
  char* string;
  double number;
  bool boolean;
} JsonValue;

struct Json {
  JsonType type;
  JsonValue value;
};

Json* json_new();

void json_object_free(JsonObject* object);
void json_array_free(JsonArray* array);
void json_free(Json* json);