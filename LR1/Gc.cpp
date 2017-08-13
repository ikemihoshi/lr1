#include "Gc.h"

GC theGC;

GC::GC(){
	_isNeedGC = false;
	_countTimesGC = 0;
}
GC::~GC(){
}
bool GC::isNeedGC()
{
	return _isNeedGC;
}
void GC::free(Object* obj)
{
	if (obj->isType(ObjectTypes::voidType) || obj->isType(ObjectTypes::nullType)) {
		return;
	}
	if(!_inUse.isDefined(obj->getType()))
	{
		throw "object type is illegal";
	}
	CArray<Object*> &arrobj = _inUse[obj->getType()]; 
	int i;
	for(i = 0; i < arrobj.size(); i++)
	{
		if(arrobj[i] == obj)
		{
			break;
		}
	}
	if(i == arrobj.size())
	{
		throw "object is illegal";
	}
	if(_unUse.isDefined(obj->getType()))
	{
		_unUse.set(obj->getType(), CArray<Object*>());
	}
	_unUse[obj->getType()].add(obj);
	arrobj.del(i);
}
void GC::gc(){
	CArray<String> keys = _inUse.getKeys();
	for(int i = 0 ; i< keys.size(); i++){
		if(!_unUse.isDefined(keys[i])){
			_unUse.set(keys[i], CArray<Object*>());
		}
		for(int j = 0; j < _inUse.get(keys[i]).size(); j++){
			if(_inUse.get(keys[i]).get(j)->getRefer() <= 0){
				_unUse.get(keys[i]).add(_inUse.get(keys[i]).get(j));
				_inUse.get(keys[i]).del(j);
				j--;
			}
		}
	}
	_countTimesGC++;
	_isNeedGC = false;
}

Object* GC::allocObject(String type){
	if(!_inUse.isDefined(type)){
		_inUse.set(type, CArray<Object*>());
	}
	if(!_unUse.isDefined(type)){
		_unUse.set(type, CArray<Object*>());
	}
	Object* obj = NULL;
	if(_unUse.get(type).size() > 0){
		obj = _unUse.get(type).get(0);
		_unUse.get(type).del(0);
		_inUse.get(type).add(obj);
		obj->setRefer(0);
		return obj;
	}

	if(_inUse[type].size() >= GC_NUM_LIMIT){
		_isNeedGC = true;
	}
	
	if(type == ObjectTypes::intType){
		obj = new IntObject();
		_inUse[type].add(obj);
		return obj;
	}else if(type == ObjectTypes::stringType){
		obj = new StringObject();
		_inUse[type].add(obj);
		return obj;
	}else if(type == ObjectTypes::boolType){
		obj = new BoolObject();
		_inUse[type].add(obj);
		return obj;
	}else if(type == ObjectTypes::referType){
		obj = new ReferObject();
		_inUse[type].add(obj);
		return obj;
	}else if(type == ObjectTypes::funcType){
		obj = new FuncObject();
		_inUse[type].add(obj);
		return obj;
	}
	else if (type == ObjectTypes::floatType) {
		obj = new FloatObject();
		_inUse[type].add(obj);
		return obj;
	}
	throw "未知的类型分配!" + type;
}

IntObject * allocInt()
{
	return (IntObject *)theGC.allocObject(ObjectTypes::intType);
}

FloatObject * allocFloat()
{
	return (FloatObject *)theGC.allocObject(ObjectTypes::floatType);
}

BoolObject * allocBool()
{
	return (BoolObject *)theGC.allocObject(ObjectTypes::boolType);
}

StringObject * allocString()
{
	return (StringObject *)theGC.allocObject(ObjectTypes::stringType);
}

FuncObject * allocFunc()
{
	return (FuncObject *)theGC.allocObject(ObjectTypes::funcType);
}
