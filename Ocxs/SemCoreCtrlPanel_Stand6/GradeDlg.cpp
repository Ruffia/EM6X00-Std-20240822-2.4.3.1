// GradeDlg.cpp : implementation file
//
// Last Modify : 2008.05.09

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand6.h"
#include "GradeDlg.h"
#include "SemCoreCtrlPanel_Stand6Ctl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGradeDlg dialog


CGradeDlg::CGradeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGradeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGradeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nCtrlGrade	= 1; // 0粗调, 1细调, 2微调
	m_pFont			= NULL;
	m_clrColor		= 0;
	m_bAutoVisible	= TRUE;
}


void CGradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGradeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGradeDlg, CDialog)
	//{{AFX_MSG_MAP(CGradeDlg)
	ON_BN_CLICKED(IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_COARSE, OnRadioCoarse)
	ON_BN_CLICKED(IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_FINE, OnRadioFine)
	ON_BN_CLICKED(IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_TINY, OnRadioTiny)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_DIALOG_SEMCORECTRLPANEL_STAND6_CHECK_Rev, OnCHECKRev)
	ON_BN_CLICKED(IDC_DIALOG_SEMCORECTRLPANEL_STAND6_BUTTON_Auto, OnBUTTONAuto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGradeDlg message handlers

BOOL CGradeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add your specialized creation code here
	if (m_nCtrlGrade == 0)
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_COARSE ))->SetCheck( 1 );
	else if (m_nCtrlGrade == 1)
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_FINE ))->SetCheck( 1 );
	else if (m_nCtrlGrade == 2)
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_TINY ))->SetCheck( 1 );
	else 
	{
		m_nCtrlGrade = 1;
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_FINE ))->SetCheck( 1 );
	}

	if ( m_pFont != NULL )
	{
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_COARSE ))->SetFont( m_pFont );
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_FINE ))->SetFont( m_pFont );
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_TINY ))->SetFont( m_pFont );
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_CHECK_Rev ))->SetFont( m_pFont );
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_BUTTON_Auto ))->SetFont( m_pFont );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGradeDlg::ReportGradeChanged()
{
	if ( !m_blReady )
		return;

	m_pStand6CtrlPanel->ReportGradeChanged( m_nCtrlGrade );
}

//-----2008.05.09-----
void CGradeDlg::NotifyGradeChanged()
{
	if ( !m_blReady )
		return;

	m_pStand6CtrlPanel->NotifyGradeChanged();
}

void CGradeDlg::SyncGradeChanged( int nCtrlGrade )
{
	if( m_nCtrlGrade == nCtrlGrade )
		return;

	m_nCtrlGrade = nCtrlGrade;
	if (m_nCtrlGrade == 0)
	{
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_COARSE ))->SetCheck( 1 );
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_FINE ))->SetCheck( 0 );
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_TINY ))->SetCheck( 0 );
	}
	else if (m_nCtrlGrade == 1)
	{
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_COARSE ))->SetCheck( 0 );
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_FINE ))->SetCheck( 1 );
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_TINY ))->SetCheck( 0 );
	}
	else if (m_nCtrlGrade == 2)
	{
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_COARSE ))->SetCheck( 0 );
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_FINE ))->SetCheck( 0 );
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_TINY ))->SetCheck( 1 );
	}
	else 
	{
		m_nCtrlGrade = 1;
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_COARSE ))->SetCheck( 0 );
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_FINE ))->SetCheck( 1 );
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_TINY ))->SetCheck( 0 );
	}
}
//-----2008.05.09-----

void CGradeDlg::NotifyActionChanged( int nIndex, int nCheck )
{
	if ( !m_blReady )
		return;

	m_pStand6CtrlPanel->NotifyActionChanged( nIndex, nCheck );
}

// 09.02.16 添加方法以同步文字显示
void CGradeDlg::SyncResString( int nResourceID, CString strRes )
{
	GetDlgItem( nResourceID )->SetWindowText( strRes );
}

