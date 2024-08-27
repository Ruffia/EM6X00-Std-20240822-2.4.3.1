// SemCtrlDlgRenewSelect.cpp : implementation file
//
// Last Modify : 2011.07.19

///////////////////////////////////////////////////////////////////////////////
// 11.07.19 加入隐含项，全选时可以记录下位机发送的当前未直接记录的消息
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XXX2.h"
#include "SemCtrlDlgRenewSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSemCtrlDlgRenewSelect dialog


CSemCtrlDlgRenewSelect::CSemCtrlDlgRenewSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CSemCtrlDlgRenewSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSemCtrlDlgRenewSelect)
	m_csMsg						= _T("");
	m_boolHV					= FALSE;
	m_boolScanShift				= FALSE;
	m_boolBeemAlignment			= FALSE;
	m_boolBrightness			= FALSE;
	m_boolContrast				= FALSE;
	m_boolStigmator				= FALSE;
	m_boolObjectiveLens			= FALSE;
	m_boolCondenserLens			= FALSE;
	m_boolFilamentCurrent		= FALSE;
	m_boolMagnifier				= FALSE;
	m_boolWorkDistance			= FALSE;
	m_boolObjectiveLensReverse	= FALSE;
	m_boolCondenserLensReverse	= FALSE;
	m_boolAuxiliaryStigmation	= FALSE;
	m_boolDCondenserLens		= FALSE;
	m_boolScanRotate			= FALSE;
	m_boolSACP					= FALSE;
	m_boolDFocus				= FALSE;
	m_boolDFocusSwitch			= FALSE;
	m_boolDObjectiveLens		= FALSE;
	m_boolDObjectiveLensSwitch	= FALSE;
	m_boolRobinsonDetectorSwitch= FALSE;
	m_boolSignal2Brightness		= FALSE;
	m_boolSignal2Contrast		= FALSE;
	m_boolYAdjust				= FALSE;
	m_boolImageReverse			= FALSE;
	m_boolGunCurrent			= FALSE;
	m_boolMsgFromMicroPC		= FALSE;
	//}}AFX_DATA_INIT

	m_boolIsRecord = FALSE;	
}


void CSemCtrlDlgRenewSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSemCtrlDlgRenewSelect)
	DDX_Text(pDX, IDC_STATIC_MSG_RenewSelect, m_csMsg);
	DDX_Check(pDX, IDC_CHECK_HV, m_boolHV);
	DDX_Check(pDX, IDC_CHECK_ScanShift, m_boolScanShift);
	DDX_Check(pDX, IDC_CHECK_BeemAlignment, m_boolBeemAlignment);
	DDX_Check(pDX, IDC_CHECK_Brightness, m_boolBrightness);
	DDX_Check(pDX, IDC_CHECK_Contrast, m_boolContrast);
	DDX_Check(pDX, IDC_CHECK_Stigmator, m_boolStigmator);
	DDX_Check(pDX, IDC_CHECK_ObjectiveLens, m_boolObjectiveLens);
	DDX_Check(pDX, IDC_CHECK_CondenserLens, m_boolCondenserLens);
	DDX_Check(pDX, IDC_CHECK_FilamentCurrent, m_boolFilamentCurrent);
	DDX_Check(pDX, IDC_CHECK_Magnifier, m_boolMagnifier);
	DDX_Check(pDX, IDC_CHECK_WorkDistance, m_boolWorkDistance);
	DDX_Check(pDX, IDC_CHECK_ObjectiveLensReverse, m_boolObjectiveLensReverse);
	DDX_Check(pDX, IDC_CHECK_CondenserLensReverse, m_boolCondenserLensReverse);
	DDX_Check(pDX, IDC_CHECK_AuxiliaryStigmation, m_boolAuxiliaryStigmation);
	DDX_Check(pDX, IDC_CHECK_DCondenserLens, m_boolDCondenserLens);
	DDX_Check(pDX, IDC_CHECK_ScanRotate, m_boolScanRotate);
	DDX_Check(pDX, IDC_CHECK_SACP, m_boolSACP);
	DDX_Check(pDX, IDC_CHECK_DFocus, m_boolDFocus);
	DDX_Check(pDX, IDC_CHECK_DFocusSwitch, m_boolDFocusSwitch);
	DDX_Check(pDX, IDC_CHECK_DObjectiveLens, m_boolDObjectiveLens);
	DDX_Check(pDX, IDC_CHECK_DObjectiveLensSwitch, m_boolDObjectiveLensSwitch);
	DDX_Check(pDX, IDC_CHECK_RobinsonDetectorSwitch, m_boolRobinsonDetectorSwitch);
	DDX_Check(pDX, IDC_CHECK_Signal2Brightness, m_boolSignal2Brightness);
	DDX_Check(pDX, IDC_CHECK_Signal2Contrast, m_boolSignal2Contrast);
	DDX_Check(pDX, IDC_CHECK_YAdjust, m_boolYAdjust);
	DDX_Check(pDX, IDC_CHECK_ImageReverse, m_boolImageReverse);
	DDX_Check(pDX, IDC_CHECK_GunCurrent, m_boolGunCurrent);
	DDX_Check(pDX, IDC_CHECK_MsgFromMicroPC, m_boolMsgFromMicroPC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSemCtrlDlgRenewSelect, CDialog)
	//{{AFX_MSG_MAP(CSemCtrlDlgRenewSelect)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_Clear, OnClearAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSemCtrlDlgRenewSelect message handlers

