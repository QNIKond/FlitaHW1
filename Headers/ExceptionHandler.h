#pragma once

#include "Structures/Token.h"

#define CHECKEX if (IsException()) return 0;
#define THROWEX(X, Y) return (void*)ThrowException(X,Y);
#define MEMALCOUNT ++(*GetMemAllocCount());
#define MEMFREECOUNT --(*GetMemAllocCount());

int IsException();

const char *GetExceptionInfo(int *position);

void *ThrowException(int exCode, int position);

void CheckExceptions();

int *GetMemAllocCount();