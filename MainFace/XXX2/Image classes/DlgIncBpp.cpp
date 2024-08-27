// DlgIncBpp.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "DlgIncBpp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgIncBpp dialog


CDlgIncBpp::CDlgIncBpp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIncBpp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgIncBpp)
	//}}AFX_DATA_INIT
}


void CDlgIncBpp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgIncBpp)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Control(pDX, IDC_RADIO1, m_r4);
	DDX_Control(pDX, IDC_RADIO3, m_r24);
	DDX_Control(pDX, IDC_RADIO2, m_r8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgIncBpp, CDialog)
	//{{AFX_MSG_MAP(CDlgIncBpp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgIncBpp message handlers

BOOL CDlgIncBpp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	
	if (m_bit == 4) m_r4.SetCheck(1);
	else if (m_bit == 8) m_r8.SetCheck(1);
	else m_r24.SetCheck(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgIncBpp::OnOK() 
{
	if (m_r4.GetCheck()) m_bit=4;
	else if (m_r8.GetCheck()) m_bit=8;
	else m_bit=24;
	
	CDialog::OnOK();
}
