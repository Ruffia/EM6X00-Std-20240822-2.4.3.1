// DlgCaliSCN.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgCaliSCN.h"

#include "MainFrm.h"
#include "MsWord9.h"
#include "ControlLayer.h"
#include "BoardComm.h"
#include "ScaleManager.h"
#include "SemCoreAssemble.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define Timer_Cali_StigX		1
#define Timer_Cali_StigY		2
#define Timer_Cali_SCN_ShiftX	3
#define Timer_Cali_SCN_ShiftY	4


/////////////////////////////////////////////////////////////////////////////
// CDlgCaliSCN dialog


CDlgCaliSCN::CDlgCaliSCN(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCaliSCN::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCaliSCN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nChnIndex1	= Diag_Stig_X;
	m_strRes		= "";
}


void CDlgCaliSCN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCaliSCN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCaliSCN, CDialog)
	//{{AFX_MSG_MAP(CDlgCaliSCN)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_SaveGraph, OnSaveGraph)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_11, OnStartStigX)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_21, OnStartStigY)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_31, OnStartShiftX)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_41, OnStartShiftY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliSCN message handlers

void CDlgCaliSCN::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	for( int i=0; i<4; i++ )
		PaintGraph( &dc, i );

	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgCaliSCN::PaintGraph( CDC* pDC, int nIndex, BOOL bErase )
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

	if( CControlLayer::Instance().m_Cali.nCaliSCN_Total[nIndex] < 2 )
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
	nTotal = CControlLayer::Instance().m_Cali.nCaliSCN_Total[nIndex];

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
		dYMin = (CControlLayer::Instance().m_Cali.pCaliSCN[nIndex])[xMin];
		for( i=xMin+1; i<xMax; i++ )
		{
			if( CControlLayer::Instance().m_Cali.pCaliSCN[nIndex][i] < dYMin )
				dYMin = CControlLayer::Instance().m_Cali.pCaliSCN[nIndex][i];
		}
