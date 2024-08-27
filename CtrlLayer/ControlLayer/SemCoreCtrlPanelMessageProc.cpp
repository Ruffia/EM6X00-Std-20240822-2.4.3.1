#include "StdAfx.h"
#include "SemCoreCtrlPanelMessageProc.h"
using namespace SemCoreCtrlPanelMessageProcessor;
#include "DebugMessageDispatcher.h"
#include "LanguageManagerCtrl.h"
#include "AppLogManager.h"
#include "ControlLayer.h"
#include "SemCtrl.h"
#include "SemDataType.h"
#include "Factory.h"
#include "ScaleManager.h"
#include "SemCoreAssemble.h"

static const int IDF_SD_SemControlPanelClassID_Null = 44100;

IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,ImageReverse,SemControlPanelClassID,SemControlPanelClass_ImageReverse)
void ImageReverse::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;	
	if ( CAppLogManager::Instance().m_boolImageReverseLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_SignalProcesser.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,DFocus,SemControlPanelClassID,SemControlPanelClass_DFocus)
void DFocus::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;	
	if ( CAppLogManager::Instance().m_boolDFocusLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_DFocus.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,DFocusSwitch,SemControlPanelClassID,SemControlPanelClass_DFocusSwitch)
void DFocusSwitch::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if( bXPos )
		csX = pLanguageManagerCtrl->GetResString( IDS_STATE_OPEN );
	else
		csX = pLanguageManagerCtrl->GetResString( IDS_STATE_CLOSE );

	csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
	csText = csTitle + _T(" ") + csX;
	CDebugMessageDispatcher::Instance().DisplayMsg( csText );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,DObjectiveLens,SemControlPanelClassID,SemControlPanelClass_DObjectiveLens)
void DObjectiveLens::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolDObjectiveLensLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_DObjectiveLens.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,DObjectiveLensSwitch,SemControlPanelClassID,SemControlPanelClass_DObjectiveLensSwitch)
void DObjectiveLensSwitch::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolDObjectiveLensSwitchLog )
	{
		if( bXPos )
			csX = pLanguageManagerCtrl->GetResString( IDS_STATE_OPEN );
		else
			csX = pLanguageManagerCtrl->GetResString( IDS_STATE_CLOSE );

		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,RobinsonDetectorSwitch,SemControlPanelClassID,SemControlPanelClass_RobinsonDetectorSwitch)
void RobinsonDetectorSwitch::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolRobinsonDetectorSwitchLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_RobinsonDetectorSwitch.FormatText();

		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,Signal2Brightness,SemControlPanelClassID,SemControlPanelClass_Signal2Brightness)
void Signal2Brightness::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolSignal2BrightnessLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_Signal2Brightness.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,Signal2Contrast,SemControlPanelClassID,SemControlPanelClass_Signal2Contrast)
void Signal2Contrast::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolSignal2ContrastLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_Signal2Contrast.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,YAdjust,SemControlPanelClassID,SemControlPanelClass_YAdjust)
void YAdjust::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolYAdjustLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_YAdjust.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,ObjectiveLens,SemControlPanelClassID,SemControlPanelClass_ObjectiveLens)
void ObjectiveLens::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_ObjectiveLens.FormatText();
	if ( CAppLogManager::Instance().m_boolObjectiveLensLog )
	{
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}

	_PerformAddtionalTask();
}

void ObjectiveLens::_PerformAddtionalTask()
{
	//05.06.13保持放大倍数、工作距离、物镜电流的改变同步
	VARIANT varData = CSemCoreAssemble::Instance().m_Magnifier.GetPos();

	CSemVirtualMCICard::Instance().UpdateScreenMag( varData.dblVal );
	CControlLayer::Instance().SyncFineMag();
	CString csWD = CSemCoreAssemble::Instance().m_WD.FormatText();
	if ( !CSemVirtualMCICard::Instance().IsStopScan() )
		CSemVirtualMCICard::Instance().UpdateWD( csWD );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,WorkDistance,SemControlPanelClassID,SemControlPanelClass_WorkDistance)
void WorkDistance::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolWorkDistanceLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_WD.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}

	_PerformAddtionalTask();
}

