// DlgCaliSPR.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgCaliSPR.h"

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

#define Timer_Cali_Contrast	1


/////////////////////////////////////////////////////////////////////////////
// CDlgCaliSPR dialog


CDlgCaliSPR::CDlgCaliSPR(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCaliSPR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCaliSPR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nChnIndex1	= Diag_SPRDAC_ChA;
	m_strRes		= "";
}


void CDlgCaliSPR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCaliSPR)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCaliSPR, CDialog)
	//{{AFX_MSG_MAP(CDlgCaliSPR)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_SaveGraph, OnSaveGraph)
	ON_BN_CLICKED(IDC_CHECK_AT_Cali_Start_11, OnStartContrast)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliSPR message handlers

void CDlgCaliSPR::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	for( int i=0; i<4; i++ )
		PaintGraph( &dc, i );

	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgCaliSPR::PaintGraph( CDC* pDC, int nIndex, BOOL bErase )
{
	/////////////////////////////////////////////////////////////////
	// ���ۻ��벻����������ͼ�������
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

	if( CControlLayer::Instance().m_Cali.nCaliSPR_Total[nIndex] < 2 )
		return;

	/////////////////////////////////////////////////////////////////
	// ���ñ���
	double dYMin, dYMax, dRatio;//, dYMinLog, dYMaxLog, dY;
	int nHeight, xMin, xMax, x, y, nTotal, i, xIntMain, xInt;//, yLog;
	// ���񻭱ʳ�ʼ��
	CPen pen, penGrid, *poldpen;
	penGrid.CreatePen( PS_DOT, 1, RGB(192,192,192) );
	int noldBKmode;
	CString str = "100";
	CSize sz = pDC->GetTextExtent( str );

	// ��ͼ����
	rc1 = rc;
	rc1.left	+= 20;
	rc1.right	-= 20;
	rc1.top		+= 20;
	rc1.bottom	-= 20;
	nHeight = rc1.Height();
	nTotal = CControlLayer::Instance().m_Cali.nCaliSPR_Total[nIndex];

	// X���ͼ����
	xMin = 0;
	xMax = nTotal;

	// ���ݸ���
	nTotal = xMax - xMin;
	dRatio = 1.0 * rc1.Width() / (nTotal-1);

	// ���������ֵ����Сֵ
	// ȡ��2Ϊ�׵Ķ�����������
	// log2(y) = ln(y) / ln(2)
	// C������log(x)��ʾ��eΪ�׵���Ȼ������log10(x)��ʾ��10Ϊ�׵ĳ��ö���
	{
		dYMin = (CControlLayer::Instance().m_Cali.pCaliSPR[nIndex])[xMin];
		for( i=xMin+1; i<xMax; i++ )
		{
			if( CControlLayer::Instance().m_Cali.pCaliSPR[nIndex][i] < dYMin )
				dYMin = CControlLayer::Instance().m_Cali.pCaliSPR[nIndex][i];
		}
//		dYMinLog = log(dYMin) / log(2);
	}
	{
		dYMax = (CControlLayer::Instance().m_Cali.pCaliSPR[nIndex])[xMin];
		for( i=xMin+1; i<xMax; i++ )
		{
			if( CControlLayer::Instance().m_Cali.pCaliSPR[nIndex][i] > dYMax )
				dYMax = CControlLayer::Instance().m_Cali.pCaliSPR[nIndex][i];
		}
//		dYMaxLog = log(dYMax) / log(2);
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// ��������
	noldBKmode = pDC->SetBkMode(TRANSPARENT);
	// ����
	for( i=0; i<9; i++ )
	{
		if( i % 4 == 0 )		// ��Ҫ����
		{
			y = rc1.top + i*nHeight/8;
			pDC->MoveTo( rc1.left -20, y );
			pDC->LineTo( rc1.left -10, y );
			poldpen = pDC->SelectObject( &penGrid );
			pDC->LineTo( rc1.right, y );
			pDC->SelectObject( poldpen );
		}
		else if( i % 2 == 0 )	// ��Ҫ����
		{
			y = rc1.top + i*nHeight/8;
			pDC->MoveTo( rc1.left -15, y );
			pDC->LineTo( rc1.left -10, y );
			poldpen = pDC->SelectObject( &penGrid );
			pDC->LineTo( rc1.right, y );
			pDC->SelectObject( poldpen );
		}
	}
	// ����
	{
		xIntMain	= 10;
		xInt		= 2;
	}
	for( i=xMin; i<xMax; i++ )
	{
		if( i % xIntMain == 0 )		// ��Ҫ����
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
		else if( i % xInt == 0 )	// ��Ҫ����
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
	// ��������
	// ѡ�񻭱�
	// ��������
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
	// �����Ƶ���ʼ��
	if( dYMax == dYMin )
	{
		y = rc1.bottom;
//		yLog = rc1.bottom;
	}
	else
	{
		y = (int)(rc1.top + ( nHeight - (CControlLayer::Instance().m_Cali.pCaliSPR[nIndex][xMin] -dYMin) / (dYMax-dYMin) * nHeight ));
//		dY		= CControlLayer::Instance().m_Cali.pCaliSPR[nIndex][xMin];
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
	// ������ͼ
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
			y = (int)(rc1.top + ( nHeight - (CControlLayer::Instance().m_Cali.pCaliSPR[nIndex][i] -dYMin) / (dYMax-dYMin) * nHeight ));
//			dY		= CControlLayer::Instance().m_Cali.pCaliSPR[nIndex][i];
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
	// ��ע������Сֵ
	noldBKmode = pDC->SetBkMode(TRANSPARENT);
	double dValue = CBoardComm::Instance().GetPhysicsFromAdcs(5*16 +m_nChnIndex2[nIndex], dYMin, CControlLayer::Instance().m_DL.log.strCalc);
	str.Format( "%.6f ( %.6f )", dYMin, dValue );
	pDC->TextOut( rc1.left -8, rc1.bottom -sz.cy/2, str );
	// ��ע�������ֵ
	dValue = CBoardComm::Instance().GetPhysicsFromAdcs(5*16 +m_nChnIndex2[nIndex], dYMax, CControlLayer::Instance().m_DL.log.strCalc);
	str.Format( "%.6f ( %.6f )", dYMax, dValue );
	pDC->TextOut( rc1.left -8, rc1.top -sz.cy/2, str );
	// ����ǩ
	str.Format( "%s", CControlLayer::Instance().m_DL.adc.chn[5*16 +m_nChnIndex2[nIndex]].strLabel );
	sz = pDC->GetTextExtent( str );
	pDC->TextOut( rc.left + rc.Width()/2 - sz.cx/2, rc.top +11, str );
	pDC->SetBkMode(noldBKmode);

	pen.DeleteObject();
	penGrid.DeleteObject();
}

void CDlgCaliSPR::OnSaveGraph() 
{
	// TODO: Add your control notification handler code here
	CString filename;
	char szFilter[] = "BMP file|*.bmp|JPG file|*.jpg|PNG file|*.png|TIF file|*.tif|PCX file|*.pcx|TGA file|*.tga||";
	CFileDialog dlgFile(FALSE, "BMP", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	if( dlgFile.DoModal() == IDOK )
		filename = dlgFile.GetPathName();
	else
		return;
	// ��ȡ��׺��
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
	// �ɺ�׺���жϴ�ȡ��ʽ����
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
	// ��һ����������ͼ
	CRect client, rc;
	CClientDC dc( this );
	GetClientRect( &client );
	GetDlgItem( IDC_STATIC_AT_Cali_Graph1 )->GetClientRect( (LPRECT)rc );
	client.right = rc.Width() +40;
	client.bottom += 40;
	//��ȡͼ���Ⱥ͸߶�
	int nWidth = client.Width();
	int nHeight = client.Height();
	//��������Ļ���ڼ��ݵ��ڴ��豸����
	CDC bmpDC;
	bmpDC.CreateCompatibleDC( &dc );
	//�����������ڴ��豸�����л�ͼ��λͼ��������ѡ���ڴ��豸����
	CBitmap& bitmap = *(new CBitmap);
	bitmap.CreateCompatibleBitmap( &dc, nWidth,	nHeight );
	CBitmap* poldBmp = bmpDC.SelectObject( &bitmap );
	//��Ŀ�������԰�ɫ���
	bmpDC.FillRect( &CRect(0, 0, nWidth, nHeight), &CBrush(RGB(255,255,255)) );
	//�����豸��������ԭʼ��
	CPoint point = bmpDC.SetWindowOrg( 0, 0 );
	//��ͼ
	CString str;
	str.Format( "Calibration SPR" );
	bmpDC.TextOut( 10, 0, str );
	for(int i=0; i<4; i++ )
		PaintGraph( &bmpDC, i, TRUE );
	//�ָ��豸��������ԭ�е�ԭʼ��
	bmpDC.SetWindowOrg( point );
	Invalidate( FALSE );
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// �ڶ�������bmpͼ��
	CxImage *newima = new CxImage();
	newima->CreateFromHBITMAP( (HBITMAP)bitmap );
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// ��������Ϊ�ļ�
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

void CDlgCaliSPR::Report( CString filename ) 
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
	str = "SPR Board";
	sel.TypeText( str );
	sel.TypeParagraph();
	par.SetAlignment( WORDALIGNMENT_LEFT );
	sel.TypeParagraph();

	font.SetBold( 0 );
	font.SetSize( 8 );
	par.SetAlignment( WORDALIGNMENT_RIGHT );
	range.AttachDispatch( sel.GetRange() );
	tables.AttachDispatch( sel.GetTables() );
	tables.Add( range, Cali_16bitDAC_Steps +2, 2, varTrue, varTrue );
	tables.ReleaseDispatch();
	range.ReleaseDispatch();

	//���ͷ
	sel.SelectRow();
	font.SetBold( 1 );
	font.SetSize( 10 );
	str = "Contrast Ctrl";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "SPR DAC 1 Read";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
/*	sel.Move( &varOptional, &varOptional );
	str = "OL Ctrl";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "OL Read";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "AlignX Ctrl";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "AlignX Read";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "AlignY Ctrl";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
	sel.Move( &varOptional, &varOptional );
	str = "AlignY Read";
	par.SetAlignment( WORDALIGNMENT_CENTER );
	sel.TypeText( str );
*/
	//���
	font.SetBold( 0 );
	font.SetSize( 8 );
	int nCtrlValue = 0;
	/////////////////////////////////////////////
	for( int i=0; i<Cali_16bitDAC_Steps +1; i++ )
	{
		for( int j=0; j<1; j++ )
		{
			strCtrl.Format( "0x%04X", nCtrlValue );
			sel.Move( &varOptional, &varOptional );
			sel.TypeText( strCtrl );

			str.Format( "%.6f", CControlLayer::Instance().m_Cali.pCaliSPR[j][i]);
			sel.Move( &varOptional, &varOptional );
			sel.TypeText( str );
		}
		nCtrlValue += Cali_16bitDAC_dChange;
		if( nCtrlValue > g_n16bitDAC_Max )
			nCtrlValue = g_n16bitDAC_Max;
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

void CDlgCaliSPR::InitParams()
{
	for( int i=0; i<4; i++ )
	{
		CControlLayer::Instance().m_Cali.pCaliSPR[i] = new double[Cali_16bitDAC_Steps+1];
		for( int j=0; j<Cali_16bitDAC_Steps+1; j++ )
			CControlLayer::Instance().m_Cali.pCaliSPR[i][j] = 0;
		CControlLayer::Instance().m_Cali.nCaliSPR_Total[i] = 0;
		m_nChnIndex2[i] = 0;
	}
}

void CDlgCaliSPR::EnableParams( int nIndex, BOOL bEnable )
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
			// Contrast
			ClassID.lVal = SemControlPanelClass_Contrast;
			Flag.lVal	= 1;
			var.lVal	= CControlLayer::Instance().m_Cali.nCaliSPR_OldPos[0];
			Xpos		= var;
			
			nTimerIndex = Timer_Cali_Contrast;
			break;
		}

		// ֹͣ��ʱ��
		for(int i=0; i<1; i++ )
			KillTimer( nTimerIndex +i);
		// ����ԭʼֵ
		CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
		// �ָ���ť״̬
		for(int i=0; i<2; i+=2 )
		{
			((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +i)))->EnableWindow( TRUE );
			((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +i)))->SetCheck( 0 );
		}
		// �رռ�¼�ļ�
		CloseHandle(CControlLayer::Instance().m_Cali.hLogFile);
	}
	else
	{
		// ���ð�ť״̬��׼�����
		for(int i=0; i<2; i+=2 )
		{
			((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +i)))->SetCheck( 0 );
			((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +i)))->EnableWindow( FALSE );
		}
		((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +nIndex)))->SetCheck( 1 );
		((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11 +nIndex)))->EnableWindow( TRUE );
		// ׼����¼�ļ�
		char appfilepath[255];
		::GetModuleFileName( NULL, appfilepath, 255 );
		char *ptemp = strrchr( appfilepath, '\\');
		ptemp[1] = 0;

		CString str, strTemp, strParam, strFirst;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
		str.Format( "%szCaliSPR-%s.txt", appfilepath, strTemp );
		CControlLayer::Instance().m_Cali.hLogFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	}
}

