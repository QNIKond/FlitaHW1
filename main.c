#include <stdio.h>
#include <stdlib.h>
#include "Headers/Builders/TokenTreeExecutor.h"
#include "Headers/ExceptionHandler.h"
#include "Headers/Interface.h"

void SaveSets() {
    ClearAnonymousAndEmptySets();
    const Set *sets = GetSetsTable();
    if (!sets)
        return;

    FILE *f = fopen("sets.txt", "w");
    while (sets) {
        fprintf(f, "%s = {%f", sets->name, sets->data[0]);
        for (int i = 1; i < sets->filled; ++i) {
            fprintf(f, ", %f", sets->data[i]);
        }
        fprintf(f, "}\n");
        sets = sets->prevSet;
    }
    fclose(f);
}

void LoadSets() {
    FILE *f = fopen("sets.txt", "r");
    char s[1000];
    while (fgets(s, 1000, f)) {
        if(!TryExecute(s))
            break;
    }
    fclose(f);
}

void AtExit() {
    SaveSets();
    FreeTokens();
    FreeSets();
    if (!*GetMemAllocCount())
        printf("Memory deallocated");
    else
        printf("MEMORY LEAK (%d)", *GetMemAllocCount());
}

int main() {
    atexit(AtExit);
    LoadSets();
    char s[1000];
    RedrawPage();
    while (1) {
        CheckExceptions();
        GetInput(s);
        if (s[0] == '`')
            break;
        if (TryExecute(s))
            RedrawPage();
    }
}
