// AnalogPhoto.cpp : implementation file
//

#include "stdafx.h"
#include "AnalogPhoto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const COLORREF colorAnalogPhotoIndication = RGB(0,0,255);

/////////////////////////////////////////////////////////////////////////////
// CAnalogPhoto

IMPLEMENT_DYNCREATE(CAnalogPhoto, CRasterScan)

CAnalogPhoto::CAnalogPhoto()
{
	m_pPhotoBuff		= NULL;
	m_pGraphBuff		= NULL;

	m_boolOneOff		= TRUE;
}

CAnalogPhoto::~CAnalogPhoto()
{
	if ( m_pPhotoBuff != NULL )
	{
		GlobalFree( m_pPhotoBuff );
		m_pPhotoBuff	= NULL;
	}
}

BOOL CAnalogPhoto::InitInstance()
{
	CRasterScan::InitInstance();	
	
	///////////////////////////////////////////////////////
	CRect rcDest	= m_rcDestFrame;
	rcDest.right	= rcDest.left + 8;

	if( CSemVirtualMCICard::Instance().isReady() )
		CSemVirtualMCICard::Instance().PhotoIndication( rcDest, RGB(0,0,0) );
	///////////////////////////////////////////////////////

	return FALSE;
}

int CAnalogPhoto::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CRasterScan::ExitInstance();
}

BEGIN_MESSAGE_MAP(CAnalogPhoto, CRasterScan)
	//{{AFX_MSG_MAP(CAnalogPhoto)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnalogPhoto message handlers

BOOL CAnalogPhoto::InitData()
{
	if ( !CRasterScan::InitData() )
		return FALSE;

//	m_pPhotoBuff = GlobalAlloc( GPTR, PHOTOFIFOSIZE );
	if (m_pPhotoBuff == NULL)
	{
		GlobalFree( m_pBuff );
		m_pBuff	= NULL;
		return FALSE;
	}

	m_dwPhotoLineCounter	= 0;
	m_rcPhotoSlipWindow		= m_rcSlipWindow;

	m_boolPhotoReady		= FALSE;

	return TRUE;
}

void CAnalogPhoto::MainLoop()
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

	////////////////////////////////
	// Photo
	////////////////////////////////
	if ( !m_boolPhotoReady )
		if ( PreparePhotoData() == TRUE )
			m_boolPhotoReady = TRUE;

	if ( OutputPhotoData() == TRUE )
		m_boolPhotoReady = FALSE;
	////////////////////////////////

	////////////////////////////////////////
	// Raster
	////////////////////////////////////////
	///////////////////////////////
	if ( !InputData() )	return;
	///////////////////////////////

	///////////////////////////////
	static CRect rcDest	= m_rcDestFrame;
	rcDest.right	= rcDest.left + 8;
	rcDest.top		= m_dwLineCounter;
	/////----------------------////

	///////////////////////////////
	OutputData();
	///////////////////////////////

	/////----------------------////
	///////////////////////////////
	if ((long)m_dwLineCounter <= rcDest.top)
	{
		rcDest.bottom	= m_rcDestFrame.bottom;
	}
	else
	{
		rcDest.bottom	= m_dwLineCounter;
	}
	///////////////////////////////

	if( CSemVirtualMCICard::Instance().isReady() )
		CSemVirtualMCICard::Instance().PhotoIndication( rcDest, colorAnalogPhotoIndication );
	///////////////////////////////
	////////////////////////////////////////
}

BOOL CAnalogPhoto::PreparePhotoData()
{
/*	DWORD	dwLines;
	LPBYTE	pGraphBuff;

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
			pGraphBuff = &(m_pGraphBuff[m_rcPhotoSlipWindow.top * g_szFrameGraph.cx]);
			memcpy( m_pPhotoBuff, pGraphBuff, 
				m_rcPhotoSlipWindow.Width() * m_rcPhotoSlipWindow.Height() );
		}
		
		m_rcPhotoSlipWindow.top		= m_rcPhotoSlipWindow.bottom;
	}
	else
	{
		m_rcPhotoSlipWindow.bottom	= m_rcDestFrame.bottom;
		m_dwPhotoDMABlockSize		= m_rcPhotoSlipWindow.Height() * m_rcPhotoSlipWindow.Width();
		
		if ( !m_blRedirectToDump )
		{
			pGraphBuff = &(m_pGraphBuff[m_rcPhotoSlipWindow.top * g_szFrameGraph.cx]);
			memcpy( m_pPhotoBuff, pGraphBuff, 
				m_rcPhotoSlipWindow.Width() * m_rcPhotoSlipWindow.Height() );
		}
		
		m_rcPhotoSlipWindow.bottom	= m_rcDestFrame.top;
		m_rcPhotoSlipWindow.top		= m_rcPhotoSlipWindow.bottom;

		m_dwPhotoLineCounter		-= m_rcDestFrame.Height();
		m_rcPhotoSlipWindow.bottom	+= m_dwPhotoLineCounter;
		DWORD	dwOffset			= m_dwPhotoDMABlockSize;
		m_dwPhotoDMABlockSize		= m_rcPhotoSlipWindow.Height() * m_rcPhotoSlipWindow.Width();
		
		if ( !m_blRedirectToDump )
		{
			pGraphBuff = &(m_pGraphBuff[m_rcPhotoSlipWindow.top * g_szFrameGraph.cx]);
			memcpy( m_pPhotoBuff, pGraphBuff, 
				m_rcPhotoSlipWindow.Width() * m_rcPhotoSlipWindow.Height() );
		}
		
		m_rcPhotoSlipWindow.top		= m_rcPhotoSlipWindow.bottom;
	}
*/
	return TRUE;
}

BOOL CAnalogPhoto::OutputPhotoData()
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
