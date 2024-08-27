// DlgImpPanelDebugA.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgImpPanelDebugA.h"
#include "ScanView.h"
#include "ControlLayer.h"
#include "ScaleManager.h"
#include "SemCoreAssemble.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelDebugA dialog


CDlgImpPanelDebugA::CDlgImpPanelDebugA(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImpPanelDebugA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImpPanelDebugA)
	m_nCS1State	= 0;
	m_nCS2State	= 0;
	m_nSPState	= SignalProcesser_Close;
	//}}AFX_DATA_INIT
}


void CDlgImpPanelDebugA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImpPanelDebugA)
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_DObjectiveLens, m_ctrlDObjectiveLens);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND7CTRL_DObjectiveLens, m_ctrlDObjectiveLensSwitch);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_DFocus, m_ctrlDFocus);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND7CTRL_DFocus, m_ctrlDFocusSwitch);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND6CTRL_ObjectiveLens, m_ctrlObjectiveLens);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_YAdjust, m_ctrlYAdjust);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND7CTRL_SACP, m_ctrlSACP);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND7CTRL_ImageReverse, m_ctrlImageReverse);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_RobinsonDetector, m_ctrlRobinsonDetector);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND7CTRL_RobinsonDetector, m_ctrlRobinsonDetectorSwitch);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_Signal2Brightness, m_ctrlSignal2Brightness);
	DDX_Control(pDX, IDC_SEMCORECTRLPANELSTAND1CTRL_Brightness, m_ctrlBrightness);
	DDX_Radio(pDX, IDC_CS1_Channel_AB, m_nCS1State);
	DDX_Radio(pDX, IDC_CS2_Channel_AB, m_nCS2State);
	DDX_Radio(pDX, IDC_SP_Close, m_nSPState);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImpPanelDebugA, CDialog)
	//{{AFX_MSG_MAP(CDlgImpPanelDebugA)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CS1_Channel_AB, OnCS1Change)
	ON_BN_CLICKED(IDC_CS2_Channel_AB, OnCS2Change)
	ON_BN_CLICKED(IDC_SP_Close, OnSPChange)
	ON_BN_CLICKED(IDC_CS1_Channel_C, OnCS1Change)
	ON_BN_CLICKED(IDC_CS1_Channel_D, OnCS1Change)
	ON_BN_CLICKED(IDC_CS1_Channel_E, OnCS1Change)
	ON_BN_CLICKED(IDC_CS2_Channel_C, OnCS2Change)
	ON_BN_CLICKED(IDC_CS2_Channel_D, OnCS2Change)
	ON_BN_CLICKED(IDC_CS2_Channel_E, OnCS2Change)
	ON_BN_CLICKED(IDC_SP_LP, OnSPChange)
	ON_BN_CLICKED(IDC_SP_SE, OnSPChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgImpPanelDebugA, CDialog)
	//{{AFX_EVENTSINK_MAP(CDlgImpPanelDebugA)
	ON_EVENT(CDlgImpPanelDebugA, IDC_SEMCORECTRLPANELSTAND1CTRL_DObjectiveLens, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebugA, IDC_SEMCORECTRLPANELSTAND1CTRL_DFocus, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebugA, IDC_SEMCORECTRLPANELSTAND1CTRL_YAdjust, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebugA, IDC_SEMCORECTRLPANELSTAND7CTRL_DObjectiveLens, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebugA, IDC_SEMCORECTRLPANELSTAND7CTRL_DFocus, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebugA, IDC_SEMCORECTRLPANELSTAND6CTRL_ObjectiveLens, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebugA, IDC_SEMCORECTRLPANELSTAND7CTRL_SACP, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebugA, IDC_SEMCORECTRLPANELSTAND7CTRL_ImageReverse, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebugA, IDC_SEMCORECTRLPANELSTAND1CTRL_RobinsonDetector, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebugA, IDC_SEMCORECTRLPANELSTAND1CTRL_Signal2Brightness, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebugA, IDC_SEMCORECTRLPANELSTAND7CTRL_RobinsonDetector, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	ON_EVENT(CDlgImpPanelDebugA, IDC_SEMCORECTRLPANELSTAND1CTRL_Brightness, 1 /* ReportPos */, OnReportPos, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelDebugA message handlers

void CDlgImpPanelDebugA::Localize(void)
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
	
	VARIANT	ResourceID;
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_COARSE;
	m_ctrlObjectiveLens.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Coarse)); 
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_FINE;
	m_ctrlObjectiveLens.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Fine)); 
	ResourceID.intVal		= IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_TINY;
	m_ctrlObjectiveLens.SyncResString(ResourceID, GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_Tiny)); 
	
	m_ctrlObjectiveLens.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND6CTRL_ObjectiveLens)); 

	GetDlgItem(IDC_STATIC_IMP_PANEL_Group_Accessory)->SetWindowText(GetResString(IDS_STATIC_IMP_PANEL_Group_Accessory)); 

	
	m_ctrlRobinsonDetectorSwitch.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND7CTRL_RobinsonDetector)); 
	m_ctrlRobinsonDetector.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_RobinsonDetector)); 
	m_ctrlSignal2Brightness.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_Signal2Brightness)); 
	m_ctrlBrightness.SetWindowText(GetResString(IDS_SEMCORECTRLPANELSTAND1CTRL_Brightness)); 
	
	ClassID = m_ctrlRobinsonDetectorSwitch.GetPanelClassID();
	SerialNumber = m_ctrlRobinsonDetectorSwitch.GetPanelSerialNumber();
	CSemCtrlPanelManager::Instance().SyncSemCoreCtrlPanel( ClassID, SerialNumber, Xpos, Ypos, Flag );
	
	GetDlgItem(IDC_STATIC_IMP_PANEL_Group_Robinson)->SetWindowText(GetResString(IDS_STATIC_IMP_PANEL_Group_Robinson)); 

	
	GetDlgItem(IDC_STATIC_GROUP_VIDEO_CHN1)->SetWindowText(GetResString(IDS_STATIC_GROUP_VIDEO_CHN1)); 
	GetDlgItem(IDC_STATIC_GROUP_VIDEO_CHN2)->SetWindowText(GetResString(IDS_STATIC_GROUP_VIDEO_CHN2)); 
	GetDlgItem(IDC_CS1_Channel_AB)->SetWindowText(GetResString(IDS_CS1_Channel_AB)); 
	GetDlgItem(IDC_CS1_Channel_C)->SetWindowText(GetResString(IDS_CS1_Channel_C)); 
	GetDlgItem(IDC_CS1_Channel_D)->SetWindowText(GetResString(IDS_CS1_Channel_D)); 
	GetDlgItem(IDC_CS1_Channel_E)->SetWindowText(GetResString(IDS_CS1_Channel_E)); 
	GetDlgItem(IDC_CS2_Channel_AB)->SetWindowText(GetResString(IDS_CS2_Channel_AB)); 
	GetDlgItem(IDC_CS2_Channel_C)->SetWindowText(GetResString(IDS_CS2_Channel_C)); 
	GetDlgItem(IDC_CS2_Channel_D)->SetWindowText(GetResString(IDS_CS2_Channel_D)); 
	GetDlgItem(IDC_CS2_Channel_E)->SetWindowText(GetResString(IDS_CS2_Channel_E)); 
	GetDlgItem(IDC_STATIC_GROUP_VIDEO_FUNC)->SetWindowText(GetResString(IDS_STATIC_GROUP_VIDEO_FUNC)); 
	GetDlgItem(IDC_SP_Close)->SetWindowText(GetResString(IDS_SP_Close)); 
	GetDlgItem(IDC_SP_LP)->SetWindowText(GetResString(IDS_SP_LP)); 
	GetDlgItem(IDC_SP_SE)->SetWindowText(GetResString(IDS_SP_SE)); 
	
	GetDlgItem(IDC_STATIC_IMP_PANEL_Group_Video)->SetWindowText(GetResString(IDS_STATIC_GROUP_VIDEO)); 
}

