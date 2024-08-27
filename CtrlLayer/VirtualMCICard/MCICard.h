// Last Modify : 2009.11.24

/////////////////////////////////////////////////////////////////////////////
// 09.11.24在SetZoom中加入同步语句
/////////////////////////////////////////////////////////////////////////////

//#if !defined(AFX_MCICARD_H__3E643262_9D82_467B_91C1_DCF4B3D230DD__INCLUDED_)
//#define AFX_MCICARD_H__3E643262_9D82_467B_91C1_DCF4B3D230DD__INCLUDED_

//#if _MSC_VER > 1000
#pragma once
//#endif // _MSC_VER > 1000
// MCICard.h : header file
//

#include "DIBAPI.H"
#include "RectManager.h"
#include "MCICardParam.h"
//#include "CriticalResource.h"
#include <afxmt.h>
#include "MP.h"
#include "LabelControler.h"
#include "MobileUScaleControler.h"
#include "OscillographControler.h"
#include "FrameRateControler.h"
#include "HeaderControler.h"
#include "AngleControler.h"

#include <USB_Functions.h>

/////////////////////////////////////////////////////////////////////////////
// CMCICard thread

class CMCICard : public CWinThread
{
	DECLARE_DYNCREATE(CMCICard)
protected:
	BOOL				m_boolReady;

	DWORD				m_lastTickCount;

	HANDLE				m_hEventKill;
	HANDLE				m_hEventDead;

	CWnd*				m_pScreenWnd;
	CRect				m_rcScreenClientRect;
	CPoint				m_ptScreenClientPos;

	int					m_nusbFunction;
	BOOL				m_bSplit;
	BOOL				m_bLive;
	CRect				m_rcDestFrame;
	double				m_dblZoom;			// The map of Image to screen

	BOOL				m_boolShouldReset;

	BOOL				m_boolPreview;
	//06.11.22 3200与3900采集屏显示兼容
	BOOL				m_boolNewScanScreenMode;

	double				m_dblPixelsPerMM_HORZ, m_dblPixelsPerMM_VERT;

public:
	int					m_nChnIndex[2];		// 双通道显示方式时左右各显示哪个通道的数据。
	// 0：对应硬件通道组中的偶数通道的数据Za；
	// 1：对应硬件通道组中的奇数通道的数据Zb；
	// 2：两通道数据平均值作为显示数据(Za+Zb)/2

public:
	CMCICard();   // protected constructor used by dynamic creation

	BOOL	IsReady(){ return m_boolReady; };
	BOOL	IsShouldReset(){ return m_boolShouldReset; };

	HDIB	GetDib(){return m_hdibImage;};

	HDIB	CopyImageDib();
	BOOL	CopyGraphBmp( CBitmap& bmpGraphLay );
	BOOL	CopyScreenBmp( CBitmap& bmpScreen );
	// 2021.06 原始数据存储
	void	GetImageDataSrc( int nIndex, WORD* pData );

	//05.02.06
	void	SetPhotoPreview( BOOL boolPreview ){m_boolPreview = boolPreview; };
	BOOL    GetPhotoPreview(){return m_boolPreview;};
	void    CalculateClientWindowRect();
	//06.11.22 3200与3900采集屏显示兼容
	void	SetScanScreenMode( BOOL boolNewScanScreenMode = TRUE );

// Attributes
public:
	CSize	GetResolution();
	void	SetResolution(CSize szReso);

	void	SetFunction( USB_ScanType nFunction );

	int GetFunction()
	{
		return m_nusbFunction;
	}

	BOOL Det2nd()
	{
		return m_bDet2nd;
	}

	CLabelControler* GetLabelControler()
	{
		return m_pLabelControler;
	}

	CMobileUScaleControler* GetMobileUScaleControler()
	{
		return m_pMobileUScaleControler;
	}

	COscillographControler* GetOscillographControler()
	{
		return m_pOscillographControler;
	}

	CFrameRateControler* GetFrameRateControler()
	{
		return m_pFrameRateControler;
	}

