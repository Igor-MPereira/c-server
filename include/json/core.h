#pragma once

typedef struct Json Json;
struct Json {
  char* key;
  char* value;
  Json* next;
};

typedef struct JsonValue {
  char* key;
  char* value;
} JsonValue;