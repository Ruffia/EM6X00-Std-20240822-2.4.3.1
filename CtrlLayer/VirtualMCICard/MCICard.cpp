// MCICard.cpp : implementation file
//
// Last Modify : 2009.12.03

/////////////////////////////////////////////////////////////////////////////
// 09.11.24在SetZoom中加入同步语句
// 09.12.03修改Mainloop中同步语句的位置，将GetClientWindow也纳入同步
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ddutil.h"
#include "MCICard.h"
#include "CommFunctions.h"
#include "Util.h"
#include <string.h>
using namespace std;
#include "MCICardGetDataLogicExecutor.h"
using namespace MCICardGetDataLogicExecutor;
#include "Factory.h"
#include "RectConvertor.h"
#include "MCICardOption.h"
#include "MCICardCommonSetting.h"
#include "GraphBuffManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TCHAR g_exeFullPath[255];
int g_index;

CCriticalSection CMCICard::m_mutexLayAll;
CCriticalSection CMCICard::m_mutexImageDib;
CCriticalSection CMCICard::m_mutexImageUSB;


void CaptureScreenRect( CBitmap& bmpScreen, CDC* pScreenDC, const CRect& rcCapture )
{
	/////////////////////////////////////////////
	HDC hcompDC = CreateCompatibleDC(pScreenDC->m_hDC);
	BITMAPINFO	bmi;
	BYTE*		bits = NULL;
		
	// Setup BITMAPINFOHEAD.
	bmi.bmiHeader.biSize			= sizeof (BITMAPINFOHEADER) ;
	bmi.bmiHeader.biWidth			= rcCapture.Width();
	bmi.bmiHeader.biHeight			= rcCapture.Height();
	bmi.bmiHeader.biPlanes			= 1;
	bmi.bmiHeader.biBitCount		= 32;
	bmi.bmiHeader.biCompression		= BI_RGB;
	bmi.bmiHeader.biSizeImage		= 0;
	bmi.bmiHeader.biXPelsPerMeter	= 0;
	bmi.bmiHeader.biYPelsPerMeter	= 0;
	bmi.bmiHeader.biClrUsed			= 0;
	bmi.bmiHeader.biClrImportant	= 0;
	bmpScreen.m_hObject = CreateDIBSection (NULL, &bmi, 0, (void **)&bits, NULL, 0) ;
	/////////////////////////////////////////////
	if( bmpScreen.m_hObject == NULL)
		return;
	HBITMAP hbmp=(HBITMAP)SelectObject(hcompDC, bmpScreen.m_hObject);
	BitBlt(hcompDC, 0, 0, rcCapture.Width(), rcCapture.Height(), pScreenDC->m_hDC, rcCapture.left, rcCapture.top, SRCCOPY);
	SelectObject(hcompDC, hbmp);
	DeleteDC(hcompDC);
}

BOOL GetModuleFilePath(LPTSTR exeFullPath)
{
	CString str;
	str.Format( "H:\\Temp\\SEM\\Log\\" );
	sprintf( exeFullPath, "%s", str );
//	return TRUE;
	
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
	strcat( exeFullPath, "Log\\" );

	if ( dwLength == 0 )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMCICard

IMPLEMENT_DYNCREATE(CMCICard, CWinThread)

CMCICard::CMCICard()
{
	/////////////////////////////////////////////////////////////////
	m_bAutoDelete			= FALSE;

	// kill event starts out in the signaled state
	m_hEventKill = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	m_boolReady				= FALSE;
	m_boolShouldReset		= FALSE;
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	m_lpDD					= NULL;		// DirectDraw object
	m_lpDDSPrimary			= NULL;		// DirectDraw primary surface
	m_lpDDPalette			= NULL;		// DirectDraw palette
	m_lpClipper				= NULL;		// clipper for primary

	m_lpDDSPrimaryBackLay	= NULL;		// Offscreen surface Primary Back Lay
	m_lpDDSGraphBackLay		= NULL;		// Offscreen surface Graph Back Lay

	m_lpDDSImageLay			= NULL;		// Offscreen surface Image Lay
	m_lpDDSGraphLay			= NULL;		// Offscreen surface Graph Lay

	m_hdibImage				= NULL;
	m_lpImageDibPlan		= NULL;

	m_pScreenWnd			= NULL;
	m_dblZoom				= 1.0;

	m_lastTickCount			= 0;

	m_dwByteCount			= 0;
    m_dwBytesLast			= 0;
	m_dwBytes				= 0;

	m_dwInputCount			= 0;
	/////////////////////////////////////////////////////////////////
	m_boolPreview			= FALSE;
	m_boolNewScanScreenMode	= TRUE;

	m_nDataStoreMode		= 1;					// XYZ存储方式
	m_nDetectorMode			= 0;					// 12.02.15 单探测器模式
	m_bDet2nd				= FALSE;				// 默认显示偶数通道图像
	m_bDebugZ				= FALSE;				// 13.04.03 非调试模式

	m_nMaxPix = 2559;
	m_nDiv = 10;
	m_dSEratio = 0.5;

	GetModuleFilePath( g_exeFullPath );

	m_bDataNeedUpdated		= FALSE;
	m_bStopScan				= FALSE;
//	m_pBuffUSB				= NULL;
	m_pBuffUSB	= new unsigned short[LOOP_COUNTS*409600];
	m_dwExpectantDMABlockSize = 0;
	m_pPixX		= new WORD[ LOOP_COUNTS*409600/4 ];
	m_pPixY		= new WORD[ LOOP_COUNTS*409600/4 ];
	m_pPixZ		= new WORD[ LOOP_COUNTS*409600/4 ];
	m_pPixX2	= new WORD[ LOOP_COUNTS*409600/4 ];
	m_pPixY2	= new WORD[ LOOP_COUNTS*409600/4 ];
	m_pPixZ2	= new WORD[ LOOP_COUNTS*409600/4 ];
	m_pPixZmix	= new WORD[ LOOP_COUNTS*409600/4 ];
    m_pPixZOrigin = new WORD[ LOOP_COUNTS*409600/4 ];
	// 2021.06 原始数据存储
	m_pPixZsrc	= NULL;
	m_pPixZsrc2	= NULL;

	m_hFileLog		= NULL;
	m_bNeedMon		= FALSE;
	m_hLineDataLog	= NULL;

	m_nSharedImageBuffIndexRead		= 0;
	m_nSharedImageBuffIndexWrite	= 0;

	m_nCurrentY1	= 0;
	m_nCurrentY2	= 0;

	m_pLabelControler = new CLabelControler(m_mutexLayAll,m_lpDD,m_lpDDSGraphBackLay);
	m_pMobileUScaleControler = new CMobileUScaleControler(m_mutexLayAll,m_lpDD,m_lpDDSGraphBackLay);
	m_pOscillographControler = new COscillographControler(m_mutexLayAll,m_lpDD,m_lpDDSGraphBackLay);
	m_pFrameRateControler = new CFrameRateControler(m_mutexLayAll,m_lpDD,m_lpDDSGraphBackLay);
	m_pHeaderControler = new CHeaderControler(m_mutexLayAll,m_lpDD,m_lpDDSGraphBackLay);
	m_pAngleControler = new CAngleControler(m_mutexLayAll,m_lpDD,m_lpDDSGraphBackLay);
}

CMCICard::~CMCICard()
{
	if ( m_pBuffUSB != NULL )
	{
		delete [] m_pBuffUSB;
		m_pBuffUSB	= NULL;
	}
	if ( m_pPixX != NULL )
	{
		delete [] m_pPixX;
		m_pPixX	= NULL;
	}
	if ( m_pPixY != NULL )
	{
		delete [] m_pPixY;
		m_pPixY	= NULL;
	}
	if ( m_pPixZ != NULL )
	{
		delete [] m_pPixZ;
		m_pPixZ	= NULL;
	}
	if ( m_pPixX2 != NULL )
	{
		delete [] m_pPixX2;
		m_pPixX2	= NULL;
	}
	if ( m_pPixY2 != NULL )
	{
		delete [] m_pPixY2;
		m_pPixY2	= NULL;
	}
	if ( m_pPixZ2 != NULL )
	{
		delete [] m_pPixZ2;
		m_pPixZ2	= NULL;
	}
	if ( m_pPixZmix != NULL )
	{
		delete [] m_pPixZmix;
		m_pPixZmix	= NULL;
	}
	// 2021.06 原始数据存储
	if ( m_pPixZsrc != NULL )
	{
		delete [] m_pPixZsrc;
		m_pPixZsrc	= NULL;
	}
	if ( m_pPixZsrc2 != NULL )
	{
		delete [] m_pPixZsrc2;
		m_pPixZsrc2	= NULL;
	}
		if ( m_pPixZOrigin != NULL )
	{
		delete [] m_pPixZOrigin;
		m_pPixZOrigin	= NULL;
	}

	Close();

	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);

	if (m_pLabelControler)
	{
		delete m_pLabelControler;
		m_pLabelControler = NULL;
	}

	if (m_pMobileUScaleControler)
	{
		delete m_pMobileUScaleControler;
		m_pMobileUScaleControler = NULL;
	}

	if (m_pOscillographControler)
	{
		delete m_pOscillographControler;
		m_pOscillographControler = NULL;
	}

	if (m_pFrameRateControler)
	{
		delete m_pFrameRateControler;
		m_pFrameRateControler = NULL;
	}

	if (m_pHeaderControler)
	{
		delete m_pHeaderControler;
		m_pHeaderControler = NULL;
	}

	if (m_pAngleControler)
	{
		delete m_pAngleControler;
		m_pAngleControler = NULL;
	}
}

BOOL CMCICard::InitInstance()
{
	while (WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT)
	{
		MainLoop();
		Sleep(1);
	}

	return FALSE;  // TRUE;
}

int CMCICard::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMCICard, CWinThread)
	//{{AFX_DebugMessage_MAP(CMCICard)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DebugMessage_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCICard message handlers
void CMCICard::Delete()
{
	// calling the base here won't do anything but it is a good habit
	CWinThread::Delete();

	// acknowledge receipt of kill notification
	VERIFY(SetEvent(m_hEventDead));
}

void CMCICard::KillThread()
{
	// Note: this function is called in the context of other threads,
	//  not the thread itself.

	// reset the m_hEventKill which signals the thread to shutdown
	VERIFY(SetEvent(m_hEventKill));

	// allow thread to run at higher priority during kill process
	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	WaitForSingleObject(m_hEventDead, INFINITE);
	WaitForSingleObject(m_hThread, INFINITE);

	// now delete CWinThread object since no longer necessary
	delete this;
}

BOOL CMCICard::Close()
{
	m_mutexLayAll.Lock();

	m_boolReady		= FALSE;

    if( m_lpDD != NULL )
    {
		if( m_lpDDSPrimary != NULL )
		{
			m_lpDDSPrimary->Release();
			m_lpDDSPrimary = NULL;
		}
		if( m_lpDDPalette != NULL )
		{
			m_lpDDPalette->Release();
			m_lpDDPalette = NULL;
		}
		if( m_lpClipper != NULL )
		{
			m_lpClipper->Release();
			m_lpClipper	= NULL;
		}

		if( m_lpDDSPrimaryBackLay != NULL )
		{
			m_lpDDSPrimaryBackLay->Release();
			m_lpDDSPrimaryBackLay = NULL;
		}

		if( m_lpDDSGraphBackLay != NULL )
		{
			m_lpDDSGraphBackLay->Release();
			m_lpDDSGraphBackLay = NULL;
		}
		
		if( m_lpDDSImageLay != NULL )
		{
			m_lpDDSImageLay->Release();
			m_lpDDSImageLay = NULL;
		}

		if( m_lpDDSGraphLay != NULL )
		{
			m_lpDDSGraphLay->Release();
			m_lpDDSGraphLay = NULL;
		}

		m_pLabelControler->ReleaseLay();
		m_pMobileUScaleControler->ReleaseLay();
		m_pOscillographControler->ReleaseLay();
		m_pFrameRateControler->ReleaseLay();
		m_pHeaderControler->ReleaseLay();
		m_pAngleControler->ReleaseLay();

		m_lpDD->Release();
		m_lpDD = NULL;
    }
	
	if( m_hdibImage != NULL )
	{
		GlobalUnlock( (HGLOBAL)m_hdibImage );
		GlobalFree( (HGLOBAL)m_hdibImage );
		m_hdibImage = NULL;
	}
	m_lpImageDibPlan	= NULL;

	m_paletteImage.DeleteObject();

    m_blSrcColorkey		= FALSE;
    m_blDestColorkey	= FALSE;

    m_pFrameRateControler->Close();
	m_pLabelControler->Close();
	m_pHeaderControler->Close();
	m_pAngleControler->Close();

	m_mutexLayAll.Unlock();

	return TRUE;
}

