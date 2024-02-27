#include "../Headers/Structures/SetsTable.h"
#include <malloc.h>
#include "../Headers/ExceptionHandler.h"

#define DEFAULTSIZE 16

Set *setsTable = 0;
Set *lastSet = 0;

void SetLastSet(){
    lastSet = setsTable;
    while (lastSet && lastSet->nextSet)
        lastSet = lastSet->nextSet;
}

Set *CreateSet(char *name) {
    Set *set = malloc(sizeof(Set));
    MEMALCOUNT
    if(!setsTable)
        setsTable = set;
    set->name = name;
    set->data = calloc(DEFAULTSIZE, sizeof(int));
    MEMALCOUNT
    set->filled = 0;
    set->size = DEFAULTSIZE;
    set->nextSet = 0;
    if(lastSet)
        lastSet->nextSet = set;
    lastSet = set;
    return set;
}

const Set *GetSetsTable() { return setsTable; }

char *CopyStr(const char *start, int length) {
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
    while (currentSet) {
        i = 0;
        if (currentSet->name) {
            while ((name[i] == currentSet->name[i]) && (name[i] != 0))
                ++i;
            if ((currentSet->name[i] == 0) && (i == length)) {
                return currentSet;
            }
        }
        currentSet = currentSet->nextSet;
    }
    return CreateSet(CopyStr(name, length));
}

void PlaceNumberInSet(Set *set, int n) {
    if (set->size - set->filled <= 2) {
        set->size *= 2;
        set->data = realloc(set->data, set->size * sizeof(int));
    }
    int *ptr = set->data;
    while ((*ptr < n) && (ptr - set->data < set->filled))
        ++ptr;
    if ((ptr - set->data == set->filled) || (*ptr != n)) {
        for (int *i = set->data + set->filled; i > ptr; --i)
            *i = *(i - 1);
        *ptr = n;
        ++(set->filled);
    }
}

Set *CreateUnnamedCopy(Set *set) {
    Set *newSet = CreateSet(0);
    newSet->filled = set->filled;
    newSet->size = set->size;

    newSet->data = calloc(newSet->size, sizeof(int));
    MEMALCOUNT
    for (int i = 0; i < set->filled; ++i)
        newSet->data[i] = set->data[i];
    return newSet;
}

void ReplaceData(Set *set, Set *source) {
    if (set->data) {
        if (source->size > set->size) {
            set->data = realloc(set->data, source->size * sizeof(int));
            set->size = source->size;
        }
    } else {
        set->data = calloc(source->size, sizeof(int));
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

void PopSet(Set *prev, Set **set) {
    Set *nextSet = (*set)->nextSet;
    FreeSet(*set);
    if (prev)
        prev->nextSet = nextSet;
    *set = nextSet;
}

void ClearAnonymousAndEmptySets() {
    while (setsTable && (!setsTable->filled) || (!setsTable->name))
        PopSet(0, &setsTable);

    if (!setsTable)
        return;

    Set *next = setsTable;
    Set *set = setsTable->nextSet;
    while (set) {
        if ((!set->filled) || (!set->name)) {
            PopSet(next, &set);
        } else if (set) {
            next = set;
            set = set->nextSet;
        }
    }
    SetLastSet();
}

void RecursivelyFreeSets(Set *set) {
    if (set->nextSet)
        RecursivelyFreeSets(set->nextSet);
    FreeSet(set);
}

void FreeSets() {
    if (setsTable)
        RecursivelyFreeSets(setsTable);
}