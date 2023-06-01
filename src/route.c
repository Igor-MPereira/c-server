#include "route.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

void router_init() {
  router.routes = null;
  router.length = 0;
}

void print_routes() {
  printf("Routes:\n");
  for (int i = 0; i < router.length; i++)
    printf("\t%s %s\n", router.routes[i].method, router.routes[i].path);
}

void router_add(char* path, char* method, Response* (*cb)(Request*)) {
  Route* route = (Route*)malloc(ROUTESIZE);

  route->path = (char*)malloc(strlen(path) + 1);
  strcpy(route->path, path);

  route->method = (char*)malloc(strlen(method) + 1);
  strcpy(route->method, method);

  route->callback = cb;

  router.length++;

  if (router.routes == null)
    router.routes = (Route*)malloc(ROUTESIZE);
  else
    router.routes = (Route*)realloc(router.routes, ROUTESIZE * router.length);

  router.routes[router.length - 1] = *route;
}

void get(char* path, Response* (*cb)(Request*)) {
  router_add(path, "GET", cb);
}

void post(char* path, Response* (*cb)(Request*)) {
  router_add(path, "POST", cb);
}

Response* router_route(Request* req) {
  for (int i = 0; i < router.length; i++) {
    Route route = router.routes[i];
    if (streq(route.path, req->path) && streq(route.method, req->method))
      return route.callback(req);
  }

  Response* res = response_new();

  res->status_code = 404;
  sprintf(res->status_text, "Not Found");
  headers_add(res->headers, "Content-Type", "text/html");
  response_set_body(res, "<h1>404 Not Found</h1>");

  return res;
}
