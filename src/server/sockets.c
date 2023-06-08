#include <server/sockets.h>

#include <stdio.h>

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