// DlgAutoTestSCN.cpp : implementation file
//

#include "stdafx.h"
#include "insample0.h"
#include "DlgAutoTestSCN.h"

#include "Insample0Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	Timer_StigX_seg11		1
#define Timer_StigX_seg12		2
#define Timer_StigX_seg13		3
#define Timer_StigX_seg14		4
#define Timer_StigX_seg15		5
#define Timer_StigX_seg16		6

#define	Timer_StigX_seg21		7
#define Timer_StigX_seg22		8
#define Timer_StigX_seg23		9
#define Timer_StigX_seg24		10
#define Timer_StigX_seg25		11
#define Timer_StigX_seg26		12

#define	Timer_StigY_seg11		21
#define Timer_StigY_seg12		22
#define Timer_StigY_seg13		23
#define Timer_StigY_seg14		24
#define Timer_StigY_seg15		25
#define Timer_StigY_seg16		26

#define	Timer_StigY_seg21		27
#define Timer_StigY_seg22		28
#define Timer_StigY_seg23		29
#define Timer_StigY_seg24		30
#define Timer_StigY_seg25		31
#define Timer_StigY_seg26		32

#define Timer_ShiftX_seg11		41
#define Timer_ShiftX_seg12		42
#define Timer_ShiftX_seg13		43
#define Timer_ShiftX_seg14		44
#define Timer_ShiftX_seg15		45
#define Timer_ShiftX_seg16		46

#define Timer_ShiftX_seg21		47
#define Timer_ShiftX_seg22		48
#define Timer_ShiftX_seg23		49
#define Timer_ShiftX_seg24		50
#define Timer_ShiftX_seg25		51
#define Timer_ShiftX_seg26		52

#define Timer_ShiftY_seg11		61
#define Timer_ShiftY_seg12		62
#define Timer_ShiftY_seg13		63
#define Timer_ShiftY_seg14		64
#define Timer_ShiftY_seg15		65
#define Timer_ShiftY_seg16		66

#define Timer_ShiftY_seg21		67
#define Timer_ShiftY_seg22		68
#define Timer_ShiftY_seg23		69
#define Timer_ShiftY_seg24		70
#define Timer_ShiftY_seg25		71
#define Timer_ShiftY_seg26		72

#define Elapse_AutoTest_SCN	1000

int g_nAutoTestSCN_seg1[] = { 0x801, 0x802, 0x804, 0x808 };
int g_nAutoTestSCN_seg2[] = { 0x810, 0x820, 0x840, 0x880 };
int g_nAutoTestSCN_seg3[] = { 0x900, 0xA00, 0xC00, 0xFFF };

int g_nAutoTestSCN_seg4[] = { 0x800, 0x400, 0x200, 0x100 };
int g_nAutoTestSCN_seg5[] = { 0x080, 0x040, 0x020, 0x010 };
int g_nAutoTestSCN_seg6[] = { 0x008, 0x004, 0x002, 0x001 };

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestSCN dialog


CDlgAutoTestSCN::CDlgAutoTestSCN(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoTestSCN::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoTestSCN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgAutoTestSCN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoTestSCN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutoTestSCN, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoTestSCN)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_SaveGraph, OnSaveGraph)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_11, OnStartStigX)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_21, OnStartStigY)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_31, OnStartShiftX)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_41, OnStartShiftY)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_12, OnStartStigX2)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_22, OnStartStigY2)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_32, OnStartShiftX2)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_42, OnStartShiftY2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestSCN message handlers

void CDlgAutoTestSCN::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	for( int i=0; i<3; i++ )
		PaintGraph( &dc, i );

	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgAutoTestSCN::PaintGraph( CDC* pDC, int nIndex, BOOL bErase )
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

	if( theApp.m_AT.nAutoTestSCN_Total[nIndex] < 2 )
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
	nTotal = theApp.m_AT.nAutoTestSCN_Total[nIndex];

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
		dYMin = (theApp.m_AT.pAutoTestSCN[nIndex])[xMin];
		for( i=xMin+1; i<xMax; i++ )
		{
			if( theApp.m_AT.pAutoTestSCN[nIndex][i] < dYMin )
				dYMin = theApp.m_AT.pAutoTestSCN[nIndex][i];
		}
