// WSM30KVTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WSM30KVTest.h"
#include "WSM30KVTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWSM30KVTestDlg dialog

CWSM30KVTestDlg::CWSM30KVTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWSM30KVTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWSM30KVTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	ZeroMemory( &m_cfgWSM30KV, sizeof(Cfg_WSM30KV) );
	ZeroMemory( &m_statusWSM30KV, sizeof(Status_WSM30KV) );
	ZeroMemory( &m_monWSM30KV, sizeof(Mon_WSM30KV) );
	ZeroMemory( &m_setWSM30KV, sizeof(Setting_WSM30KV) );
}

CWSM30KVTestDlg::~CWSM30KVTestDlg()
{
	m_brushHLgreen.DeleteObject();
	m_brushHLred.DeleteObject();
}

void CWSM30KVTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWSM30KVTestDlg)
	DDX_Control(pDX, IDC_CHECK_WSM30KV_V1Test, m_checkV1);
	DDX_Control(pDX, IDC_CHECK_WSM30KV_FIL, m_checkFIL);
	DDX_Control(pDX, IDC_CHECK_WSM30KV_EMS, m_checkEMS);
	DDX_Control(pDX, IDC_CHECK_WSM30KV_ACL, m_checkACL);
	DDX_Control(pDX, IDC_SCROLLBAR_WSM30KV_PMT_V, m_scrollPMTv);
	DDX_Control(pDX, IDC_SCROLLBAR_WSM30KV_FIL_I, m_scrollFILi);
	DDX_Control(pDX, IDC_SCROLLBAR_WSM30KV_ACL_V, m_scrollACLv);
	DDX_Control(pDX, IDC_SCROLLBAR_WSM30KV_ACL_I, m_scrollACLi);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWSM30KVTestDlg, CDialog)
	//{{AFX_MSG_MAP(CWSM30KVTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_WSM30KV_Link, OnBUTTONWSM30KVLink)
	ON_BN_CLICKED(IDC_CHECK_WSM30KV_V1Test, OnCHECKWSM30KVV1Test)
	ON_BN_CLICKED(IDC_CHECK_WSM30KV_ACL, OnCheckWsm30kvAcl)
	ON_BN_CLICKED(IDC_CHECK_WSM30KV_EMS, OnCheckWsm30kvEms)
	ON_BN_CLICKED(IDC_CHECK_WSM30KV_FIL, OnCheckWsm30kvFil)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_Test, OnBUTTONTest)
	ON_BN_CLICKED(IDC_CHECK_WSM30KV_SCI, OnCheckWsm30kvSci)
	ON_BN_CLICKED(IDC_CHECK_WSM30KV_COL, OnCheckWsm30kvCol)
	ON_BN_CLICKED(IDC_CHECK_WSM30KV_PMT, OnCheckWsm30kvPmt)
	ON_BN_CLICKED(IDC_BUTTON_WSM30KV_ClearFault, OnBUTTONWSM30KVClearFault)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_SPELLMAN_PMSKV_SETPARAM, OnWSM30KVSetChanged)
	ON_MESSAGE(WM_USER_SPELLMAN_PMSKV_UpdateSet, OnWSM30KVUpdateSet)
	ON_MESSAGE(WM_USER_SPELLMAN_PMSKV_UpdateMon, OnWSM30KVUpdateMon)
	ON_MESSAGE(WM_USER_SPELLMAN_PMSKV_UpdateStatus, OnWSM30KVUpdateStatus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWSM30KVTestDlg message handlers

BOOL CWSM30KVTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_brushHLgreen.CreateSolidBrush(RGB(0,255,0));
	m_brushHLred.CreateSolidBrush(RGB(255,0,0));
	
	// TODO: Add extra initialization here
	m_scrollACLv.SetScrollRange(0, 30000);
	m_scrollACLi.SetScrollRange(0, 300);
	m_scrollFILi.SetScrollRange(0, 3000);
	m_scrollPMTv.SetScrollRange(0, 1300);
	m_scrollACLv.SetScrollPos(0);
	m_scrollACLi.SetScrollPos(0);
	m_scrollFILi.SetScrollPos(0);
	m_scrollPMTv.SetScrollPos(0);
// 	m_scrollACLv.EnableWindow(FALSE);
// 	m_scrollACLi.EnableWindow(FALSE);
// 	m_scrollFILi.EnableWindow(FALSE);
// 	m_scrollPMTv.EnableWindow(FALSE);
// 	m_checkACL.EnableWindow(FALSE);
// 	m_checkEMS.EnableWindow(FALSE);
// 	m_checkFIL.EnableWindow(FALSE);
	m_checkV1.EnableWindow(FALSE);

	CString str;
	CComboBox* pPort = (CComboBox*)(GetDlgItem( IDC_COMBO_WSM30KV_Port));
	if( pPort != NULL )
	{
		for( int i=0; i<30; i++ )
		{
			str.Format( "COM %d", i+1 );
			pPort->AddString(str);
		}
		pPort->SetCurSel(2);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWSM30KVTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWSM30KVTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWSM30KVTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWSM30KVTestDlg::OnDestroy() 
{
	m_WSM30KV.Close();
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CWSM30KVTestDlg::InitWSM30KV()
{
	m_WSM30KV.InitHWND( this->m_hWnd );
	m_WSM30KV.SetParam(0, m_cfgWSM30KV.nPort);
	m_WSM30KV.Init();
}

void CWSM30KVTestDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CWSM30KVTestDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CWSM30KVTestDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		CWnd* pWnd = GetFocus();
		if( pWnd != NULL )
		{
			CString str = "";
			int nID = pWnd->GetDlgCtrlID();
			switch( nID )
			{
			case	IDC_EDIT_WSM30KV_Set_ACL_V:
			case	IDC_EDIT_WSM30KV_Set_ACL_I:
			case	IDC_EDIT_WSM30KV_Set_FIL_I:
			case	IDC_EDIT_WSM30KV_Set_PMT_V:
				GetDlgItem( nID )->GetWindowText( str );
				break;
			}
			if( str.IsEmpty() )
				return CDialog::PreTranslateMessage(pMsg);
			int nPos = atoi(str);
			switch( nID )
			{
			case	IDC_EDIT_WSM30KV_Set_ACL_V:
				m_scrollACLv.SetScrollPos( nPos );
				Change_ACL_V( nPos );
				break;
			case	IDC_EDIT_WSM30KV_Set_ACL_I:
				m_scrollACLi.SetScrollPos( nPos );
				Change_ACL_I( nPos );
				break;
			case	IDC_EDIT_WSM30KV_Set_FIL_I:
				m_scrollFILi.SetScrollPos( nPos );
				Change_FIL_I( nPos );
				break;
			case	IDC_EDIT_WSM30KV_Set_PMT_V:
				m_scrollPMTv.SetScrollPos( nPos );
				Change_PMT_V( nPos );
				break;
			}
		}
		return TRUE;
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

void CWSM30KVTestDlg::OnBUTTONTest() 
{
	// TODO: Add your control notification handler code here
	m_WSM30KV.SetParam(100, 0);
}

HBRUSH CWSM30KVTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_BTN )
	{
		int nID = pWnd->GetDlgCtrlID();
		int nCheck = ((CButton*)pWnd)->GetCheck();
	}
	else if( nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		int nID = pWnd->GetDlgCtrlID();
		if( nID > IDC_EDIT_WSM30KV_Status_Output_Bit0 -1 && nID < IDC_EDIT_WSM30KV_Status_Output_Bit5 +1 )
			Display_Status_Output( pDC, hbr, nID );
		else if( nID > IDC_EDIT_WSM30KV_Status_PB01_Bit0 -1 && nID < IDC_EDIT_WSM30KV_Status_PB01_Bit5 +1 )
			Display_Status_Protect01( pDC, hbr, nID );
		else if( nID > IDC_EDIT_WSM30KV_Status_PB25_Bit0 -1 && nID < IDC_EDIT_WSM30KV_Status_PB25_Bit5 +1 )
			Display_Status_Protect25( pDC, hbr, nID );
		else if( nID > IDC_EDIT_WSM30KV_Status_PV_Bit0 -1 && nID < IDC_EDIT_WSM30KV_Status_PV_Bit5 +1 )
			Display_Status_ProtectOV( pDC, hbr, nID );
		else if( nID > IDC_EDIT_WSM30KV_Status_PI_Bit0 -1 && nID < IDC_EDIT_WSM30KV_Status_PI_Bit5 +1 )
			Display_Status_ProtectOC( pDC, hbr, nID );
		else if( nID == IDC_EDIT_WSM30KV_Status_PA_Bit1 )
		{
			if( m_statusWSM30KV.statusA25.status.ACL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CWSM30KVTestDlg::Display_Status_Output( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_EDIT_WSM30KV_Status_Output_Bit0:
		{
			if( m_statusWSM30KV.statusO01.status.SCI )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_Output_Bit1:
		{
			if( m_statusWSM30KV.statusO01.status.ACL_FIL )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_Output_Bit2:
		{
			if( m_statusWSM30KV.statusO01.status.EMS )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_Output_Bit3:
		{
			if( m_statusWSM30KV.statusO25.status.ACL_FIL )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_Output_Bit4:
		{
			if( m_statusWSM30KV.statusO01.status.COL )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_Output_Bit5:
		{
			if( m_statusWSM30KV.statusO01.status.PMT )
				hbr = (HBRUSH)m_brushHLgreen;
		}
		break;
	}
}

void CWSM30KVTestDlg::Display_Status_Protect01( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_EDIT_WSM30KV_Status_PB01_Bit0:
		{
			if( m_statusWSM30KV.statusB01.status.P3V3 )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PB01_Bit1:
		{
			if( m_statusWSM30KV.statusB01.status.P5V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PB01_Bit2:
		{
			if( m_statusWSM30KV.statusB01.status.P15V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PB01_Bit3:
		{
			if( m_statusWSM30KV.statusB01.status.N15V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PB01_Bit4:
		{
			if( m_statusWSM30KV.statusB01.status.P24V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PB01_Bit5:
		{
			if( m_statusWSM30KV.statusB01.status.Temp )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	}
}

void CWSM30KVTestDlg::Display_Status_Protect25( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_EDIT_WSM30KV_Status_PB25_Bit0:
		{
			if( m_statusWSM30KV.statusB25.status.P3V3 )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PB25_Bit1:
		{
			if( m_statusWSM30KV.statusB25.status.P5V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PB25_Bit2:
		{
			if( m_statusWSM30KV.statusB25.status.P15V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PB25_Bit3:
		{
			if( m_statusWSM30KV.statusB25.status.N15V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PB25_Bit4:
		{
			if( m_statusWSM30KV.statusB25.status.P24V )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PB25_Bit5:
		{
			if( m_statusWSM30KV.statusB25.status.Temp )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	}
}

void CWSM30KVTestDlg::Display_Status_ProtectOV( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_EDIT_WSM30KV_Status_PV_Bit0:
		{
			if( m_statusWSM30KV.statusV01.status.SCI )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PV_Bit1:
		{
			if( m_statusWSM30KV.statusV01.status.ACL_FIL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PV_Bit3:
		{
			if( m_statusWSM30KV.statusV25.status.ACL_FIL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PV_Bit4:
		{
			if( m_statusWSM30KV.statusV01.status.COL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PV_Bit5:
		{
			if( m_statusWSM30KV.statusV01.status.PMT )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	}
}

void CWSM30KVTestDlg::Display_Status_ProtectOC( CDC* pDC, HBRUSH& hbr, int nID )
{
	switch( nID )
	{
	case	IDC_EDIT_WSM30KV_Status_PI_Bit0:
		{
			if( m_statusWSM30KV.statusI01.status.SCI )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PI_Bit1:
		{
			if( m_statusWSM30KV.statusI01.status.ACL_FIL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PI_Bit3:
		{
			if( m_statusWSM30KV.statusI25.status.ACL_FIL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PI_Bit4:
		{
			if( m_statusWSM30KV.statusI01.status.COL )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	case	IDC_EDIT_WSM30KV_Status_PI_Bit5:
		{
			if( m_statusWSM30KV.statusI01.status.PMT )
			{
				pDC->SetTextColor(RGB(255,255,255));
				hbr = (HBRUSH)m_brushHLred;
			}
			else
				pDC->SetTextColor(RGB(0,0,0));
		}
		break;
	}
}

void CWSM30KVTestDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int nOldPos = pScrollBar->GetScrollPos();
	int nNewPos = pScrollBar->GetScrollPos();
	SCROLLINFO  scrollInfo;
	pScrollBar->GetScrollInfo( &scrollInfo, SIF_ALL );
	switch (nSBCode) 
	{
	case SB_LINELEFT:
		nNewPos = nOldPos-10;								//步进量为1
		nNewPos = ( nNewPos < scrollInfo.nMin )?scrollInfo.nMin:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	case SB_LINERIGHT:
		nNewPos = nOldPos+10;
		nNewPos = ( nNewPos > scrollInfo.nMax )?scrollInfo.nMax:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	case SB_LEFT:
		pScrollBar->SetScrollPos( scrollInfo.nMax );
		break;

	case SB_RIGHT:
		pScrollBar->SetScrollPos( scrollInfo.nMin );
		break;

	case SB_PAGELEFT:
		nNewPos = nOldPos-100;//scrollInfo.nPage;			//页进量为10
		nNewPos = ( nNewPos < scrollInfo.nMin )?scrollInfo.nMin:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	case SB_PAGERIGHT:
		nNewPos = nOldPos+100;//scrollInfo.nPage;
		nNewPos = ( nNewPos > scrollInfo.nMax )?scrollInfo.nMax:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	case SB_ENDSCROLL:
		break;

	case SB_THUMBPOSITION:
		pScrollBar->SetScrollPos( nPos );
		break;

	case SB_THUMBTRACK:
		pScrollBar->SetScrollPos( scrollInfo.nTrackPos );
		break;
	}

	nNewPos = pScrollBar->GetScrollPos();
	if( nOldPos == nNewPos )
		return;

	int id = pScrollBar->GetDlgCtrlID();					//获取滚动条ID
	switch( id )
	{
	case IDC_SCROLLBAR_WSM30KV_ACL_V:
		Change_ACL_V( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_WSM30KV_ACL_I:
		Change_ACL_I( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_WSM30KV_FIL_I:
		Change_FIL_I( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_WSM30KV_PMT_V:
		Change_PMT_V( pScrollBar->GetScrollPos() );
		break;
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CWSM30KVTestDlg::Change_ACL_V( int nPos )
{
	CString str;
	str.Format( "%d", nPos );
	GetDlgItem(IDC_EDIT_WSM30KV_Set_ACL_V)->SetWindowText(str);
	m_WSM30KV.SetParam( 1, nPos );
}

void CWSM30KVTestDlg::Change_ACL_I( int nPos )
{
	CString str;
	str.Format( "%d", nPos );
	GetDlgItem(IDC_EDIT_WSM30KV_Set_ACL_I)->SetWindowText(str);
	m_WSM30KV.SetParam( 2, nPos );
}

void CWSM30KVTestDlg::Change_FIL_I( int nPos )
{
	CString str;
	str.Format( "%d", nPos );
	GetDlgItem(IDC_EDIT_WSM30KV_Set_FIL_I)->SetWindowText(str);
	m_WSM30KV.SetParam( 3, nPos );
}

void CWSM30KVTestDlg::Change_PMT_V( int nPos )
{
	CString str;
	str.Format( "%d", nPos );
	GetDlgItem(IDC_EDIT_WSM30KV_Set_PMT_V)->SetWindowText(str);
	m_WSM30KV.SetParam( 4, nPos );
}

void CWSM30KVTestDlg::OnBUTTONWSM30KVLink() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pPort = (CComboBox*)(GetDlgItem(IDC_COMBO_WSM30KV_Port));
	if( pPort != NULL )
	{
		m_cfgWSM30KV.nPort = pPort->GetCurSel() +1;
		InitWSM30KV();
	}
}

void CWSM30KVTestDlg::OnCHECKWSM30KVV1Test() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_V1Test)))->GetCheck();
	if( nCheck )
	{
		// 开使能，设置初始值
		m_WSM30KV.SetParam( 17, 1 );	// ALL EN
//		Sleep(100);
		m_WSM30KV.SetParam( 4, 0 );		// PMT 0
//		Sleep(100);
		m_WSM30KV.SetParam( 5, 1 );		// SCI OPEN
//		Sleep(100);
		m_WSM30KV.SetParam( 6, 1 );		// COL OPEN
// 		m_checkACL.EnableWindow(TRUE);
// 		m_checkEMS.EnableWindow(TRUE);
// 		m_checkFIL.EnableWindow(TRUE);
// 		m_scrollPMTv.EnableWindow(TRUE);
		m_scrollPMTv.SetScrollPos(0);
	}
	else
	{
		// 各值归零，关使能
		m_scrollACLv.SetScrollPos(0);
		m_scrollACLi.SetScrollPos(0);
		m_scrollFILi.SetScrollPos(0);
		m_scrollPMTv.SetScrollPos(0);
		Change_FIL_I(0);
//		Sleep(100);
		Change_ACL_I(0);
//		Sleep(100);
		Change_ACL_V(0);
//		Sleep(100);
		Change_PMT_V(0);
		m_WSM30KV.SetParam( 17, 0 );	// ALL DISABLE
// 		m_scrollACLv.EnableWindow(FALSE);
// 		m_scrollACLi.EnableWindow(FALSE);
// 		m_scrollFILi.EnableWindow(FALSE);
// 		m_scrollPMTv.EnableWindow(FALSE);
		m_checkACL.SetCheck(0);
		m_checkEMS.SetCheck(0);
		m_checkFIL.SetCheck(0);
// 		m_checkACL.EnableWindow(FALSE);
// 		m_checkEMS.EnableWindow(FALSE);
// 		m_checkFIL.EnableWindow(FALSE);
	}
}

void CWSM30KVTestDlg::OnCheckWsm30kvSci() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_SCI)))->GetCheck();
	if( nCheck )
	{
		m_WSM30KV.SetParam( 11, 1 );	// SCI EN
		m_WSM30KV.SetParam( 5, 1 );
	}
	else
	{
		m_WSM30KV.SetParam( 5, 0 );
		m_WSM30KV.SetParam( 11, 0 );
	}
}

void CWSM30KVTestDlg::OnCheckWsm30kvCol() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_COL)))->GetCheck();
	if( nCheck )
	{
		m_WSM30KV.SetParam( 14, 1 );	// COL EN
		m_WSM30KV.SetParam( 6, 1 );
	}
	else
	{
		m_WSM30KV.SetParam( 6, 0 );
		m_WSM30KV.SetParam( 14, 0 );
	}
}

void CWSM30KVTestDlg::OnCheckWsm30kvPmt() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_PMT)))->GetCheck();
	if( nCheck )
	{
		m_WSM30KV.SetParam( 15, 1 );	// PMT EN
		m_WSM30KV.SetParam( 4, 0 );
		m_scrollPMTv.EnableWindow(TRUE);
		m_scrollPMTv.SetScrollPos(0);
	}
	else
	{
		m_scrollPMTv.SetScrollPos(0);
		m_scrollPMTv.EnableWindow(FALSE);
		m_WSM30KV.SetParam( 4, 0 );
		m_WSM30KV.SetParam( 15, 0 );
	}
}

void CWSM30KVTestDlg::OnCheckWsm30kvAcl() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_ACL)))->GetCheck();
	if( nCheck )
	{
		m_WSM30KV.SetParam( 12, 1 );	// ACL EN
		m_WSM30KV.SetParam( 1, 0 );
		m_scrollACLv.EnableWindow(TRUE);
		m_scrollACLv.SetScrollPos(0);
	}
	else
	{
		m_scrollACLv.SetScrollPos(0);
		m_scrollACLv.EnableWindow(FALSE);
		m_WSM30KV.SetParam( 1, 0 );
		m_WSM30KV.SetParam( 12, 0 );
	}
}

void CWSM30KVTestDlg::OnCheckWsm30kvEms() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_ACL)))->GetCheck();
	if( nCheck )
	{
		m_WSM30KV.SetParam( 13, 1 );	// EMS EN
		m_WSM30KV.SetParam( 2, 0 );
		m_scrollACLi.EnableWindow(TRUE);
		m_scrollACLi.SetScrollPos(0);
	}
	else
	{
		m_scrollACLi.SetScrollPos(0);
		m_scrollACLi.EnableWindow(FALSE);
		m_WSM30KV.SetParam( 2, 0 );
		m_WSM30KV.SetParam( 13, 0 );
	}
}

void CWSM30KVTestDlg::OnCheckWsm30kvFil() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_ACL)))->GetCheck();
	if( nCheck )
	{
		m_WSM30KV.SetParam( 16, 1 );	// FIL EN
		m_WSM30KV.SetParam( 3, 0 );
		m_scrollFILi.EnableWindow(TRUE);
		m_scrollFILi.SetScrollPos(0);
	}
	else
	{
		m_scrollFILi.SetScrollPos(0);
		m_scrollFILi.EnableWindow(FALSE);
		m_WSM30KV.SetParam( 3, 0 );
		m_WSM30KV.SetParam( 16, 0 );
	}
}





LRESULT CWSM30KVTestDlg::OnWSM30KVSetChanged(WPARAM wParam, LPARAM lParam)
{
	CString str;
	if( wParam == 2 )
	{
	}
	return 0;
}

LRESULT CWSM30KVTestDlg::OnWSM30KVUpdateSet(WPARAM wParam, LPARAM lParam)
{
/*	// 灯丝自动过程中，5个设置值的界面同步
	int nPos = (int)lParam;
	CString str, strEdit;
	strEdit.Format( "%d", nPos );
	switch( wParam )
	{
	case	g_nIndexSetEHT:
		theApp.m_cfg50KV.setCur.wAccV	= nPos;
		GetDlgItem( IDC_EDIT_WSM30KV_EHT_1 )->SetWindowText( strEdit );
		str.Format( "AUTO Accelerator = %5d", nPos );
		break;
	case	g_nIndexSetFila:
		theApp.m_cfg50KV.setCur.wFilaI	= nPos;
		GetDlgItem( IDC_EDIT_WSM30KV_FIL_1 )->SetWindowText( strEdit );
		str.Format( "AUTO Filament = %5d", nPos );
		break;
	case	g_nIndexSetSup:
		theApp.m_cfg50KV.setCur.wSupV	= nPos;
		GetDlgItem( IDC_EDIT_WSM30KV_SUP_1 )->SetWindowText( strEdit );
		str.Format( "AUTO Suppressor = %5d", nPos );
		break;
	case	g_nIndexSetExt:
		theApp.m_cfg50KV.setCur.wExtV	= nPos;
		GetDlgItem( IDC_EDIT_WSM30KV_EXT_1 )->SetWindowText( strEdit );
		str.Format( "AUTO Extractor = %5d", nPos );
		break;
	case	g_nIndexSetLens:
		theApp.m_cfg50KV.setCur.wLensV	= nPos;
		GetDlgItem( IDC_EDIT_WSM30KV_LEN_1 )->SetWindowText( strEdit );
		str.Format( "AUTO Lens = %5d", nPos );
		break;
	}
	Message( str );
*/
	return 0;
}

LRESULT CWSM30KVTestDlg::OnWSM30KVUpdateMon(WPARAM wParam, LPARAM lParam)
{
/*	if( wParam > AddrWSM30KVR01_FB_SCI_V -1 && wParam < AddrWSM30KVR01_FB_P3V3 )
	{
		UpdateMon_FB( wParam, lParam );
	}
	else if( wParam > AddrWSM30KVR01_Given_SCI_V -1 && wParam < AddrWSM30KVR01_InquireStatus1 )
	{
		UpdateMon_Given( wParam, lParam );
	}*/
	switch( wParam )
	{
	case	AddrWSM30KVR01_FB_P3V3:
		UpdateMon_VT( wParam, lParam );
		break;
	case	AddrWSM30KVR01_FB_SCI_V:
		UpdateMon_FB( wParam, lParam );
		break;
	case	AddrWSM30KVR01_Given_SCI_V:
		UpdateMon_Given( wParam, lParam );
		break;
	}
	return 0;
}

void CWSM30KVTestDlg::UpdateMon_VT( WPARAM wParam, LPARAM lParam )
{
	m_monWSM30KV.monVT = *((MonVT_WSM30KV*)lParam);
	CString str;
	str.Format( "%.3f", m_monWSM30KV.monVT.fP3V3[0] );
	GetDlgItem(IDC_EDIT_WSM30KV_MonVT01_P3V3)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monVT.fP5V[0] );
	GetDlgItem(IDC_EDIT_WSM30KV_MonVT01_P5V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monVT.fP15V[0] );
	GetDlgItem(IDC_EDIT_WSM30KV_MonVT01_P15V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monVT.fN15V[0] );
	GetDlgItem(IDC_EDIT_WSM30KV_MonVT01_N15V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monVT.fP24V[0] );
	GetDlgItem(IDC_EDIT_WSM30KV_MonVT01_P24V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monVT.fTemp[0] );
	GetDlgItem(IDC_EDIT_WSM30KV_MonVT01_Temp)->SetWindowText(str);

	str.Format( "%.3f", m_monWSM30KV.monVT.fP3V3[1] );
	GetDlgItem(IDC_EDIT_WSM30KV_MonVT25_P3V3)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monVT.fP5V[1] );
	GetDlgItem(IDC_EDIT_WSM30KV_MonVT25_P5V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monVT.fP15V[1] );
	GetDlgItem(IDC_EDIT_WSM30KV_MonVT25_P15V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monVT.fN15V[1] );
	GetDlgItem(IDC_EDIT_WSM30KV_MonVT25_N15V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monVT.fP24V[1] );
	GetDlgItem(IDC_EDIT_WSM30KV_MonVT25_P24V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monVT.fTemp[1] );
	GetDlgItem(IDC_EDIT_WSM30KV_MonVT25_Temp)->SetWindowText(str);
}

void CWSM30KVTestDlg::UpdateMon_FB( WPARAM wParam , LPARAM lParam )
{
	m_monWSM30KV.monFB = *((MonFeedback_WSM30KV*)lParam);
	CString str;
	str.Format( "%.3f", m_monWSM30KV.monFB.fSCIv );
	GetDlgItem(IDC_EDIT_WSM30KV_MonFB_SCI_V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monFB.fSCIi );
	GetDlgItem(IDC_EDIT_WSM30KV_MonFB_SCI_I)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monFB.fCOLv );
	GetDlgItem(IDC_EDIT_WSM30KV_MonFB_COL_V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monFB.fCOLi );
	GetDlgItem(IDC_EDIT_WSM30KV_MonFB_COL_I)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monFB.fACLv );
	GetDlgItem(IDC_EDIT_WSM30KV_MonFB_ACL_V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monFB.fACLi );
	GetDlgItem(IDC_EDIT_WSM30KV_MonFB_ACL_I)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monFB.fPMTv );
	GetDlgItem(IDC_EDIT_WSM30KV_MonFB_PMT_V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monFB.fPMTi );
	GetDlgItem(IDC_EDIT_WSM30KV_MonFB_PMT_I)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monFB.fFILv );
	GetDlgItem(IDC_EDIT_WSM30KV_MonFB_FIL_V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monFB.fFILi );
	GetDlgItem(IDC_EDIT_WSM30KV_MonFB_FIL_I)->SetWindowText(str);
}

void CWSM30KVTestDlg::UpdateMon_Given( WPARAM wParam, LPARAM lParam )
{
	m_monWSM30KV.monOP = *((MonDACOutput_WSM30KV*)lParam);
	CString str;
	str.Format( "%.3f", m_monWSM30KV.monOP.fSCIv );
	GetDlgItem(IDC_EDIT_WSM30KV_MonOP_SCI_V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monOP.fCOLv );
	GetDlgItem(IDC_EDIT_WSM30KV_MonOP_COL_V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monOP.fACLv );
	GetDlgItem(IDC_EDIT_WSM30KV_MonOP_ACL_V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monOP.fACLi );
	GetDlgItem(IDC_EDIT_WSM30KV_MonOP_ACL_I)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monOP.fPMTv );
	GetDlgItem(IDC_EDIT_WSM30KV_MonOP_PMT_V)->SetWindowText(str);
	str.Format( "%.3f", m_monWSM30KV.monOP.fFILi );
	GetDlgItem(IDC_EDIT_WSM30KV_MonOP_FIL_I)->SetWindowText(str);
}

LRESULT CWSM30KVTestDlg::OnWSM30KVUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	switch( wParam )
	{
	case	1:
		m_cfgWSM30KV.bReady = (BOOL)lParam;
		if( m_cfgWSM30KV.bReady )
		{
// 			m_checkACL.EnableWindow(FALSE);
// 			m_checkEMS.EnableWindow(FALSE);
// 			m_checkFIL.EnableWindow(FALSE);
			m_checkV1.EnableWindow(TRUE);
		}
		break;
	case	AddrWSM30KVR01_INTStatus:
	case	AddrWSM30KVR01_OutputStatus1:
	case	AddrWSM30KVR01_ProtectStatusB:
	case	AddrWSM30KVR01_ProtectStatusI:
	case	AddrWSM30KVR01_ProtectStatusV:
		UpdateStatus_01( wParam, lParam );
		break;
	case	byteWSM30KV_Offset +AddrWSM30KVR25_INTStatus:
	case	byteWSM30KV_Offset +AddrWSM30KVR25_OutputStatus1:
	case	byteWSM30KV_Offset +AddrWSM30KVR25_ProtectStatusB:
	case	byteWSM30KV_Offset +AddrWSM30KVR25_ProtectStatusI:
	case	byteWSM30KV_Offset +AddrWSM30KVR25_ProtectStatusV:
	case	byteWSM30KV_Offset +AddrWSM30KVR25_ProtectStatusArc:
		UpdateStatus_25( wParam, lParam );
		break;
	case	AddrWSM30KVR01_InquireStatus1:
	case	AddrWSM30KVR01_InquireStatus2:
	case	AddrWSM30KVR01_InquireStatus3:
		UpdateInquire_01( wParam, lParam );
		break;
	case	byteWSM30KV_Offset +AddrWSM30KVR25_InquireStatus1:
	case	byteWSM30KV_Offset +AddrWSM30KVR25_InquireStatus2:
	case	byteWSM30KV_Offset +AddrWSM30KVR25_InquireStatus3:
		UpdateInquire_25( wParam, lParam );
		break;
	}
	return 0;
}

void CWSM30KVTestDlg::UpdateStatus_01( WPARAM wParam, LPARAM lParam )
{
	CString str;
	switch( wParam )
	{
	case	AddrWSM30KVR01_INTStatus:
		m_statusWSM30KV.statusINT01 = *((StatusWSM30KV_INTStatus*)lParam);
		str.Format( "%04X", m_statusWSM30KV.statusINT01.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_INT_01)->SetWindowText(str);
		if( m_statusWSM30KV.statusINT01.status.SCI )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_INT_Bit0)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_INT_Bit0)->SetWindowText("0");
		if( m_statusWSM30KV.statusINT01.status.ACL_FIL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_INT_Bit1)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_INT_Bit1)->SetWindowText("0");
		if( m_statusWSM30KV.statusINT01.status.COL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_INT_Bit2)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_INT_Bit2)->SetWindowText("0");
		if( m_statusWSM30KV.statusINT01.status.PMT )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_INT_Bit3)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_INT_Bit3)->SetWindowText("0");
		break;
	case	AddrWSM30KVR01_OutputStatus1:
		m_statusWSM30KV.statusO01 = *((StatusWSM30KV_Output1Status*)lParam);
		str.Format( "%04X", m_statusWSM30KV.statusO01.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_Output_01)->SetWindowText(str);
		if( m_statusWSM30KV.statusO01.status.SCI )
		{
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Output_Bit0)->SetWindowText("1");
			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_SCI)))->SetCheck(1);
		}
		else
		{
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Output_Bit0)->SetWindowText("0");
			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_SCI)))->SetCheck(0);
		}
		if( m_statusWSM30KV.statusO01.status.ACL_FIL )
		{
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Output_Bit1)->SetWindowText("1");
			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_ACL)))->SetCheck(1);
			m_scrollACLv.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Output_Bit1)->SetWindowText("0");
			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_ACL)))->SetCheck(0);
			m_scrollACLv.EnableWindow(FALSE);
		}
		if( m_statusWSM30KV.statusO01.status.EMS )
		{
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Output_Bit2)->SetWindowText("1");
			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_EMS)))->SetCheck(1);
			m_scrollACLi.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Output_Bit2)->SetWindowText("0");
			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_EMS)))->SetCheck(0);
			m_scrollACLi.EnableWindow(FALSE);
		}
		if( m_statusWSM30KV.statusO01.status.COL )
		{
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Output_Bit4)->SetWindowText("1");
			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_COL)))->SetCheck(1);
		}
		else
		{
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Output_Bit4)->SetWindowText("0");
			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_COL)))->SetCheck(0);
		}
		if( m_statusWSM30KV.statusO01.status.PMT )
		{
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Output_Bit5)->SetWindowText("1");
			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_PMT)))->SetCheck(1);
			m_scrollPMTv.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Output_Bit5)->SetWindowText("0");
			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_PMT)))->SetCheck(0);
			m_scrollPMTv.EnableWindow(FALSE);
		}
		break;
	case	AddrWSM30KVR01_ProtectStatusB:
		m_statusWSM30KV.statusB01 = *((StatusWSM30KV_ProtectStatusB*)lParam);
		str.Format( "%04X", m_statusWSM30KV.statusB01.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_PB_01)->SetWindowText(str);
		if( m_statusWSM30KV.statusB01.status.P3V3 )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB01_Bit0)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB01_Bit0)->SetWindowText("0");
		if( m_statusWSM30KV.statusB01.status.P5V )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB01_Bit1)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB01_Bit1)->SetWindowText("0");
		if( m_statusWSM30KV.statusB01.status.P15V )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB01_Bit2)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB01_Bit2)->SetWindowText("0");
		if( m_statusWSM30KV.statusB01.status.N15V )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB01_Bit3)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB01_Bit3)->SetWindowText("0");
		if( m_statusWSM30KV.statusB01.status.P24V )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB01_Bit4)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB01_Bit4)->SetWindowText("0");
		if( m_statusWSM30KV.statusB01.status.Temp )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB01_Bit5)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB01_Bit5)->SetWindowText("0");
		break;
	case	AddrWSM30KVR01_ProtectStatusV:
		m_statusWSM30KV.statusV01 = *((StatusWSM30KV_ProtectStatusV*)lParam);
		str.Format( "%04X", m_statusWSM30KV.statusV01.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_PV_01)->SetWindowText(str);
		if( m_statusWSM30KV.statusV01.status.SCI )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PV_Bit0)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PV_Bit0)->SetWindowText("0");
		if( m_statusWSM30KV.statusV01.status.ACL_FIL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PV_Bit1)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PV_Bit1)->SetWindowText("0");
		if( m_statusWSM30KV.statusV01.status.COL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PV_Bit4)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PV_Bit4)->SetWindowText("0");
		if( m_statusWSM30KV.statusV01.status.PMT )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PV_Bit5)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PV_Bit5)->SetWindowText("0");
		break;
	case	AddrWSM30KVR01_ProtectStatusI:
		m_statusWSM30KV.statusI01 = *((StatusWSM30KV_ProtectStatusI*)lParam);
		str.Format( "%04X", m_statusWSM30KV.statusI01.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_PI_01)->SetWindowText(str);
		if( m_statusWSM30KV.statusI01.status.SCI )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PI_Bit0)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PI_Bit0)->SetWindowText("0");
		if( m_statusWSM30KV.statusI01.status.ACL_FIL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PI_Bit1)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PI_Bit1)->SetWindowText("0");
		if( m_statusWSM30KV.statusI01.status.COL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PI_Bit4)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PI_Bit4)->SetWindowText("0");
		if( m_statusWSM30KV.statusI01.status.PMT )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PI_Bit5)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PI_Bit5)->SetWindowText("0");
		break;
	}
}

