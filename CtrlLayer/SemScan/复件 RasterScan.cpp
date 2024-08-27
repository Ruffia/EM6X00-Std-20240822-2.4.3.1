// RasterScan.cpp : implementation file
//

#include "stdafx.h"
#include "RasterScan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRasterScan

IMPLEMENT_DYNCREATE(CRasterScan, CBasisScan)

CRasterScan::CRasterScan()
{
#ifdef _DEBUG_RECORD_ERRINFO_
	CString	csError;
	csError.Format(" CRasterScan::CRasterScan()");
	PostErrorInformation( csError );
#endif //_DEBUG_RECORD_ERRINFO_

	m_blRedirectToDump = FALSE;
	m_pBuff	= NULL;
}

CRasterScan::~CRasterScan()
{
	if ( m_pBuff != NULL )
	{
		GlobalFree( m_pBuff );
		m_pBuff	= NULL;
	}

	//======2009.06.01_���ɨ��λ��ָʾ====================
	// �޸ı�ʾ��2009.06.01_���ɨ��λ��ָʾ
	// �޸����ڣ�2009.06.01
	// �޸���  ��J.Y.Ma
	// �޸�ԭ����ȷɨ��λ�ã����ڲ鿴�Ƿ�����ɨ��

	// �����޸�ǰ���븱��
	/*-----------------------------------------------------
	//-----------------------------------------------------*/

	// �����޸ĺ����
	//-----------------------------------------------------
	//ɨ��ֹͣʱ������ɨ��λ����
	if( m_pImageCard->isReady() )
		m_pImageCard->PhotoIndication( m_rcIndication, RGB(0,0,0), FALSE );
	//======2009.06.01_���ɨ��λ��ָʾ====================
}

BOOL CRasterScan::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return CBasisScan::InitInstance();
}

int CRasterScan::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CBasisScan::ExitInstance();
}

BEGIN_MESSAGE_MAP(CRasterScan, CBasisScan)
	//{{AFX_MSG_MAP(CRasterScan)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRasterScan message handlers

BOOL CRasterScan::InitData()
{
	if ( !CBasisScan::InitData() )
		return FALSE;

	m_pBuff = GlobalAlloc( GPTR, SCANFIFOSIZE );
	if (m_pBuff	== NULL)
		return FALSE;
	
	m_dwLineCounter	= 0;

	m_rcSlipWindow.left		= m_rcDestFrame.left;
	m_rcSlipWindow.right	= m_rcDestFrame.right;
	m_rcSlipWindow.top		= m_rcDestFrame.top;
	m_rcSlipWindow.bottom	= m_rcDestFrame.top;

	return TRUE;
}

void CRasterScan::MainLoop()
{
	if ( m_boolFinished )
	{
		Sleep( 13-5 );
		return;
	}

	if ( m_dwExpectantInterval < 13 )
		Sleep( m_dwExpectantInterval );
	else
		Sleep( 13-5 );

	if ( !InputData() )
		return;

	//======2009.06.01_���ɨ��λ��ָʾ====================
	// �޸ı�ʾ��2009.06.01_���ɨ��λ��ָʾ
	// �޸����ڣ�2009.06.01
	// �޸���  ��J.Y.Ma
	// �޸�ԭ����ȷɨ��λ�ã����ڲ鿴�Ƿ�����ɨ��

	// �����޸�ǰ���븱��
	/*-----------------------------------------------------
	OutputData();
	//-----------------------------------------------------*/

	// �����޸ĺ����
	//-----------------------------------------------------
	static CRect rcDest	= m_rcDestFrame;
	rcDest.right	= rcDest.left + 8;
	rcDest.top		= m_dwLineCounter;

	//������һ��λ����
	if( m_pImageCard->isReady() )
		m_pImageCard->PhotoIndication( m_rcIndication, RGB(0,0,0), FALSE );

	OutputData();

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
	if( m_pImageCard->isReady() )
		m_pImageCard->PhotoIndication( rcDest, RGB(255,255,255), FALSE );
	//======2009.06.01_���ɨ��λ��ָʾ====================
}

