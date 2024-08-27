// DlgAutoTestVAC.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgAutoTestVAC.h"
#include "ControlLayer.h"
#include "BoardComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestVAC dialog


CDlgAutoTestVAC::CDlgAutoTestVAC(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoTestVAC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoTestVAC)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgAutoTestVAC::~CDlgAutoTestVAC()
{
	m_brushError.DeleteObject();
}

void CDlgAutoTestVAC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoTestVAC)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutoTestVAC, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoTestVAC)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_VAC_Start, OnStart)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_VAC_Clear, OnClear)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestVAC message handlers

HBRUSH CDlgAutoTestVAC::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_STATIC )
	{
		int nID = pWnd->GetDlgCtrlID();
		int nIndex = nID - IDC_STATIC_AT_Cali_VAC_0_0;
		if( nID == IDC_STATIC_AT_Cali_VAC_0_0 +nIndex
			&& nIndex >=0 && nIndex <= 22
			&& CControlLayer::Instance().m_AT.bVAC_OutofRange[nIndex] )
		{
			pDC->SetTextColor(RGB(255,255,255));	// 设置文本颜色  
			pDC->SetBkMode(TRANSPARENT);			// 设置背景透明
			hbr = (HBRUSH)m_brushError;
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,0));			// 设置文本颜色  
			pDC->SetBkMode(TRANSPARENT);			// 设置背景透明
		}
	}

	return hbr;
}

void CDlgAutoTestVAC::InitParams()
{
	/////////////////////////////////////////////////////////////////
	// VAC ADC#0:
	for(int i=0; i<16; i++ )
		GetDlgItem( IDC_STATIC_AT_Cali_VAC_0_0 +i)->SetWindowText( CControlLayer::Instance().m_DL.adc.chn[2*16+i].strLabel );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// VAC ADC#1:
	for(int i=0; i<6; i++ )
		GetDlgItem( IDC_STATIC_AT_Cali_VAC_1_0 +i)->SetWindowText( CControlLayer::Instance().m_DL.adc.chn[3*16+i].strLabel );
	/////////////////////////////////////////////////////////////////

	CControlLayer::Instance().m_AT.pAutoTestVAC = new double[22];
	for(int i=0; i<22; i++ )
	{
		CControlLayer::Instance().m_AT.pAutoTestVAC[i] = 0.0;
		CControlLayer::Instance().m_AT.bVAC_OutofRange[i] = FALSE;
	}

	m_brushError.CreateSolidBrush(RGB(255,0,0));
}

void CDlgAutoTestVAC::OnStart() 
{
	// TODO: Add your control notification handler code here
	// 暂停真空状态机的工作
	if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
		CSMvac::Instance().SetParams( 22, 1 );
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_VAC_Start)))->SetWindowText( "In Progress");
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_VAC_Start)))->EnableWindow( FALSE );

	/////////////////////////////////////////////////////////////////
	// 读回ADC，并显示回读值
	/////////////////////////////////////////////////////////////////
	CString str, strRes;
	double dValue = 0.0;
	int i, k=0;
	MSG msg;
	/////////////////////////////////////////////
	// VAC0
	CBoardComm::Instance().ChangedADCselectOB( 2 );
	for( i=0; i<16; i++ )
	{
		dValue = 0.0;
		CBoardComm::Instance().ChangedADCsChSelect( i , CControlLayer::Instance().m_DL.adc.chn );
		str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
		strRes += str;
		CControlLayer::Instance().m_AT.pAutoTestVAC[k] = dValue;
		str.Format( "%.6f", CControlLayer::Instance().m_AT.pAutoTestVAC[i] );
		GetDlgItem( IDC_EDIT_AT_Cali_VAC_VAC00_Vadc + k )->SetWindowText( str );
		k++;
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			GetMessage( &msg, NULL, 0, 0 );
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// VAC1
	CBoardComm::Instance().ChangedADCselectOB( 3 );
	for( i=0; i<6; i++ )
	{
		dValue = 0.0;
		CBoardComm::Instance().ChangedADCsChSelect( i , CControlLayer::Instance().m_DL.adc.chn );
		str = CBoardComm::Instance().ReadSelectedADC_Logging2( 80, dValue, CControlLayer::Instance().m_DL.adc, CControlLayer::Instance().m_DL.log );
		strRes += str;
		CControlLayer::Instance().m_AT.pAutoTestVAC[k] = dValue;
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_VAC_VAC00_Vadc + k )->SetWindowText( str );
		k++;
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			GetMessage( &msg, NULL, 0, 0 );
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	/////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// 显示ADC计算分析值
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////
	// 计算值
	k = 0;
	// VAC0
	for( i=0; i<16; i++ )
	{
		dValue = CBoardComm::Instance().GetPhysicsFromAdcs( 2*16 +i, CControlLayer::Instance().m_AT.pAutoTestVAC[k] , CControlLayer::Instance().m_DL.log.strCalc);
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_VAC_VAC00_Vadc3 +k )->SetWindowText( str );
		k++;
	}

	// VAC1
	for( i=0; i<6; i++ )
	{
		dValue = CBoardComm::Instance().GetPhysicsFromAdcs( 3*16 +i, CControlLayer::Instance().m_AT.pAutoTestVAC[k] , CControlLayer::Instance().m_DL.log.strCalc);
		str.Format( "%.6f", dValue );
		GetDlgItem( IDC_EDIT_AT_Cali_VAC_VAC00_Vadc3 +k )->SetWindowText( str );
		k++;
	}
	/////////////////////////////////////////////////////////////////

	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_VAC_Start)))->SetWindowText( "Start");
	((CButton*)(GetDlgItem(IDC_BUTTON_AT_Cali_VAC_Start)))->EnableWindow( TRUE );
	// 恢复真空状态机的工作
	if( CSemCtrl::Instance().m_nVACBoardType < 1 && CSMvac::Instance().isReady() )
		CSMvac::Instance().SetParams( 22, 0 );
}

void CDlgAutoTestVAC::OnClear() 
{
	// TODO: Add your control notification handler code here
	for( int i=0; i<22; i++ )
	{
		GetDlgItem( IDC_EDIT_AT_Cali_VAC_VAC00_Vadc + i )->SetWindowText( "" );
		GetDlgItem( IDC_EDIT_AT_Cali_VAC_VAC00_Vadc2 + i )->SetWindowText( "" );
		GetDlgItem( IDC_EDIT_AT_Cali_VAC_VAC00_Vadc3 + i )->SetWindowText( "" );
		GetDlgItem( IDC_EDIT_AT_Cali_VAC_VAC00_Vadc4 + i )->SetWindowText( "" );
	}
}
