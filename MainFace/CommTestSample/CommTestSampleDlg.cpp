// CommTestSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CommTestSample.h"
#include "CommTestSampleDlg.h"

#include <math.h>
#include <mmsystem.h>		// system timer support

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FLAGBIT( flag, flag_bit )	( ( (flag) & (flag_bit) ) == (flag_bit) )

KYKYCommMsg_ImgFrameData	*g_pcommMsgImgFrameData;
KYKYCommMsg_ImgData			*g_pcommmsgImgData;
int g_nCurIndex;

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
// CCommTestSampleDlg dialog

CCommTestSampleDlg::CCommTestSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommTestSampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommTestSampleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CCommTestSampleDlg::~CCommTestSampleDlg()
{
}

void CCommTestSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommTestSampleDlg)
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_Image);
	DDX_Control(pDX, IDC_IPADDR, m_IPAddr);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCommTestSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CCommTestSampleDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_Connect, OnBUTTONConnect)
	ON_BN_CLICKED(IDC_BUTTON_Disconnect, OnBUTTONDisconnect)
	ON_BN_CLICKED(IDC_RADIO_Type_SEM, OnRADIOQuerySEMType)
	ON_BN_CLICKED(IDC_RADIO_Type_HV, OnRADIOQueryHVType)
	ON_BN_CLICKED(IDC_RADIO_Query_Mag, OnRADIOQueryMag)
	ON_BN_CLICKED(IDC_RADIO_Query_MagRange, OnRADIOQueryMagRange)
	ON_BN_CLICKED(IDC_RADIO_Query_RotAngleInit, OnRADIOQueryRotAngleInit)
	ON_BN_CLICKED(IDC_RADIO_Query_RotAngle, OnRADIOQueryRotAngle)
	ON_BN_CLICKED(IDC_RADIO_Query_WD, OnRADIOQueryWD)
	ON_BN_CLICKED(IDC_RADIO_Query_HV, OnRADIOQueryHV)
	ON_BN_CLICKED(IDC_RADIO_Query_Fila, OnRADIOQueryFila)
	ON_BN_CLICKED(IDC_RADIO_Query_BeamCurrent, OnRADIOQueryBeamCurrent)
	ON_BN_CLICKED(IDC_RADIO_Stage_Status, OnRADIOStageStatus)
	ON_BN_CLICKED(IDC_RADIO_Stage_Grating, OnRADIOStageGrating)
	ON_BN_CLICKED(IDC_RADIO_Stage_Position, OnRADIOStagePosition)
	ON_BN_CLICKED(IDC_RADIO_Stage_Range, OnRADIOStageRange)
	ON_BN_CLICKED(IDC_RADIO_Query_Stigmator, OnRADIOQueryStigmator)
	ON_BN_CLICKED(IDC_BUTTON_Stage_Position_Set, OnBUTTONStagePositionSet)
	ON_BN_CLICKED(IDC_BUTTON_SetMag, OnBUTTONSetMag)
	ON_BN_CLICKED(IDC_BUTTON_SetScnRotAngle, OnBUTTONSetScnRotAngle)
	ON_BN_CLICKED(IDC_BUTTON_SetStig, OnBUTTONSetStig)
	ON_BN_CLICKED(IDC_BUTTON_SetHV, OnBUTTONSetHV)
	ON_BN_CLICKED(IDC_BUTTON_SetFila, OnBUTTONSetFila)
	ON_BN_CLICKED(IDC_BUTTON_SetBeamCurrent, OnBUTTONSetBeamCurrent)
	ON_BN_CLICKED(IDC_BUTTON_Stage_Stop, OnBUTTONStageStop)
	ON_BN_CLICKED(IDC_BUTTON_AutoBC, OnBUTTONAutoBC)
	ON_BN_CLICKED(IDC_RADIO_Query_Brt, OnRADIOQueryBrt)
	ON_BN_CLICKED(IDC_RADIO_Query_Cst, OnRADIOQueryCst)
	ON_BN_CLICKED(IDC_BUTTON_SetBrt, OnBUTTONSetBrt)
	ON_BN_CLICKED(IDC_BUTTON_SetCst, OnBUTTONSetCst)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_Query_GunStatus, OnRADIOQueryGunStatus)
	ON_BN_CLICKED(IDC_RADIO_Query_VacStatus, OnRADIOQueryVacStatus)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Speed1, OnRADIOScanCtrlSpeed1)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Speed2, OnRADIOScanCtrlSpeed2)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Speed3, OnRADIOScanCtrlSpeed3)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Speed4, OnRADIOScanCtrlSpeed4)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Reso1, OnRADIOScanCtrlReso1)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Reso2, OnRADIOScanCtrlReso2)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Type1, OnRADIOScanCtrlType1)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Type2, OnRADIOScanCtrlType2)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Type3, OnRADIOScanCtrlType3)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Type4, OnRADIOScanCtrlType4)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Type5, OnRADIOScanCtrlType5)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Type6, OnRADIOScanCtrlType6)
	ON_BN_CLICKED(IDC_RADIO_ScanCtrl_Type7, OnRADIOScanCtrlType7)
	ON_BN_CLICKED(IDC_BUTTON_SetWD, OnBUTTONSetWD)
	ON_BN_CLICKED(IDC_BUTTON_Stage_GoHome, OnBUTTONStageGoHome)
	ON_BN_CLICKED(IDC_RADIO_Query_EMI, OnRADIOQueryEMI)
	ON_BN_CLICKED(IDC_RADIO_Query_All, OnRADIOQueryAll)
	ON_BN_CLICKED(IDC_CHECK_Polling, OnCHECKPolling)
	ON_BN_CLICKED(IDC_BUTTON_Log_Clear, OnLogClear)
	ON_BN_CLICKED(IDC_BUTTON_Clear, OnBUTTONClear)
	ON_BN_CLICKED(IDC_CHECK_ScanSpots, OnCHECKScanSpots)
	ON_BN_CLICKED(IDC_BUTTON_ShutdownBeam, OnBUTTONShutdownBeam)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RADIO_Query_Speed, OnRADIOQuerySpeed)
	ON_BN_CLICKED(IDC_RADIO_Query_Resolution, OnRADIOQueryResolution)
	ON_BN_CLICKED(IDC_BUTTON_Image_Clear, OnBUTTONImageClear)
	ON_BN_CLICKED(IDC_RADIO_Query_FrameData, OnRADIOQueryFrameData)
	ON_BN_CLICKED(IDC_RADIO_Query_Image, OnRADIOQueryImage)
	ON_BN_CLICKED(IDC_BUTTON_Save_Params, OnBUTTONSaveParams)
	ON_BN_CLICKED(IDC_BUTTON_Load_Params, OnBUTTONLoadParams)
	ON_BN_CLICKED(IDC_RADIO_Query_FOV, OnRADIOQueryFOV)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_KYKYComm_Connect_Status, OnGetConnectStatus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommTestSampleDlg message handlers

BOOL CCommTestSampleDlg::OnInitDialog()
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
	m_IPAddr.SetAddress(127,0,0,1);		// (192,168,1,34);
	m_Client.KYKYSEM_Init(this->m_hWnd);
	m_bConnected = FALSE;
	m_StageCfg.nStatus = 0;
	SetSendStatus(FALSE);
	SetConnectStatus(TRUE);

	GetDlgItem(IDC_EDIT_Stage_Position_X2)->SetWindowText("-1.28");
	GetDlgItem(IDC_EDIT_Stage_Position_Y2)->SetWindowText("-0.76");
	GetDlgItem(IDC_EDIT_Stage_Position_Z2)->SetWindowText("0.54");
	GetDlgItem(IDC_EDIT_Stage_Position_R2)->SetWindowText("12.5");
	GetDlgItem(IDC_EDIT_Stage_Position_T2)->SetWindowText("1.5");
	UpdateData(FALSE);

	m_nTimer = 0;

	//设置Static控件大小和位置
	CRect rc;
	m_Image.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_Image.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Width()*3/4, 0);

	m_imageReso = CSize(1024, 768);
	ResetImageWindow();

	CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_EDIT_Log) );
	peditLog->SetLimitText( 4294967294 );
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCommTestSampleDlg::ResetImageWindow()
{
	// 图像
	m_ImageCard.SetResolution(m_imageReso);
//	m_ImageCard.SetSplit(FALSE);
//	m_ImageCard.SetLiveMode(FALSE);
	m_ImageCard.Reset(m_Image.GetWindow(NULL));
// 	m_ImageCard.ShowLabel(FALSE);
// 	m_ImageCard.ShowTime(FALSE);
//	m_ImageCard.SetPhotoPreview( FALSE );
	m_ImageCard.SetZoom(0);

	CString str;
	str.Format( "Reset Image [%d, %d]", m_imageReso.cx, m_imageReso.cy );
	Message(str);
}

