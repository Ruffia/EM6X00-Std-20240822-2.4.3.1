#include "stdafx.h"
#include "MobileUScaleControler.h"
#include "RectConvertor.h"
#include "MCICardCommonSetting.h"
#include "LabelControler.h"
#include "OscillographControler.h"
#include "MCICardOption.h"
#include <math.h>
#include "Ddutil.h"

const int		UScale_nFontSize		= 32;

CMobileUScaleControler::CMobileUScaleControler(CCriticalSection& cs,LPDIRECTDRAW2& lpDD,LPDIRECTDRAWSURFACE& lpDDSGraphBackLay):
	m_mutexLayAll(cs),m_lpDD(lpDD),m_lpDDSGraphBackLay(lpDDSGraphBackLay)
{
	m_boolRedrawUScal		= FALSE;

	m_lpDDSMobileUScaleLay	= NULL;		// Offscreen surface UScale Lay
	
	m_csMobileUScaleText	= _T("300 μm");
}

CMobileUScaleControler::~CMobileUScaleControler()
{

}

void CMobileUScaleControler::Initialize()
{
		/////////////////////////////////////////////////
	m_nMobileUScalePixelNum	= CMCICardCommonSetting::Instance().m_sizeReso.cx;	//300;				// 移动标尺显示长度

	m_csMobileUScaleText	= _T("300 μm");	// 移动标尺显示字符
//	m_ptMobileUScaleText	= CPoint( m_nMobileUScalePixelNum/2, 8 );
	m_ptMobileUScaleText	= CPoint( m_nMobileUScalePixelNum/2, 10 );
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	m_ptMobileUScale		= CPoint( 0, 100 );	// 移动标尺位置
	//======2007.08.20_任意方向移动标尺========================
	// 修改标示：2007.08.20_任意方向移动标尺
	// 修改日期：2007.08.20星期一
	// 修改人  ：J.Y.Ma
	// 修改原因：移动标尺由单一水平方向变为任意方向

	// 本次修改前代码副本
	/*----------------------------------------------------- 
	//---------------------------------------------------*/

	// 本次修改后代码
	//-----------------------------------------------------
	///////////////////////////////////////////////////////
	m_ptMobileUScale2		= CPoint( CMCICardCommonSetting::Instance().m_sizeReso.cx, 100 );		// 移动标尺位置
	m_ptMobileUScaleSaved	= m_ptMobileUScale;
	m_ptMobileUScale2Saved	= m_ptMobileUScale2;
	//-----------------------------------------------------
	//======2007.08.20_任意方向移动标尺========================

	// 16.06.12 多移动标尺实验
	for(int i=0; i<MobileUScaleCounts; i++ )
	{
		m_nMobileUScalePixelNumArray[i]	= 200;
		m_csMobileUScaleTextArray[i]	= _T("200 μm");
		m_ptMobileUScaleArray[i]		= CPoint( -1, -1 );
		m_ptMobileUScaleArray2[i]		= CPoint( -1, -1 );
	}
	// 16.06.12 多移动标尺实验
}


void CMobileUScaleControler::ReleaseLay()
{
	if( m_lpDDSMobileUScaleLay != NULL )
	{
		m_lpDDSMobileUScaleLay->Release();
		m_lpDDSMobileUScaleLay = NULL;
	}
}


void CMobileUScaleControler::ConvertPtArray( CPoint* ptArray1, CPoint* ptArray2, CString* strArray ) 
{
	ptArray1[0] = m_ptMobileUScale;
	ptArray2[0] = m_ptMobileUScale2;
	strArray[0] = m_csMobileUScaleText;
	for(int i=0; i<MobileUScaleCounts; i++ )
	{
		ptArray1[i+1]	= m_ptMobileUScaleArray[i];
		ptArray2[i+1]	= m_ptMobileUScaleArray2[i];
		strArray[i+1]	= m_csMobileUScaleTextArray[i];
	}
}


void CMobileUScaleControler::UpdateUScale(
	int			nMobileUScalePixelNum,	// 移动标尺显示长度
	CString		csMobileUScaleText )	// 移动标尺显示字符
{
	m_mutexLayAll.Lock();

	m_nMobileUScalePixelNum	= nMobileUScalePixelNum;
	m_ptMobileUScaleText.x	= nMobileUScalePixelNum / 2;
	m_csMobileUScaleText	= csMobileUScaleText;
	m_boolRedrawUScal = TRUE;

	m_mutexLayAll.Unlock();
}