//		dYMinLog = log(dYMin) / log(2);
	}
	{
		dYMax = (theApp.m_AT.pAutoTestSCN[nIndex])[xMin];
		for( i=xMin+1; i<xMax; i++ )
		{
			if( theApp.m_AT.pAutoTestSCN[nIndex][i] > dYMax )
				dYMax = theApp.m_AT.pAutoTestSCN[nIndex][i];
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
		y = (int)(rc1.top + ( nHeight - (theApp.m_AT.pAutoTestSCN[nIndex][xMin] -dYMin) / (dYMax-dYMin) * nHeight ));
//		dY		= theApp.m_AT.pAutoTestSCN[nIndex][xMin];
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
			y = (int)(rc1.top + ( nHeight - (theApp.m_AT.pAutoTestSCN[nIndex][i] -dYMin) / (dYMax-dYMin) * nHeight ));
//			dY		= theApp.m_AT.pAutoTestSCN[nIndex][i];
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
	double dValue = theApp.GetPhysicsFromAdcs(m_nChnIndex2, dYMin);
	str.Format( "%.6f ( %.6f )", dYMin, dValue );
	pDC->TextOut( rc1.left -8, rc1.bottom -sz.cy/2, str );
	// 标注纵向最大值
	dValue = theApp.GetPhysicsFromAdcs(m_nChnIndex2, dYMax);
	str.Format( "%.6f ( %.6f )", dYMax, dValue );
	pDC->TextOut( rc1.left -8, rc1.top -sz.cy/2, str );
	// 画标签
	int* pn;
	if( m_bReverse )
	{
		switch( nIndex )
		{
		case 0:
			pn = g_nAutoTestSCN_seg4;
			break;
		case 1:
			pn = g_nAutoTestSCN_seg5;
			break;
		case 2:
			pn = g_nAutoTestSCN_seg6;
			break;
		}
	}
	else
	{
		switch( nIndex )
		{
		case 0:
			pn = g_nAutoTestSCN_seg1;
			break;
		case 1:
			pn = g_nAutoTestSCN_seg2;
			break;
		case 2:
			pn = g_nAutoTestSCN_seg3;
			break;
		}
	}
	str.Format( "%s [%04X---%04X]", theApp.m_DL.adc.chn[m_nChnIndex2].strLabel, pn[0], pn[AutoTest_12bitDAC_StepsEach-1] );
	sz = pDC->GetTextExtent( str );
	pDC->TextOut( rc.left + rc.Width()/2 - sz.cx/2, rc.top +11, str );
	pDC->SetBkMode(noldBKmode);

	pen.DeleteObject();
	penGrid.DeleteObject();
}

void CDlgAutoTestSCN::OnSaveGraph() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CString filename;
	char szFilter[] = "BMP file|*.bmp|JPG file|*.jpg|PNG file|*.png|TIF file|*.tif|PCX file|*.pcx|TGA file|*.tga||";
	CFileDialog dlgFile(FALSE, "png", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	if( dlgFile.DoModal() == IDOK )
		filename = dlgFile.GetPathName();
	else
		return;
	// 获取后缀名
	CString ext = "";
	int i, len = filename.GetLength();
	for ( i = len-1; i >= 0; i--)
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
	str.Format( "AutoTest SCN" );
	bmpDC.TextOut( 10, 0, str );
	for( i=0; i<3; i++ )
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

void CDlgAutoTestSCN::InitParams()
{
	for( int i=0; i<4; i++ )
	{
		theApp.m_AT.pAutoTestSCN[i] = new double[AutoTest_12bitDAC_Steps*3];
		theApp.m_AT.nAutoTestSCN_Total[i] = 0;
	}
}

void CDlgAutoTestSCN::EnableParams( int nIndex, BOOL bEnable )
{
	if( bEnable )
	{
		int nTimerIndex = 0;

		// 设置原始值
		/////////////////////////////////////////////////////////////////
		switch( nIndex )
		{
		case	0:
			// Stigmator
			XstigDACSB_Change( theApp.m_AT.nAutoTestMCI_OldPos[0] );
			nTimerIndex = Timer_StigX_seg11;
			break;
		case	1:
			// Stigmator
			XstigDACSB_Change( theApp.m_AT.nAutoTestMCI_OldPos[0] );
			nTimerIndex = Timer_StigX_seg21;
			break;
		case	2:
			// Stigmator
			YstigDACSB_Change( theApp.m_AT.nAutoTestMCI_OldPos[1] );
			nTimerIndex = Timer_StigY_seg11;
			break;
		case	3:
			// Stigmator
			YstigDACSB_Change( theApp.m_AT.nAutoTestMCI_OldPos[1] );
			nTimerIndex = Timer_StigY_seg21;
			break;
		case	4:
			// ScanShiftX
			XshiftDACSB_Change( theApp.m_AT.nAutoTestMCI_OldPos[2] );
			nTimerIndex = Timer_ShiftX_seg11;
			break;
		case	5:
			// ScanShiftX
			XshiftDACSB_Change( theApp.m_AT.nAutoTestMCI_OldPos[2] );
			nTimerIndex = Timer_ShiftX_seg21;
			break;
		case	6:
			// ScanShiftY
			YshiftDACSB_Change( theApp.m_AT.nAutoTestMCI_OldPos[3] );
			nTimerIndex = Timer_ShiftY_seg11;
			break;
		case	7:
			// ScanShiftY
			YshiftDACSB_Change( theApp.m_AT.nAutoTestMCI_OldPos[3] );
			nTimerIndex = Timer_ShiftY_seg21;
			break;
		}

		// 停止定时器
		int i;
		for( i=0; i<6; i++ )
			KillTimer( nTimerIndex +i);
		// 恢复按钮状态
		for( i=0; i<8; i++ )
		{
			((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +i)))->EnableWindow( TRUE );
			((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +i)))->SetCheck( 0 );
		}
		// 关闭记录文件
		CloseHandle(theApp.m_AT.hLogFile);
	}
	else
	{
		// 设置按钮状态，准备检测
		for( int i=0; i<8; i++ )
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
		str.Format( "%szAutoTestSCN-%s.txt", appfilepath, strTemp );
		theApp.m_AT.hLogFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	}
}

