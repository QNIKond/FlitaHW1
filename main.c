#include <stdio.h>
#include <stdlib.h>
#include "SyntaxAnalyzer.h"
#include "TokenTreeExecutor.h"
#include "ExceptionHandler.h"


void PrintErrorMessage(const char* prefix)
{
    const char* errorMessage;
    const char* errorMod;
    int errorPosition;
    errorMessage = GetExceptionInfo(&errorPosition);
    printf("%*s",errorPosition+2,"^\n");
    printf("%*s",errorPosition+2,"|\n");
    printf(prefix);
    printf(errorMessage);
    printf("\n");
    fflush(stdout);
}

void SaveSets()
{
    FILE* f = fopen("sets.txt","w");
    ClearAnonymousAndEmptySets();
    const Set* sets = GetSetsTable();
    while (sets) {
        fprintf(f, "%s = {%d", sets->name, sets->data[0]);
        for (int i = 1; i < sets->filled; ++i) {
            fprintf(f,", %d", sets->data[i]);
        }
        fprintf(f,"}\n");
        sets = sets->nextSet;
    }
    fclose(f);
}

void LoadSets()
{
    FILE* f = fopen("sets.txt","r");
    char s[1000];
    int line = 1;
    while(fgets(s,1000,f))
    {
        Token* tree = BuildTokenTree(s);
        if (IsException() || !tree) {
            printf("Error in save file (line %d)\n", line);
            ClearAnonymousAndEmptySets();
            return;
        }
        ExecuteTree(tree);
        if (IsException()) {
            printf("Error in save file (line %d)\n", line);
            ClearAnonymousAndEmptySets();
            return;
        }
        ClearAnonymousAndEmptySets();
        ++line;
    }
    fclose(f);
}

void AtExit()
{
    SaveSets();
    FreeTokens();
    FreeSets();
    printf("Memory deallocated");
}



void PrintSets()
{
    const Set* set = GetSetsTable();
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

int main()
{

    atexit(AtExit);
    LoadSets();
    //char s[100] = "Art = {}";
    char s[1000];
    while(1) {
        PrintSets();
        CheckException();
        fgets(s,1000,stdin);
        fflush(stdin);
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
    }
}
