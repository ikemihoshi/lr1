#include"Object.h"
#include"DefaultObjectFunction.h"
#include"Gc.h"

const String ObjectTypes::intType = "int";
const String ObjectTypes::floatType = "float";
const String ObjectTypes::charType = "char";
const String ObjectTypes::stringType = "string";
const String ObjectTypes::voidType = "void";
const String ObjectTypes::nullType = "null";
const String ObjectTypes::mapType = "map";
const String ObjectTypes::arrayType = "array";
const String ObjectTypes::funcType = "function";
const String ObjectTypes::referType = "refer";
const String ObjectTypes::boolType = "bool";

Object* Object::callFunc(CArray<Object*> params)
{
	if(this->isType(ObjectTypes::funcType))
	{
		return ((FuncObject*)this)->call(params);
	}
	throw getType() + " is not a function!";
}
Object* Object::getAttr(String attrName)
{
	if(_attrs.isDefined(attrName))
	{
		return _attrs.get(attrName);
	}
	throw getType() + "." + attrName + " is not defined!";
}

String Object::getType()
{
	return _type;
}

Object* Object::clone()
{
	throw "clone() not implemented by subclass!";
}

Object * Object::tryGetNewObject()
{
	return this;
}

double Object::getDoubleValue()
{
	throw "class " + _type + " can't be cast to int!";
}

int Object::getIntValue()
{
	throw "class " + _type + " can't be cast to int!";
}

bool Object::getBoolValue()
{
	throw "class " + _type + " can't be cast to bool!";
}

String Object::getStringValue()
{
	return _type;
}

bool Object::isType(String typeName)
{
	return _type == typeName;
}

void Object::incRefer()
{
	_refer_times++;
}
void Object::decRefer()
{
	_refer_times--;
}
int Object::getRefer()
{
	return _refer_times;
}
int Object::setRefer(int ref)
{
	_refer_times = ref;
	return ref;
}
void Object::addMethod(String methodName, OBJFUNC func)
{
	FuncObject* obj = NULL;
	if(_attrs.isDefined(methodName)){
		if(_attrs.get(methodName) == NULL || !_attrs.get(methodName)->isType(ObjectTypes::funcType)){
			throw "添加成员函数"+_type+"."+methodName+"失败!成员已经存在且为空(函数类型不允许为空)或者该成员不是函数类型!";
		}
		obj = (FuncObject*)_attrs.get(methodName);
	}else{
		obj = (FuncObject*)theGC.allocObject(ObjectTypes::funcType);
		obj->incRefer();
	}
	obj->set(func, this);
	_attrs.set(methodName, obj);
}

Object::Object(){
	_refer_times = 0;
}

Object::~Object()
{
}

//////////////////////////////////////////////////////////////////////////


IntObject::IntObject()
{
	_type = ObjectTypes::intType;
	_value = 0;
}
void IntObject::set(int value, Object* objThis){
	_attrs.set("this",objThis);
	_value = value;
	initFuncs();
}

