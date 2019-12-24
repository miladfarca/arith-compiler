#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "ast.h"
#include "codegen.h"

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
        // start evaluating and codegen
        init_codegen();
        evaluate_ast(root);

        /*load_int_to_register(5, ecx);
        load_int_to_register(20, ebx);
        divide_register_by_register(ebx, ecx);
        move_register_to_register(eax, ebx);*/

        printf("= %d\n", run_codegen_and_return());
    }

    free(line);
    return 0;
}