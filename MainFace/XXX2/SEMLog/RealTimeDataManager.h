#pragma once
#include <map>
#include <tuple>
using namespace std;

class CRealTimeLogData
{
public:
	CRealTimeLogData(__int64 _i64CurTime,double dCCG,double dHV,double dFila,double dEMI);
	CRealTimeLogData();

public:
	__int64  m_i64Time;
	double   m_dCCG;
	float    m_dHV;
	float    m_dFila;
	float    m_dEMI;
};



class CRealTimeDataManager
{
protected:
	CRealTimeDataManager();

public:
	static CRealTimeDataManager& Instance();

	void SetAppStartTime(__int64 i64Time)
	{
		m_i64AppStartTime = i64Time;
	}

	__int64 GetAppStartTime()
	{
		return m_i64AppStartTime;
	}

	void AddData(__int64 _i64CurTime,double dCCG,double dHV,double dFila,double dEMI);

	map<__int64,CRealTimeLogData>& GetData()
	{
		return m_mapTimeSecond2Data;
	}

	//��ȡĳ��ʱ�������ڵ�����
	void GetData(__int64 _i64BeginTime,__int64 _64EndTime,map<__int64,CRealTimeLogData>& mapTimeSecond2Data);

	//��ȡĳ��ʱ����Ժ����������
	vector<tuple<__int64,double,float,float,float>>& GetNextData(__int64 _i64Time);

public:
	__int64    m_i64AppStartTime;     //��������ʱ��(����ʱ���)

	//Second-->Data
	//��¼������������ʵʱ����
	map<__int64,CRealTimeLogData>  m_mapTimeSecond2Data;    
};