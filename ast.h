#ifndef AST
#include "codegen.h"
#define AST
typedef enum node_type
{
    operator_plus,
    operator_minus,
    operator_mul,
    operator_div,
    unary_minus,
    number_value
} node_type;

typedef struct node
{
    struct node *left;
    struct node *right;
    node_type type;
    int value;
} node;

node *new_node(node *left, node *right, int value, node_type type);
a_register evaluate_ast(node *ast);
#endif