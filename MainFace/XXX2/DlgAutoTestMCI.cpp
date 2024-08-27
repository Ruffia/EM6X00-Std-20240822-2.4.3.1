// DlgAutoTestMCI.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgAutoTestMCI.h"
#include "ControlLayer.h"
#include "BoardComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	Timer_FineMagX_seg11		1
#define Timer_FineMagX_seg12		2
#define Timer_FineMagX_seg13		3
#define Timer_FineMagX_seg14		4
#define Timer_FineMagX_seg15		5
#define Timer_FineMagX_seg16		6

#define	Timer_FineMagX_seg21		7
#define Timer_FineMagX_seg22		8
#define Timer_FineMagX_seg23		9
#define Timer_FineMagX_seg24		10
#define Timer_FineMagX_seg25		11
#define Timer_FineMagX_seg26		12

#define	Timer_FineMagY_seg11		21
#define Timer_FineMagY_seg12		22
#define Timer_FineMagY_seg13		23
#define Timer_FineMagY_seg14		24
#define Timer_FineMagY_seg15		25
#define Timer_FineMagY_seg16		26

#define	Timer_FineMagY_seg21		27
#define Timer_FineMagY_seg22		28
#define Timer_FineMagY_seg23		29
#define Timer_FineMagY_seg24		30
#define Timer_FineMagY_seg25		31
#define Timer_FineMagY_seg26		32

#define Timer_MCIShiftX_seg11		41
#define Timer_MCIShiftX_seg12		42
#define Timer_MCIShiftX_seg13		43
#define Timer_MCIShiftX_seg14		44
#define Timer_MCIShiftX_seg15		45
#define Timer_MCIShiftX_seg16		46

#define Timer_MCIShiftX_seg21		47
#define Timer_MCIShiftX_seg22		48
#define Timer_MCIShiftX_seg23		49
#define Timer_MCIShiftX_seg24		50
#define Timer_MCIShiftX_seg25		51
#define Timer_MCIShiftX_seg26		52

#define Timer_MCIShiftY_seg11		61
#define Timer_MCIShiftY_seg12		62
#define Timer_MCIShiftY_seg13		63
#define Timer_MCIShiftY_seg14		64
#define Timer_MCIShiftY_seg15		65
#define Timer_MCIShiftY_seg16		66

#define Timer_MCIShiftY_seg21		67
#define Timer_MCIShiftY_seg22		68
#define Timer_MCIShiftY_seg23		69
#define Timer_MCIShiftY_seg24		70
#define Timer_MCIShiftY_seg25		71
#define Timer_MCIShiftY_seg26		72

#define Elapse_AutoTest_MCI	1000

int g_nAutoTestMCI_seg1[] = { 0x801, 0x802, 0x804, 0x808 };
int g_nAutoTestMCI_seg2[] = { 0x810, 0x820, 0x840, 0x880 };
int g_nAutoTestMCI_seg3[] = { 0x900, 0xA00, 0xC00, 0xFFF };

int g_nAutoTestMCI_seg4[] = { 0x800, 0x400, 0x200, 0x100 };
int g_nAutoTestMCI_seg5[] = { 0x080, 0x040, 0x020, 0x010 };
int g_nAutoTestMCI_seg6[] = { 0x008, 0x004, 0x002, 0x001 };

int g_nAutoTestMCI_seg7[] = { 0x8001, 0x8002, 0x8004, 0x8008, 0x8010, 0x8020 };
int g_nAutoTestMCI_seg8[] = { 0x8020, 0x8040, 0x8080, 0x8100, 0x8200, 0x8400 };
int g_nAutoTestMCI_seg9[] = { 0x8400, 0x8800, 0x9000, 0xA000, 0xC000, 0xFFFF };

int g_nAutoTestMCI_seg10[] = { 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400 };
int g_nAutoTestMCI_seg11[] = { 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020 };
int g_nAutoTestMCI_seg12[] = { 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001 };

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestMCI dialog


CDlgAutoTestMCI::CDlgAutoTestMCI(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoTestMCI::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoTestMCI)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nOrder		= -1;
	m_nChnIndex1	= -1;	// 1为记录位置数组的索引
	m_nChnIndex2	= -1;	// 2为6ADC通道索引
	m_nSegIndex		= -1;	// 在做哪个功能的AutoTest
	m_bContinue		= TRUE;
	m_bReverse		= FALSE;
}


void CDlgAutoTestMCI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoTestMCI)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutoTestMCI, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoTestMCI)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_SaveGraph, OnSaveGraph)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_11, OnStartFineMagX)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_21, OnStartFineMagY)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_31, OnStartShiftX)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_41, OnStartShiftY)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_12, OnStartFineMagX2)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_22, OnStartFineMagY2)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_32, OnStartShiftX2)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_42, OnStartShiftY2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestMCI message handlers

void CDlgAutoTestMCI::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	for( int i=0; i<3; i++ )
		PaintGraph( &dc, i );

	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgAutoTestMCI::PaintGraph( CDC* pDC, int nIndex, BOOL bErase )
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

	if( CControlLayer::Instance().m_AT.nAutoTestMCI_Total[nIndex] < 2 )
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
	nTotal = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[nIndex];

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
		dYMin = (CControlLayer::Instance().m_AT.pAutoTestMCI[nIndex])[xMin];
		for( i=xMin+1; i<xMax; i++ )
		{
			if( CControlLayer::Instance().m_AT.pAutoTestMCI[nIndex][i] < dYMin )
				dYMin = CControlLayer::Instance().m_AT.pAutoTestMCI[nIndex][i];
		}
