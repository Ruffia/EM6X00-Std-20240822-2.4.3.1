// Scan.cpp: implementation of the CScan class.
//
// Last Modify : 2006.09.04
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BasisScan.h"
#include "RasterScan.h"
#include "AnalogPhoto.h"
#include "DigitalPhoto.h"
#include "LineScan.h"
#include "SyncScan.h"

#include "Scan.h"
#include "UsbDI16A.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DWORD	g_TickCount;
DWORD	g_thisTickCount, g_lastTickCount;

CCriticalSection CScan::m_criticalOperation;

const DWORD		g_wFullRange	= 65536;			// 满量程2的16次方
const double	g_dFullRange2	= 65536 /sqrt((double)2);	// 65536 / sqrt(2)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScan::CScan()
{
	m_bReady		= FALSE;
	m_pPhotoBuff	= NULL;
	
	m_pScanCtrl		= NULL;

	m_dwExpectantDMABlockSize	= 0;
	m_dwExpectantInterval		= 0;	// ms

	m_bDebugScan	= FALSE;	// 正常模式
	m_bSplit		= FALSE;

	m_rcDestFrame.left		= 0;
	m_rcDestFrame.right		= 1024;
	m_rcDestFrame.top		= 0;
	m_rcDestFrame.bottom	= 768;

	m_nusbPixRate		= 8;
	m_nXOffset			= 0;
	m_nYOffset			= 0;
	m_sizeReso			= CSize(1024, 768);
	m_usbFunction		= USB_SF_RasterScan;
	///////////////////////////////////

	// 初始化手动设置的扫描参数
	// 像素时钟N值
	m_nusbDebugPixRate	= 8;
	WORD	wStepX		= 64;	// (WORD)(g_wFullRange / m_sizeReso.cx);
	WORD	wStepY		= 85;	// (WORD)(g_wFullRange / m_sizeReso.cy);
	double	dRatio		= 1.0;	// sqrt( dHV / 30.0 );
	// X扫描波
	m_usbDebugParamX.wOffset		= 0;
	m_usbDebugParamX.wValley		= (WORD)(50000 / (m_nusbDebugPixRate *125) /4);
	if( m_usbDebugParamX.wValley < 1 )	// Valley必须大于等于1
		m_usbDebugParamX.wValley	= 1;
	m_usbDebugParamX.wRise			= 1024;	// (WORD)m_sizeReso.cx;
	m_usbDebugParamX.wDRise			= (WORD)(wStepX * dRatio);
	m_usbDebugParamX.wPeak			= PeakX;
	m_usbDebugParamX.wFall			= FallX;
	m_usbDebugParamX.wDFall			= (WORD)(m_usbDebugParamX.wRise *m_usbDebugParamX.wDRise -1);

	// Y扫描波
	m_usbDebugParamY.wOffset		= 0;
	m_usbDebugParamY.wValley		= (WORD)(50000 / (m_nusbDebugPixRate *125) /4);
	if( m_usbDebugParamY.wValley < 1 )	// Valley必须大于等于1
		m_usbDebugParamY.wValley	= 1;
	m_usbDebugParamY.wRise			= 768;	// (WORD)m_sizeReso.cy;
	m_usbDebugParamY.wDRise			= (WORD)(wStepY * dRatio);
	m_usbDebugParamY.wPeak			= PeakY;
	m_usbDebugParamY.wFall			= FallY;
	m_usbDebugParamY.wDFall			= (WORD)(m_usbDebugParamY.wRise *m_usbDebugParamY.wDRise -1);

	m_nMCIdetPair	= 0;
	for(int i=0; i<3; i++ )
		m_nMCIfilter[i] = 0;
}

CScan::~CScan()
{
//	StopScan();
}

