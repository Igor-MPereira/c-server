#include <json/core.h>

#include <utils/memory.h>

Json* json_new() {
  Json* json = memalloc(sizeof(Json));

  json->type = JSON_TYPE_NULL;
  json->value = (JsonValue){0};

  return json;
}

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

void json_free(Json* json) {
  switch (json->type) {
    case JSON_TYPE_OBJECT:
      json_object_free(json->value.object);
      break;
    case JSON_TYPE_ARRAY:
      json_array_free(json->value.array);
      break;
    case JSON_TYPE_STRING:
      memfree(json->value.string);
      break;
    default:
      break;
  }

  memfree(json);
}