#include "declarations/declarations.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
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

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  FILE *file = fopen("some_file.txt", "wb");
  char *name = malloc(sizeof(char) * 32);
  char *CPF = malloc(sizeof(char) * 11);
  s32 *array = malloc(sizeof(s32) * 64);
  defer(name, CPF, array, file, NULL);
  return 0;
}
