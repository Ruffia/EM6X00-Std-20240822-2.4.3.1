// PageSys.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "PageSys.h"
#include "ScanView.h"
#include "AppOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSys property page

IMPLEMENT_DYNCREATE(CPageSys, CPropertyPage)

CPageSys::CPageSys() : CPropertyPage(CPageSys::IDD)
{
	//{{AFX_DATA_INIT(CPageSys)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPageSys::~CPageSys()
{
	m_Font.DeleteObject();
}

void CPageSys::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSys)
	DDX_Control(pDX, IDC_PAGE_SYS_Color_ComboBox_LineWidth, m_comboLineWidth);
	DDX_Control(pDX, IDC_PAGE_SYS_Font_ComboBox_UScaleText_Size, m_comboUScaleTextSize);
	DDX_Control(pDX, IDC_PAGE_SYS_Font_ComboBox_LabelText_Size, m_comboLabelTextSize);
	DDX_Control(pDX, IDC_PAGE_SYS_Font_ComboBox_EditText_Size, m_comboEditTextSize);
	DDX_Control(pDX, IDC_PAGE_SYS_Font_ComboBox_UScaleText_Name, m_comboUScaleText);
	DDX_Control(pDX, IDC_PAGE_SYS_Font_ComboBox_LabelText_Name, m_comboLabelText);
	DDX_Control(pDX, IDC_PAGE_SYS_Font_ComboBox_EditText_Name, m_comboEditText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSys, CPropertyPage)
	//{{AFX_MSG_MAP(CPageSys)
	ON_BN_CLICKED(IDC_PAGE_SYS_Color_Btn_Back_Sel, OnPAGESYSColorBtnBackSel)
	ON_BN_CLICKED(IDC_PAGE_SYS_Color_Btn_EditText_Sel, OnPAGESYSColorBtnEditTextSel)
	ON_BN_CLICKED(IDC_PAGE_SYS_Color_Btn_LabelText_Sel, OnPAGESYSColorBtnLabelTextSel)
	ON_BN_CLICKED(IDC_PAGE_SYS_Color_Btn_LabelLogoText_Sel, OnPAGESYSColorBtnLabelLogoTextSel)
	ON_BN_CLICKED(IDC_PAGE_SYS_Color_Btn_UScaleText_Sel, OnPAGESYSColorBtnUScaleTextSel)
	ON_BN_CLICKED(IDC_PAGE_SYS_Color_Btn_UScaleLine_Sel, OnPAGESYSColorBtnUScaleLineSel)
	ON_BN_CLICKED(IDC_PAGE_SYS_Color_Btn_OscGrid_Sel, OnPAGESYSColorBtnOscGridSel)
	ON_BN_CLICKED(IDC_PAGE_SYS_Color_Btn_OscGraph_Sel, OnPAGESYSColorBtnOscGraphSel)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_PAGE_SYS_Color_Btn_Default, OnPAGESYSColorBtnDefault)
	ON_CBN_SELCHANGE(IDC_PAGE_SYS_Font_ComboBox_EditText_Name, OnSelchangePAGESYSFontComboBoxEditTextName)
	ON_CBN_SELCHANGE(IDC_PAGE_SYS_Font_ComboBox_LabelText_Name, OnSelchangePAGESYSFontComboBoxLabelTextName)
	ON_CBN_SELCHANGE(IDC_PAGE_SYS_Font_ComboBox_UScaleText_Name, OnSelchangePAGESYSFontComboBoxUScaleTextName)
	ON_BN_CLICKED(IDC_PAGE_SYS_Font_Btn_Default, OnPAGESYSFontBtnDefault)
	ON_BN_CLICKED(IDC_PAGE_SYS_Color_Btn_Trans_Sel, OnPAGESYSColorBtnTransSel)
	ON_CBN_SELCHANGE(IDC_PAGE_SYS_Color_ComboBox_LineWidth, OnSelchangePAGESYSColorComboBoxLineWidth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPageSys::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_PAGE_SYS_STATIC_GROUP_Color)->SetWindowText(GetResString(IDS_PAGE_SYS_STATIC_GROUP_Color)); 
	GetDlgItem(IDC_PAGE_SYS_Color_Static_Back)->SetWindowText(GetResString(IDS_PAGE_SYS_Color_Static_Back)); 
	GetDlgItem(IDC_PAGE_SYS_Color_Static_EditText)->SetWindowText(GetResString(IDS_PAGE_SYS_Color_Static_EditText)); 
	GetDlgItem(IDC_PAGE_SYS_Color_Static_LabelText)->SetWindowText(GetResString(IDS_PAGE_SYS_Color_Static_LabelText)); 
	GetDlgItem(IDC_PAGE_SYS_Color_Static_LabelLogoText)->SetWindowText(GetResString(IDS_PAGE_SYS_Color_Static_LabelLogoText)); 
	GetDlgItem(IDC_PAGE_SYS_Color_Static_UScaleText)->SetWindowText(GetResString(IDS_PAGE_SYS_Color_Static_UScaleText)); 
 	GetDlgItem(IDC_PAGE_SYS_Color_Static_UScaleLine)->SetWindowText(GetResString(IDS_PAGE_SYS_Color_Static_UScaleLine)); 
	GetDlgItem(IDC_PAGE_SYS_Color_Static_OscGrid)->SetWindowText(GetResString(IDS_PAGE_SYS_Color_Static_OscGrid)); 
	GetDlgItem(IDC_PAGE_SYS_Color_Static_OscGraph)->SetWindowText(GetResString(IDS_PAGE_SYS_Color_Static_OscGraph)); 
	GetDlgItem(IDC_PAGE_SYS_Color_Btn_Default)->SetWindowText(GetResString(IDS_PAGE_SYS_Color_Btn_Default)); 
	GetDlgItem(IDC_PAGE_SYS_Color_Btn_Trans_Sel)->SetWindowText(GetResString(IDS_PAGE_SYS_Color_Static_Trans)); 

	GetDlgItem(IDC_PAGE_SYS_STATIC_GROUP_Font)->SetWindowText(GetResString(IDC_PAGE_SYS_STATIC_GROUP_Font)); 
	GetDlgItem(IDC_PAGE_SYS_Font_Static_EditText_Name)->SetWindowText(GetResString(IDS_PAGE_SYS_Font_Static_EditText_Name)); 
	GetDlgItem(IDC_PAGE_SYS_Font_Static_EditText_Size)->SetWindowText(GetResString(IDS_PAGE_SYS_Font_Static_EditText_Size)); 
	GetDlgItem(IDC_PAGE_SYS_Font_Static_LabelText_Name)->SetWindowText(GetResString(IDS_PAGE_SYS_Font_Static_LabelText_Name)); 
	GetDlgItem(IDC_PAGE_SYS_Font_Static_LabelText_Size)->SetWindowText(GetResString(IDS_PAGE_SYS_Font_Static_LabelText_Size)); 
	GetDlgItem(IDC_PAGE_SYS_Font_Static_UScaleText_Name)->SetWindowText(GetResString(IDS_PAGE_SYS_Font_Static_UScaleText_Name)); 
	GetDlgItem(IDC_PAGE_SYS_Font_Static_UScaleText_Size)->SetWindowText(GetResString(IDS_PAGE_SYS_Font_Static_UScaleText_Size)); 
	GetDlgItem(IDC_PAGE_SYS_Font_Btn_Default)->SetWindowText(GetResString(IDS_PAGE_SYS_Font_Btn_Default)); 
} 

