#include "Executor.h"
#include "Gc.h"
#include "Utils.h"

#include <time.h>

#define PI 3.141592653589
#define E 2.718281828459
int psi;

Object* Executor::getVar(String varName)
{
	if(!varStack.isDefined(varName)){
		IntObject* obj = allocInt();
		obj->set(0);
		obj->incRefer();
		varStack.set(varName, obj);
	}
	return varStack.get(varName);
}

void Executor::defineGlobalVal(String varName, Object* obj) {
	if (!varStack.isDefined(varName)) {
		obj->incRefer();
		varStack.set(varName, obj);
	}
	else {
		throw "redefined value!" + varName;
	}
}

void Executor::defineGlobalFunc(String varName, OBJFUNC func) {
	if (!varStack.isDefined(varName)) {
		FuncObject* obj = allocFunc();
		obj->incRefer();
		obj->set(func);
		varStack.set(varName, obj);
	}
	else {
		throw "redefined func!" + varName;
	}
}

void Executor::clearStatements()
{
	_finalObj = NULL;
	psi = 0;
	statements.empty();
}

void Executor::setFinalVar(Object * finalObj)
{
	if (_finalObj != NULL) {
		_finalObj->decRefer();
		_finalObj = NULL;
	}
	if (finalObj != NULL) {
		_finalObj = finalObj->tryGetNewObject();
		_finalObj->incRefer();
	}	
}

Object * Executor::getFinalVar()
{
	return _finalObj;
}

void Executor::clearTmpVar(String prev)
{
	CArray<String> keys = varStack.getKeys();
	for (int i = 0; i < keys.size(); i++) {
		if (keys[i].indexOf(prev) == 0) {
			Object* obj = varStack.get(keys[i]);
			obj->decRefer();
			varStack.erase(keys[i]);
		}
	}
}

void Executor::dumpVar()
{
	CArray<String> keys = varStack.getKeys();
	for (int i = 0; i < keys.size(); i++) {
		Object* obj = varStack.get(keys[i]);
		if (obj != NULL && !obj->isType(ObjectTypes::funcType) && keys[i] != "PI" && keys[i] != "E") {
			printf("(\%s)  %s = %s\n", obj->getType().c_str(), keys[i].c_str(), obj->getStringValue().c_str());
		}
	}
}

void handleIntInput(StackNode& sn, LR1* lr1)
{
	int res = 0;
	sscanf(sn[0].szName.c_str(),"%d",&res);
	IntObject* obj = allocInt();
	obj->set(res);
	sn[0].ExtraInfo = obj;
}

void handleDoubleInput(StackNode& sn, LR1* lr1)
{
	double res = 0;
	sscanf(sn[0].szName.c_str(), "%lf", &res);
	FloatObject* obj = allocFloat();
	obj->set(res);
	sn[0].ExtraInfo = obj;
}

void handleStringInput(StackNode& sn, LR1* lr1)
{
	StringObject* obj = allocString();
	obj->set(sn[0].szName);
	sn[0].ExtraInfo = obj;
}
	
void handleVar(StackNode& sn, LR1* lr1)		
{
	Executor* exec = getExecutor(lr1);
	sn[0].ExtraInfo = exec->getVar(sn[0].szName);
}
void handleVarAttr(StackNode& sn, LR1* lr1)		
{
	Object* var = (Object*)sn[0].ExtraInfo;
	sn[0].ExtraInfo = var->getAttr(sn[2].szName);
}
void handleFunccall(StackNode& sn, LR1* lr1)		
{
	Object* func = (Object*)sn[0].ExtraInfo;
	CArray<Object*> *pargs = (CArray<Object*>*)sn[2].ExtraInfo;
	Object* var = func->callFunc(pargs[0]);
	delete pargs;
	sn[0].ExtraInfo = var;
}
void handleFunccall0(StackNode& sn, LR1* lr1)		
{
	Object* func = (Object*)sn[0].ExtraInfo;
	CArray<Object*> args;
	Object* var = func->callFunc(args);
	sn[0].ExtraInfo = var;
}
void handleSet(StackNode& sn, LR1* lr1)		
{
	Executor* exec = getExecutor(lr1);
	Object* var = (Object*)sn[2].ExtraInfo;
 	sn[0].ExtraInfo = var;
	Object* varOld = exec->getVar(sn[0].szName);
	varOld->decRefer();
	Object* varNew = (Object*)sn[0].ExtraInfo;
	exec->varStack.set(sn[0].szName, varNew);
	varNew->incRefer();
	psi ++;
}

void handleFinalSet(StackNode& sn, LR1* lr1)
{
	Executor* exec = getExecutor(lr1);
	Object* var = (Object*)sn[0].ExtraInfo;
	exec->setFinalVar(var);
}

void handleSetVar(StackNode& sn, LR1* lr1)
{
	Executor* exec = getExecutor(lr1);
	Object* var = exec->getVar(sn[2].szName);
	Object* varOld = exec->getVar(sn[0].szName);
	varOld->decRefer();
	Object* varNew = var->tryGetNewObject();
	exec->varStack.set(sn[0].szName, varNew);
	varNew->incRefer();
	sn[0].ExtraInfo = varNew;
	psi++;
}

void handleIfValueGoto(StackNode& sn, LR1* lr1)		
{
	Object* var = (Object*)sn[1].ExtraInfo;
	if(var->getBoolValue()){
		sscanf(sn[3].szName, "%d", &psi);
	}else{
		psi ++;
	}
}

