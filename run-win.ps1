gcc -o server.exe `
  main.c `
  src/http.c `
  .\src\request.c `
  .\src\response.c `
  .\src\hash.c `
  .\src\headers.c `
  .\src\string.c `
  -lws2_32 `
  -Wall -Wextra `
  -std=c99 `
  -O3

./server 8080