#include "stdafx.h"
#include "LabelControler.h"
#include "RectConvertor.h"
#include "MCICardParam.h"
#include "MCICardOption.h"
#include "MCICardCommonSetting.h"
#include "Factory.h"
#include <string>
using namespace std;
#include "LabelInitializer.h"
using namespace LabelInitializer;
#include "MobileUScaleControler.h"
#include "AngleControler.h"
#include "Ddutil.h"

CLabelControler::CLabelControler(CCriticalSection& cs,LPDIRECTDRAW2& lpDD,LPDIRECTDRAWSURFACE& lpDDSGraphBackLay):
	m_mutexLayAll(cs),m_lpDD(lpDD),m_lpDDSGraphBackLay(lpDDSGraphBackLay)
{
	m_boolRedrawLabel		= FALSE;
	m_boolShowLabel			= TRUE;
	m_boolShowTime			= FALSE;
	m_boolShowNote			= FALSE;
	m_boolShowPixelSize		= TRUE;

	m_hfontEditText			= NULL;	// 2004-8-2_加强图形层显示
	m_hfontLabelText		= NULL;
	m_hfontLabelLogoText	= NULL;

	m_boolShowMobileUScale	= FALSE;
	m_hfontMobileUScaleText	= NULL;

	m_lpDDSLabelLay			= NULL;		// Offscreen surface Label Lay

	m_csHV					= _T("0.0 KV");
	m_csMag					= _T("150");
	m_csWD					= _T("12.0 mm");
	m_csUScal				= _T("100 μm");
	m_nUScalePixelNum       = 300;

	m_csSG1			= _T("SE");
	m_csSG2			= _T("SE/BSE");
	for( int i=0; i<6; i++ )
		m_csDet[i]	= _T("SE/BSE");
}


CLabelControler::~CLabelControler()
{
}



