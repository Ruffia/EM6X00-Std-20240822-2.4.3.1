// insample0Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "insample0.h"
#include "insample0Dlg.h"

#include "DetectDialog.h"
#include "DlgDiagLog.h"

#include <afxtempl.h>		// MFC support for Template
#include <Mmsystem.h>
#include <Math.h>

#include "Winusb.h"

#define READ_CLOCK 0.0
#define WRITE_CLOCK 0.0

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CInsample0Dlg* m_g_Dlg;

BOOL GetModuleFilePath(LPTSTR exeFullPath)
{
	DWORD i, dwLength;

	dwLength = GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	for ( i=dwLength-1; i > 0; i-- )
	{
		if ( exeFullPath[i] == '\\' )
		{
			exeFullPath[i+1] = '\0';
			break;
		}
	}
	
	if ( dwLength == 0 )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CInsample0Dlg dialog

CInsample0Dlg::CInsample0Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsample0Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsample0Dlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pTabDialog	= NULL;
	m_pBtnMCI		= NULL;
	m_pBtnScnLns	= NULL;
	m_pBtnVacSpr	= NULL;
	m_pPageMCI		= NULL;
	m_pPageSL		= NULL;
	m_pPageVS		= NULL;

	m_hFil			= NULL;
}

CInsample0Dlg::~CInsample0Dlg()
{
	if(	m_hFil != NULL )
		CloseHandle( m_hFil );
	if( m_dlgGraph.m_hWnd != NULL )
		m_dlgGraph.DestroyWindow();
	if( m_dlgAutotest.m_hWnd != NULL )
		m_dlgAutotest.DestroyWindow();
}

void CInsample0Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsample0Dlg)
	DDX_Control(pDX, IDC_BUTTON_Reset_Communicate, m_btnResetComm);
	DDX_Control(pDX, IDC_BUTTON_Diagnose, m_btnDiag);
	DDX_Control(pDX, IDC_REPORT_LIST, m_listReport);
	DDX_Control(pDX, IDC_LIST_ReadbackLB, m_listReadback);
	DDX_Control(pDX, IDC_CHECK_TestLEDs, m_btnTestLEDs);
	DDX_Control(pDX, IDC_DLGPOS, m_dlgPos);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInsample0Dlg, CDialog)
	//{{AFX_MSG_MAP(CInsample0Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_TestLEDs, OnTestLEDs)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_Diagnose, OnDiagnose)
	ON_BN_CLICKED(IDC_BUTTON_Reset_Communicate, OnResetCommunicate)
	ON_BN_CLICKED(IDC_CHECK_TestReadPort, OnCHECKTestReadPort)
	ON_BN_CLICKED(IDC_CHECK_TestReadFIFO, OnCHECKTestReadFIFO)
	ON_BN_CLICKED(IDC_BUTTON_Diagnose_Setup, OnDiagnoseSetup)
	ON_BN_CLICKED(IDC_BUTTON_ClearList, OnClearList)
	ON_BN_CLICKED(IDC_BUTTON_ClearFIFO, OnClearFIFO)
	ON_BN_CLICKED(IDC_BUTTON_AutoTest, OnAutoTest)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USB_SERIAL_RECV, OnSerialRecv)
	ON_MESSAGE(WM_USB_SERIAL_SEND, OnSerialSend)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsample0Dlg message handlers

