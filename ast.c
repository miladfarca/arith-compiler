#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

node *new_node(node *left, node *right, int value, node_type type)
{
    node *return_node = (node *)malloc(sizeof(node));
    return_node->left = left;
    return_node->right = right;
    return_node->value = value;
    return_node->type = type;
    return return_node;
}

double evaluate_ast(node *ast)
{
    if (ast == NULL)
        printf("AST is NULL!\n");

    if (ast->type == number_value)
        return ast->value;
    else if (ast->type == unary_minus)
        return -evaluate_ast(ast->left);
    else
    {
        double v1 = evaluate_ast(ast->left);
        double v2 = evaluate_ast(ast->right);
        switch (ast->type)
        {
        case operator_plus:
            return v1 + v2;
        case operator_minus:
            return v1 - v2;
        case operator_mul:
            return v1 * v2;
        case operator_div:
            return v1 / v2;
        }
    }
    return 0;
}