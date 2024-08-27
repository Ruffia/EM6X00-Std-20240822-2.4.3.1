// DlgDiagLogGraph.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgDiagLogGraph.h"
#include "BoardComm.h"
#include "ControlLayer.h"
#include "GraphBuffManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const double g_dGraphRangeX[] = {1024, 512, 256, 128, 64, 32, 16};
const int	g_dGraphRangeMaxIndexX = sizeof(g_dGraphRangeX) / sizeof(double) - 1;

const double g_dGraphRangeY[] = {10, 5, 2, 1, 0.5, 0.2, 0.1, 0.05, 0.02, 0.01, 0.005, 0.002, 0.001, 0.0005, 0.0002, 0.0001, 0.00005, 0.00002, 0.00001, 0.000005, 0.000002, 0.000001};
const int	g_dGraphRangeMaxIndexY = sizeof(g_dGraphRangeY) / sizeof(double) - 1;

/////////////////////////////////////////////////////////////////////////////
// CDlgDiagLogGraph dialog


CDlgDiagLogGraph::CDlgDiagLogGraph(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDiagLogGraph::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDiagLogGraph)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgDiagLogGraph::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDiagLogGraph)
	DDX_Control(pDX, IDC_SCROLLBAR_Graph4_X, m_scroll4);
	DDX_Control(pDX, IDC_SCROLLBAR_Graph3_X, m_scroll3);
	DDX_Control(pDX, IDC_SCROLLBAR_Graph2_X, m_scroll2);
	DDX_Control(pDX, IDC_SCROLLBAR_Graph1_X, m_scroll1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDiagLogGraph, CDialog)
	//{{AFX_MSG_MAP(CDlgDiagLogGraph)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_Graph1_Setup, OnBUTTONGraph1Setup)
	ON_BN_CLICKED(IDC_BUTTON_Graph2_Setup, OnBUTTONGraph2Setup)
	ON_BN_CLICKED(IDC_BUTTON_Graph3_Setup, OnBUTTONGraph3Setup)
	ON_BN_CLICKED(IDC_BUTTON_Graph4_Setup, OnBUTTONGraph4Setup)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Graph1_Y, OnDeltaposSPINGraph1Y)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Graph2_Y, OnDeltaposSPINGraph2Y)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Graph3_Y, OnDeltaposSPINGraph3Y)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Graph4_Y, OnDeltaposSPINGraph4Y)
	ON_BN_CLICKED(IDC_BUTTON_Graph1_Default, OnBUTTONGraph1Default)
	ON_BN_CLICKED(IDC_BUTTON_Graph2_Default, OnBUTTONGraph2Default)
	ON_BN_CLICKED(IDC_BUTTON_Graph3_Default, OnBUTTONGraph3Default)
	ON_BN_CLICKED(IDC_BUTTON_Graph4_Default, OnBUTTONGraph4Default)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Graph1_X, OnDeltaposSPINGraph1X)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Graph2_X, OnDeltaposSPINGraph2X)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Graph3_X, OnDeltaposSPINGraph3X)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Graph4_X, OnDeltaposSPINGraph4X)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_Graph_SaveGraph, OnBUTTONGraphSaveGraph)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDiagLogGraph message handlers

void CDlgDiagLogGraph::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	for( int i=0; i<GraphDataBuffGroups; i++ )
		PaintGraph( &dc, i );