BOOL CInsample0Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    EnableToolTips(TRUE);		//允许在线提示

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	///////////////////////////////////////////////////////////////////////////
	// 按最快100ms一个数据计算，1秒10个，1分钟600个，1小时36000个，10小时360000个
	for( int i=0; i<GraphDataBuffGroups; i++ )
	{
		theApp.m_pGraphDataBuff[i] = new double[GraphDataBuffCounts];
		ZeroMemory( theApp.m_pGraphDataBuff[i], sizeof(double)*GraphDataBuffCounts );
	}
	///////////////////////////////////////////////////////////////////////////

	unsigned long pID;
	DetectDialog box;
	theApp.deviceIndex = 0;
	if (QueryDeviceInfo(diOnly, &pID, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		if ( pID == 0x800D )
		{
			theApp.deviceIndex = diOnly;
			theApp.m_bIsDI16AReady = TRUE;
		}
	}

	CString str, strError;
	BOOL bLoad = theApp.m_232Device.Serial_Load( strError );

	if (theApp.deviceIndex == 0 || !bLoad )
	{
		if (box.DoModal() == IDOK)
			theApp.deviceIndex = box.index;
	}
	else
	{
		if( !bLoad )
			UpdateListString( strError );
		else
		{
			// 找FT232设备
			theApp.ft232Index = -1;
			int numDevs = theApp.m_232Device.Serial_ListDevicesCounts();
			if( numDevs > -1 )
			{
				str.Format("%d FT232 device(s) attached:", numDevs);
				UpdateListString(str);

				if( numDevs > 0 )
				{
					for( int i=0; i<numDevs; i++)
					{
						str.Format("   #%d Serial Number: %s ", i, theApp.m_232Device.Serial_GetDeviceSerialNumber(i) );
						UpdateListString(str);
						FT_HANDLE ftHandle;
						FT_DEVICE ftDevice;
						FT_STATUS ftStatus;
						DWORD deviceID;
						char SerialNumber[16];
						char Description[64];
						ftStatus = FT_Open(i, &ftHandle);
						if(ftStatus != FT_OK)
						{
							// FT_Open failed return;
							continue;
						}
						ftStatus = FT_GetDeviceInfo( ftHandle, &ftDevice, &deviceID, SerialNumber, Description, NULL );
						if (ftStatus == FT_OK)
						{
							str.Format("   #%d ftDevice: %d ", i, ftDevice );
							UpdateListString(str);
							if (ftDevice == FT_DEVICE_232H)
							{
								// device is FT232H
								theApp.ft232Index = i;
								FT_Close(ftHandle);
								break;
							}
						}
						FT_Close(ftHandle);
					}
				}
				else
				{
					UpdateListString("do not found FT232H device!");
				}
			}
			else
				UpdateListString("do not found FT232 device!");
		}
	}

	theApp.m_bIsFT232Ready = theApp.m_232Device.Serial_Open(theApp.ft232Index);
	str.Format( "ft232Index = %d, ready = %d", theApp.ft232Index, theApp.m_bIsFT232Ready );
	UpdateListString( str );
	///////////////////////////////////////////////////////////////////////////
	InitTabDialog();
	theApp.InitParams();

	if (!m_dlgAutotest.Create( CDlgAutoTestMain::IDD, this ))
	{
		TRACE0("Failed to create CDlgAutoTestMain dlg\n");
	}
	if (!m_dlgGraph.Create( CDlgDiagLogGraph::IDD, this ))
	{
		TRACE0("Failed to create m_dlgGraph dlg\n");
		return FALSE;       // fail to create
	}

	m_btnDiag.SetColor( ::GetSysColor(COLOR_3DFACE) );
	m_btnOK.SetColor( g_crSYS );
	m_btnCancel.SetColor( g_crSYS );
	m_btnResetComm.SetColor( g_crON_Comm );

	m_bDiag = FALSE;
	///////////////////////////////////////////////////////////////////////////
	if( !theApp.m_bIsFT232Ready )
	{
		m_pPageMCI->Init( FALSE );	// 为保持上次状态，不设置初始值了
		return TRUE;
	}

	///////////////////////////////////////////////////////////////////////////
	Init();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CInsample0Dlg::Init()
{
	if( theApp.m_bIsDI16AReady )
	{
		int status;
		double read_clock, write_clock;

		///////////////////////////////////////////////////////////////////////////
		UpdateConfig();

		read_clock = READ_CLOCK;
		write_clock = WRITE_CLOCK;
		status = DIO_StreamSetClocks(theApp.deviceIndex, &read_clock, &write_clock);
		if (status != ERROR_SUCCESS)
		{
			MessageBox("Could not set clocks.", "ERROR");
			return;
		}

		// by DIFast
		DIO_StreamOpen(theApp.deviceIndex, true);
		DIO_StreamClose(theApp.deviceIndex);

		unsigned long lHandle;
		unsigned long lRet = GetDeviceHandle(theApp.deviceIndex, &lHandle);
		if( lRet == 0 )
		{
			unsigned long lNewTimeout = 100;	// in milliseconds.
			BOOL bRet = WinUsb_SetPipePolicy( (WINUSB_INTERFACE_HANDLE)lHandle, 0x86, PIPE_TRANSFER_TIMEOUT, sizeof(lNewTimeout), &lNewTimeout );
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// send version and check returns of port B,D
	BOOL bFromDI16A = FALSE;
	unsigned char input[4];
	DIO_ReadAll(theApp.deviceIndex, &input);
	CString str;
	str.Format( "Read port B & D from USB-DI16A in Init: " );
	UpdateListString( str );
	str.Format( "     D = 0x%02X, B = 0x%02X", input[3], input[1] );
	UpdateListString( str );
	if( input[3] != 0x07 )
//		theApp.m_bIsDI16AReady = TRUE;
		bFromDI16A = TRUE;

	BOOL bFromFT232 = FALSE;
	str = theApp.m_232Device.Serial_WriteToBoard( USB_ReadVersion, FALSE, 0 );
	UpdateListString(str);
	Sleep(100);

	BYTE bBuf[1024];
	DWORD	dwBytesReturned = 0;
	int nRet = theApp.m_232Device.Serial_Read( bBuf, dwBytesReturned );
	if( (nRet == 0) && dwBytesReturned > 0 )
	{
		CString str, strMsg;
		strMsg = "Read port B & D from FT232 in Init: ";
		for(DWORD i=0; i<dwBytesReturned; i++)
		{
			str.Format("0x%02X, \n", bBuf[i]);
			strMsg += str;	
		}
		UpdateListString( strMsg );
		///////////////////////////////////
//		帧头	cD		cB				帧尾
//		××	××	××	FF	FF	××
		///////////////////////////////////
		// port D: 0x01, means MCI power on
		if( (dwBytesReturned > 3)			// 回读数据量够
			&& (bBuf[0] == byte_HeadINT)	// 是BD信息
			&& (bBuf[2] != 0x07) )			// port D = 0x01
			bFromFT232	= TRUE;
	}
 	else
	{
		if( nRet > 0 )
		{
			str.Format( "Read ret = %d", nRet );
			UpdateListString( str );
		}
		if( dwBytesReturned < 1 )
		{
			UpdateListString("no data in buf");
		}
	}

	if( bFromDI16A || bFromFT232 )
		theApp.m_bIsFT232Ready = TRUE;
	else
		theApp.m_bIsFT232Ready = FALSE;

	if( bFromFT232 )//theApp.m_bIsFT232Ready )
	{
/*		// 建立FT232H的读/写线程
		theApp.m_pSend = new CUSB_Send();
		if ( theApp.m_pSend != NULL )
			theApp.m_pSend->Bind( &(theApp.m_232Device) );
		if (!theApp.m_pSend->CreateThread(CREATE_SUSPENDED))
		{
			delete theApp.m_pSend;
			theApp.m_pSend = NULL;
		}
		else
		{
			theApp.m_pSend->SetHandle( this->m_hWnd );
			theApp.m_pSend->SetSharedParams( 3, (void*)theApp.m_pSPlist );
			VERIFY(theApp.m_pSend->SetThreadPriority(THREAD_PRIORITY_NORMAL));
			theApp.m_pSend->ResumeThread();
		}

		theApp.m_pRecv = new CUSB_Recv();
		if ( theApp.m_pRecv != NULL )
			theApp.m_pRecv->Bind( &(theApp.m_232Device) );
		if (!theApp.m_pRecv->CreateThread(CREATE_SUSPENDED))
		{
			delete theApp.m_pRecv;
			theApp.m_pRecv = NULL;
		}
		else
		{
			theApp.m_pRecv->SetHandle( this->m_hWnd );
			theApp.m_pRecv->SetSharedParams( 0, (void*)(theApp.m_DL.adc.chn) );
			theApp.m_pRecv->SetSharedParams( 1, (void*)(theApp.m_pSPbackADCs) );
			theApp.m_pRecv->SetSharedParams( 2, (void*)(theApp.m_pSPbackOthers) );
			theApp.m_pRecv->SetSharedParams( 3, (void*)(theApp.m_pSPlist) );
			VERIFY(theApp.m_pRecv->SetThreadPriority(THREAD_PRIORITY_NORMAL));
			theApp.m_pRecv->ResumeThread();
		}
*/
		// MCI power on, then:
		// set interrupt mask register, mask all off
		theApp.WriteToBoard( USB_SetIMR, TRUE, 0 );
		// communicate with boards
		m_pPageVS->Init();
		SetTimer( tmrWaitToInit, 10, NULL );
	}
	else
	{
		CString str;
		str.LoadString( IDS_MAIN_Power_Not_ON );
		AfxMessageBox( str );
	}
}

void CInsample0Dlg::Init2()
{
	m_pPageMCI->Init();
	m_pPageSL->Init();
	if( m_dlgAutotest.m_hWnd != NULL )
		m_dlgAutotest.InitParams();
}

///////////////////////////////////////////////////////////
// Initialize the TabDialog
BOOL CInsample0Dlg::InitTabDialog()
{
	//create the TabDialog
	m_pTabDialog = new CTabDialog(IDD_TABDLG, this);
	if (m_pTabDialog->Create(IDD_TABDLG, this) == FALSE)
		return FALSE;

	if(!AddPagesToTabDialog())
		return FALSE;

	//get TabDialog's position from the static control
	CRect rect;
	m_dlgPos.GetWindowRect(&rect);
	ScreenToClient((LPRECT) rect);

	//set the TabDialog's positon
	m_pTabDialog->SetWindowPos(this, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);

	//initialize the showing of TabDialog
	m_pTabDialog->InitPagesShow(0);
	m_pBtnMCI->SetCheck( 1 );
//	m_pBtnScnLns->SetCheck( 1 );
//	m_pBtnVacSpr->SetCheck( 1 );
	
	return TRUE;
}

////////////////////////////////////////////////////////////
//Add pages (include button and dialog) to TabDialog
BOOL CInsample0Dlg::AddPagesToTabDialog()
{
	/////////////////////////////////////////////////////////////////
	//create first button
	m_pBtnMCI = new CMacRadioButton();
	RECT rectMCI;
	rectMCI.left	= BTNMCI_LOCATION.x;
	rectMCI.right	= BTNMCI_LOCATION.x+BUTTON_WIDTH;
	rectMCI.top		= BTNMCI_LOCATION.y;
	rectMCI.bottom	= BTNMCI_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnMCI->Create("MCI", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectMCI, m_pTabDialog, IDC_BUTTON_TAB_MCI);
	m_pBtnMCI->SetColor( RGB(128,0,0) );

	//create first dialog
	m_pPageMCI = new CPageMCI(m_pTabDialog);
	if(m_pPageMCI->Create(IDD_PAGE_MCI, m_pTabDialog) == FALSE)
		return FALSE;

	//add first page
	m_pTabDialog->AddPage(m_pPageMCI, m_pBtnMCI);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//Create second button
	m_pBtnScnLns = new CMacRadioButton();
	RECT rectScnLns;
	rectScnLns.left		= BTNSCNLNS_LOCATION.x;
	rectScnLns.right	= BTNSCNLNS_LOCATION.x+BUTTON_WIDTH;
	rectScnLns.top		= BTNSCNLNS_LOCATION.y;
	rectScnLns.bottom	= BTNSCNLNS_LOCATION.y+BUTTON_HEIGHT;
	
	m_pBtnScnLns->Create("SCN - LNS", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectScnLns, m_pTabDialog, IDC_BUTTON_TAB_SCN_LNS);
	m_pBtnScnLns->SetColor( RGB(0,128,0) );

	//create second dialog
	m_pPageSL = new CPageSL(m_pTabDialog);
	if(m_pPageSL->Create(IDD_PAGE_SCN_LNS, m_pTabDialog) == FALSE)
		return FALSE;

	//add second page
	m_pTabDialog->AddPage(m_pPageSL, m_pBtnScnLns);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//Create third button
	m_pBtnVacSpr = new CMacRadioButton();
	RECT rectVacSpr;
	rectVacSpr.left		= BTNVACSPR_LOCATION.x;
	rectVacSpr.right	= BTNVACSPR_LOCATION.x+BUTTON_WIDTH;
	rectVacSpr.top		= BTNVACSPR_LOCATION.y;
	rectVacSpr.bottom	= BTNVACSPR_LOCATION.y+BUTTON_HEIGHT;
	
	m_pBtnVacSpr->Create("VAC - SPR", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectVacSpr, m_pTabDialog, IDC_BUTTON_TAB_VAC_SPR);
	m_pBtnVacSpr->SetColor( RGB(0,0,255) );

	//create third dialog
	m_pPageVS = new CPageVS(m_pTabDialog);
	if(m_pPageVS->Create(IDD_PAGE_VAC_SPR, m_pTabDialog) == FALSE)
		return FALSE;

	//add third page
	m_pTabDialog->AddPage(m_pPageVS, m_pBtnVacSpr);
	/////////////////////////////////////////////////////////////////

	return TRUE;
}

void CInsample0Dlg::OnOK() 
{
	// TODO: Add extra validation here
	Close();
	CDialog::OnOK();
}

void CInsample0Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	Close();
	CDialog::OnCancel();
}

void CInsample0Dlg::Close()
{
	if (theApp.deviceIndex != 0)
	{
		DIO_StreamClose(theApp.deviceIndex);

		// set interrupt mask register, mask all off
		theApp.WriteToBoard( USB_SetIMR, TRUE, 0 );
		// Clear port B & D
		theApp.WriteToBoard( USB_ClearDB, FALSE, 0 );

		ClearDevices();
	}

	if( m_pBtnMCI		!= NULL )
		delete m_pBtnMCI;
	if( m_pBtnScnLns	!= NULL )
		delete m_pBtnScnLns;
	if( m_pBtnVacSpr	!= NULL )
		delete m_pBtnVacSpr;
	if( m_pPageMCI		!= NULL )
	{
		m_pPageMCI->DestroyWindow();
		delete m_pPageMCI;
		m_pPageMCI = NULL;
	}
	if( m_pPageSL		!= NULL )
	{
		m_pPageSL->DestroyWindow();
		delete m_pPageSL;
		m_pPageSL = NULL;
	}
	if( m_pPageVS		!= NULL )
	{
		m_pPageVS->DestroyWindow();
		delete m_pPageVS;
		m_pPageVS = NULL;
	}
	if( m_pTabDialog	!= NULL )
	{
		m_pTabDialog->DestroyWindow();
		delete m_pTabDialog;
		m_pTabDialog = NULL;
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CInsample0Dlg::OnPaint() 
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

void CInsample0Dlg::UpdateConfig()
{
	unsigned char OutMask = 0x05;
	unsigned long DIOData = 0x0;//0xFFFFFFFF;
	unsigned char TristateMask = 0x00;
	DIO_ConfigureEx(theApp.deviceIndex, &OutMask, &DIOData, &TristateMask);
//	DIO_ConfigurationQuery(theApp.deviceIndex, &OutMask, &TristateMask);
}

void CInsample0Dlg::UpdateListString( CString listStr )
{
	m_listReport.AddString( listStr );
	m_listReport.AddString( "" );
	m_listReport.SetCurSel( m_listReport.GetCount() -1 );
}

void CInsample0Dlg::UpdateListReadbackString( CString listStr )
{
	m_listReadback.AddString( listStr );
//	m_listReadback.AddString( "" );
	m_listReadback.SetCurSel( m_listReadback.GetCount() -1 );
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CInsample0Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CInsample0Dlg::OnTestLEDs() 
{
	// Utilizes TestLED16State global, temporarily uses CL wobbler On ckt address (0x1c)
 	unsigned long DIOData = 0;
	CString str = "";

	int nCheck = m_btnTestLEDs.GetCheck();
	if( nCheck )
	{
		m_btnTestLEDs.SetColor( g_crON_Test );
		str.LoadString( IDS_MAIN_Test_LEDs_ON );
		m_btnTestLEDs.SetWindowText( str );
	}
	else
	{
		m_btnTestLEDs.SetColor( ::GetSysColor(COLOR_3DFACE) );
		str.LoadString( IDS_MAIN_Test_LEDs_OFF );
		m_btnTestLEDs.SetWindowText( str );
	}

	///////////////////////////////////////////////////////////////////////////
	theApp.CommToBoards2(	USB_CID_SCN_Test_LED,		// Low byte = ckt addr. of Test LED 16
							USB_TM_SCN_Write_08,		// Serial Comm operand high byte (8 bit write to SCN board)
							0x0,						// Low byte for 8 bit write needs to be in hi byte operand location (for now)
							nCheck );					// Hi byte data is actually low byte of 8 bit write
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	theApp.CommToBoards2(	USB_CID_LNS_Test_LED,		// Low byte = ckt addr. of Test LED 16
							USB_TM_LNS_Write_08,		// Serial Comm operand high byte (8 bit write to LNS board)
							0x0,						// Low byte for 8 bit write needs to be in hi byte operand location (for now)
							nCheck );					// Hi byte data is actually low byte of 8 bit write
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	theApp.CommToBoards2(	USB_CID_VAC_Test_LED,		// Low byte = ckt addr. of Test LED 16
							USB_TM_VAC_Write_08,		// Serial Comm operand high byte (8 bit write to VAC board)
							0x0,						// OP3 = 0 for 8 bit write
							nCheck );					// OP4 contains bit 0 value for LED on/off control
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	theApp.CommToBoards2(	USB_CID_SPR_Test_LED,		// Low byte = ckt addr. of Test LED 16
							USB_TM_SPR_Write_08,		// Serial Comm operand high byte (8 bit write to SPR board)
							0x0,						// OP3 = 0 for 8 bit write
							nCheck );					// OP4 contains bit 0 value for LED on/off control
	///////////////////////////////////////////////////////////////////////////
}

void CInsample0Dlg::OnResetCommunicate() 
{
	// Reset / Initialize Board Communication Circuit
	theApp.WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
}

void CInsample0Dlg::OnClearList() 
{
	// TODO: Add your control notification handler code here
	m_listReport.ResetContent();
	m_listReadback.ResetContent();
}

void CInsample0Dlg::OnClearFIFO() 
{
	// TODO: Add your control notification handler code here
	unsigned long lRet = AIOUSB_ClearFIFO(theApp.deviceIndex, 0);
}

void CInsample0Dlg::OnCHECKTestReadPort() 
{
	// TODO: Add your control notification handler code here
	int bCheck = ((CButton*)(GetDlgItem( IDC_CHECK_TestReadPort )))->GetCheck();
	if( bCheck )
	{
		// 实验用，看USB-DI16A的API函数DIO_ReadAll会不会出错
		SetTimer( tmrReadPort, 500, NULL );
		// 每2分钟清一次列表
		SetTimer( tmrClear, 120000, NULL );
		m_nTestTimes = 0;
		m_nRec0Times = 0;
		GetDlgItem( IDC_CHECK_TestReadFIFO )->EnableWindow( FALSE );

		// 设置日志文件
		TCHAR exeFullPath[255];
		GetModuleFilePath( exeFullPath );
		CTime	timeCurrent = CTime::GetCurrentTime();
		CString strTemp = timeCurrent.Format("%Y%m%d%H%M");
		CString str;
		str.Format( "%sztp%s.log", exeFullPath, strTemp );
		m_hFil = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	}
	else
	{
		KillTimer( tmrReadPort );
		KillTimer( tmrClear );
		GetDlgItem( IDC_CHECK_TestReadFIFO )->EnableWindow( TRUE );

		if(	m_hFil != NULL )
		{
			CloseHandle( m_hFil );
			m_hFil = NULL;
		}
	}
}

DWORD WINAPI CommThreadReadPort(LPVOID lpParam);
DWORD WINAPI CommThreadWritePort(LPVOID lpParam);

HANDLE	g_hThreadReadPort, g_hThreadWritePort;
DWORD	g_dwThreadReadPort, g_dwThreadWritePort;
HANDLE	g_hEventWorkReadPort		= CreateEvent(NULL,FALSE, FALSE, _T("WORKRead"));
HANDLE	g_hEventWorkWritePort		= CreateEvent(NULL,FALSE, FALSE, _T("WORKWrite"));
HANDLE	g_hEventTeminateReadPort	= CreateEvent(NULL,FALSE, FALSE, _T("EndRead"));
HANDLE	g_hEventTeminateWritePort	= CreateEvent(NULL,FALSE, FALSE, _T("EndWrite"));

DWORD WINAPI CommThreadReadPort(LPVOID lpParam)
{
	unsigned char input;
	CString str;
	while(WaitForSingleObject(g_hEventTeminateReadPort, 100) != WAIT_OBJECT_0)
	{
		// 读一次port B&D
		DIO_Read8(theApp.deviceIndex, 3, &input);
		str.Format( "Port D = %02X", input );
		m_g_Dlg->m_listReport.AddString( str );
		DIO_Read8(theApp.deviceIndex, 1, &input);
		str.Format( "Port B = %02X", input );
		m_g_Dlg->m_listReport.AddString( str );
		m_g_Dlg->m_listReport.SetCurSel( m_g_Dlg->m_listReport.GetCount() -1);
		Sleep( 1 );
	}

	SetEvent( g_hEventWorkReadPort );
	return 0;
}

DWORD WINAPI CommThreadWritePort(LPVOID lpParam)
{
	CString str, strTemp;
	unsigned long DIOData = 0;
	while(WaitForSingleObject(g_hEventTeminateWritePort, 150) != WAIT_OBJECT_0)
	{
		// 写一次port A&C
		DIOData = 0xFFF4FF00 | 0x4D;			// Video_ON
		DIO_WriteAll(theApp.deviceIndex, &DIOData);
		strTemp.Format( "0x%X; ", DIOData );
		str = strTemp;
		DIOData = 0xFFF7FF00 | 0x4D;			// Write it to the board
		DIO_WriteAll(theApp.deviceIndex, &DIOData);
		strTemp.Format( "0x%X; ", DIOData );
		str += strTemp;
		DIOData = 0xFFF0FF00 | 0x4D;			// Clear it to the board
		DIO_WriteAll(theApp.deviceIndex, &DIOData);
		strTemp.Format( "0x%X; ", DIOData );
		str += strTemp;

		m_g_Dlg->m_listReport.AddString( str );
		m_g_Dlg->m_listReport.SetCurSel( m_g_Dlg->m_listReport.GetCount() -1);
		Sleep( 1 );
	}

	SetEvent( g_hEventWorkWritePort );
	return 0;
}

void CInsample0Dlg::TestReadPort()
{
 	unsigned long dwInput;
 	unsigned long status = DIO_ReadAll(theApp.deviceIndex, &dwInput);

	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%Y/%m/%d %H:%M:%S");
	CString str;
	str.Format( "[%s] DIO_ReadAll = %ld, 0x%08X", csCurrentTime, status, dwInput );
	m_listReport.AddString( str );
	m_listReport.SetCurSel( m_listReport.GetCount() -1);

	if( m_hFil != NULL )
	{
		unsigned long cbRet;
		char Buf[100];

		sprintf(Buf, "%s", str );
		WriteFile(m_hFil, Buf, strlen(Buf), &cbRet, NULL);
	}
/*
	theApp.WriteToBoard( USB_ReadVersion, FALSE, 0 );
	Sleep(50);
	BYTE bData[1024];
	DWORD i, dwBytesReturned = 0;
	if( theApp.Serial_Read( bData, dwBytesReturned ) == 0 )
	{
		CString str, strTemp;
		if( dwBytesReturned > 0 )
		{
			CTime	timeCurrent = CTime::GetCurrentTime();
			CString csCurrentTime = timeCurrent.Format("%Y/%m/%d %H:%M:%S");
			str.Format( "[%s] DIO_ReadAll = ", csCurrentTime );
			for( i=0; i<dwBytesReturned; i++ )
			{
				strTemp.Format( "0x%02X, ", bData[i] );
				str += strTemp;
			}
			m_listReport.AddString( str );
			m_listReport.SetCurSel( m_listReport.GetCount() -1);
			if( m_hFil != NULL )
			{
				unsigned long cbRet;
				char Buf[100];

				sprintf(Buf, "%s", str );
				WriteFile(m_hFil, Buf, strlen(Buf), &cbRet, NULL);
			}
		}
	}*/
}

void CInsample0Dlg::OnCHECKTestReadFIFO() 
{
	// TODO: Add your control notification handler code here
	long lReadSize = 256;
	unsigned long lBytesTransferred = 0;
	// Allocate on the heap
	unsigned short* pnFrameData = new unsigned short[ lReadSize ];
	ZeroMemory(pnFrameData, sizeof(pnFrameData));

	unsigned long lRet0 = DIO_StreamOpen( theApp.deviceIndex, TRUE );	// TRUE for read; FALSE for write.
//	unsigned long lRet1 = DIO_StreamOpen( theApp.deviceIndex, TRUE );	// TRUE for read; FALSE for write.
	unsigned long lRet2 = DIO_StreamFrame( theApp.deviceIndex, lReadSize, pnFrameData, &lBytesTransferred );
	unsigned long lRet3 = DIO_StreamFrame( theApp.deviceIndex, lReadSize, pnFrameData, &lBytesTransferred );
	unsigned long lRet4 = DIO_StreamClose( theApp.deviceIndex );
	unsigned long lRet5 = DIO_StreamClose( theApp.deviceIndex );
	unsigned long lRet6 = AIOUSB_ClearFIFO(theApp.deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
	unsigned long lRet7 = AIOUSB_ClearFIFO(theApp.deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);
	delete [] pnFrameData;

	CString str;
//	str.Format( "Open1=%d, Open2=%d, Stream1=%d, Stream2=%d", lRet0, lRet1, lRet2, lRet3 );
	str.Format( "Open1=%d, Stream1=%d, Stream2=%d", lRet0, lRet2, lRet3 );
	m_listReport.AddString( str );
	str.Format( "Close1=%d, Close2=%d, Clear1=%d, Clear2=%d", lRet4, lRet5, lRet6, lRet7 );
	m_listReport.AddString( str );
	m_listReport.SetCurSel( m_listReport.GetCount() -1);
	return;

	int bCheck = ((CButton*)(GetDlgItem( IDC_CHECK_TestReadFIFO )))->GetCheck();
	if( bCheck )
	{
		// 实验用，看USB-DI16A的API函数DIO_ReadAll会不会出错
		SetTimer( tmrReadFIFO, 5000, NULL );
		// 每2分钟清一次列表
		SetTimer( tmrClear, 120000, NULL );
		m_nTestTimes = 0;
		m_nRec0Times = 0;
		GetDlgItem( IDC_CHECK_TestReadPort )->EnableWindow( FALSE );

		// 设置日志文件
		TCHAR exeFullPath[255];
		GetModuleFilePath( exeFullPath );
		CTime	timeCurrent = CTime::GetCurrentTime();
		CString strTemp = timeCurrent.Format("%Y%m%d%H%M");
		CString str;
		str.Format( "%sztf%s.log", exeFullPath, strTemp );
		m_hFil = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	}
	else
	{
		KillTimer( tmrReadFIFO );
		KillTimer( tmrClear );
		GetDlgItem( IDC_CHECK_TestReadPort )->EnableWindow( TRUE );

		if(	m_hFil != NULL )
		{
			CloseHandle( m_hFil );
			m_hFil = NULL;
		}
	}
}

void CInsample0Dlg::TestReadFIFO()
{
	// Start MCI Serial instruction (Step 1 of Serial process A)
	theApp.CommToBoards2(	USB_CID_VAC_ReadThermoCCGComp,	// OP1 = ckt addr. of comparator read register on VAC board
							USB_TM_VAC_Read_16,				// OP2 = 0xA3 (16 bit read from VAC board)
	// Start MCI Serial instruction (Step 1 of Serial process D1)
							0x0,							// OP3 = 0 (No data to be written)
							0x0 );							// OP4 = 0 (No data to be written)

	// **************** READ PORTION ******************** //
	long lReadSize = 256;
	unsigned long lBytesTransferred = 0;
	// Allocate on the heap
	unsigned short* pnFrameData = new unsigned short[ lReadSize ];
	ZeroMemory(pnFrameData, sizeof(pnFrameData));

	unsigned long lRet1 = DIO_StreamOpen( theApp.deviceIndex, TRUE );	// TRUE for read; FALSE for write.
	unsigned long lRet2 = DIO_StreamFrame( theApp.deviceIndex, lReadSize, pnFrameData, &lBytesTransferred );
	unsigned long lRet3 = DIO_StreamClose( theApp.deviceIndex );
	unsigned long lRet4 = AIOUSB_ClearFIFO(theApp.deviceIndex, 0);//TIME_METHOD_WAIT_INPUT_ENABLE);

	CString str, strMsg;
	strMsg = "";
	BOOL bFound = FALSE;
	for( unsigned long i=0; i<lBytesTransferred/2; i++ )
	{
		str.Format( "%04X, ", pnFrameData[i] );
		strMsg += str;
	}
	delete [] pnFrameData;

	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%Y/%m/%d %H:%M:%S");
	if( lBytesTransferred /2 == 0 )
	{
		str.Format( "[%s] %ld words, %ld, %ld, %ld, %ld", csCurrentTime, lBytesTransferred /2, lRet1, lRet2, lRet3, lRet4 );
		m_nRec0Times++;
		if( m_nRec0Times > 5 )
		{
			// Reset / Initialize Board Communication Circuit
			theApp.WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
			str += ", Reset";
		}
	}
	else
		str.Format( "[%s] %ld words, %s", csCurrentTime, lBytesTransferred /2, strMsg );
	m_listReport.AddString( str );
	m_listReport.SetCurSel( m_listReport.GetCount() -1);

	if( m_hFil != NULL )
	{
		unsigned long cbRet;
		char Buf[100];

		sprintf(Buf, "%s", str );
		WriteFile(m_hFil, Buf, strlen(Buf), &cbRet, NULL);
	}
}

void CInsample0Dlg::OnDiagnose() 
{
	// Diagnose
//	if( !theApp.m_bIsFT232Ready )
//		return;

	int nCheck = m_btnDiag.GetCheck();
	if( nCheck )
		m_dlgGraph.ShowWindow( SW_SHOW );
	Diagnose( nCheck );
}

void CInsample0Dlg::Diagnose( BOOL bCheck )
{
//	m_btnDiag.SetCheck( bCheck );
	int i;
	if( bCheck )
	{
		///////////////////////////////////////////////////
		// 准备记录文件
		char appfilepath[255];
		::GetModuleFileName( NULL, appfilepath, 255 );
		char *ptemp = strrchr( appfilepath, '\\');
		ptemp[1] = 0;

		CString str, strTemp, strParam, strFirst;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
		str.Format( "%szLog%s.txt", appfilepath, strTemp );
		theApp.m_DL.other.hLogFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		theApp.m_DL.other.lTimes = 1;
		///////////////////////////////////////////////////

		///////////////////////////////////////////////////
		// 更新参数设置
		str			= "\t";
		strParam	= "Min,Max,Step\t";
		strFirst	= "0000\t";

		///////////////////////////////////////////////////
		// Logging Params
		unsigned long cbRet;
		char Buf[1024];

		theApp.m_DL.other.lTimes	= 0;
		theApp.m_DL.other.lLoggingDataCounts = 0;
		if( theApp.m_DL.other.bGaugeCompr )
		{
			strTemp.Format( "Guage compr\t" );
			str += strTemp;
		}
		CString strTemp2;
		for( i=0; i<9; i++ )
		{
			if( theApp.m_DL.log.bLogParam[i] )
			{
				strTemp2.LoadString(IDS_CHECK_PARAM_LNS_L1DAC +i);
				strTemp.Format( "%s\t", strTemp2 );
				str += strTemp;
			}
		}
		for( i=0; i<96; i++ )	// 6*16
		{
			if( theApp.m_DL.log.bLog[i] )
			{
				strTemp.Format( "%s\t\t\t\t\t", theApp.m_DL.adc.chn[i].strLabel );
				str += strTemp;
			}
		}
		sprintf( Buf, "%s\n", str );
		WriteFile( theApp.m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		sprintf( Buf, "%s\n", strParam );
		WriteFile( theApp.m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		sprintf( Buf, "%s\n", strFirst );
		WriteFile( theApp.m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
//		m_pPageVS->SetTimers( Diag_Total, TRUE );
		///////////////////////////////////////////////////

		// SCN
		for( i=Diag_Stig_X; i<Diag_CalI_TC0; i++ )
		{
			theApp.m_DL.diag[i].nIntv	= theApp.m_DL.other.nElapseChanging;
			theApp.m_DL.diag[i].nFinish	= -1;
			if( theApp.m_DL.diag[i].nCheck )
			{
				strTemp.Format( "%s\t", theApp.m_DL.diag[i].strLabel );
				str += strTemp;
				strTemp.Format( "%04X,%04X,%04X\t", theApp.m_DL.diag[i].nMin, theApp.m_DL.diag[i].nMax, theApp.m_DL.diag[i].nStep );
				strParam += strTemp;
				strTemp.Format( "%04X\t", theApp.m_DL.diag[i].nMin );
				strFirst += strTemp;
			}
		}
		// VAC & SPR
		for( i=Diag_CalI_TC0; i<Diag_Offset_0; i++ )
		{
			theApp.m_DL.diag[i].nIntv	= theApp.m_DL.other.nElapseChanging;
			theApp.m_DL.diag[i].nFinish	= -1;
			if( theApp.m_DL.diag[i].nCheck )
			{
				strTemp.Format( "%s\t\t", theApp.m_DL.diag[i].strLabel );
				str += strTemp;
				strTemp.Format( "%04X,%04X,%04X\t\t", theApp.m_DL.diag[i].nMin, theApp.m_DL.diag[i].nMax, theApp.m_DL.diag[i].nStep );
				strParam += strTemp;
				strTemp.Format( "%04X\t\t", theApp.m_DL.diag[i].nMin );
				strFirst += strTemp;
			}
		}
		// MCI
		for( i=Diag_Offset_0; i<Diag_Total; i++ )
		{
			theApp.m_DL.diag[i].nIntv	= theApp.m_DL.other.nElapseChanging;
			theApp.m_DL.diag[i].nFinish	= -1;
			if( theApp.m_DL.diag[i].nCheck )
			{
				strTemp.Format( "%s\t", theApp.m_DL.diag[i].strLabel );
				str += strTemp;
				strTemp.Format( "%04X,%04X,%04X\t", theApp.m_DL.diag[i].nMin, theApp.m_DL.diag[i].nMax, theApp.m_DL.diag[i].nStep );
				strParam += strTemp;
				strTemp.Format( "%04X\t", theApp.m_DL.diag[i].nMin );
				strFirst += strTemp;
			}
		}
		///////////////////////////////////////////////////

		// 准备工作
		///////////////////////////////////////////////////
		m_pPageSL->InitDiagnose( FALSE );
		m_pPageVS->InitDiagnose( FALSE );
		KillTimer( tmrReadPort );
		KillTimer( tmrClear );

		// Changing Params
		// SCN & LNS
		for( i=Diag_Stig_X; i<Diag_CalI_TC0; i++ )
			if( theApp.m_DL.diag[i].nCheck )
				m_pPageSL->SetTimers( i, TRUE );
		// VAC & SPR
		for( i=Diag_CalI_TC0; i<Diag_Offset_0; i++ )
			if( theApp.m_DL.diag[i].nCheck )
				m_pPageVS->SetTimers( i, TRUE );
		// MCI
		for( i=Diag_Offset_0; i<Diag_Total; i++ )
			if( theApp.m_DL.diag[i].nCheck )
				m_pPageMCI->SetTimers( i, TRUE );
		///////////////////////////////////////////////////

		m_dlgGraph.m_nTotal = -1;
		m_dlgGraph.InitGraphData();

		///////////////////////////////////////////////////
		// 停止采集、设置USB-PIXEL RATE、清空FIFO
		m_pPageVS->StopScan();
		m_pPageVS->SetPxlClkto2();
		unsigned long lRet = AIOUSB_ClearFIFO(theApp.deviceIndex, 0);

		GetDlgItem( IDC_BUTTON_Diagnose_Setup )->EnableWindow( FALSE );
		m_btnDiag.SetColor( g_crEmphasis3 );
		str.LoadString( IDS_Diag_Begin );
		m_btnDiag.SetWindowText( str );

		SetTimer( Diag_Total, theApp.m_DL.other.nElapseLogging, NULL );
	}
	else
	{
/*		// SCN & LNS
		for( int i=Diag_Stig_X; i<Diag_CalI_TC0; i++ )
			m_pPageSL->SetTimers( i, FALSE );
		// VAC & SPR
		for( i=Diag_CalI_TC0; i<Diag_Offset_0; i++ )
			m_pPageVS->SetTimers( i, FALSE );
		// MCI
		for( i=Diag_Offset_0; i<Diag_Total; i++ )
			m_pPageMCI->SetTimers( i, FALSE );
*/
		KillTimer( Diag_Total );
		CloseHandle(theApp.m_DL.other.hLogFile);

		m_pPageSL->InitDiagnose( TRUE );
		m_pPageVS->InitDiagnose( TRUE );

		GetDlgItem( IDC_BUTTON_Diagnose_Setup )->EnableWindow( TRUE );
		m_btnDiag.SetColor( ::GetSysColor(COLOR_3DFACE) );
		CString str, strTemp;
		str.LoadString( IDS_Diag_Finish );
		m_btnDiag.SetWindowText( str );

		// 准备记录文件
		char appfilepath[255];
		::GetModuleFileName( NULL, appfilepath, 255 );
		char *ptemp = strrchr( appfilepath, '\\');
		ptemp[1] = 0;

		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d-%H%M%S");
		str.Format( "%szLog%s-Graph.txt", appfilepath, strTemp );
		theApp.m_DL.other.hLogFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		unsigned long cbRet;
		char Buf[100];

		for( i=0; i<theApp.m_DL.other.lTimes-1; i++ )
		{
			str = "";
			for( int j=0; j<GraphDataBuffGroups; j++ )
			{
				if( theApp.m_DL.graph[j].bGraph )
				{
					strTemp.Format( "%.8f\t", theApp.m_pGraphDataBuff[j][i] );
					str += strTemp;
				}
			}
			sprintf( Buf, "%s\n", str );
			WriteFile( theApp.m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
		}
		CloseHandle(theApp.m_DL.other.hLogFile);
	}
}

void CInsample0Dlg::OnDiagnoseSetup() 
{
	// TODO: Add your control notification handler code here
	CDlgDiagLog dlg;
	dlg.DoModal();
}

void CInsample0Dlg::OnAutoTest() 
{
	// TODO: Add your control notification handler code here
	m_dlgAutotest.ShowWindow(SW_SHOW);
}

void CInsample0Dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == tmrReadPort )
		TestReadPort();
	else if( nIDEvent == tmrReadFIFO )
		TestReadFIFO();
	else if( nIDEvent == tmrClear )
	{
		m_listReport.ResetContent();
		CTime	timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime = timeCurrent.Format("%Y/%m/%d %H:%M:%S");
		CString str;
		str.Format( "[%s] %04d times", csCurrentTime, ++m_nTestTimes );
		m_listReadback.AddString( str );
		m_listReadback.SetCurSel( m_listReadback.GetCount() -1);
	}
	else if( nIDEvent == tmrWaitToInit )
	{
		Init2();
		KillTimer( tmrWaitToInit );
	}
	else if( nIDEvent == tmrDiag )
	{
		for( int i=0; i<Diag_Total; i++ )
		{
			if( theApp.m_DL.diag[i].nCheck && (theApp.m_DL.diag[i].nFinish == 0) )
				return;
		}
		KillTimer( tmrDiag );
		m_btnDiag.SetCheck( 0 );
		Diagnose( 0 );
	}
	else if( nIDEvent == Diag_Total )
	{
		CString str, strTemp;
		str.Format( "%04d\t", theApp.m_DL.other.lTimes++ );

		strTemp = Diag_Logging2();
		str += strTemp;
		strTemp = Diag_SCN_LNS();
		str += strTemp;
		strTemp = Diag_VAC();
		str += strTemp;
		strTemp = Diag_SPR();
		str += strTemp;
		strTemp = Diag_MCI();
		str += strTemp;

		unsigned long cbRet;
		char Buf[4096];
		sprintf( Buf, "%s\n", str );
		WriteFile( theApp.m_DL.other.hLogFile, Buf, strlen(Buf), &cbRet, NULL );
	}

	CDialog::OnTimer(nIDEvent);
}

CString CInsample0Dlg::Diag_SCN_LNS()
{
	int nIndex = 0;
	CString str, strRes;
	strRes = "";

	if( theApp.m_DL.diag[Diag_Stig_X].nCheck )
	{
		nIndex = Diag_Stig_X;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos +2048 );
		strRes += str;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax -2048 +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin -2048;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageSL->m_scrollXstigDAC.SetScrollPos( nPos );
			m_pPageSL->XstigDACSB_Change( nPos );
		}
	}
	if( theApp.m_DL.diag[Diag_Stig_Y].nCheck )
	{
		nIndex = Diag_Stig_Y;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos +2048 );
		strRes += str;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax -2048 +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin -2048;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageSL->m_scrollYstigDAC.SetScrollPos( nPos );
			m_pPageSL->YstigDACSB_Change( nPos );
		}
	}
	if( theApp.m_DL.diag[Diag_Shift_X].nCheck )
	{
		nIndex = Diag_Shift_X;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos +2048 );
		strRes += str;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax -2048 +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin -2048;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageSL->m_scrollXshiftDAC.SetScrollPos( nPos );
			m_pPageSL->XshiftDACSB_Change( nPos );
		}
	}
	if( theApp.m_DL.diag[Diag_Shift_Y].nCheck )
	{
		nIndex = Diag_Shift_Y;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos +2048 );
		strRes += str;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax -2048 +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin -2048;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageSL->m_scrollYshiftDAC.SetScrollPos( nPos );
			m_pPageSL->YshiftDACSB_Change( nPos );
		}
	}
	if( theApp.m_DL.diag[Diag_xover_X].nCheck )
	{
		nIndex = Diag_xover_X;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos +2048 );
		strRes += str;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax -2048 +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin -2048;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageSL->m_scrollXxoverDAC.SetScrollPos( nPos );
			m_pPageSL->XxoverDACSB_Change( nPos );
		}
	}
	if( theApp.m_DL.diag[Diag_xover_Y].nCheck )
	{
		nIndex = Diag_xover_Y;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos +2048 );
		strRes += str;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax -2048 +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin -2048;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageSL->m_scrollYxoverDAC.SetScrollPos( nPos );
			m_pPageSL->YxoverDACSB_Change( nPos );
		}
	}
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	if( theApp.m_DL.diag[Diag_CL].nCheck )
	{
		nIndex = Diag_CL;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		strRes += str;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax -32768 +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin -32768;
//				theApp.m_DL.diag[Diag_CL].nFinish = 1;
			}
			m_pPageSL->m_scrollL1DAC.SetScrollPos( nPos );
			m_pPageSL->L1DACSB_Change( nPos );
		}
	}
	if( theApp.m_DL.diag[Diag_OL].nCheck )
	{
		nIndex = Diag_OL;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		strRes += str;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax -32768 +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin -32768;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageSL->m_scrollL2SDAC.SetScrollPos( nPos );
			m_pPageSL->L2SDACSB_Change( nPos );
		}
	}
	if( theApp.m_DL.diag[Diag_Align_X].nCheck )
	{
		nIndex = Diag_Align_X;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		strRes += str;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax -32768 +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin -32768;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageSL->m_scrollXalDAC.SetScrollPos( nPos );
			m_pPageSL->XalDACSB_Change( nPos );
		}
	}
	if( theApp.m_DL.diag[Diag_Align_Y].nCheck )
	{
		nIndex = Diag_Align_Y;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		str.Format( "%04X\t", nPos +32768 );
		strRes += str;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax -32768 +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin -32768;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageSL->m_scrollYalDAC.SetScrollPos( nPos );
			m_pPageSL->YalDACSB_Change( nPos );
		}
	}
	if( theApp.m_DL.diag[Diag_Wob].nCheck )
	{
		nIndex = Diag_Wob;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		str.Format( "%03X\t", nPos +2048 );
		strRes += str;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax -2048 +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin -2048;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageSL->m_scrollWobDAC.SetScrollPos( nPos );
			m_pPageSL->WobDACSB_Change( nPos );
		}
	}

	return strRes;
}

