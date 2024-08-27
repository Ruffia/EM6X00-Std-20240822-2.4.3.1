// DlgAutoTestLNS.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgAutoTestLNS.h"
#include <math.h>
#include "ControlLayer.h"
#include "BoardComm.h"
#include "ScaleManager.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*
#define Timer_CL_MidToMax	1
#define Timer_CL_MaxToMid	2
#define Timer_CL_MidToMin	3
#define Timer_CL_MinToMid	4
#define Timer_OL_MidToMax	5
#define Timer_OL_MaxToMid	6
#define Timer_OL_MidToMin	7
#define Timer_OL_MinToMid	8
#define Timer_ALX_MidToMax	9
#define Timer_ALX_MaxToMid	10
#define Timer_ALX_MidToMin	11
#define Timer_ALX_MinToMid	12
#define Timer_ALY_MidToMax	13
#define Timer_ALY_MaxToMid	14
#define Timer_ALY_MidToMin	15
#define Timer_ALY_MinToMid	16
*/
#define	Timer_CL_seg11		1
#define Timer_CL_seg12		2
#define Timer_CL_seg13		3
#define Timer_CL_seg14		4
#define Timer_CL_seg15		5
#define Timer_CL_seg16		6

#define	Timer_CL_seg21		7
#define Timer_CL_seg22		8
#define Timer_CL_seg23		9
#define Timer_CL_seg24		10
#define Timer_CL_seg25		11
#define Timer_CL_seg26		12

#define	Timer_OL_seg11		21
#define Timer_OL_seg12		22
#define Timer_OL_seg13		23
#define Timer_OL_seg14		24
#define Timer_OL_seg15		25
#define Timer_OL_seg16		26

#define	Timer_OL_seg21		27
#define Timer_OL_seg22		28
#define Timer_OL_seg23		29
#define Timer_OL_seg24		30
#define Timer_OL_seg25		31
#define Timer_OL_seg26		32

#define Timer_ALX_seg11		41
#define Timer_ALX_seg12		42
#define Timer_ALX_seg13		43
#define Timer_ALX_seg14		44
#define Timer_ALX_seg15		45
#define Timer_ALX_seg16		46

#define Timer_ALX_seg21		47
#define Timer_ALX_seg22		48
#define Timer_ALX_seg23		49
#define Timer_ALX_seg24		50
#define Timer_ALX_seg25		51
#define Timer_ALX_seg26		52

#define Timer_ALY_seg11		61
#define Timer_ALY_seg12		62
#define Timer_ALY_seg13		63
#define Timer_ALY_seg14		64
#define Timer_ALY_seg15		65
#define Timer_ALY_seg16		66

#define Timer_ALY_seg21		67
#define Timer_ALY_seg22		68
#define Timer_ALY_seg23		69
#define Timer_ALY_seg24		70
#define Timer_ALY_seg25		71
#define Timer_ALY_seg26		72

#define Elapse_AutoTest_LNS	1000

int g_nAutoTestLNS_seg1[] = { 0x8001, 0x8002, 0x8004, 0x8008, 0x8010, 0x8020 };
int g_nAutoTestLNS_seg2[] = { 0x8020, 0x8040, 0x8080, 0x8100, 0x8200, 0x8400 };
int g_nAutoTestLNS_seg3[] = { 0x8400, 0x8800, 0x9000, 0xA000, 0xC000, 0xFFFF };

int g_nAutoTestLNS_seg4[] = { 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400 };
int g_nAutoTestLNS_seg5[] = { 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020 };
int g_nAutoTestLNS_seg6[] = { 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001 };

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestLNS dialog


CDlgAutoTestLNS::CDlgAutoTestLNS(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoTestLNS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoTestLNS)
	//}}AFX_DATA_INIT
}


void CDlgAutoTestLNS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoTestLNS)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutoTestLNS, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoTestLNS)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start, OnStart)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_11, OnStartCL)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_21, OnStartOL)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_31, OnStartALX)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_41, OnStartALY)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_SaveGraph, OnSaveGraph)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_12, OnStartCL2)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_22, OnStartOL2)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_32, OnStartALX2)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_42, OnStartALY2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestLNS message handlers

