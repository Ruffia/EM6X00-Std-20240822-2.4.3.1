#pragma once
#include "USB_Functions.h"
#include "PMSKV.h"

//�޸�ʱ�䣺  2020-07-20
//�޸��ˣ�  �ܺ�
//�޸�ԭ���ع�����ȫ�ֲ���������CXXX2App �ж���������
//          ����CXXX2App �ĸ��Ӷ�
//���ܣ� ȫ�ֲ���ģ��

class AFX_EXT_CLASS CGlobalParamSetting 
{
public:
	static CGlobalParamSetting& Instance();

	void  ResetCPS3603Param();
	void  ResetSpellmanDebugParam();

protected:
	CGlobalParamSetting();

public:
	SMvac_Param				g_VacParamMain;
	SMvac_TCAmp				g_VacTCAmp;
	CfgVAC_IP_CCG			g_cfgVacCCG;

	CPS3603_Param			g_CPS3603;
	DebugParam_Spellman		g_Spellmans;
	Status_WSM30KV2			g_WSM30KV2status;
	Setting_WSM30KV			g_setWSM30KV;
	Setting_WSM30KV2		g_WSM30KV2set;
	Mon_WSM30KV2			g_WSM30KV2mon;

	double	g_dHV;
	double  g_dHV2;
	double  g_dHV3;	// �ֱ��¼��ѹ��ƫѹ��771��������ֵ������˿���ȵ���������ֵ

	double	g_dfMCBRatio;
	double	g_dfMobileUScaleParameter;
	CString g_csUScaleUnit;

	double	g_dfMag;
	double	g_dblHV;

	WORD	m_wLanguageID;
};