CString CInsample0Dlg::Diag_VAC()
{
	int nIndex = 0;
	CString str, strRes;
	strRes = "";

	if( theApp.m_DL.diag[Diag_CalI_TC0].nCheck )
	{
		nIndex = Diag_CalI_TC0;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nVACDACselected	= 0;
			m_pPageVS->m_nVACDACchannel		= 0;
			m_pPageVS->VACDACgroupOptionChanged();
			m_pPageVS->VACDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->VACDACvalUpdate( nPos );
			m_pPageVS->m_scrollVACDACsVal.SetScrollPos( nPos );
			m_pPageVS->VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_CalI_TC1].nCheck )
	{
		nIndex = Diag_CalI_TC1;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nVACDACselected	= 0;
			m_pPageVS->m_nVACDACchannel		= 1;
			m_pPageVS->VACDACgroupOptionChanged();
			m_pPageVS->VACDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->VACDACvalUpdate( nPos );
			m_pPageVS->m_scrollVACDACsVal.SetScrollPos( nPos );
			m_pPageVS->VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_CalI_TC2].nCheck )
	{
		nIndex = Diag_CalI_TC2;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nVACDACselected	= 0;
			m_pPageVS->m_nVACDACchannel		= 2;
			m_pPageVS->VACDACgroupOptionChanged();
			m_pPageVS->VACDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->VACDACvalUpdate( nPos );
			m_pPageVS->m_scrollVACDACsVal.SetScrollPos( nPos );
			m_pPageVS->VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_CalI_TC3].nCheck )
	{
		nIndex = Diag_CalI_TC3;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nVACDACselected	= 0;
			m_pPageVS->m_nVACDACchannel		= 3;
			m_pPageVS->VACDACgroupOptionChanged();
			m_pPageVS->VACDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->VACDACvalUpdate( nPos );
			m_pPageVS->m_scrollVACDACsVal.SetScrollPos( nPos );
			m_pPageVS->VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_CalI_TC4].nCheck )
	{
		nIndex = Diag_CalI_TC4;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nVACDACselected	= 2;
			m_pPageVS->m_nVACDACchannel		= 0;
			m_pPageVS->VACDACgroupOptionChanged();
			m_pPageVS->VACDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->VACDACvalUpdate( nPos );
			m_pPageVS->m_scrollVACDACsVal.SetScrollPos( nPos );
			m_pPageVS->VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_CalI_TC5].nCheck )
	{
		nIndex = Diag_CalI_TC5;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nVACDACselected	= 2;
			m_pPageVS->m_nVACDACchannel		= 1;
			m_pPageVS->VACDACgroupOptionChanged();
			m_pPageVS->VACDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->VACDACvalUpdate( nPos );
			m_pPageVS->m_scrollVACDACsVal.SetScrollPos( nPos );
			m_pPageVS->VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	if( theApp.m_DL.diag[Diag_SetPoint_TC0].nCheck )
	{
		nIndex = Diag_SetPoint_TC0;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nVACDACselected	= 1;
			m_pPageVS->m_nVACDACchannel		= 0;
			m_pPageVS->VACDACgroupOptionChanged();
			m_pPageVS->VACDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->VACDACvalUpdate( nPos );
			m_pPageVS->m_scrollVACDACsVal.SetScrollPos( nPos );
			m_pPageVS->VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_SetPoint_TC1].nCheck )
	{
		nIndex = Diag_SetPoint_TC1;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nVACDACselected	= 1;
			m_pPageVS->m_nVACDACchannel		= 1;
			m_pPageVS->VACDACgroupOptionChanged();
			m_pPageVS->VACDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->VACDACvalUpdate( nPos );
			m_pPageVS->m_scrollVACDACsVal.SetScrollPos( nPos );
			m_pPageVS->VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_SetPoint_TC2].nCheck )
	{
		nIndex = Diag_SetPoint_TC2;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nVACDACselected	= 1;
			m_pPageVS->m_nVACDACchannel		= 2;
			m_pPageVS->VACDACgroupOptionChanged();
			m_pPageVS->VACDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->VACDACvalUpdate( nPos );
			m_pPageVS->m_scrollVACDACsVal.SetScrollPos( nPos );
			m_pPageVS->VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_SetPoint_TC3].nCheck )
	{
		nIndex = Diag_SetPoint_TC3;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nVACDACselected	= 1;
			m_pPageVS->m_nVACDACchannel		= 3;
			m_pPageVS->VACDACgroupOptionChanged();
			m_pPageVS->VACDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->VACDACvalUpdate( nPos );
			m_pPageVS->m_scrollVACDACsVal.SetScrollPos( nPos );
			m_pPageVS->VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_SetPoint_TC4].nCheck )
	{
		nIndex = Diag_SetPoint_TC4;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nVACDACselected	= 2;
			m_pPageVS->m_nVACDACchannel		= 2;
			m_pPageVS->VACDACgroupOptionChanged();
			m_pPageVS->VACDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->VACDACvalUpdate( nPos );
			m_pPageVS->m_scrollVACDACsVal.SetScrollPos( nPos );
			m_pPageVS->VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_SetPoint_TC5].nCheck )
	{
		nIndex = Diag_SetPoint_TC5;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nVACDACselected	= 2;
			m_pPageVS->m_nVACDACchannel		= 3;
			m_pPageVS->VACDACgroupOptionChanged();
			m_pPageVS->VACDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->VACDACvalUpdate( nPos );
			m_pPageVS->m_scrollVACDACsVal.SetScrollPos( nPos );
			m_pPageVS->VACDACupdate( nPos );
		}
		str.Format( "%03X\t%.3f\t", nPos, 1.0 *nPos /4095 * 2.5 );
		strRes += str;
	}

	return strRes;
}

