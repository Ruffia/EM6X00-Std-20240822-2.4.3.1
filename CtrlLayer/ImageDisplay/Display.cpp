// Display.cpp : implementation file
//

#include "stdafx.h"
#include "Ddutil.h"
#include "Display.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCriticalSection CDisplay::m_mutexLayAll;
CCriticalSection CDisplay::m_mutexImageDib;


void CaptureScreenRect( CBitmap& bmpScreen, CDC* pScreenDC, const CRect& rcCapture )
{
	/////////////////////////////////////////////
	HDC hcompDC = CreateCompatibleDC(pScreenDC->m_hDC);
	BITMAPINFO	bmi;
	BYTE		bits;

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

/////////////////////////////////////////////////////////////////////////////
// CDisplay

IMPLEMENT_DYNCREATE(CDisplay, CWinThread)

CDisplay::CDisplay()
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
// 	m_lpDDSLabelLay			= NULL;		// Offscreen surface Label Lay
// 	m_lpDDSMobileUScaleLay	= NULL;		// Offscreen surface UScale Lay
// 	m_lpDDSOscillographLay	= NULL;		// Offscreen surface Oscillograph Lay
// 	m_lpDDSFrameRate		= NULL;		// Offscreen surface Info Lay
/*
	m_sysOpt.color[SysOpt_clrTransparent] = g_clrUSB_Transparent;

#ifdef GRAPHIC_Transparent
	m_sysOpt.color[SysOpt_clrBack]		= g_clrUSB_Transparent;
#else
	m_sysOpt.color[SysOpt_clrBack]		= g_clrUSB_Background;
#endif
*/
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	m_hdibImage				= NULL;
	m_lpImageDibPlan		= NULL;
	/////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	m_pScreenWnd			= NULL;
	m_dblZoom				= 1.0;
//	SetResolution( USB_RESO_512_512 );

	m_lastTickCount			= 0;
	///////////////////////////////////////////////////////////////////

/*	/////////////////////////////////////////////////////////////////
	// Label & uScale
	/////////////////////////////////////////////////////////////////
	m_boolRedrawLabel		= FALSE;
	m_boolRedrawUScal		= FALSE;

	m_boolShowLabel			= TRUE;
	m_boolShowMobileUScale	= FALSE;

    m_hfontEditText			= NULL;	// 2004-8-2_加强图形层显示
    m_hfontLabelText		= NULL;
    m_hfontLabelLogoText	= NULL;
	m_hfontMobileUScaleText	= NULL;
	///////////////////////////////////////////////////////////////////
	m_sysOpt.color[SysOpt_clrEditText]			= g_clrUSB_EditText;
	m_sysOpt.color[SysOpt_clrLabelText]			= g_clrUSB_LabelText;
	m_sysOpt.color[SysOpt_clrLabelLogoText]		= g_clrUSB_LabelLogoText;
	m_sysOpt.color[SysOpt_clrOscillographGrid]	= RGB(0,255,0);
	m_sysOpt.color[SysOpt_clrOscillographGraph]	= g_clrUSB_MobileUScale;
	///////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// Statistical infomation
	/////////////////////////////////////////////////////////////////
    //m_boolShowFrameRate	= TRUE;
    m_boolShowFrameRate		= FALSE;

    m_hFontFrameRate		= NULL;
	m_sysOpt.color[SysOpt_clrFrameRate] = RGB(255,0,0);
    m_szFrameRate[0]		= '\0';
	sprintf( m_szFPS, "%s", "FPS %03d BPS %04dK IPS %03d" );

    m_dwFrameTime			= 0;

	m_dwFrameCount			= 0;
    m_dwFramesLast			= 0;
	m_dwFrames				= 0;
    m_iFrameRateX			= 0;

    m_dwAveFrameRate		= 0;
    m_dwAveFrameRateCount	= 0;
*/
	m_dwByteCount			= 0;
    m_dwBytesLast			= 0;
	m_dwBytes				= 0;
//	m_iBpsRateX				= 0;

	m_dwInputCount			= 0;
    m_dwInputsLast			= 0;
	m_dwInputs				= 0;
//	m_iIPSRateX				= 0;
	/////////////////////////////////////////////////////////////////
	m_boolPreview			= FALSE;
	m_boolNewScanScreenMode	= TRUE;
/*
	m_nDataStoreMode		= 1;					// XYZ存储方式
	m_nDetectorMode			= 0;					// 12.02.15 单探测器模式
	m_bDet2nd				= FALSE;				// 默认显示偶数通道图像
	m_bDebugZ				= FALSE;				// 13.04.03 非调试模式
	m_bDebugOsc				= FALSE;
	m_debugOSC.b90			= FALSE;
	m_debugOSC.nIndexY		= 1;					// 13.04.03 视频示波器Y向512像素代表+/-5
	m_debugOSC.dOffset		= 0.0;					// 13.08.05 示波器偏移量

	GetModuleFilePath( g_exeFullPath );

	m_bDataNeedUpdated		= FALSE;
	m_bStopScan				= FALSE;
//	m_pBuffUSB				= NULL;
	m_pBuffUSB	= new unsigned short[LOOP_COUNTS*409600];
	m_dwExpectantDMABlockSize = 0;
	m_pPixX		= new WORD[ LOOP_COUNTS*409600/4 ];
	m_pPixY		= new WORD[ LOOP_COUNTS*409600/4 ];
	m_pPixZ		= new WORD[ LOOP_COUNTS*409600/4 ];

	m_pMPComm		= NULL;
	m_hFileLog		= NULL;
	m_bNeedMon		= FALSE;
	m_hLineDataLog	= NULL;
	m_hOscillLog	= NULL;

	GetMonitorSizeFromEDIDEx( m_dblPixelsPerMM_HORZ, m_dblPixelsPerMM_VERT );

	for( int i=0; i<SharedImageBuffCounts; i++ )
		m_pSharedImageBuff[i] = NULL;
	m_nSharedImageBuffIndexRead		= 0;
	m_nSharedImageBuffIndexWrite	= 0;
*/
	m_nCurrentY1	= 0;
	m_nCurrentY2	= 0;
}

