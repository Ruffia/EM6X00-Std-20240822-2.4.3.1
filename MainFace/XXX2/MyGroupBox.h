#if !defined(AFX_MYGROUPBOX_H__920EAD96_DC3D_4FE6_A5B9_67B9482FCE93__INCLUDED_)
#define AFX_MYGROUPBOX_H__920EAD96_DC3D_4FE6_A5B9_67B9482FCE93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyGroupBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyGroupBox window

#if (!defined LAYOUT_RTL)
	#define LAYOUT_RTL								0x00000001
#endif // (!defined LAYOUT_RTL)

#if (!defined LAYOUT_LTR)
	#define LAYOUT_LTR								0x00000000
#endif // (!defined LAYOUT_LTR)

class CExtMemoryDC : public CDC 
{
protected:	
	CBitmap m_bmp;			// offscreen bitmap
	CBitmap * m_pOldBmp;	// bitmap originally found in CExtMemoryDC
	CDC * m_pDC;			// saves CDC passed in constructor
	CRect m_rect;			// rectangle of drawing area
	BOOL m_bMemDC;			// TRUE if CDC really is a memory DC
	COLORREF * m_pHelperDibSurface; // DIB surface if created as DIB

public:
	enum eOptFlags
	{
		MDCOPT_TO_MEMORY			= 0x001L,
		MDCOPT_FILL_SURFACE			= 0x002L,
		MDCOPT_FORCE_BLACK			= 0x004L,
		MDCOPT_FORCE_DIB			= 0x008L,
		MDCOPT_NO_COPY_OPT			= 0x010L,
		MDCOPT_FILL_BITS			= 0x020L,
		MDCOPT_RTL_COMPATIBILITY	= 0, // MDCOPT_NO_COPY_OPT
		MDCOPT_DEFAULT				= MDCOPT_TO_MEMORY|MDCOPT_FILL_SURFACE|MDCOPT_RTL_COMPATIBILITY
	}; // enum eOptFlags

	void __InitMemoryDC(
		CDC * pDC,
		const CRect * pRect = NULL,
		DWORD dwOptFlags = MDCOPT_DEFAULT
		)
	{
		ASSERT( m_pDC == NULL ); 
		ASSERT( pDC != NULL ); 
		ASSERT( pDC->GetSafeHdc() != NULL ); 

		m_pDC = pDC;
		m_pOldBmp = NULL;
		if( dwOptFlags & MDCOPT_TO_MEMORY )
			m_bMemDC = !pDC->IsPrinting();
		else
			m_bMemDC = FALSE;

		if( pRect == NULL ) 
			pDC->GetClipBox( &m_rect );
		else 
			m_rect = *pRect;
		if( m_bMemDC ) 
		{
			CreateCompatibleDC( pDC );
			DWORD dwLayout = pDC->GetLayout();
			if( dwLayout == GDI_ERROR )
				dwLayout = LAYOUT_LTR;
			if( (dwOptFlags & MDCOPT_NO_COPY_OPT) == 0 )
			{
				SetLayout( dwLayout );
				if( (dwLayout&LAYOUT_RTL) != 0 )
					m_rect.SwapLeftRight();
				pDC->LPtoDP( &m_rect );
			} // if( (dwOptFlags & MDCOPT_NO_COPY_OPT) == 0 )
			if( dwOptFlags & MDCOPT_FORCE_BLACK )
				m_bmp.CreateBitmap(
					m_rect.Width(),
					m_rect.Height(),
					1,
					1,
					NULL
					);
			else if( dwOptFlags & MDCOPT_FORCE_DIB )
			{
				BITMAPINFOHEADER bih;
				bih.biSize = sizeof(BITMAPINFOHEADER);
				bih.biWidth = m_rect.Width();
				bih.biHeight = m_rect.Height();
				bih.biPlanes = 1;
				bih.biBitCount = 32;
				bih.biCompression = BI_RGB;
				bih.biSizeImage = bih.biWidth*bih.biHeight;
				bih.biXPelsPerMeter = 0;
				bih.biYPelsPerMeter = 0;
				bih.biClrUsed = 0;
				bih.biClrImportant = 0;
				ASSERT( m_pHelperDibSurface == NULL );
				HBITMAP hDIB =
					::CreateDIBSection(
						pDC->GetSafeHdc(),
						(LPBITMAPINFO)&bih,
						DIB_RGB_COLORS,
						(void **)&m_pHelperDibSurface,
						NULL,
						NULL
						);
				if( hDIB == NULL || m_pHelperDibSurface == NULL )
				{
					ASSERT( FALSE );
					return;
				}
				m_bmp.Attach( hDIB );
			}
			else
				m_bmp.CreateCompatibleBitmap(
					pDC,
					m_rect.Width(),
					m_rect.Height()
					);
			m_pOldBmp = SelectObject( &m_bmp );
			if( (dwOptFlags & MDCOPT_NO_COPY_OPT) == 0 )
			{
				SetMapMode( pDC->GetMapMode() );
				if( (dwLayout&LAYOUT_RTL) != 0 )
					m_rect.SwapLeftRight();
				pDC->DPtoLP( &m_rect );
				SetWindowOrg( m_rect.left, m_rect.top );
			}
		} 
		else 
		{
			if( pDC->IsPrinting() )
				m_bPrinting = pDC->m_bPrinting;
			m_hDC       = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}

		if( dwOptFlags & MDCOPT_FILL_BITS )
			BitBlt(
				0, 0,
				m_rect.Width(), m_rect.Height(),
				pDC,
				m_rect.left, m_rect.top,
				SRCCOPY
				);
		else if( dwOptFlags & MDCOPT_FILL_SURFACE )
			FillSolidRect( m_rect, pDC->GetBkColor() );
	};