void CCommTestSampleDlg::SetSendStatus(BOOL bEnable)
{
	CRect rc, rcRef;
	GetWindowRect(&rc);
	GetDlgItem(IDC_STATIC_Rect)->GetWindowRect(&rcRef);
	rcRef.bottom = rc.bottom;
	CWnd *pWnd = GetWindow(GW_CHILD);
	while(pWnd != NULL)
	{
		pWnd->GetWindowRect(&rc);
		if( PtInRect( rcRef, rc.CenterPoint() ) )
			pWnd->EnableWindow(bEnable);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	};
}

void CCommTestSampleDlg::SetConnectStatus(BOOL bEnable)
{
	GetDlgItem(IDC_BUTTON_Connect)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_Disconnect)->EnableWindow(!bEnable);
}

void CCommTestSampleDlg::Message(LPCTSTR lpszMessage)
{
	CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_EDIT_Log) );
	if( peditLog == NULL )
		return;

	DWORD	thisTickCount = timeGetTime();
//	CString csMessage = CString(lpszMessage) + _T("\n");
	CString csMessage;
	csMessage.Format( "[%010Ld]: %s\n", thisTickCount, lpszMessage );
	int len = peditLog->GetWindowTextLength();
	peditLog->SetSel(len,len);
	peditLog->ReplaceSel( (LPCTSTR)csMessage );
}

void CCommTestSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCommTestSampleDlg::OnPaint() 
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
HCURSOR CCommTestSampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCommTestSampleDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	EndDialog(IDCANCEL);
	CDialog::OnClose();
}

void CCommTestSampleDlg::OnDestroy() 
{
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	m_ImageCard.Close();
	Disconnect();
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CCommTestSampleDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CCommTestSampleDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CCommTestSampleDlg::OnBUTTONConnect() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	BYTE nf1,nf2,nf3,nf4;
	m_IPAddr.GetAddress(nf1,nf2,nf3,nf4);
	m_Client.KYKYSEM_Connect(nf1,nf2,nf3,nf4);
}

void CCommTestSampleDlg::OnBUTTONDisconnect() 
{
	// TODO: Add your control notification handler code here
	for( int i=1; i<5; i++ )
		KillTimer(i);
	Disconnect();
}

void CCommTestSampleDlg::Disconnect()
{
	// TODO: Add your control notification handler code here
	if (m_bConnected) 
	{
		m_Client.KYKYSEM_Disconnect(TRUE);
		m_bConnected = FALSE;
		SetSendStatus(FALSE);
		GetDlgItem(IDC_BUTTON_Connect)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_Disconnect)->EnableWindow(FALSE);
	}
}

LRESULT CCommTestSampleDlg::OnGetConnectStatus(WPARAM wParam, LPARAM lParam)
{
	switch (BYTE(wParam))
	{
	case	KYKYComm_CMD_Sys_Connect_Success:
		{
			int nConnect = (int)lParam;
			if( nConnect == 1 )
				m_bConnected = TRUE;
			else if( nConnect == 3 )
				m_bImgConnected = TRUE;
			if( !m_bConnected )
				break;
			CString str;
			str.Format( "Connect success: %d", nConnect );
			Message((LPCSTR)str);
			// Brightness
			double dBrt;
			m_Client.KYKYSEM_GetBrightness( &dBrt );
			str.Format( "%.2f", dBrt );
			GetDlgItem(IDC_EDIT_Brightness)->SetWindowText(str);
			str.Format("Brightness = %.2f", dBrt);
			Message((LPCSTR)str);
			// Contrast
			double dCst;
			m_Client.KYKYSEM_GetContrast( &dCst );
			str.Format( "%.2f", dCst );
			GetDlgItem(IDC_EDIT_Contrast)->SetWindowText(str);
			str.Format("Contrast = %.2f", dCst);
			Message((LPCSTR)str);
			// Mag
			double dMag;
			m_Client.KYKYSEM_GetMagn( &dMag );
			str.Format( "%.1f", dMag );
			GetDlgItem(IDC_EDIT_Mag)->SetWindowText(str);
			if( dMag > 1000 )
				str.Format("Mag = %.1f KX", dMag /1000.0);
			else if( dMag > 100 )
				str.Format("Mag = %d X", (int)(dMag +0.5));
			else
				str.Format("Mag = %.1f X", dMag);
			Message((LPCSTR)str);
			// Rotate initial angle
			double dAngle;
			m_Client.KYKYSEM_GetScnRotAngleInit( &dAngle );
			str.Format( "%.0f", dAngle );
			GetDlgItem(IDC_EDIT_ScnRotAngleInit)->SetWindowText(str);
			str.Format("Rotate Angle Init = %.0f", dAngle);
			Message((LPCSTR)str);
			// Rotate angle
			m_Client.KYKYSEM_GetScnRotAngle( &dAngle );
			str.Format( "%.0f", dAngle );
			GetDlgItem(IDC_EDIT_ScnRotAngle)->SetWindowText(str);
			str.Format("Rotate Angle = %.0f", dAngle);
			Message((LPCSTR)str);
			// Work Distance
			double dWD;
			m_Client.KYKYSEM_GetWD( &dWD );
			str.Format( "%.1f", dWD );
			GetDlgItem(IDC_EDIT_WD)->SetWindowText(str);
			str.Format("WD = %.1f mm", dWD);
			Message((LPCSTR)str);
			// Stigmator
			double dX, dY;
			m_Client.KYKYSEM_GetStig( &dX, &dY );
			str.Format( "%.0f", dX );
			GetDlgItem(IDC_EDIT_Stig_X)->SetWindowText(str);
			str.Format( "%.0f", dY );
			GetDlgItem(IDC_EDIT_Stig_Y)->SetWindowText(str);
			str.Format("Stigmator = [%.0f, %.0f]", dX, dY);
			Message((LPCSTR)str);
			// Stage status
			m_Client.KYKYSEM_StageGetStatus( &(m_StageCfg.nStatus) );
			UpdateStageStatus();
/*
			// 图像连接成功
			if( m_bImgConnected )
			{
				InterFaceClear();
				g_pcommmsgImgData = (KYKYCommMsg_ImgData *)m_Client.KYKYSEM_ImgRequest();
				int nImgSize = g_pcommmsgImgData->rcDest.Height() *g_pcommmsgImgData->rcDest.Width();
				int nLen = m_ImageCard.WriteCommImageData((LPVOID)g_pcommmsgImgData->bImageBuff,nImgSize,g_pcommmsgImgData->rcDest);
				str.Format( "msgData.rcDest: [%d,%d,%d,%d], %x",
											g_pcommmsgImgData->rcDest.left, g_pcommmsgImgData->rcDest.top,
											g_pcommmsgImgData->rcDest.right, g_pcommmsgImgData->rcDest.bottom,
											g_pcommmsgImgData->bImageBuff );
				Message(str);
				// 开启图像显示线程
// 				m_bEndImgProcess = FALSE;
// 				DWORD dwID;
// 				HANDLE hImaHandle = CreateThread(NULL, 0, ThreadImageProcess, this, 0, &dwID);
// 				CloseHandle(hImaHandle);
			}
*/		}
		break;

	case	KYKYComm_CMD_Sys_Disconnect:
		{
			m_Client.KYKYSEM_Disconnect(FALSE);
			m_bConnected = FALSE;
		}
		break;
	}

	if (m_bConnected )
	{
		SetSendStatus(TRUE);
		SetConnectStatus(FALSE);
		CString str;
		/////////////////////////////////////////
		// Vacuum status
		m_Client.KYKYSEM_GetVacuumStatus( &m_nStatusVac );
		str.Format( "%d", m_nStatusVac );
		GetDlgItem( IDC_EDIT_VacStatus )->SetWindowText( str );
		// Gun status
		m_Client.KYKYSEM_GetGunStatus( &m_nStatusGun );
		str.Format( "%d", m_nStatusGun );
		GetDlgItem( IDC_EDIT_GunStatus )->SetWindowText( str );

		if( m_nStatusVac > 0 )
			Message( "Vacuum is OK" );
		else
			Message( "Vacuum is NOT OK" );
		EnablePowerSupply();
		/////////////////////////////////////////
		/////////////////////////////////////////
		// Stage
		if( (m_StageCfg.nStatus & 0x01) == 0x01 )	// has stage
		{
			GetDlgItem( IDC_RADIO_Stage_Grating )->EnableWindow( TRUE );
			GetDlgItem( IDC_RADIO_Stage_Range )->EnableWindow( TRUE );
			// Stage Gratings
			int nX, nY;
			m_Client.KYKYSEM_StageGetGratings( &nX, &nY );
			str.Format( "%03d, %03d", nX, nY );
			GetDlgItem(IDC_EDIT_Stage_Grating)->SetWindowText(str);
			str.Format("Stage Gratings [nm] = [%d, %d]", nX, nY);
			Message((LPCSTR)str);
			float fRange[4];
			// Stage Ranges
			m_Client.KYKYSEM_StageGetRange2( fRange );
			str.Format( "[%.2f, %.2f],[%.2f, %.2f]", fRange[0], fRange[1], fRange[2], fRange[3] );
			GetDlgItem(IDC_EDIT_Stage_Range)->SetWindowText(str);
			str.Format("Stage Ranges [mm] = [%.2f, %.2f],[%.2f, %.2f]", fRange[0], fRange[1], fRange[2], fRange[3] );
			Message((LPCSTR)str);
		}
		else
		{
			GetDlgItem( IDC_RADIO_Stage_Grating )->EnableWindow( FALSE );
			GetDlgItem( IDC_RADIO_Stage_Range )->EnableWindow( FALSE );
		}
		if( (m_StageCfg.nStatus & 0x02) == 0x02 )	// stage is ready
		{
			GetDlgItem( IDC_RADIO_Stage_Position )->EnableWindow( TRUE );
			GetDlgItem( IDC_EDIT_Stage_Position_X )->EnableWindow( TRUE );
			GetDlgItem( IDC_EDIT_Stage_Position_Y )->EnableWindow( TRUE );
			GetDlgItem( IDC_BUTTON_Stage_Position_Set )->EnableWindow( TRUE );
			GetDlgItem( IDC_BUTTON_Stage_Stop )->EnableWindow( TRUE );
		}
		else
		{
			GetDlgItem( IDC_RADIO_Stage_Position )->EnableWindow( FALSE );
			GetDlgItem( IDC_EDIT_Stage_Position_X )->EnableWindow( FALSE );
			GetDlgItem( IDC_EDIT_Stage_Position_Y )->EnableWindow( FALSE );
			GetDlgItem( IDC_BUTTON_Stage_Position_Set )->EnableWindow( FALSE );
			GetDlgItem( IDC_BUTTON_Stage_Stop )->EnableWindow( FALSE );
		}
		/////////////////////////////////////////
	}
	else
	{
		SetSendStatus(FALSE);
		SetConnectStatus(TRUE);
	}

	return 0;
}

