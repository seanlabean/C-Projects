#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_SEXPR, LVAL_QEXPR };

/* Declare new struct called lval. This allows the \
LERR/LVAL enums to be passed. Structure points to set
S&Q-Expressions in memory when able and self-contains
a pointer to its pointer. */

typedef struct lval {
  int type;
  double num;
  /* Error and Symbol types are static in memory. */
  char* err;
  char* sym;
  /* Count and Pointer to list of "lval*" */
  int count;
  struct lval** cell;
} lval;

/* Using the lval* structure. "Constructors"*/

/* Construct a pointer to a new Number lval. */
lval* lval_num(long x) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_NUM;
  v->num = x;
  return v;
}

/* Construct a pointer to a new Error lval */
lval* lval_err(char* m) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_ERR;
  v->err = malloc(strlen(m) + 1);
  strcpy(v->err, m);
  return v;
}

/* Construct a pointer to a new Symbol lval */
lval* lval_sym(char* s) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SYM;
  v->sym = malloc(strlen(s) + 1);
  strcpy(v->sym, s);
  return v;
}

/* A pointer to a new empty Sexpr lval */
lval* lval_sexpr(void) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SEXPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

lval* lval_qexpr(void) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_QEXPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

//typedef struct {
//  int type;
//  long num;
//  int err;
//} lval;

/* Use above struct to create NUM and ERR type lval */
//lval lval_num(long x) {
//  lval v;
//  v.type = LVAL_NUM;
//  v.num = x;
//  return v;
//}
//lval lval_err(int x) {
//  lval v;
//  v.type = LVAL_ERR;
//  v.err = x;
//  return v;
//}

/* lval* Destroyer */
/* Special function to delete all lval* pointers to prevent 
   Memory Leaks. */
void lval_del(lval* v) {
  switch (v->type) {
    case LVAL_NUM: break;

    /* For Err or Sym free the sting data */
    case LVAL_ERR: free(v->err); break;
    case LVAL_SYM: free(v->sym); break;

    /* If Sexpr or Qexpr then delete all elements inside */
    case LVAL_QEXPR:
    case LVAL_SEXPR:
      for (int i = 0; i < v->count; i++) {
        lval_del(v->cell[i]);
      }
      /* Also free the memory allocated to contain the pointers */
      free(v->cell);
    break;
  }
  /* Free memory allocated for "lval" struct itself */
  free(v);
}

/* Reading Expressions. Construct lval* that represents program,
and convert abstract regex tree syntax to an S-Expression.
Naturally need to look recursively through tree nodes and construct 
different lval* depending on tag and contents fields of each node. */

/* Special case for nums */
lval* lval_read_num(mpc_ast_t* t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE ?
    lval_num(x) : lval_err("invalid number");
}

/* Enable abillity to add elements to S-Expressions */
lval* lval_add(lval* v, lval* x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  v->cell[v->count-1] = x;
  return v;
}

lval* lval_read(mpc_ast_t* t) {
  /* If Symbol or Number return conversion to that type */
  if (strstr(t->tag, "number")) { return lval_read_num(t); }
  if (strstr(t->tag, "symbol")) { return lval_sym(t->contents); }

  /* If root (>) or sexpr then create empty list */
  lval* x = NULL;
  if (strcmp(t->tag, ">") == 0) { x = lval_sexpr(); }
  if (strstr(t->tag, "sexpr")) { x = lval_sexpr(); }
  if (strstr(t->tag, "qexpr")) { x = lval_qexpr(); }

  /* Fill this list with any valid expression contained within */
  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
    if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
    if (strcmp(t->children[i]->contents, "{") == 0) { continue; }
    if (strcmp(t->children[i]->contents, "}") == 0) { continue; }
    if (strcmp(t->children[i]->tag,  "regex") == 0) { continue; }
    x = lval_add(x, lval_read(t->children[i]));
  }
  return x;
}

void lval_print(lval* v); /* Allows for compilation before def */

/* Special print that prints S-expressions */
void lval_expr_print(lval* v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {
    /* Print value contained within */
    lval_print(v->cell[i]);

    /* Don't print trailing space if last element */
    if (i != (v->count-1)) {
      putchar(' ');
    }
  }
  putchar(close);
}