//		dYMinLog = log(dYMin) / log(2);
	}
	{
		dYMax = (CControlLayer::Instance().m_Cali.pCaliSCN[nIndex])[xMin];
		for( i=xMin+1; i<xMax; i++ )
		{
			if( CControlLayer::Instance().m_Cali.pCaliSCN[nIndex][i] > dYMax )
				dYMax = CControlLayer::Instance().m_Cali.pCaliSCN[nIndex][i];
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
		y = (int)(rc1.top + ( nHeight - (CControlLayer::Instance().m_Cali.pCaliSCN[nIndex][xMin] -dYMin) / (dYMax-dYMin) * nHeight ));
//		dY		= CControlLayer::Instance().m_Cali.pCaliSCN[nIndex][xMin];
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
			y = (int)(rc1.top + ( nHeight - (CControlLayer::Instance().m_Cali.pCaliSCN[nIndex][i] -dYMin) / (dYMax-dYMin) * nHeight ));
//			dY		= CControlLayer::Instance().m_Cali.pCaliSCN[nIndex][i];
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
	double dValue = CBoardComm::Instance().GetPhysicsFromAdcs(m_nChnIndex2[nIndex], dYMin, CControlLayer::Instance().m_DL.log.strCalc);
	str.Format( "%.6f ( %.6f )", dYMin, dValue );
	pDC->TextOut( rc1.left -8, rc1.bottom -sz.cy/2, str );
	// 标注纵向最大值
	dValue = CBoardComm::Instance().GetPhysicsFromAdcs(m_nChnIndex2[nIndex], dYMax, CControlLayer::Instance().m_DL.log.strCalc);
	str.Format( "%.6f ( %.6f )", dYMax, dValue );
	pDC->TextOut( rc1.left -8, rc1.top -sz.cy/2, str );
	// 画标签
	str.Format( "%s", CControlLayer::Instance().m_DL.adc.chn[m_nChnIndex2[nIndex]].strLabel );
	sz = pDC->GetTextExtent( str );
	pDC->TextOut( rc.left + rc.Width()/2 - sz.cx/2, rc.top +11, str );
	pDC->SetBkMode(noldBKmode);

	pen.DeleteObject();
	penGrid.DeleteObject();
}

void CDlgCaliSCN::OnSaveGraph() 
{
	// TODO: Add your control notification handler code here
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
	str.Format( "Calibration SCN" );
	bmpDC.TextOut( 10, 0, str );
	for(int i=0; i<4; i++ )
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

	char appfilepath[255];
	sprintf(appfilepath, "%s", filename );
	char *ptemp = strrchr( appfilepath, '.');
	ptemp[1] = 0;
	strcat( appfilepath, "doc" );
	Report( appfilepath );
}

void CDlgCaliSCN::Report( CString filename ) 
{
	// TODO: Add your control notification handler code here
	BOOL result;
	COleException	e;
	_Application	app;
	_Document		doc;
	Documents		docs;
	Selection		sel;
	_Font			font;
	Paragraph		par;
	Tables			tables;
	Range			range;

	CString str, strCtrl;

	//Define a VARIANT as optional parameter passed
	VARIANT varOptional;
	varOptional.vt = VT_ERROR;
	varOptional.scode = DISP_E_PARAMNOTFOUND;
	COleVariant varTrue((short)-1, VT_BOOL), varFalse((short)0, VT_BOOL);

	//Create a Application object of Word
	result = app.CreateDispatch( "Word.Application", &e );
	if( !result )
	{
		char szErrMsg[255];
		e.GetErrorMessage( szErrMsg, 255);
		AfxMessageBox( szErrMsg, MB_ICONSTOP );
		return;
	}

	::SetCursor(LoadCursor(NULL, IDC_WAIT));//02.08.30
	//Get the selection of documents
	docs.AttachDispatch( app.GetDocuments() );
	docs.Add( &varOptional, &varOptional, &varOptional, &varOptional );
	docs.ReleaseDispatch();
	//Get selection object
	sel.AttachDispatch( app.GetSelection() );
	//Alignment
	par.AttachDispatch( sel.GetParagraphFormat() );
	//Get font
	font.AttachDispatch( sel.GetFont() );
	str = "Arial";
	font.SetName( str );
	float fontsize = font.GetSize();
	font.SetSize( (float)(fontsize*1.5) );
	font.SetBold( 2 );

	par.SetAlignment(WORDALIGNMENT_CENTER);
	str = "Calibration Report";
	sel.TypeText( str );
	sel.TypeParagraph();
	font.SetBold( 1 );
	str = "SCN Board";
	sel.TypeText( str );
	sel.TypeParagraph();
	par.SetAlignment( WORDALIGNMENT_LEFT );
	sel.TypeParagraph();

	font.SetBold( 0 );
	font.SetSize( 8 );
	par.SetAlignment( WORDALIGNMENT_RIGHT );
	range.AttachDispatch( sel.GetRange() );
	tables.AttachDispatch( sel.GetTables() );
	tables.Add( range, Cali_12bitDAC_Steps +2, 8, varTrue, varTrue );
	tables.ReleaseDispatch();
	range.ReleaseDispatch();

	//存表头
	sel.SelectRow();
	font.SetBold( 1 );
	font.SetSize( 10 );
	str = "StigX Ctrl";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "StigX Read";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "StigY Ctrl";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "StigY Read";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "SCN ShiftX Ctrl";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "ScanX Read";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "SCN ShiftY Ctrl";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "ScanY Read";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );

	//存表
	font.SetBold( 0 );
	font.SetSize( 8 );
	int nCtrlValue = 0;
	/////////////////////////////////////////////
	for( int i=0; i<Cali_12bitDAC_Steps +1; i++ )
	{
		for( int j=0; j<4; j++ )
		{
			strCtrl.Format( "0x%04X", nCtrlValue );
			sel.Move( &varOptional, &varOptional );
			sel.TypeText( strCtrl );

			str.Format( "%.6f", CControlLayer::Instance().m_Cali.pCaliSCN[j][i]);
			sel.Move( &varOptional, &varOptional );
			sel.TypeText( str );
		}
		nCtrlValue += Cali_12bitDAC_dChange;
		if( nCtrlValue > g_n12bitDAC_Max )
			nCtrlValue = g_n12bitDAC_Max;
	}

	font.ReleaseDispatch();
	par.ReleaseDispatch();
	sel.ReleaseDispatch();

	COleVariant varFileName(filename), varPasswd("");
	COleVariant varFileFormat((short)0);
	//Get the active document object
	doc.AttachDispatch( app.GetActiveDocument() );
	//Save the document
	doc.SaveAs( varFileName, varFileFormat, varFalse, varPasswd, varTrue, varPasswd, varFalse, varFalse, varFalse, varFalse, varFalse);
	//Close the document
	doc.Close(&varOptional, &varOptional, &varOptional);
	doc.ReleaseDispatch();

	app.Quit(&varOptional, &varOptional, &varOptional);
	app.DetachDispatch();
	app.ReleaseDispatch();

	::SetCursor(LoadCursor(NULL, IDC_CROSS));//02.08.30
}

