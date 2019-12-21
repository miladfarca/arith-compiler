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
 * R      -> +TERM R (Note: In AST nodes needs to be put back in order as "R TERM" from left to right while evaluating TERM first)
 *        -> -TERM R (Note: In AST nodes needs to be put back in order as "R TERM" from left to right while evaluating TERM first)
 *        -> epsilon
 * TERM   -> FACTOR S
 * S      -> *FACTOR S (Note: In AST nodes needs to be put back in order as "S F" from left to right while evaluating F first)
 *        -> /FACTOR S (Note: In AST nodes needs to be put back in order as "S F" from left to right while evaluating F first)
 *        -> epsilon
 * FACTOR -> (E) 
 *        -> -E
 *        -> number
 * 
 * Now lets implenets a recursive decent parse using the above grammer. We will create a function for each variable.
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.h"

// Helpers
char getCurrentChar()
{
    return line[currentIndex];
}
int ReadNumber()
{
    int startIndex = currentIndex;
    int offset = 0;
    while (isdigit(line[currentIndex]))
    {
        offset++;
        currentIndex++;
    }
    //initialize all the indexes to 0
    char buffer[32] = {0};
    memcpy(buffer, &line[startIndex], offset);
    return atoi(buffer);
}

// CGF returning AST Nodes
Node *E()
{
    return NewNode(TERM(), R(), 0, OperatorPlus);
}
Node *R()
{
    Node *t;
    Node *r;
    if (getCurrentChar() == '+')
    {
        match('+');
        t = TERM();
        r = R();
        return NewNode(r, t, 0, OperatorPlus);
    }
    else if (getCurrentChar() == '-')
    {
        match('-');
        t = TERM();
        r = R();
        return NewNode(r, t, 0, OperatorMinus);
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
    Node *f;
    Node *s;
    if (getCurrentChar() == '*')
    {
        match('*');
        f = FACTOR();
        s = S();
        return NewNode(s, f, 0, OperatorMul);
    }
    else if (getCurrentChar() == '/')
    {
        match('/');
        f = FACTOR();
        s = S();
        return NewNode(s, f, 0, OperatorDiv);
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
        int value = ReadNumber();
        //match is omitted here as ReadNumber already increments the index
        return NewNode(NULL, NULL, value, NumberValue);
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