#include<stdio.h>
#include<stdlib.h>
#include<String.h>
#include<math.h>
#include"MyString.h"
#include"BufferIn.h"
#include"CStruct.h"

#ifndef __LR1_H__
#define __LR1_H__

#define DIGINTDEX "[0-9]*[0-9]"
#define DIGINTHEX "0[xX][[a-f][A-F][0-9]]*[[a-f][A-F][0-9]]"
#define DIGFLOAT "*[0-9].[0-9]*[0-9]*00-01{[eE]*00-01[+-][0-9]*[0-9]}"
#define BOOLTRUE	"true$[_[0-9][a-z][A-Z]]"
#define BOOLFALSE	"false$[_[0-9][a-z][A-Z]]"
//#define KEYWORDAFT	"$[_[0-9][a-z][A-Z]]"
#define NULLVALUE	"null$[_[0-9][a-z][A-Z]]"

#define LR1_REC_STACK "rStack"

class LR1;

typedef struct StackNode_//储存分析节点的信息,abcd 每一个位置对应一个
{
	StackNode_()
	{
		type = -1;
		ExtraInfo = NULL;
		i64Val = 0;
	}
	StackNode_& operator=(StackNode_ sn)
	{
		this->type = sn.type;
		this->szName = sn.szName;
		this->i64Val = sn.i64Val;
		this->ExtraInfo = sn.ExtraInfo;
		return *this;
	}
	int type;
	String szName;
	union
	{
		char cVal;
		short sVal;
		int iVal;
		__int64 i64Val;
		double dfVal;
	};
	void * ExtraInfo;
}STACKNODE,*PSTACKNODE;
typedef CArray<StackNode_>  StackNode;

typedef void (*REDUCE_HANDLER)(StackNode& sn, LR1* lr1);
typedef void (*ONMEET_HANDLER)(StackNode_& sn, LR1* lr1);

void defaultHandler(StackNode& sn, LR1* lr1);
void defaultOnMeetHandler(StackNode_& sn, LR1* lr1);

typedef CSet<String> RSTable,*PRSTable;
typedef struct LR1Table_
{
	LR1Table_()
	{
		iCode = -1;
	}
	LR1Table_(int iCode , String strChange)
	{
		this->iCode = iCode;
		this->szChange = strChange;
	}
	LR1Table_& operator=(LR1Table_ lr1t)
	{
		this->iCode = lr1t.iCode;
		this->szChange = lr1t.szChange;
		return *this;
	}
	bool operator==(LR1Table_& lr1t)
	{
		if(iCode > 0 && lr1t.iCode == iCode)
		{
			return true;
		}
		return false;
	}
	int iCode;
	String szChange;		//储存内容,如S4,r1等
}LR1TABLE_,*PLR1TABLE_;

typedef CSet<LR1Table_> LR1Table,*PLR1Table;

typedef struct VTable_	//文法(非)终结符链
{
	VTable_()
	{
		iCode = -1;
		onMeetHandler = defaultOnMeetHandler;
	}
	VTable_(int iCode , String ch )
	{
		this->iCode = iCode;
		this->ch = ch;
		onMeetHandler = defaultOnMeetHandler;
	}
	int iCode;		//记录终结符所对应的编号,像main对应1等等
	String ch;		//记录(非)终结符的string
	CSet<String> realString;	//记录真实的string识别类型.
	bool operator==(VTable_ &vt)
	{
		if(ch == vt.ch && ch.size() > 0)
		{
			return true;
		}
		return false;
	}
	VTable_& operator=(VTable_ vt)
	{
		iCode = vt.iCode;
		ch = vt.ch;
		rshead = vt.rshead;
		rsfhead = vt.rsfhead;
		lr1head = vt.lr1head;
		return *this;
	}
	RSTable rshead;	//记录非终结符的Follow集
	RSTable rsfhead;//记录所有非终结符的First集
	LR1Table lr1head;//记录非终结符的Action或非终结符的Goto表
	ONMEET_HANDLER onMeetHandler;
	//	PFONADDNODE pOan;
}VTABLE_,*PVTABLE_;

typedef CSet<VTable_> VTable,*PVTable;