//		dYMinLog = log(dYMin) / log(2);
	}
	{
		dYMax = (CControlLayer::Instance().m_AT.pAutoTestMCI[nIndex])[xMin];
		for( i=xMin+1; i<xMax; i++ )
		{
			if( CControlLayer::Instance().m_AT.pAutoTestMCI[nIndex][i] > dYMax )
				dYMax = CControlLayer::Instance().m_AT.pAutoTestMCI[nIndex][i];
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
		y = (int)(rc1.top + ( nHeight - (CControlLayer::Instance().m_AT.pAutoTestMCI[nIndex][xMin] -dYMin) / (dYMax-dYMin) * nHeight ));
//		dY		= CControlLayer::Instance().m_AT.pAutoTestMCI[nIndex][xMin];
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
			y = (int)(rc1.top + ( nHeight - (CControlLayer::Instance().m_AT.pAutoTestMCI[nIndex][i] -dYMin) / (dYMax-dYMin) * nHeight ));
//			dY		= CControlLayer::Instance().m_AT.pAutoTestMCI[nIndex][i];
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
	double dValue = CBoardComm::Instance().GetPhysicsFromAdcs(m_nChnIndex2, dYMin, CControlLayer::Instance().m_DL.log.strCalc);
	str.Format( "%.6f ( %.6f )", dYMin, dValue );
	pDC->TextOut( rc1.left -8, rc1.bottom -sz.cy/2, str );
	// 标注纵向最大值
	dValue = CBoardComm::Instance().GetPhysicsFromAdcs(m_nChnIndex2, dYMax, CControlLayer::Instance().m_DL.log.strCalc);
	str.Format( "%.6f ( %.6f )", dYMax, dValue );
	pDC->TextOut( rc1.left -8, rc1.top -sz.cy/2, str );
	// 画标签
	int* pn;
	int nMax;
	if( m_nSegIndex < 4 )
	{
		if( m_nSegIndex % 2 > 0 )
		{
			switch( nIndex )
			{
			case 0:
				pn = g_nAutoTestMCI_seg4;
				break;
			case 1:
				pn = g_nAutoTestMCI_seg5;
				break;
			case 2:
				pn = g_nAutoTestMCI_seg6;
				break;
			}
		}
		else
		{
			switch( nIndex )
			{
			case 0:
				pn = g_nAutoTestMCI_seg1;
				break;
			case 1:
				pn = g_nAutoTestMCI_seg2;
				break;
			case 2:
				pn = g_nAutoTestMCI_seg3;
				break;
			}
		}
		nMax = AutoTest_12bitDAC_StepsEach -1;
	}
	else
	{
		if( m_nSegIndex % 2 > 0 )
		{
			switch( nIndex )
			{
			case 0:
				pn = g_nAutoTestMCI_seg10;
				break;
			case 1:
				pn = g_nAutoTestMCI_seg11;
				break;
			case 2:
				pn = g_nAutoTestMCI_seg12;
				break;
			}
		}
		else
		{
			switch( nIndex )
			{
			case 0:
				pn = g_nAutoTestMCI_seg7;
				break;
			case 1:
				pn = g_nAutoTestMCI_seg8;
				break;
			case 2:
				pn = g_nAutoTestMCI_seg9;
				break;
			}
		}
		nMax = AutoTest_16bitDAC_StepsEach -1;
	}
	str.Format( "%s [%04X---%04X]", CControlLayer::Instance().m_DL.adc.chn[m_nChnIndex2].strLabel, pn[0], pn[nMax] );
	sz = pDC->GetTextExtent( str );
	pDC->TextOut( rc.left + rc.Width()/2 - sz.cx/2, rc.top +11, str );
	pDC->SetBkMode(noldBKmode);

	pen.DeleteObject();
	penGrid.DeleteObject();
}

void CDlgAutoTestMCI::OnSaveGraph() 
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
	str.Format( "AutoTest MCI" );
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

void CDlgAutoTestMCI::InitParams()
{
	for( int i=0; i<4; i++ )
	{
		CControlLayer::Instance().m_AT.pAutoTestMCI[i] = new double[AutoTest_16bitDAC_Steps*3];
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[i] = 0;
	}
}

void CDlgAutoTestMCI::EnableParams( int nIndex, BOOL bEnable )
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
			// Stigmator
			ClassID.lVal = SemControlPanelClass_Stigmator;
			Flag.lVal	= 1;
			var.lVal	= CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[0];
			Xpos		= var;
			
			nTimerIndex = Timer_FineMagX_seg11;
			break;
		case	1:
			// Stigmator
			ClassID.lVal = SemControlPanelClass_Stigmator;
			Flag.lVal	= 1;
			var.lVal	= CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[0];
			Xpos		= var;

			nTimerIndex = Timer_FineMagX_seg21;
			break;
		case	2:
			// Stigmator
			ClassID.lVal = SemControlPanelClass_Stigmator;
			Flag.lVal	= 2;
			var.lVal	= CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[1];
			Ypos		= var;
			
			nTimerIndex = Timer_FineMagY_seg11;
			break;
		case	3:
			// Stigmator
			ClassID.lVal = SemControlPanelClass_Stigmator;
			Flag.lVal	= 2;
			var.lVal	= CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[1];
			Ypos		= var;

			nTimerIndex = Timer_FineMagY_seg21;
			break;
		case	4:
			// ScanShiftX
			ClassID.lVal = SemControlPanelClass_ScanShift;
			Flag.lVal	= 1;
			var.lVal	= CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[2];
			Xpos		= var;
			
			nTimerIndex = Timer_MCIShiftX_seg11;
			break;
		case	5:
			// ScanShiftX
			ClassID.lVal = SemControlPanelClass_ScanShift;
			Flag.lVal	= 1;
			var.lVal	= CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[2];
			Xpos		= var;

			nTimerIndex = Timer_MCIShiftX_seg21;
			break;
		case	6:
			// ScanShiftY
			ClassID.lVal = SemControlPanelClass_ScanShift;
			Flag.lVal	= 2;
			var.lVal	= CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[3];
			Ypos		= var;
			
			nTimerIndex = Timer_MCIShiftY_seg11;
			break;
		case	7:
			// ScanShiftY
			ClassID.lVal = SemControlPanelClass_ScanShift;
			Flag.lVal	= 2;
			var.lVal	= CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[3];
			Ypos		= var;

			nTimerIndex = Timer_MCIShiftY_seg21;
			break;
		}

		// 停止定时器
		for(int i=0; i<6; i++ )
			KillTimer( nTimerIndex +i);
		// 设置原始值
