#pragma once

#include "request.h"
#include "response.h"
#include "stdio.h"

void serve_file(const char* path, Request* request, Response* response);