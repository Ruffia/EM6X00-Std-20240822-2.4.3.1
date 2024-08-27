// DlgInput.cpp : implementation file
//

#include "stdafx.h"
#include "Options_StageMC600Wnd.h"
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
	m_bPassword = FALSE;
	m_strInput = "";
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

void CDlgInput::Localize()
{
	char szBuf[255];
	// ½çÃæÔªËØ
	if(GetPrivateProfileString("Label", "CapInput", "", szBuf, 50, m_cPathLocalize) != 0)
		SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "BtnOK", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDOK)->SetWindowText( szBuf );
}

void CDlgInput::OnOK() 
{
	// TODO: Add extra validation here
	GetDlgItem(IDC_EDIT_Input)->GetWindowText(m_strInput);
	CDialog::OnOK();
}

BOOL CDlgInput::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( m_bPassword )
	{
		CEdit* pInput = (CEdit*)(GetDlgItem(IDC_EDIT_Input));
		if( pInput != NULL )
		{
			long lStyle = GetWindowLong(pInput->m_hWnd, GWL_STYLE);
			SetWindowLong(pInput->m_hWnd, GWL_STYLE, lStyle | ES_PASSWORD);
			pInput->SetPasswordChar('*');
		}
	}
	GetDlgItem(IDC_EDIT_Input)->SetWindowText(m_strInput);
	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
