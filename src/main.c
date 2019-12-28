#include <stdio.h>
#include <stdlib.h>
#include "flags.h"
#include "parser.h"
#include "ast.h"
#include "codegen.h"

// globals
char *line = NULL;
int current_index = 0;

int main(int argc, char **argv)
{

    //setup flags
    parse_args(argc, argv);

    size_t len = 0;
    ssize_t read;
    //read from stdin
    read = getline(&line, &len, stdin);
    if (read > 0)
    {
        // start parsing and creating the AST
        node *root = E();
        // check we need to print the ast tree
        if (flag__print_ast)
        {
            print_ast(root, 0);
        }
        // start evaluating and interpret or codegen
        int output;
        if (flag__if_jitless)
        {
            output = (int)evaluate_ast_and_interpret(root);
        }
        else
        {
            init_codegen();
            evaluate_ast_and_codegen(root);
            output = run_codegen_and_return();
        }

        /*load_int_to_register(5, ecx);
        load_int_to_register(20, ebx);
        divide_register_by_register(ebx, ecx);
        move_register_to_register(eax, ebx);*/

        printf("= %d\n", output);
    }

    free(line);
    return 0;
}