	CHeaderControler* GetHeaderControler()
	{
		return m_pHeaderControler;
	}

	CAngleControler* GetAngleControler()
	{
		return m_pAngleControler;
	}

	CString  GetMobileUScaleText();

	void	SetSplit( BOOL bSplit );
	void	SetLiveMode( BOOL bLive );

	double	GetZoom(){ return m_dblZoom; };
	void	SetZoom(double dblZoom);

	CSize	GetSizes( int nIndex );
	void	SetDestFrame( CRect rcDestFrame );

	int		m_nCurrentY1, m_nCurrentY2;

	// 15.07.23 设置底标、标签、活动标尺等的标注颜色、标注字体、标注字体大小等
	void	SetColor( int nType, COLORREF color );
	void	SetFontName( int nType, CString strName );
	void	SetFontSize( int nType, int nSize );
	void	SetFontWeight( int nType, int nWeight );
	void	SetTransparent( int nTransparent );

private:
	void	SetImageWindow(CRect rcImageWindow);

    UINT					m_uBPP;
	BOOL					m_blColorFill;
    BOOL					m_blSrcColorkey;
    BOOL					m_blDestColorkey;

protected:
	LPDIRECTDRAW2			m_lpDD;					// DirectDraw object
	LPDIRECTDRAWSURFACE		m_lpDDSPrimary;			// DirectDraw primary surface
	LPDIRECTDRAWPALETTE		m_lpDDPalette;			// DirectDraw palette
	LPDIRECTDRAWCLIPPER 	m_lpClipper;			// clipper for primary

	LPDIRECTDRAWSURFACE		m_lpDDSPrimaryBackLay;	// Offscreen surface Primary Back Lay
	LPDIRECTDRAWSURFACE		m_lpDDSGraphBackLay;	// Offscreen surface Graph Back Lay

	LPDIRECTDRAWSURFACE		m_lpDDSImageLay;		// Offscreen surface Image Lay
	LPDIRECTDRAWSURFACE		m_lpDDSGraphLay;		// Offscreen surface Graph Lay
	
	DWORD					m_dwVidMemTotal;
	DWORD					m_dwFrameMemSizeUsed;

	HDIB					m_hdibImage;
	LPVOID					m_lpImageDibPlan;
	CPalette				m_paletteImage;

	static	CCriticalSection		m_mutexLayAll;
	static	CCriticalSection		m_mutexImageDib;
	static	CCriticalSection		m_mutexImageUSB;

	CLabelControler*        m_pLabelControler;                //Label控制器
	CMobileUScaleControler* m_pMobileUScaleControler;         //移动标尺控制器
	COscillographControler* m_pOscillographControler;         //示波器控制器
	CFrameRateControler*    m_pFrameRateControler;            //FrameRate控制器
	CHeaderControler*		m_pHeaderControler;
	CAngleControler*		m_pAngleControler;

	// Statistical infomation
	DWORD					m_dwByteCount;
    DWORD					m_dwBytesLast;
	DWORD					m_dwBytes;

	DWORD					m_dwInputCount;

// Operations
public:
	void	KillThread();

	BOOL	Reset( CWnd* pWnd );

	BOOL	Close();

	BOOL	IsSupportColorFill() {  return m_blColorFill; };

	DWORD	ImageDibWrite( LPVOID lpBuff, long lBuffSize, CRect& rcDest );
	DWORD	ImageDibWriteImp( LPVOID lpBuff, long lBuffSize, CRect& rcDest );			// 10.08
	DWORD	ImageDibWriteNewXYZ( LPVOID lpBuff, long lBuffSize, CRect& rcDest );		// 11.11.28
	DWORD	ImageDibWriteNewZZZ( LPVOID lpBuff, long lBuffSize, CRect& rcDest );		// 11.12.05
	DWORD	ImageDibWriteNewZZZ2( LPVOID lpBuff, long lBuffSize, CRect& rcDest );		// 11.12.05