DWORD WINAPI CCommTestSampleDlg::ThreadImageProcess(LPVOID lParam)
{
	CCommTestSampleDlg* pDlg = (CCommTestSampleDlg*)lParam;
	CKYKYSEMComm* pClient = &(pDlg->m_Client);
    g_nCurIndex = 0; 

	while( !pDlg->m_bEndImgProcess)
	{
		int nLastIndex = 0;
		pClient->KYKYSEM_ImgGetLastRecvIndex(&nLastIndex);
		if (g_nCurIndex == nLastIndex)
			continue;

		TRACE("nLastIndex = %d\r\n",nLastIndex);
		pDlg->RecvImgDataShow();	
		g_pcommmsgImgData ++;
		g_nCurIndex ++;
		if (g_nCurIndex == KYKYComm_MALLOC_SIZE)
		{
			g_nCurIndex = 0;
			g_pcommmsgImgData -= KYKYComm_MALLOC_SIZE;
		}	
	}

	return 0;	
}

void CCommTestSampleDlg::RecvImgDataShow()
{
	int nImgSize = g_pcommmsgImgData->rcDest.Height() *g_pcommmsgImgData->rcDest.Width();
	CString str;
	str.Format("msgData.rcDest : %d,%d\r\n",g_pcommmsgImgData->rcDest.top,g_pcommmsgImgData->rcDest.bottom);
	Message(str);
	if (g_pcommmsgImgData->rcDest.top < 0)  return;
/*
	if (m_preImageReso != m_imageReso)
	{
		InterFaceClear();
	}
*/
	int nLen = m_ImageCard.WriteCommImageData((LPVOID)g_pcommmsgImgData->bImageBuff,nImgSize,g_pcommmsgImgData->rcDest);
	m_preImageReso = m_imageReso;
}

void CCommTestSampleDlg::InterFaceClear()
{
	int nSize = m_imageReso.cx * m_imageReso.cy; 
	PBYTE pzeroMem;
	CRect rcFrame = CRect(0,0,m_imageReso.cx,m_imageReso.cy);
	pzeroMem = (BYTE *)malloc(nSize);
	ZeroMemory(pzeroMem,nSize);
	m_ImageCard.WriteCommImageData((LPVOID)pzeroMem,nSize,rcFrame);
	free(pzeroMem);
	pzeroMem = NULL;	
}

void CCommTestSampleDlg::OnRADIOQueryFrameData() 
{
	// TODO: Add your control notification handler code here
	if( m_bImgConnected )
	{
//		InterFaceClear();
// 		m_Client.KYKYSEM_ImgRequestFrame();
// 		Sleep(5);
		g_pcommMsgImgFrameData = (KYKYCommMsg_ImgFrameData *)m_Client.KYKYSEM_ImgRequestFrame();
		int nImgSize = g_pcommMsgImgFrameData->rcDest.Height() *g_pcommMsgImgFrameData->rcDest.Width();
		int nLen = m_ImageCard.WriteCommImageData((LPVOID)g_pcommMsgImgFrameData->bImageBuff, nImgSize, g_pcommMsgImgFrameData->rcDest);
		CString str;
		str.Format( "msgData.rcDest: [%d,%d,%d,%d], nSize = %ld",
									g_pcommMsgImgFrameData->rcDest.left, g_pcommMsgImgFrameData->rcDest.top,
									g_pcommMsgImgFrameData->rcDest.right, g_pcommMsgImgFrameData->rcDest.bottom, nImgSize );
		Message(str);
		str = "";
		CString strTemp;
		for( int i=0; i<nImgSize; i++ )
		{
			strTemp.Format( "%02X,", g_pcommMsgImgFrameData->bImageBuff[i] );
			str += strTemp;
			if( i % 16 == 0 )
			{
				str += "\r\n";
				Message(str);
				str = "";
			}
		}
	}
}

void CCommTestSampleDlg::OnRADIOQueryImage() 
{
	if( !m_bImgConnected )
		return;
	int nCheck = ((CButton*)(GetDlgItem(IDC_RADIO_Query_Image)))->GetCheck();
	if( nCheck )
	{
		// 开启图像显示线程
		m_bEndImgProcess = FALSE;
		DWORD dwID;
		HANDLE hImaHandle = CreateThread(NULL, 0, ThreadImageProcess, this, 0, &dwID);
		CloseHandle(hImaHandle);
	}
	else
	{
		m_Client.KYKYSEM_ImgStop();
		m_bEndImgProcess = TRUE;
	}
}

void CCommTestSampleDlg::OnBUTTONImageClear() 
{
	// TODO: Add your control notification handler code here
	InterFaceClear();
}

void CCommTestSampleDlg::OnRADIOQueryAll() 
{
	// TODO: Add your control notification handler code here
	if( !m_bConnected )
		return;
	InitReadParams_Sys();
	InitReadParams();
}

void CCommTestSampleDlg::InitReadParams()
{
	InitReadParams_SEM();
	InitReadParams_VacGun();
	InitReadParams_Stage();
}

void CCommTestSampleDlg::InitReadParams_Sys()
{
	OnRADIOQuerySEMType();
	OnRADIOQueryHVType();
	OnRADIOQueryRotAngleInit();
	OnRADIOStageStatus();
	OnRADIOStageRange();
}

void CCommTestSampleDlg::InitReadParams_SEM()
{
	OnRADIOQueryBrt();
	OnRADIOQueryCst();
	OnRADIOQueryMagRange();
	OnRADIOQueryMag();
	OnRADIOQueryRotAngle();
	OnRADIOQueryWD();
	OnRADIOQueryStigmator();
}

void CCommTestSampleDlg::InitReadParams_VacGun()
{
	OnRADIOQueryVacStatus();
	OnRADIOQueryGunStatus();
}

