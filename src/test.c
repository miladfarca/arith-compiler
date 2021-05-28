#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "test.h"

#define COMPILE(str, result)                         \
    current_index = 0;                               \
    line = str;                                      \
    node *root = E();                                \
    init_codegen();                                  \
    fpr reg_result = evaluate_ast_and_codegen(root); \
    result = run_codegen_and_return(reg_result);

int tests_run = 0;

static char *test_add_1()
{
    int result;
    COMPILE("1+2", result)
    t_assert("error, result !=3", result == 3);
    return 0;
}

static char *test_add_2()
{
    int result;
    COMPILE("123456+78910", result)
    t_assert("error, result !=202366", result == 202366);
    return 0;
}

static char *test_sub_1()
{
    int result;
    COMPILE("-1-2", result)
    t_assert("error, result !=-3", result == -3);
    return 0;
}

static char *test_sub_2()
{
    int result;
    COMPILE("123456-78910", result)
    t_assert("error, result !=44546", result == 44546);
    return 0;
}

static char *test_mul_1()
{
    int result;
    COMPILE("-1*2", result)
    t_assert("error, result !=-2", result == -2);
    return 0;
}

static char *test_mul_2()
{
    int result;
    COMPILE("1234*789", result)
    t_assert("error, result !=973626", result == 973626);
    return 0;
}

static char *test_div_1()
{
    int result;
    COMPILE("6/3", result)
    t_assert("error, result !=2", result == 2);
    return 0;
}

static char *test_div_2()
{
    int result;
    COMPILE("1234/45", result)
    t_assert("error, result !=3", result == 27);
    return 0;
}

static char *test_unary_1()
{
    int result;
    COMPILE("-(-5-5)", result)
    t_assert("error, result !=10", result == 10);
    return 0;
}

static char *test_complex_1()
{
    int result;
    COMPILE("(-3/2)-2+((6*8/12-26)/3)", result)
    t_assert("error, result !=-10", result == -10);
    return 0;
}

static char *all_tests()
{
    t_run_test(test_add_1);
    t_run_test(test_add_2);
    t_run_test(test_sub_1);
    t_run_test(test_sub_2);
    t_run_test(test_mul_1);
    t_run_test(test_mul_2);
    t_run_test(test_div_1);
    t_run_test(test_div_2);
    t_run_test(test_unary_1);
    t_run_test(test_complex_1);
    return 0;
}

int run_tests()
{
    char *result = all_tests();
    if (result != 0)
    {
        printf("%s\n", result);
    }
    else
    {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
