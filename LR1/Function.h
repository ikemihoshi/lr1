#include "VariableManager.h"
class Function
{
public:
	String funcName;			//func
	String packageName;			//package
	String className;			//class
	String fullName;			//package.class.func,��execute��ʱ��ʼ�ջ�������ַ�ʽ,���ϵ�ֻ�������ж��Ƿ����func
	void addStatement(String statement);

};