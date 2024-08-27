// MyGroupBox.cpp : implementation file
//

#include "stdafx.h"
#include "MyGroupBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyGroupBox

IMPLEMENT_DYNAMIC( CMyGroupBox, CButton );

CMyGroupBox::CMyGroupBox()
	: m_eStyle( STYLE_SYSTEM )
	, m_eTextAlign( ALIGN_DEFAULT )
	, m_nCaptionHeaderHeight( 18 )
	, m_clrTextNormal( COLORREF(-1L) )
	, m_clrTextDisabled( COLORREF(-1L) )
	, m_clrBackground( COLORREF(-1L) )
{
	 m_clrCaptionBackground = ::GetSysColor(COLOR_3DSHADOW);
}

CMyGroupBox::~CMyGroupBox()
{
}


BEGIN_MESSAGE_MAP(CMyGroupBox, CButton)
	//{{AFX_MSG_MAP(CMyGroupBox)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyGroupBox message handlers

void CMyGroupBox::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle( 0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS );
	CButton::PreSubclassWindow();
}

BOOL CMyGroupBox::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	pDC;
	return FALSE;
}

void CMyGroupBox::OnPaint() 
{
	// TODO: Add your message handler code here
	CPaintDC dcPaint( this );
	CRect rcClient;
	GetClientRect( &rcClient );
	if( rcClient.IsRectEmpty() )
		return;
	DoPaint( &dcPaint, rcClient );
	// Do not call CButton::OnPaint() for painting messages
}

void CMyGroupBox::DoPaint( 
	CDC * pDC,
	CRect & rcClient
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pDC );
	
	// exclude siblings items area
	CWnd * pParent = GetParent();
	for(	CWnd * pWnd = pParent->GetWindow(GW_CHILD);
			pWnd != NULL && pWnd != this;
			pWnd = pWnd->GetWindow(GW_HWNDNEXT)
		)
	{
		if( (pWnd->GetStyle() & WS_VISIBLE) == 0 )
			continue;
		
		CString sClassName;
		::GetClassName(
			pWnd->GetSafeHwnd(),
			sClassName.GetBuffer( MAX_PATH + 1 ),
			MAX_PATH
			);
		sClassName.ReleaseBuffer();
//		if( sClassName == LPCTSTR( __EXT_SIZE_GRIPPER_CLASS_NAME ) )
//			continue;

		CRect rcChildRect, rcIntersection;
		pWnd->GetWindowRect( &rcChildRect );
		ScreenToClient( &rcChildRect );
		rcIntersection.IntersectRect(
			&rcClient,
			&rcChildRect
			);
		
		if( !rcIntersection.IsRectEmpty() )
			pDC->ExcludeClipRect( &rcIntersection );
	}

	//创建与屏幕窗口兼容的内存设备环境
//	CDC dc;
//	dc.CreateCompatibleDC( pDC );
	CExtMemoryDC dc(
		pDC,
		&rcClient
		);


	bool bEnabled = IsWindowEnabled() ? true : false;

	// paint background
	COLORREF clrBackground = GetBkColor();

	dc.FillSolidRect(
		&rcClient,
		(clrBackground != COLORREF(-1L)) 
			? clrBackground 
			: ::GetSysColor(COLOR_3DFACE)
		);

	int nOldBkMode = dc.SetBkMode(TRANSPARENT);

	// caption 
	CString strText;
	int nTextLen = GetWindowTextLength();
	if( nTextLen > 0 )
	{
		GetWindowText( strText.GetBuffer( nTextLen + 2 ), nTextLen + 1 );
		strText.ReleaseBuffer();
	} // if( nTextLen > 0 )

CRect rcBorder( rcClient );
CRect rcText( 0, 0, 0, 0 );

