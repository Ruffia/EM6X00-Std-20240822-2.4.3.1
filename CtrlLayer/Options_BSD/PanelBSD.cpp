// PanelBSD.cpp : implementation file
//

#include "stdafx.h"
#include "Options_BSD.h"
#include "PanelBSD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Declare this global handle in one of your project files.
HHOOK g_hookBSD = NULL;

/////////////////////////////////////////////////////////////////////////////
// CPanelBSD dialog


CPanelBSD::CPanelBSD(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelBSD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelBSD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bFineRev = FALSE;
}


void CPanelBSD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelBSD)
	DDX_Control(pDX, IDC_SCROLLBAR_Fine, m_scrollFine);
	DDX_Control(pDX, IDC_SCROLLBAR_Brt, m_scrollBrt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanelBSD, CDialog)
	//{{AFX_MSG_MAP(CPanelBSD)
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_Port, OnSelchangeCOMBOPort)
	ON_BN_CLICKED(IDC_RADIO_Channels_A, OnChannelsChanged)
	ON_BN_CLICKED(IDC_RADIO_Coarse_X050, OnCoarseChanged)
	ON_BN_CLICKED(IDC_RADIO_Channels_B, OnChannelsChanged)
	ON_BN_CLICKED(IDC_RADIO_Channels_AB1, OnChannelsChanged)
	ON_BN_CLICKED(IDC_RADIO_Channels_AB2, OnChannelsChanged)
	ON_BN_CLICKED(IDC_RADIO_Coarse_X010, OnCoarseChanged)
	ON_BN_CLICKED(IDC_RADIO_Coarse_X100, OnCoarseChanged)
	ON_BN_CLICKED(IDC_CHECK_Fine, OnCHECKFine)
	ON_BN_CLICKED(IDC_BUTTON_ReLink, OnRelink)
	ON_BN_CLICKED(IDC_CHECK_InOut, OnCHECKInOut)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_BSD_Update, OnUpdateBSD)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelBSD message handlers

// After that, implement your hook procedure and call TranslateAccelerator.

LRESULT CALLBACK GetBSDMsgHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LPMSG lpMsg = (LPMSG)lParam;
	
	if( nCode >= 0 && PM_REMOVE == wParam )
	{
		// Don't translate non-input events.
//		if( (lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST) )
		if( lpMsg->message == WM_KEYDOWN && lpMsg->wParam == VK_RETURN )
		{
			//			if( IsDialogMessage(m_hWnd, lpMsg) )
			if( theApp.m_pPanelBSD != NULL && theApp.m_pPanelBSD->m_hWnd != NULL )
			{
				if( IsDialogMessage(theApp.m_pPanelBSD->m_hWnd, lpMsg) )
				{
					theApp.m_pPanelBSD->PreTranslateMessage(lpMsg);
					// The value returned from this hookproc is ignored, 
					// and it cannot be used to tell Windows the message
					// has been handled. To avoid further processing,
					// convert the message to WM_NULL before returning.
					lpMsg->message	= WM_NULL;
					lpMsg->lParam	= 0;
					lpMsg->wParam	= 0;
				}
			}
		}
	}
	return CallNextHookEx(g_hookBSD, nCode, wParam, lParam);
}