//		PaintGraph_FromNodes( &dc, i );

	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDlgDiagLogGraph::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nTotal = -1;
	GetDlgItem( IDC_EDIT_Graph1_Min )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph1_Max )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph2_Min )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph2_Max )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph3_Min )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph3_Max )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph4_Min )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph4_Max )->SetWindowText( "0.000000" );

	for( int i=0; i<GraphDataBuffGroups; i++ )
	{
		CControlLayer::Instance().m_DL.graph[i].nZoomXIndex = 3;	// 横向显示128个数据
		CControlLayer::Instance().m_DL.graph[i].nZoomYIndex = -1;

		CControlLayer::Instance().m_DL.graph[i].dXMax = g_dGraphRangeX[3];
		CControlLayer::Instance().m_DL.graph[i].dXMin = 0;

		CControlLayer::Instance().m_DL.graph[i].dYMin = 0.0;
		CControlLayer::Instance().m_DL.graph[i].dYMax = 0.0;
	}
	m_scroll1.SetScrollRange( 0, (int)(CControlLayer::Instance().m_DL.graph[0].dXMax) );
	m_scroll1.ShowWindow( TRUE );
	m_scroll2.SetScrollRange( 0, (int)(CControlLayer::Instance().m_DL.graph[1].dXMax) );
	m_scroll2.ShowWindow( TRUE );
	m_scroll3.SetScrollRange( 0, (int)(CControlLayer::Instance().m_DL.graph[2].dXMax) );
	m_scroll3.ShowWindow( TRUE );
	m_scroll4.SetScrollRange( 0, (int)(CControlLayer::Instance().m_DL.graph[3].dXMax) );
	m_scroll4.ShowWindow( TRUE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDiagLogGraph::InitGraphData()
{
	CString str;
	GetDlgItem( IDC_EDIT_Graph1_Min )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[0].dYMin = atof( str );
	GetDlgItem( IDC_EDIT_Graph1_Max )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[0].dYMax = atof( str );
	GetDlgItem( IDC_EDIT_Graph2_Min )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[1].dYMin = atof( str );
	GetDlgItem( IDC_EDIT_Graph2_Max )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[1].dYMax = atof( str );
	GetDlgItem( IDC_EDIT_Graph3_Min )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[2].dYMin = atof( str );
	GetDlgItem( IDC_EDIT_Graph3_Max )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[2].dYMax = atof( str );
	GetDlgItem( IDC_EDIT_Graph4_Min )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[3].dYMin = atof( str );
	GetDlgItem( IDC_EDIT_Graph4_Max )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[3].dYMax = atof( str );

	for( int i=0; i<GraphDataBuffGroups; i++ )
		CControlLayer::Instance().m_DL.graph[i].dXMin = 0.0;

	m_nIntv = CControlLayer::Instance().m_DL.other.nElapseLogging;
}

void CDlgDiagLogGraph::UpdateGraphData( int nTotal )
{
	if( nTotal == 0 )
		return;
	m_nTotal = nTotal;

	m_scroll1.SetScrollRange(0, m_nTotal);
//	m_scroll1.SetScrollPos(m_nTotal);
	m_scroll2.SetScrollRange(0, m_nTotal);
	m_scroll3.SetScrollRange(0, m_nTotal);
	m_scroll4.SetScrollRange(0, m_nTotal);

	for( int i=0; i<GraphDataBuffGroups; i++ )
	{
		if( CControlLayer::Instance().m_DL.graph[i].nZoomXIndex > -1 )
		{
			CControlLayer::Instance().m_DL.graph[i].dXMax = m_nTotal;
			CControlLayer::Instance().m_DL.graph[i].dXMin = m_nTotal - g_dGraphRangeX[CControlLayer::Instance().m_DL.graph[i].nZoomXIndex];
		}
	}
	CString str;
	str.Format( "%d", nTotal +1);
	GetDlgItem( IDC_EDIT_Graph_TotalCounts )->SetWindowText( str );
	Invalidate( FALSE );
}

void CDlgDiagLogGraph::PaintGraph( CDC* pDC, int nIndex, BOOL bErase )
{
	// TODO: Add your message handler code here
	/////////////////////////////////////////////////////////////////
	// 无论画与不画，都将绘图区域清空
	CRect rc, rc1;
	GetDlgItem( IDC_STATIC_Logging_Graph1 +nIndex)->GetWindowRect( (LPRECT)rc );
	ScreenToClient( (LPRECT)rc );
	pDC->Rectangle( CRect(rc.left +10, rc.top +10, rc.right -10, rc.bottom -10) );

	if( !bErase )
	{
		CBrush brush, *pOldBrush;
		brush.CreateSysColorBrush(COLOR_BTNFACE);
		pOldBrush = (CBrush*)pDC->SelectObject(&brush);
		CString str = "100";
		CSize sz = pDC->GetTextExtent( str );
		pDC->FillRect( CRect(rc.left, rc.bottom -10, rc.right, rc.bottom -10 +sz.cy +1), &brush );
		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();
	}
	/////////////////////////////////////////////////////////////////

	if( !(CControlLayer::Instance().m_DL.graph[nIndex].bGraph) || (m_nTotal < 2) )
		return;

	/////////////////////////////////////////////////////////////////
	// 设置变量
	double dYMin, dYMax, dRatio;
	int nHeight, xMin, xMax, y, nTotal, i, xIntMain, xInt;
	// 网格画笔初始化
	CPen pen, penGrid, *poldpen;
	penGrid.CreatePen( PS_DOT, 1, RGB(192,192,192) );
	int noldBKmode;
	CString str = "100";
	CSize sz = pDC->GetTextExtent( str );

	// 画图区域
	rc1 = rc;
	rc1.left	+= 20;
	rc1.right	-= 20;
	rc1.top		+= 20;
	rc1.bottom	-= 20;
	nHeight = rc1.Height();
	nTotal = m_nTotal;

	// X向绘图比例
	if( CControlLayer::Instance().m_DL.graph[nIndex].nZoomXIndex > -1 )	// 有设置值
	{
		xMin = (int)(CControlLayer::Instance().m_DL.graph[nIndex].dXMin);
		xMax = (int)(CControlLayer::Instance().m_DL.graph[nIndex].dXMax);
		if( xMax > m_nTotal )
			xMax = m_nTotal;
		if( xMin >= xMax )
			xMin = xMax -2;
		if( xMin < 0 )
			xMin = 0;
	}
	else
	{
		xMin = 0;
		xMax = m_nTotal;
	}

	// 数据个数
	nTotal = xMax - xMin;
	dRatio = 1.0 * rc1.Width() / nTotal;

	// 纵坐标最大值、最小值
	if( (CControlLayer::Instance().m_DL.graph[nIndex].nZoomYIndex > -1)
		|| ((CControlLayer::Instance().m_DL.graph[nIndex].nZoomYIndex == -1)
			&& ( (CControlLayer::Instance().m_DL.graph[nIndex].dYMin != 0.0)
				|| (CControlLayer::Instance().m_DL.graph[nIndex].dYMax != 0.0) ) ) )	// 如果有设置值
	{
		dYMin = CControlLayer::Instance().m_DL.graph[nIndex].dYMin;
		dYMax = CControlLayer::Instance().m_DL.graph[nIndex].dYMax;
	}
	else
	{
		dYMin = (CGraphBuffMgr::Instance().m_pGraphDataBuff[nIndex])[xMin];
		dYMax = (CGraphBuffMgr::Instance().m_pGraphDataBuff[nIndex])[xMin];
		for( i=xMin+1; i<xMax; i++ )
		{
			if( CGraphBuffMgr::Instance().m_pGraphDataBuff[nIndex][i] < dYMin
				&& CGraphBuffMgr::Instance().m_pGraphDataBuff[nIndex][i] != 0.0 )
				dYMin = CGraphBuffMgr::Instance().m_pGraphDataBuff[nIndex][i];
			if( CGraphBuffMgr::Instance().m_pGraphDataBuff[nIndex][i] > dYMax
				&& CGraphBuffMgr::Instance().m_pGraphDataBuff[nIndex][i] > dYMax != 0.0 )
				dYMax = CGraphBuffMgr::Instance().m_pGraphDataBuff[nIndex][i];
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// 画坐标轴
	noldBKmode = pDC->SetBkMode(TRANSPARENT);
	// 纵轴
	for( i=0; i<9; i++ )
	{
		if( i % 4 == 0 )		// 主要网格
		{
			y = rc1.top + i*nHeight/8;
			pDC->MoveTo( rc1.left -20, y );
			pDC->LineTo( rc1.left -10, y );
			poldpen = pDC->SelectObject( &penGrid );
			pDC->LineTo( rc1.right, y );
			pDC->SelectObject( poldpen );
		}
		else if( i % 2 == 0 )	// 次要网格
		{
			y = rc1.top + i*nHeight/8;
			pDC->MoveTo( rc1.left -15, y );
			pDC->LineTo( rc1.left -10, y );
			poldpen = pDC->SelectObject( &penGrid );
			pDC->LineTo( rc1.right, y );
			pDC->SelectObject( poldpen );
		}
	}
	// 横轴
	if( dRatio > 30 )
	{
		xIntMain	= 5;
		xInt		= 1;
	}
	else if( dRatio > 12 )
	{
		xIntMain	= 10;
		xInt		= 2;
	}
	else if( dRatio > 3 )
	{
		xIntMain	= 20;
		xInt		= 5;
	}
	else if( dRatio > 0.6 )
	{
		xIntMain	= 50;
		xInt		= 10;
	}
	else if( dRatio > 0.3 )
	{
		xIntMain	= 100;
		xInt		= 20;
	}
	else if( dRatio > 0.15 )
	{
		xIntMain	= 200;
		xInt		= 50;
	}
	else
	{
		xIntMain	= 500;
		xInt		= 100;
	}
	for( i=xMin; i<=xMax; i++ )
	{
		if( i % xIntMain == 0 )		// 主要网格
		{
			pDC->MoveTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.bottom +10 );
			pDC->LineTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.bottom );
			poldpen = pDC->SelectObject( &penGrid );
			pDC->LineTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.top );
			pDC->SelectObject( poldpen );
			str.Format( "%.1f", i * m_nIntv / 1000.0 );
			sz = pDC->GetTextExtent( str );
			pDC->TextOut( (int)(rc1.left +(i -xMin) *dRatio -sz.cx/2), rc1.bottom +12, str );
		}
		else if( i % xInt == 0 )	// 次要网格
		{
			pDC->MoveTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.bottom +10 );
			pDC->LineTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.bottom +5 );
			if( dRatio > 8 )
			{
				poldpen = pDC->SelectObject( &penGrid );
				pDC->LineTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.top );
				pDC->SelectObject( poldpen );
				str.Format( "%.1f", i * m_nIntv / 1000.0 );
				sz = pDC->GetTextExtent( str );
				pDC->TextOut( (int)(rc1.left +(i -xMin) *dRatio -sz.cx/2), rc1.bottom +12, str );
			}
		}
	}
	pDC->SetBkMode(noldBKmode);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// 画数据区
	// 选择画笔
	// 画数据区
	switch( nIndex )
	{
	case	0:
		pen.CreatePen( PS_SOLID, 2, RGB(255,0,0));
		break;
	case	1:
		pen.CreatePen( PS_SOLID, 2, RGB(0,128,0));
		break;
	case	2:
		pen.CreatePen( PS_SOLID, 2, RGB(0,0,255));
		break;
	case	3:
		pen.CreatePen( PS_SOLID, 2, RGB(255,0,255));
		break;
	}
	// 画笔移到起始点
	y = (int)(rc1.top + ( nHeight - (CGraphBuffMgr::Instance().m_pGraphDataBuff[nIndex][xMin] -dYMin) / (dYMax-dYMin) * nHeight ));
	if( y < rc1.top )
		y = rc1.top;
	if( y > rc1.bottom )
		y = rc1.bottom;
	pDC->MoveTo( rc1.left, y );
	// 画折线图
	int x;
	poldpen = pDC->SelectObject( &pen );
	for( i=xMin+1; i<xMax; i++ )
	{
		if( CGraphBuffMgr::Instance().m_pGraphDataBuff[nIndex][i] != 0.0 )
		{
			y = (int)(rc1.top + ( nHeight - (CGraphBuffMgr::Instance().m_pGraphDataBuff[nIndex][i] -dYMin) / (dYMax-dYMin) * nHeight ));
			if( (y >= rc1.top) && (y <= rc1.bottom) )
			{
				x = (int)(rc1.left +(i-xMin)*dRatio);
				pDC->LineTo( x, y );
				pDC->Ellipse( CRect(x-2, y-2, x+2, y+2) );
			}
		}
	}
	pDC->SelectObject(poldpen);
	/////////////////////////////////////////////////////////////////
	// 标注纵向最小值
	noldBKmode = pDC->SetBkMode(TRANSPARENT);
	double dValue = CBoardComm::Instance().GetPhysicsFromAdcs(CControlLayer::Instance().m_DL.graph[nIndex].nIndex, dYMin, CControlLayer::Instance().m_DL.log.strCalc);
	str.Format( "%.6f (%.6f)", dYMin, dValue );
	pDC->TextOut( rc1.left -8, rc1.bottom -sz.cy/2, str );
	// 标注纵向最大值
	dValue = CBoardComm::Instance().GetPhysicsFromAdcs(CControlLayer::Instance().m_DL.graph[nIndex].nIndex, dYMax, CControlLayer::Instance().m_DL.log.strCalc);
	str.Format( "%.6f (%.6f)", dYMax, dValue );
	pDC->TextOut( rc1.left -8, rc1.top -sz.cy/2, str );
	// 画标签
	str = CControlLayer::Instance().m_DL.adc.chn[CControlLayer::Instance().m_DL.graph[nIndex].nIndex].strLabel;
	sz = pDC->GetTextExtent( str );
	pDC->TextOut( rc.left + rc.Width()/2 - sz.cx/2, rc.top +11, str );
	pDC->SetBkMode(noldBKmode);

	pen.DeleteObject();
	penGrid.DeleteObject();
}

