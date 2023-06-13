#include <json/core.h>

#include <json/utils.h>
#include <utils/memory.h>

Json* json_new() {
  Json* json = memalloc(sizeof(Json));

  json->type = JSON_TYPE_NULL;
  json->value = (JsonValue){0};

  return json;
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