#include "../Headers/Interface.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Headers/Structures/SetsTable.h"
#include "../Headers/ExceptionHandler.h"

void PrintNum(double num)
{
    if(num - (int)num)
        printf("%.3f", num);
    else
        printf("%d", (int)num);
    fflush(stdout);
}

void PrintSets() {
    const Set *set = GetSetsTable();
    printf("----------------------\n");
    printf("Sets list:\n\n");
    while (set) {
        printf("%s = {", set->name);
        PrintNum(set->data[0]);
        for (int i = 1; i < set->filled; ++i) {
            printf(", ");
            PrintNum(set->data[i]);
        }
        printf("}\n");
        fflush(stdout);
        set = set->prevSet;
    }
    printf("----------------------\n\n");
    fflush(stdout);
}

void RedrawPage() {
    system("cls");
    PrintSets();
}

void ShowExceptionMessage() {
    const char *errorMessage;
    int errorPosition;
    errorMessage = GetExceptionInfo(&errorPosition);
    printf("%*s", errorPosition + 2, "^\n");
    printf("%*s", errorPosition + 2, "|\n");
    printf("Error: ");
    printf(errorMessage);
    printf("\n");
    fflush(stdout);
}

void GetInput(char *s) {
    fgets(s, 1000, stdin);
    fflush(stdin);
}