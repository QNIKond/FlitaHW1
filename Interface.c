#include "Interface.h"
#include <stdio.h>
#include <stdlib.h>
#include "SetsTable.h"
#include "ExceptionHandler.h"

void PrintSets()
{
    const Set* set = GetSetsTable();
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
    printf("----------------------\n\n");
    fflush(stdout);
}

void RedrawPage()
{
    system("cls");
    PrintSets();
}

void ShowExceptionMessage()
{
    const char* errorMessage;
    int errorPosition;
    errorMessage = GetExceptionInfo(&errorPosition);
    printf("%*s",errorPosition+2,"^\n");
    printf("%*s",errorPosition+2,"|\n");
    printf("Error: ");
    printf(errorMessage);
    printf("\n");
    fflush(stdout);
}

void GetInput(char* s)
{
    fgets(s,1000,stdin);
    fflush(stdin);
}