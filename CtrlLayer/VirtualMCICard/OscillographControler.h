#pragma once
#include <afxmt.h>
#include "USB_Functions.h"
#include "MCICardParam.h"
#include "RectManager.h"

typedef struct tagDebug_OSC_MCI
{
	BOOL	b90;					// X/Y互换
	int		nIndexY;				// 调试模式下，视频示波器Y向档位
	double	dOffset;				// 示波器Y向偏移量
}DebugOSCMCI;

//示波器相关功能控制器
class COscillographControler 
{
public:
	COscillographControler(CCriticalSection& cs,LPDIRECTDRAW2& lpDD,LPDIRECTDRAWSURFACE& lpDDSGraphBackLay);
	~COscillographControler();

	BOOL    InitData(CString str);

	void    ReleaseLay();

	void    CalculateOscillographSize(CSize &szReso);

	void    CloseLogHandle();

	BOOL	ShowOscillograph(BOOL boolShow = TRUE);
	void	SetOscillographPos( CPoint ptOscillograph );
	void	GetOscillographParam( CPoint& ptOscillograph );
	void	UpdateOscillograph( LPBYTE pOscillographBuff, long nScanLine, long nStart, long nStop,int nDetectorMode,CSize&  szImage);
	void	UpdateOscillographNew(	DWORD dwCounts, LPWORD pPixX, LPWORD pPixY, LPWORD pPixZ,
		long		nScanLine,			// 当前扫描线
		long nStart, long nStop,
		bool        bDebugZ,
		int         nDataStoreMode);
	void	SetOscillographAmplifer(double dblAmplifer = 1.0);

	BOOL	IsReadyFramGrayHistogram(){ return m_boolFramGrayHistogramBuffReady; };
	void	FramGrayHistogramStart();
	void	FramGrayHistogramStop();
	void	FramGrayHistogramReset();
	DWORD	CopyFramGrayHistogramBuff( DWORD* pFramGrayHistogramBuff );

	void UpdateGrayHistogram(
		LPBYTE		pVideoDataBuff,		// 视频数据
		long		nBuffLength,
		int     nDetectorMode,
		CSize&  szImage);

	void	UpdateFramGrayHistogram(LPBYTE	pVideoDataBuff, long nBuffLength, long lCurrentLine);
	void	UpdateGrayHistogramNew(LPWORD	pVideoDataBuff, long nBuffLength, bool   bDebugZ);
	void	UpdateFramGrayHistogramNew(
		DWORD dwCounts, LPWORD pPixX, LPWORD pPixY, LPWORD pPixZ );

	BOOL  GetShowOscillograph(){return m_boolShowOscillograph;}

	BOOL NeedLog(){return m_bNeedMon;}
	void SetNeedLog(BOOL b){m_bNeedMon = b;}

	CRect LayWindowMapClip_Image_to_Oscillograph( CRect& rcImage );

	void    UpdateOscillographRect(); 

	// Oscillograph
	///////////////////////////////////////////////////////////////////
	BOOL	InitOscillographLay(DWORD dwFrameMemSizeUsed,DWORD dwVidMemTotal,BOOL blSrcColorkey,BOOL bDebugZ);
	long	RedrawOscillographLay(BOOL bDebugZ);
	long	UpdateOscillographLay(BOOL bReady);

private:
	BOOL    _InitOscillographLay();

public:
	///////////////////////////////////////////////////////////////////
	// Oscillograph
	///////////////////////////////////////////////////////////////////
	BOOL					m_boolRedrawOscillograph;
	BOOL					m_boolShowOscillograph;

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

	HANDLE	m_hOscillLog;

	BOOL	m_bDebugOsc;
	DebugOSCMCI	m_debugOSC;

	CRectManager	m_UpdateRectsOscillograph;

	CSize	m_szOscill;
	LPDIRECTDRAW2&			m_lpDD;					// DirectDraw object
	LPDIRECTDRAWSURFACE&	m_lpDDSGraphBackLay;	// Offscreen surface Graph Lay
	LPDIRECTDRAWSURFACE		m_lpDDSOscillographLay;	// Offscreen surface Oscillograph Lay
	CCriticalSection&       m_mutexLayAll;

private:
	BOOL	m_bNeedMon;

};

