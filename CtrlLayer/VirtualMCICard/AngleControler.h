#pragma once
#include <afxmt.h>
#include "USB_Functions.h"
#include "RectManager.h"

class CLabelControler;
class COscillographControler;

//Ò³Ãæ±êÇ©¿ØÖÆÆ÷
class CAngleControler 
{
public:
	CAngleControler(CCriticalSection& cs,LPDIRECTDRAW2& lpDD,LPDIRECTDRAWSURFACE& lpDDSGraphBackLay);
	~CAngleControler();

	BOOL    ShowAngle( BOOL boolShow = TRUE );
	void    Close();
	void    ReleaseLay();

	void	ChangeAngle( int nIndex, BOOL bAdd );
	void	ReArrangeAngles();
	void	ResetAngles();
	void	SetAngleParam( int nIndex, AngleMeasure* pAM );

	CRect   LayWindowMapClip_Image_to_Angle( CRect& rcImage );

	BOOL	InitAngleLay(CLabelControler* pLabelControler,const CRect& rcScreenClientRect);
	long	UpdateAngleLay(CLabelControler* pLabelControler,COscillographControler* pOscillographControler,BOOL bReady);
	long    RedrawAngleLay(CLabelControler* pLabelControler, const CRect& rcScreenClientRect);

	BOOL    CreateSurface( BOOL boolUseSysMemAngleLay ); 

public:
	BOOL					m_boolRedrawAngle;
	BOOL					m_boolShowAngle;

	HFONT					m_hfontAngleText;

	CRectManager			m_UpdateRectsAngle;

	ListAngle				*m_plistAngle;
	int						m_nAngleCounts;

	LPDIRECTDRAW2&			 m_lpDD;					// DirectDraw object
	LPDIRECTDRAWSURFACE&	 m_lpDDSGraphBackLay;		// Offscreen surface Graph Lay
	LPDIRECTDRAWSURFACE		 m_lpDDSAngleLay;			// Offscreen surface Label Lay
	CCriticalSection&        m_mutexLayAll;
};

