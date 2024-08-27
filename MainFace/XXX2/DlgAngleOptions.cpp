// DlgAngleOptions.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "DlgAngleOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAngleOptions dialog


CDlgAngleOptions::CDlgAngleOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAngleOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAngleOptions)
	//}}AFX_DATA_INIT
}


void CDlgAngleOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAngleOptions)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAngleOptions, CDialog)
	//{{AFX_MSG_MAP(CDlgAngleOptions)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON_AngleOptions_Color_Line, OnAngleOptionsColorLine)
	ON_BN_CLICKED(IDC_BUTTON_AngleOptions_Color_Arc, OnAngleOptionsColorArc)
	ON_BN_CLICKED(IDC_BUTTON_AngleOptions_Color_Text, OnAngleOptionsColorText)
	ON_BN_CLICKED(IDC_BUTTON_AngleOptions_SetDefault, OnAngleOptionsSetDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAngleOptions message handlers

void CDlgAngleOptions::Localize(void)
{
	if( m_hWnd == NULL )
		return;
	
	GetDlgItem(IDC_STATIC_AngleOptions_Color_Line)->SetWindowText(GetResString( IDS_STATIC_AngleOptions_Color_Line ));
	GetDlgItem(IDC_STATIC_AngleOptions_Color_Arc)->SetWindowText(GetResString( IDS_STATIC_AngleOptions_Color_Arc ));
	GetDlgItem(IDC_STATIC_AngleOptions_Color_Text)->SetWindowText(GetResString( IDS_STATIC_AngleOptions_Color_Text ));
	GetDlgItem(IDC_STATIC_AngleOptions_Font_Size)->SetWindowText(GetResString( IDS_STATIC_AngleOptions_Font_Size ));
	GetDlgItem(IDC_BUTTON_AngleOptions_SetDefault)->SetWindowText(GetResString( IDS_BUTTON_AngleOptions_SetDefault ));

	SetWindowText(GetResString( IDS_Angle_Options ));
}

BOOL CDlgAngleOptions::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CComboBox* pFontSize = (CComboBox*)(GetDlgItem(IDC_COMBO_AngleOptions_Font_Size));
	if( pFontSize != NULL )
	{
		char szBuf[50];
		for( int i=0; i<10; i++ )
		{
			sprintf( szBuf, "%02d", i*4 +12 );
			pFontSize->AddString( szBuf );
		}
		sprintf( szBuf, "%d", m_AO.nSize );
		pFontSize->SelectString( 0, szBuf );
	}
	m_bSetDefault = FALSE;

	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAngleOptions::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

	// Lets make a CDC for ease of use
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	ASSERT(pDC); // Attached failed
	// Save off context attributes
	int nIndexDC = pDC->SaveDC();

	CRect rc(lpDrawItemStruct->rcItem);
	pDC->SetBkMode(TRANSPARENT);

	COLORREF color;
	switch( nIDCtl )
	{
	case	IDC_BUTTON_AngleOptions_Color_Line:
		color	= m_AO.clrLine;
		break;
	case	IDC_BUTTON_AngleOptions_Color_Arc:
		color	= m_AO.clrArc;
		break;
	case	IDC_BUTTON_AngleOptions_Color_Text:
		color	= m_AO.clrText;
		break;
	default:
		color	= RGB(0,0,0);
	}
	CBrush brushFill, brushFrame;
	brushFill.CreateSolidBrush(color);
	brushFrame.CreateSolidBrush(RGB(0,0,0));
	pDC->FillRect(rc, &brushFill);
	pDC->FrameRect(rc, &brushFrame);

	brushFill.DeleteObject();
	brushFrame.DeleteObject();
	// Restore State of context
	pDC->RestoreDC(nIndexDC);

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDlgAngleOptions::OnAngleOptionsColorLine() 
{
	// TODO: Add your control notification handler code here
	CColorDialog ColorDlg(0, 0, this);
	if( ColorDlg.DoModal() == IDOK )
	{
		COLORREF color = ColorDlg.GetColor();
		if( color != m_AO.clrLine )
		{
			m_AO.clrLine = color;
			Invalidate();
		}
	}
}

void CDlgAngleOptions::OnAngleOptionsColorArc() 
{
	// TODO: Add your control notification handler code here
	CColorDialog ColorDlg(0, 0, this);
	if( ColorDlg.DoModal() == IDOK )
	{
		COLORREF color = ColorDlg.GetColor();
		if( color != m_AO.clrArc )
		{
			m_AO.clrArc = color;
			Invalidate();
		}
	}
}

void CDlgAngleOptions::OnAngleOptionsColorText() 
{
	// TODO: Add your control notification handler code here
	CColorDialog ColorDlg(0, 0, this);
	if( ColorDlg.DoModal() == IDOK )
	{
		COLORREF color = ColorDlg.GetColor();
		if( color != m_AO.clrText )
		{
			m_AO.clrText = color;
			Invalidate();
		}
	}
}

void CDlgAngleOptions::OnAngleOptionsSetDefault()
{
	m_bSetDefault = TRUE;
}

void CDlgAngleOptions::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	((CComboBox*)(GetDlgItem(IDC_COMBO_AngleOptions_Font_Size)))->GetWindowText( str );
	m_AO.nSize = atoi( str );
	CDialog::OnOK();
}