void CDlgDiagLogGraph::PaintGraph_FromNodes( CDC* pDC, int nIndex )
{
/*	// TODO: Add your message handler code here
	/////////////////////////////////////////////////////////////////
	// 无论画与不画，都将绘图区域清空
	CRect rc, rc1;
	GetDlgItem( IDC_STATIC_Logging_Graph1 +nIndex)->GetWindowRect( (LPRECT)rc );
	ScreenToClient( (LPRECT)rc );
	pDC->Rectangle( CRect(rc.left +10, rc.top +10, rc.right -10, rc.bottom -10) );

	CBrush brush, *pOldBrush;
	brush.CreateSysColorBrush(COLOR_BTNFACE);
	pOldBrush = (CBrush*)pDC->SelectObject(&brush);
	CString str = "100";
	CSize sz = pDC->GetTextExtent( str );
	pDC->FillRect( CRect(rc.left, rc.bottom -10, rc.right, rc.bottom -10 +sz.cy +1), &brush );
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
	/////////////////////////////////////////////////////////////////

	if( !(CControlLayer::Instance().m_DL.graph[nIndex].bGraph) || (m_nTotal < 2) 
		|| (CControlLayer::Instance().m_DL.graph[nIndex].link == NULL) )
		return;

	/////////////////////////////////////////////////////////////////
	// 设置变量
	double dYMin, dYMax, dRatio;
	int nHeight, xMin, xMax, y, nTotal, i, xIntMain, xInt;
	// 网格画笔初始化
	CPen pen, penGrid, *poldpen;
	penGrid.CreatePen( PS_DOT, 1, RGB(192,192,192) );
	int noldBKmode;
	
	// 画图区域
	rc1 = rc;
	rc1.left	+= 20;
	rc1.right	-= 20;
	rc1.top		+= 20;
	rc1.bottom	-= 20;
	nHeight = rc1.Height();
	nTotal = m_nTotal;

	// X向绘图比例
	if( CControlLayer::Instance().m_DL.graph[nIndex].nZoomXIndex > -1 )	// 有设置值
	{
		xMin = (int)(CControlLayer::Instance().m_DL.graph[nIndex].dXMin);
		xMax = (int)(CControlLayer::Instance().m_DL.graph[nIndex].dXMax);
		if( xMax > m_nTotal )
			xMax = m_nTotal;
		if( xMin >= xMax )
			xMin = xMax -2;
		if( xMin < 0 )
			xMin = 0;
	}
	else
	{
		xMin = 0;
		xMax = m_nTotal;
	}
	// 找到数据链表中的用于绘图的最小值
	LinkList p = CControlLayer::Instance().m_DL.graph[nIndex].link->next;	// 第一个数据
	for( i=0; i<xMin; i++ )
		p = p->next;
	if( p == NULL )
		return;

	// 数据个数
	nTotal = xMax - xMin;
	dRatio = 1.0 * rc1.Width() / nTotal;
	LinkList pMin = p;

	// 纵坐标最大值、最小值
	if( CControlLayer::Instance().m_DL.graph[nIndex].nZoomYIndex > -1 )	// 如果有设置值
		dYMin = CControlLayer::Instance().m_DL.graph[nIndex].dYMin;
	else
	{
		p = pMin;
		dYMin = p->data;
		for( i=xMin+1; i<xMax; i++ )
		{
			if( p->next->data < dYMin )
				dYMin = p->next->data;
			p = p->next;
		}
		if( p->data < dYMin )
			dYMin = p->data;
	}
	if( CControlLayer::Instance().m_DL.graph[nIndex].nZoomYIndex > -1 )	// 如果有设置值
		dYMax = CControlLayer::Instance().m_DL.graph[nIndex].dYMax;
	else
	{
		p = pMin;
		dYMax = p->data;
		for( i=xMin+1; i<xMax; i++ )
		{
			if( p->next->data > dYMax )
				dYMax = p->next->data;
			p = p->next;
		}
		if( p->data > dYMax )
			dYMax = p->data;
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// 画坐标轴
	noldBKmode = pDC->SetBkMode(TRANSPARENT);
	// 纵轴
	for( i=0; i<9; i++ )
	{
		if( i % 4 == 0 )		// 主要网格
		{
			y = rc1.top + i*nHeight/8;
			pDC->MoveTo( rc1.left -20, y );
			pDC->LineTo( rc1.left -10, y );
			poldpen = pDC->SelectObject( &penGrid );
			pDC->LineTo( rc1.right, y );
			pDC->SelectObject( poldpen );
		}
		else if( i % 2 == 0 )	// 次要网格
		{
			y = rc1.top + i*nHeight/8;
			pDC->MoveTo( rc1.left -15, y );
			pDC->LineTo( rc1.left -10, y );
			poldpen = pDC->SelectObject( &penGrid );
			pDC->LineTo( rc1.right, y );
			pDC->SelectObject( poldpen );
		}
	}
	// 横轴
	if( dRatio > 30 )
	{
		xIntMain	= 5;
		xInt		= 1;
	}
	else if( dRatio > 12 )
	{
		xIntMain	= 10;
		xInt		= 2;
	}
	else if( dRatio > 3 )
	{
		xIntMain	= 20;
		xInt		= 5;
	}
	else if( dRatio > 0.6 )
	{
		xIntMain	= 50;
		xInt		= 10;
	}
	else if( dRatio > 0.3 )
	{
		xIntMain	= 100;
		xInt		= 20;
	}
	else if( dRatio > 0.15 )
	{
		xIntMain	= 200;
		xInt		= 50;
	}
	else
	{
		xIntMain	= 500;
		xInt		= 100;
	}
	for( i=xMin; i<=xMax; i++ )
	{
		if( i % xIntMain == 0 )		// 主要网格
		{
			pDC->MoveTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.bottom +10 );
			pDC->LineTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.bottom );
			poldpen = pDC->SelectObject( &penGrid );
			pDC->LineTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.top );
			pDC->SelectObject( poldpen );
			str.Format( "%.1f", i * m_nIntv / 1000.0 );
			sz = pDC->GetTextExtent( str );
			pDC->TextOut( (int)(rc1.left +(i -xMin) *dRatio -sz.cx/2), rc1.bottom +12, str );
		}
		else if( i % xInt == 0 )	// 次要网格
		{
			pDC->MoveTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.bottom +10 );
			pDC->LineTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.bottom +5 );
			if( dRatio > 8 )
			{
				poldpen = pDC->SelectObject( &penGrid );
				pDC->LineTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.top );
				pDC->SelectObject( poldpen );
				str.Format( "%.1f", i * m_nIntv / 1000.0 );
				sz = pDC->GetTextExtent( str );
				pDC->TextOut( (int)(rc1.left +(i -xMin) *dRatio -sz.cx/2), rc1.bottom +12, str );
			}
		}
	}
	pDC->SetBkMode(noldBKmode);

	// 画标签
	str = CControlLayer::Instance().m_DL.adc.chn[CControlLayer::Instance().m_DL.graph[nIndex].nIndex].strLabel;
	sz = pDC->GetTextExtent( str );
	pDC->TextOut( rc.left + rc.Width()/2 - sz.cx/2, rc.top +11, str );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// 画数据区
	// 选择画笔
	switch( nIndex )
	{
	case	0:
		pen.CreatePen( PS_SOLID, 2, RGB(255,0,0));
		break;
	case	1:
		pen.CreatePen( PS_SOLID, 2, RGB(0,128,0));
		break;
	case	2:
		pen.CreatePen( PS_SOLID, 2, RGB(0,0,255));
		break;
	case	3:
		pen.CreatePen( PS_SOLID, 2, RGB(255,0,255));
		break;
	}
	// 画笔移到起始点
	p = pMin;
	y = (int)(rc1.top + ( nHeight - (p->data -dYMin) / (dYMax-dYMin) * nHeight ));
	if( y < rc1.top )
		y = rc1.top;
	if( y > rc1.bottom )
		y = rc1.bottom;
	pDC->MoveTo( rc1.left, y );
	// 画折线图
	int x;
	poldpen = pDC->SelectObject( &pen );
	for( i=xMin+1; i<xMax; i++ )
	{
		y = (int)(rc1.top + ( nHeight - (p->next->data -dYMin) / (dYMax-dYMin) * nHeight ));
		if( (y >= rc1.top) && (y <= rc1.bottom) )
		{
			x = (int)(rc1.left +(i-xMin)*dRatio);
			pDC->LineTo( x, y );
			pDC->Ellipse( CRect(x-2, y-2, x+2, y+2) );
		}
		p = p->next;
	}
	// 画最后一个点
	y = (int)(rc1.top + ( nHeight - (p->data -dYMin) / (dYMax-dYMin) * nHeight ));
	if( (y >= rc1.top) && (y <= rc1.bottom) )
	{
		x = (int)(rc1.left +(i-xMin)*dRatio);
		pDC->LineTo( x, y );
		pDC->Ellipse( CRect(x-2, y-2, x+2, y+2) );
	}
	pDC->SelectObject(poldpen);
	/////////////////////////////////////////////////////////////////
	// 标注纵向最小值
	noldBKmode = pDC->SetBkMode(TRANSPARENT);
	double dValue = CBoardComm::Instance().GetPhysicsFromAdcs(CControlLayer::Instance().m_DL.graph[nIndex].nIndex, dYMin);
	str.Format( "%.6f (%.6f)", dYMin, dValue );
	pDC->TextOut( rc1.left -8, rc1.bottom -sz.cy/2, str );
	// 标注纵向最大值
	dValue = CBoardComm::Instance().GetPhysicsFromAdcs(CControlLayer::Instance().m_DL.graph[nIndex].nIndex, dYMax);
	str.Format( "%.6f (%.6f)", dYMax, dValue );
	pDC->TextOut( rc1.left -8, rc1.top -sz.cy/2, str );
	pDC->SetBkMode(noldBKmode);

	pen.DeleteObject();
	penGrid.DeleteObject();*/
}

