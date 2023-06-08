#pragma once

#include <headers.h>

typedef struct _server_options {
  Headers* headers;
} ServerOptions;

typedef struct _cors_options {
  const char *origin, *methods, *headers, *max_age, *credentials;
} CorsOptions;

#define CORSPRE "Access-Control-"
#define CORS_ORIGIN CORSPRE "Allow-Origin"
#define CORS_METHODS CORSPRE "Allow-Methods"
#define CORS_HEADERS CORSPRE "Allow-Headers"
#define CORS_MAX_AGE CORSPRE "Max-Age"
#define CORS_CREDENTIALS CORSPRE "Allow-Credentials"

ServerOptions* server_options_new();
void server_options_free(ServerOptions* options);

CorsOptions* cors_options_new();
void cors_options_free(CorsOptions* cors);

void use_default_headers(ServerOptions* options, Headers* headers);
void use_cors(ServerOptions* options, CorsOptions* cors);
