// DlgAutoTestSPR.cpp : implementation file
//

#include "stdafx.h"
#include "AutoTest.h"
#include "DlgAutoTestSPR.h"
#include "SMVac.h"
#include "SemCoreAssemble.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	Timer_Contrast_seg11		1
#define Timer_Contrast_seg12		2
#define Timer_Contrast_seg13		3
#define Timer_Contrast_seg14		4
#define Timer_Contrast_seg15		5
#define Timer_Contrast_seg16		6

#define	Timer_Contrast_seg21		7
#define Timer_Contrast_seg22		8
#define Timer_Contrast_seg23		9
#define Timer_Contrast_seg24		10
#define Timer_Contrast_seg25		11
#define Timer_Contrast_seg26		12

#define Elapse_AutoTest_SPR	1000

int g_nAutoTestSPR_seg1[] = { 0x8001, 0x8002, 0x8004, 0x8008, 0x8010, 0x8020 };
int g_nAutoTestSPR_seg2[] = { 0x8020, 0x8040, 0x8080, 0x8100, 0x8200, 0x8400 };
int g_nAutoTestSPR_seg3[] = { 0x8400, 0x8800, 0x9000, 0xA000, 0xC000, 0xFFFF };

int g_nAutoTestSPR_seg4[] = { 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400 };
int g_nAutoTestSPR_seg5[] = { 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020 };
int g_nAutoTestSPR_seg6[] = { 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001 };

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestSPR dialog


CDlgAutoTestSPR::CDlgAutoTestSPR(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoTestSPR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoTestSPR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nOrder		= -1;
	m_nChnIndex1	= -1;	// 1为记录位置数组的索引
	m_nChnIndex2	= -1;	// 2为6ADC通道索引
	m_nSegIndex		= -1;	// 在做哪个功能的AutoTest
	m_bContinue		= TRUE;
	m_bReverse		= FALSE;
}


void CDlgAutoTestSPR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoTestSPR)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutoTestSPR, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoTestSPR)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_SaveGraph, OnSaveGraph)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_11, OnStartContrast)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_12, OnStartContrast2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestSPR message handlers

void CDlgAutoTestSPR::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	int i;
	for( i=0; i<3; i++ )
		PaintGraph( &dc, i );

	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgAutoTestSPR::PaintGraph( CDC* pDC, int nIndex, BOOL bErase )
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

	if( theApp.m_AT.nAutoTestSPR_Total[nIndex] < 2 )
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
	nTotal = theApp.m_AT.nAutoTestSPR_Total[nIndex];

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
		dYMin = (theApp.m_AT.pAutoTestSPR[nIndex])[xMin];
		for( i=xMin+1; i<xMax; i++ )
		{
			if( theApp.m_AT.pAutoTestSPR[nIndex][i] < dYMin )
				dYMin = theApp.m_AT.pAutoTestSPR[nIndex][i];
		}
