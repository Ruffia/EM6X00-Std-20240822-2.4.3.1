#pragma once
#include <afxmt.h>
#include "USB_Functions.h"
#include "RectManager.h"

#define GRAPHIC_BLACK

//FrameRate控制器
class CFrameRateControler 
{
public:
	CFrameRateControler(CCriticalSection& cs,LPDIRECTDRAW2& lpDD,LPDIRECTDRAWSURFACE& lpDDSGraphBackLay);
	~CFrameRateControler();

	void    ReleaseLay();
	void    Close();
	BOOL    ShowFrameRate(BOOL boolShow);

	///////////////////////////////////////////////////////////////////
	// Statistical infomation
	///////////////////////////////////////////////////////////////////
	long	UpdateFrameRateLay(LPDIRECTDRAWSURFACE& lpDDSImageLay, BOOL bReady);
	long	FormatFrameRate(DWORD& dwBytes,DWORD& dwByteCount,DWORD& m_dwInputCount,DWORD& dwBytesLast);
	BOOL	InitFrameRate(DWORD& dwFrameMemSizeUsed,DWORD dwVidMemTotal);
	long	InitNumSurface();

public:

	BOOL					m_boolShowFrameRate;

	DWORD					m_dwFrameTime;

	DWORD					m_dwFrameCount;
	DWORD					m_dwFramesLast;
	DWORD					m_dwFrames;


	DWORD					m_dwAveFrameRate;
	DWORD					m_dwAveFrameRateCount;


	SIZE					m_sizeFPS;
	int						m_iFrameRateX;
	int						m_iBpsRateX;
	int						m_iIPSRateX;



	char					m_szFrameRate[64];
	char					m_szFPS[64];

	HFONT					m_hFontFrameRate;


	DWORD					m_dwInputsLast;
	DWORD					m_dwInputs;


	CSize	                m_szLabel;
	BOOL					m_boolRedrawLabel;

	BOOL					m_boolShowLabel;

	HFONT					m_hfontEditText;			// 2004-8-2_加强图形层显示

	HFONT					m_hfontLabelText;

	HFONT					m_hfontLabelLogoText;


	LPDIRECTDRAW2&			 m_lpDD;					// DirectDraw object
	LPDIRECTDRAWSURFACE&	 m_lpDDSGraphBackLay;	// Offscreen surface Graph Lay
	LPDIRECTDRAWSURFACE	     m_lpDDSFrameRate;		// Offscreen surface Info Lay
	CCriticalSection&        m_mutexLayAll;
};