CString CInsample0Dlg::Diag_SPR()
{
	int nIndex = 0;
	CString str, strRes;
	strRes = "";

	if( theApp.m_DL.diag[Diag_SPRDAC_ChA].nCheck )
	{
		nIndex = Diag_SPRDAC_ChA;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nSPRDACchannel = 0;
			m_pPageVS->SPRDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->SPRDACsValUpdae( nPos -32768 );
			m_pPageVS->SPRDACupdate();
		}
		str.Format( "%04X\t%.6f\t", nPos, 2.5 *nPos /65536 );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_SPRDAC_ChB].nCheck )
	{
		nIndex = Diag_SPRDAC_ChB;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nSPRDACchannel = 1;
			m_pPageVS->SPRDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->SPRDACsValUpdae( nPos -32768 );
			m_pPageVS->SPRDACupdate();
		}
		str.Format( "%04X\t%.6f\t", nPos, 2.5 *nPos /65536 );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_SPRDAC_ChC].nCheck )
	{
		nIndex = Diag_SPRDAC_ChC;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nSPRDACchannel = 2;
			m_pPageVS->SPRDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->SPRDACsValUpdae( nPos -32768 );
			m_pPageVS->SPRDACupdate();
		}
		str.Format( "%04X\t%.6f\t", nPos, 2.5 *nPos /65536 );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_SPRDAC_ChD].nCheck )
	{
		nIndex = Diag_SPRDAC_ChD;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageVS->m_nSPRDACchannel = 3;
			m_pPageVS->SPRDACselOptionChanged();
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageVS->SPRDACsValUpdae( nPos -32768 );
			m_pPageVS->SPRDACupdate();
		}
		str.Format( "%04X\t%.6f\t", nPos, 2.5 *nPos /65536 );
		strRes += str;
	}

	return strRes;
}

