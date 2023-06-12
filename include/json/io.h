#pragma once

#include <json/core.h>

void json_print_object(JsonObject* json_obj, size_t indent);
void json_print_array(JsonArray* json_array, size_t indent);
void json_print(Json* json, size_t indent);