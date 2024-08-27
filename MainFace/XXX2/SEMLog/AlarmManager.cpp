#include "StdAfx.h"
#include "AlarmManager.h"
#include "FieldMeta.h"
#include "DBDataManager.h"
#include "FormatConvertor.h"

#define CCGThreshold 5.0E-5   //CCG 报警阈值

CAlarmManager::CAlarmManager()
{
	m_i64AppStartTime = 0;
	m_bVacNormal = false; 
	m_dCCGLast = 0.0;
	m_bCCGInit = false;
}


CAlarmManager& CAlarmManager::Instance()
{
	static CAlarmManager Inst;
	return Inst;
}

void CAlarmManager::CheckVAC(__int64 _i64CurTime,double dCCG)
{
	if(!m_bCCGInit)
	{
		m_dCCGLast = dCCG;
		m_bCCGInit = true;
		return;
	}

	//CCG 变好，达到看像条件
	if (m_dCCGLast > CCGThreshold && dCCG <= CCGThreshold)
	{
		m_bVacNormal = true;
		m_dCCGLast = dCCG;
		return;
	}

	//达到看像条件下，CCG 变坏，记录报警
	if(m_dCCGLast <= CCGThreshold && dCCG > CCGThreshold && m_bVacNormal)
	{
		m_bVacNormal = false;
		m_dCCGLast = dCCG;

		char szAlarmContent[128] = {0};
		sprintf_s(szAlarmContent,128,"CCG > %.2e",dCCG);
		AddAlarm(AlarmType::Alarm_VAC,_i64CurTime,string(szAlarmContent));
	}
}


void CAlarmManager::AddAlarm(AlarmType eType,__int64 _i64CurTime, const string& sContent)
{
	if(eType != Alarm_VAC) return;

	string sTableName = "AlarmLog";
	string sSQL = "insert into ";	
	sSQL += sTableName;
	sSQL += "(";

	int i = 0;
	map<string,CFieldDesc*>& meta = CDBDataManager::Instance().GetTableMeta(sTableName);

	vector<pair<string,CFieldDesc*>> metaV(meta.begin(),meta.end());//用map对该容器初始化
	sort(metaV.begin(),metaV.end(),CompareBySequenceID);//用sort排序

	::sort(metaV.begin(), metaV.end(), CompareBySequenceID); 

	for (vector<pair<string,CFieldDesc*>>::iterator it = metaV.begin(); it != metaV.end();it++)
	{
		CFieldDesc* pFieldDesc = it->second;
		if(!pFieldDesc) continue;

		if(i != metaV.size() - 1)
		{
			sSQL += pFieldDesc->m_strFieldID;
			sSQL += ",";
		}
		else
		{
			sSQL += pFieldDesc->m_strFieldID;
		}
		i++;
	}
	sSQL += ")";
	sSQL += " values(";

	string sFormat = "";
	CFormatConvertor FC;
	i = 0;
	for (vector<pair<string,CFieldDesc*>>::iterator it = metaV.begin(); it != metaV.end();it++)
	{
		CFieldDesc* pFieldDesc = it->second;
		if(!pFieldDesc) continue;
		if(i != metaV.size() - 1)
		{
			sFormat += FC.GetFormat(pFieldDesc->m_strDataType);
			sFormat += ",";
		}
		else
		{
			sFormat += FC.GetFormat(pFieldDesc->m_strDataType);
		}
		i++;
	}

	char szValue[512] = {0};
	sprintf_s(szValue,256,sFormat.c_str(),_i64CurTime, m_i64AppStartTime,(int)eType, sContent.c_str());
	sSQL += szValue;

	sSQL += ")";
	CDBDataManager::Instance().Exec(sSQL);
}