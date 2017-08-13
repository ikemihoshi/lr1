#include "CStruct.h"
#include "Object.h"
#include <math.h>

#ifndef _C_DEFAULT_FUNCS_H_
#define _C_DEFAULT_FUNCS_H_

void checkParams(CArray<Object*>& params, int numOfParams);
/*Object* objectDefaultSet(CArray<Object*> params, Object* objCall);*/

class GlobalFuncs
{
public:
	static Object* trace(CArray<Object*> params, Object* objCall);
	static Object* gcd(CArray<Object*> params, Object* objCall);
	static Object* lcm(CArray<Object*> params, Object* objCall);
	static Object* axbyc(CArray<Object*> params, Object* objCall);
	static Object* decompose(CArray<Object*> params, Object* objCall);
	static Object* powermod(CArray<Object*> params, Object* objCall);
};

class MathFuncs{
public:
	static Object* abs(CArray<Object*> params, Object* objCall);
	static Object* acos(CArray<Object*> params, Object* objCall);
	static Object* asin(CArray<Object*> params, Object* objCall);
	static Object* atan(CArray<Object*> params, Object* objCall);
	static Object* ceil(CArray<Object*> params, Object* objCall);
	static Object* cos(CArray<Object*> params, Object* objCall);
	static Object* exp(CArray<Object*> params, Object* objCall);
	static Object* floor(CArray<Object*> params, Object* objCall);
	static Object* log(CArray<Object*> params, Object* objCall);
	static Object* log10(CArray<Object*> params, Object* objCall);
	static Object* max(CArray<Object*> params, Object* objCall);
	static Object* min(CArray<Object*> params, Object* objCall);
	static Object* pow(CArray<Object*> params, Object* objCall);
	static Object* random(CArray<Object*> params, Object* objCall);
	static Object* round(CArray<Object*> params, Object* objCall);
	static Object* sin(CArray<Object*> params, Object* objCall);
	static Object* sqrt(CArray<Object*> params, Object* objCall);
	static Object* tan(CArray<Object*> params, Object* objCall);
};

class IntFuncs
{
public:
	static Object* intAdd(CArray<Object*> params, Object* objCall);
	static Object* intSub(CArray<Object*> params, Object* objCall);
	static Object* intMul(CArray<Object*> params, Object* objCall);
	static Object* intDiv(CArray<Object*> params, Object* objCall);
	static Object* intMod(CArray<Object*> params, Object* objCall);
	
	static Object* intAddAsign(CArray<Object*> params, Object* objCall);
	static Object* intSubAsign(CArray<Object*> params, Object* objCall);
	static Object* intMulAsign(CArray<Object*> params, Object* objCall);
	static Object* intDivAsign(CArray<Object*> params, Object* objCall);
	static Object* intModAsign(CArray<Object*> params, Object* objCall);

	static Object* intLeftShift(CArray<Object*> params, Object* objCall);
	static Object* intRightShift(CArray<Object*> params, Object* objCall);
	static Object* intLeftShiftAsign(CArray<Object*> params, Object* objCall);
	static Object* intRightShiftAsign(CArray<Object*> params, Object* objCall);

	static Object* intAnd(CArray<Object*> params, Object* objCall);
	static Object* intOr(CArray<Object*> params, Object* objCall);
	static Object* intNot(CArray<Object*> params, Object* objCall);
	static Object* intXor(CArray<Object*> params, Object* objCall);

	static Object* intAndAsign(CArray<Object*> params, Object* objCall);
	static Object* intOrAsign(CArray<Object*> params, Object* objCall);
	static Object* intXorAsign(CArray<Object*> params, Object* objCall);

	static Object* intPositive(CArray<Object*> params, Object* objCall);
	static Object* intNegative(CArray<Object*> params, Object* objCall);

