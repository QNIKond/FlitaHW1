#include "LexAnalyzer.h"

#define ISLETTER(X) (((X>='A')&&(X<='Z')) || ((X>='a')&&(X<='z')) || (X=='_'))
#define ISNUM(X) ((X>='0')&&(X<='9'))
#define ISLETTERORNUM(X) (ISLETTER(X) || ISNUM(X))

void Strip(char** head)
{
    while((**head==' ')||(**head=='\n')||(**head=='\t'))
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


Token* GetConstant(char** head)
{
    Set* set = CreateSet(0);
    char* end = *head + 1;
    PlaceNumberInSet(set, GetNumber(&end));
    Strip(&end);
    while(*end != '}')
    {
        ++end;
        PlaceNumberInSet(set, GetNumber(&end));
        Strip(&end);
    }
    *head = end + 1;
    return CreateToken(TTVariable, set);
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
    Set* set = FindSet(*head, end - *head);

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