void CDlgAutoTestSCN::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent >= Timer_StigX_seg11 && nIDEvent <=Timer_StigX_seg16 )
		TimerChangeStigX( nIDEvent );
	else if( nIDEvent >= Timer_StigY_seg11 && nIDEvent <=Timer_StigY_seg16 )
		TimerChangeStigY( nIDEvent );
	else if( nIDEvent >= Timer_ShiftX_seg11 && nIDEvent <=Timer_ShiftX_seg16 )
		TimerChangeShiftX( nIDEvent );
	else if( nIDEvent >= Timer_ShiftY_seg11 && nIDEvent <=Timer_ShiftY_seg16 )
		TimerChangeShiftY( nIDEvent );
	else if( nIDEvent >= Timer_StigX_seg21 && nIDEvent <=Timer_StigX_seg26 )
		TimerChangeStigXReverse( nIDEvent );
	else if( nIDEvent >= Timer_StigY_seg21 && nIDEvent <=Timer_StigY_seg26 )
		TimerChangeStigYReverse( nIDEvent );
	else if( nIDEvent >= Timer_ShiftX_seg21 && nIDEvent <=Timer_ShiftX_seg26 )
		TimerChangeShiftXReverse( nIDEvent );
	else if( nIDEvent >= Timer_ShiftY_seg21 && nIDEvent <=Timer_ShiftY_seg26 )
		TimerChangeShiftYReverse( nIDEvent );
	CDialog::OnTimer(nIDEvent);
}

void CDlgAutoTestSCN::OnStartStigX() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11)))->GetCheck();
	if( nCheck )
	{
		theApp.ChangedADCselectOB( 0 );
		theApp.ChangedADCsChSelect( 0 );
		theApp.ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Stig_X;
		m_nChnIndex2	= 4;
		m_strRes		= "";
		int nMax = AutoTest_12bitDAC_Steps;
		if( theApp.m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			theApp.m_AT.pAutoTestSCN[0][i] = 0;
			theApp.m_AT.pAutoTestSCN[1][i] = 0;
			theApp.m_AT.pAutoTestSCN[2][i] = 0;
		}
		theApp.m_AT.nAutoTestSCN_Total[0] = 0;
		theApp.m_AT.nAutoTestSCN_Total[1] = 0;
		theApp.m_AT.nAutoTestSCN_Total[2] = 0;

		theApp.m_AT.nAutoTestSCN_OldPos[0] = theApp.m_DL.diag[Diag_Stig_X].nCurPos;
		m_bContinue = TRUE;
		m_bReverse	= FALSE;

		int nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];	// 设置起始值
		m_nOrder++;
		XstigDACSB_Change( nPos );
		SetTimer(Timer_StigX_seg11, Elapse_AutoTest_SCN, NULL);

		EnableParams( 0, FALSE );
	}
	else
	{
		EnableParams( 0, TRUE );
	}
}

void CDlgAutoTestSCN::OnStartStigY() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_21)))->GetCheck();
	if( nCheck )
	{
		theApp.ChangedADCselectOB( 0 );
		theApp.ChangedADCsChSelect( 1 );
		theApp.ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Stig_Y;
		m_nChnIndex2	= 6;
		m_strRes		= "";
		int nMax = AutoTest_12bitDAC_Steps;
		if( theApp.m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			theApp.m_AT.pAutoTestSCN[0][i] = 0;
			theApp.m_AT.pAutoTestSCN[1][i] = 0;
			theApp.m_AT.pAutoTestSCN[2][i] = 0;
		}
		theApp.m_AT.nAutoTestSCN_Total[0] = 0;
		theApp.m_AT.nAutoTestSCN_Total[1] = 0;
		theApp.m_AT.nAutoTestSCN_Total[2] = 0;

		theApp.m_AT.nAutoTestSCN_OldPos[1] = theApp.m_DL.diag[Diag_Stig_Y].nCurPos;
		m_bContinue = TRUE;
		m_bReverse	= FALSE;

		int nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];	// 设置起始值
		m_nOrder++;
		YstigDACSB_Change( nPos );
		SetTimer(Timer_StigY_seg11, Elapse_AutoTest_SCN, NULL);

		EnableParams( 2, FALSE );
	}
	else
	{
		EnableParams( 2, TRUE );
	}
}

