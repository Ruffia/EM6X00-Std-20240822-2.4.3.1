// DlgInput.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInput dialog


CDlgInput::CDlgInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nTitleIndex	= 0;
	m_strInput		= _T("");
}


void CDlgInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInput)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInput, CDialog)
	//{{AFX_MSG_MAP(CDlgInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInput message handlers

void CDlgInput::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDOK)->SetWindowText(GetResString(IDS_OK));
	GetDlgItem(IDCANCEL)->SetWindowText(GetResString(IDS_CANCEL));

	if( m_nTitleIndex > 0 )
		SetWindowText(GetResString( m_nTitleIndex ));
	else
		SetWindowText("");
}

BOOL CDlgInput::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Localize();

	GetDlgItem( IDC_EDIT_Input )->SetWindowText( m_strInput );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInput::OnOK() 
{
	// TODO: Add extra validation here
	GetDlgItem( IDC_EDIT_Input )->GetWindowText( m_strInput );
	
	CDialog::OnOK();
}
