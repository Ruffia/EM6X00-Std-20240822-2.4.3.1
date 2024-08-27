// Calc.h

#if !defined(_CALC_INCLUDED_)
#define _CALC_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#define CHECK \
	parp=parF;while(parp->next!=NULL){cout<<parp->kind<<"\t";parp=parp->next;}cout<<parp->kind<<endl;\
	parp=parF;while(parp->next!=NULL){cout<<parp->num<<"\t";parp=parp->next;}cout<<parp->num<<endl; \
	parp=parF;while(parp->next!=NULL){cout<<parp->oper<<"\t";parp=parp->next;}cout<<parp->oper<<endl;\
parp=parF;while(parp->next!=NULL){cout<<parp->func<<"\t";parp=parp->next;}cout<<parp->func<<endl;//check

struct COUNTERpar//元素
{
	char kind;//元素种类:'N'为数字 'O'为运算(+-* /^!) 'F'为函数(sin asin ln.....) 和正反括号
	double num;//数字值
	//	double GC;//注释根号次数
	char oper;//运算符
	char *func;//函数名
	COUNTERpar *last;//上一个
	COUNTERpar *next;//下一个
};

struct COUNTER_RESULT//结果储存
{
	int order;
	double result;
	COUNTER_RESULT *next;
};
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////

double counter( char* cFunc );

/////////////////////////////////////////////////

#endif // _CALC_INCLUDED_