BOOL CMCICard::Reset( CWnd* pWnd )
{
    DDSURFACEDESC			ddsd;
    HRESULT					ddrval;

	Close();

	m_pScreenWnd = pWnd;

	m_hdibImage = CreateImageBitmap();
	if ( m_hdibImage == NULL )
	{
		return FALSE;
	}
	
    // create the main DirectDraw object
	LPDIRECTDRAW lpDD;
	ddrval = DirectDrawCreate( NULL, &lpDD, NULL );
	if( ddrval != DD_OK )
		return FALSE;
	ddrval = lpDD->QueryInterface(IID_IDirectDraw2, (LPVOID *) &m_lpDD );
	if( ddrval != S_OK )
	{
		lpDD->Release();
		return FALSE;
	}
	lpDD->Release();
	ddrval = m_lpDD->SetCooperativeLevel( m_pScreenWnd->m_hWnd, DDSCL_NORMAL );



    // check capabilites
    HDC			hdc;
    hdc = GetDC(NULL);
	int iPlanes		= GetDeviceCaps(hdc, PLANES);
	int iBitsPixel	= GetDeviceCaps(hdc, BITSPIXEL);
	m_uBPP			= iPlanes * iBitsPixel;
    ReleaseDC(NULL, hdc);

    DDCAPS		ddcaps;
    ddcaps.dwSize = sizeof( ddcaps );

    ddrval = m_lpDD->GetCaps( &ddcaps, NULL );
    if( ddrval != DD_OK )
    {
        DebugMessage("GetCaps failed err=%d", ddrval);
		return FALSE;
    }

    if( ddcaps.dwCaps & DDCAPS_NOHARDWARE )
    {
        DebugMessage( "No hardware support at all" );
    }

    if( ddcaps.dwCaps & DDCAPS_BLTCOLORFILL )
    {
        m_blColorFill = TRUE;
        DebugMessage( "Device supports color fill" );
    }
    else
    {
        m_blColorFill = FALSE;
        DebugMessage( "Device does not support color fill" );
    }

	BOOL boolUseSysMemPrimaryBackLay;
	BOOL boolUseSysMemGraphBackLay;
	BOOL boolUseSysMemImageLay;
	BOOL boolUseSysMemGraphLay;
	BOOL boolUseSysMemLabelLay;
	BOOL boolUseSysMemUScaleLay;
	BOOL boolUseSysMemHeaderLay;
	BOOL boolUseSysMemAngleLay;

	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	CSize& m_szGraph = CMCICardCommonSetting::Instance().m_szGraph;

	DWORD dwFrameMemSizePrimaryBackLay = GetSystemMetrics(SM_CXSCREEN) * GetSystemMetrics(SM_CYSCREEN) * ( m_uBPP / 8 );
	DWORD dwFrameMemSizeImageLay = m_szImage.cx * m_szImage.cy * ( m_uBPP / 8 );
	DWORD dwFrameMemSizeGraphLay = m_szGraph.cx * m_szGraph.cy * ( m_uBPP / 8 );
	DWORD dwFrameMemSizeLabelLay = m_pLabelControler->m_szLabel.cx * m_pLabelControler->m_szLabel.cy * ( m_uBPP / 8 );
	DWORD dwFrameMemSizeHeaderLay = m_pHeaderControler->m_szHeader.cx * m_pHeaderControler->m_szHeader.cy * ( m_uBPP / 8 );
	//07.10.31
	DWORD dwFrameMemSizeUScaleLay;
//	dwFrameMemSizeUScaleLay = m_szUScale.cx * m_szUScale.cy * ( m_uBPP / 8 );
#ifdef NEW_SCANSCREEN_MODE
	dwFrameMemSizeUScaleLay = m_szGraph.cx * m_szGraph.cy * ( m_uBPP / 8 );
#else
	dwFrameMemSizeUScaleLay = m_szUScale.cx * m_szUScale.cy * ( m_uBPP / 8 );
#endif
	//07.10.31
	DWORD dwFrameMemSizeAngleLay = m_szGraph.cx * m_szGraph.cy * ( m_uBPP / 8 );
	DWORD dwFrameMemSizeGraphBackLay = dwFrameMemSizeGraphLay;
	/////////////////////////////////////////////////////////////////

	m_dwVidMemTotal = ddcaps.dwVidMemTotal;
	/////////////////////////////////////////////////////////////////
    if( ddcaps.dwVidMemTotal <= dwFrameMemSizePrimaryBackLay )
    {
		boolUseSysMemPrimaryBackLay	= TRUE;
		boolUseSysMemGraphBackLay	= TRUE;
		boolUseSysMemImageLay	= TRUE;
		boolUseSysMemGraphLay	= TRUE;
		boolUseSysMemLabelLay	= TRUE;
		boolUseSysMemUScaleLay	= TRUE;
		boolUseSysMemHeaderLay	= TRUE;
		boolUseSysMemAngleLay	= TRUE;

		m_dwFrameMemSizeUsed	= 0;

        DebugMessage("Using system memory at all (not enough memory)");
    }
    else if( ddcaps.dwVidMemTotal <= dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay )
    {
		boolUseSysMemPrimaryBackLay	= FALSE;
		boolUseSysMemGraphBackLay	= TRUE;
		boolUseSysMemImageLay	= TRUE;
		boolUseSysMemGraphLay	= TRUE;
		boolUseSysMemLabelLay	= TRUE;
		boolUseSysMemUScaleLay	= TRUE;
		boolUseSysMemHeaderLay	= TRUE;
		boolUseSysMemAngleLay	= TRUE;

		m_dwFrameMemSizeUsed	= dwFrameMemSizePrimaryBackLay;

        DebugMessage("Using system memory partly (not enough memory)");
    }
    else if( ddcaps.dwVidMemTotal <= dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay )
    {
		boolUseSysMemPrimaryBackLay	= FALSE;
		boolUseSysMemGraphBackLay	= FALSE;
		boolUseSysMemImageLay	= TRUE;
		boolUseSysMemGraphLay	= TRUE;
		boolUseSysMemLabelLay	= TRUE;
		boolUseSysMemUScaleLay	= TRUE;
		boolUseSysMemHeaderLay	= TRUE;
		boolUseSysMemAngleLay	= TRUE;

		m_dwFrameMemSizeUsed	= dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay;

        DebugMessage("Using system memory partly (not enough memory)");
    }
    else if( ddcaps.dwVidMemTotal <= 
		dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay )
    {
		boolUseSysMemPrimaryBackLay	= FALSE;
		boolUseSysMemGraphBackLay	= FALSE;
		boolUseSysMemImageLay	= FALSE;
		boolUseSysMemGraphLay	= TRUE;
		boolUseSysMemLabelLay	= TRUE;
		boolUseSysMemUScaleLay	= TRUE;
		boolUseSysMemHeaderLay	= TRUE;
		boolUseSysMemAngleLay	= TRUE;

		m_dwFrameMemSizeUsed	= dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay;

        DebugMessage("Using system memory partly (not enough memory)");
    }
    else if( ddcaps.dwVidMemTotal <= 
		dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay + dwFrameMemSizeLabelLay )
    {
		boolUseSysMemPrimaryBackLay	= FALSE;
		boolUseSysMemGraphBackLay	= FALSE;
		boolUseSysMemImageLay	= FALSE;
		boolUseSysMemGraphLay	= FALSE;
		boolUseSysMemLabelLay	= TRUE;
		boolUseSysMemUScaleLay	= TRUE;
		boolUseSysMemHeaderLay	= TRUE;
		boolUseSysMemAngleLay	= TRUE;

		m_dwFrameMemSizeUsed	= dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay;

        DebugMessage("Using system memory partly (not enough memory)");
	}
    else if( ddcaps.dwVidMemTotal <= 
		dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay + dwFrameMemSizeLabelLay + dwFrameMemSizeUScaleLay )
    {
		boolUseSysMemPrimaryBackLay	= FALSE;
		boolUseSysMemGraphBackLay	= FALSE;
		boolUseSysMemImageLay	= FALSE;
		boolUseSysMemGraphLay	= FALSE;
		boolUseSysMemLabelLay	= FALSE;
		boolUseSysMemUScaleLay	= TRUE;
		boolUseSysMemHeaderLay	= TRUE;
		boolUseSysMemAngleLay	= TRUE;

		m_dwFrameMemSizeUsed	=
			dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay + dwFrameMemSizeLabelLay;

        DebugMessage("Using system memory partly (not enough memory)");
	}
	else if( ddcaps.dwVidMemTotal <= 
		dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay + dwFrameMemSizeLabelLay + dwFrameMemSizeUScaleLay + dwFrameMemSizeHeaderLay )
	{
		boolUseSysMemPrimaryBackLay	= FALSE;
		boolUseSysMemGraphBackLay	= FALSE;
		boolUseSysMemImageLay	= FALSE;
		boolUseSysMemGraphLay	= FALSE;
		boolUseSysMemLabelLay	= FALSE;
		boolUseSysMemUScaleLay	= FALSE;
		boolUseSysMemHeaderLay	= TRUE;
		boolUseSysMemAngleLay	= TRUE;

		m_dwFrameMemSizeUsed	=
			dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay + dwFrameMemSizeLabelLay + dwFrameMemSizeUScaleLay;

		DebugMessage("Using system memory partly (not enough memory)");
	}
	else if( ddcaps.dwVidMemTotal <= 
		dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay + dwFrameMemSizeLabelLay + dwFrameMemSizeUScaleLay + dwFrameMemSizeHeaderLay + dwFrameMemSizeAngleLay)
	{
		boolUseSysMemPrimaryBackLay	= FALSE;
		boolUseSysMemGraphBackLay	= FALSE;
		boolUseSysMemImageLay	= FALSE;
		boolUseSysMemGraphLay	= FALSE;
		boolUseSysMemLabelLay	= FALSE;
		boolUseSysMemUScaleLay	= FALSE;
		boolUseSysMemHeaderLay	= FALSE;
		boolUseSysMemAngleLay	= TRUE;

		m_dwFrameMemSizeUsed	=
			dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay + dwFrameMemSizeLabelLay + dwFrameMemSizeUScaleLay + dwFrameMemSizeHeaderLay;

		DebugMessage("Using system memory partly (not enough memory)");
	}
	else
	{
		boolUseSysMemPrimaryBackLay	= FALSE;
		boolUseSysMemGraphBackLay	= FALSE;
		boolUseSysMemImageLay	= FALSE;
		boolUseSysMemGraphLay	= FALSE;
		boolUseSysMemLabelLay	= FALSE;
		boolUseSysMemUScaleLay	= FALSE;
		boolUseSysMemHeaderLay	= FALSE;
		boolUseSysMemAngleLay	= FALSE;

		m_dwFrameMemSizeUsed	=
			dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay + dwFrameMemSizeLabelLay + dwFrameMemSizeUScaleLay + dwFrameMemSizeHeaderLay + dwFrameMemSizeAngleLay;

		DebugMessage("Using video memory at all");	
	}

    if( ddcaps.dwCaps & DDCAPS_COLORKEY )
    {
        if( ddcaps.dwCKeyCaps & DDCKEYCAPS_SRCBLT )
        {
            m_blSrcColorkey = TRUE;
			DebugMessage( "Can do Src colorkey in hardware" );
        }

        if( ddcaps.dwCKeyCaps & DDCKEYCAPS_DESTBLT )
        {
            m_blDestColorkey = TRUE;
            DebugMessage( "Can do Dest colorkey in hardware" );
        }
    }
    else
    {
        DebugMessage( "Can't do color key in hardware!" );
    }

    // Create the primary surface
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    ddrval = m_lpDD->CreateSurface( &ddsd, &m_lpDDSPrimary, NULL );
    if( ddrval != DD_OK )
	{
        DebugMessage("Create Primary Surface failed err=%d", ddrval);
		return FALSE;
	}

    // create a clipper for the primary surface
    ddrval = m_lpDD->CreateClipper( 0, &m_lpClipper, NULL );
    if( ddrval != DD_OK )
	{
        DebugMessage("Create Clipper failed err=%d", ddrval);
		return FALSE;
	}
    
    ddrval = m_lpClipper->SetHWnd( 0, m_pScreenWnd->m_hWnd );
    if( ddrval != DD_OK )
	{
        DebugMessage("Clipper SetHWnd failed err=%d", ddrval);
		return FALSE;
	}

    ddrval = m_lpDDSPrimary->SetClipper( m_lpClipper );
    if( ddrval != DD_OK )
	{
        DebugMessage("SetClipper failed err=%d", ddrval);
		return FALSE;
	}

	// create a DirectDrawSurface for Primary BackLay
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize			= sizeof(ddsd);
    ddsd.dwFlags		= DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth		= GetSystemMetrics(SM_CXSCREEN);
    ddsd.dwHeight		= GetSystemMetrics(SM_CYSCREEN);
	if( boolUseSysMemPrimaryBackLay )
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
    ddrval = m_lpDD->CreateSurface( &ddsd, &m_lpDDSPrimaryBackLay, NULL );
    if ( ddrval != DD_OK )
	{
        DebugMessage("Create BackLay Surface failed err=%d", ddrval);
		return FALSE;
	}

	// create a DirectDrawSurface for Graph BackLay
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize			= sizeof(ddsd);
    ddsd.dwFlags		= DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth		= m_szGraph.cx;
    ddsd.dwHeight		= m_szGraph.cy;
	if( boolUseSysMemGraphBackLay )
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
    ddrval = m_lpDD->CreateSurface( &ddsd, &m_lpDDSGraphBackLay, NULL );
    if ( ddrval != DD_OK )
	{
        DebugMessage("Create Graph BackLay Surface failed err=%d", ddrval);
		return FALSE;
	}

	// create a DirectDrawSurface for ImageLay
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize			= sizeof(ddsd);
    ddsd.dwFlags		= DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth		= m_szImage.cx;
    ddsd.dwHeight		= m_szImage.cy;
	if( boolUseSysMemImageLay )
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
    ddrval = m_lpDD->CreateSurface( &ddsd, &m_lpDDSImageLay, NULL );
    if ( ddrval != DD_OK )
	{
        DebugMessage("Create ImageLay Surface failed err=%d", ddrval);
		return FALSE;
	}

    // create a DirectDrawSurface for GraphLay
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize			= sizeof(ddsd);
    ddsd.dwFlags		= DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth		= m_szGraph.cx;
    ddsd.dwHeight		= m_szGraph.cy;
	if( boolUseSysMemGraphLay )
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
    ddrval = m_lpDD->CreateSurface( &ddsd, &m_lpDDSGraphLay, NULL );
    if ( ddrval != DD_OK )
	{
        DebugMessage("Create GraphLay Surface failed err=%d", ddrval);
		return FALSE;
	}

    // create a DirectDrawSurface for LabelLay
	BOOL bRet = FALSE;
	bRet = m_pLabelControler->CreateSurface(boolUseSysMemLabelLay);
	if(!bRet) {return FALSE;}

    // create a DirectDrawSurface for UScaleLay
	bRet = m_pMobileUScaleControler->CreateSurface(boolUseSysMemUScaleLay);
	if(!bRet) {return FALSE;}

	// create a DirectDrawSurface for HeaderLay
	bRet = m_pHeaderControler->CreateSurface(boolUseSysMemHeaderLay);
	if(!bRet) {return FALSE;}

	// create a DirectDrawSurface for AngleLay
	bRet = m_pAngleControler->CreateSurface(boolUseSysMemAngleLay);
	if(!bRet) {return FALSE;}

	// init the color key
	DWORD	dwColorKey = 0;		
	dwColorKey = DDColorMatch(m_lpDDSGraphLay, CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrTransparent]);
	
    if (m_blSrcColorkey)
    {
		DDCOLORKEY		ddck;
		ddck.dwColorSpaceLowValue	= dwColorKey;
		ddck.dwColorSpaceHighValue	= dwColorKey;
		m_lpDDSGraphBackLay->SetColorKey( DDCKEY_SRCBLT, &ddck );
		m_lpDDSGraphLay->SetColorKey( DDCKEY_SRCBLT, &ddck );
		m_pLabelControler->m_lpDDSLabelLay->SetColorKey( DDCKEY_SRCBLT, &ddck );
		m_pMobileUScaleControler->m_lpDDSMobileUScaleLay->SetColorKey( DDCKEY_SRCBLT, &ddck );
		m_pHeaderControler->m_lpDDSHeaderLay->SetColorKey( DDCKEY_SRCBLT, &ddck );
		m_pAngleControler->m_lpDDSAngleLay->SetColorKey( DDCKEY_SRCBLT, &ddck );
	}

	// Init Lay
	HBRUSH brBackBrush = CreateSolidBrush( CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrTransparent] );

