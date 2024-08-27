// TransparentImage.cpp : implementation file
//

#include "stdafx.h"
#include "DllTest.h"
#include "TransparentImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransparentImage

CTransparentImage::CTransparentImage()
{
}

CTransparentImage::~CTransparentImage()
{
}


BEGIN_MESSAGE_MAP(CTransparentImage, CStatic)
	//{{AFX_MSG_MAP(CTransparentImage)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransparentImage message handlers

void CTransparentImage::OnPaint() 
{
	// TODO: Add your message handler code here
	HBITMAP l_hbmpBitmap = GetBitmap() ;

	if( l_hbmpBitmap == NULL ) 
	{ 
		Default() ;
		return ;
	} 
	CPaintDC dc(this); // device context for painting
	CRect l_rcClient;
	GetClientRect( &l_rcClient );
	CDC l_MaskDC;
	l_MaskDC.CreateCompatibleDC( &dc );
	CBitmap l_MaskBitmap;
	l_MaskBitmap.CreateBitmap( l_rcClient.Width(), l_rcClient.Height(), 1, 1, NULL );
	CBitmap* l_pOldMaskBitmap = l_MaskDC.SelectObject( &l_MaskBitmap );
	CDC l_MemoryDC;
	l_MemoryDC.CreateCompatibleDC( &dc );
	CBitmap* l_pOldMemoryBitmap = l_MemoryDC.SelectObject( CBitmap::FromHandle( l_hbmpBitmap ) );
	COLORREF l_crOldBack =l_MemoryDC.SetBkColor( RGB(255, 0, 255) );
	l_MaskDC.BitBlt( 0, 0, l_rcClient.Width(), l_rcClient.Height(), &l_MemoryDC, 0, 0, SRCCOPY );	// src: 直接将源位图拷贝到目的设备上 
	dc.BitBlt( 0, 0, l_rcClient.Width(), l_rcClient.Height(), &l_MemoryDC, 0, 0, SRCINVERT );		// src XOR dest: 将源插入到目标。二次使用时，将目标恢复到它原来的状态。在某种条件下可以代替SRCPAINT 操作
	dc.BitBlt( 0, 0, l_rcClient.Width(), l_rcClient.Height(), &l_MaskDC, 0, 0, SRCAND );			// src AND dest: 将目标文件中对应于源文件黑色区域的部分变黑，将对应于白色区域的部分留着不动
	dc.BitBlt( 0, 0, l_rcClient.Width(), l_rcClient.Height(), &l_MemoryDC, 0, 0, SRCINVERT );
	l_MemoryDC.SelectObject( l_pOldMemoryBitmap );
	l_MaskDC.SelectObject( l_pOldMaskBitmap );
	// Do not call CStatic::OnPaint() for painting messages
}
