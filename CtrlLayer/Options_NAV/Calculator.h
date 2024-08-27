#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>

//������������Ȩ�ȼ�
enum PRIO_LV 
{
	PRIO_LV0 = 0,
	PRIO_LV1 = 1,
	PRIO_LV2 = 2,
	PRIO_LV3 = 3,
	PRIO_LV4 = 4,
};


class Calculator
{
public:
	Calculator();
	void getFormat();					//���ʽ�Զ����׼��ʽ��
	int getPrior(char c);				//��ȡ�����������ȼ�
	void getPostfix();					//��׺���ʽת��
	void calResult();					//������
	void calculate();					//���㷽��
	double getResult();					//��ȡ���

	std::string operatorSym;					//�������
	std::string infix;						//���ʽ����

private:
	std::vector<std::string> postfix;				//��׺���ʽ����
	std::stack<char> symStack;				//����ջ
	std::stack<double> figStack;				//����ջ
	std::string stdInfix;					//�Զ����׼��ʽ�����ʽ
	double result;						//���ռ�����


};