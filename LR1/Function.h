#include "VariableManager.h"
class Function
{
public:
	String funcName;			//func
	String packageName;			//package
	String className;			//class
	String fullName;			//package.class.func,在execute的时候始终会调用这种方式,以上的只是用来判断是否存在func
	void addStatement(String statement);

};