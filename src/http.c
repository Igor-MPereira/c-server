#include "http.h"

int listen_socket(SOCKET* sSock, int port) {
  t_sockaddr_in sAddr;
  int ADDRINSIZE = sizeof(sAddr);

  win_WSAStart();

  *sSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (*sSock == INVALID_SOCKET) {
    perror("Failed to create server socket.\n");
    WSACleanup();
    return 1;
  }

  sAddr.sin_family = AF_INET;
  sAddr.sin_addr.s_addr = INADDR_ANY;
  sAddr.sin_port = htons(port);

  if (bind(*sSock, (t_sockaddr*)&sAddr, ADDRINSIZE) == SOCKET_ERROR) {
    perror("Failed to bind server socket.\n");
    closesocket(*sSock);
    return 1;
  }

  if (listen(*sSock, SOMAXCONN) == SOCKET_ERROR) {
    perror("Failed to listen on server socket.\n");
    closesocket(*sSock);
    return 1;
  }

  printf("Server running on port http://localhost:%d...\n", port);

  return 0;
}