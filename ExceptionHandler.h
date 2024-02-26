#pragma once
#include "Token.h"

#define CHECKEX if (IsException()) return 0;
#define THROWEX(X,Y) return (void*)ThrowException(X,Y);

int IsException();

const char* GetExceptionInfo(int* position);

void* ThrowException(int exCode, int position);

void CheckException();