void CDlgCaliSCN::InitParams()
{
	for( int i=0; i<4; i++ )
	{
		CControlLayer::Instance().m_Cali.pCaliSCN[i] = new double[Cali_16bitDAC_Steps+1];
		for( int j=0; j<Cali_16bitDAC_Steps+1; j++ )
			CControlLayer::Instance().m_Cali.pCaliSCN[i][j] = 0;
		CControlLayer::Instance().m_Cali.nCaliSCN_Total[i] = 0;
		m_nChnIndex2[i] = 0;
	}
}

void CDlgCaliSCN::EnableParams( int nIndex, BOOL bEnable )
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
			var.lVal	= CControlLayer::Instance().m_Cali.nCaliSCN_OldPos[0];
			Xpos		= var;
			
			nTimerIndex = Timer_Cali_StigX;
			break;
		case	2:
			// Stigmator
			ClassID.lVal = SemControlPanelClass_Stigmator;
			Flag.lVal	= 2;
			var.lVal	= CControlLayer::Instance().m_Cali.nCaliSCN_OldPos[1];
			Ypos		= var;
			
			nTimerIndex = Timer_Cali_StigY;
			break;
		case	4:
			// ScanShiftX
			ClassID.lVal = SemControlPanelClass_ScanShift;
			Flag.lVal	= 1;
			var.lVal	= CControlLayer::Instance().m_Cali.nCaliSCN_OldPos[2];
			Xpos		= var;
			
			nTimerIndex = Timer_Cali_SCN_ShiftX;
			break;
		case	6:
			// ScanShiftY
			ClassID.lVal = SemControlPanelClass_ScanShift;
			Flag.lVal	= 2;
			var.lVal	= CControlLayer::Instance().m_Cali.nCaliSCN_OldPos[3];
			Ypos		= var;
			
			nTimerIndex = Timer_Cali_SCN_ShiftY;
			break;
		}

		// 停止定时器
		for(int i=0; i<1; i++ )
			KillTimer( nTimerIndex +i);
		// 设置原始值
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		// 恢复按钮状态
		for(int i=0; i<8; i+=2 )
		{
			((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +i)))->EnableWindow( TRUE );
			((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +i)))->SetCheck( 0 );
		}
		// 关闭记录文件
		CloseHandle(CControlLayer::Instance().m_Cali.hLogFile);
	}
	else
	{
		// 设置按钮状态，准备检测
		for(int i=0; i<8; i+=2 )
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
		str.Format( "%szCaliSCN-%s.txt", appfilepath, strTemp );
		CControlLayer::Instance().m_Cali.hLogFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	}
}

void CDlgCaliSCN::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == Timer_Cali_StigX )
		TimerChangeStigX( Timer_Cali_StigX );
	else if( nIDEvent == Timer_Cali_StigY )
		TimerChangeStigY( Timer_Cali_StigY );
	else if( nIDEvent == Timer_Cali_SCN_ShiftX )
		TimerChangeShiftX( Timer_Cali_SCN_ShiftX );
	else if( nIDEvent == Timer_Cali_SCN_ShiftY )
		TimerChangeShiftY( Timer_Cali_SCN_ShiftY );
	CDialog::OnTimer(nIDEvent);
}

void CDlgCaliSCN::OnStartStigX() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11)))->GetCheck();
	if( nCheck )
	{
		CBoardComm::Instance().ChangedADCselectOB( 0 );
		CBoardComm::Instance().ChangedADCsChSelect( 4 , CControlLayer::Instance().m_DL.adc.chn );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nChnIndex1	= Diag_Stig_X;
		m_nChnIndex2[0]	= 4;
		m_strRes		= "";
		int nMax = Cali_12bitDAC_Steps;
		for( int i=0; i<nMax; i++ )
			CControlLayer::Instance().m_Cali.pCaliSCN[0][i] = 0;
		CControlLayer::Instance().m_Cali.nCaliSCN_Total[0] = 0;
		CControlLayer::Instance().m_DL.adc.nAveCounts	= Cali_Ave_Counts;

		VARIANT var = CSemCoreAssemble::Instance().m_StigmatorX.GetPos();
		CControlLayer::Instance().m_Cali.nCaliSCN_OldPos[0] = var.lVal;

		int nPos = -2048;	// 设置起始值
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.SelchangePage( 3 );
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.m_scrollXstigDAC.SetScrollPos( nPos );
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.XstigDACSB_Change( nPos );
		SetTimer(Timer_Cali_StigX, Elapse_Cali_2448, NULL);

		EnableParams( 0, FALSE );
	}
	else
	{
		EnableParams( 0, TRUE );
	}
}

