// MCICard.cpp : implementation file
//

#include "stdafx.h"
#include "Ddutil.h"
#include "MCICard.h"
#include "RectConvertor.h"
#include "MCICardOption.h"
#include "MCICardCommonSetting.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMCICard

BOOL CMCICard::GraphDrawLineEx( const int nLength )	// 单位：mm
{
	HDC				hdc;
    HRESULT			ddrval;

	//////////////////////////////////////////////////////////////////
//	m_mutexLayAll.Lock();
	//////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSGraphLay == NULL )
	{
//		m_mutexLayAll.Unlock();
		return FALSE;
	}
	ddrval = m_lpDDSGraphLay->GetDC( &hdc );
	if( ddrval != DD_OK )
	{
//		m_mutexLayAll.Unlock();
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	HPEN hPenOld;
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB( 255,255,0));	// 黄色
	/////////////////////////////////////////////////////////////////

//	int nOldMapMode = GetMapMode( hdc );
//	SetMapMode( hdc, MM_LOMETRIC );	// 单位：0.1mm
//	int nPoint = GetViewportOrg( hdc );
//	SetViewportOrg( hdc, 300, 200 );

	int nLen	= (int)(nLength * m_dblPixelsPerMM_HORZ + 0.5 );

	hPenOld = (HPEN)SelectObject(hdc, hPen);
	MoveToEx ( hdc, 300, 200, NULL );
	LineTo ( hdc, 300 + nLen, 200 );
	SelectObject(hdc, hPenOld);

//	SetViewportOrg( hdc, nPoint );
//	SetMapMode( hdc, nOldMapMode );

	/////////////////////////////////////////////////////////////////
	DeleteObject( hPen );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	m_lpDDSGraphLay->ReleaseDC(hdc);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	CRect rcGrapic = CRect( 300, 200, 300 +nLen, 202 );
/*
	rcGrapic.left	= min(ptPos0.x, ptPos1.x) - 1;
	rcGrapic.right	= max(ptPos0.x, ptPos1.x) + 1;
	rcGrapic.top	= min(ptPos0.y, ptPos1.y) - 1;
	rcGrapic.bottom	= max(ptPos0.y, ptPos1.y) + 1;
*/
	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

	CMCICardCommonSetting::Instance().m_UpdateRectsGraph.Add( rcImage );
	/////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
//	m_mutexLayAll.Unlock();
	//////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL CMCICard::GraphDrawLine(
		const CPoint ptPos0, const CPoint ptPos1, const COLORREF color )
{
	HDC				hdc;
    HRESULT			ddrval;

	//////////////////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	//////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSGraphLay == NULL )
	{
		m_mutexLayAll.Unlock();
		return FALSE;
	}
	ddrval = m_lpDDSGraphLay->GetDC( &hdc );
	if( ddrval != DD_OK )
	{
		m_mutexLayAll.Unlock();
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	HPEN hPenOld;
	HPEN hPen = CreatePen(PS_SOLID, 0, color);
	/////////////////////////////////////////////////////////////////

	hPenOld = (HPEN)SelectObject(hdc, hPen);
	MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y );
	SelectObject(hdc, hPenOld);

	/////////////////////////////////////////////////////////////////
	DeleteObject( hPen );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	m_lpDDSGraphLay->ReleaseDC(hdc);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	CRect rcGrapic;

	rcGrapic.left	= min(ptPos0.x, ptPos1.x) - 1;
	rcGrapic.right	= max(ptPos0.x, ptPos1.x) + 1;
	rcGrapic.top	= min(ptPos0.y, ptPos1.y) - 1;
	rcGrapic.bottom	= max(ptPos0.y, ptPos1.y) + 1;

	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

	if ( color == CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrTransparent] )
	{	
		CMCICardCommonSetting::Instance().m_UpdateRectsImage.Add( rcImage );
	}
	else
	{
		CMCICardCommonSetting::Instance().m_UpdateRectsGraph.Add( rcImage );
	}
	/////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	//////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL CMCICard::GraphDrawRect( const CRect rcDest, const COLORREF color )
{
	HDC				hdc;
    HRESULT			ddrval;

	//////////////////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	//////////////////////////////////////////////////////////////////

	if ( m_lpDDSGraphLay == NULL )
	{
		m_mutexLayAll.Unlock();
		return FALSE;
	}
	ddrval = m_lpDDSGraphLay->GetDC( &hdc );
	if( ddrval != DD_OK )
	{
		m_mutexLayAll.Unlock();
		return FALSE;
	}

	/////////////////////////////////////////////////////////////////
	HBRUSH brBrush = CreateSolidBrush( color );
	/////////////////////////////////////////////////////////////////

	FrameRect( hdc, (LPCRECT)rcDest, brBrush );

	/////////////////////////////////////////////////////////////////
	DeleteObject( brBrush );
	/////////////////////////////////////////////////////////////////

	m_lpDDSGraphLay->ReleaseDC(hdc);

	/////////////////////////////////////////////////////////////////
	CRect rcGrapic = rcDest;
	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

	if ( color == CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrTransparent] )
	{	
		CMCICardCommonSetting::Instance().m_UpdateRectsImage.Add( rcImage );
	}
	else
	{
		CMCICardCommonSetting::Instance().m_UpdateRectsGraph.Add( rcImage );
	}
	/////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	//////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL CMCICard::GraphDrawRectEx( const BOOL bDrawCircle, const CRect rcDest, const COLORREF color )
{
	HDC				hdc;
    HRESULT			ddrval;

	//////////////////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	//////////////////////////////////////////////////////////////////

	if ( m_lpDDSGraphLay == NULL )
	{
		m_mutexLayAll.Unlock();
		return FALSE;
	}
	ddrval = m_lpDDSGraphLay->GetDC( &hdc );
	if( ddrval != DD_OK )
	{
		m_mutexLayAll.Unlock();
		return FALSE;
	}

	/////////////////////////////////////////////////////////////////
	HBRUSH brBrush = CreateSolidBrush( color );
	/////////////////////////////////////////////////////////////////

	FrameRect( hdc, (LPCRECT)rcDest, brBrush );

	/////////////////////////////////////////////////////////////////
	DeleteObject( brBrush );
	/////////////////////////////////////////////////////////////////

	if( bDrawCircle )
	{
		LOGBRUSH lplb;
		lplb.lbStyle = BS_HOLLOW;
		HBRUSH brBrush = CreateBrushIndirect(&lplb);
		HPEN pen = CreatePen(PS_SOLID, 1, color);
		SelectObject(hdc, brBrush);
		SelectObject(hdc, pen);
		Ellipse( hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom );
		DeleteObject( brBrush );
		DeleteObject( pen );
	}

	m_lpDDSGraphLay->ReleaseDC(hdc);

	/////////////////////////////////////////////////////////////////
	CRect rcGrapic = rcDest;
	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

	if ( color == CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrTransparent] )
	{	
		CMCICardCommonSetting::Instance().m_UpdateRectsImage.Add( rcImage );
	}
	else
	{
		CMCICardCommonSetting::Instance().m_UpdateRectsGraph.Add( rcImage );
	}
	/////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	//////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL CMCICard::GraphDrawCircle( const CPoint ptCenter,	// 跟踪点圆心
							 const CPoint ptEnd,		// 跟踪线尾端
							 BOOL bDrawArrow,			// 是否画箭头
							 const int nRadius,			// 跟踪点圆形标识半径
							 const COLORREF color )		// 画刷/笔颜色
{
	HDC				hdc;
    HRESULT			ddrval;

	//////////////////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	//////////////////////////////////////////////////////////////////

	if ( m_lpDDSGraphLay == NULL )
	{
		m_mutexLayAll.Unlock();
		return FALSE;
	}
	ddrval = m_lpDDSGraphLay->GetDC( &hdc );
	if( ddrval != DD_OK )
	{
		m_mutexLayAll.Unlock();
		return FALSE;
	}

	// 画跟踪点圆
	CRect rcGrapic = CRect(ptCenter.x -nRadius, ptCenter.y -nRadius, ptCenter.x +nRadius, ptCenter.y +nRadius);
	/////////////////////////////////////////////////////////////////
	HBRUSH brBrush = CreateSolidBrush( color );
	HGDIOBJ hgdiobj = SelectObject( hdc, brBrush);
	/////////////////////////////////////////////////////////////////

	Ellipse( hdc, ptCenter.x -nRadius, ptCenter.y -nRadius, ptCenter.x +nRadius, ptCenter.y +nRadius );

	/////////////////////////////////////////////////////////////////
	SelectObject( hdc, hgdiobj);
	DeleteObject( brBrush );
	/////////////////////////////////////////////////////////////////

	if( bDrawArrow )
	{
		// 画跟踪线
		/////////////////////////////////////////////////////////////////
		HPEN hPenOld;
		HPEN hPen = CreatePen(PS_SOLID, 2, color);
		hPenOld = (HPEN)SelectObject(hdc, hPen);

		MoveToEx ( hdc, ptCenter.x, ptCenter.y, NULL );
		LineTo ( hdc, ptEnd.x, ptEnd.y );

		SelectObject(hdc, hPenOld);
		DeleteObject( hPen );
		/////////////////////////////////////////////////////////////////

		// 画箭头
		/////////////////////////////////////////////////////////////////
		/* 箭头就是一个三角形,可以定义一个长度为3的CPoint数组,定义一个画刷,
		然后就可以用Polygon函数画填充的三角形了;
		至于确定箭头三角形除箭头顶点的其他两点坐标,用以下方法确定:
		假设箭头线的起点为begin,箭头顶点为end
		double slopy , cosy , siny;
		double Par = 10.0;  //length of Arrow
		slopy = atan2((begin.y - end.y), (begin.x - end.x));
		cosy = cos(slopy);
		siny = sin(slopy);

		两点坐标为
		end.x + int(Par * cosy - (Par / 2.0 * siny)),
		end.y + int(Par * siny + (Par / 2.0 * cosy));

		end.x + int(Par * cosy + Par / 2.0 * siny),
		end.y - int(Par / 2.0 * cosy - Par * siny)*/
		HBRUSH hBrushOld;
		HBRUSH hBrush = CreateSolidBrush( color );
		hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

		double slopy , cosy , siny;
		double Par = nRadius *2;  //length of Arrow
		slopy = atan2(double(ptCenter.y - ptEnd.y), double(ptCenter.x - ptEnd.x));
		cosy = cos(slopy);
		siny = sin(slopy);
		CPoint pt[3];
		pt[0].x = ptEnd.x + int(Par * cosy - (Par / 2.0 * siny));
		pt[0].y = ptEnd.y + int(Par * siny + (Par / 2.0 * cosy));
		pt[1].x = ptEnd.x + int(Par * cosy + Par / 2.0 * siny);
		pt[1].y = ptEnd.y - int(Par / 2.0 * cosy - Par * siny);
		pt[2] = ptEnd;

		Polygon( hdc, pt, 3 );

		SelectObject(hdc, hBrushOld);
		DeleteObject( hBrush );
		/////////////////////////////////////////////////////////////////

		// 更新区域
		CRect rc = rcGrapic;
		for(int i=0; i<3; i++ )
		{
			rc.left		= min( rc.left, pt[i].x );
			rc.right	= max( rc.right, pt[i].x );
			rc.top		= min( rc.top, pt[i].y );
			rc.bottom	= max( rc.bottom, pt[i].y );
		}
		rcGrapic.left	= rc.left -1;
		rcGrapic.right	= rc.right +1;
		rcGrapic.top	= rc.top -1;
		rcGrapic.bottom	= rc.bottom +1;
	}

	m_lpDDSGraphLay->ReleaseDC(hdc);

	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

	if ( color == CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrTransparent] )
	{	
		CMCICardCommonSetting::Instance().m_UpdateRectsImage.Add( rcImage );
	}
	else
	{
		CMCICardCommonSetting::Instance().m_UpdateRectsGraph.Add( rcImage );
	}
	/////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	//////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL CMCICard::GraphClear( const CRect rcDest,	const COLORREF color )
{
	HDC				hdc;
    HRESULT			ddrval;

	//////////////////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	//////////////////////////////////////////////////////////////////

	if ( m_lpDDSGraphLay == NULL )
	{
		m_mutexLayAll.Unlock();
		return FALSE;
	}
	ddrval = m_lpDDSGraphLay->GetDC( &hdc );
	if( ddrval != DD_OK )
	{
		m_mutexLayAll.Unlock();
		return FALSE;
	}

	/////////////////////////////////////////////////////////////////
	HBRUSH brBrush = CreateSolidBrush( color );
	/////////////////////////////////////////////////////////////////

	CRect rcGraph = rcDest;
	if( rcGraph == CRect(0, 0, g_szUSB_FrameGraph.cx, g_szUSB_FrameGraph.cy) )
	{
		rcGraph = CRect( 0, 0, CMCICardCommonSetting::Instance().m_szGraph.cx, CMCICardCommonSetting::Instance().m_szGraph.cy );
	}

	FillRect( hdc, (LPCRECT)rcGraph, brBrush );

	/////////////////////////////////////////////////////////////////
	DeleteObject( brBrush );
	/////////////////////////////////////////////////////////////////

	m_lpDDSGraphLay->ReleaseDC(hdc);

	/////////////////////////////////////////////////////////////////
	CRect rcGrapic = rcDest;
	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

	if ( color == CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrTransparent] )
	{	
		CMCICardCommonSetting::Instance().m_UpdateRectsImage.Add( rcImage );
	}
	else
	{
		CMCICardCommonSetting::Instance().m_UpdateRectsGraph.Add( rcImage );
	}
	/////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	//////////////////////////////////////////////////////////////////

	return TRUE;
}