//		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
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
		str.Format( "%s\\Log\\AutoTestMCI-%s.txt", appfilepath, strTemp );
		CControlLayer::Instance().m_AT.hLogFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	}
}

void CDlgAutoTestMCI::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent >= Timer_FineMagX_seg11 && nIDEvent <=Timer_FineMagX_seg16 )
		TimerChangeFineMagX( nIDEvent );
	else if( nIDEvent >= Timer_FineMagY_seg11 && nIDEvent <=Timer_FineMagY_seg16 )
		TimerChangeFineMagY( nIDEvent );
	else if( nIDEvent >= Timer_MCIShiftX_seg11 && nIDEvent <=Timer_MCIShiftX_seg16 )
		TimerChangeMCIShiftX( nIDEvent );
	else if( nIDEvent >= Timer_MCIShiftY_seg11 && nIDEvent <=Timer_MCIShiftY_seg16 )
		TimerChangeMCIShiftY( nIDEvent );
	else if( nIDEvent >= Timer_FineMagX_seg21 && nIDEvent <=Timer_FineMagX_seg26 )
		TimerChangeFineMagXReverse( nIDEvent );
	else if( nIDEvent >= Timer_FineMagY_seg21 && nIDEvent <=Timer_FineMagY_seg26 )
		TimerChangeFineMagYReverse( nIDEvent );
	else if( nIDEvent >= Timer_MCIShiftX_seg21 && nIDEvent <=Timer_MCIShiftX_seg26 )
		TimerChangeMCIShiftXReverse( nIDEvent );
	else if( nIDEvent >= Timer_MCIShiftY_seg21 && nIDEvent <=Timer_MCIShiftY_seg26 )
		TimerChangeMCIShiftYReverse( nIDEvent );
	CDialog::OnTimer(nIDEvent);
}

