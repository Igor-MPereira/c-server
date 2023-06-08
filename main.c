#include <file.h>
#include <route.h>
#include <server/http.h>

#include <stdio.h>
#include <stdlib.h>

#define PORT 8080

void onload(u16 port) {
  printf(
      "Running...\n\n"

      "Server listening on port %d.\n\n"

      "http://localhost:%d\n\n",
      port, port);
}

void Index(Request* _, Response* res) {
  send_file("static/index.html", _, res);
}

void Users(__UNUSED_PARAM(Request* _), Response* res) {
  response_set_status(res, 200, "OK");
  headers_add(res->headers, "Content-Type", "application/json");
  response_set_body(res,
                    "{\"users\": ["
                    "{\"name\": \"John Doe\", \"age\": 30},"
                    "{\"name\": \"Jane Doe\", \"age\": 25}"
                    "]}");
}

void PostUsers(__UNUSED_PARAM(Request* _), Response* res) {
  response_set_status(res, 200, "OK");
  headers_add(res->headers, "Content-Type", "application/json");
  response_set_body(res, "{\"message\": \"User created\"}");
}

void PutUsers(__UNUSED_PARAM(Request* _), Response* res) {
  response_set_status(res, 200, "OK");
  headers_add(res->headers, "Content-Type", "application/json");
  response_set_body(res, "{\"message\": \"User updated\"}");
}

int main(int argc, char** argv) {
  SOCKET sSock;

  u16 port = PORT;

  if (argc > 1)
    port = atoi(argv[1]);

  serve_static("/static", "static");

  route_get("/", Index);
  route_get("/users", Users);
  route_post("/users", PostUsers);
  route_put("/users", PutUsers);

  ServerOptions* opt = server_options_new();
  CorsOptions* cors = cors_options_new();

  cors->origin = "*";
  use_cors(opt, cors);

  headers_add(opt->headers, "X-Powered-By", "c-http-server");

  sSock = http_server(port, opt, onload);

  cors_options_free(cors);
  server_options_free(opt);

  if (sSock == INVALID_SOCKET)
    return 1;

  return 0;
}