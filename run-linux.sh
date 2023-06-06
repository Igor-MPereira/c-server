gcc -o ./server main.c \
  src/http.c \
  src/request.c \
  src/response.c \
  src/hash.c \
  src/headers.c \
  src/string.c \
  src/route.c \
  src/file.c \
  -ggdb3 \
  -std=gnu99 \
  -Wall -Wextra
# -O3