void CCommTestSampleDlg::InitReadParams_Stage()
{
	OnRADIOStageStatus();
	OnRADIOStagePosition();
}

void CCommTestSampleDlg::OnRADIOQuerySEMType() 
{
	// TODO: Add your control notification handler code here
	int nType;
	m_Client.KYKYSEM_GetSEMType( &nType );
	CString str;
	str.Format( "%d", nType );
	GetDlgItem(IDC_EDIT_SEMType)->SetWindowText(str);
	str.Format("SEM Type: %d", nType);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOQueryHVType() 
{
	// TODO: Add your control notification handler code here
	int nType;
	m_Client.KYKYSEM_GetHVPowerType( &nType );
	CString str;
	str.Format( "%d", nType );
	GetDlgItem(IDC_EDIT_HVType)->SetWindowText(str);
	if( nType > 0 )
	{
		GetDlgItem(IDC_EDIT_HVType)->SetWindowText("SPM");
		GetDlgItem(IDC_RADIO_Query_BeamCurrent)->SetWindowText("Beam Current [uA]");
		str = "HV Type: 1, SPM";
	}
	else
	{
		GetDlgItem(IDC_EDIT_HVType)->SetWindowText("CPS");
		GetDlgItem(IDC_RADIO_Query_BeamCurrent)->SetWindowText("Bias [V]");
		str = "HV Type: 0, CPS";
	}
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOQueryBrt() 
{
	// TODO: Add your control notification handler code here
	double dBrt;
	m_Client.KYKYSEM_GetBrightness( &dBrt );
	CString str;
	str.Format( "%.2f", dBrt );
	GetDlgItem(IDC_EDIT_Brightness)->SetWindowText(str);
	str.Format("Brightness = %.2f", dBrt);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnBUTTONSetBrt() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_Brightness2)->GetWindowText( str );
	double brt = atof(str);
	m_Client.KYKYSEM_SetBrightness( brt );
}

void CCommTestSampleDlg::OnRADIOQueryCst() 
{
	// TODO: Add your control notification handler code here
	double dCst;
	m_Client.KYKYSEM_GetContrast( &dCst );
	CString str;
	str.Format( "%.2f", dCst );
	GetDlgItem(IDC_EDIT_Contrast)->SetWindowText(str);
	str.Format("Contrast = %.2f", dCst);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnBUTTONSetCst() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_Contrast2)->GetWindowText( str );
	double cst = atof(str);
	m_Client.KYKYSEM_SetContrast( cst );
}

void CCommTestSampleDlg::OnRADIOQueryFOV() 
{
	// TODO: Add your control notification handler code here
	double dX, dY;
	int nUnit;
	m_Client.KYKYSEM_GetFOV( &dX, &dY, &nUnit );
	CString str, strUnit;
	switch( nUnit )
	{
	case	0:
		strUnit = "mm";
		break;
	case	1:
		strUnit = "cm";
		break;
	case	2:
		strUnit = "um";
		break;
	case	3:
		strUnit	= "nm";
		break;
	}
	str.Format( "%.1f %s | %.1f %s", dX, strUnit, dY, strUnit );
	GetDlgItem(IDC_EDIT_FOV)->SetWindowText(str);
	str.Format( "Field of View: %.1f %s | %.1f %s", dX, strUnit, dY, strUnit );
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOQueryMagRange() 
{
	// TODO: Add your control notification handler code here
	double dMin, dMax;
	m_Client.KYKYSEM_GetMagnRange( &dMin, &dMax );
	CString str;
	str.Format( "%.2f, %.2f", dMin, dMax );
	GetDlgItem(IDC_EDIT_MagRange)->SetWindowText(str);
	str.Format("Mag Range = %.2f, %.2f", dMin, dMax );
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOQueryMag() 
{
	// TODO: Add your control notification handler code here
	double dMag;
	m_Client.KYKYSEM_GetMagn( &dMag );
	CString str;
	str.Format( "%.1f", dMag );
	GetDlgItem(IDC_EDIT_Mag)->SetWindowText(str);
	if( dMag > 1000 )
		str.Format("Mag = %.1f KX", dMag /1000.0);
	else if( dMag > 100 )
		str.Format("Mag = %d X", (int)(dMag +0.5));
	else
		str.Format("Mag = %.1f X", dMag);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnBUTTONSetMag() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_Mag2)->GetWindowText( str );
	double mgn = atof(str);
	m_Client.KYKYSEM_SetMagn( mgn );
}

void CCommTestSampleDlg::OnRADIOQueryRotAngleInit() 
{
	// TODO: Add your control notification handler code here
	double dAngle;
	m_Client.KYKYSEM_GetScnRotAngleInit( &dAngle );
	CString str;
	str.Format( "%.0f", dAngle );
	GetDlgItem(IDC_EDIT_ScnRotAngleInit)->SetWindowText(str);
	str.Format("Rotate Angle Init = %.0f", dAngle);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOQueryRotAngle() 
{
	// TODO: Add your control notification handler code here
	double dAngle;
	m_Client.KYKYSEM_GetScnRotAngle( &dAngle );
	CString str;
	str.Format( "%.0f", dAngle );
	GetDlgItem(IDC_EDIT_ScnRotAngle)->SetWindowText(str);
	str.Format("Scan Rotate Angle = %.0f", dAngle);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnBUTTONSetScnRotAngle() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_ScnRotAngle2)->GetWindowText( str );
	double angle = atof(str);
	m_Client.KYKYSEM_SetScnRotAngle( angle );
}

void CCommTestSampleDlg::OnRADIOQueryWD() 
{
	// TODO: Add your control notification handler code here
	double dWD;
	m_Client.KYKYSEM_GetWD( &dWD );
	CString str;
	str.Format( "%.1f", dWD );
	GetDlgItem(IDC_EDIT_WD)->SetWindowText(str);
	str.Format("WD = %.6f mm", dWD);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnBUTTONSetWD() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_WD2)->GetWindowText( str );
	double wd = atof(str);
	m_Client.KYKYSEM_SetWD( wd );
}

void CCommTestSampleDlg::OnRADIOQueryStigmator() 
{
	// TODO: Add your control notification handler code here
	double dX, dY;
	m_Client.KYKYSEM_GetStig( &dX, &dY );
	CString str;
	str.Format( "%.0f", dX );
	GetDlgItem(IDC_EDIT_Stig_X)->SetWindowText(str);
	str.Format( "%.0f", dY );
	GetDlgItem(IDC_EDIT_Stig_Y)->SetWindowText(str);
	str.Format("Stigmator = [%.0f, %.0f]", dX, dY);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnBUTTONSetStig() 
{
	// TODO: Add your control notification handler code here
	double fX, fY;
	CString str;
	GetDlgItem(IDC_EDIT_Stig_X2)->GetWindowText(str);
	fX = atof(str);
	GetDlgItem(IDC_EDIT_Stig_Y2)->GetWindowText(str);
	fY = atof(str);
	m_Client.KYKYSEM_SetStig(fX, fY);
}

void CCommTestSampleDlg::EnablePowerSupply()
{
	if( m_nStatusVac > 0 && m_nStatusGun > 0 )	// HV is on
	{
		GetDlgItem( IDC_BUTTON_SetHV )->EnableWindow( TRUE );
		GetDlgItem( IDC_BUTTON_SetFila )->EnableWindow( TRUE );
		GetDlgItem( IDC_BUTTON_SetBeamCurrent )->EnableWindow( TRUE );
		if( m_nStatusGun > 1 )
			Message( "HV is ON" );
		else
			Message( "Interlock is ON, HV is not on" );
	}
	else
	{
		GetDlgItem( IDC_BUTTON_SetHV )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON_SetFila )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON_SetBeamCurrent )->EnableWindow( FALSE );
		if( m_nStatusGun < 1 )
			Message( "Interlock is OFF" );
	}
}