BOOL CScan::IsAutoStop()
{
	if ( m_pScanCtrl != NULL )
	{
		if ( m_pScanCtrl->IsFinish() )
		{
			StopScan();
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CScan::GetErrorInformation( CString& csError )
{

#ifdef _DEBUm_ADD_ERRINFO_
	if ( m_pScanCtrl != NULL )
		return m_pScanCtrl->GetErrorInformation( csError );
#endif //_DEBUm_ADD_ERRINFO_

	return FALSE;
}

/*
const int SET_PARAMS_StoreMode			= 1;
const int SET_PARAMS_DetectorMode		= 2;
const int SET_PARAMS_NeedMon			= 3;
*/

void CScan::USB_SetParams( int nType, int nParam )		// 设置参数
{
/*	if( nType == SET_PARAMS_StoreMode )
		m_nDataStoreMode = nParam;						// 设置数据存储模式
	else */if( nType == SET_PARAMS_NeedMon )
	{
		m_bNeedMon = nParam;							// 设置采集时是否要监测数据
		if( m_pScanCtrl != NULL )
			m_pScanCtrl->m_bNeedMon = nParam;
	}
	///////////////////////////////////////////////////////
	// 调试模式时手动设置扫描参数
	else if( nType == 5 )
		m_bDebugScan = (BOOL)nParam;
	else if( nType == 10 )
		m_nusbDebugPixRate = nParam;	// Pixel Clock N value
	else if( nType == 11 )
		m_usbDebugParamX.wOffset	= (WORD)nParam;
	else if( nType == 12 )
		m_usbDebugParamX.wValley	= (WORD)nParam;
	else if( nType == 13 )
		m_usbDebugParamX.wRise		= (WORD)nParam;
	else if( nType == 14 )
		m_usbDebugParamX.wDRise		= (WORD)nParam;
	else if( nType == 15 )
		m_usbDebugParamX.wPeak		= (WORD)nParam;
	else if( nType == 16 )
		m_usbDebugParamX.wFall		= (WORD)nParam;
	else if( nType == 17 )
		m_usbDebugParamX.wDFall		= (WORD)nParam;
	else if( nType == 21 )
		m_usbDebugParamY.wOffset	= (WORD)nParam;
	else if( nType == 22 )
		m_usbDebugParamY.wValley	= (WORD)nParam;
	else if( nType == 23 )
		m_usbDebugParamY.wRise		= (WORD)nParam;
	else if( nType == 24 )
		m_usbDebugParamY.wDRise		= (WORD)nParam;
	else if( nType == 25 )
		m_usbDebugParamY.wPeak		= (WORD)nParam;
	else if( nType == 26 )
		m_usbDebugParamY.wFall		= (WORD)nParam;
	else if( nType == 27 )
		m_usbDebugParamY.wDFall		= (WORD)nParam;
	///////////////////////////////////////////////////////
	else if( nType == 31 )
		m_bSplit = (BOOL)nParam;
	else if( nType == 32 )
		m_nMCIdetPair	= (int)nParam;
	else if( nType == 33 )
		m_nMCIfilter[0]	= (int)nParam;
	else if( nType == 34 )
		m_nMCIfilter[1]	= (int)nParam;
	else if( nType == 35 )
		m_nMCIfilter[2]	= (int)nParam;

}

BOOL CScan::USB_SetScanFunctionEx(
			USB_ScanFunction&			Function,		// 工作方式
			CSize&						szReso,			// 分辨率
			unsigned short&				nPixRate,		// 像素率 = 点停留时间 /4
			USB_ScanParam&				Param,			// 扫描参数
			double						dHV )			// 高压
{
	BOOL	blRet = TRUE;

	///////////////////////////////////////////////////////
	m_criticalOperation.Lock();
	///////////////////////////////////////////////////////

//	m_usbResolution		= Resolution;
	m_sizeReso			= szReso;
	m_usbFunction		= Function;
	m_nusbPixRate		= nPixRate;

	switch (Function)
	{
	case USB_SF_RasterScan:
	case USB_SF_AreaScan:
//	case USB_SF_SplitScan:
		blRet = USB_SFRasterScan( Function, szReso, m_nusbPixRate, Param.RasterScan, dHV );
		break;

	case USB_SF_LineScan_HORZ:
	case USB_SF_LineScan_VERT:
		blRet = USB_SFLineScan( szReso, m_nusbPixRate, Param.LineScan );
		break;
	case USB_SF_SpotScan:
		blRet = USB_SFSpotScan( szReso, m_nusbPixRate, Param.LineScan );
		break;
		
	case USB_SF_AnalogPhoto:
		blRet = USB_SFSyncScan( szReso, m_nusbPixRate, Param.RasterScan, dHV );
		break;
/*
	case USB_SF_DualMagnification:
		blRet = USB_SFDualMagnification( Speed, Param.DualMag );
		break;

	case USB_SF_DigitalPhoto:
		blRet = USB_SFDigitalPhoto( Speed );
		break;
*/
	default:
		blRet = USB_ClearScan();
		blRet = FALSE;
		break;
	}

	USB_SetGraph( Function,	szReso,	Param );

	///////////////////////////////////////////////////////
	m_criticalOperation.Unlock();
	///////////////////////////////////////////////////////

	return blRet;
}

BOOL CScan::USB_StartRasterScan()
{
	CRasterScan* pWorker = new CRasterScan();
	
	pWorker->m_dwDMABuffSize			= 1024;//m_pPciCard->GetDMABuffSize();
	pWorker->m_dwExpectantDMABlockSize	= m_dwExpectantDMABlockSize;
	pWorker->m_dwExpectantInterval		= m_dwExpectantInterval;
	pWorker->m_rcDestFrame				= m_rcDestFrame;
	pWorker->m_bNeedMon					= m_bNeedMon;

	if (!pWorker->InitData())
	{
		delete pWorker;
		pWorker = NULL;
		return FALSE;
	}

	// 打开端口
	DWORD dwRet = 0;
	dwRet = CUsbDI16A::Instance().USB_StreamOpen(1);
	if( CSemVirtualMCICard::Instance().isReady() )
	{
		CSemVirtualMCICard::Instance().SetParams( SET_PARAMS_NeedMon, m_bNeedMon );
		CSemVirtualMCICard::Instance().SetFunction( (USB_ScanType)m_usbFunction );
		CSemVirtualMCICard::Instance().InitData( m_dwExpectantDMABlockSize, m_nXOffset, m_nYOffset );
		CSemVirtualMCICard::Instance().SetDestFrame( m_rcDestFrame );
	}

	if (!pWorker->CreateThread(CREATE_SUSPENDED))
	{
		delete pWorker;
		pWorker = NULL;
		return FALSE;
	}
	else
	{
		VERIFY(pWorker->SetThreadPriority(THREAD_PRIORITY_NORMAL));

		// 设置为常规扫描
		USB_WriteToBoard( USB_ScanModeNormal, FALSE, 0 );
		// Stop scan
		USB_WriteToBoard( USB_StopScan, FALSE, 0 );
		// 将FIFO清空
		CUsbDI16A::Instance().USB_ClearFIFO();
		// 开启视频
		CUsbDI16A::Instance().USB_VideoON();
		// 发起扫描
		USB_WriteToBoard( USB_StartScan, FALSE, 0 );

		// Now the thread can run wild
		pWorker->ResumeThread();
	}

	m_pScanCtrl = (CBasisScan *)pWorker;

	return TRUE;
}

BOOL CScan::USB_StartLineScan()
{
	CLineScan*	pWorker = new CLineScan();
	pWorker->m_dwDMABuffSize			= 1024;//m_pPciCard->GetDMABuffSize();
	pWorker->m_dwExpectantDMABlockSize	= m_dwExpectantDMABlockSize;
	pWorker->m_dwExpectantInterval		= m_dwExpectantInterval;
	pWorker->m_rcDestFrame				= m_rcDestFrame;
	pWorker->m_bNeedMon					= m_bNeedMon;

	if (!pWorker->InitData())
	{
		delete pWorker;
		pWorker = NULL;
		return FALSE;
	}

	// 打开端口
	DWORD dwRet = 0;
	dwRet = CUsbDI16A::Instance().USB_StreamOpen(1);
/*	if( dwRet )
	{
		delete pWorker;
		pWorker = NULL;
		return FALSE;
	}
*/	// Virtual MCI Card
	if( CSemVirtualMCICard::Instance().isReady() )
	{
		CSemVirtualMCICard::Instance().SetParams( SET_PARAMS_NeedMon, m_bNeedMon );
		CSemVirtualMCICard::Instance().SetFunction( (USB_ScanType)m_usbFunction );
		CSemVirtualMCICard::Instance().InitData( m_dwExpectantDMABlockSize, m_nXOffset, m_nYOffset );
		CSemVirtualMCICard::Instance().SetDestFrame( m_rcDestFrame );
	}

	if (!pWorker->CreateThread(CREATE_SUSPENDED))
	{
		delete pWorker;
		pWorker = NULL;
		return FALSE;
	}
	else
	{
		VERIFY(pWorker->SetThreadPriority(THREAD_PRIORITY_NORMAL));

		// Stop scan
		USB_WriteToBoard( USB_StopScan, FALSE, 0 );
		// 将FIFO清空
		CUsbDI16A::Instance().USB_ClearFIFO();
		// 开启视频
//			USB_WriteToBoard( USB_Video_ON, FALSE, 0 );
		CUsbDI16A::Instance().USB_VideoON();
		// 设置为线扫描
		if( m_usbFunction == USB_SF_LineScan_HORZ )
			USB_WriteToBoard( USB_ScanModeLineHorizontal, FALSE, 0 );
		else
			USB_WriteToBoard( USB_ScanModeLineVertical, FALSE, 0 );
		// 发起扫描
		USB_WriteToBoard( USB_StartScan, FALSE, 0 );

		// Now the thread can run wild
		pWorker->ResumeThread();
	}

	m_pScanCtrl = (CBasisScan *)pWorker;

	return TRUE;
}

BOOL CScan::USB_StartSpotScan()
{
	CLineScan*	pWorker = new CLineScan();
	pWorker->m_dwDMABuffSize			= 1024;//m_pPciCard->GetDMABuffSize();
	pWorker->m_dwExpectantDMABlockSize	= m_dwExpectantDMABlockSize;
	pWorker->m_dwExpectantInterval		= m_dwExpectantInterval;
	pWorker->m_rcDestFrame				= m_rcDestFrame;

	if (!pWorker->InitData())
	{
		delete pWorker;
		pWorker = NULL;
		return FALSE;
	}

	// 打开端口
	DWORD dwRet = 0;
	dwRet = CUsbDI16A::Instance().USB_StreamOpen(1);
	if( CSemVirtualMCICard::Instance().isReady() )
	{
		CSemVirtualMCICard::Instance().SetParams( SET_PARAMS_NeedMon, m_bNeedMon );
//		CSemVirtualMCICard::Instance().SetParams( SET_PARAMS_StoreMode, m_nDataStoreMode );
		CSemVirtualMCICard::Instance().SetFunction( (USB_ScanType)m_usbFunction );
		CSemVirtualMCICard::Instance().InitData( m_dwExpectantDMABlockSize, m_nXOffset, m_nYOffset );
		CSemVirtualMCICard::Instance().SetDestFrame( m_rcDestFrame );
	}

	if (!pWorker->CreateThread(CREATE_SUSPENDED))
	{
		delete pWorker;
		pWorker = NULL;
		return FALSE;
	}
	else
	{
		VERIFY(pWorker->SetThreadPriority(THREAD_PRIORITY_NORMAL));

		// Stop scan
		USB_WriteToBoard( USB_StopScan, FALSE, 0 );
		// 将FIFO清空
		CUsbDI16A::Instance().USB_ClearFIFO();
		// 开启视频
//			USB_WriteToBoard( USB_Video_ON, FALSE, 0 );
//			CUsbDI16A::Instance().USB_VideoON();
		// 设置为点扫描
		USB_WriteToBoard( USB_ScanModeSpot, FALSE, 0 );
		// 发起扫描
		USB_WriteToBoard( USB_StartScan, FALSE, 0 );

		// Now the thread can run wild
		pWorker->ResumeThread();
	}

	m_pScanCtrl = (CBasisScan *)pWorker;

	return TRUE;
}

BOOL CScan::USB_StartSyncScan()
{
	CSyncScan*	pWorker = new CSyncScan();
	pWorker->m_dwDMABuffSize			= 1024;//m_pPciCard->GetDMABuffSize();
	pWorker->m_dwExpectantDMABlockSize	= m_dwExpectantDMABlockSize;
	pWorker->m_dwExpectantInterval		= m_dwExpectantInterval;
	pWorker->m_rcDestFrame				= m_rcDestFrame;
	pWorker->m_bNeedMon					= m_bNeedMon;

	if (!pWorker->InitData())
	{
		delete pWorker;
		pWorker = NULL;
		return FALSE;
	}

	// 打开端口
	DWORD dwRet = 0;
	dwRet = CUsbDI16A::Instance().USB_StreamOpen(1);
	if( CSemVirtualMCICard::Instance().isReady() )
	{
		CSemVirtualMCICard::Instance().SetParams( SET_PARAMS_NeedMon, m_bNeedMon );
//		CSemVirtualMCICard::Instance().SetParams( SET_PARAMS_StoreMode, m_nDataStoreMode );
		CSemVirtualMCICard::Instance().SetFunction( (USB_ScanType)m_usbFunction );
		CSemVirtualMCICard::Instance().InitData( m_dwExpectantDMABlockSize, m_nXOffset, m_nYOffset );
		CSemVirtualMCICard::Instance().SetDestFrame( m_rcDestFrame );
	}

	if (!pWorker->CreateThread(CREATE_SUSPENDED))
	{
		delete pWorker;
		pWorker = NULL;
		return FALSE;
	}
	else
	{
		VERIFY(pWorker->SetThreadPriority(THREAD_PRIORITY_NORMAL));

		// 设置为常规扫描
		USB_WriteToBoard( USB_ScanModeNormal, FALSE, 0 );
		// Stop scan
		USB_WriteToBoard( USB_StopScan, FALSE, 0 );
		// 将FIFO清空
		CUsbDI16A::Instance().USB_ClearFIFO();
		// 开启视频
//			USB_WriteToBoard( USB_Video_ON, FALSE, 0 );
		CUsbDI16A::Instance().USB_VideoON();
		// 发起扫描
		USB_WriteToBoard( USB_StartScan, FALSE, 0 );

		// Now the thread can run wild
		pWorker->ResumeThread();
	}

	m_pScanCtrl = (CBasisScan *)pWorker;

	return TRUE;
}

// 设置四段扫描波各参数
void CScan::USB_SetRampParam(
//			USB_ScanFunction&	Function,	
			CSize				szReso,
			CRect rcFrame, USB_InitData& data,
			double	dHV )
{
	if( m_bDebugScan )
	{
		data.ScanRampParamX = m_usbDebugParamX;
		data.ScanRampParamY = m_usbDebugParamY;
		return;
	}
	// 始终使用满量程2的14次方，步距根据当前分辨率设置
//	WORD	wFullRange	= (WORD)pow(2, 14);
	WORD	wStepX		= (WORD)(g_wFullRange / (szReso.cx +m_nXOffset) );
	WORD	wStepY		= (WORD)(g_wFullRange / szReso.cy);
	double	dRatio		= sqrt( dHV / 30.0 );

	double dOffset;
	USB_InitScanRampParam usbParam;
	// 以30KV时常规扫描的中心为定点，而不是以0点为定点
	// X扫描波
	dOffset					= rcFrame.left + g_wFullRange *( 1 -dRatio ) /2 -1;
	if( dOffset < 0 )
		dOffset = 0;
	usbParam.wOffset		= (WORD)(dOffset +rcFrame.left *wStepX*dRatio);
	usbParam.wValley		= (WORD)(50000 / (m_nusbPixRate *125) /4);//ValleyX;
	if( usbParam.wValley < 1 )	// Valley必须大于等于1
		usbParam.wValley	= 1;
	usbParam.wRise			= (WORD)rcFrame.Width();
	usbParam.wDRise			= (WORD)(wStepX * dRatio);
	usbParam.wPeak			= PeakX;
	usbParam.wFall			= FallX;
//	usbParam.wDFall			= DFallX;//(WORD)(rcFrame.Width()/usbParam.wFall *wStepX);
	if( usbParam.wFall > 0 )
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise /usbParam.wFall);
	else
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise -1);
//	usbParam.wDirection		= 0;
	data.ScanRampParamX		= usbParam;

	// Y扫描波
	dOffset					= rcFrame.top + g_wFullRange *( 1 -dRatio ) /2 -1;
	if( dOffset < 0 )
		dOffset = 0;
	usbParam.wOffset		= (WORD)(dOffset +rcFrame.top *wStepY*dRatio);
	usbParam.wValley		= (WORD)(50000 / (m_nusbPixRate *125) /4);//ValleyY;
	if( usbParam.wValley < 1 )	// Valley必须大于等于1
		usbParam.wValley	= 1;
	usbParam.wRise			= (WORD)rcFrame.Height();
	usbParam.wDRise			= (WORD)(wStepY * dRatio);
	usbParam.wPeak			= PeakY;
	usbParam.wFall			= FallY;
//	usbParam.wDFall			= DFallY;//(WORD)(rcFrame.Height()/usbParam.wFall *wStepX);
	if( usbParam.wFall > 0 )
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise /usbParam.wFall);
	else
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise -1);
//	usbParam.wDirection		= 1;
	data.ScanRampParamY		= usbParam;
}
/*
// 设置四段扫描波各参数
void CScan::USB_SetRampParam2(
//			USB_ScanFunction&	Function,	
			CSize				szReso,
			CRect rcFrame, USB_InitData& data,
			double	dHV )
{
	if( m_bDebugScan )
	{
		data.ScanRampParamX = m_usbDebugParamX;
		data.ScanRampParamY = m_usbDebugParamY;
		return;
	}

	m_nXOffset	= szReso.cx /8;
	m_nYOffset	= szReso.cy /8;
	// 始终使用满量程2的14次方，步距根据当前分辨率设置
//	WORD	wFullRange	= (WORD)pow(2, 14);
	WORD	wPoint		= 0;
	// 取样点数取分辨率中的大值
	if( (szReso.cx+m_nXOffset) > (szReso.cy+m_nYOffset) )
		wPoint	= szReso.cx +m_nXOffset;
	else
		wPoint	= szReso.cy +m_nYOffset;
	// 根据取样点数得到步距，X/Y扫描波步距相同；否则图像变形
	WORD	wStep		= (WORD)(g_dFullRange2 / wPoint );
	double	dRatio		= sqrt( dHV / 30.0 );

	double dOffset;
	USB_InitScanRampParam usbParam;
	// 以30KV时常规扫描的中心为定点，而不是以0点为定点
	// X扫描波
	dOffset					= rcFrame.left + g_wFullRange *( 1 -dRatio ) /2 + ( g_wFullRange -g_dFullRange2 ) /2 -1;
	if( dOffset < 0 )
		dOffset = 0;
//	usbParam.wOffset		= (WORD)(dOffset *wStep);
	if( (szReso.cy+m_nYOffset) > (szReso.cx+m_nXOffset) )
		usbParam.wOffset	= (WORD)(dOffset +(rcFrame.left +((szReso.cy+m_nYOffset) -(szReso.cx+m_nXOffset)) /2) *wStep*dRatio);
	else
		usbParam.wOffset	= (WORD)(dOffset +rcFrame.left *wStep*dRatio);
	usbParam.wValley		= (WORD)(50000 / (m_nusbPixRate *125) /4);
	if( usbParam.wValley < 1 )	// Valley必须大于等于1
		usbParam.wValley	= 1;
	usbParam.wRise			= (WORD)(rcFrame.Width() +m_nXOffset);
	usbParam.wDRise			= (WORD)(wStep * dRatio);
	usbParam.wPeak			= PeakX;
	usbParam.wFall			= FallX;
//	usbParam.wDFall			= DFallX;//(WORD)(rcFrame.Width()/usbParam.wFall *wStepX);
	if( usbParam.wFall > 0 )
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise /usbParam.wFall);
	else
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise -1);
//	usbParam.wDirection		= 0;
	data.ScanRampParamX		= usbParam;

	// Y扫描波
	dOffset					= rcFrame.top + g_wFullRange *( 1 -dRatio ) /2 + ( g_wFullRange -g_dFullRange2 ) /2 -1;
	if( dOffset < 0 )
		dOffset = 0;
//	usbParam.wOffset		= (WORD)(dOffset *wStep);
	if( (szReso.cx+m_nXOffset) > (szReso.cy+m_nYOffset) )
		usbParam.wOffset	= (WORD)(dOffset +(rcFrame.top +((szReso.cx +m_nXOffset) -(szReso.cy +m_nYOffset)) /2) *wStep*dRatio);
	else
		usbParam.wOffset	= (WORD)(dOffset +rcFrame.top *wStep*dRatio);
	usbParam.wValley		= (WORD)(50000 / (m_nusbPixRate *125) /4 /usbParam.wRise );
	if( usbParam.wValley < 1 )	// Valley必须大于等于1
		usbParam.wValley	= 1;
	usbParam.wRise			= (WORD)(rcFrame.Height() +m_nYOffset);
	usbParam.wDRise			= (WORD)(wStep * dRatio);
	usbParam.wPeak			= PeakY;
	usbParam.wFall			= FallY;
//	usbParam.wDFall			= DFallY;//(WORD)(rcFrame.Height()/usbParam.wFall *wStepX);
	if( usbParam.wFall > 0 )
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise /usbParam.wFall);
	else
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise -1);
//	usbParam.wDirection		= 1;
	data.ScanRampParamY		= usbParam;
}
*/
void CScan::USB_SetRampParam2(
//			USB_ScanFunction&	Function,	
			CSize				szReso,
			CRect rcFrame, USB_InitData& data,
			double	dHV )
{
	if( m_bDebugScan )
	{
		data.ScanRampParamX = m_usbDebugParamX;
		data.ScanRampParamY = m_usbDebugParamY;
		return;
	}

	if( szReso.cx /16 + szReso.cx > 4095 )
		m_nXOffset = 4095 +1 -szReso.cx;
	else
		m_nXOffset	= szReso.cx /16;
	m_nYOffset	= 0;
	if( !CUsbDI16A::Instance().USB_isReady() )
	{
		m_nXOffset	= 0;
		m_nYOffset	= 0;
	}

	// 始终使用满量程2的14次方，步距根据当前分辨率设置
//	WORD	wFullRange	= (WORD)pow(2, 14);
	WORD	wPoint		= 0;
	// 取样点数取分辨率中的大值
	if( (szReso.cx+m_nXOffset) > (szReso.cy+m_nYOffset) )
		wPoint	= szReso.cx +m_nXOffset;
	else
		wPoint	= szReso.cy +m_nYOffset;
	// 根据取样点数得到步距，X/Y扫描波步距相同；否则图像变形
	double	dRatio		= sqrt( dHV / 30.0 );
	WORD	wStep		= (WORD)(g_dFullRange2 *dRatio/ wPoint );

	double dOffset;
	USB_InitScanRampParam usbParam;
	// 以30KV时常规扫描的中心为定点，而不是以0点为定点
	// X扫描波
	dOffset	= szReso.cx/2 +m_nXOffset -rcFrame.left;
	usbParam.wOffset		= (WORD)(g_wFullRange/2 - dOffset *wStep);
	usbParam.wValley		= (WORD)(100000 / (m_nusbPixRate *125) /4);
	if( usbParam.wValley < 1 )	// Valley必须大于等于1
		usbParam.wValley	= 1;
	usbParam.wRise			= (WORD)(rcFrame.Width() +m_nXOffset);
	usbParam.wDRise			= (WORD)(wStep);
	usbParam.wPeak			= PeakX;
	usbParam.wFall			= FallX;
	if( usbParam.wFall > 0 )
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise /usbParam.wFall);
	else
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise -1);
	data.ScanRampParamX		= usbParam;

	// Y扫描波
	dOffset = (szReso.cy +m_nYOffset)/2 -rcFrame.top;
	usbParam.wOffset		= (WORD)(g_wFullRange/2 - dOffset *wStep);
	usbParam.wValley		= (WORD)(50000 / (m_nusbPixRate *125) /4 /usbParam.wRise );
	if( usbParam.wValley < 1 )	// Valley必须大于等于1
		usbParam.wValley	= 1;
	usbParam.wRise			= (WORD)(rcFrame.Height() +m_nYOffset);
	usbParam.wDRise			= (WORD)(wStep);
	usbParam.wPeak			= PeakY;
	usbParam.wFall			= FallY;
	if( usbParam.wFall > 0 )
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise /usbParam.wFall);
	else
		usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise -1);
	data.ScanRampParamY		= usbParam;
}

