#include "declarations/declarations.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {

  void *pointer;
  void (*caller)(void *ptr);

} collector;

void defer_callback(void *ptr, void (*free_func)(void *data), ...) {

  va_list args;
  va_start(args, free_func);
  collector c;
  c.caller = free_func;
  c.pointer = ptr;
  while (c.caller != NULL) {

    c.caller(c.pointer);
    c.caller = va_arg(args, void (*)(void *));
    c.pointer = va_arg(args, void *);
  }

  va_end(args);
}

/// @brief Call free to every pointer passed.
/// @warning for custom types use defer_callback
void defer(void *data, ...) {

  va_list args;
  va_start(args, data);
  void *pointers = data;
  while (pointers != NULL) {
    free(pointers);
    pointers = va_arg(args, void *);
  }
  va_end(args);
}

void free_close(void *ptr) {
  printf("free close called!\n");
  fclose((FILE *)ptr);
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  FILE *fp = fopen("my_file.txt", "wb");
  char *name = malloc(sizeof(char) * 32);
  char *CPF = malloc(sizeof(char) * 11);
  s32 *array = malloc(sizeof(s32) * 64);
  defer(name, CPF, array, NULL);
  defer_callback(fp, free_close);
  return 0;
}