/////////////////////////////////////////////////////////////////////////////
// CPageSys message handlers

BOOL CPageSys::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// ÑÕÉ«
	for(int i=0; i<10; i++ )
		m_sysOpt.color[i]	= CAppOption::Instance().m_sysOpt.color[i];
	m_sysOpt.nShowParams[SysOpt_showLabelTrans]	= CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showLabelTrans];
	((CButton*)(GetDlgItem(IDC_PAGE_SYS_Color_Btn_Trans_Sel)))->SetCheck( m_sysOpt.nShowParams[SysOpt_showLabelTrans] );
/*	// ×ÖÌå
	for(int i=0; i<3; i++ )
		m_sysOpt.fName[i]	= CAppOption::Instance().m_sysOpt.fName[i];
	m_comboEditText.Initialize();
	m_comboEditText.SelectString( 0, m_sysOpt.fName[SysOpt_nameEditText] );
	m_comboEditText.SetFontInUse( m_sysOpt.fName[SysOpt_nameEditText] );
	m_comboLabelText.Initialize();
	m_comboLabelText.SelectString( 0, m_sysOpt.fName[SysOpt_nameLabelText] );
	m_comboLabelText.SetFontInUse( m_sysOpt.fName[SysOpt_nameLabelText] );
	m_comboUScaleText.Initialize();
	m_comboUScaleText.SelectString( 0, m_sysOpt.fName[SysOpt_nameMobileUScaleText] );
	m_comboUScaleText.SetFontInUse( m_sysOpt.fName[SysOpt_nameMobileUScaleText] );
	// ×ÖÌå´óÐ¡
	for(int i=0; i<3; i++ )
		m_sysOpt.fSize[i] = CAppOption::Instance().m_sysOpt.fSize[i];
*/	// ÒÆ¶¯±ê³ßÏß¿í
	CString str;
	for(int i=0; i<8; i++ )
	{
		str.Format( "%d", i+1 );
		m_comboLineWidth.AddString( str );
	}
	m_sysOpt.nLineWidth = CAppOption::Instance().m_sysOpt.nLineWidth;
	m_comboLineWidth.SetCurSel( m_sysOpt.nLineWidth -1 );

	Localize();

	m_Font.CreateFont(-12, 0, 0, 0, FW_NORMAL, 0, 0, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");

	SetFont( &m_Font, TRUE );
	CWnd *pw = GetWindow(GW_CHILD);
	while(pw != NULL)
	{   
		pw->SetFont( &m_Font);
		pw = pw->GetWindow(GW_HWNDNEXT);
	}; 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageSys::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
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
	case	IDC_PAGE_SYS_Color_Btn_Back_Sel:
		color	= m_sysOpt.color[SysOpt_clrBack];
		break;
	case	IDC_PAGE_SYS_Color_Btn_EditText_Sel:
		color	= m_sysOpt.color[SysOpt_clrEditText];
		break;
	case	IDC_PAGE_SYS_Color_Btn_LabelText_Sel:
		color	= m_sysOpt.color[SysOpt_clrLabelText];
		break;
	case	IDC_PAGE_SYS_Color_Btn_LabelLogoText_Sel:
		color	= m_sysOpt.color[SysOpt_clrLabelLogoText];
		break;
	case	IDC_PAGE_SYS_Color_Btn_UScaleText_Sel:
		color	= m_sysOpt.color[SysOpt_clrMobileUScaleText];
		break;
	case	IDC_PAGE_SYS_Color_Btn_UScaleLine_Sel:
		color	= m_sysOpt.color[SysOpt_clrMobileUScaleLine];
		break;
	case	IDC_PAGE_SYS_Color_Btn_OscGrid_Sel:
		color	= m_sysOpt.color[SysOpt_clrOscillographGrid];
		break;
	case	IDC_PAGE_SYS_Color_Btn_OscGraph_Sel:
		color	= m_sysOpt.color[SysOpt_clrOscillographGraph];
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

	CPropertyPage::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CPageSys::OnPAGESYSColorBtnDefault() 
{
	// TODO: Add your control notification handler code here
	m_sysOpt.color[SysOpt_clrBack]				= RGB(0,0,127);
	m_sysOpt.color[SysOpt_clrEditText]			= RGB(155,255,255);
	m_sysOpt.color[SysOpt_clrLabelText]			= RGB(155,255,255);
	m_sysOpt.color[SysOpt_clrLabelLogoText]		= RGB(155,255,255);
	m_sysOpt.color[SysOpt_clrMobileUScaleText]	= RGB(255,255,155);
	m_sysOpt.color[SysOpt_clrMobileUScaleLine]	= RGB(255,0,0);
	m_sysOpt.color[SysOpt_clrOscillographGrid]	= RGB(0,255,0);
	m_sysOpt.color[SysOpt_clrOscillographGraph]	= RGB(255,255,155);
	m_sysOpt.nLineWidth = 3;
	SetModified();
	m_comboLineWidth.SetCurSel( m_sysOpt.nLineWidth -1 );
	Invalidate();
}

void CPageSys::OnPAGESYSFontBtnDefault() 
{
	// TODO: Add your control notification handler code here
	m_sysOpt.fName[SysOpt_nameEditText]			= "Î¢ÈíÑÅºÚ";
	m_sysOpt.fName[SysOpt_nameLabelText]		= "Î¢ÈíÑÅºÚ";
	m_sysOpt.fName[SysOpt_nameMobileUScaleText]	= "Î¢ÈíÑÅºÚ";
	m_sysOpt.fSize[SysOpt_sizeEditText]			= 32;
	m_sysOpt.fSize[SysOpt_sizeLabelText]		= 28;
	m_sysOpt.fSize[SysOpt_sizeMobileUScaleText]	= 32;
	SetModified();
	m_comboEditText.SelectString( 0, m_sysOpt.fName[SysOpt_nameEditText] );
	m_comboEditText.SetFontInUse( m_sysOpt.fName[SysOpt_nameEditText] );
	m_comboLabelText.SelectString( 0, m_sysOpt.fName[SysOpt_nameLabelText] );
	m_comboLabelText.SetFontInUse( m_sysOpt.fName[SysOpt_nameLabelText] );
	m_comboUScaleText.SelectString( 0, m_sysOpt.fName[SysOpt_nameMobileUScaleText] );
	m_comboUScaleText.SetFontInUse( m_sysOpt.fName[SysOpt_nameMobileUScaleText] );
	Invalidate();
}

void CPageSys::OnPAGESYSColorBtnBackSel() 
{
	// TODO: Add your control notification handler code here
	CColorDialog ColorDlg(0, 0, this);
	if( ColorDlg.DoModal() == IDOK )
	{
		COLORREF color = ColorDlg.GetColor();
		if( color != m_sysOpt.color[SysOpt_clrBack] )
		{
			m_sysOpt.color[SysOpt_clrBack] = color;
			SetModified();
			Invalidate();
		}
	}
}

void CPageSys::OnSelchangePAGESYSColorComboBoxLineWidth() 
{
	// TODO: Add your control notification handler code here
	if( m_comboLineWidth.GetCurSel() != m_sysOpt.nLineWidth -1 )
	{
		m_sysOpt.nLineWidth = m_comboLineWidth.GetCurSel() +1;
		SetModified();
	}
}

void CPageSys::OnPAGESYSColorBtnEditTextSel() 
{
	// TODO: Add your control notification handler code here
	CColorDialog ColorDlg(0, 0, this);
	if( ColorDlg.DoModal() == IDOK )
	{
		COLORREF color = ColorDlg.GetColor();
		if( color != m_sysOpt.color[SysOpt_clrEditText] )
		{
			m_sysOpt.color[SysOpt_clrEditText] = color;
			SetModified();
			Invalidate();
		}
	}
}

void CPageSys::OnPAGESYSColorBtnLabelTextSel() 
{
	// TODO: Add your control notification handler code here
	CColorDialog ColorDlg(0, 0, this);
	if( ColorDlg.DoModal() == IDOK )
	{
		COLORREF color = ColorDlg.GetColor();
		if( color != m_sysOpt.color[SysOpt_clrLabelText] )
		{
			m_sysOpt.color[SysOpt_clrLabelText] = color;
			SetModified();
			Invalidate();
		}
	}
}

void CPageSys::OnPAGESYSColorBtnLabelLogoTextSel() 
{
	// TODO: Add your control notification handler code here
	CColorDialog ColorDlg(0, 0, this);
	if( ColorDlg.DoModal() == IDOK )
	{
		COLORREF color = ColorDlg.GetColor();
		if( color != m_sysOpt.color[SysOpt_clrLabelLogoText] )
		{
			m_sysOpt.color[SysOpt_clrLabelLogoText] = color;
			SetModified();
			Invalidate();
		}
	}
}

void CPageSys::OnPAGESYSColorBtnUScaleTextSel() 
{
	// TODO: Add your control notification handler code here
	CColorDialog ColorDlg(0, 0, this);
	if( ColorDlg.DoModal() == IDOK )
	{
		COLORREF color = ColorDlg.GetColor();
		if( color != m_sysOpt.color[SysOpt_clrMobileUScaleText] )
		{
			m_sysOpt.color[SysOpt_clrMobileUScaleText] = color;
			SetModified();
			Invalidate();
		}
	}
}

void CPageSys::OnPAGESYSColorBtnUScaleLineSel() 
{
	// TODO: Add your control notification handler code here
	CColorDialog ColorDlg(0, 0, this);
	if( ColorDlg.DoModal() == IDOK )
	{
		COLORREF color = ColorDlg.GetColor();
		if( color != m_sysOpt.color[SysOpt_clrMobileUScaleLine] )
		{
			m_sysOpt.color[SysOpt_clrMobileUScaleLine] = color;
			SetModified();
			Invalidate();
		}
	}
}

void CPageSys::OnPAGESYSColorBtnOscGridSel() 
{
	// TODO: Add your control notification handler code here
	CColorDialog ColorDlg(0, 0, this);
	if( ColorDlg.DoModal() == IDOK )
	{
		COLORREF color = ColorDlg.GetColor();
		if( color != m_sysOpt.color[SysOpt_clrOscillographGrid] )
		{
			m_sysOpt.color[SysOpt_clrOscillographGrid] = color;
			SetModified();
			Invalidate();
		}
	}
}

void CPageSys::OnPAGESYSColorBtnOscGraphSel() 
{
	// TODO: Add your control notification handler code here
	CColorDialog ColorDlg(0, 0, this);
	if( ColorDlg.DoModal() == IDOK )
	{
		COLORREF color = ColorDlg.GetColor();
		if( color != m_sysOpt.color[SysOpt_clrOscillographGraph] )
		{
			m_sysOpt.color[SysOpt_clrOscillographGraph] = color;
			SetModified();
			Invalidate();
		}
	}
}

void CPageSys::OnPAGESYSColorBtnTransSel() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_PAGE_SYS_Color_Btn_Trans_Sel)))->GetCheck();
	if( nCheck != m_sysOpt.nShowParams[SysOpt_showLabelTrans] )
	{
		m_sysOpt.nShowParams[SysOpt_showLabelTrans] = nCheck;
		SetModified();
	}
}

