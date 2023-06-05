#include "route.h"
#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "string.h"

void router_init() {
  router.routes = null;
  router.length = 0;
}

void router_destroy() {
  for (int i = 0; i < router.length; i++) {
    Route* route = &router.routes[i];

    if (!route->is_static) {
      free(route->api_route->path);
      free(route->api_route->method);
      free(route->api_route);
    } else {
      free(route->static_route->base_path);
      free(route->static_route->dir);
      free(route->static_route);
    }

    free(route);
  }

  free(router.routes);
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

void serve_static(char* base_path, char* dir) {
  Route* route = (Route*)malloc(ROUTESIZE);
  StaticRoute* static_route = (StaticRoute*)malloc(STATIC_ROUTESIZE);

  static_route->base_path = (char*)malloc(strlen(base_path) + 1);
  strcpy(static_route->base_path, base_path);

  static_route->dir = (char*)malloc(strlen(dir) + 1);
  strcpy(static_route->dir, dir);

  route->static_route = static_route;
  route->is_static = true;

  router.length++;

  if (router.routes == null)
    router.routes = (Route*)malloc(ROUTESIZE);
  else
    router.routes = (Route*)realloc(router.routes, ROUTESIZE * router.length);

  router.routes[router.length - 1] = *route;
}

void handle_static(StaticRoute* route, Request* req, Response* res) {
  char* path = req->path + strlen(route->base_path);
  char* file_path = (char*)malloc(strlen(route->dir) + strlen(path) + 1);

  strcpy(file_path, route->dir);

  if (path[0] != '/')
    strcat(file_path, "/");

  strcat(file_path, path);

  serve_file(file_path, req, res);

  free(file_path);
}

void route_get(char* path, route_handler cb) {
  router_add_api(path, "GET", cb);
}

void route_post(char* path, route_handler cb) {
  router_add_api(path, "POST", cb);
}

void route_put(char* path, route_handler cb) {
  router_add_api(path, "PUT", cb);
}

void route_patch(char* path, route_handler cb) {
  router_add_api(path, "PATCH", cb);
}

void route_delete(char* path, route_handler cb) {
  router_add_api(path, "DELETE", cb);
}

void route_head(char* path, route_handler cb) {
  router_add_api(path, "HEAD", cb);
}

void route_options(char* path, route_handler cb) {
  router_add_api(path, "OPTIONS", cb);
}

void route_trace(char* path, route_handler cb) {
  router_add_api(path, "TRACE", cb);
}

void route_connect(char* path, route_handler cb) {
  router_add_api(path, "CONNECT", cb);
}

void router_route(Request* req, Response* res) {
  for (int i = 0; i < router.length; i++) {
    Route route = router.routes[i];

    if (route.is_static) {
      if (strcasestr(req->path, route.static_route->base_path) != req->path)
        continue;

      if (!(streq(req->method, "GET") || streq(req->method, "HEAD")))
        continue;

      return handle_static(route.static_route, req, res);
    }

    if (!streq(req->method, route.api_route->method))
      continue;

    if (strcaseeq(req->path, route.api_route->path))
      return route.api_route->callback(req, res);
  }

  response_set_status(res, 404, "Not Found");
  headers_add(res->headers, "Content-Type", "text/html");
  response_set_body(res, "<h1>404 Not Found</h1>");
}
