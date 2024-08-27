#include "StdAfx.h"
#include "Calculator.h"

using namespace std;
//��ȡ�����������ȼ�
int Calculator::getPrior(char c)
{
	if (c == '+' || c == '-')
	{
		return PRIO_LV1;
	}
	else if (c == '*' || c == '/')
	{
		return PRIO_LV2;
	}
	else
	{
		return PRIO_LV0;
	}
}

Calculator::Calculator()
{
	result = 0.0;
}

//���ʽ�Զ����׼��ʽ��
void Calculator::getFormat() 
{
	stdInfix = infix;
	//ʵ��������
	//string.size()����size_type���ͣ������±�����ʱ���������
	for (size_t i = 0; i < stdInfix.size(); i++) 
	{			
		//-xת��Ϊ0-x��+xת��Ϊ0+x
		if (stdInfix[i] == '-' || stdInfix[i] == '+') 
		{		
			if (i == 0) 
			{
				stdInfix.insert(0, 1, '0');
			}
			else if (stdInfix[i - 1] == '(') 
			{
				stdInfix.insert(i, 1, '0');
			}
		}
	}
}



//��׺���ʽת��
void Calculator::getPostfix() 
{
	string tmp;
	//string.size()����size_type���ͣ������±�����ʱ���������
	for (size_t i = 0; i < stdInfix.size(); i++) 
	{					
		tmp = "";
		switch (stdInfix[i]) 
		{
		case '+':
		case '-':
		case '*':
		case '/':	
			if (symStack.empty() || symStack.top() == '(' ) 
			{
				symStack.push(stdInfix[i]);
			}
			else 
			{
				while (!symStack.empty() && (getPrior(symStack.top()) >= getPrior(stdInfix[i]))) 
				{
					tmp += symStack.top();
					postfix.push_back(tmp);
					symStack.pop();
					tmp = "";
				}
				symStack.push(stdInfix[i]);
			}
			break;
		case '(':
			symStack.push(stdInfix[i]);
			break;
		case ')':
			while (!symStack.empty() && symStack.top() != '(') 
			{
				tmp += symStack.top();
				postfix.push_back(tmp);
				symStack.pop();
				tmp = "";
			}
			if (!symStack.empty() && symStack.top() == '(') 
			{
				symStack.pop();							//�������ų�ջ����
			}
			break;
		default:
			if ((stdInfix[i] >= '0' && stdInfix[i] <= '9')) 
			{
				tmp += stdInfix[i];
				while (i + 1 < stdInfix.length() && (stdInfix[i + 1] >= '0' && stdInfix[i + 1] <= '9' || stdInfix[i + 1] == '.')) 
				{		//С������

					tmp += stdInfix[i + 1];			//�����������֣���׷��
					i++;
				}
				if (tmp[tmp.length() - 1] == '.') 
				{
					tmp += '0';						//��x.��x.0����
				}
				postfix.push_back(tmp);
			}
			break;
		}//end switch
	}//end for

	while (!symStack.empty()) 
	{						//��ջ��ʣ����ż����׺���ʽ
		tmp = "";
		tmp += symStack.top();
		postfix.push_back(tmp);
		symStack.pop();
	}
}

	
//����
void Calculator::calResult() 
{

	string tmp;
	double number = 0;
	double op1 = 0, op2 = 0;

	for (int i = 0; i < postfix.size(); i++) 
	{
		tmp = postfix[i];
		if (tmp[0] >= '0' && tmp[0] <= '9') 
		{
			number = atof(tmp.c_str());
			figStack.push(number);
		}
		else if (postfix[i] == "+") 
		{
			if (!figStack.empty()) 
			{
				op2 = figStack.top();
				figStack.pop();
			}
			if (!figStack.empty()) 
			{
				op1 = figStack.top();
				figStack.pop();
			}
			figStack.push(op1 + op2);
		}
		else if (postfix[i] == "-") 
		{
			if (!figStack.empty()) 
			{
				op2 = figStack.top();
				figStack.pop();
			}
			if (!figStack.empty()) 
			{
				op1 = figStack.top();
				figStack.pop();
			}
			figStack.push(op1 - op2);
		}
		else if (postfix[i] == "*") 
		{
			if (!figStack.empty()) 
			{
				op2 = figStack.top();
				figStack.pop();
			}
			if (!figStack.empty()) 
			{
				op1 = figStack.top();
				figStack.pop();
			}
			figStack.push(op1 * op2);
		}
		else if (postfix[i] == "/") 
		{
			if (!figStack.empty()) 
			{
				op2 = figStack.top();
				figStack.pop();
			}
			if (!figStack.empty()) 
			{
				op1 = figStack.top();
				figStack.pop();
			}
			if (op2 != 0) 
			{
				///������Ϊ0��δ������Ĭ��
			}
			figStack.push(op1 / op2);
		}
	}//end for
	if (!figStack.empty()) 
	{
		result = figStack.top();
	}
}

void Calculator::calculate()
{
	getFormat();				//���ʽ�Զ����׼��ʽ��
	getPostfix();				//��׺���ʽת��
	calResult();				//��ȡ�������
}

double Calculator::getResult()
{
	return result;
}