void CScan::USB_SetRampParam_CoadjustHV( double dHV )
{
	// 高压统调时若正在采集则只统调扫描波形参数
	if( m_usbFunction == USB_SF_SpotScan )
		return;	// 点扫描不设置扫描波形参数

	USB_InitParam	param;
	// 计算扫描波形参数
	if( m_bDebugScan )
	{
		param.Data.ScanRampParamX = m_usbDebugParamX;
		param.Data.ScanRampParamY = m_usbDebugParamY;
	}
	else
	{
		CRect	rcFrame	= m_rcDestFrame;
		CSize	szReso	= m_sizeReso;
		if( szReso.cx /16 + szReso.cx > 4095 )
			m_nXOffset = 4095 +1 -szReso.cx;
		else
			m_nXOffset	= szReso.cx /16;
		m_nYOffset	= 0;
		// 始终使用满量程2的14次方，步距根据当前分辨率设置
//		WORD	wFullRange	= (WORD)pow(2, 14);
		WORD	wPoint		= 0;
		// 取样点数取分辨率中的大值
		if( (szReso.cx+m_nXOffset) > (szReso.cy+m_nYOffset) )
			wPoint	= szReso.cx +m_nXOffset;
		else
			wPoint	= szReso.cy +m_nYOffset;
		// 根据取样点数得到步距，X/Y扫描波步距相同
		double	dRatio		= sqrt( dHV / 30.0 );
		WORD	wStep		= (WORD)(g_dFullRange2 *dRatio/ wPoint );

		double dOffset;
		USB_InitScanRampParam usbParam;
		// 以30KV时常规扫描的中心为定点，而不是以0点为定点
		// X扫描波
		dOffset	= szReso.cx/2 +m_nXOffset -rcFrame.left;
		if( dOffset < 0 )
			dOffset = 0;
		usbParam.wOffset		= (WORD)(g_wFullRange/2 - dOffset *wStep);
		usbParam.wValley		= (WORD)(100000 / (m_nusbPixRate *125) /4);
		if( usbParam.wValley < 1 )	// Valley必须大于等于1
			usbParam.wValley	= 1;
		usbParam.wRise			= (WORD)(rcFrame.Width() +m_nXOffset);
		usbParam.wDRise			= (WORD)(wStep);
		usbParam.wPeak			= PeakX;
		usbParam.wFall			= FallX;
//		usbParam.wDFall			= DFallX;//(WORD)(rcFrame.Width()/usbParam.wFall *wStepX);
		if( usbParam.wFall > 0 )
			usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise /usbParam.wFall);
		else
			usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise -1);
