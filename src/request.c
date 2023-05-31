#include "request.h"

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

void request_parse_headers(Request* r, char* reqStr) {
  char* line = strtok(reqStr, "\r\n");

  while (line && !(line[0] == '\0' || line[0] == '\r' ||
                   (line[0] == '\n' && line[1] == '\0'))) {
    char* key = strtok(line, ":");
    char* value = strtok(null, ":");

    headers_add(r->headers, key, value);

    line = strtok(null, "\r\n");
  }
}

void request_parse_body(Request* r, char* reqStr) {
  char* contentLength = headers_get(r->headers, "Content-Length");

  if (contentLength) {
    int length = atoi(contentLength);
    r->body = (char*)malloc(length + 1);
    strcpy(r->body, strtok(null, "\r\n"));
  }
}

void request_parse(Request* r, char* reqStr) {
  char* line = strtok(reqStr, "\r\n");
  char* method = strtok(line, " ");
  char* path = strtok(null, " ");
  char* httpVersion = strtok(null, " ");

  strcpy(r->method, method);
  strcpy(r->path, path);
  strcpy(r->http_version, httpVersion);

  request_parse_headers(r, null);

  request_parse_body(r, null);
}

void request_print(Request* r) {
  printf("Method: %s\n", r->method);
  printf("Path: %s\n", r->path);
  printf("HTTP Version: %s\n", r->http_version);
  printf("Headers:\n");
  headers_print(r->headers);
  printf("Body: %s\n", r->body);
}