#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ast.h"
#include "codegen.h"

node *new_node(node *left, node *right, int value, node_type type)
{
    node *return_node = (node *)malloc(sizeof(node));
    return_node->left = left;
    return_node->right = right;
    return_node->value = value;
    return_node->type = type;
    return return_node;
}

// tree evaluation and instrcution selection
a_register evaluate_ast(node *ast)
{
    if (ast == NULL)
    {
        printf("AST is NULL!\n");
    }
    else if (ast->type == number_value)
    {
        a_register free_reg = get_free_register();
        load_int_to_register(ast->value, free_reg);
        set_final_destination(free_reg);
        return free_reg;
    }
    else if (ast->type == unary_minus)
    {
        //-evaluate_ast(ast->left);
    }
    else
    {
        a_register r0 = evaluate_ast(ast->left);
        a_register r1 = evaluate_ast(ast->right);
        switch (ast->type)
        {
        case operator_plus:
            add_register_to_register(r0, r1);
            set_final_destination(r0);
            dealocate_reg(r1);
            return r0;
        case operator_minus:
            subtract_register_from_register(r0, r1);
            set_final_destination(r0);
            dealocate_reg(r1);
            return r0;
        case operator_mul:
            multiply_register_to_register(r0, r1);
            set_final_destination(r0);
            dealocate_reg(r1);
            return r0;
        case operator_div:
            divide_register_by_register(r0, r1);
            set_final_destination(r0);
            dealocate_reg(r1);
            return r0;
        }
    }
    return -1;
}