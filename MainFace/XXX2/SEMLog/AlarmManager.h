#pragma once
#include <string>
using namespace std;

enum AlarmType
{
	Alarm_None = 0,
	Alarm_VAC,          //真空报警
	Alarm_LowMag,       //低倍看像
	Alarm_FilaBroken,   //灯丝断开
};

class CAlarmManager
{
protected:
	CAlarmManager();

public:
	static CAlarmManager& Instance();

	void SetAppStartTime(__int64 i64Time)
	{
		m_i64AppStartTime = i64Time;
	}

	void AddAlarm(AlarmType eType,__int64 _i64CurTime,const string& sContent);
	void CheckVAC(__int64 _i64CurTime,double dCCG);

public:
	__int64    m_i64AppStartTime;     //程序启动时间(毫秒时间戳)
	bool       m_bVacNormal;          //真空是否正常
	double     m_dCCGLast;            //上一次的CCG值
	bool       m_bCCGInit;            //CCG的值是否被初始化，只赋值一次
};