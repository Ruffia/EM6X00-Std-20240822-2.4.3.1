// CommTestClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CommTestClient.h"
#include "CommTestClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma comment(lib,"..\\..\\Bin\\KYKYClientWrapper.lib")

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
// CCommTestClientDlg dialog

CCommTestClientDlg::CCommTestClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommTestClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommTestClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CCommTestClientDlg::~CCommTestClientDlg()
{
}

void CCommTestClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommTestClientDlg)
	DDX_Control(pDX, IDC_IPADDR, m_IPAddr);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCommTestClientDlg, CDialog)
	//{{AFX_MSG_MAP(CCommTestClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Connect, OnBUTTONConnect)
	ON_BN_CLICKED(IDC_BUTTON_Disconnect, OnBUTTONDisconnect)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Speed1, OnRADIOScanCtrlSpeed1)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Speed2, OnRADIOScanCtrlSpeed2)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Speed3, OnRADIOScanCtrlSpeed3)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Speed4, OnRADIOScanCtrlSpeed4)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Reso1, OnRADIOScanCtrlReso1)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Reso2, OnRADIOScanCtrlReso2)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Reso3, OnRADIOScanCtrlReso3)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Reso4, OnRADIOScanCtrlReso4)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Type1, OnRADIOScanCtrlType1)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Type2, OnRADIOScanCtrlType2)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Type3, OnRADIOScanCtrlType3)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Type4, OnRADIOScanCtrlType4)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Type5, OnRADIOScanCtrlType5)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Type6, OnRADIOScanCtrlType6)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Type7, OnRADIOScanCtrlType7)
	ON_BN_CLICKED(IDC_RADIO_Query_Version, OnRADIOQueryVersion)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RADIO_Query_Mag, OnRADIOQueryMag)
	ON_BN_CLICKED(IDC_RADIO_Query_RotAngle, OnRADIOQueryRotAngle)
	ON_BN_CLICKED(IDC_RADIO_Query_WD, OnRADIOQueryWD)
	ON_BN_CLICKED(IDC_RADIO_Query_HV, OnRADIOQueryHV)
	ON_BN_CLICKED(IDC_RADIO_Query_EMI, OnRADIOQueryEMI)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_KYKYRM_Connect_Status, OnGetConnectStatus)
	ON_MESSAGE(WM_USER_KYKYRM_Recv, OnReceiveData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommTestClientDlg message handlers

BOOL CCommTestClientDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here
	m_IPAddr.SetAddress(192,168,1,58);
	UpdateData(FALSE);

//	SEMClient.KYKYSEM_Init(this->m_hWnd);
	m_Client.KYKYSEM_Init(this->m_hWnd);

	m_bConnected	= FALSE;
	m_bCmdConnected	= FALSE;
	SetSendStatus(FALSE);
	SetConnectStatus(TRUE);

	CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_EDIT_Log) );
	peditLog->SetLimitText( 4294967294 );
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCommTestClientDlg::SetSendStatus(BOOL bEnable)
{
	int i;
	for ( i=IDC_RADIO_ScanCtrl_Speed1; i<=IDC_RADIO_ScanCtrl_Type7; i++ )
		GetDlgItem(i)->EnableWindow(bEnable);
	for ( i=IDC_RADIO_Query_Version; i<=IDC_RADIO_Query_EMI; i++ )
		GetDlgItem(i)->EnableWindow(bEnable);
}

void CCommTestClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCommTestClientDlg::OnPaint() 
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
HCURSOR CCommTestClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCommTestClientDlg::OnBUTTONConnect() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	BYTE nf1,nf2,nf3,nf4;
	m_IPAddr.GetAddress(nf1,nf2,nf3,nf4);
//	SEMClient.KYKYSEM_Connect(nf1,nf2,nf3,nf4);
	m_Client.KYKYSEM_Connect(nf1,nf2,nf3,nf4);
}

void CCommTestClientDlg::OnBUTTONDisconnect() 
{
	// TODO: Add your control notification handler code here
	Disconnect(TRUE);
}

