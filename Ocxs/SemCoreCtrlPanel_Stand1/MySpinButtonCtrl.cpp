// MySpinButtonCtrl.cpp : implementation file
// Author : J.Y.Ma
// Last Modify : 2005.04.07

#include "stdafx.h"
#include "MySpinButtonCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySpinButtonCtrl

CMySpinButtonCtrl::CMySpinButtonCtrl()
{
	m_bActiveSpinPressed = FALSE;
	m_rcIsPressed = CRect(0,0,0,0);
}

CMySpinButtonCtrl::~CMySpinButtonCtrl()
{
	if( m_bmpUp.m_hObject != NULL )
		m_bmpUp.DeleteObject();
	if( m_bmpDown.m_hObject != NULL )
		m_bmpDown.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMySpinButtonCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CMySpinButtonCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySpinButtonCtrl message handlers

void CMySpinButtonCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//I need custom pens to draw up_down control

	COLORREF clr=::GetSysColor(COLOR_3DDKSHADOW);
	m_penDarkShadow=::CreatePen(PS_SOLID,0,clr);	

	clr=::GetSysColor(COLOR_BTNSHADOW);
	m_penShadow=::CreatePen(PS_SOLID,0,clr);

	clr=::GetSysColor(COLOR_3DHILIGHT);
	m_penLight=::CreatePen(PS_SOLID,0,clr);

	clr=::GetSysColor(COLOR_3DLIGHT);
	m_penLightShadow=::CreatePen(PS_SOLID,0,clr);

	clr=::GetSysColor(COLOR_BTNFACE);
	m_penButtonFace=::CreatePen(PS_SOLID,0,clr);

	Init();
	CSpinButtonCtrl::PreSubclassWindow();
}

void CMySpinButtonCtrl::Init()
{
	// TODO: Add your specialized creation code here
	GetRange32(m_nMinPos,m_nMaxPos);	
	if( m_nMinPos > m_nMaxPos)
	{
		int nTemp = m_nMinPos;
		m_nMinPos = m_nMaxPos;
		m_nMaxPos=nTemp;
	}
	m_nPrevPos = GetPos();
}

void CMySpinButtonCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc, rcPaintUp, rcPaintDown;
	GetClientRect( &rc );

	CBrush*		pNewBrush = CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE));