void CDlgDiagLogGraph::OnBUTTONGraph1Setup() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem( IDC_EDIT_Graph1_Min )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[0].dYMin = atof( str );
	GetDlgItem( IDC_EDIT_Graph1_Max )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[0].dYMax = atof( str );
	CControlLayer::Instance().m_DL.graph[0].nZoomYIndex = -1;
	Invalidate(FALSE);
}

void CDlgDiagLogGraph::OnBUTTONGraph2Setup() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem( IDC_EDIT_Graph2_Min )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[1].dYMin = atof( str );
	GetDlgItem( IDC_EDIT_Graph2_Max )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[1].dYMax = atof( str );
	CControlLayer::Instance().m_DL.graph[1].nZoomYIndex = -1;
	Invalidate(FALSE);
}

void CDlgDiagLogGraph::OnBUTTONGraph3Setup() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem( IDC_EDIT_Graph3_Min )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[2].dYMin = atof( str );
	GetDlgItem( IDC_EDIT_Graph3_Max )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[2].dYMax = atof( str );
	CControlLayer::Instance().m_DL.graph[2].nZoomYIndex = -1;
	Invalidate(FALSE);
}

void CDlgDiagLogGraph::OnBUTTONGraph4Setup() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem( IDC_EDIT_Graph4_Min )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[3].dYMin = atof( str );
	GetDlgItem( IDC_EDIT_Graph4_Max )->GetWindowText( str );
	CControlLayer::Instance().m_DL.graph[3].dYMax = atof( str );
	CControlLayer::Instance().m_DL.graph[3].nZoomYIndex = -1;
	Invalidate(FALSE);
}

