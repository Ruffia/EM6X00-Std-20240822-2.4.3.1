// DlgProgress.cpp : implementation file
//

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand8.h"
#include "SemCoreCtrlPanel_Stand8Ctl.h"
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

	m_nTimmeID			= NULL;
	m_nTimerElapse		= 150;
	m_pStand8CtrlPanel	= NULL;
}


void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProgress)
	DDX_Control(pDX, IDC_DIALOG_SEMCORECTRLPANEL_STAND8_Progress, m_Progress);
	DDX_Text(pDX, IDC_DIALOG_SEMCORECTRLPANEL_STAND8_STATIC_Msg, m_csMessage);
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

BOOL CDlgProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Progress.SetRange( m_iRangeFrom, m_iRangeTo );
	m_Progress.SetStep( 1 );
	m_iPos = m_iRangeFrom;

	m_nTimmeID = SetTimer( TimmerCtrlID,  m_nTimerElapse, NULL );
	
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

BOOL CDlgProgress::Bind( CSemCoreCtrlPanel_Stand8Ctrl* pStand8CtrlPanel )
{
	if ( pStand8CtrlPanel == NULL )
		return FALSE;

	m_blReady = TRUE;
	m_pStand8CtrlPanel = pStand8CtrlPanel;
	return TRUE;
}

void CDlgProgress::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	m_Progress.StepIt( );
	m_iPos ++;

	if ( m_blReady )
		m_pStand8CtrlPanel->SetCtrlPos( m_iPos );

	if (m_iPos >= m_iRangeTo)
	{
		CDialog::OnOK();
	}
	
	//CDialog::OnTimer(nIDEvent);
}