BOOL CMobileUScaleControler::ShowMobileUScale(CLabelControler* pLabelControler, BOOL boolShow)
{
	if ( pLabelControler->m_boolShowMobileUScale == boolShow )
		return pLabelControler->m_boolShowMobileUScale;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();

	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	CRectManager& m_UpdateRectsImage = CMCICardCommonSetting::Instance().m_UpdateRectsImage;

	CRect rcGrapic;
#ifdef NEW_SCANSCREEN_MODE
	//07.08.27
	rcGrapic.left	= 0;
	rcGrapic.right	= CMCICardCommonSetting::Instance().m_szGraph.cx;
	rcGrapic.top	= 0;
	rcGrapic.bottom = CMCICardCommonSetting::Instance().m_szGraph.cy;
	//07.08.27
#else
	rcGrapic.left	= 0;
	rcGrapic.right	= m_szGraph.cx;
	rcGrapic.top	= m_ptMobileUScale.y;
	rcGrapic.bottom = rcGrapic.top + m_szUScale.cy;
#endif

	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic);

	if ( pLabelControler->m_boolShowMobileUScale == TRUE )
	{
		m_UpdateRectsImage.Add( rcImage );
	}
	else
	{
		m_UpdateRectsMobileUScale.Add( rcImage );
	}
	//-----------------------------------------------------
	//======2007.08.20_任意方向移动标尺========================

	pLabelControler->m_boolShowMobileUScale = boolShow;
	//	m_boolRedrawUScal = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////

	return pLabelControler->m_boolShowMobileUScale;
}



void CMobileUScaleControler::GetMobileUScaleParam(int&	nMobileUScalePixelNum, CPoint& ptMobileUScale )
{
	m_mutexLayAll.Lock();

	nMobileUScalePixelNum = m_nMobileUScalePixelNum;	// 移动标尺显示长度
	ptMobileUScale = m_ptMobileUScale;					// 移动标尺位置

	m_mutexLayAll.Unlock();
}


void CMobileUScaleControler::GetMobileUScaleParamEx(int&	nMobileUScalePixelNum, CPoint& ptMobileUScale, CPoint& ptMobileUScale2 )
{
	m_mutexLayAll.Lock();

	nMobileUScalePixelNum	= m_nMobileUScalePixelNum;	// 移动标尺显示长度
	ptMobileUScale			= m_ptMobileUScale;			// 移动标尺位置
	ptMobileUScale2			= m_ptMobileUScale2;

	m_mutexLayAll.Unlock();
}



void CMobileUScaleControler::SetMobileUScalePosEx(const CPoint ptMobileUScale, const CPoint ptMobileUScale2, const COLORREF color )
{
	if ( m_ptMobileUScale == ptMobileUScale
		&& m_ptMobileUScale2 == ptMobileUScale2 )
		return;

	m_mutexLayAll.Lock();

	m_ptMobileUScale		= ptMobileUScale;					// 移动标尺位置
	m_ptMobileUScale2		= ptMobileUScale2;

	m_mutexLayAll.Unlock();
}


void CMobileUScaleControler::GetMobileUScaleParamArray(int nIndex, int&  nMobileUScalePixelNum, CPoint& ptMobileUScale, CPoint& ptMobileUScale2 )
{
	m_mutexLayAll.Lock();

	nMobileUScalePixelNum	= m_nMobileUScalePixelNumArray[nIndex];	// 移动标尺显示长度
	ptMobileUScale			= m_ptMobileUScaleArray[nIndex];		// 移动标尺位置
	ptMobileUScale2			= m_ptMobileUScaleArray2[nIndex];

	m_mutexLayAll.Unlock();
}


void CMobileUScaleControler::SetMobileUScalePosArray( int nIndex,
													 const CPoint ptMobileUScale, const CPoint ptMobileUScale2, const COLORREF color )
{
	if ( m_ptMobileUScaleArray[nIndex] == ptMobileUScale
		&& m_ptMobileUScaleArray2[nIndex] == ptMobileUScale2 )
		return;

	m_mutexLayAll.Lock();

	m_ptMobileUScaleArray[nIndex]		= ptMobileUScale;					// 移动标尺位置
	m_ptMobileUScaleArray2[nIndex]		= ptMobileUScale2;

	m_mutexLayAll.Unlock();
}


