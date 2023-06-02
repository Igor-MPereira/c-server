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
void router_add_api(char* path, char* method, route_handler callback);
void get(char* path, route_handler callback);
void post(char* path, route_handler callback);
/*
TODO: Implement these methods.

  void put(char* path, route_handler callback);
  void patch(char* path, route_handler callback);
  void del(char* path, route_handler callback);
  void head(char* path, route_handler callback);
  void options(char* path, route_handler callback);
  void trace(char* path, route_handler callback);
  void connect(char* path, route_handler callback);
*/

void router_route(Request* req, Response* res);