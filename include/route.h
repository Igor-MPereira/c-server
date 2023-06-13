#pragma once

#include "request.h"
#include "response.h"

typedef void (*route_handler)(Request*, Response*);

typedef struct {
  char* path;
  char* method;
  route_handler callback;
} ApiRoute;

typedef struct {
  char* base_path;
  char* dir;
} StaticRoute;

typedef struct {
  bool is_static;
  union {
    ApiRoute* api_route;
    StaticRoute* static_route;
  };
} Route;

#define ROUTESIZE sizeof(Route)
#define API_ROUTESIZE sizeof(ApiRoute)
#define STATIC_ROUTESIZE sizeof(StaticRoute)

#define ROUTER_MAX_ROUTES 100

typedef struct {
  Route** routes;
  size_t length;
} Router;

void router_free();

void serve_static(const char* base_path, const char* dir);
void handle_static(StaticRoute* route, Request* req, Response* res);

void router_add_api(const char* path,
                    const char* method,
                    route_handler callback);

void route_get(const char* path, route_handler callback);
void route_post(const char* path, route_handler callback);
void route_put(const char* path, route_handler callback);
void route_patch(const char* path, route_handler callback);
void route_delete(const char* path, route_handler callback);
void route_head(const char* path, route_handler callback);
void route_options(const char* path, route_handler callback);
void route_trace(const char* path, route_handler callback);
void route_connect(const char* path, route_handler callback);

void router_handle_request(Request* req, Response* res);