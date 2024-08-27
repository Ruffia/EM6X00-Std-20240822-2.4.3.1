#include "stdafx.h"
#include "PanelDataProcessor.h"
#include <math.h>
#include "SemCoreAssemble.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "Factory.h"
#include "PanelModeMgr.h"
#include "Message.h"
#include "HVPowerDataManager.h"
#include "Util.h"
#include "GlobalParamSetting.h"
#include "SpellmanStatusDetector.h"
#include "PanelCoefficientMgr.h"
#include "XXX2.h"
#include "../SEM Control/SemCtrl_global.h"


IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_ScanShift_X,int,0x01)
PanelDataProcessor_ScanShift_X::PanelDataProcessor_ScanShift_X()
{ 
	m_fDivisor = 0.125f;
	m_byteFunctionCode = 0x01;
}


void PanelDataProcessor_ScanShift_X::_SyncPos(long lValue)
{
	VARIANT  ClassID, SerialNumber, Flag;
	VARIANT  Xmin, Xmax, Xpos;
	VARIANT  Ymin, Ymax, Ypos;

	CSemCoreAssemble::Instance().m_ScanShiftX.GetRange( Xmin, Xmax );
	CSemCoreAssemble::Instance().m_ScanShiftY.GetRange( Ymin, Ymax );

	Xpos = CSemCoreAssemble::Instance().m_ScanShiftX.GetPos();
	Xpos.lVal += lValue;
	Xpos.lVal = Xpos.lVal < Xmax.lVal?Xpos.lVal:Xmax.lVal; 
	
	Ypos = CSemCoreAssemble::Instance().m_ScanShiftY.GetPos();

	Flag.lVal = 1;
	ClassID.lVal = SemControlPanelClass_ScanShift;
	SerialNumber.lVal = -1;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}


IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_ScanShift_Y,int,0x02)
PanelDataProcessor_ScanShift_Y::PanelDataProcessor_ScanShift_Y()
{ 
	m_fDivisor = 0.125f;
	m_byteFunctionCode = 0x02;
}

void PanelDataProcessor_ScanShift_Y::_SyncPos(long lValue)
{
	VARIANT  ClassID, SerialNumber, Flag;
	VARIANT  Xmin, Xmax, Xpos;
	VARIANT  Ymin, Ymax, Ypos;

	CSemCoreAssemble::Instance().m_ScanShiftX.GetRange( Xmin, Xmax );
	CSemCoreAssemble::Instance().m_ScanShiftY.GetRange( Ymin, Ymax );

	Xpos = CSemCoreAssemble::Instance().m_ScanShiftX.GetPos();

	Ypos = CSemCoreAssemble::Instance().m_ScanShiftY.GetPos();
	Ypos.lVal += lValue;
	Ypos.lVal = Ypos.lVal < Ymax.lVal?Ypos.lVal:Ymax.lVal; 

	Flag.lVal = 2;
	ClassID.lVal = SemControlPanelClass_ScanShift;
	SerialNumber.lVal = -1;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}




//F1快捷键
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_F1,int,0x05)
PanelDataProcessor_F1::PanelDataProcessor_F1()
{
	m_byteFunctionCode = 0x05;
}

void PanelDataProcessor_F1::Notify()
{
	CString strDataPrevious = "";
	CString strDataCurrent = "";
	for (int i = 0; i < CmdLen; i++)
	{
		CString strByte = "";
		strByte.Format("%02X  ",m_dataPrevious[i]);
		strDataPrevious +=  strByte;
		strByte = "";
		strByte.Format("%02X  ",m_dataCurrent[i]);
		strDataCurrent += strByte;
	}

	strDataPrevious += "\r\n";
	strDataCurrent += "\r\n";

	char sz[512] = {0};
	ZeroMemory(sz,512);
	sprintf_s(sz,512,"IPanelDataProcessor::_CalcChangeData(), %s  %s \r\n",(LPCSTR)strDataPrevious,(LPCSTR)strDataCurrent);
	OutputDebugString(sz);

	if (m_dataCurrent[Data1] != m_dataPrevious[Data1])
	{
		if (m_dataCurrent[Data1] == 0x01)
		{
			PostMessage(m_hWnd,WM_USER_ControlPanel_PhotoPreview,1,0);
		}
	}
}

void PanelDataProcessor_F1::_SyncPos(long lValue)
{
}


