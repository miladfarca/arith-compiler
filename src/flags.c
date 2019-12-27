#include <stdio.h>
#include <string.h>

int flag__print_ast = 0;
int flag__print_code = 0;
int flag__print_reg_aloc = 0;
int flag__code_header_printed = 0;

void parse_args(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        int valid_flag = 0;
        if (strcmp(argv[i], "--print-ast") == 0)
        {
            flag__print_ast = 1;
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
        if (!valid_flag)
        {
            printf("Unknown flag: %s\n", argv[i]);
        }
    }
}