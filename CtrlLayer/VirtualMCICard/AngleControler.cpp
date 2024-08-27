#include "stdafx.h"
#include "AngleControler.h"
#include "RectConvertor.h"
#include "MCICardParam.h"
#include "MCICardOption.h"
#include "MCICardCommonSetting.h"
#include "LabelControler.h"
#include "OscillographControler.h"

CAngleControler::CAngleControler(CCriticalSection& cs,LPDIRECTDRAW2& lpDD,LPDIRECTDRAWSURFACE& lpDDSGraphBackLay):
	m_mutexLayAll(cs),m_lpDD(lpDD),m_lpDDSGraphBackLay(lpDDSGraphBackLay)
{
	m_lpDDSAngleLay		= NULL;		// Offscreen surface Angle Lay

	m_boolRedrawAngle	= FALSE;
	m_boolShowAngle		= TRUE;

	m_hfontAngleText	= NULL;

	m_lpDDSAngleLay		= NULL;		// Offscreen surface Angle Lay

	m_plistAngle		= new ListAngle;
	m_nAngleCounts		= 0;
}


CAngleControler::~CAngleControler()
{
	POSITION pos;
	while( (pos = m_plistAngle->GetHeadPosition()) )
	{
		AngleMeasure* pItem = m_plistAngle->GetAt( pos );
		m_plistAngle->RemoveHead();
		delete pItem;
	}
	delete m_plistAngle;
}

void CAngleControler::ReleaseLay()
{
	if(m_lpDDSAngleLay != NULL )
	{
		m_lpDDSAngleLay->Release();
		m_lpDDSAngleLay = NULL;
	}
}

void CAngleControler::Close()
{
	if (m_hfontAngleText != NULL)
	{
		DeleteObject(m_hfontAngleText);
		m_hfontAngleText = NULL;
	}
}

BOOL CAngleControler::ShowAngle(BOOL boolShow)
{
	if ( m_boolShowAngle == boolShow )
		return m_boolShowAngle;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	CRectManager& m_UpdateRectsImage = CMCICardCommonSetting::Instance().m_UpdateRectsImage;
	CSize& m_szGraph = CMCICardCommonSetting::Instance().m_szGraph;

	CRect rcGrapic = CRect( 0, m_szGraph.cx, 0, m_szGraph.cy );
	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

	if ( m_boolShowAngle == TRUE )
	{	
		m_UpdateRectsImage.Add( rcImage );
	}
	else
	{
		m_UpdateRectsAngle.Add( rcImage );
	}

	m_boolShowAngle = boolShow;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////

	return m_boolShowAngle;
}

void CAngleControler::ChangeAngle( int nIndex, BOOL bAdd )
{
	if( bAdd )
	{
		if( m_plistAngle->GetCount() > AngleMeasureCounts -1 )
			return;
		AngleMeasure* pAngle = new AngleMeasure();
		ZeroMemory( pAngle, sizeof(AngleMeasure) );
		pAngle->nAngleStatus	= 1;	// 生成
		pAngle->nIndex			= nIndex;
		m_plistAngle->AddTail( pAngle );
		m_nAngleCounts = m_plistAngle->GetCount();
	}
	else
	{
		POSITION pos = m_plistAngle->GetHeadPosition();
		while( pos )
		{
			AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
			if( pAngle->nIndex == nIndex )
			{
				m_plistAngle->RemoveAt( pos );
				delete pAngle;
				break;
			}
			m_plistAngle->GetNext( pos );
		}
		m_nAngleCounts = m_plistAngle->GetCount();
	}
	m_boolRedrawAngle = TRUE;
}

void CAngleControler::ReArrangeAngles()
{
	m_mutexLayAll.Lock();

	int nIndex = 0;
	POSITION pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		pAngle->nIndex = nIndex++;
		m_plistAngle->GetNext( pos );
	}
	m_nAngleCounts = m_plistAngle->GetCount();

	m_boolRedrawAngle = TRUE;
	m_mutexLayAll.Unlock();
}

void CAngleControler::ResetAngles()
{
	m_mutexLayAll.Lock();

	POSITION pos;
	while( (pos = m_plistAngle->GetHeadPosition()) )
	{
		AngleMeasure* pItem = m_plistAngle->GetAt( pos );
		m_plistAngle->RemoveHead();
		delete pItem;
	}
	m_nAngleCounts = 0;

	m_boolRedrawAngle = TRUE;
	m_mutexLayAll.Unlock();
}