//	HPEN hPenOld;
	HPEN hPenR = CreatePen(PS_SOLID, 0, RGB(0xFF,0,0));
	HPEN hPenG = CreatePen(PS_SOLID, 0, RGB(0,0xFF,0));
	HPEN hPenB = CreatePen(PS_SOLID, 0, RGB(0,0,0xFF));

	CRect	rcFill;

	ddrval = m_lpDDSGraphBackLay->GetDC( &hdc );
	if( ddrval == DD_OK )
	{
		rcFill = CRect(0,0,m_szGraph.cx,m_szGraph.cy);
		FillRect( hdc, (LPRECT)rcFill, brBackBrush );
		m_lpDDSGraphBackLay->ReleaseDC(hdc);
	}

	ddrval = m_lpDDSGraphLay->GetDC( &hdc );
	if( ddrval == DD_OK )
	{
		rcFill = CRect(0,0,m_szGraph.cx,m_szGraph.cy);
		FillRect( hdc, (LPRECT)rcFill, brBackBrush );
		m_lpDDSGraphLay->ReleaseDC(hdc);
	}

	ddrval = m_pLabelControler->m_lpDDSLabelLay->GetDC( &hdc );
	if( ddrval == DD_OK )
	{
		rcFill = CRect(0,0,m_pLabelControler->m_szLabel.cx,m_pLabelControler->m_szLabel.cy);
		FillRect( hdc, (LPRECT)rcFill, brBackBrush );
		m_pLabelControler->m_lpDDSLabelLay->ReleaseDC(hdc);
	}

	ddrval = m_pMobileUScaleControler->m_lpDDSMobileUScaleLay->GetDC( &hdc );
	if( ddrval == DD_OK )
	{
		rcFill = CRect(0,0,m_pMobileUScaleControler->m_szUScale.cx,m_pMobileUScaleControler->m_szUScale.cy);
		FillRect( hdc, (LPRECT)rcFill, brBackBrush );
		m_pMobileUScaleControler->m_lpDDSMobileUScaleLay->ReleaseDC(hdc);
	}

	ddrval = m_pHeaderControler->m_lpDDSHeaderLay->GetDC( &hdc );
	if( ddrval == DD_OK )
	{
		rcFill = CRect(0,0,m_pHeaderControler->m_szHeader.cx,m_pHeaderControler->m_szHeader.cy);
		FillRect( hdc, (LPRECT)rcFill, brBackBrush );
		m_pHeaderControler->m_lpDDSHeaderLay->ReleaseDC(hdc);
	}

	ddrval = m_pAngleControler->m_lpDDSAngleLay->GetDC( &hdc );
	if( ddrval == DD_OK )
	{
//		rcFill = CRect(0,0,m_pAngleControler->m_szAngle.cx,m_pAngleControler->m_szAngle.cy);
		rcFill = CRect(0,0,m_szGraph.cx,m_szGraph.cy);
		FillRect( hdc, (LPRECT)rcFill, brBackBrush );
		m_pAngleControler->m_lpDDSAngleLay->ReleaseDC(hdc);
	}

	DeleteObject( hPenR );
	DeleteObject( hPenG );
	DeleteObject( hPenB );

	DeleteObject( brBackBrush );

    if ( !m_pFrameRateControler->InitFrameRate(m_dwFrameMemSizeUsed,m_dwVidMemTotal) )
	{
        DebugMessage("InitFrameRate failed err");
		return FALSE;
	}

	if ( !m_pLabelControler->InitLabelLay(m_bSplit,m_bDet2nd) )
	{
        DebugMessage("InitLabelLay failed err");
		return FALSE;
	}

	if ( !m_pMobileUScaleControler->InitMobileUScaleLay(m_pLabelControler,m_rcScreenClientRect) )
	{
        DebugMessage("InitMobileUScale failed err");
		return FALSE;
	}

	if ( !m_pOscillographControler->InitOscillographLay(m_dwFrameMemSizeUsed,m_dwVidMemTotal,m_blSrcColorkey,m_bDebugZ) )
	{
        DebugMessage("InitOscillographLay failed err");
		return FALSE;
	}

	if ( !m_pHeaderControler->InitHeaderLay(m_bSplit) )
	{
		DebugMessage("InitHeaderLay failed err");
		return FALSE;
	}

	if ( !m_pAngleControler->InitAngleLay(m_pLabelControler,m_rcScreenClientRect) )
	{
		DebugMessage("InitAngleLay failed err");
		return FALSE;
	}

	CMCICardCommonSetting::Instance().m_UpdateRectsImage.Add( CRect(0, 0, m_szImage.cx, m_szImage.cy) );
	m_boolReady	= TRUE;

	return TRUE;
}



DWORD CMCICard::ImageDibWrite( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	LPSTR		lpDIBHdr, lpDIBBits;
	LPSTR		lpBuffLine;
	LONG		lBuffLineOffset;
	LPSTR		lpImageWindowLine;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;
	DWORD		i, iMaxLine;

	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;
	
	m_mutexImageDib.Lock();

	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hdibImage);
	if ( lpDIBHdr == NULL )
	{
		m_mutexImageDib.Unlock();
		return 0;
	}

	lpDIBBits = ::FindDIBBits(lpDIBHdr);
	if ( lpDIBBits == NULL )
	{
		::GlobalUnlock((HGLOBAL) m_hdibImage);
		m_mutexImageDib.Unlock();
		return 0;
	}

	if ( lBuffSize >= rcFrameDestWindow.Width() * rcFrameDestWindow.Height() )
	{
		iMaxLine = rcFrameDestWindow.Height();
	}
	else
	{
		iMaxLine = lBuffSize / rcFrameDestWindow.Width();
	}

	lBuffLineOffset	= rcFrameDestWindow.Width();
	lpBuffLine		= (LPSTR)lpBuff;

	lImageLineOffset	= CMCICardCommonSetting::Instance().m_szImage.cx;
	lpImageWindowLine	= &(lpDIBBits[ (m_szImage.cy - rcImage.top - 1) * lImageLineOffset + rcImage.left ] );

	for ( i=0; i<iMaxLine; i++ )
	{
		memcpy( (void *)lpImageWindowLine, (const void *)lpBuffLine, lBuffLineOffset );
		lpBuffLine			+= lBuffLineOffset;
		lpImageWindowLine	-= lImageLineOffset;
	}
	::GlobalUnlock((HGLOBAL) m_hdibImage);

	CMCICardCommonSetting::Instance().m_UpdateRectsImage.Add( rcFrameDestWindow );

	m_mutexImageDib.Unlock();

	i = iMaxLine * lBuffLineOffset;
	m_dwByteCount += i;
	m_dwInputCount ++;

	return i;
}

// 10.08
DWORD CMCICard::ImageDibWriteImp( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	LPSTR		lpDIBHdr, lpDIBBits;
	LPSTR		lpBuffLine;
	LONG		lBuffLineOffset;
//	LPSTR		lpImageWindowLine;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;
	DWORD		i, iMaxLine;

	////////////////////////////////////////////////////////////////////
	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;
	
	m_mutexImageDib.Lock();

	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hdibImage);
	if ( lpDIBHdr == NULL )
	{
		m_mutexImageDib.Unlock();
		return 0;
	}

	lpDIBBits = ::FindDIBBits(lpDIBHdr);
	if ( lpDIBBits == NULL )
	{
		::GlobalUnlock((HGLOBAL) m_hdibImage);
		m_mutexImageDib.Unlock();
		return 0;
	}

	
	iMaxLine = lBuffSize /5;
	WORD wx, wy;

	lBuffLineOffset		= rcFrameDestWindow.Width();
	lpBuffLine			= (LPSTR)lpBuff;

	lImageLineOffset	= m_szImage.cx;

	BYTE bParam = 0x0;
	for (i=0; i<iMaxLine; i++)
	{
		bParam	= (BYTE)(((LPBYTE)lpBuffLine)[i*5]);							// low byte of x
		wx		= (WORD)bParam | ((WORD)(((LPBYTE)lpBuffLine)[i*5+1]) << 8);	// combine low & high of x
		bParam	= (BYTE)(((LPBYTE)lpBuffLine)[i*5+2]);							// low byte of y
		wy		= (WORD)bParam | ((WORD)(((LPBYTE)lpBuffLine)[i*5+3]) << 8);	// combine low & high of y
		// set z value to point(x,y)
		((LPBYTE)lpDIBBits)[ 
		  (m_szImage.cy - wy - 1) * lImageLineOffset + wx ]  = ((LPBYTE)lpBuffLine)[i*5+4];
	}
	
	::GlobalUnlock((HGLOBAL) m_hdibImage);

	CMCICardCommonSetting::Instance().m_UpdateRectsImage.Add( rcFrameDestWindow );

	m_mutexImageDib.Unlock();

	i = iMaxLine * lBuffLineOffset;
	m_dwByteCount += i;
	m_dwInputCount ++;

	return i;
}
// 10.08

BOOL CMCICard::InitData( DWORD dwExpectantDMABlockSize, int nXOffset, int nYOffset )
{
	m_dwExpectantDMABlockSize = dwExpectantDMABlockSize;

	ZeroMemory(m_pPixX, LOOP_COUNTS*102400/4 * sizeof(WORD));
	ZeroMemory(m_pPixY, LOOP_COUNTS*102400/4 * sizeof(WORD));
	ZeroMemory(m_pPixZ, LOOP_COUNTS*102400/4 * sizeof(WORD));
	ZeroMemory(m_pPixX2, LOOP_COUNTS*102400/4 * sizeof(WORD));
	ZeroMemory(m_pPixY2, LOOP_COUNTS*102400/4 * sizeof(WORD));
	ZeroMemory(m_pPixZ2, LOOP_COUNTS*102400/4 * sizeof(WORD));
	ZeroMemory(m_pPixZmix, LOOP_COUNTS*102400/4 * sizeof(WORD));
	ZeroMemory(m_pPixZOrigin, LOOP_COUNTS*102400/4 * sizeof(WORD));
	// 2021.06 原始数据存储
	if( m_pPixZsrc	!= NULL )
	{
		delete [] m_pPixZsrc;
		m_pPixZsrc = NULL;
	}
	if( m_pPixZsrc2 != NULL )
	{
		delete [] m_pPixZsrc2;
		m_pPixZsrc2 = NULL;
	}

	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	m_pPixZsrc	= new WORD[ m_szImage.cx *m_szImage.cy ];
	m_pPixZsrc2	= new WORD[ m_szImage.cx *m_szImage.cy ];
	ZeroMemory(m_pPixZsrc,	m_szImage.cx *m_szImage.cy * sizeof(WORD));
	ZeroMemory(m_pPixZsrc2,	m_szImage.cx *m_szImage.cy * sizeof(WORD));

	m_dwX			= 0xFFFF;
	m_dwY			= 0xFFFF;
	m_dwZa			= 0xFFFF;
	m_nIndexX		= 0;		// 有效的数据的X序号
	m_nIndexY		= 0;		// 有效的数据的Y序号
	m_nX			= 0;
	m_nY			= 0;
	if( m_szImage.cx > 4095 )
		m_nXOffset	= -1;
	else
		m_nXOffset	= nXOffset;
	if( m_szImage.cy > 4095 )
		m_nYOffset	= -1;
	else
		m_nYOffset	= nYOffset;
	m_bPeakX		= FALSE;
	m_bPeakY		= FALSE;

	m_bDataNeedUpdated = FALSE;
	m_bStopScan		= FALSE;

	g_index			= 0;

	m_bSyncFinished	= FALSE;
	m_bFindFirstBOF	= FALSE;

	m_nSharedImageBuffIndexRead		= 0;
	m_nSharedImageBuffIndexWrite	= 0;

	m_nCurrentY1	= 0;
	m_nCurrentY2	= 0;

	if( m_hFileLog != NULL )
	{
		CloseHandle(m_hFileLog);
		m_hFileLog = NULL;
	}
	if( m_hLineDataLog != NULL )
	{
		CloseHandle(m_hLineDataLog);
		m_hLineDataLog = NULL;
	}

	CString str = "";
	if( (m_nusbFunction == 3) && m_bNeedMon )
	{
		CString strTemp;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
		str.Format( "%szLine%s-%05d.log", g_exeFullPath, strTemp, g_index );
		m_hLineDataLog = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		str.Format( "%szOsc%s-%05d.log", g_exeFullPath, strTemp, g_index );
	}

	m_pOscillographControler->InitData(str);

	return TRUE;
}

