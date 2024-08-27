#include "StdAfx.h"
#include "RealTimeDataManager.h"

CRealTimeLogData::CRealTimeLogData(__int64 _i64CurTime,double dCCG,double dHV,double dFila,double dEMI):
	m_i64Time(_i64CurTime),m_dCCG(dCCG),m_dHV(dHV),m_dFila(dFila),m_dEMI(dEMI)
{

}
	
CRealTimeLogData::CRealTimeLogData():
	m_i64Time(0),m_dCCG(0.0),m_dHV(0.f),m_dFila(0.f),m_dEMI(0.f)
{

}

CRealTimeDataManager::CRealTimeDataManager()
{
	m_i64AppStartTime = 0;
	m_mapTimeSecond2Data.clear();
}


CRealTimeDataManager& CRealTimeDataManager::Instance()
{
	static CRealTimeDataManager Inst;
	return Inst;
}

void CRealTimeDataManager::AddData(__int64 _i64CurTime,double dCCG,double dHV,double dFila,double dEMI)
{
	m_mapTimeSecond2Data.insert(make_pair(_i64CurTime,CRealTimeLogData(_i64CurTime,dCCG,dHV,dFila,dEMI)));
}

void CRealTimeDataManager::GetData(__int64 _i64BeginTime,__int64 _64EndTime,map<__int64,CRealTimeLogData>& mapTimeSecond2Data)
{
	mapTimeSecond2Data.clear();
	for (map<__int64,CRealTimeLogData>::iterator it = m_mapTimeSecond2Data.begin();
		 it != m_mapTimeSecond2Data.end(); it++)
	{
		__int64 _i64Time = it->first * 1000;
		if (_i64BeginTime <= _i64Time && _i64Time <= _64EndTime)
		{
			mapTimeSecond2Data.insert(make_pair(_i64Time,it->second));
		}
	}	
}