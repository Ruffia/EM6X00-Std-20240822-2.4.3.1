#pragma once
#include "USB_Functions.h"
#include "PMSKV.h"

//修改时间：  2020-07-20
//修改人：  周宏
//修改原因：重构，将全局参数变量从CXXX2App 中独立出来，
//          减少CXXX2App 的复杂度
//功能： 全局参数模块

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
	double  g_dHV3;	// 分别记录高压、偏压（771束流设置值）、灯丝加热电流的设置值

	double	g_dfMCBRatio;
	double	g_dfMobileUScaleParameter;
	CString g_csUScaleUnit;

	double	g_dfMag;
	double	g_dblHV;

	WORD	m_wLanguageID;
};

