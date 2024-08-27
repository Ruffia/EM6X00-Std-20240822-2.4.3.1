// SemCoreCtrlPanel_Stand5Ppg.cpp : Implementation of the CSemCoreCtrlPanel_Stand5PropPage property page class.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand5.h"
#include "SemCoreCtrlPanel_Stand5Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand5PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand5PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand5PropPage)
	ON_BN_CLICKED(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_RADIO_INT, OnRadioInt)
	ON_BN_CLICKED(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_RADIO_FLOAT, OnRadioFloat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand5PropPage, "SEMCORECTRLPANELSTAND5.SemCoreCtrlPanelStand5PropPage.1",
	0xdbf3e2c0, 0xb2cd, 0x48e8, 0xb7, 0x22, 0x8e, 0x9b, 0x66, 0x79, 0x65, 0xd6)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5PropPage::CSemCoreCtrlPanel_Stand5PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand5PropPage

BOOL CSemCoreCtrlPanel_Stand5PropPage::CSemCoreCtrlPanel_Stand5PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SEMCORECTRLPANEL_STAND5_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5PropPage::CSemCoreCtrlPanel_Stand5PropPage - Constructor

CSemCoreCtrlPanel_Stand5PropPage::CSemCoreCtrlPanel_Stand5PropPage() :
	COlePropertyPage(IDD, IDS_SEMCORECTRLPANEL_STAND5_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSemCoreCtrlPanel_Stand5PropPage)
	m_strCaption = _T("");
	m_nStyle = 0;
	m_nScaleDecimals = 0;
	m_nValueDecimals = 0;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5PropPage::DoDataExchange - Moves data between page and properties

void CSemCoreCtrlPanel_Stand5PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSemCoreCtrlPanel_Stand5PropPage)
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_EDIT_Caption, m_strCaption, _T("Caption") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_EDIT_Caption, m_strCaption);
	DDV_MaxChars(pDX, m_strCaption, 256);
	DDP_Radio(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_RADIO_INT, m_nStyle, _T("ControlStyle") );
	DDX_Radio(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_RADIO_INT, m_nStyle);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_EDIT_SCALE, m_nScaleDecimals, _T("ScaleDecimals") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_EDIT_SCALE, m_nScaleDecimals);
	DDV_MinMaxInt(pDX, m_nScaleDecimals, 0, 10);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_EDIT_VALUE, m_nValueDecimals, _T("ValueDecimals") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_EDIT_VALUE, m_nValueDecimals);
	DDV_MinMaxInt(pDX, m_nValueDecimals, 0, 10);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5PropPage message handlers

BOOL CSemCoreCtrlPanel_Stand5PropPage::OnInitDialog() 
{
	COlePropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	if ( m_nStyle == 0 )
	{
		((CButton *)GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_RADIO_INT))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_RADIO_FLOAT))->SetCheck(0);

		GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_EDIT_SCALE)->EnableWindow( FALSE );
		GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_EDIT_VALUE)->EnableWindow( FALSE );
	}
	else
	{
		((CButton *)GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_RADIO_INT))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_RADIO_FLOAT))->SetCheck(1);

		GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_EDIT_SCALE)->EnableWindow();
		GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_EDIT_VALUE)->EnableWindow();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSemCoreCtrlPanel_Stand5PropPage::OnRadioInt() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_EDIT_SCALE)->EnableWindow( FALSE );
	GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_EDIT_VALUE)->EnableWindow( FALSE );
}

void CSemCoreCtrlPanel_Stand5PropPage::OnRadioFloat() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_EDIT_SCALE)->EnableWindow();
	GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND5_EDIT_VALUE)->EnableWindow();
}
