#ifndef AST
#define AST
typedef enum NodeType
{
    Undefined,
    OperatorPlus,
    OperatorMinus,
    OperatorMul,
    OperatorDiv,
    UnaryMinus,
    NumberValue
} NodeType;

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    NodeType type;
    int value;
} Node;

Node *NewNode(Node *left, Node *right, int value, NodeType type);
double EvaluateAST(Node *ast);
#endif