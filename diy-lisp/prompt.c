#include <stdio.h>
#include <stdlib.h>
#include "./mpc.h"
#include "./utils.h"

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

/* Create enumeration of possible error types */
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };
/* Create enumeration of possible lval types */
enum { LVAL_NUM, LVAL_ERR };

/* Declare new struct called lval. This allows the \
LERR/LVAL enums to be passed */
typedef struct {
  int type;
  long num;
  int err;
} lval;

/* Use above struct to create NUM and ERR type lval */
lval lval_num(long x) {
  lval v;
  v.type = LVAL_NUM;
  v.num = x;
  return v;
}
lval lval_err(int x) {
  lval v;
  v.type = LVAL_ERR;
  v.err = x;
  return v;
}

/* Print an "lval" by checking v.type of lval */
void lval_print(lval v) {
  switch (v.type) {
    /* In the case that the type is a number print it */
    /* Then 'break' out of the switch. */
    case LVAL_NUM: printf("%li", v.num); break;

    /* In the case the type is an error */
    case LVAL_ERR:
      /* Check what type of error and print it */
      if (v.err == LERR_DIV_ZERO) {
        printf("Error: Division by Zero!");
      }
      if (v.err == LERR_BAD_OP) {
        printf("Error: Invalid Operator.");
      }
      if (v.err == LERR_BAD_NUM) {
        printf("Error: Invalid Number");
      }
    break;
  }
}

/* Print an "lval" followed by a newline */
void lval_println(lval v) { lval_print(v); putchar('\n'); }

/* Use operator string to see which operation to perform */
/* Return into lval struct. */
lval eval_op(lval x, char* op, lval y) {

  /* If either value is an error return it */
  if (x.type == LVAL_ERR) { return x; }
  if (y.type == LVAL_ERR) { return y; }
  
  /* Otherwise do math on the number values */
  if (strcmp(op, "+") == 0) { return lval_num(x.num + y.num); }
  if (strcmp(op, "-") == 0) { return lval_num(x.num - y.num); }
  if (strcmp(op, "*") == 0) { return lval_num(x.num * y.num); }
  if (strcmp(op, "/") == 0) { 
    /* If second operand is zero, return error */
    return y.num == 0
      ? lval_err(LERR_DIV_ZERO)
      : lval_num(x.num / y.num); 
    }
  if (strcmp(op, "^") == 0) { return lval_num(pow(x.num,y.num)); }
  if (strcmp(op, "%") == 0) { return lval_num(x.num % y.num); }
  if (strcmp(op, "max") == 0) { return lval_num(MAX(x.num,y.num)); }
  if (strcmp(op, "min") == 0) { return lval_num(MIN(x.num,y.num)); }
  if (strcmp(op, "<") == 0) { return lval_num(x.num < y.num); }
  if (strcmp(op, ">") == 0) { return lval_num(x.num > y.num); }

  return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t* t) {

  /* If tagged as number return it directly. */
  if (strstr(t->tag, "number")) {
    /* Check if there is some error in conversion */
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
  }

  /* The operator is always second child. */
  char* op = t->children[1]->contents;

  /* We store the third child in `x` as lval struct */
  lval x = eval(t->children[2]);

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
    operator : '+' | '-' | '*' | '/' | '^' | '%'        \
             | \"max\" | \"min\" | '<' | '>' ;		\
    expr     : <number> | '(' <operator> <expr>+ ')' ;  \
    sartoris : /^/ <operator> <expr>+ /$/ ;             \
  ",
  Number, Operator, Expr, Sartoris);

  /* Print Version and Exit information */
  puts("Welcome to Derivative Invesitigation of Coding Knowledge v0.1");
  puts("Press ^C to Exit\n");

  /* A never-ending loop */
  while (1) {
    /* Output prompt and get input */
    char* input = readline("8==D~ ");

    /* Add input to history */
    add_history(input);

    /* Attempt to Parse the user Input */
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Sartoris, &r)) {
      /* On Success Print the evaluated output */
      lval result = eval(r.output);
      lval_println(result);
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
