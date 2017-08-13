#include "Object.h"

#ifndef _H_GC_H__
#define _H_GC_H__

#define GC_NUM_LIMIT 10
class GC
{
public:
	GC();
	~GC();
	void gc();
	bool isNeedGC();
	Object* allocObject(String type);
	void free(Object* obj);
private:
	CMap<String, CArray<Object*> > _inUse;
	CMap<String, CArray<Object*> > _unUse;
	bool _isNeedGC;
	int _countTimesGC;
};

extern GC theGC;
#define gcnew(type) theGC.allocObject(type);

IntObject* allocInt();
FloatObject* allocFloat();
BoolObject* allocBool();
StringObject* allocString();
FuncObject* allocFunc();
#endif