CDisplay::~CDisplay()
{
	Close();

	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);
}

BOOL CDisplay::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	// loop but check for kill notification
	while (WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT)
		MainLoop();

	// avoid entering standard message loop by returning FALSE
	return FALSE;  // TRUE;
}

int CDisplay::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CDisplay, CWinThread)
	//{{AFX_MSG_MAP(CDisplay)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplay message handlers
void CDisplay::Delete()
{
	// calling the base here won't do anything but it is a good habit
	CWinThread::Delete();

	// acknowledge receipt of kill notification
	VERIFY(SetEvent(m_hEventDead));
}

void CDisplay::KillThread()
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

BOOL CDisplay::Close()
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_boolReady		= FALSE;

	/////////////////////////////////////////////////////////////////
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

/*		if( m_lpDDSLabelLay != NULL )
		{
			m_lpDDSLabelLay->Release();
			m_lpDDSLabelLay = NULL;
		}

		if( m_lpDDSMobileUScaleLay != NULL )
		{
			m_lpDDSMobileUScaleLay->Release();
			m_lpDDSMobileUScaleLay = NULL;
		}

		if( m_lpDDSOscillographLay != NULL )
		{
			m_lpDDSOscillographLay->Release();
			m_lpDDSOscillographLay = NULL;
		}

		if( m_lpDDSFrameRate != NULL )
		{
			m_lpDDSFrameRate->Release();
			m_lpDDSFrameRate = NULL;
		}
*/
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
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
    m_blSrcColorkey		= FALSE;
    m_blDestColorkey	= FALSE;
	/////////////////////////////////////////////////////////////////

/*	/////////////////////////////////////////////////////////////////
    if (m_hFontFrameRate != NULL)
    {
        DeleteObject(m_hFontFrameRate);
		m_hFontFrameRate = NULL;
    }
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
    if (m_hfontEditText != NULL)	// 2004-8-2_加强图形层显示
    {
        DeleteObject(m_hfontEditText);
		m_hfontEditText = NULL;
    }
    if (m_hfontLabelText != NULL)
    {
        DeleteObject(m_hfontLabelText);
		m_hfontLabelText = NULL;
    }
    if (m_hfontLabelLogoText != NULL)
    {
        DeleteObject(m_hfontLabelLogoText);
		m_hfontLabelLogoText = NULL;
    }
	/////////////////////////////////////////////////////////////////
    if (m_hfontMobileUScaleText != NULL)
    {
        DeleteObject(m_hfontMobileUScaleText);
		m_hfontMobileUScaleText = NULL;
    }
	/////////////////////////////////////////////////////////////////
*/
	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
	return TRUE;
}

