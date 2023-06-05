#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "hashed_file_extensions.h"
#include "string.h"

Headers* headers_new() {
  Headers* h = (Headers*)malloc(sizeof(Headers));
  h->first = null;
  h->last = null;
  return h;
}

void headers_destroy(Headers* h) {
  Header* header = h->first;

  while (header) {
    Header* next = header->next;

    free(header->key);
    free(header->value);
    free(header);

    header = next;
  }

  free(h);
}

void headers_add(Headers* h, const char* key, const char* value) {
  Header* header = (Header*)malloc(sizeof(Header));
  header->key = strdup(key);
  header->value = strdup(value);
  header->next = null;
  header->prev = null;

  if (!h->first) {
    h->first = header;
    h->last = header;
  } else {
    h->last->next = header;
    header->prev = h->last;
    h->last = header;
  }
}

void headers_remove(Headers* h, char* key) {
  Header* header = h->first;

  while (header) {
    if (streq(header->key, key)) {
      if (header->prev)
        header->prev->next = header->next;
      else
        h->first = header->next;

      if (header->next)
        header->next->prev = header->prev;
      else
        h->last = header->prev;

      free(header->key);
      free(header->value);
      free(header);

      return;
    }

    header = header->next;
  }
}

char* headers_get(Headers* h, char* key) {
  Header* header = h->first;

  while (header) {
    if (streq(header->key, key))
      return header->value;

    header = header->next;
  }

  return null;
}

bool headers_has(Headers* h, char* key) {
  Header* header = h->first;

  while (header) {
    if (streq(header->key, key))
      return true;

    header = header->next;
  }

  return false;
}

void headers_set(Headers* h, char* key, char* value) {
  Header* header = h->first;

  while (header) {
    if (streq(header->key, key)) {
      free(header->value);
      header->value = value;
      return;
    }

    header = header->next;
  }

  headers_add(h, key, value);
}

void header_print(Header* header) {
  printf("\t\t%s: %s\n", header->key, header->value);
}

void headers_print(Headers* h) {
  Header* header = h->first;

  while (header) {
    header_print(header);
    header = header->next;
  }
}

char* headers_stringify(Headers* h) {
  char* string = (char*)malloc(HEADER_MAX_SIZE);
  Header* header = h->first;

  string[0] = '\0';
  while (header) {
    strcat_s(string, HEADER_MAX_SIZE, header->key);
    strcat_s(string, HEADER_MAX_SIZE, ": ");
    strcat_s(string, HEADER_MAX_SIZE, header->value);
    strcat_s(string, HEADER_MAX_SIZE, "\r\n");
    header = header->next;
  }

  return string;
}

const char* mime_type(const char* path) {
  const char* last_dot = strrchr(path, '.');

  if (!last_dot)
    return "text/plain";

  switch (hash(last_dot + 1)) {
    case HTML:
      return "text/html";
    case CSS:
      return "text/css";
    case JS:
    case MJS:
    case CJS:
      return "application/javascript";
    case PNG:
      return "image/png";
    case JPG:
    case JPEG:
      return "image/jpeg";
    case ICO:
      return "image/x-icon";
    case GIF:
      return "image/gif";
    case SVG:
      return "image/svg+xml";
    case TXT:
      return "text/plain";
    case WEBP:
      return "image/webp";
    case JSON:
      return "application/json";
    default:
      return "application/octet-stream";
  }
}

void escprintf(const char* str) {
  while (*str) {
    switch (*str) {
      case '\n':
        printf("\\n");
        break;
      case '\r':
        printf("\\r");
        break;
      case '\t':
        printf("\\t");
        break;
      default:
        printf("%c", *str);
        break;
    }

    str++;
  }

  printf("\n");
}

void headers_parse(Headers* h, char** ctx) {
  char* line_ctx;
  char* line = strtok_s(*ctx, "\r\n", ctx);

  while (line) {
    char* key = strtok_s(line, ": ", &line_ctx);
    char* value = strtok(++line_ctx, "\r\n");

    headers_add(h, key, value);

    // removes leading \n
    (*ctx)++;

    if (strneq(*ctx, "\r\n", 2))
      break;

    line = strtok_s(*ctx, "\r\n", ctx);
  }
}