void CDlgAutoTestSCN::OnStartShiftX() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_31)))->GetCheck();
	if( nCheck )
	{
		theApp.ChangedADCselectOB( 0 );
		theApp.ChangedADCsChSelect( 2 );
		theApp.ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Shift_X;
		m_nChnIndex2	= 0;
		m_strRes		= "";
		int nMax = AutoTest_12bitDAC_Steps;
		if( theApp.m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			theApp.m_AT.pAutoTestSCN[0][i] = 0;
			theApp.m_AT.pAutoTestSCN[1][i] = 0;
			theApp.m_AT.pAutoTestSCN[2][i] = 0;
		}
		theApp.m_AT.nAutoTestSCN_Total[0] = 0;
		theApp.m_AT.nAutoTestSCN_Total[1] = 0;
		theApp.m_AT.nAutoTestSCN_Total[2] = 0;

		theApp.m_AT.nAutoTestSCN_OldPos[2] = theApp.m_DL.diag[Diag_Shift_X].nCurPos;
		m_bContinue = TRUE;
		m_bReverse	= FALSE;

		// FineMag居中，Digital Shift归零，测试Analog Shift
//		int nValue = 0x800;
		WORD wValue = (((0x08 & 0xF) | (0x02 << 6)) << 8) | 0x00;	// X FineMag
		theApp.WriteToBoard( USB_Det45StoreDAC, TRUE, wValue );
		theApp.WriteToBoard( USB_SetXoffset, TRUE, 0 );	// Digital Shift X

		int nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];	// 设置起始值
		m_nOrder++;
		XshiftDACSB_Change( nPos );
		SetTimer(Timer_ShiftX_seg11, Elapse_AutoTest_SCN, NULL);

		EnableParams( 4, FALSE );
	}
	else
	{
		EnableParams( 4, TRUE );
	}
}

void CDlgAutoTestSCN::OnStartShiftY() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_41)))->GetCheck();
	if( nCheck )
	{
		theApp.ChangedADCselectOB( 0 );
		theApp.ChangedADCsChSelect( 3 );
		theApp.ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Shift_Y;
		m_nChnIndex2	= 2;
		m_strRes		= "";
		int nMax = AutoTest_12bitDAC_Steps;
		if( theApp.m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			theApp.m_AT.pAutoTestSCN[0][i] = 0;
			theApp.m_AT.pAutoTestSCN[1][i] = 0;
			theApp.m_AT.pAutoTestSCN[2][i] = 0;
		}
		theApp.m_AT.nAutoTestSCN_Total[0] = 0;
		theApp.m_AT.nAutoTestSCN_Total[1] = 0;
		theApp.m_AT.nAutoTestSCN_Total[2] = 0;

		theApp.m_AT.nAutoTestSCN_OldPos[3] = theApp.m_DL.diag[Diag_Shift_Y].nCurPos;
		m_bContinue = TRUE;
		m_bReverse	= FALSE;

		// FineMag居中，Digital Shift归零，测试Analog Shift
//		int nValue = 0x800;
		WORD wValue = (((0x08 & 0xF) | (0x03 << 6)) << 8) | 0x00;	// Y FineMag
		theApp.WriteToBoard( USB_Det45StoreDAC, TRUE, wValue );
		theApp.WriteToBoard( USB_SetYoffset, TRUE, 0 );	// Digital Shift Y

		int nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];	// 设置起始值
		m_nOrder++;
		YshiftDACSB_Change( nPos );
		SetTimer(Timer_ShiftY_seg11, Elapse_AutoTest_SCN, NULL);

		EnableParams( 6, FALSE );
	}
	else
	{
		EnableParams( 6, TRUE );
	}
}

void CDlgAutoTestSCN::OnStartStigX2() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_12)))->GetCheck();
	if( nCheck )
	{
		theApp.ChangedADCselectOB( 0 );
		theApp.ChangedADCsChSelect( 0 );
		theApp.ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Stig_X;
		m_nChnIndex2	= 5;
		m_strRes		= "";
		int nMax = AutoTest_12bitDAC_Steps;
		if( theApp.m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			theApp.m_AT.pAutoTestSCN[0][i] = 0;
			theApp.m_AT.pAutoTestSCN[1][i] = 0;
			theApp.m_AT.pAutoTestSCN[2][i] = 0;
		}
		theApp.m_AT.nAutoTestSCN_Total[0] = 0;
		theApp.m_AT.nAutoTestSCN_Total[1] = 0;
		theApp.m_AT.nAutoTestSCN_Total[2] = 0;

		theApp.m_AT.nAutoTestSCN_OldPos[0] = theApp.m_DL.diag[Diag_Stig_X].nCurPos;
		m_bContinue = TRUE;
		m_bReverse	= TRUE;

		int nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];	// 设置起始值
		m_nOrder++;
		XstigDACSB_Change( nPos );
		SetTimer(Timer_StigX_seg21, Elapse_AutoTest_SCN, NULL);

		EnableParams( 1, FALSE );
	}
	else
	{
		EnableParams( 1, TRUE );
	}
}

void CDlgAutoTestSCN::OnStartStigY2() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_22)))->GetCheck();
	if( nCheck )
	{
		theApp.ChangedADCselectOB( 0 );
		theApp.ChangedADCsChSelect( 1 );
		theApp.ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Stig_Y;
		m_nChnIndex2	= 7;
		m_strRes		= "";
		int nMax = AutoTest_12bitDAC_Steps;
		if( theApp.m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			theApp.m_AT.pAutoTestSCN[0][i] = 0;
			theApp.m_AT.pAutoTestSCN[1][i] = 0;
			theApp.m_AT.pAutoTestSCN[2][i] = 0;
		}
		theApp.m_AT.nAutoTestSCN_Total[0] = 0;
		theApp.m_AT.nAutoTestSCN_Total[1] = 0;
		theApp.m_AT.nAutoTestSCN_Total[2] = 0;

		theApp.m_AT.nAutoTestSCN_OldPos[1] = theApp.m_DL.diag[Diag_Stig_Y].nCurPos;
		m_bContinue = TRUE;
		m_bReverse	= TRUE;

		int nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];	// 设置起始值
		m_nOrder++;
		YstigDACSB_Change( nPos );
		SetTimer(Timer_StigY_seg21, Elapse_AutoTest_SCN, NULL);

		EnableParams( 3, FALSE );
	}
	else
	{
		EnableParams( 3, TRUE );
	}
}

