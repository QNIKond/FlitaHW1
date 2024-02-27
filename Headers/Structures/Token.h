#pragma once

#include "SetsTable.h"

typedef enum {
    TTHelp,
    TTOpenBracket,
    TTCloseBracket,

    TTEqual,
    TTEqualExcept,
    TTEqualOr,
    TTEqualAnd,
    TTExcept,
    TTOr,
    TTAnd,

    TTCount,
    TTUnknown,
    TTVariable,
    TTEOF

} TokenType;

typedef struct TOKEN {
    struct TOKEN *prevToken;
    struct TOKEN *lvalue;
    struct TOKEN *rvalue;

    int priority;
    TokenType type;
    Set *set;

    int lexemPosition;

    struct TOKEN *prevCreated;
} Token;

Token *CreateToken(TokenType type, Set *set);

void FreeTokens();

