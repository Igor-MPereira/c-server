#pragma once

#include <server/core.h>
#include <utils/types.h>

SOCKET listen_socket(u16 port);

SOCKET accept_socket(SOCKET sSock);