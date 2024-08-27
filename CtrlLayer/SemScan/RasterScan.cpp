// RasterScan.cpp : implementation file
//

#include "stdafx.h"
#include "RasterScan.h"
#include "UsbDI16A.h"
#include "../GlobalComponent/GraphBuffManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern DWORD	g_TickCount;
extern DWORD	g_thisTickCount, g_lastTickCount;

TCHAR exeFullPath[255];
int nIndex;

BOOL GetModuleFilePath(LPTSTR exeFullPath)
{
	DWORD i, dwLength;

	dwLength = GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	for ( i=dwLength-1; i > 0; i-- )
	{
		if ( exeFullPath[i] == '\\' )
		{
			exeFullPath[i+1] = '\0';
			break;
		}
	}
	
	if ( dwLength == 0 )
		return FALSE;

	return TRUE;
}

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

	m_blRedirectToDump		= FALSE;
	m_pBuff					= NULL;
	m_pBuffUSB				= NULL;
	m_lBytes_transferred	= 0;

	GetModuleFilePath( exeFullPath );
}

CRasterScan::~CRasterScan()
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

	//扫描停止时，擦除扫描位置线
	if( CSemVirtualMCICard::Instance().isReady() )
		CSemVirtualMCICard::Instance().PhotoIndication( m_rcIndication, RGB(0,0,0), FALSE );
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
	
	m_pBuffUSB = new unsigned short[LOOP_COUNTS*(m_dwExpectantDMABlockSize +512)];
	m_dwLineCounter	= 0;
	nIndex = 0;

	m_nSharedImageBuffIndex = 0;

	m_rcSlipWindow.left		= m_rcDestFrame.left;
	m_rcSlipWindow.right	= m_rcDestFrame.right;
	m_rcSlipWindow.top		= m_rcDestFrame.top;
	m_rcSlipWindow.bottom	= m_rcDestFrame.bottom;

	// 将FIFO清空
	CUsbDI16A::Instance().USB_ClearFIFO();

#ifdef _DEBUG_WITHOUT_PCICARD_
	if( !CUsbDI16A::Instance().USB_isReady() )
	{
		m_dwDebugOffsetZ		= 0;
		m_dwDebugOffsetY		= 0;
		m_dwDebugOffsetX		= 0;
		m_dwPosX				= 1;
		m_dwPosY				= 1;

		m_dwDebugCounts			= m_dwExpectantDMABlockSize /4;				// 一次的数据量相当于多少个像素点的数据
		m_dwDebugCountsX		= m_rcDestFrame.Width();					// 一行多少个点
		m_dwDebugCountsY		= m_dwDebugCounts / m_rcDestFrame.Height();	// 一次的数据量相当于多少行

		ZeroMemory(m_pBuffUSB, sizeof(m_pBuffUSB));
	}
#endif //_DEBUG_WITHOUT_PCICARD_

	return TRUE;
}

void CRasterScan::MainLoop()
{
	Sleep( m_dwExpectantInterval );

	if ( !InputData() )
		return;

	OutputData();
}


