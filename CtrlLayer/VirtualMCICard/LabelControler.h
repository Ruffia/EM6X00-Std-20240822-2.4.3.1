#pragma once
#include <afxmt.h>
#include "USB_Functions.h"
#include "RectManager.h"
class CMobileUScaleControler;
class CAngleControler;

#define GRAPHIC_BLACK

//ҳ���ǩ������
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
		CString		csMag,					// �Ŵ�����ʾ�ַ�
		int			nUScalePixelNum,		// �̶������ʾ����
		CString		csUScal);				// �̶������ʾ�ַ�

public:
	CSize	                m_szLabel;
	BOOL					m_boolRedrawLabel;

	BOOL					m_boolShowLabel;

	HFONT					m_hfontEditText;			// 2004-8-2_��ǿͼ�β���ʾ

	HFONT					m_hfontLabelText;

	HFONT					m_hfontLabelLogoText;

	CString					m_csHV;						// ��ѹ��ʾ�ַ�
	CString					m_csMag;					// �Ŵ�����ʾ�ַ�
	CString					m_csUScal;					// �̶������ʾ�ַ�
	CString					m_csLogo;					// ��Ʒ��ʶ��ʾ�ַ�
	CString					m_csPhotoSN;				// ��������ʾ�ַ�
	CString					m_csTime;					// ����ʱ����ʾ�ַ�
	CString					m_csWD;						// ����������ʾ�ַ�
	CString					m_csSG1, m_csSG2, m_csDet[6];// ̽����������ʾ�ַ�
	CString					m_csNote;					// ��ע��ʾ�ַ�
	CString					m_csPixelSize;
	CString					m_csVAC;					// ���ֵ�������ֵ����ʾ�ַ�

	int						m_nUScalePixelNum;			// �̶������ʾ����
	///////////////////////////////////////////////////////////////////
	BOOL					m_boolShowTime;				// ����ʱ����ʾ����
	BOOL					m_boolShowNote;				// ��ע��ʾ����
	BOOL					m_boolShowPixelSize;		// PixelSize��ʾ����

	/////////////////////////////////////////////////////////////////////
	BOOL					m_boolShowMobileUScale;		// �ƶ������ʾ����

	HFONT					m_hfontMobileUScaleText;

	CRectManager	m_UpdateRectsLabel;

	LPDIRECTDRAW2&			 m_lpDD;					// DirectDraw object
	LPDIRECTDRAWSURFACE&	 m_lpDDSGraphBackLay;	// Offscreen surface Graph Lay
	LPDIRECTDRAWSURFACE		 m_lpDDSLabelLay;		// Offscreen surface Label Lay
	CCriticalSection&        m_mutexLayAll;
};