void CLabelControler::DrawLabel( HDC& hdc, COLORREF clrBack, int nOffsetX, BOOL bSplit, BOOL bDet2nd ) 
{
	Footer_Options& m_footerOpt = CMCICardOption::Instance().m_footerOpt;
	SYS_Options& m_sysOpt = CMCICardOption::Instance().m_sysOpt;
	if(m_hfontLabelText != NULL)
	{
		SIZE szText;
		CString str;
		SelectObject(hdc, m_hfontLabelText);
		SetTextColor(hdc, m_sysOpt.color[SysOpt_clrLabelText]);
		SetBkColor(hdc, clrBack);

		if( m_footerOpt.opt[Footer_HV].bVisible )
		{
			str.Format( "%s:%s", m_footerOpt.opt[Footer_HV].strDisp, m_csHV );
			TextOut(hdc, m_footerOpt.opt[Footer_HV].ptPos.x, m_footerOpt.opt[Footer_HV].ptPos.y, (LPCSTR)str, str.GetLength());
		}
		if( m_footerOpt.opt[Footer_MAG].bVisible )
		{
			str.Format( "%s:%s", m_footerOpt.opt[Footer_MAG].strDisp, m_csMag );
			TextOut(hdc, m_footerOpt.opt[Footer_MAG].ptPos.x, m_footerOpt.opt[Footer_MAG].ptPos.y, (LPCSTR)str, str.GetLength());
		}
		if( m_footerOpt.opt[Footer_WD].bVisible )
		{
			str.Format( "%s:%s", m_footerOpt.opt[Footer_WD].strDisp, m_csWD );
			TextOut(hdc, m_footerOpt.opt[Footer_WD].ptPos.x, m_footerOpt.opt[Footer_WD].ptPos.y, (LPCSTR)str, str.GetLength());
		}
		if( m_footerOpt.opt[Footer_VAC].bVisible )
		{
			str.Format( "%s:%s", m_footerOpt.opt[Footer_VAC].strDisp, m_csVAC );
			TextOut(hdc, m_footerOpt.opt[Footer_VAC].ptPos.x, m_footerOpt.opt[Footer_VAC].ptPos.y, (LPCSTR)str, str.GetLength());
		}
		if( m_footerOpt.opt[Footer_DET].bVisible )
		{
			if( bDet2nd )	// 只看第二通道时
			{
				str.Format( "%s:%s", m_footerOpt.opt[Footer_DET].strDisp, m_csSG2 );
				TextOut(hdc, m_footerOpt.opt[Footer_DET].ptPos.x, m_footerOpt.opt[Footer_DET].ptPos.y, (LPCSTR)str, str.GetLength());
			}
			else
			{
				str.Format( "%s:%s", m_footerOpt.opt[Footer_DET].strDisp, m_csSG1 );
				TextOut(hdc, m_footerOpt.opt[Footer_DET].ptPos.x, m_footerOpt.opt[Footer_DET].ptPos.y, (LPCSTR)str, str.GetLength());
			}
		}
		if( m_footerOpt.opt[Footer_UBAR].bVisible )
		{
			GetTextExtentPoint( hdc, (LPCSTR)m_csUScal, m_csUScal.GetLength(), &szText );
			TextOut(hdc, m_footerOpt.opt[Footer_UBAR].ptPos.x +(m_nUScalePixelNum -szText.cx)/2, m_footerOpt.opt[Footer_UBAR].ptPos.y, (LPCSTR)m_csUScal, m_csUScal.GetLength());
		}
		if( m_boolShowTime && m_footerOpt.opt[Footer_TIME].bVisible )
			TextOut(hdc, m_footerOpt.opt[Footer_TIME].ptPos.x, m_footerOpt.opt[Footer_TIME].ptPos.y, (LPCSTR)m_csTime, m_csTime.GetLength());
		if( m_boolShowPixelSize && m_footerOpt.opt[Footer_NOTE].bVisible )
			TextOut(hdc, m_footerOpt.opt[Footer_NOTE].ptPos.x, m_footerOpt.opt[Footer_NOTE].ptPos.y, (LPCSTR)m_csPixelSize, m_csPixelSize.GetLength());
		else if( m_boolShowNote && m_footerOpt.opt[Footer_NOTE].bVisible )
			TextOut(hdc, m_footerOpt.opt[Footer_NOTE].ptPos.x, m_footerOpt.opt[Footer_NOTE].ptPos.y, (LPCSTR)m_csNote, m_csNote.GetLength());

		if( bSplit )
		{
			if( m_footerOpt.opt[Footer_HV].bVisible )
			{
				str.Format( "%s:%s", m_footerOpt.opt[Footer_HV].strDisp, m_csHV );
				TextOut(hdc, m_footerOpt.opt[Footer_HV].ptPos.x +nOffsetX, m_footerOpt.opt[Footer_HV].ptPos.y, (LPCSTR)str, str.GetLength());
			}
			if( m_footerOpt.opt[Footer_MAG].bVisible )
			{
				str.Format( "%s:%s", m_footerOpt.opt[Footer_MAG].strDisp, m_csMag );
				TextOut(hdc, m_footerOpt.opt[Footer_MAG].ptPos.x +nOffsetX, m_footerOpt.opt[Footer_MAG].ptPos.y, (LPCSTR)str, str.GetLength());
			}
			if( m_footerOpt.opt[Footer_WD].bVisible )
			{
				str.Format( "%s:%s", m_footerOpt.opt[Footer_WD].strDisp, m_csWD );
				TextOut(hdc, m_footerOpt.opt[Footer_WD].ptPos.x +nOffsetX, m_footerOpt.opt[Footer_WD].ptPos.y, (LPCSTR)str, str.GetLength());
			}
			if( m_footerOpt.opt[Footer_VAC].bVisible )
			{
				str.Format( "%s:%s", m_footerOpt.opt[Footer_VAC].strDisp, m_csVAC );
				TextOut(hdc, m_footerOpt.opt[Footer_VAC].ptPos.x +nOffsetX, m_footerOpt.opt[Footer_VAC].ptPos.y, (LPCSTR)str, str.GetLength());
			}
			if( m_footerOpt.opt[Footer_DET].bVisible )
			{
				str.Format( "%s:%s", m_footerOpt.opt[Footer_DET].strDisp, m_csSG2 );
				TextOut(hdc, m_footerOpt.opt[Footer_DET].ptPos.x +nOffsetX, m_footerOpt.opt[Footer_DET].ptPos.y, (LPCSTR)str, str.GetLength());
			}

			if( m_footerOpt.opt[Footer_UBAR].bVisible )
			{
				GetTextExtentPoint( hdc, (LPCSTR)m_csUScal, m_csUScal.GetLength(), &szText );
				TextOut(hdc, m_footerOpt.opt[Footer_UBAR].ptPos.x +nOffsetX +(m_nUScalePixelNum -szText.cx)/2, m_footerOpt.opt[Footer_UBAR].ptPos.y-4, (LPCSTR)m_csUScal, m_csUScal.GetLength());
			}
			if( m_boolShowTime && m_footerOpt.opt[Footer_TIME].bVisible )
				TextOut(hdc, m_footerOpt.opt[Footer_TIME].ptPos.x +nOffsetX, m_footerOpt.opt[Footer_TIME].ptPos.y, (LPCSTR)m_csTime, m_csTime.GetLength());
			if( m_boolShowPixelSize && m_footerOpt.opt[Footer_NOTE].bVisible )
				TextOut(hdc, m_footerOpt.opt[Footer_NOTE].ptPos.x +nOffsetX, m_footerOpt.opt[Footer_NOTE].ptPos.y, (LPCSTR)m_csPixelSize, m_csPixelSize.GetLength());
			else if( m_boolShowNote && m_footerOpt.opt[Footer_NOTE].bVisible )
				TextOut(hdc, m_footerOpt.opt[Footer_NOTE].ptPos.x +nOffsetX, m_footerOpt.opt[Footer_NOTE].ptPos.y, (LPCSTR)m_csNote, m_csNote.GetLength());
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if(m_hfontLabelLogoText != NULL && m_footerOpt.opt[Footer_LOGO].bVisible)
	{
		SelectObject(hdc, m_hfontLabelLogoText);
		SetTextColor(hdc, m_sysOpt.color[SysOpt_clrLabelLogoText]);
		SetBkColor(hdc, clrBack);

		TextOut(hdc, m_footerOpt.opt[Footer_LOGO].ptPos.x, m_footerOpt.opt[Footer_LOGO].ptPos.y, (LPCSTR)m_csLogo, m_csLogo.GetLength());
		if( bSplit )
			TextOut(hdc, m_footerOpt.opt[Footer_LOGO].ptPos.x +nOffsetX, m_footerOpt.opt[Footer_LOGO].ptPos.y, (LPCSTR)m_csLogo, m_csLogo.GetLength());
	}
}


BOOL CLabelControler::ShowLabel(BOOL boolShow)
{
	if ( m_boolShowLabel == boolShow )
		return m_boolShowLabel;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	CRectManager& m_UpdateRectsImage = CMCICardCommonSetting::Instance().m_UpdateRectsImage;
	CSize& m_szGraph = CMCICardCommonSetting::Instance().m_szGraph;

	CRect rcGrapic = CRect(0, m_szGraph.cy - m_szLabel.cy,
		m_szGraph.cx, m_szGraph.cy);
	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

	if ( m_boolShowLabel == TRUE )
	{	
		m_UpdateRectsImage.Add( rcImage );
	}
	else
	{
		m_UpdateRectsLabel.Add( rcImage );
	}

	m_boolShowLabel = boolShow;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////

	return m_boolShowLabel;
}


void CLabelControler::Close()
{
	/////////////////////////////////////////////////////////////////
	if (m_hfontEditText != NULL)	// 2004-8-2_加强图形层显示
	{
		DeleteObject(m_hfontEditText);
		m_hfontEditText = NULL;
	}
	if (m_hfontLabelText != NULL)
	{
		DeleteObject(m_hfontLabelText);
		m_hfontLabelText = NULL;
	}
	if (m_hfontLabelLogoText != NULL)
	{
		DeleteObject(m_hfontLabelLogoText);
		m_hfontLabelLogoText = NULL;
	}

	/////////////////////////////////////////////////////////////////
	if (m_hfontMobileUScaleText != NULL)
	{
		DeleteObject(m_hfontMobileUScaleText);
		m_hfontMobileUScaleText = NULL;
	}

	//if( m_lpDDSLabelLay != NULL )
	//{
	//	m_lpDDSLabelLay->Release();
	//	m_lpDDSLabelLay = NULL;
	//}
}

void CLabelControler::ReleaseLay()
{
	if(m_lpDDSLabelLay != NULL )
	{
		m_lpDDSLabelLay->Release();
		m_lpDDSLabelLay = NULL;
	}
}

void CLabelControler::SetProductLogo( CString csLogo )
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csLogo = csLogo;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}


void CLabelControler::UpdateHV(CString	csHV)
{	
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csHV = csHV;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

void CLabelControler::UpdateWD(CString	csWD)
{	
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csWD = csWD;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

void CLabelControler::UpdatePhotoSN(CString	csPhotoSN)
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csPhotoSN = csPhotoSN;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

void CLabelControler::UpdateTime(CString	csTime)
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csTime = csTime;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

void CLabelControler::UpdateVac(CString	csVac)
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csVAC = csVac;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

void CLabelControler::UpdateNote(CString	csNote)
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csNote = csNote;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

void CLabelControler::UpdatePixelSize(CString	csPixelSize)
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csPixelSize = csPixelSize;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}


CRect CLabelControler::LayWindowMapClip_Image_to_Label( CRect& rcImage )
{
	CSize& szGraph = CMCICardCommonSetting::Instance().m_szGraph;

	CRect	rcGrapic = LayWindowMapClip_Image_to_Graph( rcImage);
	CPoint	ptOrigin(0, szGraph.cy - m_szLabel.cy);

	CRect	rcLabel = rcGrapic;
	rcLabel.OffsetRect( - ptOrigin );
	rcLabel &= CRect(0, 0, m_szLabel.cx, m_szLabel.cy);

	if( !rcLabel.IsRectEmpty() )
	{
		rcGrapic = rcLabel;
		rcGrapic.OffsetRect( ptOrigin );
		rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );
	}

	return rcLabel;
}


long CLabelControler::RedrawLabelLay(BOOL bSplit, BOOL bDet2nd  )
{
	HDC				hdc;
	HRESULT			ddrval;

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSLabelLay == NULL ) return DD_OK;
	ddrval = m_lpDDSLabelLay->GetDC( &hdc );
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
		clrBack	= CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrBack];
	HBRUSH brBackBrush = CreateSolidBrush( clrBack );
	CRect rcFill = CRect(0,0,m_szLabel.cx,m_szLabel.cy);
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
	int nLineWidth;
	switch( m_sizeReso.cx )
	{
	case	256:
	case	384:
	case	480:
	case	512:
		nLineWidth	= 1;
		break;
	case	600:
	case	640:
	case	768:
	case	800:
		nLineWidth	= 1;
		break;
	case	1024:
		nLineWidth	= 1;
		break;
	case	1280:
	case	1440:
	case	1536:
		nLineWidth	= 2;
		break;
	case	2048:
		nLineWidth	= 5;
		break;
	case	4096:
		nLineWidth	= 10;
		break;
	default:
		nLineWidth	= 1;
		break;
	}
	HPEN hPen, hPenOld;
	hPen = CreatePen(PS_SOLID, nLineWidth, CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrLabelText]);
	hPenOld = (HPEN)SelectObject(hdc, hPen);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if( CMCICardOption::Instance().m_footerOpt.opt[Footer_UBAR].bVisible )
	{
		CPoint ptPos0(1,12);
		CPoint ptPos1(1,12);
		switch( m_sizeReso.cx )
		{
		case	128:
			ptPos0.y = 5;
			ptPos1.y = 5;
			break;
		case	256:
			ptPos0.y = 10;
			ptPos1.y = 10;
			break;
		case	512:
		case	1024:
			ptPos0.y = 16;
			ptPos1.y = 16;
			break;
		case	2048:
			ptPos0.y = 24;
			ptPos1.y = 24;
			break;
		case	4096:
			ptPos0.y = 48;
			ptPos1.y = 48;
			break;
		default:
			ptPos0.y = 10;
			ptPos1.y = 10;
			break;
		}
		ptPos0.x = CMCICardOption::Instance().m_footerOpt.opt[Footer_UBAR].ptPos.x;;
		ptPos1.x = ptPos0.x + m_nUScalePixelNum;

#ifdef GRAPHIC_BLACK
		//		MoveToEx ( hdc, ptPos0.x, ptPos0.y-2, NULL );
		//		LineTo ( hdc, ptPos1.x, ptPos1.y-2 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+1 );
		//		MoveToEx ( hdc, ptPos0.x, ptPos0.y+2, NULL );
		//		LineTo ( hdc, ptPos1.x, ptPos1.y+2 );

		MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-6, NULL );
		LineTo ( hdc, ptPos0.x-1, ptPos0.y+7 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y-6, NULL );
		LineTo ( hdc, ptPos0.x, ptPos0.y+7 );
		//		MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-6, NULL );
		//		LineTo ( hdc, ptPos0.x+1, ptPos0.y+7 );

		//		MoveToEx ( hdc, ptPos1.x-1, ptPos1.y-6, NULL );
		//		LineTo ( hdc, ptPos1.x-1, ptPos1.y+7 );
		MoveToEx ( hdc, ptPos1.x, ptPos1.y-6, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+7 );
		MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-6, NULL );
		LineTo ( hdc, ptPos1.x+1, ptPos1.y+7 );

		if( bSplit )
		{
			ptPos0.x += nOffsetX;
			ptPos1.x += nOffsetX;

			//			MoveToEx ( hdc, ptPos0.x, ptPos0.y-2, NULL );
			//			LineTo ( hdc, ptPos1.x, ptPos1.y-2 );
			MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
			LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
			MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
			LineTo ( hdc, ptPos1.x, ptPos1.y );
			MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
			LineTo ( hdc, ptPos1.x, ptPos1.y+1 );
			//			MoveToEx ( hdc, ptPos0.x, ptPos0.y+2, NULL );
			//			LineTo ( hdc, ptPos1.x, ptPos1.y+2 );

			MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-6, NULL );
			LineTo ( hdc, ptPos0.x-1, ptPos0.y+7 );
			MoveToEx ( hdc, ptPos0.x, ptPos0.y-6, NULL );
			LineTo ( hdc, ptPos0.x, ptPos0.y+7 );
			//			MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-6, NULL );
			//			LineTo ( hdc, ptPos0.x+1, ptPos0.y+7 );

			//			MoveToEx ( hdc, ptPos1.x-1, ptPos1.y-6, NULL );
			//			LineTo ( hdc, ptPos1.x-1, ptPos1.y+7 );
			MoveToEx ( hdc, ptPos1.x, ptPos1.y-6, NULL );
			LineTo ( hdc, ptPos1.x, ptPos1.y+7 );
			MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-6, NULL );
			LineTo ( hdc, ptPos1.x+1, ptPos1.y+7 );
		}