void CDlgAutoTestSCN::OnStartShiftX2() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_32)))->GetCheck();
	if( nCheck )
	{
		theApp.ChangedADCselectOB( 0 );
		theApp.ChangedADCsChSelect( 2 );
		theApp.ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Shift_X;
		m_nChnIndex2	= 1;
		m_strRes		= "";
		int nMax = AutoTest_12bitDAC_Steps;
		if( theApp.m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			theApp.m_AT.pAutoTestSCN[0][i] = 0;
			theApp.m_AT.pAutoTestSCN[1][i] = 0;
			theApp.m_AT.pAutoTestSCN[2][i] = 0;
		}
		theApp.m_AT.nAutoTestSCN_Total[0] = 0;
		theApp.m_AT.nAutoTestSCN_Total[1] = 0;
		theApp.m_AT.nAutoTestSCN_Total[2] = 0;

		theApp.m_AT.nAutoTestSCN_OldPos[2] = theApp.m_DL.diag[Diag_Shift_X].nCurPos;
		m_bContinue = TRUE;
		m_bReverse	= TRUE;

		// FineMag居中，Digital Shift归零，测试Analog Shift
//		int nValue = 0x800;
		WORD wValue = (((0x08 & 0xF) | (0x02 << 6)) << 8) | 0x00;	// X FineMag
		theApp.WriteToBoard( USB_Det45StoreDAC, TRUE, wValue );
		theApp.WriteToBoard( USB_SetXoffset, TRUE, 0 );	// Digital Shift X

		int nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];	// 设置起始值
		m_nOrder++;
		XshiftDACSB_Change( nPos );
		SetTimer(Timer_ShiftX_seg21, Elapse_AutoTest_SCN, NULL);

		EnableParams( 5, FALSE );
	}
	else
	{
		EnableParams( 5, TRUE );
	}
}

void CDlgAutoTestSCN::OnStartShiftY2() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_42)))->GetCheck();
	if( nCheck )
	{
		theApp.ChangedADCselectOB( 0 );
		theApp.ChangedADCsChSelect( 3 );
		theApp.ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Shift_Y;
		m_nChnIndex2	= 3;
		m_strRes		= "";
		int nMax = AutoTest_12bitDAC_Steps;
		if( theApp.m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			theApp.m_AT.pAutoTestSCN[0][i] = 0;
			theApp.m_AT.pAutoTestSCN[1][i] = 0;
			theApp.m_AT.pAutoTestSCN[2][i] = 0;
		}
		theApp.m_AT.nAutoTestSCN_Total[0] = 0;
		theApp.m_AT.nAutoTestSCN_Total[1] = 0;
		theApp.m_AT.nAutoTestSCN_Total[2] = 0;

		theApp.m_AT.nAutoTestSCN_OldPos[3] = theApp.m_DL.diag[Diag_Shift_Y].nCurPos;
		m_bContinue = TRUE;
		m_bReverse	= TRUE;

		// FineMag居中，Digital Shift归零，测试Analog Shift
//		int nValue = 0x800;
		WORD wValue = (((0x08 & 0xF) | (0x03 << 6)) << 8) | 0x00;	// Y FineMag
		theApp.WriteToBoard( USB_Det45StoreDAC, TRUE, wValue );
		theApp.WriteToBoard( USB_SetYoffset, TRUE, 0 );	// Digital Shift Y

		int nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];	// 设置起始值
		m_nOrder++;
		YshiftDACSB_Change( nPos );
		SetTimer(Timer_ShiftY_seg21, Elapse_AutoTest_SCN, NULL);

		EnableParams( 7, FALSE );
	}
	else
	{
		EnableParams( 7, TRUE );
	}
}

