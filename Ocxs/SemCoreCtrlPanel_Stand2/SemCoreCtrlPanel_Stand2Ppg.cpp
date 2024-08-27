// SemCoreCtrlPanel_Stand2Ppg.cpp : Implementation of the CSemCoreCtrlPanel_Stand2PropPage property page class.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand2.h"
#include "SemCoreCtrlPanel_Stand2Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand2PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand2PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand2PropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand2PropPage, "SEMCORECTRLPANELSTAND2.SemCoreCtrlPanelStand2PropPage.1",
	0xefecaabe, 0xd384, 0x4258, 0xa7, 0x1d, 0x1, 0x6c, 0xac, 0x43, 0xb7, 0x3b)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand2PropPage::CSemCoreCtrlPanel_Stand2PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand2PropPage

BOOL CSemCoreCtrlPanel_Stand2PropPage::CSemCoreCtrlPanel_Stand2PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SEMCORECTRLPANEL_STAND2_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand2PropPage::CSemCoreCtrlPanel_Stand2PropPage - Constructor

CSemCoreCtrlPanel_Stand2PropPage::CSemCoreCtrlPanel_Stand2PropPage() :
	COlePropertyPage(IDD, IDS_SEMCORECTRLPANEL_STAND2_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSemCoreCtrlPanel_Stand2PropPage)
	m_bCaptionVisible = FALSE;
	m_strXMin = _T("");
	m_strXMax = _T("");
	m_strCaption = _T("");
	m_bDebugInfoVisible = FALSE;
	m_strYMax = _T("");
	m_strYMin = _T("");
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand2PropPage::DoDataExchange - Moves data between page and properties

void CSemCoreCtrlPanel_Stand2PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSemCoreCtrlPanel_Stand2PropPage)
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND2_CHECK_CaptionVisible, m_bCaptionVisible, _T("CaptionVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND2_CHECK_CaptionVisible, m_bCaptionVisible);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND2_EDIT_XMin, m_strXMin, _T("XMinString") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND2_EDIT_XMin, m_strXMin);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND2_EDIT_XMax, m_strXMax, _T("XMaxString") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND2_EDIT_XMax, m_strXMax);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND2_EDIT_Caption, m_strCaption, _T("Caption") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND2_EDIT_Caption, m_strCaption);
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND2_CHECK_DebugInfoVisible, m_bDebugInfoVisible, _T("DebugInfoVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND2_CHECK_DebugInfoVisible, m_bDebugInfoVisible);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND2_EDIT_YMax, m_strYMax, _T("YMaxString") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND2_EDIT_YMax, m_strYMax);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND2_EDIT_YMin, m_strYMin, _T("YMinString") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND2_EDIT_YMin, m_strYMin);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand2PropPage message handlers
