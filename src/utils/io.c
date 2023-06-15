#include <utils/io.h>

void escprintf(char* str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
    switch (str[i]) {
      case '\n':
        printf("\\n");
        break;
      case '\t':
        printf("\\t");
        break;
      case '\r':
        printf("\\r");
        break;
      case '\b':
        printf("\\b");
        break;
      case '\f':
        printf("\\f");
        break;
      case '\v':
        printf("\\v");
        break;
      case '\\':
        printf("\\\\");
        break;
      case '\'':
        printf("\\'");
        break;
      case '\"':
        printf("\\\"");
        break;
      default:
        printf("%c", str[i]);
        break;
    }
  }
}