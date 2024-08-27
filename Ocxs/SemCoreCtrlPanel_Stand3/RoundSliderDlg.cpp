// RoundSliderDlg.cpp : implementation file
//
// Last Modify : 2009.03.11

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand3.h"
#include "RoundSliderDlg.h"
#include "SemCoreCtrlPanel_Stand3Ctl.h"
#include "PublicFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRoundSliderDlg dialog
CString csRoundCtrlGrade[] = {"粗调", "细调", "微调"};


CRoundSliderDlg::CRoundSliderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRoundSliderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRoundSliderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_lMin = 0;
	m_lMax = 20000 - 1;
	m_lPos = 5000;
	m_csRoundCtrl		= _T("-?-");
	m_nRoundCtrlGrade	= 1; // 0粗调, 1细调, 2微调

	m_bGradeVisible = TRUE;
}


void CRoundSliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRoundSliderDlg)
	DDX_Control(pDX, IDC_DIALOG_SEMCORECTRLPANEL_STAND3_SLIDER, m_ctrlRoundSlider);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRoundSliderDlg, CDialog)
	//{{AFX_MSG_MAP(CRoundSliderDlg)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_COARSE, OnRadioCoarse)
	ON_BN_CLICKED(IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_FINE, OnRadioFine)
	ON_BN_CLICKED(IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_TINY, OnRadioTiny)
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoundSliderDlg message handlers

void CRoundSliderDlg::ReportPos()
{
	if ( !m_blReady )
		return;

	m_pCoreCtrlPanel->ReportPos( m_lPos );
}

BOOL CRoundSliderDlg::Bind( CSemCoreCtrlPanel_Stand3Ctrl* pCoreCtrlPanel )
{
	if ( pCoreCtrlPanel == NULL )
		return FALSE;

	m_blReady = TRUE;
	m_pCoreCtrlPanel = pCoreCtrlPanel;
	return TRUE;
}


void CRoundSliderDlg::SetRange( long lMin, long lMax )
{
	m_lMin = lMin;
	m_lMax = lMax;

	if (m_lPos < m_lMin)
		m_lPos = m_lMin;
	if (m_lPos > m_lMax)
		m_lPos = m_lMax;

	AdjustRoundSlider();
}


void CRoundSliderDlg::SetPos( long lPos )
{
	m_lPos = lPos;

	if (m_lPos < m_lMin)
		m_lPos = m_lMin;
	if (m_lPos > m_lMax)
		m_lPos = m_lMax;

	AdjustRoundSlider();
}

void CRoundSliderDlg::AdjustRoundSlider()
{
	if ( m_ctrlRoundSlider.m_hWnd == NULL )
		return;

	if (m_bGradeVisible)
	{
		m_ctrlRoundSlider.SetRangeEx(m_lMin, m_lMax, 100);
		m_ctrlRoundSlider.SetPosEx(m_lPos);

		switch (m_nRoundCtrlGrade)
		{
			case 0:
				OnRadioCoarse();
				break;
			case 1:
				OnRadioFine();
				break;
			case 2:
				OnRadioTiny();
				break;
			default:
				OnRadioFine();
				break;
		}
	}
	else
	{
		m_ctrlRoundSlider.SetCircle( TRUE );
		m_ctrlRoundSlider.SetRangeEx(m_lMin, m_lMax, m_lMax-m_lMin+1);
		m_ctrlRoundSlider.SetPosEx(m_lPos);

		m_ctrlRoundSlider.SetText(
			m_csRoundCtrl + " " + "%.1f");
		m_ctrlRoundSlider.RedrawWindow();
	}
}

BOOL CRoundSliderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	///////////////////////////////////////////
	// 初始化 RoundSlider
	///////////////////////////////////////////
