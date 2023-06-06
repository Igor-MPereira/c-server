#pragma once

#pragma region Cross-Platform Compatibility

#ifdef _WIN32
#include <winsock2.h>
#define win_WSAStart()                             \
  WSADATA wsaData;                                 \
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { \
    perror("Failed to initialize Winsock.\n");     \
    return 1;                                      \
  }

// Defining linux types, functions and macros for winsock2

#define close closesocket
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// Defining winsock2 types, functions and macros for linux

#define WSACleanup()
#define win_WSAStart()

typedef int SOCKET;
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#endif

#pragma endregion

#include <request.h>
#include <response.h>

SOCKET listen_socket(u16 port);

SOCKET accept_socket(SOCKET sSock);

bool recv_request(SOCKET cSock, Request* request);

SOCKET http_server(u16 port, void (*onload)(u16), bool cors);

void enable_cors(bool enable, Headers* headers);