BOOL CDisplay::Reset( CWnd* pWnd )
{
    DDSURFACEDESC			ddsd;
    HRESULT					ddrval;

	Close();

	m_pScreenWnd = pWnd;

	/////////////////////////////////////////////////////////////////
	m_hdibImage = CreateImageBitmap();
	if ( m_hdibImage == NULL )
	{
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////
    // *
    // * create the main DirectDraw object
    // *
	/////////////////////////////////////////////////////////////////
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
/*	ddrval = DirectDrawCreate( NULL, &m_lpDD, NULL );
    if( ddrval != DD_OK )
	{
        DebugMessage("DirectDrawCreate failed err=%d", ddrval);
		return FALSE;
	}
*/	ddrval = m_lpDD->SetCooperativeLevel( m_pScreenWnd->m_hWnd, DDSCL_NORMAL );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
    // *
    // * check capabilites
    // *
	/////////////////////////////////////////////////////////////////
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
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
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
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	BOOL boolUseSysMemPrimaryBackLay;
	BOOL boolUseSysMemGraphBackLay;
	BOOL boolUseSysMemImageLay;
 	BOOL boolUseSysMemGraphLay;
// 	BOOL boolUseSysMemLabelLay;
// 	BOOL boolUseSysMemUScaleLay;

	DWORD dwFrameMemSizePrimaryBackLay = 
		GetSystemMetrics(SM_CXSCREEN) * GetSystemMetrics(SM_CYSCREEN) * ( m_uBPP / 8 );
	DWORD dwFrameMemSizeImageLay = m_szImage.cx * m_szImage.cy * ( m_uBPP / 8 );
	DWORD dwFrameMemSizeGraphLay = m_szGraph.cx * m_szGraph.cy * ( m_uBPP / 8 );
/*	DWORD dwFrameMemSizeLabelLay = m_szLabel.cx * m_szLabel.cy * ( m_uBPP / 8 );
	//07.10.31
	DWORD dwFrameMemSizeUScaleLay;
//	dwFrameMemSizeUScaleLay = m_szUScale.cx * m_szUScale.cy * ( m_uBPP / 8 );
#ifdef NEW_SCANSCREEN_MODE
	dwFrameMemSizeUScaleLay = m_szGraph.cx * m_szGraph.cy * ( m_uBPP / 8 );
#else
	dwFrameMemSizeUScaleLay = m_szUScale.cx * m_szUScale.cy * ( m_uBPP / 8 );
#endif
	//07.10.31
*/	DWORD dwFrameMemSizeGraphBackLay = dwFrameMemSizeGraphLay;
	/////////////////////////////////////////////////////////////////

	m_dwVidMemTotal = ddcaps.dwVidMemTotal;
	/////////////////////////////////////////////////////////////////
    if( ddcaps.dwVidMemTotal <= dwFrameMemSizePrimaryBackLay )
    {
		boolUseSysMemPrimaryBackLay	= TRUE;
		boolUseSysMemGraphBackLay	= TRUE;
		boolUseSysMemImageLay	= TRUE;
 		boolUseSysMemGraphLay	= TRUE;
// 		boolUseSysMemLabelLay	= TRUE;
// 		boolUseSysMemUScaleLay	= TRUE;

		m_dwFrameMemSizeUsed	= 0;

        DebugMessage("Using system memory at all (not enough memory)");
    }
    else if( ddcaps.dwVidMemTotal <= dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay )
    {
		boolUseSysMemPrimaryBackLay	= FALSE;
		boolUseSysMemGraphBackLay	= TRUE;
		boolUseSysMemImageLay	= TRUE;
 		boolUseSysMemGraphLay	= TRUE;
// 		boolUseSysMemLabelLay	= TRUE;
// 		boolUseSysMemUScaleLay	= TRUE;

		m_dwFrameMemSizeUsed	= dwFrameMemSizePrimaryBackLay;

        DebugMessage("Using system memory partly (not enough memory)");
    }
    else if( ddcaps.dwVidMemTotal <= dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay )
    {
		boolUseSysMemPrimaryBackLay	= FALSE;
		boolUseSysMemGraphBackLay	= FALSE;
		boolUseSysMemImageLay	= TRUE;
 		boolUseSysMemGraphLay	= TRUE;
// 		boolUseSysMemLabelLay	= TRUE;
// 		boolUseSysMemUScaleLay	= TRUE;

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
// 		boolUseSysMemLabelLay	= TRUE;
// 		boolUseSysMemUScaleLay	= TRUE;

		m_dwFrameMemSizeUsed	= dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay;

        DebugMessage("Using system memory partly (not enough memory)");
    }
/*	else if( ddcaps.dwVidMemTotal <= 
		dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay + dwFrameMemSizeLabelLay )
    {
		boolUseSysMemPrimaryBackLay	= FALSE;
		boolUseSysMemGraphBackLay	= FALSE;
		boolUseSysMemImageLay	= FALSE;
// 		boolUseSysMemGraphLay	= FALSE;
// 		boolUseSysMemLabelLay	= TRUE;
// 		boolUseSysMemUScaleLay	= TRUE;

		m_dwFrameMemSizeUsed	= dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay;

        DebugMessage("Using system memory partly (not enough memory)");
    }
    else if( ddcaps.dwVidMemTotal <= 
		dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay + dwFrameMemSizeLabelLay + dwFrameMemSizeUScaleLay )
    {
		boolUseSysMemPrimaryBackLay	= FALSE;
		boolUseSysMemGraphBackLay	= FALSE;
		boolUseSysMemImageLay	= FALSE;
// 		boolUseSysMemGraphLay	= FALSE;
// 		boolUseSysMemLabelLay	= FALSE;
// 		boolUseSysMemUScaleLay	= TRUE;

		m_dwFrameMemSizeUsed	=
			dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay + dwFrameMemSizeLabelLay;

        DebugMessage("Using system memory partly (not enough memory)");
    }
*/	else
	{
		boolUseSysMemPrimaryBackLay	= FALSE;
		boolUseSysMemGraphBackLay	= FALSE;
		boolUseSysMemImageLay	= FALSE;
 		boolUseSysMemGraphLay	= FALSE;
// 		boolUseSysMemLabelLay	= FALSE;
// 		boolUseSysMemUScaleLay	= FALSE;

		m_dwFrameMemSizeUsed	=
			dwFrameMemSizePrimaryBackLay + dwFrameMemSizeGraphBackLay + dwFrameMemSizeImageLay + dwFrameMemSizeGraphLay;// + dwFrameMemSizeLabelLay + dwFrameMemSizeUScaleLay;

        DebugMessage("Using video memory at all");	
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
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
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
    // Create the primary surface
	/////////////////////////////////////////////////////////////////
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    ddrval = m_lpDD->CreateSurface( &ddsd, &m_lpDDSPrimary, NULL );
    if( ddrval != DD_OK )
	{
        DebugMessage("Create Primary Surface failed err=%d", ddrval);
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
    // create a clipper for the primary surface
	/////////////////////////////////////////////////////////////////
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
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
/*	if ( m_uBPP == 8 )
	{
	    // load our palette
	    m_lpDDPalette = DDLoadPalette(m_lpDD, m_hdibImage);

		// make sure to set the palette before loading bitmaps.
		if (m_lpDDPalette)
		{
			m_lpDDSPrimary->SetPalette(m_lpDDPalette);
		}
	}*/
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// create a DirectDrawSurface for Primary BackLay
	/////////////////////////////////////////////////////////////////
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
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// create a DirectDrawSurface for Graph BackLay
	/////////////////////////////////////////////////////////////////
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
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// create a DirectDrawSurface for ImageLay
	/////////////////////////////////////////////////////////////////
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
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
    // create a DirectDrawSurface for GraphLay
	/////////////////////////////////////////////////////////////////
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
	/////////////////////////////////////////////////////////////////

/*	/////////////////////////////////////////////////////////////////
    // create a DirectDrawSurface for LabelLay
	/////////////////////////////////////////////////////////////////
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize			= sizeof(ddsd);
    ddsd.dwFlags		= DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth		= m_szLabel.cx;
    ddsd.dwHeight		= m_szLabel.cy;
	if( boolUseSysMemLabelLay )
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
    ddrval = m_lpDD->CreateSurface( &ddsd, &m_lpDDSLabelLay, NULL );
    if ( ddrval != DD_OK )
	{
        DebugMessage("Create LabelLay Surface failed err=%d", ddrval);
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
    // create a DirectDrawSurface for UScaleLay
	/////////////////////////////////////////////////////////////////
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize			= sizeof(ddsd);
    ddsd.dwFlags		= DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
	//07.10.31
//	ddsd.dwWidth		= szUScale.cx;
//	ddsd.dwHeight		= szUScale.cy;
#ifdef NEW_SCANSCREEN_MODE
	ddsd.dwWidth		= m_szGraph.cx;
	ddsd.dwHeight		= m_szGraph.cy;
#else
	ddsd.dwWidth		= m_szUScale.cx;
	ddsd.dwHeight		= m_szUScale.cy;
#endif
	//07.10.31
	if( boolUseSysMemUScaleLay )
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
    ddrval = m_lpDD->CreateSurface( &ddsd, &m_lpDDSMobileUScaleLay, NULL );
    if ( ddrval != DD_OK )
	{
        DebugMessage("Create UScaleLay Surface failed err=%d", ddrval);
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////
*/
	/////////////////////////////////////////////////////////////////
    // *
    // * init the color key
    // *
	/////////////////////////////////////////////////////////////////
	DWORD	dwColorKey = 0;	
	
	dwColorKey = DDColorMatch(m_lpDDSGraphLay, RGB(0,0,0));	//m_sysOpt.color[SysOpt_clrTransparent]);
	
    if (m_blSrcColorkey)
    {
		DDCOLORKEY		ddck;
		ddck.dwColorSpaceLowValue	= dwColorKey;
		ddck.dwColorSpaceHighValue	= dwColorKey;
		m_lpDDSGraphBackLay->SetColorKey( DDCKEY_SRCBLT, &ddck );
		m_lpDDSGraphLay->SetColorKey( DDCKEY_SRCBLT, &ddck );
// 		m_lpDDSLabelLay->SetColorKey( DDCKEY_SRCBLT, &ddck );
// 		m_lpDDSMobileUScaleLay->SetColorKey( DDCKEY_SRCBLT, &ddck );
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// Init Lay
	/////////////////////////////////////////////////////////////////
	HBRUSH brBackBrush = CreateSolidBrush( RGB(0,0,0));	//m_sysOpt.color[SysOpt_clrTransparent] );

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

/*	ddrval = m_lpDDSLabelLay->GetDC( &hdc );
	if( ddrval == DD_OK )
	{
		rcFill = CRect(0,0,m_szLabel.cx,m_szLabel.cy);
		FillRect( hdc, (LPRECT)rcFill, brBackBrush );
		m_lpDDSLabelLay->ReleaseDC(hdc);
	}

	ddrval = m_lpDDSMobileUScaleLay->GetDC( &hdc );
	if( ddrval == DD_OK )
	{
		rcFill = CRect(0,0,m_szUScale.cx,m_szUScale.cy);
		FillRect( hdc, (LPRECT)rcFill, brBackBrush );
		m_lpDDSMobileUScaleLay->ReleaseDC(hdc);
	}
*/
	DeleteObject( hPenR );
	DeleteObject( hPenG );
	DeleteObject( hPenB );

	DeleteObject( brBackBrush );
	/////////////////////////////////////////////////////////////////

/*	/////////////////////////////////////////////////////////////////
    if ( !InitFrameRate() )
	{
        DebugMessage("InitFrameRate failed err");
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( !InitLabelLay() )
	{
        DebugMessage("InitLabelLay failed err");
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( !InitMobileUScaleLay() )
	{
        DebugMessage("InitMobileUScale failed err");
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////
	if ( !InitOscillographLay() )
	{
        DebugMessage("InitOscillographLay failed err");
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////
*/	
	/////////////////////////////////////////////////////////////////
	m_UpdateRectsImage.Add( CRect(0, 0, m_szImage.cx, m_szImage.cy) );
	m_boolReady	= TRUE;
	/////////////////////////////////////////////////////////////////
	return TRUE;
}

DWORD CDisplay::ImageDibWrite( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	LPSTR		lpDIBHdr, lpDIBBits;
	LPSTR		lpBuffLine;
	LONG		lBuffLineOffset;
	LPSTR		lpImageWindowLine;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;
	DWORD		i, iMaxLine;

	////////////////////////////////////////////////////////////////////
	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;
	////////////////////////////////////////////////////////////////////
	
	m_mutexImageDib.Lock();
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
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
		memcpy( (void *)lpImageWindowLine, (const void *)lpBuffLine, lBuffLineOffset );
		lpBuffLine			+= lBuffLineOffset;
		lpImageWindowLine	-= lImageLineOffset;
	}
	::GlobalUnlock((HGLOBAL) m_hdibImage);
	////////////////////////////////////////////////////////////////////

	m_UpdateRectsImage.Add( rcFrameDestWindow );

	m_mutexImageDib.Unlock();
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	i = iMaxLine * lBuffLineOffset;
	m_dwByteCount += i;
	m_dwInputCount ++;
	////////////////////////////////////////////////////////////////////

	return i;
}

DWORD CDisplay::ImageDibRead( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	LPSTR		lpDIBHdr, lpDIBBits;
	LPSTR		lpBuffLine;
	LONG		lBuffLineOffset;
	LPSTR		lpImageWindowLine;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;
	DWORD		i, iMaxLine;

	////////////////////////////////////////////////////////////////////
	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;
	////////////////////////////////////////////////////////////////////
	
	m_mutexImageDib.Lock();
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
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
	////////////////////////////////////////////////////////////////////

	m_mutexImageDib.Unlock();
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	i = iMaxLine * lBuffLineOffset;
	////////////////////////////////////////////////////////////////////

	return i;
}

DWORD CDisplay::ImageDibCopy( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	LPSTR		lpDIBHdr, lpDIBBits;
	LPSTR		lpBuffLine;
	LONG		lBuffLineOffset;
	LPSTR		lpImageWindowLine;
	LONG		lImageLineOffset;
	CRect		rcImage, rcFrame;
	DWORD		i, iMaxLine;

	////////////////////////////////////////////////////////////////////
	rcFrame.SetRect( 0,0,m_szImage.cx,m_szImage.cy );
	rcImage = rcFrameDestWindow & rcFrame;
	if ( rcImage != rcFrameDestWindow )
		return 0;
	////////////////////////////////////////////////////////////////////
	
	m_mutexImageDib.Lock();
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
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
	////////////////////////////////////////////////////////////////////

	m_mutexImageDib.Unlock();
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	i = iMaxLine * lBuffLineOffset;
	////////////////////////////////////////////////////////////////////

	return i;
}

HDIB CDisplay::CreateImageBitmap()
{
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
	for (i=0; i<256; i++)
	{ 
		bGrey = (BYTE)i;
		pDibInfo->bmiColors[i].rgbRed		= bGrey;
		pDibInfo->bmiColors[i].rgbGreen		= bGrey;
		pDibInfo->bmiColors[i].rgbBlue		= bGrey;
		pDibInfo->bmiColors[i].rgbReserved	= (BYTE)0;
	}

	// Fill the bits.
//	FillMemory( (PVOID)pDibBits, dwBitsSize, 0xff );
	
/*	for (LONG dwLine=64; dwLine<m_szImage.cy; dwLine+=64)
	{
		FillMemory( (PVOID)&(pDibBits[dwLine*m_szImage.cx]),
			m_szImage.cx, 0xFF );
	}
*/
	::GlobalUnlock((HGLOBAL) hDIB);
	m_lpImageDibPlan = NULL;

	if ( hDIB != NULL ) 
		CreateDIBPalette(hDIB, &m_paletteImage);

	return hDIB;
}

long CDisplay::UpdateImageLay()
{
	CRect			rcTemp;
    HRESULT			ddrval = DD_OK;
    HDC             hdc;

	m_mutexImageDib.Lock();

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

long CDisplay::UpdateGraphBackLay()
{
	CRect			rcTemp;
    HRESULT			ddrval = DD_OK;
	
	while ( !m_UpdateRectsGraphBack.IsEmpty() )
	{
		rcTemp = m_UpdateRectsGraphBack.Get();
		
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

long CDisplay::UpdatePriamryLay()
{
    HRESULT			ddrval;

	CRect rcSrcImageLay	= m_rcImageWindow;
	CRect rcDestPriamryLay	= LayWindowMapClip_Image_to_Priamry( rcSrcImageLay );

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

//	ddrval = m_lpDDSPrimary->Blt(NULL, m_lpDDSPrimaryBackLay, NULL, DDBLT_WAIT, NULL); 
	ddrval = m_lpDDSPrimary->Blt(
		(LPRECT)rcDestPriamryLay,		// dest rect
		m_lpDDSPrimaryBackLay,			// src surface
		(LPRECT)rcDestPriamryLay,		// src rect
		DDBLT_WAIT,
		NULL);
	
	return ddrval;
}

// 09.11.24加入同步语句
// 改变放大倍数，由任意变为Fit时有时会出现图像被拉伸；或由大变小时，小区域到原来区域间有未被重画的区域
// 在调试过程中发现：GetClientWindow中会对m_rcScreenClient根据放大倍数做调整，
//					而LayWindowMapClip_Image_to_Priamry中如果Fit模式，会用到m_rcScreenClient，
//					这有可能造成使用的同步问题，故加入同步语句
void CDisplay::SetZoom( double dblZoom )
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_dblZoom = dblZoom;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

void CDisplay::SetDestFrame(CRect rcDestFrame)
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_rcDestFrame = rcDestFrame;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

CSize CDisplay::GetSizes( int nIndex )
{
	CSize sz(0,0);
	switch( nIndex )
	{
	case	0:
		sz.cx	= m_rcImageWindow.Width();
		sz.cy	= m_rcImageWindow.Height();
		break;
	case	1:
		sz		= m_szImage;
		break;
	}
	return sz;
}

void CDisplay::GetClientWindow()
{
	///////////////////////////////////////////////////////////////////
	if( m_boolPreview )	//05.02.06 照相预览问题；
	{					//05.06.07修正，去掉Border和Caption部分，保证尺寸为12cm X 9cm
		CRect rcRect(0,0,0,0);
/*		CDC* pDC = m_pScreenWnd->GetDC();
		double dblPixelsPerMM_HORZ = 
			(double)pDC->GetDeviceCaps(HORZRES) / pDC->GetDeviceCaps(HORZSIZE);
		double dblPixelsPerMM_VERT = 
			(double)pDC->GetDeviceCaps(VERTRES) / pDC->GetDeviceCaps(VERTSIZE);
		m_pScreenWnd->ReleaseDC(pDC);

		rcRect.right	= (int)(SEM_dfStandImageWidth * dblPixelsPerMM_HORZ + 0.5);
		rcRect.bottom	= (int)(SEM_dfStandImageHeight * dblPixelsPerMM_VERT + 0.5);
*/
/*		CDC dc;
		dc.Attach( m_pScreenWnd->GetDC()->m_hDC );
		double dblPixelsPerInch_HORZ = dc.GetDeviceCaps( LOGPIXELSX );	// 屏幕横向每英寸像素数，例如：96
		double dblPixelsPerInch_VERT = dc.GetDeviceCaps( LOGPIXELSX );	// 屏幕纵向每英寸像素数，例如：96
		double dblPhysicalDisW = dc.GetDeviceCaps( HORZSIZE );	// 屏幕横向尺寸，单位毫米，例如410mm
		double dblPhysicalDisH = dc.GetDeviceCaps( VERTSIZE );	// 屏幕纵向尺寸，单位毫米，例如210mm
		double dblDisW = dc.GetDeviceCaps( HORZRES );	// 屏幕横向分辨率，单位像素，例如：1440
		double dblDisH = dc.GetDeviceCaps( VERTRES );	// 屏幕纵向分辨率，单位像素，例如：900
		dc.Detach();
*/
//		rcRect.right	= (int)(SEM_dfStandImageWidth * dblDisW / dblPhysicalDisW + 0.5 );
//		rcRect.bottom	= (int)(SEM_dfStandImageHeight * dblDisH / dblPhysicalDisH + 0.5 );
		double dRatio	= 1.0 *m_sizeReso.cy /m_sizeReso.cx;
		if( dRatio == 0.5 )
		{
			rcRect.right	= (int)(120 * m_dblPixelsPerMM_HORZ + 0.5 );
			rcRect.bottom	= (int)(60 * m_dblPixelsPerMM_VERT + 0.5 );
		}
		else if( dRatio == 1.0 )
		{
			rcRect.right	= (int)(120 * m_dblPixelsPerMM_HORZ + 0.5 );
			rcRect.bottom	= (int)(120 * m_dblPixelsPerMM_VERT + 0.5 );
		}
		else
		{
			rcRect.right	= (int)(120 * m_dblPixelsPerMM_HORZ + 0.5 );
			rcRect.bottom	= (int)(90 * m_dblPixelsPerMM_VERT + 0.5 );
		}
		//======2006.11.22_3200与3900采集屏显示兼容============
		// 修改标示：2006.11.22_3200与3900采集屏显示兼容
		// 修改日期：2006.11.22
		// 修改人  ：J.Y.Ma
		// 修改原因：合并程序后，使各主程序均能调用同一动态链接库

		// 本次修改前代码副本
		/*----------------------------------------------------- 
//			rcRect.right += GetSystemMetrics( SM_CXBORDER ) * 2;
//			rcRect.bottom += GetSystemMetrics( SM_CYBORDER ) * 2;
//			rcRect.bottom += GetSystemMetrics( SM_CYCAPTION );
		-----------------------------------------------------*/
		
		// 本次修改后代码
		//-----------------------------------------------------
		if( !m_boolNewScanScreenMode )	//06.11.22
		{
			rcRect.right += GetSystemMetrics( SM_CXBORDER ) * 2;
			rcRect.bottom += GetSystemMetrics( SM_CYBORDER ) * 2;
			rcRect.bottom += GetSystemMetrics( SM_CYCAPTION );
		}
		//-----------------------------------------------------
		//======2006.11.22_3200与3900采集屏显示兼容============

		m_rcScreenClientRect = rcRect;
	}
	else
	{
		m_pScreenWnd->GetClientRect( (LPRECT)m_rcScreenClientRect );
//------05.02.05 采集窗4:3问题 Start ----------------
		//======2006.11.22_3200与3900采集屏显示兼容============
		// 修改标示：2006.11.22_3200与3900采集屏显示兼容
		// 修改日期：2006.11.22
		// 修改人  ：J.Y.Ma
		// 修改原因：合并程序后，使各主程序均能调用同一动态链接库

		// 本次修改前代码副本
		/*----------------------------------------------------- 
		-----------------------------------------------------*/
		
		// 本次修改后代码
		//-----------------------------------------------------
		if( m_dblZoom == 0. && m_boolNewScanScreenMode )	//06.11.22
		{
/*			int w = m_rcScreenClientRect.Width();
			int h = m_rcScreenClientRect.Height();
			int w_scan, h_scan, base;
			if( w/4*3 < h )
			{
				base = (int)(1.*w/16 +0.5);
				w_scan = base*16;
				h_scan = base*12;
			}
			else
			{
				base = (int)(1.*h/12 +0.5);
				h_scan = base*12;
				w_scan = base*16;
			}

			m_rcScreenClientRect = CRect( m_rcScreenClientRect.left, m_rcScreenClientRect.top,
									m_rcScreenClientRect.left +w_scan, m_rcScreenClientRect.top +h_scan);
*/		}
		//-----------------------------------------------------
		//======2006.11.22_3200与3900采集屏显示兼容============
//------05.02.05 采集窗4:3问题 End ------------------
	}
	m_ptScreenClientPos.x = m_pScreenWnd->GetScrollPos( SB_HORZ );
	m_ptScreenClientPos.y = m_pScreenWnd->GetScrollPos( SB_VERT );
	///////////////////////////////////////////////////////////////////

	m_pScreenWnd->ClientToScreen( (LPRECT)m_rcScreenClientRect );
}

void CDisplay::PaintScreenWindow()
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
			m_pScreenWnd->InvalidateRect(NULL);
		}

		ptScreenPos_Old = ptScreenPos;
	}
}

void CDisplay::MainLoop()
{
	DWORD	thisTickCount;
    
	///////////////////////////////////////////////////////
	if ( !m_boolReady )
    {
		Sleep( 1 );
		return;
    }
	///////////////////////////////////////////////////////
	if ( m_boolShouldReset )
    {
		m_lpDDSPrimary->Restore();
		m_boolShouldReset = FALSE;
		Sleep( 1 );
		return;
    }

//	if( m_bDataNeedUpdated )
/*	if( m_nSharedImageBuffIndexRead != m_nSharedImageBuffIndexWrite )
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
*/	{
		// 12.02.15 使用m_nDetectorMode临时作为判断是采集/冻结状态
		// -1表示冻结
		// 采集时如果没有数据更新就返回
		// 冻结时要继续执行之后的语句
/*		if( m_nDetectorMode > -1 )
		{
			Sleep( 1 );
			return;
		}*/
	///////////////////////////////////////////////////////
	thisTickCount = GetTickCount();
    if ((thisTickCount - m_lastTickCount) < 8)//g_dwUSB_UpdateDelay)
    {
		Sleep( 1 );
//		#ifdef _DEBUG
//			afxDump << "thisTickCount - m_lastTickCount = " << thisTickCount << " - " << m_lastTickCount << " = " << thisTickCount - m_lastTickCount << "\n";
//		#endif
		return;
    }
//		#ifdef _DEBUG
//			afxDump << "thisTickCount - m_lastTickCount = " << thisTickCount << " - " << m_lastTickCount << " = " << thisTickCount - m_lastTickCount << "\n";
//		#endif
	m_lastTickCount = thisTickCount;
	///////////////////////////////////////////////////////

	}

//	DWORD this1, this2;
//	this1 = GetTickCount();
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();	//09.12.03 (v 1.0.0.6 +)
	///////////////////////////////////////////////////////
//	this2 = GetTickCount();
//	#ifdef _DEBUG
//		afxDump << "01. Lock = " << this2 - this1 << "\n";
//	#endif

//	this1 = GetTickCount();
	GetClientWindow();

	///////////////////////////////////////////////////////////////////
	m_dblScaleImageToGraph_W = (double)m_szGraph.cx / m_rcImageWindow.Width();
	m_dblScaleImageToGraph_H = (double)m_szGraph.cy / m_rcImageWindow.Height();
	///////////////////////////////////////////////////////////////////
//	this2 = GetTickCount();
//	#ifdef _DEBUG
//		afxDump << "02. GetClientWindow = " << this2 - this1 << "\n";
//	#endif

	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
//	m_mutexLayAll.Lock();	//09.12.03 (v 1.0.0.5 src, v 1.0.0.6 -)
	///////////////////////////////////////////////////////

	
//	this1 = GetTickCount();
	UpdateImageLay();
//	this2 = GetTickCount();
//	#ifdef _DEBUG
//		afxDump << "06. ImageLay = " << this2 - this1 << "\n";
//	#endif


//	this1 = GetTickCount();
	UpdateGraphBackLay();
//	this2 = GetTickCount();
//	#ifdef _DEBUG
//		afxDump << "11. GraphBackLay = " << this2 - this1 << "\n";
//	#endif


//	this1 = GetTickCount();
	long ddrval = UpdatePriamryLay();
	////////////////////////////////////
	if ((ddrval == DDERR_SURFACELOST )
		||(ddrval == DDERR_GENERIC ))
	{
		m_boolShouldReset	= TRUE;
	}
	////////////////////////////////////
//	this2 = GetTickCount();
//	#ifdef _DEBUG
//		afxDump << "13. PriamryLay = " << this2 - this1 << "\n";
//	#endif

//	this1 = GetTickCount();
	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
//	this2 = GetTickCount();
//	#ifdef _DEBUG
//		afxDump << "14. Unlock = " << this2 - this1 << "\n";
//	#endif

//	this1 = GetTickCount();
	PaintScreenWindow();
	Sleep(0);
//	this2 = GetTickCount();
//	#ifdef _DEBUG
//		afxDump << "15. PaintScreenWindow = " << this2 - this1 << "\n";
//	#endif
}

void CDisplay::SetResolution(CSize szReso)
{
	///////////////////////////////////////////////////////
	m_mutexImageDib.Lock();
//	m_mutexImageUSB.Lock();
	///////////////////////////////////////////////////////

	int nX;
	nX = szReso.cx;
	SetImageWindow( CRect(0,0,nX,szReso.cy) );
	if( szReso.cx < 16+1 )
	{
		m_szImage		= CSize( nX, szReso.cy );
		m_szGraph		= CSize( nX, szReso.cy );
	}
	else if( szReso.cx < 32+1 )
	{
		m_szImage		= CSize( nX, szReso.cy );
		m_szGraph		= CSize( nX, szReso.cy );
	}
	else if( szReso.cx < 64+1 )
	{
		m_szImage		= CSize( nX, szReso.cy );
		m_szGraph		= CSize( nX, szReso.cy );
	}
	else if( szReso.cx < 128+1 )
	{
		m_szImage		= CSize( nX, szReso.cy );
		m_szGraph		= CSize( nX, szReso.cy );
	}
	else if( szReso.cx < 256+1 )
	{
		m_szImage		= CSize( nX, szReso.cy );
		m_szGraph		= CSize( nX, szReso.cy );
	}
	else if( szReso.cx < 512+1 )
	{
		m_szImage		= CSize( nX, szReso.cy );
		m_szGraph		= CSize( nX, szReso.cy );
	}
	else if( szReso.cx < 640+1 )
	{
		m_szImage		= CSize( nX, szReso.cy );
		m_szGraph		= CSize( nX, szReso.cy );
	}
	else if( szReso.cx < 800+1 )
	{
		m_szImage		= CSize( nX, szReso.cy );
		m_szGraph		= CSize( nX, szReso.cy );
	}
	else if( szReso.cx < 1024+1 )
	{
		m_szImage		= CSize( nX, szReso.cy );
		m_szGraph		= CSize( nX, szReso.cy );
	}
	else
	{
		m_szImage		= CSize( nX, szReso.cy );
		m_szGraph		= CSize( nX, szReso.cy );
	}

	m_sizeReso = szReso;

	///////////////////////////////////////////////////////
//	m_mutexImageUSB.Unlock();
	m_mutexImageDib.Unlock();
	///////////////////////////////////////////////////////
}

void CDisplay::SetFunction( int nFunction )
{
	m_nusbFunction = nFunction;
}

void CDisplay::SetImageWindow( CRect rcImageWindow )
{
	m_rcImageWindow = rcImageWindow;
}

CRect CDisplay::LayWindowMapClip_Image_to_Priamry( CRect& rcImage )
{
	//////////////////////////////////////////////////////////////////////
	double	dblScaleImageToPriamry_W;
	double	dblScaleImageToPriamry_H;

	//////////////////////////////////////////////////////////////////////
	if ( m_dblZoom <= 0 )
	{
		dblScaleImageToPriamry_W = (double)m_rcScreenClientRect.Width() / m_rcImageWindow.Width();
		dblScaleImageToPriamry_H = (double)m_rcScreenClientRect.Height() / m_rcImageWindow.Height();
		m_ptScreenClientPos = CPoint(0, 0);	
	}
	else
	{
		dblScaleImageToPriamry_W = m_dblZoom;
		dblScaleImageToPriamry_H = m_dblZoom;
	}
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	CRect	rcPriamry;
	CRect	rcPriamryWindow;
	CPoint	ptOrigin(m_rcImageWindow.left, m_rcImageWindow.top);

	rcPriamryWindow.left = 0;
	rcPriamryWindow.top = 0;
	rcPriamryWindow.right = (int)(m_rcImageWindow.Width() * dblScaleImageToPriamry_W + 0.5);
	rcPriamryWindow.bottom = (int)(m_rcImageWindow.Height() * dblScaleImageToPriamry_H + 0.5);
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	// Image to Priamry (Logic)
	//////////////////////////////////////////////////////////////////////
	rcImage.OffsetRect( - ptOrigin );

	rcPriamry.left = (int)(rcImage.left * dblScaleImageToPriamry_W + 0.5);
	rcPriamry.right = (int)(rcImage.right * dblScaleImageToPriamry_W + 0.5);
	rcPriamry.top = (int)(rcImage.top * dblScaleImageToPriamry_H + 0.5);
	rcPriamry.bottom = (int)(rcImage.bottom * dblScaleImageToPriamry_H + 0.5);
	rcPriamry &= rcPriamryWindow;
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	// Priamry to Client (Logic)
	//////////////////////////////////////////////////////////////////////
	CRect	rcClient = rcPriamry;
	rcClient.OffsetRect( - m_ptScreenClientPos );
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	// Client to ScreenClient
	//////////////////////////////////////////////////////////////////////
	CRect	rcScreenClient;
	CPoint	ptOriginScreenClient(m_rcScreenClientRect.left, m_rcScreenClientRect.top);

	rcScreenClient = rcClient;
	rcScreenClient.OffsetRect( ptOriginScreenClient );
	rcScreenClient &= m_rcScreenClientRect;

	CRect rcSystemScreen(0,0,
		GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	rcScreenClient &= rcSystemScreen;
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	// ScreenClient to Client
	//////////////////////////////////////////////////////////////////////
	rcClient = rcScreenClient;
	rcClient.OffsetRect( - ptOriginScreenClient );
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	// Client to Priamry
	//////////////////////////////////////////////////////////////////////
	rcPriamry = rcClient;
	rcPriamry.OffsetRect( m_ptScreenClientPos );
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	// Priamry to Image
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	rcImage.left = (int)(rcPriamry.left / dblScaleImageToPriamry_W + 0.5);
	rcImage.right = (int)(rcPriamry.right / dblScaleImageToPriamry_W + 0.5);
	rcImage.top = (int)(rcPriamry.top / dblScaleImageToPriamry_H + 0.5);
	rcImage.bottom = (int)(rcPriamry.bottom / dblScaleImageToPriamry_H + 0.5);

	rcImage.OffsetRect( ptOrigin );
	//////////////////////////////////////////////////////////////////////

	return rcScreenClient;
}

CRect CDisplay::LayWindowMapClip_Image_to_Graph( CRect& rcImage )
{
	CRect	rcGrapic;
	CPoint	ptOrigin(m_rcImageWindow.left, m_rcImageWindow.top);

	rcImage.OffsetRect( - ptOrigin );

	//////////////////////////////////////////////////////////////////////
	rcGrapic.left = (int)(rcImage.left * m_dblScaleImageToGraph_W + 0.5);
	rcGrapic.right = (int)(rcImage.right * m_dblScaleImageToGraph_W + 0.5);
	rcGrapic.top = (int)(rcImage.top * m_dblScaleImageToGraph_H + 0.5);
	rcGrapic.bottom = (int)(rcImage.bottom * m_dblScaleImageToGraph_H + 0.5);

	rcGrapic &= CRect(0, 0, m_szGraph.cx, m_szGraph.cy);
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	rcImage.left = (int)(rcGrapic.left / m_dblScaleImageToGraph_W + 0.5);
	rcImage.right = (int)(rcGrapic.right / m_dblScaleImageToGraph_W + 0.5);
	rcImage.top = (int)(rcGrapic.top / m_dblScaleImageToGraph_H + 0.5);
	rcImage.bottom = (int)(rcGrapic.bottom / m_dblScaleImageToGraph_H + 0.5);
	//////////////////////////////////////////////////////////////////////

	rcImage.OffsetRect( ptOrigin );

	return rcGrapic;
}

HDIB CDisplay::CopyImageDib()
{
	CRect rcImageWindow = m_rcImageWindow;	//GetImageWindow();
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
	dwOffBits += sizeof(RGBQUAD) * 256;										// Palette ( RGBQUAD area )
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

