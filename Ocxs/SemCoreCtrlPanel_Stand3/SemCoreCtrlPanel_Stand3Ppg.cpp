// SemCoreCtrlPanel_Stand3Ppg.cpp : Implementation of the CSemCoreCtrlPanel_Stand3PropPage property page class.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand3.h"
#include "SemCoreCtrlPanel_Stand3Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand3PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand3PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand3PropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand3PropPage, "SEMCORECTRLPANELSTAND3.SemCoreCtrlPanelStand3PropPage.1",
	0xc9a78b50, 0x69e7, 0x4d16, 0xb5, 0xd4, 0xd5, 0xba, 0x1e, 0xe, 0x99, 0xfb)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3PropPage::CSemCoreCtrlPanel_Stand3PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand3PropPage

BOOL CSemCoreCtrlPanel_Stand3PropPage::CSemCoreCtrlPanel_Stand3PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SEMCORECTRLPANEL_STAND3_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3PropPage::CSemCoreCtrlPanel_Stand3PropPage - Constructor

CSemCoreCtrlPanel_Stand3PropPage::CSemCoreCtrlPanel_Stand3PropPage() :
	COlePropertyPage(IDD, IDS_SEMCORECTRLPANEL_STAND3_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSemCoreCtrlPanel_Stand3PropPage)
	m_bCaptionVisible = FALSE;
	m_strCaption = _T("");
	m_bDebugInfoVisible = FALSE;
	m_bGradeVisible = FALSE;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3PropPage::DoDataExchange - Moves data between page and properties

void CSemCoreCtrlPanel_Stand3PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSemCoreCtrlPanel_Stand3PropPage)
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND3_CHECK_CaptionVisible, m_bCaptionVisible, _T("CaptionVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND3_CHECK_CaptionVisible, m_bCaptionVisible);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND3_EDIT_Caption, m_strCaption, _T("Caption") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND3_EDIT_Caption, m_strCaption);
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND3_CHECK_DebugInfoVisible, m_bDebugInfoVisible, _T("DebugInfoVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND3_CHECK_DebugInfoVisible, m_bDebugInfoVisible);
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND3_CHECK_GradeInfoVisible, m_bGradeVisible, _T("GradeVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND3_CHECK_GradeInfoVisible, m_bGradeVisible);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3PropPage message handlers
