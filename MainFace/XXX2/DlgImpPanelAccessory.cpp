// DlgImpPanelAccessory.cpp : implementation file
//
// Last Modify : 2010.05.20

#include "stdafx.h"
#include "XXX2.h"
#include "DlgImpPanelAccessory.h"
#include "ScanView.h"
#include "ControlLayer.h"
#include "ScaleManager.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelAccessory dialog


CDlgImpPanelAccessory::CDlgImpPanelAccessory(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImpPanelAccessory::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImpPanelAccessory)
	//}}AFX_DATA_INIT
}


void CDlgImpPanelAccessory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImpPanelAccessory)
	DDX_Control(pDX, IDC_STATIC_IMP_PANEL_Group_Accessory, m_staticGroup);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_DObjectiveLens, m_ctrlDObjectiveLens);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND7CTRL_DObjectiveLens, m_ctrlDObjectiveLensSwitch);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_DFocus, m_ctrlDFocus);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND7CTRL_DFocus, m_ctrlDFocusSwitch);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_YAdjust, m_ctrlYAdjust);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND7CTRL_SACP, m_ctrlSACP);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND7CTRL_ImageReverse, m_ctrlImageReverse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImpPanelAccessory, CDialog)
	//{{AFX_MSG_MAP(CDlgImpPanelAccessory)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgImpPanelAccessory, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgImpPanelAccessory)
	ON_EVENT(CDlgImpPanelAccessory, IDC_SEMCORECTRLPANELSTAND1CTRL_DObjectiveLens, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAccessory, IDC_SEMCORECTRLPANELSTAND1CTRL_DFocus, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAccessory, IDC_SEMCORECTRLPANELSTAND1CTRL_YAdjust, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAccessory, IDC_SEMCORECTRLPANELSTAND7CTRL_DObjectiveLens, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAccessory, IDC_SEMCORECTRLPANELSTAND7CTRL_DFocus, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAccessory, IDC_SEMCORECTRLPANELSTAND7CTRL_SACP, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelAccessory, IDC_SEMCORECTRLPANELSTAND7CTRL_ImageReverse, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelAccessory message handlers

void CDlgImpPanelAccessory::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	m_ctrlDObjectiveLens.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_DObjectiveLens)); 
	m_ctrlDObjectiveLensSwitch.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND7CTRL_DObjectiveLens)); 
	m_ctrlDFocus.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_DFocus)); 
	m_ctrlDFocusSwitch.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND7CTRL_DFocus)); 
	m_ctrlYAdjust.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_YAdjust)); 
	m_ctrlSACP.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND7CTRL_SACP)); 
	m_ctrlImageReverse.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND7CTRL_ImageReverse)); 

	VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
	Flag.lVal = 1;
	ClassID = m_ctrlSACP.GetPanelClassID();
	SerialNumber = m_ctrlSACP.GetPanelSerialNumber();
	CSemCtrlPanelManager::Instance().SyncSemCoreCtrlPanel( ClassID, SerialNumber, Xpos, Ypos, Flag );
	ClassID = m_ctrlImageReverse.GetPanelClassID();
	SerialNumber = m_ctrlImageReverse.GetPanelSerialNumber();
	CSemCtrlPanelManager::Instance().SyncSemCoreCtrlPanel( ClassID, SerialNumber, Xpos, Ypos, Flag );
	ClassID = m_ctrlDObjectiveLensSwitch.GetPanelClassID();
	SerialNumber = m_ctrlDObjectiveLensSwitch.GetPanelSerialNumber();
	CSemCtrlPanelManager::Instance().SyncSemCoreCtrlPanel( ClassID, SerialNumber, Xpos, Ypos, Flag );
	ClassID = m_ctrlDFocusSwitch.GetPanelClassID();
	SerialNumber = m_ctrlDFocusSwitch.GetPanelSerialNumber();
	CSemCtrlPanelManager::Instance().SyncSemCoreCtrlPanel( ClassID, SerialNumber, Xpos, Ypos, Flag );

	GetDlgItem(IDC_STATIC_IMP_PANEL_Group_Accessory)->SetWindowText(GetResString(IDS_STATIC_IMP_PANEL_Group_Accessory)); 
}

