#include "Function.h"				//�ݶ�������������
class FunctionManager
{
public:
	bool addFunction(Function* func);
	bool isFuncExist(String funcName);
	Function* getFunction(String funcName);
};