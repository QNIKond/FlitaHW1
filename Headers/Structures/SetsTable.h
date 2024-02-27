#pragma once

typedef struct SET {
    char *name;
    int *data;
    int filled;
    int size;


    struct SET *prevSet;
} Set;

Set *CreateSet(char *name);

const Set *GetSetsTable();

Set *FindSet(char *name, int length);

void PlaceNumberInSet(Set *set, int n);

Set *CreateUnnamedCopy(Set *set);

void ReplaceData(Set *set, Set *source);

void ClearAnonymousAndEmptySets();

void FreeSets();