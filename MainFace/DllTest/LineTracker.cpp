//CLineTracker files

#include "stdafx.h"
#include "LineTracker.h"
#include <math.h>

//CLineTracker
void CLineTracker::GetHandleRect(CPoint& pPoint, CRect& pRect)
{
	pRect.left = pPoint.x-5;
	pRect.right = pPoint.x+5;
	pRect.top = pPoint.y-5;
	pRect.bottom = pPoint.y+5;
}

void CLineTracker::Draw( CDC* pDC )
{
	// set initial DC state
	VERIFY(pDC->SaveDC() != 0);
	pDC->SetMapMode(MM_TEXT);

	CPen* pOldPen = NULL;
	CBrush* pOldBrush = NULL;
	
	// draw lines
	CPen pen( PS_SOLID, 2, RGB(0XFF, 0XFF, 0) );
	pOldPen = (CPen*)pDC->SelectObject( &pen );
	pOldBrush = (CBrush*)pDC->SelectStockObject(WHITE_BRUSH);
	
	int nOldROP = pDC->SetROP2( R2_XORPEN );

//draw line
	pDC->MoveTo( m_rect.TopLeft() );
	pDC->LineTo( m_rect.BottomRight() );
// draw resize handles
	CRect rect;
	GetHandleRect( m_rect.TopLeft(), rect);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
	GetHandleRect( m_rect.BottomRight(), rect);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);

	pDC->SetROP2(nOldROP);

// cleanup pDC state
	if (pOldPen != NULL)
		pDC->SelectObject(pOldPen);
	if (pOldBrush != NULL)
		pDC->SelectObject(pOldBrush);
	m_rectLast = m_rect;
	VERIFY(pDC->RestoreDC(-1));
}

void CLineTracker::DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd)
{
	// set initial DC state
	VERIFY(pDC->SaveDC() != 0);
	pDC->SetMapMode(MM_TEXT);

	CPen* pOldPen = NULL;
	CBrush* pOldBrush = NULL;
	int nOldROP;
	CRect rect;
	CRect oldrect;
	// draw lines
	CPen pen( PS_SOLID, 2, RGB(0XFF, 0XFF, 0) );
	pOldPen = (CPen*)pDC->SelectObject( &pen );
	pOldBrush = (CBrush*)pDC->SelectStockObject(WHITE_BRUSH);
	if (m_bFinalErase || !m_bErase)
	{
		nOldROP = pDC->SetROP2( R2_XORPEN );

		pDC->MoveTo( m_rectLast.TopLeft() );
		pDC->LineTo( m_rectLast.BottomRight() );
		GetHandleRect( m_rectLast.TopLeft(), rect);
		pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
		GetHandleRect( m_rectLast.BottomRight(), rect);
		pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
	
		pDC->MoveTo( lpRect->left, lpRect->top );
		pDC->LineTo( lpRect->right, lpRect->bottom );
		oldrect = CRect(*lpRect);
		GetHandleRect( oldrect.TopLeft(), rect);
		pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
		GetHandleRect( oldrect.BottomRight(), rect);
		pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);

		pDC->SetROP2(nOldROP);
	}

	// cleanup pDC state
	if (pOldBrush != NULL)
		pDC->SelectObject(pOldBrush);
	if (pOldPen != NULL )
		pDC->SelectObject(pOldPen);
	VERIFY(pDC->RestoreDC(-1));

	m_rectLast = CRect( *lpRect );
}

void CLineTracker::AdjustRect(int nHandle, LPRECT lpRect)
{
	CRectTracker::AdjustRect(nHandle, lpRect);
}

void CLineTracker::OnChangedRect(const CRect& rectOld)
{
	CRectTracker::OnChangedRect(rectOld);
}

UINT CLineTracker::GetHandleMask() const
{
	return CRectTracker::GetHandleMask();
}

int CLineTracker::HitTest(CPoint point)
{
	CRect rect;
	UINT mask = GetHandleMask();

	// see if hit anywhere inside the tracker
	GetTrueRect(&rect);
	if ( (m_sizeMin.cx>0) && (m_sizeMin.cy>0 )&& !rect.PtInRect(point))
		return hitNothing;  // totally missed
	// see if we hit a handle
	GetHandleRect(m_rect.TopLeft(), rect);
	if (rect.PtInRect(point))
		return (TrackerHit)0;
	GetHandleRect(m_rect.BottomRight(), rect);
	if (rect.PtInRect(point))
		return (TrackerHit)2;

	// last of all, check for non-hit outside of object, between resize handles
	double l1 = sqrt(	pow(point.x-m_rect.left, 2)+pow(point.y-m_rect.top, 2));
	double l2 = sqrt(	pow(point.x-m_rect.right, 2)+pow(point.y-m_rect.bottom, 2));
	double l3 = sqrt(	pow(m_rect.Width(),  2)+pow(m_rect.Height(), 2) );

	if( (l1+l2)/l3 > 1.01 || (l1+l2)/l3 < 0.99 )
		return hitNothing;
	return hitMiddle;   // no handle hit, but hit object (or object border)
}

