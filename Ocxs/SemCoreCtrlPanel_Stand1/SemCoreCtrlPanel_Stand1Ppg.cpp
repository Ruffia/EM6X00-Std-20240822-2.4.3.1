// SemCoreCtrlPanel_Stand1Ppg.cpp : Implementation of the CSemCoreCtrlPanel_Stand1PropPage property page class.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand1.h"
#include "SemCoreCtrlPanel_Stand1Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand1PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand1PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand1PropPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand1PropPage, "SEMCORECTRLPANELSTAND1.SemCoreCtrlPanelStand1PropPage.1",
	0xf890a9f8, 0xc25d, 0x4e0f, 0x87, 0x7a, 0x9e, 0x6, 0x23, 0xd1, 0x93, 0xde)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1PropPage::CSemCoreCtrlPanel_Stand1PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand1PropPage

BOOL CSemCoreCtrlPanel_Stand1PropPage::CSemCoreCtrlPanel_Stand1PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SEMCORECTRLPANEL_STAND1_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1PropPage::CSemCoreCtrlPanel_Stand1PropPage - Constructor

CSemCoreCtrlPanel_Stand1PropPage::CSemCoreCtrlPanel_Stand1PropPage() :
	COlePropertyPage(IDD, IDS_SEMCORECTRLPANEL_STAND1_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSemCoreCtrlPanel_Stand1PropPage)
	m_bCaptionVisible = FALSE;
	m_strXMin = _T("");
	m_strXMax = _T("");
	m_strCaption = _T("");
	m_bDebugInfoVisible = FALSE;
	m_nStyle = 0;
	//}}AFX_DATA_INIT

}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1PropPage::DoDataExchange - Moves data between page and properties

void CSemCoreCtrlPanel_Stand1PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSemCoreCtrlPanel_Stand1PropPage)
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_CHECK_CaptionVisible, m_bCaptionVisible, _T("CaptionVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_CHECK_CaptionVisible, m_bCaptionVisible);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_EDIT_XMin, m_strXMin, _T("XMinString") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_EDIT_XMin, m_strXMin);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_EDIT_XMax, m_strXMax, _T("XMaxString") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_EDIT_XMax, m_strXMax);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_EDIT_Caption, m_strCaption, _T("Caption") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_EDIT_Caption, m_strCaption);
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_CHECK_DebugInfoVisible, m_bDebugInfoVisible, _T("DebugInfoVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_CHECK_DebugInfoVisible, m_bDebugInfoVisible);
	DDP_Radio(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_RADIO_Horizontal, m_nStyle, _T("ControlStyle") );
	DDX_Radio(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_RADIO_Horizontal, m_nStyle);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);

}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1PropPage message handlers

BOOL CSemCoreCtrlPanel_Stand1PropPage::OnInitDialog() 
{
	COlePropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	if ( m_nStyle == 0 )
	{
		((CButton *)GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_RADIO_Horizontal))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_RADIO_Vertical))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_RADIO_BtnLabel))->SetCheck(0);
	}
	else if (m_nStyle == 1 )
	{
		((CButton *)GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_RADIO_Horizontal))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_RADIO_Vertical))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_RADIO_BtnLabel))->SetCheck(0);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_RADIO_Horizontal))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_RADIO_Vertical))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_PROPPAGE_SEMCORECTRLPANEL_STAND1_RADIO_BtnLabel))->SetCheck(1);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
