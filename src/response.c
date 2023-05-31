#include "response.h"

Response* response_new() {
  Response* response = malloc(sizeof(Response));

  response->status_code = 0;
  response->status_text[0] = '\0';
  response->headers = headers_new();
  response->body = NULL;

  return response;
}

void response_destroy(Response* response) {
  headers_destroy(response->headers);
  free(response->body);
  free(response);
}

void response_print(Response* response) {
  printf("Response:\n");
  printf("  status_code: %d\n", response->status_code);
  printf("  status_text: %s\n", response->status_text);
  printf("  headers:\n");
  headers_print(response->headers);
  printf("  body: %s\n", response->body);
}

void response_parse(Response* response, char* string) {
  char* token;
  char* context;

  token = strtok_s(string, " ", &context);
  if (token == NULL)
    return;
  response->status_code = atoi(token);

  token = strtok_s(NULL, " ", &context);
  if (token == NULL)
    return;
  strncpy_s(response->status_text, 16, token, _TRUNCATE);

  token = strtok_s(NULL, "\r\n", &context);
  if (token == NULL)
    return;

  headers_parse(response->headers, token);

  token = strtok_s(NULL, "\r\n", &context);
  if (token == NULL)
    return;

  response->body = malloc(strlen(token) + 1);
  strcpy_s(response->body, strlen(token) + 1, token);
}

char* response_stringify(Response* response) {
  char* headers;
  size_t size = response_size(response, &headers);
  char* string = malloc(size);

  sprintf_s(string, size, "HTTP/1.1 %d %s\r\n%s\r\n%s", response->status_code,
            response->status_text, headers, response->body);

  free(headers);

  return string;
}

size_t response_size(Response* response, char** headers) {
  *headers = headers_stringify(response->headers);
  return SIZEOF_VERSION + SIZEOF_STATUS_CODE + strlen(response->status_text) +
         SIZEOF_NEWLINE + strlen(headers) + SIZEOF_NEWLINE +
         strlen(response->body) + 1;
}