//		dYMinLog = log(dYMin) / log(2);
	}
	{
		dYMax = (theApp.m_AT.pAutoTestSPR[nIndex])[xMin];
		for( i=xMin+1; i<xMax; i++ )
		{
			if( theApp.m_AT.pAutoTestSPR[nIndex][i] > dYMax )
				dYMax = theApp.m_AT.pAutoTestSPR[nIndex][i];
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
		y = (int)(rc1.top + ( nHeight - (theApp.m_AT.pAutoTestSPR[nIndex][xMin] -dYMin) / (dYMax-dYMin) * nHeight ));
//		dY		= theApp.m_AT.pAutoTestSPR[nIndex][xMin];
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
			y = (int)(rc1.top + ( nHeight - (theApp.m_AT.pAutoTestSPR[nIndex][i] -dYMin) / (dYMax-dYMin) * nHeight ));
//			dY		= theApp.m_AT.pAutoTestSPR[nIndex][i];
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
	double dValue = theApp.GetPhysicsFromAdcs(5*16 +m_nChnIndex2, dYMin);
	str.Format( "%.6f ( %.6f )", dYMin, dValue );
	pDC->TextOut( rc1.left -8, rc1.bottom -sz.cy/2, str );
	// 标注纵向最大值
	dValue = theApp.GetPhysicsFromAdcs(5*16 +m_nChnIndex2, dYMax);
	str.Format( "%.6f ( %.6f )", dYMax, dValue );
	pDC->TextOut( rc1.left -8, rc1.top -sz.cy/2, str );
	// 画标签
	int* pn;
	int nMax;
	if( m_nSegIndex % 2 > 0 )
	{
		switch( nIndex )
		{
		case 0:
			pn = g_nAutoTestSPR_seg4;
			break;
		case 1:
			pn = g_nAutoTestSPR_seg5;
			break;
		case 2:
			pn = g_nAutoTestSPR_seg6;
			break;
		}
	}
	else
	{
		switch( nIndex )
		{
		case 0:
			pn = g_nAutoTestSPR_seg1;
			break;
		case 1:
			pn = g_nAutoTestSPR_seg2;
			break;
		case 2:
			pn = g_nAutoTestSPR_seg3;
			break;
		}
	}
	nMax = AutoTest_16bitDAC_StepsEach -1;
	str.Format( "%s [%04X---%04X]", theApp.m_DL.adc.chn[5*16 +m_nChnIndex2].strLabel, pn[0], pn[nMax] );
	sz = pDC->GetTextExtent( str );
	pDC->TextOut( rc.left + rc.Width()/2 - sz.cx/2, rc.top +11, str );
	pDC->SetBkMode(noldBKmode);

	pen.DeleteObject();
	penGrid.DeleteObject();
}

void CDlgAutoTestSPR::OnSaveGraph() 
{
	// TODO: Add your control notification handler code here
	CString filename;
	char szFilter[] = "BMP file|*.bmp|JPG file|*.jpg|PNG file|*.png|TIF file|*.tif|PCX file|*.pcx|TGA file|*.tga||";
	CFileDialog dlgFile(FALSE, "PNG", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
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
/*
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
*/
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
	str.Format( "AutoTest SPR" );
	bmpDC.TextOut( 10, 0, str );
	for(int i=0; i<3; i++ )
		PaintGraph( &bmpDC, i, TRUE );
	//恢复设备环境窗口原有的原始点
	bmpDC.SetWindowOrg( point );
	Invalidate( FALSE );
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// 第二步生产bmp图像
//	CxImage *newima = new CxImage();
//	newima->CreateFromHBITMAP( (HBITMAP)bitmap );
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// 第三步存为文件
//	bool retval = newima->Save(filename, type);
	///////////////////////////////////////////////////////
	bmpDC.SelectObject( poldBmp );
	delete &bitmap;
//	delete newima;
}

void CDlgAutoTestSPR::InitParams()
{
}

void CDlgAutoTestSPR::EnableParams( int nIndex, BOOL bEnable )
{
	int i;
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
			// Contrast
			ClassID.lVal = SemControlPanelClass_Contrast;
			Flag.lVal	= 1;
			var.lVal	= theApp.m_AT.nAutoTestSPR_OldPos[0];
			Xpos		= var;

			nTimerIndex = Timer_Contrast_seg11;
			break;
		case	1:
			// Contrast
			ClassID.lVal = SemControlPanelClass_Contrast;
			Flag.lVal	= 1;
			var.lVal	= theApp.m_AT.nAutoTestSPR_OldPos[0];
			Xpos		= var;

			nTimerIndex = Timer_Contrast_seg21;
			break;
		}

		// 停止定时器
		for( i=0; i<6; i++ )
			KillTimer( nTimerIndex +i);
		// 设置原始值
		theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		// 恢复按钮状态
		for( i=0; i<2; i++ )
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
		for( i=0; i<2; i++ )
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
		str.Format( "%s\\Log\\AutoTestSPR-%s.txt", appfilepath, strTemp );
		theApp.m_AT.hLogFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		unsigned long cbRet;
		char Buf[1024];
		sprintf( Buf, "Auto test for SPR board --- Contrast\n" );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
	}
}