void CDlgAutoTestMCI::OnStartFineMagX() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 0 );
		CBoardComm::Instance().ChangedADCsChSelect( 0, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_FineMag_X;
		m_nChnIndex2	= 0;
		m_nSegIndex		= 0;
		m_strRes		= "";
		int nMax = AutoTest_12bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_StigmatorX.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[0] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= FALSE;

		// Analog Shift归零，Digital Shift归零，测试FineMag
		XshiftDACSB_Change( 0 );
		CSemCtrl::Instance().USB_WriteToBoard( USB_SetXoffset, TRUE, 0 );	// Digital Shift X

		int nPos = g_nAutoTestMCI_seg1[m_nOrder];	// 设置起始值
		m_nOrder++;
		Det45SB_Change( nPos, 2 );
		SetTimer(Timer_FineMagX_seg11, Elapse_AutoTest_MCI, NULL);

		EnableParams( m_nSegIndex, FALSE );
	}
	else
	{
		EnableParams( m_nSegIndex, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestMCI::OnStartFineMagY() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_21)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady())
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 0 );
		CBoardComm::Instance().ChangedADCsChSelect( 2, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_FineMag_Y;
		m_nChnIndex2	= 2;
		m_nSegIndex		= 2;
		m_strRes		= "";
		int nMax = AutoTest_12bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_StigmatorY.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[1] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= FALSE;

		// Analog Shift归零，Digital Shift归零，测试FineMag
		YshiftDACSB_Change( 0 );
		CSemCtrl::Instance().USB_WriteToBoard( USB_SetYoffset, TRUE, 0 );	// Digital Shift Y

		int nPos = g_nAutoTestMCI_seg1[m_nOrder];	// 设置起始值
		m_nOrder++;
		Det45SB_Change( nPos, 3 );
		SetTimer(Timer_FineMagY_seg11, Elapse_AutoTest_MCI, NULL);

		EnableParams( m_nSegIndex, FALSE );
	}
	else
	{
		EnableParams( m_nSegIndex, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestMCI::OnStartShiftX() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_31)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 0 );
		CBoardComm::Instance().ChangedADCsChSelect( 0, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Shift_X;
		m_nChnIndex2	= 0;
		m_nSegIndex		= 4;
		m_strRes		= "";
		int nMax = AutoTest_16bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_ScanShiftX.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[2] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= FALSE;

		// FineMag居中，Analog Shift归零，测试Digital Shift
		Det45SB_Change( 2047, 2 );
		XshiftDACSB_Change( 0 );

		int nPos = -32768 + g_nAutoTestMCI_seg7[m_nOrder];	// 设置起始值
		m_nOrder++;
		SegDShiftXSB_Change( nPos );
		SetTimer(Timer_MCIShiftX_seg11, Elapse_AutoTest_MCI, NULL);

		EnableParams( m_nSegIndex, FALSE );
	}
	else
	{
		EnableParams( m_nSegIndex, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestMCI::OnStartShiftY() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_41)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 0 );
		CBoardComm::Instance().ChangedADCsChSelect( 2, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Shift_Y;
		m_nChnIndex2	= 2;
		m_nSegIndex		= 6;
		m_strRes		= "";
		int nMax = AutoTest_16bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_ScanShiftY.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[3] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= FALSE;

		// FineMag居中，Analog Shift归零，测试Digital Shift
		Det45SB_Change( 2047, 3 );
		YshiftDACSB_Change( 0 );

		int nPos = -32768 + g_nAutoTestMCI_seg7[m_nOrder];	// 设置起始值
		m_nOrder++;
		SegDShiftYSB_Change( nPos );
		SetTimer(Timer_MCIShiftY_seg11, Elapse_AutoTest_MCI, NULL);

		EnableParams( m_nSegIndex, FALSE );
	}
	else
	{
		EnableParams( m_nSegIndex, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestMCI::OnStartFineMagX2() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_12)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 0 );
		CBoardComm::Instance().ChangedADCsChSelect( 0, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_FineMag_X;
		m_nChnIndex2	= 0;
		m_nSegIndex		= 1;
		m_strRes		= "";
		int nMax = AutoTest_12bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_StigmatorX.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[0] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= TRUE;

		// Analog Shift归零，Digital Shift归零，测试FineMag
		XshiftDACSB_Change( 0 );
		CSemCtrl::Instance().USB_WriteToBoard( USB_SetXoffset, TRUE, 0 );	// Digital Shift X

		int nPos = g_nAutoTestMCI_seg4[m_nOrder];	// 设置起始值
		m_nOrder++;
		Det45SB_Change( nPos, 2 );
		SetTimer(Timer_FineMagX_seg21, Elapse_AutoTest_MCI, NULL);

		EnableParams( m_nSegIndex, FALSE );
	}
	else
	{
		EnableParams( m_nSegIndex, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestMCI::OnStartFineMagY2() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_22)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 0 );
		CBoardComm::Instance().ChangedADCsChSelect( 2, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_FineMag_Y;
		m_nChnIndex2	= 2;
		m_nSegIndex		= 3;
		m_strRes		= "";
		int nMax = AutoTest_12bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_StigmatorY.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[1] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= TRUE;

		// Analog Shift归零，Digital Shift归零，测试FineMag
		YshiftDACSB_Change( 0 );
		CSemCtrl::Instance().USB_WriteToBoard( USB_SetYoffset, TRUE, 0 );	// Digital Shift Y

		int nPos = g_nAutoTestMCI_seg4[m_nOrder];	// 设置起始值
		m_nOrder++;
		Det45SB_Change( nPos, 3 );
		SetTimer(Timer_FineMagY_seg21, Elapse_AutoTest_MCI, NULL);

		EnableParams( m_nSegIndex, FALSE );
	}
	else
	{
		EnableParams( m_nSegIndex, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestMCI::OnStartShiftX2() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_32)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 0 );
		CBoardComm::Instance().ChangedADCsChSelect( 0, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Shift_X;
		m_nChnIndex2	= 0;
		m_nSegIndex		= 5;
		m_strRes		= "";
		int nMax = AutoTest_16bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_ScanShiftX.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[2] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= TRUE;

		// FineMag居中，Analog Shift归零，测试Digital Shift
		Det45SB_Change( 2047, 2 );
		XshiftDACSB_Change( 0 );

		int nPos = -32768 + g_nAutoTestMCI_seg10[m_nOrder];	// 设置起始值
		m_nOrder++;
		SegDShiftXSB_Change( nPos );
		SetTimer(Timer_MCIShiftX_seg21, Elapse_AutoTest_MCI, NULL);

		EnableParams( m_nSegIndex, FALSE );
	}
	else
	{
		EnableParams( m_nSegIndex, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestMCI::OnStartShiftY2() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_42)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		CBoardComm::Instance().ChangedADCselectOB( 0 );
		CBoardComm::Instance().ChangedADCsChSelect( 2, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_Shift_Y;
		m_nChnIndex2	= 2;
		m_nSegIndex		= 7;
		m_strRes		= "";
		int nMax = AutoTest_16bitDAC_Steps;
		if( CControlLayer::Instance().m_DL.other.bDebug )
			nMax *= 3;
		for( int i=0; i<nMax; i++ )
		{
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][i] = 0;
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][i] = 0;
		}
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1] = 0;
		CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_ScanShiftY.GetPos();
		CControlLayer::Instance().m_AT.nAutoTestMCI_OldPos[3] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= TRUE;

		// FineMag居中，Analog Shift归零，测试Digital Shift
		Det45SB_Change( 2047, 3 );
		YshiftDACSB_Change( 0 );

		int nPos = -32768 + g_nAutoTestMCI_seg10[m_nOrder];	// 设置起始值
		m_nOrder++;
		SegDShiftYSB_Change( nPos );
		SetTimer(Timer_MCIShiftY_seg21, Elapse_AutoTest_MCI, NULL);

		EnableParams( m_nSegIndex, FALSE );
	}
	else
	{
		EnableParams( m_nSegIndex, TRUE );
		// 恢复真空状态机的工作
		if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestMCI::TimerChangeFineMagX( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_FineMagX_seg11 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_FineMagX_seg11);

			m_nOrder = sizeof(g_nAutoTestMCI_seg1) / sizeof(int) - 1;
			// 设置起始值
			nPos = g_nAutoTestMCI_seg1[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 2 );
			SetTimer(Timer_FineMagX_seg12, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg1[m_nOrder];
			m_nOrder++;
			Det45SB_Change( nPos, 2 );
		}
	}
	else if( nIDEvent == Timer_FineMagX_seg12 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_FineMagX_seg12);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg1[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 2 );
				SetTimer(Timer_FineMagX_seg11, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg2[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 2 );
				SetTimer(Timer_FineMagX_seg13, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg1[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 2 );
		}
	}
	else if( nIDEvent == Timer_FineMagX_seg13 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_FineMagX_seg13);

			m_nOrder = sizeof(g_nAutoTestMCI_seg2) / sizeof(int) - 1;
			// 设置起始值
			nPos = g_nAutoTestMCI_seg2[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 2 );
			SetTimer(Timer_FineMagX_seg14, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg2[m_nOrder];
			m_nOrder++;
			Det45SB_Change( nPos, 2 );
		}
	}
	else if( nIDEvent == Timer_FineMagX_seg14 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_FineMagX_seg14);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg2[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 2 );
				SetTimer(Timer_FineMagX_seg13, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg3[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 2 );
				SetTimer(Timer_FineMagX_seg15, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg2[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 2 );
		}
	}
	else if( nIDEvent == Timer_FineMagX_seg15 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_FineMagX_seg15);

			m_nOrder = sizeof(g_nAutoTestMCI_seg3) / sizeof(int) - 1;
			// 设置起始值
			nPos = g_nAutoTestMCI_seg3[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 2 );
			SetTimer(Timer_FineMagX_seg16, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg3[m_nOrder];
			m_nOrder++;
			Det45SB_Change( nPos, 2 );
		}
	}
	else if( nIDEvent == Timer_FineMagX_seg16 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_FineMagX_seg16);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg3[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 2 );
				SetTimer(Timer_FineMagX_seg15, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				EnableParams( m_nSegIndex, TRUE );
				// 恢复真空状态机的工作
				if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
					CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg3[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 2 );
		}
	}
}

