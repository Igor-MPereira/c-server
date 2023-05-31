#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUFFER_SIZE /* 2 ** 15 */ 32768

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
  char body[BUFFER_SIZE];
  char headers[BUFFER_SIZE];
} Request;

typedef struct {
  int statusCode;
  char statusText[100];
  char contentType[100];
  char body[BUFFER_SIZE];
} Response;

int init_server(SOCKET* sSock);
void close_server(SOCKET* sSock);
int init_client(SOCKET* cSock, SOCKET* sSock);
int receive_request(SOCKET* cSock, char* reqBuffer);
bool parse_request(char* reqBuffer, Request* req);
void print_request(Request* req);
bool is_static(Request* req);
void file_extension(char* path, char* extBuf);
char* mime_type(char* ext);
void fill_response(Response* res,
                   int statusCode,
                   char* statusText,
                   char* contentType,
                   char* body);
void print_response(Response* res);
void stringify_response(Response* res, char* resStr);

int main() {
  SOCKET sSock, cSock;
  char reqBuffer[BUFFER_SIZE], fileBuffer[BUFFER_SIZE], extBuf[10], *resStr;
  Request req;
  Response res;
  FILE* fp;

  if (init_server(&sSock)) {
    perror("webserver (init)\n");
    return 1;
  }

  while (1) {
    if (init_client(&cSock, &sSock))
      return 1;

    if (receive_request(&cSock, reqBuffer))
      return 1;

    printf("Received request:\n%s\n", reqBuffer);

    if (parse_request(reqBuffer, &req)) {
      print_request(&req);

      if (is_static(&req) || streq(req.path, "/")) {
        fp = fopen(req.path + 1, "rb");

        if (fp == NULL) {
          printf("Failed to open file.\n");
          continue;
        }

        file_extension(req.path, extBuf);

        memset(fileBuffer, 0, sizeof(fileBuffer));
        fread(fileBuffer, sizeof(char), sizeof(fileBuffer) - 1, fp);
        fclose(fp);

        fill_response(&res, 200, "OK", mime_type(extBuf), fileBuffer);

        print_response(&res);

        resStr = malloc(BUFFER_SIZE);
        stringify_response(&res, resStr);
        send(cSock, resStr, strlen(resStr), 0);
        free(resStr);
        closesocket(cSock);
        continue;
      }
    } else {
      printf("Failed to parse the request.\n");
    }

    // Send a simple HTTP response
    resStr =
        "HTTP/1.1 200 OK\r\nContent-Length: 13\r\nContent-Type: "
        "text/plain\r\n\r\nHello, World!";
    send(cSock, resStr, strlen(resStr), 0);

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

void print_request(Request* req) {
  printf("Method: %s\n", req->method);
  printf("Path: %s\n", req->path);
  printf("HTTP Version: %s\n", req->httpVersion);
  printf("Headers: %s\n", req->headers);
  printf("Content: %s\n", req->body);
}

void fill_response(Response* res,
                   int status,
                   char* statusText,
                   char* type,
                   char* body) {
  res->statusCode = status;
  strncpy_s(res->statusText, 100, statusText, _TRUNCATE);
  strncpy_s(res->contentType, 100, type, _TRUNCATE);
  strncpy_s(res->body, BUFFER_SIZE, body, _TRUNCATE);
}

void print_response(Response* res) {
  printf("Status Code: %d\n", res->statusCode);
  printf("Status Text: %s\n", res->statusText);
  printf("Content Type: %s\n", res->contentType);
  printf("Content: %s\n", res->body);
}

void stringify_response(Response* res, char* resStr) {
  char statusCode[4];
  sprintf_s(statusCode, 4, "%d", res->statusCode);

  strcpy_s(resStr, BUFFER_SIZE, "HTTP/1.1 ");
  strcat_s(resStr, BUFFER_SIZE, statusCode);
  strcat_s(resStr, BUFFER_SIZE, " ");
  strcat_s(resStr, BUFFER_SIZE, res->statusText);
  strcat_s(resStr, BUFFER_SIZE, "\r\nContent-Type: ");
  strcat_s(resStr, BUFFER_SIZE, res->contentType);
  strcat_s(resStr, BUFFER_SIZE, "\r\n\r\n");
  strcat_s(resStr, BUFFER_SIZE, res->body);

  printf("Response String:\n%s\n", resStr);
}

bool streq(char* s1, char* s2) {
  return strcmp(s1, s2) == 0;
}

bool parse_request(char* request, Request* req) {
  char* token;
  char* context;

  token = strtok_s(request, " ", &context);
  if (token == NULL)
    return false;
  strncpy_s(req->method, SIZEMETHOD, token, _TRUNCATE);

  token = strtok_s(NULL, " ", &context);
  if (token == NULL)
    return false;
  strncpy_s(req->path, SIZEPATH, token, _TRUNCATE);

  token = strtok_s(NULL, "\r\n", &context);
  if (token == NULL)
    return false;
  strncpy_s(req->httpVersion, SIZEVERSION, token, _TRUNCATE);

  req->headers[0] = '\0';

  while (token != NULL) {
    token = strtok_s(NULL, "\r\n", &context);
    if (token == NULL || strncmp(token, "\r\n", 2) == 0 ||
        (strncmp(token, "\n", 1) == 0) || (token[0] == '\0'))
      break;
    strncat_s(req->headers, strlen(token), token, _TRUNCATE);
    strncat_s(req->headers, strlen(token), "\r\n", _TRUNCATE);
  }

  req->body[0] = '\0';

  strncpy_s(req->body, BUFFER_SIZE, context, _TRUNCATE);

  return true;
}

void file_extension(char* path, char* extension) {
  char* token;
  char* context;

  token = strtok_s(path, ".", &context);
  if (token == NULL)
    return;

  token = strtok_s(NULL, ".", &context);
  if (token == NULL)
    return;

  strncpy_s(extension, 10, token, _TRUNCATE);
}

char* mime_type(char* extension) {
  if (streq(extension, "html") || streq(extension, "htm"))
    return "text/html";
  else if (streq(extension, "css"))
    return "text/css";
  else if (streq(extension, "js"))
    return "text/javascript";
  else if (streq(extension, "json"))
    return "application/json";
  else if (streq(extension, "jpg") || streq(extension, "jpeg"))
    return "image/jpeg";
  else if (streq(extension, "png"))
    return "image/png";
  else if (streq(extension, "gif"))
    return "image/gif";
  else if (streq(extension, "svg"))
    return "image/svg+xml";
  else if (streq(extension, "ico"))
    return "image/x-icon";
  else if (streq(extension, "txt"))
    return "text/plain";
  else
    return "application/octet-stream";
}

bool is_static(Request* req) {
  return streq(req->method, "GET") && strncmp(req->path, "/static/", 8) == 0;
}