//		usbParam.wDirection		= 0;
		param.Data.ScanRampParamX		= usbParam;

		// Y扫描波
		dOffset = (szReso.cy +m_nYOffset)/2 -rcFrame.top;
		if( dOffset < 0 )
			dOffset = 0;
		usbParam.wOffset		= (WORD)(g_wFullRange/2 - dOffset *wStep);
		usbParam.wValley		= (WORD)(50000 / (m_nusbPixRate *125) /4 /usbParam.wRise );
		if( usbParam.wValley < 1 )	// Valley必须大于等于1
			usbParam.wValley	= 1;
		usbParam.wRise			= (WORD)(rcFrame.Height() +m_nYOffset);
		usbParam.wDRise			= (WORD)(wStep);
		usbParam.wPeak			= PeakY;
		usbParam.wFall			= FallY;
//		usbParam.wDFall			= DFallY;//(WORD)(rcFrame.Height()/usbParam.wFall *wStepX);
		if( usbParam.wFall > 0 )
			usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise /usbParam.wFall);
		else
			usbParam.wDFall		= (WORD)(usbParam.wRise *usbParam.wDRise -1);
//		usbParam.wDirection		= 1;
		param.Data.ScanRampParamY		= usbParam;
	}

	// 设置扫描波形参数
	unsigned char funcNo;
	if( m_usbFunction != USB_SF_LineScan_VERT )	// 水平线扫描时不设置X波形参数
	{
		// 设置X扫描波形7参数
		funcNo = USB_SetXoffset;
		for (int i=0; i<7; i++)
			USB_WriteToBoard( funcNo +i, TRUE, param.wData[i] );
		USB_WriteToBoard( USB_UpdateXramp, FALSE, 0 );
	}

	if( m_usbFunction != USB_SF_LineScan_HORZ )	// 垂直线扫描时不设置Y波形参数
	{
		// 设置Y扫描波形7参数
		funcNo = USB_SetYoffset;
		for (int i=0; i<7; i++)
			USB_WriteToBoard( funcNo +i, TRUE, param.wData[i +7] );
		USB_WriteToBoard( USB_UpdateYramp, FALSE, 0 );
	}
}

