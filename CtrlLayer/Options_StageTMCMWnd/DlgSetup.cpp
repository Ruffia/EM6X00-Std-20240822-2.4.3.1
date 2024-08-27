// DlgSetup.cpp : implementation file
//

#include "stdafx.h"
#include "Options_StageTMCMWnd.h"
#include "DlgSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup dialog


CDlgSetup::CDlgSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nPort		= -1;
	m_nVentSel	= -1;
	m_bAdvance	= FALSE;
}

CDlgSetup::~CDlgSetup()
{
}


void CDlgSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetup)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgSetup)
	ON_BN_CLICKED(IDC_BUTTON_Relink, OnBnClickedButtonRelink)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup message handlers

void CDlgSetup::Localize()
{
	char szBuf[255];
	// 界面元素
	if(GetPrivateProfileString("Label", "CapSetup ", "", szBuf, 50, m_cPathLocalize) != 0)
		SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "CommPort", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_CommPort)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "SetupClick", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_CHECK_Backlash_Click)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "SetupTrack", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_CHECK_Backlash_Track)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "SetupRelink", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_BUTTON_Relink)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "VentGroup", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_Group_Vent)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "VentGoHome", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_RADIO_Vent_GoHome)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "VentGoOrigin", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_RADIO_Vent_GoOrigin)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "VentNoMove", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_RADIO_Vent_NoMove)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "SoftLimit", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_CHECK_SoftLimit)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "BtnOK", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDOK)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "DoubleClick", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_CHECK_DbClick)->SetWindowText( szBuf );
}

BOOL CDlgSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	CComboBox* pPort = ((CComboBox*)(GetDlgItem(IDC_COMBO_CommPort)));
	if( pPort != NULL )
	{
		for( int i=0; i<30; i++ )
		{
			str.Format( "COM %d", i+1 );
			pPort->AddString( str );
		}
		pPort->SetCurSel( m_nPort -1 );
	}

	if( m_bClick )
		((CButton*)(GetDlgItem(IDC_CHECK_Backlash_Click)))->SetCheck(1);
	if( m_bTrack )
		((CButton*)(GetDlgItem(IDC_CHECK_Backlash_Track)))->SetCheck(1);
	((CButton*)(GetDlgItem(IDC_RADIO_Vent_GoHome)))->SetCheck(0);
	((CButton*)(GetDlgItem(IDC_RADIO_Vent_GoOrigin)))->SetCheck(0);
	((CButton*)(GetDlgItem(IDC_RADIO_Vent_NoMove)))->SetCheck(0);
	switch( m_nVentSel )
	{
	case	0:
		((CButton*)(GetDlgItem(IDC_RADIO_Vent_GoHome)))->SetCheck(1);
		break;
	case	1:
		((CButton*)(GetDlgItem(IDC_RADIO_Vent_GoOrigin)))->SetCheck(1);
		break;
	case	2:
		((CButton*)(GetDlgItem(IDC_RADIO_Vent_NoMove)))->SetCheck(1);
		break;
	}
	if( m_bSoftLimit )
		((CButton*)(GetDlgItem(IDC_CHECK_SoftLimit)))->SetCheck(1);
	if(m_bDbClick)
		((CButton*)(GetDlgItem(IDC_CHECK_DbClick)))->SetCheck(1);

	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetup::OnOK() 
{
	// TODO: Add extra validation here
	CString str;

	CComboBox* pPort = ((CComboBox*)(GetDlgItem(IDC_COMBO_CommPort)));
	if( pPort != NULL )
		m_nPort = pPort->GetCurSel() +1;

	m_bClick = ((CButton*)(GetDlgItem(IDC_CHECK_Backlash_Click)))->GetCheck();
	m_bTrack = ((CButton*)(GetDlgItem(IDC_CHECK_Backlash_Track)))->GetCheck();
	int nCheck1 = ((CButton*)(GetDlgItem(IDC_RADIO_Vent_GoHome)))->GetCheck();
	int nCheck2 = ((CButton*)(GetDlgItem(IDC_RADIO_Vent_GoOrigin)))->GetCheck();
	int nCheck3 = ((CButton*)(GetDlgItem(IDC_RADIO_Vent_NoMove)))->GetCheck();
	m_nVentSel = ( nCheck1 > 0 ) ? 0 : ((nCheck2 > 0) ? 1 : 2);
	m_bSoftLimit = ((CButton*)(GetDlgItem(IDC_CHECK_SoftLimit)))->GetCheck();
	m_bDbClick	= ((CButton*)(GetDlgItem(IDC_CHECK_DbClick)))->GetCheck();
	CDialog::OnOK();
}

void CDlgSetup::OnBnClickedButtonRelink()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDC_BUTTON_Relink);
}

