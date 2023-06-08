#include <route.h>

#include <stdio.h>

#include <file.h>
#include <utils/memory.h>
#include <utils/string.h>

static Router r = {.routes = null, .length = 0};

void router_free() {
  for (u64 i = 0; i < r.length; i++) {
    Route* route = r.routes[i];

    if (!route->is_static) {
      memfree(route->api_route->path);
      memfree(route->api_route->method);
      memfree(route->api_route);
    } else {
      memfree(route->static_route->base_path);
      memfree(route->static_route->dir);
      memfree(route->static_route);
    }

    memfree(route);
  }

  memfree(r.routes);
}

void router_add_api(const char* path, const char* method, route_handler cb) {
  Route* route = (Route*)memalloc(ROUTESIZE);
  ApiRoute* api_route = (ApiRoute*)memalloc(API_ROUTESIZE);

  api_route->path = (char*)memalloc(strlen(path) + 1);
  strcpy(api_route->path, path);

  api_route->method = (char*)memalloc(strlen(method) + 1);
  strcpy(api_route->method, method);

  api_route->callback = cb;

  route->api_route = api_route;
  route->is_static = false;

  r.length++;

  if (!r.routes)
    r.routes = (Route**)memalloc(sizeof(Route*));
  else
    r.routes = (Route**)memrealloc(r.routes, sizeof(Route*) * r.length);

  r.routes[r.length - 1] = route;
}

void serve_static(const char* base_path, const char* dir) {
  Route* route = (Route*)memalloc(ROUTESIZE);
  StaticRoute* static_route = (StaticRoute*)memalloc(STATIC_ROUTESIZE);

  static_route->base_path = (char*)memalloc(strlen(base_path) + 1);
  strcpy(static_route->base_path, base_path);

  static_route->dir = (char*)memalloc(strlen(dir) + 1);
  strcpy(static_route->dir, dir);

  route->static_route = static_route;
  route->is_static = true;

  r.length++;

  if (!r.routes)
    r.routes = (Route**)memalloc(sizeof(Route*));
  else
    r.routes = (Route**)memrealloc(r.routes, sizeof(Route*) * r.length);

  r.routes[r.length - 1] = route;
}

void handle_static(StaticRoute* route, Request* req, Response* res) {
  char* path = req->path + strlen(route->base_path);
  size_t dir_len = strlen(route->dir);
  char* file_path = (char*)memalloc(dir_len + strlen(path) + 1);

  strcpy(file_path, route->dir);

  if (file_path[dir_len - 1] != '/' && path[0] != '/')
    strcat(file_path, "/");
  else if (file_path[dir_len - 1] == '/' && path[0] == '/')
    path++;

  strcat(file_path, path);

  send_file(file_path, req, res);

  memfree(file_path);
}

void route_get(const char* path, route_handler cb) {
  router_add_api(path, "GET", cb);
}

void route_post(const char* path, route_handler cb) {
  router_add_api(path, "POST", cb);
}

void route_put(const char* path, route_handler cb) {
  router_add_api(path, "PUT", cb);
}

void route_patch(const char* path, route_handler cb) {
  router_add_api(path, "PATCH", cb);
}

void route_delete(const char* path, route_handler cb) {
  router_add_api(path, "DELETE", cb);
}

void route_head(const char* path, route_handler cb) {
  router_add_api(path, "HEAD", cb);
}

void route_options(const char* path, route_handler cb) {
  router_add_api(path, "OPTIONS", cb);
}

void route_trace(const char* path, route_handler cb) {
  router_add_api(path, "TRACE", cb);
}

void route_connect(const char* path, route_handler cb) {
  router_add_api(path, "CONNECT", cb);
}

void router_handle_request(Request* req, Response* res) {
  for (u64 i = 0; i < r.length; i++) {
    Route* route = r.routes[i];

    if (route->is_static) {
      if (!strcasestarts(req->path, route->static_route->base_path) ||
          (strdiff(req->method, "GET") && strdiff(req->method, "HEAD")))
        continue;

      return handle_static(route->static_route, req, res);
    }

    if (strdiff(req->method, route->api_route->method))
      continue;

    if (strcaseeq(req->path, route->api_route->path))
      return route->api_route->callback(req, res);
  }

  response_set_status(res, 404, "Not Found");
  headers_add(res->headers, "Content-Type", "text/html");
  response_set_body(res, "<h1>404 Not Found</h1>");
}