DWORD dwDrawTextFlags = DT_LEFT;
	if( strText.Find( _T('\t') ) != -1 ) // do tabs expanding
		dwDrawTextFlags |= DT_EXPANDTABS;
	dwDrawTextFlags |= (DT_VCENTER|DT_END_ELLIPSIS); 

bool bDrawText = 
		( strText.GetLength() > 0 && m_eStyle != STYLE_CAPTION ) 
			? true 
			: false;
	if( bDrawText )
	{
		rcText.SetRect( 9, 0, rcClient.right - 9, rcClient.bottom );

		HFONT hFont = OnQueryFont();
		CRect rcTextDimension =
			stat_CalcTextDimension(
				dc,
				*(CFont::FromHandle(hFont)),
				strText
				);

		e_text_align_t eTextAlign = m_eTextAlign;
		if( eTextAlign == ALIGN_DEFAULT )
		{
			DWORD dwStyle = CWnd::GetStyle();
			const UINT nMask = (BS_LEFT|BS_CENTER|BS_RIGHT);
			if( (dwStyle & nMask) == BS_RIGHT )
				eTextAlign = ALIGN_RIGHT;
			else if( (dwStyle & nMask) == BS_CENTER )
				eTextAlign = ALIGN_CENTER;
			else
				eTextAlign = ALIGN_LEFT;
		}

		if( eTextAlign == ALIGN_LEFT )
			rcText.right = rcText.left + rcTextDimension.Width();
		else if( eTextAlign == ALIGN_RIGHT )
			rcText.left = rcText.right - rcTextDimension.Width();
		else if( eTextAlign == ALIGN_CENTER )
		{
			rcText.left = (rcText.Width() - rcTextDimension.Width())/2;
			rcText.right = rcText.left + rcTextDimension.Width();
		}
		rcText.bottom = rcText.top + rcTextDimension.Height();

		rcBorder.top += rcText.Height()/2;
	}
	else
	{
		rcBorder.top += 6;
	}

	if( bDrawText )
	{
		CRect rcTextClip( rcText );
		rcTextClip.InflateRect(2,0,2,0);
		dc.ExcludeClipRect( &rcTextClip );
	}

	// frame rect
	OnDrawFrameRect(
		dc,
		rcBorder,
		bEnabled
		);

	if( bDrawText )
		dc.SelectClipRgn( NULL );

	// caption text
	if( bDrawText )
		OnDrawLabelText(
			dc,
			rcText,
			strText,
			dwDrawTextFlags,
			bEnabled
			);

	dc.SetBkMode(nOldBkMode);

	//DefWindowProc( WM_PAINT, WPARAM(dc.GetSafeHdc()), 0L );
}

CRect CMyGroupBox::stat_CalcTextDimension(
	CDC & dc,
	CFont & font,
	const CString & strText,
	UINT uFlags // = DT_SINGLELINE|DT_LEFT|DT_CALCRECT
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( font.GetSafeHandle() != NULL );
CRect rcText(0,0,0,0);
CFont * pOldFont = dc.SelectObject(
		&font
		);
	ASSERT( pOldFont != NULL );

	if( (uFlags&DT_CALCRECT) == 0 )
		uFlags |= DT_CALCRECT;

	if( strText.Find( _T('\t') ) != -1 ) // do tabs expanding
		uFlags |= DT_EXPANDTABS;
 
	dc.DrawText(
		strText,
		strText.GetLength(),
		&rcText,
		uFlags
		);
	dc.SelectObject(pOldFont);
	rcText.OffsetRect( -rcText.TopLeft() );
	return rcText;
}

HFONT CMyGroupBox::OnQueryFont() const
{
	ASSERT_VALID( this );
HFONT hFont = (HFONT)
		::SendMessage( m_hWnd, WM_GETFONT, 0L, 0L );
	if( hFont == NULL )
	{
		HWND hWndParent = ::GetParent( m_hWnd );
		if( hWndParent != NULL )
			hFont = (HFONT)
				::SendMessage( hWndParent, WM_GETFONT, 0L, 0L );
	} // if( hFont == NULL )
	if( hFont == NULL )
	{
		hFont = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
		if( hFont == NULL )
			hFont = (HFONT)::GetStockObject( SYSTEM_FONT );
	} // if( hFont == NULL )
	return hFont;
}