BOOL CPanelBSD::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	CComboBox* pPort = ((CComboBox*)(GetDlgItem(IDC_COMBO_Port)));
	if( pPort != NULL )
	{
		for( int i=0; i<30; i++ )
		{
			str.Format( "COM %d", i+1 );
			pPort->AddString( str );
		}
	}

	((CButton*)(GetDlgItem(IDC_RADIO_Channels_A)))->SetCheck(1);
	((CButton*)(GetDlgItem(IDC_RADIO_Coarse_X010)))->SetCheck(1);
	m_scrollFine.SetScrollRange(0, 0x7FFF);
	m_scrollFine.SetScrollPos(0);
	GetDlgItem(IDC_STATIC_Fine_Value)->SetWindowText("[ 0xBFFF ],  75.000");
	m_scrollBrt.SetScrollRange(0, 0xFFF);
	m_scrollBrt.SetScrollPos(0);
	GetDlgItem(IDC_STATIC_Brt_Value)->SetWindowText("[ 0x7FF ],  49.99");

	SetStatus(FALSE);
	Connect();

	// 将对话框位置设置到客户区中部靠右下
	CRect rc;
	GetWindowRect(rc);
	SetWindowPos(&CWnd::wndTop,
		GetSystemMetrics(SM_CXSCREEN) - 2.5*rc.Width() -10,
		GetSystemMetrics(SM_CYSCREEN) - 1.8*rc.Height() -10,
		rc.Width(),
		rc.Height(),
		SWP_NOSIZE);

	// Place this code inside an initialization method in your implementation file (.cpp)
	g_hookBSD = SetWindowsHookEx(WH_GETMESSAGE, GetBSDMsgHookProc, NULL, GetCurrentThreadId());
	// 原文链接：https://blog.csdn.net/wewaa/article/details/46422187
	//			https://blog.csdn.net/sy10086/article/details/54232202

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPanelBSD::OnDestroy() 
{
	// Finally, when your application is shutting down or when you no longer need the hook, unhook the procedure.
	UnhookWindowsHookEx(g_hookBSD);
	m_BSD.Close();
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CPanelBSD::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int nOldPos = pScrollBar->GetScrollPos();
	int nNewPos = pScrollBar->GetScrollPos();
	SCROLLINFO  scrollInfo;
	pScrollBar->GetScrollInfo( &scrollInfo, SIF_ALL );
	switch (nSBCode) 
	{
	case SB_LINELEFT:
		nNewPos = nOldPos-1;
		nNewPos = ( nNewPos < scrollInfo.nMin )?scrollInfo.nMin:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	case SB_LINERIGHT:
		nNewPos = nOldPos+1;
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
		nNewPos = nOldPos-10;
		nNewPos = ( nNewPos < scrollInfo.nMin )?scrollInfo.nMin:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	case SB_PAGERIGHT:
		nNewPos = nOldPos+10;
		nNewPos = ( nNewPos > scrollInfo.nMax )?scrollInfo.nMax:nNewPos;
		pScrollBar->SetScrollPos( nNewPos );
		break;

	case SB_ENDSCROLL:
		break;

	case SB_THUMBPOSITION:
		pScrollBar->SetScrollPos( scrollInfo.nTrackPos );
		break;

	case SB_THUMBTRACK:
		pScrollBar->SetScrollPos( scrollInfo.nTrackPos );
		break;
	}

	nNewPos = pScrollBar->GetScrollPos();
	if( nOldPos == nNewPos )
		return;

	int id = pScrollBar->GetDlgCtrlID();	//获取滚动条ID
	switch( id )
	{
	case IDC_SCROLLBAR_Fine:
		BSD_Fine_Change( pScrollBar->GetScrollPos() );
		break;
	case IDC_SCROLLBAR_Brt:
		BSD_Brt_Change( pScrollBar->GetScrollPos() );
		break;
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPanelBSD::OnOK() 
{
	// TODO: Add extra validation here
//	CDialog::OnOK();
}

void CPanelBSD::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}

void CPanelBSD::InitHWND( HWND hWnd )
{
	m_hMain = hWnd;
}

void CPanelBSD::Connect()
{
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\BSD\\BSD.ini");
	char szBuf[255];
	// 通讯端口
	m_nPort = 1;
	if(GetPrivateProfileString("BSDCfg", "Port", "3", szBuf, 15, path) != 0)
		m_nPort = atoi(szBuf);
	CComboBox* pPort = ((CComboBox*)(GetDlgItem(IDC_COMBO_Port)));
	if( pPort != NULL )
		pPort->SetCurSel( m_nPort -1 );

	BOOL bNeedCommTest = FALSE;	// 默认是老用户，不支持通讯测试命令 2024.06
	if(GetPrivateProfileString("BSDCfg", "CommTest", "0", szBuf, 15, path) != 0)
		m_BSD.SetCommTest( (BOOL)(atoi(szBuf)) );

	// 默认不支持自动移入移出 2024.08
	int nInOut = GetPrivateProfileInt("BSDCfg", "InOut", 0, path);
	if( nInOut > 0 )
		GetDlgItem(IDC_CHECK_InOut)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_CHECK_InOut)->ShowWindow(SW_HIDE);

	m_BSD.InitHWND( this->m_hWnd );
	m_bIsReady = m_BSD.Open(m_nPort);
	Localize();
}

void CPanelBSD::SetLanguageID( WORD wID )
{
	m_wLanguageID = wID;
	char szBuf[20];
	if( wID == MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED))
		sprintf_s( szBuf, "zh_CN" );
	else if( wID == MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT))
		sprintf_s( szBuf, "en_US" );
	else if( wID == MAKELANGID(LANG_RUSSIAN,SUBLANG_DEFAULT))
		sprintf_s( szBuf, "ru_RU" );
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\BSD\\BSD.ini");
	// 写入配置文件
	WritePrivateProfileString( "BSDCfg", "Language", szBuf, path );
	Localize();
}

void CPanelBSD::Localize()
{
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\BSD\\BSD.ini");
	char szLang[20];
	if(GetPrivateProfileString("BSDCfg", "Language", "", szLang, 20, path) != 0)
	{
		exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		if( strcmp( szLang, "zh_CN" ) == 0)
			strcat_s(path, "\\Locale\\zh\\BSD_zh_CN.ini");
		else if( strcmp( szLang, "en_US" ) == 0)
			strcat_s(path, "\\Locale\\en\\BSD_en_US.ini");
		else if( strcmp( szLang, "ru_RU" ) == 0)
			strcat_s(path, "\\Locale\\ru\\BSD_ru_RU.ini");
		strcpy_s( m_cPathLocalize, path );
		char szBuf[255];
		// 界面元素
		if(GetPrivateProfileString("Label", "Cap", "", szBuf, 20, path) != 0)
			SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "CommPort", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_STATIC_Port)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Channels", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_STATIC_Group_Channels)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Coarse", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_STATIC_Group_Coarse)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Fine", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_STATIC_Fine)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Brt", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_STATIC_Brt)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "FineRev", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_CHECK_Fine)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Relink", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_BUTTON_ReLink)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "InOut", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_CHECK_InOut)->SetWindowText( szBuf );
	}
}

