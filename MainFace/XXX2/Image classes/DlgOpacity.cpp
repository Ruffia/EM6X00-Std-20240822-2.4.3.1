// DlgOpacity.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "DlgOpacity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOpacity dialog


CDlgOpacity::CDlgOpacity(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOpacity::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOpacity)
	m_level = 0;
	//}}AFX_DATA_INIT
}


void CDlgOpacity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOpacity)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Text(pDX, IDC_EDIT1, m_level);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOpacity, CDialog)
	//{{AFX_MSG_MAP(CDlgOpacity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOpacity message handlers

BOOL CDlgOpacity::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