void CDlgAutoTestLNS::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	for( int i=0; i<3; i++ )
		PaintGraph( &dc, i );

	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgAutoTestLNS::PaintGraph( CDC* pDC, int nIndex, BOOL bErase )
{
	/////////////////////////////////////////////////////////////////
	// 无论画与不画，都将绘图区域清空
	CRect rc, rc1;
	GetDlgItem( IDC_STATIC_AT_Cali_Graph1 +nIndex)->GetWindowRect( (LPRECT)rc );
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

	if( CControlLayer::Instance().m_AT.nAutoTestLNS_Total[nIndex] < 2 )
		return;

	/////////////////////////////////////////////////////////////////
	// 设置变量
	double dYMin, dYMax, dRatio;//, dYMinLog, dYMaxLog, dY;
	int nHeight, xMin, xMax, x, y, nTotal, i, xIntMain, xInt;//, yLog;
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
	nTotal = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[nIndex];

	// X向绘图比例
	xMin = 0;
	xMax = nTotal;

	// 数据个数
	nTotal = xMax - xMin;
	dRatio = 1.0 * rc1.Width() / (nTotal-1);

	// 纵坐标最大值、最小值
	// 取以2为底的对数做纵坐标
	// log2(y) = ln(y) / ln(2)
	// C语言中log(x)表示以e为底的自然对数，log10(x)表示以10为底的常用对数
	{
		dYMin = (CControlLayer::Instance().m_AT.pAutoTestLNS[nIndex])[xMin];
		for( i=xMin+1; i<xMax; i++ )
		{
			if( CControlLayer::Instance().m_AT.pAutoTestLNS[nIndex][i] < dYMin )
				dYMin = CControlLayer::Instance().m_AT.pAutoTestLNS[nIndex][i];
		}
//		dYMinLog = log(dYMin) / log(2);
	}
	{
		dYMax = (CControlLayer::Instance().m_AT.pAutoTestLNS[nIndex])[xMin];
		for( i=xMin+1; i<xMax; i++ )
		{
			if( CControlLayer::Instance().m_AT.pAutoTestLNS[nIndex][i] > dYMax )
				dYMax = CControlLayer::Instance().m_AT.pAutoTestLNS[nIndex][i];
		}
//		dYMaxLog = log(dYMax) / log(2);
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
	{
		xIntMain	= 10;
		xInt		= 2;
	}
	for( i=xMin; i<xMax; i++ )
	{
		if( i % xIntMain == 0 )		// 主要网格
		{
			pDC->MoveTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.bottom +10 );
			pDC->LineTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.bottom );
			poldpen = pDC->SelectObject( &penGrid );
			pDC->LineTo( (int)(rc1.left +(i -xMin) *dRatio), rc1.top );
			pDC->SelectObject( poldpen );
			str.Format( "%d", i );
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
				str.Format( "%d", i );
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
	if( dYMax == dYMin )
	{
		y = rc1.bottom;
//		yLog = rc1.bottom;
	}
	else
	{
		y = (int)(rc1.top + ( nHeight - (CControlLayer::Instance().m_AT.pAutoTestLNS[nIndex][xMin] -dYMin) / (dYMax-dYMin) * nHeight ));
//		dY		= CControlLayer::Instance().m_AT.pAutoTestLNS[nIndex][xMin];
//		yLog	= (int)(rc1.top + ( nHeight - ( log(dY) / log(2) -dYMinLog) / (dYMaxLog-dYMinLog) * nHeight ));
	}
	if( y < rc1.top )
		y = rc1.top;
	if( y > rc1.bottom )
		y = rc1.bottom;
	pDC->MoveTo( rc1.left, y );
//	if( yLog < rc1.top )
//		yLog = rc1.top;
//	if( yLog > rc1.bottom )
//		yLog = rc1.bottom;
//	pDC->MoveTo( rc1.left, yLog );
	// 画折线图
	poldpen = pDC->SelectObject( &pen );
	for( i=xMin+1; i<xMax; i++ )
	{
		if( dYMax == dYMin )
		{
			y = rc1.bottom;
//			yLog = rc1.bottom;
		}
		else
		{
			y = (int)(rc1.top + ( nHeight - (CControlLayer::Instance().m_AT.pAutoTestLNS[nIndex][i] -dYMin) / (dYMax-dYMin) * nHeight ));
//			dY		= CControlLayer::Instance().m_AT.pAutoTestLNS[nIndex][i];
//			yLog	= (int)(rc1.top + ( nHeight - ( log(dY) / log(2) -dYMinLog) / (dYMaxLog-dYMinLog) * nHeight ));
		}
		if( (y >= rc1.top) && (y <= rc1.bottom) )
//		if( (yLog >= rc1.top) && (yLog <= rc1.bottom) )
		{
			x = (int)(rc1.left +(i-xMin)*dRatio);
			pDC->LineTo( x, y );
			pDC->Ellipse( CRect(x-2, y-2, x+2, y+2) );
//			pDC->LineTo( x, yLog );
//			pDC->Ellipse( CRect(x-2, yLog-2, x+2, yLog+2) );
		}
	}
	pDC->SelectObject(poldpen);
	/////////////////////////////////////////////////////////////////
	// 标注纵向最小值
	noldBKmode = pDC->SetBkMode(TRANSPARENT);
	double dValue = CBoardComm::Instance().GetPhysicsFromAdcs(1*16 +m_nChnIndex2, dYMin, CControlLayer::Instance().m_DL.log.strCalc);
	str.Format( "%.6f ( %.6f )", dYMin, dValue );
	pDC->TextOut( rc1.left -8, rc1.bottom -sz.cy/2, str );
	// 标注纵向最大值
	dValue = CBoardComm::Instance().GetPhysicsFromAdcs(1*16 +m_nChnIndex2, dYMax, CControlLayer::Instance().m_DL.log.strCalc);
	str.Format( "%.6f ( %.6f )", dYMax, dValue );
	pDC->TextOut( rc1.left -8, rc1.top -sz.cy/2, str );
	// 画标签
	int* pn;
	if( m_bReverse )
	{
		switch( nIndex )
		{
		case 0:
			pn = g_nAutoTestLNS_seg4;
			break;
		case 1:
			pn = g_nAutoTestLNS_seg5;
			break;
		case 2:
			pn = g_nAutoTestLNS_seg6;
			break;
		}
	}
	else
	{
		switch( nIndex )
		{
		case 0:
			pn = g_nAutoTestLNS_seg1;
			break;
		case 1:
			pn = g_nAutoTestLNS_seg2;
			break;
		case 2:
			pn = g_nAutoTestLNS_seg3;
			break;
		}
	}
	str.Format( "%s [%04X---%04X]", CControlLayer::Instance().m_DL.adc.chn[1*16 +m_nChnIndex2].strLabel, pn[0], pn[AutoTest_16bitDAC_StepsEach-1] );
	sz = pDC->GetTextExtent( str );
	pDC->TextOut( rc.left + rc.Width()/2 - sz.cx/2, rc.top +11, str );
	pDC->SetBkMode(noldBKmode);

	pen.DeleteObject();
	penGrid.DeleteObject();
}

void CDlgAutoTestLNS::OnSaveGraph() 
{
	// TODO: Add your control notification handler code here
	CString filename;
	char szFilter[] = "BMP file|*.bmp|JPG file|*.jpg|PNG file|*.png|TIF file|*.tif|PCX file|*.pcx|TGA file|*.tga||";
	CFileDialog dlgFile(FALSE, "BMP", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	if( dlgFile.DoModal() == IDOK )
		filename = dlgFile.GetPathName();
	else
		return;
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
	GetDlgItem( IDC_STATIC_AT_Cali_Graph1 )->GetClientRect( (LPRECT)rc );
	client.right = rc.Width() +40;
	client.bottom += 40;
	//获取图像宽度和高度
	int nWidth = client.Width();
	int nHeight = client.Height();
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
	str.Format( "AutoTest LNS" );
	bmpDC.TextOut( 10, 0, str );
	for(int i=0; i<3; i++ )
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

void CDlgAutoTestLNS::InitParams()
{
	for( int i=0; i<4; i++ )
	{
		CControlLayer::Instance().m_AT.pAutoTestLNS[i] = new double[AutoTest_16bitDAC_Steps*3];
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[i] = 0;
	}
	CControlLayer::Instance().m_DL.other.bDebug = CControlLayer::Instance().m_DL.other.bDebug;
	UpdateData( FALSE );
}

void CDlgAutoTestLNS::EnableParams( int nIndex, BOOL bEnable )
{
	if( bEnable )
	{
		int nTimerIndex = 0;
		VARIANT	var;

		/////////////////////////////////////////////////////////////////
		VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
		SerialNumber.lVal = -1;

		switch( nIndex )
		{
		case	0:
			// CondenserLens
			ClassID.lVal = SemControlPanelClass_CondenserLens;
			Flag.lVal	= 1;
			var.lVal	= CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[0];
			Xpos		= var;
			
			nTimerIndex = Timer_CL_seg11;
			break;
		case	1:
			// CondenserLens
			ClassID.lVal = SemControlPanelClass_CondenserLens;
			Flag.lVal	= 1;
			var.lVal	= CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[0];
			Xpos		= var;

			nTimerIndex = Timer_CL_seg21;
			break;
		case	2:
			// ObjectiveLens
			ClassID.lVal = SemControlPanelClass_ObjectiveLens;
			Flag.lVal	= 1;
			var.dblVal	= CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[1];
			var.lVal	= (long)(var.dblVal);
			Xpos		= var;
			
			nTimerIndex = Timer_OL_seg11;
			break;
		case	3:
			// ObjectiveLens
			ClassID.lVal = SemControlPanelClass_ObjectiveLens;
			Flag.lVal	= 1;
			var.dblVal	= CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[1];
			var.lVal	= (long)(var.dblVal);
			Xpos		= var;

			nTimerIndex = Timer_OL_seg21;
			break;
		case	4:
			// BeemAlignmentX
			ClassID.lVal = SemControlPanelClass_BeemAlignment;
			Flag.lVal	= 1;
			var.lVal	= CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[2];
			Xpos		= var;
			
			nTimerIndex = Timer_ALX_seg11;
			break;
		case	5:
			// BeemAlignmentX
			ClassID.lVal = SemControlPanelClass_BeemAlignment;
			Flag.lVal	= 1;
			var.lVal	= CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[2];
			Xpos		= var;

			nTimerIndex = Timer_ALX_seg21;
			break;
		case	6:
			// BeemAlignmentY
			ClassID.lVal = SemControlPanelClass_BeemAlignment;
			Flag.lVal	= 2;
			var.lVal	= CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[3];
			Ypos		= var;
			
			nTimerIndex = Timer_ALY_seg11;
			break;
		case	7:
			// BeemAlignmentY
			ClassID.lVal = SemControlPanelClass_BeemAlignment;
			Flag.lVal	= 2;
			var.lVal	= CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[3];
			Ypos		= var;

			nTimerIndex = Timer_ALY_seg21;
			break;
		}

		// 停止定时器
		for(int i=0; i<6; i++ )
			KillTimer( nTimerIndex +i);
		// 设置原始值
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag  );
		// 恢复按钮状态
		for(int i=0; i<8; i++ )
		{
			((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +i)))->EnableWindow( TRUE );
			((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +i)))->SetCheck( 0 );
		}
		// 关闭记录文件
		CloseHandle(CControlLayer::Instance().m_AT.hLogFile);
	}
	else
	{
		// 设置按钮状态，准备检测
		for(int i=0; i<8; i++ )
		{
			((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +i)))->SetCheck( 0 );
			((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +i)))->EnableWindow( FALSE );
		}
		((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +nIndex)))->SetCheck( 1 );
		((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +nIndex)))->EnableWindow( TRUE );
		// 准备记录文件
		char appfilepath[255];
		::GetModuleFileName( NULL, appfilepath, 255 );
		char *ptemp = strrchr( appfilepath, '\\');
		ptemp[1] = 0;
		
		CString str, strTemp, strParam, strFirst;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
		str.Format( "%s\\Log\\AutoTestLNS-%s.txt", appfilepath, strTemp );
		CControlLayer::Instance().m_AT.hLogFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	}
}

