#include "headers.h"
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

void headers_add(Headers* h, char* key, char* value) {
  Header* header = (Header*)malloc(sizeof(Header));

  header->key = key;
  header->value = value;
  header->next = null;
  header->prev = null;

  if (h->first) {
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
      header->value = value;
      return;
    }

    header = header->next;
  }

  headers_add(h, key, value);
}

void headers_print(Headers* h) {
  Header* header = h->first;

  while (header) {
    printf("%s: %s\n", header->key, header->value);
    header = header->next;
  }
}
