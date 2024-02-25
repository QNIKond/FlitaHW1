#include <stdio.h>
#include <malloc.h>

int mlocCount = 0;

//Types that are lower in list have higher priority and lower position in tree
typedef enum
{
    TTOpenBracket,
    TTCloseBracket,

    TTEqual,
    TTExcept,
    TTOr,
    TTAnd,

    TTCount,
    TTUnknown,
    TTVariable,
    TTEOF

} TokenType;

#define ISLETTER(X) (((X>='A')&&(X<='Z')) || ((X>='a')&&(X<='z')) || (X=='_'))
#define ISNUM(X) ((X>='0')&&(X<='9'))
#define ISLETTERORNUM(X) (ISLETTER(X) || ISNUM(X))

#define MAXSETSIZE 48

typedef struct SET
{
    char* name;
    int* data;

    struct SET* nextSet;
} Set;
Set* sets;

Set* GetLastSet(Set* set)
{
    if (set->nextSet)
        return GetLastSet(set->nextSet);
    return set;
}

Set* CreateSet(char* name, int* data)
{
    Set* set = malloc(sizeof(Set));
    ++mlocCount;
    set->name = name;
    set->data = data;
    set->nextSet = 0;
    if (sets)
        GetLastSet(sets)->nextSet = set;
    else
        sets = set;
    return set;
}

Set* FindSet(char* name)
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
    return result;
}

void FreeSets(Set* set)
{
    if(set->nextSet)
        FreeSets(set->nextSet);
    if(set->name)
    {
        free(set->name);
        --mlocCount;
    }
    if(set->data)
    {
        free(set->data);
        --mlocCount;
    }
    free(set);
    --mlocCount;
}

typedef struct TOKEN
{
    struct TOKEN* prevToken;
    struct TOKEN* lvalue;
    struct TOKEN* rvalue;

    int priority;
    TokenType type;
    Set* set;

    struct TOKEN* prevCreated;
} Token;

Token* CreateToken(TokenType type, Set* set)
{
    static Token* lastCreatedToken = 0;
    Token* newToken = malloc(sizeof(Token));
    ++mlocCount;
    Token toAssign = {0,0,0,0,type,set,lastCreatedToken};
    *newToken =  toAssign;

    lastCreatedToken = newToken;
    return newToken;
}

void FreeTokens(Token* token)
{
    if(token->prevCreated != 0)
        FreeTokens(token->prevCreated);
    free(token);
    --mlocCount;
}

void AddToTokenTree(Token* lastToken, Token* newToken)
{

    if (lastToken == 0)
        return;
    if((lastToken->priority < newToken->priority) ||
    ((lastToken->priority == newToken->priority) && (lastToken->type <= newToken->type)))
    {
        newToken->lvalue = lastToken->rvalue;
        lastToken->rvalue = newToken;
        newToken->prevToken = lastToken;
    }
    else if (lastToken->prevToken != 0)
    {
        AddToTokenTree(lastToken->prevToken, newToken);
    }
    else
    {
        newToken->lvalue = lastToken;
        lastToken->prevToken = newToken;
    }
}

void Strip(char** head)
{
    while((**head==' ')||(**head=='\t'))
        ++(*head);
}
char* CopyStr(char* start, char* end)
{
    char* str = calloc(end-start+1,sizeof(char));
    ++mlocCount;
    for(int i = 0;i < end - start;++i)
        str[i] = start[i];
    str[end-start] = 0;
    return str;
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
    ++mlocCount;
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
    Set* set = FindSet(*head);
    if (set==0)
    {
        set = CreateSet(CopyStr(*head,end), 0);

    }
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

Token* GetTokenTree(char* head)
{
    int priority = 0;
    Token* lastToken = 0;
    Token* nextToken = GetNextToken(&head);

    while(nextToken->type != TTEOF)
    {
        if((nextToken->type == TTOpenBracket) || (nextToken->type == TTCloseBracket))
        {
            priority += nextToken->type == TTOpenBracket ? 1 : -1;
            nextToken = GetNextToken(&head);
            continue;
        }
        nextToken->priority = priority;
        AddToTokenTree(lastToken,nextToken);
        lastToken = nextToken;
        nextToken = GetNextToken(&head);
    }
    while(lastToken->prevToken)
        lastToken = lastToken->prevToken;
    return lastToken;
}

int main()
{
    Token* t;
    const char* operators[9] = {"(",")","&","|","/","=","count","unknown","var"};
    char* s = "A={1,3,4,5}/B";
    char* head = s;
    t = GetTokenTree(head);
    //printf("%d %d %d\n", t[0],t[1],t[2]);
    fflush(stdout);

/*     char s[101];
    do
    {
        fgets(s,101,stdin);
        fflush(stdin);
        printf("%d  %d  %d  %d",s[0],s[1],s[2],s[3]);
    }
    while(0); */
    FreeTokens(CreateToken(0,0));
    FreeSets(sets);
    printf("\n------%d------",mlocCount);

}
