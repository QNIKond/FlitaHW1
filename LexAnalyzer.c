#include "LexAnalyzer.h"
#include "ExceptionHandler.h"

#define ISLETTER(X) (((X>='A')&&(X<='Z')) || ((X>='a')&&(X<='z')) || (X=='_'))
#define ISNUM(X) ((X>='0')&&(X<='9'))
#define ISLETTERORNUM(X) (ISLETTER(X) || ISNUM(X))

char* lexems[TTCount] = {0};
char* lineStart;

void BuildLexemsArray()
{
    lexems[TTOpenBracket] = "(";
    lexems[TTCloseBracket] = ")";
    lexems[TTEqual] = "=";
    lexems[TTEqualExcept] = "/=";
    lexems[TTEqualOr] = "|=";
    lexems[TTEqualAnd] = "&=";
    lexems[TTExcept] = "/";
    lexems[TTOr] = "|";
    lexems[TTAnd] = "&";
}

void NewLine(char* start)
{
    lineStart = start;
}

void Strip(char** head)
{
    while((**head==' ')||(**head=='\n')||(**head=='\t'))
        ++(*head);
}

int TryNumber(char** end, int* number)
{
    if(!ISNUM(**end))
        return 0;
    Strip(end);
    *number = 0;
    while(ISNUM(**end))
    {
        *number = (*number)*10 + **end - (int)'0';
        ++(*end);
    }
    return 1;
}

int TrySymbol(char** head, char symbol)
{
    Strip(head);
    if(**head == symbol)
    {
        ++(*head);
        Strip(head);
        return 1;
    }
    return 0;
}

Token* TryConstants(char** head)
{
    int nextNumber = 0;
    char* end = *head;
    if(!TrySymbol(&end,'{'))
        return 0;
    Set* set = CreateSet(0);
    if(!TryNumber(&end, &nextNumber)) {
        if (TrySymbol(&end, '}')) {
            *head = end;
            return CreateToken(TTVariable, set);
        } else {
            THROWEX(1, *head - lineStart)
        }
    }

    PlaceNumberInSet(set, nextNumber);
    while(*end != '}')
    {
        TrySymbol(&end,','); CHECKEX
        if(!TryNumber(&end, &nextNumber))
            THROWEX(1,  *head - lineStart)
        PlaceNumberInSet(set, nextNumber);
    }
    *head = end + 1;
    return CreateToken(TTVariable, set);
}

Token* TryVariables(char** head)
{
    if(!ISLETTER(**head))
        return 0;
    char* end = *head;
    while(ISLETTERORNUM(*(end)))
        ++end;
    Set* set = FindSet(*head, end - *head);

    *head = end;
    return CreateToken(TTVariable,set);
}

Token* TryKeyWords(char** head)
{
    if(!lexems[0])
        BuildLexemsArray();

    TokenType tokenType = TTUnknown;
    int maxLength = 0;
    int j;
    for(int i = 0; i < TTCount; ++i)
    {
        j = 0;
        while(((*head)[j]==lexems[i][j])&&((*head)[j]!=0))
            ++j;
        if ((lexems[i][j]==0)&&(j>maxLength))
        {
            maxLength = j;
            tokenType = i;
        }
    }
    if(tokenType == TTUnknown)
        return 0;
    (*head) += maxLength;
    return CreateToken(tokenType,0);
}

Token* GetNextToken(char** head)
{
    Strip(head);
    char* start = *head;
    if(**head==0)
        return CreateToken(TTEOF,0);

    Token* nextToken = TryKeyWords(head);
    if (!nextToken)
        nextToken = TryConstants(head); CHECKEX
    if (!nextToken)
        nextToken = TryVariables(head);
    if (!nextToken)
        THROWEX(3,  *head - lineStart)
    int t =     (start - lineStart);
    int k = (*head - start);
    nextToken->lexemPosition = t + k/2;
    return nextToken;
}