// DlgProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgProgress.h"
#include "afxdialogex.h"


// CDlgProgress 对话框

IMPLEMENT_DYNAMIC(CDlgProgress, CDialog)

CDlgProgress::CDlgProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProgress::IDD, pParent)
{
	
}

CDlgProgress::~CDlgProgress()
{
}

void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_NAV, m_progressCtrl);
	DDX_Control(pDX, IDC_STATIC_NAV_Prompt, m_staticPrompt);
}

BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
END_MESSAGE_MAP()


// CDlgProgress 消息处理程序

BOOL CDlgProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	//CRect rtDesk;
    //CRect rtDlg;
    //::GetWindowRect(::GetDesktopWindow(), &rtDesk);
    //GetWindowRect(&rtDlg);
    //int iXpos = rtDesk.Width() / 2 - rtDlg.Width() / 2;
    //int iYpos = rtDesk.Height() / 2 - rtDlg.Height() / 2;
    //SetWindowPos(NULL, iXpos, iYpos, 0, 0, SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);
	return TRUE;
}

void CDlgProgress::OnCancel()
{
	m_bIsCancelled = TRUE;  // 设置标志位为TRUE
	CDialog::OnCancel();
}
