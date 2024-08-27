// DlgThreshold.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "DlgThreshold.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgThreshold dialog


CDlgThreshold::CDlgThreshold(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgThreshold::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgThreshold)
	m_level = 0;
	m_mean = 0;
	m_thresh1 = 0;
	m_thresh2 = 0;
	//}}AFX_DATA_INIT
}


void CDlgThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgThreshold)
	DDX_Control(pDX, IDC_CHECK1, m_cPreserve);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Text(pDX, IDC_EDIT1, m_level);
	DDV_MinMaxByte(pDX, m_level, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_mean);
	DDX_Text(pDX, IDC_EDIT4, m_thresh1);
	DDX_Text(pDX, IDC_EDIT5, m_thresh2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgThreshold, CDialog)
	//{{AFX_MSG_MAP(CDlgThreshold)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgThreshold message handlers

BOOL CDlgThreshold::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	m_cPreserve.SetCheck(m_bPreserve);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgThreshold::OnOK() 
{
	m_bPreserve = m_cPreserve.GetCheck();

	CDialog::OnOK();
}
