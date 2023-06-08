#include <response.h>

#include <stdio.h>

#include <utils/memory.h>
#include <utils/string.h>

Response* response_new() {
  Response* response = memalloc(sizeof(Response));

  response->status_code = 0;
  response->status_text[0] = '\0';
  response->headers = headers_new();
  response->body = null;

  return response;
}

void response_free(Response* response) {
  headers_free(response->headers);
  memfree(response->body);
  memfree(response);
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
  *string = memalloc(*size);

  snprintf(*string, *size, "HTTP/1.1 %d %s\r\n%s\r\n%s", response->status_code,
           response->status_text, headers, response->body);

  memfree(headers);

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
  memfree(response->body);

  response->body = memalloc(size + 1);
  strncpy(response->body, body, size + 1);

  char* con_len = memalloc(20);
  snprintf(con_len, 20, "%ld", size);

  headers_set(response->headers, "Content-Length", con_len);

  memfree(con_len);
}

void response_set_status(Response* response,
                         u16 status_code,
                         char* status_text) {
  response->status_code = status_code;
  strncpy(response->status_text, status_text, 15);
}