void CDlgAutoTestMCI::TimerChangeFineMagY( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_FineMagY_seg11 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_FineMagY_seg11);

			m_nOrder = sizeof(g_nAutoTestMCI_seg1) / sizeof(int) - 1;
			// 设置起始值
			nPos = g_nAutoTestMCI_seg1[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 3 );
			SetTimer(Timer_FineMagY_seg12, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg1[m_nOrder];
			m_nOrder++;
			Det45SB_Change( nPos, 3 );
		}
	}
	else if( nIDEvent == Timer_FineMagY_seg12 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_FineMagY_seg12);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg1[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 3 );
				SetTimer(Timer_FineMagY_seg11, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg2[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 3 );
				SetTimer(Timer_FineMagY_seg13, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg1[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 3 );
		}
	}
	else if( nIDEvent == Timer_FineMagY_seg13 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_FineMagY_seg13);

			m_nOrder = sizeof(g_nAutoTestMCI_seg2) / sizeof(int) - 1;
			// 设置起始值
			nPos = g_nAutoTestMCI_seg2[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 3 );
			SetTimer(Timer_FineMagY_seg14, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg2[m_nOrder];
			m_nOrder++;
			Det45SB_Change( nPos, 3 );
		}
	}
	else if( nIDEvent == Timer_FineMagY_seg14 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_FineMagY_seg14);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg2[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 3 );
				SetTimer(Timer_FineMagY_seg13, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg3[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 3 );
				SetTimer(Timer_FineMagY_seg15, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg2[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 3 );
		}
	}
	else if( nIDEvent == Timer_FineMagY_seg15 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_FineMagY_seg15);

			m_nOrder = sizeof(g_nAutoTestMCI_seg3) / sizeof(int) - 1;
			// 设置起始值
			nPos = g_nAutoTestMCI_seg3[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 3 );
			SetTimer(Timer_FineMagY_seg16, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg3[m_nOrder];
			m_nOrder++;
			Det45SB_Change( nPos, 3 );
		}
	}
	else if( nIDEvent == Timer_FineMagY_seg16 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_FineMagY_seg16);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg3[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 3 );
				SetTimer(Timer_FineMagY_seg15, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				EnableParams( m_nSegIndex, TRUE );
				// 恢复真空状态机的工作
				if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
					CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg3[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 3 );
		}
	}
}

void CDlgAutoTestMCI::TimerChangeMCIShiftX( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_MCIShiftX_seg11 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_MCIShiftX_seg11);

			m_nOrder = sizeof(g_nAutoTestMCI_seg7) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestMCI_seg7[m_nOrder];
			m_nOrder--;
			SegDShiftXSB_Change( nPos );
			SetTimer(Timer_MCIShiftX_seg12, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg7[m_nOrder];
			m_nOrder++;
			SegDShiftXSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftX_seg12 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_MCIShiftX_seg12);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg7[m_nOrder];
				m_nOrder++;
				SegDShiftXSB_Change( nPos );
				SetTimer(Timer_MCIShiftX_seg11, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg8[m_nOrder];
				m_nOrder++;
				SegDShiftXSB_Change( nPos );
				SetTimer(Timer_MCIShiftX_seg13, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg7[m_nOrder];
			m_nOrder--;
			SegDShiftXSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftX_seg13 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_MCIShiftX_seg13);

			m_nOrder = sizeof(g_nAutoTestMCI_seg8) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestMCI_seg8[m_nOrder];
			m_nOrder--;
			SegDShiftXSB_Change( nPos );
			SetTimer(Timer_MCIShiftX_seg14, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg8[m_nOrder];
			m_nOrder++;
			SegDShiftXSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftX_seg14 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_MCIShiftX_seg14);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg8[m_nOrder];
				m_nOrder++;
				SegDShiftXSB_Change( nPos );
				SetTimer(Timer_MCIShiftX_seg13, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg9[m_nOrder];
				m_nOrder++;
				SegDShiftXSB_Change( nPos );
				SetTimer(Timer_MCIShiftX_seg15, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg8[m_nOrder];
			m_nOrder--;
			SegDShiftXSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftX_seg15 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_MCIShiftX_seg15);

			m_nOrder = sizeof(g_nAutoTestMCI_seg9) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestMCI_seg9[m_nOrder];
			m_nOrder--;
			SegDShiftXSB_Change( nPos );
			SetTimer(Timer_MCIShiftX_seg16, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg9[m_nOrder];
			m_nOrder++;
			SegDShiftXSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftX_seg16 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_MCIShiftX_seg16);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg9[m_nOrder];
				m_nOrder++;
				SegDShiftXSB_Change( nPos );
				SetTimer(Timer_MCIShiftX_seg15, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				EnableParams( m_nSegIndex, TRUE );
				// 恢复真空状态机的工作
				if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
					CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg9[m_nOrder];
			m_nOrder--;
			SegDShiftXSB_Change( nPos );
		}
	}
}