void CDlgDiagLogGraph::OnDeltaposSPINGraph1Y(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		CControlLayer::Instance().m_DL.graph[0].nZoomYIndex++;
	else
		CControlLayer::Instance().m_DL.graph[0].nZoomYIndex--;
	if( CControlLayer::Instance().m_DL.graph[0].nZoomYIndex > g_dGraphRangeMaxIndexY )
		CControlLayer::Instance().m_DL.graph[0].nZoomYIndex = g_dGraphRangeMaxIndexY;
	if( CControlLayer::Instance().m_DL.graph[0].nZoomYIndex < -1 )
		CControlLayer::Instance().m_DL.graph[0].nZoomYIndex = -1;

	int nIndex = CControlLayer::Instance().m_DL.graph[0].nZoomYIndex;
	if( nIndex > -1 )
	{
		CControlLayer::Instance().m_DL.graph[0].dYMax = g_dGraphRangeY[nIndex];
		CControlLayer::Instance().m_DL.graph[0].dYMin = -g_dGraphRangeY[nIndex];
	}
	else
	{
		CControlLayer::Instance().m_DL.graph[0].dYMax = 0.0;
		CControlLayer::Instance().m_DL.graph[0].dYMin = 0.0;
	}

	CString str;
	str.Format( "%.6f", CControlLayer::Instance().m_DL.graph[0].dYMin );
	GetDlgItem( IDC_EDIT_Graph1_Min )->SetWindowText( str );
	str.Format( "%.6f", CControlLayer::Instance().m_DL.graph[0].dYMax );
	GetDlgItem( IDC_EDIT_Graph1_Max )->SetWindowText( str );

	Invalidate(FALSE);
	*pResult = 0;
}

void CDlgDiagLogGraph::OnDeltaposSPINGraph2Y(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		CControlLayer::Instance().m_DL.graph[1].nZoomYIndex++;
	else
		CControlLayer::Instance().m_DL.graph[1].nZoomYIndex--;
	if( CControlLayer::Instance().m_DL.graph[1].nZoomYIndex > g_dGraphRangeMaxIndexY )
		CControlLayer::Instance().m_DL.graph[1].nZoomYIndex = g_dGraphRangeMaxIndexY;
	if( CControlLayer::Instance().m_DL.graph[1].nZoomYIndex < -1 )
		CControlLayer::Instance().m_DL.graph[1].nZoomYIndex = -1;

	int nIndex = CControlLayer::Instance().m_DL.graph[1].nZoomYIndex;
	if( nIndex > -1 )
	{
		CControlLayer::Instance().m_DL.graph[1].dYMax = g_dGraphRangeY[nIndex];
		CControlLayer::Instance().m_DL.graph[1].dYMin = -g_dGraphRangeY[nIndex];
	}
	else
	{
		CControlLayer::Instance().m_DL.graph[1].dYMax = 0.0;
		CControlLayer::Instance().m_DL.graph[1].dYMin = 0.0;
	}

	CString str;
	str.Format( "%.6f", CControlLayer::Instance().m_DL.graph[1].dYMin );
	GetDlgItem( IDC_EDIT_Graph2_Min )->SetWindowText( str );
	str.Format( "%.6f", CControlLayer::Instance().m_DL.graph[1].dYMax );
	GetDlgItem( IDC_EDIT_Graph2_Max )->SetWindowText( str );

	Invalidate(FALSE);
	*pResult = 0;
}

