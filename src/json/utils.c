#include <json/utils.h>

bool isspace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

char skip_ws(char** s, size_t* position) {
  while (isspace(**s)) {
    (*s)++;
    (*position)++;
  }

  return **s;
}

char peek(char* s) {
  return *s;
}

char next(char** s, size_t* position) {
  (*s)++;
  (*position)++;

  return **s;
}