BOOL CScan::USB_ScanStop()
{
	///////////////////////////////////////////////////////
	m_criticalOperation.Lock();
	///////////////////////////////////////////////////////

	USB_ClearScan();

	/////////////////////////////////////////////////////////////////
	// 为使扫描线圈的电流最小，在停止扫描时，
	// 要让电子束在全幅区域的中心点16x16小范围扫描
	// 要让电子束以当前分辨率，以4us点时钟做常规扫描
	USB_InitData	data;
	ZeroMemory( &data, sizeof(USB_InitData) );
	// 设置扫描区域为全幅区域中心的16x16范围
	CRect rc = CRect(0, 0, m_sizeReso.cx, m_sizeReso.cy);
	if( rc.IsRectEmpty() )
		return TRUE;
	rc.left		= rc.left +rc.Width() /2 -8;
	if( rc.left < 0 )
		rc.left = 0;
	rc.right	= rc.left +16;
	rc.top		= rc.top +rc.Height() /2 -8;
	if( rc.top < 0 )
		rc.top = 0;
	rc.bottom	= rc.top +16;
	USB_SetRampParam2( m_sizeReso, rc, data );
	// 设置扫描方式为常规扫描
	m_usbFunction = USB_SF_RasterScan;
	// 设置扫描速度为常用扫描速度1us，点时钟4us
	data.PixRate	= 2;
	// 将参数写入采集卡
	USB_SetInitData( &data );
	// 设置为常规扫描
	USB_WriteToBoard( USB_ScanModeNormal, FALSE, 0 );
	// 关闭视频
	CUsbDI16A::Instance().USB_VideoON();
	// 发起扫描
	USB_WriteToBoard( USB_StartScan, FALSE, 0 );

	m_criticalOperation.Unlock();

	return TRUE;
}

