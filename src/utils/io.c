#include <utils/io.h>

size_t escprintf(char* str) {
  size_t escaped = 0;
  for (size_t i = 0; str[i] != '\0'; i++) {
    switch (str[i]) {
      case '\n':
        printf("\\n");
        escaped++;
        break;
      case '\t':
        printf("\\t");
        escaped++;
        break;
      case '\r':
        printf("\\r");
        escaped++;
        break;
      case '\b':
        printf("\\b");
        escaped++;
        break;
      case '\f':
        printf("\\f");
        escaped++;
        break;
      case '\v':
        printf("\\v");
        escaped++;
        break;
      case '\\':
        printf("\\\\");
        escaped++;
        break;
      case '\'':
        printf("\\'");
        escaped++;
        break;
      case '"':
        printf("\\\"");
        escaped++;
        break;
      default:
        printf("%c", str[i]);
        break;
    }
  }

  return escaped;
}