// CommTestServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CommTestServer.h"
#include "CommTestServerDlg.h"

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
// CCommTestServerDlg dialog

CCommTestServerDlg::CCommTestServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommTestServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommTestServerDlg)
	m_nMag = 500;
	m_nRotAngle = 10;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	///////////////////////////////////
	// 15.09 远程功能
	m_commandThd	= NULL;
	m_pServer		= NULL;
	bListenStarted	= FALSE;
	///////////////////////////////////
}

void CCommTestServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommTestServerDlg)
	DDX_Text(pDX, IDC_EDIT_Mag, m_nMag);
	DDV_MinMaxInt(pDX, m_nMag, 1, 10000);
	DDX_Text(pDX, IDC_EDIT_RotAngle, m_nRotAngle);
	DDV_MinMaxInt(pDX, m_nRotAngle, 0, 360);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCommTestServerDlg, CDialog)
	//{{AFX_MSG_MAP(CCommTestServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_Listen_Start, OnBUTTONListenStart)
	ON_BN_CLICKED(IDC_BUTTON_Listen_Stop, OnBUTTONListenStop)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Mag, OnDeltaposSPINMag)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RotAngle, OnDeltaposSPINRotAngle)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_KYKYComm_Recv, OnKYKYComm_ReplyAsk)
	ON_MESSAGE(WM_USER_KYKYComm_Thread_Send_Connect, OnKYKYComm_ConnectThread)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommTestServerDlg message handlers

BOOL CCommTestServerDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCommTestServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCommTestServerDlg::OnPaint() 
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
HCURSOR CCommTestServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCommTestServerDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CCommTestServerDlg::OnDeltaposSPINMag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CCommTestServerDlg::OnDeltaposSPINRotAngle(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;
}



void CCommTestServerDlg::OnBUTTONListenStart() 
{
	// TODO: Add your control notification handler code here
	m_pServer = new CTCPServer(this->m_hWnd);
	m_pServer->SetPort(KYKYComm_ListenPort1);
	
	if (m_pServer->StartListen())
	{
//		if (m_pSendImgData->StartListen())
		{
			bListenStarted = TRUE;
		}
	}
}

void CCommTestServerDlg::OnBUTTONListenStop() 
{
	// TODO: Add your control notification handler code here
	KYKYComm_EndConnectThread();
	if( m_pServer != NULL )
	{
		delete m_pServer;
		m_pServer = NULL;
	}
	bListenStarted = FALSE;
}

LRESULT CCommTestServerDlg::OnKYKYComm_ConnectThread(WPARAM wParam, LPARAM lParam)
{
	if (m_commandThd == NULL)
	{
		m_commandThd	= (CWinThread*)lParam;	// 命令线程先建立
	}
// 	else
// 	{
// 		m_sendImageThd = (CWinThread*)lParam;	// 图像数据线程后建立		
// 	}	
	return 0;	
}