void CDlgAutoTestSPR::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent >= Timer_Contrast_seg11 && nIDEvent <=Timer_Contrast_seg16 )
		TimerChangeContrast( nIDEvent );
	else if( nIDEvent >= Timer_Contrast_seg21 && nIDEvent <=Timer_Contrast_seg26 )
		TimerChangeContrastReverse( nIDEvent );
	CDialog::OnTimer(nIDEvent);
}

void CDlgAutoTestSPR::OnStartContrast() 
{
	// TODO: Add your control notification handler code here
	int i, nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		theApp.ChangedADCselectOB( 5 );
		theApp.ChangedADCsChSelect( 12 );
		theApp.ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_SPRDAC_ChA;
		m_nChnIndex2	= 12;
		m_nSegIndex		= 0;
		m_strRes		= "";
		int nMax = AutoTest_16bitDAC_Steps;
		if( theApp.m_DL.other.bDebug )
			nMax *= 3;
		for( i=0; i<nMax; i++ )
		{
			theApp.m_AT.pAutoTestSPR[0][i] = 0;
			theApp.m_AT.pAutoTestSPR[1][i] = 0;
			theApp.m_AT.pAutoTestSPR[2][i] = 0;
		}
		theApp.m_AT.nAutoTestSPR_Total[0] = 0;
		theApp.m_AT.nAutoTestSPR_Total[1] = 0;
		theApp.m_AT.nAutoTestSPR_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_Contrast.GetPos();
		theApp.m_AT.nAutoTestSPR_OldPos[0] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= FALSE;

		int nPos = -32768 +g_nAutoTestSPR_seg1[m_nOrder];	// 设置起始值
		m_nOrder++;
		theApp.SPR_DACupdate( nPos );
		SetTimer(Timer_Contrast_seg11, Elapse_AutoTest_SPR, NULL);

		EnableParams( m_nSegIndex, FALSE );
	}
	else
	{
		EnableParams( m_nSegIndex, TRUE );
		// 恢复真空状态机的工作
		CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestSPR::OnStartContrast2() 
{
	// TODO: Add your control notification handler code here
	int i, nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_12)))->GetCheck();
	if( nCheck )
	{
		// 暂停真空状态机的工作
		if( CSMvac::Instance().isReady() )
			CSMvac::Instance().SetParams( 22, 1 );
		theApp.ChangedADCselectOB( 5 );
		theApp.ChangedADCsChSelect( 12 );
		theApp.ReadSelectedADC(FALSE);

		m_nOrder		= 0;
		m_nChnIndex1	= Diag_SPRDAC_ChA;
		m_nChnIndex2	= 12;
		m_nSegIndex		= 1;
		m_strRes		= "";
		int nMax = AutoTest_16bitDAC_Steps;
		if( theApp.m_DL.other.bDebug )
			nMax *= 3;
		for( i=0; i<nMax; i++ )
		{
			theApp.m_AT.pAutoTestSPR[0][i] = 0;
			theApp.m_AT.pAutoTestSPR[1][i] = 0;
			theApp.m_AT.pAutoTestSPR[2][i] = 0;
		}
		theApp.m_AT.nAutoTestSPR_Total[0] = 0;
		theApp.m_AT.nAutoTestSPR_Total[1] = 0;
		theApp.m_AT.nAutoTestSPR_Total[2] = 0;

		VARIANT var = CSemCoreAssemble::Instance().m_Contrast.GetPos();
		theApp.m_AT.nAutoTestSPR_OldPos[0] = var.lVal;
		m_bContinue = TRUE;
		m_bReverse	= TRUE;

		int nPos = -32768 +g_nAutoTestSPR_seg4[m_nOrder];	// 设置起始值
		m_nOrder++;
		theApp.SPR_DACupdate( nPos );
		SetTimer(Timer_Contrast_seg21, Elapse_AutoTest_SPR, NULL);

		EnableParams( m_nSegIndex, FALSE );
	}
	else
	{
		EnableParams( m_nSegIndex, TRUE );
		// 恢复真空状态机的工作
		CSMvac::Instance().SetParams( 22, 0 );
	}
}

