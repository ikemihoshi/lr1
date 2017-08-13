#include "MyString.h"
#include "CStruct.h"

#ifndef _H_VARIABLE_H_
#define _H_VARIABLE_H_

class Object;

class Variable
{
public:
	Variable();
	Variable(Object* obj);
	String getType();
	Variable callFunc(CArray<Variable> params);
	Variable getAttr(String attrName);
	void setObject(Object* newObj);
	Object* getObject();
private:
	Object* _obj;
};

#endif