// DlgAutoSum.cpp : implementation file
//

#include "stdafx.h"
#include "AutoTest.h"
#include "DlgAutoSum.h"
#include "DlgDiagLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoSum dialog


CDlgAutoSum::CDlgAutoSum(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoSum::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoSum)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSemCtrl	= NULL;
}


void CDlgAutoSum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoSum)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutoSum, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoSum)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Main_Tab_Power, OnOpenAutoTest)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Main_Tab_MCI, OnOpenAutoCalibration)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Main_Tab_VAC, OnOpenDiagLogSetup)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Main_Tab_SCN, OnStartDiagnose)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Main_Tab_LNS, OnStartLogging)
	ON_BN_CLICKED(IDC_BUTTON_Scan_Start, OnScanStart)
	ON_BN_CLICKED(IDC_BUTTON_Scan_Stop, OnScanStop)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Main_Tab_SPR, OnHVto30KV)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoSum message handlers

void CDlgAutoSum::PostNcDestroy() 
{
	// Free the C++ class
	delete this;
}

BOOL CDlgAutoSum::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//--Auto Test--------------------------
	if (!m_dlgAutotest.Create( CDlgAutoTestMain::IDD, this ))
	{
		TRACE0("Failed to create m_dlgAutotest dlg\n");
		return FALSE;       // fail to create
	}
	m_dlgAutotest.ShowWindow(SW_HIDE);
	if( m_dlgAutotest.m_hWnd != NULL )
		m_dlgAutotest.InitParams();

	//--Calibration------------------------
	if (!m_dlgCali.Create( CDlgCaliMain::IDD, this ))
	{
		TRACE0("Failed to create m_dlgCali dlg\n");
		return FALSE;       // fail to create
	}
	m_dlgCali.ShowWindow(SW_HIDE);
	if( m_dlgCali.m_hWnd != NULL )
		m_dlgCali.InitParams();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAutoSum::OnDestroy() 
{
	if( m_dlgAutotest.m_hWnd != NULL )
		m_dlgAutotest.DestroyWindow();
	if( m_dlgCali.m_hWnd != NULL )
		m_dlgCali.DestroyWindow();

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CDlgAutoSum::Bind( void* pSemCtrl )
{
	if( pSemCtrl != NULL )
	{
		CSemCtrlPanelManager::Instance().Bind( );
	}
}

void CDlgAutoSum::OnOpenAutoTest() 
{
	// TODO: Add your control notification handler code here
	if( m_dlgAutotest.m_hWnd != NULL )
	{
/*		BOOL bVisible = ((m_dlgAutotest.GetStyle() & WS_VISIBLE) != 0);
		if (bVisible)
			m_dlgAutotest.ShowWindow(SW_HIDE);
		else
*/			m_dlgAutotest.ShowWindow(SW_SHOW);
	}
}

void CDlgAutoSum::OnOpenAutoCalibration() 
{
	// TODO: Add your control notification handler code here
	if( m_dlgCali.m_hWnd != NULL )
		m_dlgCali.ShowWindow(SW_SHOW);
}

void CDlgAutoSum::OnOpenDiagLogSetup() 
{
	// TODO: Add your control notification handler code here
	CDlgDiagLog dlg;
	dlg.DoModal();
}

void CDlgAutoSum::OnStartDiagnose() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgAutoSum::OnStartLogging() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgAutoSum::OnScanStart() 
{
	// Clear FIFO
	unsigned long lRet = CSemCtrl::Instance().USB_ClearFIFO();//TIME_METHOD_WAIT_INPUT_ENABLE);
	// Start scan
	CSemCtrl::Instance().USB_WriteToBoard( USB_StartScan, FALSE, 0 );
}

void CDlgAutoSum::OnScanStop() 
{
	CSemCtrl::Instance().USB_WriteToBoard( USB_StopScan, FALSE, 0 );
}

void CDlgAutoSum::OnHVto30KV() 
{
	// TODO: Add your control notification handler code here
	VARIANT Xpos, Ypos, Flag, ClassID, SerialNumber;
	Xpos.lVal			= 120;
	Flag.lVal			= 1;
	SerialNumber.lVal	= -1;
	ClassID.lVal		= SemControlPanelClass_CPS_HV;
	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	ClassID.lVal		= SemControlPanelClass_SP_HV;
	theApp.SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
}
