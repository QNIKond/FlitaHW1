#include "TokenTreeExecutor.h"
#include "Token.h"
#include "ExceptionHandler.h"

Set* ExecAnd(Set* lvalue, Set* rvalue)
{
    Set* newSet = CreateSet(0);
    int j = 0;
    for(int i = 0; i < lvalue->filled; ++i)
    {
        while((j<rvalue->filled) && (rvalue->data[j] < lvalue->data[i]))
            ++j;
        if (j>=rvalue->filled)
            break;
        if (rvalue->data[j]==lvalue->data[i])
            PlaceNumberInSet(newSet,lvalue->data[i]);
    }
    return newSet;
}
Set* ExecOr(Set* lvalue, Set* rvalue)
{
    Set* newSet = CreateUnnamedCopy(lvalue);
    for(int i = 0; i < rvalue->filled; ++ i)
        PlaceNumberInSet(newSet,rvalue->data[i]);
    return newSet;
}
Set* ExecExcept(Set* lvalue, Set* rvalue)
{
    Set* newSet = CreateSet(0);
    int j = 0;
    for(int i = 0; i < lvalue->filled; ++i)
    {
        while((j < rvalue->filled) && (rvalue->data[j] < lvalue->data[i]))
            ++j;

        if ((j >= rvalue->filled) || (rvalue->data[j] != lvalue->data[i]))
            PlaceNumberInSet(newSet,lvalue->data[i]);
    }
    return newSet;
}
Set* ExecEqual(Set* lvalue, Set* rvalue)
{
    if(!lvalue->name)
        THROWEX(1,"named set")
    ReplaceData(lvalue,rvalue);
    return lvalue;
}

Set* Execute(Set* lvalue, Set* rvalue,TokenType operator)
{
    switch (operator) {
        case TTEqual: return ExecEqual(lvalue,rvalue);
        case TTExcept: return ExecExcept(lvalue,rvalue);
        case TTOr: return ExecOr(lvalue,rvalue);
        case TTAnd: return ExecAnd(lvalue,rvalue);
        default: return 0;
    }
}

void TestNode(Token* node){}
Set* ExecuteTree(Token* node)
{
    if(node->type==TTVariable) {
        if (node->lvalue || node->rvalue)
            THROWEX(1,"operator")
        return node->set;
    }
    if(!node->lvalue || !node->rvalue )
        THROWEX(2,0)
    Set* lvalue = ExecuteTree(node->lvalue); CHECKEX
    Set* rvalue = ExecuteTree(node->rvalue); CHECKEX
    return Execute(lvalue,rvalue, node->type);
}
