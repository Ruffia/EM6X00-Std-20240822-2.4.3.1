// TransparentImage.cpp : implementation file
//

#include "stdafx.h"
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
	HBITMAP hBitmap = GetBitmap();

	BITMAP bm;
	GetObject(hBitmap, sizeof(BITMAP), &bm);

	CBitmap maskBmp;
	maskBmp.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1,NULL); 

	CDC maskDC;                                               //定义掩码DC
	maskDC.CreateCompatibleDC(NULL);                          //初始化设备DC
	CBitmap *oldMaskBmp = maskDC.SelectObject(&maskBmp);

	CBitmap* bmpSquare;                                        //定义一个CBitmap对象存储将要绘制的图(ID为IDC_SQUARE)
	bmpSquare = CBitmap::FromHandle(hBitmap);                            //加载位图

	CDC memDC;                                                //创建内存DC
	memDC.CreateCompatibleDC(NULL);                           //初始化DC
	CBitmap *oldMemBmp = memDC.SelectObject(bmpSquare);      //将位图存入内存DC中,并返回旧的位图
	memDC.SetBkColor(RGB(255,0,255));        

	maskDC.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0,0, SRCCOPY);

	CPaintDC dc(this);            //窗口DC
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0,SRCINVERT);

	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &maskDC, 0, 0,SRCAND);

	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0,SRCINVERT);

	maskDC.SelectObject(oldMaskBmp);
	memDC.SelectObject(oldMemBmp);
	memDC.DeleteDC();
}