BOOL CMCICard::ClearData()
{
	m_bStopScan						= TRUE;
	// 15.10.08 与MainLoop中有同步问题？？？
	// 冻结后，MainLoop中Read还是不等于Write，就会进入do循环
	m_nSharedImageBuffIndexRead		= 0;
	m_nSharedImageBuffIndexWrite	= 0;

	if( (m_nusbFunction == 3) && m_bNeedMon )
	{
		if( m_hLineDataLog != NULL )
		{
			CloseHandle(m_hLineDataLog);
			m_hLineDataLog = NULL;
		}

        m_pOscillographControler->CloseLogHandle();
	}

	if( m_nusbFunction == 6 )	// 同步照相
	{
		CRect rcIndication = CRect(0,m_rcDestFrame.top,CMCICardCommonSetting::Instance().m_sizeReso.cx *8 /1024,m_rcDestFrame.bottom);
		PhotoIndication( rcIndication, RGB(0,0,0) );
	}
	return TRUE;
}

BOOL CMCICard::PostData( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	m_mutexImageUSB.Lock();
	memcpy( (void*)m_pBuffUSB, (const void*)lpBuff, lBuffSize * sizeof( unsigned short ) );
	m_mutexImageUSB.Unlock();
	m_bDataNeedUpdated = TRUE;

	return TRUE;
}

BOOL CMCICard::GetData( /*LPVOID lpBuff, long lBuffSize, */CRect& rcFrameDestWindow )
{
	m_mutexImageUSB.Lock();
	long lBuffSize = LOOP_COUNTS*(m_dwExpectantDMABlockSize +512);
	LPVOID lpBuff = (void*)(CGraphBuffMgr::Instance().m_pSharedImageBuff[m_nSharedImageBuffIndexRead]);

	std::string sFunctionID = GetFunctionID("m_bSplit",m_bSplit,"m_nDataStoreMode",(bool)m_nDataStoreMode,"GetData");
	IMCICardGetDataLogicExecutor* pExecuter = Factory<IMCICardGetDataLogicExecutor,std::string>::Instance().BuildProduct(sFunctionID);
	if (pExecuter)
	{
		pExecuter->GetData(this,lpBuff,lBuffSize,rcFrameDestWindow);
		delete pExecuter;pExecuter = NULL; 
	}
	
	m_bDataNeedUpdated = FALSE;

	m_mutexImageUSB.Unlock();
	return TRUE;
}

// 2021.06
void CMCICard::GetImageDataSrc( int nIndex, WORD* pData )
{
	CSize& m_sizeReso = CMCICardCommonSetting::Instance().m_sizeReso;
	if( nIndex < 1 )
	{
		if( m_pPixZsrc != NULL )
			memcpy( pData, m_pPixZsrc, m_sizeReso.cx *m_sizeReso.cy *sizeof(WORD) );
	}
	else
	{
		if( m_pPixZsrc2 != NULL )
			memcpy( pData, m_pPixZsrc2, m_sizeReso.cx *m_sizeReso.cy *sizeof(WORD) );
	}
}

// 11.11.28
DWORD CMCICard::ImageDibWriteNewXYZ( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	if( lpBuff == NULL )
		return 0;

	LPSTR		lpDIBHdr, lpDIBBits;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;
	DWORD		i;

	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	CRectManager& m_UpdateRectsImage = CMCICardCommonSetting::Instance().m_UpdateRectsImage;

	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;
	
	m_mutexImageDib.Lock();

	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hdibImage);
	if ( lpDIBHdr == NULL )
	{
		m_mutexImageDib.Unlock();
		return 0;
	}

	lpDIBBits = ::FindDIBBits(lpDIBHdr);
	if ( lpDIBBits == NULL )
	{
		::GlobalUnlock((HGLOBAL) m_hdibImage);
		m_mutexImageDib.Unlock();
		return 0;
	}

	lImageLineOffset	= m_szImage.cx;

	if( m_bNeedMon )
	{
		unsigned long cbRet;
		char Buf[32];
		CString str, strTemp;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M");
		str.Format( "%sz%s-%05d-set.log", g_exeFullPath, strTemp, g_index );
		m_hFileLog = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		sprintf_s(Buf, "/BEGIN set z value\\\r\n");
		WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
	}

	DWORD dwOffset = 0;
	DWORD dwCounts = 0;
	DWORD dwIndexY1, dwIndexY2;
	unsigned long lSize = 0;
	for( int j=0; j<LOOP_COUNTS; j++ )
	{
		lSize = ((unsigned short*)lpBuff)[dwOffset];
		if( dwCounts > 0 )
			m_pOscillographControler->UpdateOscillographNew( dwCounts, m_pPixX, m_pPixY, (LPWORD)m_pPixZ, -1, 0, m_pOscillographControler->m_szOscill.cx, m_bDebugZ,m_nDataStoreMode);

		if( dwCounts > 0 )
		{
			dwIndexY1 = m_pPixY[0];
			dwIndexY2 = m_pPixY[dwCounts -1];
			for( i=0; i<dwCounts; i++ )
			{
				// set z value to point(x,y)
				((LPBYTE)lpDIBBits)[ (m_szImage.cy - m_pPixY[i] - 1) * lImageLineOffset + m_pPixX[i] ]  = (BYTE)m_pPixZ[i];
			}

			if( m_bNeedMon && m_hFileLog != NULL )
			{
				unsigned long cbRet;
				char Buf[32];
				for( i=0; i<dwCounts; i++ )
				{
					sprintf_s(Buf, "%04X, %04X, %04X\r\n", m_pPixX[i], m_pPixY[i], m_pPixZ[i] );
					WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
				}
			}
		}

		dwOffset = (j+1) *(m_dwExpectantDMABlockSize +512);
		g_index++;

		// Use the delete operator as follows:
		if( m_szImage.cy > 512 )
		{
			if( dwIndexY1 < dwIndexY2 )
			{
				m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, dwIndexY1, m_rcDestFrame.right, dwIndexY2+1) );
//				LowpassFilter( 0, 0, dwIndexY1, m_szImage.cx, dwIndexY2, lpDIBBits );
			}
			else
			{
				m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, dwIndexY1, m_rcDestFrame.right, m_szImage.cy) );
				m_UpdateRectsImage.Add( CRect(m_rcDestFrame.left, m_rcDestFrame.top, m_szImage.cx, dwIndexY2+1) );
//				LowpassFilter( 0, 0, dwIndexY1, m_szImage.cx, m_szImage.cy, lpDIBBits );
//				LowpassFilter( 0, 0, 0, m_szImage.cx, dwIndexY2, lpDIBBits );
			}
		}
		else
			m_UpdateRectsImage.Add( rcFrame );
	}
	if( m_bNeedMon )
	{
		if( m_hFileLog != NULL )
		{
			unsigned long cbRet;
			char Buf[32];
			sprintf_s(Buf, "\\END set z value/\r\n\r\n");
			WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
			CloseHandle(m_hFileLog);
			m_hFileLog = NULL;
		}
	}

	::GlobalUnlock((HGLOBAL) m_hdibImage);

	m_mutexImageDib.Unlock();

	return i;
}

// 11.12.05
DWORD CMCICard::ImageDibWriteNewZZZ( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	if( lpBuff == NULL )
		return 0;
	
	LPSTR		lpDIBHdr, lpDIBBits;
	LONG		lBuffLineOffset;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;
	DWORD		i, nloops, iMaxLine;

	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	CRectManager& m_UpdateRectsImage = CMCICardCommonSetting::Instance().m_UpdateRectsImage;

	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;
	
	m_mutexImageDib.Lock();

	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hdibImage);
	if ( lpDIBHdr == NULL )
	{
		m_mutexImageDib.Unlock();
		return 0;
	}

	lpDIBBits = ::FindDIBBits(lpDIBHdr);
	if ( lpDIBBits == NULL )
	{
		::GlobalUnlock((HGLOBAL) m_hdibImage);
		m_mutexImageDib.Unlock();
		return 0;
	}

	if ( lBuffSize >= rcFrameDestWindow.Width() * rcFrameDestWindow.Height() )
	{
		iMaxLine = rcFrameDestWindow.Height();
	}
	else
	{
		iMaxLine = lBuffSize / rcFrameDestWindow.Width();
	}

	lBuffLineOffset	= rcFrameDestWindow.Width();
//	lpBuffLine		= (LPSTR)lpBuff;

	lImageLineOffset	= m_szImage.cx;
	
	if( m_bNeedMon )
	{
		unsigned long cbRet;
		char Buf[32];
		CString str, strTemp;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M");
		str.Format( "%sz%s-%05d-set.log", g_exeFullPath, strTemp, g_index );
		m_hFileLog = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		sprintf_s(Buf, "/BEGIN set z value\\\r\n");
		WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
	}
	
	CSize sz = m_szImage;
	DWORD dwOffset = 0;
	DWORD dwCounts = 0;
	WORD dwIndexY1, dwIndexY2;
	unsigned long lSize = 0;
	unsigned long cbRet;
	char Buf[32];
	for( nloops=0; nloops<LOOP_COUNTS; nloops++ )
	{
		lSize = ((unsigned short*)lpBuff)[dwOffset];

		dwCounts = PixConvertZZZ( &(((unsigned short*)lpBuff)[dwOffset]), lSize*2, m_pPixX, m_pPixY, m_pPixZ, dwIndexY1, dwIndexY2 );
		if( dwCounts > 0 )
		{
			m_pOscillographControler->UpdateOscillographNew( dwCounts, m_pPixX, m_pPixY, (LPWORD)m_pPixZ, -1, 0, m_pOscillographControler->m_szOscill.cx, m_bDebugZ,m_nDataStoreMode);
			for( i=0; i<dwCounts; i++ )
			{
				// set z value to point(x,y)
				((LPBYTE)lpDIBBits)[ (m_szImage.cy - m_pPixY[i] - 1) * lImageLineOffset + m_pPixX[i] ]  = (BYTE)m_pPixZ[i];
			}

			if( m_bNeedMon && m_hFileLog != NULL )
			{
				for( i=0; i<dwCounts; i++ )
				{
					sprintf_s(Buf, "%04X, %04X, %04X\r\n", m_pPixX[i], m_pPixY[i], m_pPixZ[i] );
					WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
				}
			}
		}

		dwOffset = (nloops+1) *(m_dwExpectantDMABlockSize +512);
		g_index++;

		if( m_szImage.cy > 512 )
		{
			if( dwIndexY1 < dwIndexY2 )
				m_UpdateRectsImage.Add( CRect(0, dwIndexY1, m_szImage.cx, dwIndexY2+1) );
			else
			{
				m_UpdateRectsImage.Add( CRect(0, dwIndexY1, m_szImage.cx, m_szImage.cy) );
				m_UpdateRectsImage.Add( CRect(0, 0, m_szImage.cx, dwIndexY2+1) );
			}
		}
		else
			m_UpdateRectsImage.Add( rcFrame );
	}
	if( m_bNeedMon )
	{
		if( m_hFileLog != NULL )
		{
			unsigned long cbRet;
			char Buf[32];
			sprintf_s(Buf, "\\END set z value/\r\n\r\n");
			WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
			CloseHandle(m_hFileLog);
			m_hFileLog = NULL;
		}
	}

	::GlobalUnlock((HGLOBAL) m_hdibImage);

	m_mutexImageDib.Unlock();

	return i;
}

///////////////////////////////////////////////////////////////////////////
// 像素数据转换：ZZZ模式
// 输入:	FrameData: 数据存储区
//			dwPixX: 用来存储X值的数组
//			dwPixY: 用来存储Y值的数组
//			dwPixZ: 用来存储Z值的数组
// 输出:	实际得到的有效数据的个数
DWORD CMCICard::PixConvertZZZ( unsigned short* pnFrameData, long lBytesTransferred,
								  WORD* pPixX, WORD* pPixY, WORD* pPixZ,
								  WORD& dwIndexY1, WORD& dwIndexY2 )
{
	// 因为每批数据的第一个数是总个数，所以nLoop要+1，且循环时从1开始
	long nLoop = lBytesTransferred/2 +1;
	long i;

	///////////////////////////////////////////////////////////////////////////
	BOOL bIsBOL, bIsBOF;
	unsigned short dwData	= 0;
	DWORD dwMaskBOL	= 0x1000;	// BOL 的掩码
	DWORD dwMaskBOF	= 0x3000;	// BOF 的掩码
	DWORD dwBitsOn;
//	CSize sz = m_szImage;
	DWORD dwDestWidth	= m_rcDestFrame.Width();
	DWORD dwDestHeight	= m_rcDestFrame.Height();
	int nLineCounts	= ValleyX +dwDestWidth +PeakX +FallX;
	int nFrameCounts= ValleyY +dwDestHeight +PeakY +FallY;
	bIsBOL		= FALSE;		// 当前数据是BOL
	bIsBOF		= FALSE;		// 当前数据是BOF
	dwIndexY1	= m_nIndexY;
	int nIndex	= 0;			// 有效的数据个数

	for( i=1; i<nLoop; i++ )
	{
		dwData = pnFrameData[i];			// 得到16进制的数据
		if( dwData == 0x3456 )
			continue;

		dwBitsOn = dwData & 0x3000;			// 得到所需的掩码位

		bIsBOL = (dwBitsOn == dwMaskBOL);
		if( bIsBOL )
		{
//			if( m_nY > ValleyY -1 && m_nY < ValleyY +sz.cy
//				&& m_nX > ValleyX -1 && m_nX < ValleyX +sz.cx )
			if( m_nY > 0 && m_nY < dwDestHeight +1
				&& m_nX > 0 && m_nX < dwDestWidth +1 )
			{
				m_nIndexX = 0;
				m_nIndexY++;
				if( m_nIndexY > dwDestHeight -1 )
					m_nY = 0;
			}
			m_nX = 0;
			m_nY++;
			if( m_nY > nFrameCounts -1 )
				m_nY = 0;
		}
		else
		{
			bIsBOF = (dwBitsOn == dwMaskBOF);
			if( bIsBOF )
			{
				m_nX		= 0;
				m_nY		= 0;
				m_nIndexX	= 0;
				m_nIndexY	= 0;
			}
			else
			{
				m_nX++;
			}
		}

		if( m_nY > 0 && m_nY < dwDestHeight +1
			&& m_nX > 0 && m_nX < dwDestWidth +1 )
		{
			// 填充数组
			pPixX[nIndex] = m_nIndexX +m_rcDestFrame.left;
			pPixY[nIndex] = m_nIndexY +m_rcDestFrame.top;
//			pPixZ[nIndex] = ( dwData & 0x0FFF ) /16;
			pPixZ[nIndex] = ( dwData & 0x0FF0 ) >> 4;
			nIndex++;
			m_nIndexX++;
			if( m_nIndexX > dwDestWidth -1 )
			{
				m_nIndexX = 0;
				m_nIndexY++;
			}
			if( m_nIndexY > dwDestHeight -1 )
			{
				m_nIndexX = 0;
				m_nIndexY = 0;
			}
		}
//		bIsBOL	= FALSE;
//		bIsBOF	= FALSE;
	}

	dwIndexY2 = m_nIndexY;
	return nIndex;//(sz.cx * sz.cy);
}

