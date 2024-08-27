#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>

//算术符号优先权等级
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
	void getFormat();					//表达式自定义标准格式化
	int getPrior(char c);				//获取算术符号优先级
	void getPostfix();					//后缀表达式转换
	void calResult();					//计算结果
	void calculate();					//计算方法
	double getResult();					//获取结果

	std::string operatorSym;					//运算符号
	std::string infix;						//表达式缓存

private:
	std::vector<std::string> postfix;				//后缀表达式向量
	std::stack<char> symStack;				//符号栈
	std::stack<double> figStack;				//数字栈
	std::string stdInfix;					//自定义标准格式化表达式
	double result;						//最终计算结果


};