#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUFFER_SIZE 1048576

typedef struct sockaddr t_sockaddr;
typedef struct sockaddr_in t_sockaddr_in;

#define ADDRINSIZE sizeof(t_sockaddr_in)

#define SIZEVERSION 20
#define SIZEMETHOD 10
#define SIZEPATH 100
#define SIZECONTENT 100

typedef struct {
  char method[SIZEMETHOD];
  char path[SIZEPATH];
  char httpVersion[SIZEVERSION];
  char content[BUFFER_SIZE];
  char contentType[100];
  char headers[BUFFER_SIZE];
} HttpRequest;

typedef struct {
  int statusCode;
  char statusText[100];
  char contentType[100];
  char content[BUFFER_SIZE];
} HttpResponse;

int init_server(SOCKET* sSock);
int init_client(SOCKET* cSock, SOCKET* sSock);
void close_server(SOCKET* sSock);

bool streq(char* s1, char* s2) { return strcmp(s1, s2) == 0; }

bool parse_request(char* request, HttpRequest* req) {
  char* token;
  char* context;

  token = strtok_s(request, " ", &context);
  if (token == NULL) return false;
  strncpy_s(req->method, SIZEMETHOD, token, _TRUNCATE);

  token = strtok_s(NULL, " ", &context);
  if (token == NULL) return false;
  strncpy_s(req->path, SIZEPATH, token, _TRUNCATE);

  token = strtok_s(NULL, "\r\n", &context);
  if (token == NULL) return false;
  strncpy_s(req->httpVersion, SIZEVERSION, token, _TRUNCATE);

  return 1;
}

bool is_static(HttpRequest* req) {
  return streq(req->method, "GET") && strncmp(req->path, "/static/", 8) == 0;
}

int main() {
  SOCKET sSock, cSock;
  char reqBuffer[BUFFER_SIZE], fileBuffer[BUFFER_SIZE], *res;
  int bytesRead;
  HttpRequest req;
  FILE* fp;

  if (init_server(&sSock)) {
    perror("webserver (init)\n");
    return 1;
  }

  while (1) {
    if (init_client(&cSock, &sSock)) return 1;

    if (receive_request(&cSock, reqBuffer)) return 1;

    printf("Received request:\n%s\n", reqBuffer);

    if (parse_request(reqBuffer, &req)) {
      printf("Method: %s\n", req.method);
      printf("Path: %s\n", req.path);
      printf("HTTP Version: %s\n", req.httpVersion);

      if (streq(req.method, "GET") && strncmp(req.path, "/static/", 8) == 0) {
        fp = fopen(req.path + 1, "rb");
        if (fp == NULL) {
          printf("Failed to open file.\n");
          continue;
        }

        memset(fileBuffer, 0, sizeof(fileBuffer));
        fread(fileBuffer, sizeof(char), sizeof(fileBuffer) - 1, fp);
        fclose(fp);

        // Send the file
        res = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        send(cSock, res, strlen(res), 0);
        send(cSock, fileBuffer, strlen(fileBuffer), 0);
        closesocket(cSock);
        continue;
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

int init_server(SOCKET* sSock) {
  WSADATA wsaData;
  t_sockaddr_in sAddr;

  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    perror("Failed to initialize Winsock.\n");
    return 1;
  }

  *sSock = socket(AF_INET, SOCK_STREAM, 0);
  if (*sSock == INVALID_SOCKET) {
    perror("Failed to create server socket.\n");
    WSACleanup();
    return 1;
  }

  sAddr.sin_family = AF_INET;
  sAddr.sin_addr.s_addr = INADDR_ANY;
  sAddr.sin_port = htons(8080);

  if (bind(*sSock, (t_sockaddr*)&sAddr, ADDRINSIZE) == SOCKET_ERROR) {
    perror("Failed to bind server socket.\n");
    close_server(sSock);
    return 1;
  }

  if (listen(*sSock, SOMAXCONN) == SOCKET_ERROR) {
    perror("Failed to listen on server socket.\n");
    close_server(sSock);
    return 1;
  }

  printf("Server running on port http://localhost:8080...\n");

  return 0;
}

void close_server(SOCKET* sSock) {
  closesocket(*sSock);
  WSACleanup();
}

int init_client(SOCKET* cSock, SOCKET* sSock) {
  t_sockaddr_in cAddr;
  int cAddrLen = sizeof(cAddr);

  *cSock = accept(*sSock, (t_sockaddr*)&cAddr, &cAddrLen);

  if (*cSock == INVALID_SOCKET) {
    perror("webserver (accept)");
    close_server(sSock);
    return 1;
  }

  return 0;
}

int receive_request(SOCKET* cSock, char* reqBuffer) {
  int bytesRead = recv(*cSock, reqBuffer, BUFFER_SIZE, 0);

  if (bytesRead == SOCKET_ERROR) {
    perror("webserver (recv)");
    closesocket(*cSock);
    return 1;
  }

  reqBuffer[bytesRead] = '\0';

  return 0;
}