void CDlgAutoTestMCI::TimerChangeMCIShiftY( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_MCIShiftY_seg11 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_MCIShiftY_seg11);

			m_nOrder = sizeof(g_nAutoTestMCI_seg7) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestMCI_seg7[m_nOrder];
			m_nOrder--;
			SegDShiftYSB_Change( nPos );
			SetTimer(Timer_MCIShiftY_seg12, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg7[m_nOrder];
			m_nOrder++;
			SegDShiftYSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftY_seg12 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_MCIShiftY_seg12);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg7[m_nOrder];
				m_nOrder++;
				SegDShiftYSB_Change( nPos );
				SetTimer(Timer_MCIShiftY_seg11, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg8[m_nOrder];
				m_nOrder++;
				SegDShiftYSB_Change( nPos );
				SetTimer(Timer_MCIShiftY_seg13, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg7[m_nOrder];
			m_nOrder--;
			SegDShiftYSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftY_seg13 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_MCIShiftY_seg13);

			m_nOrder = sizeof(g_nAutoTestMCI_seg8) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestMCI_seg8[m_nOrder];
			m_nOrder--;
			SegDShiftYSB_Change( nPos );
			SetTimer(Timer_MCIShiftY_seg14, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg8[m_nOrder];
			m_nOrder++;
			SegDShiftYSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftY_seg14 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_MCIShiftY_seg14);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg8[m_nOrder];
				m_nOrder++;
				SegDShiftYSB_Change( nPos );
				SetTimer(Timer_MCIShiftY_seg13, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg9[m_nOrder];
				m_nOrder++;
				SegDShiftYSB_Change( nPos );
				SetTimer(Timer_MCIShiftY_seg15, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg8[m_nOrder];
			m_nOrder--;
			SegDShiftYSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftY_seg15 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_MCIShiftY_seg15);

			m_nOrder = sizeof(g_nAutoTestMCI_seg9) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestMCI_seg9[m_nOrder];
			m_nOrder--;
			SegDShiftYSB_Change( nPos );
			SetTimer(Timer_MCIShiftY_seg16, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg9[m_nOrder];
			m_nOrder++;
			SegDShiftYSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftY_seg16 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_MCIShiftY_seg16);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg9[m_nOrder];
				m_nOrder++;
				SegDShiftYSB_Change( nPos );
				SetTimer(Timer_MCIShiftY_seg15, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				EnableParams( m_nSegIndex, TRUE );
				// 恢复真空状态机的工作
				if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
					CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg9[m_nOrder];
			m_nOrder--;
			SegDShiftYSB_Change( nPos );
		}
	}
}

void CDlgAutoTestMCI::TimerChangeFineMagXReverse( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_FineMagX_seg21 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_FineMagX_seg21);

			m_nOrder = sizeof(g_nAutoTestMCI_seg4) / sizeof(int) - 1;
			// 设置起始值
			nPos = g_nAutoTestMCI_seg4[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 2 );
			SetTimer(Timer_FineMagX_seg22, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg4[m_nOrder];
			m_nOrder++;
			Det45SB_Change( nPos, 2 );
		}
	}
	else if( nIDEvent == Timer_FineMagX_seg22 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_FineMagX_seg22);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg4[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 2 );
				SetTimer(Timer_FineMagX_seg21, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg5[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 2 );
				SetTimer(Timer_FineMagX_seg23, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg4[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 2 );
		}
	}
	else if( nIDEvent == Timer_FineMagX_seg23 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_FineMagX_seg23);

			m_nOrder = sizeof(g_nAutoTestMCI_seg5) / sizeof(int) - 1;
			// 设置起始值
			nPos = g_nAutoTestMCI_seg5[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 2 );
			SetTimer(Timer_FineMagX_seg24, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg5[m_nOrder];
			m_nOrder++;
			Det45SB_Change( nPos, 2 );
		}
	}
	else if( nIDEvent == Timer_FineMagX_seg24 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log  );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_FineMagX_seg24);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg5[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 2 );
				SetTimer(Timer_FineMagX_seg23, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg6[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 2 );
				SetTimer(Timer_FineMagX_seg25, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg5[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 2 );
		}
	}
	else if( nIDEvent == Timer_FineMagX_seg25 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_FineMagX_seg25);

			m_nOrder = sizeof(g_nAutoTestMCI_seg6) / sizeof(int) - 1;
			// 设置起始值
			nPos = g_nAutoTestMCI_seg6[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 2 );
			SetTimer(Timer_FineMagX_seg26, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg6[m_nOrder];
			m_nOrder++;
			Det45SB_Change( nPos, 2 );
		}
	}
	else if( nIDEvent == Timer_FineMagX_seg26 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_FineMagX_seg26);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg6[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 2 );
				SetTimer(Timer_FineMagX_seg25, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				EnableParams( m_nSegIndex, TRUE );
				// 恢复真空状态机的工作
				if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
					CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg6[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 2 );
		}
	}
}

void CDlgAutoTestMCI::TimerChangeFineMagYReverse( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_FineMagY_seg21 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_FineMagY_seg21);

			m_nOrder = sizeof(g_nAutoTestMCI_seg4) / sizeof(int) - 1;
			// 设置起始值
			nPos = g_nAutoTestMCI_seg4[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 3 );
			SetTimer(Timer_FineMagY_seg22, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg4[m_nOrder];
			m_nOrder++;
			Det45SB_Change( nPos, 3 );
		}
	}
	else if( nIDEvent == Timer_FineMagY_seg22 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_FineMagY_seg22);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg4[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 3 );
				SetTimer(Timer_FineMagY_seg21, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg5[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 3 );
				SetTimer(Timer_FineMagY_seg23, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg4[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 3 );
		}
	}
	else if( nIDEvent == Timer_FineMagY_seg23 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_FineMagY_seg23);

			m_nOrder = sizeof(g_nAutoTestMCI_seg5) / sizeof(int) - 1;
			// 设置起始值
			nPos = g_nAutoTestMCI_seg5[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 3 );
			SetTimer(Timer_FineMagY_seg24, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg5[m_nOrder];
			m_nOrder++;
			Det45SB_Change( nPos, 3 );
		}
	}
	else if( nIDEvent == Timer_FineMagY_seg24 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_FineMagY_seg24);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg5[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 3 );
				SetTimer(Timer_FineMagY_seg23, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg6[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 3 );
				SetTimer(Timer_FineMagY_seg25, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg5[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 3 );
		}
	}
	else if( nIDEvent == Timer_FineMagY_seg25 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_12bitDAC_StepsEach )
		{
			KillTimer(Timer_FineMagY_seg25);

			m_nOrder = sizeof(g_nAutoTestMCI_seg6) / sizeof(int) - 1;
			// 设置起始值
			nPos = g_nAutoTestMCI_seg6[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 3 );
			SetTimer(Timer_FineMagY_seg26, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg6[m_nOrder];
			m_nOrder++;
			Det45SB_Change( nPos, 3 );
		}
	}
	else if( nIDEvent == Timer_FineMagY_seg26 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_FineMagY_seg26);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = g_nAutoTestMCI_seg6[m_nOrder];
				m_nOrder++;
				Det45SB_Change( nPos, 3 );
				SetTimer(Timer_FineMagY_seg25, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				EnableParams( m_nSegIndex, TRUE );
				// 恢复真空状态机的工作
				if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
					CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = g_nAutoTestMCI_seg6[m_nOrder];
			m_nOrder--;
			Det45SB_Change( nPos, 3 );
		}
	}
}