// 16.06.12 多移动标尺实验
void CMobileUScaleControler::UpdateUScaleArray(int	 nIndex,
								 int			nMobileUScalePixelNum,	// 移动标尺显示长度
								 CString		csMobileUScaleText )	// 移动标尺显示字符
{
	m_mutexLayAll.Lock();

	m_nMobileUScalePixelNumArray[nIndex]	= nMobileUScalePixelNum;
	m_csMobileUScaleTextArray[nIndex]		= csMobileUScaleText;
	m_boolRedrawUScal = TRUE;

	m_mutexLayAll.Unlock();
}



CRect CMobileUScaleControler::LayWindowMapClip_Image_to_MobileUScale( CRect& rcImage)
{
	CRect	rcGrapic = LayWindowMapClip_Image_to_Graph( rcImage );
	//07.10.31
	CPoint	ptOrigin;
	//	ptOrigin = m_ptMobileUScale;
#ifdef NEW_SCANSCREEN_MODE
	ptOrigin.x = 0;//min( m_ptMobileUScale.x, m_ptMobileUScale2.x );
	ptOrigin.y = 0;//min( m_ptMobileUScale.y, m_ptMobileUScale2.y );
#else
	ptOrigin = m_ptMobileUScale;
#endif
	//07.10.31

	CRect	rcUScale = rcGrapic;
	rcUScale.OffsetRect( - ptOrigin );

#ifdef NEW_SCANSCREEN_MODE
	rcUScale &= CRect(CPoint(0,0), CMCICardCommonSetting::Instance().m_szGraph);
#else
	rcUScale &= CRect(CPoint(0,0), m_szUScale);
#endif
	//07.10.31

	if( !rcUScale.IsRectEmpty() )
	{
		rcGrapic = rcUScale;
		rcGrapic.OffsetRect( ptOrigin );
		rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );
	}

	return rcUScale;
}

void CMobileUScaleControler::UpdateUScaleRect(CLabelControler* pLabelControler) 
{
	m_mutexLayAll.Lock();
	CRect rcGrapic;
	CRect rcImage;
	///////////////////////////////////////////////////////

	//07.08.27
	rcGrapic.left	= 0;
	rcGrapic.right	= CMCICardCommonSetting::Instance().m_szGraph.cx;
	rcGrapic.top	= 0;
	rcGrapic.bottom = CMCICardCommonSetting::Instance().m_szGraph.cy;
	//07.08.27
	///////////////////////////////////////////////////////
	rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic);
	if (pLabelControler && pLabelControler->m_boolShowMobileUScale )
		CMCICardCommonSetting::Instance().m_UpdateRectsImage.Add( rcImage );
	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
}


long CMobileUScaleControler::UpdateUScaleMobileLay(CLabelControler* pLabelControler,COscillographControler* pOscillographControler,BOOL bReady)
{
	CRect			rcTemp;
    HRESULT			ddrval = DD_OK;

	while ( !m_UpdateRectsMobileUScale.IsEmpty() )
	{
		rcTemp = m_UpdateRectsMobileUScale.Get();

		if ( !rcTemp.IsRectEmpty() )
		{
			//m_UpdateRectsGraphBack.Add( rcTemp );
			pOscillographControler->m_UpdateRectsOscillograph.Add( rcTemp );

			if ( pLabelControler->m_boolShowMobileUScale )
			{
				CRect rcDestImageLay	= rcTemp;
				CRect rcSrcUScaleLay	= LayWindowMapClip_Image_to_MobileUScale( rcDestImageLay );

				/*
				ddrval = m_lpDDSImageLay->Blt(
					(LPRECT)rcDestImageLay,			// dest rect
					m_lpDDSMobileUScaleLay,				// src surface
					(LPRECT)rcSrcUScaleLay,			// src rect
					DDBLT_KEYSRC | DDBLT_WAIT,
					NULL);
				*/
				
				CRect rcDestGraphLay		= LayWindowMapClip_Image_to_Graph( rcDestImageLay );
				
				if ( !bReady ) return DD_OK;

				ddrval = m_lpDDSGraphBackLay->Blt(
					(LPRECT)rcDestGraphLay,			// dest rect
					m_lpDDSMobileUScaleLay,				// src surface
					(LPRECT)rcSrcUScaleLay,			// src rect
					DDBLT_KEYSRC | DDBLT_WAIT,
					NULL);
			}
		}
	}

	return ddrval;
}



