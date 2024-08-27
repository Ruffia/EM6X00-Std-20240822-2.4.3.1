// DlgDiagLogDiffGraph.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgDiagLogDiffGraph.h"
#include "CommonFunctionManager.h"
#include "CFGFileManager.h"
#include "ControlLayer.h"
#include "GraphBuffManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const double g_dDiffGraphRangeX[] = {1024, 512, 256, 128, 64, 32, 16};
const int	g_dDiffGraphRangeMaxIndexX = sizeof(g_dDiffGraphRangeX) / sizeof(double) - 1;

const double g_dDiffGraphRangeY[] = {10, 5, 2, 1, 0.5, 0.2, 0.1, 0.05, 0.02, 0.01, 0.005, 0.002, 0.001, 0.0005, 0.0002, 0.0001, 0.00005, 0.00002, 0.00001, 0.000005, 0.000002, 0.000001};
const int	g_dDiffGraphRangeMaxIndexY = sizeof(g_dDiffGraphRangeY) / sizeof(double) - 1;

/////////////////////////////////////////////////////////////////////////////
// CDlgDiagLogDiffGraph dialog


CDlgDiagLogDiffGraph::CDlgDiagLogDiffGraph(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDiagLogDiffGraph::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDiagLogDiffGraph)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nTotal		= -1;
	m_nSemType		= 0;
	m_nGraphType	= 0;
	m_dFactors[0]	= 0.0;
	m_dFactors[1]	= 0.0;
	m_dFactors[2]	= 0.0;
	m_pdWD_Obj		= NULL;
	m_pdObj_MagMin	= NULL;
}


void CDlgDiagLogDiffGraph::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDiagLogDiffGraph)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDiagLogDiffGraph, CDialog)
	//{{AFX_MSG_MAP(CDlgDiagLogDiffGraph)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDiagLogDiffGraph message handlers

BOOL CDlgDiagLogDiffGraph::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
/*	GetDlgItem( IDC_EDIT_Graph1_Min )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph1_Max )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph2_Min )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph2_Max )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph3_Min )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph3_Max )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph4_Min )->SetWindowText( "0.000000" );
	GetDlgItem( IDC_EDIT_Graph4_Max )->SetWindowText( "0.000000" );
*/
	for(int i=0; i<GraphDataBuffGroups; i++ )
	{
		CControlLayer::Instance().m_DL.dgraph[i].nZoomXIndex = 3;	// 横向显示128个数据
		CControlLayer::Instance().m_DL.dgraph[i].nZoomYIndex = -1;
		
		CControlLayer::Instance().m_DL.dgraph[i].dXMax = g_dDiffGraphRangeX[3];
		CControlLayer::Instance().m_DL.dgraph[i].dXMin = 0;
		
		CControlLayer::Instance().m_DL.dgraph[i].dYMin = 0.0;
		CControlLayer::Instance().m_DL.dgraph[i].dYMax = 0.0;
	}
