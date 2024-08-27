// dlginputacq.cpp : implementation file
// Author : J.Y.Ma
// Last Modify : 2006.09.12

#include "stdafx.h"
#include "XXX2.h"
#include "dlginputacq.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInputAcq dialog


CDlgInputAcq::CDlgInputAcq(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputAcq::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInputAcq)
	m_nHV = 20;
	m_nMag = 1000;
	//}}AFX_DATA_INIT
}


void CDlgInputAcq::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInputAcq)
	DDX_Text(pDX, IDC_EDIT_INPUTACQ_SAVEASSEM_HV, m_nHV);
	DDV_MinMaxInt(pDX, m_nHV, 0, 30);
	DDX_Text(pDX, IDC_EDIT_INPUTACQ_SAVEASSEM_MAG, m_nMag);
	DDV_MinMaxInt(pDX, m_nMag, 15, 150000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInputAcq, CDialog)
	//{{AFX_MSG_MAP(CDlgInputAcq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInputAcq message handlers

void CDlgInputAcq::Localize(void)
{ 
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_STATIC_INPUTACQ_PROMPT)->SetWindowText(GetResString(IDS_STATIC_INPUTACQ_PROMPT)); 
	GetDlgItem(IDC_STATIC_INPUTACQ_HV)->SetWindowText(GetResString(IDS_STATIC_INPUTACQ_HV)); 
	GetDlgItem(IDC_STATIC_INPUTACQ_KV)->SetWindowText(GetResString(IDS_STATIC_INPUTACQ_KV)); 
	GetDlgItem(IDC_STATIC_INPUTACQ_MAG)->SetWindowText(GetResString(IDS_STATIC_INPUTACQ_MAG)); 

	GetDlgItem(IDOK)->SetWindowText(GetResString(IDS_OK));
	GetDlgItem(IDCANCEL)->SetWindowText(GetResString(IDS_CANCEL));

	SetWindowText(GetResString(IDS_DLG_InputACQ_ForSEM)); 
} 

BOOL CDlgInputAcq::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
