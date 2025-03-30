#include "declarations/declarations.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {

  void *pointer;
  void (*caller)(void *ptr);

} collector;
/// @brief call a function that disposes the memory for a pointer.
/// @param ptr - the desired pointer to be freed.
/// @param free_func the function that will be called to act upon the ptr param.
void defer_callback(void *ptr, void (*free_func)(void *data), ...) {

  va_list args;
  va_start(args, free_func);
  collector c = {0};
  c.caller = free_func;
  c.pointer = ptr;
  c.caller(c.pointer);

  bool shouldCall = false;
  u8 counter = 0;
  while (c.pointer != NULL) {
    shouldCall = counter == 2;
    if (shouldCall) {
      counter = 0;
      c.caller(c.pointer);
      continue;
    }
    c.pointer = counter == 0 ? va_arg(args, void *) : c.pointer;
    c.caller = counter != 0 ? va_arg(args, void (*)(void *)) : c.caller;
    counter++;
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

/// @brief Frees the FILE* using fclose
void free_close(void *ptr) {
  printf("free_close called!\n");
  fclose((FILE *)ptr);
}

///@brief frees the FILE* using pclose
void freep_close(void *ptr) {
  printf("freep called!\n");
  pclose((FILE *)ptr);
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  // opening a file
  FILE *fp = fopen("my_file.txt", "wb");
  if (fp == NULL) {
    perror("Error creating the file!\n");
    return -1;
  }
  // opening a process for reading
  FILE *proc = popen("ls", "r"); // Open a process for reading
  if (proc == NULL) {
    perror("Error opening a pipe process!\n");
    return -1;
  }
  char *someVar = malloc(sizeof(char) * 32);
  char *otherVar = malloc(sizeof(char) * 11);
  s32 *intArray = malloc(sizeof(s32) * 64);
  defer(someVar, otherVar, intArray, NULL);
  defer_callback(fp, free_close, proc, freep_close, NULL);
  return 0;
}