LRESULT CCommTestServerDlg::OnKYKYComm_ReplyAsk(WPARAM wParam,LPARAM lParam)
{
	if( !m_commandThd )
		return 1;

	BYTE brecvCommand = BYTE(wParam);
	switch (brecvCommand)
	{
	case	KYKYComm_CMD_Sys_Disconnect:
		KYKYComm_EndConnectThread();
		break;

	case	KYKYComm_CMD_Sys_GetVersion:
//		DWORD versionNumber;
//		versionNumber = MAKEWORD(1,1);   //(low,high) (sofware,hardware)
		WORD versionNumber;
		versionNumber = 0x1001;	// theApp.GetVersionInfo();
		m_commandThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, wParam, versionNumber);
		break;

/*	case	KYKYComm_CMD_ScanCtrl_Sync:
		OnScanTypeChanged(ID_SCAN_TYPE_CAMERA_SIMULATE);
		TRACE("simaulscan\r\n");
		break;

	case	KYKYComm_CMD_ScanCtrl_Normal:
		OnScanTypeChanged(ID_SCAN_TYPE_NORMAL);
		break;

	case	KYKYComm_CMD_ScanCtrl_Freeze:
		OnScanTypeChanged(ID_SCAN_TYPE_FREEZE);
		break;

	case	KYKYComm_CMD_ScanCtrl_Speed1:
	case	KYKYComm_CMD_ScanCtrl_Speed2:
	case	KYKYComm_CMD_ScanCtrl_Speed3:
	case	KYKYComm_CMD_ScanCtrl_Speed4:
		{
			int nIndex = brecvCommand - KYKYComm_CMD_ScanCtrl_Speed1;
			int nCommand = ID_SCAN_SPEED_1;
			nCommand += nIndex;
			OnScanSpeedChanged(nCommand);
			TRACE("SPEED %d\r\n",nIndex+1);
		}		
		break;

	case	KYKYComm_CMD_ScanCtrl_Reso1:
	case	KYKYComm_CMD_ScanCtrl_Reso2:
	case	KYKYComm_CMD_ScanCtrl_Reso3:
	case	KYKYComm_CMD_ScanCtrl_Reso4:
		{
			int nIndex = brecvCommand - KYKYComm_CMD_ScanCtrl_Reso1;
			int nCommand = ID_SCAN_RESOLUTION_5;
			nCommand += nIndex;		
			OnScanResolutionChanged(nCommand);
		}
		break;
*/
	case	KYKYComm_CMD_Get_Magnification:
			KYKYComm_SendMag();
			break;
		
	case	KYKYComm_CMD_Get_ScanRotAngle:
			KYKYComm_SendSRotAng();
			break;
	}

/*	if ( !m_sendImageThd )
		return 1;

	switch (brecvCommand )
	{
	case	KYKYComm_CMD_Image_Sync:
			bSendImgData = TRUE;
			KYKYComm_SimulImage();
			break;
	case	KYKYComm_CMD_Image_Stop:
			bBatchSendEnd = TRUE;
			bSendImgData = FALSE;
			break;
	}
*/
	return 0;
}

void CCommTestServerDlg::KYKYComm_EndConnectThread()
{
	bBatchSendEnd = TRUE;
	if (m_commandThd!=NULL)
	{
		m_commandThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Sys_Disconnect, 0);
		m_commandThd->PostThreadMessage(WM_QUIT, 0, 0);
		m_commandThd = NULL;	
	}
// 	if (m_sendImageThd != NULL)
// 	{
// 		m_sendImageThd->PostThreadMessage(WM_QUIT, 0, 0);
// 		m_sendImageThd = NULL;
// 	}
}

void CCommTestServerDlg::KYKYComm_SendReso()
{
	CSize sz(1024,768);
	LPARAM LParam;
	LParam = (LPARAM)&sz;
	m_commandThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, (WPARAM)KYKYComm_CMD_ScanCtrl_Reso, LParam);
	TRACE("imageReso = (%d, %d)\r\n",sz.cx,sz.cy);
}

void CCommTestServerDlg::KYKYComm_SendMag()
{
	//	CString strMag = theApp.m_SemCtrl.m_SemCore.m_Magnifier.FormatText();
	nMagnification = m_nMag;
	LPARAM lParam;
	lParam = (LPARAM)&nMagnification;
	m_commandThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_Magnification, lParam);
}

void CCommTestServerDlg::KYKYComm_SendSRotAng()
{
	//	CString strSRotAngle = theApp.m_SemCtrl.m_SemCore.m_ScanRotate.FormatText();
	nScanRotAng = m_nRotAngle;
	LPARAM lParam;
	lParam = (LPARAM)&nScanRotAng;
	m_commandThd->PostThreadMessage(WM_USER_KYKYComm_Thread_Send, KYKYComm_CMD_Get_ScanRotAngle, lParam);
}
