#include "StdAfx.h"
#include "AutoFocusThread.h"
#include "ScanParameterManager.h"
#include "SemCoreAssemble.h"
#include "Algorithm_AutoFocus.h"
#include "DebugMessageDispatcher.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "SemVirtualMCICard.h"
#include "MCICardCommonSetting.h"
#include "Factory.h"
#include "SemCtrl.h"
#include "HVPowerDataManager.h"
#include "CommFunctions.h"
#include "Preparation4Scan.h"
using namespace Prepare4Scan;

int  CAutoFocusThread::m_nExecRound  = 0;


CAutoFocusThread::CAutoFocusThread(void)
{
	m_hWnd = NULL;
	m_bImageBuff			= new BYTE[256*256];
	m_dwSleep = 1000;
}


CAutoFocusThread::~CAutoFocusThread(void)
{
	delete []m_bImageBuff;
}


BOOL CAutoFocusThread::Start()
{
	BOOL bStart = __super::Start();
	return bStart;
}


void CAutoFocusThread::Release()
{
	__super::Release();
}


DWORD CAutoFocusThread::DoJob()
{
	_AutoFocusProcess();
	Sleep( 1 );

	return 0L;
}


void CAutoFocusThread::Initialize(HWND hWnd,const DWORD dwOpr)
{
	m_hWnd = hWnd;
	CScanParameterManager::Instance().m_nusbScanTypeSaved	= CScanParameterManager::Instance().m_nusbScanType;
	CScanParameterManager::Instance().m_nusbPixRateSaved	= CScanParameterManager::Instance().m_nusbPixRate;

	CScanParameterSaver::Instance().m_lObjectiveLensSaved	= CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos().lVal;
	CScanParameterSaver::Instance().m_dblMagnifierSaved		= CSemCoreAssemble::Instance().m_Magnifier.GetPos().dblVal;

	if( dwOpr > 0 )
	{
		// Test 调试信息
		CString		csMsg;
		csMsg.Format("AF-Start, F =[0x%04X] %5.1f, M = %5.1f",
			CScanParameterSaver::Instance().m_lObjectiveLensSaved, CScanParameterSaver::Instance().m_lObjectiveLensSaved/OL_TO_LED*2, CScanParameterSaver::Instance().m_dblMagnifierSaved );
		CDebugMessageDispatcher::Instance().DisplayMsg( csMsg );
	}
	else
		CDebugMessageDispatcher::Instance().DisplayMsg( "AF-Start" );

	// 设定初始尝试值
	long 		lObjectiveLens	= CScanParameterSaver::Instance().m_lObjectiveLensSaved;
	double		dblMagnifier	= CScanParameterSaver::Instance().m_dblMagnifierSaved;
	AutoFocus_Initialize1( lObjectiveLens, dblMagnifier );

	_SetPos( lObjectiveLens, dblMagnifier );

	// 设定当前扫描
	CScanParameterManager::Instance().m_nusbPixRate = 16;	// USB_SS_8us
	_ChangeScan( USB_ST_AutoFocusArea );

	// 进入自动聚焦过程
	CScanParameterSaver::Instance().m_nAutoFuction = AutoFocus;
}



void CAutoFocusThread::_SetPos( const long lObjectiveLens, const double dblMagnifier )
{
	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	SerialNumber.lVal = -1;

	CScanParameterSaver::Instance().m_lLastlObjectiveLens	= lObjectiveLens;

	CScanParameterSaver::Instance().m_dbLastlMagnifierSaved =dblMagnifier;

	// 设置物镜聚焦
	ClassID.lVal	= SemControlPanelClass_ObjectiveLens;
	Flag.lVal		= 1;
	Xpos.dblVal		= lObjectiveLens;
	Xpos.lVal		= lObjectiveLens;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanel_SetPos( ClassID, SerialNumber, Xpos, Ypos, Flag );

	// 设置放大倍数
	ClassID.lVal	= SemControlPanelClass_Magnifier;
	Flag.lVal		= 1;
	Xpos.dblVal		= dblMagnifier;
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanel_SetPos(	ClassID, SerialNumber, Xpos, Ypos, Flag  );
}


void CAutoFocusThread::_ChangeScan( USB_ScanType nusbScanType )
{
	static int nusbScanTypeSave = USB_ST_RasterScan;

	CSemVirtualMCICard::Instance().SetStopScan(nusbScanType == USB_ST_Stop);

	USB_ScanFunction		Function	= USB_SF_RasterScan;	// 工作方式
	unsigned short			nPixRate	= CScanParameterManager::Instance().m_nusbPixRate;

	USB_ScanParam		Param;				// 扫描参数
	Param.RasterScan.dwStartX	= 0;
	Param.RasterScan.dwStartY	= 0;
	Param.RasterScan.dwStopX	= CMCICardCommonSetting::Instance().m_sizeReso.cx;
	Param.RasterScan.dwStopY	= CMCICardCommonSetting::Instance().m_sizeReso.cy;

	// ResetImageCard()
	if ( nusbScanType == -1 )
	{
		nusbScanType	= CScanParameterManager::Instance().m_nusbScanType;

		Function	= CScanParameterManager::Instance().m_usbFunction;		// 工作方式
		Param		= CScanParameterManager::Instance().m_usbParam;			// 扫描参数
	}

	IPrepare4Scan* pPreparation = Factory<IPrepare4Scan,USB_ScanType>::Instance().BuildProduct((USB_ScanType)nusbScanType);
	if (pPreparation)
	{
		pPreparation->Prepare(NULL,Param,Function,nPixRate);
		delete pPreparation;
	}
	else
	{
		nusbScanType = USB_ST_Stop;		// 默认冻结图象扫描
		CSemCtrl::Instance().m_SemScan.USB_ScanStop();
	}

	if ( USB_ST_Stop < nusbScanType && nusbScanType <= USB_ST_Max )
	{
		CSemCtrl::Instance().m_SemScan.USB_SetScanFunctionEx(
			Function,			// 工作方式
			CMCICardCommonSetting::Instance().m_sizeReso,			// 分辨率
			nPixRate,			// 像素率 = 点停留时间/4，必须为整数
			Param,				// 扫描参数
			CHVPowerDataManager::Instance().m_dHV );
	}

	// 保存参数 for ResetImageCard()
	nusbScanTypeSave	= nusbScanType;
	CScanParameterManager::Instance().m_usbFunction	  = Function;			// 工作方式
	CScanParameterManager::Instance().m_usbParam	  = Param;			// 扫描参数
	CScanParameterManager::Instance().m_nusbScanType  = nusbScanType;
}