BOOL CRasterScan::InputData()
{
	DWORD dwLines;

#ifdef _DEBUG_WITHOUT_PCICARD_
	if( !m_pPciCard->isReady() )
	{
		dwLines = 8;
		m_dwDMABlockSize = dwLines * m_rcDestFrame.Width();
		return TRUE;
	}
#endif //_DEBUG_WITHOUT_PCICARD_

	// ���Ĵ���
	ReadParam	Param1;
	ReadParam	Param2;
	m_pPciCard->IoPortReadL( ADDR_10001000B, Param1.dwParam );
	m_pPciCard->IoPortReadL( ADDR_10001001B, Param2.dwParam );
	DWORD	dwFifoCounter = Param1.ADDR_10001000B.FIFO_NUM * 4;
	// ���Ĵ���
	m_pPciCard->IoPortReadL( ADDR_10001000B, Param1.dwParam );
	m_pPciCard->IoPortReadL( ADDR_10001001B, Param2.dwParam );
	DWORD	dwFifoCounterCheck = Param1.ADDR_10001000B.FIFO_NUM * 4;

	// �жϴ�FIFO�Ƿ����
	if ( Param1.ADDR_10001000B.OVERFLOW == 1 )
	{
#ifdef _DEBUG_RECORD_ERRINFO_
		if( !m_boolOverflow )
		{
			CString	csError;
			csError = _T("CRasterScan::InputData() ---- OVERFLOW = 1");
			PostErrorInformation( csError );
		}
#endif //_DEBUG_RECORD_ERRINFO_
		m_boolOverflow = TRUE;
		return FALSE;
	}

	// �ж϶��Ĵ���(��FIFO Size)�Ƿ��ܵ�����
	// dwFifoCounterCheck ������С�� dwFifoCounter 
	// dwFifoCounterCheck Ҳ�����ܴ��� dwFifoCounter too much
	if ( dwFifoCounterCheck < dwFifoCounter )
	{
#ifdef _DEBUG_RECORD_ERRINFO_
		CString	csError;
		csError.Format(" dwFifoCounterCheck(0x%08X) < dwFifoCounter(0x%08X)", dwFifoCounterCheck, dwFifoCounter );
		PostErrorInformation( csError );
#endif //_DEBUG_RECORD_ERRINFO_
		return FALSE;
	}
	else if ( dwFifoCounterCheck > dwFifoCounter + 1024 )
	{
#ifdef _DEBUG_RECORD_ERRINFO_
		CString	csError;
		csError.Format(" dwFifoCounterCheck(0x%08X) > dwFifoCounter(0x%08X) + 1024", dwFifoCounterCheck, dwFifoCounter );
		PostErrorInformation( csError );
#endif //_DEBUG_RECORD_ERRINFO_
		return FALSE;
	}

	// �ж��Ƿ���
	// Param1.ADDR_10001000B.FIFO_NUM => dwFifoCounter
	// Param1.ADDR_10001000B.DACX
	// Param2.ADDR_10001001B.DACY
	// m_rcDestFrame
	// m_dwLineCounter

	// ����������
	if ( dwFifoCounter < m_dwExpectantDMABlockSize )
		return FALSE;
	else if ( dwFifoCounter > m_dwDMABuffSize )
		dwFifoCounter = m_dwDMABuffSize;

	dwLines = dwFifoCounter / m_rcDestFrame.Width();
	m_dwDMABlockSize = dwLines * m_rcDestFrame.Width();
	if (m_dwDMABlockSize <= 0)
		return FALSE;

	// ��PCI��������
	if ( !m_pPciCard->ImageInputStream( m_pBuff, m_dwDMABlockSize ) )
		return FALSE;

	return TRUE;
}