void CDlgAutoTestSCN::TimerChangeStigX( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_StigX_seg11 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_StigX_seg11);

			m_nOrder = sizeof(g_nAutoTestSCN_seg1) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
			m_nOrder--;
			XstigDACSB_Change( nPos );
			SetTimer(Timer_StigX_seg12, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
			m_nOrder++;
			XstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigX_seg12 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_StigX_seg12);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
				m_nOrder++;
				XstigDACSB_Change( nPos );
				SetTimer(Timer_StigX_seg11, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
				m_nOrder++;
				XstigDACSB_Change( nPos );
				SetTimer(Timer_StigX_seg13, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
			m_nOrder--;
			XstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigX_seg13 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_StigX_seg13);

			m_nOrder = sizeof(g_nAutoTestSCN_seg2) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
			m_nOrder--;
			XstigDACSB_Change( nPos );
			SetTimer(Timer_StigX_seg14, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
			m_nOrder++;
			XstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigX_seg14 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_StigX_seg14);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
				m_nOrder++;
				XstigDACSB_Change( nPos );
				SetTimer(Timer_StigX_seg13, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
				m_nOrder++;
				XstigDACSB_Change( nPos );
				SetTimer(Timer_StigX_seg15, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
			m_nOrder--;
			XstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigX_seg15 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_StigX_seg15);

			m_nOrder = sizeof(g_nAutoTestSCN_seg3) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
			m_nOrder--;
			XstigDACSB_Change( nPos );
			SetTimer(Timer_StigX_seg16, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
			m_nOrder++;
			XstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigX_seg16 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_StigX_seg16);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
				m_nOrder++;
				XstigDACSB_Change( nPos );
				SetTimer(Timer_StigX_seg15, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				EnableParams( 0, TRUE );
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
			m_nOrder--;
			XstigDACSB_Change( nPos );
		}
	}
}

void CDlgAutoTestSCN::TimerChangeStigY( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_StigY_seg11 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_StigY_seg11);

			m_nOrder = sizeof(g_nAutoTestSCN_seg1) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
			m_nOrder--;
			YstigDACSB_Change( nPos );
			SetTimer(Timer_StigY_seg12, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
			m_nOrder++;
			YstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigY_seg12 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_StigY_seg12);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
				m_nOrder++;
				YstigDACSB_Change( nPos );
				SetTimer(Timer_StigY_seg11, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
				m_nOrder++;
				YstigDACSB_Change( nPos );
				SetTimer(Timer_StigY_seg13, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
			m_nOrder--;
			YstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigY_seg13 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_StigY_seg13);

			m_nOrder = sizeof(g_nAutoTestSCN_seg2) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
			m_nOrder--;
			YstigDACSB_Change( nPos );
			SetTimer(Timer_StigY_seg14, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
			m_nOrder++;
			YstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigY_seg14 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_StigY_seg14);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
				m_nOrder++;
				YstigDACSB_Change( nPos );
				SetTimer(Timer_StigY_seg13, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
				m_nOrder++;
				YstigDACSB_Change( nPos );
				SetTimer(Timer_StigY_seg15, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
			m_nOrder--;
			YstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigY_seg15 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_StigY_seg15);

			m_nOrder = sizeof(g_nAutoTestSCN_seg3) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
			m_nOrder--;
			YstigDACSB_Change( nPos );
			SetTimer(Timer_StigY_seg16, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
			m_nOrder++;
			YstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigY_seg16 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_StigY_seg16);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
				m_nOrder++;
				YstigDACSB_Change( nPos );
				SetTimer(Timer_StigY_seg15, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				EnableParams( 2, TRUE );
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
			m_nOrder--;
			YstigDACSB_Change( nPos );
		}
	}
}

void CDlgAutoTestSCN::TimerChangeShiftX( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_ShiftX_seg11 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_ShiftX_seg11);

			m_nOrder = sizeof(g_nAutoTestSCN_seg1) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
			m_nOrder--;
			XshiftDACSB_Change( nPos );
			SetTimer(Timer_ShiftX_seg12, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
			m_nOrder++;
			XshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftX_seg12 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ShiftX_seg12);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
				m_nOrder++;
				XshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftX_seg11, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
				m_nOrder++;
				XshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftX_seg13, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
			m_nOrder--;
			XshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftX_seg13 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_ShiftX_seg13);

			m_nOrder = sizeof(g_nAutoTestSCN_seg2) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
			m_nOrder--;
			XshiftDACSB_Change( nPos );
			SetTimer(Timer_ShiftX_seg14, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
			m_nOrder++;
			XshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftX_seg14 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ShiftX_seg14);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
				m_nOrder++;
				XshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftX_seg13, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
				m_nOrder++;
				XshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftX_seg15, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
			m_nOrder--;
			XshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftX_seg15 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_ShiftX_seg15);

			m_nOrder = sizeof(g_nAutoTestSCN_seg3) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
			m_nOrder--;
			XshiftDACSB_Change( nPos );
			SetTimer(Timer_ShiftX_seg16, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
			m_nOrder++;
			XshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftX_seg16 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ShiftX_seg16);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
				m_nOrder++;
				XshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftX_seg15, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				EnableParams( 4, TRUE );
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
			m_nOrder--;
			XshiftDACSB_Change( nPos );
		}
	}
}