BOOL CRasterScan::_CheckUSB() 
{
	if( !CUsbDI16A::Instance().USB_isReady() )
	{
		unsigned short* pBuff = CGraphBuffMgr::Instance().m_pSharedImageBuff[m_nSharedImageBuffIndex];
		DWORD dwOffset	= 0;
		DWORD i,j,x,y,z;
		for( j=0; j<LOOP_COUNTS; j++ )
		{
			for (i=0; i<m_dwDebugCounts; i++)
			{
				// 因为第一个数据为整个数据包的个数，所以实际数据向后错1
				//				x = (DWORD)((i+m_dwDebugOffsetX+1) % m_dwDebugCountsX);
				//				y = (DWORD)(i / m_dwDebugCountsX +j*m_dwDebugCountsY +m_dwDebugOffsetY);
				x = m_dwPosX++;
				if( x > m_dwDebugCountsX )
				{
					m_dwPosX = 1;
					x = m_dwPosX++;
					m_dwPosY++;
				}
				y = m_dwPosY;
				if( m_dwPosY > m_rcDestFrame.Height() )
				{
					m_dwPosX = 1;
					m_dwPosY = 1;
					x = m_dwPosX++;
					y = m_dwPosY;
					m_dwDebugOffsetZ += 8;
					if( m_dwDebugOffsetZ > 0xFFF0 )
						m_dwDebugOffsetZ = 0;
				}
				z = (DWORD)(((x+m_dwDebugOffsetZ) % 256) << 4);
				if( x == 1 )
					z |= 0x1000;	// BOL
				if( x == 1 && y == 1)
					z |= 0x3000;	// BOF
				(pBuff)[dwOffset +i*4 +1] = (WORD)(x | 0x8000);	// 0x8xxx
				(pBuff)[dwOffset +i*4 +2] = (WORD)(y | 0x4000);	// 0x4xxx
				(pBuff)[dwOffset +i*4 +3] = (WORD)z;
				(pBuff)[dwOffset +i*4 +4] = (WORD)z;
			}
			(pBuff)[dwOffset] = (WORD)m_dwExpectantDMABlockSize;
			dwOffset = (j+1) *(m_dwExpectantDMABlockSize +512);
		}

		m_lBytes_transferred = LOOP_COUNTS * (m_dwExpectantDMABlockSize +512) *2;

		m_nSharedImageBuffIndex++;
		if( m_nSharedImageBuffIndex >= SharedImageBuffCounts )
			m_nSharedImageBuffIndex = 0;

		Sleep( 13-5 );
		return TRUE;
	}

	return FALSE;
}

BOOL CRasterScan::InputData()
{
#ifdef _DEBUG_WITHOUT_PCICARD_
	static DWORD dwOffsetZ = 0;
	static DWORD dwOffsetY = 0;

	BOOL bCheckUSB = _CheckUSB();
	if(bCheckUSB) {return TRUE;}

#endif //_DEBUG_WITHOUT_PCICARD_

	bCheckUSB = CUsbDI16A::Instance().USB_isReady();
	if (!bCheckUSB) {return TRUE;}

	DWORD dwRet = 0;
	dwRet = CUsbDI16A::Instance().USB_ImageInputStream1( m_dwExpectantDMABlockSize, CGraphBuffMgr::Instance().m_pSharedImageBuff[m_nSharedImageBuffIndex],
		&m_lBytes_transferred, -1 );

	if( dwRet > 0 && m_lBytes_transferred == 0)
	{
		//			#ifdef _DEBUG
		//				afxDump << "Bytes_transferred = " << m_lBytes_transferred << ", Ret = " << dwRet << "\r\n";
		//			#endif
		return FALSE;
	}

	m_nSharedImageBuffIndex++;
	if( m_nSharedImageBuffIndex == SharedImageBuffCounts )
		m_nSharedImageBuffIndex = 0;
	return TRUE;
}

BOOL CRasterScan::OutputData()
{
	CSemVirtualMCICard::Instance().SetSharedImageBuffIndex( m_nSharedImageBuffIndex );
	return TRUE;
}

BOOL CRasterScan::OutputData_ZZZ(BYTE* pBuff)
{
	DWORD dwLines = 1;//m_dwDMABlockSize / m_rcDestFrame.Width();
	DWORD dwDestFrameHeight = m_rcDestFrame.Height();

	// 更新示波器数据
	//////////////////////////////////////////////////////////////////////////
	if( CSemVirtualMCICard::Instance().isReady() )
		CSemVirtualMCICard::Instance().UpdateOscillograph( pBuff, m_dwLineCounter, m_rcSlipWindow.left, m_rcSlipWindow.right );
	//////////////////////////////////////////////////////////////////////////
	// 将数据写到 ImageCard
	m_dwLineCounter += dwLines;
	if ( (int)m_dwLineCounter < dwDestFrameHeight )
	{
		// 如果还不大于图像高度，就直接写入
		m_rcSlipWindow.bottom	+= dwLines;

		if ( !m_blRedirectToDump )
		{
			if( CSemVirtualMCICard::Instance().isReady() )
				CSemVirtualMCICard::Instance().Write( (void*)pBuff, m_rcDestFrame.Width(), m_rcSlipWindow );
		}
		
		m_rcSlipWindow.top		= m_rcSlipWindow.bottom;
	}
	else
	{
		// 如果加上这次的数据量后，大于图像高度
		// 那么先把图像最底部的数据直接写入
		m_rcSlipWindow.bottom	= m_rcDestFrame.bottom;
		m_dwDMABlockSize		= m_rcSlipWindow.Height() * m_rcSlipWindow.Width();
		
		if ( !m_blRedirectToDump )
		{
			if( CSemVirtualMCICard::Instance().isReady() )
				CSemVirtualMCICard::Instance().Write( (void*)pBuff, m_rcDestFrame.Width(), m_rcSlipWindow );
		}

		// 然后将Y调整至图像顶部的数行
		m_rcSlipWindow.bottom	= m_rcDestFrame.top;
		m_rcSlipWindow.top		= m_rcSlipWindow.bottom;

		m_dwLineCounter			-= dwDestFrameHeight;
		m_rcSlipWindow.bottom	+= m_dwLineCounter;
	}

	return TRUE;
}

