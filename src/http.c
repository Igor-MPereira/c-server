#include "http.h"
#include <stdio.h>
#include "route.h"

SOCKET listen_socket(int port) {
  SOCKET sSock;
  t_sockaddr_in addr;

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

  if (bind(sSock, (t_sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
    perror("Failed to bind socket.\n");
    closesocket(sSock);
    WSACleanup();
    return INVALID_SOCKET;
  }

  if (listen(sSock, SOMAXCONN) == SOCKET_ERROR) {
    perror("Failed to listen on socket.\n");
    closesocket(sSock);
    WSACleanup();
    return INVALID_SOCKET;
  }

  return sSock;
}

SOCKET accept_socket(SOCKET sSock) {
  SOCKET cSock;
  t_sockaddr_in addr;
  int addr_size = sizeof(addr);

  cSock = accept(sSock, (t_sockaddr*)&addr, &addr_size);
  if (cSock == INVALID_SOCKET) {
    perror("Failed to accept socket.\n");
    closesocket(sSock);
    WSACleanup();
    return INVALID_SOCKET;
  }

  return cSock;
}

Request* recv_request(SOCKET cSock) {
  Request* request = request_new();
  char buffer[BUFFER_SIZE];
  int bytes_received;

  bytes_received = recv(cSock, buffer, BUFFER_SIZE, 0);

  if (bytes_received == SOCKET_ERROR) {
    perror("Failed to receive data.\n");
    closesocket(cSock);
    WSACleanup();
    return NULL;
  }

  printf("Received %d bytes.\n\n", bytes_received);

  request_parse(request, buffer);

  return request;
}

int send_response(SOCKET cSock, Response* response) {
  char* string;
  size_t size;

  response_stringify(response, &string, &size);

  if (send(cSock, string, size, 0) == SOCKET_ERROR) {
    perror("Failed to send data.\n");
    closesocket(cSock);
    WSACleanup();
    return 1;
  }

  free(string);

  return 0;
}

SOCKET http_server(int port, void (*onload)(), bool cors) {
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

    closesocket(cSock);

    request_destroy(request);
    response_destroy(response);
  }

  closesocket(sSock);
  WSACleanup();

  return 0;
}

void enable_cors(bool enable, Headers* headers) {
  if (enable) {
    headers_set(headers, "Access-Control-Allow-Origin", "*");
  }
}