void CDlgAutoTestLNS::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent >= Timer_CL_seg11 && nIDEvent <=Timer_CL_seg16 )
		TimerChangeCL( nIDEvent );
	else if( nIDEvent >= Timer_OL_seg11 && nIDEvent <=Timer_OL_seg16 )
		TimerChangeOL( nIDEvent );
	else if( nIDEvent >= Timer_ALX_seg11 && nIDEvent <=Timer_ALX_seg16 )
		TimerChangeALX( nIDEvent );
	else if( nIDEvent >= Timer_ALY_seg11 && nIDEvent <=Timer_ALY_seg16 )
		TimerChangeALY( nIDEvent );
	else if( nIDEvent >= Timer_CL_seg21 && nIDEvent <=Timer_CL_seg26 )
		TimerChangeCLReverse( nIDEvent );
	else if( nIDEvent >= Timer_OL_seg21 && nIDEvent <=Timer_OL_seg26 )
		TimerChangeOLReverse( nIDEvent );
	else if( nIDEvent >= Timer_ALX_seg21 && nIDEvent <=Timer_ALX_seg26 )
		TimerChangeALXReverse( nIDEvent );
	else if( nIDEvent >= Timer_ALY_seg21 && nIDEvent <=Timer_ALY_seg26 )
		TimerChangeALYReverse( nIDEvent );
	CDialog::OnTimer(nIDEvent);
}