BOOL CMobileUScaleControler::InitMobileUScaleLay(CLabelControler* pLabelControler,const CRect& rcScreenClientRect)
{
	Initialize();

	/////////////////////////////////////////////////
	//m_colorMobileUScaleText	= RGB(255,255,150); //2004-7-21_加强图形层显示
	//	m_colorMobileUScaleText	= g_clrUSB_MobileUScale;
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
	if (pLabelControler->m_hfontMobileUScaleText != NULL)
		DeleteObject(pLabelControler->m_hfontMobileUScaleText);

	pLabelControler->m_hfontMobileUScaleText = CreateFont(
		UScale_nFontSize, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE, 
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
		VARIABLE_PITCH,
		CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameMobileUScaleText] );
	/////////////////////////////////////////////////

	RedrawMobileUScaleLay(pLabelControler,rcScreenClientRect);

	return TRUE;
}

long CMobileUScaleControler::RedrawMobileUScaleLay(CLabelControler* pLabelControler, const CRect& rcScreenClientRect)
{
	HDC				hdc;
	HRESULT			ddrval;

	if ( m_lpDDSMobileUScaleLay == NULL ) return DD_OK;
	ddrval = m_lpDDSMobileUScaleLay->GetDC( &hdc );
	if( ddrval != DD_OK )
		return ddrval;
	/////////////////////////////////////////////////////////////////
	HBRUSH brBackBrush = CreateSolidBrush( CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrTransparent] );
	HBRUSH brBackBrush1 = CreateSolidBrush( CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrBack] );
	HPEN hPen, hPenOld;
	/////////////////////////////////////////////////////////////////

	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	CSize& m_szGraph = CMCICardCommonSetting::Instance().m_szGraph;
	//07.10.31
#ifndef NEW_SCANSCREEN_MODE
	{
		CRect rcFill = CRect(0,0,m_szUScale.cx,m_szUScale.cy);
		FillRect( hdc, (LPRECT)rcFill, brBackBrush );

		//		rcFill = CRect(0,2,m_nMobileUScalePixelNum,11);
		//		FillRect( hdc, (LPRECT)rcFill, brBackBrush1 );


		/////////////////////////////////////////////////////////////
		if(pLabelControler->m_hfontLabelText != NULL)
		{
			SelectObject(hdc, pLabelControler->m_hfontMobileUScaleText);
			SetTextColor(hdc, m_colorMobileUScaleText);
			SetBkColor(hdc, RGB(0,0,1));//m_colorBack );//m_colorTransparent

			// 2005-1-17_u标尺字符被吃掉问题
			SIZE szText;
			GetTextExtentPoint( hdc, (LPCSTR)m_csMobileUScaleText, 
				m_csMobileUScaleText.GetLength(), &szText );
			if ( szText.cx <= m_nMobileUScalePixelNum )
			{
				SetTextAlign(hdc, TA_TOP | TA_CENTER);
				TextOut(hdc, m_ptMobileUScaleText.x, m_ptMobileUScaleText.y+1,
					(LPCSTR)m_csMobileUScaleText, m_csMobileUScaleText.GetLength());
			}
			else
			{
				SetTextAlign(hdc, TA_TOP | TA_LEFT);
				TextOut(hdc, 0, m_ptMobileUScaleText.y+1,
					(LPCSTR)m_csMobileUScaleText, m_csMobileUScaleText.GetLength());
			}

			// 2005-1-17_u标尺字符被吃掉问题
			//SetTextAlign(hdc, TA_TOP | TA_CENTER);
			//TextOut(hdc, m_ptMobileUScaleText.x, m_ptMobileUScaleText.y+1,
			//	(LPCSTR)m_csMobileUScaleText, m_csMobileUScaleText.GetLength());
		}
		/////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////
		hPen = CreatePen(PS_SOLID, 0, m_colorMobileUScaleLine);//g_clrUSB_MobileUScaleLine);
		hPenOld = (HPEN)SelectObject(hdc, hPen);

#ifdef GRAPHIC_BLACK
		CPoint ptPos0(1,6);
		CPoint ptPos1(1,6);
		int		nStart	= 1;

		ptPos0.x = nStart;
		ptPos1.x = ptPos0.x + m_nMobileUScalePixelNum;

		MoveToEx ( hdc, ptPos0.x, ptPos0.y-2, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y-2 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y+2, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+2 );

		MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-6, NULL );
		LineTo ( hdc, ptPos0.x-1, ptPos0.y+7 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y-6, NULL );
		LineTo ( hdc, ptPos0.x, ptPos0.y+7 );
		MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-6, NULL );
		LineTo ( hdc, ptPos0.x+1, ptPos0.y+7 );

		MoveToEx ( hdc, ptPos1.x-1, ptPos1.y-6, NULL );
		LineTo ( hdc, ptPos1.x-1, ptPos1.y+7 );
		MoveToEx ( hdc, ptPos1.x, ptPos1.y-6, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+7 );
		MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-6, NULL );
		LineTo ( hdc, ptPos1.x+1, ptPos1.y+7 );
