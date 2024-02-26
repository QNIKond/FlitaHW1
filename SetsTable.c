#include "SetsTable.h"
#include <malloc.h>

#define DEFAULTSIZE 16

Set* setsTable = 0;

Set* CreateSet(char* name)
{
    Set* set = malloc(sizeof(Set));
    Set* lastSet = setsTable;

    //++mlocCount;
    set->name = name;
    set->data = calloc(DEFAULTSIZE,sizeof(int));
    set->filled = 0;
    set->size = DEFAULTSIZE;
    set->nextSet = 0;

    if(!setsTable)
    {
        setsTable = set;
    }
    else
    {
        while(lastSet->nextSet)
            lastSet = lastSet->nextSet;
        lastSet->nextSet = set;
    }
    return set;
}

const Set* GetSetsTable() {return setsTable;}

char* CopyStr(char* start, int length)
{
    char* str = calloc(length+1,sizeof(char));
    //++mlocCount;
    for(int i = 0;i < length;++i)
        str[i] = start[i];
    str[length] = 0;
    return str;
}

Set* FindSet(char* name, int length)
{
    Set* currentSet = setsTable;
    int i;
    int maxLength = 0;
    Set* result = 0;
    while(currentSet)
    {
        i = 0;
        if (currentSet->name)
        {
            while ((name[i] == currentSet->name[i]) && (name[i] != 0))
                ++i;
            if ((currentSet->name[i] == 0) && (i > maxLength)) {
                maxLength = i;
                result = currentSet;
            }
        }
        currentSet = currentSet->nextSet;
    }
    if (!result)
        result = CreateSet(CopyStr(name,length));
    return result;
}

void PlaceNumberInSet(Set* set, int n)
{
    if(set->size - set->filled <= 2)
    {
        set->size *= 2;
        set->data = realloc(set->data,set->size*sizeof(int));
    }
    int* ptr = set->data;
    while ((*ptr < n) && (ptr - set->data < set->filled))
        ++ptr;
    if ((ptr - set->data == set->filled) || (*ptr != n))
    {
        for(int* i = set->data + set->filled; i>ptr; --i)
            *i = *(i-1);
        *ptr = n;
        ++(set->filled);
    }
}

Set* CreateUnnamedCopy(Set* set)
{
    Set* newSet = CreateSet(0);
    newSet->filled = set->filled;
    newSet->size = set->size;

    newSet->data = calloc(newSet->size,sizeof(int));
    for(int i = 0; i < set->filled; ++i)
        newSet->data[i] = set->data[i];
    return newSet;
}

void ReplaceData(Set* set, Set* source)
{
    if(set->data) {
        if (source->size > set->size) {
            set->data = realloc(set->data, source->size * sizeof(int));
            set->size = source->size;
        }
    }
    else {
        set->data = calloc(source->size, sizeof(int));
        set->size = source->size;
    }
    for(int i = 0; i < source->filled; ++i)
        set->data[i] = source->data[i];
    set->filled = source->filled;
}

void FreeSet(Set* set)
{
    if (set == setsTable)
        setsTable = 0;
    if(set->name)
        free(set->name);
    if(set->data)
        free(set->data);
    free(set);
}

void PopSet(Set* prev,Set** set)
{
    Set* nextSet = (*set)->nextSet;
    FreeSet(*set);
    if(prev)
        prev->nextSet = nextSet;
    *set = nextSet;
}

void ClearAnonymousAndEmptySets()
{
    while(setsTable && ((!setsTable->filled) || (!setsTable->name)))
        PopSet(0, &setsTable);

    if(!setsTable)
        return;

    Set* prev = setsTable;
    Set* set = setsTable->nextSet;
    while(set)
    {
        if((!set->filled)||(!set->name))
        {
            PopSet(prev, &set);
        }
        else if(set)
        {
            prev = set;
            set = set->nextSet;
        }
    }
}

void RecursivelyFreeSets(Set* set)
{
    if(set->nextSet)
        RecursivelyFreeSets(set->nextSet);
    FreeSet(set);
}

void FreeSets()
{
    if(setsTable)
        RecursivelyFreeSets(setsTable);
}