void CDlgAutoTestLNS::OnStart() 
{
	// TODO: Add your control notification handler code here
	int i, nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start)))->GetCheck();
	if( nCheck )
	{
		for( i=0; i<4; i++ )
		{
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[i] = 0;
			for( int j=0; j<AutoTest_16bitDAC_Steps; j++ )
				CControlLayer::Instance().m_AT.pAutoTestLNS[i][j] = 0;
		}
		VARIANT var = CSemCoreAssemble::Instance().m_CondenserLens.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[0] = var.lVal;
		var = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[1] = var.lVal;
		var = CSemCoreAssemble::Instance().m_BeemAlignmentX.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[2] = var.lVal;
		var = CSemCoreAssemble::Instance().m_BeemAlignmentY.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[3] = var.lVal;

		m_strRes	= "";
		m_bContinue = TRUE;

		CBoardComm::Instance().ChangedADCselectOB( 1 );
		CBoardComm::Instance().ChangedADCsChSelect( 0, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);
		
		m_nOrder	= 0;
		m_nChnIndex1	= Diag_CL;
		
		int nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];	// 设置起始值从MidRange开始，从中值到上半段最大值
		CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		SetTimer(Timer_CL_seg11, Elapse_AutoTest_LNS, NULL);

		for( i=1; i<5; i++ )
			((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start +i)))->EnableWindow( FALSE );
	}
	else
	{
		for( i=0; i<4; i++ )
			EnableParams( i, TRUE );
	}
}

