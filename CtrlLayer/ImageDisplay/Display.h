#if !defined(AFX_DISPLAY_H__D9B2B06D_AB8B_4F42_A1F6_8218BF488443__INCLUDED_)
#define AFX_DISPLAY_H__D9B2B06D_AB8B_4F42_A1F6_8218BF488443__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Display.h : header file
//

#include "DIBAPI.H"
#include "RectManager.h"

//_TB _TN _NB _NN

//#define GRAPHIC_Transparent
#define GRAPHIC_BLACK

// 07.10.31显示模式
#define	NEW_SCANSCREEN_MODE

#ifdef GRAPHIC_BLACK
const int	c_fnWeight	= FW_BLACK;		// font weight
#else
const int	c_fnWeight	= FW_NORMAL;	// font weight
#endif

const int		Edit_nFontSize			= 32;
const CString	Edit_csFontFacename		= _T("黑体");//_T("宋体");

const int		Label_nFontSize			= 40;	//32
const CString	Label_csFontFacename	= _T("黑体");//_T("宋体");

const int		Logo_nFontSize			= 40;	//32
const CString	Logo_csFontFacename		= _T("黑体");//_T("Times New Roman");

const int		UScale_nFontSize		= 32;
const CString	UScale_csFontFacename	= _T("黑体");//_T("宋体");

// 1 inch = 25.4 mm
const double mm_TO_inch				= (double)0.0393700787401574803149606299212598;
const double SEM_dfStandImageWidth	= (double)120;		// mm
const double SEM_dfStandImageHeight	= (double)90;		// mm


/////////////////////////////////////////////////////////////////////////////
// CDisplay thread

class CDisplay : public CWinThread
{
	DECLARE_DYNCREATE(CDisplay)
protected:
	///////////////////////////////////////////////////////////////////
	BOOL				m_boolReady;
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	DWORD				m_lastTickCount;
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	HANDLE				m_hEventKill;
	HANDLE				m_hEventDead;
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	CWnd*				m_pScreenWnd;
	CRect				m_rcScreenClientRect;
	CPoint				m_ptScreenClientPos;
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	CSize				m_sizeReso;
	int					m_nusbFunction;
//	BOOL				m_bSplit;
	CRect				m_rcImageWindow;	// Graphic Frame map to ImageWindow
	CRect				m_rcDestFrame;
	double				m_dblZoom;			// The map of Image to screen

	double				m_dblScaleImageToGraph_W;
	double				m_dblScaleImageToGraph_H;
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	BOOL				m_boolShouldReset;
	///////////////////////////////////////////////////////////////////

	BOOL				m_boolPreview;
	//06.11.22 3200与3900采集屏显示兼容
	BOOL				m_boolNewScanScreenMode;

	double				m_dblPixelsPerMM_HORZ, m_dblPixelsPerMM_VERT;

//	unsigned short*		m_pSharedImageBuff[SharedImageBuffCounts];

// Attributes
public:

// Operations
public:
	CDisplay();           // protected constructor used by dynamic creation
	BOOL	IsReady(){ return m_boolReady; };
	BOOL	IsShouldReset(){ return m_boolShouldReset; };

	HDIB	GetDib(){return m_hdibImage;};
	HDIB	CopyImageDib();

	CSize	GetResolution(){ return m_sizeReso; };
	void	SetResolution(CSize szReso);
	void	SetFunction( int nFunction );
//	void	SetSplit( BOOL bSplit );

	double	GetZoom(){ return m_dblZoom; };
	void	SetZoom(double dblZoom);

	CSize	GetSizes( int nIndex );
	void	SetDestFrame( CRect rcDestFrame );

//	void	GetSizes( CSize &szImage, CSize &szGraph, CSize &szLabel, CSize &szUScale, CSize &szOscill );
	CSize	m_szImage, m_szGraph;//, m_szLabel, m_szUScale, m_szOscill;
	int		m_nCurrentY1, m_nCurrentY2;

/*	/////////////////////////////////////////////////////////////////
	// 15.07.23 设置底标、标签、活动标尺等的标注颜色、标注字体、标注字体大小等
	void	SetColor( int nType, COLORREF color );
// 	COLORREF				m_colorTransparent;
// 	COLORREF				m_colorBack;
// 	COLORREF				m_colorEditText;
// 	COLORREF				m_colorLabelText;
// 	COLORREF				m_colorLabelLogoText;
// 	COLORREF				m_colorMobileUScaleText;
//	COLORREF				m_colorMobileUScaleLine;
// 	COLORREF				m_colorOscillographGrid;
// 	COLORREF				m_colorOscillographGraph;
// 	COLORREF				m_crFrameRateColor;
	void	SetFontName( int nType, CString strName );
	void	SetFontSize( int nType, int nSize );
	void	SetFontWeight( int nType, int nWeight );
	/////////////////////////////////////////////////////////////////
*/
private:
	void	SetImageWindow(CRect rcImageWindow);

