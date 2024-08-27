// DigitalPhoto.cpp : implementation file
//

#include "stdafx.h"
#include "DigitalPhoto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const COLORREF colorDigitalPhotoIndication = RGB(0,0,255);

/////////////////////////////////////////////////////////////////////////////
// CDigitalPhoto

IMPLEMENT_DYNCREATE(CDigitalPhoto, CBasisScan)

CDigitalPhoto::CDigitalPhoto()
{
	m_pPhotoBuff	= NULL;
	m_pImageBuff	= NULL;

	m_boolOneOff	= TRUE;

	m_blRedirectToDump = FALSE;
}

CDigitalPhoto::~CDigitalPhoto()
{
	if ( m_pPhotoBuff != NULL )
	{
		GlobalFree( m_pPhotoBuff );
		m_pPhotoBuff	= NULL;
	}
}

BOOL CDigitalPhoto::InitInstance()
{
	CBasisScan::InitInstance();

	///////////////////////////////////////////////////////
	CRect rcDest	= m_rcDestFrame;
	rcDest.right	= rcDest.left + 8;

	if( CSemVirtualMCICard::Instance().isReady() )
		CSemVirtualMCICard::Instance().PhotoIndication( rcDest, RGB(0,0,0) );
	///////////////////////////////////////////////////////

	return FALSE;
}

int CDigitalPhoto::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CBasisScan::ExitInstance();
}

BEGIN_MESSAGE_MAP(CDigitalPhoto, CBasisScan)
	//{{AFX_MSG_MAP(CDigitalPhoto)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDigitalPhoto message handlers

BOOL CDigitalPhoto::InitData()
{
	if ( !CBasisScan::InitData() )
		return FALSE;

//	m_pPhotoBuff = GlobalAlloc( GPTR, PHOTOFIFOSIZE );
	if (m_pPhotoBuff == NULL)
	{
		return FALSE;
	}

	m_dwPhotoLineCounter	= 0;

	m_rcPhotoSlipWindow.left	= m_rcDestFrame.left;
	m_rcPhotoSlipWindow.right	= m_rcDestFrame.right;
	m_rcPhotoSlipWindow.top		= m_rcDestFrame.top;
	m_rcPhotoSlipWindow.bottom	= m_rcDestFrame.top;

	m_boolPhotoReady		= FALSE;
	m_rcPhotoIndication		= m_rcDestFrame;

	return TRUE;
}

void CDigitalPhoto::MainLoop()
{
	////////////////////////////////////////////////////
	if ( m_boolFinished )
	{
		Sleep( 13-5 );
		return;
	}
	////////////////////////////////////////////////////

	////////////////////////////////////////////////////
	if ( m_dwExpectantInterval < 13 )
		Sleep( m_dwExpectantInterval );
	else
		Sleep( 13-5 );
	////////////////////////////////////////////////////

	////////////////////////////////////////////////////
	// Photo
	////////////////////////////////////////////////////
	if ( !m_boolPhotoReady )
	{
		////////////////////////////////
		m_rcPhotoIndication.right	= m_rcPhotoIndication.left + 8;
		m_rcPhotoIndication.top		= m_dwPhotoLineCounter;
		/////----------------------/////

		if ( PreparePhotoData() == TRUE )
			m_boolPhotoReady = TRUE;
	}

	if ( OutputPhotoData() == TRUE )
	{
		m_boolPhotoReady = FALSE;

		/////----------------------/////
		if ((long)m_dwPhotoLineCounter <= m_rcPhotoIndication.top)
		{
			m_rcPhotoIndication.bottom	= m_rcDestFrame.bottom;
		}
		else
		{
			m_rcPhotoIndication.bottom	= m_dwPhotoLineCounter;
		}
		////////////////////////////////

		////////////////////////////////
		if( CSemVirtualMCICard::Instance().isReady() )
			CSemVirtualMCICard::Instance().PhotoIndication( m_rcPhotoIndication, colorDigitalPhotoIndication );
		////////////////////////////////
	}
	////////////////////////////////////////////////////
}

