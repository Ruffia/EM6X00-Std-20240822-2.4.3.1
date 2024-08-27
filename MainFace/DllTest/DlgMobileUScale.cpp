// DlgMobileUScale.cpp : implementation file
//

#include "stdafx.h"
#include "DllTest.h"
#include "DlgMobileUScale.h"

#include "MainFrm.h"
#include "DllTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMobileUScale dialog


CDlgMobileUScale::CDlgMobileUScale(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMobileUScale::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMobileUScale)
	m_dMag = 100.0;
	//}}AFX_DATA_INIT
}


void CDlgMobileUScale::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMobileUScale)
	DDX_Control(pDX, IDC_SPIN_Mag, m_Mag);
	DDX_Control(pDX, IDC_SPIN_Mag10, m_Mag10);
	DDX_Control(pDX, IDC_SPIN_Mag1000, m_Mag1000);
	DDX_Text(pDX, IDC_EDIT_Mag, m_dMag);
	DDV_MinMaxDouble(pDX, m_dMag, 1., 250000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMobileUScale, CDialog)
	//{{AFX_MSG_MAP(CDlgMobileUScale)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Mag1000, OnDeltaposSPINMag1000)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Mag10, OnDeltaposSPINMag10)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Mag, OnDeltaposSPINMag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMobileUScale message handlers

BOOL CDlgMobileUScale::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMobileUScale::MagChange()
{
	((CDllTestView*)(((CMainFrame *)theApp.GetMainWnd())->GetActiveView()))->CalculateUScale2(1024, m_dMag);
}

void CDlgMobileUScale::OnOK() 
{
	// TODO: Add extra validation here
	MagChange();
//	CDialog::OnOK();
}

void CDlgMobileUScale::CoherenceCheck()
{
	if( m_dMag > 250000 )
		m_dMag = 250000;
	if( m_dMag < 1 )
		m_dMag = 1;
}

void CDlgMobileUScale::OnDeltaposSPINMag1000(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData();
	if( pNMUpDown->iDelta > 0 )
		m_dMag /= 10;
	else
		m_dMag *= 10;
	CoherenceCheck();
	UpdateData(FALSE);
	MagChange();
	*pResult = 0;
}

void CDlgMobileUScale::OnDeltaposSPINMag10(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData();
	if( m_dMag > 10000 )
	{
		if( pNMUpDown->iDelta > 0 )
			m_dMag -= 100;
		else
			m_dMag += 100;
	}
	else
	{
		if( pNMUpDown->iDelta > 0 )
			m_dMag -= 10;
		else
			m_dMag += 10;
	}
	CoherenceCheck();
	UpdateData(FALSE);
	MagChange();
	*pResult = 0;
}

void CDlgMobileUScale::OnDeltaposSPINMag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData();
	if( pNMUpDown->iDelta > 0 )
		m_dMag -= 1;
	else
		m_dMag += 1;
	CoherenceCheck();
	UpdateData(FALSE);
	MagChange();
	*pResult = 0;
}

BOOL CDlgMobileUScale::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		UpdateData();
	}
	return CDialog::PreTranslateMessage(pMsg);
}
