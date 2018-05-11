#ifndef MYDLL_H
#define MYDLL_H
#include "stdafx.h"
extern "C"_declspec(dllexport) int add(int a, int b);
extern "C"_declspec(dllexport) void logd(char *s);
#endif