	//////////////////////////////////////////
    UINT					m_uBPP;
	BOOL					m_blColorFill;
    BOOL					m_blSrcColorkey;
    BOOL					m_blDestColorkey;
	//////////////////////////////////////////

protected:
	///////////////////////////////////////////////////////////////////
	LPDIRECTDRAW2			m_lpDD;					// DirectDraw object
	LPDIRECTDRAWSURFACE		m_lpDDSPrimary;			// DirectDraw primary surface
	LPDIRECTDRAWPALETTE		m_lpDDPalette;			// DirectDraw palette
	LPDIRECTDRAWCLIPPER 	m_lpClipper;			// clipper for primary

	LPDIRECTDRAWSURFACE		m_lpDDSPrimaryBackLay;	// Offscreen surface Primary Back Lay
	LPDIRECTDRAWSURFACE		m_lpDDSGraphBackLay;	// Offscreen surface Graph Back Lay

	LPDIRECTDRAWSURFACE		m_lpDDSImageLay;		// Offscreen surface Image Lay
	LPDIRECTDRAWSURFACE		m_lpDDSGraphLay;		// Offscreen surface Graph Lay
// 	LPDIRECTDRAWSURFACE		m_lpDDSLabelLay;		// Offscreen surface Label Lay
// 	LPDIRECTDRAWSURFACE		m_lpDDSMobileUScaleLay;	// Offscreen surface MobileUScale Lay
// 	LPDIRECTDRAWSURFACE		m_lpDDSOscillographLay;	// Offscreen surface Oscillograph Lay
// 	LPDIRECTDRAWSURFACE		m_lpDDSFrameRate;		// Offscreen surface Info Lay
	