void lval_print(lval* v) {
  switch (v->type) {
    case LVAL_NUM:   printf("%.2f", v->num); break;
    case LVAL_ERR:   printf("Error: %s", v->err); break;
    case LVAL_SYM:   printf("%s", v->sym); break;
    case LVAL_SEXPR: lval_expr_print(v, '(', ')'); break;
    case LVAL_QEXPR: lval_expr_print(v, '{', '}'); break;
  }
}

/* Print an "lval" by checking v.type of lval */
//void lval_print(lval v) {
//  switch (v.type) {
    /* In the case that the type is a number print it */
    /* Then 'break' out of the switch. */
//    case LVAL_NUM: printf("%li", v.num); break;

    /* In the case the type is an error */
//    case LVAL_ERR:
      /* Check what type of error and print it */
//      if (v.err == LERR_DIV_ZERO) {
//        printf("Error: Division by Zero!");
//      }
//      if (v.err == LERR_BAD_OP) {
//        printf("Error: Invalid Operator.");
//      }
//      if (v.err == LERR_BAD_NUM) {
//        printf("Error: Invalid Number");
//      }
//    break;
//  }
//}

/* Print an "lval" followed by a newline */
void lval_println(lval* v) { lval_print(v); putchar('\n'); }

/* Macro for error testing */
#define LASSERT(args, cond, err) \
  if (!(cond)) { lval_del(args); return lval_err(err); }

/* S-Expression lval type manipulators (lval_pop, lval_take). */

/* lval_pop removes a S-Expression element at index i and shifts the list backwards
so it no longer contains that lval*, it then returns the extracted value. The value
and the list still remain on the stack and so will later need to be deleted with 
lval_del() */

lval* lval_pop(lval* v, int i) {
  /* Find the item at "i" */
  lval* x = v->cell[i];
  /* Shift memory after the item at "i" over the top */
  memmove(&v->cell[i], &v->cell[i+1],
    sizeof(lval*) * (v->count-i-1));
  /* Decrease the count of items in the list */
  v->count--;
  /* Reallocate the memory used */
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  return x;
}

/* lval_take operates similarly, but it deletes the list it takes a value from. 
This is used to help with code readability later. */

lval* lval_take(lval* v, int i) {
  lval* x = lval_pop(v, i);
  lval_del(v);
  return x;
}

/* builtin_op operates on a single lval* representing the list of all arguments
to operate on. First checks that all arguments are numbers and pops the first arg.
(also performs unary negation on first number if there are no other args and 
operator is subtraction). It then pops the next arg and performs aritmetic depending 
on operator. Errors are also handled if encountered like div 0. Finally returns new 
expression and deletes old args. */

lval* builtin_op(lval* a, char* op) {
  /* Ensure all args are numbers */
  for (int i = 0; i < a->count; i++) {
    LASSERT(a, a->cell[i]->type == LVAL_NUM,
      "Cannot operate on non-number!");
    //if (a->cell[i]->type != LVAL_NUM) {
    //  lval_del(a);
    //  return lval_err("Cannot operate on non-number!");
    //}
  }
  /* Pop first element */
  lval* x = lval_pop(a, 0);

  /* If there are no args and sub operator then perform unary negation */
  if ((strcmp(op, "-") == 0) && a->count == 0) {
    x->num = -x->num;
  }
  /* While there are still elements remaining: operate, cleanup, loop */
  while (a->count > 0) {
    /* Pop the next element */
    lval* y = lval_pop(a, 0);

    if (strcmp(op, "+") == 0) { x->num += y->num; }
    if (strcmp(op, "-") == 0) { x->num -= y->num; }
    if (strcmp(op, "*") == 0) { x->num *= y->num; }
    if (strcmp(op, "/") == 0) {
      if (y->num == 0) {
        lval_del(x); lval_del(y);
        x = lval_err("Div Zero!"); break;
      }
      x->num /= y->num;
    }
    if (strcmp(op, "^") == 0) { x->num = pow(x->num,y->num); }
    if (strcmp(op, "%") == 0) { x->num = fmod(x->num, y->num); }
    if (strcmp(op, "max") == 0) { x->num = MAX(x->num, y->num); }
    if (strcmp(op, "min") == 0) { x->num = MIN(x->num, y->num); }
    if (strcmp(op, ">") == 0) { x->num = x->num > y->num; }
    if (strcmp(op, "<") == 0) { x->num = x->num < y->num; }
    lval_del(y);
  }
  lval_del(a); return x;
}