#else
		CPoint ptPos0(0,4);
		CPoint ptPos1(0,4);
		int		nStart	= 0;

		ptPos0.x = nStart;
		ptPos1.x = ptPos0.x + m_nMobileUScalePixelNum;

		MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+1 );

		MoveToEx ( hdc, ptPos0.x, ptPos0.y-4, NULL );
		LineTo ( hdc, ptPos0.x, ptPos0.y+5 );
		MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-4, NULL );
		LineTo ( hdc, ptPos0.x+1, ptPos0.y+5 );

		MoveToEx ( hdc, ptPos1.x-1, ptPos1.y-4, NULL );
		LineTo ( hdc, ptPos1.x-1, ptPos1.y+5 );
		MoveToEx ( hdc, ptPos1.x, ptPos1.y-4, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+5 );
#endif

		int		nScale, i;
		double	dblStep = m_nUScalePixelNum / 5.0;

		if (m_nUScalePixelNum > 0)
			for ( nScale = m_nUScalePixelNum;
				nScale<m_nMobileUScalePixelNum;
				nScale += m_nUScalePixelNum )
			{
#ifdef GRAPHIC_BLACK
				ptPos0.x = nStart + nScale;

				MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-5, NULL );
				LineTo ( hdc, ptPos0.x-1, ptPos0.y+6 );
				MoveToEx ( hdc, ptPos0.x, ptPos0.y-5, NULL );
				LineTo ( hdc, ptPos0.x, ptPos0.y+6 );
				MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-5, NULL );
				LineTo ( hdc, ptPos0.x+1, ptPos0.y+6 );
#else
				ptPos0.x = nStart + nScale;

				MoveToEx ( hdc, ptPos0.x, ptPos0.y-3, NULL );
				LineTo ( hdc, ptPos0.x, ptPos0.y+4 );
				MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-3, NULL );
				LineTo ( hdc, ptPos0.x+1, ptPos0.y+4 );
#endif
			}

			for (i=1; i<1024; i++)
			{
				ptPos0.x = (int)(nStart + dblStep * i + 0.5);

				if (ptPos0.x >= m_nMobileUScalePixelNum)
					break;

#ifdef GRAPHIC_BLACK
				MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-5, NULL );
				LineTo ( hdc, ptPos0.x-1, ptPos0.y );
				MoveToEx ( hdc, ptPos0.x, ptPos0.y-5, NULL );
				LineTo ( hdc, ptPos0.x, ptPos0.y );
				MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-5, NULL );
				LineTo ( hdc, ptPos0.x+1, ptPos0.y );
#else
				MoveToEx ( hdc, ptPos0.x, ptPos0.y-2, NULL );
				LineTo ( hdc, ptPos0.x, ptPos0.y );
				MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-2, NULL );
				LineTo ( hdc, ptPos0.x+1, ptPos0.y );
#endif
			}
	}
