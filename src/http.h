#pragma once

#ifdef _WIN32
#include <winsock2.h>
#define win_WSAStart()                             \
  WSADATA wsaData;                                 \
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { \
    perror("Failed to initialize Winsock.\n");     \
    return 1;                                      \
  }
#else
#include <sys/socket.h>
#define WSACleanup()
#define win_WSAStart()
#endif

#include "request.h"
#include "response.h"

typedef struct sockaddr t_sockaddr;
typedef struct sockaddr_in t_sockaddr_in;

SOCKET listen_socket(int port);

SOCKET accept_socket(SOCKET sSock);

Request* recv_request(SOCKET cSock);

int send_response(SOCKET cSock, Response* response);

SOCKET http_server(int port, void (*onload)());