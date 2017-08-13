#include "lr1.h"
#include "Executor.h"
#include "Gc.h"
#include "Utils.h"

class Circle_
{
public:
	//下面一个意义不明
//	CSet<int> _preCircle;			//使用for循环的时候会用到
	CSet<int> _circleEntey;			//循环的入口(BlockEntry)
	CSet<int> _chargeEntry;			//判断的入口(ValueEntry)
	CSet<int> _endCircle;			//循环的终点(BlockEnd)

	int conditionStart;
	int circleStart;
	int circleEnd;
};

String tmpVarPre = "tmpVar#";
int tmpVarIndex = 0;
String getTmpVar()
{
	tmpVarIndex ++;
	if(tmpVarIndex < 0)
	{
		tmpVarIndex = 0;
		tmpVarPre = tmpVarPre + "#";
	}
	return tmpVarPre + tmpVarIndex;
}

void defFuncHandler2(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	String tmpVar = getTmpVar(); 
	String statement = tmpVar + " = " + sn[0].szName + " . operator" + sn[1].szName + " ( "+sn[2].szName+" )";
	executor->statements.add(statement);
	sn[0].szName = tmpVar;
}

void defFuncHandlerPrev(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	String tmpVar = getTmpVar(); 
	String statement = tmpVar + " = " + sn[1].szName + " . operator" + sn[0].szName + "p ( )";
	executor->statements.add(statement);
	sn[0].szName = tmpVar;
}

void defFuncHandlerAfter(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	String tmpVar = getTmpVar(); 
	String statement = tmpVar + " = " + sn[0].szName + " . operator" + sn[1].szName + "a ( )";
	executor->statements.add(statement);
	sn[0].szName = tmpVar;
}

void defFuncHandlerBrackets(StackNode& sn, LR1* lr1)
{
	sn[0].szName = sn[1].szName;
}

void defFuncHandler1(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	String tmpVar = getTmpVar();
	String statement = tmpVar + " = " + sn[1].szName + " . operator" + sn[0].szName + " ( )";
	executor->statements.add(statement);
	sn[0].szName = tmpVar;
}
void defFuncHandlerSet(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	String statement = sn[0].szName + " = " + sn[2].szName;
	executor->statements.add(statement);
}
// void defFuncHandlerFunccall(StackNode& sn, LR1* lr1)
// {
//	  Executor* executor = getExecutor(lr1);
// 	//executor->statements.add(sn[0].szName);
// }

void defFuncHandlerArgs(StackNode& sn, LR1* lr1)
{
	sn[0].szName = sn[0].szName + " , " + sn[2].szName;
}

void defFuncHandlerCallFunc0(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	String tmpVar = getTmpVar(); 
	String statement = tmpVar + " = " + sn[0].szName + " ( )";
	executor->statements.add(statement);
	sn[0].szName = tmpVar;
}
void defFuncHandlerCallFuncn(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	String tmpVar = getTmpVar(); 
	String statement = tmpVar + " = " + sn[0].szName + " ( "+sn[2].szName+" )";
	executor->statements.add(statement);
	sn[0].szName = tmpVar;
}

void defFuncHandlerVarDot(StackNode& sn, LR1* lr1)
{
	sn[0].szName = sn[0].szName + " . " + sn[2].szName;
}

void defFuncHandlerOrOr(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	int oldPsi = ((int*)(sn[1].ExtraInfo))[0];
	String res = executor->statements[oldPsi];
	executor->statements.add(res + " = " + sn[2].szName);
	int psi = executor->statements.size();
	executor->statements[oldPsi] += " = " + sn[0].szName;
	executor->statements[oldPsi + 1] += psi;
	sn[0].szName = res;
	delete sn[1].ExtraInfo;
}

void defFuncHandlerAndAnd(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	int oldPsi = ((int*)(sn[1].ExtraInfo))[0];
	String res = executor->statements[oldPsi];
	executor->statements.add(res + " = " + sn[2].szName);
	int psi = executor->statements.size();
	executor->statements[oldPsi] += " = " + sn[0].szName;
	executor->statements[oldPsi + 2] += psi;
	sn[0].szName = res;
	delete sn[1].ExtraInfo;
}

void defFuncHandlerBreak(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	CStack<Circle_>* cc = (CStack<Circle_>*) lr1->getParam("circleStack");
	executor->statements.add(String(""));
	cc->top()._endCircle.add(executor->statements.size() - 1);
}

