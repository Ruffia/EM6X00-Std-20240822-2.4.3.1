// SemCoreCtrlPanel_StandAPpg.cpp : Implementation of the CSemCoreCtrlPanel_StandAPropPage property page class.

#include "stdafx.h"
#include "SemCoreCtrlPanel_StandA.h"
#include "SemCoreCtrlPanel_StandAPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_StandAPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_StandAPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_StandAPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_StandAPropPage, "SEMCORECTRLPANELSTANDA.SemCoreCtrlPanelStandAPropPage.1",
	0x384affed, 0xc72a, 0x40eb, 0xa9, 0x74, 0x7a, 0x9b, 0xf4, 0x84, 0x95, 0x5b)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandAPropPage::CSemCoreCtrlPanel_StandAPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_StandAPropPage

BOOL CSemCoreCtrlPanel_StandAPropPage::CSemCoreCtrlPanel_StandAPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SEMCORECTRLPANEL_STANDA_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandAPropPage::CSemCoreCtrlPanel_StandAPropPage - Constructor

CSemCoreCtrlPanel_StandAPropPage::CSemCoreCtrlPanel_StandAPropPage() :
	COlePropertyPage(IDD, IDS_SEMCORECTRLPANEL_STANDA_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSemCoreCtrlPanel_StandAPropPage)
	m_bCaptionVisible = FALSE;
	m_strCaption = _T("");
	m_bDebugInfoVisible = FALSE;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandAPropPage::DoDataExchange - Moves data between page and properties

void CSemCoreCtrlPanel_StandAPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSemCoreCtrlPanel_StandAPropPage)
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STANDA_CHECK_CaptionVisible, m_bCaptionVisible, _T("CaptionVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STANDA_CHECK_CaptionVisible, m_bCaptionVisible);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STANDA_EDIT_Caption, m_strCaption, _T("Caption") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STANDA_EDIT_Caption, m_strCaption);
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STANDA_CHECK_DebugInfoVisible, m_bDebugInfoVisible, _T("DebugInfoVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STANDA_CHECK_DebugInfoVisible, m_bDebugInfoVisible);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandAPropPage message handlers
