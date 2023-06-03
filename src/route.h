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
  Route* routes;
  int length;
} Router;

static Router router;

void router_init();
void router_destroy();

void serve_static(char* base_path, char* dir);
void handle_static(StaticRoute* route, Request* req, Response* res);

void router_add_api(char* path, char* method, route_handler callback);

void route_get(char* path, route_handler callback);
void route_post(char* path, route_handler callback);
void route_put(char* path, route_handler callback);
void route_patch(char* path, route_handler callback);
void route_delete(char* path, route_handler callback);
void route_head(char* path, route_handler callback);
void route_options(char* path, route_handler callback);
void route_trace(char* path, route_handler callback);
void route_connect(char* path, route_handler callback);

void router_route(Request* req, Response* res);