void CPageSys::OnSelchangePAGESYSFontComboBoxEditTextName() 
{
	// TODO: Add your control notification handler code here
	CString strName = m_comboEditText.m_strDefault;
	if( strName != m_sysOpt.fName[SysOpt_nameEditText] )
	{
		m_sysOpt.fName[SysOpt_nameEditText] = strName;
		SetModified();
	}
}

void CPageSys::OnSelchangePAGESYSFontComboBoxLabelTextName() 
{
	// TODO: Add your control notification handler code here
	CString strName = m_comboLabelText.m_strDefault;
	if( strName != m_sysOpt.fName[SysOpt_nameLabelText] )
	{
		m_sysOpt.fName[SysOpt_nameLabelText] = strName;
		SetModified();
	}
}

void CPageSys::OnSelchangePAGESYSFontComboBoxUScaleTextName() 
{
	// TODO: Add your control notification handler code here
	CString strName = m_comboUScaleText.m_strDefault;
	if( strName != m_sysOpt.fName[SysOpt_nameMobileUScaleText] )
	{
		m_sysOpt.fName[SysOpt_nameMobileUScaleText] = strName;
		SetModified();
	}
}

void CPageSys::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bSystemSettingChanged = CAppOption::Instance().SystemSettingsChanged(m_sysOpt);
	if( bSystemSettingChanged )
	{
		CScanView* pView = (CScanView*)theApp.GetScanView();
		if( pView )
			pView->SendMessage(WM_PARAM_CHANGED, (WPARAM)this, 2);
	}
	else
	{
		SetModified( FALSE );
		return;
	}
	CPropertyPage::OnOK();
}