BOOL CRasterScan::OutputData()
{
	DWORD dwLines			= m_dwDMABlockSize / m_rcDestFrame.Width();
	DWORD dwDestFrameHeight = m_rcDestFrame.Height();

	// �������̫�࣬ѡ����̫Сֻ������һЩ����
	if ( dwDestFrameHeight > 1 )
	{
		dwLines %= dwDestFrameHeight;
	}
	else
	{
		dwLines = 1;
		m_dwLineCounter = 0;
		dwDestFrameHeight = 1;
	}

	if (dwLines <= 0)
		return TRUE;

#ifdef _DEBUG_WITHOUT_PCICARD_
	static DWORD dwOffset = 0;
#endif //_DEBUG_WITHOUT_PCICARD_

	// ����ʾ��������
	//////////////////////////////////////////////////////////////////////////
	if( m_pImageCard->isReady() )
		m_pImageCard->UpdateOscillograph( (LPBYTE)m_pBuff, m_dwLineCounter, m_rcSlipWindow.left, m_rcSlipWindow.right );
	//////////////////////////////////////////////////////////////////////////

	// ������д�� ImageCard
	m_dwLineCounter += dwLines;
	if ( (int)m_dwLineCounter < dwDestFrameHeight )
	{
		m_rcSlipWindow.bottom	+= dwLines;

		if ( !m_blRedirectToDump )
		{
#ifdef _DEBUG_WITHOUT_PCICARD_
			if( !m_pPciCard->isReady() )
			{
				for (DWORD loop=0; loop<m_dwDMABlockSize; loop++)
					((LPBYTE)m_pBuff)[loop] = (BYTE)((loop+dwOffset) % 256);
			}
#endif //_DEBUG_WITHOUT_PCICARD_

			if( m_pImageCard->isReady() )
				m_pImageCard->Write( m_pBuff, m_dwDMABlockSize, m_rcSlipWindow );
		}
		
		m_rcSlipWindow.top		= m_rcSlipWindow.bottom;
	}
	else
	{
		m_rcSlipWindow.bottom	= m_rcDestFrame.bottom;
		m_dwDMABlockSize		= m_rcSlipWindow.Height() * m_rcSlipWindow.Width();
		
		if ( !m_blRedirectToDump )
		{
#ifdef _DEBUG_WITHOUT_PCICARD_
			if( !m_pPciCard->isReady() )
			{
				for (DWORD loop=0; loop<m_dwDMABlockSize; loop++)
					((LPBYTE)m_pBuff)[loop] = (BYTE)((loop+dwOffset) % 256);
			}
#endif //_DEBUG_WITHOUT_PCICARD_

			if( m_pImageCard->isReady() )
				m_pImageCard->Write( m_pBuff, m_dwDMABlockSize, m_rcSlipWindow );
		}

		if ( m_boolOneOff )
		{
			m_boolFinished = TRUE;
			return TRUE;
		}

#ifdef _DEBUG_WITHOUT_PCICARD_
		dwOffset += 8;
#endif //_DEBUG_WITHOUT_PCICARD_
		
		m_rcSlipWindow.bottom	= m_rcDestFrame.top;
		m_rcSlipWindow.top		= m_rcSlipWindow.bottom;

		m_dwLineCounter			-= dwDestFrameHeight;
		m_rcSlipWindow.bottom	+= m_dwLineCounter;
		DWORD	dwOffset		= m_dwDMABlockSize;
		m_dwDMABlockSize		= m_rcSlipWindow.Height() * m_rcSlipWindow.Width();
		
		if ( !m_blRedirectToDump )
		{
#ifdef _DEBUG_WITHOUT_PCICARD_
			if( !m_pPciCard->isReady() )
			{
				for (DWORD loop=0; loop<m_dwDMABlockSize; loop++)
					((LPBYTE)m_pBuff)[loop] = (BYTE)((loop+dwOffset) % 256);
			}
#endif //_DEBUG_WITHOUT_PCICARD_

			if( m_pImageCard->isReady() )
				m_pImageCard->Write( (LPVOID)&( ((LPBYTE)m_pBuff)[dwOffset] ), m_dwDMABlockSize, m_rcSlipWindow );
		}
		
		m_rcSlipWindow.top		= m_rcSlipWindow.bottom;
	}

	return TRUE;
}
