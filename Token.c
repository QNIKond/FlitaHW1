#include "Token.h"
#include <malloc.h>

static Token* lastCreatedToken = 0;

Token* CreateToken(TokenType type, Set* set)
{
    Token* newToken = malloc(sizeof(Token));
    //++mlocCount;
    Token toAssign = {0,0,0,0,type,set,0,lastCreatedToken};
    *newToken =  toAssign;

    lastCreatedToken = newToken;
    return newToken;
}

void RecursivelyFreeTokens(Token* token)
{
    if(token == lastCreatedToken)
        lastCreatedToken = 0;
    if(!token)
        return;
    if(token->prevCreated != 0)
        RecursivelyFreeTokens(token->prevCreated);
    free(token);
    //--mlocCount;
}

void FreeTokens()
{
    RecursivelyFreeTokens(lastCreatedToken);
}
