#include "../Headers/Builders/TokenTreeExecutor.h"
#include "../Headers/Structures/Token.h"
#include "../Headers/Builders/SyntaxAnalyzer.h"
#include "../Headers/ExceptionHandler.h"
#include "../Headers/Interface.h"

Set *ExecAnd(Set *lvalue, Set *rvalue) {
    Set *newSet = CreateSet(0);
    int j = 0;
    for (int i = 0; i < lvalue->filled; ++i) {
        while ((j < rvalue->filled) && (rvalue->data[j] < lvalue->data[i]))
            ++j;
        if (j >= rvalue->filled)
            break;
        if (rvalue->data[j] == lvalue->data[i])
            PlaceNumberInSet(newSet, lvalue->data[i]);
    }
    return newSet;
}

Set *ExecOr(Set *lvalue, Set *rvalue) {
    Set *newSet = CreateUnnamedCopy(lvalue);
    for (int i = 0; i < rvalue->filled; ++i)
        PlaceNumberInSet(newSet, rvalue->data[i]);
    return newSet;
}

Set *ExecExcept(Set *lvalue, Set *rvalue) {
    Set *newSet = CreateSet(0);
    int j = 0;
    for (int i = 0; i < lvalue->filled; ++i) {
        while ((j < rvalue->filled) && (rvalue->data[j] < lvalue->data[i]))
            ++j;

        if ((j >= rvalue->filled) || (rvalue->data[j] != lvalue->data[i]))
            PlaceNumberInSet(newSet, lvalue->data[i]);
    }
    return newSet;
}

Set *ExecEqual(Set *lvalue, Set *rvalue, int position) {
    if (!lvalue->name)
        THROWEX(4, position)
    ReplaceData(lvalue, rvalue);
    return lvalue;
}

Set *ExecEqualExcept(Set *lvalue, Set *rvalue, int position) {
    return ExecEqual(lvalue, ExecExcept(lvalue, rvalue), position);
}

Set *ExecEqualOr(Set *lvalue, Set *rvalue, int position) {
    return ExecEqual(lvalue, ExecOr(lvalue, rvalue), position);
}

Set *ExecEqualAnd(Set *lvalue, Set *rvalue, int position) {
    return ExecEqual(lvalue, ExecAnd(lvalue, rvalue), position);
}

Set *Execute(Set *lvalue, Set *rvalue, Token *operator) {
    switch (operator->type) {
        case TTEqual:
            return ExecEqual(lvalue, rvalue, operator->lexemPosition);
        case TTEqualExcept:
            return ExecEqualExcept(lvalue, rvalue, operator->lexemPosition);
        case TTEqualOr:
            return ExecEqualOr(lvalue, rvalue, operator->lexemPosition);
        case TTEqualAnd:
            return ExecEqualAnd(lvalue, rvalue, operator->lexemPosition);
        case TTExcept:
            return ExecExcept(lvalue, rvalue);
        case TTOr:
            return ExecOr(lvalue, rvalue);
        case TTAnd:
            return ExecAnd(lvalue, rvalue);
        default:
            return 0;
    }
}

void TestNode(Token *node) {}

Set *ExecuteTree(Token *node) {
    if (!node)
        return 0;
    if (node->type == TTVariable) {
        if (node->lvalue || node->rvalue)
            THROWEX(5, node->lexemPosition)
        return node->set;
    }
    if (!node->lvalue || !node->rvalue)
        THROWEX(2, node->lexemPosition)
    Set *lvalue = ExecuteTree(node->lvalue);
    CHECKEX
    Set *rvalue = ExecuteTree(node->rvalue);
    CHECKEX
    return Execute(lvalue, rvalue, node);
}

Set *TryExecute(char *s) {
    Token *tree = BuildTokenTree(s);
    if (IsException()) {
        ShowExceptionMessage();
        ClearAnonymousAndEmptySets();
        return 0;
    }
    Set *set = ExecuteTree(tree);
    if (IsException()) {
        ShowExceptionMessage();
        ClearAnonymousAndEmptySets();
        return 0;
    }
    ClearAnonymousAndEmptySets();
    return set;
}