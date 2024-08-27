// DlgProgress.cpp : implementation file
//

#include "stdafx.h"
#include "Options_XYStage.h"
#include "DlgProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog
#define TimmerCtrlID			112


CDlgProgress::CDlgProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProgress)
	m_csMessage = _T("");
	//}}AFX_DATA_INIT

	m_csTitle			= _T("");

	m_nTimmeID			= NULL;
	m_nTimerElapse		= 1000;
	m_iRangeFrom		= 0;
	m_iRangeTo			= 10;

}


void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProgress)
	DDX_Control(pDX, IDC_PROGRESS_RATE, m_Progress);
	DDX_Text(pDX, IDC_STATIC_MSG_PROGRESS, m_csMessage);
	DDV_MaxChars(pDX, m_csMessage, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
	//{{AFX_MSG_MAP(CDlgProgress)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress message handlers

void CDlgProgress::Localize(void)
{ 
	if( m_hWnd == NULL )
		return;

	CString str = _T("");
	str.LoadString(IDS_CANCEL);
	GetDlgItem(IDCANCEL)->SetWindowText(str);
} 

BOOL CDlgProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText( m_csTitle );

	m_iPos = m_iRangeFrom;
	m_Progress.SetRange( m_iRangeFrom, m_iRangeTo );
	m_Progress.SetPos( m_iPos );
	m_Progress.SetStep( 1 );

	m_nTimmeID = SetTimer( TimmerCtrlID,  m_nTimerElapse, NULL );

	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProgress::OnDestroy() 
{
	if (m_nTimmeID != NULL)
	{
		KillTimer( m_nTimmeID );
		m_nTimmeID = NULL;
	}	

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CDlgProgress::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_iPos < m_iRangeTo)
	{
		m_Progress.StepIt( );
		m_iPos ++;
	}
	
	//CDialog::OnTimer(nIDEvent);
}
