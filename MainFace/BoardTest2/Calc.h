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

struct COUNTERpar//Ԫ��
{
	char kind;//Ԫ������:'N'Ϊ���� 'O'Ϊ����(+-* /^!) 'F'Ϊ����(sin asin ln.....) ����������
	double num;//����ֵ
	//	double GC;//ע�͸��Ŵ���
	char oper;//�����
	char *func;//������
	COUNTERpar *last;//��һ��
	COUNTERpar *next;//��һ��
};

struct COUNTER_RESULT//�������
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
