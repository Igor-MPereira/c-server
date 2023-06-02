#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Response* response_new() {
  Response* response = malloc(sizeof(Response));

  response->status_code = 0;
  response->status_text[0] = '\0';
  response->headers = headers_new();
  response->body = null;

  return response;
}

void response_destroy(Response* response) {
  headers_destroy(response->headers);
  free(response->body);
  free(response);
}

void response_print(Response* response) {
  printf("Response:\n");
  printf("\tStatusCode: %d\n", response->status_code);
  printf("\tStatusText: %s\n", response->status_text);
  printf("\tHeaders:\n");
  headers_print(response->headers);
  printf("\tBody: \n\t\t%s\n", response->body);
}

char* response_stringify(Response* response, char** string, size_t* size) {
  char* headers;
  *size = response_size(response, &headers);
  *string = malloc(*size);

  sprintf_s(*string, *size, "HTTP/1.1 %d %s\r\n%s\r\n%s", response->status_code,
            response->status_text, headers, response->body);

  free(headers);

  return *string;
}

size_t response_size(Response* response, char** headers) {
  *headers = headers_stringify(response->headers);
  return SIZEOF_VERSION + SIZEOF_STATUS_CODE + strlen(response->status_text) +
         SIZEOF_NEWLINE + strlen(*headers) + SIZEOF_NEWLINE +
         strlen(response->body) + 1;
}

void response_set_body(Response* response, char* body) {
  size_t size = strlen(body);
  char* con_len = malloc(7);
  free(response->body);
  response->body = malloc(size + 1);
  strcpy_s(response->body, size + 1, body);
  sprintf_s(con_len, 7, "%d", size);
  headers_set(response->headers, "Content-Length", con_len);
}

void response_set_status(Response* response,
                         int status_code,
                         char* status_text) {
  response->status_code = status_code;
  strcpy_s(response->status_text, 16, status_text);
}