void CDlgAutoTestMCI::TimerChangeMCIShiftXReverse( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_MCIShiftX_seg21 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_MCIShiftX_seg21);

			m_nOrder = sizeof(g_nAutoTestMCI_seg10) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestMCI_seg10[m_nOrder];
			m_nOrder--;
			SegDShiftXSB_Change( nPos );
			SetTimer(Timer_MCIShiftX_seg22, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg10[m_nOrder];
			m_nOrder++;
			SegDShiftXSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftX_seg22 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_MCIShiftX_seg22);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg10[m_nOrder];
				m_nOrder++;
				SegDShiftXSB_Change( nPos );
				SetTimer(Timer_MCIShiftX_seg21, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg11[m_nOrder];
				m_nOrder++;
				SegDShiftXSB_Change( nPos );
				SetTimer(Timer_MCIShiftX_seg23, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg10[m_nOrder];
			m_nOrder--;
			SegDShiftXSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftX_seg23 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_MCIShiftX_seg23);

			m_nOrder = sizeof(g_nAutoTestMCI_seg11) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestMCI_seg11[m_nOrder];
			m_nOrder--;
			SegDShiftXSB_Change( nPos );
			SetTimer(Timer_MCIShiftX_seg24, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg11[m_nOrder];
			m_nOrder++;
			SegDShiftXSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftX_seg24 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_MCIShiftX_seg24);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg11[m_nOrder];
				m_nOrder++;
				SegDShiftXSB_Change( nPos );
				SetTimer(Timer_MCIShiftX_seg23, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg12[m_nOrder];
				m_nOrder++;
				SegDShiftXSB_Change( nPos );
				SetTimer(Timer_MCIShiftX_seg25, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg11[m_nOrder];
			m_nOrder--;
			SegDShiftXSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftX_seg25 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_MCIShiftX_seg25);

			m_nOrder = sizeof(g_nAutoTestMCI_seg12) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestMCI_seg12[m_nOrder];
			m_nOrder--;
			SegDShiftXSB_Change( nPos );
			SetTimer(Timer_MCIShiftX_seg26, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg12[m_nOrder];
			m_nOrder++;
			SegDShiftXSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftX_seg26 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_MCIShiftX_seg26);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg12[m_nOrder];
				m_nOrder++;
				SegDShiftXSB_Change( nPos );
				SetTimer(Timer_MCIShiftX_seg25, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				EnableParams( m_nSegIndex, TRUE );
				// 恢复真空状态机的工作
				if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
					CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg12[m_nOrder];
			m_nOrder--;
			SegDShiftXSB_Change( nPos );
		}
	}
}

void CDlgAutoTestMCI::TimerChangeMCIShiftYReverse( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_MCIShiftY_seg21 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_MCIShiftY_seg21);

			m_nOrder = sizeof(g_nAutoTestMCI_seg10) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestMCI_seg10[m_nOrder];
			m_nOrder--;
			SegDShiftYSB_Change( nPos );
			SetTimer(Timer_MCIShiftY_seg22, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg10[m_nOrder];
			m_nOrder++;
			SegDShiftYSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftY_seg22 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0];
			CControlLayer::Instance().m_AT.pAutoTestMCI[0][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_MCIShiftY_seg22);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg10[m_nOrder];
				m_nOrder++;
				SegDShiftYSB_Change( nPos );
				SetTimer(Timer_MCIShiftY_seg21, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg11[m_nOrder];
				m_nOrder++;
				SegDShiftYSB_Change( nPos );
				SetTimer(Timer_MCIShiftY_seg23, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg10[m_nOrder];
			m_nOrder--;
			SegDShiftYSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftY_seg23 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_MCIShiftY_seg23);

			m_nOrder = sizeof(g_nAutoTestMCI_seg11) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestMCI_seg11[m_nOrder];
			m_nOrder--;
			SegDShiftYSB_Change( nPos );
			SetTimer(Timer_MCIShiftY_seg24, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg11[m_nOrder];
			m_nOrder++;
			SegDShiftYSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftY_seg24 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1];
			CControlLayer::Instance().m_AT.pAutoTestMCI[1][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_MCIShiftY_seg24);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg11[m_nOrder];
				m_nOrder++;
				SegDShiftYSB_Change( nPos );
				SetTimer(Timer_MCIShiftY_seg23, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg12[m_nOrder];
				m_nOrder++;
				SegDShiftYSB_Change( nPos );
				SetTimer(Timer_MCIShiftY_seg25, Elapse_AutoTest_MCI, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg11[m_nOrder];
			m_nOrder--;
			SegDShiftYSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftY_seg25 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_MCIShiftY_seg25);

			m_nOrder = sizeof(g_nAutoTestMCI_seg12) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 + g_nAutoTestMCI_seg12[m_nOrder];
			m_nOrder--;
			SegDShiftYSB_Change( nPos );
			SetTimer(Timer_MCIShiftY_seg26, Elapse_AutoTest_MCI, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg12[m_nOrder];
			m_nOrder++;
			SegDShiftYSB_Change( nPos );
		}
	}
	else if( nIDEvent == Timer_MCIShiftY_seg26 )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( CControlLayer::Instance().m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging_onetime( 80, dValue, CControlLayer::Instance().m_DL.adc.chn, TRUE );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
			nPos = CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2];
			CControlLayer::Instance().m_AT.pAutoTestMCI[2][nPos] = dValue;
			CControlLayer::Instance().m_AT.nAutoTestMCI_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_MCIShiftY_seg26);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 + g_nAutoTestMCI_seg12[m_nOrder];
				m_nOrder++;
				SegDShiftYSB_Change( nPos );
				SetTimer(Timer_MCIShiftY_seg25, Elapse_AutoTest_MCI, NULL);
			}
			else
			{
				EnableParams( m_nSegIndex, TRUE );
				// 恢复真空状态机的工作
				if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
					CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 + g_nAutoTestMCI_seg12[m_nOrder];
			m_nOrder--;
			SegDShiftYSB_Change( nPos );
		}
	}
}


