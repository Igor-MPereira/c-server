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

void router_add(char* path, char* method, route_handler cb) {
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

void get(char* path, route_handler cb) {
  router_add(path, "GET", cb);
}

void post(char* path, route_handler cb) {
  router_add(path, "POST", cb);
}

void router_route(Request* req, Response* res) {
  for (int i = 0; i < router.length; i++) {
    Route route = router.routes[i];
    if (streq(route.path, req->path) && streq(route.method, req->method))
      return route.callback(req, res);
  }

  response_set_status(res, 404, "Not Found");
  headers_add(res->headers, "Content-Type", "text/html");
  response_set_body(res, "<h1>404 Not Found</h1>");
}