//F2快捷键
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_F2,int,0x06)
PanelDataProcessor_F2::PanelDataProcessor_F2()
{
	m_byteFunctionCode = 0x06;
}

void PanelDataProcessor_F2::Notify()
{
	if (m_dataCurrent[Data1] != m_dataPrevious[Data1])
	{
		if (m_dataCurrent[Data1] == 0x01)
		{
			PostMessage(m_hWnd,WM_USER_ControlPanel_SyncScan,1,0);
		}
	}
}

void PanelDataProcessor_F2::_SyncPos(long lValue)
{}


//F3快捷键
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_F3,int,0x07)
PanelDataProcessor_F3::PanelDataProcessor_F3()
{
	m_byteFunctionCode = 0x07;
}

void PanelDataProcessor_F3::Notify()
{
	if (m_dataCurrent[Data1] != m_dataPrevious[Data1])
	{
		if (m_dataCurrent[Data1] == 0x01)
		{
			PostMessage(m_hWnd,WM_USER_ControlPanel_StopScan,1,0);
		}
	}
}

void PanelDataProcessor_F3::_SyncPos(long lValue)
{

}


//S1快捷键
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_S1,int,0x08)
PanelDataProcessor_S1::PanelDataProcessor_S1()
{
	m_byteFunctionCode = 0x08;
}


void PanelDataProcessor_S1::Notify()
{
	if (m_dataCurrent[Data1] != m_dataPrevious[Data1])
	{
		if (m_dataCurrent[Data1] == 0x01)
		{
			PostMessage(m_hWnd,WM_USER_ControlPanel_ChangeScan,0,0);
		}
	}
}

void PanelDataProcessor_S1::_SyncPos(long lValue)
{}


//S2快捷键
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_S2,int,0x09)
PanelDataProcessor_S2::PanelDataProcessor_S2()
{
	m_byteFunctionCode = 0x09;
}

void PanelDataProcessor_S2::Notify()
{
	if (m_dataCurrent[Data1] != m_dataPrevious[Data1])
	{
		if (m_dataCurrent[Data1] == 0x01)
		{
			PostMessage(m_hWnd,WM_USER_ControlPanel_ChangeScan,1,0);
		}
	}
}

void PanelDataProcessor_S2::_SyncPos(long lValue)
{

}


//S3快捷键
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_S3,int,0x10)
PanelDataProcessor_S3::PanelDataProcessor_S3()
{
	m_byteFunctionCode = 0x10;
}

void PanelDataProcessor_S3::Notify()
{
}

void PanelDataProcessor_S3::_SyncPos(long lValue)
{
}



IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_Stigmator_X,int,0x0B)
PanelDataProcessor_Stigmator_X::PanelDataProcessor_Stigmator_X()
{
	m_fDivisor = 1.f;
	m_byteFunctionCode = 0x0B;
}

void PanelDataProcessor_Stigmator_X::_SyncPos(long lValue)
{
	VARIANT  ClassID, SerialNumber, Flag;
	VARIANT  Xmin, Xmax, Xpos;
	VARIANT  Ymin, Ymax, Ypos;

	CSemCoreAssemble::Instance().m_StigmatorX.GetRange( Xmin, Xmax );
	CSemCoreAssemble::Instance().m_StigmatorY.GetRange( Ymin, Ymax );

	Xpos = CSemCoreAssemble::Instance().m_StigmatorX.GetPos();
	Xpos.lVal += lValue;
	Xpos.lVal = Xpos.lVal < Xmax.lVal?Xpos.lVal:Xmax.lVal; 

	Ypos = CSemCoreAssemble::Instance().m_StigmatorY.GetPos();

	Flag.lVal = 1;
	ClassID.lVal = SemControlPanelClass_Stigmator;
	SerialNumber.lVal = -1;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}


IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_Stigmator_Y,int,0x0C)
PanelDataProcessor_Stigmator_Y::PanelDataProcessor_Stigmator_Y()
{
	m_fDivisor = 1.f;
	m_byteFunctionCode = 0x0C;
}

