// DlgRemark.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "DlgRemark.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRemark dialog


CDlgRemark::CDlgRemark(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRemark::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRemark)
	m_csRemark = _T("");
	//}}AFX_DATA_INIT
}


void CDlgRemark::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRemark)
	DDX_Text(pDX, IDC_EDIT_REMARK, m_csRemark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRemark, CDialog)
	//{{AFX_MSG_MAP(CDlgRemark)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRemark message handlers

void CDlgRemark::Localize(void)
{ 
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDOK)->SetWindowText(GetResString(IDS_OK));

	SetWindowText(GetResString(IDS_SEMCTL_Remark)); 
} 

BOOL CDlgRemark::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