BOOL CLineTracker::Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert, CWnd* pWndClipTo)
{
	// perform hit testing on the handles
	int nHandle = HitTest(point);
	if (nHandle < 0)
	{
		// didn't hit a handle, so just return FALSE
		return FALSE;
	}

	// otherwise, call helper function to do the tracking
	m_bAllowInvert = bAllowInvert;
	return TrackHandle(nHandle, pWnd, point, pWndClipTo);
}

BOOL CLineTracker::SetCursor(CWnd* pWnd, UINT nHitTest)
{
	// trackers should only be in client area
	if (nHitTest != HTCLIENT)
		return FALSE;

	// convert cursor position to client co-ordinates
	CPoint point;
	GetCursorPos(&point);
	pWnd->ScreenToClient(&point);
	point.Offset( pWnd->GetScrollPos(SB_HORZ), pWnd->GetScrollPos(SB_VERT) );

	// do hittest and normalize hit
	int nHitTest_ = HitTest(point);
	if( nHitTest_ < 0 )
		return FALSE;
	if( nHitTest_ == 0 )
		::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
	else if( nHitTest_ == 2 )
		::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
	else if( nHitTest_ == 8 )
		::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));

	return TRUE;
}

CRectTrackerEx::CRectTrackerEx()
:CRectTracker()
{ 
	m_dx = 0;
	m_dy = 0;
	m_doubleScale = 1;
	m_nStyle = CRectTracker::dottedLine|CRectTracker::resizeInside;
}

CRectTrackerEx::CRectTrackerEx(LPCRECT lpSrcRect, UINT nStyle)
:CRectTracker( lpSrcRect, nStyle )
{ 
	m_dx = 0;
	m_dy = 0;
	m_doubleScale = 1;
}

void CRectTrackerEx::Draw( CDC* pDC )
{
	CRectTracker::Draw( pDC );
}

int CRectTrackerEx::HitTest(CPoint point)
{
/*	CRect rect;
	// see if hit anywhere inside the tracker
	GetRealRect(rect);
	if ( !rect.PtInRect(point))
		return hitNothing;  // totally missed
	return hitMiddle;   // no handle hit, but hit object (or object border)
*/	return CRectTracker::HitTest( point );
}

BOOL CRectTrackerEx::SetCursor(CWnd* pWnd, UINT nHitTest)
{
/*	// trackers should only be in client area
	if (nHitTest != HTCLIENT)
		return FALSE;

	// convert cursor position to client co-ordinates
	CPoint point;
	GetCursorPos(&point);
	pWnd->ScreenToClient(&point);
	point.Offset( pWnd->GetScrollPos(SB_HORZ), pWnd->GetScrollPos(SB_VERT) );

	// do hittest and normalize hit
	int nHandle = HitTest(point);
	if (nHandle < 0)
		return FALSE;

	if( (nHandle == 0) || (nHandle == 2) )
		::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
	else if( (nHandle == 1) || (nHandle == 3) )
		::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
	else if( (nHandle == 4) || (nHandle == 6) )
		::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
	else if( (nHandle == 5) || (nHandle == 7) )
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
	else if( nHandle == 8 )
		::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));

	return TRUE;*/
	return CRectTracker::SetCursor( pWnd, nHitTest );
}

BOOL CRectTrackerEx::Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert, CWnd* pWndClipTo)
{
	return CRectTracker::Track( pWnd, point, bAllowInvert, pWndClipTo );
}

void CRectTrackerEx::UpdateRect( int dx, int dy, double ds )
{
	if( ds <=0 )
		return;
	m_dx = dx;
	m_dy = dy;
	m_doubleScale	= ds;
	m_rect.top		= (long)(m_rect.top*ds);
	m_rect.left		= (long)(m_rect.left*ds);
	m_rect.bottom	= (long)(m_rect.bottom*ds);
	m_rect.right	= (long)(m_rect.right*ds);
	m_rect.OffsetRect( dx, dy );
}

void CRectTrackerEx::SetRect( const CRect& rect )
{
	m_rect = rect;
}

void CRectTrackerEx::GetRect( CRect& rect )
{
	rect = m_rect;
}

void CRectTrackerEx::SetRealRect( const CRect& rect )
{
}

void CRectTrackerEx::GetRealRect( CRect& rect )
{
	rect = m_rect;
	rect.top	= (long)(rect.top /m_doubleScale);
	rect.left	= (long)(rect.left /m_doubleScale);
	rect.right	= (long)(rect.right /m_doubleScale);
	rect.bottom	= (long)(rect.bottom /m_doubleScale);
	rect.OffsetRect( -m_dx, -m_dy );
}

void CLineTracker::SetLastRect(const CRect &rect)
{
	m_rectLast = rect;
}
