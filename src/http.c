#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include "route.h"

SOCKET listen_socket(u16 port) {
  SOCKET sSock;

  SOCKADDR_IN addr;

  win_WSAStart();

  sSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sSock == INVALID_SOCKET) {
    perror("Failed to create socket.\n");
    WSACleanup();
    return INVALID_SOCKET;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sSock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
    perror("Failed to bind socket.\n");
    close(sSock);
    WSACleanup();
    return INVALID_SOCKET;
  }

  if (listen(sSock, SOMAXCONN) == SOCKET_ERROR) {
    perror("Failed to listen on socket.\n");
    close(sSock);
    WSACleanup();
    return INVALID_SOCKET;
  }

  return sSock;
}

SOCKET accept_socket(SOCKET sSock) {
  SOCKET cSock;
  SOCKADDR_IN addr;
  u32 addr_size = sizeof(addr);

  cSock = accept(sSock, (SOCKADDR*)&addr, &addr_size);
  if (cSock == INVALID_SOCKET) {
    perror("Failed to accept socket.\n");
    close(sSock);
    WSACleanup();
    return INVALID_SOCKET;
  }

  return cSock;
}

Request* recv_request(SOCKET cSock) {
  Request* request = request_new();
  char buffer[BUFFER_SIZE];
  ssize_t bytes_received;

  bytes_received = recv(cSock, buffer, BUFFER_SIZE, 0);

  if (bytes_received == SOCKET_ERROR) {
    perror("Failed to receive data.\n");
    close(cSock);
    WSACleanup();
    return NULL;
  }

  printf("Received %ld bytes.\n\n", bytes_received);

  request_parse(request, buffer);

  return request;
}

int send_response(SOCKET cSock, Response* response) {
  char* string;
  size_t size;

  response_stringify(response, &string, &size);

  if (send(cSock, string, size, 0) == SOCKET_ERROR) {
    perror("Failed to send data.\n");
    close(cSock);
    WSACleanup();
    return 1;
  }

  free(string);

  return 0;
}

SOCKET http_server(u16 port, void (*onload)(), bool cors) {
  SOCKET sSock = listen_socket(port);

  if (sSock == INVALID_SOCKET) {
    perror("Failed to create socket.\n");
    return 1;
  }

  onload();

  while (1) {
    SOCKET cSock = accept_socket(sSock);

    if (cSock == INVALID_SOCKET) {
      perror("Failed to accept socket.\n");
      return 1;
    }

    printf("Accepted connection.\n\n");

    Request* request = recv_request(cSock);

    if (!request) {
      perror("Failed to receive request.\n");
      return 1;
    }

    Response* response = response_new();

    router_route(request, response);

    enable_cors(cors, response->headers);

    send_response(cSock, response);

    close(cSock);

    request_destroy(request);
    response_destroy(response);
  }

  close(sSock);
  WSACleanup();

  return 0;
}

void enable_cors(bool enable, Headers* headers) {
  if (enable) {
    headers_set(headers, "Access-Control-Allow-Origin", "*");
  }
}