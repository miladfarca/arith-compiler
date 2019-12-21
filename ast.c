#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

Node *NewNode(Node *left, Node *right, int value, NodeType type)
{
    Node *returnNode = (Node *)malloc(sizeof(Node));
    returnNode->left = left;
    returnNode->right = right;
    returnNode->value = value;
    returnNode->type = type;
    return returnNode;
}

double EvaluateAST(Node *ast)
{
    if (ast == NULL)
        printf("AST is NULL!\n");

    if (ast->type == NumberValue)
        return ast->value;
    else if (ast->type == UnaryMinus)
        return -EvaluateAST(ast->left);
    else
    {
        double v1 = EvaluateAST(ast->left);
        double v2 = EvaluateAST(ast->right);
        switch (ast->type)
        {
        case OperatorPlus:
            return v1 + v2;
        case OperatorMinus:
            return v1 - v2;
        case OperatorMul:
            return v1 * v2;
        case OperatorDiv:
            return v1 / v2;
        }
    }
    return 0;
}