CString CInsample0Dlg::Diag_MCI()
{
	int nIndex = 0;
	CString str, strRes;
	strRes = "";

	if( theApp.m_DL.diag[Diag_Offset_0].nCheck )
	{
		nIndex = Diag_Offset_0;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageMCI->m_nDet03Chn2 = 0;
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageMCI->m_scrollDet03.SetScrollPos( nPos );
			m_pPageMCI->Det03SB_Change( nPos );
		}
		str.Format( "%03X\t", nPos );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_Offset_1].nCheck )
	{
		nIndex = Diag_Offset_1;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageMCI->m_nDet03Chn2 = 1;
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageMCI->m_scrollDet03.SetScrollPos( nPos );
			m_pPageMCI->Det03SB_Change( nPos );
		}
		str.Format( "%03X\t", nPos );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_Offset_2].nCheck )
	{
		nIndex = Diag_Offset_2;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageMCI->m_nDet03Chn2 = 2;
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageMCI->m_scrollDet03.SetScrollPos( nPos );
			m_pPageMCI->Det03SB_Change( nPos );
		}
		str.Format( "%03X\t", nPos );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_Offset_3].nCheck )
	{
		nIndex = Diag_Offset_3;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageMCI->m_nDet03Chn2 = 3;
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageMCI->m_scrollDet03.SetScrollPos( nPos );
			m_pPageMCI->Det03SB_Change( nPos );
		}
		str.Format( "%03X\t", nPos );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_Offset_4].nCheck )
	{
		nIndex = Diag_Offset_4;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageMCI->m_nDet45Chn2 = 0;
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageMCI->m_scrollDet45.SetScrollPos( nPos );
			m_pPageMCI->Det45SB_Change( nPos );
		}
		str.Format( "%03X\t", nPos );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_Offset_5].nCheck )
	{
		nIndex = Diag_Offset_5;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageMCI->m_nDet45Chn2 = 1;
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageMCI->m_scrollDet45.SetScrollPos( nPos );
			m_pPageMCI->Det45SB_Change( nPos );
		}
		str.Format( "%03X\t", nPos );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_FineMag_X].nCheck )
	{
		nIndex = Diag_FineMag_X;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageMCI->m_nDet45Chn2 = 2;
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageMCI->m_scrollDet45.SetScrollPos( nPos );
			m_pPageMCI->Det45SB_Change( nPos );
		}
		str.Format( "%03X\t", nPos );
		strRes += str;
	}
	if( theApp.m_DL.diag[Diag_FineMag_Y].nCheck )
	{
		nIndex = Diag_FineMag_Y;
		int nPos = theApp.m_DL.diag[nIndex].nCurPos;
		if( theApp.m_DL.diag[nIndex].nFinish < 0 )
		{
			m_pPageMCI->m_nDet45Chn2 = 3;
			nPos += theApp.m_DL.diag[nIndex].nStep;
			if( nPos > theApp.m_DL.diag[nIndex].nMax +1)
			{
				nPos = theApp.m_DL.diag[nIndex].nMin;
//				theApp.m_DL.diag[nIndex].nFinish = 1;
			}
			m_pPageMCI->m_scrollDet45.SetScrollPos( nPos );
			m_pPageMCI->Det45SB_Change( nPos );
		}
		str.Format( "%03X\t", nPos );
		strRes += str;
	}

	return strRes;
}

