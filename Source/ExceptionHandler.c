#include "../Headers/ExceptionHandler.h"

int memAllocCount = 0;

char *exceptionMessages[] = {"", "Expected number", "Missing argument", "Uknown lexem",
                             "Expected named set", "Expected operator", "Number is too big",
                             "Expected comma","Invalid name for variable"};

int exceptionCode = 0;
int exceptionPosition;

int IsException() {
    return exceptionCode;
}

const char *GetExceptionInfo(int *position) {
    *position = exceptionPosition;
    return exceptionMessages[exceptionCode];
}

void *ThrowException(int exCode, int position) {
    if (!exceptionCode) {
        exceptionCode = exCode;
        exceptionPosition = position;
    }
    return 0;
}

void CheckExceptions() {
    exceptionCode = 0;
}

int *GetMemAllocCount() {
    return &memAllocCount;
}