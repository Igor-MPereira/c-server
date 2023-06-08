#include <json/parse.h>

#include <json/exception.h>
#include <json/utils.h>
#include <utils/string.h>

void json_parse_object(Json* json, char* src, size_t* position) {
  char token;

  json->type = json_t(OBJECT);
  json->value.object = null;

  next(&src, position);

  while (peek(src) != '}') {
    token = skip_ws(&src, position);

    if (token == '"') {
      next(&src, position);
      char* key = src;

      while ((token = peek(src)) != '"') {
        switch (token) {
          case '\\':
            next(&src, position);
            break;
          case '\0':
            return json_seterr(jsonex(UNEXPECTED_END_OF_INPUT), *position);
          case '\b':
          case '\f':
          case '\n':
          case '\r':
          case '\t':
          case '\v':
            return json_seterr(jsonex(BAD_CONTROL_CHARACTER), *position);
        }

        next(&src, position);
      }

      size_t key_length = src - key;

      json->value.object->key = malloc(key_length + 1);
      strncpy(json->value.object->key, key, key_length);

      next(&src, position);

      token = skip_ws(&src, position);

      if (token != ':')
        return json_seterr(jsonex(EXPECTED_COLON), *position);

      next(&src, position);

      Json* value = json_new();

      json_parse(value, src, position);

      json->value.object->value = value;
    } else {
      return json_seterr(jsonex(UNEXPECTED_TOKEN), *position);
    }
  }
}

void json_parse(Json* json, char* src, size_t* position) {
  char token;

  if (json_haserr())
    return;

  if (!json)
    return json_seterr(jsonex(NULLPTR), *position);

  token = skip_ws(&src, position);

  switch (token) {
    case '{':
      return json_parse_object(json, src, position);
    case '[':
      json->type = json_t(ARRAY);
      json->value.array = null;
      break;
    case '"':
      json->type = json_t(STRING);
      json->value.string = null;
      break;
    default:
      if (strneq(src, "true", 4)) {
        json->type = json_t(BOOLEAN);
        json->value.boolean = true;
        src += 4;
      } else if (strneq(src, "false", 5)) {
        json->type = json_t(BOOLEAN);
        json->value.boolean = false;
        src += 5;
      } else if (strneq(src, "null", 4)) {
        json->type = json_t(NULL);
        src += 4;
      } else if (*src == '-' || (*src >= '0' && *src <= '9')) {
        json->type = json_t(NUMBER);
        json->value.number = 0;
      } else {
        // unexpected token
      }
      break;
  }
}