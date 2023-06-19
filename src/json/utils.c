#include <json/utils.h>
#include <utils/memory.h>

// PARSING UTILS

bool is_ws(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }

bool is_digit(char c) { return c >= '0' && c <= '9' || c == '-' || c == '+'; }

char skip_ws(char** s, size_t* position) {
  while (is_ws(**s)) {
    (*s)++;
    (*position)++;
  }

  return **s;
}

char peek(char** s) { return **s; }

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
  json_free(json_array->value);
  memfree(json_array);

  if (json_array->next != null) {
    json_array_free(json_array->next);
  }
}