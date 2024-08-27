#include "stdafx.h"
#include "HeaderControler.h"
#include "RectConvertor.h"
#include "MCICardParam.h"
#include "MCICardOption.h"
#include "MCICardCommonSetting.h"
#include "MobileUScaleControler.h"

CHeaderControler::CHeaderControler(CCriticalSection& cs,LPDIRECTDRAW2& lpDD,LPDIRECTDRAWSURFACE& lpDDSGraphBackLay):
	m_mutexLayAll(cs),m_lpDD(lpDD),m_lpDDSGraphBackLay(lpDDSGraphBackLay)
{
	m_lpDDSHeaderLay		= NULL;		// Offscreen surface Header Lay

	m_boolRedrawHeader		= FALSE;
	m_boolShowHeader		= TRUE;
	m_boolShowFOV			= TRUE;

	m_hfontHeaderText		= NULL;

	m_lpDDSHeaderLay		= NULL;		// Offscreen surface Header Lay

	m_csFOV					= _T("xxxx.xx nm | xxxx.xx nm");
	m_szHeader				= CSize(1024, 8);
}


CHeaderControler::~CHeaderControler()
{
}

void CHeaderControler::ReleaseLay()
{
	if(m_lpDDSHeaderLay != NULL )
	{
		m_lpDDSHeaderLay->Release();
		m_lpDDSHeaderLay = NULL;
	}
}

void CHeaderControler::Close()
{
	if (m_hfontHeaderText != NULL)
	{
		DeleteObject(m_hfontHeaderText);
		m_hfontHeaderText = NULL;
	}
}

BOOL CHeaderControler::ShowHeader(BOOL boolShow)
{
	if ( m_boolShowHeader == boolShow )
		return m_boolShowHeader;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	CRectManager& m_UpdateRectsImage = CMCICardCommonSetting::Instance().m_UpdateRectsImage;
	CSize& m_szGraph = CMCICardCommonSetting::Instance().m_szGraph;

	CRect rcGrapic = CRect(0, 0, m_szHeader.cx, m_szHeader.cy);
	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

	if ( m_boolShowHeader == TRUE )
	{	
		m_UpdateRectsImage.Add( rcImage );
	}
	else
	{
		m_UpdateRectsHeader.Add( rcImage );
	}

	m_boolShowHeader = boolShow;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////

	return m_boolShowHeader;
}

void CHeaderControler::UpdateFOV(CString	csFOV)
{	
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csFOV = csFOV;
	m_boolRedrawHeader = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

CRect CHeaderControler::LayWindowMapClip_Image_to_Header( CRect& rcImage )
{
	CRect	rcGrapic = LayWindowMapClip_Image_to_Graph( rcImage);

	CRect	rcHeader = rcGrapic;
	rcHeader &= CRect(0, 0, m_szHeader.cx, m_szHeader.cy);

	if( !rcHeader.IsRectEmpty() )
	{
		rcGrapic = rcHeader;
		rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );
	}

	return rcHeader;
}

long CHeaderControler::RedrawHeaderLay( BOOL bSplit )
{
	HDC				hdc;
	HRESULT			ddrval;

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSHeaderLay == NULL ) return DD_OK;
	ddrval = m_lpDDSHeaderLay->GetDC( &hdc );
	if( ddrval != DD_OK )
		return ddrval;
	/////////////////////////////////////////////////////////////////

	CSize& m_sizeReso = CMCICardCommonSetting::Instance().m_sizeReso;
	CRectManager& m_UpdateRectsImage = CMCICardCommonSetting::Instance().m_UpdateRectsImage;
	CSize& m_szGraph = CMCICardCommonSetting::Instance().m_szGraph;

	/////////////////////////////////////////////////////////////////
	COLORREF clrBack;
	if( CMCICardOption::Instance().m_sysOpt.nShowParams[SysOpt_showLabelTrans] )
		clrBack = g_clrUSB_Transparent;
	else
		clrBack = CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrBack];
	HBRUSH brBackBrush = CreateSolidBrush( clrBack );
	CRect rcFill = CRect(0,0,m_szHeader.cx,m_szHeader.cy);
	FillRect( hdc, (LPRECT)rcFill, brBackBrush );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	int nOffsetX;
	if( bSplit )
		nOffsetX	= m_szGraph.cx /2;
	else
		nOffsetX	= 0;
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
//	SetTextAlign(hdc, TA_TOP | TA_LEFT);
	if(m_hfontHeaderText != NULL)
	{
		CString str;
		SelectObject(hdc, m_hfontHeaderText);
		SetTextColor(hdc, CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrLabelText]);
		SetBkColor(hdc, clrBack);

		CSize sz;
		GetTextExtentPoint(hdc, m_csFOV, m_csFOV.GetLength(), &sz);
		m_ptFOV.x = m_szHeader.cx -sz.cx -5;
		if( bSplit )
			m_ptFOV.x = m_szHeader.cx /2 -sz.cx -5;
		m_ptFOV.y = 0;

		if( m_boolShowHeader && m_boolShowFOV )
			TextOut(hdc, m_ptFOV.x, m_ptFOV.y, (LPCSTR)m_csFOV, m_csFOV.GetLength());
