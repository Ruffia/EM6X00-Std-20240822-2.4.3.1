// DlgProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgProgress.h"


// CDlgProgress 对话框

IMPLEMENT_DYNAMIC(CDlgProgress, CDialog)

CDlgProgress::CDlgProgress()
	: CDialog(CDlgProgress::IDD)
{
	m_nTimer= -1;
	m_bStopProgress = FALSE;
	m_strTitle = _T("等待对话框");
	m_strText = _T("处理中......");
}

CDlgProgress::~CDlgProgress()
{
}

void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_PROGRESS,m_ProgCtrl);
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
	//ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgProgress 消息处理程序
BOOL CDlgProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ProgCtrl.SetRange(0,500);
	m_ProgCtrl.SetStep(1);
	m_ProgCtrl.SetPos(0);
	m_nTimer= SetTimer(1,10,NULL);
	SetTitle(m_strTitle);
	SetText(m_strText);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProgress::OnTimer(UINT nIDEvent) 
{
	static int nPos = 0;
	if (nIDEvent == m_nTimer)
	{
		if (m_bStopProgress)
		{
			m_bStopProgress = FALSE;
			OnOK();
		}
		else
		{
			m_ProgCtrl.StepIt();

		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgProgress::OnDestroy() 
{
	if (m_nTimer!= -1)
	{
		KillTimer(m_nTimer);
	}

	CDialog::OnDestroy();
}

void CDlgProgress::SetTitle(LPCTSTR szTitle)
{
	m_strTitle= szTitle;
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		SetWindowText(m_strTitle);
	}
}

void CDlgProgress::StopProgress()
{
	//m_bStopProgress = TRUE;
	OnOK();
}

void CDlgProgress::SetText(LPCTSTR szText)
{
	m_strText= szText;

	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		CWnd* pText= GetDlgItem(IDC_STATIC_TEXT);
		if (pText)
		{
			pText->SetWindowText(m_strText);
		}
	}
}

void CDlgProgress::SetRange(const int nStepCount)
{
	m_ProgCtrl.SetRange(0,nStepCount);
	m_ProgCtrl.SetPos(0);
}

void CDlgProgress::SetProgressInfo(CString strProgressInfo)
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		CWnd* pText= GetDlgItem(IDC_STATIC_PROGRESS_ASYNC);
		if (pText)
		{
			pText->SetWindowText(strProgressInfo);
		}
	}

}

void CDlgProgress::OnClose()
{
	OnOK();
}