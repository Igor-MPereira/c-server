#pragma once

#include "request.h"
#include "response.h"
#include "stdio.h"

void serve_file(const char* path, Request* request, Response* response);

bool open_file(const char* path, FILE** file);

size_t file_size(FILE* file);

void readfile(FILE* file, char* buffer, size_t size);
