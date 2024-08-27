// LineScan.cpp : implementation file
//

#include "stdafx.h"
#include "LineScan.h"
// USB
#include "UsbDI16A.h"
#include "../GlobalComponent/GraphBuffManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineScan

IMPLEMENT_DYNCREATE(CLineScan, CBasisScan)

CLineScan::CLineScan()
{
	m_pBuff					= NULL;
	m_pBuffUSB				= NULL;
	m_lBytes_transferred	= 0;

	m_dwDMABlockSize		= 0;
}

CLineScan::~CLineScan()
{
	if ( m_pBuff != NULL )
	{
		GlobalFree( m_pBuff );
		m_pBuff	= NULL;
	}

	if ( m_pBuffUSB != NULL )
	{
		delete [] m_pBuffUSB;
		m_pBuffUSB	= NULL;
	}
}

BOOL CLineScan::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return 	CBasisScan::InitInstance();
}

int CLineScan::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CBasisScan::ExitInstance();
}

BEGIN_MESSAGE_MAP(CLineScan, CBasisScan)
	//{{AFX_MSG_MAP(CLineScan)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineScan message handlers

BOOL CLineScan::InitData()
{
	if ( !CBasisScan::InitData() )
		return FALSE;

	m_pBuffUSB	= new unsigned short[LOOP_COUNTS*(m_dwExpectantDMABlockSize +512)];
	m_lOffset	= 0;
	m_lIndex	= 0;

	m_nSharedImageBuffIndex = 0;

	// 将FIFO清空
	CUsbDI16A::Instance().USB_ClearFIFO();

#ifdef _DEBUG_WITHOUT_PCICARD_
	if( !CUsbDI16A::Instance().USB_isReady() )
	{
		m_dwDebugOffsetZ		= 0;
		m_dwDebugOffsetY		= 0;

		m_dwDebugCounts			= m_dwExpectantDMABlockSize /4;				// 一次的数据量相当于多少个像素点的数据
		m_dwDebugCountsX		= m_rcDestFrame.Width();					// 一行多少个点
		m_dwDebugCountsY		= m_dwDebugCounts / m_rcDestFrame.Height();	// 一次的数据量相当于多少行

		ZeroMemory(m_pBuffUSB, sizeof(m_pBuffUSB));
	}
#endif //_DEBUG_WITHOUT_PCICARD_

	return TRUE;
}


void CLineScan::MainLoop()
{
	if ( m_boolFinished )
	{
		Sleep( 13-5 );
		return;
	}

	Sleep( m_dwExpectantInterval );

	if ( !InputData() )
		return;

	OutputData();
}

BOOL CLineScan::InputData()
{
#ifdef _DEBUG_WITHOUT_PCICARD_
	if(!CUsbDI16A::Instance().USB_isReady() )
	{
		unsigned short* pBuff = CGraphBuffMgr::Instance().m_pSharedImageBuff[m_nSharedImageBuffIndex];
		DWORD dwOffset	= 0;
		DWORD i,j;
		for( j=0; j<LOOP_COUNTS; j++ )
		{
			for (i=0; i<m_dwDebugCounts; i++)
			{
				// 因为第一个数据为整个数据包的个数，所以实际数据向后错1
				(pBuff)[dwOffset +i*4 +1] = (WORD)((i % m_dwDebugCountsX) | 0x8000);		// 0x8xxx
				(pBuff)[dwOffset +i*4 +2] = (WORD)(m_rcDestFrame.top | 0x4000);			// 固定值
				(pBuff)[dwOffset +i*4 +3] = (WORD)(((i+m_dwDebugOffsetZ) % 256) << 4);	// 0x0xxx
				(pBuff)[dwOffset +i*4 +4] = (WORD)(((i+m_dwDebugOffsetZ) % 256) << 4);
			}
			(pBuff)[dwOffset] = (WORD)m_dwExpectantDMABlockSize;
			dwOffset = (j+1) *(m_dwExpectantDMABlockSize +512);
		}
		m_dwDebugOffsetZ += 8;
		if( m_dwDebugOffsetZ > 0xFFF0 )
			m_dwDebugOffsetZ = 0;
		m_lBytes_transferred = LOOP_COUNTS * (m_dwExpectantDMABlockSize +512) *2;

		m_nSharedImageBuffIndex++;
		if( m_nSharedImageBuffIndex >= SharedImageBuffCounts )
			m_nSharedImageBuffIndex = 0;

		Sleep( 50 );
		return TRUE;
	}
#endif //_DEBUG_WITHOUT_PCICARD_

	if( CUsbDI16A::Instance().USB_isReady() )
	{
		DWORD dwRet = 0;
		dwRet = CUsbDI16A::Instance().USB_ImageInputStream1( m_dwExpectantDMABlockSize, 
												CGraphBuffMgr::Instance().m_pSharedImageBuff[m_nSharedImageBuffIndex],
												&m_lBytes_transferred, -1 );
		if( dwRet > 0 && m_lBytes_transferred == 0)
			return FALSE;

		m_nSharedImageBuffIndex++;
		if( m_nSharedImageBuffIndex == SharedImageBuffCounts )
			m_nSharedImageBuffIndex = 0;
		return TRUE;
	}

	return TRUE;
}

BOOL CLineScan::OutputData()
{
	if( CSemVirtualMCICard::Instance().isReady() )
		CSemVirtualMCICard::Instance().SetSharedImageBuffIndex( m_nSharedImageBuffIndex );

	return TRUE;
}
