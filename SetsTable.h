#pragma once

typedef struct SET
{
    char* name;
    int* data;

    struct SET* nextSet;
} Set;

Set* CreateSet(char* name, int* data);

Set* GetSet(char* name, int length);

void FreeSets();