void CDlgAutoTestLNS::OnStartCL() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 1 );
		CBoardComm::Instance().ChangedADCsChSelect( 0, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_CL;
		m_nChnIndex2	= 0;
		m_strRes		= "";
		int nMax = AutoTest_16bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_CondenserLens.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[0] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= FALSE;

		int nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];	// 设置起始值
		m_nOrder++;
		CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		SetTimer(Timer_CL_seg11, Elapse_AutoTest_LNS, NULL);

		EnableParams( 0, FALSE );
	}
	else
	{
		EnableParams( 0, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestLNS::OnStartOL() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_21)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 1 );
		CBoardComm::Instance().ChangedADCsChSelect( 1, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);
		
		m_nOrder		= 0;
		m_nChnIndex1	= Diag_OL;
		m_nChnIndex2	= 1;
		m_strRes		= "";
		int nMax = AutoTest_16bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[1] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= FALSE;
		
		int nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];	// 设置起始值
		m_nOrder++;
		CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
		SetTimer(Timer_OL_seg11, Elapse_AutoTest_LNS, NULL);

		EnableParams( 2, FALSE );
	}
	else
	{
		EnableParams( 2, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestLNS::OnStartALX() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_31)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 1 );
		CBoardComm::Instance().ChangedADCsChSelect( 2, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);
		
		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Align_X;
		m_nChnIndex2	= 2;
		m_strRes		= "";
		int nMax = AutoTest_16bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_BeemAlignmentX.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[2] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= FALSE;
		
		int nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];	// 设置起始值
		m_nOrder++;
		CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
		SetTimer(Timer_ALX_seg11, Elapse_AutoTest_LNS, NULL);

		EnableParams( 4, FALSE );
	}
	else
	{
		EnableParams( 4, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestLNS::OnStartALY() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_41)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 1 );
		CBoardComm::Instance().ChangedADCsChSelect( 3 , CControlLayer::Instance().m_DL.adc.chn );
		CBoardComm::Instance().ReadSelectedADC(FALSE);
		
		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Align_Y;
		m_nChnIndex2	= 3;
		m_strRes		= "";
		int nMax = AutoTest_16bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_BeemAlignmentY.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[3] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= FALSE;
		
		int nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];	// 设置起始值
		m_nOrder++;
		CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
		SetTimer(Timer_ALY_seg11, Elapse_AutoTest_LNS, NULL);

		EnableParams( 6, FALSE );
	}
	else
	{
		EnableParams( 6, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestLNS::OnStartCL2() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_12)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 1 );
		CBoardComm::Instance().ChangedADCsChSelect( 0, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_CL;
		m_nChnIndex2	= 0;
		m_strRes		= "";
		int nMax = AutoTest_16bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_CondenserLens.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[0] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= TRUE;

		int nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];	// 设置起始值
		m_nOrder++;
		CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		SetTimer(Timer_CL_seg21, Elapse_AutoTest_LNS, NULL);

		EnableParams( 1, FALSE );
	}
	else
	{
		EnableParams( 1, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestLNS::OnStartOL2() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_22)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 1 );
		CBoardComm::Instance().ChangedADCsChSelect( 1, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_OL;
		m_nChnIndex2	= 1;
		m_strRes		= "";
		int nMax = AutoTest_16bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_ObjectiveLens.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[1] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= TRUE;

		int nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];	// 设置起始值
		m_nOrder++;
		CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
		SetTimer(Timer_OL_seg21, Elapse_AutoTest_LNS, NULL);

		EnableParams( 3, FALSE );
	}
	else
	{
		EnableParams( 3, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestLNS::OnStartALX2() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_32)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 1 );
		CBoardComm::Instance().ChangedADCsChSelect( 2 , CControlLayer::Instance().m_DL.adc.chn );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Align_X;
		m_nChnIndex2	= 2;
		m_strRes		= "";
		int nMax = AutoTest_16bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_BeemAlignmentX.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[2] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= TRUE;

		int nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];	// 设置起始值
		m_nOrder++;
		CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
		SetTimer(Timer_ALX_seg21, Elapse_AutoTest_LNS, NULL);

		EnableParams( 5, FALSE );
	}
	else
	{
		EnableParams( 5, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestLNS::OnStartALY2() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_42)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 1 );
		CBoardComm::Instance().ChangedADCsChSelect( 3, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Align_Y;
		m_nChnIndex2	= 3;
		m_strRes		= "";
		int nMax = AutoTest_16bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_BeemAlignmentY.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestLNS_OldPos[3] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= TRUE;

		int nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];	// 设置起始值
		m_nOrder++;
		CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
		SetTimer(Timer_ALY_seg21, Elapse_AutoTest_LNS, NULL);

		EnableParams( 7, FALSE );
	}
	else
	{
		EnableParams( 7, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestLNS::TimerChangeCL( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_CL_seg11 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_CL_seg11);

			m_nOrder = sizeof(g_nAutoTestLNS_seg1) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
			SetTimer(Timer_CL_seg12, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_CL_seg12 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log  );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_CL_seg12);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
				SetTimer(Timer_CL_seg11, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
				SetTimer(Timer_CL_seg13, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_CL_seg13 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log  );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_CL_seg13);

			m_nOrder = sizeof(g_nAutoTestLNS_seg2) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
			SetTimer(Timer_CL_seg14, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_CL_seg14 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_CL_seg14);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
				SetTimer(Timer_CL_seg13, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
				SetTimer(Timer_CL_seg15, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_CL_seg15 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log  );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_CL_seg15);

			m_nOrder = sizeof(g_nAutoTestLNS_seg3) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
			SetTimer(Timer_CL_seg16, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_CL_seg16 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log  );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_CL_seg16);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
				SetTimer(Timer_CL_seg15, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				EnableParams( 0, TRUE );
				// 恢复真空状态机的工作
				if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
					CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		}
	}
}