void CAngleControler::SetAngleParam( int nIndex, AngleMeasure* pAM )
{
	m_mutexLayAll.Lock();

	CSize& m_szGraph = CMCICardCommonSetting::Instance().m_szGraph;
	CRect rcGrapic = CRect( 0, m_szGraph.cx, 0, m_szGraph.cy );

	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );
	CRectManager& m_UpdateRectsImage = CMCICardCommonSetting::Instance().m_UpdateRectsImage;
	if ( m_boolShowAngle )
		m_UpdateRectsImage.Add( rcImage );

	POSITION pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		if( pAngle->nIndex == nIndex )
		{
			pAngle->AP				= pAM->AP;
			pAngle->APbak			= pAM->APbak;
			pAngle->AO				= pAM->AO;
			pAngle->nAngleStatus	= pAM->nAngleStatus;
			pAngle->bStart			= pAM->bStart;
			pAngle->bCenter			= pAM->bCenter;
			pAngle->bEnd			= pAM->bEnd;
			pAngle->bAngleHitTest	= pAM->bAngleHitTest;
			break;
		}
		m_plistAngle->GetNext( pos );
	}

	m_boolRedrawAngle = TRUE;
	m_mutexLayAll.Unlock();
}

CRect CAngleControler::LayWindowMapClip_Image_to_Angle( CRect& rcImage )
{
	CRect	rcGrapic = LayWindowMapClip_Image_to_Graph( rcImage);
	CSize& m_szGraph = CMCICardCommonSetting::Instance().m_szGraph;

	CRect	rcAngle = rcGrapic;
	rcAngle &= CRect(0, 0, m_szGraph.cx, m_szGraph.cy);

	if( !rcAngle.IsRectEmpty() )
	{
		rcGrapic = rcAngle;
		rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );
	}

	return rcAngle;
}