#else
		MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+1 );

		MoveToEx ( hdc, ptPos0.x, ptPos0.y-3, NULL );
		LineTo ( hdc, ptPos0.x, ptPos0.y+4 );
		MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-3, NULL );
		LineTo ( hdc, ptPos0.x+1, ptPos0.y+4 );

		MoveToEx ( hdc, ptPos1.x, ptPos1.y-3, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+4 );
		MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-3, NULL );
		LineTo ( hdc, ptPos1.x+1, ptPos1.y+4 );

		if( bSplit )
		{
			ptPos0.x += nOffsetX;
			ptPos1.x += nOffsetX;

			MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
			LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
			MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
			LineTo ( hdc, ptPos1.x, ptPos1.y );
			MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
			LineTo ( hdc, ptPos1.x, ptPos1.y+1 );

			MoveToEx ( hdc, ptPos0.x, ptPos0.y-3, NULL );
			LineTo ( hdc, ptPos0.x, ptPos0.y+4 );
			MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-3, NULL );
			LineTo ( hdc, ptPos0.x+1, ptPos0.y+4 );

			MoveToEx ( hdc, ptPos1.x, ptPos1.y-3, NULL );
			LineTo ( hdc, ptPos1.x, ptPos1.y+4 );
			MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-3, NULL );
			LineTo ( hdc, ptPos1.x+1, ptPos1.y+4 );
		}
