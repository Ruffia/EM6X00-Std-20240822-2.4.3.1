// DlgCCDtest.cpp : implementation file
//

#include "stdafx.h"
#include "DllTest.h"
#include "DlgCCDtest.h"

#include "ExportCameraFunc.h"
//#pragma comment(lib, "..\\..\\Bin\\Options_Camera.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCCDtest dialog


CDlgCCDtest::CDlgCCDtest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCCDtest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCCDtest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCCDtest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCCDtest)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCCDtest, CDialog)
	//{{AFX_MSG_MAP(CDlgCCDtest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCCDtest message handlers

BOOL CDlgCCDtest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ShowCameraWnd(this->m_hWnd);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
