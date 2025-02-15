// DlgDither.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "DlgDither.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDither dialog


CDlgDither::CDlgDither(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDither::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDither)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgDither::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDither)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Control(pDX, IDC_RADIO8, m_r8);
	DDX_Control(pDX, IDC_RADIO7, m_r7);
	DDX_Control(pDX, IDC_RADIO6, m_r6);
	DDX_Control(pDX, IDC_RADIO5, m_r5);
	DDX_Control(pDX, IDC_RADIO4, m_r4);
	DDX_Control(pDX, IDC_RADIO3, m_r3);
	DDX_Control(pDX, IDC_RADIO2, m_r2);
	DDX_Control(pDX, IDC_RADIO1, m_r1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDither, CDialog)
	//{{AFX_MSG_MAP(CDlgDither)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDither message handlers

BOOL CDlgDither::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	
	switch(m_method)
	{
	case 1:
		m_r2.SetCheck(1);
		break;
	case 2:
		m_r3.SetCheck(1);
		break;
	case 3:
		m_r4.SetCheck(1);
		break;
	case 4:
		m_r5.SetCheck(1);
		break;
	case 5:
		m_r6.SetCheck(1);
		break;
	case 6:
		m_r7.SetCheck(1);
		break;
	case 7:
		m_r8.SetCheck(1);
		break;
	default:
		m_r1.SetCheck(1);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDither::OnOK() 
{
	if (m_r2.GetCheck())
		m_method = 1;
	else if (m_r3.GetCheck())
		m_method = 2;
	else if (m_r4.GetCheck())
		m_method = 3;
	else if (m_r5.GetCheck())
		m_method = 4;
	else if (m_r6.GetCheck())
		m_method = 5;
	else if (m_r7.GetCheck())
		m_method = 6;
	else if (m_r8.GetCheck())
		m_method = 7;
	else m_method = 0;
	CDialog::OnOK();
}
