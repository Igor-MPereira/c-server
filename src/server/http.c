#include <server/http.h>

#include <stdio.h>
#include <stdlib.h>

#include <route.h>
#include <server/sockets.h>
#include <utils/defs.h>

bool recv_request(SOCKET cSock, Request* request) {
  char buffer[REQ_MAXSIZE] = {0};

  size_t bytes_received = recv(cSock, buffer, REQ_MAXSIZE, 0);

  printf("Received %ld bytes.\n\n", bytes_received);

  if (bytes_received <= 0)
    return false;

  request_parse(request, buffer);

  return true;
}

bool send_response(SOCKET cSock, Response* response) {
  char* string;
  size_t size;

  response_stringify(response, &string, &size);

  if (send(cSock, string, size, 0) != SOCKET_ERROR) {
    free(string);
    return true;
  }

  perror("Failed to send data.\n");
  free(string);
  close(cSock);
  WSACleanup();
  return false;
}

SOCKET http_server(u16 port, ServerOptions* opt, void (*onload)(u16)) {
  SOCKET sSock = listen_socket(port);

  if (sSock == INVALID_SOCKET)
    return INVALID_SOCKET;

  onload(port);

  while (1) {
    SOCKET cSock = accept_socket(sSock);

    if (cSock == INVALID_SOCKET) {
      perror("Failed to accept socket.\n");
      continue;
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

    headers_merge(response->headers, opt->headers);

    router_handle_request(request, response);

    send_response(cSock, response);

    close(cSock);

    request_free(request);
    response_free(response);
  }

  close(sSock);
  WSACleanup();

  return 0;
}