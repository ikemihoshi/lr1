#include"Variable.h"

class VariableManager
{
public:
	Variable* getVariable(String varName);
	bool isVariableExist(String varName);
	Variable* mallocVariable(String varName);
	void createVariablePackage();
	void destroyVariablePackage();
};