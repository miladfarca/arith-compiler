#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

int flag__print_ast = 0;
int flag__print_ast_json = 0;
int flag__print_code = 0;
int flag__print_reg_aloc = 0;
int flag__print_all = 0;
int flag__print_help = 0;
int flag__inline = 0;
int flag__no_output = 0;
int flag__if_jitless = 0;
int flag__code_header_printed = 0;

int expecting_inline = 0;
char *line = NULL;

void parse_args(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        int valid_flag = 0;
        if (strcmp(argv[i], "--help") == 0)
        {
            flag__print_help = 1;
            valid_flag = 1;
        }
        if (strcmp(argv[i], "--inline") == 0)
        {
            flag__inline = 1;
            expecting_inline = 1;
            valid_flag = 1;
            continue;
        }
        if (strcmp(argv[i], "--no-output") == 0)
        {
            flag__no_output = 1;
            valid_flag = 1;
        }
        if (strcmp(argv[i], "--print-ast") == 0)
        {
            flag__print_ast = 1;
            valid_flag = 1;
        }
        if (strcmp(argv[i], "--print-ast-json") == 0)
        {
            flag__print_ast_json = 1;
            valid_flag = 1;
        }
        if (strcmp(argv[i], "--print-reg-aloc") == 0)
        {
            flag__print_reg_aloc = 1;
            valid_flag = 1;
        }
        if (strcmp(argv[i], "--print-code") == 0)
        {
            flag__print_code = 1;
            valid_flag = 1;
        }
        if (strcmp(argv[i], "--print-all") == 0)
        {
            flag__print_ast = 1;
            flag__print_ast_json = 1;
            flag__print_reg_aloc = 1;
            flag__print_code = 1;
            valid_flag = 1;
        }
        if (strcmp(argv[i], "--jitless") == 0)
        {
            flag__if_jitless = 1;
            valid_flag = 1;
        }
        if (expecting_inline){
            line = malloc(strlen(argv[i]));
            strcpy(line, argv[i]);
            expecting_inline = 0;
        }
        else if (!valid_flag)
        {
            printf("Unknown flag: %s\n", argv[i]);
        }
    }
}
