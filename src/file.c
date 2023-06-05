#include "file.h"
#include <stdlib.h>

void serve_file(const char* path, Request* req, Response* res) {
  FILE* file = fopen(path, "rb");

  if (file == null) {
    printf("File not found: %s\n", path);
    response_set_status(res, 404, "Not Found");
    headers_add(res->headers, "Content-Type", "text/html");
    response_set_body(res, "<h1>404 Not Found</h1>");
    return;
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* buffer = (char*)malloc(size + 1);
  fread(buffer, 1, size, file);
  buffer[size] = '\0';

  fclose(file);

  response_set_status(res, 200, "OK");
  headers_add(res->headers, "Content-Type", mime_type(path));
  response_set_body(res, buffer);

  free(buffer);
}

bool open_file(const char* path, FILE** file) {
  *file = fopen(path, "rb");

  if (*file == null)
    return false;

  return true;
}

size_t file_size(FILE* file) {
  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  fseek(file, 0, SEEK_SET);

  return size;
}

void readfile(FILE* file, char* buffer, size_t size) {
  fread(buffer, 1, size, file);
}