void PanelDataProcessor_Stigmator_Y::_SyncPos(long lValue)
{
	VARIANT  ClassID, SerialNumber, Flag;
	VARIANT  Xmin, Xmax, Xpos;
	VARIANT  Ymin, Ymax, Ypos;

	CSemCoreAssemble::Instance().m_StigmatorX.GetRange( Xmin, Xmax );
	CSemCoreAssemble::Instance().m_StigmatorY.GetRange( Ymin, Ymax );

	Xpos = CSemCoreAssemble::Instance().m_StigmatorX.GetPos();

	Ypos = CSemCoreAssemble::Instance().m_StigmatorY.GetPos();
	Ypos.lVal += lValue;
	Ypos.lVal = Ypos.lVal < Ymax.lVal?Ypos.lVal:Ymax.lVal; 

	Flag.lVal = 2;
	ClassID.lVal = SemControlPanelClass_Stigmator;
	SerialNumber.lVal = -1;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}



//电对中X
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_BeemAlignment_X,int,0x0D)
PanelDataProcessor_BeemAlignment_X::PanelDataProcessor_BeemAlignment_X()
{   
	m_fDivisor = 0.125f;
	m_byteFunctionCode = 0x0D;
}

void PanelDataProcessor_BeemAlignment_X::_SyncPos(long lValue)
{
	VARIANT  ClassID, SerialNumber, Flag;
	VARIANT  Xmin, Xmax, Xpos;
	VARIANT  Ymin, Ymax, Ypos;

	CSemCoreAssemble::Instance().m_BeemAlignmentX.GetRange( Xmin, Xmax );
	CSemCoreAssemble::Instance().m_BeemAlignmentY.GetRange( Ymin, Ymax );

	Xpos = CSemCoreAssemble::Instance().m_BeemAlignmentX.GetPos();
	Xpos.lVal += lValue;
	Xpos.lVal = Xpos.lVal < Xmax.lVal?Xpos.lVal:Xmax.lVal; 

	Ypos = CSemCoreAssemble::Instance().m_BeemAlignmentY.GetPos();

	Flag.lVal = 1;
	ClassID.lVal = SemControlPanelClass_BeemAlignment;
	SerialNumber.lVal = -1;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}


//电对中Y
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_BeemAlignment_Y,int,0x0E)
PanelDataProcessor_BeemAlignment_Y::PanelDataProcessor_BeemAlignment_Y()
{
	m_fDivisor = 0.125f;
	m_byteFunctionCode = 0x0E;
}


void PanelDataProcessor_BeemAlignment_Y::_SyncPos(long lValue)
{
	VARIANT  ClassID, SerialNumber, Flag;
	VARIANT  Xmin, Xmax, Xpos;
	VARIANT  Ymin, Ymax, Ypos;

	CSemCoreAssemble::Instance().m_BeemAlignmentX.GetRange( Xmin, Xmax );
	CSemCoreAssemble::Instance().m_BeemAlignmentY.GetRange( Ymin, Ymax );

	Xpos = CSemCoreAssemble::Instance().m_BeemAlignmentX.GetPos();

	Ypos = CSemCoreAssemble::Instance().m_BeemAlignmentY.GetPos();
	Ypos.lVal += lValue;
	Ypos.lVal = Ypos.lVal < Ymax.lVal?Ypos.lVal:Ymax.lVal; 

	Flag.lVal = 2;
	ClassID.lVal = SemControlPanelClass_BeemAlignment;
	SerialNumber.lVal = -1;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
};


//电子束倾斜X
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_ElectronBeemTilt_X,int,0x0F)
PanelDataProcessor_ElectronBeemTilt_X::PanelDataProcessor_ElectronBeemTilt_X()
{   
	m_byteFunctionCode = 0x0F;
}


void PanelDataProcessor_ElectronBeemTilt_X::_SyncPos(long lValue)
{
}


//电子束倾斜Y
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_ElectronBeemTilt_Y,int,0x10)
PanelDataProcessor_ElectronBeemTilt_Y::PanelDataProcessor_ElectronBeemTilt_Y()
{   
	m_byteFunctionCode = 0x10;
}

void PanelDataProcessor_ElectronBeemTilt_Y::_SyncPos(long lValue)
{
}


//放大倍数粗调细调
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_MagnificationMode,int,0x11)
PanelDataProcessor_MagnificationMode::PanelDataProcessor_MagnificationMode()
{m_byteFunctionCode = 0x11;}

void PanelDataProcessor_MagnificationMode::Notify()
{
	MagnificationControlMode Mode = (m_dataCurrent[Data1] == 0x00)?MagnificationMode_Large:MagnificationMode_Small;
	CPanelModeMgr::Instance().MagMode = Mode;
}

void PanelDataProcessor_MagnificationMode::_SyncPos(long lValue)
{}