DWORD CMCICard::ImageDibWriteNewZZZ2( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	LPSTR		lpDIBHdr, lpDIBBits;
//	LPSTR		lpBuffLine;
	LONG		lBuffLineOffset;
//	LPSTR		lpImageWindowLine;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;
	DWORD		i, nloops, iMaxLine;

	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	CRectManager& m_UpdateRectsImage = CMCICardCommonSetting::Instance().m_UpdateRectsImage;

	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;
	
	m_mutexImageDib.Lock();

	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hdibImage);
	if ( lpDIBHdr == NULL )
	{
		m_mutexImageDib.Unlock();
		return 0;
	}

	lpDIBBits = ::FindDIBBits(lpDIBHdr);
	if ( lpDIBBits == NULL )
	{
		::GlobalUnlock((HGLOBAL) m_hdibImage);
		m_mutexImageDib.Unlock();
		return 0;
	}

	if ( lBuffSize >= rcFrameDestWindow.Width() * rcFrameDestWindow.Height() )
	{
		iMaxLine = rcFrameDestWindow.Height();
	}
	else
	{
		iMaxLine = lBuffSize / rcFrameDestWindow.Width();
	}

	lBuffLineOffset	= rcFrameDestWindow.Width();
//	lpBuffLine		= (LPSTR)lpBuff;

	lImageLineOffset	= m_szImage.cx;
	
	if( m_bNeedMon )
	{
		unsigned long cbRet;
		char Buf[32];
		CString str, strTemp;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M");
		str.Format( "%sz%s-%05d-set.log", g_exeFullPath, strTemp, g_index );
		m_hFileLog = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		sprintf_s(Buf, "/BEGIN set z value\\\r\n");
		WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
	}
	
	CSize sz = m_szImage;
	DWORD dwOffset = 0;
	DWORD dwCounts = 0;
	WORD dwIndexY1, dwIndexY2;
	unsigned long lSize = 0;
	unsigned long cbRet;
	char Buf[32];
	for( nloops=0; nloops<LOOP_COUNTS; nloops++ )
	{
		lSize = ((unsigned short*)lpBuff)[dwOffset];
		dwCounts = PixConvertZZZ2( &(((unsigned short*)lpBuff)[dwOffset]), lSize*2, m_pPixX, m_pPixY, m_pPixZ, dwIndexY1, dwIndexY2 );

		if( dwCounts > 0 )
		{
			for( i=0; i<dwCounts; i++ )
			{
				// set z value to point(x,y)
				((LPBYTE)lpDIBBits)[ (m_szImage.cy - m_pPixY[i] - 1) * lImageLineOffset + m_pPixX[i] ]  = (BYTE)m_pPixZ[i];
			}

			if( m_bNeedMon && m_hFileLog != NULL )
			{
				for( i=0; i<dwCounts; i++ )
				{
					sprintf_s(Buf, "%04X\r\n", m_pPixZ[i] );
					WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
				}
			}
		}

		dwOffset = (nloops+1) *(m_dwExpectantDMABlockSize +512);
		g_index++;

		if( m_szImage.cy > 512 )
		{
			if( dwIndexY1 < dwIndexY2 )
				m_UpdateRectsImage.Add( CRect(0, dwIndexY1, m_szImage.cx, dwIndexY2+1) );
			else
			{
				m_UpdateRectsImage.Add( CRect(0, dwIndexY1, m_szImage.cx, m_szImage.cy) );
				m_UpdateRectsImage.Add( CRect(0, 0, m_szImage.cx, dwIndexY2+1) );
			}
		}
		else
			m_UpdateRectsImage.Add( rcFrame );
	}
	if( m_bNeedMon )
	{
		if( m_hFileLog != NULL )
		{
			unsigned long cbRet;
			char Buf[32];
			sprintf_s(Buf, "\\END set z value/\r\n\r\n");
			WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);
			CloseHandle(m_hFileLog);
			m_hFileLog = NULL;
		}
	}

	::GlobalUnlock((HGLOBAL) m_hdibImage);

	m_mutexImageDib.Unlock();

	return i;
}
// 11.12.05

///////////////////////////////////////////////////////////////////////////
// 像素数据转换：ZZZ模式（专用于双探测器）
// 输入:	FrameData: 数据存储区
//			dwPixX: 用来存储X值的数组
//			dwPixY: 用来存储Y值的数组
//			dwPixZ: 用来存储Z值的数组
// 输出:	实际得到的有效数据的个数
DWORD CMCICard::PixConvertZZZ2( unsigned short* pnFrameData, long lBytesTransferred,
								  WORD* pPixX, WORD* pPixY, WORD* pPixZ,
								  WORD& dwIndexY1, WORD& dwIndexY2 )
{
	// 因为每批数据的第一个数是总个数，所以nLoop要+1，且循环时从1开始
	long nLoop = lBytesTransferred/2 +1;
	long i;

	BOOL bIsBOL, bIsBOF;
	unsigned short dwData	= 0;
	DWORD dwMaskBOL	= 0x1000;	// EOL 的掩码
	DWORD dwMaskBOF	= 0x3000;	// EOF 的掩码
	DWORD dwBitsOn;
	CSize& sz = CMCICardCommonSetting::Instance().m_szImage;
	int nLineCounts	= ValleyX +sz.cx +PeakX +FallX;
	int nFrameCounts= ValleyY +sz.cy +PeakY +FallY;
	bIsBOL		= FALSE;		// 当前数据是EOL
	bIsBOF		= FALSE;		// 当前数据是EOF
	dwIndexY1	= m_nIndexY;
	int nIndex	= 0;			// 有效的数据个数

	for( i=1; i<nLoop; i++ )
	{
		dwData = pnFrameData[i];			// 得到16进制的数据
		if( dwData == 0x3456 )
			continue;

		dwBitsOn = dwData & 0x3000;			// 得到所需的掩码位

		bIsBOL = (dwBitsOn == dwMaskBOL);
		if( bIsBOL )
		{
			if( m_nY > ValleyY -1 && m_nY < ValleyY +sz.cy
				&& m_nX > ValleyX -1 && m_nX < ValleyX +sz.cx )
			{
				m_nIndexX = 0;
				m_nIndexY++;
//				CheckIndexs( sz.cx, sz.cy, m_nIndexX, m_nIndexY );
				if( m_nIndexY > sz.cy -1 )
					m_nY = 0;
			}
			m_nX = 0;
			m_nY++;
//			CheckIndexs( nLineCounts, nFrameCounts, m_nX, m_nY );
			if( m_nY > nFrameCounts -1 )
				m_nY = 0;
		}
		else
		{
			bIsBOF = (dwBitsOn == dwMaskBOF);
			if( bIsBOF )
			{
				m_nX		= 0;
				m_nY		= 0;
				m_nIndexX	= 0;
				m_nIndexY	= 0;
			}
			else
			{
				m_nX++;
//				CheckIndexs( nLineCounts, nFrameCounts, m_nX, m_nY );
				if( m_nX > nLineCounts -1 )
				{
					m_nX = 0;
					m_nY++;
				}
				if( m_nY > nFrameCounts -1 )
				{
					m_nX = 0;
					m_nY = 0;
				}
			}
		}

		if( m_nY > ValleyY && m_nY < ValleyY +sz.cy +1
			&& m_nX > ValleyX && m_nX < ValleyX +sz.cx +1 )
		{
			pPixX[nIndex] = m_nIndexX;
			pPixY[nIndex] = m_nIndexY;
//			pPixZ[nIndex] = ( dwData & 0x0FFF ) /16;
			pPixZ[nIndex] = ( dwData & 0x0FF0 ) >> 4;
			nIndex++;
			m_nIndexX++;
//			CheckIndexs( sz.cx, sz.cy, m_nIndexX, m_nIndexY );
			if( m_nIndexX > sz.cx -1 )
			{
				m_nIndexX = 0;
				m_nIndexY++;
			}
			if( m_nIndexY > sz.cy -1 )
			{
				m_nIndexX = 0;
				m_nIndexY = 0;
			}
		}
	}

	dwIndexY2 = m_nIndexY;
	return nIndex;//(sz.cx * sz.cy);
}

void CMCICard::CheckIndexs( int nLineCounts, int nFrameCounts, int& nX, int& nY )
{
	if( nX > nLineCounts -1 )
	{
		nX = 0;
		nY++;
	}
	if( nY > nFrameCounts -1 )
	{
		nX = 0;
		nY = 0;
	}
}

DWORD CMCICard::ImageDibRead( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	LPSTR		lpDIBHdr, lpDIBBits;
	LPSTR		lpBuffLine;
	LONG		lBuffLineOffset;
	LPSTR		lpImageWindowLine;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;
	DWORD		i, iMaxLine;

	////////////////////////////////////////////////////////////////////
	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;
	
	m_mutexImageDib.Lock();

	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hdibImage);
	if ( lpDIBHdr == NULL )
	{
		m_mutexImageDib.Unlock();
		return 0;
	}

	lpDIBBits = ::FindDIBBits(lpDIBHdr);
	if ( lpDIBBits == NULL )
	{
		::GlobalUnlock((HGLOBAL) m_hdibImage);
		m_mutexImageDib.Unlock();
		return 0;
	}

	if ( lBuffSize >= rcFrameDestWindow.Width() * rcFrameDestWindow.Height() )
	{
		iMaxLine = rcFrameDestWindow.Height();
	}
	else
	{
		iMaxLine = lBuffSize / rcFrameDestWindow.Width();
	}

	lBuffLineOffset	= rcFrameDestWindow.Width();
	lpBuffLine		= (LPSTR)lpBuff;

	lImageLineOffset	= m_szImage.cx;
	lpImageWindowLine	= &(lpDIBBits[ 
		  (m_szImage.cy - rcImage.top - 1) * lImageLineOffset
		+ rcImage.left ] );

	for ( i=0; i<iMaxLine; i++ )
	{
		memcpy( (void *)lpBuffLine, (const void *)lpImageWindowLine, lBuffLineOffset );
		lpBuffLine			+= lBuffLineOffset;
		lpImageWindowLine	-= lImageLineOffset;
	}
	::GlobalUnlock((HGLOBAL) m_hdibImage);

	m_mutexImageDib.Unlock();

	i = iMaxLine * lBuffLineOffset;
	return i;
}

DWORD CMCICard::ImageDibCopy( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	LPSTR		lpDIBHdr, lpDIBBits;
	LPSTR		lpBuffLine;
	LONG		lBuffLineOffset;
	LPSTR		lpImageWindowLine;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;
	DWORD		i, iMaxLine;

	////////////////////////////////////////////////////////////////////
	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;
	
	m_mutexImageDib.Lock();

	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hdibImage);
	if ( lpDIBHdr == NULL )
	{
		m_mutexImageDib.Unlock();
		return 0;
	}

	lpDIBBits = ::FindDIBBits(lpDIBHdr);
	if ( lpDIBBits == NULL )
	{
		::GlobalUnlock((HGLOBAL) m_hdibImage);
		m_mutexImageDib.Unlock();
		return 0;
	}

	if ( lBuffSize >= rcFrameDestWindow.Width() * rcFrameDestWindow.Height() )
	{
		iMaxLine = rcFrameDestWindow.Height();
	}
	else
	{
		iMaxLine = lBuffSize / rcFrameDestWindow.Width();
	}

	lBuffLineOffset	= rcFrameDestWindow.Width();
	lpBuffLine		= (LPSTR)lpBuff + lBuffLineOffset * iMaxLine;

	lImageLineOffset	= m_szImage.cx;
	lpImageWindowLine	= &(lpDIBBits[ 
		  (m_szImage.cy - rcImage.top) * lImageLineOffset
		+ rcImage.left ] );

	for ( i=0; i<iMaxLine; i++ )
	{
		lpBuffLine			-= lBuffLineOffset;
		lpImageWindowLine	-= lImageLineOffset;
		memcpy( (void *)lpBuffLine, (const void *)lpImageWindowLine, lBuffLineOffset );
	}
	::GlobalUnlock((HGLOBAL) m_hdibImage);

	m_mutexImageDib.Unlock();

	i = iMaxLine * lBuffLineOffset;
	return i;
}


