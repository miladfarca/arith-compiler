#include <stdio.h>
#include <stdlib.h>
#include "flags.h"
#include "parser.h"
#include "ast.h"
#include "codegen.h"

// globals
int current_index = 0;

int main(int argc, char **argv)
{

    //setup flags
    parse_args(argc, argv);

    //if help is needed
    if (flag__print_help)
    {
        printf("\nflags:\n");
        printf("%-20s %s", "--print-ast", "Print the generated Abstract Syntax Tree (AST)\n");
        printf("%-20s %s", "--print-ast-json", "Print the generated Abstract Syntax Tree (AST) in JSON format\n");
        printf("%-20s %s", "--print-reg-aloc", "Print allocation and de-allocation of registers\n");
        printf("%-20s %s", "--print-code", "Print the generated instructions\n");
        printf("%-20s %s", "--print-all", "Print all the above\n");
        printf("%-20s %s", "--no-output", "Do not print the final output\n");
        printf("%-20s %s", "--jitless", "Interpret the result and do not generated code\n");
        printf("%-20s %s", "--inline", "Enter the input as an argument\n");
        printf("\n");
        exit(0);
    }

    size_t len = 0;
    ssize_t read;
    if (!flag__inline){
        // read from stdin
        read = getline(&line, &len, stdin);
    }
    if (flag__inline || read > 0)
    {
        // start parsing and creating the AST
        node *root = E();
        // check we need to print the ast tree
        if (flag__print_ast)
        {
            printf("--- Syntax Tree ---\n\n");
            print_ast(root, 0);
        }
        if (flag__print_ast_json)
        {
            print_ast_json(root);
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

        if (!flag__no_output){
            printf("= %d\n", output);
        }
    }

    if (!flag__inline){
        free(line);
    }
    return 0;
}