CSize CMCICard::GraphTextOut( const CPoint ptPos, const CString csText )
{
	HDC			hdc;
	CDC			dc;
    HRESULT		ddrval;
	CSize		szTextOut(0,0);

	//////////////////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	//////////////////////////////////////////////////////////////////

	if ( m_lpDDSGraphLay == NULL )
	{
		m_mutexLayAll.Unlock();
		return szTextOut;
	}
	ddrval = m_lpDDSGraphLay->GetDC( &hdc );
	if( ddrval != DD_OK )
	{
		m_mutexLayAll.Unlock();
		return szTextOut;
	}

	dc.Attach( hdc );
	if(m_pLabelControler->m_hfontLabelText != NULL)
	{
		dc.SelectObject( m_pLabelControler->m_hfontEditText );
		dc.SetTextColor( CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrEditText] );
		dc.SetBkColor( CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrBack] );

		dc.SetTextAlign( TA_TOP | TA_LEFT );
		dc.TextOut( ptPos.x, ptPos.y, csText );

		szTextOut = dc.GetOutputTextExtent( csText );
	}
	dc.Detach();

	m_lpDDSGraphLay->ReleaseDC(hdc);

	/////////////////////////////////////////////////////////////////
	CRect rcGrapic( ptPos, szTextOut );
	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

	CMCICardCommonSetting::Instance().m_UpdateRectsGraph.Add( rcImage );
	/////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	//////////////////////////////////////////////////////////////////

	return szTextOut;
}

CSize CMCICard::GetOutputTextExtent( const CString csText )
{
	HDC			hdc;
	CDC			dc;
    HRESULT		ddrval;
	CSize		szTextOut(0,0);

	//////////////////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	//////////////////////////////////////////////////////////////////

	if ( m_lpDDSGraphLay == NULL )
	{
		m_mutexLayAll.Unlock();
		return szTextOut;
	}
	ddrval = m_lpDDSGraphLay->GetDC( &hdc );
	if( ddrval != DD_OK )
	{
		m_mutexLayAll.Unlock();
		return szTextOut;
	}

	dc.Attach( hdc );
	if(m_pLabelControler->m_hfontLabelText != NULL)
	{
		dc.SelectObject( m_pLabelControler->m_hfontEditText );
		szTextOut = dc.GetOutputTextExtent( csText );
	}
	dc.Detach();

	m_lpDDSGraphLay->ReleaseDC(hdc);

	//////////////////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	//////////////////////////////////////////////////////////////////

	return szTextOut;
}
