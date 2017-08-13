#include "lr1.h"
#include "StateFileHelper.h"
#include <Windows.h>

#define MSG_DBG false

void defaultHandler(StackNode& sn, LR1* lr1){
}

void defaultOnMeetHandler(StackNode_& sn, LR1* lr1){

}

bool LR1::isUpLetter(char ch)	//是否大写
{
	return ch >= 'A' && ch <= 'Z';
}
bool LR1::isDnLetter(char ch)	//是否小写
{
	return ch >= 'a' && ch <= 'z';
}
bool LR1::isLetter(char ch)		//是否字符
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}
bool LR1::isNum(char ch)
{
	return (ch >= '0' && ch <= '9');
}
bool LR1::isHex(char ch)
{
	return isLetter(ch) || isNum(ch);
}
int LR1::HexToDig(char ch)
{
	if(!isHex(ch))
	{
		return -1;
	}
	if(isNum(ch))
	{
		return ch - '0';
	}
	if(isUpLetter(ch))
	{
		return ch - 'A' + 10;
	}
	return ch - 'a' + 10;
}

char LR1::DigToHex(int i)
{
	const char buf[] = "0123456789ABCDEF";
	return buf[i & 0x0000000f];
}

LR1::LR1()
{
	lastError = 0;
	isRightRule = false;
	return ;
}
LR1::~LR1()
{
	clearParam();
	return ;
}

void* LR1::getParam(String key){
	if(params.isDefined(key)){
		return params[key];
	}
	return NULL;
}
bool LR1::setParam(String key, void* value){
	if (value == NULL) {
		if (params.isDefined(key)) {
			params.erase(key);
		}
		return true;
	}
	return params.setWithCheck(key, value);
}
void LR1::clearParam(){
	CArray<String> keys = params.getKeys();
	for(int i = 0; i < keys.size(); i++){
		delete params.get(keys[i]);
		params.erase(keys[i]);
	}
}
void* LR1::resetParam(String key, bool isDelete){
	if(!params.isDefined(key)){
		throw "params" + key + "not defined yet!";
		return NULL;
 	}
	void* value = params.get(key);
	params.erase(key);
	if(isDelete){
		delete value;
		return NULL;
	}
	return value;
}

