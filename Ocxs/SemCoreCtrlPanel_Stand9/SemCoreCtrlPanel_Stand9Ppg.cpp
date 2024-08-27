// SemCoreCtrlPanel_Stand9Ppg.cpp : Implementation of the CSemCoreCtrlPanel_Stand9PropPage property page class.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand9.h"
#include "SemCoreCtrlPanel_Stand9Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand9PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand9PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand9PropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand9PropPage, "SEMCORECTRLPANELSTAND9.SemCoreCtrlPanelStand9PropPage.1",
	0xe8a7a5f, 0xf299, 0x404b, 0xb2, 0x38, 0x67, 0x4e, 0x45, 0x20, 0x3e, 0xc)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand9PropPage::CSemCoreCtrlPanel_Stand9PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand9PropPage

BOOL CSemCoreCtrlPanel_Stand9PropPage::CSemCoreCtrlPanel_Stand9PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SEMCORECTRLPANEL_STAND9_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand9PropPage::CSemCoreCtrlPanel_Stand9PropPage - Constructor

CSemCoreCtrlPanel_Stand9PropPage::CSemCoreCtrlPanel_Stand9PropPage() :
	COlePropertyPage(IDD, IDS_SEMCORECTRLPANEL_STAND9_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSemCoreCtrlPanel_Stand9PropPage)
	m_bCaptionVisible = FALSE;
	m_strCaption = _T("");
	m_bDebugInfoVisible = FALSE;
	m_bNewDisplayMode = FALSE;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand9PropPage::DoDataExchange - Moves data between page and properties

void CSemCoreCtrlPanel_Stand9PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSemCoreCtrlPanel_Stand9PropPage)
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND9_CHECK_CaptionVisible, m_bCaptionVisible, _T("CaptionVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND9_CHECK_CaptionVisible, m_bCaptionVisible);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND9_EDIT_Caption, m_strCaption, _T("Caption") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND9_EDIT_Caption, m_strCaption);
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND9_CHECK_DebugInfoVisible, m_bDebugInfoVisible, _T("DebugInfoVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND9_CHECK_DebugInfoVisible, m_bDebugInfoVisible);
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND9_CHECK_DisplayMode, m_bNewDisplayMode, _T("NewDisplayMode") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND9_CHECK_DisplayMode, m_bNewDisplayMode);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand9PropPage message handlers