/* builtin functions for head, tail, list, eval Q-Expr calls */
lval* builtin_head(lval* a) {
  /* Check Error Conditions */
  LASSERT(a, a->count == 1, 
    "Function 'head' passed too many arguments!");
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'head' passed incorrect types!");
  LASSERT(a, a->cell[0]->count != 0,
    "Function 'head' passed {}!")

  /* Otherwise take the first argument */
  lval* v = lval_take(a, 0);

  /* Delete all elements that are not head and return */
  while (v->count > 1) { lval_del(lval_pop(v, 1)); }
  return v;
}

lval* builtin_tail(lval* a) {
  /* Check Error Conditions */
  if (a->count != 1) {
    lval_del(a);
    return lval_err("Function 'tail' passed too many arguments!");
  }
  if (a->cell[0]->type != LVAL_QEXPR) {
    lval_del(a);
    return lval_err("Function 'tail' passed incorrect types!");
  }
  if (a->cell[0]->count == 0) {
    lval_del(a);
    return lval_err("Function 'tail' passed {}!");
  }

  /* Take first argument */
  lval* v = lval_take(a, 0);

  /* Delete first element and return */
  lval_del(lval_pop(v, 0));
  return v;
}

lval* builtin_list(lval* a) {
  a->type = LVAL_QEXPR;
  return a;
}

lval* lval_eval(lval* v); // for compilation

lval* builtin_eval(lval* a) {
  LASSERT(a, a->count == 1, 
    "Function 'eval' passed too many arguments!");
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'eval' passed incorrect type!");
  
  lval* x = lval_take(a,0);
  x->type = LVAL_SEXPR;
  return lval_eval(x);
}

lval* lval_join(lval* x, lval* y) {
  /* for each cell in 'y' add it to 'x' */
  while (y->count) {
    x = lval_add(x, lval_pop(y,0));
  }

  /* Delete the empty 'y' and return 'x' */
  lval_del(y);
  return x;
}

lval* builtin_join(lval* a) {
  for (int i = 0; i < a->count; i++) {
    LASSERT(a, a->cell[i]->type == LVAL_QEXPR,
      "Function 'join' passed incorrect type.");
  }

  lval* x = lval_pop(a, 0);
  
  while (a->count) {
    x = lval_join(x, lval_pop(a, 0));
  }

  lval_del(a);
  return x;
}

lval* builtin(lval* a, char* func) {
  if (strcmp("list", func) == 0) { return builtin_list(a); }
  if (strcmp("head", func) == 0) { return builtin_head(a); }
  if (strcmp("tail", func) == 0) { return builtin_tail(a); }
  if (strcmp("join", func) == 0) { return builtin_join(a); }
  if (strcmp("eval", func) == 0) { return builtin_eval(a); }
  if (strstr("+-/*^%maxmin<>", func)) { return builtin_op(a, func); }
  lval_del(a);
  return lval_err("Unknown Function!");
}


/* S-Expression evaluator. Iteratively check expression for children and evaluate, 
check and express errors. Check for empty expression, return. Check that first 
element is a symbol, return err if not, the evaluate symbol with operator builtin.  */

lval* lval_eval_sexpr(lval* v) {

  /* Iteratively Evaluate Children */
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lval_eval(v->cell[i]);
  }
  /* Error Checking */
  for (int i = 0; i < v->count; i++) {
    if (v->cell[i]->type == LVAL_ERR) { return lval_take(v,i); }
  }
  /* Empty Expression */
  if (v->count == 0) { return v; }
  /* Single Expression */
  if (v->count == 1) { return lval_take(v,0); }
  /* Ensure First Element is Symbol */
  lval* f = lval_pop(v, 0);
  if (f->type != LVAL_SYM) {
    lval_del(f); lval_del(v);
    return lval_err("S-expression does not start with symbol!");
  }
  /* Call builtin with operator */
  lval* result = builtin(v, f->sym);
  lval_del(f);
  return result;
}

/* S-Expression evaluator */

lval* lval_eval(lval* v) {
  if (v->type == LVAL_SEXPR) { return lval_eval_sexpr(v); }
  /* All other lval types pass through unchanged */
  return v;
}

/* Use operator string to see which operation to perform */
/* Return into lval struct. */

