#pragma once
#include <string>
using namespace std;

enum AlarmType
{
	Alarm_None = 0,
	Alarm_VAC,          //��ձ���
	Alarm_LowMag,       //�ͱ�����
	Alarm_FilaBroken,   //��˿�Ͽ�
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
	__int64    m_i64AppStartTime;     //��������ʱ��(����ʱ���)
	bool       m_bVacNormal;          //����Ƿ�����
	double     m_dCCGLast;            //��һ�ε�CCGֵ
	bool       m_bCCGInit;            //CCG��ֵ�Ƿ񱻳�ʼ����ֻ��ֵһ��
};