void CMCICard::_InitDibHeader( LPBITMAPINFO& pDibInfo, const DWORD dwBitsSize) 
{
	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	BYTE			bGrey = 0;
	// Setup BITMAPINFOHEAD.
	pDibInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	pDibInfo->bmiHeader.biWidth			= WIDTHBYTES( m_szImage.cx * 8 ); 
	pDibInfo->bmiHeader.biHeight		= m_szImage.cy; 
	pDibInfo->bmiHeader.biPlanes		= 1;
	pDibInfo->bmiHeader.biBitCount		= 8;
	pDibInfo->bmiHeader.biCompression	= 0;
	pDibInfo->bmiHeader.biSizeImage		= dwBitsSize;
	pDibInfo->bmiHeader.biXPelsPerMeter	= 0;
	pDibInfo->bmiHeader.biYPelsPerMeter	= 0;
	pDibInfo->bmiHeader.biClrUsed		= 0;
	pDibInfo->bmiHeader.biClrImportant	= 0;

	// Image-lay pallete
	for (int i=0; i<256; i++)
	{ 
		bGrey = (BYTE)i;
		pDibInfo->bmiColors[i].rgbRed		= bGrey;
		pDibInfo->bmiColors[i].rgbGreen		= bGrey;
		pDibInfo->bmiColors[i].rgbBlue		= bGrey;
		pDibInfo->bmiColors[i].rgbReserved	= (BYTE)0;
	}
}


HDIB CMCICard::CreateImageBitmap()
{
	DWORD			dwBitsSize = 0;
	DWORD			dwOffBits = 0;
	HDIB			hDIB = NULL;
	LPSTR			pDIB = NULL;
	LPBITMAPINFO 	pDibInfo = NULL;
	LPSTR	 		pDibBits = NULL;

	// Calculate the size of memory for hDIB.
	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	dwOffBits = sizeof(BITMAPINFO);											// BITMAPINFO
	dwOffBits += sizeof(RGBQUAD) * 255;										// Palette ( RGBQUAD area )
	dwBitsSize = WIDTHBYTES( m_szImage.cx * 8L ) * (DWORD)m_szImage.cy;		// Image bits

	// Allocate memory for DIB.
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize + dwOffBits);
	if (hDIB == 0)
	{
		return NULL;
	}
	DWORD dws	= GlobalSize( hDIB );
	pDIB		= (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	pDibInfo	= (BITMAPINFO *)pDIB;	
	pDibBits	= &( pDIB[dwOffBits] );
	_InitDibHeader(pDibInfo, dwBitsSize);

	::GlobalUnlock((HGLOBAL) hDIB);
	m_lpImageDibPlan = NULL;

	if ( hDIB != NULL ) 
		CreateDIBPalette(hDIB, &m_paletteImage);

	return hDIB;
}

long CMCICard::UpdateImageLay()
{
	CRect			rcTemp;
    HRESULT			ddrval = DD_OK;
    HDC             hdc;

	m_mutexImageDib.Lock();
	//m_UpdateRectsImage.Iterate("m_UpdateRectsImage");
	//m_UpdateRectsGraph.Iterate("m_UpdateRectsGraph");

	CRectManager& m_UpdateRectsImage = CMCICardCommonSetting::Instance().m_UpdateRectsImage;
	CRectManager& m_UpdateRectsGraph = CMCICardCommonSetting::Instance().m_UpdateRectsGraph;

	while ( !m_UpdateRectsImage.IsEmpty() )
	{
		rcTemp = m_UpdateRectsImage.Get();

		if ( !rcTemp.IsRectEmpty() )
		{
			m_UpdateRectsGraph.Add( rcTemp );

			if ( !IsReady() ) 
			{
				m_mutexImageDib.Unlock();
				return DD_OK;
			}

			if ((ddrval = m_lpDDSImageLay->GetDC(&hdc)) == DD_OK)
			{
				PaintDIB (hdc, (LPRECT)rcTemp, (HDIB)m_hdibImage, (LPRECT)rcTemp, &m_paletteImage);
//				DrawDIB (hdc, (LPRECT)rcTemp, (HDIB)m_hdibImage, (LPRECT)rcTemp);
				
				m_lpDDSImageLay->ReleaseDC(hdc);
			}
		}
	}

	m_mutexImageDib.Unlock();

	return ddrval;
}

long CMCICard::UpdateGraphLay(CLabelControler* pLabelControler, CHeaderControler* pHeaderControler)
{
	CRect			rcTemp;
    HRESULT			ddrval = DD_OK;

	while ( !CMCICardCommonSetting::Instance().m_UpdateRectsGraph.IsEmpty() )
	{
		rcTemp = CMCICardCommonSetting::Instance().m_UpdateRectsGraph.Get();

		if ( !rcTemp.IsRectEmpty() )
		{
			pLabelControler->m_UpdateRectsLabel.Add( rcTemp );
			pHeaderControler->m_UpdateRectsHeader.Add( rcTemp );

			CRect rcDestImageLay	= rcTemp;
			CRect rcSrcGraphLay		= LayWindowMapClip_Image_to_Graph( rcDestImageLay );

			if ( !IsReady() ) return DD_OK;

			ddrval = m_lpDDSGraphBackLay->Blt(
				(LPRECT)rcSrcGraphLay,			// dest rect
				m_lpDDSGraphLay,				// src surface
				(LPRECT)rcSrcGraphLay,			// src rect
				DDBLT_WAIT,		// DDBLT_KEYSRC | 
				NULL);

		}
	}

	return ddrval;
}



void CMCICard::PhotoIndication( const CRect& rcDest, const COLORREF color, const BOOL boolFill )//09.06.01
{
	HDC				hdc;
    HRESULT			ddrval;

	if ( !IsReady() ) return;

	m_mutexLayAll.Lock();

	if( m_lpDDSGraphLay == NULL )
	{
		m_mutexLayAll.Unlock();
		return;
	}

	ddrval = m_lpDDSGraphLay->GetDC( &hdc );
	if( ddrval == DD_OK )
	{
		CRect	rcFill =
			rcDest & CRect(0,0,CMCICardCommonSetting::Instance().m_szGraph.cx,CMCICardCommonSetting::Instance().m_szGraph.cy);
		if( boolFill )	//填充
		{
			HBRUSH brBrush = CreateSolidBrush( color );

			FillRect( hdc, (LPRECT)rcFill, brBrush );
			DeleteObject( brBrush );
		}
		else			//仅画线
		{
			HPEN hpen = CreatePen(PS_SOLID, 1, color );
			HPEN hpenOld = (HPEN)SelectObject(hdc, hpen);

			MoveToEx (hdc, rcFill.left, rcFill.bottom -1, NULL);
			LineTo (hdc, rcFill.right, rcFill.bottom -1);

			SelectObject(hdc, hpenOld);
			DeleteObject(hpen);
		}
		m_lpDDSGraphLay->ReleaseDC(hdc);
	}

	CRect rcGrapic = rcDest;
	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );
	if ( color != CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrTransparent] )
		CMCICardCommonSetting::Instance().m_UpdateRectsGraph.Add( rcImage );
	else
		CMCICardCommonSetting::Instance().m_UpdateRectsImage.Add( rcImage );

	m_mutexLayAll.Unlock();
}

long CMCICard::UpdateGraphBackLay()
{
	CRect			rcTemp;
    HRESULT			ddrval = DD_OK;

	while ( !CMCICardCommonSetting::Instance().m_UpdateRectsGraphBack.IsEmpty() )
	{
		rcTemp = CMCICardCommonSetting::Instance().m_UpdateRectsGraphBack.Get();

		if ( !rcTemp.IsRectEmpty() )
		{
			CRect rcDestImageLay	= rcTemp;
			CRect rcSrcGraphLay		= LayWindowMapClip_Image_to_Graph( rcDestImageLay );

			if ( !IsReady() ) return DD_OK;

			ddrval = m_lpDDSImageLay->Blt(
				(LPRECT)rcDestImageLay,			// dest rect
				m_lpDDSGraphBackLay,			// src surface
				(LPRECT)rcSrcGraphLay,			// src rect
				DDBLT_KEYSRC | DDBLT_WAIT,		//
				NULL);
		}
	}

	return ddrval;
}

long CMCICard::UpdatePriamryLay()
{
    HRESULT			ddrval = DD_OK;

	CRect rcSrcImageLay	= CMCICardCommonSetting::Instance().m_rcImageWindow;
	CRect rcDestPriamryLay	= LayWindowMapClip_Image_to_Priamry( rcSrcImageLay );

	//======2007.05.14_扫描区域居中========================
	// 修改标示：2007.05.14_扫描区域居中
	// 修改日期：2007.05.14星期一
	// 修改人  ：J.Y.Ma
	// 修改原因：无论放大倍数多少，使图像扫描区域居中

	// 本次修改前代码副本
	/*----------------------------------------------------- 
	//---------------------------------------------------*/

	// 本次修改后代码
	//-----------------------------------------------------
	///////////////////////////////////////////////////////
/*	CPoint pt(0,0);
	// 非照相预览模式时，如果画图区大于图像，
	// 则计算将图像显示在中央所需偏移量，并进行偏移
	if( m_dblZoom != 0
		&& m_rcScreenClientRect.Width() >= rcSrcImageLay.Width()*m_dblZoom 
		&& m_rcScreenClientRect.Height() >= rcSrcImageLay.Height()*m_dblZoom )
		pt = CPoint( -( m_rcScreenClientRect.Width() - rcSrcImageLay.Width()*m_dblZoom ) / 2,
					-( m_rcScreenClientRect.Height() - rcSrcImageLay.Height()*m_dblZoom ) / 2 );
	rcDestPriamryLay.OffsetRect( -pt );
*/	///////////////////////////////////////////////////////


	if ( !IsReady() ) return DD_OK;

	ddrval = m_lpDDSPrimaryBackLay->Blt(
		(LPRECT)rcDestPriamryLay,		// dest rect
		m_lpDDSImageLay,				// src surface
		(LPRECT)rcSrcImageLay,			// src rect
		DDBLT_WAIT,
		NULL);
	if ( ddrval != DD_OK )
		return ddrval;

	if ( !IsReady() ) return DD_OK;

	ddrval = m_lpDDSPrimary->Blt(
		(LPRECT)rcDestPriamryLay,		// dest rect
		m_lpDDSPrimaryBackLay,			// src surface
		(LPRECT)rcDestPriamryLay,		// src rect
		DDBLT_WAIT,
		NULL);
	
	return ddrval;
}


void CMCICard::SetScanScreenMode( BOOL boolNewScanScreenMode )
{
	if( m_boolNewScanScreenMode == boolNewScanScreenMode )
		return;

	m_mutexLayAll.Lock();
	m_boolNewScanScreenMode = boolNewScanScreenMode;
	m_mutexLayAll.Unlock();
}


void CMCICard::SetColor( int nType, COLORREF color )
{
	CMCICardOption::Instance().m_sysOpt.color[nType]	= color;
	switch( nType )
	{
	case	SysOpt_clrBack:
		m_pLabelControler->RedrawLabelLay(m_bSplit,m_bDet2nd);
		m_pHeaderControler->RedrawHeaderLay(m_bSplit);
		break;
	case	SysOpt_clrLabelText:
	case	SysOpt_clrLabelLogoText:
		m_pLabelControler->RedrawLabelLay(m_bSplit,m_bDet2nd);
		m_pHeaderControler->RedrawHeaderLay(m_bSplit);
		break;
	case	SysOpt_clrMobileUScaleText:
	case	SysOpt_clrMobileUScaleLine:
		m_pMobileUScaleControler->RedrawMobileUScaleLay(m_pLabelControler,m_rcScreenClientRect);
		break;
	case	SysOpt_clrOscillographGrid:
	case	SysOpt_clrOscillographGraph:
		m_pOscillographControler->RedrawOscillographLay(m_bDebugZ);
		break;
	case	SysOpt_clrAngleLine:
	case	SysOpt_clrAngleArc:
	case	SysOpt_clrAngleText:
		m_pAngleControler->RedrawAngleLay(m_pLabelControler,m_rcScreenClientRect);
		break;
	}
}

void CMCICard::SetFontName( int nType, CString strName )
{
	CMCICardOption::Instance().m_sysOpt.fName[nType]	= strName;
}

void CMCICard::SetFontSize( int nType, int nSize )
{
	CMCICardOption::Instance().m_sysOpt.fSize[nType]	= nSize;
}

void CMCICard::SetFontWeight( int nType, int nWeight )
{

}

void CMCICard::SetTransparent( int nTransparent )
{
	CMCICardOption::Instance().m_sysOpt.nShowParams[SysOpt_showLabelTrans]	= nTransparent;
}

/*
const int SET_PARAMS_StoreMode			= 1;
const int SET_PARAMS_DetectorMode		= 2;
const int SET_PARAMS_NeedMon			= 3;
*/

void CMCICard::SetParams( int nType, int nParam )
{
	switch( nType )
	{
	case	SET_PARAMS_StoreMode:
		m_nDataStoreMode = nParam;
		break;
	case	SET_PARAMS_DetectorMode:
		m_nDetectorMode = nParam;
		break;
	case	SET_PARAMS_NeedMon:
		{
			m_bNeedMon = (BOOL)nParam;
			m_pOscillographControler->SetNeedLog(m_bNeedMon);
		}
		break;
	case	4:
		{
			m_bDet2nd = (BOOL)nParam;
			m_pLabelControler->RedrawLabelLay(m_bSplit,m_bDet2nd);
		}
		break;
	case	5:
		m_bDebugZ = (BOOL)nParam;
		break;
	case	6:
		{
			// 视频示波器分析及显示意义不同
			m_pOscillographControler->m_bDebugOsc		= (BOOL)nParam;
			m_pOscillographControler->CalculateOscillographSize(CMCICardCommonSetting::Instance().m_sizeReso);
		}
		break;
	case	7:
		{
			// 示波器在调试模式下，Y向档位调节
			m_pOscillographControler->m_debugOSC.nIndexY		= nParam;
		}
		break;
	case	8:
		{
			m_pOscillographControler->m_debugOSC.b90 = (BOOL)nParam;
			ZeroMemory( &m_pOscillographControler->m_bOscillographBuff, sizeof(WORD)*8192);
//			m_boolRedrawOscillograph = TRUE;
		}
		break;
	case	9:
		{
			// 示波器在调试模式下，Y向偏移量
			m_pOscillographControler->m_debugOSC.dOffset			= nParam / 10000.0;
//			m_boolRedrawOscillograph	= TRUE;
		}
		break;
	case	10:
		{
			CMCICardOption::Instance().m_sysOpt.nLineWidth = nParam;
			m_pMobileUScaleControler->RedrawMobileUScaleLay(m_pLabelControler,m_rcScreenClientRect);
		}
		break;
	case	11:
		{
			m_nChnIndex[0] = nParam;
			UpdateSGname( m_nChnIndex[0], m_pLabelControler->m_csSG1 );
			m_pLabelControler->RedrawLabelLay(m_bSplit,m_bDet2nd);
		}
		break;
	case	12:
		{
			m_nChnIndex[1] = nParam;
			UpdateSGname( m_nChnIndex[1], m_pLabelControler->m_csSG2 );
			m_pLabelControler->RedrawLabelLay(m_bSplit,m_bDet2nd);
		}
		break;
	case	13:
		m_dblPixelsPerMM_HORZ = 1.0 * GetSystemMetrics(SM_CXSCREEN) / nParam;
		break;
	case	14:
		m_dblPixelsPerMM_VERT = 1.0 * GetSystemMetrics(SM_CYSCREEN) / nParam;
		break;
	case	15:
		m_nMaxPix = nParam;
		break;
	case	16:
		m_nDiv = nParam;
		break;
	case	17:
		m_dSEratio = nParam /100.0;
		break;
	}
}

