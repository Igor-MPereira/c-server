#include <json/core.h>

Json* json_new() {
  Json* json = malloc(sizeof(Json));

  json->type = JSON_TYPE_NULL;
  json->value = (JsonValue){0};

  return json;
}

void json_object_free(JsonObject* json_obj) {
  free(json_obj->key);
  json_free(json_obj->value);
  free(json_obj);
}

void json_array_free(JsonArray* json_array) {
  for (size_t i = 0; i < json_array->length; i++) {
    json_free(json_array->items[i]);
  }

  free(json_array->items);
  free(json_array);
}

void json_free(Json* json) {
  switch (json->type) {
    case JSON_TYPE_OBJECT:
      json_object_free(json->value.object);
      break;
    case JSON_TYPE_ARRAY:
      json_array_free(json->value.array);
      break;
    case JSON_TYPE_STRING:
      free(json->value.string);
      break;
    default:
      break;
  }

  free(json);
}