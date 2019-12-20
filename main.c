#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main()
{
    line = NULL;
    size_t len = 0;
    ssize_t read;
    currentIndex = 0;

    //read from stdin
    read = getline(&line, &len, stdin);
    if (read > 0)
    {
        // start parsing
        E();
    }

    free(line);
}