BOOL CDigitalPhoto::PreparePhotoData()
{
/*	DWORD	dwLines;
	LPBYTE	pImageBuff;

	// 根据小FIFO半满，由PHOTOFIFOSIZE计算数据量
	dwLines = PHOTOFIFOSIZE / m_rcDestFrame.Width() / 2;
	m_dwPhotoDMABlockSize = dwLines * m_rcDestFrame.Width();

	// 从 VGD 读数据
	m_dwPhotoLineCounter += dwLines;

	if ( (int)m_dwPhotoLineCounter < m_rcDestFrame.Height() )
	{
		m_rcPhotoSlipWindow.bottom	+= dwLines;

		if ( !m_blRedirectToDump )
		{
			pImageBuff = &(m_pImageBuff[m_rcPhotoSlipWindow.top * g_szFrameGraph.cx]);
			memcpy( m_pPhotoBuff, pImageBuff, 
				m_rcPhotoSlipWindow.Width() * m_rcPhotoSlipWindow.Height() );

			//m_pImageCard->Read( m_pPhotoBuff, m_dwPhotoDMABlockSize, m_rcPhotoSlipWindow );
		}
		
		m_rcPhotoSlipWindow.top		= m_rcPhotoSlipWindow.bottom;
	}
	else
	{
		m_rcPhotoSlipWindow.bottom	= m_rcDestFrame.bottom;
		m_dwPhotoDMABlockSize		= m_rcPhotoSlipWindow.Height() * m_rcPhotoSlipWindow.Width();
		
		if ( !m_blRedirectToDump )
		{
			pImageBuff = &(m_pImageBuff[m_rcPhotoSlipWindow.top * g_szFrameGraph.cx]);
			memcpy( m_pPhotoBuff, pImageBuff, 
				m_rcPhotoSlipWindow.Width() * m_rcPhotoSlipWindow.Height() );
			
			//m_pImageCard->Read( m_pPhotoBuff, m_dwPhotoDMABlockSize, m_rcPhotoSlipWindow );
		}

		if ( m_boolOneOff )
		{
			m_boolFinished = TRUE;
			return TRUE;
		}
		
		m_rcPhotoSlipWindow.bottom	= m_rcDestFrame.top;
		m_rcPhotoSlipWindow.top		= m_rcPhotoSlipWindow.bottom;

		m_dwPhotoLineCounter		-= m_rcDestFrame.Height();
		m_rcPhotoSlipWindow.bottom	+= m_dwPhotoLineCounter;
		DWORD	dwOffset			= m_dwPhotoDMABlockSize;
		m_dwPhotoDMABlockSize		= m_rcPhotoSlipWindow.Height() * m_rcPhotoSlipWindow.Width();
		
		if ( !m_blRedirectToDump )
		{
			pImageBuff = &(m_pImageBuff[m_rcPhotoSlipWindow.top * g_szFrameGraph.cx]);
			memcpy( m_pPhotoBuff, pImageBuff, 
				m_rcPhotoSlipWindow.Width() * m_rcPhotoSlipWindow.Height() );

			//m_pImageCard->Read( m_pPhotoBuff, m_dwPhotoDMABlockSize, m_rcPhotoSlipWindow );
		}
		
		m_rcPhotoSlipWindow.top		= m_rcPhotoSlipWindow.bottom;
	}
*/
	return TRUE;
}

BOOL CDigitalPhoto::OutputPhotoData()
{
/*	DWORD dwLines;

	// 读寄存器
	ReadParam	Param1;
	m_pPciCard->IoPortReadL( ADDR_10001001B, Param1.dwParam );

	// 判断小FIFO是否半满
	if ( Param1.ADDR_10001001B.PHOTO_FIFO != 1 )
		return FALSE;

	// 根据小FIFO半满，由PHOTOFIFOSIZE计算数据量
	dwLines = PHOTOFIFOSIZE / m_rcDestFrame.Width() / 2;
	m_dwPhotoDMABlockSize = dwLines * m_rcDestFrame.Width();

	// 将数据传送到小FIFO
	if ( !(m_pPciCard->PhotoOutputStream( m_pPhotoBuff, m_dwPhotoDMABlockSize )) )
		return FALSE;

	//////////////////////////////////////////////////////////
	// Enable Photo 2002-3-11 add by LMH
	//////////////////////////////////////////////////////////
	unsigned long	dwData = 0x03;
	m_pPciCard->IoPortWriteL( ADDR_10000001B, dwData );
	//////////////////////////////////////////////////////////
*/
	return TRUE;
}

