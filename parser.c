/**
 * First thing we need to do is come up with a Context Free Grammer which will be passed to the parse as an input.
 * Parser will use the grammer to verify the inoput string and build an abstract syntax tree from it.
 * 
 * The grammer start like this:
 * 
 * E -> E+E
 *   -> E-E
 *   -> E*E
 *   -> E/E
 *   -> (E)
 *   -> -E
 *   -> number
 * 
 * The above grammer does preserve the precedence of the operators (higher precedence goes last),
 * however it has a problem. It's left recursive and needs to be fixed:
 * 
 * Let's introduce new non-terminals instead of just E.
 * 
 * E      -> E+TERM
 *        -> E-TERM
 *        -> TERM
 * TERM   -> TERM*FACTOR
 *        -> TERM/FACTOR
 *        -> FACTOR
 * FACTOR -> (E) 
 *        -> -E
 *        -> number
 * 
 * Now let's fix the left recursion using the methods explained in the dragon book:
 * 
 * E      -> TERM R
 * R      -> +TERM R
 *        -> -TERM R
 *        -> epsilon
 * TERM   -> FACTOR S
 * S      -> *FACTOR S
 *        -> /FACTOR S
 *        -> epsilon
 * FACTOR -> (E) 
 *        -> -E
 *        -> number
 * 
 * Now lets implenets a recursive decent parse using the above grammer. We will create a function for each variable.
 */
#include <ctype.h>
#include <stdio.h>
#include "ast.h"
#include "parser.h"

// Helpers
char getCurrentChar()
{
    return line[currentIndex];
}

// CGF returning AST Nodes
Node *E()
{
    return NewNode(TERM(), R(), 0, OperatorPlus);
}
Node *R()
{
    if (getCurrentChar() == '+')
    {
        match('+');
        return NewNode(R(), TERM(), 0, OperatorPlus);
    }
    else if (getCurrentChar() == '-')
    {
        match('-');
        return NewNode(R(), TERM(), 0, OperatorMinus);
    }
    else
    {
        //do nothing
    }
    return NewNode(NULL, NULL, 0, NumberValue);
}
Node *TERM()
{
    return NewNode(FACTOR(), S(), 0, OperatorMul);
}
Node *S()
{
    if (getCurrentChar() == '*')
    {
        match('*');
        return NewNode(S(), FACTOR(), 0, OperatorMul);
    }
    else if (getCurrentChar() == '/')
    {
        match('/');
        return NewNode(S(), FACTOR(), 0, OperatorDiv);
    }
    else
    {
        //do nothing
    }
    return NewNode(NULL, NULL, 1, NumberValue);
}
Node *FACTOR()
{
    if (getCurrentChar() == '(')
    {
        match('(');
        Node *out = E();
        match(')');
        return out;
    }
    else if (getCurrentChar() == '-')
    {
        match('-');
        return NewNode(FACTOR(), NULL, 0, UnaryMinus);
    }
    else if (isdigit(getCurrentChar()))
    {
        char value = getCurrentChar();
        match(value);
        //convert char to int
        return NewNode(NULL, NULL, value - '0', NumberValue);
    }
    else
    {
        printf("Error while parsing!\n");
    }
    return NewNode(NULL, NULL, 0, 0);
}

void match(char in)
{
    if (getCurrentChar() == in)
    {
        currentIndex++;
    }
    else
    {
        printf("Error while parsing!\n");
    }
}