void CDlgAutoTestSCN::TimerChangeShiftY( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_ShiftY_seg11 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_ShiftY_seg11);

			m_nOrder = sizeof(g_nAutoTestSCN_seg1) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
			m_nOrder--;
			YshiftDACSB_Change( nPos );
			SetTimer(Timer_ShiftY_seg12, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
			m_nOrder++;
			YshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftY_seg12 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ShiftY_seg12);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
				m_nOrder++;
				YshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftY_seg11, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
				m_nOrder++;
				YshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftY_seg13, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg1[m_nOrder];
			m_nOrder--;
			YshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftY_seg13 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_ShiftY_seg13);

			m_nOrder = sizeof(g_nAutoTestSCN_seg2) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
			m_nOrder--;
			YshiftDACSB_Change( nPos );
			SetTimer(Timer_ShiftY_seg14, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
			m_nOrder++;
			YshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftY_seg14 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ShiftY_seg14);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
				m_nOrder++;
				YshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftY_seg13, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
				m_nOrder++;
				YshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftY_seg15, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg2[m_nOrder];
			m_nOrder--;
			YshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftY_seg15 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_ShiftY_seg15);

			m_nOrder = sizeof(g_nAutoTestSCN_seg3) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
			m_nOrder--;
			YshiftDACSB_Change( nPos );
			SetTimer(Timer_ShiftY_seg16, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
			m_nOrder++;
			YshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftY_seg16 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ShiftY_seg16);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
				m_nOrder++;
				YshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftY_seg15, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				EnableParams( 6, TRUE );
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg3[m_nOrder];
			m_nOrder--;
			YshiftDACSB_Change( nPos );
		}
	}
}

void CDlgAutoTestSCN::TimerChangeStigXReverse( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_StigX_seg21 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_StigX_seg21);

			m_nOrder = sizeof(g_nAutoTestSCN_seg4) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
			m_nOrder--;
			XstigDACSB_Change( nPos );
			SetTimer(Timer_StigX_seg22, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
			m_nOrder++;
			XstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigX_seg22 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_StigX_seg22);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
				m_nOrder++;
				XstigDACSB_Change( nPos );
				SetTimer(Timer_StigX_seg21, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
				m_nOrder++;
				XstigDACSB_Change( nPos );
				SetTimer(Timer_StigX_seg23, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
			m_nOrder--;
			XstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigX_seg23 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_StigX_seg23);

			m_nOrder = sizeof(g_nAutoTestSCN_seg5) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
			m_nOrder--;
			XstigDACSB_Change( nPos );
			SetTimer(Timer_StigX_seg24, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
			m_nOrder++;
			XstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigX_seg24 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_StigX_seg24);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
				m_nOrder++;
				XstigDACSB_Change( nPos );
				SetTimer(Timer_StigX_seg23, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
				m_nOrder++;
				XstigDACSB_Change( nPos );
				SetTimer(Timer_StigX_seg25, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
			m_nOrder--;
			XstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigX_seg25 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_StigX_seg25);

			m_nOrder = sizeof(g_nAutoTestSCN_seg6) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
			m_nOrder--;
			XstigDACSB_Change( nPos );
			SetTimer(Timer_StigX_seg26, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
			m_nOrder++;
			XstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigX_seg26 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_StigX_seg26);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
				m_nOrder++;
				XstigDACSB_Change( nPos );
				SetTimer(Timer_StigX_seg25, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				EnableParams( 1, TRUE );
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
			m_nOrder--;
			XstigDACSB_Change( nPos );
		}
	}
}

void CDlgAutoTestSCN::TimerChangeStigYReverse( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_StigY_seg21 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_StigY_seg21);

			m_nOrder = sizeof(g_nAutoTestSCN_seg1) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
			m_nOrder--;
			YstigDACSB_Change( nPos );
			SetTimer(Timer_StigY_seg22, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
			m_nOrder++;
			YstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigY_seg22 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_StigY_seg22);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
				m_nOrder++;
				YstigDACSB_Change( nPos );
				SetTimer(Timer_StigY_seg21, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
				m_nOrder++;
			YstigDACSB_Change( nPos );
				SetTimer(Timer_StigY_seg23, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
			m_nOrder--;
			YstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigY_seg23 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_StigY_seg23);

			m_nOrder = sizeof(g_nAutoTestSCN_seg5) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
			m_nOrder--;
			YstigDACSB_Change( nPos );
			SetTimer(Timer_StigY_seg24, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
			m_nOrder++;
			YstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigY_seg24 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_StigY_seg24);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
				m_nOrder++;
				YstigDACSB_Change( nPos );
				SetTimer(Timer_StigY_seg23, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
				m_nOrder++;
				YstigDACSB_Change( nPos );
				SetTimer(Timer_StigY_seg25, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
			m_nOrder--;
			YstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigY_seg25 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_StigY_seg25);

			m_nOrder = sizeof(g_nAutoTestSCN_seg6) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
			m_nOrder--;
			YstigDACSB_Change( nPos );
			SetTimer(Timer_StigY_seg26, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
			m_nOrder++;
			YstigDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_StigY_seg26 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_StigY_seg26);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
				m_nOrder++;
				YstigDACSB_Change( nPos );
				SetTimer(Timer_StigY_seg25, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				EnableParams( 3, TRUE );
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
			m_nOrder--;
			YstigDACSB_Change( nPos );
		}
	}
}