	void __Flush( BOOL bMoveImage = TRUE )
	{
		if( m_pDC == NULL )
			return;

		if( m_bMemDC ) 
		{
			if( bMoveImage )
				m_pDC->BitBlt(
					m_rect.left, m_rect.top,
					m_rect.Width(), m_rect.Height(),
					this,
					m_rect.left, m_rect.top,
					SRCCOPY
					);			
			SelectObject( m_pOldBmp );
		} 
		else 
		{
			m_hDC = m_hAttribDC = NULL;
		}	
		m_pDC = NULL;
		m_pOldBmp = NULL;
		m_rect.SetRectEmpty();
		m_bMemDC = FALSE;
		m_pHelperDibSurface = NULL;
	};

	CExtMemoryDC(
		CDC * pDC = NULL,
		const CRect * pRect = NULL,
		DWORD dwOptFlags = MDCOPT_DEFAULT
		) : CDC()
	{
		m_pOldBmp = NULL;
		m_pDC = NULL;
		m_rect.SetRectEmpty();
		m_pHelperDibSurface = NULL;
		if( pDC != NULL )
			__InitMemoryDC(
				pDC,
				pRect,
				dwOptFlags
				);
	};
	~CExtMemoryDC()	
	{
		__Flush();
	};
	
	CRect & __GetDrawRect()
	{
		return m_rect;
	};
	const CRect & __GetDrawRect() const
	{
		return m_rect;
	};

	BOOL __IsMemDC() const
	{
		return m_bMemDC;
	}

	CExtMemoryDC * operator -> () 
	{
		return this;
	};
	operator CExtMemoryDC * () 
	{
		return this;
	};

