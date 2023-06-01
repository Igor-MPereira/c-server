#include "src/http.h"
#include <stdio.h>

int main(int argc, char** argv) {
  SOCKET sSock;

  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    return 1;
  }

  sSock = http_server(atoi(argv[1]));

  if (sSock == INVALID_SOCKET)
    return 1;

  return 0;
}