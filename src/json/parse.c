#include <json/exception.h>
#include <json/parse.h>
#include <json/utils.h>
#include <utils/memory.h>
#include <utils/string.h>

void __json_parse(Json* json, char** src, size_t* position, bool is_root) {
  char token;

  if (json_haserr()) return;

  if (!json) return json_seterr(jsonex(NULLPTR), *position);

  token = skip_ws(src, position);

  switch (token) {
    case '\0':
      return json_seterr(jsonex(UNEXPECTED_END_OF_INPUT), *position);
    case '{':
      json->type = json_t(OBJECT);
      return __json_parse_object(&json->value.object, src, position);
    case '[':
      json->type = json_t(ARRAY);
      return __json_parse_array(&json->value.array, src, position);
      break;
    case '"':
      json->type = json_t(STRING);
      return __json_parse_string(&json->value.string, src, position);
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
      } else if (token == '-' || token == '+' ||
                 (token >= '0' && token <= '9')) {
        json->type = json_t(NUMBER);
        return __json_parse_number(&json->value.number, src, position);
      } else {
        return json_seterr(jsonex(UNEXPECTED_TOKEN), *position);
      }
      break;
  }

  skip_ws(src, position);

  if (is_root && peek(src) != '\0')
    return json_seterr(jsonex(UNEXPECTED_NON_WHITESPACE), *position);
}

void __json_parse_object(JsonObject** obj, char** src, size_t* position) {
  char token;

  next(src, position);
  skip_ws(src, position);

  while (peek(src) != '}') {
    if (peek(src) != '"')
      return json_seterr(jsonex(UNEXPECTED_TOKEN), *position);

    *obj = memalloc(sizeof(JsonObject));
    (*obj)->next = null;
    char* key;

    __json_parse_string(&key, src, position);

    if (json_haserr()) return;

    (*obj)->key = key;

    if (peek(src) != ':') return json_seterr(jsonex(EXPECTED_COLON), *position);

    next(src, position);

    Json* value = json_new();

    __json_parse(value, src, position, false);

    if (json_haserr()) return;

    (*obj)->value = value;

    token = skip_ws(src, position);

    if (token == ',') {
      next(src, position);
      skip_ws(src, position);
      obj = &((*obj)->next);
    }
  }
}

void __json_parse_array(JsonArray** arr, char** src, size_t* position) {
  char token;

  next(src, position);
  skip_ws(src, position);

  while (peek(src) != ']') {
    Json* value = json_new();

    __json_parse(value, src, position, false);

    if (json_haserr()) return;

    if (!*arr) {
      *arr = memalloc(sizeof(JsonArray));
      (*arr)->next = null;
      (*arr)->value = value;
    } else {
      JsonArray* temp = *arr;

      while (temp->next) temp = temp->next;

      temp->next = memalloc(sizeof(JsonArray));
      temp->next->next = null;
      temp->next->value = value;
    }

    token = skip_ws(src, position);

    if (token == ',') {
      next(src, position);
      skip_ws(src, position);
      arr = &((*arr)->next);
    }
  }

  next(src, position);
}

void __json_parse_string(char** dest, char** src, size_t* position) {
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

void __json_parse_number(f64* dest, char** src, size_t* position) {
  char* temp;
  char* end;

  temp = *src;

  while (is_digit(peek(src)) || peek(src) == '.' || peek(src) == 'e' ||
         peek(src) == 'E' || peek(src) == '+' || peek(src) == '-') {
    next(src, position);
  }

  if (peek(src) == '\0')
    return json_seterr(jsonex(UNEXPECTED_END_OF_INPUT), *position);

  *dest = strtod(temp, &end);

  if (end != *src) return json_seterr(jsonex(UNEXPECTED_NUMBER), *position);
}

void __json_parse_boolean(bool* dest, char** src, size_t* position) {
  if (strneq(*src, "true", 4)) {
    *dest = true;
    *src += 4;
  } else if (strneq(*src, "false", 5)) {
    *dest = false;
    *src += 5;
  } else {
    return json_seterr(jsonex(UNEXPECTED_TOKEN), *position);
  }
}

void __json_parse_null(char** src, size_t* position) {
  if (strneq(*src, "null", 4)) {
    *src += 4;
  } else {
    return json_seterr(jsonex(UNEXPECTED_TOKEN), *position);
  }
}