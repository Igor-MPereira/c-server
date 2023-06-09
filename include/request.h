#pragma once

#include <headers.h>

typedef struct Request {
  char method[8], http_version[16], path[256];
  char* query;
  char* body;
  Headers* headers;
} Request;

Request* request_new();

void request_free(Request* request);

void request_parse(Request* request, char* buffer);

void request_parse_body(Request* request, char* ctx);

void request_print(Request* request);
