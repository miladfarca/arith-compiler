#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "flags.h"
#include "ast.h"
#include "codegen.h"

node *new_node(node *left, node *right, int value, node_type type)
{
    //TODO need to clear mem here
    node *return_node = (node *)malloc(sizeof(node));
    return_node->left = left;
    return_node->right = right;
    return_node->value = value;
    return_node->type = type;
    return return_node;
}

// tree evaluation and interpretation (jitless, Depth-first, Post-order)
double evaluate_ast_and_interpret(node *ast)
{
    if (ast == NULL)
        printf("AST is NULL!\n");

    if (ast->type == number_value)
        return ast->value;
    else if (ast->type == unary_minus)
        return -evaluate_ast_and_interpret(ast->left);
    else
    {
        double v1 = evaluate_ast_and_interpret(ast->left);
        double v2 = evaluate_ast_and_interpret(ast->right);
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

// tree evaluation and instrcution selection (Depth-first, Post-order)
a_register evaluate_ast_and_codegen(node *ast)
{
    if (flag__print_code && !flag__code_header_printed)
    {
        printf("--- Code ---\n");
        printf("address = %p\n\n", codegen_mem);
        flag__code_header_printed = 1;
    }
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
        //-evaluate_ast_and_codegen(ast->left);
    }
    else
    {
        a_register r0 = evaluate_ast_and_codegen(ast->left);
        a_register r1 = evaluate_ast_and_codegen(ast->right);
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

// debugging
int space_offset_chars = 5;
void print_ast(node *root, int space)
{
    if (root == NULL)
        return;
    //print left node
    print_ast(root->right, space + space_offset_chars);
    //print this node
    switch (root->type)
    {
    case operator_plus:
        printf("%*s%c\n", space, "", '+');
        break;
    case operator_minus:
        printf("%*s%c\n", space, "", '-');
        break;
    case operator_mul:
        printf("%*s%c\n", space, "", '*');
        break;
    case operator_div:
        printf("%*s%c\n", space, "", '/');
        break;
    default:
        printf("%*s%d\n", space, "", root->value);
    }
    //print right node
    print_ast(root->left, space + space_offset_chars);
}