#endif
		SelectObject(hdc, hPenOld);
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	SetTextAlign(hdc, TA_TOP | TA_LEFT);
	DrawLabel(hdc, clrBack, nOffsetX,bSplit,bDet2nd);

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	DeleteObject( hPen );
	DeleteObject( brBackBrush );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSLabelLay == NULL ) return DD_OK;
	m_lpDDSLabelLay->ReleaseDC(hdc);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_boolShowLabel )
	{
		CRect rcGrapic = CRect(0, m_szGraph.cy - m_szLabel.cy, m_szGraph.cx, m_szGraph.cy);
		CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic);

		m_UpdateRectsImage.Add( rcImage );
	}
	/////////////////////////////////////////////////////////////////

	return DD_OK;
}


BOOL CLabelControler::InitLabelLay(BOOL	bSplit,BOOL bDet2nd)
{
	std::string sFunctionID = "";
	CSize& m_sizeReso = CMCICardCommonSetting::Instance().m_sizeReso;
	if( m_sizeReso.cx < 128+1 )
		sFunctionID = "InitLabelLay_0128";
	else if( m_sizeReso.cx < 384+1 )
		sFunctionID = "InitLabelLay_0256";
	else if( m_sizeReso.cx < 480+1 )
		sFunctionID = "InitLabelLay_0480";
	else if( m_sizeReso.cx < 600+1 )
		sFunctionID = "InitLabelLay_0512";
	else if( m_sizeReso.cx < 640+1 )
		sFunctionID = "InitLabelLay_0640";
	else if( m_sizeReso.cx < 800+1 )
		sFunctionID = "InitLabelLay_0800";
	else if( m_sizeReso.cx < 1280+1 )
		sFunctionID = "InitLabelLay_1024";
	else if( m_sizeReso.cx < 1536+1 )
		sFunctionID = "InitLabelLay_1440";
	else if( m_sizeReso.cx < 2048+1 )
		sFunctionID = "InitLabelLay_2048";
	else
		sFunctionID = "InitLabelLay_4096";

	IMCILabelInitializer* pInitializer = Factory<IMCILabelInitializer,std::string>::Instance().BuildProduct(sFunctionID);
	if (pInitializer)
	{
		pInitializer->Initialize(this);
		delete pInitializer; pInitializer = NULL;
	}
	RedrawLabelLay(bSplit,bDet2nd);

	return TRUE;
}



