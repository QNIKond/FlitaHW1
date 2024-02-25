#include <stdio.h>
#include "SyntaxAnalyzer.h"
#include "TokenTreeExecutor.h"


int main()
{
    const char* operators[9] = {"(",")","&","|","/","=","count","unknown","var"};
    char s[100];
    const Set* set;
    while(1) {
       fgets(s,100,stdin);
        if(s[0]=='`')
            break;
        Token* t = BuildTokenTree(s);
        ExecuteTree(t);
        ClearAnonymousAndEmptySets();
        set = GetSetsTable();
        while (set) {
            printf("\n%s = {%d", set->name, set->data[0]);
            for (int i = 1; i < set->filled; ++i) {
                printf(", %d", set->data[i]);
                fflush(stdout);
            }
            printf("}\n");
            fflush(stdout);
            set = set->nextSet;
        }
    }
    FreeTokens();
    FreeSets();

}
