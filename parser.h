//global input string
char *line;
//global int to hold current parsing index
int currentIndex;
char getCurrentchar();

//define CFG
void E();
void TERM();
void R();
void FACTOR();
void S();
void match(char in);