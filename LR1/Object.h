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
/*	Object��,�����Ͽ��԰�������һ��������
/*	����������Ժͷ���(����û�в��,����Object)
/************************************************************************/
class Object{
public:
	Object();
	/************************************************************************/
	/*	��ȡ����,���û�ж�Ӧ������ֵ�򷵻�nullptr,
	/*	������Զ���᷵��C++,�����NULL
	/************************************************************************/
	
	virtual Object* getAttr(String attrName);
	/************************************************************************/
	/*	���ú���, params��һ������,������Ĳ���                                                                    */
	/************************************************************************/

	virtual Object* callFunc(CArray<Object*> params);
	/************************************************************************/
	/* �����Ƿ���ڶ�Ӧ������                                                                     */
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
// 	virtual Object* getReferValue();		//�������õ����� a = 10; refer b = a; b = 20;=>a = 20

	/************************************************************************/
	/* �������Ƿ���ָ��������
	/* IntObject it(0);it.isType("int") => true;
	/************************************************************************/
	virtual bool isType(String typeName);

	/************************************************************************/
	/* ���ݵ�ǰ����ֵ���п�¡
	/*����Ŀ�¡����ȿ�¡,����������Եݹ���п�¡
	/*����һ��,���ﻹ���жѺ�ջ�ĸ����(��Ȼ��Ҷ��ڶ�����...)
	/*������ͨ����(int֮���Դ�����,clone()�᷵��һ���µĶ���)
	/*����Object����,��᷵����ͬ����������
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

