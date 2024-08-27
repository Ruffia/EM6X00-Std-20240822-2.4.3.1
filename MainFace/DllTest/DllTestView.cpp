// DllTestView.cpp : implementation of the CDllTestView class
//

#include "stdafx.h"
#include "DllTest.h"

#include "DllTestView.h"

#include <math.h>
#include "LineTracker.h"
#include "DlgAngleOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDllTestView

IMPLEMENT_DYNCREATE(CDllTestView, CView)

BEGIN_MESSAGE_MAP(CDllTestView, CView)
	//{{AFX_MSG_MAP(CDllTestView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_Test_AngleMeasure, OnTestAngleMeasure)
	ON_UPDATE_COMMAND_UI(ID_Test_AngleMeasure, OnUpdateTestAngleMeasure)
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_Change_Angle_Options, OnChangeAngleOptions)
	ON_COMMAND(ID_Change_Angle_Delete, OnChangeAngleDelete)
	ON_COMMAND(ID_Test_Line, OnTestLine)
	ON_UPDATE_COMMAND_UI(ID_Test_Line, OnUpdateTestLine)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDllTestView construction/destruction

CDllTestView::CDllTestView()
{
	// TODO: add construction code here
	m_nScalePixels		= 256;
	m_csUScal			= "300 μm";
	m_nScalePixelsSrc	= 256;
	m_csUScalSrc		= "300 μm";


	m_nAngleCounts		= 0;
	m_bAddAngle			= FALSE;
	m_plistAngle		= new ListAngle;

	m_hCursors[0] = LoadCursor(NULL, IDC_ARROW);	// Normal
	m_hCursors[1] = LoadCursor(NULL, IDC_SIZEWE);	// E
	m_hCursors[2] = LoadCursor(NULL, IDC_SIZEWE);	// W
	m_hCursors[3] = LoadCursor(NULL, IDC_SIZEALL);
	m_hCursors[4] = LoadCursor(NULL, IDC_SIZEALL);
	m_hCursors[5] = LoadCursor(NULL, IDC_SIZEALL);
	m_hCursors[6] = LoadCursor(NULL, IDC_ARROW);

	m_bLine			= FALSE;
	m_nHitLine		= -1;
	m_Line.pt1		= CPoint( 100, 100 );
	m_Line.pt2		= CPoint( 300, 200 );
	m_Line.pt1bak	= m_Line.pt1;
	m_Line.pt2bak	= m_Line.pt2;
}

CDllTestView::~CDllTestView()
{
	m_Font.DeleteObject();

	POSITION pos;
	while( (pos = m_plistAngle->GetHeadPosition()) )
	{
		AngleMeasure* pItem = m_plistAngle->GetAt( pos );
		m_plistAngle->RemoveHead();
		delete pItem;
	}
	delete m_plistAngle;
}

BOOL CDllTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDllTestView drawing

void CDllTestView::OnDraw(CDC* pDC)
{
	CDllTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	CRect client, rc;
	GetClientRect( &client );
// 	GetDlgItem( IDC_STATIC_AT_Cali_Graph1 )->GetClientRect( (LPRECT)rc );
// 	client.right = rc.Width() +40;
// 	client.bottom += 40;
	//获取图像宽度和高度
	int nWidth = client.Width();
	int nHeight = client.Height();
	//创建与屏幕窗口兼容的内存设备环境
	CDC bmpDC;
	bmpDC.CreateCompatibleDC( pDC );
	//创建用于在内存设备环境中绘图的位图，并将其选进内存设备环境
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap( pDC, client.Width(), client.Height() );
	CBitmap* poldBmp = bmpDC.SelectObject( &bitmap );
	//将目的区域以白色填充
	bmpDC.FillRect( &CRect(0, 0, nWidth, nHeight), &CBrush(RGB(255,255,255)) );
	//设置设备环境窗口原始点
	CPoint point = bmpDC.SetWindowOrg( 0, 0 );
	//画图
	DrawUScale( &bmpDC );
	DrawAngle( &bmpDC );
	if( m_bLine )
		DrawLine( &bmpDC );
	//恢复设备环境窗口原有的原始点
	bmpDC.SetWindowOrg( point );
	//将位图拷贝回屏幕环境设备中
	pDC->BitBlt( GetScrollPos(SB_HORZ), GetScrollPos( SB_VERT ), client.Width(), client.Height(), &bmpDC, 0, 0, SRCCOPY );
	//选回老位图，并将所创位图删除
	bmpDC.SelectObject( poldBmp );
	bitmap.DeleteObject();
}

void CDllTestView::DrawUScale( CDC* pDC )
{
	CPen penLine, *poldpen;
	/////////////////////////////////////////////////////////////////
	// 移动标尺
	penLine.CreatePen( PS_SOLID, 3, RGB(255,0,0) );
	poldpen = pDC->SelectObject( &penLine );

	pDC->MoveTo( 10, 10 );
	pDC->LineTo( 10, 30 );
	pDC->MoveTo( 10, 20 );
	pDC->LineTo( 10 +m_nScalePixels, 20 );
	pDC->MoveTo( 10 +m_nScalePixels, 10 );
	pDC->LineTo( 10 +m_nScalePixels, 30 );

	pDC->MoveTo( 10, 110 );
	pDC->LineTo( 10, 130 );
	pDC->MoveTo( 10, 120 );
	pDC->LineTo( 10 +m_nScalePixelsSrc, 120 );
	pDC->MoveTo( 10 +m_nScalePixelsSrc, 110 );
	pDC->LineTo( 10 +m_nScalePixelsSrc, 130 );

	pDC->SelectObject( poldpen );
	penLine.DeleteObject();

	CSize sz = pDC->GetTextExtent(m_csUScal);
	pDC->TextOut( 10 +(m_nScalePixels -sz.cx)/2, 20 -sz.cy/2, m_csUScal );

	sz = pDC->GetTextExtent(m_csUScalSrc);
	pDC->TextOut( 10 +(m_nScalePixelsSrc -sz.cx)/2, 120 -sz.cy/2, m_csUScalSrc );
	/////////////////////////////////////////////////////////////////
}

