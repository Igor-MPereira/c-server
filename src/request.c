#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "string.h"

Request* request_new() {
  Request* r = (Request*)malloc(sizeof(Request));
  r->headers = headers_new();
  r->body = null;

  return r;
}

void request_destroy(Request* r) {
  headers_destroy(r->headers);
  free(r->body);
  free(r);
}

void request_parse_body(Request* r, char* ctx) {
  char* contentLength = headers_get(r->headers, "Content-Length");

  if (contentLength) {
    int length = atoi(contentLength);
    r->body = (char*)malloc(length + 1);
    strcpy(r->body, ctx);
  }
}

void request_parse(Request* r, char* reqStr) {
  char* ctx;
  char* line = strtok_s(reqStr, "\r\n", &ctx);
  char* method = strtok(line, " ");
  char* path = strtok(null, " ");
  char* httpVersion = strtok(null, " ");

  strcpy(r->method, method);
  strcpy(r->path, path);
  strcpy(r->http_version, httpVersion);

  headers_parse(r->headers, &ctx);

  request_parse_body(r, ctx);
}

void request_print(Request* r) {
  printf("Request:\n");
  printf("\tMethod: %s\n", r->method);
  printf("\tPath: %s\n", r->path);
  printf("\tVersion: %s\n", r->http_version);
  printf("\tHeaders:\n");
  headers_print(r->headers);
  printf("\tBody: %s\n", r->body);
}