BOOL CScan::StopScan()
{
	m_criticalOperation.Lock();

	USB_ClearScan();

	USB_NormalScan();

	m_criticalOperation.Unlock();

	return TRUE;
}

BOOL CScan::USB_ClearScan()
{
	// close Worker
//	g_lastTickCount = GetTickCount();
	if ( m_pScanCtrl != NULL )
	{
		m_pScanCtrl->KillThread();
		m_pScanCtrl = NULL;
	}

	// Stop scan
	USB_WriteToBoard( USB_StopScan, FALSE, 0 );

	// 关闭端口
	CUsbDI16A::Instance().USB_StreamClose();
	// 将FIFO清空
	CUsbDI16A::Instance().USB_ClearFIFO();
	// 关闭端口
//		CUsbDI16A::Instance().USB_StreamClose();
	Sleep(200);

	if( CSemVirtualMCICard::Instance().isReady() )
		CSemVirtualMCICard::Instance().ClearData();

	return TRUE;
}

void CScan::USB_NormalScan()
{
	// 停止采集
	USB_ClearScan();

	// 将参数写入采集卡
//	USB_SetInitData( &data );
//	USB_WriteToBoard( USB_SetDefaultRamps, FALSE, 0 );	// 设置为默认扫描波
}

void CScan::Get_DataSize_IntervalEx(
			CSize					szReso,
			unsigned short			nPixRate,
			DWORD &dwDataSize, DWORD &dwInterval )
{
	if( nPixRate < 80 )	// PixClk = 5us,为20us点时钟
		dwDataSize = (30 * 1000 * 1000) / (nPixRate *125);
	else
 		dwDataSize = (50 * 1000 * 1000) / (nPixRate *125);	// 50ms能采集到多少数据，1个点4个数据
 	dwDataSize /= LOOP_COUNTS;
	if( dwDataSize > 30000 )
		dwDataSize = 30000;
	/////////////////////////////////////////////
	dwInterval = (nPixRate *125) * dwDataSize / 1000 / 1000;
	if( dwInterval > ThreadSwitchOptionTime_USB )
		dwInterval -= ThreadSwitchOptionTime_USB;

	if( nPixRate < 80 )
		dwInterval = 0;
	if( !CUsbDI16A::Instance().USB_isReady() )
		dwInterval = 0;
}

void CScan::Get_DataSize_Interval_DebugEx(DWORD &dwDataSize, DWORD &dwInterval )
{
	double dReso = sqrt( 1.0*m_rcDestFrame.Width()*m_rcDestFrame.Height() );

	/////////////////////////////////////////////
	if( m_nusbDebugPixRate < 40 )	// N=40,为20us点时钟
		dwDataSize = (30 * 1000 * 1000) / (m_nusbDebugPixRate * 125);
	else
		dwDataSize = (50 * 1000 * 1000) / (m_nusbDebugPixRate * 125);	// 50ms能采集到多少数据，1个点4个数据
	dwDataSize /= LOOP_COUNTS;
	if( dwDataSize > 30000 )
		dwDataSize = 30000;
	/////////////////////////////////////////////
	dwInterval = (m_nusbDebugPixRate *125) * dwDataSize / 1000 / 1000;
	if ( dwInterval > ThreadSwitchOptionTime_USB )
		dwInterval -= ThreadSwitchOptionTime_USB;

	if( m_nusbDebugPixRate < 40 )
		dwInterval = 0;
}

void CScan::Get_DataSize_Interval_Debug(
			DWORD &dwDataSize, DWORD &dwInterval )
{
	double dReso = sqrt( 1.0*m_rcDestFrame.Width()*m_rcDestFrame.Height() );
	// 根据分辨率估算每次读取的数据量
	// 根据点逗留时间设置读取数据时间间隔
	dwDataSize = 8192;

	if( m_nusbDebugPixRate > 16 )
		dwDataSize /= 2;
	if( m_nusbDebugPixRate > 40 )
		dwDataSize /= 2;
	if( m_nusbDebugPixRate > 128 )
		dwDataSize /= 2;
	if( m_nusbDebugPixRate > 280 )
		dwDataSize /= 2;
	if( m_nusbDebugPixRate > 720 )
		dwDataSize /= 2;
	if( m_nusbDebugPixRate > 960 )
		dwDataSize /= 2;
	if( m_nusbDebugPixRate < 8 )
		dwDataSize *= 2;
	if( m_nusbDebugPixRate < 4 )
		dwDataSize *= 2;
	if( m_nusbDebugPixRate < 2 )
		dwDataSize *= 2;

	if( m_nusbDebugPixRate < 4 )	// 250ns & 125ns
		dwDataSize *= 5;
	if( dwDataSize > 50000 )
		dwDataSize = 1024*50;

	if( m_nusbDebugPixRate < 2 )	// 125ns反而不要太大的数据量
		dwDataSize = 1024*25;

	dwInterval = (DWORD)(0.5* m_nusbDebugPixRate * dwDataSize / 1000 / 1000);
	if ( dwInterval > ThreadSwitchOptionTime_USB )
		dwInterval -= ThreadSwitchOptionTime_USB;
	if( dwInterval > 2 )		// 不加存储数据，读FIFO约1ms，显示0-1ms；加存储数据，视数据量多少，读FIFO约4-15ms，显示1ms
		dwInterval -= 2;

	if( m_nusbDebugPixRate < 40 && dReso < 1025 )
		dwInterval = 0;
}

BOOL CScan::USB_SFRasterScan(
			USB_ScanFunction&			Function,	
			CSize&						szReso,
			unsigned short&				nPixRate,	// 像素率 = 点停留时间 /4，必须为整数
			USB_ExtrParamRasterScan&	Param,
			double						dHV )
{
	BOOL	blRet = TRUE;
	USB_InitData	data;
	USB_ScanFunction fbak = m_usbFunction;

	// 停止采集
	USB_ClearScan();
	ZeroMemory( &data, sizeof(USB_InitData) );

	m_usbFunction			= fbak;
	// 设置采集参数
	m_rcDestFrame.left		= Param.dwStartX;
	m_rcDestFrame.right		= Param.dwStopX;
	m_rcDestFrame.top		= Param.dwStartY;
	m_rcDestFrame.bottom	= Param.dwStopY;

	data.PixRate			= nPixRate;
	if( data.PixRate < 0 )
		data.PixRate = 8;

	if( m_bDebugScan )
		Get_DataSize_Interval_DebugEx( m_dwExpectantDMABlockSize, m_dwExpectantInterval );
	else
		Get_DataSize_IntervalEx( szReso, data.PixRate, m_dwExpectantDMABlockSize, m_dwExpectantInterval );

	USB_SetRampParam2( szReso, m_rcDestFrame, data, dHV );

	// 设置同步信号
//	USB_WriteToBoard( USB_MainsLock_ON, FALSE, 0 );
	// 设置同步信号
//	USB_WriteToBoard( USB_MainsLock_FrameSync, FALSE, 0 );
	// 将参数写入采集卡
	USB_SetInitData( &data );

	switch (Function)
	{
	case USB_SF_RasterScan:
	case USB_SF_AreaScan:
		USB_StartRasterScan();
		break;

	default:
		USB_StartRasterScan();
		break;
	}

	return blRet;
}

