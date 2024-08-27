// SemCoreCtrlPanel_Stand7Ppg.cpp : Implementation of the CSemCoreCtrlPanel_Stand7PropPage property page class.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand7.h"
#include "SemCoreCtrlPanel_Stand7Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand7PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand7PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand7PropPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand7PropPage, "SEMCORECTRLPANELSTAND7.SemCoreCtrlPanelStand7PropPage.1",
	0x8a82781e, 0x8041, 0x4b4a, 0x95, 0x9a, 0x5c, 0x2, 0x1d, 0x90, 0x2, 0xde)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand7PropPage::CSemCoreCtrlPanel_Stand7PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand7PropPage

BOOL CSemCoreCtrlPanel_Stand7PropPage::CSemCoreCtrlPanel_Stand7PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SEMCORECTRLPANEL_STAND7_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand7PropPage::CSemCoreCtrlPanel_Stand7PropPage - Constructor

CSemCoreCtrlPanel_Stand7PropPage::CSemCoreCtrlPanel_Stand7PropPage() :
	COlePropertyPage(IDD, IDS_SEMCORECTRLPANEL_STAND7_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSemCoreCtrlPanel_Stand7PropPage)
	m_bCaptionVisible = FALSE;
	m_strCaption = _T("");
	m_bDebugInfoVisible = FALSE;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand7PropPage::DoDataExchange - Moves data between page and properties

void CSemCoreCtrlPanel_Stand7PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSemCoreCtrlPanel_Stand7PropPage)
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND7_CHECK_CaptionVisible, m_bCaptionVisible, _T("CaptionVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND7_CHECK_CaptionVisible, m_bCaptionVisible);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND7_EDIT_Caption, m_strCaption, _T("Caption") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND7_EDIT_Caption, m_strCaption);
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND7_CHECK_DebugInfoVisible, m_bDebugInfoVisible, _T("DebugInfoVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND7_CHECK_DebugInfoVisible, m_bDebugInfoVisible);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand7PropPage message handlers

BOOL CSemCoreCtrlPanel_Stand7PropPage::OnInitDialog() 
{
	COlePropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
