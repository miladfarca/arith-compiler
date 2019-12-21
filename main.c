#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "ast.h"

int main()
{
    line = NULL;
    size_t len = 0;
    ssize_t read;
    currentIndex = 0;

    //read from stdin
    read = getline(&line, &len, stdin);
    if (read > 0)
    {
        // start parsing and creating the AST
        Node *root = E();
        printf("= %d\n", (int)EvaluateAST(root));
    }

    free(line);
}