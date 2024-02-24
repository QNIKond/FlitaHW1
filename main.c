#include <stdio.h>
#include <malloc.h>

int mlocCount = 0;

typedef enum
{
    TTOpenBracket,
    TTCloseBracket,
    TTAnd,
    TTOr,
    TTExcept,
    TTEqual,

    TTCount,
    TTUnknown,
    TTVariable,
    TTEOF

} TokenType;

#define ISLETTER(X) (((X>='A')&&(X<='Z')) || ((X>='a')&&(X<='z')) || (X=='_'))
#define ISLETTERORNUM(X) (((X>='0')&&(X<='9')) || ISLETTER(X))

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

Set* CreateSet(char* name)
{
    Set* set = malloc(sizeof(Set));
    ++mlocCount;
    set->name = name;
    set->data = 0;
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
        while((name[i] == currentSet->name[i]) && (name[i] != 0))
            ++i;
        if((currentSet->name[i] == 0) && (i > maxLength))
        {
            maxLength = i;
            result = currentSet;
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
    struct TOKEN* prevOperand;
    struct TOKEN* lvalue;
    struct TOKEN* rvalue;

    int priority;
    TokenType type;
    Set* set;

    struct TOKEN* prevCreated;
} Token;

Token* CreateToken(TokenType type, Set* set, int priority)
{
    static Token* lastCreatedToken = 0;
    Token* newToken = malloc(sizeof(Token));
    ++mlocCount;
    newToken->type = type;
    newToken->set = set;
    newToken->prevCreated = lastCreatedToken;
    newToken->priority = priority;
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

void AddToTokenTree(Token* lastOperand, Token* newOperand)
{
    if(lastOperand->type >= newOperand->type)
    {
        newOperand->lvalue = lastOperand->rvalue;
        lastOperand->rvalue = newOperand;
        newOperand->prevOperand = lastOperand;
    }
    else if (lastOperand->prevOperand != 0)
    {
        AddToTokenTree(lastOperand->prevOperand, newOperand);
    }
    else
    {
        newOperand->lvalue = lastOperand;
        lastOperand->prevOperand = newOperand;
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
    for(int i = 0;i < end - start;++i)
        str[i] = start[i];
    str[end-start] = 0;
    return str;
}

Token* GetConstant(char** head)
{
    return 0;
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
        set = CreateSet(CopyStr(*head,end));

    }
    *head = end;
    Strip(head);
    return CreateToken(TTVariable,set,0);
}

TokenType GetTokenType(char** head)
{
    const char* operators[TTCount] = {"(",")","&","|","/","="};

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
    Strip(head);
    return tokenType;
}

Token* GetNextToken(char** head, int* priority)
{
    if(**head==0)
        return 0;
    TokenType type = GetTokenType(head);
    if (type == TTVariable)
        return GetVariable(head);
    else if(type == TTOpenBracket)
    {
        ++(*priority);
        return GetNextToken(head,priority);
    }
    else if(type == TTCloseBracket)
    {
        --(*priority);
        return GetNextToken(head,priority);
    }
    else
        return CreateToken(type, 0,*priority);
}

Token* GetTokenTree(char* head)
{
    int priority;
    Strip(&head);
    Token* firstVariable = GetNextToken(&head,&priority);
    Token* lastOperand = GetNextToken(&head,&priority);
    lastOperand->rvalue = GetNextToken(&head,&priority);
    lastOperand->lvalue = firstVariable;


    Token* nextOperand;
    while((*head)!=0)
    {
        nextOperand = GetNextToken(&head,&priority);
        nextOperand->rvalue = GetNextToken(&head,&priority);
        AddToTokenTree(lastOperand,nextOperand);
        lastOperand = nextOperand;
    }
    while(lastOperand->prevOperand)
        lastOperand = lastOperand->prevOperand;
    return lastOperand;
}

int main()
{
    const char* t;
    int c = 0;
    const char* operators[9] = {"(",")","&","|","/","=","count","unknown","var"};
    char* s = "var1 = a | b&c | (d | e)";
    char* head = s;
    while(*head!=0)
    {
        t = operators[GetNextToken(&head,&c)->type];
        printf("%s %d\n", t, c);
    }
/*     char s[101];
    do
    {
        fgets(s,101,stdin);
        fflush(stdin);
        printf("%d  %d  %d  %d",s[0],s[1],s[2],s[3]);
    }
    while(0); */
    FreeTokens(CreateToken(0,0,0));
    FreeSets(sets);
    printf("\n------%d------",mlocCount);
}
