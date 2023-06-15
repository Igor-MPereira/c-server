#include <stdio.h>

#include <file.h>
#include <json/core.h>
#include <json/io.h>
#include <route.h>
#include <server/http.h>
#include <utils/memory.h>

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

void GetUsers(Request* _ UNUSED, Response* res) {
  response_set_status(res, 200, "OK");
  headers_add(res->headers, "Content-Type", "application/json");
  response_set_body(res,
                    "{\"users\": ["
                    "{\"name\": \"John Doe\", \"age\": 30},"
                    "{\"name\": \"Jane Doe\", \"age\": 25}"
                    "]}");
}

void PostUsers(Request* _ UNUSED, Response* res) {
  response_set_status(res, 200, "OK");
  headers_add(res->headers, "Content-Type", "application/json");
  response_set_body(res, "{\"message\": \"User created\"}");
}

void PutUsers(Request* _ UNUSED, Response* res) {
  response_set_status(res, 200, "OK");
  headers_add(res->headers, "Content-Type", "application/json");
  response_set_body(res, "{\"message\": \"User updated\"}");
}

int main(int argc UNUSED, char** argv UNUSED) {
  /*
  SOCKET sSock;

  u16 port = PORT;

  if (argc > 1)
    port = atoi(argv[1]);

  serve_static("/static", "static");

  route_get("/", Index);
  route_get("/users", GetUsers);
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
  router_free();

  if (sSock == INVALID_SOCKET)
    return 1;
  */

  size_t position = 0;
  char* src = "{\"name\": \"John Doe\", \"age\": 30";
  Json* json = json_new();

  json_parse(json, src);

  JsonError err;

  if (json_geterr(&err)) {
    json_perror(err, src);

    return 1;
  }

  printf("position %zu\n", position);
  printf("src after %s\n", src);
  printf("src before %s\n", src - position);

  // Json* json = json_new();

  // char* s = "{\"name\": \"John Doe\", \"age\": 30}";

  // size_t position = 0;

  // json_parse(json, &s, &position);

  // json_print(json, 2);
  // printf("\n");

  // printf("json[\"%s\"] = \"%s\"\n", json->value.object->key,
  //        json->value.object->value->value.string);

  // json_free(json);

  return 0;
}