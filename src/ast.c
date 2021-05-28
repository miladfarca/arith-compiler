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
fpr evaluate_ast_and_codegen(node *ast)
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
        fpr free_reg = allocate_fpr();
        load_int_to_fpr(ast->value, free_reg);
        return free_reg;
    }
    else if (ast->type == unary_minus)
    {
        fpr fpr0 = evaluate_ast_and_codegen(ast->left);
        negate_fpr(fpr0);
        return fpr0;
    }
    else
    {
        fpr fpr0 = evaluate_ast_and_codegen(ast->left);
        fpr fpr1 = evaluate_ast_and_codegen(ast->right);
        switch (ast->type)
        {
        case operator_plus:
            add_fpr_to_fpr(fpr0, fpr1);
            dealocate_fpr(fpr1);
            return fpr0;
        case operator_minus:
            subtract_fpr_from_fpr(fpr0, fpr1);
            dealocate_fpr(fpr1);
            return fpr0;
        case operator_mul:
            multiply_fpr_to_fpr(fpr0, fpr1);
            dealocate_fpr(fpr1);
            return fpr0;
        case operator_div:
            divide_fpr_by_fpr(fpr0, fpr1);
            dealocate_fpr(fpr1);
            return fpr0;
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
    //print right node
    print_ast(root->right, space + space_offset_chars);
    //print this node
    switch (root->type)
    {
    case operator_plus:
        printf("%*s%c\n", space, "", '+');
        break;
    case unary_minus:
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
    //print left node
    print_ast(root->left, space + space_offset_chars);
}
void print_ast_json(node *root, int print_comma)
{
    if (root == NULL)
        return;
    // print this node
    if (print_comma)
    {
        printf(",");
    }
    printf("{");
    switch (root->type)
    {
    case operator_plus:
        printf("\"text\": { \"name\": \"+\" },");
        break;
    case unary_minus:
    case operator_minus:
        printf("\"text\": { \"name\": \"-\" },");
        break;
    case operator_mul:
        printf("\"text\": { \"name\": \"*\" },");
        break;
    case operator_div:
        printf("\"text\": { \"name\": \"/\" },");
        break;
    default:
        printf("\"text\": { \"name\": \"%d\" },", root->value);
    }
    printf("\"children\": [");
    // print left node
    print_ast_json(root->left, 0);
    // print right node
    print_ast_json(root->right, 1);
    printf("]");
    printf("}");
}