void WorkDistance::_PerformAddtionalTask()
{
	//05.06.13保持放大倍数、工作距离、物镜电流的改变同步
	VARIANT varData = CSemCoreAssemble::Instance().m_Magnifier.GetPos();
	CSemVirtualMCICard::Instance().UpdateScreenMag( varData.dblVal );
	CControlLayer::Instance().SyncFineMag();
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,Magnifier,SemControlPanelClassID,SemControlPanelClass_Magnifier)
void Magnifier::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_Magnifier.FormatText();
	if ( CAppLogManager::Instance().m_boolMagnifierLog )
	{
		//csX = csY = CSemCoreAssemble::Instance().m_Magnifier.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}

	_PerformAddtionalTask();
}

void Magnifier::_PerformAddtionalTask()
{
	VARIANT varData = CSemCoreAssemble::Instance().m_Magnifier.GetPos();
	double dblVal = varData.dblVal;
	CControlLayer::Instance().MCB_Adjust_Ratio( dblVal );
	CSemVirtualMCICard::Instance().UpdateScreenMag( varData.dblVal );
	CControlLayer::Instance().SyncFineMag();
	CString csWD = CSemCoreAssemble::Instance().m_WD.FormatText();
	if ( !CSemVirtualMCICard::Instance().IsStopScan() )
		CSemVirtualMCICard::Instance().UpdateWD( csWD );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,CondenserLens,SemControlPanelClassID,SemControlPanelClass_CondenserLens)
void CondenserLens::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolCondenserLensLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_CondenserLens.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,CPS_FilamentCurrent,SemControlPanelClassID,SemControlPanelClass_CPS_FilamentCurrent)
void CPS_FilamentCurrent::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolFilamentCurrentLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_CPS_FilamentCurrent.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,GunCurrent,SemControlPanelClassID,SemControlPanelClass_GunCurrent)
void GunCurrent::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_GunCurrent.FormatText();
	if ( CAppLogManager::Instance().m_boolGunCurrentLog )
	{
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,ObjectiveLensReverse,SemControlPanelClassID,SemControlPanelClass_ObjectiveLensReverse)
void ObjectiveLensReverse::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolObjectiveLensReverseLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_ObjectiveLensReverse.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,CondenserLensReverse,SemControlPanelClassID,SemControlPanelClass_CondenserLensReverse)
void CondenserLensReverse::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolCondenserLensReverseLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_CondenserLensReverse.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,DCondenserLens,SemControlPanelClassID,SemControlPanelClass_DCondenserLens)
void DCondenserLens::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolDCondenserLensLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_DCondenserLens.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,ScanRotate,SemControlPanelClassID,SemControlPanelClass_ScanRotate)
void ScanRotate::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolScanRotateLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_ScanRotate.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,SACP,SemControlPanelClassID,SemControlPanelClass_SACP)
void SACP::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolSACPLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_SACP.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,CPS_HV,SemControlPanelClassID,SemControlPanelClass_CPS_HV)
void CPS_HV::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_CPS_HV.FormatText();
	if ( CAppLogManager::Instance().m_boolHVLog )
	{
		//csX = csY = CSemCoreAssemble::Instance().m_CPS_HV.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + SemControlPanelClass_HV), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}

	_PerformAddtionalTask();
}

void CPS_HV::_PerformAddtionalTask()
{
	CString csHV = CSemCoreAssemble::Instance().m_CPS_HV.FormatText();
	// 2005-1-17_0.0KV问题
	if ( !CSemVirtualMCICard::Instance().IsStopScan() )
		CSemVirtualMCICard::Instance().UpdateHV( csHV );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,SP_HV,SemControlPanelClassID,SemControlPanelClass_SP_HV)
void SP_HV::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_SP_HV.FormatText();
	if ( CAppLogManager::Instance().m_boolHVLog )
	{
		//csX = csY = CSemCoreAssemble::Instance().m_CPS_HV.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + SemControlPanelClass_HV), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}

	_PerformAddtionalTask();
}

