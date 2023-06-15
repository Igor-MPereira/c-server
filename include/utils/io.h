#pragma once

#include <stdio.h>
#include <time.h>

#define timeprintf(format, ...)                 \
  {                                             \
    time_t t = time(null);                      \
    struct tm* tm = localtime(&t);              \
    char buf[64];                               \
    strftime(buf, sizeof(buf), "%c", tm);       \
    printf("[%s] " format, buf, ##__VA_ARGS__); \
  }

void escprintf(char* str);