//放大倍数调节
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_Magnifier,int,0x12)
PanelDataProcessor_Magnifier::PanelDataProcessor_Magnifier()
{m_byteFunctionCode = 0x12;}

void PanelDataProcessor_Magnifier::_SyncPos(long lValue)
{
	VARIANT ClassID, SerialNumber, Flag;
	VARIANT Xpos, Ypos;
	SerialNumber.lVal = -1;
	Flag.lVal = 1;
	Ypos.lVal = 0;

	ClassID.lVal = SemControlPanelClass_Magnifier;
	Xpos = CSemCoreAssemble::Instance().m_Magnifier.GetPos();

	double sblStep = Xpos.dblVal;
	if (CPanelModeMgr::Instance().MagMode == MagnificationMode_Small)
	{
		if( lValue > 0 )
		{
			sblStep = GetThirdSignificandWeight( Xpos.dblVal, lValue, TRUE );
			Xpos.dblVal += sblStep;
		}
		else if( lValue < 0 )
		{
			sblStep = GetThirdSignificandWeight( Xpos.dblVal, lValue, FALSE );
			Xpos.dblVal += sblStep;
		}
	}
	else
	{
		if( lValue > 0 )
		{
			sblStep = GetThirdSignificandWeight( Xpos.dblVal, lValue * 10, TRUE  );
			Xpos.dblVal += sblStep;
		}
		else if( lValue < 0 )
		{
			sblStep = GetThirdSignificandWeight( Xpos.dblVal, lValue * 10, FALSE );
			Xpos.dblVal += sblStep;
		}
	}

	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(ClassID,SerialNumber,Xpos,Ypos,Flag);
}


//自动亮度对比度
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_AutoBrightContrast,int,0x14)
PanelDataProcessor_AutoBrightContrast::PanelDataProcessor_AutoBrightContrast()
{m_byteFunctionCode = 0x14;}

void PanelDataProcessor_AutoBrightContrast::Notify()
{
	if (m_dataCurrent[Data1] != m_dataPrevious[Data1])
	{
		if (m_dataCurrent[Data1] == 0x01)
		{
			PostMessage(m_hWnd,WM_USER_ControlPanel_AutoBrightContrast,0,0);
		}
	}

}

void PanelDataProcessor_AutoBrightContrast::_SyncPos(long lValue)
{}



//亮度调节
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_Brightness,int,0x15)
PanelDataProcessor_Brightness::PanelDataProcessor_Brightness()
{
	m_fDivisor = 1.f;
	m_byteFunctionCode = 0x15;
}

void PanelDataProcessor_Brightness::_SyncPos(long lValue)
{
	VARIANT ClassID, SerialNumber, Flag;
	VARIANT Xpos, Ypos;
	SerialNumber.lVal = -1;
	Flag.lVal = 1;
	Ypos.lVal = 0;

	ClassID.lVal = SemControlPanelClass_Brightness;
	Xpos = CSemCoreAssemble::Instance().m_Brightness.GetPos();
	Xpos.lVal += lValue;

	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(ClassID,SerialNumber,Xpos,Ypos,Flag);
}


//对比度调节
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_Contrast,int,0x16)
PanelDataProcessor_Contrast::PanelDataProcessor_Contrast()
{
	m_fDivisor = 1.f;
	m_byteFunctionCode = 0x16;
}

void PanelDataProcessor_Contrast::_SyncPos(long lValue)
{
	VARIANT ClassID, SerialNumber, Flag;
	VARIANT Xpos, Ypos;
	SerialNumber.lVal = -1;
	Flag.lVal = 1;
	Ypos.lVal = 0;

	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )		// CPS 3603
	{
		ClassID.lVal = SemControlPanelClass_Contrast;
		Xpos = CSemCoreAssemble::Instance().m_Contrast.GetPos();
		Xpos.lVal += lValue;

		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(ClassID,SerialNumber,Xpos,Ypos,Flag);
	}
	else if( CHVPowerDataManager::Instance().m_dwHVtype < 2 )	// SPM 771
	{
		ClassID.lVal = SemControlPanelClass_SP_PMT;
		Xpos = CSemCoreAssemble::Instance().m_SP_PMT.GetPos();
		Xpos.lVal += lValue;

		CHVPowerDataManager::Instance().m_Spellmans.pS.dVP	= Xpos.lVal;
		CGlobalParamSetting::Instance().g_Spellmans.pS.dVP	= Xpos.lVal *10 *1350.0 /4095;	// Xpos.lVal;
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
		CSpellmanStatusDetector::Instance().SetParam( 35, Xpos.lVal *10 );
	}
	else	// WSM
	{
		ClassID.lVal = SemControlPanelClass_SP_PMT;
		Xpos = CSemCoreAssemble::Instance().m_SP_PMT.GetPos();
		Xpos.lVal += lValue;

		CHVPowerDataManager::Instance().m_Spellmans.pS.dVP	= Xpos.lVal;
		CGlobalParamSetting::Instance().g_Spellmans.pS.dVP	= Xpos.lVal *1300.0 /400;
		theApp.m_setWSM30KV.fPMTv	= (float)(Xpos.lVal *1300.0 /400);
		theApp.m_setWSM30KV2.fPMTv	= (float)(Xpos.lVal *1300.0 /400);
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
		if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
			theApp.m_WSM30KV2.SetParam( 4, (int)(Xpos.lVal *1300.0 /400) );
		else
			theApp.m_WSM30KV.SetParam( 4, (int)(Xpos.lVal *1300.0 /400) );
	}
}