void CDlgAutoTestLNS::TimerChangeOL( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_OL_seg11 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_OL_seg11);

			m_nOrder = sizeof(g_nAutoTestLNS_seg1) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
			SetTimer(Timer_OL_seg12, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_OL_seg12 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_OL_seg12);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
				SetTimer(Timer_OL_seg11, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
				SetTimer(Timer_OL_seg13, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_OL_seg13 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_OL_seg13);

			m_nOrder = sizeof(g_nAutoTestLNS_seg2) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
			SetTimer(Timer_OL_seg14, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_OL_seg14 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_OL_seg14);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
				SetTimer(Timer_OL_seg13, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
				SetTimer(Timer_OL_seg15, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_OL_seg15 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_OL_seg15);

			m_nOrder = sizeof(g_nAutoTestLNS_seg3) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
			SetTimer(Timer_OL_seg16, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_OL_seg16 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_OL_seg16);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
				SetTimer(Timer_OL_seg15, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				EnableParams( 2, TRUE );
				// 恢复真空状态机的工作
				if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
					CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
		}
	}
}

void CDlgAutoTestLNS::TimerChangeALX( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_ALX_seg11 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn,TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log);
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_ALX_seg11);

			m_nOrder = sizeof(g_nAutoTestLNS_seg1) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
			SetTimer(Timer_ALX_seg12, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALX_seg12 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ALX_seg12);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
				SetTimer(Timer_ALX_seg11, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
				SetTimer(Timer_ALX_seg13, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALX_seg13 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_ALX_seg13);

			m_nOrder = sizeof(g_nAutoTestLNS_seg2) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
			SetTimer(Timer_ALX_seg14, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALX_seg14 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue,CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue,CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue,CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ALX_seg14);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
				SetTimer(Timer_ALX_seg13, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
				SetTimer(Timer_ALX_seg15, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALX_seg15 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn ,TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_ALX_seg15);

			m_nOrder = sizeof(g_nAutoTestLNS_seg3) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
			SetTimer(Timer_ALX_seg16, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALX_seg16 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue,CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue ,CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ALX_seg16);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
				SetTimer(Timer_ALX_seg15, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				EnableParams( 4, TRUE );
				// 恢复真空状态机的工作
				CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
		}
	}
}