#else
	{
		CRect rcFill = CRect(0,0,m_szGraph.cx,m_szGraph.cy);
		FillRect( hdc, (LPRECT)rcFill, brBackBrush );

		/////////////////////////////////////////////////////////////
		//		if(m_hfontMobileUScaleText != NULL)
		if(pLabelControler->m_hfontLabelText != NULL)
		{
			//			SelectObject(hdc, m_hfontMobileUScaleText);
			SelectObject(hdc, pLabelControler->m_hfontLabelText);
			SetTextColor(hdc, CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrMobileUScaleText]);
			//			SetBkColor(hdc, RGB(0,0,1) );//m_colorTransparent);
			SetBkColor(hdc, CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrTransparent]);
			SetTextAlign(hdc, TA_TOP | TA_LEFT);
			// 			if( m_sizeReso.cx > 2047 )
			// 				hPen = CreatePen(PS_SOLID, 6, m_sysOpt.color[SysOpt_clrMobileUScaleLine]);
			// 			else
			// 				hPen = CreatePen(PS_SOLID, 3, m_sysOpt.color[SysOpt_clrMobileUScaleLine]);
			hPen = CreatePen(PS_SOLID, 	CMCICardOption::Instance().m_sysOpt.nLineWidth, CMCICardOption::Instance().m_sysOpt.color[SysOpt_clrMobileUScaleLine]);
			hPenOld = (HPEN)SelectObject(hdc, hPen);

			CPoint pt, pt1, pt2, pt31, pt32, pt41, pt42;
			SIZE szTextOut;
			CRect rc = rcScreenClientRect;
			CPoint ptArray1[MobileUScaleCounts +1], ptArray2[MobileUScaleCounts +1];
			CString strArray[MobileUScaleCounts +1];
			ConvertPtArray(ptArray1, ptArray2, strArray);

			for(int i=0; i<MobileUScaleCounts +1; i++ )
			{
				if( ptArray1[i].x < 0 || ptArray2[i].x < 0 )
					continue;
				GetTextExtentPoint( hdc, (LPCSTR)strArray[i], strArray[i].GetLength(), &szTextOut );

				pt1 = ptArray1[i];
				pt2 = ptArray2[i];
				pt.x = pt1.x + ( pt2.x - pt1.x ) / 2;

				if( abs( pt1.x - pt2.x ) < 10 )
				{
					// 垂直
					pt.x = pt1.x + 4;
					if( pt.x + szTextOut.cx > m_szGraph.cx )
						pt.x = pt1.x - 2 - szTextOut.cx;
					pt.y = pt1.y + ( pt2.y - pt1.y - szTextOut.cy ) / 2;
				}
				else if( abs( pt1.y - pt2.y ) < 10 )
				{
					// 水平
					pt.x = pt1.x + ( pt2.x - pt1.x - szTextOut.cx ) / 2;
					pt.y = pt1.y + 4;
					if( pt.x + szTextOut.cx > max( pt1.x, pt2.x ) )
						pt.x = min( pt1.x, pt2.x );
					if( pt.x + szTextOut.cx > m_szGraph.cx )
						pt.x = m_szGraph.cx - 2 - szTextOut.cx;
					if( pt.y + szTextOut.cy > m_szGraph.cy )
						pt.y = pt1.y - 2 - szTextOut.cy;
				}
				else if( (pt1.x < pt2.x && pt1.y > pt2.y)
					|| (pt1.x > pt2.x && pt1.y < pt2.y ) )
				{
					pt.y = pt1.y + ( pt2.y - pt1.y ) / 2;
					if( pt.x + szTextOut.cx > m_szGraph.cx )
					{
						pt.x = pt.x -szTextOut.cx;
						pt.y = pt.y-szTextOut.cy;
						if( pt.y < 0 )
							pt.y = 0;
					}
					if( pt.y + szTextOut.cy > m_szGraph.cy )
						pt.y = pt1.y - 2 - szTextOut.cy;
				}
				else
				{
					pt.y = pt1.y + ( pt2.y - pt1.y ) / 2 - szTextOut.cy;
					if( pt.x + szTextOut.cx > m_szGraph.cx )
					{
						pt.x = pt.x -szTextOut.cx;
						pt.y = pt.y +szTextOut.cy;
					}
					if( pt.y + szTextOut.cy > m_szGraph.cy )
						pt.y = m_szGraph.cy -2 -szTextOut.cy;
				}

				// 斜线的两端刻度位置
				double dK1, dK2, dB1, dB2, dA, dB, dC;
				dK1 = dK2 = 0;
				int nDiff = 0;
				if( (pt2.x == pt1.x) || (abs( pt1.x - pt2.x ) < 15) )
				{
					// 垂直
					pt31.x	= pt1.x -8;
					pt31.y	= pt1.y;
					pt32.x	= pt1.x +8;
					pt32.y	= pt1.y;
					pt41.x	= pt2.x -8;
					pt41.y	= pt2.y;
					pt42.x	= pt2.x +8;
					pt42.y	= pt2.y;
				}
				else
				{
					if( abs( pt1.y - pt2.y ) < 15 )	// 水平
					{
						pt31.x	= pt1.x;
						pt31.y	= pt1.y -8;
						pt32.x	= pt1.x;
						pt32.y	= pt1.y +8;
						pt41.x	= pt2.x;
						pt41.y	= pt2.y -8;
						pt42.x	= pt2.x;
						pt42.y	= pt2.y +8;
					}
					else
					{
						// 刻度所在直线方程与标尺所在直线方程垂直
						// 所以，两直线方程的K积为-1，即K1 * K2 = -1
						dK1 = 1.0 *(pt2.y -pt1.y) / (pt2.x -pt1.x);
						dK2	= -1 / dK1;				// 标尺刻度所在直线方程的斜率
						dB1	= pt1.y -dK2 *pt1.x;	// 标尺一端刻度所在直线方程的截距
						dB2	= pt2.y -dK2 *pt2.x;	// 标尺另一端刻度所在直线方程的截距
						// 设刻度总长为10
						// 根据SQRT( (X2-X1)*(X2-X1) + (Y2-Y1)*(Y2-Y1) ) = 10
						// X1,Y1为标尺一端的坐标，而所求刻度两个点所在的直线方程已知
						// 最终由AX*X +BX +C = 0标准一元二次方程求解，求得刻度两个点的X坐标
						// A = K2*K2 +1;
						// B = 2*K2*B1 -2*K2*y1 -2x1;
						// C = x1*x1 + (B1-y1)*(B1-y1) -10*10;
						// pt3.x = [-B +- sqrt(B*B -4AC)] / 2A;
						dA	= dK2 * dK2 +1;
						dB	= 2*dK2*dB1 -2*dK2*pt1.y -2*pt1.x;
						dC	= pt1.x*pt1.x + (dB1 -pt1.y)*(dB1 -pt1.y) -100;
						pt31.x	= ( -dB + sqrt(dB*dB -4*dA*dC) ) / (2*dA);
						pt32.x	= ( -dB - sqrt(dB*dB -4*dA*dC) ) / (2*dA);
						// 保证对称
						if( pt31.x == pt1.x )
							pt31.x = pt1.x *2 -pt32.x;
						else
						{
							if( abs(pt1.x -pt31.x) < abs(pt1.x -pt32.x) )
								nDiff = abs(pt1.x -pt31.x);
							else
								nDiff = abs(pt1.x -pt32.x);
							pt31.x	= pt1.x -nDiff;
							pt32.x	= pt1.x +nDiff;
						}
						pt31.y	= dK2 *pt31.x +dB1;
						pt32.y	= dK2 *pt32.x +dB1;
						// 另一端
						dB	= 2*dK2*dB2 -2*dK2*pt2.y -2*pt2.x;
						dC	= pt2.x*pt2.x + (dB2 -pt2.y)*(dB2 -pt2.y) -100;
						pt41.x	= ( -dB + sqrt(dB*dB -4*dA*dC) ) / (2*dA);
						pt42.x	= ( -dB - sqrt(dB*dB -4*dA*dC) ) / (2*dA);
						// 保证对称
						if( pt41.x == pt2.x )
							pt41.x = pt2.x *2 -pt42.x;
						else
						{
							if( abs(pt2.x -pt41.x) < abs(pt2.x -pt42.x) )
								nDiff = abs(pt2.x -pt41.x);
							else
								nDiff = abs(pt2.x -pt42.x);
							pt41.x	= pt2.x -nDiff;
							pt42.x	= pt2.x +nDiff;
						}
						pt41.y	= dK2 *pt41.x +dB2;
						pt42.y	= dK2 *pt42.x +dB2;
					}
				}
				TextOut(hdc, pt.x, pt.y, (LPCSTR)strArray[i], strArray[i].GetLength());
				/////////////////////////////////////////////////////////

				MoveToEx ( hdc, ptArray1[i].x, ptArray1[i].y, NULL );
				LineTo ( hdc, ptArray2[i].x, ptArray2[i].y );
				MoveToEx ( hdc, pt31.x, pt31.y, NULL );
				LineTo ( hdc, pt32.x, pt32.y );
				MoveToEx ( hdc, pt41.x, pt41.y, NULL );
				LineTo ( hdc, pt42.x, pt42.y );
			}
			SelectObject(hdc, hPenOld);
		}
	}
