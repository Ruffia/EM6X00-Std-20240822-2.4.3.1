// DlgFFT.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "ImageDoc.h"
#include "DlgFFT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFFT dialog


CDlgFFT::CDlgFFT(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFFT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFFT)
	//}}AFX_DATA_INIT
}


void CDlgFFT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFFT)
	DDX_Control(pDX, IDC_CHECK1, m_cinverse);
	DDX_Control(pDX, IDC_CHECK3, m_cmagnitude);
	DDX_Control(pDX, IDC_CHECK4, m_cforcefft);
	DDX_Control(pDX, IDC_IMAG, m_imag);
	DDX_Control(pDX, IDC_REAL, m_real);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Control(pDX, IDOK, m_ok);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFFT, CDialog)
	//{{AFX_MSG_MAP(CDlgFFT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFFT message handlers

BOOL CDlgFFT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);

	m_cforcefft.SetCheck(1);
	//m_cmagnitude.SetCheck(1);

	m_real.AddString(_T("(None)"));
	m_real.SetItemData(m_real.GetCount()-1, 0);
	m_imag.AddString(_T("(None)"));
	m_imag.SetItemData(m_imag.GetCount()-1, 0);

		POSITION pos2 = theApp.m_pSemImageDocTemplate->GetFirstDocPosition();
		while (pos2 != NULL) {
			CImageDoc* pDoc = (CImageDoc*)(theApp.m_pSemImageDocTemplate->GetNextDoc(pos2));
			ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CImageDoc)));
			CString title = pDoc->GetTitle();
			m_real.AddString(title);
			m_real.SetItemData(m_real.GetCount()-1, (DWORD)pDoc);
			m_imag.AddString(title);
			m_imag.SetItemData(m_imag.GetCount()-1, (DWORD)pDoc);
		}
	m_real.SetCurSel(0);
	m_imag.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFFT::OnOK() 
{
	pDocReal = (CImageDoc*) m_real.GetItemData(m_real.GetCurSel());
	pDocImag = (CImageDoc*) m_imag.GetItemData(m_imag.GetCurSel());

	if (pDocReal==0 && pDocImag==0) return;

	bInverse = m_cinverse.GetCheck() ? -1 : 1;
	bForceFFT = m_cforcefft.GetCheck();
	bMagnitude = m_cmagnitude.GetCheck();
	
	CDialog::OnOK();
}
