#include "ExceptionHandler.h"

char* exceptionMessages[] = {"", "Expected %s","Missing argument","Uknown lexem"};

int exceptionCode = 0;
const char* messageMod;

int IsException()
{
    return exceptionCode;
}

const char* GetExceptionInfo(const char** mod, char** position)
{
    *mod = messageMod;
    return exceptionMessages[exceptionCode];
}

void* ThrowException(int exCode,const char* mod)
{
    if(!exceptionCode)
    {
        exceptionCode = exCode;
        messageMod = mod;
    }
    return 0;
}

void CheckException()
{
    exceptionCode = 0;
}