void CDlgAutoTestMCI::Det45SB_Change( int nPos, int nChn )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos;
	byteLo	= (BYTE)( nValue & 0x00FF );
	byteHi	= (BYTE)( nValue >> 8 );

	WORD wValue = byteLo;
	wValue |= (((byteHi & 0xF) | (nChn << 6)) << 8);
	CSemCtrl::Instance().USB_WriteToBoard( USB_Det45StoreDAC, TRUE, wValue );
}

void CDlgAutoTestMCI::SegDShiftXSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	// 目前使用14bit DAC
/*	int nMin, nMax;
	m_scrollSegDShiftX.GetScrollRange( &nMin, &nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;

	double dValue = 0.0;
	CString str = "";
	str.Format( "%04X", nPos -nMin );
	GetDlgItem( IDC_EDIT_Seg_DShift_X )->SetWindowText( str );
	UpdateOffsetX();*/
}

void CDlgAutoTestMCI::SegDShiftYSB_Change( int nPos )
{
	// This Sub uses global long DIOData (8 hex digits) as AccesIO USB write data
	// 目前使用14bit DAC
/*	int nMin, nMax;
	m_scrollSegDShiftX.GetScrollRange( &nMin, &nMax );
	if( nPos < nMin )
		nPos = nMin;
	if( nPos > nMax )
		nPos = nMax;

	double dValue = 0.0;
	CString str = "";
	str.Format( "%04X", nPos -nMin );
	GetDlgItem( IDC_EDIT_Seg_DShift_Y )->SetWindowText( str );
	UpdateOffsetY();*/
}
/*
int CDlgAutoTestMCI::UpdateOffsetX()
{
	// 目前使用14bit DAC，所以最大值为16384，中心位置是8192
	// Offset	= 8192 +DShift - Rise * dRise /2
	// Center	= 8192 +DShift;
	// Max		= 8192 +DShift + Rise * dRise /2
	int nMin, nMax;
	m_scrollSegDShiftX.GetScrollRange( &nMin, &nMax );
	int nDShift	= m_scrollSegDShiftX.GetScrollPos() -nMin;
	int nRise	= m_scrollSegRiseX.GetScrollPos();
	int nDRise	= m_scrollSegDRiseX.GetScrollPos();
	CString str;
	int nOffset	= g_n16bitDAC_Mid + nDShift - nRise *nDRise /2;
	str.Format( "%04X,%04X", nOffset, nOffset << 2 );
	GetDlgItem( IDC_EDIT_Seg_Offset_X )->SetWindowText( str );
	str.Format( "%04X", g_n16bitDAC_Mid + nDShift );
	GetDlgItem( IDC_EDIT_Seg_CenterX )->SetWindowText( str );
	str.Format( "%04X", g_n16bitDAC_Mid + nDShift + nRise *nDRise /2 -1 );
	GetDlgItem( IDC_EDIT_Seg_MaxX )->SetWindowText( str );
	return nOffset;
}

int CDlgAutoTestMCI::UpdateOffsetY()
{
	// 目前使用14bit DAC，所以最大值为16384，中心位置是8192
	// Offset	= 8192 +DShift - Rise * dRise /2
	// Center	= 8192 +DShift;
	// Max		= 8192 +DShift + Rise * dRise /2
	int nMin, nMax;
	m_scrollSegDShiftY.GetScrollRange( &nMin, &nMax );
	int nDShift	= m_scrollSegDShiftY.GetScrollPos() -nMin;
	int nRise	= m_scrollSegRiseY.GetScrollPos();
	int nDRise	= m_scrollSegDRiseY.GetScrollPos();
	CString str;
	int nOffset	= g_n16bitDAC_Mid + nDShift - nRise *nDRise /2;
	str.Format( "%04X,%04X", nOffset, nOffset << 2 );
	GetDlgItem( IDC_EDIT_Seg_Offset_Y )->SetWindowText( str );
	str.Format( "%04X", g_n16bitDAC_Mid + nDShift );
	GetDlgItem( IDC_EDIT_Seg_CenterY )->SetWindowText( str );
	str.Format( "%04X", g_n16bitDAC_Mid + nDShift + nRise *nDRise /2 -1 );
	GetDlgItem( IDC_EDIT_Seg_MaxY )->SetWindowText( str );
	return nOffset;
}
*/
void CDlgAutoTestMCI::XshiftDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if X shift.

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Xshift_Crossover,	// Serial Comm operand low byte (X shift/crossover DAC circuit address)
										USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
										byteLo,		// Serial Comm operand low byte
										byteHi );	// Serial Comm operand high byte
}

void CDlgAutoTestMCI::YshiftDACSB_Change( int nPos )
{
	int nValue;
	BYTE byteHi, byteLo;

	nValue = nPos + 2048;
	byteLo = (BYTE)( nValue & 0x00FF );
	byteHi = (BYTE)( nValue >> 8 );		//Or &H80 (MSB = 1 if crossover), MSB = 0 if Y shift.

	// Write via MCI to SCN board.  First write 0x21 (16 bit write to Board 1), low byte = 0x12 (XY Stig DAC)
	CSemCtrl::Instance().USB_CommToBoards2(	USB_CID_SCN_Yshift_Crossover,	// Serial Comm operand low byte (Y shift/crossover DAC circuit address)
										USB_TM_SCN_Write_16,			// Serial Comm operand high byte (16 bit write to SCN board)
										byteLo,		// Serial Comm operand low byte
										byteHi );	// Serial Comm operand high byte
}