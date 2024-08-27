#include "stdafx.h"
#include "GlobalParamSetting.h"

CGlobalParamSetting& CGlobalParamSetting::Instance()
{
    static CGlobalParamSetting Inst;
	return Inst;
}

CGlobalParamSetting::CGlobalParamSetting()
{
	g_dHV = 0;
	g_dHV2 = 0;
	g_dHV3 = 0;	
	g_dfMCBRatio = 0.0;
	g_dfMobileUScaleParameter = -1;

	g_dfMag = 150;
	g_dblHV = 0;
}

void CGlobalParamSetting::ResetCPS3603Param()
{
	g_CPS3603.dVoutA		= 0.0;
	g_CPS3603.dIoutA		= 0.0;
	g_CPS3603.dCoronalevel	= 0.0;
	g_CPS3603.dIoutF		= 0.0;
	g_CPS3603.dVoutE		= 0.0;
}


void CGlobalParamSetting::ResetSpellmanDebugParam()
{
	g_Spellmans.pS.dVC		= 0.0;
	g_Spellmans.pS.dVE		= 0.0;
	g_Spellmans.pS.dIB		= 0.0;
	g_Spellmans.pS.dIH		= 0.0;
	g_Spellmans.pS.dVP		= 0.0;
	g_Spellmans.pM.dVC		= 0.0;
	g_Spellmans.pM.dVE		= 0.0;
	g_Spellmans.pM.dIB		= 0.0;
	g_Spellmans.pM.dIH		= 0.0;
	g_Spellmans.pM.dVP		= 0.0;
	g_Spellmans.pM.dII		= 0.0;
}