void CMyGroupBox::OnDrawFrameRect(
	CDC & dc,
	const CRect & rcClient,
	bool bEnabled
	)
{
CRect rcBorder( rcClient );
	switch( m_eStyle )
	{
	case STYLE_SYSTEM:
		{
			PaintGroupBoxFrame(
				dc,
				rcBorder,
				this
				);
		}
		break;
	case STYLE_ETCHED:
		{
			dc.Draw3dRect(
				&rcBorder,
				::GetSysColor(COLOR_3DSHADOW),//PmBridge_GetPM()->GetColor( COLOR_3DSHADOW, this ),
				::GetSysColor(COLOR_WINDOW)//PmBridge_GetPM()->GetColor( COLOR_WINDOW, this )
				);
			rcBorder.DeflateRect(1,1);
			dc.Draw3dRect(
				&rcBorder,
				::GetSysColor(COLOR_WINDOW),//PmBridge_GetPM()->GetColor( COLOR_WINDOW, this )
				::GetSysColor(COLOR_3DSHADOW)//PmBridge_GetPM()->GetColor( COLOR_3DSHADOW, this )
				);
		}
		break;
	case STYLE_ROUNDED:
		{
			CBrush * pBrush =
				CBrush::FromHandle( (HBRUSH)::GetStockObject(NULL_BRUSH) );
			CBrush * pOldBrush = dc.SelectObject( pBrush );
			CPen pen;
			pen.CreatePen(
				PS_SOLID,
				1,
				::GetSysColor(COLOR_3DSHADOW)//PmBridge_GetPM()->GetColor( COLOR_3DSHADOW, this )
				);
			CPen * pOldPen = dc.SelectObject( &pen );
			dc.RoundRect(
				&rcClient,
				CPoint( 10, 10 )
				);
			dc.SelectObject( pOldPen );
			dc.SelectObject( pOldBrush );
		}
		break;
	case STYLE_FLAT:
		{
			dc.Draw3dRect(
				&rcBorder,
				::GetSysColor(COLOR_3DSHADOW),//PmBridge_GetPM()->GetColor( COLOR_3DSHADOW, this ),
				::GetSysColor(COLOR_3DSHADOW)//PmBridge_GetPM()->GetColor( COLOR_3DSHADOW, this )
				);
		}
		break;
	case STYLE_CAPTION:
		{
			GetClientRect( &rcBorder );
			dc.Draw3dRect(
				&rcBorder,
				::GetSysColor(COLOR_3DSHADOW),//PmBridge_GetPM()->GetColor( COLOR_3DSHADOW, this ),
				::GetSysColor(COLOR_3DSHADOW)//PmBridge_GetPM()->GetColor( COLOR_3DSHADOW, this )
				);
			CRect rcHeader( rcBorder);
			rcHeader.bottom = rcHeader.top + m_nCaptionHeaderHeight;
			dc.FillSolidRect( 
				&rcHeader, 
				m_clrCaptionBackground//PmBridge_GetPM()->GetColor( COLOR_3DSHADOW, this )
				);
			
			// caption 
			CString strText;
			int nTextLen = GetWindowTextLength();
			if( nTextLen > 0 )
			{
				GetWindowText( strText.GetBuffer( nTextLen + 2 ), nTextLen + 1 );
				strText.ReleaseBuffer();
			} // if( nTextLen > 0 )
			if( strText.GetLength() > 0 )
			{
				e_text_align_t eTextAlign = m_eTextAlign;
				if( eTextAlign == ALIGN_DEFAULT )
				{
					DWORD dwStyle = CWnd::GetStyle();
					const UINT nMask = (BS_LEFT|BS_CENTER|BS_RIGHT);
					if( (dwStyle & nMask) == BS_RIGHT )
						eTextAlign = ALIGN_RIGHT;
					else if( (dwStyle & nMask) == BS_CENTER )
						eTextAlign = ALIGN_CENTER;
					else
						eTextAlign = ALIGN_LEFT;
				}

				DWORD dwDrawTextFlags = DT_LEFT;
				if( eTextAlign == ALIGN_RIGHT )
					dwDrawTextFlags = DT_RIGHT;
				else if( eTextAlign == ALIGN_CENTER )
					dwDrawTextFlags = DT_CENTER;

				if( strText.Find( _T('\t') ) != -1 ) // do tabs expanding
					dwDrawTextFlags |= DT_EXPANDTABS;
				dwDrawTextFlags |= (DT_VCENTER|DT_END_ELLIPSIS); 
				
				HFONT hFont = OnQueryFont();
				LOGFONT lf;
				::GetObject( 
					(HGDIOBJ)hFont, 
					sizeof(lf), 
					(LPVOID) &lf 
					);
				hFont = NULL;
				
				lf.lfWeight = FW_BOLD;
				
				hFont = ::CreateFontIndirect( &lf );
				ASSERT( hFont != NULL );
				CFont _fontDestructor;
				_fontDestructor.Attach( hFont );
				
				HGDIOBJ hOldFont = NULL;
				if( hFont != NULL )
					hOldFont = ::SelectObject( dc, (HGDIOBJ)hFont );
				INT nOldBkMode = dc.SetBkMode( TRANSPARENT );

				COLORREF clrText = GetTextColor( bEnabled );
				COLORREF clrOldText =
					dc.SetTextColor(
						clrText != COLORREF(-1L) 
							? clrText 
							: ::GetSysColor(COLOR_WINDOW)//PmBridge_GetPM()->GetColor( COLOR_WINDOW, this ),
						);
				
				CRect rcTextDimension =
					stat_CalcTextDimension(
					dc,
					*(CFont::FromHandle(hFont)),
					strText
					);
				
				CRect rcText(
					5,
					(m_nCaptionHeaderHeight - rcTextDimension.Height())/2,
					rcClient.right - 4,
					rcClient.bottom
					);
				dc.DrawText(
					LPCTSTR(strText),
					int(_tcslen(strText)),
					rcText,
					dwDrawTextFlags
					);
				
				dc.SetTextColor( clrOldText );
				dc.SetBkMode( nOldBkMode );
				if( hFont != NULL )
					::SelectObject( dc, hOldFont );
			} // if( strText.GetLength() > 0 )
		}
		break;
	} // switch( m_eStyle )
}