BOOL CScan::USB_SFLineScan(
			CSize&					szReso,
			unsigned short&			nPixRate,	// 像素率 = 点停留时间 /4，必须为整数
			USB_ExtrParamLineScan&	Param,
			double					dHV )
{
	BOOL		blRet = TRUE;
	USB_InitData	data;
	USB_ScanFunction fbak = m_usbFunction;

	// 停止采集
	USB_ClearScan();
	ZeroMemory( &data, sizeof(USB_InitData) );

	m_usbFunction			= fbak;
	// 设置采集参数
	m_rcDestFrame.left		= Param.dwStartX;
	m_rcDestFrame.right		= Param.dwStopX;
	m_rcDestFrame.top		= Param.dwStartY;
	m_rcDestFrame.bottom	= Param.dwStopY;

	data.PixRate			= nPixRate;
	if( data.PixRate < 0 )
		data.PixRate = 8;

	if( m_bDebugScan )
		Get_DataSize_Interval_DebugEx( m_dwExpectantDMABlockSize, m_dwExpectantInterval );
	else
		Get_DataSize_IntervalEx( szReso, data.PixRate, m_dwExpectantDMABlockSize, m_dwExpectantInterval );

	CRect rc = CRect( 0, 0, m_sizeReso.cx, m_sizeReso.cy );
	USB_SetRampParam2( szReso, rc, data, dHV );
//	USB_SetRampParam2( szReso, m_rcDestFrame, data, dHV );

	// 将参数写入采集卡
	USB_SetInitData( &data );
	if( m_usbFunction == USB_SF_LineScan_HORZ )
	{
		// Freeze Y
		USB_WriteToBoard( USB_FreezeY, TRUE, (WORD)Param.dwStartY );
	}
	else
	{
		// Freeze X
		USB_WriteToBoard( USB_FreezeX, TRUE, (WORD)Param.dwStartX );
	}

	USB_StartLineScan();

	return blRet;
}

BOOL CScan::USB_SFSpotScan(
			CSize&					szReso,
			unsigned short&			nPixRate,	// 像素率 = 点停留时间 /4，必须为整数
			USB_ExtrParamLineScan&	Param,
			double					dHV )
{
	BOOL		blRet = TRUE;
	USB_InitData	data;
	USB_ScanFunction fbak = m_usbFunction;

	// 停止采集
	USB_ClearScan();
	ZeroMemory( &data, sizeof(USB_InitData) );

	m_usbFunction			= fbak;
	// 设置采集参数
	m_rcDestFrame.left		= Param.dwStartX;
	m_rcDestFrame.right		= Param.dwStopX;
	m_rcDestFrame.top		= Param.dwStartY;
	m_rcDestFrame.bottom	= Param.dwStopY;

	data.PixRate			= nPixRate;
	if( data.PixRate < 0 )
		data.PixRate = 8;

	CRect rc = CRect( 0, 0, m_sizeReso.cx, m_sizeReso.cy );
	USB_SetRampParam2( m_sizeReso, rc, data, dHV );

	// 将参数写入采集卡
	USB_SetInitData( &data );
	// Freeze X & Y
	USB_WriteToBoard( USB_FreezeX, TRUE, (WORD)Param.dwStartX );
	USB_WriteToBoard( USB_FreezeY, TRUE, (WORD)Param.dwStartY );

	USB_StartSpotScan();

	return blRet;
}

BOOL CScan::USB_SFSyncScan(
						   CSize&					szReso,
						   unsigned short&			nPixRate,	// 像素率 = 点停留时间 /4，必须为整数
						   USB_ExtrParamRasterScan&	Param,
						   double					dHV )
{
	BOOL		blRet = TRUE;
	USB_InitData	data;
	USB_ScanFunction fbak = m_usbFunction;

	// 停止采集
	USB_ClearScan();
	ZeroMemory( &data, sizeof(USB_InitData) );

	m_usbFunction			= fbak;
	// 设置采集参数
	m_rcDestFrame.left		= Param.dwStartX;
	m_rcDestFrame.right		= Param.dwStopX;
	m_rcDestFrame.top		= Param.dwStartY;
	m_rcDestFrame.bottom	= Param.dwStopY;

	data.PixRate			= nPixRate;
	if( data.PixRate < 0 )
		data.PixRate = 8;

	if( m_bDebugScan )
		Get_DataSize_Interval_DebugEx( m_dwExpectantDMABlockSize, m_dwExpectantInterval );
	else
		Get_DataSize_IntervalEx( szReso, data.PixRate, m_dwExpectantDMABlockSize, m_dwExpectantInterval );

	USB_SetRampParam2( szReso, m_rcDestFrame, data, dHV );

	// 设置同步信号
	USB_WriteToBoard( USB_MainsLock_ON, FALSE, 0 );
	// 将参数写入采集卡
	USB_SetInitData( &data );

	USB_StartSyncScan();

	return blRet;
}

BOOL CScan::USB_SetInitData( USB_InitData* pData )
{
	BOOL	blRet = TRUE;

	// 设置点逗留时间 pixel rate
	if( m_bDebugScan )
		USB_WriteToBoard( USB_SetPixelRate, TRUE, m_nusbDebugPixRate );
	else
		USB_WriteToBoard( USB_SetPixelRate, TRUE, pData->PixRate );

	// 设置数据存储模式
	USB_WriteToBoard( USB_VideoXYZ, FALSE, 0 );

	// 设置探测器储模式
	USB_WriteToBoard( USB_DuelDet, FALSE, 0 );

	unsigned char funcNo;
	USB_InitParam* pParam;

	pParam = (USB_InitParam *)pData;

	// 设置X扫描波形7参数
	funcNo = USB_SetXoffset;
	for (int i=0; i<7; i++)
		USB_WriteToBoard( funcNo +i, TRUE, pParam->wData[i] );
	USB_WriteToBoard( USB_UpdateXramp, FALSE, 0 );

	// 设置Y扫描波形7参数
	funcNo = USB_SetYoffset;
	for (int i=0; i<7; i++)
		USB_WriteToBoard( funcNo +i, TRUE, pParam->wData[i +7] );
	USB_WriteToBoard( USB_UpdateYramp, FALSE, 0 );

	// 滤波器值
	// 照相时将当前通道组的Filter设置为5
	if( m_usbFunction == USB_SF_AnalogPhoto )
	{
		WORD wValue = (WORD)( (m_nMCIdetPair << 8) | 0x05 );
		USB_WriteToBoard( USB_SetDetFilter, TRUE, wValue );
	}
	else
	{
		// 将当前通道组的Filter恢复
		WORD wValue = (WORD)( (m_nMCIdetPair << 8) | (BYTE)(m_nMCIfilter[m_nMCIdetPair]) );
		USB_WriteToBoard( USB_SetDetFilter, TRUE, wValue );
	}

	// 注意：要加上通知下位机切换Y的驱动时钟（平时为行时钟，对角线式线扫描时应切换为像素时钟）
	return blRet;
}