void CDlgAutoTestSPR::TimerChangeContrast( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_Contrast_seg11 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[0];
			theApp.m_AT.pAutoTestSPR[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[0];
			theApp.m_AT.pAutoTestSPR[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSPR_Total[0];
			theApp.m_AT.pAutoTestSPR[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_Contrast_seg11);

			m_nOrder = sizeof(g_nAutoTestSPR_seg1) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 +g_nAutoTestSPR_seg1[m_nOrder];
			m_nOrder--;
			theApp.SPR_DACupdate( nPos );
			SetTimer(Timer_Contrast_seg12, Elapse_AutoTest_SPR, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 +g_nAutoTestSPR_seg1[m_nOrder];
			m_nOrder++;
			theApp.SPR_DACupdate( nPos );
		}
	}
	else if( nIDEvent == Timer_Contrast_seg12 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[0];
			theApp.m_AT.pAutoTestSPR[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[0];
			theApp.m_AT.pAutoTestSPR[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSPR_Total[0];
			theApp.m_AT.pAutoTestSPR[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_Contrast_seg12);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 +g_nAutoTestSPR_seg1[m_nOrder];
				m_nOrder++;
				theApp.SPR_DACupdate( nPos );
				SetTimer(Timer_Contrast_seg11, Elapse_AutoTest_SPR, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 +g_nAutoTestSPR_seg2[m_nOrder];
				m_nOrder++;
				theApp.SPR_DACupdate( nPos );
				SetTimer(Timer_Contrast_seg13, Elapse_AutoTest_SPR, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 +g_nAutoTestSPR_seg1[m_nOrder];
			m_nOrder--;
			theApp.SPR_DACupdate( nPos );
		}
	}
	else if( nIDEvent == Timer_Contrast_seg13 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[1];
			theApp.m_AT.pAutoTestSPR[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[1];
			theApp.m_AT.pAutoTestSPR[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSPR_Total[1];
			theApp.m_AT.pAutoTestSPR[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_Contrast_seg13);

			m_nOrder = sizeof(g_nAutoTestSPR_seg2) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 +g_nAutoTestSPR_seg2[m_nOrder];
			m_nOrder--;
			theApp.SPR_DACupdate( nPos );
			SetTimer(Timer_Contrast_seg14, Elapse_AutoTest_SPR, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 +g_nAutoTestSPR_seg2[m_nOrder];
			m_nOrder++;
			theApp.SPR_DACupdate( nPos );
		}
	}
	else if( nIDEvent == Timer_Contrast_seg14 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[1];
			theApp.m_AT.pAutoTestSPR[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[1];
			theApp.m_AT.pAutoTestSPR[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSPR_Total[1];
			theApp.m_AT.pAutoTestSPR[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_Contrast_seg14);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 +g_nAutoTestSPR_seg2[m_nOrder];
				m_nOrder++;
				theApp.SPR_DACupdate( nPos );
				SetTimer(Timer_Contrast_seg13, Elapse_AutoTest_SPR, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 +g_nAutoTestSPR_seg3[m_nOrder];
				m_nOrder++;
				theApp.SPR_DACupdate( nPos );
				SetTimer(Timer_Contrast_seg15, Elapse_AutoTest_SPR, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 +g_nAutoTestSPR_seg2[m_nOrder];
			m_nOrder--;
			theApp.SPR_DACupdate( nPos );
		}
	}
	else if( nIDEvent == Timer_Contrast_seg15 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[2];
			theApp.m_AT.pAutoTestSPR[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[2];
			theApp.m_AT.pAutoTestSPR[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSPR_Total[2];
			theApp.m_AT.pAutoTestSPR[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_Contrast_seg15);

			m_nOrder = sizeof(g_nAutoTestSPR_seg3) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 +g_nAutoTestSPR_seg3[m_nOrder];
			m_nOrder--;
			theApp.SPR_DACupdate( nPos );
			SetTimer(Timer_Contrast_seg16, Elapse_AutoTest_SPR, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 +g_nAutoTestSPR_seg3[m_nOrder];
			m_nOrder++;
			theApp.SPR_DACupdate( nPos );
		}
	}
	else if( nIDEvent == Timer_Contrast_seg16 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[2];
			theApp.m_AT.pAutoTestSPR[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[2];
			theApp.m_AT.pAutoTestSPR[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSPR_Total[2];
			theApp.m_AT.pAutoTestSPR[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_Contrast_seg16);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 +g_nAutoTestSPR_seg3[m_nOrder];
				m_nOrder++;
				theApp.SPR_DACupdate( nPos );
				SetTimer(Timer_Contrast_seg15, Elapse_AutoTest_SPR, NULL);
			}
			else
			{
				EnableParams( m_nSegIndex, TRUE );
				// 恢复真空状态机的工作
				CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 +g_nAutoTestSPR_seg3[m_nOrder];
			m_nOrder--;
			theApp.SPR_DACupdate( nPos );
		}
	}
}

void CDlgAutoTestSPR::TimerChangeContrastReverse( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
	if( nIDEvent == Timer_Contrast_seg21 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[0];
			theApp.m_AT.pAutoTestSPR[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[0];
			theApp.m_AT.pAutoTestSPR[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSPR_Total[0];
			theApp.m_AT.pAutoTestSPR[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_Contrast_seg21);

			m_nOrder = sizeof(g_nAutoTestSPR_seg4) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 +g_nAutoTestSPR_seg4[m_nOrder];
			m_nOrder--;
			theApp.SPR_DACupdate( nPos );
			SetTimer(Timer_Contrast_seg22, Elapse_AutoTest_SPR, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 +g_nAutoTestSPR_seg4[m_nOrder];
			m_nOrder++;
			theApp.SPR_DACupdate( nPos );
		}
	}
	else if( nIDEvent == Timer_Contrast_seg22 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[0];
			theApp.m_AT.pAutoTestSPR[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[0]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[0];
			theApp.m_AT.pAutoTestSPR[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[0]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSPR_Total[0];
			theApp.m_AT.pAutoTestSPR[0][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[0]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_Contrast_seg22);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 +g_nAutoTestSPR_seg4[m_nOrder];
				m_nOrder++;
				theApp.SPR_DACupdate( nPos );
				SetTimer(Timer_Contrast_seg21, Elapse_AutoTest_SPR, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 +g_nAutoTestSPR_seg5[m_nOrder];
				m_nOrder++;
				theApp.SPR_DACupdate( nPos );
				SetTimer(Timer_Contrast_seg23, Elapse_AutoTest_SPR, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 +g_nAutoTestSPR_seg4[m_nOrder];
			m_nOrder--;
			theApp.SPR_DACupdate( nPos );
		}
	}
	else if( nIDEvent == Timer_Contrast_seg23 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[1];
			theApp.m_AT.pAutoTestSPR[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[1];
			theApp.m_AT.pAutoTestSPR[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSPR_Total[1];
			theApp.m_AT.pAutoTestSPR[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_Contrast_seg23);

			m_nOrder = sizeof(g_nAutoTestSPR_seg5) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 +g_nAutoTestSPR_seg5[m_nOrder];
			m_nOrder--;
			theApp.SPR_DACupdate( nPos );
			SetTimer(Timer_Contrast_seg24, Elapse_AutoTest_SPR, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 +g_nAutoTestSPR_seg5[m_nOrder];
			m_nOrder++;
			theApp.SPR_DACupdate( nPos );
		}
	}
	else if( nIDEvent == Timer_Contrast_seg24 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[1];
			theApp.m_AT.pAutoTestSPR[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[1]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[1];
			theApp.m_AT.pAutoTestSPR[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[1]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSPR_Total[1];
			theApp.m_AT.pAutoTestSPR[1][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[1]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_Contrast_seg24);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 +g_nAutoTestSPR_seg5[m_nOrder];
				m_nOrder++;
				theApp.SPR_DACupdate( nPos );
				SetTimer(Timer_Contrast_seg23, Elapse_AutoTest_SPR, NULL);
			}
			else
			{
				m_bContinue = TRUE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 +g_nAutoTestSPR_seg6[m_nOrder];
				m_nOrder++;
			theApp.SPR_DACupdate( nPos );
				SetTimer(Timer_Contrast_seg25, Elapse_AutoTest_SPR, NULL);
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 +g_nAutoTestSPR_seg5[m_nOrder];
			m_nOrder--;
			theApp.SPR_DACupdate( nPos );
		}
	}
	else if( nIDEvent == Timer_Contrast_seg25 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[2];
			theApp.m_AT.pAutoTestSPR[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[2];
			theApp.m_AT.pAutoTestSPR[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSPR_Total[2];
			theApp.m_AT.pAutoTestSPR[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == AutoTest_16bitDAC_StepsEach )
		{
			KillTimer(Timer_Contrast_seg25);

			m_nOrder = sizeof(g_nAutoTestSPR_seg6) / sizeof(int) - 1;
			// 设置起始值
			nPos = -32768 +g_nAutoTestSPR_seg6[m_nOrder];
			m_nOrder--;
			theApp.SPR_DACupdate( nPos );
			SetTimer(Timer_Contrast_seg26, Elapse_AutoTest_SPR, NULL);
		}
		else
		{
			// 设置新值
			nPos = -32768 +g_nAutoTestSPR_seg6[m_nOrder];
			m_nOrder++;
			theApp.SPR_DACupdate( nPos );
		}
	}
	else if( nIDEvent == Timer_Contrast_seg26 )
	{
		nPos = theApp.m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		if( theApp.m_DL.other.bDebug )
		{
			// 调试模式下，记录3次值
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[2];
			theApp.m_AT.pAutoTestSPR[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[2]++;
			m_strRes += str;

			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging_onetime( 80, dValue, TRUE );
			nPos = theApp.m_AT.nAutoTestSPR_Total[2];
			theApp.m_AT.pAutoTestSPR[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[2]++;
			m_strRes += str;
		}
		else
		{
			dValue = 0.0;
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			nPos = theApp.m_AT.nAutoTestSPR_Total[2];
			theApp.m_AT.pAutoTestSPR[2][nPos] = dValue;
			theApp.m_AT.nAutoTestSPR_Total[2]++;
			m_strRes += str;
		}
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( theApp.m_AT.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( m_nOrder == -1 )
		{
			KillTimer(Timer_Contrast_seg26);

			if( m_bContinue )
			{
				m_bContinue = FALSE;
				m_nOrder = 0;
				// 设置起始值
				nPos = -32768 +g_nAutoTestSPR_seg6[m_nOrder];
				m_nOrder++;
				theApp.SPR_DACupdate( nPos );
				SetTimer(Timer_Contrast_seg25, Elapse_AutoTest_SPR, NULL);
			}
			else
			{
				EnableParams( m_nSegIndex, TRUE );
				// 恢复真空状态机的工作
				CSMvac::Instance().SetParams( 22, 0 );
			}
		}
		else
		{
			// 设置新值
			nPos = -32768 +g_nAutoTestSPR_seg6[m_nOrder];
			m_nOrder--;
			theApp.SPR_DACupdate( nPos );
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
