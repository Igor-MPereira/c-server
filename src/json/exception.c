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
