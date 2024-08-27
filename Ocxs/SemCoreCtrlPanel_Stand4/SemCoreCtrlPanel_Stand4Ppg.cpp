// SemCoreCtrlPanel_Stand4Ppg.cpp : Implementation of the CSemCoreCtrlPanel_Stand4PropPage property page class.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand4.h"
#include "SemCoreCtrlPanel_Stand4Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand4PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand4PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand4PropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand4PropPage, "SEMCORECTRLPANELSTAND4.SemCoreCtrlPanelStand4PropPage.1",
	0xe024c321, 0xa742, 0x4aa7, 0xbb, 0x3c, 0x58, 0x3c, 0x2, 0x3f, 0x52, 0x77)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand4PropPage::CSemCoreCtrlPanel_Stand4PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand4PropPage

BOOL CSemCoreCtrlPanel_Stand4PropPage::CSemCoreCtrlPanel_Stand4PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SEMCORECTRLPANEL_STAND4_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand4PropPage::CSemCoreCtrlPanel_Stand4PropPage - Constructor

CSemCoreCtrlPanel_Stand4PropPage::CSemCoreCtrlPanel_Stand4PropPage() :
	COlePropertyPage(IDD, IDS_SEMCORECTRLPANEL_STAND4_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSemCoreCtrlPanel_Stand4PropPage)
	m_strCaption = _T("");
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand4PropPage::DoDataExchange - Moves data between page and properties

void CSemCoreCtrlPanel_Stand4PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSemCoreCtrlPanel_Stand4PropPage)
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND4_EDIT_Caption, m_strCaption, _T("Caption") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND4_EDIT_Caption, m_strCaption);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand4PropPage message handlers