void CCommTestSampleDlg::OnRADIOQueryGunStatus() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_GetGunStatus( &m_nStatusGun );
	EnablePowerSupply();
	CString str;
	str.Format( "%d", m_nStatusGun );
	GetDlgItem( IDC_EDIT_GunStatus )->SetWindowText( str );
	str.Format("Gun Status = %d", m_nStatusGun);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOQueryVacStatus() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_GetVacuumStatus( &m_nStatusVac );
	EnablePowerSupply();
	CString str;
	str.Format( "%d", m_nStatusVac );
	GetDlgItem( IDC_EDIT_VacStatus )->SetWindowText( str );
	str.Format("VAC Status = %d", m_nStatusVac);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOQueryHV() 
{
	// TODO: Add your control notification handler code here
	double dHV;
	m_Client.KYKYSEM_GetAccVoltage( &dHV );
	CString str;
	str.Format( "%.1f", dHV );
	GetDlgItem(IDC_EDIT_HV)->SetWindowText(str);
	str.Format("HV = %.1f KV", dHV);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnBUTTONSetHV() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_HV2)->GetWindowText(str);
	double dVal = atof(str);
	m_Client.KYKYSEM_SetAccVoltage(dVal);
	str.Format("Set HV to %.1f", dVal);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOQueryFila() 
{
	// TODO: Add your control notification handler code here
	double dFila;
	m_Client.KYKYSEM_GetFilament( &dFila );
	CString str;
	str.Format( "%.2f", dFila );
	GetDlgItem(IDC_EDIT_Fila)->SetWindowText(str);
	str.Format("Filament = %.2f A", dFila);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnBUTTONSetFila() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_Fila2)->GetWindowText(str);
	double dVal = atof(str);
	m_Client.KYKYSEM_SetFilament(dVal);
	str.Format("Set Fila to %.1f", dVal);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOQueryBeamCurrent() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_HVType)->GetWindowText(str);
	if( str == "SPM" )
	{
		double dBeamCurrent;
		m_Client.KYKYSEM_GetBeamCurrent( &dBeamCurrent );
		str.Format( "%.1f", dBeamCurrent );
		GetDlgItem(IDC_EDIT_BeamCurrent)->SetWindowText(str);
		str.Format("Beam Current = %.1f uA", dBeamCurrent);
	}
	else
	{
		double dBias;
		m_Client.KYKYSEM_GetBias( &dBias );
		str.Format( "%.1f", dBias );
		GetDlgItem(IDC_EDIT_BeamCurrent)->SetWindowText(str);
		str.Format("Bias voltage = %.1f V", dBias);
	}
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnBUTTONSetBeamCurrent() 
{
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT_HVType)->GetWindowText(str);
	if( str == "SPM" )
	{
		GetDlgItem(IDC_EDIT_BeamCurrent2)->GetWindowText(str);
		double dVal = atof(str);
		m_Client.KYKYSEM_SetBeamCurrent(dVal);
		str.Format("Set BeamCurrent to %.1f", dVal);
	}
	else
	{
		GetDlgItem(IDC_EDIT_BeamCurrent2)->GetWindowText(str);
		double dVal = atof(str);
		m_Client.KYKYSEM_SetBias(dVal);
		str.Format("Set Bias to %.1f", dVal);
	}
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOQueryEMI() 
{
	// TODO: Add your control notification handler code here
	double dEMI;
	m_Client.KYKYSEM_GetEMI( &dEMI );
	CString str;
	str.Format( "%.3f", dEMI );
	GetDlgItem(IDC_EDIT_EMI)->SetWindowText(str);
	str.Format("EMI = %.3f uA", dEMI);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOStageStatus() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_StageGetStatus( &(m_StageCfg.nStatus) );
	UpdateStageStatus();
	UpdateStageEnables();
	if( m_nTimer != 0 && (m_StageCfg.nStatus & 0x08) == 0x00 )
	{
		KillTimer( 1 );
		m_nTimer = 0;
		OnRADIOStagePosition();
		OnRADIOStagePosition();
	}
}

void CCommTestSampleDlg::UpdateStageStatus()
{
	BYTE bFlag = m_StageCfg.nStatus;
	CString str;
	BYTE	buf[16];
	const BYTE	bRecord = 0xFF;

	if ( FLAGBIT(bRecord,0x01) ) 
		buf[8] = FLAGBIT(bFlag,0x01) ? '1':'0';
	else buf[8] = '*';

	if ( FLAGBIT(bRecord,0x02) )
		buf[7] = FLAGBIT(bFlag,0x02) ? '1':'0';
	else buf[7] = '*';

	if ( FLAGBIT(bRecord,0x04) )
		buf[6] = FLAGBIT(bFlag,0x04) ? '1':'0';
	else buf[6] = '*';

	if ( FLAGBIT(bRecord,0x08) )
		buf[5] = FLAGBIT(bFlag,0x08) ? '1':'0';
	else buf[5] = '*';

	buf[4] = ' ';

	if ( FLAGBIT(bRecord,0x10) )
		buf[3] = FLAGBIT(bFlag,0x10) ? '1':'0';
	else buf[3] = '*';

	if ( FLAGBIT(bRecord,0x20) )
		buf[2] = FLAGBIT(bFlag,0x20) ? '1':'0';
	else buf[2] = '*';

	if ( FLAGBIT(bRecord,0x40) )
		buf[1] = FLAGBIT(bFlag,0x40) ? '1':'0';
	else buf[1] = '*';

	if ( FLAGBIT(bRecord,0x80) )
		buf[0] = FLAGBIT(bFlag,0x80) ? '1':'0';
	else buf[0] = '*';

	buf[9] = '\0';

	str.Format( "%9s", buf );
	GetDlgItem(IDC_EDIT_Stage_Status)->SetWindowText(str);
	str.Format("Stage Status = 0x%02X", m_StageCfg.nStatus);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::UpdateStageEnables()
{
	/////////////////////////////////////////
	/////////////////////////////////////////
	// Stage
	m_StageCfg.nType = m_StageCfg.nStatus & 0x03;
	if( m_StageCfg.nType == 1 )		// has 2-axes stage
	{
		GetDlgItem( IDC_RADIO_Stage_Grating )->EnableWindow( TRUE );
		GetDlgItem( IDC_RADIO_Stage_Range )->EnableWindow( TRUE );
		GetDlgItem( IDC_RADIO_Stage_Position )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_Stage_Position_Z )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_Stage_Position_R )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_Stage_Position_T )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_Stage_Position_Z2 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_Stage_Position_R2 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_EDIT_Stage_Position_T2 )->ShowWindow( SW_HIDE );
		Message( "has 2-axes auto-stage" );
	}
	else if( m_StageCfg.nType > 1 )	// has 4-5 axes stage
	{
		GetDlgItem( IDC_RADIO_Stage_Grating )->EnableWindow( FALSE );
		GetDlgItem( IDC_RADIO_Stage_Range )->EnableWindow( TRUE );
		GetDlgItem( IDC_RADIO_Stage_Position )->EnableWindow( TRUE );
		if( m_StageCfg.nType > 2 )
			Message( "has 5 axes auto-stage" );
		else
			Message( "has 4 axes auto-stage" );
	}
	else	// no stage
	{
		Message( "has no auto-stage" );
	}
	m_StageCfg.bReady = ((m_StageCfg.nStatus & 0x04) == 0x04);
	if( m_StageCfg.bReady )	// stage is ready
	{
		Message( "auto-stage is ready" );
		if( m_StageCfg.nType == 1 )
		{
			GetDlgItem( IDC_RADIO_Stage_Position )->EnableWindow( TRUE );
			GetDlgItem( IDC_EDIT_Stage_Position_X2 )->EnableWindow( TRUE );
			GetDlgItem( IDC_EDIT_Stage_Position_Y2 )->EnableWindow( TRUE );
			GetDlgItem( IDC_BUTTON_Stage_Position_Set )->EnableWindow( TRUE );
		}
		else
		{
			GetDlgItem( IDC_RADIO_Stage_Position )->EnableWindow( TRUE );
			GetDlgItem( IDC_EDIT_Stage_Position_X2 )->EnableWindow( TRUE );
			GetDlgItem( IDC_EDIT_Stage_Position_Y2 )->EnableWindow( TRUE );
			GetDlgItem( IDC_EDIT_Stage_Position_Z2 )->EnableWindow( TRUE );
			GetDlgItem( IDC_EDIT_Stage_Position_R2 )->EnableWindow( TRUE );
			if( m_StageCfg.nType == 2 )
			{
				GetDlgItem( IDC_EDIT_Stage_Position_T2 )->EnableWindow( FALSE );
				GetDlgItem( IDC_RADIO_Stage_Position )->SetWindowText( "Stage Pos 4" );
				GetDlgItem( IDC_RADIO_Stage_Range )->SetWindowText( "Stage Range 4" );
			}
			else
			{
				GetDlgItem( IDC_EDIT_Stage_Position_T2 )->EnableWindow( TRUE );
				GetDlgItem( IDC_RADIO_Stage_Position )->SetWindowText( "Stage Pos 5" );
				GetDlgItem( IDC_RADIO_Stage_Range )->SetWindowText( "Stage Range 5" );
			}
			GetDlgItem( IDC_BUTTON_Stage_Position_Set )->EnableWindow( TRUE );
		}
		GetDlgItem( IDC_BUTTON_Stage_Stop )->EnableWindow( TRUE );
		GetDlgItem( IDC_BUTTON_Stage_GoHome )->EnableWindow( TRUE );
		GetDlgItem( IDC_RADIO_Stage_Range )->EnableWindow( TRUE );
	}
	else
	{
		Message( "auto-stage is NOT ready" );
		GetDlgItem( IDC_RADIO_Stage_Position )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_Stage_Position_X2 )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_Stage_Position_Y2 )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_Stage_Position_Z2 )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_Stage_Position_R2 )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_Stage_Position_T2 )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON_Stage_Position_Set )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON_Stage_Stop )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON_Stage_GoHome )->EnableWindow( FALSE );
		GetDlgItem( IDC_RADIO_Stage_Range )->EnableWindow( FALSE );
	}
	/////////////////////////////////////////
	if( (m_StageCfg.nStatus & 0x08) == 0x08 )	// stage is moving
		Message( "auto-stage is moving" );
	else
		Message( "auto-stage is idle" );
}

