#pragma once
#include <stdio.h>

#include <request.h>
#include <response.h>

#ifdef _WIN32
#define PATHSEP '\\'
#define PATHSEPSTR "\\"
#else
#define PATHSEP '/'
#define PATHSEPSTR "/"
#endif

void join_path(const char* base, const char* path, char* buffer, size_t size);

void resolve_path(const char* base,
                  const char* path,
                  char* buffer,
                  size_t size);

void send_file(const char* path, Request* request, Response* response);

bool open_file(const char* path, FILE** file);

size_t file_size(FILE* file);

void readfile(FILE* file, char* buffer, size_t size);
