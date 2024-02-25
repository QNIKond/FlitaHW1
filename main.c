#include <stdio.h>
#include <stdlib.h>
#include "SyntaxAnalyzer.h"
#include "TokenTreeExecutor.h"

int main()
{
    char s[100] = "Art = {}";
    //char s[100];
    const Set* set;
    while(1) {
       //fgets(s,100,stdin);
        if(s[0]=='`')
            break;
        Token* t = BuildTokenTree(s);
        ExecuteTree(t);
        ClearAnonymousAndEmptySets();
        set = GetSetsTable();
        system("cls");
        printf("----------------------\n");
        printf("Sets list:\n\n");
        while (set) {
            printf("%s = {%d", set->name, set->data[0]);
            for (int i = 1; i < set->filled; ++i) {
                printf(", %d", set->data[i]);
                fflush(stdout);
            }
            printf("}\n");
            fflush(stdout);
            set = set->nextSet;
        }
        printf("----------------------\n");
    }
    FreeTokens();
    FreeSets();

}