void IntObject::initFuncs()
{
	addMethod("operator+", IntFuncs::intAdd);
	addMethod("operator-", IntFuncs::intSub);
	addMethod("operator*", IntFuncs::intMul);
	addMethod("operator/", IntFuncs::intDiv);
	addMethod("operator%", IntFuncs::intMod);
	addMethod("operator+=", IntFuncs::intAddAsign);
	addMethod("operator-=", IntFuncs::intSubAsign);
	addMethod("operator*=", IntFuncs::intMulAsign);
	addMethod("operator/=", IntFuncs::intDivAsign);
	addMethod("operator%=", IntFuncs::intModAsign);

	addMethod("operator<<", IntFuncs::intLeftShift);
	addMethod("operator>>", IntFuncs::intRightShift);
	addMethod("operator<<=", IntFuncs::intLeftShiftAsign);
	addMethod("operator>>=", IntFuncs::intRightShiftAsign);

	addMethod("operator&", IntFuncs::intAnd);
	addMethod("operator|", IntFuncs::intOr);
	addMethod("operator~", IntFuncs::intNot);
	addMethod("operator^", IntFuncs::intXor);

	addMethod("operator&=", IntFuncs::intAndAsign);
	addMethod("operator|=", IntFuncs::intOrAsign);
	addMethod("operator^=", IntFuncs::intXorAsign);

	addMethod("operator-p", IntFuncs::intNegative);
	addMethod("operator+p", IntFuncs::intPositive);

	addMethod("operator++a", IntFuncs::intIncA);
	addMethod("operator--a", IntFuncs::intDecA);
	addMethod("operator++p", IntFuncs::intIncP);
	addMethod("operator--p", IntFuncs::intDecP);
	addMethod("operator>", IntFuncs::intJg);
	addMethod("operator<", IntFuncs::intJb);
	addMethod("operator>=", IntFuncs::intJge);
	addMethod("operator<=", IntFuncs::intJbe);
	addMethod("operator==", IntFuncs::intCmp);
	addMethod("operator!=", IntFuncs::intNcmp);
	addMethod("toString", IntFuncs::toString);
// 	_attrs.set("operator+", new FuncObject(IntFuncs::intAdd, this));
// 	_attrs.set("toString", new FuncObject(IntFuncs::toString, this));
//	_attrs.set("operator=", new FuncObject(objectDefaultSet, this));
}
Object* IntObject::clone()
{
	IntObject* obj = allocInt();
	obj->set(_value);
	return obj;
}
int IntObject::getIntValue()
{
	return _value;
}

bool IntObject::getBoolValue()
{
	return _value != 0;
}

double IntObject::getDoubleValue()
{
	return _value;
}

String IntObject::getStringValue()
{
	return String("") + _value;
}

Object * IntObject::tryGetNewObject()
{
	return clone();
}

//////////////////////////////////////////////////////////////////////////

BoolObject::BoolObject()
{
	_type = ObjectTypes::boolType;
}
void BoolObject::set(bool value, Object* objThis){
	_attrs.set("this",objThis);
	_value = value;
	initFuncs();
}
void BoolObject::initFuncs()
{
	addMethod("toString", BoolFuncs::boolToString);
	addMethod("operator!", BoolFuncs::boolNot);
//	addMethod("toString", IntFuncs::toString);
}
Object* BoolObject::clone()
{
	BoolObject* obj = allocBool();
	obj->set(_value);
	return obj;
}
/*int BoolObject::getIntValue()
{
	return _value?1:0;
}*/

bool BoolObject::getBoolValue()
{
	return _value;
}

String BoolObject::getStringValue()
{
	if(_value){
		return "true";
	}
	return "false";
}

Object * BoolObject::tryGetNewObject()
{
	return clone();
}

//////////////////////////////////////////////////////////////////////////

StringObject::StringObject()
{
	_type = ObjectTypes::stringType;
}
void StringObject::set(String value, Object* objThis){
	_attrs.set("this",objThis);
	_value = value;
	initFuncs();
}
void StringObject::initFuncs()
{
	addMethod("toString", StringFuncs::toString);
	addMethod("operator+", StringFuncs::stringAdd);
	addMethod("operator==", StringFuncs::stringCmp);
	addMethod("compare", StringFuncs::stringCompare);
	addMethod("endWith", StringFuncs::stringEndsWith);
	addMethod("equals", StringFuncs::stringEquals);
	addMethod("indexOf", StringFuncs::stringIndexOf);
	addMethod("operator<", StringFuncs::stringJb);
	addMethod("operator<=", StringFuncs::stringJbe);
	addMethod("operator>", StringFuncs::stringJg);
	addMethod("operator>=", StringFuncs::stringJge);
//	addMethod("lastIndexOf", StringFuncs::stringLastIndexOf);
	addMethod("length", StringFuncs::stringLength);
	addMethod("operator!=", StringFuncs::stringNcmp);
	addMethod("replace", StringFuncs::stringReplace);
	addMethod("startWith", StringFuncs::stringStartsWith);
	addMethod("subString", StringFuncs::stringSubString);
}
Object* StringObject::clone()
{
	StringObject* obj = allocString();
	obj->set(_value);
	return obj;
}

String StringObject::getStringValue()
{
	return _value;
}