typedef struct SingleString_		//文法链
{
	SingleString_()
	{
		//		pfOnDealRule = DefaultOnDealRule;
		iNum = 0;
		handler = defaultHandler;
	}
	SingleString_(int iNum , String szWenFa)
	{
		this->iNum = iNum ;
		this->szWenFa = szWenFa;
	}
	bool operator==(SingleString_ &vt)
	{
		if(this->iNum == vt.iNum && szWenFa == szWenFa && rssHead == vt.rssHead)
		{
			return true;
		}
		return false;
	}
	SingleString_& operator=(SingleString_ vt)
	{
		iNum = vt.iNum;
		szWenFa = vt.szWenFa;
		rssHead = vt.rssHead.clone();
		return *this;
	}
	int iNum;		//文法单位所属的序号
	String szWenFa;
	RSTable rssHead;	//展望符列表,LR(1)分析法用
	REDUCE_HANDLER handler;
	//	PFONDEALRULE pfOnDealRule;
}SISTR_,*PSISTR_;

typedef CSet<SingleString_> SIStr,*PSIStr;

typedef struct NextState_	//状态转换链,记录某一个状态到其他状态的转换
{
	NextState_()
	{
		iNum = 0;
	}
	NextState_(int iNum , String StepBy)
	{
		this->iNum = iNum;
		this->StepBy = StepBy;
	}
	bool operator==(NextState_ &vt)
	{
		if(vt.iNum == iNum && StepBy == StepBy)
		{
			return true;
		}
		return false;
	}
	NextState_& operator=(NextState_ vt)
	{
		iNum = vt.iNum;
		StepBy = vt.StepBy;
		return *this;
	}
	int iNum;
	String StepBy;
}NEXTSTATE_,*PNEXTSTATE_;

typedef CSet<NextState_> NextState,*PNextState;

typedef struct CurState_		//分析状态链,记录所有状态
{
	CurState_()
	{
		iCode = 0;
	}
	bool operator==(CurState_ &vt)
	{
		if(iCode == vt.iCode && sihead == vt.sihead )
		{
			return true;
		}
		return false;
	}
	CurState_& operator=(CurState_ vt)
	{
		iCode = vt.iCode;
		sihead = vt.sihead;
		nehead = vt.nehead;
		return *this;
	}
	int hashCode;
	int iCode;			//当前状态号
	SIStr sihead;		//当前状态的拓展文法
	NextState nehead;	//当前状态到其他状态的转换
}CURSTATE_,*PCURSTATE_;

typedef CSet<CurState_> CurState,*PCurState;


typedef struct AnalysisStack_
{
	AnalysisStack_()
	{
		iState = 0;
	}
	AnalysisStack_(String strSymbol , int iState)
	{
		this->iState = iState;
		this->strSymbol = strSymbol;
	}
	AnalysisStack_& operator=(AnalysisStack_ as)
	{
		this->iState = as.iState;
		this->strSymbol = as.strSymbol;
		this->sn = as.sn;
		return *this;
	}
	int iState;			//栈中状态
	String strSymbol;		//栈中符号
public:
	StackNode_ sn;		//储存相关信息
}ANALYSISSTACK_ , *PANALYSISSTACK_;

typedef CStack<AnalysisStack_> AnalysisStack;

typedef struct TokenList_
{
	String strToken;
	int iCode;
	TokenList_()
	{
		iCode = -1;
	}
	TokenList_(String strToken , int iCode)
	{
		this->strToken = strToken;
		this->iCode = iCode;
	}
	TokenList_ operator=(TokenList_& tl)
	{
		this->strToken = tl.strToken;
		this->iCode = tl.iCode;
	}
	bool operator ==(TokenList_ &tl)
	{
		return this->strToken == tl.strToken;
	}
}TOKENLIST,*PTOKENLIST;

typedef CArray<TOKENLIST> TokenList;