BOOL CDlgImpPanelDebugA::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlDObjectiveLens, SemControlPanelClass_DObjectiveLens );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlDObjectiveLensSwitch, SemControlPanelClass_DObjectiveLensSwitch );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlDFocus, SemControlPanelClass_DFocus );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlDFocusSwitch, SemControlPanelClass_DFocusSwitch );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlObjectiveLens, SemControlPanelClass_ObjectiveLens );
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
	
//	ClassID.lVal = SemControlPanelClass_DFocus;
//	CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
	
	// 09.01.15 S.A.C.P.功能默认状态为不启用
	( GetDlgItem( IDC_SEMCORECTRLPANELSTAND7CTRL_SACP ) )->ShowWindow( SW_HIDE );
	
	
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlRobinsonDetectorSwitch, SemControlPanelClass_RobinsonDetectorSwitch );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlRobinsonDetector, SemControlPanelClass_Signal2Contrast );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlSignal2Brightness, SemControlPanelClass_Signal2Brightness );
	CSemCtrlPanelManager::Instance().Regist( &m_ctrlBrightness, SemControlPanelClass_Brightness );
	
	ClassID.lVal = SemControlPanelClass_Signal2Brightness;
	CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
	ClassID.lVal = SemControlPanelClass_Signal2Contrast;
	CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgImpPanelDebugA::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlDObjectiveLens );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlDObjectiveLensSwitch );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlDFocus );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlDFocusSwitch );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlObjectiveLens );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlYAdjust );
	
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlSACP );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlImageReverse );
	
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlRobinsonDetectorSwitch );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlRobinsonDetector );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlSignal2Brightness );
	CSemCtrlPanelManager::Instance().UnRegist( &m_ctrlBrightness );
}

