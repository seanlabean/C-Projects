#include <stdio.h>
#include <stdlib.h>
#include "./mpc.h"
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

/* Declare a buffer for user input of size 2048. */
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

/* Use operator strong to see which operation to perform */
long eval_op(long x, char* op, long y){
  if (strcmp(op, "+") == 0) { return x + y; }
  if (strcmp(op, "-") == 0) { return x - y; }
  if (strcmp(op, "*") == 0) { return x * y; }
  if (strcmp(op, "/") == 0) { return x / y; }
  return 0;
}

long eval(mpc_ast_t* t) {

  /* If tagged as number return it directly. */
  if (strstr(t->tag, "number")) {
    return atoi(t->contents);
  }
  /* The operator is always second child. */
  char* op = t->children[1]->contents;

  /* We store the third child in `x` */
  long x = eval(t->children[2]);

  /* Iterate the remaining children and combining. */

  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }
  
  return x;
}

int main(int argc, char** argv) {
  /* Create Some Parsers */
  mpc_parser_t* Number   = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr     = mpc_new("expr");
  mpc_parser_t* Sartoris = mpc_new("sartoris");

  /* Define them with the following Language */
  mpca_lang(MPCA_LANG_DEFAULT,
  "                                                     \
    number   : /-?[0-9]+/ ;                             \
    operator : '+' | '-' | '*' | '/' ;                  \
    expr     : <number> | '(' <operator> <expr>+ ')' ;  \
    sartoris : /^/ <operator> <expr>+ /$/ ;             \
  ",
  Number, Operator, Expr, Sartoris);

  /* Print Version and Exit information */
  puts("Welcome to Sartoris Version 0.0.0.3");
  puts("Press ^C to Exit\n");

  /* A never-ending loop */
  while (1) {
    /* Output prompt and get input */
    char* input = readline("Sartoris > ");

    /* Add input to history */
    add_history(input);

    /* Attempt to Parse the user Input */
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Sartoris, &r)) {
      /* On Success Print the evaluated output */
      long result = eval(r.output);
      printf("%li\n", result);
      mpc_ast_delete(r.output);
      
    } else {
      /* Otherwise Print the Error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    
    /* Free retrieved input */
    free(input);
  }
  
  mpc_cleanup(4, Number, Operator, Expr, Sartoris);
  
  return 0;
}