void CMCICard::UpdateSGname( int nIndex, CString& strName )
{
	if( nIndex != 2 )	//< 6 )
		strName = m_pLabelControler->m_csDet[nIndex];
	else
		strName = m_pLabelControler->m_csDet[0] + "+" + m_pLabelControler->m_csDet[1];
/*	switch( nIndex )
	{
	case	0:
		strName	= _T("SE");
		break;
	case	1:
		strName	= _T("BSE");
		break;
	case	2:
		strName	= _T("SE+BSE");
		break;
	}*/
}

void CMCICard::UpdateChnNames( int nIndex, CString csText )
{
	m_pLabelControler->m_csDet[nIndex] = csText;
}

void CMCICard::UpdateFOV( CString csFOV )
{
	m_pHeaderControler->UpdateFOV(csFOV);
}

void CMCICard::SetSharedImageBuffIndex( int nIndex )
{
	m_nSharedImageBuffIndexWrite = nIndex;
}

// 09.11.24加入同步语句
// 改变放大倍数，由任意变为Fit时有时会出现图像被拉伸；或由大变小时，小区域到原来区域间有未被重画的区域
// 在调试过程中发现：GetClientWindow中会对m_rcScreenClient根据放大倍数做调整，
//					而LayWindowMapClip_Image_to_Priamry中如果Fit模式，会用到m_rcScreenClient，
//					这有可能造成使用的同步问题，故加入同步语句
void CMCICard::SetZoom( double dblZoom )
{
	m_mutexLayAll.Lock();
	m_dblZoom = dblZoom;
	m_mutexLayAll.Unlock();
}

void CMCICard::SetDestFrame(CRect rcDestFrame)
{
	m_mutexLayAll.Lock();
	m_rcDestFrame = rcDestFrame;
	m_mutexLayAll.Unlock();
}

CSize CMCICard::GetSizes( int nIndex )
{
	CSize sz(0,0);
	switch( nIndex )
	{
	case	0:
		sz.cx	= CMCICardCommonSetting::Instance().m_rcImageWindow.Width();
		sz.cy	= CMCICardCommonSetting::Instance().m_rcImageWindow.Height();
		break;
	case	1:
		sz		= CMCICardCommonSetting::Instance().m_szImage;
		break;
	case	2:
		sz		= m_pLabelControler->m_szLabel;
		break;
	case	3:
		sz		= m_pMobileUScaleControler->m_szUScale;
		break;
	case	4:
		sz		= m_pOscillographControler->m_szOscill;
		break;
	}
	return sz;
}

void CMCICard::GetClientWindow()
{
	CalculateClientWindowRect();
	if (GetPhotoPreview())
	{
	}
	else
	{
		::GetClientRect(m_pScreenWnd->m_hWnd,m_rcScreenClientRect);
	}

	m_ptScreenClientPos.x = ::GetScrollPos(m_pScreenWnd->m_hWnd,SB_HORZ );
	m_ptScreenClientPos.y = ::GetScrollPos(m_pScreenWnd->m_hWnd,SB_VERT );

	POINT ptLeftTop;
	ptLeftTop.x = m_rcScreenClientRect.left;
	ptLeftTop.y = m_rcScreenClientRect.top;
	::ClientToScreen(m_pScreenWnd->m_hWnd,&ptLeftTop);
	m_rcScreenClientRect.left = ptLeftTop.x;
	m_rcScreenClientRect.top = ptLeftTop.y;

	POINT ptBottomRight;
	ptBottomRight.x = m_rcScreenClientRect.right;
	ptBottomRight.y = m_rcScreenClientRect.bottom;
	::ClientToScreen(m_pScreenWnd->m_hWnd,&ptBottomRight);
	m_rcScreenClientRect.right = ptBottomRight.x;
	m_rcScreenClientRect.bottom = ptBottomRight.y;
}

void CMCICard::CalculateClientWindowRect() 
{
	///////////////////////////////////////////////////////////////////
	CSize& m_sizeReso = CMCICardCommonSetting::Instance().m_sizeReso; 
	if( m_boolPreview )	
	{					
		CRect rcRect(0,0,0,0);

		double dRatio	= 1.0 *m_sizeReso.cy /m_sizeReso.cx;
		if( dRatio == 0.5 )
		{
			rcRect.right	= (int)(127 * m_dblPixelsPerMM_HORZ + 0.5 );
			rcRect.bottom	= (int)(63 * m_dblPixelsPerMM_VERT + 0.5 );
		}
		else if( dRatio == 1.0 )
		{
			rcRect.right	= (int)(127 * m_dblPixelsPerMM_HORZ + 0.5 );
			rcRect.bottom	= (int)(127 * m_dblPixelsPerMM_VERT + 0.5 );
		}
		else
		{
			rcRect.right	= (int)(127 * m_dblPixelsPerMM_HORZ + 0.5 );
			rcRect.bottom	= (int)(89 * m_dblPixelsPerMM_VERT + 0.5 );
		}

		if( !m_boolNewScanScreenMode )	//06.11.22
		{
			rcRect.right += GetSystemMetrics( SM_CXBORDER ) * 2;
			rcRect.bottom += GetSystemMetrics( SM_CYBORDER ) * 2;
			rcRect.bottom += GetSystemMetrics( SM_CYCAPTION );
		}

		m_rcScreenClientRect = rcRect;
	}
	else
	{
		if( m_dblZoom == 0. && m_boolNewScanScreenMode )	//06.11.22
		{

    	}
	}
}

void CMCICard::PaintScreenWindow()
{
	static CPoint ptScreenPos_Old;
	
	CPoint ptScreenPos;

	ptScreenPos.x = m_rcScreenClientRect.left;
	ptScreenPos.y = m_rcScreenClientRect.top;

	if ( ptScreenPos_Old != ptScreenPos )
	{
		if ( ( ptScreenPos.x < ptScreenPos_Old.x )
			|| ( ptScreenPos.y < ptScreenPos_Old.y ) )
		{
			m_pScreenWnd->InvalidateRect(NULL,FALSE);
		}

		ptScreenPos_Old = ptScreenPos;
	}
}

void CMCICard::MainLoop()
{
	DWORD	thisTickCount;
    
	if ( !m_boolReady )
    {
		Sleep( 1 );
		return;
    }

	if ( m_boolShouldReset )
    {
		m_lpDDSPrimary->Restore();
		m_boolShouldReset = FALSE;
		Sleep( 1 );
		return;
    }

	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	if( m_nSharedImageBuffIndexRead != m_nSharedImageBuffIndexWrite )
	{
		do
		{
			GetData( CRect(0,0,m_szImage.cx,m_szImage.cy) );
			m_nSharedImageBuffIndexRead++;
			if( m_nSharedImageBuffIndexRead >= SharedImageBuffCounts )
				m_nSharedImageBuffIndexRead = 0;
		}while( m_nSharedImageBuffIndexRead != m_nSharedImageBuffIndexWrite );
	}
	else
	{
		// 12.02.15 使用m_nDetectorMode临时作为判断是采集/冻结状态
		// -1表示冻结
		// 采集时如果没有数据更新就返回
		// 冻结时要继续执行之后的语句
/*		if( m_nDetectorMode > -1 )
		{
			Sleep( 1 );
			return;
		}*/

		thisTickCount = GetTickCount();
		if ((thisTickCount - m_lastTickCount) < g_dwUSB_UpdateDelay)
		{
			Sleep( 1 );
			return;
		}

		 m_lastTickCount = thisTickCount;
	}

	m_mutexLayAll.Lock();	//09.12.03 (v 1.0.0.6 +)

	GetClientWindow();

	CSize& m_szGraph = CMCICardCommonSetting::Instance().m_szGraph;
	CRect& m_rcImageWindow = CMCICardCommonSetting::Instance().m_rcImageWindow;
	CMCICardCommonSetting::Instance().m_dblScaleImageToGraph_W = (double)m_szGraph.cx / m_rcImageWindow.Width();
	CMCICardCommonSetting::Instance().m_dblScaleImageToGraph_H = (double)m_szGraph.cy / m_rcImageWindow.Height();

	if (m_pLabelControler->m_boolRedrawLabel)
	{
		m_pLabelControler->RedrawLabelLay(m_bSplit,m_bDet2nd);
		m_pLabelControler->m_boolRedrawLabel = FALSE;
	}

	if (m_pHeaderControler->m_boolRedrawHeader)
	{
		m_pHeaderControler->RedrawHeaderLay(m_bSplit);
		m_pHeaderControler->m_boolRedrawHeader = FALSE;
	}

	if (m_pMobileUScaleControler->m_boolRedrawUScal)
	{
		m_pMobileUScaleControler->RedrawMobileUScaleLay(m_pLabelControler,m_rcScreenClientRect);
		m_pMobileUScaleControler->m_boolRedrawUScal = FALSE;
	}

	if (m_pAngleControler->m_boolRedrawAngle)
	{
		m_pAngleControler->RedrawAngleLay(m_pLabelControler,m_rcScreenClientRect);
		m_pAngleControler->m_boolRedrawAngle = FALSE;
	}

	if (m_pOscillographControler->m_boolRedrawOscillograph)
	{
		if (m_pOscillographControler->m_boolShowOscillograph)
			m_pOscillographControler->RedrawOscillographLay(m_bDebugZ);

		m_pOscillographControler->m_boolRedrawOscillograph = FALSE;
	}

	UpdateImageLay();

	UpdateGraphLay(m_pLabelControler, m_pHeaderControler);

	m_pLabelControler->UpdateLabelLay(m_pMobileUScaleControler,m_pAngleControler,m_boolReady);

	m_pHeaderControler->UpdateHeaderLay(m_pMobileUScaleControler,m_boolReady);

	m_pMobileUScaleControler->UpdateUScaleMobileLay(m_pLabelControler,m_pOscillographControler,m_boolReady);

	m_pAngleControler->UpdateAngleLay(m_pLabelControler,m_pOscillographControler,m_boolReady);

	m_pOscillographControler->UpdateOscillographLay(m_boolReady);

	UpdateGraphBackLay();

//	m_pFrameRateControler->FormatFrameRate(m_dwBytes,m_dwByteCount,m_dwInputCount,m_dwBytesLast);
//	m_pFrameRateControler->UpdateFrameRateLay(m_lpDDSImageLay,m_boolReady);

	long ddrval = UpdatePriamryLay();

	if (ddrval == DDERR_SURFACELOST ||ddrval == DDERR_GENERIC )
	{
		m_boolShouldReset	= TRUE;
	}

	m_mutexLayAll.Unlock();

	PaintScreenWindow();
}


CSize CMCICard::GetResolution()
{ 
	return CMCICardCommonSetting::Instance().m_sizeReso; 
};

void CMCICard::SetResolution(CSize szReso)
{
	m_mutexImageDib.Lock();
	m_mutexImageUSB.Lock();

	int nXWidth = m_bSplit?szReso.cx *2:szReso.cx;
	SetImageWindow( CRect(0,0,nXWidth,szReso.cy) );
	_SetSize(szReso, nXWidth);

	// 重新计算一下示波器大小
	m_pOscillographControler->CalculateOscillographSize(szReso);

	CMCICardCommonSetting::Instance().m_sizeReso = szReso;

	m_mutexImageUSB.Unlock();
	m_mutexImageDib.Unlock();
}

void CMCICard::SetFunction( USB_ScanType nFunction )
{
	m_nusbFunction = nFunction;
}

void CMCICard::SetSplit( BOOL bSplit )
{
	m_bSplit = bSplit;
}

void CMCICard::SetLiveMode( BOOL bLive )
{
	m_bLive = bLive;
}

void CMCICard::SetImageWindow( CRect rcImageWindow )
{
	CMCICardCommonSetting::Instance().m_rcImageWindow = rcImageWindow;
}