BOOL CGradeDlg::Bind( CSemCoreCtrlPanel_Stand6Ctrl* pStand6CtrlPanel )
{
	if ( pStand6CtrlPanel == NULL )
		return FALSE;

	m_blReady = TRUE;
	m_pStand6CtrlPanel = pStand6CtrlPanel;
	return TRUE;
}

void CGradeDlg::OnRadioCoarse() 
{
	// TODO: Add your control notification handler code here
	if ( m_nCtrlGrade != 0 )
	{
		m_nCtrlGrade		= 0; // 0粗调, 1细调, 2微调	
		ReportGradeChanged();
	}
//-----2008.05.09-----
	NotifyGradeChanged();
}

void CGradeDlg::OnRadioFine() 
{
	// TODO: Add your control notification handler code here
	if ( m_nCtrlGrade != 1 )
	{
		m_nCtrlGrade		= 1; // 0粗调, 1细调, 2微调	
		ReportGradeChanged();
	}
//-----2008.05.09-----
	NotifyGradeChanged();
}

void CGradeDlg::OnRadioTiny() 
{
	// TODO: Add your control notification handler code here
	if ( m_nCtrlGrade != 2 )
	{
		m_nCtrlGrade		= 2; // 0粗调, 1细调, 2微调	
		ReportGradeChanged();
	}
//-----2008.05.09-----
	NotifyGradeChanged();
}

void CGradeDlg::OnCHECKRev() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND6_CHECK_Rev)))->GetCheck();
	NotifyActionChanged( 10, nCheck );
}

void CGradeDlg::OnBUTTONAuto() 
{
	// TODO: Add your control notification handler code here
	NotifyActionChanged( 11, 0 );
}

void CGradeDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CStatic* pCtrl;
	CRect	 rectClient, rectCtrl;

	GetClientRect( (LPRECT)rectClient );
	rectCtrl.top	= rectClient.top;
	rectCtrl.bottom = rectClient.bottom;

	pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_COARSE);
	if ( pCtrl != NULL )
	{
		rectCtrl.left	= rectClient.left;
		rectCtrl.right	= rectClient.left + rectClient.Width()/5;

		pCtrl->SetWindowPos(NULL,
			rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(),
			SWP_NOZORDER );
		pCtrl->ShowWindow(SW_SHOW);
	}

	pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_FINE);
	if ( pCtrl != NULL )
	{
		rectCtrl.left	= rectClient.left + rectClient.Width()/5;
		rectCtrl.right	= rectClient.left + rectClient.Width()/5*2;

		pCtrl->SetWindowPos(NULL,
			rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(),
			SWP_NOZORDER );
		pCtrl->ShowWindow(SW_SHOW);
	}

	pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_TINY);
	if ( pCtrl != NULL )
	{
		rectCtrl.left	= rectClient.left + rectClient.Width()/5*2;
		rectCtrl.right	= rectClient.left + rectClient.Width()/5*3;

		pCtrl->SetWindowPos(NULL,
			rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(),
			SWP_NOZORDER );
		pCtrl->ShowWindow(SW_SHOW);
	}

	pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND6_CHECK_Rev);
	if ( pCtrl != NULL )
	{
		rectCtrl.left	= rectClient.left + rectClient.Width()/5*3;
		rectCtrl.right	= rectClient.left + rectClient.Width()/5*4;
		
		pCtrl->SetWindowPos(NULL,
			rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(),
			SWP_NOZORDER );
		pCtrl->ShowWindow(SW_SHOW);
	}

	pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND6_BUTTON_Auto);
	if ( pCtrl != NULL )
	{
		rectCtrl.left	= rectClient.left + rectClient.Width()/5*4;
		rectCtrl.right	= rectClient.right;
		
		pCtrl->SetWindowPos(NULL,
			rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(),
			SWP_NOZORDER );
		pCtrl->ShowWindow(SW_SHOW);
		if( !m_bAutoVisible )
			pCtrl->EnableWindow(FALSE);
	}
}