	static Object* intIncA(CArray<Object*> params, Object* objCall);
	static Object* intDecA(CArray<Object*> params, Object* objCall);
	static Object* intIncP(CArray<Object*> params, Object* objCall);
	static Object* intDecP(CArray<Object*> params, Object* objCall);
	static Object* intJg(CArray<Object*> params, Object* objCall);
	static Object* intJb(CArray<Object*> params, Object* objCall);
	static Object* intJge(CArray<Object*> params, Object* objCall);
	static Object* intJbe(CArray<Object*> params, Object* objCall);
	static Object* intCmp(CArray<Object*> params, Object* objCall);
	static Object* intNcmp(CArray<Object*> params, Object* objCall);
	static Object* toString(CArray<Object*> params, Object* objCall);
};

class StringFuncs
{
public:
	static Object* stringAdd(CArray<Object*> params, Object* objCall);
	static Object* stringJg(CArray<Object*> params, Object* objCall);
	static Object* stringJb(CArray<Object*> params, Object* objCall);
	static Object* stringJge(CArray<Object*> params, Object* objCall);
	static Object* stringJbe(CArray<Object*> params, Object* objCall);
	static Object* stringCmp(CArray<Object*> params, Object* objCall);
	static Object* stringNcmp(CArray<Object*> params, Object* objCall);
	static Object* toString(CArray<Object*> params, Object* objCall);

	static Object* stringIndexOf(CArray<Object*> params, Object* objCall);
	static Object* stringLastIndexOf(CArray<Object*> params, Object* objCall);
	static Object* stringSubString(CArray<Object*> params, Object* objCall);
	static Object* stringReplace(CArray<Object*> params, Object* objCall);
//	static Object* stringCharAt(CArray<Object*> params, Object* objCall);
//	static Object* stringMid(CArray<Object*> params, Object* objCall);		//operator[]
	static Object* stringCompare(CArray<Object*> params, Object* objCall);
	static Object* stringEquals(CArray<Object*> params, Object* objCall);
	static Object* stringLength(CArray<Object*> params, Object* objCall);
	static Object* stringStartsWith(CArray<Object*> params, Object* objCall);
	static Object* stringEndsWith(CArray<Object*> params, Object* objCall);
//	static Object* stringAppend(CArray<Object*> params, Object* objCall);		delete
//	static Object* stringSplit(CArray<Object*> params, Object* objCall);	//return array
};

class BoolFuncs
{
public:
	static Object* boolToString(CArray<Object*> params, Object* objCall);
	static Object* boolNot(CArray<Object*> params, Object* objCall);
};

class FloatFuncs
{
public:
	static Object* floatAdd(CArray<Object*> params, Object* objCall);
	static Object* floatSub(CArray<Object*> params, Object* objCall);
	static Object* floatMul(CArray<Object*> params, Object* objCall);
	static Object* floatDiv(CArray<Object*> params, Object* objCall);
	static Object* floatMod(CArray<Object*> params, Object* objCall);

	static Object* floatAddAsign(CArray<Object*> params, Object* objCall);
	static Object* floatSubAsign(CArray<Object*> params, Object* objCall);
	static Object* floatMulAsign(CArray<Object*> params, Object* objCall);
	static Object* floatDivAsign(CArray<Object*> params, Object* objCall);
	static Object* floatModAsign(CArray<Object*> params, Object* objCall);

	static Object* floatPositive(CArray<Object*> params, Object* objCall);
	static Object* floatNegative(CArray<Object*> params, Object* objCall);

	static Object* floatJg(CArray<Object*> params, Object* objCall);
	static Object* floatJb(CArray<Object*> params, Object* objCall);
	static Object* floatJge(CArray<Object*> params, Object* objCall);
	static Object* floatJbe(CArray<Object*> params, Object* objCall);
	static Object* floatCmp(CArray<Object*> params, Object* objCall);
	static Object* floatNcmp(CArray<Object*> params, Object* objCall);
	static Object* toString(CArray<Object*> params, Object* objCall);
};

class NullFuncs
{
public:
	static Object* nullAdd(CArray<Object*> params, Object* objCall);
	static Object* nullCmp(CArray<Object*> params, Object* objCall);
	static Object* nullNcmp(CArray<Object*> params, Object* objCall);
};

#endif