#include <server/options.h>

#include <stdlib.h>

#include <headers.h>

ServerOptions* server_options_new() {
  ServerOptions* opt = malloc(sizeof(ServerOptions));

  opt->headers = headers_new();

  return opt;
}

void server_options_free(ServerOptions* opt) {
  headers_free(opt->headers);
  free(opt);
}

CorsOptions* cors_options_new() {
  CorsOptions* cors = malloc(sizeof(CorsOptions));

  cors->origin = null;
  cors->methods = null;
  cors->headers = null;
  cors->max_age = null;
  cors->credentials = null;

  return cors;
}

void cors_options_free(CorsOptions* cors) {
  free(cors);
}

void use_default_headers(ServerOptions* opt, Headers* headers) {
  headers_merge(opt->headers, headers);
}

void use_cors(ServerOptions* opt, CorsOptions* cors) {
  if (cors->origin)
    headers_set(opt->headers, CORSPRE "Allow-Origin", cors->origin);

  if (cors->methods)
    headers_set(opt->headers, CORSPRE "Allow-Methods", cors->methods);

  if (cors->headers)
    headers_set(opt->headers, CORSPRE "Allow-Headers", cors->headers);

  if (cors->max_age)
    headers_set(opt->headers, CORSPRE "Max-Age", cors->max_age);

  if (cors->credentials)
    headers_set(opt->headers, CORSPRE "Allow-Credentials", cors->credentials);
}
