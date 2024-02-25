#include "SetsTable.h"
#include <malloc.h>

Set* sets;

Set* CreateSet(char* name, int* data)
{
    static Set* lastSet = 0;
    Set* set = malloc(sizeof(Set));
    //++mlocCount;
    set->name = name;
    set->data = data;
    set->nextSet = 0;
    if (lastSet)
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

Set* GetSet(char* name,int length)
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
        result = CreateSet(CopyStr(name,length), 0);
    return result;
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