void CCommTestSampleDlg::OnRADIOStageGrating() 
{
	// TODO: Add your control notification handler code here
	int nX, nY;
	m_Client.KYKYSEM_StageGetGratings( &nX, &nY );
	CString str;
	str.Format( "%03d, %03d", nX, nY );
	GetDlgItem(IDC_EDIT_Stage_Grating)->SetWindowText(str);
	str.Format("Stage Gratings [nm] = [%d, %d]", nX, nY);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOStageRange() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if( m_StageCfg.nType == 1 )		// has 2-axes stage
	{
		float fRange[4];
		m_Client.KYKYSEM_StageGetRange2( fRange );
		str.Format( "[%.2f, %.2f],[%.2f, %.2f]", fRange[0], fRange[1], fRange[2], fRange[3] );
		GetDlgItem(IDC_EDIT_Stage_Range)->SetWindowText(str);
		str.Format("Stage Ranges [mm] = [%.2f, %.2f],[%.2f, %.2f]", fRange[0], fRange[1], fRange[2], fRange[3] );
	}
	else if( m_StageCfg.nType > 1 )	// has 4-5 axes stage
	{
		float fRange[10];
		m_Client.KYKYSEM_StageGetRange5( fRange );
		str.Format( "[%.2f,%.2f], [%.2f,%.2f], [%.1f,%.1f], [%.1f,%.1f], [%.1f,%.1f]",
					fRange[0], fRange[1], fRange[2], fRange[3], fRange[4],
					fRange[5], fRange[6], fRange[7], fRange[8], fRange[9] );
		GetDlgItem(IDC_EDIT_Stage_Range)->SetWindowText(str);
		str.Format("Stage Ranges [mm] = [%.2f,%.2f], [%.2f,%.2f], [%.1f,%.1f], [%.1f,%.1f], [%.1f,%.1f]",
					fRange[0], fRange[1], fRange[2], fRange[3], fRange[4],
					fRange[5], fRange[6], fRange[7], fRange[8], fRange[9] );
	}
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOStagePosition() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if( m_StageCfg.nType == 1 )		// has 2-axes stage
	{
		float fPos[2];
		m_Client.KYKYSEM_StageGetPos2( fPos );
		str.Format( "%.2f", fPos[0] );
		GetDlgItem(IDC_EDIT_Stage_Position_X)->SetWindowText(str);
		str.Format( "%.2f", fPos[1] );
		GetDlgItem(IDC_EDIT_Stage_Position_Y)->SetWindowText(str);
		str.Format("Stage Position [mm] = [%.2f, %.2f]", fPos[0], fPos[1]);
	}
	else if( m_StageCfg.nType > 1 )	// has 4-5 axes stage
	{
		float fPos[5];
		m_Client.KYKYSEM_StageGetPos5( fPos );
		str.Format( "%.2f", fPos[0] );
		GetDlgItem(IDC_EDIT_Stage_Position_X)->SetWindowText(str);
		str.Format( "%.2f", fPos[1] );
		GetDlgItem(IDC_EDIT_Stage_Position_Y)->SetWindowText(str);
		str.Format( "%.2f", fPos[2] );
		GetDlgItem(IDC_EDIT_Stage_Position_Z)->SetWindowText(str);
		str.Format( "%.1f", fPos[3] );
		GetDlgItem(IDC_EDIT_Stage_Position_R)->SetWindowText(str);
		str.Format( "%.1f", fPos[4] );
		GetDlgItem(IDC_EDIT_Stage_Position_T)->SetWindowText(str);
		str.Format("Stage Position [mm] = [%.2f, %.2f, %.2f, %.1f, %.1f]", fPos[0], fPos[1], fPos[2], fPos[3], fPos[4]);
	}
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnBUTTONStagePositionSet() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if( m_StageCfg.nType == 1 )		// has 2-axes stage
	{
		float fX, fY;
		GetDlgItem(IDC_EDIT_Stage_Position_X2)->GetWindowText( str );
		fX = (float)(atof( str ));
		GetDlgItem(IDC_EDIT_Stage_Position_Y2)->GetWindowText( str );
		fY = (float)(atof( str ));
		m_Client.KYKYSEM_StageSetPos2( fX, fY );
	}
	else if( m_StageCfg.nType > 1 )	// has 4-5 axes stage
	{
		float fX, fY, fZ, fR, fT;
		GetDlgItem(IDC_EDIT_Stage_Position_X2)->GetWindowText( str );
		fX = (float)(atof( str ));
		GetDlgItem(IDC_EDIT_Stage_Position_Y2)->GetWindowText( str );
		fY = (float)(atof( str ));
		GetDlgItem(IDC_EDIT_Stage_Position_Z2)->GetWindowText( str );
		fZ = (float)(atof( str ));
		GetDlgItem(IDC_EDIT_Stage_Position_R2)->GetWindowText( str );
		fR = (float)(atof( str ));
		GetDlgItem(IDC_EDIT_Stage_Position_T2)->GetWindowText( str );
		fT = (float)(atof( str ));
		m_Client.KYKYSEM_StageSetPos5( fX, fY, fZ, fR, fT );
	}
	Message( "Set Postions, start timer...");
	SetTimer( 1, 200, NULL );
}

BOOL CCommTestSampleDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		CString str = "";
		int nID = GetFocus()->GetDlgCtrlID();
		switch( nID )
		{
		case	IDC_EDIT_Stage_Position_X2:
		case	IDC_EDIT_Stage_Position_Y2:
		case	IDC_EDIT_Stage_Position_Z2:
		case	IDC_EDIT_Stage_Position_R2:
		case	IDC_EDIT_Stage_Position_T2:
			StageMove(nID);
			break;
		}
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CCommTestSampleDlg::StageMove( int nID )
{
	CString str;
	GetDlgItem(nID)->GetWindowText(str);
	float fPos = (float)(atof(str));
	m_Client.KYKYSEM_StageSetPos( nID -IDC_EDIT_Stage_Position_X2, fPos );
	SetTimer( 1, 200, NULL );
}

void CCommTestSampleDlg::OnBUTTONStageStop() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_StageStop();
//	KillTimer(1);
}

void CCommTestSampleDlg::OnBUTTONStageGoHome() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_StageGoHome();
	SetTimer( 1, 200, NULL );
}

void CCommTestSampleDlg::OnBUTTONAutoBC() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_FuncAutoCstBrt();
}

void CCommTestSampleDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == 1 )
	{
		int nStatus = m_StageCfg.nStatus;
		m_Client.KYKYSEM_StageGetStatus( &(m_StageCfg.nStatus) );
		UpdateStageStatus();
 		if( ((nStatus & 0x08) == 0x08) && ((m_StageCfg.nStatus & 0x08) == 0x00) )
 			KillTimer( 1 );
	}
	else if( nIDEvent == 2 )
		InitReadParams();
	else if( nIDEvent == 3 )
	{
		CString str;
		str.Format( "Change SpotScan to [%d, %d]", m_pointScanSpots[m_nScanSpotsIndex].x, 
										m_pointScanSpots[m_nScanSpotsIndex].y );
		Message(str);
		m_Client.KYKYSEM_ScanStartSpot( m_pointScanSpots[m_nScanSpotsIndex].x, 
										m_pointScanSpots[m_nScanSpotsIndex].y );
		m_nScanSpotsIndex++;
		if( m_nScanSpotsIndex > 99 )
			m_nScanSpotsIndex = 0;
	}
	else if( nIDEvent == 4 )
	{
		int nIsStopScan = 0;
		m_Client.KYKYSEM_ScanIsStop( &nIsStopScan );
		if( nIsStopScan )
		{
			OnRADIOQueryFrameData();
			KillTimer(4);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CCommTestSampleDlg::OnCHECKPolling() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Polling)))->GetCheck();
	if( nCheck )
		SetTimer(2, 500, NULL);
	else
		KillTimer(2);
}

