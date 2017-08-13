#include "DefaultObjectFunction.h"
#include "Object.h"
#include "Gc.h"
#include "Utils.h"
#include <math.h>
#include <stdlib.h>

void checkParams(CArray<Object*> &params, int numOfParams)
{
	if(params.size() != numOfParams){
		throw "参数个数不匹配!";
	}
}
// Object* objectDefaultSet(CArray<Object*> params, Object* objCall)
// {
// 	return NULL;
// 	checkParams(params, 1);
// 	objCall.getAttr("this").setObject(params[0].getObject()->clone());
// 	return objCall.getAttr("this");
// }

Object* IntFuncs::intAdd(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::stringType)){
		StringObject* obj = allocString();
		obj->set(params[0]->getStringValue() + objCall->getStringValue());
		return obj;
	}else if(params[0]->isType(ObjectTypes::charType) 
			|| params[0]->isType(ObjectTypes::intType)){
		IntObject* obj = allocInt();
		obj->set(params[0]->getIntValue() + objCall->getIntValue());
		return obj;
	}
	else if (params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(params[0]->getDoubleValue() + objCall->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
//	else if(params[0]->isType())
//	IntObject* obj = allocInt();
//	obj->set(params[0]->getIntValue() + objCall->getIntValue());
//	return obj;
// 	if(params[0].getType(ObjectTypes::floatType))
// 	{
// 		//return new FloatObject(params.get(0)->getDoubleValue() + params.get(1)->getDoubleValue());
// 		throw "error!";
// 	}
// 	else
// 	{
// 		return new IntObject(params.get(0)->getIntValue() + objCall->getIntValue());
// 	}
}
Object* IntFuncs::intSub(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::boolType) || params[0]->isType(ObjectTypes::charType) 
		|| params[0]->isType(ObjectTypes::intType)){
			IntObject* obj = allocInt();
			obj->set(objCall->getIntValue() - params[0]->getIntValue());
			return obj;
	}
	else if (params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(objCall->getDoubleValue() - params[0]->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intMul(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::intType)) {
		IntObject* obj = allocInt();
		obj->set(params[0]->getIntValue() * objCall->getIntValue());
		return obj;
	}
	else if (params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(params[0]->getDoubleValue() * objCall->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intDiv(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::intType)) {
		IntObject* obj = allocInt();
		obj->set(objCall->getIntValue() / params[0]->getIntValue());
		return obj;
	}
	else if (params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(objCall->getDoubleValue() / params[0]->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intMod(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::intType)) {
		IntObject* obj = allocInt();
		obj->set(objCall->getIntValue() % params[0]->getIntValue());
		return obj;
	}
	else if (params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(fmod(objCall->getDoubleValue(), params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intAddAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType) 
		|| params[0]->isType(ObjectTypes::floatType)) {
		((IntObject*)objCall)->set(objCall->getIntValue() + params[0]->getIntValue());
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intSubAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		((IntObject*)objCall)->set(objCall->getIntValue() - params[0]->getIntValue());
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intMulAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		((IntObject*)objCall)->set(objCall->getIntValue() * params[0]->getIntValue());
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intDivAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		((IntObject*)objCall)->set(objCall->getIntValue() / params[0]->getIntValue());
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intModAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		((IntObject*)objCall)->set(objCall->getIntValue() % params[0]->getIntValue());
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intLeftShift(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		IntObject* obj = allocInt();
		obj->set(objCall->getIntValue() << params[0]->getIntValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intRightShift(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		IntObject* obj = allocInt();
		obj->set(objCall->getIntValue() >> params[0]->getIntValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intLeftShiftAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		((IntObject*)objCall)->set(objCall->getIntValue() << params[0]->getIntValue());
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intRightShiftAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		((IntObject*)objCall)->set(objCall->getIntValue() >> params[0]->getIntValue());
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intAnd(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		IntObject* obj = allocInt();
		obj->set(objCall->getIntValue() & params[0]->getIntValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intOr(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		IntObject* obj = allocInt();
		obj->set(objCall->getIntValue() | params[0]->getIntValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intNot(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 0);
	IntObject* obj = allocInt();
	obj->set(~objCall->getIntValue());
	return obj;
}

Object * IntFuncs::intXor(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		IntObject* obj = allocInt();
		obj->set(objCall->getIntValue() ^ params[0]->getIntValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intAndAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		((IntObject*)objCall)->set(objCall->getIntValue() & params[0]->getIntValue());
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intOrAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		((IntObject*)objCall)->set(objCall->getIntValue() | params[0]->getIntValue());
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intXorAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		((IntObject*)objCall)->set(objCall->getIntValue() ^ params[0]->getIntValue());
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * IntFuncs::intPositive(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 0);
	IntObject* obj = allocInt();
	obj->set(objCall->getIntValue());
	return obj;
}

Object * IntFuncs::intNegative(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 0);
	IntObject* obj = allocInt();
	obj->set(-objCall->getIntValue());
	return obj;
}

Object * IntFuncs::intIncA(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 0);
	IntObject* obj = allocInt();
	obj->set(objCall->getIntValue());
	((IntObject*)objCall)->set(objCall->getIntValue() + 1);
	return obj;
}

Object * IntFuncs::intDecA(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 0);
	IntObject* obj = allocInt();
	obj->set(objCall->getIntValue());
	((IntObject*)objCall)->set(objCall->getIntValue() - 1);
	return obj;
}

Object * IntFuncs::intIncP(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 0);
	IntObject* obj = allocInt();
	obj->set(objCall->getIntValue() + 1);
	((IntObject*)objCall)->set(objCall->getIntValue() + 1);
	return obj;
}

Object * IntFuncs::intDecP(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 0);
	IntObject* obj = allocInt();
	obj->set(objCall->getIntValue() - 1);
	((IntObject*)objCall)->set(objCall->getIntValue() - 1);
	return obj;
}

Object* IntFuncs::intJg(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::boolType) || params[0]->isType(ObjectTypes::charType) 
		|| params[0]->isType(ObjectTypes::intType)){
			BoolObject* obj = allocBool();
			obj->set(objCall->getIntValue() > params[0]->getIntValue());
			return obj;
	}
	else if (params[0]->isType(ObjectTypes::floatType)) {
		BoolObject* obj = allocBool();
		obj->set(objCall->getDoubleValue() > params[0]->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* IntFuncs::intJb(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::boolType) || params[0]->isType(ObjectTypes::charType) 
		|| params[0]->isType(ObjectTypes::intType)){
			BoolObject* obj = allocBool();
			obj->set(objCall->getIntValue() < params[0]->getIntValue());
			return obj;
	}
	else if (params[0]->isType(ObjectTypes::floatType)) {
		BoolObject* obj = allocBool();
		obj->set(objCall->getDoubleValue() < params[0]->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* IntFuncs::intJge(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::boolType) || params[0]->isType(ObjectTypes::charType) 
		|| params[0]->isType(ObjectTypes::intType)){
			BoolObject* obj = allocBool();
			obj->set(objCall->getIntValue() >= params[0]->getIntValue());
			return obj;
	}
	else if (params[0]->isType(ObjectTypes::floatType)) {
		BoolObject* obj = allocBool();
		obj->set(objCall->getDoubleValue() >= params[0]->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* IntFuncs::intJbe(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::boolType) || params[0]->isType(ObjectTypes::charType) 
		|| params[0]->isType(ObjectTypes::intType)){
			BoolObject* obj = allocBool();
			obj->set(objCall->getIntValue() <= params[0]->getIntValue());
			return obj;
	}
	else if (params[0]->isType(ObjectTypes::floatType)) {
		BoolObject* obj = allocBool();
		obj->set(objCall->getDoubleValue() <= params[0]->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* IntFuncs::intCmp(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)){
			BoolObject* obj = allocBool();
			obj->set(objCall->getIntValue() == params[0]->getIntValue());
			return obj;
	}
	else if (params[0]->isType(ObjectTypes::floatType)) {
		BoolObject* obj = allocBool();
		obj->set(fabs(objCall->getDoubleValue() - params[0]->getDoubleValue()) < MIN_DIFF);
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* IntFuncs::intNcmp(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)){
		BoolObject* obj = allocBool();
		obj->set(objCall->getIntValue() != params[0]->getIntValue());
		return obj;
	}
	else if (params[0]->isType(ObjectTypes::floatType)) {
		BoolObject* obj = allocBool();
		obj->set(fabs(objCall->getDoubleValue() - params[0]->getDoubleValue()) >= MIN_DIFF);
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object* IntFuncs::toString(CArray<Object*> params, Object* objCall)
{
	StringObject *obj = allocString();
	if (params.size() == 0) {
		obj->set(objCall->getStringValue());
		return obj;
	}
	else if (params.size() == 1) {
		int digType = 10;
		if (params[0]->isType(ObjectTypes::intType)) {
			digType = ((IntObject*)params[0])->getIntValue();
		}
		else if (params[0]->isType(ObjectTypes::floatType)) {
			digType = ((FloatObject*)params[0])->getIntValue();
		}
		else {
			throw "param type error!";
		}
		obj->set(formatInt(objCall->getIntValue(), digType));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object* StringFuncs::stringAdd(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	StringObject* obj = allocString(); 
	obj->set(objCall->getStringValue() + params[0]->getStringValue());
	return obj;

}
Object* StringFuncs::stringJg(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::stringType))
	{
		BoolObject* obj = allocBool();
		obj->set(objCall->getStringValue() > params[0]->getStringValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* StringFuncs::stringJb(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::stringType))
	{
		BoolObject* obj = allocBool();
		obj->set(objCall->getStringValue() < params[0]->getStringValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* StringFuncs::stringJge(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::stringType))
	{
		BoolObject* obj = allocBool();
		obj->set(objCall->getStringValue() >= params[0]->getStringValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* StringFuncs::stringJbe(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::stringType))
	{
		BoolObject* obj = allocBool();
		obj->set(objCall->getStringValue() <= params[0]->getStringValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* StringFuncs::stringCmp(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::stringType))
	{
		BoolObject* obj = allocBool();
		obj->set(objCall->getStringValue() == params[0]->getStringValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* StringFuncs::stringNcmp(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::stringType))
	{
		BoolObject* obj = allocBool();
		obj->set(objCall->getStringValue() != params[0]->getStringValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* StringFuncs::toString(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 0);
	StringObject* obj = allocString(); 
	obj->set(objCall->getStringValue());
	return obj;
}

Object* StringFuncs::stringIndexOf(CArray<Object*> params, Object* objCall)
{
	if(params.size() == 1 && params[0]->isType(ObjectTypes::stringType)){
		IntObject* obj = allocInt();
		obj->set(objCall->getStringValue().find(params[0]->getStringValue()));
		return obj;
	}
	if(params.size() == 2 && 
		(params[0]->isType(ObjectTypes::stringType) || params[0]->isType(ObjectTypes::charType)) &&
		params[1]->isType(ObjectTypes::intType))
	{
		IntObject* obj = allocInt();
		obj->set(objCall->getStringValue().find(params[0]->getStringValue(), ((IntObject*)(params[1]))->getIntValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* StringFuncs::stringLastIndexOf(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::stringType) || params[0]->isType(ObjectTypes::charType))
	{
		IntObject* obj = allocInt();
		obj->set(objCall->getStringValue().find_last_of(params[0]->getStringValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* StringFuncs::stringSubString(CArray<Object*> params, Object* objCall)
{
	if(params.size() == 1 && params[0]->isType(ObjectTypes::intType))
	{
		int start = params[0]->getIntValue();
		if(start < 0)
		{
			start = 0;
		}
		StringObject *obj = allocString();
		obj->set(objCall->getStringValue().substr(start));
		return obj;
	}
	if(params.size() == 2 && params[0]->isType(ObjectTypes::intType) && params[1]->isType(ObjectTypes::intType))
	{
		int istart = 0,iend = 0;
		istart = params[0]->getIntValue();
		iend = params[1]->getIntValue();
		if(istart < 0)
		{
			istart = 0;
		}
		if(iend < 0)
		{
			iend = 0;
		}
		StringObject *obj = allocString();
		obj->set(objCall->getStringValue().substr(istart, iend));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* StringFuncs::stringReplace(CArray<Object*> params, Object* objCall)
{
	if(params.size() == 2 && params[0]->isType(ObjectTypes::stringType) && params[1]->isType(ObjectTypes::stringType))
	{
		String str = objCall->getStringValue();
		str.replace(params[0]->getStringValue().c_str(), params[1]->getStringValue().c_str());
		StringObject *obj = allocString();
		obj->set(str);
		return obj;
	}
	if(params.size() == 3 && params[0]->isType(ObjectTypes::stringType) && params[1]->isType(ObjectTypes::stringType) && params[2]->isType(ObjectTypes::intType))
	{
		String str = objCall->getStringValue();
		str.replace(params[0]->getStringValue().c_str(), params[1]->getStringValue().c_str(), params[2]->getIntValue());
		StringObject *obj = allocString();
		obj->set(str);
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
//	static Object* stringCharAt(CArray<Object*> params, Object* objCall);
//	static Object* stringMid(CArray<Object*> params, Object* objCall);		//operator[]
Object* StringFuncs::stringCompare(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::stringType))
	{
		IntObject *obj = allocInt();
		obj->set(objCall->getStringValue().compare(params[0]->getStringValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* StringFuncs::stringEquals(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::stringType))
	{
		BoolObject* obj = allocBool();
		obj->set(objCall->getStringValue() == params[0]->getStringValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* StringFuncs::stringLength(CArray<Object*> params, Object* objCall)
{
	IntObject* obj = allocInt();
	obj->set(objCall->getStringValue().length());
	return obj;
}
Object* StringFuncs::stringStartsWith(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::stringType))
	{
		BoolObject* obj = allocBool();
		obj->set(objCall->getStringValue().find(params[0]->getStringValue()) == 0);
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
Object* StringFuncs::stringEndsWith(CArray<Object*> params, Object* objCall)
{
	checkParams(params, 1);
	if(params[0]->isType(ObjectTypes::stringType))
	{
		BoolObject* obj = allocBool();
		obj->set(objCall->getStringValue().find_last_of(params[0]->getStringValue()) == objCall->getStringValue().length() - params[0]->getStringValue().length());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object* BoolFuncs::boolToString(CArray<Object*> params, Object* objCall)
{
	StringObject* obj = allocString();
	if(objCall->getBoolValue()){
		obj->set("true");
	}else{
		obj->set("false");
	}
	return obj;
}
Object* BoolFuncs::boolNot(CArray<Object*> params, Object* objCall)
{
	BoolObject* obj = allocBool();
	obj->set(!obj->getBoolValue());
	return obj;
}

Object * FloatFuncs::floatAdd(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::stringType)) {
		StringObject* obj = allocString();
		obj->set(objCall->getStringValue() + params[0]->getStringValue());
		return obj;
	}
	else if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType) ||
		params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(objCall->getDoubleValue() + params[0]->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatSub(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType) ||
		params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(objCall->getDoubleValue() - params[0]->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatMul(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType) ||
		params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(objCall->getDoubleValue() * params[0]->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatDiv(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType) ||
		params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(objCall->getDoubleValue() / params[0]->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatMod(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType) ||
		params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(fmod(objCall->getDoubleValue(), params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatAddAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		((FloatObject*)objCall)->set(objCall->getDoubleValue() + params[0]->getDoubleValue());
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatSubAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		((FloatObject*)objCall)->set(objCall->getDoubleValue() - params[0]->getDoubleValue());
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatMulAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		((FloatObject*)objCall)->set(objCall->getDoubleValue() * params[0]->getDoubleValue());
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatDivAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		((FloatObject*)objCall)->set(objCall->getDoubleValue() / params[0]->getDoubleValue());
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatModAsign(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		((FloatObject*)objCall)->set(fmod(objCall->getDoubleValue() , params[0]->getDoubleValue()));
		return objCall;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatPositive(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 0);
	FloatObject* obj = allocFloat();
	obj->set(objCall->getDoubleValue());
	return obj;
}

Object * FloatFuncs::floatNegative(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 0);
	FloatObject* obj = allocFloat();
	obj->set(-objCall->getDoubleValue());
	return obj;
}

Object * FloatFuncs::floatJg(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType) ||
		params[0]->isType(ObjectTypes::floatType)) {
		BoolObject* obj = allocBool();
		obj->set(objCall->getDoubleValue() > params[0]->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatJb(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType) ||
		params[0]->isType(ObjectTypes::floatType)) {
		BoolObject* obj = allocBool();
		obj->set(objCall->getDoubleValue() < params[0]->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatJge(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType) ||
		params[0]->isType(ObjectTypes::floatType)) {
		BoolObject* obj = allocBool();
		obj->set(objCall->getDoubleValue() >= params[0]->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatJbe(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType) ||
		params[0]->isType(ObjectTypes::floatType)) {
		BoolObject* obj = allocBool();
		obj->set(objCall->getDoubleValue() <= params[0]->getDoubleValue());
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatCmp(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType) ||
		params[0]->isType(ObjectTypes::floatType)) {
		BoolObject* obj = allocBool();
		obj->set(fabs(objCall->getDoubleValue() - params[0]->getDoubleValue()) < MIN_DIFF);
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::floatNcmp(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::charType) || params[0]->isType(ObjectTypes::intType) ||
		params[0]->isType(ObjectTypes::floatType)) {
		BoolObject* obj = allocBool();
		obj->set(fabs(objCall->getDoubleValue() - params[0]->getDoubleValue()) >= MIN_DIFF);
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * FloatFuncs::toString(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 0);
	StringObject* obj = allocString();
	obj->set(String("") + objCall->getDoubleValue());
	return obj;
}

Object * GlobalFuncs::trace(CArray<Object*> params, Object * objCall)
{
	for (int i = 0; i < params.size(); i++) {
		printf("%s\n", params.get(i)->getStringValue().c_str());
	}
	return VoidObject::voidObj;
}

Object * GlobalFuncs::gcd(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 2);
	IntObject* obj = allocInt();
	obj->set(::gcd(params[0]->getIntValue(), params[1]->getIntValue()));
	return obj;
}

Object * GlobalFuncs::lcm(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 2);
	IntObject* obj = allocInt();
	obj->set(::lcm(params[0]->getIntValue(), params[1]->getIntValue()));
	return obj;
}

Object * GlobalFuncs::axbyc(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 2);
	StringObject* obj = allocString();
	int a = params[0]->getIntValue();
	int b = params[1]->getIntValue();
	int g = 1;
	axbyg(a, b, g);
	obj->set(String("") + a + "," + b + "," + g);
	return obj;
}

Object * GlobalFuncs::decompose(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	StringObject* obj = allocString();
	obj->set(::decompose(params[0]->getIntValue()));
	return obj;
}

Object * GlobalFuncs::powermod(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 3);
	IntObject* obj = allocInt();
	obj->set(::powermod(params[0]->getIntValue(), params[1]->getIntValue(), params[2]->getIntValue()));
	return obj;
}

Object * NullFuncs::nullAdd(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	StringObject* obj = allocString();
	obj->set(String("null") + objCall->getDoubleValue());
	return obj;
}

Object * NullFuncs::nullCmp(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	BoolObject* obj = allocBool();
	obj->set(objCall == params.get(0));
	return obj;
}

Object * NullFuncs::nullNcmp(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	BoolObject* obj = allocBool();
	obj->set(objCall != params.get(0));
	return obj;
}

Object * MathFuncs::abs(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType)) {
		IntObject* obj = allocInt();
		obj->set(::abs(params[0]->getIntValue()));
		return obj;
	}
	if (params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::abs(params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::acos(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::acos(params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::asin(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::asin(params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::atan(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::atan(params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::ceil(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType) 
		|| params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::ceil(params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::cos(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::cos(params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::exp(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::exp(params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::floor(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::floor(params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::log(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::log(params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::log10(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::log10(params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::max(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 2);
	if ((params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType))
		&&(params[1]->isType(ObjectTypes::intType) || params[1]->isType(ObjectTypes::charType))) {
		IntObject* obj = allocInt();
		obj->set(::max(params[0]->getIntValue(), params[1]->getIntValue()));
		return obj;
	}
	else if (params[0]->isType(ObjectTypes::floatType) && params[1]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::max(params[0]->getDoubleValue(), params[1]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::min(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 2);
	if ((params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType))
		&& (params[1]->isType(ObjectTypes::intType) || params[1]->isType(ObjectTypes::charType))) {
		IntObject* obj = allocInt();
		obj->set(::min(params[0]->getIntValue(), params[1]->getIntValue()));
		return obj;
	}
	else if (params[0]->isType(ObjectTypes::floatType) && params[1]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::min(params[0]->getDoubleValue(), params[1]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::pow(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 2);
	if ((params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType))
		&& (params[1]->isType(ObjectTypes::intType) || params[1]->isType(ObjectTypes::charType))) {
		IntObject* obj = allocInt();
		obj->set(::pow(params[0]->getIntValue(), params[1]->getIntValue()));
		return obj;
	}
	else if (params[0]->isType(ObjectTypes::floatType) && params[1]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::pow(params[0]->getDoubleValue(), params[1]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::random(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 0);
	FloatObject* obj = allocFloat();
	obj->set(1.0 * (::rand() % (1<<30)) / (1<<30));
	return obj;
}

Object * MathFuncs::round(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::round(params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::sin(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::sin(params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::sqrt(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::sqrt(params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}

Object * MathFuncs::tan(CArray<Object*> params, Object * objCall)
{
	checkParams(params, 1);
	if (params[0]->isType(ObjectTypes::intType) || params[0]->isType(ObjectTypes::charType)
		|| params[0]->isType(ObjectTypes::floatType)) {
		FloatObject* obj = allocFloat();
		obj->set(::tan(params[0]->getDoubleValue()));
		return obj;
	}
	throw "error! params is not suitable for the function!";
}