void defFuncHandlerContinue(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	CStack<Circle_>* cc = (CStack<Circle_>*) lr1->getParam("circleStack");
	executor->statements.add(String(""));
	cc->top()._chargeEntry.add(executor->statements.size() - 1);
}

void defFuncHandlerMid(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	String tmpVar = getTmpVar(); 
	String statement = sn[0].szName + " . operator[] ( "+sn[2].szName+" )";
	executor->statements.add(statement);
	sn[0].szName = tmpVar;
}
void defFuncHandlerMid3(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	String tmpVar = getTmpVar(); 
	String statement = sn[1].szName + " . operator[] ( "+sn[4].szName+" )";
	executor->statements.add(statement);
	sn[0].szName = tmpVar;
}
void defFuncHandlerCond(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	int psi = executor->statements.size();
	executor->statements.add(String("if ") + sn[0].szName + " goto " + (psi + 2));
	executor->statements.add("goto ");
	sn[0].ExtraInfo = new int(psi+1);
}
void defFuncHandlerIf(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	int psi = *((int*)(sn[2].ExtraInfo));
	executor->statements.get(psi) += executor->statements.size();
	delete sn[2].ExtraInfo;
}
void defFuncHandlerIfElse(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	int psi0 = *((int*)(sn[2].ExtraInfo));
	int psi1 = *((int*)(sn[5].ExtraInfo));
	executor->statements.get(psi0) += psi1 + 1;
	executor->statements.get(psi1) += executor->statements.size();
	delete sn[2].ExtraInfo;
	delete sn[5].ExtraInfo;
}
void fixCircle(Circle_ &ct, Executor &exe)
{
	int i = 0;
	for(i = 0; i < ct._chargeEntry.size(); i++)
	{
		int j = ct._chargeEntry.get(i);
		exe.statements[j] = String("goto ") + ct.conditionStart;
	}
	for(i = 0; i < ct._circleEntey.size(); i++)
	{
		int j = ct._circleEntey.get(i);
		exe.statements[j] = String("goto ") + ct.circleStart;
	}
	for(i = 0; i < ct._endCircle.size(); i++)
	{
		int j = ct._endCircle.get(i);
		exe.statements[j] = String("goto ") + ct.circleEnd;
	}
}
void defFuncHandlerWhile(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	CStack<Circle_>* cc = (CStack<Circle_>*) lr1->getParam("circleStack");
	int ind = *((int*)(sn[2].ExtraInfo));
	executor->statements.add(String("goto ") + cc->top().conditionStart);
	cc->top().circleEnd = executor->statements.size();
	cc->top().circleStart = ind + 1;
	fixCircle(cc->top(), *executor);
	executor->statements[ind] = String("goto ") + executor->statements.size();

	delete sn[2].ExtraInfo;
	cc->pop();
}

void defFuncHandlerDoWhile(StackNode& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	CStack<Circle_>* cc = (CStack<Circle_>*) lr1->getParam("circleStack");
	int ind = *((int*)(sn[4].ExtraInfo));
	executor->statements.del(ind);
	ind --;
	cc->top().circleEnd = executor->statements.size();
	fixCircle(cc->top(), *executor);
	String strTmp = executor->statements[ind];
	int i = strTmp.size() - 1;
	while(strTmp.at(i) >= '0' && strTmp.at(i) <= '9')i--;
	executor->statements[ind] = String(strTmp.substr(0, i+1)) + cc->top().circleStart;

	delete sn[3].ExtraInfo;
	cc->pop();
	
}
void onHandleStringInput(StackNode& sn, LR1* lr1)
{
	sn[0].szName = "\"" + sn[0].szName + "\"";
}

/************************************************************************/
/* 接下来 i = b.toString()                                                                     */
/************************************************************************/
/************************************************************************/
/* 接下来GC的优化                                                                     */
/************************************************************************/
/************************************************************************/
/* 正在做循环语句处理   
/* 处理if else 先
/************************************************************************/
void onMeetIf(StackNode_& sn, LR1* lr1)
{
}
void onMeetElse(StackNode_& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	sn.ExtraInfo = new int(executor->statements.size());
	executor->statements.add("goto ");
}
void onMeetWhile(StackNode_& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	AnalysisStack *as = (AnalysisStack*)lr1->getParam(LR1_REC_STACK);
	if(as == NULL)
	{
		throw "获取分析栈失败!";
	}
	CArray<AnalysisStack_> asa = as->toArray();
	if(asa.size() >= 3 && asa.get(asa.size() - 2).sn.szName == "do")	//doWhile
	{
		CStack<Circle_>* cc = (CStack<Circle_>*) lr1->getParam("circleStack");
		Circle_ &ct = cc->top();
		ct.conditionStart = executor->statements.size();
	}
	else		//while
	{
		CStack<Circle_>* cc = (CStack<Circle_>*) lr1->getParam("circleStack");
		Circle_ ct;
		ct.conditionStart = executor->statements.size();
		cc->push(ct);
	}
}