bool CMyGroupBox::PaintGroupBoxFrame(
	CDC & dc,
	const RECT & rcBorder,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );

CWnd * pWnd =
		DYNAMIC_DOWNCAST(
			CWnd,
			pHelperSrc
			);
	ASSERT( pWnd != NULL );
	ASSERT_VALID( pWnd );

//	if( ! g_PaintManager.m_UxTheme.IsControlsThemed() )
	{
		CRect rcClient( rcBorder );
		dc.Draw3dRect(
			&rcClient,
			::GetSysColor(COLOR_3DSHADOW),//GetColor( COLOR_3DSHADOW, pHelperSrc, lParam ),
			::GetSysColor(COLOR_WINDOW)//GetColor( COLOR_WINDOW, pHelperSrc, lParam )
			);
		rcClient.DeflateRect(1,1);
		dc.Draw3dRect(
			&rcClient,
			::GetSysColor(COLOR_WINDOW),//GetColor( COLOR_WINDOW, pHelperSrc, lParam ),
			::GetSysColor(COLOR_3DSHADOW)//GetColor( COLOR_3DSHADOW, pHelperSrc, lParam )
			);
	} // if( ! g_PaintManager.m_UxTheme.IsControlsThemed() )
/*	else
	{
		if( g_PaintManager.m_UxTheme.OpenThemeData( 
				pWnd->GetSafeHwnd(), 
				L"BUTTON" 
				) != NULL
			)
		{
			VERIFY( 
				g_PaintManager.m_UxTheme.DrawThemeBackground(
					dc.GetSafeHdc(), 
					BP_GROUPBOX, 
					pWnd->IsWindowEnabled() 
						? GBS_NORMAL 
						: GBS_DISABLED, 
					&rcBorder, 
					&rcBorder
					) == S_OK
				);
			g_PaintManager.m_UxTheme.CloseThemeData();
		}
	} // else from if( ! g_PaintManager.m_UxTheme.IsControlsThemed() )
*/
	return true;
}
	
