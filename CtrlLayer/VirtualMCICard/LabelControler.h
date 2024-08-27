#pragma once
#include <afxmt.h>
#include "USB_Functions.h"
#include "RectManager.h"
class CMobileUScaleControler;
class CAngleControler;

#define GRAPHIC_BLACK

//页面标签控制器
class CLabelControler 
{
public:
	CLabelControler(CCriticalSection& cs,LPDIRECTDRAW2& lpDD,LPDIRECTDRAWSURFACE& lpDDSGraphBackLay);
	~CLabelControler();

	void    DrawLabel( HDC& hdc, COLORREF clrBack, int nOffsetX, BOOL bSplit, BOOL bDet2nd ); 
	BOOL    ShowLabel( BOOL boolShow = TRUE );
	void    Close();
	void    ReleaseLay();

	void	SetProductLogo( CString csLogo );

	void	UpdateHV( CString csHV );
	void	UpdateWD( CString csWD);
	void	UpdatePhotoSN( CString csPhotoSN );
	void	UpdateTime( CString csTime );
	void	UpdateNote( CString csNote );
	void	UpdatePixelSize(CString	csPixelSize);
	void	UpdateVac( CString csVac );
	CRect   LayWindowMapClip_Image_to_Label( CRect& rcImage );

	BOOL	InitLabelLay(BOOL bSplit,BOOL bDet2nd);
	long	UpdateLabelLay(CMobileUScaleControler* pMobileUScaleControler,CAngleControler* pAngleControler,BOOL bReady);
	long    RedrawLabelLay(BOOL bSplit, BOOL bDet2nd );

	BOOL    CreateSurface( BOOL boolUseSysMemLabelLay ); 

	void    UpdateMAG(
		CString		csMag,					// 放大倍数显示字符
		int			nUScalePixelNum,		// 固定标尺显示长度
		CString		csUScal);				// 固定标尺显示字符

public:
	CSize	                m_szLabel;
	BOOL					m_boolRedrawLabel;

	BOOL					m_boolShowLabel;

	HFONT					m_hfontEditText;			// 2004-8-2_加强图形层显示

	HFONT					m_hfontLabelText;

	HFONT					m_hfontLabelLogoText;

	CString					m_csHV;						// 高压显示字符
	CString					m_csMag;					// 放大倍数显示字符
	CString					m_csUScal;					// 固定标尺显示字符
	CString					m_csLogo;					// 产品标识显示字符
	CString					m_csPhotoSN;				// 照相编号显示字符
	CString					m_csTime;					// 日期时间显示字符
	CString					m_csWD;						// 工作距离显示字符
	CString					m_csSG1, m_csSG2, m_csDet[6];// 探测器类型显示字符
	CString					m_csNote;					// 备注显示字符
	CString					m_csPixelSize;
	CString					m_csVAC;					// 真空值（低真空值）显示字符

	int						m_nUScalePixelNum;			// 固定标尺显示长度
	///////////////////////////////////////////////////////////////////
	BOOL					m_boolShowTime;				// 日期时间显示开关
	BOOL					m_boolShowNote;				// 备注显示开关
	BOOL					m_boolShowPixelSize;		// PixelSize显示开关

	/////////////////////////////////////////////////////////////////////
	BOOL					m_boolShowMobileUScale;		// 移动标尺显示开关

	HFONT					m_hfontMobileUScaleText;

	CRectManager	m_UpdateRectsLabel;

	LPDIRECTDRAW2&			 m_lpDD;					// DirectDraw object
	LPDIRECTDRAWSURFACE&	 m_lpDDSGraphBackLay;	// Offscreen surface Graph Lay
	LPDIRECTDRAWSURFACE		 m_lpDDSLabelLay;		// Offscreen surface Label Lay
	CCriticalSection&        m_mutexLayAll;
};