long CLabelControler::UpdateLabelLay(CMobileUScaleControler* pMobileUScaleControler,CAngleControler* pAngleControler,BOOL bReady)
{
	CRect			rcTemp;
	HRESULT			ddrval = DD_OK;

	while ( !m_UpdateRectsLabel.IsEmpty() )
	{
		rcTemp = m_UpdateRectsLabel.Get();

		if ( !rcTemp.IsRectEmpty() )
		{
			pMobileUScaleControler->m_UpdateRectsMobileUScale.Add( rcTemp );
			pAngleControler->m_UpdateRectsAngle.Add( rcTemp );

			if ( m_boolShowLabel )
			{
				CRect rcDestImageLay	= rcTemp;
				CRect rcSrcLabelLay		= LayWindowMapClip_Image_to_Label( rcDestImageLay );
				CRect rcDestGraphLay = LayWindowMapClip_Image_to_Graph( rcDestImageLay );

				if ( !bReady ) return DD_OK;

				ddrval = m_lpDDSGraphBackLay->Blt(
					(LPRECT)rcDestGraphLay,			// dest rect
					m_lpDDSLabelLay,				// src surface
					(LPRECT)rcSrcLabelLay,			// src rect
					DDBLT_KEYSRC | DDBLT_WAIT,
					NULL);
			}
		}
	}

	return ddrval;
}


BOOL CLabelControler::CreateSurface( BOOL boolUseSysMemLabelLay ) 
{
	DDSURFACEDESC			ddsd;
	HRESULT					ddrval;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize			= sizeof(ddsd);
	ddsd.dwFlags		= DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth		= m_szLabel.cx;
	ddsd.dwHeight		= m_szLabel.cy;
	if( boolUseSysMemLabelLay )
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
	ddrval = m_lpDD->CreateSurface( &ddsd, &m_lpDDSLabelLay, NULL );
	if ( ddrval != DD_OK )
	{
		DebugMessage("Create LabelLay Surface failed err=%d", ddrval);
		return FALSE;
	}

	return TRUE;
}


void CLabelControler::UpdateMAG(
	CString		csMag,					// 放大倍数显示字符
	int			nUScalePixelNum,		// 固定标尺显示长度
	CString		csUScal)				// 固定标尺显示字符
{
	m_mutexLayAll.Lock();

	m_csMag					= csMag;
	m_nUScalePixelNum		= nUScalePixelNum;
	m_csUScal				= csUScal;
	m_boolRedrawLabel = TRUE;

	m_mutexLayAll.Unlock();
}