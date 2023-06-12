#include <json/io.h>

#include <stdio.h>

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
    json_print(json_obj->value, indent + 2);

    if (json_obj->next != null) {
      printf(",");
    }

    printf("\n");

    json_obj = json_obj->next;
  }

  for (size_t i = 0; i < indent - 2; i++) {
    printf(" ");
  }

  printf("}");
}

void json_print_array(JsonArray* json_array, size_t indent) {
  printf("[");

  if (indent > 0) {
    printf("\n");
  }

  for (size_t i = 0; i < json_array->length; i++) {
    for (size_t j = 0; j < indent; j++) {
      printf(" ");
    }

    json_print(json_array->items[i], indent + 2);

    if (i < json_array->length - 1) {
      printf(",");
    }

    printf("\n");
  }

  for (size_t i = 0; i < indent - 2; i++) {
    printf(" ");
  }

  printf("]");
}

void json_print(Json* json, size_t indent) {
  switch (json->type) {
    case JSON_TYPE_OBJECT:
      json_print_object(json->value.object, indent);
      break;
    case JSON_TYPE_ARRAY:
      json_print_array(json->value.array, indent);
      break;
    case JSON_TYPE_STRING:
      printf("\"%s\"", json->value.string);
      break;
    case JSON_TYPE_NUMBER:
      printf("%lf", json->value.number);
      break;
    case JSON_TYPE_BOOLEAN:
      printf("%s", json->value.boolean ? "true" : "false");
      break;
    case JSON_TYPE_NULL:
      printf("null");
      break;
  }
}