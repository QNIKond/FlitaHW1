#pragma once

typedef struct SET
{
    char* name;
    int* data;
    int filled;
    int size;


    struct SET* nextSet;
} Set;

Set* CreateSet(char* name);

Set* FindSet(char* name, int length);

void PlaceNumberInSet(Set* set, int n);

Set* CreateUnnamedCopy(Set* set);

void ReplaceData(Set* set, int* data);

void FreeSets();