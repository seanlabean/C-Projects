#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"
/* 
I am creating a REPL (read, evaluate, print, loop) that prompts me
for input, accepts any input into an input buffer, parses the input, evaluates
if possible, and prompts me for another input.

This prompt script simply takes an input from User and echos it back, leaving
the User able to provide an input again. It's simple, but a great 
building block!

I need to use the Mac-specific editline library, specifically the readline and 
add_history functions which allow for editing of pre-submitted input
and recall of previous inputs respectively.

I included an #ifdef block that identifies if the program is being compiled
on Windows. If it is, then the program runs Windows-specific code and 
functions normally. Otherwise, only code after the #endif statement is run.
*/

/* */
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/* Windows-only readline function */
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}

/* Otherwise include the editline headers */
#else
#include <editline/readline.h>
#endif

// Declare a buffer for user input of size 2048.
//static char input[2048];

int main(int argc, char** argv) {
  /* Print Version and Exit information */
  puts("meat Version 0.0.0.3");
  puts("Press ^C to Exit\n");

  /* A never-ending loop */
  while (1) {
    /* Output prompt and get input */
    char* input = readline("meat> ");

    /* Add input to history */
    add_history(input);

    /* Echo back to user */
    printf("You said: %s\n", input);

    /* Free retrieved input */
    free(input);
  }
  return 0;
}