void onMeetDo(StackNode_& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	CStack<Circle_>* cc = (CStack<Circle_>*) lr1->getParam("circleStack");
	Circle_ ct;
	ct.circleStart = executor->statements.size();
	cc->push(ct);
}

void onMeetAndAnd(StackNode_& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	String varRes = getTmpVar();
	sn.ExtraInfo = new int;
	int psi = executor->statements.size();
	((int*)(sn.ExtraInfo))[0] = psi;
	executor->statements.add(varRes);
	executor->statements.add(String("if ") + varRes + " goto " + (psi+3));
	executor->statements.add(String("goto "));
}

void onMeetOrOr(StackNode_& sn, LR1* lr1)
{
	Executor* executor = getExecutor(lr1);
	String varRes = getTmpVar();
	sn.ExtraInfo = new int;
	int psi = executor->statements.size();
	((int*)(sn.ExtraInfo))[0] = psi;
	executor->statements.add(varRes);
	executor->statements.add(String("if ") + varRes + " goto ");
}

int main()
{
	bool isReadFromFile = true;
	bool isOutOnly = false;
	LR1 lr1;
	lr1.setParam("circleStack",new CStack <Circle_>());
	if(isReadFromFile){
		lr1.initStateFromFile("D:\\state.txt");
	}else{
		lr1.InitRule("D:\\wenfa.txt",true);
		lr1.outputStateToFile("D:\\state.txt");
		if(isOutOnly)
		{
			getchar();
			return 0;
		}
	}

	//18 "StatementWithBreakContinue"->"Break"";" 0 
	//19 "StatementWithBreakContinue"->"Continue"";" 0 
	lr1.registerHandler(18,defFuncHandlerBreak);
	lr1.registerHandler(19,defFuncHandlerContinue);

	//35 "V"->"Variable""lvbase""V" 0 
	lr1.registerHandler(35,defFuncHandler2);
	//36 "V"->"Variable""=""V" 0 
	lr1.registerHandler(36,defFuncHandlerSet);

	//39 "Value"->"Value""lv0""A0" 0 
	lr1.registerHandler(39, defFuncHandlerOrOr);
	//41 "A0"->"A0""lv1""A1" 0 
	lr1.registerHandler(41, defFuncHandlerAndAnd);

	//45 "A2"->"A2""lv3""A3" 0 
	for(int i = 0; i < 7; i++){
//		String strtp = String::format("\"A%d\"->\"A%d\"\"lv%d\"\"A%d\"",i,i,i+1,i+1);
//		lr1.registerHandler(strtp.c_str(),defFuncHandler2);
		lr1.registerHandler(43 + 2*i,defFuncHandler2);
	}
	
	

//	lr1.registerHandler("\"FuZhi\"->\"RealValue\"\";\"",defFuncHandlerFunccall);

	//57 "A8"->"A8""+""A9" 0 
	lr1.registerHandler(57,defFuncHandler2);
	//58 "A8"->"A8""-""A9" 0 
	lr1.registerHandler(58,defFuncHandler2);
	//60 "A9"->"A9""lv10""A10" 0 
	lr1.registerHandler(60,defFuncHandler2);
	//62 "A10"->"lv11""A11" 0 
	lr1.registerHandler(62,defFuncHandler1);
	//63 "A10"->"+""A11" 0 
	//64 "A10"->"-""A11" 0 
	//65 "A10"->"++""Variable" 0 
	//66 "A10"->"--""Variable" 0 
	lr1.registerHandler(63,defFuncHandlerPrev);
	lr1.registerHandler(64,defFuncHandlerPrev);
	lr1.registerHandler(65,defFuncHandlerPrev);
	lr1.registerHandler(66,defFuncHandlerPrev);
	//68 "A11"->"Variable""++" 0 
	//69 "A11"->"Variable""--" 0 
	lr1.registerHandler(68,defFuncHandlerAfter);
	lr1.registerHandler(69,defFuncHandlerAfter);
	//73 "RealValue"->"(""V"")" 0 
	lr1.registerHandler(73,defFuncHandlerBrackets);
	
	//75 "Variable"->"RealValue"".""variable" 0 
	lr1.registerHandler(75,defFuncHandlerVarDot);
	//76 "RealValue"->"Funccall""[""V""]" 0 
	lr1.registerHandler(76,defFuncHandlerMid);
	//77 "RealValue"->"Variable""[""V""]" 0 
	lr1.registerHandler(77,defFuncHandlerMid);
	//78 "RealValue"->"(""V"")""[""V""]" 0										
	lr1.registerHandler(78,defFuncHandlerMid3);
	//79 "Funccall"->"Funchead""(""Args"")" 0 
	lr1.registerHandler(79,defFuncHandlerCallFuncn);
	//80 "Funccall"->"Funchead""("")" 0 
	lr1.registerHandler(80,defFuncHandlerCallFunc0);
	//83 "Funchead"->"(""V"")" 0
	lr1.registerHandler(83,defFuncHandlerBrackets);
	//85 "Args"->"Args"",""V" 0
	lr1.registerHandler(85,defFuncHandlerArgs);

	//5 "ConstValue"->"stringValue" 0 
	lr1.registerHandler(5,onHandleStringInput);
	
	
	lr1.registerOnMeetHandler("if", onMeetIf);
	lr1.registerOnMeetHandler("else", onMeetElse);
	lr1.registerOnMeetHandler("while", onMeetWhile);
	lr1.registerOnMeetHandler("do", onMeetDo);
	//"lv0":"||"
	//"lv1":"&&"
	lr1.registerOnMeetHandler("lv1", onMeetAndAnd);
	lr1.registerOnMeetHandler("lv0", onMeetOrOr);
	//27 "XunHuanWithBreakContinue"->"if""(""Condition"")""BlockWithBreakConyinue" 0 
	//31 "XunHuan"->"if""(""Condition"")""Block" 0 
	lr1.registerHandler(27,defFuncHandlerIf);
	lr1.registerHandler(31,defFuncHandlerIf);
	//28 "XunHuanWithBreakContinue"->"if""(""Condition"")""BlockWithBreakConyinue""else""Block" 0 
	//32 "XunHuan"->"if""(""Condition"")""Block""else""Block" 0 
	lr1.registerHandler(28,defFuncHandlerIfElse);
	lr1.registerHandler(32,defFuncHandlerIfElse);
	//33 "Condition"->"V" 0 
	lr1.registerHandler(33,defFuncHandlerCond);
	//25 "XunHuanWithBreakContinue"->"do""BlockWithBreakConyinue""while""(""Condition"")" 0 
	//29 "XunHuan"->"do""BlockWithBreakConyinue""while""(""Condition"")" 0 
	lr1.registerHandler(25,defFuncHandlerDoWhile);
	lr1.registerHandler(29,defFuncHandlerDoWhile);
	//26 "XunHuanWithBreakContinue"->"while""(""Condition"")""BlockWithBreakConyinue" 0
	//30 "XunHuan"->"while""(""Condition"")""BlockWithBreakConyinue" 0  
	lr1.registerHandler(26,defFuncHandlerWhile);
	lr1.registerHandler(30,defFuncHandlerWhile);

	char inputBuff[4096];
	Executor executor;
	while (true) {
		setExecutor(&lr1, &executor);

		gets_s(inputBuff, 4096);
		String inputStr = String(inputBuff) + ";";

		// 	lr1.printLR1State();
		// 	lr1.printLR1Table();
		lr1.Recognize(inputStr.c_str(), false);
		try {
			executor.execute();
			Object* resObj = executor.getFinalVar();
			if (resObj != NULL) {
				printf("%s\n", resObj->getStringValue().c_str());
				executor.setFinalVar(NULL);
			}
		}
		catch (char* s)
		{
			puts(s);
		}
		catch (const char* s)
		{
			puts(s);
		}
		catch (String str)
		{
			puts(str.c_str());
		}
		catch (...)
		{
			puts("unknown Error!");
		}
		executor.clearTmpVar(tmpVarPre);
		executor.dumpVar();
		executor.clearStatements();
	}
	theGC.gc();
	setExecutor(&lr1, NULL);
	return 0;
}