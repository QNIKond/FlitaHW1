#include "ExceptionHandler.h"


char* exceptionMessages[] = {"", "Expected number","Missing argument","Uknown lexem",
                             "Expected named set", "Expected operator", "Number is too big"};

int exceptionCode = 0;
int exceptionPosition;

int IsException()
{
    return exceptionCode;
}

const char* GetExceptionInfo(int* position)
{
    *position = exceptionPosition;
    return exceptionMessages[exceptionCode];
}

void* ThrowException(int exCode, int position)
{
    if(!exceptionCode)
    {
        exceptionCode = exCode;
        exceptionPosition = position;
    }
    return 0;
}

void CheckException()
{
    exceptionCode = 0;
}