void CSemCtrlDlgRenewSelect::OnSelectAll() 
{
	// TODO: Add your control notification handler code here
	m_boolHV					= TRUE;
	m_boolScanShift				= TRUE;
	m_boolBeemAlignment			= TRUE;
	m_boolBrightness			= TRUE;
	m_boolContrast				= TRUE;
	m_boolStigmator				= TRUE;
	m_boolObjectiveLens			= TRUE;
	m_boolCondenserLens			= TRUE;
	m_boolFilamentCurrent		= TRUE;
	m_boolMagnifier				= TRUE;
	m_boolWorkDistance			= TRUE;
	m_boolObjectiveLensReverse	= TRUE;
	m_boolCondenserLensReverse	= TRUE;
	m_boolAuxiliaryStigmation	= TRUE;
	m_boolDCondenserLens		= TRUE;
	m_boolScanRotate			= TRUE;
	m_boolSACP					= TRUE;
	m_boolDFocus				= TRUE;
	m_boolDFocusSwitch			= TRUE;
	m_boolDObjectiveLens		= TRUE;
	m_boolDObjectiveLensSwitch	= TRUE;
	m_boolRobinsonDetectorSwitch= TRUE;
	m_boolSignal2Brightness		= TRUE;
	m_boolSignal2Contrast		= TRUE;
	m_boolYAdjust				= TRUE;
	m_boolImageReverse			= TRUE;
	m_boolGunCurrent			= TRUE;
	m_boolMsgFromMicroPC		= TRUE;

	UpdateData( FALSE );
}

void CSemCtrlDlgRenewSelect::OnClearAll() 
{
	// TODO: Add your control notification handler code here
	m_boolHV					= FALSE;
	m_boolScanShift				= FALSE;
	m_boolBeemAlignment			= FALSE;
	m_boolBrightness			= FALSE;
	m_boolContrast				= FALSE;
	m_boolStigmator				= FALSE;
	m_boolObjectiveLens			= FALSE;
	m_boolCondenserLens			= FALSE;
	m_boolFilamentCurrent		= FALSE;
	m_boolMagnifier				= FALSE;
	m_boolWorkDistance			= FALSE;
	m_boolObjectiveLensReverse	= FALSE;
	m_boolCondenserLensReverse	= FALSE;
	m_boolAuxiliaryStigmation	= FALSE;
	m_boolDCondenserLens		= FALSE;
	m_boolScanRotate			= FALSE;
	m_boolSACP					= FALSE;
	m_boolDFocus				= FALSE;
	m_boolDFocusSwitch			= FALSE;
	m_boolDObjectiveLens		= FALSE;
	m_boolDObjectiveLensSwitch	= FALSE;
	m_boolRobinsonDetectorSwitch= FALSE;
	m_boolSignal2Brightness		= FALSE;
	m_boolSignal2Contrast		= FALSE;
	m_boolYAdjust				= FALSE;
	m_boolImageReverse			= FALSE;
	m_boolGunCurrent			= FALSE;
	m_boolMsgFromMicroPC		= FALSE;

	UpdateData( FALSE );
}

BOOL CSemCtrlDlgRenewSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if ( m_boolIsRecord )
	{
		GetDlgItem(IDC_CHECK_FilamentCurrent)->EnableWindow();
//		GetDlgItem(IDC_CHECK_GunCurrent)->EnableWindow();
		GetDlgItem(IDC_CHECK_WorkDistance)->EnableWindow();		
		m_csMsg = GetResString( IDS_SD_PROMPT_LOG_SELECT );
	}
	else
		m_csMsg = GetResString( IDS_SD_TITLE_SELECT );
	
	// 09.01.15 S.A.C.P.功能默认状态为不启用
	if( theApp.m_blSACPState )
		GetDlgItem( IDC_CHECK_SACP )->ShowWindow( SW_SHOW );
	else
		GetDlgItem( IDC_CHECK_SACP )->ShowWindow( SW_HIDE );

	Localize();

	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSemCtrlDlgRenewSelect::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_CHECK_HV)->SetWindowText(GetResString(IDS_CHECK_HV)); 
	GetDlgItem(IDC_CHECK_ScanShift)->SetWindowText(GetResString(IDS_CHECK_ScanShift)); 
	GetDlgItem(IDC_CHECK_BeemAlignment)->SetWindowText(GetResString(IDS_CHECK_BeemAlignment)); 
	GetDlgItem(IDC_CHECK_Brightness)->SetWindowText(GetResString(IDS_CHECK_Brightness)); 
	GetDlgItem(IDC_CHECK_Contrast)->SetWindowText(GetResString(IDS_CHECK_Contrast)); 
	GetDlgItem(IDC_CHECK_Stigmator)->SetWindowText(GetResString(IDS_CHECK_Stigmator)); 
	GetDlgItem(IDC_CHECK_ObjectiveLens)->SetWindowText(GetResString(IDS_CHECK_ObjectiveLens)); 
	GetDlgItem(IDC_CHECK_CondenserLens)->SetWindowText(GetResString(IDS_CHECK_CondenserLens)); 
	GetDlgItem(IDC_CHECK_FilamentCurrent)->SetWindowText(GetResString(IDS_CHECK_FilamentCurrent)); 
	GetDlgItem(IDC_CHECK_Magnifier)->SetWindowText(GetResString(IDS_CHECK_Magnifier)); 
	GetDlgItem(IDC_CHECK_ObjectiveLensReverse)->SetWindowText(GetResString(IDS_CHECK_ObjectiveLensReverse)); 
	GetDlgItem(IDC_CHECK_CondenserLensReverse)->SetWindowText(GetResString(IDS_CHECK_CondenserLensReverse)); 
	GetDlgItem(IDC_CHECK_AuxiliaryStigmation)->SetWindowText(GetResString(IDS_CHECK_AuxiliaryStigmation)); 
	GetDlgItem(IDC_CHECK_DCondenserLens)->SetWindowText(GetResString(IDS_CHECK_DCondenserLens)); 
	GetDlgItem(IDC_CHECK_ScanRotate)->SetWindowText(GetResString(IDS_CHECK_ScanRotate)); 
	GetDlgItem(IDC_CHECK_SACP)->SetWindowText(GetResString(IDS_CHECK_SACP)); 
	GetDlgItem(IDC_CHECK_DFocus)->SetWindowText(GetResString(IDS_CHECK_DFocus)); 
	GetDlgItem(IDC_CHECK_DFocusSwitch)->SetWindowText(GetResString(IDS_CHECK_DFocusSwitch)); 
	GetDlgItem(IDC_CHECK_DObjectiveLens)->SetWindowText(GetResString(IDS_CHECK_DObjectiveLens)); 
	GetDlgItem(IDC_CHECK_DObjectiveLensSwitch)->SetWindowText(GetResString(IDS_CHECK_DObjectiveLensSwitch)); 
	GetDlgItem(IDC_CHECK_RobinsonDetectorSwitch)->SetWindowText(GetResString(IDS_CHECK_RobinsonDetectorSwitch)); 
	GetDlgItem(IDC_CHECK_Signal2Brightness)->SetWindowText(GetResString(IDS_CHECK_Signal2Brightness)); 
	GetDlgItem(IDC_CHECK_Signal2Contrast)->SetWindowText(GetResString(IDS_CHECK_Signal2Contrast)); 
	GetDlgItem(IDC_CHECK_YAdjust)->SetWindowText(GetResString(IDS_CHECK_YAdjust)); 
	GetDlgItem(IDC_CHECK_ImageReverse)->SetWindowText(GetResString(IDS_CHECK_ImageReverse)); 
	GetDlgItem(IDC_CHECK_GunCurrent)->SetWindowText(GetResString(IDS_CHECK_GunCurrent)); 
	GetDlgItem(IDC_CHECK_WorkDistance)->SetWindowText(GetResString(IDS_CHECK_WorkDistance)); 

	GetDlgItem(IDC_BUTTON_ALL)->SetWindowText(GetResString(IDS_BUTTON_ALL)); 
	GetDlgItem(IDC_BUTTON_Clear)->SetWindowText(GetResString(IDS_BUTTON_Clear)); 
	GetDlgItem(IDOK)->SetWindowText(GetResString(IDS_OK));
	GetDlgItem(IDCANCEL)->SetWindowText(GetResString(IDS_CANCEL));

	SetWindowText(GetResString(IDS_SEMCTL_RenewSelect)); 
}