#include "SyntaxAnalyzer.h"
#include "Token.h"
#include "LexAnalyzer.h"

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