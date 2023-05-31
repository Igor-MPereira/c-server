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

typedef struct sockaddr t_sockaddr;
typedef struct sockaddr_in t_sockaddr_in;

int listen_socket(SOCKET* sSock, int port);
