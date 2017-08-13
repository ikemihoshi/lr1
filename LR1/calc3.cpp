// #include"lr1.h"
// //char szT[1024] = "S->T T->U T->T+U T->T-U U->V U->U|V V->W V->V&W W->X W->W*X W->W/X W->W%X X->Y X->X^Y Y->Z Y->~Z Z->i Z->+i Z->-i Z->(T) Z->f(T) Z->g(T,T) i=10 f=11 g=22";
// char szT[1024] = "S->T T->U T->T+U T->T-U U->V U->U|V V->W V->V&W W->X W->W*X W->W/X W->W%X X->Y X->X^Y Y->Z Y->~Z Z->i Z->+i Z->-i Z->(T) Z->f(T) Z->g(T,T) \"i\":\"*\"";
// // SPECIAL_KEY spKey[]=			
// // {
// // 	{"",0},
// // 	{"lg",11},
// // 	{"ln",11},
// // 	{"arcsin",11},
// // 	{"arccos",11},
// // 	{"arctan",11},
// // 	{"sqrt",11},
// // 	{"sin",11},
// // 	{"cos",11},
// // 	{"tan",11},
// // 	{"cot",11},
// // 	{"sinh",11},
// // 	{"cosh",11},
// // 	{"tanh",11},
// // 	{"abs",11},
// // 	{"exp",11},
// // 	{"not",11},
// // 	{"log",22},
// // 	{"mod",22},
// // 	{"pow",22},
// // 	{"max",22},
// // 	{"min",22},
// // 	{"and",22},
// // 	{"or",22},
// // 	{"xor",22},
// // 	{"",0}
// // };
// void OnDealRuleYunSuan(PSTACKNODE *pS,int iCode , const char*szWenFa)//D->D(+-*/%)X
// {
// 	if(szWenFa[4] == '+')
// 	{
// 		pS[0]->szName += pS[2]->szName;
// 	}
// 	else if(szWenFa[4] == '-')
// 	{
// 		pS[0]->szName -= pS[2]->szName;
// 	}
// 	else if(szWenFa[4] == '*')
// 	{
// 		pS[0]->szName *= pS[2]->szName;
// 	}
// 	else if(szWenFa[4] == '/')
// 	{
// 		pS[0]->szName /= pS[2]->szName;
// 	}
// 	else if(szWenFa[4] == '%')
// 	{
// 		pS[0]->szName = fmod(pS[0]->szName , pS[2]->szName);
// 	} 
// 	else if(szWenFa[4] == '^')
// 	{
// 		pS[0]->szName = pow(pS[0]->szName , pS[2]->szName);
// 	}
// 	else if(szWenFa[4] == '&')
// 	{
// 		pS[0]->szName = (int)(pS[0]->szName) & (int)(pS[2]->szName);
// 	}
// 	else if(szWenFa[4] == '|')
// 	{
// 		pS[0]->szName = (int)(pS[0]->szName) | (int)(pS[2]->szName);
// 	}
// 	delete pS[1];
// 	delete pS[2];
// 	pS[1] = pS[2] = NULL;
// //	puts(szWenFa);
// }
// void OnDealRuleKuoHao(PSTACKNODE *pS,int iCode , const char*szWenFa)//Y->(D)
// {
// 	delete pS[0];
// 	delete pS[2];
// 	pS[0] = pS[1];
// 	pS[1] = pS[2] = NULL;
// //	puts(szWenFa);
// }
// double dbV = 0;
// void OnDealRuleMain(PSTACKNODE *pS,int iCode , const char*szWenFa)//S->X
// {
// 	dbV = pS[0]->szName;
// //	puts(szWenFa);
// }
// void OnDealRuleFeiYunSuan(PSTACKNODE *pS,int iCode , const char*szWenFa)//Y->~Z
// {
// 	if(szWenFa[3] == '~')
// 	{
// 		pS[1]->szName = ~(int)(pS[1]->szName);
// 	}
// 	else if(szWenFa[3] == '-')
// 	{
// 		pS[1]->szName *= -1;
// 	}
// 	delete pS[0];
// 	pS[0] = pS[1];
// 	pS[1] = NULL;
// }
// void OnDealRuleFunc1(PSTACKNODE *pS,int iCode , const char*szWenFa)//Z->f(T)
// {
// 	if(!strcmp(pS[0]->szName,"lg"))
// 	{
// 		pS[0]->szName = log10(pS[2]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"ln"))
// 	{
// 		pS[0]->szName = log(pS[2]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"arcsin"))
// 	{
// 		pS[0]->szName = asin(pS[2]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"arccos"))
// 	{
// 		pS[0]->szName = acos(pS[2]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"arctan"))
// 	{
// 		pS[0]->szName = atan(pS[2]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"sqrt"))
// 	{
// 		pS[0]->szName = sqrt(pS[2]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"sin"))
// 	{
// 		pS[0]->szName = sin(pS[2]->szName);
// 	}else if(!strcmp(pS[0]->szName,"cos"))
// 	{
// 		pS[0]->szName = cos(pS[2]->szName);
// 	}else if(!strcmp(pS[0]->szName,"tan"))
// 	{
// 		pS[0]->szName = tan(pS[2]->szName);
// 	}else if(!strcmp(pS[0]->szName,"cot"))
// 	{
// 		pS[0]->szName = 1.0 / tan(pS[2]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"sinh"))
// 	{
// 		pS[0]->szName = sinh(pS[2]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"cosh"))
// 	{
// 		pS[0]->szName = cosh(pS[2]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"tanh"))
// 	{
// 		pS[0]->szName = tanh(pS[2]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"abs"))
// 	{
// 		pS[0]->szName = fabs(pS[2]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"exp"))
// 	{
// 		pS[0]->szName = exp(pS[2]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"not"))
// 	{
// 		pS[0]->szName = ~(__int64)pS[2]->szName;
// 	}
// 	return ;
// }
// void OnDealRuleFunc2(PSTACKNODE *pS,int iCode , const char*szWenFa)//Z->f(T,T)
// {
// 	if(!strcmp(pS[0]->szName,"log"))
// 	{
// 		pS[0]->szName = log10(pS[4]->szName) / log10(pS[2]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"mod"))
// 	{
// 		pS[0]->szName = fmod(pS[2]->szName,pS[4]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"pow"))
// 	{
// 		pS[0]->szName = pow(pS[2]->szName,pS[4]->szName);
// 	}
// 	else if(!strcmp(pS[0]->szName,"max"))
// 	{
// 		pS[0]->szName = pS[2]->szName > pS[4]->szName ? pS[2]->szName : pS[4]->szName;
// 	}
// 	else if(!strcmp(pS[0]->szName,"min"))
// 	{
// 		pS[0]->szName = pS[2]->szName < pS[4]->szName ? pS[2]->szName : pS[4]->szName;
// 	}
// 	else if(!strcmp(pS[0]->szName,"and"))
// 	{
// 		pS[0]->szName = (__int64)pS[2]->szName & (__int64)pS[4]->szName;
// 	}
// 	else if(!strcmp(pS[0]->szName,"or"))
// 	{
// 		pS[0]->szName = (__int64)pS[2]->szName | (__int64)pS[4]->szName;
// 	}
// 	else if(!strcmp(pS[0]->szName,"xor"))
// 	{
// 		pS[0]->szName = (__int64)pS[2]->szName ^ (__int64)pS[4]->szName;
// 	}
// 	return ;
// }
// void Init(LR1 &lr1)
// {
// 	lr1.OutputLR1Table();
// 	lr1.SetDefaultScaner(LR1::C_Scaner_Type);
// 	lr1.InitTokenValue(spKey);
// 	lr1.SetNumValue(10);
// 	lr1.RegisterDealRuleFunc("T->T+U",OnDealRuleYunSuan);
// 	lr1.RegisterDealRuleFunc("T->T-U",OnDealRuleYunSuan);
// 	lr1.RegisterDealRuleFunc("U->U|V",OnDealRuleYunSuan);
// 	lr1.RegisterDealRuleFunc("V->V&W",OnDealRuleYunSuan);
// 	lr1.RegisterDealRuleFunc("W->W*X",OnDealRuleYunSuan);
// 	lr1.RegisterDealRuleFunc("W->W/X",OnDealRuleYunSuan);
// 	lr1.RegisterDealRuleFunc("W->W%X",OnDealRuleYunSuan);
// 	lr1.RegisterDealRuleFunc("X->X^Y",OnDealRuleYunSuan);
// 	lr1.RegisterDealRuleFunc("Y->~Z", OnDealRuleFeiYunSuan);
// 	lr1.RegisterDealRuleFunc("Z->-i", OnDealRuleFeiYunSuan);
// 	lr1.RegisterDealRuleFunc("Z->+i", OnDealRuleFeiYunSuan);
// 	lr1.RegisterDealRuleFunc("Z->(T)",OnDealRuleKuoHao);
// 	lr1.RegisterDealRuleFunc("S->T",OnDealRuleMain);
// 	lr1.RegisterDealRuleFunc("Z->f(T)",OnDealRuleFunc1);
// 	lr1.RegisterDealRuleFunc("Z->g(T,T)",OnDealRuleFunc2);
// 	return ;
// }
// int main()
// {
// 	char szTmp[4096];
// 	LR1 lr1(szT);
// 	Init(lr1);
// 	while(1)
// 	{
// 		gets(szTmp);
// 		if(!strcmp(szTmp,"exit"))
// 		{
// 			break;
// 		}
// 		if(!lr1.Recognize(szTmp))
// 		{
// 			printf("=%.13g\n\n",dbV);
// 		}
// 		else
// 		{
// 			printf(" ‰»Î”–ŒÛ!¥ÌŒÛ¬Î %d\n" , lr1.GetLastError());
// 		}
// 	}
// 	return 0;
// }