void CDlgDiagLogGraph::OnDeltaposSPINGraph3Y(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		CControlLayer::Instance().m_DL.graph[2].nZoomYIndex++;
	else
		CControlLayer::Instance().m_DL.graph[2].nZoomYIndex--;
	if( CControlLayer::Instance().m_DL.graph[2].nZoomYIndex > g_dGraphRangeMaxIndexY )
		CControlLayer::Instance().m_DL.graph[2].nZoomYIndex = g_dGraphRangeMaxIndexY;
	if( CControlLayer::Instance().m_DL.graph[2].nZoomYIndex < -1 )
		CControlLayer::Instance().m_DL.graph[2].nZoomYIndex = -1;

	int nIndex = CControlLayer::Instance().m_DL.graph[2].nZoomYIndex;
	if( nIndex > -1 )
	{
		CControlLayer::Instance().m_DL.graph[2].dYMax = g_dGraphRangeY[nIndex];
		CControlLayer::Instance().m_DL.graph[2].dYMin = -g_dGraphRangeY[nIndex];
	}
	else
	{
		CControlLayer::Instance().m_DL.graph[2].dYMax = 0.0;
		CControlLayer::Instance().m_DL.graph[2].dYMin = 0.0;
	}

	CString str;
	str.Format( "%.6f", CControlLayer::Instance().m_DL.graph[2].dYMin );
	GetDlgItem( IDC_EDIT_Graph3_Min )->SetWindowText( str );
	str.Format( "%.6f", CControlLayer::Instance().m_DL.graph[2].dYMax );
	GetDlgItem( IDC_EDIT_Graph3_Max )->SetWindowText( str );

	Invalidate(FALSE);
	*pResult = 0;
}

void CDlgDiagLogGraph::OnDeltaposSPINGraph4Y(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		CControlLayer::Instance().m_DL.graph[3].nZoomYIndex++;
	else
		CControlLayer::Instance().m_DL.graph[3].nZoomYIndex--;
	if( CControlLayer::Instance().m_DL.graph[3].nZoomYIndex > g_dGraphRangeMaxIndexY )
		CControlLayer::Instance().m_DL.graph[3].nZoomYIndex = g_dGraphRangeMaxIndexY;
	if( CControlLayer::Instance().m_DL.graph[3].nZoomYIndex < -1 )
		CControlLayer::Instance().m_DL.graph[3].nZoomYIndex = -1;

	int nIndex = CControlLayer::Instance().m_DL.graph[3].nZoomYIndex;
	if( nIndex > -1 )
	{
		CControlLayer::Instance().m_DL.graph[3].dYMax = g_dGraphRangeY[nIndex];
		CControlLayer::Instance().m_DL.graph[3].dYMin = -g_dGraphRangeY[nIndex];
	}
	else
	{
		CControlLayer::Instance().m_DL.graph[3].dYMax = 0.0;
		CControlLayer::Instance().m_DL.graph[3].dYMin = 0.0;
	}

	CString str;
	str.Format( "%.6f", CControlLayer::Instance().m_DL.graph[3].dYMin );
	GetDlgItem( IDC_EDIT_Graph4_Min )->SetWindowText( str );
	str.Format( "%.6f", CControlLayer::Instance().m_DL.graph[3].dYMax );
	GetDlgItem( IDC_EDIT_Graph4_Max )->SetWindowText( str );

	Invalidate(FALSE);
	*pResult = 0;
}

void CDlgDiagLogGraph::OnBUTTONGraph1Default() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem( IDC_EDIT_Graph1_Min )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph1_Max )->SetWindowText( "0.000000" );
	CControlLayer::Instance().m_DL.graph[0].dYMin = 0.0;
	CControlLayer::Instance().m_DL.graph[0].dYMax = 0.0;
	CControlLayer::Instance().m_DL.graph[0].nZoomYIndex = -1;

	CControlLayer::Instance().m_DL.graph[0].nZoomXIndex = 3;	// 横向显示128个数据
	CControlLayer::Instance().m_DL.graph[0].dXMax = g_dGraphRangeX[3];
	CControlLayer::Instance().m_DL.graph[0].dXMin = 0;
	m_scroll1.SetScrollRange( 0, CControlLayer::Instance().m_DL.graph[0].dXMax > m_nTotal ? (int)(CControlLayer::Instance().m_DL.graph[0].dXMax) : m_nTotal );
	m_scroll1.ShowWindow( TRUE );

	Invalidate(FALSE);
}

void CDlgDiagLogGraph::OnBUTTONGraph2Default() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem( IDC_EDIT_Graph2_Min )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph2_Max )->SetWindowText( "0.000000" );
	CControlLayer::Instance().m_DL.graph[1].dYMin = 0.0;
	CControlLayer::Instance().m_DL.graph[1].dYMax = 0.0;
	CControlLayer::Instance().m_DL.graph[1].nZoomYIndex = -1;

	CControlLayer::Instance().m_DL.graph[1].nZoomXIndex = 3;	// 横向显示128个数据
	CControlLayer::Instance().m_DL.graph[1].dXMax = g_dGraphRangeX[3];
	CControlLayer::Instance().m_DL.graph[1].dXMin = 0;
	m_scroll2.SetScrollRange( 0, CControlLayer::Instance().m_DL.graph[1].dXMax > m_nTotal ? (int)(CControlLayer::Instance().m_DL.graph[1].dXMax) : m_nTotal );
	m_scroll2.ShowWindow( TRUE );

	Invalidate(FALSE);
}

void CDlgDiagLogGraph::OnBUTTONGraph3Default() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem( IDC_EDIT_Graph3_Min )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph3_Max )->SetWindowText( "0.000000" );
	CControlLayer::Instance().m_DL.graph[2].dYMin = 0.0;
	CControlLayer::Instance().m_DL.graph[2].dYMax = 0.0;
	CControlLayer::Instance().m_DL.graph[2].nZoomYIndex = -1;

	CControlLayer::Instance().m_DL.graph[2].nZoomXIndex = 3;	// 横向显示128个数据
	CControlLayer::Instance().m_DL.graph[2].dXMax = g_dGraphRangeX[3];
	CControlLayer::Instance().m_DL.graph[2].dXMin = 0;
	m_scroll3.SetScrollRange( 0, CControlLayer::Instance().m_DL.graph[2].dXMax > m_nTotal ? (int)(CControlLayer::Instance().m_DL.graph[2].dXMax) : m_nTotal );
	m_scroll3.ShowWindow( TRUE );

	Invalidate(FALSE);
}

void CDlgDiagLogGraph::OnBUTTONGraph4Default() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem( IDC_EDIT_Graph4_Min )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph4_Max )->SetWindowText( "0.000000" );
	CControlLayer::Instance().m_DL.graph[3].dYMin = 0.0;
	CControlLayer::Instance().m_DL.graph[3].dYMax = 0.0;
	CControlLayer::Instance().m_DL.graph[3].nZoomYIndex = -1;

	CControlLayer::Instance().m_DL.graph[3].nZoomXIndex = 3;	// 横向显示128个数据
	CControlLayer::Instance().m_DL.graph[3].dXMax = g_dGraphRangeX[3];
	CControlLayer::Instance().m_DL.graph[3].dXMin = 0;
	m_scroll4.SetScrollRange( 0, CControlLayer::Instance().m_DL.graph[3].dXMax > m_nTotal ? (int)(CControlLayer::Instance().m_DL.graph[3].dXMax) : m_nTotal );
	m_scroll4.ShowWindow( TRUE );

	Invalidate(FALSE);
}

