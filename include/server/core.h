#pragma once

// Include Main Header Files And Define Macros
// For HTTP Server of each Operating System

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