// SyncScan.cpp : implementation file
//

#include "stdafx.h"
#include "SyncScan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSyncScan

IMPLEMENT_DYNCREATE(CSyncScan, CRasterScan)

CSyncScan::CSyncScan()
{
	m_boolOneOff	= FALSE;
	m_boolFinished	= FALSE;
}

CSyncScan::~CSyncScan()
{
	// 设置同步信号
	USB_WriteToBoard( USB_MainsLock_OFF, FALSE, 0 );
}

BOOL CSyncScan::InitInstance()
{
	CRasterScan::InitInstance();	
	
	///////////////////////////////////////////////////////
	CRect rcDest	= m_rcDestFrame;
	rcDest.right	= rcDest.left + 8;
	
	if( CSemVirtualMCICard::Instance().isReady() )
	{
		rcDest.right = rcDest.left +m_rcDestFrame.Width() *8 /1024;
		CSemVirtualMCICard::Instance().PhotoIndication( rcDest, RGB(0,0,0) );
	}
	///////////////////////////////////////////////////////
	
	return FALSE;
}

int CSyncScan::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CRasterScan::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSyncScan, CRasterScan)
	//{{AFX_MSG_MAP(CSyncScan)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSyncScan message handlers

BOOL CSyncScan::InitData()
{
	if ( !CRasterScan::InitData() )
		return FALSE;

	return TRUE;
}

void CSyncScan::MainLoop()
{
	if ( m_boolFinished )
	{
		Sleep( 13-5 );
		return;
	}

// 	if ( m_dwExpectantInterval < 13 )
// 		Sleep( m_dwExpectantInterval );
// 	else
// 		Sleep( 13-5 );
	Sleep( m_dwExpectantInterval );

	if ( !CRasterScan::InputData() )
		return;

	//======2009.06.01_添加扫描位置指示====================
	// 修改标示：2009.06.01_添加扫描位置指示
	// 修改日期：2009.06.01
	// 修改人  ：J.Y.Ma
	// 修改原因：明确扫描位置，便于查看是否正在扫描

	// 本次修改前代码副本
//	/*-----------------------------------------------------
	OutputData();
//	Sleep(0);
	//-----------------------------------------------------*/
/*
	// 本次修改后代码
	//-----------------------------------------------------
	static CRect rcDest	= m_rcDestFrame;
	rcDest.right	= rcDest.left + 8;
	rcDest.top		= m_dwLineCounter;

	//擦除上一次位置线
	if( m_pImageCard != NULL && m_pImageCard->isReady() )
		m_pImageCard->PhotoIndication( m_rcIndication, RGB(0,0,0), FALSE );
	if( m_pMCICard != NULL && CSemVirtualMCICard::Instance().isReady() )
		CSemVirtualMCICard::Instance().PhotoIndication( m_rcIndication, RGB(0,0,0), FALSE );

	OutputData2();

	if ((long)m_dwLineCounter <= rcDest.top)
	{
		rcDest.bottom	= m_rcDestFrame.bottom;
	}
	else
	{
		rcDest.bottom	= m_dwLineCounter +m_rcDestFrame.top;
	}
	m_rcIndication = rcDest;

	//画新位置线
	if( m_pImageCard != NULL && m_pImageCard->isReady() )
		m_pImageCard->PhotoIndication( rcDest, RGB(255,255,255), FALSE );
	if( m_pMCICard != NULL && CSemVirtualMCICard::Instance().isReady() )
		CSemVirtualMCICard::Instance().PhotoIndication( rcDest, RGB(255,255,255), FALSE );
*/	//======2009.06.01_添加扫描位置指示====================
}

BOOL CSyncScan::OutputData()
{
//	if( m_pMCICard != NULL && CSemVirtualMCICard::Instance().isReady() )
	{
//		CSemVirtualMCICard::Instance().PostData( (void*)m_pBuffUSB, LOOP_COUNTS * (m_dwExpectantDMABlockSize +512), m_rcDestFrame );
		CSemVirtualMCICard::Instance().SetSharedImageBuffIndex( m_nSharedImageBuffIndex );
		m_boolFinished = CSemVirtualMCICard::Instance().IsSyncFinish();
	}
	return TRUE;
}