void CDlgDiagLogGraph::OnDeltaposSPINGraph1X(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		CControlLayer::Instance().m_DL.graph[0].nZoomXIndex++;
	else
		CControlLayer::Instance().m_DL.graph[0].nZoomXIndex--;
	if( CControlLayer::Instance().m_DL.graph[0].nZoomXIndex > g_dGraphRangeMaxIndexX )
		CControlLayer::Instance().m_DL.graph[0].nZoomXIndex = g_dGraphRangeMaxIndexX;
	if( CControlLayer::Instance().m_DL.graph[0].nZoomXIndex < -1 )
		CControlLayer::Instance().m_DL.graph[0].nZoomXIndex = -1;

	int nIndex = CControlLayer::Instance().m_DL.graph[0].nZoomXIndex;
	if( nIndex > -1 )
	{
		CControlLayer::Instance().m_DL.graph[0].dXMax = g_dGraphRangeX[nIndex];
		CControlLayer::Instance().m_DL.graph[0].dXMin = 0;
		m_scroll1.SetScrollRange( 0, CControlLayer::Instance().m_DL.graph[0].dXMax > m_nTotal ? (int)(CControlLayer::Instance().m_DL.graph[0].dXMax) : m_nTotal );
		m_scroll1.ShowWindow( TRUE );
	}
	else
		m_scroll1.ShowWindow( FALSE );

	Invalidate(FALSE);
	*pResult = 0;
}

void CDlgDiagLogGraph::OnDeltaposSPINGraph2X(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		CControlLayer::Instance().m_DL.graph[1].nZoomXIndex++;
	else
		CControlLayer::Instance().m_DL.graph[1].nZoomXIndex--;
	if( CControlLayer::Instance().m_DL.graph[1].nZoomXIndex > g_dGraphRangeMaxIndexX )
		CControlLayer::Instance().m_DL.graph[1].nZoomXIndex = g_dGraphRangeMaxIndexX;
	if( CControlLayer::Instance().m_DL.graph[1].nZoomXIndex < -1 )
		CControlLayer::Instance().m_DL.graph[1].nZoomXIndex = -1;

	int nIndex = CControlLayer::Instance().m_DL.graph[1].nZoomXIndex;
	if( nIndex > -1 )
	{
		CControlLayer::Instance().m_DL.graph[1].dXMax = g_dGraphRangeX[nIndex];
		CControlLayer::Instance().m_DL.graph[1].dXMin = 0;
		m_scroll2.SetScrollRange( 0, CControlLayer::Instance().m_DL.graph[1].dXMax > m_nTotal ? (int)(CControlLayer::Instance().m_DL.graph[1].dXMax) : m_nTotal );
		m_scroll2.ShowWindow( TRUE );
	}
	else
		m_scroll2.ShowWindow( FALSE );

	Invalidate(FALSE);
	*pResult = 0;
}

void CDlgDiagLogGraph::OnDeltaposSPINGraph3X(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		CControlLayer::Instance().m_DL.graph[2].nZoomXIndex++;
	else
		CControlLayer::Instance().m_DL.graph[2].nZoomXIndex--;
	if( CControlLayer::Instance().m_DL.graph[2].nZoomXIndex > g_dGraphRangeMaxIndexX )
		CControlLayer::Instance().m_DL.graph[2].nZoomXIndex = g_dGraphRangeMaxIndexX;
	if( CControlLayer::Instance().m_DL.graph[2].nZoomXIndex < -1 )
		CControlLayer::Instance().m_DL.graph[2].nZoomXIndex = -1;

	int nIndex = CControlLayer::Instance().m_DL.graph[2].nZoomXIndex;
	if( nIndex > -1 )
	{
		CControlLayer::Instance().m_DL.graph[2].dXMax = g_dGraphRangeX[nIndex];
		CControlLayer::Instance().m_DL.graph[2].dXMin = 0;
		m_scroll3.SetScrollRange( 0, CControlLayer::Instance().m_DL.graph[2].dXMax > m_nTotal ? (int)(CControlLayer::Instance().m_DL.graph[2].dXMax) : m_nTotal );
		m_scroll3.ShowWindow( TRUE );
	}
	else
		m_scroll3.ShowWindow( FALSE );

	Invalidate(FALSE);
	*pResult = 0;
}

void CDlgDiagLogGraph::OnDeltaposSPINGraph4X(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		CControlLayer::Instance().m_DL.graph[3].nZoomXIndex++;
	else
		CControlLayer::Instance().m_DL.graph[3].nZoomXIndex--;
	if( CControlLayer::Instance().m_DL.graph[3].nZoomXIndex > g_dGraphRangeMaxIndexX )
		CControlLayer::Instance().m_DL.graph[3].nZoomXIndex = g_dGraphRangeMaxIndexX;
	if( CControlLayer::Instance().m_DL.graph[3].nZoomXIndex < -1 )
		CControlLayer::Instance().m_DL.graph[3].nZoomXIndex = -1;

	int nIndex = CControlLayer::Instance().m_DL.graph[3].nZoomXIndex;
	if( nIndex > -1 )
	{
		CControlLayer::Instance().m_DL.graph[3].dXMax = g_dGraphRangeX[nIndex];
		CControlLayer::Instance().m_DL.graph[3].dXMin = 0;
		m_scroll4.SetScrollRange( 0, CControlLayer::Instance().m_DL.graph[3].dXMax > m_nTotal ? (int)(CControlLayer::Instance().m_DL.graph[3].dXMax) : m_nTotal );
		m_scroll4.ShowWindow( TRUE );
	}
	else
		m_scroll4.ShowWindow( FALSE );

	Invalidate(FALSE);
	*pResult = 0;
}