void CCommTestClientDlg::Disconnect(BOOL bActive)   //主动断开
{
	// TODO: Add your control notification handler code here
	if (m_bConnected) 
	{
//		SEMClient.KYKYSEM_Disconnect(TRUE);
		m_Client.KYKYSEM_Disconnect(TRUE);
		m_bConnected = FALSE;
		SetSendStatus(FALSE);
		GetDlgItem(IDC_BUTTON_Connect)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_Disconnect)->EnableWindow(FALSE);
	}
}

void CCommTestClientDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	Disconnect(TRUE);
// 	free(m_pClient);
// 	m_pClient = NULL;

	CDialog::OnClose();
}

void CCommTestClientDlg::SetConnectStatus(BOOL bEnable)
{
	GetDlgItem(IDC_BUTTON_Connect)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_Disconnect)->EnableWindow(!bEnable);
}

LRESULT CCommTestClientDlg::OnGetConnectStatus(WPARAM wParam, LPARAM lParam)
{
	// lparam = 0; 表示为命令线程连接成功
	// lparam = 1; 表示发送图像线程连接成功
	if (lParam == 0)
	{
		switch (BYTE(wParam))
		{
		case	KYKYRM_CMD_Sys_Connect_Success:
				m_bCmdConnected = TRUE;
				SetSendStatus(TRUE);
				break;

		case	KYKYRM_CMD_Sys_Disconnect:
//				SEMClient.KYKYSEM_Disconnect(FALSE);
				m_Client.KYKYSEM_Disconnect(FALSE);
				m_bCmdConnected = FALSE;
				break;
		}
	}
	else if (lParam == 1)
	{
		switch (BYTE(wParam))
		{
		case	KYKYRM_CMD_Sys_Connect_Success:
//				bImageConnected = TRUE;
				break;

/*		case	KYKYRM_CMD_Sys_Disconnect:
				m_precvImgThd->StopRecvThread();
				Sleep(1);
				delete m_precvImgThd;
				m_precvImgThd = NULL;
				bImageConnected = FALSE;
				break;
*/		}
	}
	
	if (m_bCmdConnected /*&& bImageConnected*/)
	{
		m_bConnected = TRUE;
		GetDlgItem(IDC_BUTTON_Connect)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_Disconnect)->EnableWindow(TRUE);
	}
	else if ( !m_bCmdConnected /*&& !bImageConnected*/)
	{
		GetDlgItem(IDC_BUTTON_Connect)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_Disconnect)->EnableWindow(FALSE);		
	}
	return 0;
}

LRESULT CCommTestClientDlg::OnReceiveData(WPARAM wParam,LPARAM lParam)
{
	CString str;
	BYTE brecvCom = (BYTE)wParam;
	switch(brecvCom)
	{
	case	KYKYRM_CMD_Query_Version:
		{
			m_wVersionNumber = WORD(lParam);
			str.Format("Version Number = %04x", m_wVersionNumber);
//			GetDlgItem(IDC_RECVDATA)->SetWindowText((LPCSTR)strd);
			Message((LPCSTR)str);
		}
		break;

	case	KYKYRM_CMD_ScanCtrl_Reso:
		{
			MSGIMAGESIZE imgSize;
			imgSize = *PMSGIMAGESIZE(lParam); 
// 			imageReso = imgSize.szReso;
// 			m_ImageCard.SetResolution(imageReso);
// 			TRACE("imageReso = (%d, %d)\r\n",imageReso.cx,imageReso.cy);
		}
		break;

	case	KYKYRM_CMD_Query_Magnification:
		{
			int nMag = *(int *)lParam;
			if( nMag > 100000 )
				str.Format("Mag = %.2f KX", nMag /100000.0);
			else if( nMag > 10000 )
				str.Format("Mag = %d X", (int)(nMag /100.0 +0.5));
			else
				str.Format("Mag = %.1f X", nMag /100.0);
			Message((LPCSTR)str);
		}
		break;

	case	KYKYRM_CMD_Query_ScanRotAngle:
		{
			int nRotAngle = *(int *)lParam;
			str.Format("Rotate Angle = %d", nRotAngle);
			Message((LPCSTR)str);
		}
		break;

	case	KYKYRM_CMD_Query_WorkDistance:
		{
			int nWD = *(int *)lParam;
			str.Format("WD = %.1f", nWD/100.0);
			Message((LPCSTR)str);
		}
		break;

	case	KYKYRM_CMD_Query_HighVoltage:
		{
			int nHV = *(int *)lParam;
			str.Format("HV = %.1f", nHV /1000.0);
			Message((LPCSTR)str);
		}
		break;

	case	KYKYRM_CMD_Query_EMI:
		{
			int nEMI = *(int *)lParam;
			str.Format("EMI = %.2f", nEMI /100.0);
			Message((LPCSTR)str);
		}
		break;
/*
	case	KYKYRM_CMD_SyncImage:
		{
			mesgData = *PMSGIMAGEDATA(lParam);
			ReceiveData_SimulScan();	
		}
		break;
*/	}
	return 0;
}

