#include "../Headers/Interface.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Headers/Structures/SetsTable.h"
#include "../Headers/ExceptionHandler.h"

void PrintNum(FILE *stream,double num)
{
    fprintf(stream, "%d", (int)num);
    if(!(num - (int)num))
        return;
    fprintf(stream, ".");
    if(num < 0)
        num *= -1;
    while(num)
    {
        fprintf(stream, "%d", (int)(num*10)%10);
        num -= (int)num;
        num *= 10;
    }
}

void PrintSets(FILE *stream) {
    const Set *set = GetSetsTable();
    while (set) {
        fprintf(stream, "%s = {", set->name);
        PrintNum(stream,set->data[0]);
        for (int i = 1; i < set->filled; ++i) {
            fprintf(stream,", ");
            PrintNum(stream,set->data[i]);
        }
        fprintf(stream, "}\n");
        fflush(stdout);
        set = set->nextSet;
    }

    fflush(stdout);
}

void RedrawPage() {
    system("cls");
    printf("----------------------\n");
    printf("Sets list:\n\n");
    PrintSets(stdout);
    printf("----------------------\n\n");
    printf("expression:\n\n");
}

void DrawHelpPage()
{
    system("cls");
    FILE* f = fopen("help.txt","r");
    if(!f) {
        printf("\nMissing help.txt file\n");
        return;
    }
    char msg[1000];
    while(fgets(msg, 1000, f))
        printf("%s",msg);
    fclose(f);
    getchar();
    RedrawPage();
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