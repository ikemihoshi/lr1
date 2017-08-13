#include "Function.h"				//暂定不允许方法重载
class FunctionManager
{
public:
	bool addFunction(Function* func);
	bool isFuncExist(String funcName);
	Function* getFunction(String funcName);
};