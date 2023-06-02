#pragma once

#include "request.h"
#include "response.h"

typedef void (*route_handler)(Request*, Response*);

typedef struct {
  char* path;
  char* method;
  route_handler callback;
} Route;

#define ROUTESIZE sizeof(Route)

#define ROUTER_MAX_ROUTES 100

typedef struct {
  Route* routes;
  int length;
} Router;

static Router router;

void router_init();
void print_routes();
void router_add(char* path, char* method, route_handler callback);
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