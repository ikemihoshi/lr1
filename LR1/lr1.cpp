#include "lr1.h"
#include "StateFileHelper.h"
#include <Windows.h>

#define MSG_DBG false

void defaultHandler(StackNode& sn, LR1* lr1){
}

void defaultOnMeetHandler(StackNode_& sn, LR1* lr1){

}

bool LR1::isUpLetter(char ch)	//�Ƿ��д
{
	return ch >= 'A' && ch <= 'Z';
}
bool LR1::isDnLetter(char ch)	//�Ƿ�Сд
{
	return ch >= 'a' && ch <= 'z';
}
bool LR1::isLetter(char ch)		//�Ƿ��ַ�
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
		if(szRule[i] == ';' || szRule[i] == '#' || !szRule[i])		//;��ʾע��,��������ע��,�����ڴ�һ������
		{
			while(szRule[i] && szRule[i] != '\n')
			{
				i++;
			}
			i++;	//ֻ������\n�Ż��˳�,�������ַ�Ϊ;,������һ��,����ת����һ��
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
				if(i == j + 2)	//û�б�,��->����û����
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
// 			else if(szRule[i] == '=' && isNum(szRule[i + 1]))	//�Է��ս�����и�ֵ,��ֵ��������Ӧ�ս������֮��
// 			{
// 				VTable_ vtmp(-1,strKey);
// 				int k = vt.find(vtmp);
// 				if(k == -1)
// 				{
// 					return 2;//WeiZhiZhongJieFu;		//�ս��δ�ҵ�!
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
// 			}		//���ڸ���������,���ﲻ����Ҫ���ֱ����Ψһ��ʶ,ȡ����֮����"[a-b]cc#[^a]":"m"����ʽ,���������
			else if(szRule[i] == ':')			//�����,���������
			{
				i++;
				int iVtIndex = vt.find(VTable_(-1,strKey));
				if(iVtIndex < 0)
				{
					return 7;	//δ�ҵ�ָ�����ս��
				}
				String strRealStr;
				while(szRule[i] && (szRule[i] == '\n' && szRule[i] == ' ' && szRule[i] == '\t'))
				{
					i++;
				}
				if(szRule[i] == '\0')
				{
					return 5;		//�쳣���ļ���β
				}
				if(szRule[i] != '\"')
				{
					return 6;		//�ս����������ȷ
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

	st.szWenFa = "\"" + preEntry  + "\"->\"" + chEntry + "\"";    //ע��!preEntry�������"��\,��˲���Decode
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
	if(bi[index] == '\"')	//�����"��ʼ
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
			else if(bi[index] == '\\')	//���ܻ��ڴ�ת��,֧��\\ת\,\"ת\,\����ֱ����\ 
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
	else	//������һ�汾,֧�ֵ��ַ�,��ʱ����Ҫ����
	{
		if(bi[index] == '\\')	//���ܻ��ڴ�ת��,֧��\\ת\,\"ת\,\����ֱ����\ 
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
void LR1::Skip(BufferIn& szBuf , int &index)		//�����ո�,�˸�,����
{
	while(szBuf[index] == ' ' || szBuf[index] == '\n' || szBuf[index] == '\r' || szBuf[index] == '\t')index++;
}
bool LR1::AddToRsTable(String ch)	//��ch��ӵ�ȫ��(��)�ս������
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
			if(vt.get(i).ch.length() == 1)		//���ݵ�1.0,������ΪҪ��ֹ���ַǷ��ַ�(����*\�ŵ�),��Ҫ�����"\?"����ʽ
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
	str.replace("\\" , "\\\\");		//˳���ܵߵ�,�������"��˵,���滻"����\",Ȼ���ٱ��\\",���Դ���
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
void LR1::FillState()		//���״̬ת��ͼDFA
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
void LR1::FillFirstSelect()	//���First��,ע��,�ķ����������e,���մ�,����û�пմ��Ĵ������,
{
	int i , j;
	String strKey , strTmp;
	for(i = 0 ; i < nt.size(); i++)							//���ѭ��,����ÿһ�����ս��,�����ս����First��,ע��,
		//���ｫ���ս�����ս��һ����(S->A,��AҲ�����б�),������㷨������������
	{
		for(j = 0 ; j < sihead.size(); j++)			//�ڲ�ѭ��,����ÿ���ķ�,(���ڸ������㷨,����Ч�ʲ���)
		{
			int k = 0;
			GetNextKeyString(strKey , sihead.get(j).szWenFa , k);		//����д��ʱ�Ѿ��ϸ��ʽ��,��˲��᷵��false
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
	for(i = 0 ; i < nt.size() ; i++)				//����ÿһ�����ս��,������е��ս��
	{
		//pst = pv->rsfhead.next;
		for(j = 0 ; j < nt.get(i).rsfhead.size(); j++)
		{
			if(isUpLetter(nt.get(i).rsfhead.get(j)[0]))		//���ս�����ս��������һ����ĸ�ж�
			{
				RSTable rst = GetFirst(nt.get(i).rsfhead.get(j));
				nt.get(i).rsfhead = nt.get(i).rsfhead.or(rst);
				nt.get(i).rsfhead.del(nt.get(i).ch);
			}
		}						
		//ע��!���ϵļ��벢δ�޳����ս��,��������ֻ������,û�м���
		//������Ϊ,��������޳��˷��ս��,����A->B B->A �ͻ���ѭ��
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
void LR1::FillFollowSelect()	//���Follow��
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
			String strFzjf , strTmp;	//strFzjf ��¼���ķ��ĵ�һ�����ս�� S->A ��strFzjf = S
			int k = 0;
			GetNextKeyString(strFzjf , sihead.get(j).szWenFa , k);
			k += 2;		//����->
			while(GetNextKeyString(strTmp , sihead.get(j).szWenFa , k))
			{
				if(nt.get(i).ch == strTmp)
				{
					if(sihead.get(j).szWenFa[k] == '\0')		//�Ѿ��������һ���ַ�
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
	}							//����Ϊ��ȡ���ս����Follow��,�����㷨����������,ע��!���ﲻ�漰e(�մ�),������First����Follow��
	//�����㷨����First�Ĵ������
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

void LR1::FillLR1Table()//��ʼ��LR(1)������
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
void LR1::AddToLR1Table(LR1Table &lr1head,String szGoto,int iCode)	//��һ���Ѿ�ȷ��Ҫ��ӵ���,������Ӳ���
{
	for(int i = 0 ; i < lr1head.size() ; i++)
	{
		if(lr1head.get(i).iCode == iCode)
		{
			if(!(lr1head.get(i).szChange == szGoto))
			{
				lastError = 10; //��ʾ�ķ����ܲ���LR1�ķ�
				throw "���� LR1 �ķ�!";
			}
			return ;
		}
	}
	lr1head.add(LR1Table_(iCode,szGoto));
	return ;
}
void LR1::AddToLR1Table(String ch , String szGoto,int iCode)	//iCode��ʾ�ڼ���״̬(�ڼ���) szGoto��ʾ����ת�� ch��ʾ��һ��(��)�ս��,�ɴ�ȷ��Ҫ��ӵ�����
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
int LR1::GetAtPos(String strToFind)		//���ڷ�@�ķ�,����ѭ��
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
int LR1::GetNextStateNum(String ch , NextState &ns)	//�������� (��)�ս�� ��ȡ��ǰ״̬�ɴ�(��)�ս��ָ�����һ��״̬
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
int LR1::GetWenFaNum(String szWenFa)	//��ȡ�����ķ���ID
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
bool LR1::isStringEqual(String si,String di)//�ж������ķ��Ƿ�һ��,�����ʡ��@���ж�
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
	j += 2;		//����->
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
bool LR1::AddWenFa(SIStr& di , String szWenFa , RSTable rst)	//���ķ���ӵ�di����
{
	if(isWenFaExist(di,szWenFa,rst,1))
	{
		return false;
	}
	for(int i = 0 ; i < di.size() ; i++)
	{
		if(di.get(i).szWenFa == szWenFa)
		{
			di.get(i).rssHead = di.get(i).rssHead.or(rst);		//��������ķ�������ͬ,��ϲ�չ���ַ���
			//���,ͬһ��״̬��,���ҽ�����һ���������ķ�,��ʱ���Է���
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
	//di.get(di.size() - 1).rssHead = di.get(di.size() - 1).rssHead.or(rst);//ԭ����CopyRSTable
	return true;
}
bool LR1::isWenFaExist(SIStr& si , String szWenfa , RSTable rst, int isNeedEqual)	//�ж��ķ��Ƿ����si����,������������LR(1)��������ʹ��
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
//  			//�����ķ��ײ���ͬ,β����ͬ,��������LALR(1)��LR(1)
// 				//������Ժϲ������ķ�Ϊһ��,��LALR(1)�ķ���
// 	 			}
			}
			else
			{
				if(rst == si.get(i).rssHead.and(rst))//�������Ҫ�ж����,����ֻ��Ҫ�µ��ķ���rst�����������е�rst���������
				{
					return true;
				}
			}	
		}
	}
	return false;
}
void LR1::CompleteState(CurState_& si)	//��si״̬���ķ��������
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
// 			j += 2;				//������ͷ��->
// 			while(si.sihead.get(i).szWenFa[j] != '@')		//���ڱض�����@,���ﲻ����ѭ��
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
bool LR1::AddSWenFa(SIStr& si , String ch , int index , RSTable rst)	//��ȫ���ķ������ҵ���ch��ͷ���ķ�����ӵ�si����,û������򷵻�0,���򷵻�1
{
	bool isAdd = false;
	String strTmp , strTmp1;
	if(!GetNextKeyString(strTmp , ch , index) || !isUpLetter(strTmp[0]))
	{
		return isAdd;
	}
	bool isWenFaNotEnd = GetNextKeyString(strTmp1 , ch , index );		//ע��,���ڵ���AddSWenFa֮ǰ,�Ѿ���@ʶ�������,��@��index֮ǰ,��������ÿ���@
//	SISTR *ps = sihead.next;
	for(int i= 0 ; i < sihead.size() ; i ++)
	{
		String psF ;
		int iTmp = 0;
		GetNextKeyString(psF , sihead.get(i).szWenFa , iTmp);
		if(psF == strTmp)			//�ҵ�����@S �� ��S->?? ��ʼ���ķ�
		{
			RSTable rsTmp;
			if(isWenFaNotEnd)				//һ����Ҫ�������Ҫ������ķ���չ���ַ���
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
void LR1::MakeAtWenFa(String& siWenFa , String& diWenFa)	//����ͨ�ķ�ת��Ϊ@�ķ�
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
bool LR1::MakeNextState(CurState_& si , CurState_& di , String Next)	//�ɵ�ǰ״̬si��Nextת���������һ��״̬,������di��
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
bool LR1::isStateExist(CurState_& state)		//�ж�״̬�Ƿ��Ѿ�����,�����򷵻�1�����iCodeΪ��״̬�ı��,���򷵻�0
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
bool LR1::isWenFaEqual(SIStr& si , SIStr& di)	//�ж������ķ��Ƿ����(˳�򲻼�)
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
			//ת���ַ�
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
			//����ͨ���
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


			//�����ͬ�ַ�ƥ��
			//*'abc'([a-z])		//����������
			//*12-34[]('')//12��34�������...//����û���յ�,����*1-'1'��ʾ������һ��1
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
			//��ʾ������β,����$����������һ���ַ�,���Ҹ��ַ�������ȫƥ��(���ڹؼ��ֺͱ���������)
		}
// 		else if(strZhongjiefu[i] == '?')
// 		{
// 			i++;
// 			strOut += bf[index];
// 			index ++;
// 			//�����ַ�
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

void LR1::scaner()					//�ʷ�������
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
		if(szToRec[ScanerIndex] == ' ' || szToRec[ScanerIndex] == '\n' || szToRec[ScanerIndex] == '\t' || szToRec[ScanerIndex] == '\r') //�����ո�,����,�˸�
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

String LR1::GetAction(int State , int iCode)	//��action���ȡת��״̬,State��ʾ��,iCode��ʾ�ս����Ӧ��ID
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

int LR1::GetGoto(int State , String& ch)	//��ȡת�����Ӧ��ת��״̬,State��ʾ��,ch��ʾ���ս��
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

String LR1::GetWenFaStr(int ID)		//��ȡָ��ID���ķ�
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
							throw "�ս��" + vt.get(i).ch + "δ�ҵ�������!";
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
					lastError = 103;		//����������ֻ�ȡ�����ķ������
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
//				pas1 = pas = pas->next;		//������ǰ����$����,���ջ���������(���絽����ǰ����ȡprev)
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
							throw "�ķ�:" + sihead.get(j).szWenFa + " δ�ҵ�������!";
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
	throw "ע�ᴦ����ʧ��!�Ҳ����ķ�!";
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
	throw "ע�ᴦ����ʧ��!�Ҳ����ķ�!";
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
		throw "ע�ᴦ����ʧ��!�Ҳ����ս��!";
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