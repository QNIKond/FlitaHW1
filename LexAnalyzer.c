#include "LexAnalyzer.h"
#include <malloc.h>

#define ISLETTER(X) (((X>='A')&&(X<='Z')) || ((X>='a')&&(X<='z')) || (X=='_'))
#define ISNUM(X) ((X>='0')&&(X<='9'))
#define ISLETTERORNUM(X) (ISLETTER(X) || ISNUM(X))

void Strip(char** head)
{
    while((**head==' ')||(**head=='\t'))
        ++(*head);
}

int GetNumber(char** end)
{
    Strip(end);
    int number = 0;
    while(ISNUM(**end))
    {
        number = number*10 + **end - (int)'0';
        ++(*end);
    }
    return number;
}
void PlaceNumberInSet(int* data,int* size, int n)
{
    int* ptr = data;
    while ((*ptr < n) && (ptr - data < *size))
        ++ptr;
    if ((ptr - data == *size) || (*ptr != n))
    {
        for(int* i = data + *size; i>ptr; --i)
            *i = *(i-1);
        *ptr = n;
        ++(*size);
    }
}

Token* GetConstant(char** head)
{
    int filled = 0;
    int size = 16;
    int* data = malloc(size*sizeof(int));
    //++mlocCount;
    char* end = *head + 1;
    PlaceNumberInSet(data, &filled, GetNumber(&end));
    Strip(&end);
    while(*end != '}')
    {
        ++end;
        PlaceNumberInSet(data, &filled, GetNumber(&end));
        Strip(&end);
        if (size - filled < 3)
        {
            size *= 2;
            data = realloc(data, size*sizeof(int));
        }
    }
    *head = end + 1;
    return CreateToken(TTVariable, CreateSet(0,data));
}

Token* GetVariable(char** head)
{
    if(**head=='{')
        return GetConstant(head);
    if(!ISLETTER(**head))
        return 0;
    char* end = *head;
    while(ISLETTERORNUM(*(end)))
        ++end;
    Set* set = GetSet(*head, end - *head);

    *head = end;
    return CreateToken(TTVariable,set);
}

TokenType GetTokenType(char** head)
{
    const char* operators[TTCount] = {"(",")","=","/","|","&"};

    TokenType tokenType = TTVariable;
    int maxLength = 0;
    int j;
    for(int i = 0; i < TTCount; ++i)
    {
        j = 0;
        while(((*head)[j]==operators[i][j])&&((*head)[j]!=0))
            ++j;
        if ((operators[i][j]==0)&&(j>maxLength))
        {
            maxLength = j;
            tokenType = i;
        }
    }
    (*head) += maxLength;
    return tokenType;
}

Token* GetNextToken(char** head)
{
    Strip(head);
    if(**head==0)
        return CreateToken(TTEOF,0);
    TokenType type = GetTokenType(head);
    if (type == TTVariable)
        return GetVariable(head);
    else
        return CreateToken(type, 0);
}