void SP_HV::_PerformAddtionalTask()
{
	CString csHV = CSemCoreAssemble::Instance().m_SP_HV.FormatText();
	// 2005-1-17_0.0KV问题
	if ( !CSemVirtualMCICard::Instance().IsStopScan() )
		CSemVirtualMCICard::Instance().UpdateHV( csHV );
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,HV,SemControlPanelClassID,SemControlPanelClass_HV)
void HV::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	csX = csY = CSemCoreAssemble::Instance().m_HV.FormatText();
	if ( CAppLogManager::Instance().m_boolHVLog )
	{
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}

	_PerformAddtionalTask();
}


void HV::_PerformAddtionalTask()
{
	CString csHV = "";
	CString csX = CSemCoreAssemble::Instance().m_HV.FormatText();
	// 调试时为使底标不显示控制值需要做一些调整
	int nPos = csX.Find( '[' );
	if( nPos == -1 )
		csHV = csX;
	else
		csHV = csX.Left( nPos -1 );
	CControlLayer::Instance().Co_UpdateScan();

	// 2005-1-17_0.0KV问题
	if ( !CSemVirtualMCICard::Instance().IsStopScan() )
		CSemVirtualMCICard::Instance().UpdateHV( csHV );
}



IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,BeemAlignment,SemControlPanelClassID,SemControlPanelClass_BeemAlignment)
void BeemAlignment::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolBeemAlignmentLog )
	{
		csX = CSemCoreAssemble::Instance().m_BeemAlignmentX.FormatText();
		csY = CSemCoreAssemble::Instance().m_BeemAlignmentY.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX + _T(" ") + csY;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,ScanShift,SemControlPanelClassID,SemControlPanelClass_ScanShift)
void ScanShift::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolScanShiftLog )
	{
		csX = CSemCoreAssemble::Instance().m_ScanShiftX.FormatText();
		csY = CSemCoreAssemble::Instance().m_ScanShiftY.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX + _T(" ") + csY;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,Stigmator,SemControlPanelClassID,SemControlPanelClass_Stigmator)
void Stigmator::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolStigmatorLog )
	{
		csX = CSemCoreAssemble::Instance().m_StigmatorX.FormatText();
		csY = CSemCoreAssemble::Instance().m_StigmatorY.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX + _T(" ") + csY;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,AuxiliaryStigmation,SemControlPanelClassID,SemControlPanelClass_AuxiliaryStigmation)
void AuxiliaryStigmation::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolAuxiliaryStigmationLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_AuxiliaryStigmation.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,Brightness,SemControlPanelClassID,SemControlPanelClass_Brightness)
void Brightness::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolBrightnessLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_Brightness.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}

	_PerformAddtionalTask();
}

void Brightness::_PerformAddtionalTask()
{
	VARIANT varData = CSemCoreAssemble::Instance().m_Brightness.GetPos();
	int nPos = varData.lVal;
	if( CControlLayer::Instance().m_BrdMCI.bDet2nd )
		CControlLayer::Instance().m_BrdMCI.nOffset[ 2 * CControlLayer::Instance().m_BrdMCI.nDetPair +1 ] = nPos;
	else
		CControlLayer::Instance().m_BrdMCI.nOffset[ 2 * CControlLayer::Instance().m_BrdMCI.nDetPair ] = nPos;
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,Contrast,SemControlPanelClassID,SemControlPanelClass_Contrast)
void Contrast::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolContrastLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_Contrast.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + ClassID.lVal), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,SP_PMT,SemControlPanelClassID,SemControlPanelClass_SP_PMT)
void SP_PMT::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText, csTitle, csX, csY;
	if ( CAppLogManager::Instance().m_boolContrastLog )
	{
		csX = csY = CSemCoreAssemble::Instance().m_SP_PMT.FormatText();
		csTitle.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null + SemControlPanelClass_Contrast), ClassID.lVal);
		csText = csTitle + _T(" ") + csX;
		CDebugMessageDispatcher::Instance().DisplayMsg( csText );
	}
}


IMPLEMENT_FACTORY(ISemCoreCtrlPanelMessageProc,Enum_NULL,SemControlPanelClassID,SemControlPanelClass_NULL)
void Enum_NULL::DisplayMsg(CLanguageManagerCtrl* pLanguageManagerCtrl,const VARIANT FAR& ClassID,BOOL bXPos)
{
	CString csText = "";
	csText.Format( pLanguageManagerCtrl->GetResString(IDF_SD_SemControlPanelClassID_Null), ClassID.lVal);
}