long CAngleControler::RedrawAngleLay( CLabelControler* pLabelControler, const CRect& rcScreenClientRect )
{
	HDC				hdc;
	HRESULT			ddrval;

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSAngleLay == NULL ) return DD_OK;
	ddrval = m_lpDDSAngleLay->GetDC( &hdc );
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
	CRect rcFill = CRect(0,0,m_szGraph.cx,m_szGraph.cy);
	FillRect( hdc, (LPRECT)rcFill, brBackBrush );
	HPEN hPen, hPenBK, hPenOld;
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	SetBkColor(hdc, clrBack);
	SetTextAlign(hdc, TA_TOP | TA_LEFT);
	hPenBK = CreatePen( PS_SOLID, CMCICardOption::Instance().m_sysOpt.nLineWidth, clrBack );

	POSITION pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );

		if (m_hfontAngleText != NULL)
			DeleteObject(m_hfontAngleText);
		m_hfontAngleText = CreateFont(
			pAngle->AO.nSize, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			VARIABLE_PITCH,
			"Arial" );
		SelectObject(hdc, m_hfontAngleText);
		SetTextColor(hdc, pAngle->AO.clrText);

		hPen = CreatePen(PS_SOLID, 	CMCICardOption::Instance().m_sysOpt.nLineWidth, pAngle->AO.clrLine);
		if( pAngle->nAngleStatus > 2 )
		{
			// 整体移位
			hPenOld = (HPEN)SelectObject(hdc, hPenBK);
			MoveToEx( hdc, pAngle->APbak.ptS.x, pAngle->APbak.ptS.y, NULL );
			LineTo( hdc, pAngle->APbak.ptC.x, pAngle->APbak.ptC.y );
			LineTo( hdc, pAngle->APbak.ptE.x, pAngle->APbak.ptE.y );
			SelectObject(hdc, hPenOld);

			hPenOld = (HPEN)SelectObject(hdc, hPen);
			MoveToEx( hdc, pAngle->AP.ptS.x, pAngle->AP.ptS.y, NULL );
			LineTo( hdc, pAngle->AP.ptC.x, pAngle->AP.ptC.y );
			LineTo( hdc, pAngle->AP.ptE.x, pAngle->AP.ptE.y );
			SelectObject(hdc, hPenOld);
		}
		else
		{
			// 两条射线生成中
			if( pAngle->bStart )
			{
				MoveToEx( hdc, pAngle->AP.ptS.x, pAngle->AP.ptS.y, NULL );
				if( pAngle->AP.ptC.x > 0 && pAngle->AP.ptC.y > 0 )
				{
					// 仅有起始点，中心点未定
					if( pAngle->APbak.ptC.x > 0 && pAngle->APbak.ptC.y > 0 )
					{
						hPenOld = (HPEN)SelectObject(hdc, hPenBK);
						LineTo( hdc, pAngle->APbak.ptC.x, pAngle->APbak.ptC.y );
						SelectObject(hdc, hPenOld);
						MoveToEx( hdc, pAngle->AP.ptS.x, pAngle->AP.ptS.y, NULL );
					}

					hPenOld = (HPEN)SelectObject(hdc, hPen);
					LineTo( hdc, pAngle->AP.ptC.x, pAngle->AP.ptC.y );
					SelectObject(hdc, hPenOld);
				}
				if( pAngle->bCenter && pAngle->AP.ptE.x > 0 && pAngle->AP.ptE.y > 0 )
				{
					// 有了起始点、中心点，终点未定
					if( pAngle->APbak.ptE.x > 0 && pAngle->APbak.ptE.y > 0 )
					{
						hPenOld = (HPEN)SelectObject(hdc, hPenBK);
						MoveToEx( hdc, pAngle->APbak.ptC.x, pAngle->APbak.ptC.y, NULL );
						LineTo( hdc, pAngle->APbak.ptE.x, pAngle->APbak.ptE.y );
						SelectObject(hdc, hPenOld);
						MoveToEx( hdc, pAngle->AP.ptC.x, pAngle->AP.ptC.y, NULL );
					}

					hPenOld = (HPEN)SelectObject(hdc, hPen);
					LineTo( hdc, pAngle->AP.ptE.x, pAngle->AP.ptE.y );
					SelectObject(hdc, hPenOld);
				}
				if( pAngle->bEnd && pAngle->AP.ptC.x > 0 && pAngle->AP.ptC.y > 0 )
				{
					// 正在修改中心点（整体移位)
					if( pAngle->APbak.ptC.x > 0 && pAngle->APbak.ptC.y > 0 )
					{
						hPenOld = (HPEN)SelectObject(hdc, hPenBK);
						MoveToEx( hdc, pAngle->APbak.ptE.x, pAngle->APbak.ptE.y, NULL );
						LineTo( hdc, pAngle->APbak.ptC.x, pAngle->APbak.ptC.y );
						SelectObject(hdc, hPenOld);
						MoveToEx( hdc, pAngle->AP.ptE.x, pAngle->AP.ptE.y, NULL );
					}

					hPenOld = (HPEN)SelectObject(hdc, hPen);
					LineTo( hdc, pAngle->AP.ptC.x, pAngle->AP.ptC.y );
					SelectObject(hdc, hPenOld);
				}
			}
			else
			{
				if( pAngle->bCenter && pAngle->bEnd && pAngle->AP.ptC.x > 0 && pAngle->AP.ptC.y > 0 && pAngle->AP.ptE.x > 0 && pAngle->AP.ptE.y > 0 )
				{
					// 正在修改起始点
					if( pAngle->APbak.ptS.x > 0 && pAngle->APbak.ptS.y > 0 )
					{
						hPenOld = (HPEN)SelectObject(hdc, hPenBK);
						MoveToEx( hdc, pAngle->AP.ptC.x, pAngle->AP.ptC.y, NULL );
						LineTo( hdc, pAngle->APbak.ptS.x, pAngle->APbak.ptS.y );
						SelectObject(hdc, hPenOld);
					}

					hPenOld = (HPEN)SelectObject(hdc, hPen);
					MoveToEx( hdc, pAngle->AP.ptS.x, pAngle->AP.ptS.y, NULL );
					LineTo( hdc, pAngle->AP.ptC.x, pAngle->AP.ptC.y );
					LineTo( hdc, pAngle->AP.ptE.x, pAngle->AP.ptE.y );
					SelectObject(hdc, hPenOld);
				}
			}
		}
		hPenOld = (HPEN)SelectObject(hdc, hPen);
		DeleteObject( hPen );

		// 弧
		if( (pAngle->bStart && pAngle->bCenter && pAngle->AP.ptE.x > 0 && pAngle->AP.ptE.y > 0)
			|| (pAngle->bStart && pAngle->bEnd && pAngle->AP.ptC.x > 0 && pAngle->AP.ptC.y > 0)
			|| (pAngle->bCenter && pAngle->bEnd && pAngle->AP.ptS.x > 0 && pAngle->AP.ptS.y > 0) )
		{
			// Arc函数总是从起点开始，逆时针画到终点
			hPenOld = (HPEN)SelectObject(hdc, hPenBK);
			Arc( hdc, pAngle->APbak.rcBound.left, pAngle->APbak.rcBound.top, pAngle->APbak.rcBound.right, pAngle->APbak.rcBound.bottom,
				pAngle->APbak.ptArcEnd.x, pAngle->APbak.ptArcEnd.y, pAngle->APbak.ptArcStart.x, pAngle->APbak.ptArcStart.y );
			SelectObject(hdc, hPenOld);

			hPen = CreatePen(PS_SOLID, 	CMCICardOption::Instance().m_sysOpt.nLineWidth, pAngle->AO.clrArc);
			hPenOld = (HPEN)SelectObject(hdc, hPen);
			Arc( hdc, pAngle->AP.rcBound.left, pAngle->AP.rcBound.top, pAngle->AP.rcBound.right, pAngle->AP.rcBound.bottom,
				pAngle->AP.ptArcEnd.x, pAngle->AP.ptArcEnd.y, pAngle->AP.ptArcStart.x, pAngle->AP.ptArcStart.y );
			SelectObject(hdc, hPenOld);

			// 角度标注
			SetBkMode(hdc, TRANSPARENT);
			CString str;
			CSize sz;
			str.Format( "%.1f", pAngle->APbak.dAngle );
			GetTextExtentPoint(hdc, str, str.GetLength(), &sz);
			FillRect( hdc, CRect(pAngle->APbak.ptLabel.x -sz.cx/2, pAngle->APbak.ptLabel.y -sz.cy/2, pAngle->APbak.ptLabel.x +sz.cx/2, pAngle->APbak.ptLabel.y +sz.cy/2), brBackBrush );

			hPenOld = (HPEN)SelectObject(hdc, hPenBK);
			SetTextColor( hdc, clrBack );
			str.Format( "%.1f", pAngle->APbak.dAngle );
			GetTextExtentPoint(hdc, str, str.GetLength(), &sz);
			TextOut( hdc, pAngle->APbak.ptLabel.x -sz.cx/2, pAngle->APbak.ptLabel.y -sz.cy/2, (LPCSTR)str, str.GetLength() );
			SelectObject(hdc, hPenOld);

			hPenOld = (HPEN)SelectObject(hdc, hPen);
			SetTextColor( hdc, pAngle->AO.clrText );
			str.Format( "%.1f", pAngle->AP.dAngle );
			GetTextExtentPoint(hdc, str, str.GetLength(), &sz);
			TextOut( hdc, pAngle->AP.ptLabel.x -sz.cx/2, pAngle->AP.ptLabel.y -sz.cy/2, (LPCSTR)str, str.GetLength() );
			SelectObject(hdc, hPenOld);
			DeleteObject( hPen );
		}
		m_plistAngle->GetNext( pos );
	}

	/////////////////////////////////////////////////////////////////
	DeleteObject( brBackBrush );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSAngleLay == NULL ) return DD_OK;
	m_lpDDSAngleLay->ReleaseDC(hdc);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_boolShowAngle )
	{
		CRect rcGrapic = CRect(0, 0, m_szGraph.cx, m_szGraph.cy);
		CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic);

		m_UpdateRectsImage.Add( rcImage );
	}
	/////////////////////////////////////////////////////////////////

	return DD_OK;
}

