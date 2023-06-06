#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define timeprintf(format, ...)                 \
  do {                                          \
    time_t t = time(null);                      \
    struct tm* tm = localtime(&t);              \
    char buf[64];                               \
    strftime(buf, sizeof(buf), "%c", tm);       \
    printf("[%s] " format, buf, ##__VA_ARGS__); \
  } while (0)