CString CInsample0Dlg::Diag_Logging()
{
	CString str, strRes;
	strRes = "";
	int i, j=0;
	int nTotal = 16;

	///////////////////////////////////////////////////////////////////////////
	// Gauge Comparators
	if( theApp.m_DL.other.bGaugeCompr )
	{
		str = m_pPageVS->ReadComprs_Logging();
		strRes += str;
	}
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// LNS set value and comp switch
	if( theApp.m_DL.log.bLogParam[Logging_CL] )
	{
		str.Format( "%04X\t", theApp.m_DL.diag[Diag_CL].nCurPos +32768);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_OL] )
	{
		str.Format( "%04X\t", theApp.m_DL.diag[Diag_OL].nCurPos +32768);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_Align_X] )
	{
		str.Format( "%04X\t", theApp.m_DL.diag[Diag_Align_X].nCurPos +32768);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_Align_Y] )
	{
		str.Format( "%04X\t", theApp.m_DL.diag[Diag_Align_Y].nCurPos +32768);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_Wob] )
	{
		str.Format( "%04X\t", theApp.m_DL.diag[Diag_Wob].nCurPos +2048);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_Comp_CL] )
	{
		str.Format( "%d\t", theApp.m_DL.log.nCheckComp[Logging_Comp_CL -5]);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_Comp_OL] )
	{
		str.Format( "%d\t", theApp.m_DL.log.nCheckComp[Logging_Comp_OL -5]);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_Comp_Align_X] )
	{
		str.Format( "%d\t", theApp.m_DL.log.nCheckComp[Logging_Comp_Align_X -5]);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_Comp_Align_Y] )
	{
		str.Format( "%d\t", theApp.m_DL.log.nCheckComp[Logging_Comp_Align_Y -5]);
		strRes += str;
	}
	///////////////////////////////////////////////////////////////////////////

	double dValue = 0.0;
	///////////////////////////////////////////////////////////////////////////
	// SCN
	theApp.ChangedADCselectOB( 0 );
	for( i=0; i<nTotal; i++ )
	{
		if( theApp.m_DL.log.bLog[j*16 +i] )
		{
			theApp.ChangedADCsChSelect( i );
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			strRes += str;
		}
	}
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	j++;
	nTotal = 8;
	// LNS
	theApp.ChangedADCselectOB( 1 );
	for( i=0; i<nTotal; i++ )
	{
		if( theApp.m_DL.log.bLog[j*16 +i] )
		{
			theApp.ChangedADCsChSelect( i );
			str = theApp.ReadSelectedADC_Logging2( 5, dValue );
			strRes += str;
		}
	}
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	j++;
	nTotal = 16;
	// VAC0
	theApp.ChangedADCselectOB( 2 );
	for( i=0; i<nTotal; i++ )
	{
		if( theApp.m_DL.log.bLog[j*16 +i] )
		{
			theApp.ChangedADCsChSelect( i );
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			strRes += str;
		}
	}
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	j++;
	// VAC1
	theApp.ChangedADCselectOB( 3 );
	for( i=0; i<nTotal; i++ )
	{
		if( theApp.m_DL.log.bLog[j*16 +i] )
		{
			theApp.ChangedADCsChSelect( i );
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			strRes += str;
		}
	}
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	j++;
	// SPR0
	theApp.ChangedADCselectOB( 4 );
	for( i=0; i<nTotal; i++ )
	{
		if( theApp.m_DL.log.bLog[j*16 +i] )
		{
			theApp.ChangedADCsChSelect( i );
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			strRes += str;
		}
	}
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	j++;
	// SPR1
	theApp.ChangedADCselectOB( 5 );
	for( i=0; i<nTotal; i++ )
	{
		if( theApp.m_DL.log.bLog[j*16 +i] )
		{
			theApp.ChangedADCsChSelect( i );
			str = theApp.ReadSelectedADC_Logging2( 80, dValue );
			strRes += str;
		}
	}
	///////////////////////////////////////////////////////////////////////////

	return strRes;
}