// 向MCI板写数据
// bOpcode:		操作码
// blOperand:	是否有操作数
// wData:		16 bits操作数
BOOL CRasterScan::USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData )
{
	return CUsbDI16A::Instance().USB_WriteToBoard( bOpcode, blOperand, wData );
}

void CRasterScan::ReadMonitorData( int nIndex )
{
	DWORD dwRet = 0;
	switch( nIndex )
	{
	case	0:	// SPR board
		for(int i=0; i<16; i++ )
		{
			dwRet = CUsbDI16A::Instance().USB_ReadMonitorData( USB_MID_SPR, 1, 4, i, -1 );
			if( dwRet > 0 )
				TRACE( "ReadMonitorData of USB_MID_SPR #1 error: %d.\n", i );
		}
		for(int i=0; i<4; i++ )
		{
			dwRet = CUsbDI16A::Instance().USB_ReadMonitorData( USB_MID_SPR, 1, 5, i, -1 );
			if( dwRet > 0 )
				TRACE( "ReadMonitorData of USB_MID_SPR #2 error: %d.\n", i );
		}
		for(int i=5; i<16; i++ )
		{
			dwRet = CUsbDI16A::Instance().USB_ReadMonitorData( USB_MID_SPR, 1, 5, i, -1 );
			if( dwRet > 0 )
				TRACE( "ReadMonitorData of USB_MID_SPR #2 error: %d.\n", i );
		}
		TRACE( "ReadMonitorData of USB_MID_SPR.\n" );
		break;
	case	1:	// VAC board
		for(int i=0; i<16; i++ )
		{
			dwRet = CUsbDI16A::Instance().USB_ReadMonitorData( USB_MID_VAC, 1, 2, i, -1 );
			if( dwRet > 0 )
				TRACE( "ReadMonitorData of USB_MID_VAC #1 error: %d.\n", i );
		}
		for(int i=0; i<6; i++ )
		{
			dwRet = CUsbDI16A::Instance().USB_ReadMonitorData( USB_MID_VAC, 1, 3, i, -1 );
			if( dwRet > 0 )
				TRACE( "ReadMonitorData of USB_MID_VAC #2 error: %d.\n", i );
		}
		for(int i=10; i<16; i++ )
		{
			dwRet = CUsbDI16A::Instance().USB_ReadMonitorData( USB_MID_VAC, 1, 3, i, -1 );
			if( dwRet > 0 )
				TRACE( "ReadMonitorData of USB_MID_VAC #2 error: %d.\n", i );
		}
		TRACE( "ReadMonitorData of USB_MID_VAC.\n" );
		break;
	case	2:	// LNS board
		for(int i=0; i<10; i++ )
		{
			dwRet = CUsbDI16A::Instance().USB_ReadMonitorData( USB_MID_LNS, 2, 1, i, -1 );
			if( dwRet > 0 )
				TRACE( "ReadMonitorData of USB_MID_LNS error: %d.\n", i );
		}
		TRACE( "ReadMonitorData of USB_MID_LNS.\n" );
		break;
	case	3:	// SCN board
		for(int i=0; i<16; i++ )
		{
			dwRet = CUsbDI16A::Instance().USB_ReadMonitorData( USB_MID_SCN, 1, 0, i, -1 );
			if( dwRet > 0 )
				TRACE( "ReadMonitorData of USB_MID_SCN error: %d.\n", i );
		}
		TRACE( "ReadMonitorData of USB_MID_SCN.\n" );
		break;
	default:
		break;
	}
}