void CCommTestSampleDlg::OnLogClear() 
{
	// TODO: Add your control notification handler code here
	CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_EDIT_Log) );
	if( peditLog == NULL )
		return;

	CString csMessage = _T("");
	int len = peditLog->GetWindowTextLength();
	peditLog->SetSel(0,len);
	peditLog->ReplaceSel( (LPCTSTR)csMessage );
}

void CCommTestSampleDlg::OnBUTTONClear() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_SEMType)->SetWindowText("");
	GetDlgItem(IDC_EDIT_HVType)->SetWindowText("");
	GetDlgItem(IDC_EDIT_WD)->SetWindowText("");
	GetDlgItem(IDC_EDIT_ScnRotAngleInit)->SetWindowText("");
	GetDlgItem(IDC_EDIT_Stage_Status)->SetWindowText("");
	GetDlgItem(IDC_EDIT_Brightness)->SetWindowText("");
	GetDlgItem(IDC_EDIT_Contrast)->SetWindowText("");
	GetDlgItem(IDC_EDIT_Mag)->SetWindowText("");
	GetDlgItem(IDC_EDIT_MagRange)->SetWindowText("");
	GetDlgItem(IDC_EDIT_ScnRotAngle)->SetWindowText("");
	GetDlgItem(IDC_EDIT_Stig_X)->SetWindowText("");
	GetDlgItem(IDC_EDIT_Stig_Y)->SetWindowText("");
	GetDlgItem(IDC_EDIT_VacStatus)->SetWindowText("");
	GetDlgItem(IDC_EDIT_GunStatus)->SetWindowText("");
	GetDlgItem(IDC_EDIT_HV)->SetWindowText("");
	GetDlgItem(IDC_EDIT_Fila)->SetWindowText("");
	GetDlgItem(IDC_EDIT_BeamCurrent)->SetWindowText("");
	GetDlgItem(IDC_EDIT_EMI)->SetWindowText("");
	GetDlgItem(IDC_EDIT_Stage_Range)->SetWindowText("");
	GetDlgItem(IDC_EDIT_Stage_Position_X)->SetWindowText("");
	GetDlgItem(IDC_EDIT_Stage_Position_Y)->SetWindowText("");
	GetDlgItem(IDC_EDIT_Stage_Position_Z)->SetWindowText("");
	GetDlgItem(IDC_EDIT_Stage_Position_R)->SetWindowText("");
	GetDlgItem(IDC_EDIT_Stage_Position_T)->SetWindowText("");
}




void CCommTestSampleDlg::OnRADIOQuerySpeed() 
{
	// TODO: Add your control notification handler code here
	double dSpeed;
	m_Client.KYKYSEM_ScanGetSpeed( &dSpeed );
	CString str;
	str.Format( "%.1f", dSpeed );
	GetDlgItem(IDC_EDIT_Speed)->SetWindowText(str);
	str.Format("Speed = %.1f us/point", dSpeed);
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOQueryResolution() 
{
	// TODO: Add your control notification handler code here
	int nResoX, nResoY;
	m_Client.KYKYSEM_ScanGetResolution( &nResoX, &nResoY );
	m_imageReso.cx = nResoX;
	m_imageReso.cy = nResoY;
	CString str;
	str.Format( "%d x %d", nResoX, nResoY );
	GetDlgItem(IDC_EDIT_Resolution)->SetWindowText(str);
	str.Format("Resolution = %d x %d", nResoX, nResoY );
	Message((LPCSTR)str);
}

void CCommTestSampleDlg::OnRADIOScanCtrlSpeed1() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_ScanSetSpeed(0);
}

void CCommTestSampleDlg::OnRADIOScanCtrlSpeed2() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_ScanSetSpeed(1);
}

void CCommTestSampleDlg::OnRADIOScanCtrlSpeed3() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_ScanSetSpeed(2);
}

void CCommTestSampleDlg::OnRADIOScanCtrlSpeed4() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_ScanSetSpeed(3);
}

void CCommTestSampleDlg::OnRADIOScanCtrlReso1() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_ScanSetResolution(0);
	ChangeResolution();
}

void CCommTestSampleDlg::OnRADIOScanCtrlReso2() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_ScanSetResolution(1);
	ChangeResolution();
}

void CCommTestSampleDlg::ChangeResolution()
{
	int nResoX, nResoY;
	m_Client.KYKYSEM_ScanGetResolution( &nResoX, &nResoY );
	Sleep(1000);
	m_Client.KYKYSEM_ScanGetResolution( &nResoX, &nResoY );
	Sleep(500);
	OnRADIOQueryResolution();
	ResetImageWindow();
	OnRADIOQueryFrameData();
}

void CCommTestSampleDlg::OnRADIOScanCtrlType1() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_ScanFreeze();
}

void CCommTestSampleDlg::OnRADIOScanCtrlType2() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_ScanStart();
}

void CCommTestSampleDlg::OnRADIOScanCtrlType3() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_ScanStartArea( 200, 200, 600, 500 );
}

void CCommTestSampleDlg::OnRADIOScanCtrlType4() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_ScanStartHLine( 200 );
}

void CCommTestSampleDlg::OnRADIOScanCtrlType5() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_ScanStartVLine( 200 );
}

void CCommTestSampleDlg::OnRADIOScanCtrlType6() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_ScanStartSpot( 300, 400 );
}

void CCommTestSampleDlg::OnRADIOScanCtrlType7() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_ScanStartSync();
	SetTimer( 4, 200, NULL );
}

void CCommTestSampleDlg::OnCHECKScanSpots() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_ScanSpots)))->GetCheck();
	if( nCheck )
	{
		int i, j, nBase = 0;
		for( i=0; i<10; i++ )
		{
			for( j=0; j<10; j++ )
			{
				m_pointScanSpots[i*10 +j].x = nBase +j;
				m_pointScanSpots[i*10 +j].y = nBase*3/4 +j;
			}
			nBase += 96;
		}
		m_nScanSpotsIndex = 0;
		SetTimer( 3, 200, NULL );
	}
	else
		KillTimer(3);
}

void CCommTestSampleDlg::OnBUTTONShutdownBeam() 
{
	// TODO: Add your control notification handler code here
	m_Client.KYKYSEM_FuncShutdownBeam();
}

