#include <stdio.h>
#include "SyntaxAnalyzer.h"


int main()
{
    const char* operators[9] = {"(",")","&","|","/","=","count","unknown","var"};
    char* s = "A={1,3,4,5}/B";
    char* head = s;


    FreeTokens();
    FreeSets();

}
