#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#define _GNU_SOURCE
#include <string.h>
#include "string.h"

Request* request_new() {
  Request* r = (Request*)malloc(sizeof(Request));
  r->headers = headers_new();
  r->body = null;

  return r;
}

void request_free(Request* r) {
  headers_free(r->headers);
  free(r->body);
  free(r);
}

void request_parse_body(Request* r, char* ctx) {
  char* contentLength = headers_get(r->headers, "Content-Length");

  if (contentLength) {
    i32 length = atoi(contentLength);
    r->body = (char*)malloc(length + 1);
    ctx += 2;
    strcpy(r->body, ctx);
    printf("ctx: %s\n", ctx);
    printf("length: %d\n", length);
    printf("body: %s\n", r->body);
    printf("strlen(body): %ld\n", strlen(r->body));
    printf("strlen(ctx): %ld\n", strlen(ctx));
    printf("streq(body, ctx): %d\n", streq(r->body, ctx));
  }
}

void request_parse(Request* r, char* reqStr) {
  printf("Request string: %s\n", reqStr);

  char* ctx;
  char* line = strtok_r(reqStr, "\r\n", &ctx);
  char* method = strtok(line, " ");
  char* path = strtok(null, " ");
  char* httpVersion = strtok(null, " ");

  strcpy(r->method, method);
  strcpy(r->path, path);
  strcpy(r->http_version, httpVersion);

  headers_parse(r->headers, &ctx);

  request_parse_body(r, ctx);
  request_print(r);
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