void CCommTestSampleDlg::OnBUTTONSaveParams() 
{
	// TODO: Add your control notification handler code here
	CString filename;
	CFileDialog dlgFile(FALSE, "val", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "*.val", this );
	if( dlgFile.DoModal() == IDOK )
		filename = dlgFile.GetPathName();
	else
		return;
	FILE *stream;
	stream = fopen( (LPCSTR)filename, "w" );
	if( stream == NULL )
		return;

	/////////////////////////////////////////////
	// Get Current Params
	CString str;
	GetDlgItem(IDC_EDIT_HV)->GetWindowText(str);
	double dHV = atof(str);
	GetDlgItem(IDC_EDIT_BeamCurrent)->GetWindowText(str);
	double dBeam = atof(str);
	GetDlgItem(IDC_EDIT_Fila)->GetWindowText(str);
	double dFil = atof(str);

	GetDlgItem(IDC_EDIT_Brightness)->GetWindowText(str);
	double dBrt = atof(str);
	GetDlgItem(IDC_EDIT_Contrast)->GetWindowText(str);
	double dCst = atof(str);
	GetDlgItem(IDC_EDIT_WD)->GetWindowText(str);
	double dWD = atof(str);
	GetDlgItem(IDC_EDIT_Mag)->GetWindowText(str);
	double dMag = atof(str);
	GetDlgItem(IDC_EDIT_Stig_X)->GetWindowText(str);
	int nStigX = atoi(str);
	GetDlgItem(IDC_EDIT_Stig_Y)->GetWindowText(str);
	int nStigY = atoi(str);

	GetDlgItem(IDC_EDIT_Stage_Position_X)->GetWindowText(str);
	double dPosX = atof(str);
	GetDlgItem(IDC_EDIT_Stage_Position_Y)->GetWindowText(str);
	double dPosY = atof(str);
	GetDlgItem(IDC_EDIT_Stage_Position_Z)->GetWindowText(str);
	double dPosZ = atof(str);
	GetDlgItem(IDC_EDIT_Stage_Position_R)->GetWindowText(str);
	double dPosR = atof(str);
	GetDlgItem(IDC_EDIT_Stage_Position_T)->GetWindowText(str);
	double dPosT = atof(str);
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// Set pointer to beginning of file:
	char szBuf[255];
	fseek( stream, 0L, SEEK_SET );
	sprintf( szBuf, "HV=%.1f\n", dHV );
	fwrite( szBuf, sizeof(char), strlen(szBuf), stream );
	sprintf( szBuf, "Beam=%.1f\n", dBeam );
	fwrite( szBuf, sizeof(char), strlen(szBuf), stream );
	sprintf( szBuf, "Fil=%.2f\n", dFil );
	fwrite( szBuf, sizeof(char), strlen(szBuf), stream );
	sprintf( szBuf, "Brt=%.2f\n", dBrt );
	fwrite( szBuf, sizeof(char), strlen(szBuf), stream );
	sprintf( szBuf, "Cst=%.2f\n", dCst );
	fwrite( szBuf, sizeof(char), strlen(szBuf), stream );
	sprintf( szBuf, "WD=%.1f\n", dWD );
	fwrite( szBuf, sizeof(char), strlen(szBuf), stream );
	sprintf( szBuf, "Mag=%.1f\n", dMag );
	fwrite( szBuf, sizeof(char), strlen(szBuf), stream );
	sprintf( szBuf, "StigX=%d\n", nStigX );
	fwrite( szBuf, sizeof(char), strlen(szBuf), stream );
	sprintf( szBuf, "StigY=%d\n", nStigY );
	fwrite( szBuf, sizeof(char), strlen(szBuf), stream );
	sprintf( szBuf, "PosX=%.3f\n", dPosX );
	fwrite( szBuf, sizeof(char), strlen(szBuf), stream );
	sprintf( szBuf, "PosY=%.3f\n", dPosY );
	fwrite( szBuf, sizeof(char), strlen(szBuf), stream );
	sprintf( szBuf, "PosZ=%.3f\n", dPosZ );
	fwrite( szBuf, sizeof(char), strlen(szBuf), stream );
	sprintf( szBuf, "PosR=%.3f\n", dPosR );
	fwrite( szBuf, sizeof(char), strlen(szBuf), stream );
	sprintf( szBuf, "PosT=%.3f\n", dPosT);
	fwrite( szBuf, sizeof(char), strlen(szBuf), stream );
	fclose( stream );
}

void CCommTestSampleDlg::OnBUTTONLoadParams() 
{
	// TODO: Add your control notification handler code here
	CString filename;
	CFileDialog dlgFile(TRUE, "val", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "*.val", this );
	if( dlgFile.DoModal() != IDOK )
		return;
	filename = dlgFile.GetPathName();
	FILE *stream;
	stream = fopen( (LPCSTR)filename, "r" );
	if( stream == NULL )
		return;

	/////////////////////////////////////////////
	TCHAR	cBuff[14][256];
	/* Set pointer to beginning of file: */
	fseek( stream, 0L, SEEK_SET );
	/* Read data back from file: */
	fscanf( stream, "%s", cBuff[0] );
	fscanf( stream, "%s", cBuff[1] );
	fscanf( stream, "%s", cBuff[2] );
	fscanf( stream, "%s", cBuff[3] );
	fscanf( stream, "%s", cBuff[4] );
	fscanf( stream, "%s", cBuff[5] );
	fscanf( stream, "%s", cBuff[6] );
	fscanf( stream, "%s", cBuff[7] );
	fscanf( stream, "%s", cBuff[8] );
	fscanf( stream, "%s", cBuff[9] );
	fscanf( stream, "%s", cBuff[10] );
	fscanf( stream, "%s", cBuff[11] );
	fscanf( stream, "%s", cBuff[12] );
	fscanf( stream, "%s", cBuff[13] );
	fclose( stream );
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	char szParam[50];
	strcpy( szParam, strchr( cBuff[0], '=' ) +1 );
	double dHV = atof(szParam);
	strcpy( szParam, strchr( cBuff[1], '=' ) +1 );
	double dBeam = atof(szParam);
	strcpy( szParam, strchr( cBuff[2], '=' ) +1 );
	double dFil = atof(szParam);
	strcpy( szParam, strchr( cBuff[3], '=' ) +1 );
	double dBrt = atof(szParam);
	strcpy( szParam, strchr( cBuff[4], '=' ) +1 );
	double dCst = atof(szParam);
	strcpy( szParam, strchr( cBuff[5], '=' ) +1 );
	double dWD = atof(szParam);
	strcpy( szParam, strchr( cBuff[6], '=' ) +1 );
	double dMag = atof(szParam);
	strcpy( szParam, strchr( cBuff[7], '=' ) +1 );
	int nStigX = atoi(szParam);
	strcpy( szParam, strchr( cBuff[8], '=' ) +1 );
	int nStigY = atoi(szParam);

	strcpy( szParam, strchr( cBuff[9], '=' ) +1 );
	double dPosX = atof(szParam);
	strcpy( szParam, strchr( cBuff[10], '=' ) +1 );
	double dPosY = atof(szParam);
	strcpy( szParam, strchr( cBuff[11], '=' ) +1 );
	double dPosZ = atof(szParam);
	strcpy( szParam, strchr( cBuff[12], '=' ) +1 );
	double dPosR = atof(szParam);
	strcpy( szParam, strchr( cBuff[13], '=' ) +1 );
	double dPosT = atof(szParam);
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	m_Client.KYKYSEM_SetBrightness( dBrt );
	m_Client.KYKYSEM_SetContrast( dCst );
	m_Client.KYKYSEM_SetAccVoltage( dHV );
	m_Client.KYKYSEM_SetBeamCurrent( dBeam );
	m_Client.KYKYSEM_SetFilament( dFil );
	m_Client.KYKYSEM_SetStigX( nStigX );
	m_Client.KYKYSEM_SetStigY( nStigY );
	m_Client.KYKYSEM_SetWD( dWD );
	m_Client.KYKYSEM_SetMagn( dMag );

	m_Client.KYKYSEM_StageSetPos5( dPosX, dPosY, dPosZ, dPosR, dPosT );
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	CString str;
	str.Format( "%.1f", dHV );
	GetDlgItem(IDC_EDIT_HV)->SetWindowText(str);
	str.Format( "%.1f", dBeam );
	GetDlgItem(IDC_EDIT_BeamCurrent)->SetWindowText(str);
	str.Format( "%.1f", dFil );
	GetDlgItem(IDC_EDIT_Fila)->SetWindowText(str);
	str.Format( "%.1f", dBrt );
	GetDlgItem(IDC_EDIT_Brightness)->SetWindowText(str);
	str.Format( "%.1f", dCst );
	GetDlgItem(IDC_EDIT_Contrast)->SetWindowText(str);
	str.Format( "%.1f", dWD );
	GetDlgItem(IDC_EDIT_WD)->SetWindowText(str);
	str.Format( "%.1f", dMag );
	GetDlgItem(IDC_EDIT_Mag)->SetWindowText(str);
	str.Format( "%d", nStigX );
	GetDlgItem(IDC_EDIT_Stig_X)->SetWindowText(str);
	str.Format( "%d", nStigY );
	GetDlgItem(IDC_EDIT_Stig_Y)->SetWindowText(str);

	str.Format( "%.2f", dPosX );
	GetDlgItem(IDC_EDIT_Stage_Position_X)->SetWindowText(str);
	str.Format( "%.2f", dPosY );
	GetDlgItem(IDC_EDIT_Stage_Position_Y)->SetWindowText(str);
	str.Format( "%.2f", dPosZ );
	GetDlgItem(IDC_EDIT_Stage_Position_Z)->SetWindowText(str);
	str.Format( "%.2f", dPosR );
	GetDlgItem(IDC_EDIT_Stage_Position_R)->SetWindowText(str);
	str.Format( "%.2f", dPosT );
	GetDlgItem(IDC_EDIT_Stage_Position_T)->SetWindowText(str);
	/////////////////////////////////////////////
}