CRect CMCICard::LayWindowMapClip_Image_to_Priamry( CRect& rcImage )
{
	double	dblScaleImageToPriamry_W;
	double	dblScaleImageToPriamry_H;

	CRect& m_rcImageWindow = CMCICardCommonSetting::Instance().m_rcImageWindow;
	if ( m_dblZoom <= 0 )
	{
		double dWidth = m_rcScreenClientRect.Width();
		double dHeight = m_rcScreenClientRect.Height();
		dblScaleImageToPriamry_W = dWidth / m_rcImageWindow.Width();
		dblScaleImageToPriamry_H = dHeight / m_rcImageWindow.Height();
		m_ptScreenClientPos = CPoint(0, 0);	
	}
	else
	{
		dblScaleImageToPriamry_W = m_dblZoom;
		dblScaleImageToPriamry_H = m_dblZoom;
	}


	CRect	rcPriamry;
	CRect	rcPriamryWindow;
	CPoint	ptOrigin(m_rcImageWindow.left, m_rcImageWindow.top);

	rcPriamryWindow.left = 0;
	rcPriamryWindow.top = 0;
	rcPriamryWindow.right = (int)(m_rcImageWindow.Width() * dblScaleImageToPriamry_W + 0.5);
	rcPriamryWindow.bottom = (int)(m_rcImageWindow.Height() * dblScaleImageToPriamry_H + 0.5);

	// Image to Priamry (Logic)
	rcImage.OffsetRect( - ptOrigin );

	rcPriamry.left = (int)(rcImage.left * dblScaleImageToPriamry_W + 0.5);
	rcPriamry.right = (int)(rcImage.right * dblScaleImageToPriamry_W + 0.5);
	rcPriamry.top = (int)(rcImage.top * dblScaleImageToPriamry_H + 0.5);
	rcPriamry.bottom = (int)(rcImage.bottom * dblScaleImageToPriamry_H + 0.5);
	rcPriamry &= rcPriamryWindow;

	// Priamry to Client (Logic)
	CRect	rcClient = rcPriamry;
	rcClient.OffsetRect( - m_ptScreenClientPos );

	// Client to ScreenClient
	CRect	rcScreenClient;
	CPoint	ptOriginScreenClient(m_rcScreenClientRect.left, m_rcScreenClientRect.top);

	rcScreenClient = rcClient;
	rcScreenClient.OffsetRect( ptOriginScreenClient );
	rcScreenClient &= m_rcScreenClientRect;

	CRect rcSystemScreen(0,0,
		GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	rcScreenClient &= rcSystemScreen;

	// ScreenClient to Client
	rcClient = rcScreenClient;
	rcClient.OffsetRect( - ptOriginScreenClient );

	// Client to Priamry
	rcPriamry = rcClient;
	rcPriamry.OffsetRect( m_ptScreenClientPos );

	// Priamry to Image
	rcImage.left = (int)(rcPriamry.left / dblScaleImageToPriamry_W + 0.5);
	rcImage.right = (int)(rcPriamry.right / dblScaleImageToPriamry_W + 0.5);
	rcImage.top = (int)(rcPriamry.top / dblScaleImageToPriamry_H + 0.5);
	rcImage.bottom = (int)(rcPriamry.bottom / dblScaleImageToPriamry_H + 0.5);

	rcImage.OffsetRect( ptOrigin );

	return rcScreenClient;
}

BOOL CMCICard::CopyScreenBmp( CBitmap& bmpScreen )
{
	HDC				hdc;
    HRESULT			ddrval;
	CRect& m_rcImageWindow = CMCICardCommonSetting::Instance().m_rcImageWindow;

	ddrval = m_lpDDSImageLay->GetDC( &hdc );
	if( ddrval == DD_OK )
	{
		CDC	dcScreen;
		
		if ( dcScreen.Attach(hdc) )
		{
			CaptureScreenRect( bmpScreen, &dcScreen, m_rcImageWindow );

			dcScreen.Detach();
			m_lpDDSImageLay->ReleaseDC(hdc);
		}
		else 
		{
			m_lpDDSImageLay->ReleaseDC(hdc);
			return FALSE;
		}
	}
	else
	{
		switch( ddrval )
		{
		case	DDERR_DCALREADYCREATED:
			DebugMessage("DDERR_DCALREADYCREATED");	
			break;
		case	DDERR_GENERIC:
			DebugMessage("DDERR_GENERIC");	
			break;
		case	DDERR_INVALIDOBJECT:
			DebugMessage("DDERR_INVALIDOBJECT");	
			break;
		case	DDERR_INVALIDPARAMS:
			DebugMessage("DDERR_INVALIDPARAMS");	
			break;
		case	DDERR_INVALIDSURFACETYPE:
			DebugMessage("DDERR_INVALIDSURFACETYPE");	
			break;
		case	DDERR_SURFACELOST:
			DebugMessage("DDERR_SURFACELOST");	
			break;
		case	DDERR_UNSUPPORTED:
			DebugMessage("DDERR_UNSUPPORTED");	
			break;
		case	DDERR_WASSTILLDRAWING:
			DebugMessage("DDERR_WASSTILLDRAWING");	
			break;
		}
	}

	return (ddrval == DD_OK);
}

BOOL CMCICard::CopyGraphBmp( CBitmap& bmpGraphLay )
{
	HDC				hdc;
    HRESULT			ddrval;

	ddrval = m_lpDDSGraphBackLay->GetDC( &hdc );
	if( ddrval == DD_OK )
	{
		CDC	dcGraph;
		
		if ( dcGraph.Attach(hdc) )
		{
			CaptureScreenRect( bmpGraphLay, &dcGraph,
				CRect(0,0,CMCICardCommonSetting::Instance().m_szGraph.cx,CMCICardCommonSetting::Instance().m_szGraph.cy) );

			dcGraph.Detach();
			m_lpDDSGraphBackLay->ReleaseDC(hdc);
		}
		else 
		{
			m_lpDDSGraphBackLay->ReleaseDC(hdc);
			return FALSE;
		}
	}

	return (ddrval == DD_OK);
}

HDIB CMCICard::CopyImageDib()
{
	CRect rcImageWindow = CMCICardCommonSetting::Instance().m_rcImageWindow;	//GetImageWindow();
	const CSize szImageSize(rcImageWindow.Width(), rcImageWindow.Height());

	DWORD			dwBitsSize;
	DWORD			dwOffBits;
	HDIB			hDIB;
	LPSTR			pDIB;
	LPBITMAPINFO 	pDibInfo;
	LPSTR	 		pDibBits;
	BYTE			bGrey;
	int				i;

	// Calculate the size of memory for hDIB.
	dwOffBits = sizeof(BITMAPINFO);											// BITMAPINFO
	dwOffBits += sizeof(RGBQUAD) * 255;										// Palette ( RGBQUAD area )
	dwBitsSize = WIDTHBYTES( szImageSize.cx * 8L ) * (DWORD)szImageSize.cy;	// Image bits

	// Allocate memory for DIB.
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize + dwOffBits);
	if (hDIB == 0)
	{
		return NULL;
	}
	pDIB		= (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	pDibInfo	= (BITMAPINFO *)pDIB;	
	pDibBits	= &( pDIB[dwOffBits] );

	// Setup BITMAPINFOHEAD.
	pDibInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	pDibInfo->bmiHeader.biWidth			= WIDTHBYTES( szImageSize.cx * 8 ); 
	pDibInfo->bmiHeader.biHeight		= szImageSize.cy; 
	pDibInfo->bmiHeader.biPlanes		= 1;
	pDibInfo->bmiHeader.biBitCount		= 8;
	pDibInfo->bmiHeader.biCompression	= 0;
	pDibInfo->bmiHeader.biSizeImage		= dwBitsSize;
	pDibInfo->bmiHeader.biXPelsPerMeter	= 0;
	pDibInfo->bmiHeader.biYPelsPerMeter	= 0;
	pDibInfo->bmiHeader.biClrUsed		= 0;
	pDibInfo->bmiHeader.biClrImportant	= 0;

	// Image-lay pallete
	for (i=0; i<256; i++)
	{ 
		bGrey = (BYTE)i;
		pDibInfo->bmiColors[i].rgbRed		= bGrey;
		pDibInfo->bmiColors[i].rgbGreen		= bGrey;
		pDibInfo->bmiColors[i].rgbBlue		= bGrey;
		pDibInfo->bmiColors[i].rgbReserved	= (BYTE)0;
	}

	// Fill the bits.
	ImageDibCopy( pDibBits, dwBitsSize, rcImageWindow );
	
	::GlobalUnlock((HGLOBAL) hDIB);

	return hDIB;
}



void CMCICard::_SetSize(const CSize &szReso,int nXWidth) 
{
	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	CSize& m_szGraph = CMCICardCommonSetting::Instance().m_szGraph;
	if( szReso.cx < 16+1 )
	{
		m_szImage		= CSize( nXWidth, szReso.cy );
		m_szGraph		= CSize( nXWidth, szReso.cy );
		m_pLabelControler->m_szLabel		= CSize( nXWidth, 2 );
		m_pMobileUScaleControler->m_szUScale		= CSize( nXWidth, 1 );
		m_pOscillographControler->m_szOscill		= CSize( nXWidth, 8 );
	}
	else if( szReso.cx < 32+1 )
	{
		m_szImage		= CSize( nXWidth, szReso.cy );
		m_szGraph		= CSize( nXWidth, szReso.cy );
		m_pLabelControler->m_szLabel		= CSize( nXWidth, 4 );
		m_pMobileUScaleControler->m_szUScale		= CSize( nXWidth, 2 );
		m_pOscillographControler->m_szOscill		= CSize( nXWidth, 16 );
	}
	else if( szReso.cx < 64+1 )
	{
		m_szImage		= CSize( nXWidth, szReso.cy );
		m_szGraph		= CSize( nXWidth, szReso.cy );
		m_pLabelControler->m_szLabel		= CSize( nXWidth, 8 );
		m_pMobileUScaleControler->m_szUScale		= CSize( nXWidth, 4 );
		m_pOscillographControler->m_szOscill		= CSize( nXWidth, 32 );
	}
	else if( szReso.cx < 128+1 )
	{
		m_szImage		= CSize( nXWidth, szReso.cy );
		m_szGraph		= CSize( nXWidth, szReso.cy );
		m_pLabelControler->m_szLabel		= CSize( nXWidth, 16 );
		m_pMobileUScaleControler->m_szUScale		= CSize( nXWidth, 8 );
		m_pOscillographControler->m_szOscill		= CSize( nXWidth, 64 );
	}
	else if( szReso.cx < 256+1 )
	{
		m_szImage		= CSize( nXWidth, szReso.cy );
		m_szGraph		= CSize( nXWidth, szReso.cy );
		m_pLabelControler->m_szLabel		= CSize( nXWidth, 24 );
		m_pMobileUScaleControler->m_szUScale		= CSize( nXWidth, 16 );
		m_pOscillographControler->m_szOscill		= CSize( nXWidth, 128 );
	}
	else if( szReso.cx < 480+1 )
	{
		m_szImage		= CSize( nXWidth, szReso.cy );
		m_szGraph		= CSize( nXWidth, szReso.cy );
		m_pLabelControler->m_szLabel		= CSize( nXWidth, 32 );
		m_pMobileUScaleControler->m_szUScale		= CSize( nXWidth, 16 );
		m_pOscillographControler->m_szOscill		= CSize( nXWidth, 128 );
	}
	else if( szReso.cx < 600+1 )
	{
		m_szImage		= CSize( nXWidth, szReso.cy );
		m_szGraph		= CSize( nXWidth, szReso.cy );
		m_pLabelControler->m_szLabel		= CSize( nXWidth, 40 );
		m_pMobileUScaleControler->m_szUScale		= CSize( nXWidth, 40 );
		m_pOscillographControler->m_szOscill		= CSize( nXWidth, 128 );
	}
	else if( szReso.cx < 640+1 )
	{
		m_szImage		= CSize( nXWidth, szReso.cy );
		m_szGraph		= CSize( nXWidth, szReso.cy );
		m_pLabelControler->m_szLabel		= CSize( nXWidth, 40 );
		m_pMobileUScaleControler->m_szUScale		= CSize( nXWidth, 40 );
		m_pOscillographControler->m_szOscill		= CSize( nXWidth, 128 );
	}
	else if( szReso.cx < 800+1 )
	{
		m_szImage		= CSize( nXWidth, szReso.cy );
		m_szGraph		= CSize( nXWidth, szReso.cy );
		m_pLabelControler->m_szLabel		= CSize( nXWidth, 48 );
		m_pMobileUScaleControler->m_szUScale		= CSize( nXWidth, 40 );
		m_pOscillographControler->m_szOscill		= CSize( nXWidth, 256 );
	}
	else if( szReso.cx < 1280+1 )
	{
		m_szImage		= CSize( nXWidth, szReso.cy );
		m_szGraph		= CSize( nXWidth, szReso.cy );
		m_pLabelControler->m_szLabel		= CSize( nXWidth, 64 );
		m_pMobileUScaleControler->m_szUScale		= CSize( nXWidth, 40 );
		m_pOscillographControler->m_szOscill		= CSize( nXWidth, 256 );
	}
	else if( szReso.cx < 1536+1 )
	{
		m_szImage		= CSize( nXWidth, szReso.cy );
		m_szGraph		= CSize( nXWidth, szReso.cy );
		m_pLabelControler->m_szLabel		= CSize( nXWidth, 96 );
		m_pMobileUScaleControler->m_szUScale		= CSize( nXWidth, 64 );
		m_pOscillographControler->m_szOscill		= CSize( nXWidth, 512 );
	}
	else if( szReso.cx < 2048+1 )
	{
		m_szImage		= CSize( nXWidth, szReso.cy );
		m_szGraph		= CSize( nXWidth, szReso.cy );
		m_pLabelControler->m_szLabel		= CSize( nXWidth, 128 );
		m_pMobileUScaleControler->m_szUScale		= CSize( nXWidth, 80 );
		m_pOscillographControler->m_szOscill		= CSize( nXWidth, 512 );
	}
	else
	{
		m_szImage		= CSize( nXWidth, szReso.cy );
		m_szGraph		= CSize( nXWidth, szReso.cy );
		m_pLabelControler->m_szLabel		= CSize( nXWidth, 256 );
		m_pMobileUScaleControler->m_szUScale		= CSize( nXWidth, 160 );
		m_pOscillographControler->m_szOscill		= CSize( nXWidth, 512 );
	}
	m_pHeaderControler->m_szHeader		= m_pLabelControler->m_szLabel;
	m_pHeaderControler->m_szHeader.cy	/= 2;
}

CString  CMCICard::GetMobileUScaleText() 
{
	return m_pMobileUScaleControler->GetMobileUScaleText();
}