#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include "route.h"
#include "util.h"

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
    WSACleanup();
    return INVALID_SOCKET;
  }

  return cSock;
}

bool recv_request(SOCKET cSock, Request* request) {
  char buffer[BUFFER_SIZE] = {0};

  size_t bytes_received = recv(cSock, buffer, BUFFER_SIZE, 0);

  printf("Received %ld bytes.\n\n", bytes_received);

  if (bytes_received <= 0)
    return false;

  request_parse(request, buffer);

  return true;
}

SOCKET http_server(u16 port, void (*onload)(u16), bool cors) {
  SOCKET sSock = listen_socket(port);

  if (sSock == INVALID_SOCKET)
    return INVALID_SOCKET;

  onload(port);

  while (1) {
    SOCKET cSock = accept_socket(sSock);

    if (cSock == INVALID_SOCKET) {
      perror("Failed to accept socket.\n");
      return 1;
    }

    printf("Accepted connection.\n\n");

    Request* request = request_new();

    if (!recv_request(cSock, request)) {
      perror("Failed to receive request.\n");
      request_free(request);
      close(cSock);
      continue;
    }

    Response* response = response_new();

    router_handle_request(request, response);

    enable_cors(cors, response->headers);

    response_send(cSock, response);

    close(cSock);

    request_free(request);
    response_free(response);
  }

  close(sSock);
  WSACleanup();

  return 0;
}

void enable_cors(bool enable, Headers* headers) {
  if (enable)
    headers_set(headers, "Access-Control-Allow-Origin", "*");
}