void CMyGroupBox::OnDrawLabelText(
	CDC & dc,
	CRect & rcText,
	LPCTSTR strText,
	DWORD dwDrawTextFlags,
	bool bEnabled
	)
{
HFONT hFont = OnQueryFont();
COLORREF clrText = OnQueryTextColor( bEnabled );

	PaintGroupBoxLabel(
		dc,
		strText,
		clrText,
		hFont,
		dwDrawTextFlags,
		bEnabled,
		rcText,
		this
		);
}

bool CMyGroupBox::PaintGroupBoxLabel(
	CDC & dc,
	LPCTSTR strText,
	COLORREF clrText,
	HFONT hFont,
	DWORD dwDrawTextFlags,
	bool bEnabled,
	const RECT & rcText,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	lParam;

CWnd * pWnd =
		DYNAMIC_DOWNCAST(
			CWnd,
			pHelperSrc
			);
	ASSERT( pWnd != NULL );
	ASSERT_VALID( pWnd );

HGDIOBJ hOldFont = NULL;
	if( hFont != NULL )
		hOldFont = ::SelectObject( dc, (HGDIOBJ)hFont );
INT nOldBkMode = dc.SetBkMode( TRANSPARENT );

/*	if(		(! g_PaintManager.m_UxTheme.IsControlsThemed()) 
		||	clrText != COLORREF(-1L)
		)
*/	{
		if( clrText == COLORREF(-1L) )
		{
/*			clrText =
				QueryObjectTextColor(
					dc,
					bEnabled,
					false,
					false,
					false,
					pHelperSrc
					);
			if( clrText == COLORREF(-1L) )
*/				clrText =
					bEnabled
						? ::GetSysColor(COLOR_BTNTEXT)
						: ::GetSysColor(COLOR_3DSHADOW);
/*					GetColor(
						bEnabled
							? COLOR_BTNTEXT
							: COLOR_3DSHADOW,
						pHelperSrc
						);
*/		}		
		COLORREF clrOldText = dc.SetTextColor( clrText );
		CRect rc( rcText );
		dc.DrawText(
			LPCTSTR(strText),
			int(_tcslen(strText)),
			rc,
			dwDrawTextFlags
			);
		dc.SetTextColor( clrOldText );
	} // if( ! g_PaintManager.m_UxTheme.IsControlsThemed() )
/*	else
	{
		if( g_PaintManager.m_UxTheme.OpenThemeData( 
				pWnd->GetSafeHwnd(), 
				L"BUTTON" 
				) != NULL
			)
		{
			VERIFY( 
				g_PaintManager.m_UxTheme.DrawThemeText(
					dc.GetSafeHdc(), 
					BP_GROUPBOX, 
					bEnabled 
						? GBS_NORMAL 
						: GBS_DISABLED, 
					LPCTSTR(strText),
					-1,
					dwDrawTextFlags,
					NULL,
					&rcText
					) == S_OK
				);
			g_PaintManager.m_UxTheme.CloseThemeData();
		}
	} // else from if( ! g_PaintManager.m_UxTheme.IsControlsThemed() )
*/
	dc.SetBkMode( nOldBkMode );
	if( hOldFont != NULL )
		::SelectObject( dc, hOldFont );

	return false;
}

