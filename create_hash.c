#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define assert(expression, message, ...)                                      \
  if (!(expression)) {                                                        \
    printf("Assertion failed: %s, line %d, file %s\n", #expression, __LINE__, \
           __FILE__);                                                         \
    printf("Message: " message "\n", __VA_ARGS__);                            \
    *(int*)0 = 0;                                                             \
  }

#define assert_not_null(expression) \
  assert(expression != NULL, "%s is null", #expression)

/// @brief Hashes a string using sdbm algorithm.
/// http://www.cse.yorku.ca/~oz/hash.html
/// @param str string to be hashed
/// @return unsigned integer hash value
int hash(const char* str) {
  int hash = 0;

  for (size_t i = 0; i < strlen(str); i++) {
    hash = str[i] + (hash << 6) + (hash << 16) - hash;
  }

  return hash;
}

#define prompt(message) printf("%-29s >> ", message)

// read a string from stdin
#define read_string(message, s) \
  prompt(message);              \
  fgets(s, 100, stdin);         \
  assert_not_null(s);           \
  if (s[0] == '\n')             \
    break;                      \
  s[strlen(s) - 1] = '\0';

int main(int argc, char const* argv[]) {
  if (argc < 2) {
    printf("Usage: %s <file>\n", argv[0]);
    return 1;
  }

  FILE* fp = fopen(argv[1], "w");

  if (fp == NULL) {
    perror("fopen ");
    return 1;
  }

  fprintf(fp, "#pragma once\n\n");

  char str[100];
  char buffer[200];
  char cwd[PATH_MAX];

  getcwd(cwd, PATH_MAX);

  assert_not_null(cwd);

  printf(
      "The generated hash values will be defined in '%s"
#ifdef _WIN32
      "\\"
#else
      "/"
#endif
      "%s'\nType 0 to exit\n\n",
      cwd, argv[1]);

  while (1) {
    buffer[0] = '\0';

    read_string("Enter the define label", str);
    if (str[0] == '0')
      break;
    sprintf(buffer, "#define %s ", str);

    read_string("Enter the string to be hashed", str);
    if (str[0] == '0')
      break;
    sprintf(buffer + strlen(buffer), "%zu\n", hash(str));

    fprintf(fp, "%s", buffer);
  }

  fclose(fp);

  return 0;
}