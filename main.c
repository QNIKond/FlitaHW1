#include <stdio.h>
#include <stdlib.h>
#include "SyntaxAnalyzer.h"
#include "TokenTreeExecutor.h"
#include "ExceptionHandler.h"

void PrintErrorMessage(const char* prefix)
{
    const char* errorMessage;
    const char* errorMod;
    char* errorPosition;
    errorMessage = GetExceptionInfo(&errorMod,&errorPosition);
    printf(prefix);
    printf(errorMessage, errorMod);
    printf("\n");
    fflush(stdout);
}

int main()
{
    //char s[100] = "Art = {}";
    char s[100];
    const Set* set;
    while(1) {
       CheckException();
        fgets(s,100,stdin);
        if(s[0]=='`')
            break;
        Token* tree = BuildTokenTree(s);
        if (IsException()) {
            PrintErrorMessage("Syntax error: ");
            continue;
        }
        ExecuteTree(tree);
        if (IsException()) {
            PrintErrorMessage("Semantic error: ");
            continue;
        }
        ClearAnonymousAndEmptySets();
        set = GetSetsTable();
        //system("cls");
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
        fflush(stdout);
    }
    FreeTokens();
    FreeSets();

}
