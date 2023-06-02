#include "route.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

void router_init() {
  router.routes = null;
  router.length = 0;
}

void router_add_api(char* path, char* method, route_handler cb) {
  Route* route = (Route*)malloc(ROUTESIZE);
  ApiRoute* api_route = (ApiRoute*)malloc(API_ROUTESIZE);

  api_route->path = (char*)malloc(strlen(path) + 1);
  strcpy(api_route->path, path);

  api_route->method = (char*)malloc(strlen(method) + 1);
  strcpy(api_route->method, method);

  api_route->callback = cb;

  route->api_route = api_route;
  route->is_static = false;

  router.length++;

  if (router.routes == null)
    router.routes = (Route*)malloc(ROUTESIZE);
  else
    router.routes = (Route*)realloc(router.routes, ROUTESIZE * router.length);

  router.routes[router.length - 1] = *route;
}

void get(char* path, route_handler cb) {
  router_add_api(path, "GET", cb);
}

void post(char* path, route_handler cb) {
  router_add_api(path, "POST", cb);
}

void router_route(Request* req, Response* res) {
  for (int i = 0; i < router.length; i++) {
    Route route = router.routes[i];

    if (route.is_static && streq(route.api_route->path, req->path) &&
        streq(route.api_route->method, req->method))
      return route.api_route->callback(req, res);
  }

  response_set_status(res, 404, "Not Found");
  headers_add(res->headers, "Content-Type", "text/html");
  response_set_body(res, "<h1>404 Not Found</h1>");
}
