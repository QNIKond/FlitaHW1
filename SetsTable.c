#include "SetsTable.h"
#include <malloc.h>

#define DEFAULTSIZE 16

Set* sets;

Set* CreateSet(char* name)
{
    static Set* lastSet = 0;
    Set* set = malloc(sizeof(Set));
    if(!sets)
        sets = set;
    //++mlocCount;
    set->name = name;
    set->data = calloc(DEFAULTSIZE,sizeof(int));
    set->filled = 0;
    set->size = DEFAULTSIZE;
    set->nextSet = 0;
    if(lastSet)
        lastSet->nextSet = set;
    lastSet = set;
    return set;
}

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
    Set* currentSet = sets;
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
        set->data = realloc(set->data,DEFAULTSIZE*sizeof(int));
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

void ReplaceData(Set* set, int* data)
{
    if(set->data)
        free(set->data);
    set->data = data;
}

void RecursivelyFreeSets(Set* set)
{
    if(set->nextSet)
        RecursivelyFreeSets(set->nextSet);
    if(set->name)
    {
        free(set->name);
        //--mlocCount;
    }
    if(set->data)
    {
        free(set->data);
        //--mlocCount;
    }
    free(set);
    //--mlocCount;
}

void FreeSets()
{
    RecursivelyFreeSets(sets);
}