void CDlgDiagLogGraph::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nOldPos = pScrollBar->GetScrollPos();
	int nNewPos = pScrollBar->GetScrollPos();
	SCROLLINFO  scrollInfo;
	pScrollBar->GetScrollInfo( &scrollInfo, SIF_ALL );
	switch (nSBCode) 
	{
	 case SB_LINELEFT:
		nNewPos = nOldPos-1;								//步进量为1
		nNewPos = ( nNewPos < scrollInfo.nMin )?scrollInfo.nMin:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	 case SB_LINERIGHT:
		nNewPos = nOldPos+1;
		nNewPos = ( nNewPos > scrollInfo.nMax )?scrollInfo.nMax:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	 case SB_LEFT:
		pScrollBar->SetScrollPos( scrollInfo.nMax );
		break;

	 case SB_RIGHT:
		pScrollBar->SetScrollPos( scrollInfo.nMin );
		break;

	 case SB_PAGELEFT:
		nNewPos = nOldPos-10;//scrollInfo.nPage;			//页进量为10
		nNewPos = ( nNewPos < scrollInfo.nMin )?scrollInfo.nMin:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	 case SB_PAGERIGHT:
		nNewPos = nOldPos+10;//scrollInfo.nPage;
		nNewPos = ( nNewPos > scrollInfo.nMax )?scrollInfo.nMax:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	 case SB_ENDSCROLL:
		break;

	 case SB_THUMBPOSITION:
		pScrollBar->SetScrollPos( nPos );
		break;

	 case SB_THUMBTRACK:
		pScrollBar->SetScrollPos( scrollInfo.nTrackPos );
		break;
	}

	nNewPos = pScrollBar->GetScrollPos();
	if( nOldPos == nNewPos )
		return;

	int id = pScrollBar->GetDlgCtrlID();					//获取滚动条ID
	switch( id )
	{
	case IDC_SCROLLBAR_Graph1_X:
		Graph1XSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Graph2_X:
		Graph2XSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Graph3_X:
		Graph3XSB_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Graph4_X:
		Graph4XSB_Change( pScrollBar->GetScrollPos() );
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgDiagLogGraph::Graph1XSB_Change( int nPos )
{
	CControlLayer::Instance().m_DL.graph[0].dXMin = nPos;
	CControlLayer::Instance().m_DL.graph[0].dXMax = nPos + g_dGraphRangeX[CControlLayer::Instance().m_DL.graph[0].nZoomXIndex];
	Invalidate(FALSE);
}

void CDlgDiagLogGraph::Graph2XSB_Change( int nPos )
{
	CControlLayer::Instance().m_DL.graph[1].dXMin = nPos;
	CControlLayer::Instance().m_DL.graph[1].dXMax = nPos + g_dGraphRangeX[CControlLayer::Instance().m_DL.graph[1].nZoomXIndex];
	Invalidate(FALSE);
}

void CDlgDiagLogGraph::Graph3XSB_Change( int nPos )
{
	CControlLayer::Instance().m_DL.graph[2].dXMin = nPos;
	CControlLayer::Instance().m_DL.graph[2].dXMax = nPos + g_dGraphRangeX[CControlLayer::Instance().m_DL.graph[2].nZoomXIndex];
	Invalidate(FALSE);
}

void CDlgDiagLogGraph::Graph4XSB_Change( int nPos )
{
	CControlLayer::Instance().m_DL.graph[3].dXMin = nPos;
	CControlLayer::Instance().m_DL.graph[3].dXMax = nPos + g_dGraphRangeX[CControlLayer::Instance().m_DL.graph[3].nZoomXIndex];
	Invalidate(FALSE);
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void CDlgDiagLogGraph::OnBUTTONGraphSaveGraph() 
{
	// TODO: Add your control notification handler code here
	CString filename;
	char szFilter[] = "BMP file|*.bmp|JPG file|*.jpg|PNG file|*.png|TIF file|*.tif|PCX file|*.pcx|TGA file|*.tga||";
	CFileDialog dlgFile(FALSE, "BMP", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	if( dlgFile.DoModal() == IDOK )
		filename = dlgFile.GetPathName();
	else
		return;
	SaveGraph( filename );
}

void CDlgDiagLogGraph::SaveGraph( CString filename )
{
	// 获取后缀名
	CString ext = "";
	int len = filename.GetLength();
	for ( int i = len-1; i >= 0; i--)
	{
		if (filename[i] == '.')
		{
			ext = filename.Mid(i+1);
			break;
		}
	}
	ext.MakeLower();
	if (ext == "")
		return;
	// 由后缀名判断存取格式类型
	int type = 0;
	if (ext == "bmp")					type = CXIMAGE_FORMAT_BMP;
#if CXIMAGE_SUPPORT_JPG
	else if (ext=="jpg"||ext=="jpeg")	type = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_PNG
	else if (ext == "png")				type = CXIMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_TIF
	else if (ext=="tiff"||ext=="tif")	type = CXIMAGE_FORMAT_TIF;
#endif
#if CXIMAGE_SUPPORT_TGA
	else if (ext=="tga")				type = CXIMAGE_FORMAT_TGA;
#endif
#if CXIMAGE_SUPPORT_PCX
	else if (ext=="pcx")				type = CXIMAGE_SUPPORT_PCX;
#endif
	else
		return;
	
	///////////////////////////////////////////////////////
	// 第一步画出曲线图
	CRect client, rc;
	CClientDC dc( this );
	GetClientRect( &client );
	GetDlgItem( IDC_STATIC_Logging_Graph1 )->GetClientRect( (LPRECT)rc );
	client.right = rc.Width() +40;
	client.bottom += 40;
	//获取图像宽度和高度
	int nWidth = client.Width();
	int nHeight = 0;
	int nCounts = 0;
	int i=0; 
	for(i=0; i<GraphDataBuffGroups; i++ )
		if( CControlLayer::Instance().m_DL.graph[i].bGraph )
			nCounts++;
	if( nCounts > 0 )
		nHeight = client.Height() * nCounts / GraphDataBuffGroups;
	else
		return;
	//创建与屏幕窗口兼容的内存设备环境
	CDC bmpDC;
	bmpDC.CreateCompatibleDC( &dc );
	//创建用于在内存设备环境中绘图的位图，并将其选进内存设备环境
	CBitmap& bitmap = *(new CBitmap);
	bitmap.CreateCompatibleBitmap( &dc, nWidth,	nHeight );
	CBitmap* poldBmp = bmpDC.SelectObject( &bitmap );
	//将目的区域以白色填充
	bmpDC.FillRect( &CRect(0, 0, nWidth, nHeight), &CBrush(RGB(255,255,255)) );
	//设置设备环境窗口原始点
	CPoint point = bmpDC.SetWindowOrg( 0, 0 );
	//画图
	CString str;
	str.Format( "nTotal = %d, Interval = %.2fs", CControlLayer::Instance().m_DL.other.lTimes, CControlLayer::Instance().m_DL.other.nElapseLogging / 1000.0 );
	bmpDC.TextOut( 10, 0, str );
	for( i=0; i<GraphDataBuffGroups; i++ )
		PaintGraph( &bmpDC, i, TRUE );
	//恢复设备环境窗口原有的原始点
	bmpDC.SetWindowOrg( point );
	Invalidate( FALSE );
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// 第二步生产bmp图像
	CxImage *newima = new CxImage();
	newima->CreateFromHBITMAP( (HBITMAP)bitmap );
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// 第三步存为文件
	bool retval = newima->Save(filename, type);
	///////////////////////////////////////////////////////
	bmpDC.SelectObject( poldBmp );
	delete &bitmap;
	delete newima;
}