//	CBrush*		pOldBrush = dc.SelectObject(pNewBrush);
	dc.FillSolidRect( &rc, ::GetSysColor(COLOR_BTNFACE) );
	
	rcPaintUp= rc;
	rcPaintUp.bottom=rcPaintUp.top+rcPaintUp.Height()/2;
	rcPaintDown=rc;
	rcPaintDown.top=rcPaintDown.bottom-rcPaintUp.Height();

	dc.DrawFrameControl(&rcPaintUp,DFC_SCROLL,DFCS_SCROLLUP);
	dc.DrawFrameControl(&rcPaintDown,DFC_SCROLL,DFCS_SCROLLDOWN);

	if( m_bActiveSpinPressed )
	{
		//The control's position has changed, so we have to
		//draw corresponding part as pressed
		if(m_rcIsPressed.IsRectEmpty())
		{
			CPoint pt=::GetMessagePos();
			ScreenToClient(&pt);
			if(rcPaintUp.PtInRect(pt))
			{
				m_rcIsPressed = rcPaintUp;
				rcPaintUp.OffsetRect( 1, 1 );
			}
			else
			{
				if(rcPaintDown.PtInRect(pt))
				{
					m_rcIsPressed = rcPaintDown; 
					rcPaintDown.OffsetRect( 1, 1 );
				}
			}

		}
		DrawPressedRect(dc,m_rcIsPressed);
		m_bActiveSpinPressed = FALSE;
	}

	// Create an in-memory DC compatible with the
	// display DC we're using to paint
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(&dc);

	// Get the size of the bitmap
	BITMAP bmpInfo;
	m_bmpUp.GetBitmap(&bmpInfo);
	// Select the bitmap into the in-memory DC
	CBitmap* pOldBitmap = dcMemory.SelectObject(&m_bmpUp);
	// Copy the bits from the in-memory DC into the on-
	// screen DC to actually do the painting. Use the centerpoint
	// we computed for the target offset.
	dc.StretchBlt(rcPaintUp.left + 2, rcPaintUp.top + 2,
				rcPaintUp.Width()-4, rcPaintUp.Height()-4,
				&dcMemory, 0,0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	dcMemory.SelectObject(pOldBitmap);

	
	// Get the size of the bitmap
	m_bmpDown.GetBitmap(&bmpInfo);
	// Select the bitmap into the in-memory DC
	pOldBitmap = dcMemory.SelectObject(&m_bmpDown);
	// Copy the bits from the in-memory DC into the on-
	// screen DC to actually do the painting. Use the centerpoint
	// we computed for the target offset.
	dc.StretchBlt(rcPaintDown.left + 2, rcPaintDown.top + 2, 
				rcPaintDown.Width()-4, rcPaintDown.Height()-4,
				&dcMemory, 0,0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	dcMemory.SelectObject(pOldBitmap);
	// Do not call CSpinButtonCtrl::OnPaint() for painting messages
}

void CMySpinButtonCtrl::SetBitmap( UINT uIDUp, UINT uIDDown )
{
	if( m_bmpUp.m_hObject != NULL )
		m_bmpUp.DeleteObject();
	m_bmpUp.LoadBitmap( uIDUp );
	if( m_bmpDown.m_hObject != NULL )
		m_bmpDown.DeleteObject();
	m_bmpDown.LoadBitmap( uIDDown );
}


void CMySpinButtonCtrl::DrawPressedRect(CDC &dc, const CRect &rctToDown) const
{

	CRect 	rctDown=rctToDown;
	HPEN hOldPen;
	
	//Offset bitmap one pixel left and down

	dc.BitBlt(rctDown.left+1,rctDown.top+1,
	rctDown.Width()-1,rctDown.Height()-1,&dc,
	rctDown.left,rctDown.top,SRCCOPY);

	// Draw the border of pressed button
	rctDown.bottom-=1;

	hOldPen = (HPEN)SelectObject(dc.m_hDC,m_penShadow);
	dc.MoveTo(rctDown.left,rctDown.bottom-1);
	dc.LineTo(rctDown.left,rctDown.top);
	dc.LineTo(rctDown.right-1,rctDown.top);
	
	SelectObject(dc.m_hDC,m_penLight);
	dc.LineTo(rctDown.right-1,rctDown.bottom);
	dc.LineTo(rctDown.left-1,rctDown.bottom);


	SelectObject(dc.m_hDC,m_penDarkShadow);
	dc.MoveTo(rctDown.left+1,rctDown.bottom-2);
	dc.LineTo(rctDown.left+1,rctDown.top+1);
	dc.LineTo(rctDown.right-2,rctDown.top+1);

	SelectObject(dc.m_hDC,m_penLightShadow);
	dc.LineTo(rctDown.right-2,rctDown.bottom-1);
	dc.LineTo(rctDown.left,rctDown.bottom-1);

	SelectObject(dc.m_hDC,m_penButtonFace);
	dc.MoveTo(rctDown.left+2,rctDown.bottom-2);
	dc.LineTo(rctDown.left+2,rctDown.top+2);
	dc.LineTo(rctDown.right-2,rctDown.top+2);

	SelectObject(dc.m_hDC,hOldPen);
}

void CMySpinButtonCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_rcIsPressed.SetRectEmpty();
	CSpinButtonCtrl::OnLButtonDown(nFlags, point);
}

void CMySpinButtonCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate();	
	UpdateWindow();
	CSpinButtonCtrl::OnLButtonUp(nFlags, point);
}
