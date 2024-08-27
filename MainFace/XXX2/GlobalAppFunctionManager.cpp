#include "stdafx.h"
#include "GlobalAppFunctionManager.h"
#include "GlobalParamSetting.h"
#include "MCICardCommonSetting.h"
#include "StageXYManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CGlobalFunctionManager& CGlobalFunctionManager::Instance()
{
	static CGlobalFunctionManager Inst;
	return Inst;
}

CGlobalFunctionManager::CGlobalFunctionManager()
{

}

void CGlobalFunctionManager::UpdateScreenMag(CSemVirtualMCICard m_ImageCard, double dMag, BOOL bSync )
{
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
		StageMC600_SetMag( dMag );

	double dMagScreen = dMag;
	if( theApp.m_sysOpt.nShowParams[SysOpt_showScrMag] > 0 )	// 是否显示屏幕放大倍数
	{
		// 屏幕放大倍数与设备放大倍数一致的概念是指12.7cm*8.9cm大小的图像
		if( m_ImageCard.GetPhotoPreview() )
			dMagScreen	= dMag;
		else
		{
			if( !bSync )
			{
				dMagScreen = dMag * CMCICardCommonSetting::Instance().m_sizeReso.cx /(127 * m_dblPixelsPerMM_HORZ);
				dMagScreen *= g_dScaleScanTable[g_nScaleScanIndex];
			}
		}
	}

	CString str;
	// Format Magnification Display string
	if( dMagScreen < 10. )
		str.Format( "%1.2f X", dMagScreen );
	else if( dMagScreen < 100. )
		str.Format( "%2.1f X", dMagScreen );
	else if( dMagScreen < 1000. )
		str.Format( "%3.0f X", dMagScreen );
	else if( dMagScreen < 10000. )
		str.Format( "%1.2f KX", dMagScreen / 1000 );
	else if( dMagScreen < 100000. )
		str.Format( "%2.1f KX", dMagScreen / 1000 );
	else //if( dMagScreen < 1000000. )
		str.Format( "%3.0f KX", dMagScreen / 1000 );
	SemLabel_UpdateMag(CMCICardCommonSetting::Instance().m_sizeReso.cx, dMag, str);
}