// 向MCI板写数据
// bOpcode:		操作码
// blOperand:	是否有操作数
// wData:		16 bits操作数
BOOL CScan::USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData )
{
	return CUsbDI16A::Instance().USB_WriteToBoard( bOpcode, blOperand, wData );
}

BOOL CScan::USB_SetGraph( 
						USB_ScanFunction&		Function,		// 工作方式
						CSize&					szReso,			// 分辨率
						USB_ScanParam&			Param )			// 扫描参数
{
			BOOL			blRet = TRUE;
	static	USB_ScanFunction	USB_LastClearScreenFunction = Function;
	static	CRect			rcDestLast;
			CRect			rcDest, rcDest2;
			CString			csText;
			BOOL			bNew;
			CPoint			ptPos0, ptPos1;

	if ( USB_LastClearScreenFunction != Function )
		bNew = TRUE;
	else
		bNew = FALSE;

	switch (Function)
	{
	case USB_SF_Stop:
		blRet = TRUE;
		break;

	case USB_SF_RasterScan:
		rcDest.left		= Param.RasterScan.dwStartX;
		rcDest.top		= Param.RasterScan.dwStartY;
		rcDest.right	= Param.RasterScan.dwStopX;
		rcDest.bottom	= Param.RasterScan.dwStopY;

		if( bNew )
		{
			USB_LastClearScreenFunction = Function;
			CSemVirtualMCICard::Instance().GraphClear();
		}

		rcDestLast = rcDest;
		break;

	case USB_SF_SpotScan:
		rcDest.left		= Param.RasterScan.dwStartX;
		rcDest.top		= Param.RasterScan.dwStartY;
		rcDest.right	= Param.RasterScan.dwStopX;
		rcDest.bottom	= Param.RasterScan.dwStopY;

		if( bNew )
		{
			USB_LastClearScreenFunction = Function;
			CSemVirtualMCICard::Instance().GraphClear();
		}

		if( !bNew )
		{
			// 擦除原来的标记
			ptPos0.x = rcDestLast.left;
			ptPos0.y = rcDestLast.top;
			ptPos1 = ptPos0;
			ptPos0.x -= 6;
			ptPos1.x += 8;
			CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1 );

			ptPos0.x = rcDestLast.left;
			ptPos0.y = rcDestLast.top;
			ptPos1 = ptPos0;

			ptPos0.y -= 6;
			ptPos1.y += 8;
			CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1 );

/*			if( m_bSplit )
			{
				ptPos0.x = rcDestLast.left +szReso.cx;
				ptPos0.y = rcDestLast.top;
				ptPos1 = ptPos0;
				ptPos0.x -= 6;
				ptPos1.x += 8;
				CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1 );

				ptPos0.x = rcDestLast.left +szReso.cx;
				ptPos0.y = rcDestLast.top;
				ptPos1 = ptPos0;

				ptPos0.y -= 6;
				ptPos1.y += 8;
				CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1 );
			}*/
		}
		ptPos0.x = rcDest.left;
		ptPos0.y = rcDest.top;
		ptPos1 = ptPos0;
		ptPos0.x -= 6;
		ptPos1.x += 8;
		CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1, RGB(255,0,0) );

		ptPos0.y += 1;
		ptPos1.y += 1;
		CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1, RGB(255,0,0) );
		/////////////////////////////////////////////////////////////

		ptPos0.x = rcDest.left;
		ptPos0.y = rcDest.top;
		ptPos1 = ptPos0;
		ptPos0.y -= 6;
		ptPos1.y += 8;
		CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1, RGB(255,0,0) );

		ptPos0.x += 1;
		ptPos1.x += 1;
		CSemVirtualMCICard::Instance().GraphDrawLine( ptPos0, ptPos1, RGB(255,0,0) );

		rcDestLast = rcDest;
		break;

	case USB_SF_LineScan_HORZ:
	case USB_SF_LineScan_VERT:
	case USB_SF_AreaScan:
		rcDest.left		= Param.RasterScan.dwStartX;
		rcDest.top		= Param.RasterScan.dwStartY;
		rcDest.right	= Param.RasterScan.dwStopX;
		rcDest.bottom	= Param.RasterScan.dwStopY;

		if( bNew )
		{
			USB_LastClearScreenFunction = Function;
			CSemVirtualMCICard::Instance().GraphClear();
		}
		if( !bNew )
		{
			rcDest2 = rcDestLast;
			// 擦除原来的标记
			CSemVirtualMCICard::Instance().GraphDrawRect( rcDestLast );
			if( szReso.cx > 1024 )
			{
				rcDestLast.OffsetRect( 0, 1 );
				CSemVirtualMCICard::Instance().GraphDrawRect( rcDestLast );
			}
			if( m_bSplit )
			{
				rcDest2.left	+= szReso.cx;
				rcDest2.right	+= szReso.cx;
				CSemVirtualMCICard::Instance().GraphDrawRect( rcDest2 );
				if( szReso.cx > 1024 )
				{
					rcDestLast.OffsetRect( 0, 1 );
					CSemVirtualMCICard::Instance().GraphDrawRect( rcDest2 );
				}
			}
		}

		CSemVirtualMCICard::Instance().GraphDrawRect( rcDest, RGB(0,255,0) );
		rcDestLast = rcDest;
		if( szReso.cx > 1024 )
		{
			rcDest.OffsetRect( 0, 1 );
			CSemVirtualMCICard::Instance().GraphDrawRect( rcDest, RGB(0,255,0) );
		}
		if( m_bSplit )
		{
			rcDest2 = rcDest;
			rcDest2.left	+= szReso.cx;
			rcDest2.right	+= szReso.cx;
			CSemVirtualMCICard::Instance().GraphDrawRect( rcDest2, RGB(0,255,0) );
			if( szReso.cx > 1024 )
			{
				rcDest2.OffsetRect( 0, 1 );
				CSemVirtualMCICard::Instance().GraphDrawRect( rcDest2, RGB(0,255,0) );
			}
		}

		break;

	case USB_SF_AnalogPhoto:
		break;

	default:
		break;
	}

	if( m_bSplit )
	{
		rcDest.left		= szReso.cx -1;
		rcDest.right	= szReso.cx;
		rcDest.top		= 0;
		rcDest.bottom	= szReso.cy;
		CSemVirtualMCICard::Instance().GraphDrawRect( rcDest, RGB(0,255,0) );
	}
	return blRet;
}