int LR1::GetLastError()
{
	return lastError;
}
int LR1::InitRule(const char *szInput,bool isFilePath)
{
	szRule.Init(szInput , isFilePath);
	if((lastError = szRule.isBad()) != 0)
	{
		szRule.~BufferIn();
		return lastError;
	}
	lastError = InitLR1Table();
	szRule.~BufferIn();
	return lastError;
}
int LR1::InitInput(const char *szInput,bool isFilePath)
{
	szToRec.Init(szInput , isFilePath);
	lastError = szToRec.isBad();
	return lastError;
}
int LR1::InitLR1Table()
{
	if((lastError = DealInputRule()) != 0)
	{
		return lastError;
	}
	int tm1 = GetTickCount(), tm2 = 0, i = 0;
	InitFollowSelect();
	if (MSG_DBG) {
		tm2 = GetTickCount(); printf("Cost%d:%d\n", i, tm2 - tm1); i++; tm1 = tm2;
	}
	FillState();
	if (MSG_DBG) {
		tm2 = GetTickCount(); printf("Cost%d:%d\n", i, tm2 - tm1); i++; tm1 = tm2;
	}
	FillLR1Table();
	if (MSG_DBG) {
		tm2 = GetTickCount(); printf("Cost%d:%d\n", i, tm2 - tm1); i++; tm1 = tm2;
	}
	if(lastError == 0)
	{
		isRightRule = true;
	}
	return lastError;
}
int LR1::DealInputRule()
{
	int i = 0;
	while(i < szRule.GetFileSize())
	{
		Skip(szRule,i);
		if(szRule[i] == ';' || szRule[i] == '#' || !szRule[i])		//;表示注释,这里跳过注释,空行在此一并跳过
		{
			while(szRule[i] && szRule[i] != '\n')
			{
				i++;
			}
			i++;	//只有遇到\n才会退出,遇到首字符为;,则跳过一行,这里转到下一行
			continue;
		}
		String strKey;
		if(GetNextKeyString(strKey , szRule , i))
		{
			int j = i;
			if(strKey.length() > 0 && isUpLetter(strKey.at(0)) 
				&& szRule[i] == '-' && szRule[i+1] == '>')
			{
				SingleString_ st;
				AddToRsTable(strKey);
				i += 2;
				EncodeString(strKey);
				st.szWenFa = "\"" + strKey + "\"";
				st.szWenFa += "->";
				while(GetNextKeyString(strKey , szRule , i))
				{
					AddToRsTable(strKey);
					EncodeString(strKey);
					st.szWenFa += "\"" + strKey + "\"";
				}
				if(i == j + 2)	//没有变,即->后面没内容
				{
					return 1;//WenFaWuTuiDao;
				}
				if(sihead.size() == 0)
				{
					st.iNum = 1;
				}
				else
				{
					st.iNum = sihead.get(sihead.size() - 1).iNum + 1;
				}
				sihead.add(st);
			}
// 			else if(szRule[i] == '=' && isNum(szRule[i + 1]))	//对非终结符进行赋值,赋值必须在相应终结符出现之后
// 			{
// 				VTable_ vtmp(-1,strKey);
// 				int k = vt.find(vtmp);
// 				if(k == -1)
// 				{
// 					return 2;//WeiZhiZhongJieFu;		//终结符未找到!
// 				}
// 
// 				int j = i + 1;
// 				String szTmp;
// 				while(isNum(szRule[j]))
// 				{
// 					szTmp += szRule[j];
// 					j++;
// 				}
// 				sscanf(szTmp.c_str(),"%d",&vtmp.iCode);
// 				vt.set(k , vtmp);
// 
// 				i = j;
// 				while(szRule[i] && szRule[i] != '\n' && szRule[i] != ' ' && szRule[i] != '\t')
// 				{
// 					i++;
// 				}
// 			}		//由于更新了需求,这里不再需要数字编号来唯一标识,取而代之的是"[a-b]cc#[^a]":"m"的形式,具体见需求
			else if(szRule[i] == ':')			//待添加,处理新情况
			{
				i++;
				int iVtIndex = vt.find(VTable_(-1,strKey));
				if(iVtIndex < 0)
				{
					return 7;	//未找到指定的终结符
				}
				String strRealStr;
				while(szRule[i] && (szRule[i] == '\n' && szRule[i] == ' ' && szRule[i] == '\t'))
				{
					i++;
				}
				if(szRule[i] == '\0')
				{
					return 5;		//异常的文件结尾
				}
				if(szRule[i] != '\"')
				{
					return 6;		//终结符描述不正确
				}
				int k = i + 1;
				i ++;
				while(szRule[i])
				{
					if(szRule[i] == '\\')
					{
						if(szRule[i + 1] == '\"' )
						{
							i += 2;
						}
					}
					else if(szRule[i] == '\"')
					{
						i++;
						vt.get(iVtIndex).realString.add(strRealStr);
						break;
					}
					strRealStr += szRule[i];
					i++;
				}
			}
			else 
			{
				return 3;	//FeiFaGuiZeGeShi;
			}
		}
		else
		{
			return 4;//FeiFaGuiZeGeShi;
		}
	}
	AdjustZhongjiefuRecString();
	if(vt.size() == 0 || nt.size() == 0)
	{
		return 5;
	}
// 	for(i = 0 ; i < vt.size() ; i++)
// 	{
// 		if(vt.get(i).iCode < 0 && vt.get(i).ch.length() == 1)
// 		{
// 			vt.get(i).iCode = vt.get(i).ch[0];
// 		}
// 	}
	if(chEntry == "")
	{
		chEntry = nt.get(0).ch;
	}
	if(nt.find(VTable_(-1,chEntry)) == -1)
	{
		return 6;//WenFaJinRuFuHaoWuXiao;
	}
	MakePrevEntry();
	// 	for(preEntry = 'Z' ; preEntry >= 'A' ; preEntry --)
	// 	{
	// 		if(!isFeiZhongJieFu(preEntry))
	// 		{
	// 			break;
	// 		}
	// 	}
	SingleString_ st(0 ,"");
	DecodeString(chEntry);

	st.szWenFa = "\"" + preEntry  + "\"->\"" + chEntry + "\"";    //注意!preEntry不会产生"和\,因此不用Decode
	sihead.insert(0,st);
	return 0;
}
bool LR1::GetNextKeyString(String& str , BufferIn& bi , int &index)
{
	str = "";
	if(index >= bi.GetFileSize() || bi[index] == ' ' || bi[index] == '\n' || bi[index] == '\r' || bi[index] == '\t')
	{
		return false;
	}
	if(bi[index] == '\"')	//定义从"开始
	{
		index ++;
		if(bi[index] == '\"')
		{
			return false;
		}
		while(index < bi.GetFileSize())
		{
			if(bi[index] == '\"')
			{
				index ++;
				return true;
			}
			else if(bi[index] == '\\')	//可能会在此转义,支持\\转\,\"转\,\其他直接是\ 
			{
				if(bi[index + 1] == '\"')
				{
					str += '\"';
					index += 2;
				}
				else if(bi[index + 1] == '\\')
				{
					str += '\\';
					index += 2;
				}
				else
				{
					str += '\\';
					index ++;
				}
			}
			else
			{
				str += bi[index];
				index ++;
			}
		}
	}
	else	//兼容上一版本,支持单字符,此时不需要引号
	{
		if(bi[index] == '\\')	//可能会在此转义,支持\\转\,\"转\,\其他直接是\ 
		{
			if(bi[index + 1] == '\"')
			{
				str += "\"";
				index += 2;
			}
			else if(bi[index + 1] == '\\')
			{
				str += "\\";
				index += 2;
			}
			else
			{
				str += "\\";
				index ++;
			}
			return true;
		}
		else
		{
			str += bi[index];
			index ++;
			return true;
		}
	}
	return false;
}
void LR1::Skip(BufferIn& szBuf , int &index)		//跳过空格,退格,换行
{
	while(szBuf[index] == ' ' || szBuf[index] == '\n' || szBuf[index] == '\r' || szBuf[index] == '\t')index++;
}
bool LR1::AddToRsTable(String ch)	//将ch添加到全局(非)终结符链中
{
	VTable *pv;
	if(isUpLetter(ch[0]))
	{
		pv = &nt;
	}
	else
	{
		pv = &vt;
	}
	int i = 0;
	for(i  = 0 ; i < pv->size(); i++)
	{
		if(pv->get(i).ch == ch)
		{
			return false;
		}
	}
	int iSize = -1;
	if(pv == &vt)
	{
		iSize = pv->size() + 1;
	}
	pv->add(VTable_(iSize, ch));
	return true;
}
void LR1::AdjustZhongjiefuRecString()
{
	for(int i = 0; i < vt.size(); i++)
	{
		if(vt.get(i).realString.size() == 0)
		{
			if(vt.get(i).ch.length() == 1)		//兼容到1.0,这里因为要防止出现非法字符(比如*\号等),需要处理成"\?"的形式
			{
				vt.get(i).realString.add("\\" + vt.get(i).ch);
			}
			else
			{
				String strTmp = vt.get(i).ch;
				EncodeString(strTmp);
				vt.get(i).realString.add(strTmp);
				
			}
			
		}
	}
}
void  LR1::DecodeString(String& str)
{
	str.replace("\\\\","\\");
	str.replace("\\\"","\"");
	return ;
}
void LR1::EncodeString(String& str)
{
	str.replace("\\" , "\\\\");		//顺序不能颠倒,否则对于"来说,先替换"会变成\",然后再变成\\",明显错误
	str.replace("\"" , "\\\"");
	return ;
}
void LR1::MakePrevEntry()
{
	char ch = 'Z';
	preEntry = "";
	String strTmp ;
	while(1)
	{
		strTmp = preEntry + ch;
		if(nt.find(VTable_(-1,strTmp)) == -1)
		{
			preEntry = String(strTmp);
			return ;
		}
		if(ch > 'A')
		{
			ch --;
		}
		else
		{
			preEntry += 'Z';
			ch = 'Z';
		}
	}
}
void LR1::InitFollowSelect()
{
	FillFirstSelect();
	FillFollowSelect();
	return;
}
void LR1::makeStateHashCode(CurState_& state)
{
	state.hashCode = 0;
	for(int i = 0 ; i < state.sihead.size(); i++)
	{
		state.hashCode += state.sihead.get(i).szWenFa.getHashCode();
		for(int j = 0 ; j < state.sihead.get(i).rssHead.size(); j++)
		{
			state.hashCode += state.sihead.get(i).rssHead.get(j).getHashCode();
		}
	}
}
void LR1::FillState()		//完成状态转换图DFA
{
	int iNum = 0;
	String szTmp = String(chEntry);
	EncodeString(szTmp);
	szTmp = "\"" + preEntry + "\"->@\"" + szTmp + "\"";
	RSTable rst;
	rst.add(strDollar);
	I0.add(CurState_());
	AddWenFa(I0.get(0).sihead , szTmp , rst);

	CompleteState(I0.get(0));
	makeStateHashCode(I0.get(0));
//	CURSTATE *curDeal = &I0;
	int i , j;
//	int tm1 = GetTickCount(), tm2;
	for(int i = 0 ; i < I0.size() ; i++)
	{
//		tm2 = GetTickCount();printf("stateCost%d:%d\thashCode:%d\n",i,tm2-tm1,I0.get(i).hashCode);tm1=tm2;
// 		pv = vt.next;
// 		pn = nt.next;
		for(j = 0 ; j < nt.size() ; j ++)
		{
			CurState_ curTmp;
			if(MakeNextState(I0.get(i) , curTmp , nt.get(j).ch))
			{
				makeStateHashCode(curTmp);
				if(!isStateExist(curTmp))
				{
					curTmp.iCode = I0.get(I0.size()-1).iCode + 1;
					I0.add(curTmp);
				}
				I0.get(i).nehead.add(NextState_(curTmp.iCode, nt.get(j).ch));
			}
		}
		for(j = 0 ; j < vt.size() ; j ++)
		{
			CurState_ curTmp;
			if(MakeNextState(I0.get(i) , curTmp , vt.get(j).ch))
			{
				makeStateHashCode(curTmp);
				if(!isStateExist(curTmp))
				{
					curTmp.iCode = I0.get(I0.size()-1).iCode + 1;
					I0.add(curTmp);
				}
				I0.get(i).nehead.add(NextState_(curTmp.iCode, vt.get(j).ch));
			}
		}
	}
	return ;
}
bool LR1::GetNextKeyString(String& str ,  String& strbi , int &index)
{
	BufferIn bi(strbi.c_str() , false);
	return GetNextKeyString(str , bi , index);
}
void LR1::FillFirstSelect()	//填充First集,注意,文法不允许出现e,即空串,这里没有空串的处理程序,
{
	int i , j;
	String strKey , strTmp;
	for(i = 0 ; i < nt.size(); i++)							//外层循环,遍历每一个非终结符,填充非终结符的First集,注意,
		//这里将非终结符和终结符一起处理(S->A,将A也加入列表),下面的算法将解决这个问题
	{
		for(j = 0 ; j < sihead.size(); j++)			//内层循环,遍历每个文法,(由于更改了算法,这里效率不高)
		{
			int k = 0;
			GetNextKeyString(strKey , sihead.get(j).szWenFa , k);		//由于写入时已经严格格式化,因此不会返回false
			if(strKey == nt.get(i).ch)
			{
				k += 2;
				GetNextKeyString(strTmp , sihead.get(j).szWenFa , k);
				if(!(nt.get(i).ch == strTmp))
				{
					nt.get(i).rsfhead.add(strTmp);
				}
			}
		}
	}
	//pv = nt.next;
	for(i = 0 ; i < nt.size() ; i++)				//遍历每一个非终结符,清除其中的终结符
	{
		//pst = pv->rsfhead.next;
		for(j = 0 ; j < nt.get(i).rsfhead.size(); j++)
		{
			if(isUpLetter(nt.get(i).rsfhead.get(j)[0]))		//非终结符与终结符依靠第一个字母判断
			{
				RSTable rst = GetFirst(nt.get(i).rsfhead.get(j));
				nt.get(i).rsfhead = nt.get(i).rsfhead.or(rst);
				nt.get(i).rsfhead.del(nt.get(i).ch);
			}
		}						
		//注意!以上的加入并未剔除非终结符,整个链条只能增加,没有减少
		//这是因为,如果立即剔除了非终结符,遇到A->B B->A 就会死循环
		for(j = 0 ; j < nt.get(i).rsfhead.size() ; j++)
		{
			if(isUpLetter(nt.get(i).rsfhead.get(j)[0]))
			{
				nt.get(i).rsfhead.del(j);
				j--;
			}
		}
	}
	return;
}
void LR1::FillFollowSelect()	//填充Follow集
{
//	VTABLE *pv = nt.next;
	int i , j;
	MakeDollarString();
	for(i = 0 ; i < nt.size() ; i++)
	{
		if(nt.get(i).ch == chEntry)
		{
			nt.get(i).rshead.add(strDollar);
		}
//		ps = sihead.next;
		for(j = 0 ; j < sihead.size(); j++)
		{
			String strFzjf , strTmp;	//strFzjf 记录本文法的第一个非终结符 S->A 则strFzjf = S
			int k = 0;
			GetNextKeyString(strFzjf , sihead.get(j).szWenFa , k);
			k += 2;		//跳过->
			while(GetNextKeyString(strTmp , sihead.get(j).szWenFa , k))
			{
				if(nt.get(i).ch == strTmp)
				{
					if(sihead.get(j).szWenFa[k] == '\0')		//已经读到最后一个字符
					{
						nt.get(i).rshead.add(strFzjf);
					}
					else 
					{
						int l = k;
						String strNewTmp ;
						GetNextKeyString(strNewTmp , sihead.get(j).szWenFa , l);
						if(isUpLetter(strNewTmp[0]))
						{
							nt.get(i).rshead = nt.get(i).rshead.or(GetFirst(strNewTmp));
						}
						else
						{
							nt.get(i).rshead.add(strNewTmp);
						}
					}
				}
			}
		}
	}							//以上为获取非终结符的Follow集,具体算法描述书上有,注意!这里不涉及e(空串),无论是First还是Follow集
	//以下算法类似First的处理过程
//	pv = nt.next;
//	RSTABLE *pst,*pst1;
	for(i = 0 ; i < nt.size() ; i ++)
	{
		//pst = pv->rshead.next;
		for(j = 0 ; j < nt.get(i).rshead.size(); j++)
		{
			if(isUpLetter(nt.get(i).rshead.get(j)[0]))
			{
				RSTable rst = GetFollow(nt.get(i).rshead.get(j));
				nt.get(i).rshead = nt.get(i).rshead.or(rst);
				nt.get(i).rshead.del(nt.get(i).ch);
			}
		}
//		pst1 = &pv->rshead;
//		pst = pst1->next;
		for(j = 0 ; j < nt.get(i).rshead.size(); j ++)
		{
			if(isUpLetter(nt.get(i).rshead.get(j)[0]))
			{
				nt.get(i).rshead.del(j);
				j --;
			}
		}
	}
	return;
}
void LR1::MakeDollarString()
{
	char ch = 'Z';
	strDollar = "$";
	if(vt.find(VTable_(-1 , strDollar)) == -1)
	{
		return ;
	}
	String strTmp ;
	while(1)
	{
		strTmp = strDollar + ch;
		if(vt.find(VTable_(-1 , strDollar)) == -1)
		{
			strDollar = String(strTmp);
			return ;
		}
		if(ch > 'A')
		{
			ch --;
		}
		else
		{
			strDollar += '$';
			ch = 'Z';
		}
	}
	return ;
}
RSTable LR1::GetFirst(String ch)
{
	RSTable rst;
	int index = nt.find(VTable_(-1 , ch));
	if(index < 0)
	{
		return rst;
	}
	rst = nt.get(index).rsfhead.clone();
	return rst;
}
RSTable LR1::GetFollow(String ch)
{
	RSTable rst;
	int index = nt.find(VTable_(-1 , ch));
	if(index < 0)
	{
		return rst;
	}
	rst = nt.get(index).rshead.clone();
	return rst;
}