void CDlgImpPanelDebugA::OnReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
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
			pView->SyncWobbler( bEnable );
			EnableCtrl( ClassID.lVal, bEnable );	// 09.01.15
			break;
		case	SemControlPanelClass_YAdjust:
			EnableCtrl( ClassID.lVal, bEnable );	// 09.01.15
			pView->SyncLean( bEnable );
			break;
		case	SemControlPanelClass_DFocusSwitch:
			pView->SyncDynFocus( bEnable );
			EnableCtrl( ClassID.lVal, bEnable );	// 09.01.15
			break;
		case	SemControlPanelClass_SACP:
			pView->SyncSacp( bEnable );
			break;
		case	SemControlPanelClass_RobinsonDetectorSwitch:
			EnableCtrl( ClassID.lVal, bEnable );	// 09.01.15
			pView->SyncRobinsonDetector( bEnable );
			break;
		}
	}
}

void CDlgImpPanelDebugA::EnableCtrl( long lClassID, BOOL bEnable )
{
	VARIANT ClassID, SerialNumber, Enabled, Flag, varPos;
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
		varPos = CSemCoreAssemble::Instance().m_DFocus.GetSwitch();
		if( varPos.lVal != 0 )
			Enabled.lVal = 1;
		else
			Enabled.lVal = 0;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
		if( varPos.lVal == 3 )
		{
			varPos.lVal = 0;
			Flag.lVal	= 1;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, varPos, varPos, Flag );
		}
		else if( varPos.lVal == 0 )
		{
			CSemCoreAssemble::Instance().m_DFocus.GetRange( varPos, Enabled );
			Flag.lVal = 1;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, varPos, varPos, Flag );
		}
		break;
	case	SemControlPanelClass_SACP:	// 09.01.15
		if( bEnable )
			( GetDlgItem( IDC_SEMCORECTRLPANELSTAND7CTRL_SACP ) )->ShowWindow( SW_SHOW );
		else
			( GetDlgItem( IDC_SEMCORECTRLPANELSTAND7CTRL_SACP ) )->ShowWindow( SW_HIDE );
		break;
	case	SemControlPanelClass_RobinsonDetectorSwitch:
		ClassID.lVal = SemControlPanelClass_Signal2Brightness;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
		ClassID.lVal = SemControlPanelClass_Signal2Contrast;
		CSemCtrlPanelManager::Instance().EnableSemCoreCtrlPanel( ClassID, SerialNumber, Enabled, Flag );
		break;
	}
}

void CDlgImpPanelDebugA::OnCS1Change() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	MessageBeep(-1);
	
	VARIANT varPos;
	varPos.lVal = m_nCS1State;
	
	CSemCoreAssemble::Instance().m_PC10SE6.SetPos( varPos );
	CSemCoreAssemble::Instance().m_PC10SE6.Flush();
}

void CDlgImpPanelDebugA::OnCS2Change() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	MessageBeep(-1);
	
	VARIANT varPos;
	varPos.lVal = m_nCS2State;
	
	CSemCoreAssemble::Instance().m_PC10SE1.SetPos( varPos );
	CSemCoreAssemble::Instance().m_PC10SE1.Flush();
}

void CDlgImpPanelDebugA::OnSPChange() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	MessageBeep(-1);
	
	VARIANT varPos;
	varPos.lVal = m_nSPState;
	
	CSemCoreAssemble::Instance().m_PC10SE6.SetSPState( varPos );
	CSemCoreAssemble::Instance().m_PC10SE6.Flush();
}

void CDlgImpPanelDebugA::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	//CDialog::OnCancel();
}

void CDlgImpPanelDebugA::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}
