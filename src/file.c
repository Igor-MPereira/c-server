#include <file.h>

#include <stdio.h>

#include <utils/memory.h>
#include <utils/string.h>

void join_path(const char* base, const char* path, char* buffer, size_t size) {
  strncpy(buffer, base, size);

  size_t lastidx = strlen(buffer) - 1;

  if (buffer[lastidx] == PATHSEP && path[0] == PATHSEP) {
    strncat(buffer, path + 1, size);
    return;
  }

  if (buffer[lastidx] != PATHSEP && path[0] != PATHSEP)
    strncat(buffer, PATHSEPSTR, size);

  strncat(buffer, path, size);
}

void resolve_path(const char* base,
                  const char* path,
                  char* buffer,
                  size_t size) {
  char first = path[0];

  if (first == '/') {
    strncpy(buffer, path, size);
    return;
  }

  if (first == '~') {
    join_path(getenv("HOME"), path + 1, buffer, size);
    return;
  }

  join_path(base, path, buffer, size);
}

void send_file(const char* path,
               Request* _ __attribute_maybe_unused__,
               Response* res) {
  char fullpath[300] = {0};

  resolve_path(__ROOT_DIR__, path, fullpath, 300);
  FILE* file = fopen(fullpath, "rb");

  if (file == null) {
    printf("File not found: %s\n", fullpath);
    response_set_status(res, 404, "Not Found");
    headers_add(res->headers, "Content-Type", "text/html");
    response_set_body(res, "<h1>404 Not Found!</h1>");
    return;
  }

  size_t size = file_size(file);

  char* buffer = (char*)memalloc(size + 1);

  readfile(file, buffer, size);
  buffer[size] = '\0';

  fclose(file);

  response_set_status(res, 200, "OK");
  headers_add(res->headers, "Content-Type", mime_type(fullpath));
  response_set_body(res, buffer);

  memfree(buffer);
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