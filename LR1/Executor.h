#pragma once
#include"CStruct.h"
#include"MyString.h"
#include "lr1.h"
#include "Object.h"
#include "DefaultObjectFunction.h"

class Executor
{
public:
	Executor(OBJMAN* globalObjs=NULL);
	OBJMAN varStack;
	~Executor();
	void execute();

	LR1 lr1;
	CArray<String> statements;
	Object* getVar(String varName);
	void defineGlobalVal(String varName, Object * obj);
	void defineGlobalFunc(String varName, OBJFUNC func);

	void clearStatements();

	void setFinalVar(Object* finalObj);
	Object* getFinalVar();
	void clearTmpVar(String prev);
	void dumpVar();
private:
	Object* _finalObj;
};

