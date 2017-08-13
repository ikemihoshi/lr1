#pragma once
#include<math.h>
#include "MyString.h"
#include "lr1.h"
#include "Executor.h"

#ifndef _C_UTILS_H_
#define _C_UTILS_H_

extern char szFmt[];

typedef long long ll;

String formatInt(int value, unsigned int fmt);

String decompose(int v);
void axbyg(int &a, int &b, int &g);
int gcd(int a, int b);
int lcm(int a, int b);
int powermod(int a, int b, int c);

Executor* getExecutor(LR1* lr1);

void setExecutor(LR1* lr1, Executor* exec);
#endif