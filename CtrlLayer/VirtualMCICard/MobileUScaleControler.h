#pragma once
#include <afxmt.h>
#include "USB_Functions.h"
#include "RectManager.h"

#define	NEW_SCANSCREEN_MODE

class CLabelControler;
class COscillographControler;

//�ƶ���߿�����
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
	//void	SetMobileUScalePos( CPoint ptMobileUScale );  --�����ѱ�����

	//07.08.20���ⷽ���ƶ��������
	void	GetMobileUScaleParamEx( int&	nMobileUScalePixelNum, CPoint& ptMobileUScale, CPoint& ptMobileUScale2 );
	void    SetMobileUScalePosEx(const CPoint ptMobileUScale, const CPoint ptMobileUScale2, const COLORREF color );

	//07.08.20���ⷽ���ƶ��������

	// 16.06.12 ���ƶ����ʵ��
	void	GetMobileUScaleParamArray( int nIndex, int&	nMobileUScalePixelNum, CPoint& ptMobileUScale, CPoint& ptMobileUScale2 );
	void	SetMobileUScalePosArray( int nIndex, const CPoint ptMobileUScale, const CPoint ptMobileUScale2,const COLORREF color = RGB(0,0,0) );
	void	UpdateUScaleArray(int	nIndex, int	nMobileUScalePixelNum, CString	csMobileUScaleText );	// �ƶ������ʾ�ַ�

	CRect	LayWindowMapClip_Image_to_MobileUScale( CRect& rcImage);

	void    UpdateUScaleRect(CLabelControler* pLabelControler);



	BOOL    InitMobileUScaleLay(CLabelControler* pLabelControler,const CRect& rcScreenClientRect);

	long	UpdateUScaleMobileLay(CLabelControler* pLabelControler,COscillographControler* pOscillographControler,BOOL bReady);

	long    RedrawMobileUScaleLay(CLabelControler* pLabelControler, const CRect& rcScreenClientRect);

	BOOL    CreateSurface(BOOL boolUseSysMemUScaleLay); 

	void    UpdateMAG(CString csMobileUScaleText);	// �ƶ������ʾ�ַ�

	CString  GetMobileUScaleText() {return m_csMobileUScaleText;}

public:
	int						m_nMobileUScalePixelNum;	// �ƶ������ʾ����
	CString					m_csMobileUScaleText;		// �ƶ������ʾ�ַ�
	CPoint					m_ptMobileUScaleText;		// �ƶ�����ַ���ʾλ��
	CPoint					m_ptMobileUScale;			// �ƶ����λ��

	// 07.08.20���ⷽ���ƶ��������
	CPoint					m_ptMobileUScale2;			// �ƶ����λ��
	CPoint					m_ptMobileUScaleSaved;
	CPoint					m_ptMobileUScale2Saved;
	// 07.08.20���ⷽ���ƶ��������

	// 16.06.12 ���ƶ����ʵ��
	CPoint					m_ptMobileUScaleArray[MobileUScaleCounts];
	CPoint					m_ptMobileUScaleArray2[MobileUScaleCounts];
	int						m_nMobileUScalePixelNumArray[MobileUScaleCounts];
	CString					m_csMobileUScaleTextArray[MobileUScaleCounts];
	// 16.06.12 ���ƶ����ʵ��
	///////////////////////////////////////////////////////////////////

	BOOL					m_boolRedrawUScal;

	CSize  m_szUScale;

	CRectManager	m_UpdateRectsMobileUScale;

    LPDIRECTDRAW2&			m_lpDD;					// DirectDraw object
	LPDIRECTDRAWSURFACE&	m_lpDDSGraphBackLay;	// Offscreen surface Graph Lay
	LPDIRECTDRAWSURFACE		m_lpDDSMobileUScaleLay;	// Offscreen surface MobileUScale Lay

	CCriticalSection&        m_mutexLayAll;
};