void CDllTestView::DrawAngle( CDC* pDC )
{
	CPen penLine, penArc, penBK, *poldpen;
	CFont font, *poldfont;
	/////////////////////////////////////////////////////////////////
	// 角度测量
	COLORREF colorBK = RGB(255,255,254);
	penBK.CreatePen( PS_SOLID, 2, colorBK );
	POSITION pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		penLine.CreatePen( PS_SOLID, 2, pAngle->AO.clrLine );
		penArc.CreatePen( PS_SOLID, 2, pAngle->AO.clrArc );

		if( pAngle->nAngleStatus > 2 )
		{
			// 整体移位
			poldpen = pDC->SelectObject( &penBK );
			pDC->MoveTo( pAngle->APbak.ptS.x, pAngle->APbak.ptS.y );
			pDC->LineTo( pAngle->APbak.ptC );
			pDC->LineTo( pAngle->APbak.ptE );
			pDC->SelectObject( poldpen );

			poldpen = pDC->SelectObject( &penLine );
			pDC->MoveTo( pAngle->AP.ptS.x, pAngle->AP.ptS.y );
			pDC->LineTo( pAngle->AP.ptC );
			pDC->LineTo( pAngle->AP.ptE );
			pDC->SelectObject( poldpen );
		}
		else
		{
			// 两条射线生成中
			if( pAngle->bStart )
			{
				pDC->MoveTo( pAngle->AP.ptS );
				if( pAngle->AP.ptC.x > 0 && pAngle->AP.ptC.y > 0 )
				{
					// 仅有起始点，中心点未定
					if( pAngle->APbak.ptC.x > 0 && pAngle->APbak.ptC.y > 0 )
					{
						poldpen = pDC->SelectObject( &penBK );
						pDC->LineTo( pAngle->APbak.ptC );
						pDC->SelectObject( poldpen );
						pDC->MoveTo( pAngle->AP.ptS );
					}

					poldpen = pDC->SelectObject( &penLine );
					pDC->LineTo( pAngle->AP.ptC );
					pDC->SelectObject( poldpen );
				}
				if( pAngle->bCenter && pAngle->AP.ptE.x > 0 && pAngle->AP.ptE.y > 0 )
				{
					// 有了起始点、中心点，终点未定
					if( pAngle->APbak.ptE.x > 0 && pAngle->APbak.ptE.y > 0 )
					{
						poldpen = pDC->SelectObject( &penBK );
						pDC->MoveTo( pAngle->APbak.ptC );
						pDC->LineTo( pAngle->APbak.ptE );
						pDC->SelectObject( poldpen );
						pDC->MoveTo( pAngle->AP.ptC );
					}

					poldpen = pDC->SelectObject( &penLine );
					pDC->LineTo( pAngle->AP.ptE );
					pDC->SelectObject( poldpen );
				}
				if( pAngle->bEnd && pAngle->AP.ptC.x > 0 && pAngle->AP.ptC.y > 0 )
				{
					// 正在修改中心点（整体移位)
					if( pAngle->APbak.ptC.x > 0 && pAngle->APbak.ptC.y > 0 )
					{
						poldpen = pDC->SelectObject( &penBK );
						pDC->MoveTo( pAngle->APbak.ptE );
						pDC->LineTo( pAngle->APbak.ptC );
						pDC->SelectObject( poldpen );
						pDC->MoveTo( pAngle->AP.ptE );
					}

					poldpen = pDC->SelectObject( &penLine );
					pDC->LineTo( pAngle->AP.ptC );
					pDC->SelectObject( poldpen );
				}
			}
			else
			{
				if( pAngle->bCenter && pAngle->bEnd && pAngle->AP.ptC.x > 0 && pAngle->AP.ptC.y > 0 && pAngle->AP.ptE.x > 0 && pAngle->AP.ptE.y > 0 )
				{
					// 正在修改起始点
					if( pAngle->APbak.ptS.x > 0 && pAngle->APbak.ptS.y > 0 )
					{
						poldpen = pDC->SelectObject( &penBK );
						pDC->MoveTo( pAngle->AP.ptC );
						pDC->LineTo( pAngle->APbak.ptS );
						pDC->SelectObject( poldpen );
					}

					poldpen = pDC->SelectObject( &penLine );
					pDC->MoveTo( pAngle->AP.ptS );
					pDC->LineTo( pAngle->AP.ptC );
					pDC->LineTo( pAngle->AP.ptE );
					pDC->SelectObject( poldpen );
				}
			}
		}

		// 弧
		if( (pAngle->bStart && pAngle->bCenter && pAngle->AP.ptE.x > 0 && pAngle->AP.ptE.y > 0)
			|| (pAngle->bStart && pAngle->bEnd && pAngle->AP.ptC.x > 0 && pAngle->AP.ptC.y > 0)
			|| (pAngle->bCenter && pAngle->bEnd && pAngle->AP.ptS.x > 0 && pAngle->AP.ptS.y > 0) )
		{
			// Arc函数总是从起点开始，逆时针画到终点
			pDC->SetBkMode(TRANSPARENT);
			poldpen = pDC->SelectObject( &penBK );
			pDC->Arc( pAngle->APbak.rcBound, pAngle->APbak.ptArcEnd, pAngle->APbak.ptArcStart );
			pDC->FrameRect( pAngle->APbak.rcBound, &CBrush(colorBK) );
			pDC->SelectObject( poldpen );

			poldpen = pDC->SelectObject( &penArc );
			pDC->Arc( pAngle->AP.rcBound, pAngle->AP.ptArcEnd, pAngle->AP.ptArcStart );
			pDC->FrameRect( pAngle->AP.rcBound, &CBrush(RGB(0,255,0)) );
			pDC->SelectObject( poldpen );

			// 角度标注
			CString str;
			CSize sz;
			str.Format( "%.1f", pAngle->APbak.dAngle );
			sz = pDC->GetTextExtent(str);
			pDC->FillRect( CRect(pAngle->APbak.ptLabel.x -sz.cx/2, pAngle->APbak.ptLabel.y -sz.cy/2, pAngle->APbak.ptLabel.x +sz.cx/2, pAngle->APbak.ptLabel.y +sz.cy/2), &CBrush(colorBK) );

			poldpen = pDC->SelectObject( &penBK );
			pDC->SetTextColor( colorBK );
			str.Format( "%.1f", pAngle->APbak.dAngle );
			sz = pDC->GetTextExtent(str);
			pDC->TextOut( pAngle->APbak.ptLabel.x -sz.cx/2, pAngle->APbak.ptLabel.y -sz.cy/2, str );
			pDC->SelectObject( poldpen );

			font.CreateFont(pAngle->AO.nSize, 0, 0, 0, FW_NORMAL, 0, 0, 0,
				ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");
			poldfont = pDC->SelectObject( &font );
			poldpen = pDC->SelectObject( &penLine );
			pDC->SetTextColor( pAngle->AO.clrText );

			str.Format( "%.1f", pAngle->AP.dAngle );
			sz = pDC->GetTextExtent(str);
			pDC->TextOut( pAngle->AP.ptLabel.x -sz.cx/2, pAngle->AP.ptLabel.y -sz.cy/2, str );

			pDC->SelectObject( poldpen );
			pDC->SelectObject( poldfont );
			font.DeleteObject();
		}
		m_plistAngle->GetNext( pos );

		penLine.DeleteObject();
		penArc.DeleteObject();
	}
	/////////////////////////////////////////////////////////////////

	penBK.DeleteObject();
}

