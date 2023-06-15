#include <json/exception.h>

static JsonException exception = jsonex(NONE);
static size_t position = -1;

void json_seterr(JsonException e, size_t p) {
  exception = e;
  position = p;
}

bool json_geterr(JsonError* error) {
  if (exception == jsonex(NONE))
    return false;

  error->exception = exception;
  error->position = position;

  exception = jsonex(NONE);
  position = -1;

  return true;
}

bool json_haserr() {
  return exception != jsonex(NONE);
}

const char* json_errmsg(JsonException exception) {
  switch (exception) {
    case jsonex(BAD_CONTROL_CHARACTER):
      return BAD_CONTROL_CHARACTER_MSG;
    case jsonex(UNEXPECTED_TOKEN):
      return UNEXPECTED_TOKEN_MSG;
    case jsonex(UNEXPECTED_END_OF_INPUT):
      return UNEXPECTED_END_OF_INPUT_MSG;
    case jsonex(UNEXPECTED_NUMBER):
      return UNEXPECTED_NUMBER_MSG;
    case jsonex(UNEXPECTED_WHITESPACE):
      return UNEXPECTED_WHITESPACE_MSG;
    case jsonex(UNEXPECTED_NON_WHITESPACE):
      return UNEXPECTED_NON_WHITESPACE_MSG;
    case jsonex(UNTERMINATED_STRING):
      return UNTERMINATED_STRING_MSG;
    case jsonex(EXPECTED_COMMA_OR_CLOSE):
      return EXPECTED_COMMA_OR_CLOSE_MSG;
    case jsonex(EXPECTED_COLON):
      return EXPECTED_COLON_MSG;
    case jsonex(EXPECTED_PROPERTY):
      return EXPECTED_PROPERTY_MSG;
    case jsonex(EXPECTED_PROPERTY_OR_CLOSE):
      return EXPECTED_PROPERTY_OR_CLOSE_MSG;
    case jsonex(NULLPTR):
      return NULLPTR_MSG;
    default:
      return "unknown error";
  }
}