	DWORD	ADxyz( LPVOID lpBuff, long lBuffSize, CRect& rcDest );
	DWORD	ADxyzLine( LPVOID lpBuff, long lBuffSize, CRect& rcDest );
	DWORD	ADxyzSync( LPVOID lpBuff, long lBuffSize, CRect& rcDest );

	DWORD	ADxyz2nd( LPVOID lpBuff, long lBuffSize, CRect& rcDest );
	DWORD	ADxyzLine2nd( LPVOID lpBuff, long lBuffSize, CRect& rcDest );
	DWORD	ADxyzSync2nd( LPVOID lpBuff, long lBuffSize, CRect& rcDest );

	DWORD	ADxyz2ndMix( LPVOID lpBuff, long lBuffSize, CRect& rcDest );
	DWORD	ADxyzLine2ndMix( LPVOID lpBuff, long lBuffSize, CRect& rcDest );
	DWORD	ADxyzSync2ndMix( LPVOID lpBuff, long lBuffSize, CRect& rcDest );

	DWORD	ADxyz2( LPVOID lpBuff, long lBuffSize, CRect& rcDest );
	DWORD	ADxyzLine2( LPVOID lpBuff, long lBuffSize, CRect& rcDest );
	DWORD	ADxyzSync2( LPVOID lpBuff, long lBuffSize, CRect& rcDest );

	BOOL	PostData( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow );
	BOOL	GetData( /*LPVOID lpBuff, long lBuffSize, */CRect& rcFrameDestWindow );

	DWORD	ImageDibRead( LPVOID lpBuff, long lBuffSize, CRect& rcDest );
	DWORD	ImageDibCopy( LPVOID lpBuff, long lBuffSize, CRect& rcDest );

	DWORD	PixConvertZZZ( unsigned short* pnFrameData, long lBytesTransferred, WORD* pPixX, WORD* pPixY, WORD* pPixZ, WORD& dwIndexY1, WORD& dwIndexY2 );
	DWORD	PixConvertZZZ2( unsigned short* pnFrameData, long lBytesTransferred, WORD* pPixX, WORD* pPixY, WORD* pPixZ, WORD& dwIndexY1, WORD& dwIndexY2 );
	void	CheckIndexs( int nLineCounts, int nFrameCounts, int& nX, int& nY );
	WORD	m_dwX, m_dwY, m_dwZa;
	int		m_nX, m_nY, m_nIndexX, m_nIndexY, m_nXOffset, m_nYOffset;
	BOOL	m_bPeakX, m_bPeakY;
	WORD	*m_pPixX, *m_pPixX2;
	WORD	*m_pPixY, *m_pPixY2;
	WORD	*m_pPixZ, *m_pPixZ2, *m_pPixZmix, *m_pPixZsrc, *m_pPixZsrc2, *m_pPixZOrigin;
	WORD	m_dwMinY, m_dwMaxY;

	BOOL	Convolution( double *Kernel, CSize KernelSize, int nX1, int nY1, int nX2, int nY2, LPSTR lpDIBBits );
	BOOL	LowpassFilter( DWORD *dwFact, int nX1, int nY1, int nX2, int nY2, LPSTR lpDIBBits );
	BOOL	Gauss(UINT uSize, int nX1, int nY1, int nX2, int nY2, LPSTR lpDIBBits);

	BOOL	InitData( DWORD dwExpectantDMABlockSize, int nXOffset = 0, int nYOffset = 0 );
	BOOL	ClearData();
	BOOL	m_bDataNeedUpdated;
	BOOL	m_bStopScan;
	unsigned short*		m_pBuffUSB;
	DWORD				m_dwExpectantDMABlockSize;
	BOOL	m_bSyncFinished;
	BOOL	m_bFindFirstBOF;
	HANDLE	m_hFileLog;
	BOOL	m_bNeedMon;
	HANDLE	m_hLineDataLog;

	void	SetSharedImageBuffIndex( int nIndex );
	int		m_nSharedImageBuffIndexRead, m_nSharedImageBuffIndexWrite;

