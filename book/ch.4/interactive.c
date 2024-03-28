#include <stdio.h>
#include <stdlib.h>

#include <editline.h>

/* Windows edtline fakery */
#ifdef _WIN32
#include <string.h>

static char inBuff[2048];

/* Readline */

char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(inBuff, 2048, stdin);

  char* copy = malloc(strlen(buffer)+1);
  strcpy(copy, buffer);

  copy[strlen(copy)-1] = '\0';

  return copy;
}

void add_history(char* unused) {}

#else
#include <editline.h>

#endif // _WIN32



int main(int argc, char** argv) {

  /* Print Version and Exit Information */
  puts("chisp 0.0.1");
  puts("Ctrl+c to Exit\n");

  /* In a never ending loop */
  while (1) {

    /* Output our prompt and get input */
    char* input = readline("chisp> ");

    /* Add input to history */
    add_history(input);

    /* Echo input back to user */
    printf("No, you're a %s\n", input);

    /* Free retrieved input */
    free(input);

  }

  return 0;
}