	DWORD					m_dwVidMemTotal;
	DWORD					m_dwFrameMemSizeUsed;

// 	COLORREF				m_colorTransparent;
// 	COLORREF				m_colorBack;
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	HDIB					m_hdibImage;
	LPVOID					m_lpImageDibPlan;
	CPalette				m_paletteImage;
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
//	CriticalResource		m_mutexLayAll;
//	CriticalResource		m_mutexImageDib;
//	CriticalResource		m_mutexImageUSB;
	static	CCriticalSection		m_mutexLayAll;
	static	CCriticalSection		m_mutexImageDib;
//	static	CCriticalSection		m_mutexImageUSB;
	///////////////////////////////////////////////////////////////////
	
/*	///////////////////////////////////////////////////////////////////
	// Oscillograph
	///////////////////////////////////////////////////////////////////
    BOOL					m_boolRedrawOscillograph;
    BOOL					m_boolShowOscillograph;

// 	COLORREF				m_colorOscillographGrid;
// 	COLORREF				m_colorOscillographGraph;
	
	CPoint					m_ptOscillograph;

	WORD					m_bOscillographBuff[8192];
	WORD					m_bOscillographData[8192];	// 示波器画图数据
	double					m_dblOscillographAmplifer;

	int						m_lOscillographState;
	long					m_lGrayHistogramBuff[256];
	///////////////////////////////////////////////////////////////////
	DWORD					m_dwFramGrayHistogramBuff[256];
	DWORD					m_dwFramGrayHistogramCountPixel;
	long					m_lFramGrayHistogramCountLine;
	long					m_lFramGrayHistogramCurrentLine;
	long					m_lFramGrayHistogramStartLine;
	BOOL					m_boolFramGrayHistogramBuffReady;
	BOOL					m_boolFramGrayHistogramBuffStart;
	///////////////////////////////////////////////////////////////////
*/
/*	SYS_Options				m_sysOpt;
	///////////////////////////////////////////////////////////////////
	// Label & uScale
	///////////////////////////////////////////////////////////////////
	BOOL					m_boolRedrawLabel;
	BOOL					m_boolRedrawUScal;

	BOOL					m_boolShowLabel;

	HFONT					m_hfontEditText;			// 2004-8-2_加强图形层显示
	COLORREF				m_colorEditText;			// 2004-8-2_加强图形层显示
	CString					m_fontEditTextName;
	int						m_fontEditTextSize;

	HFONT					m_hfontLabelText;
	COLORREF				m_colorLabelText;
	CString					m_fontLabelTextName;
	int						m_fontLabelTextSize;

	HFONT					m_hfontLabelLogoText;
	COLORREF				m_colorLabelLogoText;
*/
/* 	CString					m_csHV;						// 高压显示字符
	CString					m_csMag;					// 放大倍数显示字符
	CString					m_csUScal;					// 固定标尺显示字符
	CString					m_csLogo;					// 产品标识显示字符
	CString					m_csPhotoSN;				// 照相编号显示字符

	CPoint					m_ptHV;						// 高压字符显示位置
	CPoint					m_ptMag;					// 放大倍数字符显示位置
	CPoint					m_ptUScal;					// 固定标尺字符显示位置
	CPoint					m_ptLogo;					// 产品标识字符显示位置
	CPoint					m_ptPhotoSN;				// 照相编号字符显示位置
	///////////////////////////////////////////////////////////////////
	int						m_nUScalePixelNum;			// 固定标尺显示长度
	///////////////////////////////////////////////////////////////////
	BOOL					m_boolShowMobileUScale;		// 移动标尺显示开关

	HFONT					m_hfontMobileUScaleText;
	COLORREF				m_colorMobileUScaleText;
	COLORREF				m_colorMobileUScaleLine;
	CString					m_fontMobileUScaleName;
	int						m_fontMobileUScaleSize;

	int						m_nMobileUScalePixelNum;	// 移动标尺显示长度

	CString					m_csMobileUScaleText;		// 移动标尺显示字符
	CPoint					m_ptMobileUScaleText;		// 移动标尺字符显示位置

	CPoint					m_ptMobileUScale;			// 移动标尺位置

	//07.08.20任意方向移动标尺试验
	CPoint					m_ptMobileUScale2;			// 移动标尺位置
	CPoint					m_ptMobileUScaleSaved;
	CPoint					m_ptMobileUScale2Saved;
	//07.08.20任意方向移动标尺试验
	///////////////////////////////////////////////////////////////////
*/
/*	///////////////////////////////////////////////////////////////////
	// Statistical infomation
	///////////////////////////////////////////////////////////////////
    BOOL					m_boolShowFrameRate;

    DWORD					m_dwFrameTime;

	DWORD					m_dwFrameCount;
    DWORD					m_dwFramesLast;
	DWORD					m_dwFrames;
*/
	DWORD					m_dwByteCount;
	DWORD					m_dwBytesLast;
	DWORD					m_dwBytes;
/*
	DWORD					m_dwAveFrameRate;
	DWORD					m_dwAveFrameRateCount;

	SIZE					m_sizeFPS;
	int						m_iFrameRateX;
	int						m_iBpsRateX;
	int						m_iIPSRateX;
*/
	DWORD					m_dwInputCount;
    DWORD					m_dwInputsLast;
	DWORD					m_dwInputs;

/*	char					m_szFrameRate[64];
	char					m_szFPS[64];

	HFONT					m_hFontFrameRate;
// 	COLORREF				m_crFrameRateColor;
*/	///////////////////////////////////////////////////////////////////

public:
	void	KillThread();

	BOOL	Reset( CWnd* pWnd );
	BOOL	Close();

	///////////////////////////////////////////////////////////////////
	BOOL	IsSupportColorFill() {  return m_blColorFill; };
	///////////////////////////////////////////////////////////////////

	DWORD	ImageDibWrite( LPVOID lpBuff, long lBuffSize, CRect& rcDest );
	DWORD	ImageDibRead( LPVOID lpBuff, long lBuffSize, CRect& rcDest );
	DWORD	ImageDibCopy( LPVOID lpBuff, long lBuffSize, CRect& rcDest );

private:
	HDIB	CreateImageBitmap();

	void	MainLoop();

	void	GetClientWindow();
	void	PaintScreenWindow();

	long	UpdateImageLay();
//	long	UpdateGraphLay();
// 	long	UpdateLabelLay();
// 	long	UpdateUScaleMobileLay();

	long	UpdateGraphBackLay();
	long	UpdatePriamryLay();

	///////////////////////////////////////////////////////////////////
	CRectManager	m_UpdateRectsImage;
	CRectManager	m_UpdateRectsGraph;
	CRectManager	m_UpdateRectsGraphBack;

	CRect	LayWindowMapClip_Image_to_Priamry( CRect& rcImage );
	CRect	LayWindowMapClip_Image_to_Graph( CRect& rcImage );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplay)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
public:
	virtual ~CDisplay();
	virtual void Delete();

	// Generated message map functions
	//{{AFX_MSG(CDisplay)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAY_H__D9B2B06D_AB8B_4F42_A1F6_8218BF488443__INCLUDED_)
