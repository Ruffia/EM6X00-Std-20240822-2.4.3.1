// SemCtrlDlgDebug.cpp : implementation file
//
// Last Modify : 2010.01.22

/////////////////////////////////////////////////////////////////////////////
// 09.12.09 增加调试状态功能，包括：假高压、聚光镜反转快捷键、消像散四方向调节快捷键
// 10.01.22 加入最佳亮度、最佳对比度值的设置，为ABC和AF功能做实验
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XXX2.h"
#include "SemCtrlDlgDebug.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSemCtrlDlgDebug dialog


CSemCtrlDlgDebug::CSemCtrlDlgDebug(CWnd* pParent /*=NULL*/)
	: CDialog(CSemCtrlDlgDebug::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSemCtrlDlgDebug)
	m_strPassword	= _T("");
	m_bDebugHV		= FALSE;
	m_bDebugAccel	= FALSE;
	m_bDebugVac		= FALSE;
	m_dBrightness	= 0.0;
	m_dContrast		= 0.0;
	//}}AFX_DATA_INIT
	ASSERT(pParent != NULL);

	m_pParent		= pParent;
	m_nID			= CSemCtrlDlgDebug::IDD;
}


void CSemCtrlDlgDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSemCtrlDlgDebug)
	DDX_Text(pDX, IDC_EDIT_DebugPassword, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 16);
	DDX_Check(pDX, IDC_CHECK_Debug_HV, m_bDebugHV);
	DDX_Check(pDX, IDC_CHECK_Debug_Accel, m_bDebugAccel);
	DDX_Check(pDX, IDC_CHECK_Debug_Vacuum, m_bDebugVac);
	DDX_Text(pDX, IDC_EDIT_Debug_B, m_dBrightness);
	DDV_MinMaxDouble(pDX, m_dBrightness, -100.0, 100.0);
	DDX_Text(pDX, IDC_EDIT_Debug_C, m_dContrast);
	DDV_MinMaxDouble(pDX, m_dContrast, 0.0, 100.0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSemCtrlDlgDebug, CDialog)
	//{{AFX_MSG_MAP(CSemCtrlDlgDebug)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSemCtrlDlgDebug message handlers

void CSemCtrlDlgDebug::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDOK)->SetWindowText(GetResString(IDS_OK)); 
	GetDlgItem(IDCANCEL)->SetWindowText(GetResString(IDS_CANCEL)); 

	CWnd* pWnd = GetDlgItem( IDC_STATIC_Debug );
	CWnd* pPassword = GetDlgItem( IDC_EDIT_DebugPassword );
	BOOL bVisible = ((pPassword->GetStyle() & WS_VISIBLE) != 0);
	if( bVisible )
		pWnd->SetWindowText(GetResString( IDS_STATIC_Debug1 )); 
	else
		pWnd->SetWindowText(GetResString( IDS_STATIC_Debug2 )); 

	GetDlgItem(IDC_CHECK_Debug_HV)->SetWindowText(GetResString( IDS_CHECK_Debug_HV )); 
	GetDlgItem(IDC_CHECK_Debug_Accel)->SetWindowText(GetResString( IDS_CHECK_Debug_Accel )); 
	GetDlgItem(IDC_CHECK_Debug_Vacuum)->SetWindowText(GetResString( IDS_CHECK_Debug_Vacuum )); 

	GetDlgItem(IDC_STATIC_Debug_Setup)->SetWindowText(GetResString( IDS_STATIC_Debug_Setup )); 
	GetDlgItem(IDC_STATIC_Debug_Setup_B)->SetWindowText(GetResString( IDS_STATIC_Debug_Setup_B )); 
	GetDlgItem(IDC_STATIC_Debug_Setup_C)->SetWindowText(GetResString( IDS_STATIC_Debug_Setup_C )); 

	SetWindowText(GetResString(IDS_SEMCTL_DEBUG)); 
}