void CDlgAutoTestLNS::TimerChangeALY( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_ALY_seg11 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_ALY_seg11);

			m_nOrder = sizeof(g_nAutoTestLNS_seg1) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
			SetTimer(Timer_ALY_seg12, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALY_seg12 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ALY_seg12);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
				SetTimer(Timer_ALY_seg11, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
				SetTimer(Timer_ALY_seg13, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg1[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALY_seg13 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_ALY_seg13);

			m_nOrder = sizeof(g_nAutoTestLNS_seg2) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
			SetTimer(Timer_ALY_seg14, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALY_seg14 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ALY_seg14);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
				SetTimer(Timer_ALY_seg13, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
				SetTimer(Timer_ALY_seg15, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg2[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALY_seg15 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_ALY_seg15);

			m_nOrder = sizeof(g_nAutoTestLNS_seg3) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
			SetTimer(Timer_ALY_seg16, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALY_seg16 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ALY_seg16);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
				SetTimer(Timer_ALY_seg15, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				EnableParams( 6, TRUE );
				// 恢复真空状态机的工作
				if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
					CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg3[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
		}
	}
}

void CDlgAutoTestLNS::TimerChangeCLReverse( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_CL_seg21 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_CL_seg21);

			m_nOrder = sizeof(g_nAutoTestLNS_seg4) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
			SetTimer(Timer_CL_seg22, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_CL_seg22 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_CL_seg22);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
				SetTimer(Timer_CL_seg21, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
				SetTimer(Timer_CL_seg23, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_CL_seg23 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_CL_seg23);

			m_nOrder = sizeof(g_nAutoTestLNS_seg5) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
			SetTimer(Timer_CL_seg24, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_CL_seg24 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_CL_seg24);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
				SetTimer(Timer_CL_seg23, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
				SetTimer(Timer_CL_seg25, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_CL_seg25 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue , CControlLayer::Instance().m_DL.adc.chn);
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_CL_seg25);

			m_nOrder = sizeof(g_nAutoTestLNS_seg6) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
			SetTimer(Timer_CL_seg26, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_CL_seg26 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_CL_seg26);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
				SetTimer(Timer_CL_seg25, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				EnableParams( 1, TRUE );
				// 恢复真空状态机的工作
				if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
					CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L1DACSB_Change( nPos );
		}
	}
}

void CDlgAutoTestLNS::TimerChangeOLReverse( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_OL_seg21 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_OL_seg21);

			m_nOrder = sizeof(g_nAutoTestLNS_seg1) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
			SetTimer(Timer_OL_seg22, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_OL_seg22 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_OL_seg22);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
				SetTimer(Timer_OL_seg21, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
				SetTimer(Timer_OL_seg23, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_OL_seg23 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_OL_seg23);

			m_nOrder = sizeof(g_nAutoTestLNS_seg5) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
			SetTimer(Timer_OL_seg24, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_OL_seg24 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_OL_seg24);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
				SetTimer(Timer_OL_seg23, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
				SetTimer(Timer_OL_seg25, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_OL_seg25 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_OL_seg25);

			m_nOrder = sizeof(g_nAutoTestLNS_seg6) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
			SetTimer(Timer_OL_seg26, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_OL_seg26 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_OL_seg26);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
				SetTimer(Timer_OL_seg25, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				EnableParams( 3, TRUE );
				// 恢复真空状态机的工作
				if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
					CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_L2SDACSB_Change( nPos );
		}
	}
}

void CDlgAutoTestLNS::TimerChangeALXReverse( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_ALX_seg21 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_ALX_seg21);

			m_nOrder = sizeof(g_nAutoTestLNS_seg4) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
			SetTimer(Timer_ALX_seg22, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALX_seg22 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ALX_seg22);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
				SetTimer(Timer_ALX_seg21, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
				SetTimer(Timer_ALX_seg23, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALX_seg23 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_ALX_seg23);

			m_nOrder = sizeof(g_nAutoTestLNS_seg5) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
			SetTimer(Timer_ALX_seg24, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALX_seg24 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ALX_seg24);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
				SetTimer(Timer_ALX_seg23, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
				SetTimer(Timer_ALX_seg25, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALX_seg25 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_ALX_seg25);

			m_nOrder = sizeof(g_nAutoTestLNS_seg6) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
			SetTimer(Timer_ALX_seg26, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALX_seg26 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ALX_seg26);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
				SetTimer(Timer_ALX_seg25, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				EnableParams( 5, TRUE );
				// 恢复真空状态机的工作
				CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_XalDACSB_Change( nPos );
		}
	}
}

void CDlgAutoTestLNS::TimerChangeALYReverse( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_ALY_seg21 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_ALY_seg21);

			m_nOrder = sizeof(g_nAutoTestLNS_seg4) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
			SetTimer(Timer_ALY_seg22, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALY_seg22 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestLNS[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ALY_seg22);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
				SetTimer(Timer_ALY_seg21, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
				SetTimer(Timer_ALY_seg23, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg4[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALY_seg23 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_ALY_seg23);

			m_nOrder = sizeof(g_nAutoTestLNS_seg5) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
			SetTimer(Timer_ALY_seg24, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALY_seg24 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestLNS[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ALY_seg24);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
				SetTimer(Timer_ALY_seg23, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
				SetTimer(Timer_ALY_seg25, Elapse_AutoTest_LNS, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg5[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALY_seg25 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_ALY_seg25);

			m_nOrder = sizeof(g_nAutoTestLNS_seg6) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
			SetTimer(Timer_ALY_seg26, Elapse_AutoTest_LNS, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
			m_nOrder++;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ALY_seg26 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 5, dValue, CControlLayer::Instance().m_DL.adc.chn );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 5, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestLNS[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestLNS_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ALY_seg26);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
				m_nOrder++;
				CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
				SetTimer(Timer_ALY_seg25, Elapse_AutoTest_LNS, NULL);
			}
			else
			{
				EnableParams( 7, TRUE );
				// 恢复真空状态机的工作
				if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
					CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestLNS_seg6[m_nOrder];
			m_nOrder--;
			CControlLayer::Instance().LNS_YalDACSB_Change( nPos );
		}
	}
}