void CDlgCaliSCN::OnStartStigY() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_21)))->GetCheck();
	if( nCheck )
	{
		CBoardComm::Instance().ChangedADCselectOB( 0 );
		CBoardComm::Instance().ChangedADCsChSelect( 6, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nChnIndex1	= Diag_Stig_Y;
		m_nChnIndex2[1]	= 6;
		m_strRes		= "";
		int nMax = Cali_16bitDAC_Steps;
		for( int i=0; i<nMax; i++ )
			CControlLayer::Instance().m_Cali.pCaliSCN[1][i] = 0;
		CControlLayer::Instance().m_Cali.nCaliSCN_Total[1] = 0;
		CControlLayer::Instance().m_DL.adc.nAveCounts	= Cali_Ave_Counts;

		VARIANT var = CSemCoreAssemble::Instance().m_StigmatorY.GetPos();
		CControlLayer::Instance().m_Cali.nCaliSCN_OldPos[1] = var.lVal;

		int nPos = -2048;	// 设置起始值
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.SelchangePage( 3 );
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.m_scrollYstigDAC.SetScrollPos( nPos );
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.YstigDACSB_Change( nPos );
		SetTimer(Timer_Cali_StigY, Elapse_Cali_2448, NULL);

		EnableParams( 2, FALSE );
	}
	else
	{
		EnableParams( 2, TRUE );
	}
}

void CDlgCaliSCN::OnStartShiftX() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_31)))->GetCheck();
	if( nCheck )
	{
		CBoardComm::Instance().ChangedADCselectOB( 0 );
		CBoardComm::Instance().ChangedADCsChSelect( 0 , CControlLayer::Instance().m_DL.adc.chn );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nChnIndex1	= Diag_Shift_X;
		m_nChnIndex2[2]	= 0;
		m_strRes		= "";
		int nMax = Cali_16bitDAC_Steps;
		for( int i=0; i<nMax; i++ )
			CControlLayer::Instance().m_Cali.pCaliSCN[2][i] = 0;
		CControlLayer::Instance().m_Cali.nCaliSCN_Total[2] = 0;
		CControlLayer::Instance().m_DL.adc.nAveCounts	= Cali_Ave_Counts;

		VARIANT var = CSemCoreAssemble::Instance().m_ScanShiftX.GetPos();
		CControlLayer::Instance().m_Cali.nCaliSCN_OldPos[2] = var.lVal;

		// FineMag居中，Digital Shift归零，测试Analog Shift
//		int nValue = 0x800;
		WORD wValue = (((0x08 & 0xF) | (0x02 << 6)) << 8) | 0x00;	// X FineMag
		CSemCtrl::Instance().USB_WriteToBoard( USB_Det45StoreDAC, TRUE, wValue );
		CSemCtrl::Instance().USB_WriteToBoard( USB_SetXoffset, TRUE, 0 );	// Digital Shift X

		int nPos = -2048;	// 设置起始值
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.SelchangePage( 3 );
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.m_scrollXshiftDAC.SetScrollPos( nPos );
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.XshiftDACSB_Change( nPos );
		SetTimer(Timer_Cali_SCN_ShiftX, Elapse_Cali_2448, NULL);

		EnableParams( 4, FALSE );
	}
	else
	{
		EnableParams( 4, TRUE );
	}
}

void CDlgCaliSCN::OnStartShiftY() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_41)))->GetCheck();
	if( nCheck )
	{
		CBoardComm::Instance().ChangedADCselectOB( 0 );
		CBoardComm::Instance().ChangedADCsChSelect( 2 , CControlLayer::Instance().m_DL.adc.chn );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nChnIndex1	= Diag_Shift_Y;
		m_nChnIndex2[3]	= 2;
		m_strRes		= "";
		int nMax = Cali_16bitDAC_Steps;
		for( int i=0; i<nMax; i++ )
			CControlLayer::Instance().m_Cali.pCaliSCN[3][i] = 0;
		CControlLayer::Instance().m_Cali.nCaliSCN_Total[3] = 0;
		CControlLayer::Instance().m_DL.adc.nAveCounts	= Cali_Ave_Counts;

		VARIANT var = CSemCoreAssemble::Instance().m_ScanShiftY.GetPos();
		CControlLayer::Instance().m_Cali.nCaliSCN_OldPos[3] = var.lVal;

		// FineMag居中，Digital Shift归零，测试Analog Shift
//		int nValue = 0x800;
		WORD wValue = (((0x08 & 0xF) | (0x03 << 6)) << 8) | 0x00;	// Y FineMag
		CSemCtrl::Instance().USB_WriteToBoard( USB_Det45StoreDAC, TRUE, wValue );
		CSemCtrl::Instance().USB_WriteToBoard( USB_SetYoffset, TRUE, 0 );	// Digital Shift Y

		int nPos = -2048;	// 设置起始值
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.SelchangePage( 3 );
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.m_scrollYshiftDAC.SetScrollPos( nPos );
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.YshiftDACSB_Change( nPos );
		SetTimer(Timer_Cali_SCN_ShiftY, Elapse_Cali_2448, NULL);

		EnableParams( 6, FALSE );
	}
	else
	{
		EnableParams( 6, TRUE );
	}
}