/*	m_scroll1.SetScrollRange( 0, (int)(CControlLayer::Instance().m_DL.graph[0].dXMax) );
	m_scroll1.ShowWindow( TRUE );
	m_scroll2.SetScrollRange( 0, (int)(CControlLayer::Instance().m_DL.graph[1].dXMax) );
	m_scroll2.ShowWindow( TRUE );
	m_scroll3.SetScrollRange( 0, (int)(CControlLayer::Instance().m_DL.graph[2].dXMax) );
	m_scroll3.ShowWindow( TRUE );
	m_scroll4.SetScrollRange( 0, (int)(CControlLayer::Instance().m_DL.graph[3].dXMax) );
	m_scroll4.ShowWindow( TRUE );
*/
	for(int i=0; i<8; i++ )
		m_pdChnValue[i] = NULL;

	if( m_nGraphType == 1 )
		SetWindowText( "WD --- Obj coadjust" );
	else if( m_nGraphType == 2 )
		SetWindowText( "WD --- Mag Min" );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDiagLogDiffGraph::OnDestroy() 
{
	for( int i=0; i<8; i++ )
	{
		if ( m_pdChnValue[i] != NULL )
		{
			delete [] m_pdChnValue[i];
			m_pdChnValue[i]	= NULL;
		}
	}
	if( m_pdWD_Obj != NULL )
	{
		delete [] m_pdWD_Obj;
		m_pdWD_Obj = NULL;
	}
	if( m_pdObj_MagMin != NULL )
	{
		delete [] m_pdObj_MagMin;
		m_pdObj_MagMin = NULL;
	}
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CDlgDiagLogDiffGraph::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	if( m_nGraphType == 0 )
	{
		// 0:日志记录曲线
		if( CControlLayer::Instance().m_DL.log.bLog[0] )
			PaintGraph( &dc, 0 );
		if( CControlLayer::Instance().m_DL.log.bLog[2] )
			PaintGraph( &dc, 1 );
		if( CControlLayer::Instance().m_DL.log.bLog[4] )
			PaintGraph( &dc, 2 );
		if( CControlLayer::Instance().m_DL.log.bLog[6] )
			PaintGraph( &dc, 3 );
	}
	else
	{
		// 1/2:统调数据曲线
		PaintGraph2( &dc, m_nGraphType );
	}

	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgDiagLogDiffGraph::InitGraphData()
{
/*	CString str;
	GetDlgItem( IDC_EDIT_Graph1_Min )->GetWindowText( str );
	CControlLayer::Instance().m_DL.dgraph[0].dYMin = atof( str );
	GetDlgItem( IDC_EDIT_Graph1_Max )->GetWindowText( str );
	CControlLayer::Instance().m_DL.dgraph[0].dYMax = atof( str );
	GetDlgItem( IDC_EDIT_Graph2_Min )->GetWindowText( str );
	CControlLayer::Instance().m_DL.dgraph[1].dYMin = atof( str );
	GetDlgItem( IDC_EDIT_Graph2_Max )->GetWindowText( str );
	CControlLayer::Instance().m_DL.dgraph[1].dYMax = atof( str );
	GetDlgItem( IDC_EDIT_Graph3_Min )->GetWindowText( str );
	CControlLayer::Instance().m_DL.dgraph[2].dYMin = atof( str );
	GetDlgItem( IDC_EDIT_Graph3_Max )->GetWindowText( str );
	CControlLayer::Instance().m_DL.dgraph[2].dYMax = atof( str );
	GetDlgItem( IDC_EDIT_Graph4_Min )->GetWindowText( str );
	CControlLayer::Instance().m_DL.dgraph[3].dYMin = atof( str );
	GetDlgItem( IDC_EDIT_Graph4_Max )->GetWindowText( str );
	CControlLayer::Instance().m_DL.dgraph[3].dYMax = atof( str );
	
	for( int i=0; i<GraphDataBuffGroups; i++ )
		CControlLayer::Instance().m_DL.dgraph[i].dXMin = 0.0;
*/	
	for(int i=0; i<GraphDataBuffGroups; i++ )
	{
		CControlLayer::Instance().m_DL.dgraph[i].dXMin = 0.0;
		CControlLayer::Instance().m_DL.dgraph[i].dYMin = 0.0;
		CControlLayer::Instance().m_DL.dgraph[i].dYMax = 0.0;
	}
	m_nIntv = CControlLayer::Instance().m_DL.other.nElapseLogging;
	for(int i=0; i<8; i++ )
	{
		m_pdChnValue[i] = new double[8];
		ZeroMemory( m_pdChnValue[i], sizeof(double)*8 );
	}
}

void CDlgDiagLogDiffGraph::UpdateGraphData( int nTotal )
{
	if( nTotal == 0 )
		return;
	m_nTotal = nTotal;
/*
	m_scroll1.SetScrollRange(0, m_nTotal);
//	m_scroll1.SetScrollPos(m_nTotal);
	m_scroll2.SetScrollRange(0, m_nTotal);
	m_scroll3.SetScrollRange(0, m_nTotal);
	m_scroll4.SetScrollRange(0, m_nTotal);
*/
	for( int i=0; i<GraphDataBuffGroups; i++ )
	{
		if( CControlLayer::Instance().m_DL.dgraph[i].nZoomXIndex > -1 )
		{
			CControlLayer::Instance().m_DL.dgraph[i].dXMax = m_nTotal;
			CControlLayer::Instance().m_DL.dgraph[i].dXMin = m_nTotal - g_dDiffGraphRangeX[CControlLayer::Instance().m_DL.dgraph[i].nZoomXIndex];
		}
	}
/*	CString str;
	str.Format( "%d", nTotal +1);
	GetDlgItem( IDC_EDIT_Graph_TotalCounts )->SetWindowText( str );
*/	Invalidate( FALSE );
}

void CDlgDiagLogDiffGraph::PaintGraph( CDC* pDC, int nIndex, BOOL bErase )
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

	if( m_nTotal < 2 )
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
	if( CControlLayer::Instance().m_DL.dgraph[nIndex].nZoomXIndex > -1 )	// 有设置值
	{
		xMin = (int)(CControlLayer::Instance().m_DL.dgraph[nIndex].dXMin);
		xMax = (int)(CControlLayer::Instance().m_DL.dgraph[nIndex].dXMax);
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
	if( (CControlLayer::Instance().m_DL.dgraph[nIndex].nZoomYIndex > -1)
		|| ((CControlLayer::Instance().m_DL.dgraph[nIndex].nZoomYIndex == -1)
			&& ( (CControlLayer::Instance().m_DL.dgraph[nIndex].dYMin != 0.0)
				|| (CControlLayer::Instance().m_DL.dgraph[nIndex].dYMax != 0.0) ) ) )	// 如果有设置值
	{
		dYMin = CControlLayer::Instance().m_DL.dgraph[nIndex].dYMin;
		dYMax = CControlLayer::Instance().m_DL.dgraph[nIndex].dYMax;
	}
	else
	{
		dYMin = (CGraphBuffMgr::Instance().m_pDiffGraphDataBuff[nIndex])[xMin];
		dYMax = (CGraphBuffMgr::Instance().m_pDiffGraphDataBuff[nIndex])[xMin];
		for( i=xMin+1; i<xMax; i++ )
		{
			if( CGraphBuffMgr::Instance().m_pDiffGraphDataBuff[nIndex][i] < dYMin
				&& CGraphBuffMgr::Instance().m_pDiffGraphDataBuff[nIndex][i] != 0.0 )
				dYMin = CGraphBuffMgr::Instance().m_pDiffGraphDataBuff[nIndex][i];
			if( CGraphBuffMgr::Instance().m_pDiffGraphDataBuff[nIndex][i] > dYMax
				&& CGraphBuffMgr::Instance().m_pDiffGraphDataBuff[nIndex][i] > dYMax != 0.0 )
				dYMax = CGraphBuffMgr::Instance().m_pDiffGraphDataBuff[nIndex][i];
		}
		for( i=xMin+1; i<xMax; i++ )
		{
			if( m_pdChnValue[2*nIndex][i] < dYMin
				&& m_pdChnValue[2*nIndex][i] != 0.0 )
				dYMin = m_pdChnValue[2*nIndex][i];
			if( m_pdChnValue[2*nIndex][i] > dYMax
				&& m_pdChnValue[2*nIndex][i] > dYMax != 0.0 )
				dYMax = m_pdChnValue[2*nIndex][i];
		}
		for( i=xMin+1; i<xMax; i++ )
		{
			if( m_pdChnValue[2*nIndex+1][i] < dYMin
				&& m_pdChnValue[2*nIndex+1][i] != 0.0 )
				dYMin = m_pdChnValue[2*nIndex+1][i];
			if( m_pdChnValue[2*nIndex+1][i] > dYMax
				&& m_pdChnValue[2*nIndex+1][i] > dYMax != 0.0 )
				dYMax = m_pdChnValue[2*nIndex+1][i];
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
	y = (int)(rc1.top + ( nHeight - (CGraphBuffMgr::Instance().m_pDiffGraphDataBuff[nIndex][xMin] -dYMin) / (dYMax-dYMin) * nHeight ));
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
		if( CGraphBuffMgr::Instance().m_pDiffGraphDataBuff[nIndex][i] != 0.0 )
		{
			y = (int)(rc1.top + ( nHeight - (CGraphBuffMgr::Instance().m_pDiffGraphDataBuff[nIndex][i] -dYMin) / (dYMax-dYMin) * nHeight ));
			if( (y >= rc1.top) && (y <= rc1.bottom) )
			{
				x = (int)(rc1.left +(i-xMin)*dRatio);
				pDC->LineTo( x, y );
				pDC->Ellipse( CRect(x-2, y-2, x+2, y+2) );
			}
		}
	}
	pDC->SelectObject(poldpen);
	pen.DeleteObject();
	///////////////////////////////////
	switch( nIndex )
	{
	case	0:
		pen.CreatePen( PS_DOT, 1, RGB(192,0,0));
		break;
	case	1:
		pen.CreatePen( PS_DOT, 1, RGB(0,160,0));
		break;
	case	2:
		pen.CreatePen( PS_DOT, 1, RGB(0,0,192));
		break;
	case	3:
		pen.CreatePen( PS_DOT, 1, RGB(192,0,192));
		break;
	}
	// 画笔移到起始点
	y = (int)(rc1.top + ( nHeight - (m_pdChnValue[2*nIndex][xMin] -dYMin) / (dYMax-dYMin) * nHeight ));
	if( y < rc1.top )
		y = rc1.top;
	if( y > rc1.bottom )
		y = rc1.bottom;
	pDC->MoveTo( rc1.left, y );
	// 画折线图
	poldpen = pDC->SelectObject( &pen );
	for( i=xMin+1; i<xMax; i++ )
	{
		if( m_pdChnValue[2*nIndex][i] != 0.0 )
		{
			y = (int)(rc1.top + ( nHeight - (m_pdChnValue[2*nIndex][i] -dYMin) / (dYMax-dYMin) * nHeight ));
			if( (y >= rc1.top) && (y <= rc1.bottom) )
			{
				x = (int)(rc1.left +(i-xMin)*dRatio);
				pDC->LineTo( x, y );
				pDC->Ellipse( CRect(x-2, y-2, x+2, y+2) );
			}
		}
	}
	pDC->SelectObject(poldpen);
	pen.DeleteObject();
	///////////////////////////////////
	switch( nIndex )
	{
	case	0:
		pen.CreatePen( PS_DOT, 1, RGB(160,0,0));
		break;
	case	1:
		pen.CreatePen( PS_DOT, 1, RGB(0,192,0));
		break;
	case	2:
		pen.CreatePen( PS_DOT, 1, RGB(0,0,160));
		break;
	case	3:
		pen.CreatePen( PS_DOT, 1, RGB(160,0,160));
		break;
	}
	// 画笔移到起始点
	y = (int)(rc1.top + ( nHeight - (m_pdChnValue[2*nIndex+1][xMin] -dYMin) / (dYMax-dYMin) * nHeight ));
	if( y < rc1.top )
		y = rc1.top;
	if( y > rc1.bottom )
		y = rc1.bottom;
	pDC->MoveTo( rc1.left, y );
	// 画折线图
	poldpen = pDC->SelectObject( &pen );
	for( i=xMin+1; i<xMax; i++ )
	{
		if( m_pdChnValue[2*nIndex+1][i] != 0.0 )
		{
			y = (int)(rc1.top + ( nHeight - (m_pdChnValue[2*nIndex+1][i] -dYMin) / (dYMax-dYMin) * nHeight ));
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
	double dValue;	// CBoardComm::Instance().GetPhysicsFromAdcs(CControlLayer::Instance().m_DL.dgraph[nIndex].nIndex, dYMin);
	if( nIndex < 2 )
		dValue = dYMin /4;
	else
		dValue = dYMin /10;
	str.Format( "%.6f (%.6f)", dYMin, dValue );
	pDC->TextOut( rc1.left -8, rc1.bottom -sz.cy/2, str );
	// 标注纵向最大值
//	dValue = CBoardComm::Instance().GetPhysicsFromAdcs(CControlLayer::Instance().m_DL.dgraph[nIndex].nIndex, dYMax);
	if( nIndex < 2 )
		dValue = dYMax /4;
	else
		dValue = dYMax /10;
	str.Format( "%.6f (%.6f)", dYMax, dValue );
	pDC->TextOut( rc1.left -8, rc1.top -sz.cy/2, str );
	// 画标签
//	str = CControlLayer::Instance().m_DL.adc.chn[CControlLayer::Instance().m_DL.dgraph[nIndex].nIndex].strLabel;
	switch( nIndex )
	{
	case	0:
		str = "I Scan X";
		break;
	case	1:
		str = "I Scan Y";
		break;
	case	2:
		str = "I Stig X";
		break;
	case	3:
		str = "I Stig Y";
		break;
	}
	sz = pDC->GetTextExtent( str );
	pDC->TextOut( rc.left + rc.Width()/2 - sz.cx/2, rc.top +11, str );
	pDC->SetBkMode(noldBKmode);

	pen.DeleteObject();
	penGrid.DeleteObject();
}

void CDlgDiagLogDiffGraph::SaveGraph( CString filename )
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
	for(int i=0; i<GraphDataBuffGroups; i++ )
		if( CControlLayer::Instance().m_DL.dgraph[i].bGraph )
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
	for(int i=0; i<GraphDataBuffGroups; i++ )
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

void CDlgDiagLogDiffGraph::UpdateGraphData()
{
	// 为画统调数据曲线做准备
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	int i = 0;
	double* pdValue = new double[3];
	if( m_nGraphType < 2 )
	{
		// 统调数据曲线1：6200为直线方程；6900为WD和Obj的曲线
		if( m_nSemType < 1 )
		{
			// 6200
			if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Coadjust", "Equation", 2, 2, pdValue) == 2 )
			{
				m_dFactors[0] = pdValue[0];
				m_dFactors[1] = pdValue[1];
			}
			else
			{
				m_dFactors[0] = 0.0;
				m_dFactors[1] = 0.0;
			}
			m_pdWD_Obj = new double[28*10+1];	// WD: 2.0 - 28.0 mm
			m_nTotal = 281;
			// 6200: WD = C0 * C1 / M / 10 -12  ==>>  M = C0*C1 / (WD +12) /10
			// M = 4 * K * SQRT( N / HV ) - CNT   ==>>  K = (M + CNT) / (4 * SQRT(N/HV))
			// K = Ka * (Obj*32 +32768) + Kb   ==>>   Obj = ((K - Kb) / Ka -32768) / 32
			double dWD, dM, dK;
			for( i=0; i<m_nTotal; i++ )
			{
				dWD = i *0.1;
				dM = Co_6200_C0 * Co_6200_C1 / (dWD +12) /10;
				dK = (dM + Co_6200_CNT) / (4 *sqrt((double)Co_6200_N /300));
				m_pdWD_Obj[i] = ((dK -m_dFactors[1]) / m_dFactors[0] -32768 ) /32.0;
			}
		}
		else if( m_nSemType > 11 )
		{
			// 6900
			if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Coadjust", "Factors", 2, 3, pdValue) == 3 )
			{
				m_dFactors[0] = pdValue[0];
				m_dFactors[1] = pdValue[1];
				m_dFactors[2] = pdValue[2];
			}
			else
			{
				m_dFactors[0] = 0.0;
				m_dFactors[1] = 0.0;
				m_dFactors[2] = 0.0;
			}
			m_pdWD_Obj = new double[53*10+1];	// WD: 1.0 - 53.0 mm
			m_nTotal = 531;
			// 6900
			// OLdisp3X = 1 / sqrt( a *WD *WD + b *WD + c)
			double dWD = 1.0;
			for( i=0; i<m_nTotal; i++ )
			{
				dWD = i*0.1 +1;
				m_pdWD_Obj[i] = 1.0 / sqrt( m_dFactors[0] *dWD *dWD + m_dFactors[1] *dWD + m_dFactors[2] );
			}
		}
	}
	else
	{
		// 统调数据曲线2：6200无；6900为Obj和Mag Min的曲线
		if( m_nSemType < 1 )
		{
			// 6200
			m_pdObj_MagMin = new double[28*10+1];	// 2.0 - 28.0 mm
			m_nTotal = 281;
			// 6200: WD = C0 * C1 / M / 10 -12  ==>>  M = C0*C1 / (WD +12) /10
			// MagMin = 10 * (M /S) / 2^8;
			double dWD, dM;
			for( i=0; i<m_nTotal; i++ )
			{
				dWD = i *0.1;
				dM = Co_6200_C0 * Co_6200_C1 / (dWD +12) /10;
				m_pdObj_MagMin[i] = dM /1000 / 256;
			}
		}
		else
		{
			// 6900
			if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Coadjust", "FactorsWDmin", 2, 2, pdValue) == 2 )
			{
				m_dFactors[0] = pdValue[0];
				m_dFactors[1] = pdValue[1];
			}
			else
			{
				m_dFactors[0] = 0.0;
				m_dFactors[1] = 0.0;
			}
			m_pdObj_MagMin = new double[53*10+1];	// 1.0 - 53.0 mm
			m_nTotal = 531;
			for( i=0; i<m_nTotal; i++ )
			{
				// 6900 & 6200: MagMin = A / (WD+B), WD的单位是mm
				// dfMagMin = m_dCoFactorA / (dWD +m_dCoFactorB);
				m_pdObj_MagMin[i] = m_dFactors[0] / (i*0.1 +m_dFactors[1]);
			}
		}
	}
	delete [] pdValue;
}

void CDlgDiagLogDiffGraph::PaintGraph2( CDC* pDC, int nIndex, BOOL bErase )
{
	// TODO: Add your message handler code here
	/////////////////////////////////////////////////////////////////
	// 无论画与不画，都将绘图区域清空
	CRect rc, rc1;
//	GetWindowRect( (LPRECT)rc );
//	ScreenToClient( (LPRECT)rc );
	GetClientRect( (LPRECT)rc );
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

	if( m_nGraphType < 2 && m_pdWD_Obj[0] < 0.000001 )
		return;

	// 设置变量
	double dYMin, dYMax, dRatio;
	int nHeight, xMin, xMax, x, y, nTotal, i;

	// 画图区域
	rc1 = rc;
	rc1.left	+= 20;
	rc1.right	-= 20;
	rc1.top		+= 20;
	rc1.bottom	-= 20;
	nHeight = rc1.Height();
	nTotal = 10;
	xMin = 0;
	xMax = m_nTotal;
	dRatio = 1.0 * rc1.Width() / xMax;
	if( m_nGraphType < 2 )
	{
		dYMin = m_pdWD_Obj[m_nTotal-1];
		dYMax = m_pdWD_Obj[0];
	}
	else
	{
		dYMin = m_pdObj_MagMin[m_nTotal-1];
		dYMax = m_pdObj_MagMin[0];
	}

	// 画笔初始化
	CPen pen, pen2, *poldpen;
	pen.CreatePen( PS_DOT, 1, RGB(255,0,0) );
	pen2.CreatePen( PS_SOLID, 2, RGB(0,0,0) );
	poldpen = pDC->SelectObject( &pen );
	CBrush brush, *pOldBrush;
	brush.CreateSolidBrush(RGB(0,0,0));
	pOldBrush = (CBrush*)pDC->SelectObject(&brush);
	int noldBKmode = pDC->SetBkMode(TRANSPARENT);
	CString str = "100";
	CSize sz = pDC->GetTextExtent( str );

	if(m_nGraphType < 2 )
	{
		// 画笔移到拟合曲线起始点
		y = (int)(rc1.top + ( nHeight - (m_pdWD_Obj[0] -dYMin) / (dYMax-dYMin) * nHeight ));
		if( y < rc1.top )
			y = rc1.top;
		if( y > rc1.bottom )
			y = rc1.bottom;
		pDC->MoveTo( rc1.left, y );
		// 画拟合曲线
		for( i=xMin+1; i<xMax; i++ )
		{
			y = (int)(rc1.top + ( nHeight - (m_pdWD_Obj[i] -dYMin) / (dYMax-dYMin) * nHeight ));
			if( (y >= rc1.top) && (y <= rc1.bottom) )
			{
				x = (int)(rc1.left +(i-xMin)*dRatio);
				pDC->LineTo( x, y );
			}
		}
		pDC->SelectObject(poldpen);
		// 画校准点
		poldpen = pDC->SelectObject( &pen2 );
		for( i=0; i<nTotal; i++ )
		{
			if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] && (CControlLayer::Instance().m_Cali.dCaliCoX[i] > 0) && (CControlLayer::Instance().m_Cali.dCaliCoWD[i] > 0) )
			{
				if( m_nSemType < 1 )
					y = (int)(rc1.top + ( nHeight - ((CControlLayer::Instance().m_Cali.dCaliCoX[i] -32768) /32.0 -dYMin) / (dYMax-dYMin) * nHeight ));
				else if( m_nSemType > 11 )
					y = (int)(rc1.top + ( nHeight - (CControlLayer::Instance().m_Cali.dCaliCoX[i] -dYMin) / (dYMax-dYMin) * nHeight ));
				if( (y >= rc1.top) && (y <= rc1.bottom) )
				{
					x = (int)(rc1.left +(CControlLayer::Instance().m_Cali.dCaliCoWD[i]*10-xMin)*dRatio);
//					pDC->MoveTo( x, y );
//					pDC->FillRect( CRect(x-3, y-3, x+3, y+3), &brush );
					pDC->MoveTo( x-5, y );
					pDC->LineTo( x+4, y );
					pDC->MoveTo( x, y-4 );
					pDC->LineTo( x, y+4 );
					if( m_nSemType < 1 )
						str.Format( "%d: WD=%.1f, Obj=%.2f", i+1, CControlLayer::Instance().m_Cali.dCaliCoWD[i], (CControlLayer::Instance().m_Cali.dCaliCoX[i] -32768)/32.0 );
					else
						str.Format( "%d: WD=%.1f, Obj=%.2f", i+1, CControlLayer::Instance().m_Cali.dCaliCoWD[i], CControlLayer::Instance().m_Cali.dCaliCoX[i] );
					pDC->TextOut( x+8, y-sz.cy/2, str );
				}
			}
		}
	}
	else
	{
		// 画笔移到拟合曲线起始点
		y = (int)(rc1.top + ( nHeight - (m_pdObj_MagMin[0] -dYMin) / (dYMax-dYMin) * nHeight ));
		if( y < rc1.top )
			y = rc1.top;
		if( y > rc1.bottom )
			y = rc1.bottom;
		pDC->MoveTo( rc1.left, y );
		// 画拟合曲线
		for( i=xMin+1; i<xMax; i++ )
		{
			y = (int)(rc1.top + ( nHeight - (m_pdObj_MagMin[i] -dYMin) / (dYMax-dYMin) * nHeight ));
			if( (y >= rc1.top) && (y <= rc1.bottom) )
			{
				x = (int)(rc1.left +(i-xMin)*dRatio);
				pDC->LineTo( x, y );
			}
		}
		pDC->SelectObject(poldpen);
		if( m_nSemType > 11 )
		{
			// 画校准点
			poldpen = pDC->SelectObject( &pen2 );
			for( i=0; i<nTotal; i++ )
			{
				if( CControlLayer::Instance().m_Cali.bCaliCoSel[i] && (CControlLayer::Instance().m_Cali.dCaliCoY[i] > 0) && (CControlLayer::Instance().m_Cali.dCaliCoWD[i] > 0) )
				{
					y = (int)(rc1.top + ( nHeight - (CControlLayer::Instance().m_Cali.dCaliCoY[i] -dYMin) / (dYMax-dYMin) * nHeight ));
					if( (y >= rc1.top) && (y <= rc1.bottom) )
					{
						x = (int)(rc1.left +(CControlLayer::Instance().m_Cali.dCaliCoWD[i]*10-xMin)*dRatio);
//						pDC->MoveTo( x, y );
//						pDC->FillRect( CRect(x-3, y-3, x+3, y+3), &brush );
						pDC->MoveTo( x-5, y );
						pDC->LineTo( x+4, y );
						pDC->MoveTo( x, y-4 );
						pDC->LineTo( x, y+4 );
						str.Format( "%d: WD=%.1f, MagMin=%.1f", i+1, CControlLayer::Instance().m_Cali.dCaliCoWD[i], CControlLayer::Instance().m_Cali.dCaliCoY[i] );
						pDC->TextOut( x+8, y-sz.cy/2, str );
					}
				}
			}
		}
	}

	pDC->SelectObject(poldpen);
	pen.DeleteObject();
	pen2.DeleteObject();
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
	pDC->SetBkMode(noldBKmode);
}
