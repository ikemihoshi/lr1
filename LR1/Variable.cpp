#include "Variable.h"
#include "Object.h"

Variable::Variable()
{
	
	_obj = NULL;
}

Variable::Variable(Object* obj)
{
	_obj = obj;
}

Variable Variable::callFunc(CArray<Variable> params)
{
	return _obj->callFunc(params, this);
}

Variable Variable::getAttr(String attrName)
{
	return _obj->getAttr(attrName);
}

String Variable::getType()
{
	return _obj->getType();
}

void Variable::setObject(Object* newObj)
{
	_obj = newObj;
}
Object* Variable::getObject()
{
	return _obj;
}