LRESULT CMyGroupBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
#if (defined WM_UPDATEUISTATE)	
	ASSERT( WM_UPDATEUISTATE == 0x0128 );
#endif
	// WM_UPDATEUISTATE causes repaint without WM_PAINT, so we eat it
	if( message == 0x0128 )
		return 0;
	if(		message == WM_SETTEXT 
		||	message == WM_ENABLE
		
		)
	{
		LRESULT lResult = CButton::WindowProc(message, wParam, lParam);
		Invalidate();
		UpdateWindow();
		return lResult;
	}
	if( message == WM_PRINTCLIENT )
	{
		CDC * pDC = CDC::FromHandle( (HDC)wParam );
		CRect rcClient;
		GetClientRect( &rcClient );
//		DoPaint( pDC, rcClient );
		return (!0);
	}	
	return CButton::WindowProc(message, wParam, lParam);
}

void CMyGroupBox::SetStyle( e_style_t eStyle )
{
	ASSERT_VALID( this );
	m_eStyle = eStyle;
	if( GetSafeHwnd() != NULL )
		Invalidate();
}

CMyGroupBox::e_style_t CMyGroupBox::GetStyle() const
{
	ASSERT_VALID( this );
	return m_eStyle;
}

void CMyGroupBox::SetTextAlign( e_text_align_t eTextAlign )
{
	ASSERT_VALID( this );
	m_eTextAlign = eTextAlign;
	if( GetSafeHwnd() != NULL )
		Invalidate();
}

CMyGroupBox::e_text_align_t CMyGroupBox::GetTextAlign() const
{
	ASSERT_VALID( this );
	return m_eTextAlign;
}

void CMyGroupBox::SetCaptionHeaderHeight( int nHeight )
{
	ASSERT_VALID( this );
	m_nCaptionHeaderHeight = nHeight;
	if( GetSafeHwnd() != NULL )
		Invalidate();
}

int CMyGroupBox::GetCaptionHeaderHeight() const
{
	ASSERT_VALID( this );
	return m_nCaptionHeaderHeight;
}

void CMyGroupBox::SetBkColor( COLORREF clrBk )
{ 
	ASSERT_VALID( this );
	m_clrBackground = clrBk; 
	if( GetSafeHwnd() != NULL )
		Invalidate();
}

COLORREF CMyGroupBox::GetBkColor() const
{ 
	ASSERT_VALID( this );
	return m_clrBackground; 
}

void CMyGroupBox::SetTextColor(
	COLORREF clrText, // = COLORREF(-1L)
	bool bEnabled // = true
	)
{
	ASSERT_VALID( this );
	if( bEnabled )
		m_clrTextNormal = clrText;
	else
		m_clrTextDisabled = clrText;
	if( GetSafeHwnd() != NULL )
		Invalidate();
}

COLORREF CMyGroupBox::GetTextColor(
	bool bEnabled // = true
	) const
{
	ASSERT_VALID( this );
COLORREF clrText =
		bEnabled
			? m_clrTextNormal
			: m_clrTextDisabled
			;
	return clrText;
}

COLORREF CMyGroupBox::OnQueryTextColor(
	bool bEnabled // = true
	) const
{
	ASSERT_VALID( this );
COLORREF clrText = GetTextColor( bEnabled );
	return clrText;
}

void CMyGroupBox::SetCaptionBkColor( COLORREF clrBk )
{ 
	ASSERT_VALID( this );
	m_clrCaptionBackground = clrBk; 
	if( GetSafeHwnd() != NULL )
		Invalidate();
}

COLORREF CMyGroupBox::GetCaptionBkColor() const
{ 
	ASSERT_VALID( this );
	return m_clrCaptionBackground; 
}
