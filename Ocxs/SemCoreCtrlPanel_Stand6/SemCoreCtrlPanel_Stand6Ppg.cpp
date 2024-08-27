// SemCoreCtrlPanel_Stand6Ppg.cpp : Implementation of the CSemCoreCtrlPanel_Stand6PropPage property page class.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand6.h"
#include "SemCoreCtrlPanel_Stand6Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand6PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand6PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand6PropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand6PropPage, "SEMCORECTRLPANELSTAND6.SemCoreCtrlPanelStand6PropPage.1",
	0x4941c21e, 0xa8c4, 0x494a, 0xab, 0xe0, 0x9f, 0x62, 0x71, 0xad, 0x13, 0xa6)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6PropPage::CSemCoreCtrlPanel_Stand6PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand6PropPage

BOOL CSemCoreCtrlPanel_Stand6PropPage::CSemCoreCtrlPanel_Stand6PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SEMCORECTRLPANEL_STAND6_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6PropPage::CSemCoreCtrlPanel_Stand6PropPage - Constructor

CSemCoreCtrlPanel_Stand6PropPage::CSemCoreCtrlPanel_Stand6PropPage() :
	COlePropertyPage(IDD, IDS_SEMCORECTRLPANEL_STAND6_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSemCoreCtrlPanel_Stand6PropPage)
	m_bCaptionVisible = FALSE;
	m_strXMin = _T("");
	m_strXMax = _T("");
	m_strCaption = _T("");
	m_bDebugInfoVisible = FALSE;
	m_bGradeVisible = FALSE;
	m_bAutoVisible = FALSE;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6PropPage::DoDataExchange - Moves data between page and properties

void CSemCoreCtrlPanel_Stand6PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSemCoreCtrlPanel_Stand6PropPage)
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND6_CHECK_CaptionVisible, m_bCaptionVisible, _T("CaptionVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND6_CHECK_CaptionVisible, m_bCaptionVisible);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND6_EDIT_XMin, m_strXMin, _T("XMinString") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND6_EDIT_XMin, m_strXMin);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND6_EDIT_XMax, m_strXMax, _T("XMaxString") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND6_EDIT_XMax, m_strXMax);
	DDP_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND6_EDIT_Caption, m_strCaption, _T("Caption") );
	DDX_Text(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND6_EDIT_Caption, m_strCaption);
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND6_CHECK_DebugInfoVisible, m_bDebugInfoVisible, _T("DebugInfoVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND6_CHECK_DebugInfoVisible, m_bDebugInfoVisible);
	DDP_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND6_CHECK_GradeInfoVisible, m_bGradeVisible, _T("GradeVisible") );
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND6_CHECK_GradeInfoVisible, m_bGradeVisible);
	DDX_Check(pDX, IDC_PROPPAGE_SEMCORECTRLPANEL_STAND6_CHECK_AutoVisible, m_bAutoVisible);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6PropPage message handlers
