// DlgSetup.cpp : implementation file
//

#include "stdafx.h"
#include "Options_StageMC600Wnd.h"
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
	m_fRyp		= 16;
	m_fH		= 16;
	m_bAdvance	= FALSE;
}


void CDlgSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetup)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgSetup)
	ON_BN_CLICKED(IDC_BUTTON_LoadDefault, OnBUTTONLoadDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup message handlers

void CDlgSetup::Localize()
{
	char szBuf[255];
	// 界面元素
	if(GetPrivateProfileString("Label", "CapSetup", "", szBuf, 50, m_cPathLocalize) != 0)
		SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "CommPort", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_CommPort)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "CommPort2", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_CommPort2)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "SampleR", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_Sample_R)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "SampleH", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_Sample_H)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "SetupClick", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_CHECK_Backlash_Click)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "SetupTrack", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_CHECK_Backlash_Track)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "SetupDefault", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_BUTTON_LoadDefault)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "VentGroup", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_Group_Vent)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "VentGoHome", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_RADIO_Vent_GoHome)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "VentGoOrigin", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_RADIO_Vent_GoOrigin)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "VentNoMove", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_RADIO_Vent_NoMove)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "BtnOK", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDOK)->SetWindowText( szBuf );
	// 提示字符串
	if(GetPrivateProfileString("Msg", "MsgRangeR", "", szBuf, 255, m_cPathLocalize) != 0)
		m_strMsg[1] = szBuf;
	if(GetPrivateProfileString("Msg", "MsgRangeH", "", szBuf, 255, m_cPathLocalize) != 0)
		m_strMsg[2] = szBuf;
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
	CComboBox* pPort2 = ((CComboBox*)(GetDlgItem(IDC_COMBO_CommPort2)));
	if( pPort2 != NULL )
	{
		for( int i=0; i<30; i++ )
		{
			str.Format( "COM %d", i+1 );
			pPort2->AddString( str );
		}
		pPort2->SetCurSel( m_nPort2 -1 );
		if( m_nStageType < 3 )
			GetDlgItem(IDC_COMBO_CommPort2)->EnableWindow( FALSE );
	}
	str.Format( "%.1f", m_fRyp );
	GetDlgItem(IDC_EDIT_Sample_R)->SetWindowText(str);
	str.Format( "%.1f", m_fH );
	GetDlgItem(IDC_EDIT_Sample_H)->SetWindowText(str);
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
	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetup::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	GetDlgItem(IDC_EDIT_Sample_R)->GetWindowText(str);
	m_fRyp = (float)atof(str);
	if( m_fRyp < 8 || m_fRyp >= 50 )
	{
		MessageBox( m_strMsg[1], m_strMsg[0], MB_OK | MB_ICONERROR );
		return;
	}
	GetDlgItem(IDC_EDIT_Sample_H)->GetWindowText(str);
	m_fH = (float)atof(str);
	float fMin = 5;
	if( m_bAdvance )
		fMin = 3;
	if( m_fH < fMin || m_fH >= 30 )
	{
		MessageBox( m_strMsg[2], m_strMsg[0], MB_OK | MB_ICONERROR );
		return;
	}

	CComboBox* pPort = ((CComboBox*)(GetDlgItem(IDC_COMBO_CommPort)));
	if( pPort != NULL )
		m_nPort = pPort->GetCurSel() +1;
	CComboBox* pPort2 = ((CComboBox*)(GetDlgItem(IDC_COMBO_CommPort2)));
	if( pPort2 != NULL )
		m_nPort2 = pPort2->GetCurSel() +1;
	m_bClick = ((CButton*)(GetDlgItem(IDC_CHECK_Backlash_Click)))->GetCheck();
	m_bTrack = ((CButton*)(GetDlgItem(IDC_CHECK_Backlash_Track)))->GetCheck();
	int nCheck1 = ((CButton*)(GetDlgItem(IDC_RADIO_Vent_GoHome)))->GetCheck();
	int nCheck2 = ((CButton*)(GetDlgItem(IDC_RADIO_Vent_GoOrigin)))->GetCheck();
	int nCheck3 = ((CButton*)(GetDlgItem(IDC_RADIO_Vent_NoMove)))->GetCheck();
	m_nVentSel = ( nCheck1 > 0 ) ? 0 : ((nCheck2 > 0) ? 1 : 2);
	CDialog::OnOK();
}

void CDlgSetup::OnBUTTONLoadDefault() 
{
	// 装载默认参数（注意：非出厂参数，是KYKY的默认参数）
	BeginWaitCursor(); // display the hourglass cursor
	((CPanelStage*)GetParent())->m_MC600.LoadDefault(FALSE);
	((CPanelStage*)GetParent())->m_Vince.LoadDefault(1);
	EndWaitCursor(); // remove the hourglass cursor
}