LRESULT CPanelBSD::OnUpdateBSD(WPARAM wParam, LPARAM lParam)
{
	CString str;
	long lSteps = (long)lParam;
	switch( wParam )
	{
	case	0:
		m_bIsReady = (BOOL)lParam;
		SetStatus(m_bIsReady);
		if(m_bIsReady)
		{
//			InitLog();
			str.Format("Open COM %d successfully!", m_nPort);
			InitParams();
		}
		else
		{
			m_BSD.Close();
			str.Format("Open COM %d Failed!",m_nPort);
		}
		break;
	}
	return 0;
}

void CPanelBSD::SetStatus(BOOL bEnable)
{
	CWnd* pWndRect = GetDlgItem(IDC_STATIC_Rect);
	if( pWndRect == NULL )
		return;
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

void CPanelBSD::InitParams()
{
	int nPos = 0;
	// 亮度初始值：45%
	nPos = 0xFFF *0.45;
	m_scrollBrt.SetScrollPos(nPos);
	BSD_Brt_Change(nPos);
	// 对比度细调
	nPos = 0x7FFF *0.4;
	m_scrollFine.SetScrollPos(nPos);
	BSD_Fine_Change(nPos);
}

void CPanelBSD::OnSelchangeCOMBOPort() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pPort = ((CComboBox*)(GetDlgItem(IDC_COMBO_Port)));
	if( pPort != NULL )
		m_nPort = pPort->GetCurSel() +1;
	// 获取工作路径
	char path[255];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\BSD\\BSD.ini");
	char szBuf[255];
	// 写入配置文件
	sprintf_s( szBuf, "%d", m_nPort );
	WritePrivateProfileString( "BSDCfg", "Port", szBuf, path );
	Relink();
}

void CPanelBSD::OnRelink()
{
	Relink();
}

void CPanelBSD::Relink()
{
//	::PostMessage( m_hMain, WM_USER	+41, 3, 0 );	// WM_USER_CCD_MSG
	TRACE("BSD: close the communication module\n");
	m_BSD.Close();
	Connect();
}

void CPanelBSD::OnChannelsChanged() 
{
	// TODO: Add your control notification handler code here
	int i;
	for( i=0; i<4; i++ )
	{
		int nCheck = ((CButton*)(GetDlgItem(IDC_RADIO_Channels_A +i)))->GetCheck();
		if( nCheck > 0 )
			break;
	}
	m_BSD.ChangeChannel(i);
}

void CPanelBSD::OnCoarseChanged() 
{
	// TODO: Add your control notification handler code here
	int i;
	for( i=0; i<3; i++ )
	{
		int nCheck = ((CButton*)(GetDlgItem(IDC_RADIO_Coarse_X010 +i)))->GetCheck();
		if( nCheck > 0 )
			break;
	}
	m_BSD.ChangeCoarse(i);
}

void CPanelBSD::BSD_Fine_Change( int nPos )
{
	CString str;
	WORD wValue = 0x00;
	if( m_bFineRev )
		wValue = 0x8000 +(WORD)nPos;
	else
		wValue = 0x8000 -(WORD)nPos;
	str.Format( "[ 0x%03X ],  %5.3f", wValue, nPos *100.0 /0x7FFF );
	GetDlgItem( IDC_STATIC_Fine_Value )->SetWindowText( str );
	m_BSD.ChangeFine( wValue );
}

void CPanelBSD::BSD_Brt_Change( int nPos )
{
	CString str;
//	WORD wValue = (WORD)nPos;			// 正向
	WORD wValue = 0x0FFF -(WORD)nPos;	// 反向 2022.03.22
	str.Format( "[ 0x%03X ],  %5.2f", wValue, nPos *100.0 /0xFFF );
	GetDlgItem( IDC_STATIC_Brt_Value )->SetWindowText( str );
	m_BSD.ChangeBrt( wValue );
}

void CPanelBSD::OnCHECKFine() 
{
	// TODO: Add your control notification handler code here
	m_bFineRev = !m_bFineRev;
	BSD_Fine_Change( m_scrollFine.GetScrollPos() );
}

void CPanelBSD::OnCHECKInOut()
{
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_InOut)))->GetCheck();
	if( nCheck < 1 )
		nCheck = 0x02;
	::PostMessage( m_hMain, WM_USER_BSD_Update, 1, nCheck );
}