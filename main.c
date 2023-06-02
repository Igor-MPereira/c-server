#include <stdio.h>
#include "src/http.h"
#include "src/route.h"
#include "src/file.h"

void onload() {
  printf(
      "Running...\n\n"

      "Server is listening on port 8080.\n\n"

      "http://localhost:8080/\n");
}

void index(Request* _, Response* res) {
  serve_file("static/index.html", _, res);
}

int main(int argc, char** argv) {
  SOCKET sSock;

  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    return 1;
  }

  router_init();

  get("/", index);

  sSock = http_server(atoi(argv[1]), onload);

  if (sSock == INVALID_SOCKET)
    return 1;

  return 0;
}