void CAutoFocusThread::_AutoFocusProcess()
{
	char sz[512] = {0};
	
	// 复制图象
	CRect	rcDest = CScanParameterManager::Instance().m_rcAutoFocusAreaScanParam;
	long	lBuffSize = rcDest.Width() * rcDest.Height();

	DWORD dwTotalPixel = CSemVirtualMCICard::Instance().Read( (LPVOID)m_bImageBuff, lBuffSize, rcDest );

	// 分析图象，尝试新的聚焦值、放大倍数
	long 		lObjectiveLens_Old	= CScanParameterSaver::Instance().m_lLastlObjectiveLens;
	long 		lObjectiveLens		= CScanParameterSaver::Instance().m_lObjectiveLensSaved;
	double		dblMagnifier		= CScanParameterSaver::Instance().m_dblMagnifierSaved;

	double		dblMagnifier_Old	= CScanParameterSaver::Instance().m_dbLastlMagnifierSaved;
	double		dfSharpness = -1;

	ZeroMemory(sz,512);
	sprintf_s(sz,512,"void CAutoFocusThread::_AutoFocusProcess() ----- lObjectiveLens_Old = %d, lObjectiveLens = %d, dblMagnifier = %.1f, dblMagnifier_Old = %.1f \n", 
		lObjectiveLens_Old, lObjectiveLens, dblMagnifier, dblMagnifier_Old);
	OutputDebugString(sz);

	UINT nRet = AutoFocus_Analyze_Image1((LPVOID)m_bImageBuff, rcDest.Size(),dfSharpness,lObjectiveLens_Old , dblMagnifier ,lObjectiveLens );

	ZeroMemory(sz,512);
	sprintf_s(sz,512,"void CAutoFocusThread::_AutoFocusProcess(),after AutoFocus_Analyze_Image2 ----- lObjectiveLens_Old = %d, lObjectiveLens = %d, dblMagnifier = %.1f, dblMagnifier_Old = %.1f,m_dLastSharpness = %f, dfSharpness = %f \n", 
		lObjectiveLens_Old, lObjectiveLens, dblMagnifier, dblMagnifier_Old, m_dMaxSharpness, dfSharpness);
	OutputDebugString(sz);

	if (m_dMaxSharpness < dfSharpness)
	{
		m_dMaxSharpness = dfSharpness;
		m_dObjectiveLens = lObjectiveLens;
		m_dMagnifier = dblMagnifier;
	}

	CScanParameterSaver::Instance().m_lLastlObjectiveLens = lObjectiveLens;
	CScanParameterSaver::Instance().m_dbLastlMagnifierSaved = dblMagnifier;

	// Setup lObjectiveLens, dfMagnifier
	if ( nRet == AutoFocus_Analyze_Continue )
	{
		_SetPos( lObjectiveLens, dblMagnifier );
	}
	else
	{
		SetEvent(m_hExitEvent);
		CScanParameterSaver::Instance().m_nAutoFuction = AutoNULL;

		if ( nRet == AutoFocus_Analyze_Completed )
		{
			CScanParameterSaver::Instance().m_lObjectiveLensSaved = lObjectiveLens;
		}
		else
		{

		}

		ZeroMemory(sz,512);
		sprintf_s(sz,512,"void CAutoFocusThread::_AutoFocusProcess(),Finish ----- CScanParameterSaver::Instance().m_lObjectiveLensSaved = %f \n", 
			m_dObjectiveLens);
		OutputDebugString(sz);

		_StopAutoFocus();
	}
}


void CAutoFocusThread::_StopAutoFocus()
{
	char sz[512] = {0};
	ZeroMemory(sz,512);
	sprintf_s(sz,512,"void CAutoFocusThread::_StopAutoFocus() ----- m_dMaxSharpness = %f, m_dObjectiveLens = %.1f, m_dMagnifier = %.1f \n", 
		m_dMaxSharpness, m_dObjectiveLens, m_dMagnifier);
	OutputDebugString(sz);

	m_dMaxSharpness = 0.0;
	m_dObjectiveLens = 0.0;
	m_dMagnifier = 10.0;
	::SendMessage(m_hWnd,WM_USER_Stop_AutoFocus,0,0);

	if (0 == CAutoFocusThread::m_nExecRound)
	{
		CAutoFocusThread::m_nExecRound++;
		::SendMessage(m_hWnd,WM_USER_Start_AutoFocus,0,0);
	}
	else
	{
		CAutoFocusThread::m_nExecRound = 0;
	}

	Release();
}


BOOL CAutoFocusThread::End()
{
	m_bExitWorkThread = true;
	if(m_pWorkThread != NULL)
	{
		if(m_hExitEvent != NULL)
			SetEvent(m_hExitEvent);
		WaitForSingleObject(m_pWorkThread->m_hThread, 3000);

		//delete m_pWorkThread;
		m_pWorkThread = NULL;
	}

	return TRUE;
}