void CWSM30KVTestDlg::UpdateStatus_25( WPARAM wParam, LPARAM lParam )
{
	wParam -= byteWSM30KV_Offset;
	CString str;
	switch( wParam )
	{
	case	AddrWSM30KVR25_INTStatus:
		m_statusWSM30KV.statusINT25 = *((StatusWSM30KV_INTStatus*)lParam);
		str.Format( "%04X", m_statusWSM30KV.statusINT25.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_INT_25)->SetWindowText(str);
	case	AddrWSM30KVR25_OutputStatus1:
		m_statusWSM30KV.statusO25 = *((StatusWSM30KV_Output1Status*)lParam);
		str.Format( "%04X", m_statusWSM30KV.statusO25.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_Output_25)->SetWindowText(str);
		if( m_statusWSM30KV.statusO25.status.ACL_FIL )
		{
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Output_Bit3)->SetWindowText("1");
			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_FIL)))->SetCheck(1);
			m_scrollFILi.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Output_Bit3)->SetWindowText("0");
			((CButton*)(GetDlgItem(IDC_CHECK_WSM30KV_FIL)))->SetCheck(0);
			m_scrollFILi.EnableWindow(FALSE);
		}
		break;
	case	AddrWSM30KVR25_ProtectStatusB:
		m_statusWSM30KV.statusB25 = *((StatusWSM30KV_ProtectStatusB*)lParam);
		str.Format( "%04X", m_statusWSM30KV.statusB25.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_PB_25)->SetWindowText(str);
		if( m_statusWSM30KV.statusB25.status.P3V3 )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB25_Bit0)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB25_Bit0)->SetWindowText("0");
		if( m_statusWSM30KV.statusB25.status.P5V )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB25_Bit1)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB25_Bit1)->SetWindowText("0");
		if( m_statusWSM30KV.statusB25.status.P15V )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB25_Bit2)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB25_Bit2)->SetWindowText("0");
		if( m_statusWSM30KV.statusB25.status.N15V )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB25_Bit3)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB25_Bit3)->SetWindowText("0");
		if( m_statusWSM30KV.statusB25.status.P24V )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB25_Bit4)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB25_Bit4)->SetWindowText("0");
		if( m_statusWSM30KV.statusB25.status.Temp )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB25_Bit5)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PB25_Bit5)->SetWindowText("0");
		break;
	case	AddrWSM30KVR25_ProtectStatusV:
		m_statusWSM30KV.statusV25 = *((StatusWSM30KV_ProtectStatusV*)lParam);
		str.Format( "%04X", m_statusWSM30KV.statusV25.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_PV_25)->SetWindowText(str);
		if( m_statusWSM30KV.statusV25.status.ACL_FIL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PV_Bit3)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PV_Bit3)->SetWindowText("0");
		break;
	case	AddrWSM30KVR25_ProtectStatusI:
		m_statusWSM30KV.statusI25 = *((StatusWSM30KV_ProtectStatusI*)lParam);
		str.Format( "%04X", m_statusWSM30KV.statusI25.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_PI_25)->SetWindowText(str);
		if( m_statusWSM30KV.statusI25.status.ACL_FIL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PI_Bit3)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PI_Bit3)->SetWindowText("0");
		break;
	case	AddrWSM30KVR25_ProtectStatusArc:
		m_statusWSM30KV.statusA25 = *((StatusWSM30KV_ProtectStatusArc*)lParam);
		str.Format( "%04X", m_statusWSM30KV.statusA25.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_PA_25)->SetWindowText(str);
		if( m_statusWSM30KV.statusA25.status.ACL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PA_Bit1)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_PA_Bit1)->SetWindowText("0");
		break;
	}
}