/*	const COLORREF TextColor = RGB(0, 0, 255);
	
	m_ctrlRoundSlider.SetFontName("Microsoft Sans Serif");
//	m_ctrlRoundSlider.SetFontSize(16);
	m_ctrlRoundSlider.SetFontBold();
	//m_ctrlRoundSlider.SetFontItalic();
	m_ctrlRoundSlider.SetTextColor(TextColor);
*/	
	m_ctrlRoundSlider.SetRadioButtonStyle();
	AdjustRoundSlider();
	///////////////////////////////////////////
	
	if (m_nRoundCtrlGrade == 0)
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_COARSE ))->SetCheck( 1 );
	else if (m_nRoundCtrlGrade == 1)
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_FINE ))->SetCheck( 1 );
	else if (m_nRoundCtrlGrade == 2)
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_TINY ))->SetCheck( 1 );
	else 
	{
		m_nRoundCtrlGrade = 1;
		((CButton *)GetDlgItem( IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_FINE ))->SetCheck( 1 );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRoundSliderDlg::OnRadioCoarse() 
{
	// TODO: Add your control notification handler code here
	m_nRoundCtrlGrade		= 0; // 0粗调, 1细调, 2微调
	
	m_ctrlRoundSlider.SetLineSize(100);
	m_ctrlRoundSlider.SetPageSize(1000);
	m_ctrlRoundSlider.SetRoundRange(10000);
	
	m_ctrlRoundSlider.SetText(
		m_csRoundCtrl + csRoundCtrlGrade[m_nRoundCtrlGrade] +"%ld");
	m_ctrlRoundSlider.RedrawWindow();
}

void CRoundSliderDlg::OnRadioFine() 
{
	// TODO: Add your control notification handler code here
	m_nRoundCtrlGrade		= 1; // 0粗调, 1细调, 2微调
	
	m_ctrlRoundSlider.SetLineSize(10);
	m_ctrlRoundSlider.SetPageSize(100);
	m_ctrlRoundSlider.SetRoundRange(1000);
	
	m_ctrlRoundSlider.SetText(
		m_csRoundCtrl + csRoundCtrlGrade[m_nRoundCtrlGrade] +"%ld");
	m_ctrlRoundSlider.RedrawWindow();
}

void CRoundSliderDlg::OnRadioTiny() 
{
	// TODO: Add your control notification handler code here
	m_nRoundCtrlGrade		= 2; // 0粗调, 1细调, 2微调
	
	m_ctrlRoundSlider.SetLineSize(1);
	m_ctrlRoundSlider.SetPageSize(10);
	m_ctrlRoundSlider.SetRoundRange(100);
	
	m_ctrlRoundSlider.SetText(
		m_csRoundCtrl + csRoundCtrlGrade[m_nRoundCtrlGrade] +"%ld");
	m_ctrlRoundSlider.RedrawWindow();
}

void CRoundSliderDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (pScrollBar->GetDlgCtrlID() == IDC_DIALOG_SEMCORECTRLPANEL_STAND3_SLIDER)
	{
		if (!IsAvoidableScrollCode(nSBCode))
		{
			if (m_ctrlRoundSlider.GetPosEx() != m_lPos)
			{
				m_lPos = m_ctrlRoundSlider.GetPosEx();
				ReportPos();
			}
		}
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRoundSliderDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	// 09.02.17 将档位的更新单做一个函数
	UpdateGrade();
}

// 09.02.17 当属性页中有设置改变时更新显示
void CRoundSliderDlg::SettingChanged( BOOL bGradeChanged )
{
	if( m_bGradeVisible )
		m_ctrlRoundSlider.SetText(
			m_csRoundCtrl + " " + csRoundCtrlGrade[m_nRoundCtrlGrade] +" " +"%.1f");
	else
		m_ctrlRoundSlider.SetText(
			m_csRoundCtrl + " " + "%.1f");
	if( bGradeChanged )
		UpdateGrade();
	m_ctrlRoundSlider.RedrawWindow();
}

// 09.02.17 将档位的更新单做一个函数
void CRoundSliderDlg::UpdateGrade()
{
	CStatic* pCtrl;
	CRect	 rectClient, rectCtrl;

	GetClientRect( (LPRECT)rectClient );

	if (!m_bGradeVisible)
	{		
		pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND3_SLIDER);
		if ( pCtrl != NULL )
		{
			rectCtrl = rectClient;

			if ( rectCtrl.Width() > rectCtrl.Height() )
			{
				rectCtrl.left	+= ( rectCtrl.Width() - rectCtrl.Height() ) / 2;
				rectCtrl.right	-= ( rectCtrl.Width() - rectCtrl.Height() ) / 2;
			}
			else
			{
				rectCtrl.top	+= ( rectCtrl.Height() - rectCtrl.Width() ) / 2;
				rectCtrl.bottom	-= ( rectCtrl.Height() - rectCtrl.Width() ) / 2;
			}
			
			pCtrl->SetWindowPos(NULL,
				rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(),
				SWP_NOZORDER );
		}

		pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_COARSE);
		if ( pCtrl != NULL )
			pCtrl->ShowWindow(SW_HIDE);
		
		pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_FINE);
		if ( pCtrl != NULL )
			pCtrl->ShowWindow(SW_HIDE);

		pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_TINY);
		if ( pCtrl != NULL )
			pCtrl->ShowWindow(SW_HIDE);
	}
	else
	{		
		CRect	rcTemp(0,16,0,16);
		
		pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_COARSE);
		if ( pCtrl != NULL )
			pCtrl->GetWindowRect( (LPRECT)rcTemp );

		pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_COARSE);
		if ( pCtrl != NULL )
		{
			rectCtrl.left	= rectClient.left;
			rectCtrl.right	= rectClient.left + rectClient.Width()/3;

			rectCtrl.top	= rectClient.bottom - rcTemp.Height();
			rectCtrl.bottom = rectClient.bottom;
			
			pCtrl->SetWindowPos(NULL,
				rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(),
				SWP_NOZORDER );
			pCtrl->ShowWindow(SW_SHOW);
		}

		pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_FINE);
		if ( pCtrl != NULL )
		{
			rectCtrl.left	= rectClient.left + rectClient.Width()/3;
			rectCtrl.right	= rectClient.left + rectClient.Width()/3*2;

			rectCtrl.top	= rectClient.bottom - rcTemp.Height();
			rectCtrl.bottom = rectClient.bottom;
			
			pCtrl->SetWindowPos(NULL,
				rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(),
				SWP_NOZORDER );
			pCtrl->ShowWindow(SW_SHOW);
		}

		pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_TINY);
		if ( pCtrl != NULL )
		{
			rectCtrl.left	= rectClient.left + rectClient.Width()/3*2;
			rectCtrl.right	= rectClient.right;

			rectCtrl.top	= rectClient.bottom - rcTemp.Height();
			rectCtrl.bottom = rectClient.bottom;
			
			pCtrl->SetWindowPos(NULL,
				rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(),
				SWP_NOZORDER );
			pCtrl->ShowWindow(SW_SHOW);
		}

		pCtrl = (CStatic *)GetDlgItem(IDC_DIALOG_SEMCORECTRLPANEL_STAND3_SLIDER);
		if ( pCtrl != NULL )
		{
			rectCtrl.left	= rectClient.left;
			rectCtrl.right	= rectClient.right;

			rectCtrl.top	= rectClient.top;
			rectCtrl.bottom = rectClient.bottom - rcTemp.Height();

			if ( rectCtrl.Width() > rectCtrl.Height() )
			{
				rectCtrl.left	+= ( rectCtrl.Width() - rectCtrl.Height() ) / 2;
				rectCtrl.right	-= ( rectCtrl.Width() - rectCtrl.Height() ) / 2;
			}
			else
			{
				rectCtrl.top	+= ( rectCtrl.Height() - rectCtrl.Width() ) / 2;
				rectCtrl.bottom	-= ( rectCtrl.Height() - rectCtrl.Width() ) / 2;
			}
			
			pCtrl->SetWindowPos(NULL,
				rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height(),
				SWP_NOZORDER );
		}
	}
}

void CRoundSliderDlg::OnSetFont(CFont* pFont) 
{
	// TODO: Add your specialized code here and/or call the base class
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	
	m_ctrlRoundSlider.SetFontName(lf.lfFaceName);
	m_ctrlRoundSlider.SetFontSize(-lf.lfHeight);
	m_ctrlRoundSlider.SetFontBold(lf.lfWeight);
	m_ctrlRoundSlider.SetFontItalic(lf.lfItalic);
	m_ctrlRoundSlider.SetFontUnderline(lf.lfUnderline);
}

void CRoundSliderDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetPos( 0 );
	ReportPos();
	CDialog::OnRButtonDown(nFlags, point);
}