void LR1::FillLR1Table()//初始化LR(1)分析表
{
// 	CURSTATE *pc = &I0;
	int i , j , k;
	String szTmp , strTmp;
	strTmp = String(chEntry);
	EncodeString(strTmp);
	strTmp = "\"" + preEntry + "\"->\"" + strTmp + "\"@" ;
	for(i = 0 ; i < I0.size() ; i ++)
	{
//	ps = pc->sihead.next;
		for(j = 0 ; j < I0.get(i).sihead.size(); j++)
		{
			if(strTmp == I0.get(i).sihead.get(j).szWenFa)
			{
				AddToLR1Table(strDollar , String("acc") , I0.get(i).iCode);
				continue;
			}
			int l = GetAtPos(I0.get(i).sihead.get(j).szWenFa);
			l++;
			String strNext ;
			if(GetNextKeyString(strNext , I0.get(i).sihead.get(j).szWenFa , l))
			{
				if(!isUpLetter(strNext[0]))
				{
					int NextCode = GetNextStateNum(strNext , I0.get(i).nehead);
					if(NextCode >= 0)
					{
						szTmp.Format("S%d",NextCode);
						AddToLR1Table(strNext , szTmp , I0.get(i).iCode);
					}                                                                                                                                    
				} 
			}
			else
			{
				//RSTABLE *prst = ps->rssHead.next;
				int NextCode = GetWenFaNum(I0.get(i).sihead.get(j).szWenFa);
				szTmp.Format("r%d",NextCode);
				for(k = 0 ; k < I0.get(i).sihead.get(j).rssHead.size(); k++)
				{
					AddToLR1Table(I0.get(i).sihead.get(j).rssHead.get(k) , szTmp , I0.get(i).iCode);
				}
			}
		}
//		pn = pc->nehead.next;
		for(j = 0 ; j < I0.get(i).nehead.size();j++)
		{
			if(isUpLetter(I0.get(i).nehead.get(j).StepBy[0]))
			{
				szTmp.Format("%d",I0.get(i).nehead.get(j).iNum);
				AddToLR1Table(I0.get(i).nehead.get(j).StepBy , szTmp , I0.get(i).iCode);
			}
		}
	}
	return ;
}
void LR1::AddToLR1Table(LR1Table &lr1head,String szGoto,int iCode)	//此一步已经确定要添加的链,进行添加操作
{
	for(int i = 0 ; i < lr1head.size() ; i++)
	{
		if(lr1head.get(i).iCode == iCode)
		{
			if(!(lr1head.get(i).szChange == szGoto))
			{
				lastError = 10; //表示文法可能不是LR1文法
				throw "不是 LR1 文法!";
			}
			return ;
		}
	}
	lr1head.add(LR1Table_(iCode,szGoto));
	return ;
}
void LR1::AddToLR1Table(String ch , String szGoto,int iCode)	//iCode表示第几个状态(第几行) szGoto表示怎样转换 ch表示哪一个(非)终结符,由此确定要添加的链首
{
	VTable *pv;
	if(ch == strDollar)
	{
		AddToLR1Table(DollarChain, szGoto, iCode);
		return ;
	}
	if(isUpLetter(ch[0]))
	{
		pv = &nt;
	}
	else
	{
		pv = &vt;
	}
	for(int i = 0 ; i < pv->size() ; i++)
	{
		if(ch == pv->get(i).ch)
		{
			AddToLR1Table(pv->get(i).lr1head ,szGoto , iCode);
			return ;
		}
	}
	return ;
}
int LR1::GetAtPos(String strToFind)		//对于非@文法,会死循环
{
	String strTmp ;
	int i = 0;
	GetNextKeyString(strTmp , strToFind , i);
	i += 2;
	while(strToFind[i] != '@')
	{
		GetNextKeyString(strTmp , strToFind , i);
	}
	return i;
}
int LR1::GetNextStateNum(String ch , NextState &ns)	//依靠给定 (非)终结符 获取当前状态由此(非)终结符指向的下一个状态
{
	for(int i = 0 ; i < ns.size() ; i++)
	{
		if(ns.get(i).StepBy == ch)
		{
			return ns.get(i).iNum;
		}
	}
	return -1;
}
int LR1::GetWenFaNum(String szWenFa)	//获取给定文法的ID
{
	for(int i = 0 ; i < sihead.size() ; i++)
	{
		if(isStringEqual(sihead.get(i).szWenFa,szWenFa))
		{
			return sihead.get(i).iNum;
		}
	}
	return -1;
}
bool LR1::isStringEqual(String si,String di)//判断两个文法是否一样,这里会省略@的判断
{
	int i=0,j=0;
	String strsi , strdi;
	GetNextKeyString(strsi , si , i);
	GetNextKeyString(strdi , di , j);
	if(!(strsi == strdi))
	{
		return false;
	}
	i += 2;
	j += 2;		//跳过->
	while(true)
	{
		if(si[i] == '@')
		{
			i++;
			continue;
		}
		if(di[j] == '@')
		{
			j++;
			continue;
		}
		bool bsi,bdi;
		bsi = GetNextKeyString(strsi , si , i);
		bdi = GetNextKeyString(strdi , di , j);
		if(bsi == bdi)
		{
			if(bsi == false)
			{
				return true;
			}
			if(!(strsi == strdi))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}
bool LR1::AddWenFa(SIStr& di , String szWenFa , RSTable rst)	//将文法添加到di链中
{
	if(isWenFaExist(di,szWenFa,rst,1))
	{
		return false;
	}
	for(int i = 0 ; i < di.size() ; i++)
	{
		if(di.get(i).szWenFa == szWenFa)
		{
			di.get(i).rssHead = di.get(i).rssHead.or(rst);		//如果两个文法主部相同,则合并展望字符集
			//因此,同一个状态下,有且仅能有一个这样的文法,此时可以返回
			return true;
		}
	}
	int iNum = 0;
	if(di.size() > 0)
	{
		iNum = di.get(di.size()-1).iNum+1;
	}
	di.add(SingleString_(iNum, szWenFa));
	di.get(di.size() - 1).rssHead = rst;
	//di.get(di.size() - 1).rssHead = di.get(di.size() - 1).rssHead.or(rst);//原来是CopyRSTable
	return true;
}
bool LR1::isWenFaExist(SIStr& si , String szWenfa , RSTable rst, int isNeedEqual)	//判断文法是否存在si链中,第三个参数在LR(1)分析法中使用
{
	//SISTR *ps = si->next;
	for(int i = 0 ; i < si.size() ; i++)
	{
		if(szWenfa == si.get(i).szWenFa)
		{
			if(isNeedEqual)
			{
				if(rst.size() == si.get(i).rssHead.size() && rst == si.get(i).rssHead.and(rst))
				{
					return true;
				}
// 				else
// 				{
//  			//两个文法首部相同,尾部不同,用来区分LALR(1)和LR(1)
// 				//这里可以合并两个文法为一个,即LALR(1)文法了
// 	 			}
			}
			else
			{
				if(rst == si.get(i).rssHead.and(rst))//如果不需要判断相等,这里只需要新的文法的rst被包含在已有的rst里面就行了
				{
					return true;
				}
			}	
		}
	}
	return false;
}
void LR1::CompleteState(CurState_& si)	//将si状态的文法填充完整
{
	int isAdd;
	while(1)
	{
//		ps = si->sihead.next;
		isAdd = 0;
		for(int i = 0 ; i < si.sihead.size() ; i ++)
		{
			String strTmp;
			int j = 0;
			j = GetAtPos(si.sihead.get(i).szWenFa);
// 			GetNextKeyString(strTmp , si.sihead.get(i).szWenFa , j);
// 			j += 2;				//跳过开头和->
// 			while(si.sihead.get(i).szWenFa[j] != '@')		//由于必定存在@,这里不会死循环
// 			{
// 				GetNextKeyString(strTmp , si.sihead.get(i).szWenFa , j);
// 			}
			j++;
			int k = j;
			if(GetNextKeyString(strTmp , si.sihead.get(i).szWenFa , k) && isUpLetter(strTmp[0]))
			{
				isAdd |= AddSWenFa(si.sihead , si.sihead.get(i).szWenFa , j , si.sihead.get(i).rssHead);
			}
		}
		if(!isAdd)
		{
			break;
		}
	}
	return ;
}
bool LR1::AddSWenFa(SIStr& si , String ch , int index , RSTable rst)	//在全局文法链中找到以ch开头的文法并添加到si链中,没有添加则返回0,否则返回1
{
	bool isAdd = false;
	String strTmp , strTmp1;
	if(!GetNextKeyString(strTmp , ch , index) || !isUpLetter(strTmp[0]))
	{
		return isAdd;
	}
	bool isWenFaNotEnd = GetNextKeyString(strTmp1 , ch , index );		//注意,由于调用AddSWenFa之前,已经将@识别出来了,即@在index之前,因此治理不用考虑@
//	SISTR *ps = sihead.next;
	for(int i= 0 ; i < sihead.size() ; i ++)
	{
		String psF ;
		int iTmp = 0;
		GetNextKeyString(psF , sihead.get(i).szWenFa , iTmp);
		if(psF == strTmp)			//找到了以@S 中 以S->?? 开始的文法
		{
			RSTable rsTmp;
			if(isWenFaNotEnd)				//一下主要是填充需要加入的文法的展望字符集
			{
				if(!isUpLetter(strTmp1[0]))
				{
					rsTmp.add(strTmp1);
				}
				else
				{
					rsTmp = GetFirst(strTmp1);
				}
			}
			else
			{
				rsTmp = rst;
			}
			String szWenFa;
			MakeAtWenFa(sihead.get(i).szWenFa , szWenFa);
			if(!isWenFaExist(si , szWenFa , rsTmp))
			{
				AddWenFa(si , szWenFa , rsTmp);
				isAdd = true;
			}
		}
	}
	return isAdd;
}
void LR1::MakeAtWenFa(String& siWenFa , String& diWenFa)	//将普通文法转换为@文法
{
	int i = 0 ;
	String strTmp , strTmp1;
	GetNextKeyString(strTmp , siWenFa , i);
	i += 2;
	strTmp1 = siWenFa.substr(i , siWenFa.length() - i);
	strTmp = siWenFa.substr(0 , i);
	diWenFa = strTmp + "@" + strTmp1;
	return ;
}
bool LR1::MakeNextState(CurState_& si , CurState_& di , String Next)	//由当前状态si经Next转换后产生下一个状态,保存在di中
{
	bool isAdd = 0;
//	SISTR *ps = si->sihead.next;
	for(int i = 0 ; i < si.sihead.size() ; i++)
	{
		int j = GetAtPos(si.sihead.get(i).szWenFa);
		int k = j;
		j++;
		String strTmp ;
		if(GetNextKeyString(strTmp , si.sihead.get(i).szWenFa , j) && strTmp == Next)
		{
			String strPrev,strLast,strNew;
			strPrev = si.sihead.get(i).szWenFa.substr(0 , k);
			if(j < si.sihead.get(i).szWenFa.length())
			{
				strLast = si.sihead.get(i).szWenFa.substr(j , si.sihead.get(i).szWenFa.length() - j);
			}
			EncodeString(strTmp);
			strNew = strPrev + "\"" + strTmp + "\"@" + strLast;
			AddWenFa(di.sihead , strNew , si.sihead.get(i).rssHead);
			di.iCode = 1;
			isAdd = true;
		}
	}
	if(isAdd)
	{
		CompleteState(di);
	}
	return isAdd;
}
bool LR1::isStateExist(CurState_& state)		//判断状态是否已经存在,存在则返回1并填充iCode为该状态的编号,否则返回0
{
//	CURSTATE *pc = &I0;
	for(int i = 0 ; i < I0.size() ; i ++)
	{
		if(state.hashCode == I0.get(i).hashCode)
		{
			if(isWenFaEqual(I0.get(i).sihead , state.sihead))
			{
				state.iCode = I0.get(i).iCode;
				return true;
			}
		}
	}
	return false;
}
bool LR1::isWenFaEqual(SIStr& si , SIStr& di)	//判断两个文法是否相等(顺序不计)
{
	//SISTR * ps = si->next, *ps1 = di->next;
	for(int i = 0 ; i < si.size() ; i ++)
	{
		if(!isWenFaExist(di, si.get(i).szWenFa, si.get(i).rssHead, 1))
		{
			return false;
		}
	}
	return true;
}
void LR1::printLR1State()
{
/*	FILE *pf = fopen("C:\\state.txt","w");*/
	int i , j , k ;
	for(i = 0 ; i < I0.size() ; i++)
	{
		printf("I%d\n",I0.get(i).iCode);
/*		fprintf(pf,"I%d\r\n",I0.get(i).iCode);*/
		for(j = 0 ; j < I0.get(i).sihead.size() ; j++)
		{
			printf("%s ",I0.get(i).sihead.get(j).szWenFa.c_str());
/*			fprintf(pf,"%s ",I0.get(i).sihead.get(j).szWenFa.c_str());*/
			for(k = 0 ; k < I0.get(i).sihead.get(j).rssHead.size(); k ++)
			{
				printf(",%s",I0.get(i).sihead.get(j).rssHead.get(k).c_str());
/*				fprintf(pf,",%s",I0.get(i).sihead.get(j).rssHead.get(k).c_str());*/
			}
			puts("");
/*			fputs("\r\n",pf);*/

		}
		for(k = 0 ; k < I0.get(i).nehead.size(); k++)
		{
			printf("(%s -> %d) ",I0.get(i).nehead.get(k).StepBy.c_str() , I0.get(i).nehead.get(k).iNum);
/*			fprintf(pf,"(%s -> %d) ",I0.get(i).nehead.get(k).StepBy.c_str() , I0.get(i).nehead.get(k).iNum);*/
		}
		puts("\r\n");
/*		fputs("\r\n\r\n",pf);*/
	}
/*	fclose(pf);*/
}

void LR1::outputStateToFile(const char* outFile)
{
	StateFileHelper sfh;
	if(!sfh.BeginWrite(outFile))
	{
		return ;
	}
	int i,j,k;
	sfh.AddString("vt");
	sfh.AddInt(vt.size());
	for(i = 0; i < vt.size(); i++)
	{
		sfh.NewLine(1);
		sfh.AddString(vt.get(i).ch);
		sfh.AddInt(vt.get(i).iCode);
		sfh.AddInt(vt.get(i).realString.size());
		sfh.AddInt(vt.get(i).lr1head.size());
		sfh.NewLine(2);
		for(j = 0; j < vt.get(i).realString.size();j++)
		{
			sfh.AddString(vt.get(i).realString.get(j));
		}
		sfh.NewLine(2);
		for(j = 0; j < vt.get(i).lr1head.size();j++)
		{
			sfh.AddInt(vt.get(i).lr1head.get(j).iCode);
			sfh.AddString(vt.get(i).lr1head.get(j).szChange);
		}
	}
	sfh.NewLine();
	sfh.AddString("nt");
	sfh.AddInt(nt.size());
	for(i = 0; i < nt.size(); i++)
	{
		sfh.NewLine(1);
		sfh.AddString(nt.get(i).ch);
		sfh.AddInt(nt.get(i).iCode);
		sfh.AddInt(nt.get(i).realString.size());
		sfh.AddInt(nt.get(i).lr1head.size());
		sfh.NewLine(2);
		for(j = 0; j < nt.get(i).realString.size();j++)
		{
			sfh.AddString(nt.get(i).realString.get(j));
		}
		sfh.NewLine(2);
		for(j = 0; j < nt.get(i).lr1head.size();j++)
		{
			sfh.AddInt(nt.get(i).lr1head.get(j).iCode);
			sfh.AddString(nt.get(i).lr1head.get(j).szChange);
		}
	}

	sfh.NewLine();
	sfh.AddString("si");
	sfh.AddInt(sihead.size());
	for(i = 0; i < sihead.size(); i++)
	{
		sfh.NewLine(1);
		sfh.AddInt(sihead.get(i).iNum);
		sfh.AddString(sihead.get(i).szWenFa);
		sfh.AddInt(sihead.get(i).rssHead.size());
		sfh.NewLine(2);
		for(j = 0; j < sihead.get(i).rssHead.size(); j++)
		{
			sfh.AddString(sihead.get(i).rssHead.get(j));
		}
	}

	sfh.NewLine();
	sfh.AddString("entry");
	sfh.AddString(chEntry);

	sfh.NewLine();
	sfh.AddString("dollar");
	sfh.AddString(strDollar);

	sfh.NewLine();
	sfh.AddString("prevEntry");
	sfh.AddString(preEntry);

	sfh.NewLine();
	sfh.AddString("dollarChain");
	sfh.AddInt(DollarChain.size());
	sfh.NewLine(1);
	for(i = 0; i < DollarChain.size();i++)
	{
		sfh.AddInt(DollarChain.get(i).iCode);
		sfh.AddString(DollarChain.get(i).szChange);
	}
// 	sfh.NewLine();
// 	sfh.AddString("state");
// 	sfh.AddInt(I0.size());
// 	for(i = 0; i < I0.size(); i++)
// 	{
// 		sfh.NewLine(1);
// 		sfh.AddInt(I0.get(i).iCode);
// 		sfh.AddInt(I0.get(i).sihead.size());
// 		sfh.AddInt(I0.get(i).nehead.size());
// 		for(j = 0; j < I0.get(i).sihead.size();j++)
// 		{
// 			sfh.NewLine(2);
// 			sfh.AddInt(I0.get(i).sihead.get(j).iNum);
// 			sfh.AddString(I0.get(i).sihead.get(j).szWenFa);
// 			sfh.AddInt(I0.get(i).sihead.get(j).rssHead.size());
// 			sfh.NewLine(3);
// 			for(k = 0; k < I0.get(i).sihead.get(j).rssHead.size(); k++)
// 			{
// 				sfh.AddString(I0.get(i).sihead.get(j).rssHead.get(k));
// 			}
// 		}
// 		sfh.NewLine(2);
// 		for(j = 0; j < I0.get(i).nehead.size(); j++)
// 		{
// 			sfh.AddInt(I0.get(i).nehead.get(j).iNum);
// 			sfh.AddString(I0.get(i).nehead.get(j).StepBy);
// 		}
// 	}
	sfh.EndWrite();
}
bool LR1::initStateFromFile(const char* outFile)
{
	StateFileHelper sfh;
	if(!sfh.BeginRead(outFile))
	{
		return false;
	}
	String str0,str1;
	int i,j,k;
	int iv0,iv1,iv2,iv3;
	sfh.GetLine();
	sfh.GetString(str0);
	sfh.GetInt(iv0);
	for(i = 0; i < iv0; i++)
	{
		VTable_ t;
		sfh.GetLine();
		sfh.GetString(t.ch);
		sfh.GetInt(t.iCode);
		sfh.GetInt(iv1);
		sfh.GetInt(iv2);
		sfh.GetLine();
		for(j = 0; j < iv1; j++)
		{
			sfh.GetString(str0);
			t.realString.add(str0);
		}
		sfh.GetLine();
		for(j = 0; j < iv2; j++)
		{
			LR1Table_ lt;
			sfh.GetInt(lt.iCode);
			sfh.GetString(lt.szChange);
			t.lr1head.add(lt);
		}
		vt.add(t);
	}
	sfh.GetLine();
	sfh.GetString(str0);
	sfh.GetInt(iv0);
	for(i = 0; i < iv0; i++)
	{
		VTable_ t;
		sfh.GetLine();
		sfh.GetString(t.ch);
		sfh.GetInt(t.iCode);
		sfh.GetInt(iv1);
		sfh.GetInt(iv2);
		sfh.GetLine();
		for(j = 0; j < iv1; j++)
		{
			sfh.GetString(str0);
			t.realString.add(str0);
		}
		sfh.GetLine();
		for(j = 0; j < iv2; j++)
		{
			LR1Table_ lt;
			sfh.GetInt(lt.iCode);
			sfh.GetString(lt.szChange);
			t.lr1head.add(lt);
		}
		nt.add(t);
	}
	sfh.GetLine();
	sfh.GetString(str0);
	sfh.GetInt(iv0);
	for(i = 0; i < iv0; i++)
	{
		SingleString_ t;
		sfh.GetLine();
		sfh.GetInt(t.iNum);
		sfh.GetString(t.szWenFa);
		sfh.GetInt(iv1);
		sfh.GetLine();
		for(j = 0; j < iv1; j++)
		{
			sfh.GetString(str0);
			t.rssHead.add(str0);
		}
		sihead.add(t);
	}
	sfh.GetLine();
	sfh.GetString(str0);
	sfh.GetString(chEntry);

	sfh.GetLine();
	sfh.GetString(str0);
	sfh.GetString(strDollar);

	sfh.GetLine();
	sfh.GetString(str0);
	sfh.GetString(preEntry);

	sfh.GetLine();
	sfh.GetString(str0);
	sfh.GetInt(iv0);
	sfh.GetLine();
	for(i = 0; i < iv0; i++)
	{
		LR1Table_ t;
		sfh.GetInt(t.iCode);
		sfh.GetString(t.szChange);
		DollarChain.add(t);
	}

// 	sfh.GetLine();
// 	sfh.GetString(str0);
// 	sfh.GetInt(iv0);
// 	for(i = 0; i < iv0; i++)
// 	{
// 		CurState_ t;
// 		sfh.GetLine();
// 		sfh.GetInt(t.iCode);
// 		sfh.GetInt(iv1);
// 		sfh.GetInt(iv2);
// 		for(j = 0; j < iv1; j++)
// 		{
// 			SingleString_ sst;
// 			sfh.GetLine();
// 			sfh.GetInt(sst.iNum);
// 			sfh.GetString(sst.szWenFa);
// 			sfh.GetInt(iv3);
// 			sfh.GetLine();
// 			for(k = 0; k < iv3; k++)
// 			{
// 				sfh.GetString(str0);
// 				sst.rssHead.add(str0);
// 			}
// 			t.sihead.add(sst);
// 		}
// 		sfh.GetLine();
// 		for(j = 0; j < iv2; j++)
// 		{
// 			NextState_ nst;
// 			sfh.GetInt(nst.iNum);
// 			sfh.GetString(nst.StepBy);
// 			t.nehead.add(nst);
// 		}
// 		I0.add(t);
// 	}
	sfh.EndRead();
	return true;
}
void LR1::printLR1Table()
{
	int i , j;
	puts("ACTION");
	for(i = 0 ; i < vt.size() ; i++)
	{
		printf("%s\n",vt.get(i).ch.c_str());
		for(j = 0 ; j < vt.get(i).lr1head.size() ; j++)
		{
			printf("(%d,%s) ",vt.get(i).lr1head.get(j).iCode , vt.get(i).lr1head.get(j).szChange.c_str());
		}
		puts("");
	}
	printf("%s\n",strDollar.c_str());
	for(j = 0 ; j < DollarChain.size() ; j++)
	{
		printf("(%d,%s) ",DollarChain.get(j).iCode , DollarChain.get(j).szChange.c_str());
	}
	puts("");

	puts("GOTO");
	for(i = 0 ; i < nt.size() ; i++)
	{
		printf("%s\n",nt.get(i).ch.c_str());
		for(j = 0 ; j < nt.get(i).lr1head.size() ; j++)
		{
			printf("(%d,%s) ",nt.get(i).lr1head.get(j).iCode , nt.get(i).lr1head.get(j).szChange.c_str());
		}
		puts("");
	}
}
void LR1::ComeToEndMatch(String strZhongjiefu, int &zIndex)
{
	int count = 1;
	while(zIndex < strZhongjiefu.length() && count > 0)
	{
		if(strZhongjiefu[zIndex] == '\\')
		{
			zIndex ++;
		}
		else if(strZhongjiefu[zIndex] == '[')
		{
			count ++;
		}
		else if(strZhongjiefu[zIndex] == ']')
		{
			count --;
		}
		zIndex++;
	}
}
bool LR1::isSingleMatch(String strZhongjiefu, int &zIndex, BufferIn &bf, int bIndex)
{
	if(isHex(strZhongjiefu[zIndex + 1]) && isHex(strZhongjiefu[zIndex + 2]) && strZhongjiefu[zIndex + 3] == '-'
		&& isHex(strZhongjiefu[zIndex + 4]) && isHex(strZhongjiefu[zIndex + 5]) && strZhongjiefu[zIndex + 6] == ']')	//mode 5
	{
		int left = (HexToDig(strZhongjiefu[zIndex + 1]) << 4) | HexToDig(strZhongjiefu[zIndex + 2]);
		int right = (HexToDig(strZhongjiefu[zIndex + 4]) << 4) | HexToDig(strZhongjiefu[zIndex + 5]);
		if(bf[bIndex] >= left && bf[bIndex] <= right)
		{
			zIndex += 7;
			return true;
		}
		else 
		{
			zIndex ++;
			ComeToEndMatch(strZhongjiefu, zIndex);
			return false;
		}
	}
	else if(strZhongjiefu[zIndex + 1] == '^' 
		&& isHex(strZhongjiefu[zIndex + 2]) && isHex(strZhongjiefu[zIndex + 3]) && strZhongjiefu[zIndex + 4] == '-'
		&& isHex(strZhongjiefu[zIndex + 5]) && isHex(strZhongjiefu[zIndex + 6]) && strZhongjiefu[zIndex + 7] == ']')//mode 6
	{
		int left = (HexToDig(strZhongjiefu[zIndex + 2]) << 4) | HexToDig(strZhongjiefu[zIndex + 3]);
		int right = (HexToDig(strZhongjiefu[zIndex + 5]) << 4) | HexToDig(strZhongjiefu[zIndex + 6]);
		if(bf[bIndex] >= left && bf[bIndex] <= right)
		{
			zIndex ++;
			ComeToEndMatch(strZhongjiefu, zIndex);
			return false;
		}
		else 
		{
			bIndex += 8;
			return true;
		}
	}
	else if(strZhongjiefu[zIndex + 1] != '\0' && strZhongjiefu[zIndex + 2] == '-'
		&& strZhongjiefu[zIndex + 3] != '\0' && strZhongjiefu[zIndex + 4] == ']')//mode 2
	{
		int left = strZhongjiefu[zIndex + 1];
		int right = strZhongjiefu[zIndex + 3];
		if(bf[bIndex] >= left && bf[bIndex] <= right)
		{
			zIndex += 5;
			return true;
		}
		else 
		{
			zIndex ++;
			ComeToEndMatch(strZhongjiefu, zIndex);
			return false;
		}
	}
	else if(strZhongjiefu[zIndex + 1] == '^'
		&& strZhongjiefu[zIndex + 2] != '\0' && strZhongjiefu[zIndex + 3] == '-'
		&& strZhongjiefu[zIndex + 4] != '\0' && strZhongjiefu[zIndex + 5] == ']')//mode 4
	{
		int left = strZhongjiefu[zIndex + 2];
		int right = strZhongjiefu[zIndex + 4];
		if(bf[bIndex] >= left && bf[bIndex] <= right)
		{
			zIndex ++;
			ComeToEndMatch(strZhongjiefu, zIndex);
			return false;
		}
		else 
		{
			zIndex += 6;
			return true;
		}
	}
	else
	{
		bool res = false;
		while(zIndex < strZhongjiefu.length())
		{
			zIndex ++;
			if(res)
			{
				ComeToEndMatch(strZhongjiefu, zIndex);
				break;
			}
			if(strZhongjiefu[zIndex] == '\\')
			{
				if(bf[bIndex] == strZhongjiefu[zIndex + 1])
				{
					res = true;
				}
				zIndex ++;
			}
			else if(strZhongjiefu[zIndex] == '[')
			{
				res = isSingleMatch(strZhongjiefu, zIndex, bf, bIndex);
				if(res)
				{
 					ComeToEndMatch(strZhongjiefu, zIndex);
					break;
				}
				zIndex --;
			}
			else if(strZhongjiefu[zIndex] == ']')
			{
				zIndex ++;
				break;
			}
			else 
			{
				if(bf[bIndex] == strZhongjiefu[zIndex])
				{
					res = true;
				}
			}
		}
		return res;
	}
}
bool LR1::CheckZhongjiefuWithRealString(String strZhongjiefu, BufferIn& bf, int &index, String& strOut)
{
	int i = 0;
	strOut = "";
	while(i < strZhongjiefu.length())
	{
		if(strZhongjiefu[i] == '\\')
		{
			if(strZhongjiefu[i + 1] == bf[index])
			{
				strOut += bf[index];
				index ++;
				i += 2;
			}
			else
			{
				return false;
			}
			//转义字符
		}
		else if(strZhongjiefu[i] == '[')
		{
			int j = i;
			if(isSingleMatch(strZhongjiefu, j, bf, index))
			{
				strOut += bf[index];
				index ++;
				i = j;
			}
			else
			{
				return false;
			}
			//处理通配符
		}
		else if(strZhongjiefu[i] == '*')
		{
			int begin = 0, end = -1;
			i ++;
			String strToMatch = "";
			int j = index;
			if(isNum(strZhongjiefu[i]) && isNum(strZhongjiefu[i+1]) && strZhongjiefu[i+2] == '-'
				&& isNum(strZhongjiefu[i+3]) && isNum(strZhongjiefu[i+4]) 
				&& (strZhongjiefu[i+5] == '{' || strZhongjiefu[i+5] == '['))
			{
				begin = (strZhongjiefu[i] - '0') * 10 + strZhongjiefu[i+1] - '0';
				end = (strZhongjiefu[i+3] - '0') * 10 + strZhongjiefu[i+4] - '0';
				if(end < begin)
				{
					end = -1;
				}
				i += 5;
			}
			if(strZhongjiefu[i] == '{')
			{
				i ++;
				int count = 0;
				while(i < strZhongjiefu.length())
				{
					if(strZhongjiefu[i] == '\\')
					{
						i ++;
						strToMatch += strZhongjiefu[i];
						i ++;
						continue;
					}
					if(strZhongjiefu[i] == '}')
					{
						count--;
						if(count < 0)
						{
							i++;
							break;
						}
						count--;
					}
					if(strZhongjiefu[i] == '{')
					{
						count ++;
					}
					strToMatch += strZhongjiefu[i];
					i++;
				}
				if(count >= 0)
				{
					return false;
				}
			}
			else if(strZhongjiefu[i] == '[')
			{
				int count = 0;
				strToMatch = "[";
				i ++;
				while(i < strZhongjiefu.length())
				{
					if(strZhongjiefu[i] == '\\')
					{
						i ++;
						strToMatch += strZhongjiefu[i];
						i ++;
						continue;
					}
					if(strZhongjiefu[i] == ']')
					{
						count--;
						if(count < 0)
						{
							i++;
							strToMatch += ']';
							break;
						}
						count--;
					}
					if(strZhongjiefu[i] == '[')
					{
						count ++;
					}
					strToMatch += strZhongjiefu[i];
					i++;
				}
				if(count >= 0)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
			int k = 0;
			while(k < strToMatch.length())
			{
				String strOutTmp = "";
				if(CheckZhongjiefuWithRealString(strToMatch, bf, index, strOutTmp))
				{
					strOut += strOutTmp;
					if(begin > 0)
					{
						begin --;
					}
					if(end > 0)
					{
						end --;
					}
					else if(end == 0)
					{
						index = j;
						return false;
					}
				}
				else
				{
					if(begin == 0)
					{
						break;
					}
					else
					{
						index = j;
						return false;
					}
				}
			}


			//多个相同字符匹配
			//*'abc'([a-z])		//任意个后面的
			//*12-34[]('')//12到34个后面的...//可以没有终点,比如*1-'1'表示至少有一个1
		}
		else if(strZhongjiefu[i] == '$')
		{
			if(strZhongjiefu[i + 1] == '[')
			{
				int j = i + 1;
				if(isSingleMatch(strZhongjiefu, j, bf, index))
				{
					return false;
				}
				return true;
			}
			else if(strZhongjiefu[i + 1] == '\\')
			{
				if(strZhongjiefu[i + 2] == bf[index])
				{
					return false;
				}
				return true;
			}
			else
			{
				if(strZhongjiefu[i + 1] == bf[index])
				{
					return false;
				}
				return true;
			}
			//表示处理到结尾,并且$后面必须接上一个字符,并且该字符不能完全匹配(用于关键字和变量的区分)
		}
// 		else if(strZhongjiefu[i] == '?')
// 		{
// 			i++;
// 			strOut += bf[index];
// 			index ++;
// 			//任意字符
// 		}
		else
		{
			if(strZhongjiefu[i] == bf[index])
			{
				strOut += bf[index];
				index ++;
				i ++;
			}
			else
			{
				return false;
			}
		}
	}
	return true;

}

void LR1::scaner()					//词法分析器
{
	curType = -1;
	szToken = "";
	while(1)
	{
		if(szToRec[ScanerIndex] == '\0')
		{
			curType = 0;
			return ;
		}
		if(szToRec[ScanerIndex] == ' ' || szToRec[ScanerIndex] == '\n' || szToRec[ScanerIndex] == '\t' || szToRec[ScanerIndex] == '\r') //跳过空格,换行,退格
		{
			Skip(szToRec , ScanerIndex);
			continue;
		}
		for(int i = 0; i < vt.size(); i++)
		{
			for(int j = 0; j < vt.get(i).realString.size(); j++)
			{
				int IndexBuf = ScanerIndex;
				if(vt.get(i).realString.get(j) == "string"){
					if(szToRec[IndexBuf] == '\"'){
						int sizeBase = 5, index = 0;
						IndexBuf++;
						char *szTmp = new char[sizeBase];
						while(szToRec[IndexBuf] != '\0'){
							if(szToRec[IndexBuf] == '\\'){
								szTmp[index] = szToRec[IndexBuf+1];
								IndexBuf += 2;
								index ++;
								if(index >= sizeBase-1){
									sizeBase = resize(&szTmp,sizeBase);
								}
							}else if(szToRec[IndexBuf] == '\"'){
								break;
							}else{
								szTmp[index] = szToRec[IndexBuf];
								IndexBuf ++;
								index ++;
								if(index >= sizeBase-1){
									sizeBase = resize(&szTmp,sizeBase);
								}
							}
						}
						szTmp[index] = '\0';
						if(szToRec[IndexBuf] == '\"'){
							curType = vt.get(i).iCode;
							ScanerIndex = IndexBuf+1;
							szToken = String(szTmp);
							delete[]szTmp;
							return;
						}
						
					}
				}else if(vt.get(i).realString.get(j) == "char"){
					if(szToRec[IndexBuf] == '\''){
						if(szToRec[IndexBuf+1] == '\\' && szToRec[IndexBuf+3] == '\''){
							szToken = String("") + szToRec[IndexBuf+2];
							curType = vt.get(i).iCode;
							ScanerIndex = IndexBuf+4;
							return;
						}else if(szToRec[IndexBuf+2] == '\''){
							szToken = String("") + szToRec[IndexBuf+1];
							curType = vt.get(i).iCode;
							ScanerIndex = IndexBuf+3;
							return;
						}
					}
				}else if(vt.get(i).realString.get(j) == "int"){
					vt.get(i).realString.get(j) = DIGINTDEX;
					vt.get(i).realString.add(DIGINTHEX);
					j --;
					continue;
				}else if(vt.get(i).realString.get(j) == "float" || vt.get(i).realString.get(j) == "double"){
					vt.get(i).realString.get(j) = DIGFLOAT;
					j --;
					continue;
				}else if(vt.get(i).realString.get(j) == "bool"){
					vt.get(i).realString.get(j) = BOOLTRUE;
					vt.get(i).realString.add(BOOLFALSE);
					j --;
					continue;
				}else if(vt.get(i).realString.get(j) == "null"){
					vt.get(i).realString.get(j) = NULLVALUE;
					j --;
					continue;
				}else if(vt.get(i).realString.get(j) == "exevar"){
					if(isLetter(szToRec[IndexBuf])){
						szToken = "";
						while(szToRec[IndexBuf] && szToRec[IndexBuf] != ' ' && szToRec[IndexBuf] != '\n'){
							szToken += szToRec[IndexBuf];
							IndexBuf ++;
						}
						ScanerIndex = IndexBuf;
						curType = vt.get(i).iCode;
						return ;
					}
				}
				else if(CheckZhongjiefuWithRealString(vt.get(i).realString.get(j), szToRec, IndexBuf, szToken))
				{
					curType = vt.get(i).iCode;
					ScanerIndex = IndexBuf;
					return ;
				}
			}
		}
		szToken = "";
		return ;
	}
	return ;
}

String LR1::GetAction(int State , int iCode)	//从action表获取转换状态,State表示行,iCode表示终结符对应的ID
{
	PLR1Table ps = NULL;
	if(iCode == 0)
	{
		ps = &DollarChain;
	}
	else
	{
		for(int i = 0; i < vt.size(); i++)
		{
			if(vt.get(i).iCode == iCode)
			{
				ps = &(vt.get(i).lr1head);
				break;
			}
		}
	}
	if(ps != NULL)
	{
		for(int i = 0; i < ps[0].size(); i++)
		{
			if(ps[0].get(i).iCode == State)
			{
				return ps[0].get(i).szChange;
			}
		}
	}
	return String("");
}

int LR1::GetGoto(int State , String& ch)	//获取转换表对应的转换状态,State表示行,ch表示非终结符
{
	if(!isUpLetter(ch[0]))
	{
		return -1;
	}
	PLR1Table ps = NULL;
	for(int i = 0; i < nt.size(); i++)
	{
		if(nt.get(i).ch == ch)
		{
			ps = &(nt.get(i).lr1head);
			break;
		}
	}
	if(ps)
	{
		for(int i = 0; i < ps->size(); i++)
		{
			if(ps->get(i).iCode == State)
			{
				int Code = -1;
				sscanf(ps->get(i).szChange.c_str(), "%d", &Code);
				return Code;
			}
		}
	}
	return -1;
}

String LR1::GetZhongJieFu(int iCode)
{
	if(iCode == 0)
	{
		return strDollar;
	}
	else
	{
		for(int i = 0; i < vt.size(); i++)
		{
			if(vt.get(i).iCode == iCode)
			{
				return vt.get(i).ch;
			}
		}
	}
	return String("");
}

String LR1::GetWenFaStr(int ID)		//获取指定ID的文法
{
	for(int i= 0 ;i < sihead.size(); i++)
	{
		if(sihead.get(i).iNum == ID)
		{
			return sihead.get(i).szWenFa;
		}
	}
	return String("");
}

int LR1::Recognize(const char *szInput,bool isFilePath)
{
	InstallDefaultAttrs();
	lastError = 0;
	ScanerIndex = 0;
	InitInput(szInput,isFilePath);
	AnalysisStack as;
	as.push(AnalysisStack_(strDollar,0));
	this->params.set(LR1_REC_STACK, &as);

	scaner();
	if(MSG_DBG)
	printf("meet %s\ttype:%d(%s)\t nextChar%c\n", this->szToken.c_str(), curType,vt.get(curType-1).ch.c_str(), szToRec[ScanerIndex]);
	do{
		if(!lastError)
		{
			String acc = GetAction(as.top().iState,curType);
			if(acc.length() == 0)
			{
				lastError = 100;//
				break;
			}
			if(acc == "acc")
			{
				break;
			}
			if(acc[0] == 'S')
			{
				int iState;
				String lastSimbol = GetZhongJieFu(curType);
				sscanf(acc.c_str() + 1, "%d", &iState);

				if(lastSimbol.length() == 0)
				{
					lastError = 101;
					break;
				}
				AnalysisStack_ ast(lastSimbol, iState);
				ast.sn.szName = szToken;

				for(int i = 0; i < vt.size(); i++)
				{
					if(vt.get(i).iCode == curType)
					{
						if(vt.get(i).onMeetHandler != NULL)
						{
							vt.get(i).onMeetHandler(ast.sn, this);
						}
						else
						{
							throw "终结符" + vt.get(i).ch + "未找到处理函数!";
						}
					}
				}
				as.push(ast);
				if (MSG_DBG)
				printf("meet %s\n", lastSimbol.c_str());
				scaner();
				if(curType <= 0){
					if (MSG_DBG)
					printf("meet end or error!\n");
				}else{
					if (MSG_DBG)
					printf("meet %s\ttype:%d(%s)\t nextChar%c\n", this->szToken.c_str(), curType,vt.get(curType-1).ch.c_str(), szToRec[ScanerIndex]);
				}
			}
			else if(acc[0] == 'r')
			{
				int iCode = -1;
				sscanf(acc.c_str() + 1,"%d",&iCode);
				String pr = GetWenFaStr(iCode);
				if(pr.length() == 0)
				{
					lastError = 103;		//基本不会出现获取不到文法的情况
					break;
				}
				if (MSG_DBG)
				printf("Call %s\n" , pr.c_str());
				int i = 0 , j = 0;
				String strF ;
				GetNextKeyString(strF , pr , i);
				i += 2;
				String strTmp;
				while(GetNextKeyString(strTmp , pr , i))
				{
					j ++;
				}
//				pas1 = pas = pas->next;		//由于最前面有$保护,因此栈不会出问题(比如到了最前面再取prev)
				StackNode sn(j);
				while(j > 0)
				{
					sn.set(--j, as.top().sn);
					as.pop();
				}
				for(j = 0; j < sihead.size(); j++)
				{
					if(sihead.get(j).iNum == iCode)
					{
						if(sihead.get(j).handler == NULL)
						{
							throw "文法:" + sihead.get(j).szWenFa + " 未找到处理函数!";
						}
						else
						{     
							sihead.get(j).handler(sn, this);
						}
					}
				}
//				CallOnDealFunc(iCode , sn);
				String strTmp33;
				for(int kk = 0; kk < sn.size(); kk++)
				{
					strTmp33 += sn.get(kk).szName;
				}
				if (MSG_DBG)
				printf("%s\n",strTmp33.c_str());
				int iState = GetGoto(as.top().iState, strF);
				if(iState < 0)
				{
					lastError = 104;
					break;
				}
				AnalysisStack_ ast(strF, iState);
				ast.sn = sn.get(0);
				sn.empty();
				as.push(ast);
			}
		}
	}
	while(true);
	as.empty();
	szToRec.~BufferIn();
	DisposeDefaultValues();
	this->params.erase(LR1_REC_STACK);
	if(lastError)
	{
		if (MSG_DBG)
		printf("Error! %d\n" ,lastError);
		return lastError;
	}
	else
	{
		if (MSG_DBG)
		puts("success!");
		return 0;
	}
}

bool LR1::registerHandler(String strWenFa, REDUCE_HANDLER handler)
{
	for(int i = 0; i < sihead.size(); i++)
	{
		if(sihead.get(i).szWenFa == strWenFa)
		{
			sihead.get(i).handler = handler;
			return true;
		}
	}
	throw "注册处理函数失败!找不到文法!";
	return false;
}

bool LR1::registerHandler(int iWenFaIndex, REDUCE_HANDLER handler)
{
	for(int i = 0; i < sihead.size(); i++)
	{
		if(sihead.get(i).iNum == iWenFaIndex)
		{
			sihead.get(i).handler = handler;
			return true;
		}
	}
	throw "注册处理函数失败!找不到文法!";
	return false;
}

bool LR1::registerOnMeetHandler(String chZhongJieFu, ONMEET_HANDLER handler)
{
	int i = 0;
	for(; i < vt.size(); i++)
	{
		if(vt.get(i).ch == chZhongJieFu)
		{
			vt.get(i).onMeetHandler = handler;
			break;
		}
	}
	if(i == vt.size() && i != 0)
	{
		throw "注册处理函数失败!找不到终结符!";
		return false;
	}
	return true;
}

void LR1::InstallDefaultAttrs()
{

}

void LR1::DisposeDefaultValues()
{

}