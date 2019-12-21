#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "ast.h"

int main()
{
    line = NULL;
    size_t len = 0;
    ssize_t read;
    current_index = 0;

    //read from stdin
    read = getline(&line, &len, stdin);
    if (read > 0)
    {
        // start parsing and creating the AST
        node *root = E();
        printf("= %d\n", (int)evaluate_ast(root));
    }

    free(line);
}