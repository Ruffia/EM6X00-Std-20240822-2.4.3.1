// DlgLimitParam.cpp : 实现文件
//

#include "stdafx.h"
#include "Options_StageTMCMWnd.h"
#include "resource.h"
#include "DlgLimitParam.h"
#include "afxdialogex.h"


// CDlgLimitParam 对话框

IMPLEMENT_DYNAMIC(CDlgLimitParam, CDialog)

CDlgLimitParam::CDlgLimitParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLimitParam::IDD, pParent)
{

}

CDlgLimitParam::~CDlgLimitParam()
{
}

void CDlgLimitParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLimitParam, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LimitChange, &CDlgLimitParam::OnBnClickedButtonLimitchange)
	ON_CBN_SELCHANGE(IDC_COMBO_Axis, &CDlgLimitParam::OnSelchangeComboAxis)
	ON_BN_CLICKED(IDC_BUTTON_LimitLevelReversal, &CDlgLimitParam::OnBnClickedButtonLimitlevelreversal)
END_MESSAGE_MAP()


// CDlgLimitParam 消息处理程序
void CDlgLimitParam::Localize()
{
	char szBuf[255];
	// 界面元素

	if(GetPrivateProfileString("Label", "CapLimit", "", szBuf, 50, m_cPathLocalize) != 0)
		SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "LimitGroup", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_Group)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "SlaveAxial", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_Axis)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "LeftLimitDisable", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_CHECK_LeftLimitDisable)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "RightLimitDisable", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_CHECK_RightLimitDisable)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "LimitExchange", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_CHECK_LimitExchange)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "Setup", "", szBuf, 50, m_cPathLocalize) != 0)
	{
		GetDlgItem(IDC_BUTTON_LimitChange)->SetWindowText( szBuf );
		GetDlgItem(IDC_BUTTON_LimitLevelReversal)->SetWindowText( szBuf );
	}
	if(GetPrivateProfileString("Label", "BtnOK", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDOK)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "LimitLevelGroup", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_LevelGroup)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "LeftLimitLevelReversal", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_CHECK_LeftLimitLevelReversal)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "RightLimitLevelReversal", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_CHECK_RightLimitLevelReversal)->SetWindowText( szBuf );
}

BOOL CDlgLimitParam::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;
	CComboBox* aAxial = ((CComboBox*)(GetDlgItem(IDC_COMBO_Axis)));
	aAxial->AddString("X");
	aAxial->AddString("Y");
	aAxial->AddString("Z");
	aAxial->AddString("R");
	aAxial->AddString("T");
	aAxial->SetCurSel( 0 );
	((CButton*)(GetDlgItem(IDC_CHECK_LeftLimitDisable)))->SetCheck(m_nLeftLimit[0]);
	((CButton*)(GetDlgItem(IDC_CHECK_RightLimitDisable)))->SetCheck(m_nRightLimit[0]);
	((CButton*)(GetDlgItem(IDC_CHECK_LimitExchange)))->SetCheck(m_nExchangeLimit[0]);
	//((CButton*)(GetDlgItem(IDC_CHECK_LeftLimitDisable)))->SetCheck(m_nLeftLimit[0]);
	((CButton*)(GetDlgItem(IDC_CHECK_LeftLimitLevelReversal)))->SetCheck(m_nLeftLimitLevel[0]);
	((CButton*)(GetDlgItem(IDC_CHECK_RightLimitLevelReversal)))->SetCheck(m_nRightLimitLevel[0]);

	Localize();
	return TRUE;
}

void CDlgLimitParam::OnSelchangeComboAxis()
{
	// TODO: 在此添加控件通知处理程序代码
	int nAxial = 0;
	CComboBox* aAxial = ((CComboBox*)(GetDlgItem(IDC_COMBO_Axis)));
	if( aAxial != NULL )
		nAxial = aAxial->GetCurSel();

	((CButton*)(GetDlgItem(IDC_CHECK_LeftLimitDisable)))->SetCheck(m_nLeftLimit[nAxial]);
	((CButton*)(GetDlgItem(IDC_CHECK_RightLimitDisable)))->SetCheck(m_nRightLimit[nAxial]);
	((CButton*)(GetDlgItem(IDC_CHECK_LimitExchange)))->SetCheck(m_nExchangeLimit[nAxial]);
	((CButton*)(GetDlgItem(IDC_CHECK_LeftLimitLevelReversal)))->SetCheck(m_nLeftLimitLevel[nAxial]);
	((CButton*)(GetDlgItem(IDC_CHECK_RightLimitLevelReversal)))->SetCheck(m_nRightLimitLevel[nAxial]);
}

void CDlgLimitParam::OnBnClickedButtonLimitchange()
{
	// TODO: 在此添加控件通知处理程序代码
	int nAxial = 0;
	CComboBox* aAxial = ((CComboBox*)(GetDlgItem(IDC_COMBO_Axis)));
	if( aAxial != NULL )
		nAxial = aAxial->GetCurSel();
	m_nLeftLimit[nAxial] = ((CButton*)(GetDlgItem(IDC_CHECK_LeftLimitDisable)))->GetCheck();
	m_nRightLimit[nAxial] = ((CButton*)(GetDlgItem(IDC_CHECK_RightLimitDisable)))->GetCheck();
	m_nExchangeLimit[nAxial] = ((CButton*)(GetDlgItem(IDC_CHECK_LimitExchange)))->GetCheck();
}

void CDlgLimitParam::OnBnClickedButtonLimitlevelreversal()
{
	// TODO: 在此添加控件通知处理程序代码
	int nAxial = 0;
	CComboBox* aAxial = ((CComboBox*)(GetDlgItem(IDC_COMBO_Axis)));
	if( aAxial != NULL )
		nAxial = aAxial->GetCurSel();
	m_nLeftLimitLevel[nAxial] = ((CButton*)(GetDlgItem(IDC_CHECK_LeftLimitLevelReversal)))->GetCheck();
	m_nRightLimitLevel[nAxial] = ((CButton*)(GetDlgItem(IDC_CHECK_RightLimitLevelReversal)))->GetCheck();
}