void CDlgCaliSCN::TimerChangeStigX( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
//	if( nIDEvent == Timer_Cali_StigX )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		dValue = 0.0;
		str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
		nPos = CControlLayer::Instance().m_Cali.nCaliSCN_Total[0];
		CControlLayer::Instance().m_Cali.pCaliSCN[0][nPos] = dValue;
		CControlLayer::Instance().m_Cali.nCaliSCN_Total[0]++;
		m_strRes += str;
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_Cali.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos +2048 == g_n12bitDAC_Max )
		{
			KillTimer(Timer_Cali_StigX);
			EnableParams( 0, TRUE );
		}
		else
		{
			// 设置新值
			nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos + Cali_12bitDAC_dChange;
			if( nPos > g_n12bitDAC_Max )
				nPos = g_n12bitDAC_Max;
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.m_scrollXstigDAC.SetScrollPos( nPos );
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.XstigDACSB_Change( nPos );
		}
	}
}

void CDlgCaliSCN::TimerChangeStigY( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
//	if( nIDEvent == Timer_Cali_StigY )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		dValue = 0.0;
		str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log  );
		nPos = CControlLayer::Instance().m_Cali.nCaliSCN_Total[1];
		CControlLayer::Instance().m_Cali.pCaliSCN[1][nPos] = dValue;
		CControlLayer::Instance().m_Cali.nCaliSCN_Total[1]++;
		m_strRes += str;
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_Cali.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos +2048 == g_n12bitDAC_Max )
		{
			KillTimer(Timer_Cali_StigY);
			EnableParams( 2, TRUE );
		}
		else
		{
			// 设置新值
			nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos + Cali_12bitDAC_dChange;
			if( nPos > g_n12bitDAC_Max )
				nPos = g_n12bitDAC_Max;
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.m_scrollYstigDAC.SetScrollPos( nPos );
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.YstigDACSB_Change( nPos );
		}
	}
}

void CDlgCaliSCN::TimerChangeShiftX( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
//	if( nIDEvent == Timer_Cali_SCN_ShiftX )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		dValue = 0.0;
		str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
		nPos = CControlLayer::Instance().m_Cali.nCaliSCN_Total[2];
		CControlLayer::Instance().m_Cali.pCaliSCN[2][nPos] = dValue;
		CControlLayer::Instance().m_Cali.nCaliSCN_Total[2]++;
		m_strRes += str;
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_Cali.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos +2048 == g_n12bitDAC_Max )
		{
			KillTimer(Timer_Cali_SCN_ShiftX);
			EnableParams( 4, TRUE );
		}
		else
		{
			// 设置新值
			nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos + Cali_12bitDAC_dChange;
			if( nPos > g_n12bitDAC_Max )
				nPos = g_n12bitDAC_Max;
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.m_scrollXshiftDAC.SetScrollPos( nPos );
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.XshiftDACSB_Change( nPos );
		}
	}
}

void CDlgCaliSCN::TimerChangeShiftY( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
//	if( nIDEvent == Timer_Cali_SCN_ShiftY )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +2048 );
		m_strRes += str;

		dValue = 0.0;
		str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
		nPos = CControlLayer::Instance().m_Cali.nCaliSCN_Total[3];
		CControlLayer::Instance().m_Cali.pCaliSCN[3][nPos] = dValue;
		CControlLayer::Instance().m_Cali.nCaliSCN_Total[3]++;
		m_strRes += str;
		// 记录数值到文件
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_Cali.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos +2048 == g_n12bitDAC_Max )
		{
			KillTimer(Timer_Cali_SCN_ShiftY);
			EnableParams( 6, TRUE );
		}
		else
		{
			// 设置新值
			nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos + Cali_12bitDAC_dChange;
			if( nPos > g_n12bitDAC_Max )
				nPos = g_n12bitDAC_Max;
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.m_scrollYshiftDAC.SetScrollPos( nPos );
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelSL.YshiftDACSB_Change( nPos );
		}
	}
}
