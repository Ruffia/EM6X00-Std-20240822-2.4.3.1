#pragma once
#include <string>
#include <map>
using namespace std;

//每一项功能都有一个调节系数
class FunctionCoefficient
{
public:
	FunctionCoefficient(int nID,string sName,float fValue);
	FunctionCoefficient();

public:
	int      m_nFunctionID;
	string   m_strName;
	float    m_fDivisor;
};



//手控盒调节系数管理器
class CPanelCoefficientMgr
{
protected:
	CPanelCoefficientMgr();
	~CPanelCoefficientMgr(){};

public:
	static CPanelCoefficientMgr& Instance();
	bool  LoadSettings();
	float GetDivisor(const int nFunctionCode);

	float GetLargeFocusMode() {return m_fLargeFocusMode;}
	float GetSmallFocusMode() {return m_fSmallFocusMode;}

private:
	map<int,FunctionCoefficient>  m_ID2Coefficient;
	float  m_fLargeFocusMode;  //粗调模式下的调试值
	float  m_fSmallFocusMode;  //细调模式下的调试值
};