	COLORREF * GetDibSurface()
	{
		ASSERT( m_pHelperDibSurface != NULL );
		return m_pHelperDibSurface;
	};
	const COLORREF * GetDibSurface() const
	{
		ASSERT( m_pHelperDibSurface != NULL );
		return m_pHelperDibSurface;
	};

//#if (defined __EXT_PAINT_MANAGER_H)
//
//	void __TransparentBltFrom(
//		CDC & dcFrom,       // source DC
//		int nXOriginDest,   // x-coord of destination upper-left corner
//		int nYOriginDest,   // y-coord of destination upper-left corner
//		int nWidthDest,     // width of destination rectangle
//		int hHeightDest,    // height of destination rectangle
//		int nXOriginSrc,    // x-coord of source upper-left corner
//		int nYOriginSrc,    // y-coord of source upper-left corner
//		int nWidthSrc,      // width of source rectangle
//		int nHeightSrc,     // height of source rectangle
//		COLORREF crTransparent  // color to make transparent
//		)
//	{
//		CExtPaintManager::stat_TransparentBlt(
//			GetSafeHdc(),
//			nXOriginDest, nYOriginDest,
//			nWidthDest, hHeightDest,
//			dcFrom.GetSafeHdc(),
//			nXOriginSrc, nYOriginSrc,
//			nWidthSrc, nHeightSrc,
//			crTransparent
//			);
//	};
//
//	void __TransparentBltTo(
//		CDC & dcTo,         // destination DC
//		int nXOriginDest,   // x-coord of destination upper-left corner
//		int nYOriginDest,   // y-coord of destination upper-left corner
//		int nWidthDest,     // width of destination rectangle
//		int hHeightDest,    // height of destination rectangle
//		int nXOriginSrc,    // x-coord of source upper-left corner
//		int nYOriginSrc,    // y-coord of source upper-left corner
//		int nWidthSrc,      // width of source rectangle
//		int nHeightSrc,     // height of source rectangle
//		COLORREF crTransparent  // color to make transparent
//		)
//	{
//		CExtPaintManager::stat_TransparentBlt(
//			dcTo.GetSafeHdc(),
//			nXOriginDest, nYOriginDest,
//			nWidthDest, hHeightDest,
//			GetSafeHdc(),
//			nXOriginSrc, nYOriginSrc,
//			nWidthSrc, nHeightSrc,
//			crTransparent
//			);
//	};
//
//#endif // if with CExtPaintManager

}; // class __PROF_UIS_API CExtMemoryDC

class CMyGroupBox : public CButton
{
// Construction
public:
	DECLARE_DYNAMIC( CMyGroupBox );
	CMyGroupBox();

// Attributes
public:

// Operations
public:

	enum e_style_t
	{
		STYLE_SYSTEM	= 0,
		STYLE_ETCHED	= 1,
		STYLE_ROUNDED	= 2,
		STYLE_FLAT		= 3,
		STYLE_CAPTION	= 4,
	};
	enum e_text_align_t
	{
		ALIGN_DEFAULT	= 0,
		ALIGN_LEFT		= 1,
		ALIGN_RIGHT		= 2,
		ALIGN_CENTER	= 3,
	};

	void SetStyle( e_style_t eStyle );
	e_style_t GetStyle() const;

	void SetTextAlign( e_text_align_t eTextAlign );
	e_text_align_t GetTextAlign() const;

	void SetCaptionHeaderHeight( int nHeight );
	int GetCaptionHeaderHeight() const;
	
	void SetBkColor( 
		COLORREF clrBk 
		);
	COLORREF GetBkColor() const;
	
	void SetTextColor(
		COLORREF clrText = COLORREF(-1L),
		bool bEnabled = true
		);
	COLORREF GetTextColor(
		bool bEnabled = true
		) const;
	void SetCaptionBkColor(
		COLORREF clBk
		);
	COLORREF GetCaptionBkColor() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyGroupBox)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyGroupBox();

protected:
	e_text_align_t m_eTextAlign;
	e_style_t m_eStyle;
	int m_nCaptionHeaderHeight;
	COLORREF m_clrBackground;
	COLORREF m_clrTextNormal;
	COLORREF m_clrTextDisabled;
	COLORREF m_clrCaptionBackground;
	
	virtual void DoPaint( 
		CDC * pDC,
		CRect & rcClient
		);

	virtual HFONT OnQueryFont() const;

	virtual void OnDrawLabelText(
		CDC & dc,
		CRect& rcText,
		LPCTSTR strText,
		DWORD dwDrawTextFlags,
		bool bEnabled
		);
	virtual void OnDrawFrameRect(
		CDC & dc,
		const CRect & rcClient,
		bool bEnabled
		);
	virtual bool PaintGroupBoxFrame(
		CDC & dc,
		const RECT & rcBorder,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual bool PaintGroupBoxLabel(
		CDC & dc,
		LPCTSTR strText,
		COLORREF clrText,
		HFONT hFont,
		DWORD dwDrawTextFlags,
		bool bEnabled,
		const RECT & rcText,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual COLORREF OnQueryTextColor(
		bool bEnabled
		) const;

	static CRect stat_CalcTextDimension(
		CDC & dc,
		CFont & font,
		const CString & strText,
		UINT uFlags = DT_SINGLELINE|DT_LEFT|DT_CALCRECT
		);

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyGroupBox)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYGROUPBOX_H__920EAD96_DC3D_4FE6_A5B9_67B9482FCE93__INCLUDED_)
