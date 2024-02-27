#include "../Headers/Builders/SyntaxAnalyzer.h"
#include "../Headers/Structures/Token.h"
#include "../Headers/Builders/LexAnalyzer.h"
#include "../Headers/ExceptionHandler.h"
#include "../Headers/Interface.h"
void AddToTokenTree(Token *lastToken, Token *newToken) {

    if (lastToken == 0)
        return;
    if ((lastToken->priority < newToken->priority) ||
        ((lastToken->priority == newToken->priority) && (lastToken->type <= newToken->type))) {
        newToken->lvalue = lastToken->rvalue;
        lastToken->rvalue = newToken;
        newToken->prevToken = lastToken;
    } else if (lastToken->prevToken != 0) {
        AddToTokenTree(lastToken->prevToken, newToken);
    } else {
        newToken->lvalue = lastToken;
        lastToken->prevToken = newToken;
    }
}

Token *BuildTokenTree(char *head) {
    NewLine(head);
    int priority = 0;
    Token *lastToken = 0;
    Token *nextToken = GetNextToken(&head);
    CHECKEX
    if(nextToken->type == TTHelp)
    {
        DrawHelpPage();
        return 0;
    }
    while (nextToken->type != TTEOF) {
        if ((nextToken->type == TTOpenBracket) || (nextToken->type == TTCloseBracket)) {
            priority += nextToken->type == TTOpenBracket ? 1 : -1;
            nextToken = GetNextToken(&head);
            CHECKEX
            continue;
        }
        nextToken->priority = priority;
        AddToTokenTree(lastToken, nextToken);
        lastToken = nextToken;
        nextToken = GetNextToken(&head);
        CHECKEX
    }
    if (lastToken)
        while (lastToken->prevToken)
            lastToken = lastToken->prevToken;
    return lastToken;
}