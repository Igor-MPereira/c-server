#pragma once

#include <utils/types.h>

typedef enum JsonException {
  JSON_EXCEPTION_NONE,
  JSON_EXCEPTION_UNEXPECTED_TOKEN,
  JSON_EXCEPTION_BAD_CONTROL_CHARACTER,
  JSON_EXCEPTION_UNEXPECTED_END_OF_INPUT,
  JSON_EXCEPTION_UNTERMINATED_STRING,
  JSON_EXCEPTION_EXPECTED_COMMA_OR_CLOSE,
  JSON_EXCEPTION_EXPECTED_COLON,
  JSON_EXCEPTION_NULLPTR
} JsonException;


#define NONE NONE
#define UNEXPECTED_TOKEN UNEXPECTED_TOKEN
#define BAD_CONTROL_CHARACTER BAD_CONTROL_CHARACTER
#define UNEXPECTED_END_OF_INPUT UNEXPECTED_END_OF_INPUT
#define UNTERMINATED_STRING UNTERMINATED_STRING
#define EXPECTED_COMMA_OR_CLOSE EXPECTED_COMMA_OR_CLOSE
#define EXPECTED_COLON EXPECTED_COLON
#define NULLPTR NULLPTR
#define jsonex(X) JSON_EXCEPTION_##X

typedef struct JsonError {
  JsonException exception;
  size_t position;
} JsonError;

void json_seterr(JsonException exception, size_t position);
bool json_geterr(JsonError* error);
bool json_haserr();