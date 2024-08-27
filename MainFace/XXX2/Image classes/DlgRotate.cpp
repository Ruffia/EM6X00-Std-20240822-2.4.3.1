// DlgRotate.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "DlgRotate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////
// CDlgRotate dialog

CDlgRotate::CDlgRotate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRotate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRotate)
	m_angle = 0.0f;
	//}}AFX_DATA_INIT
}

void CDlgRotate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRotate)
	DDX_Control(pDX, IDC_CHECK1, m_ch1);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Text(pDX, IDC_EDIT1, m_angle);
	DDX_Control(pDX, IDC_COMBO_IM, inMethod);
	DDX_Control(pDX, IDC_COMBO_OM, ofMethod);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgRotate, CDialog)
	//{{AFX_MSG_MAP(CDlgRotate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
// CDlgRotate message handlers

BOOL CDlgRotate::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);

	inMethod.SetCurSel(m_method);
	ofMethod.SetCurSel(m_overflow);

	m_ch1.SetCheck(m_keepsize);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRotate::OnOK() 
{
	m_method=inMethod.GetCurSel();
	m_overflow=ofMethod.GetCurSel();

	m_keepsize=m_ch1.GetCheck();

	CDialog::OnOK();
}
