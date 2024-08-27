#pragma once
#include <afxmt.h>
#include "USB_Functions.h"
#include "RectManager.h"
class CMobileUScaleControler;

#define GRAPHIC_BLACK

//ҳ���ǩ������
class CHeaderControler 
{
public:
	CHeaderControler(CCriticalSection& cs,LPDIRECTDRAW2& lpDD,LPDIRECTDRAWSURFACE& lpDDSGraphBackLay);
	~CHeaderControler();

	BOOL    ShowHeader( BOOL boolShow = TRUE );
	void    Close();
	void    ReleaseLay();

	void	UpdateFOV( CString csFOV );
	CRect   LayWindowMapClip_Image_to_Header( CRect& rcImage );
	void    UpdateHeaderRect(); 

	BOOL	InitHeaderLay(BOOL bSplit);
//	long	UpdateHeaderLay(LPDIRECTDRAWSURFACE& lpDDSImageLay, BOOL bReady);
	long	UpdateHeaderLay(CMobileUScaleControler* pMobileUScaleControler, BOOL bReady);
	long    RedrawHeaderLay(BOOL bSplit);

	BOOL    CreateSurface( BOOL boolUseSysMemLabelLay ); 

public:
	CSize	                m_szHeader;

	CPoint					m_ptFOV;
	CString					m_csFOV;					// �ӳ���С��ʾ�ַ�

	BOOL					m_boolRedrawHeader;
	BOOL					m_boolShowHeader;
	BOOL					m_boolShowFOV;				// �ӳ���С��ʾ����

	HFONT					m_hfontHeaderText;

	CRectManager			m_UpdateRectsHeader;

	LPDIRECTDRAW2&			 m_lpDD;					// DirectDraw object
	LPDIRECTDRAWSURFACE&	 m_lpDDSGraphBackLay;		// Offscreen surface Graph Lay
	LPDIRECTDRAWSURFACE		 m_lpDDSHeaderLay;			// Offscreen surface Label Lay
	CCriticalSection&        m_mutexLayAll;
};

