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
char get_current_char()
{
    return line[current_index];
}
int read_number()
{
    int start_index = current_index;
    int offset = 0;
    while (isdigit(line[current_index]))
    {
        offset++;
        current_index++;
    }
    //initialize all the indexes to 0
    char buffer[32] = {0};
    memcpy(buffer, &line[start_index], offset);
    return atoi(buffer);
}

// CGF returning AST Nodes
node *E()
{
    node *t = TERM();
    node *r = R();
    return new_node(t, r, 0, operator_plus);
}
node *R()
{
    node *t;
    node *r;
    if (get_current_char() == '+')
    {
        match('+');
        t = TERM();
        r = R();
        return new_node(r, t, 0, operator_plus);
    }
    else if (get_current_char() == '-')
    {
        match('-');
        t = TERM();
        r = R();
        return new_node(r, t, 0, operator_minus);
    }
    else
    {
        //do nothing
    }
    return new_node(NULL, NULL, 0, number_value);
}
node *TERM()
{
    node *f = FACTOR();
    node *s = S();
    return new_node(f, s, 0, operator_mul);
}
node *S()
{
    node *f;
    node *s;
    if (get_current_char() == '*')
    {
        match('*');
        f = FACTOR();
        s = S();
        return new_node(s, f, 0, operator_mul);
    }
    else if (get_current_char() == '/')
    {
        match('/');
        f = FACTOR();
        s = S();
        return new_node(s, f, 0, operator_div);
    }
    else
    {
        //do nothing
    }
    return new_node(NULL, NULL, 1, number_value);
}
node *FACTOR()
{
    if (get_current_char() == '(')
    {
        match('(');
        node *out = E();
        match(')');
        return out;
    }
    else if (get_current_char() == '-')
    {
        match('-');
        return new_node(FACTOR(), NULL, 0, unary_minus);
    }
    else if (isdigit(get_current_char()))
    {
        int value = read_number();
        //match is omitted here as ReadNumber already increments the index
        return new_node(NULL, NULL, value, number_value);
    }
    else
    {
        printf("Error while parsing!\n");
    }
    return new_node(NULL, NULL, 0, 0);
}

void match(char in)
{
    if (get_current_char() == in)
    {
        current_index++;
    }
    else
    {
        printf("Error while parsing!\n");
    }
}