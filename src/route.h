#pragma once

#include "request.h"
#include "response.h"

typedef struct {
  char* path;
  char* method;
  Response* (*callback)(Request*);
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
void router_add(char* path, char* method, Response* (*cb)(Request*));
void get(char* path, Response* (*cb)(Request*));
void post(char* path, Response* (*cb)(Request*));
/*
TODO: Implement these methods.

  void put(char* path, void (*cb)(Request*, Response*));
  void patch(char* path, void (*cb)(Request*, Response*));
  void del(char* path, void (*cb)(Request*, Response*));
  void head(char* path, void (*cb)(Request*, Response*));
  void options(char* path, void (*cb)(Request*, Response*));
  void trace(char* path, void (*cb)(Request*, Response*));
  void connect(char* path, void (*cb)(Request*, Response*));
*/

Response * router_route(Request* req);