void CWSM30KVTestDlg::UpdateInquire_01( WPARAM wParam, LPARAM lParam )
{
	CString str;
	switch( wParam )
	{
	case	AddrWSM30KVR01_InquireStatus1:
		m_statusWSM30KV.status101 = *((StatusWSM30KV_InquireStatus1*)lParam);
		str.Format( "%04X", m_statusWSM30KV.status101.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_01)->SetWindowText(str);
		if( m_statusWSM30KV.status101.status.SCI )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit0)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit0)->SetWindowText("0");
		if( m_statusWSM30KV.status101.status.ACL_FIL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit1)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit1)->SetWindowText("0");
		if( m_statusWSM30KV.status101.status.EMS )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit2)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit2)->SetWindowText("0");
		if( m_statusWSM30KV.status101.status.COL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit4)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit4)->SetWindowText("0");
		if( m_statusWSM30KV.status101.status.PMT )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit5)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit5)->SetWindowText("0");
		break;
	case	AddrWSM30KVR01_InquireStatus2:
		m_statusWSM30KV.status201 = *((StatusWSM30KV_InquireStatus2*)lParam);
		str.Format( "%04X", m_statusWSM30KV.status201.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_01)->SetWindowText(str);
		if( m_statusWSM30KV.status201.status.SCI )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit0)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit0)->SetWindowText("0");
		if( m_statusWSM30KV.status201.status.ACL_FIL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit1)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit1)->SetWindowText("0");
		if( m_statusWSM30KV.status201.status.EMS )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit2)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit2)->SetWindowText("0");
		if( m_statusWSM30KV.status201.status.COL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit4)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit4)->SetWindowText("0");
		if( m_statusWSM30KV.status201.status.PMT )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit5)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit5)->SetWindowText("0");
		break;
	case	AddrWSM30KVR01_InquireStatus3:
		m_statusWSM30KV.status301 = *((StatusWSM30KV_InquireStatus3*)lParam);
		str.Format( "%04X", m_statusWSM30KV.status301.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire3_01)->SetWindowText(str);
		if( m_statusWSM30KV.status301.status.Reset )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire3_Bit1)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire3_Bit1)->SetWindowText("0");
		break;
	}
}