void CDlgAutoTestSCN::TimerChangeShiftXReverse( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_ShiftX_seg21 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_ShiftX_seg21);

			m_nOrder = sizeof(g_nAutoTestSCN_seg4) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
			m_nOrder--;
			XshiftDACSB_Change( nPos );
			SetTimer(Timer_ShiftX_seg22, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
			m_nOrder++;
			XshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftX_seg22 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ShiftX_seg22);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
				m_nOrder++;
				XshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftX_seg21, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
				m_nOrder++;
			XshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftX_seg23, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
			m_nOrder--;
			XshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftX_seg23 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_ShiftX_seg23);

			m_nOrder = sizeof(g_nAutoTestSCN_seg5) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
			m_nOrder--;
			XshiftDACSB_Change( nPos );
			SetTimer(Timer_ShiftX_seg24, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
			m_nOrder++;
			XshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftX_seg24 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ShiftX_seg24);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
				m_nOrder++;
				XshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftX_seg23, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
				m_nOrder++;
				XshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftX_seg25, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
			m_nOrder--;
			XshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftX_seg25 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_ShiftX_seg25);

			m_nOrder = sizeof(g_nAutoTestSCN_seg6) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
			m_nOrder--;
			XshiftDACSB_Change( nPos );
			SetTimer(Timer_ShiftX_seg26, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
			m_nOrder++;
			XshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftX_seg26 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ShiftX_seg26);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
				m_nOrder++;
				XshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftX_seg25, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				EnableParams( 5, TRUE );
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
			m_nOrder--;
			XshiftDACSB_Change( nPos );
		}
	}
}

void CDlgAutoTestSCN::TimerChangeShiftYReverse( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_ShiftY_seg21 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_ShiftY_seg21);

			m_nOrder = sizeof(g_nAutoTestSCN_seg4) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
			m_nOrder--;
			YshiftDACSB_Change( nPos );
			SetTimer(Timer_ShiftY_seg22, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
			m_nOrder++;
			YshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftY_seg22 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[0];
			theApp.m_AT.pAutoTestSCN[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ShiftY_seg22);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
				m_nOrder++;
				YshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftY_seg21, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
				m_nOrder++;
				YshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftY_seg23, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg4[m_nOrder];
			m_nOrder--;
			YshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftY_seg23 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_ShiftY_seg23);

			m_nOrder = sizeof(g_nAutoTestSCN_seg5) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
			m_nOrder--;
			YshiftDACSB_Change( nPos );
			SetTimer(Timer_ShiftY_seg24, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
			m_nOrder++;
			YshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftY_seg24 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[1];
			theApp.m_AT.pAutoTestSCN[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ShiftY_seg24);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
				m_nOrder++;
				YshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftY_seg23, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
				m_nOrder++;
				YshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftY_seg25, Elapse_AutoTest_SCN, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg5[m_nOrder];
			m_nOrder--;
			YshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftY_seg25 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_ShiftY_seg25);

			m_nOrder = sizeof(g_nAutoTestSCN_seg6) / sizeof(int) - 1;
			// 设置起始值
			nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
			m_nOrder--;
			YshiftDACSB_Change( nPos );
			SetTimer(Timer_ShiftY_seg26, Elapse_AutoTest_SCN, NULL);
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
			m_nOrder++;
			YshiftDACSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_ShiftY_seg26 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSCN_Total[2];
			theApp.m_AT.pAutoTestSCN[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSCN_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_ShiftY_seg26);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
				m_nOrder++;
				YshiftDACSB_Change( nPos );
				SetTimer(Timer_ShiftY_seg25, Elapse_AutoTest_SCN, NULL);
			}
			else
			{
				EnableParams( 7, TRUE );
			}
		}
		else
		{
			// 设置新值
			nPos = -2048 + g_nAutoTestSCN_seg6[m_nOrder];
			m_nOrder--;
			YshiftDACSB_Change( nPos );
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CDlgAutoTestSCN::XstigDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if Y), MSB = 0 if X.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	theApp.CommToBoards2(	USB_CID_SCN_Stigmation,		// Serial Comm operand low byte (X/Y Stig DAC circuit address)
							USB_TM_SCN_Write_16,		// Serial Comm operand high byte (16 bit write to SCN board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CDlgAutoTestSCN::YstigDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( (nValue >> 8 ) | 0x80 );		//Or &H80 (MSB = 1 if Y), MSB = 0 if X.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	theApp.CommToBoards2(	USB_CID_SCN_Stigmation,		// Serial Comm operand low byte (X/Y Stig DAC circuit address)
							USB_TM_SCN_Write_16,		// Serial Comm operand high byte (16 bit write to SCN board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CDlgAutoTestSCN::XshiftDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if X shift.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	theApp.CommToBoards2(	USB_CID_SCN_Xshift_Crossover,	// Serial Comm operand low byte (X shift/crossover DAC circuit address)
							USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}

void CDlgAutoTestSCN::YshiftDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if Y shift.
    ///////////////////////////////////////////////////////////////////////////

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	theApp.CommToBoards2(	USB_CID_SCN_Yshift_Crossover,	// Serial Comm operand low byte (Y shift/crossover DAC circuit address)
							USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
							byteLo,		// Serial Comm operand low byte
							byteHi );	// Serial Comm operand high byte
}