#endif
	//07.10.31

	SelectObject(hdc, hPenOld);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	DeleteObject( hPen );
	DeleteObject( brBackBrush );
	DeleteObject( brBackBrush1 );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSMobileUScaleLay == NULL ) return DD_OK;
	m_lpDDSMobileUScaleLay->ReleaseDC(hdc);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	CRect rcGrapic;

	//07.10.31
#ifdef NEW_SCANSCREEN_MODE
	//	rcGrapic.left	= min( m_ptMobileUScale.x, m_ptMobileUScale2.x );
	//	rcGrapic.right	= max( m_ptMobileUScale.x, m_ptMobileUScale2.x );
	//	rcGrapic.top	= min( m_ptMobileUScale.y, m_ptMobileUScale2.y );
	//	rcGrapic.bottom = max( m_ptMobileUScale.y, m_ptMobileUScale2.y );
	rcGrapic.left	= 0;
	rcGrapic.right	= m_szGraph.cx;
	rcGrapic.top	= 0;//min( m_ptMobileUScale.y, m_ptMobileUScale2.y );
	rcGrapic.bottom = m_szGraph.cy;//max( m_ptMobileUScale.y, m_ptMobileUScale2.y );
#else
	rcGrapic.left	= 0;
	rcGrapic.right	= m_szUScale.cx;
	rcGrapic.top	= m_ptMobileUScale.y;
	rcGrapic.bottom = rcGrapic.top + m_szUScale.cy;
