#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define MAX_BUFFER_SIZE 4096

typedef enum { false, true } bool;
typedef struct sockaddr t_sockaddr;
typedef struct sockaddr_in t_sockaddr_in;

#define ADDRINSIZE sizeof(t_sockaddr_in)

#define SIZEVERSION 20
#define SIZEMETHOD 10
#define SIZEPATH 100

typedef struct {
  char method[SIZEMETHOD];
  char path[SIZEPATH];
  char httpVersion[SIZEVERSION];
} HttpRequest;
typedef struct {
  int statusCode;
  char statusText[100];
  char contentType[100];
  char content[2048];
} HttpResponse;

bool streq(char* s1, char* s2) { return strcmp(s1, s2) == 0; }

int parseRequest(char* request, HttpRequest* req) {
  char* token;
  char* context;

  token = strtok_s(request, " ", &context);
  if (token == NULL) return 0;
  strncpy_s(req->method, SIZEMETHOD, token, _TRUNCATE);

  token = strtok_s(NULL, " ", &context);
  if (token == NULL) return 0;
  strncpy_s(req->path, SIZEPATH, token, _TRUNCATE);

  token = strtok_s(NULL, "\r\n", &context);
  if (token == NULL) return 0;
  strncpy_s(req->httpVersion, SIZEVERSION, token, _TRUNCATE);

  return 1;
}

int main() {
  WSADATA wsaData;
  SOCKET sSock, cSock;
  t_sockaddr_in sAddr, cAddr;
  int cAddrLen;
  char reqBuffer[MAX_BUFFER_SIZE], fileBuffer[MAX_BUFFER_SIZE], *res;
  int bytesRead;
  HttpRequest req;
  FILE* fp;

  // Initialize Winsock
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    printf("Failed to initialize Winsock.\n");
    return 1;
  }

  // Create server socket
  sSock = socket(AF_INET, SOCK_STREAM, 0);
  if (sSock == INVALID_SOCKET) {
    printf("Failed to create server socket.\n");
    WSACleanup();
    return 1;
  }

  // Bind server socket to a port
  sAddr.sin_family = AF_INET;
  sAddr.sin_addr.s_addr = INADDR_ANY;
  sAddr.sin_port = htons(8080);
  if (bind(sSock, (t_sockaddr*)&sAddr, ADDRINSIZE) == SOCKET_ERROR) {
    printf("Failed to bind server socket.\n");
    closesocket(sSock);
    WSACleanup();
    return 1;
  }

  // Start listening for incoming connections
  if (listen(sSock, SOMAXCONN) == SOCKET_ERROR) {
    printf("Failed to listen on server socket.\n");
    closesocket(sSock);
    WSACleanup();
    return 1;
  }

  printf("Server running on port http://127.0.0.1:8080...\n");

  // Accept and handle incoming connections
  while (1) {
    cAddrLen = sizeof(cAddr);
    cSock = accept(sSock, (t_sockaddr*)&cAddr, &cAddrLen);

    if (cSock == INVALID_SOCKET) {
      printf("Failed to accept client connection.\n");
      closesocket(sSock);
      WSACleanup();
      return 1;
    }

    // Read the client's request
    memset(reqBuffer, 0, sizeof(reqBuffer));
    bytesRead = recv(cSock, reqBuffer, sizeof(reqBuffer) - 1, 0);

    if (bytesRead == SOCKET_ERROR) {
      printf("Failed to receive client request.\n");
      closesocket(cSock);
      continue;
    }

    // Print the client's request
    printf("Received request:\n%s\n", reqBuffer);

    // Parse the request
    if (parseRequest(reqBuffer, &req)) {
      printf("Method: %s\n", req.method);
      printf("Path: %s\n", req.path);
      printf("HTTP Version: %s\n", req.httpVersion);

      if (streq(req.path, "/") || streq(req.path, "/index.html")) {
        fp = fopen("static/index.html", "r");
        if (fp == NULL) {
          printf("Failed to open static/index.html file.\n");
          return 1;
        }
      }
    } else {
      printf("Failed to parse the request.\n");
    }

    // Send a simple HTTP response
    res =
        "HTTP/1.1 200 OK\r\nContent-Length: 13\r\nContent-Type: "
        "text/plain\r\n\r\nHello, World!";
    send(cSock, res, strlen(res), 0);

    // Close the client socket
    closesocket(cSock);
  }

  // Clean up Winsock
  closesocket(sSock);
  WSACleanup();

  return 0;
}
