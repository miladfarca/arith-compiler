/**
 * First thing we need to do is come up with a Context Free Grammer which will be passed to the parse as an input.
 * Parser will use the grammer to verify the inoput string and build a tree from it.
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
#include "parser.h"

// Helpers
char getCurrentChar()
{
    return line[currentIndex];
}

// CGF
void E()
{
    TERM();
    R();
}
void R()
{
    if (getCurrentChar() == '+')
    {
        match('+');
        TERM();
        R();
    }
    else if (getCurrentChar() == '-')
    {
        match('-');
        TERM();
        R();
    }
    else
    {
        //do nothing
    }
}
void TERM()
{
    FACTOR();
    S();
}
void S()
{
    if (getCurrentChar() == '*')
    {
        match('*');
        FACTOR();
        S();
    }
    else if (getCurrentChar() == '/')
    {
        match('/');
        FACTOR();
        S();
    }
    else
    {
        //do nothing
    }
}
void FACTOR()
{
    if (getCurrentChar() == '(')
    {
        match('(');
        E();
        match(')');
    }
    else if (getCurrentChar() == '-')
    {
        match('-');
        E();
    }
    else if (isdigit(getCurrentChar()))
    {
        match(getCurrentChar());
    }
    else
    {
        printf("Error while parsing!\n");
    }
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