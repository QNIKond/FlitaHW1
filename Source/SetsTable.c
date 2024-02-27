#include "../Headers/Structures/SetsTable.h"
#include <malloc.h>
#include "../Headers/ExceptionHandler.h"

#define DEFAULTSIZE 16

Set *setsTable = 0;

Set *CreateSet(char *name) {
    Set *set = malloc(sizeof(Set));
    MEMALCOUNT
    set->name = name;
    set->data = calloc(DEFAULTSIZE, sizeof(int));
    MEMALCOUNT
    set->filled = 0;
    set->size = DEFAULTSIZE;
    set->prevSet = setsTable;
    setsTable = set;
    return set;
}

const Set *GetSetsTable() { return setsTable; }

char *CopyStr(char *start, int length) {
    char *str = calloc(length + 1, sizeof(char));
    MEMALCOUNT
    for (int i = 0; i < length; ++i)
        str[i] = start[i];
    str[length] = 0;
    return str;
}

Set *FindSet(char *name, int length) {
    Set *currentSet = setsTable;
    int i;
    int maxLength = 0;
    Set *result = 0;
    while (currentSet) {
        i = 0;
        if (currentSet->name) {
            while ((name[i] == currentSet->name[i]) && (name[i] != 0))
                ++i;
            if ((currentSet->name[i] == 0) && (i > maxLength)) {
                maxLength = i;
                result = currentSet;
            }
        }
        currentSet = currentSet->prevSet;
    }
    if (!result)
        result = CreateSet(CopyStr(name, length));
    return result;
}

void PlaceNumberInSet(Set *set, double n) {
    if (set->size - set->filled <= 2) {
        set->size *= 2;
        set->data = realloc(set->data, set->size * sizeof(double));
    }
    double *ptr = set->data;
    while ((*ptr < n) && (ptr - set->data < set->filled))
        ++ptr;
    if ((ptr - set->data == set->filled) || (*ptr != n)) {
        for (double *i = set->data + set->filled; i > ptr; --i)
            *i = *(i - 1);
        *ptr = n;
        ++(set->filled);
    }
}

Set *CreateUnnamedCopy(Set *set) {
    Set *newSet = CreateSet(0);
    newSet->filled = set->filled;
    newSet->size = set->size;

    newSet->data = calloc(newSet->size, sizeof(double));
    MEMALCOUNT
    for (int i = 0; i < set->filled; ++i)
        newSet->data[i] = set->data[i];
    return newSet;
}

void ReplaceData(Set *set, Set *source) {
    if (set->data) {
        if (source->size > set->size) {
            set->data = realloc(set->data, source->size * sizeof(double));
            set->size = source->size;
        }
    } else {
        set->data = calloc(source->size, sizeof(double));
        MEMALCOUNT
        set->size = source->size;
    }
    for (int i = 0; i < source->filled; ++i)
        set->data[i] = source->data[i];
    set->filled = source->filled;
}

void FreeSet(Set *set) {
    if (set == setsTable)
        setsTable = 0;
    if (set->name) {
        free(set->name);
        MEMFREECOUNT
    }
    if (set->data) {
        free(set->data);
        MEMFREECOUNT
    }
    free(set);
    MEMFREECOUNT
}

void PopSet(Set *next, Set **set) {
    Set *prevSet = (*set)->prevSet;
    FreeSet(*set);
    if (next)
        next->prevSet = prevSet;
    *set = prevSet;
}

void ClearAnonymousAndEmptySets() {
    while (setsTable && ((!setsTable->filled) || (!setsTable->name)))
        PopSet(0, &setsTable);

    if (!setsTable)
        return;

    Set *next = setsTable;
    Set *set = setsTable->prevSet;
    while (set) {
        if ((!set->filled) || (!set->name)) {
            PopSet(next, &set);
        } else if (set) {
            next = set;
            set = set->prevSet;
        }
    }
}

void RecursivelyFreeSets(Set *set) {
    if (set->prevSet)
        RecursivelyFreeSets(set->prevSet);
    FreeSet(set);
}

void FreeSets() {
    if (setsTable)
        RecursivelyFreeSets(setsTable);
}