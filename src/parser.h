#ifndef PARSER
#include "ast.h"
#define PARSER
//global input string
//TODO: maybe redesign to get away from globals
extern char *line;

//global int to hold current parsing index
extern int current_index;
char get_current_char();
int read_number();

//define CFG
node *E();
node *TERM();
node *R();
node *FACTOR();
node *S();
void match(char in);
#endif