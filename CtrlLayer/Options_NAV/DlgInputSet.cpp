// InputSetDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgInputSet.h"

// CDlgInputSet 对话框

IMPLEMENT_DYNAMIC(CDlgInputSet, CDialog)

CDlgInputSet::CDlgInputSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputSet::IDD, pParent)
{
	m_strInput = "";
}

CDlgInputSet::~CDlgInputSet()
{
}

void CDlgInputSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgInputSet, CDialog)
END_MESSAGE_MAP()

// CDlgInputSet 消息处理程序

void CDlgInputSet::Localize()
{
	char szBuf[255];
	// 界面元素
	if(GetPrivateProfileString("InputDlg", "Cap", "", szBuf, 50, m_cPathLocalize) != 0)
		SetWindowText( szBuf );
	if(GetPrivateProfileString("InputDlg", "ENTER_BUTTON", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDOK)->SetWindowText( szBuf );
}

BOOL CDlgInputSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	CEdit* pInput = (CEdit*)(GetDlgItem(IDC_EDIT_NAV_InputPassword));
	if( pInput != NULL )
	{
		long lStyle = GetWindowLong(pInput->m_hWnd, GWL_STYLE);
		SetWindowLong(pInput->m_hWnd, GWL_STYLE, lStyle | ES_PASSWORD);
		pInput->SetPasswordChar('*');
		pInput->SetWindowText(m_strInput);
	}
	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgInputSet::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	GetDlgItem(IDC_EDIT_NAV_InputPassword)->GetWindowText(m_strInput);
	CDialog::OnOK();
}