void CWSM30KVTestDlg::UpdateInquire_25( WPARAM wParam, LPARAM lParam )
{
	wParam -= byteWSM30KV_Offset;
	CString str;
	switch( wParam )
	{
	case	AddrWSM30KVR25_InquireStatus1:
		m_statusWSM30KV.status125 = *((StatusWSM30KV_InquireStatus1*)lParam);
		str.Format( "%04X", m_statusWSM30KV.status125.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_25)->SetWindowText(str);
		if( m_statusWSM30KV.status125.status.ACL_FIL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit3)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire1_Bit3)->SetWindowText("0");
		break;
	case	AddrWSM30KVR25_InquireStatus2:
		m_statusWSM30KV.status225 = *((StatusWSM30KV_InquireStatus2*)lParam);
		str.Format( "%04X", m_statusWSM30KV.status225.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_25)->SetWindowText(str);
		if( m_statusWSM30KV.status225.status.ACL_FIL )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit3)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire2_Bit3)->SetWindowText("0");
		break;
	case	AddrWSM30KVR25_InquireStatus3:
		m_statusWSM30KV.status325 = *((StatusWSM30KV_InquireStatus3*)lParam);
		str.Format( "%04X", m_statusWSM30KV.status325.wStatus );
		GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire3_25)->SetWindowText(str);
		if( m_statusWSM30KV.status325.status.Reset )
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire3_Bit0)->SetWindowText("1");
		else
			GetDlgItem(IDC_EDIT_WSM30KV_Status_Inquire3_Bit0)->SetWindowText("0");
		break;
	}
}

void CWSM30KVTestDlg::OnBUTTONWSM30KVClearFault() 
{
	// TODO: Add your control notification handler code here
	m_WSM30KV.SetParam( 37, 1 );
}
