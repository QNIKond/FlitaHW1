#pragma once

#define CHECKEX if (IsException()) return 0;
#define THROWEX(X,Y) return (void*)ThrowException(X,Y);

int IsException();

const char* GetExceptionInfo(const char** mod, char** position);

void* ThrowException(int exCode,const char* mod);

void CheckException();