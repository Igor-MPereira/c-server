#include <stdio.h>
#include "src/file.h"
#include "src/http.h"
#include "src/route.h"

void onload() {
  printf(
      "Running...\n\n"

      "Server is listening on port 8080.\n\n"

      "http://localhost:8080/\n");
}

void Index(Request* _, Response* res) {
  serve_file("static/index.html", _, res);
}

void Users(Request* _, Response* res) {
  response_set_status(res, 200, "OK");
  headers_add(res->headers, "Content-Type", "application/json");
  response_set_body(res,
                    "{\"users\": ["
                    "{\"name\": \"John Doe\", \"age\": 30},"
                    "{\"name\": \"Jane Doe\", \"age\": 25}"
                    "]}");
}

void PostUsers(Request* req, Response* res) {
  response_set_status(res, 200, "OK");
  headers_add(res->headers, "Content-Type", "application/json");
  response_set_body(res, "{\"message\": \"User created\"}");
}

void PutUsers(Request* req, Response* res) {
  response_set_status(res, 200, "OK");
  headers_add(res->headers, "Content-Type", "application/json");
  response_set_body(res, "{\"message\": \"User updated\"}");
}

int main(int argc, char** argv) {
  SOCKET sSock;

  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    return 1;
  }

  router_init();

  serve_static("/static", "static");

  route_get("/", Index);
  route_get("/users", Users);
  route_post("/users", PostUsers);
  route_put("/users", PutUsers);

  sSock = http_server(atoi(argv[1]), onload, true);

  router_destroy();

  if (sSock == INVALID_SOCKET)
    return 1;

  return 0;
}