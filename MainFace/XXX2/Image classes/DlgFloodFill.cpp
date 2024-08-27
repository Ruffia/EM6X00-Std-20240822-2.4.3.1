// DlgFloodFill.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
//#include "MainFrm.h"
//#include "demodoc.h"
#include "DlgFloodFill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFloodFill dialog


CDlgFloodFill::CDlgFloodFill(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFloodFill::IDD, pParent)
{
	m_pParent=NULL;
	//{{AFX_DATA_INIT(CDlgFloodFill)
	m_tol = 0;
	m_select = FALSE;
	m_opacity = 0;
	//}}AFX_DATA_INIT
}

CDlgFloodFill::CDlgFloodFill(CMainFrame * pParent)
{
	m_pParent=pParent;
	m_tol = 0;
	m_select = 0;
}
BOOL CDlgFloodFill::Create()
{
	return CDialog::Create(CDlgFloodFill::IDD);
}

void CDlgFloodFill::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFloodFill)
	DDX_Control(pDX, IDC_BUTTON1, m_btnSelColor);
	DDX_Text(pDX, IDC_EDIT1, m_tol);
	DDV_MinMaxByte(pDX, m_tol, 0, 255);
	DDX_Check(pDX, IDC_CHECK1, m_select);
	DDX_Text(pDX, IDC_EDIT2, m_opacity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFloodFill, CDialog)
	//{{AFX_MSG_MAP(CDlgFloodFill)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFloodFill message handlers

void CDlgFloodFill::OnOK() 
{
	if (m_pParent != 0){ //modeless
		UpdateData(1);
		//((CWnd*)m_pDoc)->PostMessage(...);
		DestroyWindow();
	} else
		CDialog::OnOK();
}

void CDlgFloodFill::OnCancel() 
{
	if (m_pParent != 0)	//modeless
		//((CWnd*)m_pDoc)->PostMessage(...);
		DestroyWindow();
	else
		CDialog::OnCancel();
}

void CDlgFloodFill::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CDlgFloodFill::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_btnSelColor.SetBkgColor(m_color);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFloodFill::OnButton1() 
{
	CColorDialog dlg(m_color, CC_FULLOPEN | CC_ANYCOLOR, this);

	if (dlg.DoModal() == IDOK){
		m_color = dlg.GetColor();
		m_btnSelColor.SetBkgColor(m_color);
		m_btnSelColor.Invalidate();
	}	
}