class LR1
{
public:
	LR1();
	~LR1();
	int GetLastError();
	int InitRule(const char *szInput,bool isFilePath);
	int Recognize(const char *szInput,bool isFilePath);
	void printLR1Table();
	void printLR1State();
	void outputStateToFile(const char* outFile);
	bool initStateFromFile(const char* outFile);
	bool registerHandler(String strWenFa, REDUCE_HANDLER handler);
	bool registerHandler(int iWenFaIndex, REDUCE_HANDLER handler);
	bool registerOnMeetHandler(String chZhongJieFu, ONMEET_HANDLER handler);

	void* getParam(String key);
	bool setParam(String key, void* value);
	void clearParam();
	void* resetParam(String key, bool isDelete = true);
private:
	CMap<String,void*> params;
	int lastError;
	int curType;
	String szToken;
	int ScanerIndex;

	bool isRightRule;
	BufferIn szToRec;
	BufferIn szRule;

	VTable vt , nt;		//nt表示非终结符,vt表示终结符
	SIStr sihead;
	String chEntry,preEntry,strDollar;

	CurState I0;
	LR1Table DollarChain;

	int InitLR1Table();
	int DealInputRule();
	void AdjustZhongjiefuRecString();		//调整终结符的识别串,如果是单个终结符(小写字母)并且没有设置识别串,
	//则自动填充为单个字母识别串(兼容1.0版本)
	//可考虑只要是没有设置的均将其设置为终结符串本身(2.0)
	void InitFollowSelect();
	void FillState();	
	bool GetNextKeyString(String& str , BufferIn& bi , int &index);
	bool GetNextKeyString(String& str ,  String& strbi , int &index);
	void Skip(BufferIn& szBuf , int &index);

	void makeStateHashCode(CurState_& state);

	bool isUpLetter(char ch);	//是否大写
	bool isDnLetter(char ch);	//是否小写
	bool isLetter(char ch);		//是否字符
	bool isNum(char ch);
	bool isHex(char ch);		//是否16进制可识别字符
	char DigToHex(int i);		
	int HexToDig(char ch);		//Hex char to int
	bool AddToRsTable(String ch);	//将ch添加到(非)终结符链中
	void DecodeString(String& str);
	void EncodeString(String& str);
	void MakePrevEntry();

	void FillFirstSelect();
	void FillFollowSelect();

	void MakeDollarString();
	RSTable GetFirst(String ch);
	RSTable GetFollow(String ch);

	void FillLR1Table();		//初始化LR(1)分析表
	void AddToLR1Table(LR1Table &lr1head, String szGoto, int iCode);
	void AddToLR1Table(String ch , String szGoto, int iCode);
	int GetAtPos(String strToFind);
	int GetNextStateNum(String ch , NextState& ns);
	int GetWenFaNum(String szWenFa);	//获取给定文法的ID
	bool isStringEqual(String si,String di);
	bool AddWenFa(SIStr& di , String szWenFa , RSTable rst);
	bool isWenFaExist(SIStr& si , String szWenfa , RSTable rst, int isNeedEqual = 0);	//判断文法是否存在si链中,第三个参数在LR(1)分析法中使用

	void CompleteState(CurState_& si);
	bool AddSWenFa(SIStr& si , String ch , int index , RSTable rst);
	void MakeAtWenFa(String& siWenFa , String& diWenFa);
	bool MakeNextState(CurState_& si , CurState_& di , String Next);
	bool isStateExist(CurState_& state);
	bool isWenFaEqual(SIStr& si , SIStr& di);

	int InitInput(const char *szInput,bool isFilePath);			
	void scaner();

	bool CheckZhongjiefuWithRealString(String strZhongjiefu, BufferIn &bf, int &index, String& strOut);
	bool isSingleMatch(String strZhongjiefu, int &zIndex, BufferIn &bf, int bIndex);
	void ComeToEndMatch(String strZhongjiefu, int &zIndex);

	String GetAction(int State , int iCode);	//从action表获取转换状态,State表示行,iCode表示终结符对应的ID
	int GetGoto(int State , String& ch);	//获取转换表对应的转换状态,State表示行,ch表示非终结符
	String GetZhongJieFu(int iCode);
	String GetWenFaStr(int ID);		//获取指定ID的文法

	void InstallDefaultAttrs();
	void DisposeDefaultValues();
};

#endif