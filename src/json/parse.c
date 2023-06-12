#include <json/parse.h>

#include <json/exception.h>
#include <json/utils.h>
#include <utils/memory.h>
#include <utils/string.h>

void json_parse_object(JsonObject** obj, char** src, size_t* position) {
  char token;

  next(src, position);
  skip_ws(src, position);

  while (peek(src) != '}') {
    if (peek(src) != '"')
      return json_seterr(jsonex(UNEXPECTED_TOKEN), *position);

    *obj = memalloc(sizeof(JsonObject));
    (*obj)->next = null;
    char* key;
    json_parse_string(&key, src, position);

    (*obj)->key = key;

    if (peek(src) != ':')
      return json_seterr(jsonex(EXPECTED_COLON), *position);

    next(src, position);

    Json* value = json_new();

    json_parse(value, src, position);

    (*obj)->value = value;

    token = skip_ws(src, position);

    if (token == ',') {
      next(src, position);
      skip_ws(src, position);
      obj = &((*obj)->next);
    }
  }
}

void json_parse_string(char** dest, char** src, size_t* position) {
  char token;
  char* temp;

  *dest = null;

  next(src, position);

  temp = *src;

  while ((token = peek(src)) != '"') {
    switch (token) {
      case '\\':
        next(src, position);
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

    next(src, position);
  }

  size_t length = *src - temp;

  *dest = memalloc(length + 1);
  strncpy(*dest, temp, length);
  (*dest)[length] = '\0';

  next(src, position);
}

void json_parse(Json* json, char** src, size_t* position) {
  char token;

  if (json_haserr())
    return;

  if (!json)
    return json_seterr(jsonex(NULLPTR), *position);

  token = skip_ws(src, position);

  switch (token) {
    case '{':
      json->type = json_t(OBJECT);
      return json_parse_object(&json->value.object, src, position);
    case '[':
      json->type = json_t(ARRAY);
      json->value.array = null;
      break;
    case '"':
      json->type = json_t(STRING);
      return json_parse_string(&json->value.string, src, position);
    default:
      token = peek(src);

      if (strneq(*src, "true", 4)) {
        json->type = json_t(BOOLEAN);
        json->value.boolean = true;
        src += 4;
      } else if (strneq(*src, "false", 5)) {
        json->type = json_t(BOOLEAN);
        json->value.boolean = false;
        src += 5;
      } else if (strneq(*src, "null", 4)) {
        json->type = json_t(NULL);
        src += 4;
      } else if (token == '-' || (token >= '0' && token <= '9')) {
        json->type = json_t(NUMBER);
        json->value.number = 0;
      } else {
        return json_seterr(jsonex(UNEXPECTED_TOKEN), *position);
      }
      break;
  }
}