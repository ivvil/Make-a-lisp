#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

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


long eval_op(char* op, long x, long y) {
  
  if (strcmp(op, "+") == 0 || strcmp(op, "add") == 0) { return x + y; }
  if (strcmp(op, "-") == 0 || strcmp(op, "sub") == 0) { return x - y; }
  if (strcmp(op, "*") == 0 || strcmp(op, "mul") == 0) { return x * y; }
  if (strcmp(op, "/") == 0 || strcmp(op, "div") == 0) { return x / y; }
  if (strcmp(op, "%") == 0 || strcmp(op, "mod") == 0) { return x % y; }
  return 0;
}

long eval(mpc_ast_t* t) {
  
  /* Numbers evaluate to themselves */
  if (strstr(t->tag, "num")) {
    return atoi(t->contents);
  }
  
  char* op = t->children[1]->contents;
  long x = eval(t->children[2]);

  int i = 3;
  while (strstr(t->children[i]->tag, "exp")) {
	x = eval_op(op, x, eval(t->children[i]));
	i++;
  }

  return x;
}

int main(int argc, char** argv) {

  mpc_parser_t* num = mpc_new("num");
  mpc_parser_t* op = mpc_new("op");
  mpc_parser_t* op_s = mpc_new("ops");
  mpc_parser_t* op_t = mpc_new("opt");
  mpc_parser_t* exp = mpc_new("exp");
  mpc_parser_t* chisp = mpc_new("chsp");

  mpca_lang(MPCA_LANG_DEFAULT,
			"                                                     \
    num : /-?[0-9]+/ ;											  \
    ops : '+' | '-' | '*' | '/' | '%' ;							  \
    opt : \"add\" | \"sub\" | \"mul\" | \"div\" | \"mod\" ;		  \
    op : <opt> | <ops> ;										  \
    exp : <num> | '(' <op> <exp>+ ')' ;							  \
    chsp : /^/ <op> <exp>+ /$/ ;								  \
  ",
			num, op_s, op_t, op, exp, chisp);

  /* Print Version and Exit Information */
  puts("Chisp 0.0.1");
  puts("Ctrl+c to Exit\n");

  while (1) {

    char* input = readline("chsp> ");
    add_history(input);

	mpc_result_t r;
	if (mpc_parse("<stdin>", input, chisp, &r)) {

	  /* mpc_ast_print(r.output); */
	  
	  long result = eval(r.output);
	  printf("%li\n", result);
	  mpc_ast_delete(r.output);
	} else {
	  mpc_err_print(r.error);
	  mpc_err_delete(r.error);
	}
	
    free(input);

  }

  mpc_cleanup(4, num, op_s, op_t, op, exp, chisp);

  return 0;
}
