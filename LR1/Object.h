#include "CStruct.h"
#include "MyString.h"

#ifndef _C_OBJECT_H_
#define _C_OBJECT_H_

const double MIN_DIFF = 0.000000001;

class ObjectTypes
{
public:
	static const String intType;
	static const String floatType;
	static const String charType;
	static const String stringType;
	static const String voidType;
	static const String nullType;
	static const String mapType;
	static const String arrayType;
	static const String funcType;
	static const String referType;
	static const String boolType;
};

class Object;
typedef  Object* (*OBJFUNC)(CArray<Object*> params, Object* objCall);

/************************************************************************/
/*	Object类,基本上可以把它看成一个大容器
/*	里面会有属性和方法(两者没有差别,都是Object)
/************************************************************************/
class Object{
public:
	Object();
	/************************************************************************/
	/*	获取属性,如果没有对应的属性值则返回nullptr,
	/*	这里永远不会返回C++,里面的NULL
	/************************************************************************/
	
	virtual Object* getAttr(String attrName);
	/************************************************************************/
	/*	调用函数, params是一个数组,代表传入的参数                                                                    */
	/************************************************************************/

	virtual Object* callFunc(CArray<Object*> params);
	/************************************************************************/
	/* 返回是否存在对应的属性                                                                     */
	/************************************************************************/
	
// 	virtual bool isAttrExist(String attrName);
// 	virtual void setAttr(String attrName, Object* newAttr);
// 	virtual Object* clearAttr(String attrName);
	virtual String getType();

	virtual String getStringValue();
	virtual double getDoubleValue();
	virtual int getIntValue();
	virtual bool getBoolValue();
// 	virtual char getCharValue();
// 	virtual CMap<String,Object*> getMapValue();
// 	virtual CMap<int,Object*> getArrayValue();
// 	virtual Object* getReferValue();		//返回引用的引用 a = 10; refer b = a; b = 20;=>a = 20

	/************************************************************************/
	/* 检测变量是否是指定的类型
	/* IntObject it(0);it.isType("int") => true;
	/************************************************************************/
	virtual bool isType(String typeName);

	/************************************************************************/
	/* 根据当前变量值进行克隆
	/*这里的克隆是深度克隆,会对所有属性递归进行克隆
	/*修正一下,这里还是有堆和栈的概念的(虽然大家都在堆里面...)
	/*对于普通变量(int之类自带类型,clone()会返回一个新的对象)
	/*对于Object类型,则会返回相同的引用来着
	/************************************************************************/
	virtual Object* clone();

	virtual Object* tryGetNewObject();

// 	virtual bool addAttr(String attrName, Object* obj);
// 	virtual bool removeAttr(String attrName);

	virtual void addMethod(String methodName, OBJFUNC func);

	virtual void incRefer();
	virtual void decRefer();
	virtual int getRefer();
	virtual int setRefer(int ref);

	virtual ~Object();
protected:
	CMap<String, Object*> _attrs;
	String _type;
	int _refer_times;
};

typedef CMap<String, Object*> OBJMAN;

class IntObject : public Object
{
public:
	IntObject();
	void set(int value, Object* objThis = NULL);
	Object* clone();
	int getIntValue();
	bool getBoolValue();
	double getDoubleValue();
	String getStringValue();
	Object* tryGetNewObject();
private:
	int _value;
	void initFuncs();
};

class FloatObject : public Object
{
public:
	FloatObject();
	void set(double value, Object* objThis = NULL);
	Object* clone();
	int getIntValue();
	bool getBoolValue();
	double getDoubleValue();
	String getStringValue();
	Object* tryGetNewObject();
private:
	double _value;
	void initFuncs();
};

class BoolObject : public Object
{
public:
	BoolObject();
	void set(bool value, Object* objThis = NULL);
	Object* clone();
	//int getIntValue();
	bool getBoolValue();
	String getStringValue();
	Object* tryGetNewObject();
private:
	bool _value;
	void initFuncs();
};

class  StringObject : public Object
{
public:
	StringObject();
	void set(String value, Object* objThis = NULL);
	Object* clone();
	String getStringValue();
private:
	String _value;
	void initFuncs();
};

class FuncObject :public Object
{
public:
	FuncObject();
	void set(OBJFUNC func, Object* objThis = NULL);
	Object* call(CArray<Object*> params);
	Object* clone();
	String getStringValue();
private:
	OBJFUNC _nativeFunc;
};

class ReferObject : public Object
{
public:
	ReferObject();
	void set(Object* referedObj, Object* objThis = NULL);
	Object* getAttr(String attrName);
	Object* callFunc(CArray<Object*> params);
	Object* clone();
private:
	Object* _referedObj;
};

class NullObject : public Object
{
public:
	static NullObject* nullObj;
	String getStringValue();
private:
	NullObject();
};

class VoidObject : public Object
{
public:
	static VoidObject* voidObj;
private:
	VoidObject();
};

#endif