//////////////////////////////////////////////////////////////////////////

FuncObject::FuncObject()
{
	_type = ObjectTypes::funcType;
	_nativeFunc = NULL;
}

void FuncObject::set(OBJFUNC func, Object* objThis)
{
	_attrs.set("this",objThis);
	_nativeFunc = func;
}
Object* FuncObject::clone()
{
	FuncObject* obj = (FuncObject*)theGC.allocObject(ObjectTypes::funcType);
	obj->_attrs.set("this", _attrs.get("this"));
	obj->_nativeFunc = _nativeFunc;
	return obj;
}
Object* FuncObject::call(CArray<Object*> params)
{
	if(_nativeFunc != NULL)
	{
		return _nativeFunc(params, _attrs.get("this"));
	}
	else
	{
		throw "native Function is NULL!";
	}
}

String FuncObject::getStringValue()
{
	return _type + String::format("[%0x%x]",unsigned int(_nativeFunc));
}

//////////////////////////////////////////////////////////////////////////

ReferObject::ReferObject()
{
	_type = ObjectTypes::referType;
}

void ReferObject::set(Object* referedObj, Object*objThis)
{
	_referedObj = referedObj;
}

Object* ReferObject::clone()
{
	ReferObject* obj = (ReferObject*)theGC.allocObject(ObjectTypes::referType);
	obj->set(_referedObj);
	return obj;
}

Object* ReferObject::getAttr(String attrName)
{
	return _referedObj->getAttr(attrName);
}

Object* ReferObject::callFunc(CArray<Object*> params)
{
	return _referedObj->callFunc(params);
}

FloatObject::FloatObject()
{
	_type = ObjectTypes::floatType;
	_value = 0.0;
}

void FloatObject::set(double value, Object * objThis)
{
	_attrs.set("this", objThis);
	_value = value;
	initFuncs();
}

Object * FloatObject::clone()
{
	FloatObject* obj = allocFloat();
	obj->set(_value);
	return obj;
}

int FloatObject::getIntValue()
{
	return _value;
}

bool FloatObject::getBoolValue()
{
	return fabs(_value) > MIN_DIFF;
}

double FloatObject::getDoubleValue()
{
	return _value;
}

String FloatObject::getStringValue()
{
	return String("") + _value;
}

Object * FloatObject::tryGetNewObject()
{
	return clone();
}

void FloatObject::initFuncs()
{
	addMethod("operator+", FloatFuncs::floatAdd);
	addMethod("operator-", FloatFuncs::floatSub);
	addMethod("operator*", FloatFuncs::floatMul);
	addMethod("operator/", FloatFuncs::floatDiv);
	addMethod("operator%", FloatFuncs::floatMod);
	addMethod("operator+=", FloatFuncs::floatAddAsign);
	addMethod("operator-=", FloatFuncs::floatSubAsign);
	addMethod("operator*=", FloatFuncs::floatMulAsign);
	addMethod("operator/=", FloatFuncs::floatDivAsign);
	addMethod("operator%=", FloatFuncs::floatModAsign);

	addMethod("operator-p", FloatFuncs::floatNegative);
	addMethod("operator+p", FloatFuncs::floatPositive);

	addMethod("operator>", FloatFuncs::floatJg);
	addMethod("operator<", FloatFuncs::floatJb);
	addMethod("operator>=", FloatFuncs::floatJge);
	addMethod("operator<=", FloatFuncs::floatJbe);
	addMethod("operator==", FloatFuncs::floatCmp);
	addMethod("operator!=", FloatFuncs::floatNcmp);
	addMethod("toString", FloatFuncs::toString);
}
NullObject* NullObject::nullObj = new NullObject();
String NullObject::getStringValue()
{
	return String("null");
}

NullObject::NullObject()
{
	_type = ObjectTypes::nullType;
	addMethod("operator+", NullFuncs::nullAdd);
	addMethod("operator==", NullFuncs::nullCmp);
	addMethod("operator!=", NullFuncs::nullCmp);
}

VoidObject* VoidObject::voidObj = new VoidObject();
VoidObject::VoidObject() 
{
	_type = ObjectTypes::voidType;
}