	void	SetParams( int nType, int nParam );	// 设置数据存储模式，探测器模式
	int		m_nDataStoreMode;					// 数据存储模式
	int		m_nDetectorMode;					// 探测器模式
	BOOL	m_bDet2nd;
	BOOL	m_bDebugZ;							// 调试模式下，示波器数据意义不同
	int     m_nMaxPix;
	int	    m_nDiv;
	double	m_dSEratio;

	void	PhotoIndication( const CRect& rcDest, const COLORREF color = RGB(0,0,0), const BOOL boolFill = TRUE );//09.06.01

	BOOL	GraphDrawLine( const CPoint ptPos0, const CPoint ptPos1,
						const COLORREF color = RGB(0,0,0) );
	BOOL	GraphDrawLineEx( const int nLength );	// 单位：mm
	BOOL	GraphDrawRect( const CRect rcDest, const COLORREF color = RGB(0,0,0) );
	BOOL	GraphDrawRectEx( const BOOL bDrawCircle, const CRect rcDest, const COLORREF color = RGB(0,0,0) );
	BOOL	GraphDrawCircle( const CPoint ptCenter, const CPoint ptEnd, BOOL bDrawArrow = TRUE, 
						const int nRadius = nSTAGE_TRACE_POINT_RADIUS, const COLORREF color = RGB(0,0,0) );	// 12.10.11
	BOOL	GraphClear(	const CRect rcDest = CRect(0, 0, g_szUSB_FrameGraph.cx, g_szUSB_FrameGraph.cy),
						const COLORREF color = RGB(0,0,0) );
	CSize	GraphTextOut( const CPoint ptPos, const CString csText );
	CSize	GetOutputTextExtent( const CString csText );

	BOOL	ShowTime( BOOL boolShow = TRUE );
	BOOL	ShowNote( BOOL boolShow = TRUE );
	BOOL	ShowPixelSize( BOOL boolShow = TRUE );
	BOOL	ShowAngle( BOOL boolShow = TRUE );


	void	UpdateMAG(
			CString	csMag,					// 放大倍数显示字符
			int		nUScalePixelNum,		// 固定标尺显示长度
			CString	csUScal,				// 固定标尺显示字符
			CString	csMobileUScaleText );	// 移动标尺显示字符

	void	UpdateChnNames( int nIndex, CString csText );
	void	UpdateSGname( int nIndex, CString& strName );

	void	UpdateMAGArray(	int nIndex,
			CString	csMobileUScaleText );	// 移动标尺显示字符
	// 16.06.12 多移动标尺实验

	// 19.02.18 底标自定义
	void	UpdateFooter( int nIndex, BOOL bVisible, CPoint ptPos );
	void	UpdateFooterText( int nIndex, CString csText );
	// 19.02.18 底标自定义

	void	UpdateFOV( CString csFOV );

	BOOL	IsStopScan() {return m_bStopScan;}
	void	SetStopScan(BOOL bStop) {m_bStopScan = bStop;}

private:
	HDIB	CreateImageBitmap();
	void    _InitDibHeader( LPBITMAPINFO& pDibInfo, const DWORD dwBitsSize);
	void    _SetSize(const CSize &szReso,int nXWidth);

	void	MainLoop();

	void	GetClientWindow();
	void	PaintScreenWindow();

	long	UpdateImageLay();
	long	UpdateGraphLay(CLabelControler* pLabelControler, CHeaderControler* pHeaderControler);

	long	UpdateGraphBackLay();
	long	UpdatePriamryLay();

	CRect	LayWindowMapClip_Image_to_Priamry( CRect& rcImage );
	///////////////////////////////////////////////////////////////////

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCICard)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
public:
	virtual ~CMCICard();
	virtual void Delete();

	// Generated message map functions
	//{{AFX_MSG(CMCICard)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

//#endif // !defined(AFX_MCICARD_H__3E643262_9D82_467B_91C1_DCF4B3D230DD__INCLUDED_)
