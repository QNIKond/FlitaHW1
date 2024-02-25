#include <stdio.h>
#include "SyntaxAnalyzer.h"
#include "TokenTreeExecutor.h"


int main()
{
    const char* operators[9] = {"(",")","&","|","/","=","count","unknown","var"};
    char* s = "{1,2,3}|{3,4}&{4,5}";
    char* head = s;

    Set* t = ExecuteTree(BuildTokenTree(head));
    for(int i = 0; i < t->filled; ++i) {
        printf("%d ", t->data[i]);
        fflush(stdout);
    }
    FreeTokens();
    FreeSets();

}
