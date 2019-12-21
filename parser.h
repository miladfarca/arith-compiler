#ifndef PARSER
#include "ast.h"
#define PARSER
//global input string
char *line;
//global int to hold current parsing index
int currentIndex;
char getCurrentchar();

//define CFG
Node *E();
Node *TERM();
Node *R();
Node *FACTOR();
Node *S();
void match(char in);
#endif