// SemCoreCtrlPanel_Stand8Ppg.cpp : Implementation of the CSemCoreCtrlPanel_Stand8PropPage property page class.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand8.h"
#include "SemCoreCtrlPanel_Stand8Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand8PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand8PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand8PropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand8PropPage, "SEMCORECTRLPANELSTAND8.SemCoreCtrlPanelStand8PropPage.1",
	0xda7b3d6a, 0x792d, 0x4e7f, 0x83, 0x13, 0x37, 0xf8, 0xa1, 0x31, 0x63, 0xdf)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8PropPage::CSemCoreCtrlPanel_Stand8PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand8PropPage

BOOL CSemCoreCtrlPanel_Stand8PropPage::CSemCoreCtrlPanel_Stand8PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SEMCORECTRLPANEL_STAND8_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8PropPage::CSemCoreCtrlPanel_Stand8PropPage - Constructor

CSemCoreCtrlPanel_Stand8PropPage::CSemCoreCtrlPanel_Stand8PropPage() :
	COlePropertyPage(IDD, IDS_SEMCORECTRLPANEL_STAND8_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSemCoreCtrlPanel_Stand8PropPage)
	m_bCaptionVisible = FALSE;
	m_strCaption = _T("");
	m_bDebugInfoVisible = FALSE;
	m_strProgressMessage = _T("");
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8PropPage::DoDataExchange - Moves data between page and properties

void CSemCoreCtrlPanel_Stand8PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSemCoreCtrlPanel_Stand8PropPage)
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND8_CHECK_CaptionVisible, m_bCaptionVisible, _T("CaptionVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND8_CHECK_CaptionVisible, m_bCaptionVisible);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND8_EDIT_Caption, m_strCaption, _T("Caption") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND8_EDIT_Caption, m_strCaption);
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND8_CHECK_DebugInfoVisible, m_bDebugInfoVisible, _T("DebugInfoVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND8_CHECK_DebugInfoVisible, m_bDebugInfoVisible);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND8_EDIT_ProgressMessage, m_strProgressMessage, _T("ProgressMessage") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND8_EDIT_ProgressMessage, m_strProgressMessage);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8PropPage message handlers
