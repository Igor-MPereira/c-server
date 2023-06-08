#pragma once

#include <request.h>
#include <response.h>
#include <server/core.h>
#include <server/options.h>

bool recv_request(SOCKET cSock, Request* request);
bool send_response(SOCKET cSock, Response* response);

SOCKET http_server(u16 port, ServerOptions* opt, void (*onload)(u16));
