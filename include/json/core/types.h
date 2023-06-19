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

#define json_t(X) JSON_TYPE_##X

typedef struct Json Json;
typedef struct JsonObject JsonObject;
typedef struct JsonArray JsonArray;

struct JsonObject {
  char* key;
  Json* value;
  JsonObject* next;
};

struct JsonArray {
  Json* value;
  JsonArray* next;
};

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