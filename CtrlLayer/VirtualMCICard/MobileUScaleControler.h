#pragma once
#include <afxmt.h>
#include "USB_Functions.h"
#include "RectManager.h"

#define	NEW_SCANSCREEN_MODE

class CLabelControler;
class COscillographControler;

//移动标尺控制器
class CMobileUScaleControler 
{
public:
	CMobileUScaleControler(CCriticalSection& cs,LPDIRECTDRAW2& lpDD,LPDIRECTDRAWSURFACE& lpDDSGraphBackLay);
	~CMobileUScaleControler();

	void Initialize();
	void ReleaseLay();
	void ConvertPtArray( CPoint* ptArray1, CPoint* ptArray2, CString* strArray ); 

	void	UpdateUScale(int nMobileUScalePixelNum, CString	csMobileUScaleText );
	BOOL	ShowMobileUScale(CLabelControler* pLabelControler, BOOL boolShow = TRUE );

	void	GetMobileUScaleParam( int&	nMobileUScalePixelNum, CPoint& ptMobileUScale );
	//void	SetMobileUScalePos( CPoint ptMobileUScale );  --函数已被弃用

	//07.08.20任意方向移动标尺试验
	void	GetMobileUScaleParamEx( int&	nMobileUScalePixelNum, CPoint& ptMobileUScale, CPoint& ptMobileUScale2 );
	void    SetMobileUScalePosEx(const CPoint ptMobileUScale, const CPoint ptMobileUScale2, const COLORREF color );

	//07.08.20任意方向移动标尺试验

	// 16.06.12 多移动标尺实验
	void	GetMobileUScaleParamArray( int nIndex, int&	nMobileUScalePixelNum, CPoint& ptMobileUScale, CPoint& ptMobileUScale2 );
	void	SetMobileUScalePosArray( int nIndex, const CPoint ptMobileUScale, const CPoint ptMobileUScale2,const COLORREF color = RGB(0,0,0) );
	void	UpdateUScaleArray(int	nIndex, int	nMobileUScalePixelNum, CString	csMobileUScaleText );	// 移动标尺显示字符

	CRect	LayWindowMapClip_Image_to_MobileUScale( CRect& rcImage);

	void    UpdateUScaleRect(CLabelControler* pLabelControler);



	BOOL    InitMobileUScaleLay(CLabelControler* pLabelControler,const CRect& rcScreenClientRect);

	long	UpdateUScaleMobileLay(CLabelControler* pLabelControler,COscillographControler* pOscillographControler,BOOL bReady);

	long    RedrawMobileUScaleLay(CLabelControler* pLabelControler, const CRect& rcScreenClientRect);

	BOOL    CreateSurface(BOOL boolUseSysMemUScaleLay); 

	void    UpdateMAG(CString csMobileUScaleText);	// 移动标尺显示字符

	CString  GetMobileUScaleText() {return m_csMobileUScaleText;}

public:
	int						m_nMobileUScalePixelNum;	// 移动标尺显示长度
	CString					m_csMobileUScaleText;		// 移动标尺显示字符
	CPoint					m_ptMobileUScaleText;		// 移动标尺字符显示位置
	CPoint					m_ptMobileUScale;			// 移动标尺位置

	// 07.08.20任意方向移动标尺试验
	CPoint					m_ptMobileUScale2;			// 移动标尺位置
	CPoint					m_ptMobileUScaleSaved;
	CPoint					m_ptMobileUScale2Saved;
	// 07.08.20任意方向移动标尺试验

	// 16.06.12 多移动标尺实验
	CPoint					m_ptMobileUScaleArray[MobileUScaleCounts];
	CPoint					m_ptMobileUScaleArray2[MobileUScaleCounts];
	int						m_nMobileUScalePixelNumArray[MobileUScaleCounts];
	CString					m_csMobileUScaleTextArray[MobileUScaleCounts];
	// 16.06.12 多移动标尺实验
	///////////////////////////////////////////////////////////////////

	BOOL					m_boolRedrawUScal;

	CSize  m_szUScale;

	CRectManager	m_UpdateRectsMobileUScale;

    LPDIRECTDRAW2&			m_lpDD;					// DirectDraw object
	LPDIRECTDRAWSURFACE&	m_lpDDSGraphBackLay;	// Offscreen surface Graph Lay
	LPDIRECTDRAWSURFACE		m_lpDDSMobileUScaleLay;	// Offscreen surface MobileUScale Lay

	CCriticalSection&        m_mutexLayAll;
};