BOOL CSemCtrlDlgDebug::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 设置参数值
	CString str;
	SemCtrlFigures		m_scfCtrl;
	m_scfCtrl = DefaultCtrlFigures_Brightness;
	if ( theApp.m_nOptimalBrightness < 0 )
		str.Format( "%+02.1f", 
		(double)theApp.m_nOptimalBrightness * 100.0 / abs(m_scfCtrl.varMin.lVal) );
	else
		str.Format( "%+02.1f", 
		(double)theApp.m_nOptimalBrightness * 100.0 / abs(m_scfCtrl.varMax.lVal) );
	m_dBrightness = atof( str );

	m_scfCtrl = DefaultCtrlFigures_Contrast;
	if ( theApp.m_nOptimalContrast < 0 )
		str.Format( "%+02.1f", 
		(double)theApp.m_nOptimalContrast * 100.0 / abs(m_scfCtrl.varMin.lVal) );
	else
		str.Format( "%+02.1f", 
		(double)theApp.m_nOptimalContrast * 100.0 / abs(m_scfCtrl.varMax.lVal) );
	m_dContrast = atof( str );

	UpdateData( FALSE );
	Localize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSemCtrlDlgDebug::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CEdit* pPassword = (CEdit*)(GetDlgItem( IDC_EDIT_DebugPassword ) );
	BOOL bVisible = ((pPassword->GetStyle() & WS_VISIBLE) != 0);
	if( bVisible )
	{
		// 密码编辑框可见，此时点击确定，如果密码正确就显示设置选项；
		//								如果不正确，编辑框清空，不做其它操作
		if( m_strPassword == "KX-SEM" )		// 密码正确
		{
			CRect rc, rcRef;
			GetWindowRect(&rc);
			GetDlgItem(IDC_CHECK_Debug_HV)->GetWindowRect(&rcRef);
			rcRef.bottom = rc.bottom;
			CWnd *pWnd = GetWindow(GW_CHILD);
			while(pWnd != NULL)
			{
				pWnd->GetWindowRect(&rc);
				if( PtInRect( rcRef, rc.CenterPoint() ) )
					pWnd->ShowWindow(SW_SHOW);
				pWnd = pWnd->GetWindow(GW_HWNDNEXT);
			};

			pWnd = GetDlgItem( IDC_STATIC_Debug );
			pWnd->SetWindowText(GetResString( IDS_STATIC_Debug2 )); 
			pWnd->ShowWindow( SW_SHOW );
			pPassword->ShowWindow( SW_HIDE );
			pWnd = GetDlgItem(IDCANCEL);
			pWnd->ShowWindow( SW_SHOW );
		}
		else
		{
			m_strPassword.Empty();// = _T("");
			UpdateData( FALSE );
		}
	}
	else
	{
		// 密码编辑框不可见，此时点击确定，使设置选项生效
		// 设置参数值
		CString str;
		SemCtrlFigures		m_scfCtrl;
		m_scfCtrl = DefaultCtrlFigures_Brightness;
		if ( m_dBrightness < 0 )
			str.Format( "%+02.1f", 
				m_dBrightness * abs(m_scfCtrl.varMin.lVal) / 100 );
		else
			str.Format( "%+02.1f", 
				m_dBrightness * abs(m_scfCtrl.varMax.lVal) / 100 );
		int nBrightness = atoi( str );

		m_scfCtrl = DefaultCtrlFigures_Contrast;
		if ( m_dContrast < 0 )
			str.Format( "%+02.1f", 
				m_dContrast * abs(m_scfCtrl.varMin.lVal) / 100 );
		else
			str.Format( "%+02.1f", 
				m_dContrast * abs(m_scfCtrl.varMax.lVal) / 100 );
		int nContrast = atoi( str );
//		((CScanChildFrame*)m_pParent)->SetupDebugParam( m_bDebugHV, m_bDebugAccel, nBrightness, nContrast );
		((CMainFrame *)theApp.GetMainWnd())->SetupDebugParam( m_bDebugHV, m_bDebugAccel, m_bDebugVac, FALSE, nBrightness, nContrast );
	}

//	CDialog::OnOK();
}

void CSemCtrlDlgDebug::OnCancel() 
{
	// TODO: Add extra cleanup here
//	((CScanChildFrame*)m_pParent)->DebugClose();
	((CMainFrame *)theApp.GetMainWnd())->DebugClose();
	m_bDebugHV		= FALSE;
	m_bDebugAccel	= FALSE;
	m_bDebugVac		= FALSE;
	UpdateData(FALSE);

	CWnd* pWnd = GetDlgItem( IDC_STATIC_Debug );
	pWnd->SetWindowText(GetResString( IDS_STATIC_Debug1 )); 
	CEdit* pPassword = (CEdit*)(GetDlgItem( IDC_EDIT_DebugPassword ) );
	pPassword->ShowWindow( SW_SHOW );

	CRect rc, rcRef;
	GetWindowRect(&rc);
	GetDlgItem(IDC_CHECK_Debug_HV)->GetWindowRect(&rcRef);
	rcRef.bottom = rc.bottom;
	pWnd = GetWindow(GW_CHILD);
	while(pWnd != NULL)
	{
		pWnd->GetWindowRect(&rc);
		if( PtInRect( rcRef, rc.CenterPoint() ) )
			pWnd->ShowWindow(SW_HIDE);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	};

	pWnd = GetDlgItem(IDCANCEL);
	pWnd->ShowWindow( SW_HIDE );
	m_strPassword.Empty();
	UpdateData( FALSE );
}
/*
void CSemCtrlDlgDebug::PostNcDestroy()
{
	delete this;
}
*/
