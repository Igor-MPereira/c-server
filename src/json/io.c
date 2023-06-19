#include <json/io.h>
#include <utils/io.h>
#include <utils/string.h>

void json_print_object(JsonObject* json_obj, size_t indent) {
  printf("{");

  if (indent > 0) {
    printf("\n");
  }

  while (json_obj != null) {
    for (size_t i = 0; i < indent; i++) {
      printf(" ");
    }

    printf("\"%s\": ", json_obj->key);
    json_print(json_obj->value, indent > 0 ? indent + 2 : 0);

    if (json_obj->next != null) {
      printf(",");
    }

    if (indent > 0) {
      printf("\n");
    } else {
      printf(" ");
    }

    json_obj = json_obj->next;
  }

  for (size_t i = 0; i < (indent > 1 ? indent - 2 : 0); i++) {
    printf(" ");
  }

  printf("}");
}

void json_print_array(JsonArray* json_array, size_t indent) {
  printf("[");

  if (indent > 0) {
    printf("\n");
  }

  while (json_array != null) {
    for (size_t i = 0; i < indent; i++) {
      printf(" ");
    }

    json_print(json_array->value, indent > 0 ? indent + 2 : 0);

    if (json_array->next != null) {
      printf(",");
    }

    if (indent > 0) {
      printf("\n");
    } else {
      printf(" ");
    }

    json_array = json_array->next;
  }

  for (size_t i = 0; i < (indent > 1 ? indent - 2 : 0); i++) {
    printf(" ");
  }

  printf("]");
}

void json_print(Json* json, size_t indent) {
  switch (json->type) {
    case json_t(OBJECT):
      json_print_object(json->value.object, indent);
      break;
    case json_t(ARRAY):
      json_print_array(json->value.array, indent);
      break;
    case json_t(STRING):
      printf("\"%s\"", json->value.string);
      break;
    case json_t(NUMBER):
      printf("%lf", json->value.number);
      break;
    case json_t(BOOLEAN):
      printf("%s", json->value.boolean ? "true" : "false");
      break;
    case json_t(NULL):
      printf("null");
      break;
  }
}

void json_perror(JsonError error, char* src) {
  const char* error_msg = json_errmsg(error.exception);

  printf("JSON parse error: %s at position %zu\n", error_msg, error.position);

  char view[(ERRPADDING << 1) + 2];
  size_t start = error.position > ERRPADDING ? error.position - ERRPADDING : 0;
  size_t end = error.position + ERRPADDING;

  strncpy(view, src + start, end - start);

  printf("%s", start > 0 ? "  ..." : "    \"");

  size_t escaped = escprintf(view);

  printf("%s\n", end < strlen(src) ? "...  " : "\"    ");

  for (size_t i = 0; i < error.position - start + 5 + escaped; i++) {
    printf(" ");
  }

  printf("^\n");
}