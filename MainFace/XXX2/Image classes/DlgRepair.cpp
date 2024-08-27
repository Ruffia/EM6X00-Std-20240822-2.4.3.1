// DlgRepair.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "DlgRepair.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRepair dialog


CDlgRepair::CDlgRepair(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRepair::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRepair)
	m_radius = 0.0f;
	m_iterations = 0;
	//}}AFX_DATA_INIT
}


void CDlgRepair::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRepair)
	DDX_Control(pDX, IDC_COMBO9, m_cs);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Text(pDX, IDC_EDIT2, m_radius);
	DDX_Text(pDX, IDC_EDIT3, m_iterations);
	DDV_MinMaxInt(pDX, m_iterations, 1, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRepair, CDialog)
	//{{AFX_MSG_MAP(CDlgRepair)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRepair message handlers

BOOL CDlgRepair::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	
	m_cs.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRepair::OnOK() 
{
	m_ncs = m_cs.GetCurSel();
	CDialog::OnOK();
}