BOOL CAngleControler::InitAngleLay( CLabelControler* pLabelControler,const CRect& rcScreenClientRect )
{
	if (m_hfontAngleText != NULL)
		DeleteObject(m_hfontAngleText);

	RedrawAngleLay(pLabelControler,rcScreenClientRect);

	return TRUE;
}

long CAngleControler::UpdateAngleLay(CLabelControler* pLabelControler,COscillographControler* pOscillographControler,BOOL bReady)
{
	CRect			rcTemp;
	HRESULT			ddrval = DD_OK;

	while ( !m_UpdateRectsAngle.IsEmpty() )
	{
		rcTemp = m_UpdateRectsAngle.Get();

		if ( !rcTemp.IsRectEmpty() )
		{
			pOscillographControler->m_UpdateRectsOscillograph.Add( rcTemp );

			if ( m_boolShowAngle )
			{
				CRect rcDestImageLay	= rcTemp;
				CRect rcSrcAngleLay		= LayWindowMapClip_Image_to_Angle( rcDestImageLay );
				CRect rcDestGraphLay	= LayWindowMapClip_Image_to_Graph( rcDestImageLay );

				if ( !bReady ) return DD_OK;

				ddrval = m_lpDDSGraphBackLay->Blt(
					(LPRECT)rcDestGraphLay,			// dest rect
					m_lpDDSAngleLay,				// src surface
					(LPRECT)rcSrcAngleLay,			// src rect
					DDBLT_KEYSRC | DDBLT_WAIT,
					NULL);
			}
		}
	}
	return ddrval;
}


BOOL CAngleControler::CreateSurface( BOOL boolUseSysMemAngleLay ) 
{
	CSize &m_szGraph = CMCICardCommonSetting::Instance().m_szGraph;

	DDSURFACEDESC			ddsd;
	HRESULT					ddrval;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize			= sizeof(ddsd);
	ddsd.dwFlags		= DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth		= m_szGraph.cx;
	ddsd.dwHeight		= m_szGraph.cy;
	if( boolUseSysMemAngleLay )
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
	ddrval = m_lpDD->CreateSurface( &ddsd, &m_lpDDSAngleLay, NULL );
	if ( ddrval != DD_OK )
	{
		m_lpDDSAngleLay = NULL;
		char szBuf[255];
		sprintf_s( szBuf, "Create AngleLay Surface failed err=%d", ddrval);
		OutputDebugString(szBuf);
		return FALSE;
	}

	return TRUE;
}
