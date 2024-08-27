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
	// ����ͬ���ź�
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

	//======2009.06.01_���ɨ��λ��ָʾ====================
	// �޸ı�ʾ��2009.06.01_���ɨ��λ��ָʾ
	// �޸����ڣ�2009.06.01
	// �޸���  ��J.Y.Ma
	// �޸�ԭ����ȷɨ��λ�ã����ڲ鿴�Ƿ�����ɨ��

	// �����޸�ǰ���븱��
//	/*-----------------------------------------------------
	OutputData();
//	Sleep(0);
	//-----------------------------------------------------*/
/*
	// �����޸ĺ����
	//-----------------------------------------------------
	static CRect rcDest	= m_rcDestFrame;
	rcDest.right	= rcDest.left + 8;
	rcDest.top		= m_dwLineCounter;

	//������һ��λ����
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

	//����λ����
	if( m_pImageCard != NULL && m_pImageCard->isReady() )
		m_pImageCard->PhotoIndication( rcDest, RGB(255,255,255), FALSE );
	if( m_pMCICard != NULL && CSemVirtualMCICard::Instance().isReady() )
		CSemVirtualMCICard::Instance().PhotoIndication( rcDest, RGB(255,255,255), FALSE );
*/	//======2009.06.01_���ɨ��λ��ָʾ====================
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