void CCommTestClientDlg::Message(LPCTSTR lpszMessage)
{
	CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_EDIT_Log) );
	if( peditLog == NULL )
		return;

	CString csMessage = CString(lpszMessage) + _T("\n");
	int len = peditLog->GetWindowTextLength();
	peditLog->SetSel(len,len);
	peditLog->ReplaceSel( (LPCTSTR)csMessage );
}

void CCommTestClientDlg::OnRADIOScanCtrlSpeed1() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_FastChangeScanSpeed(1);
}

void CCommTestClientDlg::OnRADIOScanCtrlSpeed2() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_FastChangeScanSpeed(2);
}

void CCommTestClientDlg::OnRADIOScanCtrlSpeed3() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_FastChangeScanSpeed(3);
}

void CCommTestClientDlg::OnRADIOScanCtrlSpeed4() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_FastChangeScanSpeed(4);
}

void CCommTestClientDlg::OnRADIOScanCtrlReso1() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_FastChangeScanResolution(1);
}

void CCommTestClientDlg::OnRADIOScanCtrlReso2() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_FastChangeScanResolution(2);
}

void CCommTestClientDlg::OnRADIOScanCtrlReso3() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_FastChangeScanResolution(3);
}

void CCommTestClientDlg::OnRADIOScanCtrlReso4() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_FastChangeScanResolution(4);
}

void CCommTestClientDlg::OnRADIOScanCtrlType1() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_ChangeScanType(0);
}

void CCommTestClientDlg::OnRADIOScanCtrlType2() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_ChangeScanType(1);
}

void CCommTestClientDlg::OnRADIOScanCtrlType3() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_ChangeScanType(2);
}

void CCommTestClientDlg::OnRADIOScanCtrlType4() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_ChangeScanType(3);
}

void CCommTestClientDlg::OnRADIOScanCtrlType5() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_ChangeScanType(4);
}

void CCommTestClientDlg::OnRADIOScanCtrlType6() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_ChangeScanType(5);
}

void CCommTestClientDlg::OnRADIOScanCtrlType7() 
{
	// TODO: Add your control notification handler code here
//	m_pClient->KYKYSEM_ChangeScanType(6);
}

void CCommTestClientDlg::OnRADIOQueryVersion() 
{
	// TODO: Add your control notification handler code here
//	SEMClient.KYKYSEM_QueryAppVersion();
	m_Client.KYKYSEM_QueryAppVersion();
}

void CCommTestClientDlg::OnRADIOQueryMag() 
{
	// TODO: Add your control notification handler code here
//	SEMClient.KYKYSEM_QueryMagnification();
	m_Client.KYKYSEM_QueryMagnification();
}

void CCommTestClientDlg::OnRADIOQueryRotAngle() 
{
	// TODO: Add your control notification handler code here
//	SEMClient.KYKYSEM_QueryScanRotAngle();
	m_Client.KYKYSEM_QueryScanRotAngle();
}

void CCommTestClientDlg::OnRADIOQueryWD() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_QueryWorkDistance();
}

void CCommTestClientDlg::OnRADIOQueryHV() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_QueryHighVoltage();
}

void CCommTestClientDlg::OnRADIOQueryEMI() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_QueryEMI();
}