//聚焦功能粗调细调
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_FocusMode,int,0x17)
PanelDataProcessor_FocusMode::PanelDataProcessor_FocusMode()
{
	m_byteFunctionCode = 0x17;
}

void PanelDataProcessor_FocusMode::Notify()
{
	FocusControlMode Mode = (m_dataCurrent[Data1] == 0x00)?FocusControlMode_Large:FocusControlMode_Small;
	CPanelModeMgr::Instance().FocusMode = Mode;
	CPanelModeMgr::Instance().FocusModeBK = Mode;
}

void PanelDataProcessor_FocusMode::_SyncPos(long lValue)
{}


//自动聚焦功能
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_AutoFocus,int,0x18)
PanelDataProcessor_AutoFocus::PanelDataProcessor_AutoFocus()
{m_byteFunctionCode = 0x18;}

void PanelDataProcessor_AutoFocus::Notify()
{
	if (m_dataCurrent[Data1] != m_dataPrevious[Data1])
	{
		if (m_dataCurrent[Data1] == 0x01)
		{
			PostMessage(m_hWnd,WM_USER_ControlPanel_AutoFocus,0,0);
		}
	}
}

void PanelDataProcessor_AutoFocus::_SyncPos(long lValue)
{}


//聚焦调节
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_Focus,int,0x19)
PanelDataProcessor_Focus::PanelDataProcessor_Focus()
{   
	m_fDivisor = 1.f;
	m_byteFunctionCode = 0x19;
}


void PanelDataProcessor_Focus::_SyncPos(long lValue)
{
	VARIANT ClassID, SerialNumber, Flag;
	VARIANT Xpos, Ypos;
	ClassID.lVal = SemControlPanelClass_ObjectiveLens;
	SerialNumber.lVal = -1;
	Ypos.lVal = 0;
	Flag.lVal = 1;

	Xpos = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
	if (CPanelModeMgr::Instance().FocusMode == FocusControlMode_Small)
	{
		Xpos.dblVal += (lValue/CPanelCoefficientMgr::Instance().GetSmallFocusMode());	
	}
	else if(CPanelModeMgr::Instance().FocusMode == FocusControlMode_Large)
	{
		Xpos.dblVal += (lValue * 8/CPanelCoefficientMgr::Instance().GetLargeFocusMode());
	}
	else if (CPanelModeMgr::Instance().FocusMode == FocusControlMode_ScreenFocus)
	{
		double	dfSlowDown = pow( 10, g_dfSlowDownlog10 );
		Xpos.dblVal = Xpos.dblVal + lValue * dfSlowDown;
	}

	Xpos.lVal = (long)(Xpos.dblVal);
	
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc(ClassID,SerialNumber,Xpos,Ypos,Flag);
}



//扫描速度-
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_ScanSpeedMinus,int,0x1A)
PanelDataProcessor_ScanSpeedMinus::PanelDataProcessor_ScanSpeedMinus()
{m_byteFunctionCode = 0x1A;}

void PanelDataProcessor_ScanSpeedMinus::Notify()
{
	if (m_dataCurrent[Data1] != m_dataPrevious[Data1])
	{
		if (1 <= m_dataCurrent[Data1] && m_dataCurrent[Data1] <= 4)
		{
			CPanelModeMgr::Instance().m_nScanSpeedMode = m_dataCurrent[Data1];
			PostMessage(m_hWnd,WM_USER_ControlPanel_ChangeScanSpeed,WPARAM(CPanelModeMgr::Instance().m_nScanSpeedMode),0);
		}	
	}
}