void CDlgCaliSPR::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == Timer_Cali_Contrast )
		TimerChangeContrast( nIDEvent );
	CDialog::OnTimer(nIDEvent);
}

void CDlgCaliSPR::OnStartContrast() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_AT_Cali_Start_11)))->GetCheck();
	if( nCheck )
	{
		CBoardComm::Instance().ChangedADCselectOB( 5 );
		CBoardComm::Instance().ChangedADCsChSelect( 12, CControlLayer::Instance().m_DL.adc.chn  );
		CBoardComm::Instance().ReadSelectedADC(FALSE);

		m_nChnIndex1	= Diag_SPRDAC_ChA;
		m_nChnIndex2[0]	= 12;
		m_strRes		= "";
		int nMax = Cali_16bitDAC_Steps;
		for( int i=0; i<nMax; i++ )
			CControlLayer::Instance().m_Cali.pCaliSPR[0][i] = 0;
		CControlLayer::Instance().m_Cali.nCaliSPR_Total[0] = 0;
		CControlLayer::Instance().m_DL.adc.nAveCounts	= Cali_Ave_Counts;

		VARIANT var = CSemCoreAssemble::Instance().m_Contrast.GetPos();
		CControlLayer::Instance().m_Cali.nCaliSPR_OldPos[0] = var.lVal;

		int nPos = -32768;	// ������ʼֵ
		m_nOrder++;
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.SelchangePage( 4 );
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelVS.m_nSPRDACchannel = 0;
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelVS.m_scrollSPRDACsVal.SetScrollPos( nPos );
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelVS.SPRDACsValUpdae( nPos );
		((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelVS.SPRDACupdate();
		SetTimer(Timer_Cali_Contrast, Elapse_Cali_2448, NULL);

		EnableParams( 0, FALSE );
	}
	else
	{
		EnableParams( 0, TRUE );
	}
}

void CDlgCaliSPR::TimerChangeContrast( UINT nIDEvent )
{
	int nPos;
	double dValue;
	CString str;
	unsigned long cbRet;
	char Buf[1024];
//	if( nIDEvent == Timer_Cali_Contrast )
	{
		nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		m_strRes += str;

		dValue = 0.0;
		str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue , CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
		nPos = CControlLayer::Instance().m_Cali.nCaliSPR_Total[0];
		CControlLayer::Instance().m_Cali.pCaliSPR[0][nPos] = dValue;
		CControlLayer::Instance().m_Cali.nCaliSPR_Total[0]++;
		m_strRes += str;
		// ��¼��ֵ���ļ�
		sprintf( Buf, "%s\n", m_strRes );
		WriteFile( CControlLayer::Instance().m_Cali.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		m_strRes = "";
		Invalidate(FALSE);

		if( CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos +32768 == g_n16bitDAC_Max )
		{
			KillTimer(Timer_Cali_Contrast);
			EnableParams( 0, TRUE );
		}
		else
		{
			// ������ֵ
			nPos = CControlLayer::Instance().m_DL.diag[m_nChnIndex1].nCurPos + Cali_16bitDAC_dChange;
			if( nPos > g_n16bitDAC_Max )
				nPos = g_n16bitDAC_Max;
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelVS.m_scrollSPRDACsVal.SetScrollPos( nPos );
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelVS.SPRDACsValUpdae( nPos );
			((CMainFrame *)theApp.GetMainWnd())->m_MainPanelDebug.m_PagePanelVS.SPRDACupdate();
		}
	}
}