void CDllTestView::DrawLine( CDC* pDC )
{
	CPen pen, *poldpen;
	COLORREF color = RGB(0,0,255);
	COLORREF colorBK = RGB(255,255,254);
	pen.CreatePen( PS_SOLID, 2, color );
	poldpen = pDC->SelectObject( &pen );

	// 全抹掉
	CRect rc( m_Line.pt1bak, m_Line.pt2bak );
	rc.InflateRect( 10, 10, 10, 10 );
	pDC->FillRect( rc, &CBrush(colorBK) );

	CPoint pt, pt1, pt2, pt31, pt32, pt41, pt42, pt51, pt52, pt61, pt62;
	CString str = "len=1.256mm";
	CSize szTextOut = pDC->GetTextExtent( str );
	CSize szGraph = CSize( 0, 1000 );
	pt1 = m_Line.pt1;
	pt2 = m_Line.pt2;
	pt.x = pt1.x + ( pt2.x - pt1.x ) / 2;

	// 标注位置
	if( abs( pt1.x - pt2.x ) < 10 )
	{
		// 垂直
		pt.x = pt1.x + 4;
		if( pt.x + szTextOut.cx > szGraph.cx )
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
		if( pt.x + szTextOut.cx > szGraph.cx )
			pt.x = szGraph.cx - 2 - szTextOut.cx;
		if( pt.y + szTextOut.cy > szGraph.cy )
			pt.y = pt1.y - 2 - szTextOut.cy;
	}
	else if( (pt1.x < pt2.x && pt1.y > pt2.y)
		|| (pt1.x > pt2.x && pt1.y < pt2.y ) )
	{
		pt.y = pt1.y + ( pt2.y - pt1.y ) / 2;
		if( pt.x + szTextOut.cx > szGraph.cx )
		{
			pt.x = pt.x -szTextOut.cx;
			pt.y = pt.y-szTextOut.cy;
			if( pt.y < 0 )
				pt.y = 0;
		}
		if( pt.y + szTextOut.cy > szGraph.cy )
			pt.y = pt1.y - 2 - szTextOut.cy;
	}
	else
	{
		pt.y = pt1.y + ( pt2.y - pt1.y ) / 2 - szTextOut.cy;
		if( pt.x + szTextOut.cx > szGraph.cx )
		{
			pt.x = pt.x -szTextOut.cx;
			pt.y = pt.y +szTextOut.cy;
		}
		if( pt.y + szTextOut.cy > szGraph.cy )
			pt.y = szGraph.cy -2 -szTextOut.cy;
	}

	pt51 = pt52 = pt1;
	pt61 = pt62 = pt2;
	// 斜线的两端刻度位置
	CPoint ptA, ptA1, ptA2;
	double dK1, dK2, db, dB1, dB2, dA, dB, dC;
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
			pt31.x	= (int)(( -dB + sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			pt32.x	= (int)(( -dB - sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
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
			pt31.y	= (int)(dK2 *pt31.x +dB1 +0.5);
			pt32.y	= (int)(dK2 *pt32.x +dB1 +0.5);
			// 另一端
			dB	= 2*dK2*dB2 -2*dK2*pt2.y -2*pt2.x;
			dC	= pt2.x*pt2.x + (dB2 -pt2.y)*(dB2 -pt2.y) -100;
			pt41.x	= (int)(( -dB + sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			pt42.x	= (int)(( -dB - sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
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
			pt41.y	= (int)(dK2 *pt41.x +dB2 +0.5);
			pt42.y	= (int)(dK2 *pt42.x +dB2 +0.5);

			// 标尺两端显示箭头
			// 标尺所在直线与水平线之间的夹角
			double angle = atan( dK1 );
			if( angle > PI )
				angle -= 2*PI;
			if( angle < -PI )
				angle += 2*PI;
//			angle = angle *180.0 /PI;	// 角度变弧度
			double angleOffset = angle -PI/4;
			dK2 = tan( angleOffset );
			dB1 = pt1.y -dK2 *pt1.x;
			dA	= dK2 * dK2 +1;
			dB	= 2*dK2*dB1 -2*dK2*pt1.y -2*pt1.x;
			dC	= pt1.x*pt1.x + (dB1 -pt1.y)*(dB1 -pt1.y) -64;
			pt51.x	= (int)(( -dB + sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			pt52.x	= (int)(( -dB - sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			if( pt52.x > min(pt1.x, pt2.x) && pt52.x < max(pt1.x, pt2.x) )
				pt51.x = pt52.x;
			pt51.y	= (int)(dK2 *pt51.x +dB1 +0.5);
			// 2
			angleOffset = angle +PI/4;
			dK2 = tan( angleOffset );
			dB1	= pt1.y -dK2 *pt1.x;	// 标尺一端的一侧箭头所在直线方程的截距
			dA	= dK2 * dK2 +1;
			dB	= 2*dK2*dB1 -2*dK2*pt1.y -2*pt1.x;
			dC	= pt1.x*pt1.x + (dB1 -pt1.y)*(dB1 -pt1.y) -64;
			int nX1	= (int)(( -dB + sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			int nX2	= (int)(( -dB - sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			if( nX1 > min(pt1.x, pt2.x) && nX1 < max(pt1.x, pt2.x) )
				pt52.x = nX1;
			else
				pt52.x = nX2;
			pt52.y	= (int)(dK2 *pt52.x +dB1 +0.5);

/*			// 箭头所在直线与标尺所在直线之间的夹角固定，则有夹角公式： tan夹角 = | K1 -K2/(1+K1*K2)|
			// 据此，可以求出箭头所在直线方程的斜率
			// 箭头与直线的交点即为直线的两个端点，据此可求出箭头所在直线方程的截距
			// 箭头长度固定为8，可求出箭头上另一个端点的坐标
			dK2 = ( tan(45.0) - dK1 ) / (dK1*dK1 -1 -tan(45.0) *dK1);
			dB1	= pt1.y -dK2 *pt1.x;	// 标尺一端的一侧箭头所在直线方程的截距
			dA	= dK2 * dK2 +1;
			dB	= 2*dK2*dB1 -2*dK2*pt1.y -2*pt1.x;
			dC	= pt1.x*pt1.x + (dB1 -pt1.y)*(dB1 -pt1.y) -64;
			pt51.x	= (int)(( -dB + sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			pt52.x	= (int)(( -dB - sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			if( pt52.x > min(pt1.x, pt2.x) && pt52.x < max(pt1.x, pt2.x) )
				pt51.x = pt52.x;
			pt51.y	= (int)(dK2 *pt51.x +dB1 +0.5);
			// 2
			dK2 = ( -tan(45.0) - dK1 ) / (dK1*dK1 -1 +tan(45.0) *dK1);
			dB1	= pt1.y -dK2 *pt1.x;	// 标尺一端的一侧箭头所在直线方程的截距
			dA	= dK2 * dK2 +1;
			dB	= 2*dK2*dB1 -2*dK2*pt1.y -2*pt1.x;
			dC	= pt1.x*pt1.x + (dB1 -pt1.y)*(dB1 -pt1.y) -64;
			int nX1	= (int)(( -dB + sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			int nX2	= (int)(( -dB - sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			if( nX1 > min(pt1.x, pt2.x) && nX1 < max(pt1.x, pt2.x) )
				pt52.x = nX1;
			else
				pt52.x = nX2;
			pt52.y	= (int)(dK2 *pt52.x +dB1 +0.5);
*/
/*			// 标尺所在直线上距离端点为20的点
			db	= pt1.y -dK1 *pt1.x;	// 标尺所在直线方程的截距
			dA	= dK1 * dK1 +1;
			dB	= 2*dK1*db -2*dK1*pt1.y -2*pt1.x;
			dC	= pt1.x*pt1.x + (db -pt1.y)*(db -pt1.y) -400;
			ptA1.x	= (int)(( -dB + sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			ptA2.x	= (int)(( -dB - sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			if( ptA1.x > min(pt1.x, pt2.x) && ptA1.x < max(pt1.x, pt2.x) )
				ptA.x = ptA1.x;
			else
				ptA.x = ptA2.x;
			ptA.y	= (int)(dK1 *ptA.x +db +0.5);
			// 经过这个点且垂直于标尺所在直线方程，且距离为7的两个点，即为箭头的两个点
			dB1	= ptA.y -dK2 *ptA.x;	// 截距
			dA	= dK2 * dK2 +1;
			dB	= 2*dK2*dB1 -2*dK2*ptA.y -2*ptA.x;
			dC	= ptA.x*ptA.x + (dB1 -ptA.y)*(dB1 -ptA.y) -49;
			pt51.x	= (int)(( -dB + sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			pt52.x	= (int)(( -dB - sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			// 保证对称
// 			if( pt51.x == ptA.x )
// 				pt51.x = ptA.x *2 -pt52.x;
// 			else
			{
				if( abs(ptA.x -pt51.x) < abs(ptA.x -pt52.x) )
					nDiff = abs(ptA.x -pt51.x);
				else
					nDiff = abs(ptA.x -pt52.x);
				pt51.x	= ptA.x -nDiff;
				pt52.x	= ptA.x +nDiff;
			}
			pt51.y	= (int)(dK2 *pt51.x +dB1 +0.5);
			pt52.y	= (int)(dK2 *pt52.x +dB1 +0.5);
			// 另一端
			dA	= dK1 * dK1 +1;
			dB	= 2*dK1*db -2*dK1*pt2.y -2*pt2.x;
			dC	= pt2.x*pt2.x + (db -pt2.y)*(db -pt2.y) -400;
			ptA1.x	= (int)(( -dB + sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			ptA2.x	= (int)(( -dB - sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			if( ptA1.x > min(pt1.x, pt2.x) && ptA1.x < max(pt1.x, pt2.x) )
				ptA.x = ptA1.x;
			else
				ptA.x = ptA2.x;
			ptA.y	= (int)(dK1 *ptA.x +db +0.5);
			// 经过这个点且垂直于标尺所在直线方程，且距离为7的两个点，即为箭头的两个点
			dB1	= ptA.y -dK2 *ptA.x;	// 截距
			dA	= dK2 * dK2 +1;
			dB	= 2*dK2*dB1 -2*dK2*ptA.y -2*ptA.x;
			dC	= ptA.x*ptA.x + (dB1 -ptA.y)*(dB1 -ptA.y) -49;
			pt61.x	= (int)(( -dB + sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			pt62.x	= (int)(( -dB - sqrt(dB*dB -4*dA*dC) ) / (2*dA) +0.5);
			// 保证对称
// 			if( pt61.x == ptA.x )
// 				pt61.x = ptA.x *2 -pt62.x;
// 			else
			{
				if( abs(ptA.x -pt61.x) < abs(ptA.x -pt62.x) )
					nDiff = abs(ptA.x -pt61.x);
				else
					nDiff = abs(ptA.x -pt62.x);
				pt61.x	= ptA.x -nDiff;
				pt62.x	= ptA.x +nDiff;
			}
			pt61.y	= (int)(dK2 *pt61.x +dB1 +0.5);
			pt62.y	= (int)(dK2 *pt62.x +dB1 +0.5);
*/		}
	}

	pDC->TextOut( pt.x, pt.y, str );
	pDC->MoveTo( pt1 );
	pDC->LineTo( pt2 );

	pDC->MoveTo( pt31 );
	pDC->LineTo( pt32 );
	pDC->MoveTo( pt41 );
	pDC->LineTo( pt42 );

	pDC->MoveTo( pt51 );
	pDC->LineTo( pt1 );
	pDC->LineTo( pt52 );
	pDC->MoveTo( pt61 );
	pDC->LineTo( pt2 );
	pDC->LineTo( pt62 );

	pDC->SelectObject( poldpen );
	pen.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// CDllTestView printing

BOOL CDllTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDllTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDllTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDllTestView diagnostics

#ifdef _DEBUG
void CDllTestView::AssertValid() const
{
	CView::AssertValid();
}

void CDllTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDllTestDoc* CDllTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDllTestDoc)));
	return (CDllTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDllTestView message handlers

int CDllTestView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	int nReady = theApp.m_SemCtrl.USB_Init(&(theApp.m_ImageCard));

	m_AngleOption.nSize		= 12;
	m_AngleOption.clrLine	= RGB( 0,128,192 );
	m_AngleOption.clrArc	= RGB( 255,128,0 );
	m_AngleOption.clrText	= RGB(0,0,0);
	m_Font.CreateFont(m_AngleOption.nSize, 0, 0, 0, FW_NORMAL, 0, 0, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");

	SetFont( &m_Font );
	return 0;
}

CString CDllTestView::CalculateUScale2( int nScreenWidth, double dfMag )
{
	CString strRet;
	int		u_scale, nScalePixels;
	CString g_csUScaleUnit;
	double	g_dfMobileUScaleParameter;
	
	////////////////////////////////////////////////////////////
	// Mag = 1, 
	// PhotoWidth = 120 mm		<==>	ScreenWidth = 1024 Pixel
	// u_scale = 10 mm			<==>	ScaleWidth	= ? Pixel
	////////////////////////////////////////////////////////////
	// ScaleWidth		= u_scale * ScreenWidth / PhotoWidth
	// SreenScaleParam	= ScreenWidth / PhotoWidth = 1024 / 120
	////////////////////////////////////////////////////////////
	const double SreenScaleParam = 1024.0 / 120.0 / (1024.0 /nScreenWidth);
//	const double SreenScaleParam = nScreenWidth / 120.0;	//1024.0 / 120.0;	// 11.05
	CString str;
	// 将固定标尺线长固定在图像分辨率的1/4以内，且在底标的靠右侧标注
	// 即标注尺寸将不一定是1、10、100这样的整数值
	double dScale = (nScreenWidth /4) / (dfMag *SreenScaleParam );
	if( dScale > 0.999 )
	{
		u_scale = (int)dScale;
		strRet.Format("%3d mm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam + 0.5);
		g_csUScaleUnit = _T("mm");
		g_dfMobileUScaleParameter = dfMag*SreenScaleParam;
	}
	else if( dScale > 0.0999 )
	{
		u_scale = (int)(dScale *1000) /100 *100;
		strRet.Format("%3d μm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);
		g_csUScaleUnit = _T("μm");
		g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;
	}
	else if( dScale > 0.00999 )
	{
		u_scale = (int)(dScale *1000 +0.05) /10 *10;
		strRet.Format("%3d μm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);
		g_csUScaleUnit = _T("μm");
		g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;
	}
	else if( dScale > 0.00099 )
	{
		u_scale = (int)(dScale *1000 +0.05);
		strRet.Format("%3d μm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);
		g_csUScaleUnit = _T("μm");
		g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;
	}
	else if( dScale > 0.000099 )
	{
		u_scale = (int)(dScale *1000000 +0.05) /100 *100;
		strRet.Format("%3d nm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000000 + 0.5);
		g_csUScaleUnit = _T("nm");
		g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000000;
	}
	else if( dScale > 0.0000099 )
	{
		u_scale = (int)(dScale *1000000 +0.05) /10 *10;
		strRet.Format("%3d nm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000000 + 0.5);
		g_csUScaleUnit = _T("nm");
		g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000000;
	}
	else if( dScale > 0.00000099 )
	{
		u_scale = (int)(dScale *1000000 +0.05);
		strRet.Format("%3d nm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000000 + 0.5);
		g_csUScaleUnit = _T("nm");
		g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000000;
	}

	m_nScalePixels	= nScalePixels;
	m_csUScal		= strRet;
	m_csUScaleUnit	= g_csUScaleUnit;
	m_dfMobileUScaleParameter	= g_dfMobileUScaleParameter;

	CalculateUScaleSrc( nScreenWidth, dfMag );
	Invalidate();
	return strRet;
}

CString CDllTestView::CalculateUScaleSrc( int nScreenWidth, double dfMag )
{
	CString strRet;
	int		u_scale, nScalePixels;
	CString g_csUScaleUnit;
	double	g_dfMobileUScaleParameter;
	const double SreenScaleParam = 1024.0 / 120.0 / (1024.0 /nScreenWidth);

	if((dfMag<10.)&&(dfMag>=1.0))
	{
		u_scale=10;
		strRet.Format("%3d mm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam + 0.5);

		g_csUScaleUnit = _T("mm");
		g_dfMobileUScaleParameter = dfMag*SreenScaleParam;
	}
	else if((dfMag<100.)&&(dfMag>=10.))
	{
		u_scale=1;
		strRet.Format("%3d mm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam + 0.5);

		g_csUScaleUnit = _T("mm");
		g_dfMobileUScaleParameter = dfMag*SreenScaleParam;
	}
	else if((dfMag<1000.)&&(dfMag>=100.))
	{
		u_scale=100;
		strRet.Format("%3d μm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);

		g_csUScaleUnit = _T("μm");
		g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;
	}
	else if((dfMag<10000.)&&(dfMag>=1000.))
	{
		u_scale=10;
		strRet.Format("%3d μm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);

		g_csUScaleUnit = _T("μm");
		g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;
	}
	else if((dfMag<100000.)&&(dfMag>=10000.))
	{
		u_scale=1;
		strRet.Format("%3d μm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);

		g_csUScaleUnit = _T("μm");
		g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;
	}
	else if(dfMag>=100000.)
	{
		u_scale=100;
		strRet.Format("%3d nm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000000 + 0.5);

		g_csUScaleUnit = _T("nm");
		g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000000;
	}
	m_nScalePixelsSrc	= nScalePixels;
	m_csUScalSrc		= strRet;
	m_csUScaleUnitSrc	= g_csUScaleUnit;
	m_dfMobileUScaleParameterSrc	= g_dfMobileUScaleParameter;
	return strRet;
}




/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

CPoint CDllTestView::ScreenToGraph( const CPoint point )
{
	CPoint ptPos = point;
	
	////////////////////////////////////////////
	// Get Logic Pos (Screen)
	////////////////////////////////////////////
	ptPos.x += GetScrollPos( SB_HORZ );
	ptPos.y += GetScrollPos( SB_VERT );
	////////////////////////////////////////////
	
	////////////////////////////////////////////
	// ZOOM to Graph (Graph)
	////////////////////////////////////////////
	double dScale = theApp.m_ImageCard.GetZoom();
	if ( dScale > 0 )
	{
		ptPos.x = (int)(ptPos.x / dScale + 0.5);
		ptPos.y = (int)(ptPos.y / dScale + 0.5);
	}
	else
	{
// 		ptPos.x = (int)(ptPos.x / g_dScaleScanTable[g_nScaleScanIndex] + 0.5);
// 		ptPos.y = (int)(ptPos.y / g_dScaleScanTable[g_nScaleScanIndex] + 0.5);
		ptPos.x = (int)(ptPos.x / 1.0 + 0.5);
		ptPos.y = (int)(ptPos.y / 1.0 + 0.5);
	}
	
	return ptPos;
}

void CDllTestView::OnTestAngleMeasure() 
{
	// TODO: Add your command handler code here
	m_bAddAngle	= !m_bAddAngle;
	if( m_bAddAngle )
		AngleAddNew( m_nAngleCounts );
	else
	{
		POSITION pos = m_plistAngle->GetTailPosition();
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		m_plistAngle->RemoveAt( pos );
		delete pAngle;
		m_nAngleCounts = m_plistAngle->GetCount();
	}
}

void CDllTestView::AngleAddNew( int nIndex )
{
	if( m_plistAngle->GetCount() > AngleMeasureCounts -1 )
	{
		m_bAddAngle = FALSE;
		return;
	}
	AngleMeasure* pAngle = new AngleMeasure();
	ZeroMemory( pAngle, sizeof(AngleMeasure) );
	pAngle->AO = m_AngleOption;
	pAngle->nAngleStatus	= 1;	// 生成
	m_plistAngle->AddTail( pAngle );
	m_nAngleCounts = m_plistAngle->GetCount();
}

void CDllTestView::OnUpdateTestAngleMeasure(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bAddAngle );
}

//#define PI 3.141592653589793

// 得到两射线之间的夹角
double CDllTestView::GetAngleOfTwoVector( CPoint ptS, CPoint ptE, CPoint ptC )
{
	double angle = atan2( ptS.x -ptC.x, ptS.y -ptC.y) - atan2( ptE.x -ptC.x, ptE.y -ptC.y );
	if( angle > PI )
		angle -= 2*PI;
	if( angle < -PI )
		angle += 2*PI;
	angle = angle *180.0 /PI;	// 角度变弧度
	return angle;
}

// 得到坐标
CPoint CDllTestView::GetCoordinates( CPoint ptCenter, double dAngle, int nR )
{
	CPoint pt;
	if( dAngle <= 0 )
		dAngle = -dAngle;
	else
	{
		double a = 180 -dAngle;
		dAngle = 180 +a;
	}
	pt = CPoint( ptCenter.x + 1.0*nR*cos(PI /180 *dAngle), ptCenter.y + 1.0*nR*sin(PI /180 *dAngle) );
	return pt;
}

// 得到测量信息
void CDllTestView::GetAngleMeasureInfo( AnglePosition& AP )
{
	AP.dAngle = GetAngleOfTwoVector( AP.ptS, AP.ptE, AP.ptC );
	if( AP.dAngle < 0 )
		AP.dAngle = 360 +AP.dAngle;
	// 弧所在圆的半径
	double dX1 = AP.ptS.x - AP.ptC.x;
	double dY1 = AP.ptS.y - AP.ptC.y;
	double dX2 = AP.ptE.x - AP.ptC.x;
	double dY2 = AP.ptE.y - AP.ptC.y;
	int nLen1 = (int)(sqrt( dX1*dX1 +dY1*dY1 ));
	int nLen2 = (int)(sqrt( dX2*dX2 +dY2*dY2 ));
	int nR = nLen1 > nLen2 ? nLen2/3 : nLen1 /3;	// 短的线段的1/3
	// 弧所在圆的外接矩形
	AP.rcBound.left		= AP.ptC.x -nR;
	AP.rcBound.top		= AP.ptC.y -nR;
	AP.rcBound.right	= AP.rcBound.left + nR*2;
	AP.rcBound.bottom	= AP.rcBound.top + nR*2;
	// 弧的起始角度
	CPoint ptEnd;	// 中心点的3点钟方向上的任意一点
	ptEnd.x = AP.ptC.x +100;
	ptEnd.y = AP.ptC.y;
	double dAngleStart = GetAngleOfTwoVector(AP.ptS, ptEnd, AP.ptC);
	// 两线夹角的一半
	double dAngleLabel = dAngleStart -AP.dAngle /2;
	// 弧的终止角度
	double dAngleEnd = GetAngleOfTwoVector(AP.ptE, ptEnd, AP.ptC);

	// 弧的起始和终止位置
	AP.ptArcStart = GetCoordinates( AP.ptC, dAngleStart, nR );
	AP.ptArcEnd = GetCoordinates( AP.ptC, dAngleEnd, nR );
	// 标注点位置
	AP.ptLabel = GetCoordinates( AP.ptC, dAngleLabel, nR );
}

void CDllTestView::GetAnglePoints( AngleMeasure* pAM, CPoint ptPos )
{
	if( !(pAM->bStart) )
	{
		pAM->AP.ptS	= ptPos;	// 第一个为起始点
		pAM->bStart	= TRUE;
	}
	else if( !(pAM->bCenter) )
	{
		pAM->AP.ptC	= ptPos;	// 第二个点为中心点
		pAM->bCenter= TRUE;
	}
	else if( !(pAM->bEnd) )
	{
		pAM->AP.ptE	= ptPos;	// 第三个为终点
		pAM->bEnd	= TRUE;
	}
	if( pAM->bStart && pAM->bCenter && pAM->bEnd )
	{
		pAM->nAngleStatus	= 0;		// 添加角度完成，单击点不再记录
		if( pAM->bAngleHitTest )
			pAM->bAngleHitTest = FALSE;
		// 3个点都有了，计算角度
		GetAngleMeasureInfo( pAM->AP );
		if( m_bAddAngle )
			AngleAddNew( m_nAngleCounts );
	}
	Invalidate(FALSE);
}

void CDllTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptPosSave = ScreenToGraph( point );
	if( m_bLine )
	{
		if( m_nHitLine != -1 )
			return;
	}

	int nIndex = 0;
	POSITION pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		if( pAngle->nAngleStatus == 1 )
		{
			// 有新生成的
			GetAnglePoints( pAngle, ptPosSave );
			break;
		}
		nIndex++;
		m_plistAngle->GetNext( pos );
	}
	if( nIndex != m_nAngleCounts )
		return;

	pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		if( pAngle->bAngleHitTest )
		{
			// 有被选中的已有角度
			int nGraphHitTest = GraphHitTest( ptPosSave );
			switch( nGraphHitTest )
			{
			case	GraphHitTest_W:
				pAngle->bStart = FALSE;
				break;
			case	GraphHitTest_E:
				pAngle->bEnd = FALSE;
				break;
			case	GraphHitTest_C:
				pAngle->bCenter = FALSE;
				break;
			case	GraphHitTest_L:
				pAngle->ptOriginal = point;
				break;
			case	GraphHitTest_In:
				pAngle->ptOriginal = point;
				break;
			}
			if( nGraphHitTest != GraphHitTest_NULL )
				pAngle->nAngleStatus = 2;		// 修改
			if( nGraphHitTest == GraphHitTest_L )
				pAngle->nAngleStatus = 4;		// 标注
			else if( nGraphHitTest == GraphHitTest_In )
				pAngle->nAngleStatus = 3;		// 整体移位
			break;
		}
		m_plistAngle->GetNext( pos );
	}

	CView::OnLButtonDown(nFlags, point);
}

void CDllTestView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptPosSave = ScreenToGraph( point );

	if( m_bLine )
	{
		if( m_nHitLine != -1 )
			return;
	}

	int nIndex = 0;
	POSITION pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		if( pAngle->nAngleStatus == 2 )
		{
			// 有要修改的
			GetAnglePoints( pAngle, ptPosSave );
			break;
		}
		nIndex++;
		m_plistAngle->GetNext( pos );
	}
	if( nIndex != m_nAngleCounts )
		return;

	pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		if( pAngle->nAngleStatus > 2 )
		{
			if( pAngle->nAngleStatus > 3 )
			{
				int nOffsetX	= point.x -pAngle->ptOriginal.x;
				int nOffsetY	= point.y -pAngle->ptOriginal.y;
				pAngle->AP.ptLabel.Offset(nOffsetX, nOffsetY);
			}
			pAngle->bAngleHitTest = FALSE;
			pAngle->nAngleStatus = 0;
			Invalidate(FALSE);
			break;
		}
		m_plistAngle->GetNext( pos );
	}

	CView::OnLButtonUp(nFlags, point);
}

void CDllTestView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// 删除选中的角度测量
	POSITION pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		if( pAngle->bAngleHitTest )
		{
			CMenu menuManual;
			// Create a new menu for the application window.
			VERIFY(menuManual.CreatePopupMenu());
			menuManual.AppendMenu(MF_STRING, ID_Change_Angle_Options, "属性");
			menuManual.AppendMenu(MF_STRING, ID_Change_Angle_Delete, "删除");

// 			menuManual.CheckMenuItem( ID_ADJUSTOR_MANUAL_ObjectiveLens +theApp.m_nAdjustorManualSort,
// 				MF_BYCOMMAND | MF_CHECKED );

			CPoint temppoint = point;
			ClientToScreen( &temppoint );
			menuManual.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
				temppoint.x, temppoint.y, this, NULL);

			// 有要删除的
// 			m_plistAngle->RemoveAt( pos );
// 			delete pAngle;
			break;
		}
		m_plistAngle->GetNext( pos );
	}
	m_nAngleCounts = m_plistAngle->GetCount();
	Invalidate(FALSE);
	CView::OnRButtonDown(nFlags, point);
}

void CDllTestView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_bLine )
	{
		if( m_nHitLine != -1 )
		{
			CPoint ptPosSave	= ScreenToGraph( point );
			switch( m_nHitLine )
			{
			case	0:
				m_Line.pt1bak	= m_Line.pt1;
				m_Line.pt1		= ptPosSave;
				Invalidate(FALSE);
				break;
			case	2:
				m_Line.pt2bak	= m_Line.pt2;
				m_Line.pt2		= ptPosSave;
				Invalidate(FALSE);
				break;
			case	8:
				break;
			}
			return;
		}
	}

	int nIndex = 0;
	POSITION pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		if( pAngle->nAngleStatus > 2 )
		{
			int nOffsetX	= point.x -pAngle->ptOriginal.x;
			int nOffsetY	= point.y -pAngle->ptOriginal.y;
			if( pAngle->nAngleStatus > 3 )
			{
				// 标注
				pAngle->APbak = pAngle->AP;
				pAngle->AP.ptLabel.Offset(nOffsetX, nOffsetY);
			}
			else
			{
				// 整体移位
				pAngle->APbak = pAngle->AP;
				pAngle->AP.ptS.Offset(nOffsetX, nOffsetY);
				pAngle->AP.ptC.Offset(nOffsetX, nOffsetY);
				pAngle->AP.ptE.Offset(nOffsetX, nOffsetY);
				GetAngleMeasureInfo( pAngle->AP );
			}
			pAngle->ptOriginal = point;
			Invalidate(FALSE);
			break;
		}
		nIndex++;
		m_plistAngle->GetNext( pos );
	}
	if( nIndex != m_nAngleCounts )
		return;

	pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		if( pAngle->bStart && pAngle->bCenter && pAngle->bEnd )
		{
			m_plistAngle->GetNext( pos );
			continue;
		}
		if( pAngle->bCenter && pAngle->bStart )
		{
			pAngle->APbak = pAngle->AP;
			CPoint ptPosSave	= ScreenToGraph( point );
			pAngle->AP.ptE		= ptPosSave;
		}
		else if( pAngle->bStart )
		{
			pAngle->APbak = pAngle->AP;
			CPoint ptPosSave	= ScreenToGraph( point );
			pAngle->AP.ptC		= ptPosSave;
		}
		else
		{
			pAngle->APbak = pAngle->AP;
			CPoint ptPosSave	= ScreenToGraph( point );
			pAngle->AP.ptS		= ptPosSave;
		}
		GetAngleMeasureInfo( pAngle->AP );
		break;
	}
	Invalidate(FALSE);
	CView::OnMouseMove(nFlags, point);
}

BOOL CDllTestView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	static HCURSOR hCursor_IBEAM = LoadCursor(NULL, IDC_IBEAM);

/*	if ( m_bCatch )
	{
		return TRUE;
	}
*/	
	if ( nHitTest != HTCLIENT )
		return CWnd::OnSetCursor(pWnd, nHitTest, message);

	POINT point;
	GetCursorPos( &point );
	ScreenToClient( &point );

	CPoint ptPos = ScreenToGraph( point );
	int nGraphHitTest = GraphHitTest( ptPos );

	if ( GetCursor() != m_hCursors[nGraphHitTest] )
		SetCursor( m_hCursors[nGraphHitTest] );
	
	return TRUE;
//	return CView::OnSetCursor(pWnd, nHitTest, message);
}

int CDllTestView::GraphHitTest( const CPoint ptGraphPos )
{
	// 测量线
	if( m_bLine )
	{
		CRect trackerRect( m_Line.pt1, m_Line.pt2 );
		CLineTracker tracker( &trackerRect, CRectTracker::resizeOutside | CRectTracker::hatchedBorder );
		m_nHitLine = tracker.HitTest( ptGraphPos );
		if( m_nHitLine == 0 )
			return GraphHitTest_W;
		else if( m_nHitLine == 2 )
			return GraphHitTest_E;
		else if( m_nHitLine == 8 )
			return GraphHitTest_In;
	}

	// 测量角度
	POSITION pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		pAngle->bAngleHitTest = FALSE;
		m_plistAngle->GetNext( pos );
	}

	pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		if( pAngle->bStart && pAngle->bCenter && pAngle->bEnd )
		{
			// 角的一条射线
			CRect trackerRect1( pAngle->AP.ptS, pAngle->AP.ptC );
			CLineTracker tracker1( &trackerRect1, CRectTracker::resizeOutside | CRectTracker::hatchedBorder );
			int nHitTest1 = tracker1.HitTest( ptGraphPos );
			if( nHitTest1 == 0 )
			{
				pAngle->bAngleHitTest = TRUE;
				return GraphHitTest_W;
			}

			// 角的另一条射线
			CRect trackerRect2( pAngle->AP.ptE, pAngle->AP.ptC );
			CLineTracker tracker2( &trackerRect2, CRectTracker::resizeOutside | CRectTracker::hatchedBorder );
			int nHitTest2 = tracker2.HitTest( ptGraphPos );
			if( nHitTest2 == 0 )
			{
				pAngle->bAngleHitTest = TRUE;
				return GraphHitTest_E;
			}
			if( nHitTest1 == 2 || nHitTest2 == 2 )
			{
				pAngle->bAngleHitTest = TRUE;
				return GraphHitTest_C;
			}

			// 标注所在大致区域
			CString str;
			str.Format( "%.1f", pAngle->AP.dAngle );
			CSize sz;
			CClientDC dc( this );
			sz = dc.GetTextExtent(str);
			CPoint ptPos1 = ScreenToGraph( pAngle->AP.ptLabel );
			ptPos1.x -= sz.cx/2;
			ptPos1.y -= sz.cy/2;
			CPoint ptPos2 = CPoint( ptPos1.x +sz.cx, ptPos1.y +sz.cy );
			CRect trackerRect3( ptPos1, ptPos2 );
			if( trackerRect3.PtInRect( ptGraphPos ) )
			{
				pAngle->bAngleHitTest = TRUE;
				return GraphHitTest_L;	// 标注
			}

			// 弧所在大致区域
			CRect trackerRect4	= pAngle->AP.rcBound;
//			trackerRect4.InflateRect(trackerRect4.Width(), trackerRect3.Height());
//			CLineTracker tracker4( &trackerRect4, CRectTracker::resizeOutside | CRectTracker::hatchedBorder );
//			int nHitTest4 = tracker4.HitTest( ptGraphPos );
//			if( nHitTest4 == 8 )
			if (trackerRect4.PtInRect(ptGraphPos))
			{
				pAngle->bAngleHitTest = TRUE;
				return GraphHitTest_In;	// 整体
			}
		}
		m_plistAngle->GetNext( pos );
	}

	return GraphHitTest_NULL;
}

void CDllTestView::OnChangeAngleOptions() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		if( pAngle->bAngleHitTest )
		{
			CDlgAngleOptions dlgAO;
			dlgAO.m_AO = pAngle->AO;
			if( dlgAO.DoModal() == IDOK )
			{
				if( pAngle->AO.clrLine != dlgAO.m_AO.clrLine )
					pAngle->AO.clrLine = dlgAO.m_AO.clrLine;
				if( pAngle->AO.clrArc != dlgAO.m_AO.clrArc )
					pAngle->AO.clrArc = dlgAO.m_AO.clrArc;
				if( pAngle->AO.clrText != dlgAO.m_AO.clrText )
					pAngle->AO.clrText = dlgAO.m_AO.clrText;
				if( pAngle->AO.nSize != dlgAO.m_AO.nSize )
					pAngle->AO.nSize = dlgAO.m_AO.nSize;
			}
			break;
		}
		m_plistAngle->GetNext( pos );
	}
	Invalidate(FALSE);
}

void CDllTestView::OnChangeAngleDelete() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_plistAngle->GetHeadPosition();
	while( pos )
	{
		AngleMeasure* pAngle = m_plistAngle->GetAt( pos );
		if( pAngle->bAngleHitTest )
		{
			// 有要删除的
			m_plistAngle->RemoveAt( pos );
			delete pAngle;
			break;
		}
		m_plistAngle->GetNext( pos );
	}
	m_nAngleCounts = m_plistAngle->GetCount();
	Invalidate(FALSE);
}

void CDllTestView::OnTestLine() 
{
	// TODO: Add your command handler code here
	m_bLine = !m_bLine;
}

void CDllTestView::OnUpdateTestLine(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_bLine );
}