void handleIfVarGoto(StackNode& sn, LR1* lr1)
{
	Executor* exec = getExecutor(lr1);
	Object* var = exec->getVar(sn[1].szName);
	if (var->getBoolValue()) {
		sscanf(sn[3].szName, "%d", &psi);
	}
	else {
		psi++;
	}
}

void handleArgsValue(StackNode& sn, LR1* lr1)		
{
	CArray<Object*> *pargs = new CArray<Object*>();
	pargs->add((Object*)sn[0].ExtraInfo);
	sn[0].ExtraInfo = pargs;
}

void handleArgsVar(StackNode& sn, LR1* lr1)
{
	Executor* exec = getExecutor(lr1);
	CArray<Object*> *pargs = new CArray<Object*>();
	Object* var = exec->getVar(sn[0].szName);
	pargs->add(var);
	sn[0].ExtraInfo = pargs;
}

void handleArgsnValue(StackNode& sn, LR1* lr1)		
{
	CArray<Object*> *pargs = (CArray<Object*>*)sn[0].ExtraInfo;
	pargs->add((Object*)sn[2].ExtraInfo);
}

void handleArgsnVar(StackNode& sn, LR1* lr1)
{
	Executor* exec = getExecutor(lr1);
	CArray<Object*> *pargs = (CArray<Object*>*)sn[0].ExtraInfo;
	Object* var = exec->getVar(sn[2].szName);
	pargs->add(var);
}

void handleGoto(StackNode& sn, LR1* lr1)		
{
	sscanf(sn[1].szName, "%d", &psi);
// 	Object* var = (Object*)sn[1].ExtraInfo;
// 	psi = var->getIntValue();
}

struct globalFunc {
	const char* funcName;
	OBJFUNC func;
}globalFuncLists[] = { {"trace", GlobalFuncs::trace}, {"gcd", GlobalFuncs::gcd}, {"lcm", GlobalFuncs::lcm}, {"axbyc", GlobalFuncs::axbyc},
	{"powermod", GlobalFuncs::powermod},{"decompose", GlobalFuncs::decompose},
	{"abs",MathFuncs::abs},{ "acos",MathFuncs::acos },{ "asin",MathFuncs::asin },{ "atan",MathFuncs::atan },
	{ "ceil",MathFuncs::ceil },{ "cos",MathFuncs::cos },{ "exp",MathFuncs::exp },{ "floor",MathFuncs::floor }, 
	{ "log",MathFuncs::log },{ "log10",MathFuncs::log10 },{ "max",MathFuncs::max },{ "min",MathFuncs::min }, 
	{ "pow",MathFuncs::pow },{ "random",MathFuncs::random },{ "round",MathFuncs::round },{ "sin",MathFuncs::sin }, 
	{ "sqrt",MathFuncs::sqrt },{ "tan",MathFuncs::tan }};

Executor::Executor(OBJMAN* globalObjs)
{
	lr1.InitRule("D:\\ExeWenFa.txt", true);
	lr1.outputStateToFile("D:\\extState.txt");
	lr1.registerHandler("\"V\"->\"int\"", handleIntInput);
	lr1.registerHandler("\"V\"->\"double\"", handleDoubleInput);
	lr1.registerHandler("\"V\"->\"string\"", handleStringInput);
	lr1.registerHandler("\"P\"->\"v\"", handleVar);
//	lr1.registerHandler("\"V\"->\"P\"", handleVar);
	lr1.registerHandler("\"P\"->\"P\"\".\"\"v\"", handleVarAttr);

	lr1.registerHandler("\"B\"->\"V\"", handleArgsValue);
	lr1.registerHandler("\"B\"->\"v\"", handleArgsVar);
	lr1.registerHandler("\"B\"->\"B\"\",\"\"V\"", handleArgsnValue);
	lr1.registerHandler("\"B\"->\"B\"\",\"\"v\"", handleArgsnVar);
	lr1.registerHandler("\"F\"->\"P\"\"(\"\"B\"\")\"", handleFunccall);
	lr1.registerHandler("\"F\"->\"P\"\"(\"\")\"", handleFunccall0);
	lr1.registerHandler("\"A\"->\"v\"\"=\"\"F\"", handleSet);
	lr1.registerHandler("\"A\"->\"v\"\"=\"\"V\"", handleSet);
	lr1.registerHandler("\"A\"->\"v\"\"=\"\"v\"", handleSetVar);
	lr1.registerHandler("\"A\"->\"IF\"\"V\"\"GOTO\"\"int\"", handleIfValueGoto);
	lr1.registerHandler("\"A\"->\"IF\"\"v\"\"GOTO\"\"int\"", handleIfVarGoto);
	lr1.registerHandler("\"A\"->\"GOTO\"\"int\"", handleGoto);
	lr1.registerHandler("\"S\"->\"A\"", handleFinalSet);

	lr1.setParam("executor", this);

	for (int i = 0; i < sizeof(globalFuncLists) / sizeof(globalFuncLists[0]); i++) {
		defineGlobalFunc(globalFuncLists[i].funcName, globalFuncLists[i].func);
	}
	FloatObject* obj = allocFloat();
	obj->set(PI);
	defineGlobalVal("PI", obj);
	obj = allocFloat();
	obj->set(E);
	defineGlobalVal("E", obj);

	srand(time(NULL));

	_finalObj = NULL;
	psi = 0;
}
void Executor::execute()
{
	while(psi < statements.size()){
		if(theGC.isNeedGC()){
			theGC.gc();
		}
		if(lr1.Recognize(statements.get(psi).c_str(), false) != 0)
		{
			throw "Ö´ÐÐÓï¾äÊ§°Ü!";
		}
	}
}
Executor::~Executor(){
	lr1.setParam("executor", NULL);
}