//			ExtTextOut( hdc, m_ptFOV.x, m_ptFOV.y, ETO_OPAQUE, &CRect(0, 0, m_szHeader.cx, m_szHeader.cy), m_csFOV, m_csFOV.GetLength(), NULL );

		if( bSplit )
		{
			if( m_boolShowHeader && m_boolShowFOV )
				TextOut(hdc, m_ptFOV.x +nOffsetX, m_ptFOV.y, (LPCSTR)m_csFOV, m_csFOV.GetLength());
		}
	}

	/////////////////////////////////////////////////////////////////
	DeleteObject( brBackBrush );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSHeaderLay == NULL ) return DD_OK;
	m_lpDDSHeaderLay->ReleaseDC(hdc);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_boolShowHeader )
	{
		CRect rcGrapic = CRect(0, 0, m_szHeader.cx, m_szHeader.cy);
		CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic);

		m_UpdateRectsImage.Add( rcImage );
	}
	/////////////////////////////////////////////////////////////////

	return DD_OK;
}

BOOL CHeaderControler::InitHeaderLay( BOOL bSplit )
{
	if (m_hfontHeaderText != NULL)
	{
		DeleteObject(m_hfontHeaderText);
	}

	// Create Font
	CSize szReso = CMCICardCommonSetting::Instance().m_sizeReso;
	int nFontSize = 24;
	if( szReso.cx < 16+1 )
		nFontSize = 8;
	else if( szReso.cx < 32+1 )
		nFontSize = 8;
	else if( szReso.cx < 64+1 )
		nFontSize = 8;
	else if( szReso.cx < 128+1 )
		nFontSize = 9;
	else if( szReso.cx < 256+1 )
		nFontSize = 9;
	else if( szReso.cx < 480+1 )
		nFontSize = 12;
	else if( szReso.cx < 600+1 )
		nFontSize = 12;
	else if( szReso.cx < 640+1 )
		nFontSize = 20;
	else if( szReso.cx < 800+1 )
		nFontSize = 20;
	else if( szReso.cx < 1280+1 )
		nFontSize = 28;
	else if( szReso.cx < 1536+1 )
		nFontSize = 40;
	else if( szReso.cx < 2048+1 )
		nFontSize = 64;
	else
		nFontSize = 120;
	m_hfontHeaderText = CreateFont(
		nFontSize, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
		VARIABLE_PITCH,
		CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );

	RedrawHeaderLay(bSplit);

	return TRUE;
}

long CHeaderControler::UpdateHeaderLay(CMobileUScaleControler* pMobileUScaleControler, BOOL bReady)
{
	CRect			rcTemp;
	HRESULT			ddrval = DD_OK;

	while ( !m_UpdateRectsHeader.IsEmpty() )
	{
		rcTemp = m_UpdateRectsHeader.Get();

		if ( !rcTemp.IsRectEmpty() )
		{
			pMobileUScaleControler->m_UpdateRectsMobileUScale.Add( rcTemp );

			if ( m_boolShowHeader )
			{
				CRect rcDestImageLay	= rcTemp;
				CRect rcSrcHeaderLay	= LayWindowMapClip_Image_to_Header( rcDestImageLay );
				CRect rcDestGraphLay	= LayWindowMapClip_Image_to_Graph( rcDestImageLay );

				if ( !bReady ) return DD_OK;

				ddrval = m_lpDDSGraphBackLay->Blt(
					(LPRECT)rcDestGraphLay,			// dest rect
					m_lpDDSHeaderLay,				// src surface
					(LPRECT)rcSrcHeaderLay,			// src rect
					DDBLT_KEYSRC | DDBLT_WAIT,
					NULL);
			}
		}
	}
	return ddrval;
}


BOOL CHeaderControler::CreateSurface( BOOL boolUseSysMemHeaderLay ) 
{
	DDSURFACEDESC			ddsd;
	HRESULT					ddrval;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize			= sizeof(ddsd);
	ddsd.dwFlags		= DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth		= m_szHeader.cx;
	ddsd.dwHeight		= m_szHeader.cy;
	if( boolUseSysMemHeaderLay )
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
	ddrval = m_lpDD->CreateSurface( &ddsd, &m_lpDDSHeaderLay, NULL );
	if ( ddrval != DD_OK )
	{
		m_lpDDSHeaderLay = NULL;
		char szBuf[255];
		sprintf_s( szBuf, "Create HeaderLay Surface failed err=%d", ddrval);
		OutputDebugString(szBuf);
		return FALSE;
	}

	return TRUE;
}
