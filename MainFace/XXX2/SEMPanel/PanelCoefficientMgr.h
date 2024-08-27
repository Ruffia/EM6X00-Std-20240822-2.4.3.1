#pragma once
#include <string>
#include <map>
using namespace std;

//ÿһ��ܶ���һ������ϵ��
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



//�ֿغе���ϵ��������
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
	float  m_fLargeFocusMode;  //�ֵ�ģʽ�µĵ���ֵ
	float  m_fSmallFocusMode;  //ϸ��ģʽ�µĵ���ֵ
};