BOOL CDlgImpPanelAccessory::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	m_staticGroup.SetStyle( CMyGroupBox::STYLE_CAPTION );
//	m_staticGroup.SetCaptionBkColor( RGB(128,128,128) );

	CSemCtrlPanelManager::Instance().Regist( &m_ctrlDObjectiveLens, SemControlPanelClass_DObjectiveLens );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlDObjectiveLensSwitch, SemControlPanelClass_DObjectiveLensSwitch );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlDFocus, SemControlPanelClass_DFocus );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlDFocusSwitch, SemControlPanelClass_DFocusSwitch );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlYAdjust, SemControlPanelClass_YAdjust );

	CSemCtrlPanelManager::Instance().Regist( &m_ctrlSACP, SemControlPanelClass_SACP );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlImageReverse, SemControlPanelClass_ImageReverse );

	VARIANT ClassID, SerialNumber, Enabled, Flag;
	SerialNumber.lVal	= -1;
	Flag.lVal			= 3;
	Enabled.lVal		= 0;

	ClassID.lVal = SemControlPanelClass_DObjectiveLens;
	CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );

//	ClassID.lVal = SemControlPanelClass_YAdjust;
//	CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );

	ClassID.lVal = SemControlPanelClass_DFocus;
	CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );

	// 09.01.15 S.A.C.P.功能默认状态为不启用
	( GetDlgItem( IDC_SEMCORECTRLPANELSTAND7CTRL_SACP ) )->ShowWindow( SW_HIDE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgImpPanelAccessory::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlDObjectiveLens );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlDObjectiveLensSwitch );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlDFocus );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlDFocusSwitch );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlYAdjust );

	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlSACP );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlImageReverse );
}

void CDlgImpPanelAccessory::OnReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your control notification handler code here
	CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	BOOL bEnable = (BOOL)(Xpos.lVal);
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView )
	{
		switch( ClassID.lVal )
		{
		case	SemControlPanelClass_DObjectiveLensSwitch:
			EnableCtrl( ClassID.lVal, bEnable );	// 09.01.15
			pView->SyncWobbler( bEnable );
			break;
		case	SemControlPanelClass_YAdjust:
			EnableCtrl( ClassID.lVal, bEnable );	// 09.01.15
			pView->SyncLean( bEnable );
			break;
		case	SemControlPanelClass_DFocusSwitch:
			EnableCtrl( ClassID.lVal, bEnable );	// 09.01.15
			pView->SyncDynFocus( bEnable );
			break;
		case	SemControlPanelClass_SACP:
			pView->SyncSacp( bEnable );
			break;
		}
	}
}

void CDlgImpPanelAccessory::EnableCtrl( long lClassID, BOOL bEnable )
{
	VARIANT ClassID, SerialNumber, Enabled, Flag;
	SerialNumber.lVal	= -1;
	Flag.lVal			= 3;
	Enabled.lVal		= (long)bEnable;
	switch( lClassID )
	{
	case	SemControlPanelClass_DObjectiveLensSwitch:
		ClassID.lVal = SemControlPanelClass_DObjectiveLens;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
		break;
/*	case	SemControlPanelClass_YAdjust:
		ClassID.lVal = SemControlPanelClass_YAdjust;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
		break;
*/	case	SemControlPanelClass_DFocusSwitch:
		ClassID.lVal = SemControlPanelClass_DFocus;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
		break;
	case	SemControlPanelClass_SACP:	// 09.01.15
		if( bEnable )
			( GetDlgItem( IDC_SEMCORECTRLPANELSTAND7CTRL_SACP ) )->ShowWindow( SW_SHOW );
		else
			( GetDlgItem( IDC_SEMCORECTRLPANELSTAND7CTRL_SACP ) )->ShowWindow( SW_HIDE );
		break;
	}
}

void CDlgImpPanelAccessory::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	//CDialog::OnCancel();
}

void CDlgImpPanelAccessory::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}