CString CInsample0Dlg::Diag_Logging2()
{
	CString str, strRes;
	strRes = "";
	int i, j=0;
	int nTotal = 16;

	///////////////////////////////////////////////////////////////////////////
	// LNS set value and comp switch
	if( theApp.m_DL.log.bLogParam[Logging_CL] )
	{
		str.Format( "%04X\t", theApp.m_DL.diag[Diag_CL].nCurPos +32768);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_OL] )
	{
		str.Format( "%04X\t", theApp.m_DL.diag[Diag_OL].nCurPos +32768);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_Align_X] )
	{
		str.Format( "%04X\t", theApp.m_DL.diag[Diag_Align_X].nCurPos +32768);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_Align_Y] )
	{
		str.Format( "%04X\t", theApp.m_DL.diag[Diag_Align_Y].nCurPos +32768);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_Wob] )
	{
		str.Format( "%04X\t", theApp.m_DL.diag[Diag_Wob].nCurPos +2048);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_Comp_CL] )
	{
		str.Format( "%d\t", theApp.m_DL.log.nCheckComp[Logging_Comp_CL -5]);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_Comp_OL] )
	{
		str.Format( "%d\t", theApp.m_DL.log.nCheckComp[Logging_Comp_OL -5]);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_Comp_Align_X] )
	{
		str.Format( "%d\t", theApp.m_DL.log.nCheckComp[Logging_Comp_Align_X -5]);
		strRes += str;
	}
	if( theApp.m_DL.log.bLogParam[Logging_Comp_Align_Y] )
	{
		str.Format( "%d\t", theApp.m_DL.log.nCheckComp[Logging_Comp_Align_Y -5]);
		strRes += str;
	}
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// 发送所有需回读的命令
	nTotal = theApp.m_DL.log2.nCounts;	// 有多少需要从FIFO回读的参数
	int nSendReadCmdCounts = 0;			// 为从FIFO回读所有参数，总共发送了多少次回读命令
	for( i=0; i<nTotal; i++ )
	{
		logParam lP = theApp.m_DL.log2.logParams[i];
		switch( lP.nType )
		{
		case	2:
			{
				theApp.OnlySendReadSelectedADCcmd( lP );
				theApp.Delay( 100 );
				nSendReadCmdCounts++;
			}
			break;

		case	3:
			{
				theApp.OnlySendReadSelectedADCcmd( lP );
				theApp.Delay( 100 );
				nSendReadCmdCounts++;
			}
			break;

		default:
			{
				if( lP.bType )
				{
					// 2448，发送2次读命令
					theApp.OnlySendReadSelectedADCcmd( lP );
					theApp.OnlySendReadSelectedADCcmd( lP );
					theApp.Delay( 100 );
					theApp.OnlySendReadSelectedADCcmd( lP );
					theApp.Delay( 100 );
					nSendReadCmdCounts += 3;
				}
				else
				{
					// 1859，发送3次读命令
					theApp.OnlySendReadSelectedADCcmd( lP );
					theApp.OnlySendReadSelectedADCcmd( lP );
					theApp.Delay( 5 );
					theApp.OnlySendReadSelectedADCcmd( lP );
					theApp.Delay( 5 );
					nSendReadCmdCounts += 3;
				}
			}
			break;
		}
	}
	Sleep(50);
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// 读一次FIFO
// 	long lReadSize = 96*6 +512;
// 	unsigned long lBytesTransferred = -1;
// 	// Allocate on the heap
// 	unsigned short* pnFrameData = new unsigned short[ lReadSize ];
// 	ZeroMemory(pnFrameData, sizeof(pnFrameData));
// 
// 	DIO_StreamOpen( theApp.deviceIndex, TRUE );	// TRUE for read; FALSE for write.
// 	unsigned long lRet = DIO_StreamFrame( theApp.deviceIndex, lReadSize, pnFrameData, &lBytesTransferred );
// 	DIO_StreamClose( theApp.deviceIndex );

	long lReadSize = 96*10 +128;
	DWORD lBytesTransferred;
	unsigned short* pnFrameData = new unsigned short[ lReadSize ];
	long lRet = theApp.Serial_Read( pnFrameData, lBytesTransferred );
	if( (lRet != 0) || (lBytesTransferred < 1) )
	{
		UpdateListReadbackString( "Serial read failed" );
		delete [] pnFrameData;
		return strRes;
	}
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// 先看一看0xD22D的个数是否符合发出命令的个数
	///////////////////////////////////////////////////////////////////////////
	nTotal = lBytesTransferred/2;
	int nRecvReadCounts = 0;	// 总共收到多少组回读值，每组6个数据，每个0xD22D后面跟着一组回读值
	for( i=0; i<nTotal; i++ )
	{
		str.Format( "@ %02d (hex %04X)", i, pnFrameData[i] );
		UpdateListReadbackString( str );
		if( pnFrameData[i] == 0xD22D )
			nRecvReadCounts++;
	}
	UpdateListReadbackString( " " );
	if( nRecvReadCounts != nSendReadCmdCounts )
	{
		str.Format( "RecvReadCounts(%d) != SendReadCmdCounts(%d)", nRecvReadCounts, nSendReadCmdCounts );
		UpdateListReadbackString( str );
		delete [] pnFrameData;
		return strRes;
	}

	///////////////////////////////////////////////////////////////////////////
	// 将有效的0xD22D后边的数据取出
	///////////////////////////////////////////////////////////////////////////
	// 每个有效数据由两个16位数据组成
	int *pnValidData;
	int nValidCount = 0;	// 总共要分析记录多少组数据
	if( theApp.m_DL.other.bDebug )
	{
		pnValidData = new int[nRecvReadCounts *2];
		ZeroMemory(pnValidData, sizeof(pnValidData));
		for( i=0; i<nTotal; i++ )
		{
			if( pnFrameData[i] == 0xD22D )
			{
				// 13.05.09 调试模式下保留每一次读的值
				pnValidData[nValidCount *2]		= pnFrameData[i+1];
				pnValidData[nValidCount *2 +1]	= pnFrameData[i+2];
				nValidCount++;
				i += 2;
			}
		}
	}
	else
	{
		pnValidData = new int[theApp.m_DL.log2.nCounts *2];
		ZeroMemory(pnValidData, sizeof(pnValidData));
		int nD22Dcount	= 0;	// 非调试模式下，用于记录已忽略的无效数据的个数
		for( i=0; i<nTotal; i++ )
		{
			if( pnFrameData[i] == 0xD22D )
			{
				nD22Dcount++;
				if( theApp.m_DL.log2.logParams[nValidCount].nType == 2 )		// gauge compr
				{
					nD22Dcount = 0;
					pnValidData[nValidCount *2]		= pnFrameData[i+1];
					pnValidData[nValidCount *2 +1]	= pnFrameData[i+1];
					nValidCount++;
					i += 1;
				}
				else if( theApp.m_DL.log2.logParams[nValidCount].nType == 3 )	// Ext. switch
				{
					nD22Dcount = 0;
					pnValidData[nValidCount *2]		= pnFrameData[i+1];
					pnValidData[nValidCount *2 +1]	= pnFrameData[i+2];
					nValidCount++;
					i += 2;
				}
				else
				{
					if( theApp.m_DL.log2.logParams[nValidCount].bType && nD22Dcount == 3 )
					{
						nD22Dcount = 0;
						pnValidData[nValidCount *2]		= pnFrameData[i+1];
						pnValidData[nValidCount *2 +1]	= pnFrameData[i+2];
						nValidCount++;

						str.Format( "@ %02d (hex %04X)", i+1, pnFrameData[i+1] );
						UpdateListReadbackString( str );
						str.Format( "@ %02d (hex %04X)", i+2, pnFrameData[i+2] );
						UpdateListReadbackString( str );
						UpdateListReadbackString( " " );
						i += 2;
					}
					else if( !(theApp.m_DL.log2.logParams[nValidCount].bType) && nD22Dcount == 3 )
					{
						nD22Dcount = 0;
						pnValidData[nValidCount *2]		= pnFrameData[i+1];
						pnValidData[nValidCount *2 +1]	= pnFrameData[i+1];
						nValidCount++;

						str.Format( "@ %02d (hex %04X)", i+1, pnFrameData[i+1] );
						UpdateListReadbackString( str );
						UpdateListReadbackString( " " );
						i += 1;
					}
				}
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// 分析数据并记录
	int k=0;
	double dValue = 0.0;
	if( theApp.m_DL.other.bDebug )
	{
		int m = 0;
		nTotal = nValidCount;
		for( i=0; i<nTotal; i++ )
		{
			logParam lP = theApp.m_DL.log2.logParams[m];
			m++;
			if( lP.nType == 2 )
			{
				str.Format( "%04X\t", pnValidData[i*2] );
				strRes += str;
			}
			else if( lP.nType == 3 )
			{
				str = "";
				strRes += str;
			}
			else
			{
/*				if( lP.bType )	// 2448，两次
				{
					str = theApp.ADCvconv_Logging( pnValidData[i*2], pnValidData[i*2+1], lP, dValue );
					strRes += str;
					if( (k < GraphDataBuffGroups) && (theApp.m_DL.graph[k].nIndex == lP.nNum*16 +lP.nChn) )
						theApp.m_pGraphDataBuff[k][theApp.m_DL.other.lLoggingDataCounts] = dValue;
					i++;
					str = theApp.ADCvconv_Logging( pnValidData[i*2], pnValidData[i*2+1], lP, dValue );
					strRes += str;
					if( (k < GraphDataBuffGroups) && (theApp.m_DL.graph[k].nIndex == lP.nNum*16 +lP.nChn) )
					{
						theApp.m_pGraphDataBuff[k][theApp.m_DL.other.lLoggingDataCounts +1] = dValue;
						theApp.m_pGraphDataBuff[k][theApp.m_DL.other.lLoggingDataCounts +2] = dValue;
						k++;
					}
				}
				else			// 1859，三次
*/				{
					str = theApp.ADCvconv_Logging( pnValidData[i*2], pnValidData[i*2+1], lP, dValue );
					strRes += str;
					if( (k < GraphDataBuffGroups) && (theApp.m_DL.graph[k].nIndex == lP.nNum*16 +lP.nChn) )
						theApp.m_pGraphDataBuff[k][theApp.m_DL.other.lLoggingDataCounts] = dValue;
					i++;
					str = theApp.ADCvconv_Logging( pnValidData[i*2], pnValidData[i*2+1], lP, dValue );
					strRes += str;
					if( (k < GraphDataBuffGroups) && (theApp.m_DL.graph[k].nIndex == lP.nNum*16 +lP.nChn) )
						theApp.m_pGraphDataBuff[k][theApp.m_DL.other.lLoggingDataCounts +1] = dValue;
					i++;
					str = theApp.ADCvconv_Logging( pnValidData[i*2], pnValidData[i*2+1], lP, dValue );
					strRes += str;
					if( (k < GraphDataBuffGroups) && (theApp.m_DL.graph[k].nIndex == lP.nNum*16 +lP.nChn) )
					{
						theApp.m_pGraphDataBuff[k][theApp.m_DL.other.lLoggingDataCounts +2] = dValue;
						k++;
					}
				}
			}
		}
	}
	else
	{
		nTotal = theApp.m_DL.log2.nCounts;
		for( i=0; i<nTotal; i++ )
		{
			logParam lP = theApp.m_DL.log2.logParams[i];
			if( lP.nType == 2 )
			{
				str = m_pPageVS->ReadComprs_Logging( pnValidData[i*2] );
				strRes += str;
			}
			else if( lP.nType == 3 )
			{
				str = "";
				strRes += str;
			}
			else
			{
				str = theApp.ADCvconv_Logging( pnValidData[i*2], pnValidData[i*2+1], lP, dValue );
				strRes += str;
				if( (k < GraphDataBuffGroups) && (theApp.m_DL.graph[k].nIndex == lP.nNum*16 +lP.nChn) )
				{
					theApp.m_pGraphDataBuff[k][theApp.m_DL.other.lTimes-1] = dValue;
					k++;
				}
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////

	delete [] pnFrameData;
	delete [] pnValidData;

	if( theApp.m_DL.other.bDebug )
	{
		// 13.05.09
		m_dlgGraph.UpdateGraphData( theApp.m_DL.other.lLoggingDataCounts );
		theApp.m_DL.other.lLoggingDataCounts += 3;
	}
	else
		m_dlgGraph.UpdateGraphData( theApp.m_DL.other.lTimes );
	return strRes;
}

LRESULT CInsample0Dlg::OnSerialRecv(WPARAM wParam, LPARAM lParam)
{
	USB_SEM_ControlMessage	usb_hostMSG;
	usb_hostMSG = *((USB_SEM_ControlMessage*)wParam);

	CString str;
	str.Format( "FT232 Data Received: %ld bytes", usb_hostMSG.bCtlMSGLength );
	UpdateListReadbackString( str );

	DWORD dwBytesReturned = usb_hostMSG.bCtlMSGLength;
	for( DWORD i=0; i<dwBytesReturned; i++ )
	{
		str.Format( "@ %d: 0x%02X", i, usb_hostMSG.bData[i] );
		UpdateListReadbackString( str );
	}
	return 0;
}

LRESULT CInsample0Dlg::OnSerialSend(WPARAM wParam, LPARAM lParam)
{
	CString csError;
//	theApp.m_pSend->GetLastErrorInformation( csError );

	UpdateListString( csError );
	return 0;
}
