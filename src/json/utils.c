#include <json/utils.h>

#include <json/core.h>

// PARSING UTILS

bool is_ws(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

char skip_ws(char** s, size_t* position) {
  while (is_ws(**s)) {
    (*s)++;
    (*position)++;
  }

  return **s;
}

char peek(char** s) {
  return **s;
}

char next(char** s, size_t* position) {
  (*s)++;
  (*position)++;

  return **s;
}

// MEMORY UTILS

void json_object_free(JsonObject* json_obj) {
  memfree(json_obj->key);
  json_free(json_obj->value);
  memfree(json_obj);

  if (json_obj->next != null) {
    json_object_free(json_obj->next);
  }
}

void json_array_free(JsonArray* json_array) {
  for (size_t i = 0; i < json_array->length; i++) {
    json_free(json_array->items[i]);
  }

  memfree(json_array->items);
  memfree(json_array);
}