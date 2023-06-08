#include <server/options.h>

#include <headers.h>
#include <utils/memory.h>

ServerOptions* server_options_new() {
  ServerOptions* opt = memalloc(sizeof(ServerOptions));

  opt->headers = headers_new();

  return opt;
}

void server_options_free(ServerOptions* opt) {
  headers_free(opt->headers);
  memfree(opt);
}

CorsOptions* cors_options_new() {
  CorsOptions* cors = memalloc(sizeof(CorsOptions));

  cors->origin = null;
  cors->methods = null;
  cors->headers = null;
  cors->max_age = null;
  cors->credentials = null;

  return cors;
}

void cors_options_free(CorsOptions* cors) {
  memfree(cors);
}

void use_default_headers(ServerOptions* opt, Headers* headers) {
  headers_merge(opt->headers, headers);
}

void use_cors(ServerOptions* opt, CorsOptions* cors) {
  if (cors->origin)
    headers_set(opt->headers, CORS_ORIGIN, (char*)cors->origin);

  if (cors->methods)
    headers_set(opt->headers, CORS_METHODS, (char*)cors->methods);

  if (cors->headers)
    headers_set(opt->headers, CORS_HEADERS, (char*)cors->headers);

  if (cors->max_age)
    headers_set(opt->headers, CORS_MAX_AGE, (char*)cors->max_age);

  if (cors->credentials)
    headers_set(opt->headers, CORS_CREDENTIALS, (char*)cors->credentials);
}