void PanelDataProcessor_ScanSpeedMinus::_SyncPos(long lValue)
{}



//扫描速度+
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_ScanSpeedAdd,int,0x1B)
PanelDataProcessor_ScanSpeedAdd::PanelDataProcessor_ScanSpeedAdd()
{m_byteFunctionCode = 0x1B;}

void PanelDataProcessor_ScanSpeedAdd::Notify()
{
	if (m_dataCurrent[Data1] != m_dataPrevious[Data1])
	{
		CPanelModeMgr::Instance().m_nScanSpeedMode = m_dataCurrent[Data1];
		if (1 <= m_dataCurrent[Data1] && m_dataCurrent[Data1] <= 4)
		{
			CPanelModeMgr::Instance().m_nScanSpeedMode = m_dataCurrent[Data1];
			PostMessage(m_hWnd,WM_USER_ControlPanel_ChangeScanSpeed,WPARAM(CPanelModeMgr::Instance().m_nScanSpeedMode),0);
		}
	}
}

void PanelDataProcessor_ScanSpeedAdd::_SyncPos(long lValue)
{}



//消像散，电对中功能切换
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_StigApertureModeSwitch,int,0x1D)
PanelDataProcessor_StigApertureModeSwitch::PanelDataProcessor_StigApertureModeSwitch()
{
	m_byteFunctionCode = 0x1D;
}

void PanelDataProcessor_StigApertureModeSwitch::Notify()
{
	if (CPanelModeMgr::Instance().m_nStigmatorApertureSwitchMode != m_dataCurrent[Data1])
	{
		if (0x01 == m_dataCurrent[Data1])
		{
			CPanelModeMgr::Instance().m_eStigmatorSwitchSatusX = StigmatorApertureSwitchStatus::StigmatorApertureSwitching;
			CPanelModeMgr::Instance().m_eStigmatorSwitchSatusY = StigmatorApertureSwitchStatus::StigmatorApertureSwitching;
		}
		else if (0x02 == m_dataCurrent[Data1])
		{
			CPanelModeMgr::Instance().m_eApertureSwitchSatusX = StigmatorApertureSwitchStatus::StigmatorApertureSwitching;
			CPanelModeMgr::Instance().m_eApertureSwitchSatusY = StigmatorApertureSwitchStatus::StigmatorApertureSwitching;
		}
		else if (0x03 == m_dataCurrent[Data1])
		{
			CPanelModeMgr::Instance().m_eElectronBeemTiltSwitchSatusX = StigmatorApertureSwitchStatus::StigmatorApertureSwitching;
			CPanelModeMgr::Instance().m_eElectronBeemTiltSwitchSatusY = StigmatorApertureSwitchStatus::StigmatorApertureSwitching;
		}
		else
		{
			OutputDebugString("PanelDataProcessor_StigApertureModeSwitch::Notify(), should not come here");
			return;
		}

		CPanelModeMgr::Instance().m_nStigmatorApertureSwitchMode = m_dataCurrent[Data1];
	}
}

void PanelDataProcessor_StigApertureModeSwitch::_SyncPos(long lValue)
{
}



//电位移，聚光镜切换模式
IMPLEMENT_FACTORY(IPanelDataProcessor,PanelDataProcessor_ShiftCondeserModeSwitch,int,0x1C)
PanelDataProcessor_ShiftCondeserModeSwitch::PanelDataProcessor_ShiftCondeserModeSwitch()
{
	m_byteFunctionCode = 0x1C;
}

void PanelDataProcessor_ShiftCondeserModeSwitch::Notify()
{
	if (CPanelModeMgr::Instance().m_nShiftCondeserSwitchMode != m_dataCurrent[Data1])
	{
		CPanelModeMgr::Instance().m_eShiftCondeserSwitchSatusX = ShiftCondeserSwitchStatus::ShiftCondeserSwitching;
		CPanelModeMgr::Instance().m_eShiftCondeserSwitchSatusY = ShiftCondeserSwitchStatus::ShiftCondeserSwitching;
		CPanelModeMgr::Instance().m_nShiftCondeserSwitchMode = m_dataCurrent[Data1];
	}
}

void PanelDataProcessor_ShiftCondeserModeSwitch::_SyncPos(long lValue)
{
}