#endif
	//07.10.31

	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

	if ( pLabelControler->m_boolShowMobileUScale == TRUE )
		CMCICardCommonSetting::Instance().m_UpdateRectsImage.Add( rcImage );
	/////////////////////////////////////////////////////////////////

	return DD_OK;
}


BOOL CMobileUScaleControler::CreateSurface(BOOL boolUseSysMemUScaleLay ) 
{
	CSize &m_szGraph = CMCICardCommonSetting::Instance().m_szGraph;

	DDSURFACEDESC			ddsd;
	HRESULT					ddrval;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize			= sizeof(ddsd);
	ddsd.dwFlags		= DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
	//07.10.31
	//	ddsd.dwWidth		= szUScale.cx;
	//	ddsd.dwHeight		= szUScale.cy;
#ifdef NEW_SCANSCREEN_MODE
	ddsd.dwWidth		= m_szGraph.cx;
	ddsd.dwHeight		= m_szGraph.cy;
#else
	ddsd.dwWidth		= m_szUScale.cx;
	ddsd.dwHeight		= m_szUScale.cy;
#endif

	//07.10.31
	if( boolUseSysMemUScaleLay )
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
	ddrval = m_lpDD->CreateSurface( &ddsd, &m_lpDDSMobileUScaleLay, NULL );
	if ( ddrval != DD_OK )
	{
		DebugMessage("Create UScaleLay Surface failed err=%d", ddrval);
		return FALSE;
	}

	return TRUE;
}


void CMobileUScaleControler::UpdateMAG(CString	csMobileUScaleText)	// 移动标尺显示字符
{
	m_mutexLayAll.Lock();

	m_csMobileUScaleText	= csMobileUScaleText;
	m_boolRedrawUScal = TRUE;

	m_mutexLayAll.Unlock();
}