//lval eval_op(lval x, char* op, lval y) {
//
//  /* If either value is an error return it */
//  if (x.type == LVAL_ERR) { return x; }
//  if (y.type == LVAL_ERR) { return y; }
//  
//  /* Otherwise do math on the number values */
//  if (strcmp(op, "+") == 0) { return lval_num(x.num + y.num); }
//  if (strcmp(op, "-") == 0) { return lval_num(x.num - y.num); }
// if (strcmp(op, "*") == 0) { return lval_num(x.num * y.num); }
//  if (strcmp(op, "/") == 0) { 
//    /* If second operand is zero, return error */
//    return y.num == 0
//      ? lval_err(LERR_DIV_ZERO)
//      : lval_num(x.num / y.num); 
//    }
//  if (strcmp(op, "^") == 0) { return lval_num(pow(x.num,y.num)); }
//  if (strcmp(op, "%") == 0) { return lval_num(x.num % y.num); }
//  if (strcmp(op, "max") == 0) { return lval_num(MAX(x.num,y.num)); }
//  if (strcmp(op, "min") == 0) { return lval_num(MIN(x.num,y.num)); }
// if (strcmp(op, "<") == 0) { return lval_num(x.num < y.num); }
//  if (strcmp(op, ">") == 0) { return lval_num(x.num > y.num); }
//
//  return lval_err(LERR_BAD_OP);
//}

//lval eval(mpc_ast_t* t) {
//
//  /* If tagged as number return it directly. */
//  if (strstr(t->tag, "number")) {
//    /* Check if there is some error in conversion */
//    errno = 0;
//    long x = strtol(t->contents, NULL, 10);
//    return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
//  }
//
//  /* The operator is always second child. */
//  char* op = t->children[1]->contents;
//
//  /* We store the third child in `x` as lval struct */
//  lval x = eval(t->children[2]);
//
//  /* Iterate the remaining children and combining. */
//  int i = 3;
//  while (strstr(t->children[i]->tag, "expr")) {
//    x = eval_op(x, op, eval(t->children[i]));
//    i++;
//  }
//  
//  return x;
//}

int main(int argc, char** argv) {
  /* Create Some Parsers */
  mpc_parser_t* Number   = mpc_new("number");
  mpc_parser_t* Symbol   = mpc_new("symbol");
  mpc_parser_t* Sexpr    = mpc_new("sexpr");
  mpc_parser_t* Qexpr    = mpc_new("qexpr");
  mpc_parser_t* Expr     = mpc_new("expr");
  mpc_parser_t* Sartoris = mpc_new("sartoris");

  /* Define them with the following Language */
  mpca_lang(MPCA_LANG_DEFAULT,
  "                                                     \
    number   : /-?[0-9]+/ ;                             \
    symbol  : \"list\" | \"head\" | \"tail\"            \
             | \"join\" | \"eval\" | '+' | '-'          \
             | '*' | '/' | '^' | '%'                    \
             | \"max\" | \"min\" | '<' | '>' ;		      \
    sexpr    : '(' <expr>* ')' ;                        \
    qexpr    : '{' <expr>* '}' ;                        \
    expr     : <number> | <symbol> | <sexpr> | <qexpr> ;\
    sartoris : /^/ <expr>* /$/ ;                        \
  ",
  Number, Symbol, Sexpr, Qexpr, Expr, Sartoris);

  /* Print Version and Exit information */
  puts("  _____\n (' V ') .o0 |Welcome to Sean's LISP!|\n((_____))\n    ^^");
  //puts("Welcome to SLISP v0.6");
  puts("Press ^C to Exit\n");

  /* A never-ending loop */
  while (1) {
    /* Output prompt and get input */
    char* input = readline(">>--> ");

    /* Add input to history */
    add_history(input);

    /* Attempt to Parse the user Input */
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Sartoris, &r)) {
      /* On Success Print the evaluated output */
      //lval result = eval(r.output);
      //lval_println(result);
      //mpc_ast_delete(r.output);
      lval* x = lval_eval(lval_read(r.output));
      lval_println(x);
      lval_del(x);
      mpc_ast_delete(r.output);
    } else {
      /* Otherwise Print the Error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    
    /* Free retrieved input */
    free(input);
  }
  
  mpc_cleanup(6, Number, Symbol, Sexpr, Qexpr, Expr, Sartoris);
  
  return 0;
}
