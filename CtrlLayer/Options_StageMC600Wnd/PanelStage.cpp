// PanelStage.cpp : implementation file
//

#include "stdafx.h"
#include "Options_StageMC600Wnd.h"
#include "PanelStage.h"
#include "DlgSetup.h"
#include "DlgInput.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/*
const	double	g_dEquivalentXY = 0.000625;		// 1mm / (200 * 8)
const	double	g_dEquivalentZ	= 0.00375;		//
const	double	g_dEquivalentR	= 0.00375;		// 360 / (200 * 8 * 60)
*/

const UINT	nStageMC600_Elapse = 300;
const long	lSpeedTrackMax = 400;

CString g_csUScaleUnit;
double	g_dfMobileUScaleParameter	= -1;

// Declare this global handle in one of your project files.
HHOOK g_hook = NULL;

/////////////////////////////////////////////////////////////////////////////
// CPanelStage dialog


CPanelStage::CPanelStage(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelStage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelStage)
	//}}AFX_DATA_INIT
	m_hIconStage1	= NULL;
	m_hIconStage2	= NULL;
	m_hIconStage3	= NULL;
	m_hIconStage4	= NULL;
	m_hIconStage5	= NULL;
	m_hIconStage6	= NULL;
	m_hIconStage7	= NULL;
	m_hIconStage8	= NULL;
	m_hIconStage9	= NULL;
	m_hIconStage10	= NULL;
	m_hFileLog		= NULL;

	m_dEquivalent[0]	= 0.0003125;
	m_dEquivalent[1]	= 0.0003125;
	m_dEquivalent[2]	= 0.0003125;
	m_dEquivalent[3]	= 0.03621;

	m_sizeReso.cx	= 1024;
	m_sizeReso.cy	= 768;

	m_nStageType	= 2;	// 4轴
}


void CPanelStage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelStage)
	DDX_Control(pDX, IDC_LIST_Position, m_listPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanelStage, CDialog)
	//{{AFX_MSG_MAP(CPanelStage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_X_Move_Minus, OnBUTTONXMoveMinus)
	ON_BN_CLICKED(IDC_BUTTON_X_Move_Plus, OnBUTTONXMovePlus)
	ON_BN_CLICKED(IDC_BUTTON_Y_Move_Minus, OnBUTTONYMoveMinus)
	ON_BN_CLICKED(IDC_BUTTON_Y_Move_Plus, OnBUTTONYMovePlus)
	ON_BN_CLICKED(IDC_BUTTON_Z_Move_Minus, OnBUTTONZMoveMinus)
	ON_BN_CLICKED(IDC_BUTTON_Z_Move_Plus, OnBUTTONZMovePlus)
	ON_BN_CLICKED(IDC_BUTTON_R_Move_Minus, OnBUTTONRMoveMinus)
	ON_BN_CLICKED(IDC_BUTTON_R_Move_Plus, OnBUTTONRMovePlus)
	ON_BN_CLICKED(IDC_BUTTON_T_Move_Minus, OnBUTTONTMoveMinus)
	ON_BN_CLICKED(IDC_BUTTON_T_Move_Plus, OnBUTTONTMovePlus)
	ON_BN_CLICKED(IDC_BUTTON_X_Move_Minus2, OnBUTTONXMoveMinus2)
	ON_BN_CLICKED(IDC_BUTTON_X_Move_Plus2, OnBUTTONXMovePlus2)
	ON_BN_CLICKED(IDC_BUTTON_Y_Move_Minus2, OnBUTTONYMoveMinus2)
	ON_BN_CLICKED(IDC_BUTTON_Y_Move_Plus2, OnBUTTONYMovePlus2)
	ON_BN_CLICKED(IDC_BUTTON_Z_Move_Minus2, OnBUTTONZMoveMinus2)
	ON_BN_CLICKED(IDC_BUTTON_Z_Move_Plus2, OnBUTTONZMovePlus2)
	ON_BN_CLICKED(IDC_BUTTON_R_Move_Minus2, OnBUTTONRMoveMinus2)
	ON_BN_CLICKED(IDC_BUTTON_R_Move_Plus2, OnBUTTONRMovePlus2)
	ON_BN_CLICKED(IDC_BUTTON_T_Move_Minus2, OnBUTTONTMoveMinus2)
	ON_BN_CLICKED(IDC_BUTTON_T_Move_Plus2, OnBUTTONTMovePlus2)
	ON_BN_CLICKED(IDC_BUTTON_X_MoveTo, OnBUTTONXMoveTo)
	ON_BN_CLICKED(IDC_BUTTON_Y_MoveTo, OnBUTTONYMoveTo)
	ON_BN_CLICKED(IDC_BUTTON_Z_MoveTo, OnBUTTONZMoveTo)
	ON_BN_CLICKED(IDC_BUTTON_R_MoveTo, OnBUTTONRMoveTo)
	ON_BN_CLICKED(IDC_BUTTON_T_MoveTo, OnBUTTONTMoveTo)
	ON_BN_CLICKED(IDC_BUTTON_MoveTo, OnBUTTONMoveTo)
	ON_BN_CLICKED(IDC_BUTTON_Stop, OnBUTTONStop)
	ON_BN_CLICKED(IDC_BUTTON_Zero, OnBUTTONZero)
	ON_BN_CLICKED(IDC_BUTTON_ZeroX, OnBUTTONZeroX)
	ON_BN_CLICKED(IDC_BUTTON_ZeroY, OnBUTTONZeroY)
	ON_BN_CLICKED(IDC_BUTTON_ZeroZ, OnBUTTONZeroZ)
	ON_BN_CLICKED(IDC_BUTTON_ZeroR, OnBUTTONZeroR)
	ON_BN_CLICKED(IDC_BUTTON_ZeroT, OnBUTTONZeroT)
	ON_BN_CLICKED(IDC_BUTTON_Home, OnBUTTONHome)
	ON_BN_CLICKED(IDC_BUTTON_HomeX, OnBUTTONHomeX)
	ON_BN_CLICKED(IDC_BUTTON_HomeY, OnBUTTONHomeY)
	ON_BN_CLICKED(IDC_BUTTON_HomeZ, OnBUTTONHomeZ)
	ON_BN_CLICKED(IDC_BUTTON_HomeR, OnBUTTONHomeR)
	ON_BN_CLICKED(IDC_BUTTON_HomeT, OnBUTTONHomeT)
	ON_WM_TIMER()
	ON_NOTIFY(UDN_DELTAPOS, IDC_Stage_Mag_Adj, OnDeltaposStageMagAdj)
	ON_BN_CLICKED(IDC_CHECK_Relative, OnCHECKRelative)
	ON_BN_CLICKED(IDC_CHECK_Advance, OnCHECKAdvance)
	ON_BN_CLICKED(IDC_BUTTON_Setup, OnBUTTONSetup)
	ON_BN_CLICKED(IDC_BUTTON_Backlash, OnBUTTONBacklash)
	ON_BN_CLICKED(IDC_BUTTON_ListChange, OnBUTTONListChange)
	ON_BN_CLICKED(IDC_BUTTON_ListAdd, OnBUTTONListAdd)
	ON_BN_CLICKED(IDC_BUTTON_ListDel, OnBUTTONListDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_Position, OnDblclkLISTPosition)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SetUserOrigin, OnBUTTONSetUserOrigin)
	ON_BN_CLICKED(IDC_CHECK_Z_WD, OnCheckZWd)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_MC600_UpdateValue, OnUpdateMC600Value)
	ON_MESSAGE(WM_USER_Vince_UpdateValue, OnUpdateVinceValue)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelStage message handlers

// After that, implement your hook procedure and call TranslateAccelerator.

LRESULT CALLBACK GetMsgHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LPMSG lpMsg = (LPMSG)lParam;

	if( nCode >= 0 && PM_REMOVE == wParam )
	{
		// Don't translate non-input events.
//		if( (lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST) )
		if( lpMsg->message == WM_KEYDOWN && lpMsg->wParam == VK_RETURN )
		{
//			if( IsDialogMessage(m_hWnd, lpMsg) )
			if( theApp.m_pPanelStage != NULL && theApp.m_pPanelStage->m_hWnd != NULL )
			{
				if( IsDialogMessage(theApp.m_pPanelStage->m_hWnd, lpMsg) )
				{
					theApp.m_pPanelStage->PreTranslateMessage(lpMsg);
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
	return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

BOOL CPanelStage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_bIsMC600Ready	= FALSE;
	m_bIsVinceReady	= FALSE;
	m_bIsMoving		= FALSE;
	m_bBlClick		= FALSE;
	m_bBlTrack		= FALSE;
	m_bWD			= FALSE;
	m_bPEswitch		= FALSE;
	m_nProcess		= -1;
	m_nMoveSteps	= -1;
	m_lStepBase		= 10000;
	m_dStepRatio	= 1.0;
	m_nHomeState	= -1;
	m_nSpeedRatio	= -1;
	m_nVentSel		= 0;
	m_bOptR			= FALSE;

	for( int i=0; i<5; i++ )
	{
		m_lCurStep[i]	= 0;
		m_nSpeed[i]		= 3;	// 5000;
		m_nTimer[i]		= 0;
		m_fCali[i]		= 0.0;
		m_fGoal[i]		= 0.0;
		m_nTimerCounts[i]	= 0;
	}
// 	m_lCenterX		= 0;	//114866;
// 	m_lCenterY		= 0;	//114866;
// 	m_lCenterZ		= 0;
	m_dEquivalent[4]	= 0.0015625;

	// 图标
	m_hIconStage1	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_Left));
	m_hIconStage2	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_Right));
	m_hIconStage3	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_Up));
	m_hIconStage4	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_Down));
	m_hIconStage5	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_RLeft));
	m_hIconStage6	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_RRight));
	m_hIconStage7	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_ZUp));
	m_hIconStage8	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_ZDown));
	m_hIconStage9	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_TUp));
	m_hIconStage10	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_TDown));
	((CButton*)( GetDlgItem( IDC_BUTTON_X_Move_Minus ) ))->SetIcon( m_hIconStage1 );
	((CButton*)( GetDlgItem( IDC_BUTTON_X_Move_Plus ) ))->SetIcon( m_hIconStage2 );
	((CButton*)( GetDlgItem( IDC_BUTTON_Y_Move_Minus ) ))->SetIcon( m_hIconStage3 );
	((CButton*)( GetDlgItem( IDC_BUTTON_Y_Move_Plus ) ))->SetIcon( m_hIconStage4 );
	((CButton*)( GetDlgItem( IDC_BUTTON_R_Move_Minus ) ))->SetIcon( m_hIconStage6 );
	((CButton*)( GetDlgItem( IDC_BUTTON_R_Move_Plus ) ))->SetIcon( m_hIconStage5 );
	((CButton*)( GetDlgItem( IDC_BUTTON_Z_Move_Minus ) ))->SetIcon( m_hIconStage8 );
	((CButton*)( GetDlgItem( IDC_BUTTON_Z_Move_Plus ) ))->SetIcon( m_hIconStage7 );
	((CButton*)( GetDlgItem( IDC_BUTTON_T_Move_Minus ) ))->SetIcon( m_hIconStage10 );
	((CButton*)( GetDlgItem( IDC_BUTTON_T_Move_Plus ) ))->SetIcon( m_hIconStage9 );
	// 运动方式
	m_bRelative		= FALSE;
	m_bRelativeBak	= FALSE;
	((CButton*)GetDlgItem(IDC_CHECK_Relative))->SetCheck(0);
//	((CButton*)GetDlgItem(IDC_RADIO_StepRatio_06))->SetCheck(1);
	SetStatus(FALSE);
	SetStatus2(FALSE);

	GetDlgItem(IDC_EDIT_X_UserDisp)->SetWindowText( "0.000" );
	GetDlgItem(IDC_EDIT_Y_UserDisp)->SetWindowText( "0.000" );
	GetDlgItem(IDC_EDIT_Z_UserDisp)->SetWindowText( "0.000" );
	GetDlgItem(IDC_EDIT_R_UserDisp)->SetWindowText( "0" );
	GetDlgItem(IDC_EDIT_T_UserDisp)->SetWindowText( "0" );

	m_dMag = 100.0;
	GetDlgItem(IDC_Stage_Mag)->SetWindowText( "100" );

	// 设置位置列表
	List_SetHead(0, "Label");
	List_SetHead(1, "X mm");
	List_SetHead(2, "Y mm");
	List_SetHead(3, "Z mm");
	List_SetHead(4, "R deg");
	List_Init();

//	Connect();

	// 将对话框位置设置到客户区左上角
	GetWindowRect(m_rcWindow);
	GetDlgItem(IDC_EDIT_Report)->GetClientRect(m_rcReport);
	SetWindowPos(&CWnd::wndTop,
				10,		//GetSystemMetrics(SM_CXSCREEN) - rc.Width()-10,
				32*3,	//GetSystemMetrics(SM_CYSCREEN) /2 -rc.Height() /2,
				m_rcWindow.Width(),
				m_rcWindow.Height() -m_rcReport.Height() -10,	//rc.Height(),
				SWP_SHOWWINDOW);	//SWP_NOSIZE);

	// Place this code inside an initialization method in your implementation file (.cpp)
	g_hook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgHookProc, NULL, GetCurrentThreadId());
	// 原文链接：https://blog.csdn.net/wewaa/article/details/46422187
	//			https://blog.csdn.net/sy10086/article/details/54232202

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPanelStage::OnDestroy() 
{
// 	Message( "Go Origion X/Y/Z/T" );
// 	m_MC600.GoOrigion('Z');
// 	m_MC600.GoOrigion('X');
// 	m_MC600.GoOrigion('Y');

	for( int i=0; i<4; i++ )
	{
		if( m_nTimer[i] != 0 )
		{
			KillTimer( m_nTimer[i] );
			m_nTimer[i] = 0;
		}
	}
	// Finally, when your application is shutting down or when you no longer need the hook, unhook the procedure.
	UnhookWindowsHookEx(g_hook);

	POSITION pos;
	while( (pos=m_plistMC600Pos->GetHeadPosition()) )
	{
		structMC600Pos* pItem = m_plistMC600Pos->GetAt( pos );
		m_plistMC600Pos->RemoveHead();
		delete pItem;
	}
	delete m_plistMC600Pos;

	m_MC600.Close();
	if( m_hFileLog != NULL )
		CloseHandle( m_hFileLog );
	m_hFileLog	= NULL;

	m_Vince.Close();
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	if( m_hIconStage1 != NULL )
		::DestroyIcon( m_hIconStage1 );
	if( m_hIconStage2 != NULL )
		::DestroyIcon( m_hIconStage2 );
	if( m_hIconStage3 != NULL )
		::DestroyIcon( m_hIconStage3 );
	if( m_hIconStage4 != NULL )
		::DestroyIcon( m_hIconStage4 );
	if( m_hIconStage5 != NULL )
		::DestroyIcon( m_hIconStage5 );
	if( m_hIconStage6 != NULL )
		::DestroyIcon( m_hIconStage6 );
	if( m_hIconStage7 != NULL )
		::DestroyIcon( m_hIconStage7 );
	if( m_hIconStage8 != NULL )
		::DestroyIcon( m_hIconStage8 );
	if( m_hIconStage9 != NULL )
		::DestroyIcon( m_hIconStage9 );
	if( m_hIconStage10 != NULL )
		::DestroyIcon( m_hIconStage10 );
}

void CPanelStage::OnOK() 
{
	// TODO: Add extra validation here

	//	CDialog::OnOK();
}

void CPanelStage::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

void CPanelStage::Message(LPCTSTR lpszMessage)
{
	CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_EDIT_Report) );
	if( peditLog == NULL )
		return;

	CString csMessage = CString(lpszMessage) + _T("\r\n");
	int len = peditLog->GetWindowTextLength();
	if( len > 10000 )
	{
		peditLog->SetWindowText("");
		len = peditLog->GetWindowTextLength();
	}
	peditLog->SetSel(len,len);
	peditLog->ReplaceSel( (LPCTSTR)csMessage );

	// 记录到日志文件
	unsigned long cbRet;
	char Buf[255];
	CString str;
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
	str.Format( "[%s],%s", csCurrentTime, lpszMessage );

	sprintf(Buf, "%s\r\n", str );
	WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);

	m_nLoopsLog++;
	if( m_nLoopsLog > 50000 )
	{
		CloseHandle( m_hFileLog );
		InitLog();
	}
}

void CPanelStage::Connect() 
{
	// TODO: Add your control notification handler code here
	ZeroMemory(&m_paramsSys, sizeof(MC600_Params_Sys));
	ZeroMemory(&m_paramsSpeed, sizeof(MC600_Params_Speed));
	ZeroMemory(&m_paramsPos, sizeof(MC600_Params_Pos));
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\Stage\\StageMC600.ini");
	char szBuf[255], szBuf2[255];
	// 通讯端口
	m_nPort = 1;
	m_nPort2 = 2;
	if(GetPrivateProfileString("StageMC600Cfg", "Port", "", szBuf, 15, path) != 0)
	{
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf2, strrchr(szBuf, ',')+1);	// szBuf2中是Vince端口值
			*strchr(szBuf, ',') = '\0';				// szBuf中是MC600端口值
			m_nPort	= atoi( szBuf );
			m_nPort2= atoi( szBuf2 );
		}
	}
	// 位移量基数
	if(GetPrivateProfileString("StageMC600Cfg", "Base", "10000", szBuf, 15, path) != 0)
		m_lStepBase = atol(szBuf);
	// 运行速度
	int nSpeed[8], i = 0;
	if(GetPrivateProfileString("StageMC600Cfg", "Speed", "", szBuf, 255, path) != 0)
	{
		do
		{
			strcpy(szBuf2, szBuf);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				nSpeed[i] = atoi(szBuf2);
				i++;
			}
		}while(strchr(szBuf, ',') != NULL);
		nSpeed[i] = atoi(szBuf);
	}
	// 中心位置
	if(GetPrivateProfileString("StageMC600Cfg", "Cali", "", szBuf, 255, path) != 0)
	{
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_fCali[0] = (float)atof(szBuf2);
		}
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_fCali[1] = (float)atof(szBuf2);
		}
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_fCali[2] = (float)atof(szBuf2);
		}
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_fCali[3] = (float)atof(szBuf2);
		}
		m_fCali[4] = (float)atof(szBuf);
	}
	m_fZ0Wd = 0.0;
	if(GetPrivateProfileString("StageMC600Cfg", "Z0WD", "", szBuf, 255, path) != 0)
		m_fZ0Wd = (float)atof(szBuf);

	// 软限位值
	if(GetPrivateProfileString("StageMC600Cfg", "Limits", "", szBuf, 255, path) != 0)
	{
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramZRes.fLimitNeg[0] = (float)atof(szBuf2);
		}
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramZRes.fLimitPos[0] = (float)atof(szBuf2);
		}
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramZRes.fLimitNeg[1] = (float)atof(szBuf2);
		}
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramZRes.fLimitPos[1] = (float)atof(szBuf2);
		}
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramZRes.fLimitNeg[2] = (float)atof(szBuf2);
		}
		m_paramZRes.fLimitPos[2] = (float)atof(szBuf);

		m_actualLimit[0] = m_paramZRes.fLimitNeg[0];	// X neg
		m_actualLimit[1] = m_paramZRes.fLimitPos[0];	// X pos
		m_actualLimit[2] = m_paramZRes.fLimitNeg[1];	// Y neg
		m_actualLimit[3] = m_paramZRes.fLimitPos[1];	// Y pos
		m_actualLimit[4] = m_paramZRes.fLimitNeg[2];	// Z neg
		m_actualLimit[5] = m_paramZRes.fLimitPos[2];	// Z pos
	}

	// 放气前的选项
	if(GetPrivateProfileString("StageMC600Cfg", "VentSel", "", szBuf, 255, path) != 0)
		m_nVentSel = atoi(szBuf);

	// R向是否使用光电开关
	if(GetPrivateProfileString("StageMC600Cfg", "OptR", "", szBuf, 255, path) != 0)
		m_bOptR = (BOOL)(atoi(szBuf));

	m_MC600.InitHWND( this->m_hWnd );
    m_bIsMC600Ready = m_MC600.Open(m_nPort);
	Localize();

	InitParam_SysParams();
	InitParam_Backlash( path );
	InitParam_ZRestraint_Fixed( path );
	InitParam_ZRestraint_User( path );

	if( m_nStageType > 2 )	// 5轴
	{
		m_Vince.InitHWND( this->m_hWnd );
		m_bIsVinceReady = m_Vince.Open(m_nPort2);
	}
}

void CPanelStage::InitParam_SysParams()
{
	m_paramsSys.fPitch[0]		= 0.5;
	m_paramsSys.fStepsRev[0]	= 1600;
	m_paramsSys.fRat[0]			= 180;
	m_paramsSys.fDistance[0]	= 256000;
	m_dEquivalent[0]			= m_paramsSys.fPitch[0] /m_paramsSys.fStepsRev[0];

	m_paramsSys.fPitch[1]		= 0.5;
	m_paramsSys.fStepsRev[1]	= 1600;
	m_paramsSys.fRat[1]			= 180;
	m_paramsSys.fDistance[1]	= 160000;
	m_dEquivalent[1]			= m_paramsSys.fPitch[1] /m_paramsSys.fStepsRev[1];

	m_paramsSys.fPitch[2]		= 0.416667;
	m_paramsSys.fStepsRev[2]	= 1600;
	m_paramsSys.fRat[2]			= 180;
	m_paramsSys.fDistance[2]	= 178000;
	m_dEquivalent[2]			= m_paramsSys.fPitch[2] /m_paramsSys.fStepsRev[2];

	m_paramsSys.fPitch[3]		= 30;
	m_paramsSys.fStepsRev[3]	= 1600;
	m_paramsSys.fRat[3]			= 6.2143;
	m_paramsSys.fDistance[3]	= 10218;
	m_dEquivalent[3]			= m_paramsSys.fPitch[3] /m_paramsSys.fStepsRev[3];
}

void CPanelStage::InitParam_Backlash( char* path )
{
	ZeroMemory(&m_paramBl, sizeof(MC600_Params_Bl));
	char szBuf[255], szBuf2[255];
	// 消间隙方向
	if(GetPrivateProfileString("StageMC600Cfg", "BlDir", "", szBuf, 255, path) != 0)
	{
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramBl.nDir[0] = (short)atoi(szBuf2);
		}
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramBl.nDir[1] = (short)atoi(szBuf2);
		}
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramBl.nDir[2] = (short)atoi(szBuf2);
		}
		m_paramBl.nDir[3] = (short)atoi(szBuf);
	}
	// 间隙量
	if(GetPrivateProfileString("StageMC600Cfg", "BlCount", "", szBuf, 255, path) != 0)
	{
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramBl.nCount[0] = (long)(atof(szBuf2) /m_dEquivalent[0]);
		}
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramBl.nCount[1] = (long)(atof(szBuf2) /m_dEquivalent[1]);
		}
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramBl.nCount[2] = (long)(atof(szBuf2) /m_dEquivalent[2]);
		}
		m_paramBl.nCount[3] = (long)(atof(szBuf) /m_dEquivalent[3]);
	}
	// 双击和跟踪时是否消间隙
	if(GetPrivateProfileString("StageMC600Cfg", "BlClick", "", szBuf, 255, path) != 0)
		m_bBlClick = (BOOL)(atoi(szBuf));
	if(GetPrivateProfileString("StageMC600Cfg", "BlTrack", "", szBuf, 255, path) != 0)
		m_bBlTrack = (BOOL)(atoi(szBuf));
	sprintf( szBuf, "Backlash X: %d, %d", m_paramBl.nDir[0], m_paramBl.nCount[0] );
	Message(szBuf);
	sprintf( szBuf, "Backlash Y: %d, %d", m_paramBl.nDir[1], m_paramBl.nCount[1] );
	Message(szBuf);
	sprintf( szBuf, "Backlash Z: %d, %d", m_paramBl.nDir[2], m_paramBl.nCount[2] );
	Message(szBuf);
	sprintf( szBuf, "Backlash R: %d, %d\r\n", m_paramBl.nDir[3], m_paramBl.nCount[3] );
	Message(szBuf);
	sprintf( szBuf, "Backlash: click=%d,track=%d", m_bBlClick, m_bBlTrack );
	Message(szBuf);
	for( short i=0; i<4; i++ )
		m_MC600.SetBacklashParams( i, m_paramBl.nDir[i], m_paramBl.nCount[i] );
}

void CPanelStage::InitParam_ZRestraint( char* path )
{
	int i;
	for( i=0; i<255; i++ )
	{
		for( int j=0; j<5; j++ )
		{
			m_fLimit[i][j] = 0.0;
		}
	}
    m_nZLimitTotal = GetPrivateProfileInt( "StageMC600Zres", "Total", 0, path );
	if( m_nZLimitTotal == 0 )
		return;
	char szBuf[255], szBuf2[255], szBufRootSection[20], szBufRootKey[20];
	sprintf(szBufRootSection, "StageMC600Zres");
	for( i=0; i<m_nZLimitTotal; i++ )
	{
		sprintf(szBufRootKey, "%d", i);
		if(GetPrivateProfileString(szBufRootSection, szBufRootKey, "", szBuf, 255, path) != 0)
		{
			strcpy(szBuf2, szBuf);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				m_fLimit[i][0] = (float)atof(szBuf2);
			}
			strcpy(szBuf2, szBuf);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				m_fLimit[i][1] = (float)atof(szBuf2);
			}
			strcpy(szBuf2, szBuf);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				m_fLimit[i][2] = (float)atof(szBuf2);
			}
			strcpy(szBuf2, szBuf);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				m_fLimit[i][3] = (float)atof(szBuf2);
			}
			m_fLimit[i][4] = (float)atof(szBuf);
		}
	}
}

void CPanelStage::InitParam_ZRestraint_Fixed( char* path )
{
	char szBuf[255], szBuf2[255], szBufRootSection[20], szBufRootKey[20];
	sprintf(szBufRootKey, "StageMC600Zres2");
	sprintf(szBufRootSection, "ResXY");
	if(GetPrivateProfileString(szBufRootKey, szBufRootSection, "", szBuf, 255, path) != 0)
	{
		// X/Y/Z 单向限制标定位置：X/Y在负向，Z标定=Z0wd -5
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramZRes.fCali[0] = (float)atof(szBuf2);
		}
		m_paramZRes.fCali[1] = (float)atof(szBuf);
	}
	sprintf(szBufRootSection, "ResR");
	if(GetPrivateProfileString(szBufRootKey, szBufRootSection, "", szBuf, 255, path) != 0)
	{
		// 探头顶端球半径
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramZRes.fRt = (float)atof(szBuf2);
		}
		// Rx0
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramZRes.fRx0 = (float)atof(szBuf2);
		}
		// Ry0
		m_paramZRes.fRy0 = (float)atof(szBuf);
	}
	sprintf(szBufRootSection, "ResZ");
	if(GetPrivateProfileString(szBufRootKey, szBufRootSection, "", szBuf, 255, path) != 0)
	{
		// 安全物距
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramZRes.fSafeWD = (float)atof(szBuf2);
		}
		// 二次电子探头下缘据物镜面Z下沉
		m_paramZRes.fZsink = (float)atof(szBuf);
	}
}

void CPanelStage::InitParam_ZRestraint_User( char* path )
{
	char szBuf[255], szBuf2[255], szBufRootSection[20], szBufRootKey[20];
	sprintf(szBufRootKey, "StageMC600Zres2");
	sprintf(szBufRootSection, "ResUser");
	if(GetPrivateProfileString(szBufRootKey, szBufRootSection, "", szBuf, 255, path) != 0)
	{
		// 样品半径Ryp
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_paramZRes.fRyp = (float)atof(szBuf2);
		}
		// 样品高度H
		m_paramZRes.fH = (float)atof(szBuf);
	}
	// Z向光电零面对应的工作距离Z0wd
	if(GetPrivateProfileString("StageMC600Cfg", "Z0WD", "", szBuf, 255, path) != 0)
		m_paramZRes.fZ0WD = (float)atof(szBuf);
	// Z标定 = Z0wd-safeWD-H
	m_paramZRes.fCali[2] = m_paramZRes.fZ0WD -m_paramZRes.fSafeWD -m_paramZRes.fH;
	// Zbr1 = Z16 = Z0wd-H-Z下沉
	m_paramZRes.fZbr[1] = m_paramZRes.fZ0WD -m_paramZRes.fH -m_paramZRes.fZsink;
	// Zbr0 = Z16 +H = Z0wd -Z下沉
	m_paramZRes.fZbr[0] = m_paramZRes.fZ0WD -m_paramZRes.fZsink;
}

void CPanelStage::SetZLimit()
{
	// Z 正向最大值要做保护
	// LimitZ+	= min（Zmax,Z标定）
//	m_paramZRes.fCali[2] = m_paramZRes.fZ0WD -m_paramZRes.fSafeWD -m_paramZRes.fH;
	if( m_paramZRes.fLimitPos[2] > m_paramZRes.fCali[2] )
		m_actualLimit[5]	= m_paramZRes.fCali[2];
	else
		m_actualLimit[5]	= m_paramZRes.fLimitPos[2];
	// 还要考虑校准时的偏移量
	m_actualLimit[5] -= m_fCali[2];	// Z pos

	if( m_bIsMC600Ready)
	{
		float lZLimitPos = (long)(m_actualLimit[5] /m_dEquivalent[2]);
		m_MC600.SetLimit(LimitPositive, 'Z', 'E', (float)lZLimitPos);
		CString str;
		str.Format( "Set Z pos limit to: %.2f mm", m_actualLimit[5] );
		Message(str);
	}
}

void CPanelStage::CoherenceCheck_Z()
{
	long lXLimit1, lXLimit2, lYLimit1, lYLimit2;
	CString str;
	if( m_lCurStep[2] *m_dEquivalent[2] < m_fLimit[0][0] )
	{
		lXLimit1 = -131200;
		lXLimit2 =  124800;
		lYLimit1 = -82500;
		lYLimit2 =  78500;
		str.Format( "Set XY limit to: -41.00, 39.00, -25.78, 24.53");
	}
	else
	{
		int i, nZ, nCurZ = (int)(m_lCurStep[2] *m_dEquivalent[2] *10);
		for( i=0; i<m_nZLimitTotal; i++ )
		{
			nZ = (int)(m_fLimit[i][0] *10 +0.1);
			if( nCurZ == nZ )
				break;
		}
		if( i == m_nZLimitTotal )
			return;
		lXLimit1 = (long)(m_fLimit[i][1] /m_dEquivalent[0]);
		lXLimit2 = (long)(m_fLimit[i][2] /m_dEquivalent[0]);
		lYLimit1 = (long)(m_fLimit[i][3] /m_dEquivalent[1]);
		lYLimit2 = (long)(m_fLimit[i][4] /m_dEquivalent[1]);
		str.Format( "Set XY limit to: %.2f, %.2f, %.2f, %.2f", m_fLimit[i][1], m_fLimit[i][2], m_fLimit[i][3], m_fLimit[i][4]);
	}
	m_MC600.SetLimit(LimitNegative, 'X', 'E', (float)lXLimit1);
	m_MC600.SetLimit(LimitPositive, 'X', 'E', (float)lXLimit2);
	m_MC600.SetLimit(LimitNegative, 'Y', 'E', (float)lYLimit1);
	m_MC600.SetLimit(LimitPositive, 'Y', 'E', (float)lYLimit2);
	Message(str);
}

void CPanelStage::CoherenceCheck_Z2( float& fXExp, float& fYExp, float fZExp, BOOL bAfter )
{
	float fZExpAct = fZExp +m_fCali[2];
	// 运动约束测算：两避让面
	// fExp为预期位置值pp
	// After为1时表示Z向已经到位，需要设置XY Limit
	// After为0时表示Z向还没移动，需要先判断XY当前位置值是否会在移动Z后的限制区域内
	// 如果是在限制区域内，需要先将XY移至限制区域外的边界值，然后再移动Z
	float fRx, fRy;
	fRx = max( m_paramZRes.fRyp, m_paramZRes.fRx0 );
	fRy = max( m_paramZRes.fRyp, m_paramZRes.fRy0 );

	// 计算变量R，是预期Z值、Zbr以及H的函数
	//  if(Z<=Zbr1,0,if(Z<=Zbr0, if(Z>=Rt+Zbr1,Rt,SQRT(Rt*Rt -(Zbr1-Z+Rt)*(Zbr1-Z+Rt))),
	//						if(Z>=Rt+Zbr0,Rt,SQRT(Rt*Rt -(Zbr0-Z+Rt)*(Zbr0-Z+Rt)))))
	double dRZ;
	if( fZExpAct <= m_paramZRes.fZbr[1] )
		dRZ = 0;
	else if( m_paramZRes.fZbr[1] < fZExpAct && fZExpAct < m_paramZRes.fZbr[1] +m_paramZRes.fRt )
		// SQRT(Rt*Rt -(Zbr1-Z+Rt)*(Zbr1-Z+Rt))
		dRZ = sqrt( m_paramZRes.fRt *m_paramZRes.fRt -
					(m_paramZRes.fZbr[1] -fZExpAct +m_paramZRes.fRt) *(m_paramZRes.fZbr[1] -fZExpAct +m_paramZRes.fRt) );
	else if( m_paramZRes.fZbr[0] < fZExpAct && fZExpAct < m_paramZRes.fZbr[0] +m_paramZRes.fRt )
		// SQRT(Rt*Rt -(Zbr0-Z+Rt)*(Zbr0-Z+Rt))
		dRZ = sqrt( m_paramZRes.fRt *m_paramZRes.fRt -
					(m_paramZRes.fZbr[0] -fZExpAct +m_paramZRes.fRt) *(m_paramZRes.fZbr[0] -fZExpAct +m_paramZRes.fRt) );
	else
		dRZ = m_paramZRes.fRt;

	// LimitX- = if(Z<Zbr1,Xmin,X标定-Rt+R(Z) + IF(Z<=Zbr0,Ryp,Rx))
	// LimitY- = if(Z<Zbr1,Ymin,Y标定-Rt+R(Z) + IF(Z<=Zbr0,Ryp,Ry))
	float fRX, fRY;
	if( fZExpAct <= m_paramZRes.fZbr[0] )
	{
		fRX = m_paramZRes.fRyp;
		fRY = m_paramZRes.fRyp;
	}
	else
	{
		fRX = fRx;
		fRY = fRy;
	}
	if( fZExpAct < m_paramZRes.fZbr[1] )
	{
		m_actualLimit[0] = m_paramZRes.fLimitNeg[0];
		m_actualLimit[2] = m_paramZRes.fLimitNeg[1];
	}
	else
	{
		m_actualLimit[0] = (float)(m_paramZRes.fCali[0] -m_paramZRes.fRt +dRZ +fRX);
		m_actualLimit[2] = (float)(m_paramZRes.fCali[1] -m_paramZRes.fRt +dRZ +fRY);
	}
	m_actualLimit[1] = m_paramZRes.fLimitPos[0];
	m_actualLimit[3] = m_paramZRes.fLimitPos[1];

	// 还要考虑校准时的偏移量
	m_actualLimit[0] -= m_fCali[0];	// X neg
	m_actualLimit[1] -= m_fCali[0];	// X pos
	m_actualLimit[2] -= m_fCali[1];	// Y neg
	m_actualLimit[3] -= m_fCali[1];	// Y pos

	CString str;
	if( bAfter )
	{
		m_MC600.SetLimit(LimitNegative, 'X', 'E', (float)(m_actualLimit[0] /m_dEquivalent[0]));
		m_MC600.SetLimit(LimitPositive, 'X', 'E', (float)(m_actualLimit[1] /m_dEquivalent[0]));
		m_MC600.SetLimit(LimitNegative, 'Y', 'E', (float)(m_actualLimit[2] /m_dEquivalent[1]));
		m_MC600.SetLimit(LimitPositive, 'Y', 'E', (float)(m_actualLimit[3] /m_dEquivalent[1]));
		str.Format( "Set XY limit to: %.2f, %.2f, %.2f, %.2f", m_actualLimit[0], m_actualLimit[1], m_actualLimit[2], m_actualLimit[3] );
		Message(str);
	}
	else
	{
		// 先判断XY当前位置值是否会在移动Z后的限制区域内
		if( fXExp < m_actualLimit[0] )
		{
			// 需要移动X向到限制边界
			fXExp = m_actualLimit[0];
		}
		if( fYExp < m_actualLimit[2] )
		{
			// 需要移动Y向到限制边界
			fYExp = m_actualLimit[2];
		}
		str.Format( "Need to move XY to: %.2f, %.2f", fXExp, fYExp );
		Message(str);
	}
}

void CPanelStage::SetStatus(BOOL bEnable)
{
	CWnd* pWndRect = GetDlgItem(IDC_STATIC_Rect);
	if( pWndRect == NULL )
		return;
	CRect rc, rcRef, rcRef3;
	GetWindowRect(&rc);
	GetDlgItem(IDC_STATIC_Rect)->GetWindowRect(&rcRef);
	GetDlgItem(IDC_STATIC_Rect3)->GetWindowRect(&rcRef3);
	rcRef3.bottom = rc.bottom;
	CWnd *pWnd = GetWindow(GW_CHILD);
	while(pWnd != NULL)
	{
		pWnd->GetWindowRect(&rc);
		if( PtInRect( rcRef, rc.CenterPoint() ) )
			pWnd->EnableWindow(bEnable);
		else if( PtInRect( rcRef3, rc.CenterPoint()))
			pWnd->EnableWindow(bEnable);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	};
	GetDlgItem(IDC_STATIC_Rect3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_Setup)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_Report)->EnableWindow(TRUE);	
}

void CPanelStage::SetStatus2( BOOL bEnable, BOOL bShow )
{
	CWnd* pWndRect = GetDlgItem(IDC_STATIC_Rect);
	if( pWndRect == NULL )
		return;
	CRect rc, rcRef;
	GetWindowRect(&rc);
	GetDlgItem(IDC_STATIC_Rect2)->GetWindowRect(&rcRef);
	CWnd *pWnd = GetWindow(GW_CHILD);
	while(pWnd != NULL)
	{
		pWnd->GetWindowRect(&rc);
		if( PtInRect( rcRef, rc.CenterPoint() ) )
		{
			if( bShow )
			{
				if( bEnable )
					pWnd->ShowWindow(SW_SHOW);
				else
					pWnd->ShowWindow(SW_HIDE);
			}
			else
				pWnd->EnableWindow(bEnable);
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	};
}

void CPanelStage::InitLog()
{
	// 设置日志
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';

	CString str, strTemp;
	CTime	timeCurrent = CTime::GetCurrentTime();
	strTemp = timeCurrent.Format("%Y%m%d%H%M");

	// 设置StageMC600日志文件
	str.Format( "%s\\Log\\StageMC600-%s.log", path, strTemp );
	if( m_hFileLog != NULL )
		CloseHandle( m_hFileLog );
	m_hFileLog	= NULL;
	m_hFileLog = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	m_nLoopsLog = 1;
	// 写StageMC600日志文件头
	if( m_hFileLog != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		str.Format( "\t%s\r\n", "Log StageMC600" );
		sprintf( Buf, "%s\r\n", str );
		WriteFile( m_hFileLog, Buf, strlen(Buf), &cbRet, NULL );
	}
}

void CPanelStage::SetLanguageID( WORD wID )
{
	m_wLanguageID = wID;
	char szBuf[20];
	if( wID == MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED))
		sprintf( szBuf, "zh_CN" );
	else
		sprintf( szBuf, "en_US" );
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\Stage\\StageMC600.ini");
	// 写入配置文件
	WritePrivateProfileString( "StageMC600Cfg", "Language", szBuf, path );
	Localize();
}

void CPanelStage::Localize()
{
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\Stage\\StageMC600.ini");
	char szLang[20];
	if(GetPrivateProfileString("StageMC600Cfg", "Language", "", szLang, 20, path) != 0)
	{
		exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		if( strcmp( szLang, "zh_CN" ) == 0)
			strcat(path, "\\Locale\\zh\\Stage_zh_CN.ini");
		else
			strcat(path, "\\Locale\\en\\Stage_en_US.ini");
		strcpy( m_cPathLocalize, path );
		char szBuf[255];
		// 界面元素
		if(GetPrivateProfileString("Label", "Cap", "", szBuf, 20, path) != 0)
			SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Zero", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_BUTTON_Zero)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Move", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_BUTTON_MoveTo)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Stop", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_BUTTON_Stop)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Relative", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_CHECK_Relative)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Setup", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_BUTTON_Setup)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Add", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_BUTTON_ListAdd)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Delete", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_BUTTON_ListDel)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Change", "", szBuf, 20, path) != 0)
			GetDlgItem(IDC_BUTTON_ListChange)->SetWindowText( szBuf );

		// 提示字符串
		if(GetPrivateProfileString("Msg", "MsgCap", "", szBuf, 50, path) != 0)
			m_strMsg[0] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgFindIndex", "", szBuf, 50, path) != 0)
			m_strMsg[1] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgCenter", "", szBuf, 50, path) != 0)
			m_strMsg[2] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgTSafe", "", szBuf, 50, path) != 0)
			m_strMsg[3] = szBuf;
	}
}

LRESULT CPanelStage::OnUpdateVinceValue(WPARAM wParam, LPARAM lParam)
{
	CString str;
	long lSteps = (long)lParam;
	switch( wParam )
	{
	case	0:
		m_bIsVinceReady = (BOOL)lParam;
		if(m_bIsVinceReady)
		{
			SetStatus2(TRUE);
			InitLog();
			str.Format("Open Vince COM %d successfully!", m_nPort2);
		}
		else
		{
			m_Vince.Close();
			SetStatus2(FALSE);
			m_bIsVinceReady = FALSE;
			str.Format("Open Vince COM %d Failed!",m_nPort2);
		}
		Message(str);
		break;
	}
	return 0;
}

LRESULT CPanelStage::OnUpdateMC600Value(WPARAM wParam, LPARAM lParam)
{
	CString str;
	long lSteps = (long)lParam;
	switch( wParam )
	{
	case	0:
		m_bIsMC600Ready = (BOOL)lParam;
		if(m_bIsMC600Ready)
		{
			SetStatus(TRUE);
			InitLog();
			str.Format("Open MC600 COM %d successfully!", m_nPort);
		}
		else
		{
			m_MC600.Close();
			SetStatus(FALSE);
			m_bIsMC600Ready = FALSE;
			str.Format("Open MC600 COM %d Failed!",m_nPort);
			::PostMessage( GetParent()->m_hWnd, WM_USER_MC600_UpdateValue, 0, 0 );
		}
		Message(str);
		break;
	case	1:
		// 读取到Params_Sys
		m_paramsSys = *((MC600_Params_Sys*)lParam);
		SyncParamsSys();
		break;
	case	2:
		// 读取到Params_Speed
		m_paramsSpeed = *((MC600_Params_Speed*)lParam);
		SyncParamsSpeed();
		break;
	case	3:
		// 读取到Params_Pos
		m_paramsPos = *((MC600_Params_Pos*)lParam);
		SyncParamsPos();
		FindIndex();
		break;
	case	111:
		str.Format( "X stop, step = %ld", (long)lParam );
		Message( str );
		GetCurStep(0);
		str.Format( "%.3f", m_lCurStep[0] *m_dEquivalent[0] );
		GetDlgItem(IDC_EDIT_X_UserDisp)->SetWindowText(str);
		break;
	case	112:
		str.Format( "Y stop, step = %ld", (long)lParam );
		Message( str );
		GetCurStep(1);
		str.Format( "%.3f", m_lCurStep[1] *m_dEquivalent[1] );
		GetDlgItem(IDC_EDIT_Y_UserDisp)->SetWindowText(str);
		break;
	case	113:
		{
			str.Format( "Z stop, step = %ld", (long)lParam );
			Message( str );
			GetCurStep(2);
			float fX, fY;
			CoherenceCheck_Z2( fX, fY, m_lCurStep[2] *m_dEquivalent[2], TRUE );
			if( m_bWD )
				str.Format( "%.1f", m_fZ0Wd -m_lCurStep[2] *m_dEquivalent[2] );
			else
				str.Format( "%.3f", m_lCurStep[2] *m_dEquivalent[2] );
			GetDlgItem(IDC_EDIT_Z_UserDisp)->SetWindowText(str);
		}
		break;
	case	114:
		str.Format( "R stop, step = %ld", (long)lParam );
		Message( str );
		GetCurStep(3);
		str.Format( "%.1f", m_lCurStep[3] *m_dEquivalent[3] );
		GetDlgItem(IDC_EDIT_R_UserDisp)->SetWindowText(str);
		break;
	}
	return 0;
}

void CPanelStage::SyncParamsSys()
{
	CString str;
	str.Format("Pitch X = %.6f mm", m_paramsSys.fPitch[0]);
	Message(str);
	str.Format("StepsRev X = %.0f PP/r", m_paramsSys.fStepsRev[0]);
	Message(str);
	str.Format("DriveRat X = %.0f", m_paramsSys.fRat[0]);
	Message(str);
	m_dEquivalent[0] = m_paramsSys.fPitch[0] /m_paramsSys.fStepsRev[0];
	str.Format("Distance X = %.2f mm\r\n", m_paramsSys.fDistance[0] *m_dEquivalent[0]);
	Message(str);

	str.Format("Pitch Y = %.6f mm", m_paramsSys.fPitch[1]);
	Message(str);
	str.Format("StepsRev Y = %.0f PP/r", m_paramsSys.fStepsRev[1]);
	Message(str);
	str.Format("DriveRat Y = %.0f", m_paramsSys.fRat[1]);
	Message(str);
	m_dEquivalent[1] = m_paramsSys.fPitch[1] /m_paramsSys.fStepsRev[1];
	str.Format("Distance Y = %.2f mm\r\n", m_paramsSys.fDistance[1] *m_dEquivalent[1]);
	Message(str);

	str.Format("Pitch Z = %.6f mm", m_paramsSys.fPitch[2]);
	Message(str);
	str.Format("StepsRev Z = %.0f PP/r", m_paramsSys.fStepsRev[2]);
	Message(str);
	str.Format("DriveRat Z = %.0f", m_paramsSys.fRat[2]);
	Message(str);
	m_dEquivalent[2] = m_paramsSys.fPitch[2] /m_paramsSys.fStepsRev[2];
	str.Format("Distance Z = %.2f mm\r\n", m_paramsSys.fDistance[2] *m_dEquivalent[2]);
	Message(str);

	str.Format("Pitch R = %.6f mm", m_paramsSys.fPitch[3]);
	Message(str);
	str.Format("StepsRev R = %.0f PP/r", m_paramsSys.fStepsRev[3]);
	Message(str);
	str.Format("DriveRat R = %.3f", m_paramsSys.fRat[3]);
	Message(str);
	m_dEquivalent[3] = 365 /m_paramsSys.fDistance[3];
	str.Format("Distance R = %.2f\r\n", m_paramsSys.fDistance[3] *m_dEquivalent[3]);
	Message(str);
}

void CPanelStage::SyncParamsSpeed()
{
	CString str;
	str.Format("X Speed = %.0f pp", m_paramsSpeed.fSpeed[0]);
	Message(str);
	str.Format("X SpeedInit = %.0f pp", m_paramsSpeed.fSpeedInit[0]);
	Message(str);
	str.Format("X SpeedAcc = %.0f pp", m_paramsSpeed.fSpeedAcc[0]);
	Message(str);
	str.Format("X Speed Home = %.0f pp\r\n", m_paramsSpeed.fSpeedHome[0]);
	Message(str);

	str.Format("Y Speed = %.0f pp", m_paramsSpeed.fSpeed[1]);
	Message(str);
	str.Format("Y SpeedInit = %.0f pp", m_paramsSpeed.fSpeedInit[1]);
	Message(str);
	str.Format("Y SpeedAcc = %.0f pp", m_paramsSpeed.fSpeedAcc[1]);
	Message(str);
	str.Format("Y Speed Home = %.0f pp\r\n", m_paramsSpeed.fSpeedHome[1]);
	Message(str);

	str.Format("Z Speed = %.0f pp", m_paramsSpeed.fSpeed[2]);
	Message(str);
	str.Format("Z SpeedInit = %.0f pp", m_paramsSpeed.fSpeedInit[2]);
	Message(str);
	str.Format("Z SpeedAcc = %.0f pp", m_paramsSpeed.fSpeedAcc[2]);
	Message(str);
	str.Format("Z Speed Home = %.0f pp\r\n", m_paramsSpeed.fSpeedHome[2]);
	Message(str);

	str.Format("R Speed = %.0f pp", m_paramsSpeed.fSpeed[3]);
	Message(str);
	str.Format("R SpeedInit = %.0f pp", m_paramsSpeed.fSpeedInit[3]);
	Message(str);
	str.Format("R SpeedAcc = %.0f pp", m_paramsSpeed.fSpeedAcc[3]);
	Message(str);
	str.Format("R Speed Home = %.0f pp\r\n", m_paramsSpeed.fSpeedHome[3]);
	Message(str);
}

void CPanelStage::SyncParamsPos()
{
	CString str;
	str.Format("X pos = %.3f mm", m_paramsPos.fSteps[0] *m_dEquivalent[0]);
	Message(str);
	str.Format("Y pos = %.3f mm", m_paramsPos.fSteps[1] *m_dEquivalent[1]);
	Message(str);
	str.Format("Z pos = %.3f mm", m_paramsPos.fSteps[2] *m_dEquivalent[2]);
	Message(str);
	str.Format("R pos = %.1f deg\r\n", m_paramsPos.fSteps[3] *m_dEquivalent[3]);
	Message(str);
}

void CPanelStage::FindIndex()
{
	if( m_bIsMoving )
		return;
	// 先回机械原点，再回扫描中心，并定义其为用户原点
	if( MessageBox( m_strMsg[1], m_strMsg[0], MB_OKCANCEL ) == IDOK )
	{
		// 先回机械原点
		Message("Start FindIndex");
		Home(11);
	}
	else
	{
		::PostMessage( GetParent()->m_hWnd, WM_USER_MC600_UpdateValue, 0, 0 );
		for( int i=0; i<5; i++ )
			GetCurStep(i);
		SetZLimit();
	}
}

void CPanelStage::MoveSteps()
{
	if( m_nProcess < 0 )
	{
		// 单轴移动已完成
		m_bIsMoving = FALSE;
		return;
	}
	else if( m_nProcess > 20 )
	{
		// 单轴HOME已完成，光电开关的电源应关闭
		if( m_nProcess < 24 || (m_nProcess == 24 && m_bOptR) )
			::PostMessage( GetParent()->m_hWnd, WM_USER_MC600_UpdateValue, 0, 0 );
		m_nProcess = -1;
		m_bIsMoving = FALSE;
		return;
	}
	if( m_bIsMoving && m_nProcess > -1 )
	{
		if( m_nMoveSteps == 0 )
		{
			// Home & Origin：先T
			// 正在进行第零步：移动T轴
			if( m_nTimer[4] == 0 )
			{
// 				// 先将Tilt回正
// 				m_Vince.SetOrg(1);
// 				m_Vince.Move( 1, 1, m_fCali[4] /m_dEquivalent[4] );
				// 开始进行第一步：移动XY轴
				m_nMoveSteps = 1;
				if( m_nProcess == 1 || m_nProcess == 11 )	// Home
				{
					m_MC600.GoHome('X');
					m_MC600.GoHome('Y');
				}
				else if( m_nProcess == 2 )	// Origin
				{
					m_MC600.GoOrigion('X');
					m_MC600.GoOrigion('Y');
				}
				else
				{
					m_MC600.MoveA('X', (long)(m_fGoal[0] /m_dEquivalent[0]));
					m_MC600.MoveA('Y', (long)(m_fGoal[1] /m_dEquivalent[1]));
				}
				m_nTimer[0] = SetTimer( 1, nStageMC600_Elapse, NULL );
				m_nTimer[1] = SetTimer( 2, nStageMC600_Elapse, NULL );
			}
		}
		else if( m_nMoveSteps == 1 )
		{
			// Home & Origin：先XY后Z
			// 正在进行第一步：移动XY轴
			if( m_nTimer[0] == 0 && m_nTimer[1] == 0 )
			{
				// 开始进行第二步：移动Z轴
				m_nMoveSteps = 2;
				if( m_nProcess == 1 || m_nProcess == 11 )	// Home
				{
					m_MC600.GoHome('Z');
				}
				else if( m_nProcess == 2 )	// Origin
				{
					m_MC600.GoOrigion('Z');
				}
				else
				{
					m_MC600.MoveA('Z', (long)(m_fGoal[2] /m_dEquivalent[2]));
				}
				m_nTimer[2] = SetTimer( 3, nStageMC600_Elapse, NULL );
			}
		}
		else if( m_nMoveSteps == 2 )
		{
			// Home & Origin：先XY后Z
			// 正在进行第二步：移动Z轴
			if( m_nTimer[2] == 0 )
			{
				float fX, fY;
				CoherenceCheck_Z2( fX, fY, m_lCurStep[2] *m_dEquivalent[2], TRUE );
				if( m_bOptR )
				{
					// 开始进行第三步：移动R轴
					m_nMoveSteps = 3;
					if( m_nProcess == 1 || m_nProcess == 11 )	// Home
					{
						m_MC600.GoHome('T');
					}
					else if( m_nProcess == 2 )	// Origin
					{
						m_MC600.GoOrigion('T');
					}
					else
					{
						m_MC600.MoveA('T', (long)(m_fGoal[3] /m_dEquivalent[3]));
					}
					m_nTimer[3] = SetTimer(4, nStageMC600_Elapse, NULL);
				}
				else
				{
					// 无R向光电开关
					m_nMoveSteps = 3;
					if( m_nProcess == 2 )	// Origin
					{
						m_MC600.GoOrigion('T');
					}
					else if( m_nProcess != 1 && m_nProcess != 11 )
					{
						m_MC600.MoveA('T', (long)(m_fGoal[3] /m_dEquivalent[3]));
					}
					m_nTimer[3] = SetTimer(4, nStageMC600_Elapse, NULL);
				}
			}
		}
		else if( m_nMoveSteps == 3 )
		{
			// 正在进行第三步：移动R轴
			if( m_nTimer[3] == 0 )
			{
				if( m_nProcess == 11 )
				{
					// 机械原点Home已找到，关闭光电开关
					::PostMessage( GetParent()->m_hWnd, WM_USER_MC600_UpdateValue, 0, 0 );
					m_bIsMoving = FALSE;
					if( m_nStageType > 2 && m_bIsVinceReady )
					{
						// 要启动5轴移动，找扫描中心
						// 先将Tilt回正
//						m_Vince.SetOrg(1);
//						m_Vince.Move( 1, 1, m_fCali[4] /m_dEquivalent[4] );
						Move5Axis(m_nProcess+1, m_fCali[0], m_fCali[1], m_fCali[2], m_fCali[3], m_fCali[4]);
					}
					else
					{
						// 要启动4轴移动，找扫描中心
						Move4Axis(m_nProcess+1, m_fCali[0], m_fCali[1], m_fCali[2], m_fCali[3]);
					}
				}
				else if( m_nProcess == 12 )
				{
					// 已到扫描中心，定义当前位置为用户原点
					SetUserOrigin();
					Message("Finish FindIndex");
					m_nProcess = -1;
					m_bIsMoving = FALSE;
					m_bRelative = m_bRelativeBak;
					SetZLimit();
				}
				else
				{
					// 4轴运动都已结束，给出提示
					m_nMoveSteps = -1;
					if( m_nProcess == 1 )
					{
						Message("Finish Go Home\r\n");
						// 机械原点Home已找到，关闭光电开关
						::PostMessage( GetParent()->m_hWnd, WM_USER_MC600_UpdateValue, 0, 0 );
						m_Vince.SetOrg(1);
					}
					else if( m_nProcess == 2 )
					{
						SetUserOrigin();
						Message("Finish Go Origin\r\n");
					}
					else
						Message("Finish Move To\r\n");
					m_nProcess = -1;
					m_bIsMoving = FALSE;
					m_bRelative = m_bRelativeBak;
				}
			}
		}
	}
}

void CPanelStage::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == 1 )
	{
		long lStep = m_lCurStep[0];
		GetCurStep(0);
		if( labs(lStep -m_lCurStep[0]) < 1 )
		{
			m_nTimerCounts[0]++;
			if( m_nTimerCounts[0] > 3 )
			{
				m_nTimerCounts[0] = 0;
				Message("KillTimer X");
				KillTimer(1);
				m_nTimer[0] = 0;
				GetDlgItem(IDC_EDIT_X_CurStep)->Invalidate();
				MoveSteps();
			}
		}
	}
	else if( nIDEvent == 2 )
	{
		long lStep = m_lCurStep[1];
		GetCurStep(1);
		if( labs(lStep -m_lCurStep[1]) < 1 )
		{
			m_nTimerCounts[1]++;
			if( m_nTimerCounts[1] > 3 )
			{
				m_nTimerCounts[1] = 0;
				Message("KillTimer Y");
				KillTimer(2);
				m_nTimer[1] = 0;
				GetDlgItem(IDC_EDIT_Y_CurStep)->Invalidate();
				MoveSteps();
			}
		}
	}
	else if( nIDEvent == 3 )
	{
		long lStep = m_lCurStep[2];
		GetCurStep(2);
		if( labs(lStep -m_lCurStep[2]) < 1 )
		{
			m_nTimerCounts[2]++;
			if( m_nTimerCounts[2] > 3 )
			{
				m_nTimerCounts[2] = 0;
				Message("KillTimer Z");
				KillTimer(3);
				m_nTimer[2] = 0;
				GetDlgItem(IDC_EDIT_Z_CurStep)->Invalidate();
				MoveSteps();
			}
		}
	}
	else if( nIDEvent == 4 )
	{
		long lStep = m_lCurStep[3];
		GetCurStep(3);
		if( labs(lStep -m_lCurStep[3]) < 1 )
		{
			m_nTimerCounts[3]++;
			if( m_nTimerCounts[3] > 3 )
			{
				m_nTimerCounts[3] = 0;
				Message("KillTimer R");
				KillTimer(4);
				m_nTimer[3] = 0;
				GetDlgItem(IDC_EDIT_R_CurStep)->Invalidate();
				MoveSteps();
			}
		}
	}
	else if( nIDEvent == 5 )
	{
		// T轴
		GetCurStep(4);
		Vince_Param_Status sp = GetStatus(4);
		if( sp.status.stp )
		{
			m_nTimerCounts[4]++;
			if( m_nTimerCounts[4] > 3 )
			{
				m_nTimerCounts[4] = 0;
				Message("KillTimer T");
				KillTimer(5);
				m_nTimer[4] = 0;
				GetDlgItem(IDC_EDIT_T_CurStep)->Invalidate();
				MoveSteps();
				Sleep(100);
				GetCurStep(4);
				CString str;
				str.Format( "%.3f", m_lCurStep[4] *m_dEquivalent[4] );
				GetDlgItem(IDC_EDIT_T_UserDisp)->SetWindowText(str);
			}
		}
	}
	else if( nIDEvent == 11 )
	{
		// 要进行HOME操作，等待光电开关开并生效
		if( m_bPEswitch )
		{
			Message("J15 PE_switch ON");
			KillTimer(11);
			if( m_nProcess < 20 )
			{
				// 回原点（即光电归零）
				if( m_nStageType > 2 && m_bIsVinceReady )
				{
					// 4轴HOME
					Message( "Go Home T/X/Y/Z/R" );
					m_nMoveSteps = 0;
					m_Vince.BackZero(1,1);
					m_nTimer[4] = SetTimer( 5, nStageMC600_Elapse, NULL );
				}
				else
				{
					// 4轴HOME
					Message( "Go Home X/Y/Z/R" );
					m_nMoveSteps = 1;
					m_MC600.GoHome('X');
					m_MC600.GoHome('Y');
					m_nTimer[0] = SetTimer(1, nStageMC600_Elapse, NULL);
					m_nTimer[1] = SetTimer(2, nStageMC600_Elapse, NULL);
				}
			}
			else
			{
				// 单轴HOME
				switch( m_nProcess )
				{
				case	21:
					m_MC600.GoHome('X');
					m_nTimer[0] = SetTimer( 1, nStageMC600_Elapse, NULL );
					break;
				case	22:
					m_MC600.GoHome('Y');
					m_nTimer[1] = SetTimer( 2, nStageMC600_Elapse, NULL );
					break;
				case	23:
					m_MC600.GoHome('Z');
					m_nTimer[2] = SetTimer( 3, nStageMC600_Elapse, NULL );
					break;
				case	24:
					m_MC600.GoHome('T');
					m_nTimer[3] = SetTimer( 4, nStageMC600_Elapse, NULL );
					break;
				case	25:
					m_Vince.BackZero(1,1);
					m_nTimer[4] = SetTimer( 5, nStageMC600_Elapse, NULL );
					break;
				}
			}
			m_bIsMoving = TRUE;
		}
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CPanelStage::QueryMoving()
{
	if( m_bIsMoving || (m_nProcess > -1) )
		return TRUE;
	return FALSE;
}

void CPanelStage::GetCurStep( short anxi )
{
	unsigned char ucAxis;
	switch( anxi )
	{
	case	0:
		ucAxis = 'X';
		break;
	case	1:
		ucAxis = 'Y';
		break;
	case	2:
		ucAxis = 'Z';
		break;
	case	3:
		ucAxis = 'T';
		break;
	}
	long lStep = 0;
	if( anxi < 4 )
		lStep = (long)(m_MC600.GetPosition(ucAxis));
	else
		lStep = m_Vince.GetPosition(1);

	CString str;
	if( anxi == 0 )
	{
		str.Format( "%-06.3f, %-6ld", lStep *m_dEquivalent[0], lStep );
		GetDlgItem(IDC_EDIT_X_CurStep)->SetWindowText(str);
		m_lCurStep[0] = lStep;
	}
	else if( anxi == 1 )
	{
		str.Format( "%-6.3f, %-6ld", lStep *m_dEquivalent[1], lStep );
		GetDlgItem(IDC_EDIT_Y_CurStep)->SetWindowText(str);
		m_lCurStep[1] = lStep;
	}
	else if( anxi == 2 )
	{
		str.Format( "%-6.3f, %-6ld", lStep *m_dEquivalent[2], lStep );
		GetDlgItem(IDC_EDIT_Z_CurStep)->SetWindowText(str);
		m_lCurStep[2] = lStep;
	}
	else if( anxi == 3 )
	{
		str.Format( "%-4.1f, %-6ld", lStep *m_dEquivalent[3], lStep );
		GetDlgItem(IDC_EDIT_R_CurStep)->SetWindowText(str);
		m_lCurStep[3] = lStep;
	}
	else if( anxi == 4 )
	{
		str.Format( "%-5.2f, %-5ld", lStep *m_dEquivalent[4], lStep );
		GetDlgItem(IDC_EDIT_T_CurStep)->SetWindowText(str);
		m_lCurStep[4] = lStep;
	}
}

void CPanelStage::SetTrackParam( int nSpeedX, int nSpeedY )
{
	// 跟踪：设置速度，并进行运动
	// 根据速度值正负判断运动方向
	// 如果与当前运动方向不一致，要停止运动、设置速度(绝对值)、开始运动
	// 如果与当前运动方向一致，只需设置速度(绝对值)

	CString str;
	// 按放大倍数分为几档速度
	int nScale = 1;
	if( m_dMag < 200 )
		nScale = 50;
	else if( m_dMag < 2000 )
		nScale = 20;
	else if( m_dMag < 20000 )
		nScale = 10;
	else
		nScale = 5;
	// X向
	int nX = abs(nSpeedX);
	nX = (nX / 50) *10;	// 将速度分档
	if( nX > lSpeedTrackMax )
		nX = lSpeedTrackMax;
	int nXdir = m_MC600.GetDir('X');
	if( abs(nSpeedX) < 5 || nX == 0 )	// 与跟踪点之间差值较小，停止运动
	{
		if( nXdir != 0 )
		{
			m_MC600.StopMove('X');
			Message("X 方向停止运动");
		}
	}
	else
	{
		BOOL bIsSame = FALSE;
		if( m_lSpeed[0] == nX )
			bIsSame = TRUE;
		m_lSpeed[0] = nX;
		if( (nXdir < 0 && nSpeedX < 0) || (nXdir > 0 && nSpeedX > 0) )
		{
			// 同方向，只设置速度
			if( !bIsSame )
			{
				m_MC600.SetSpeed( Speed, 'X', m_lSpeed[0] );
				str.Format( "Set Speed X = %d", m_lSpeed[0] );
				Message(str);
			}
		}
		else
		{
			if( nXdir != 0 )
			{
				m_MC600.StopMove('X');
				Sleep(10);
				Message("X 原方向停止运动，准备反方向");
			}
			if( !bIsSame )
			{
				m_MC600.SetSpeed( Speed, 'X', m_lSpeed[0] );
				str.Format( "Set Speed X = %d", m_lSpeed[0] );
				Message(str);
			}
			if( nSpeedX < 0 )
				m_MC600.MoveA('X', -150000, m_bBlTrack);
			else
				m_MC600.MoveA('X', 150000, m_bBlTrack);
			m_nTimer[0] = SetTimer( 1, nStageMC600_Elapse, NULL );
			Message("X 反方向开始运动");
		}
	}
	// Y向
	int nY = abs(nSpeedY);
	nY = (nY / 50) *10;	// 将速度分档
	if( nY > lSpeedTrackMax )
		nY = lSpeedTrackMax;
	int nYdir = m_MC600.GetDir('Y');
	if( abs(nSpeedY) < 5 || nY == 0 )	// 与跟踪点之间差值较小，停止运动
	{
		if( nYdir != 0 )
		{
			m_MC600.StopMove('Y');
			Message("Y 方向停止运动");
		}
	}
	else
	{
		BOOL bIsSame = FALSE;
		if( m_lSpeed[1] == nY )
			bIsSame = TRUE;
		m_lSpeed[1] = nY;
		if( (nYdir < 0 && nSpeedY < 0) || (nYdir > 0 && nSpeedY > 0) )
		{
			// 同方向，只设置速度
			if( !bIsSame )
			{
				m_MC600.SetSpeed( Speed, 'Y', m_lSpeed[1] );
				str.Format( "Set Speed Y = %d", m_lSpeed[1] );
				Message(str);
			}
		}
		else
		{
			if( nYdir != 0 )
			{
				m_MC600.StopMove('Y');
				Sleep(10);
				Message("Y 原方向停止运动，准备反方向");
			}
			if( !bIsSame )
			{
				m_MC600.SetSpeed( Speed, 'Y', m_lSpeed[1] );
				str.Format( "Set Speed Y = %d", m_lSpeed[1] );
				Message(str);
			}
			if( nSpeedY < 0 )
				m_MC600.MoveA('Y', -150000, m_bBlTrack);
			else
				m_MC600.MoveA('Y', 150000, m_bBlTrack);
			m_nTimer[1] = SetTimer( 2, nStageMC600_Elapse, NULL );
			Message("Y 反方向开始运动");
		}
	}
}

void CPanelStage::StopTrack()
{
	Stop();
	RestoreSpeed();
}

void CPanelStage::RestoreSpeed()
{
	// 恢复速度
	m_lSpeed[0] = nSpeedXY;
	m_lSpeed[1] = nSpeedXY;
	m_MC600.SetSpeed( Speed, 'X', m_lSpeed[0] );
	m_MC600.SetSpeed( Speed, 'Y', m_lSpeed[1] );
	// 常速度是和初速度以及归零速度相关联的，所以都要恢复
	m_MC600.SetSpeed( SpeedInit, 'X', nSpeedInitXY );
	m_MC600.SetSpeed( SpeedInit, 'Y', nSpeedInitXY );
	m_MC600.SetSpeed( SpeedHome, 'X', nSpeedHomeXY );
	m_MC600.SetSpeed( SpeedHome, 'Y', nSpeedHomeXY );
}

void CPanelStage::GetSpeed( unsigned char ucAxis )
{
	long lSpeed = (long)(m_MC600.GetSpeed( Speed, ucAxis ));
	CString str;
	switch( ucAxis )
	{
	case	'X':
		str.Format( "X current move speed = %ld", lSpeed );
		m_lSpeed[0] = lSpeed;
		break;
	case	'Y':
		str.Format( "Y current move speed = %ld", lSpeed );
		m_lSpeed[1] = lSpeed;
		break;
	case	'Z':
		str.Format( "Z current move speed = %ld", lSpeed );
		m_lSpeed[2] = lSpeed;
		break;
	case	'T':
		str.Format( "R current move speed = %ld", lSpeed );
		m_lSpeed[3] = lSpeed;
		break;
	}
	Message(str);
}

void CPanelStage::GetSpeedAcc( unsigned char ucAxis )
{
	long lSpeed = (long)(m_MC600.GetSpeed( SpeedAcc, ucAxis ));
	CString str;
	switch( ucAxis )
	{
	case	'X':
		str.Format( "X current acc speed = %ld", lSpeed );
		m_lSpeed2[0] = lSpeed;
		break;
	case	'Y':
		str.Format( "Y current acc speed = %ld", lSpeed );
		m_lSpeed2[1] = lSpeed;
		break;
	case	'Z':
		str.Format( "R current acc speed = %ld", lSpeed );
		m_lSpeed2[2] = lSpeed;
		break;
	case	'T':
		str.Format( "R current acc speed = %ld", lSpeed );
		m_lSpeed2[3] = lSpeed;
		break;
	}
	Message(str);
}

void CPanelStage::GetSpeedInit( unsigned char ucAxis )
{
	long lSpeed = (long)(m_MC600.GetSpeed(SpeedInit, ucAxis ));
	CString str;
	switch( ucAxis )
	{
	case	'X':
		str.Format( "X init speed = %ld", lSpeed );
		m_lSpeed3[0]= lSpeed;
		break;
	case	'Y':
		str.Format( "Y init speed = %ld", lSpeed );
		m_lSpeed3[1] = lSpeed;
		break;
	case	'Z':
		str.Format( "Z init speed = %ld", lSpeed );
		m_lSpeed3[2] = lSpeed;
		break;
	case	'T':
		str.Format( "R init speed = %ld", lSpeed );
		m_lSpeed3[3] = lSpeed;
		break;
	}
	Message(str);
}

void CPanelStage::MoveToCenter( int nCenterX, int nCenterY, int nX, int nY )
{
	// 像素距离
// 	double dLengthX = 1.0 *abs( nX -nCenterX ) *m_sizeReso.cx /1024;
// 	double dLengthY = 1.0 *abs( nY -nCenterY ) *m_sizeReso.cy /768;
	double dLengthX = abs( nX -nCenterX );
	double dLengthY = abs( nY -nCenterY );
	// 实际步数
	int nScaleX = 0;
	CalculateStageUScale( 0, dLengthX, nScaleX );
	int nScaleY = 0;
	CalculateStageUScale( 1, dLengthY, nScaleY );

	long lMoveStepX, lMoveStepY;
	if( nX > nCenterX )
		lMoveStepX = 0 -nScaleX;
	else
		lMoveStepX = nScaleX;
	if( nY > nCenterY )
		lMoveStepY = 0 -nScaleY;
	else
		lMoveStepY = nScaleY;
	CString str;
	str.Format( "move to center: %ld, %ld", lMoveStepX, lMoveStepY );
	Message(str);
	m_MC600.MoveR('X', lMoveStepX, m_bBlClick);
	m_MC600.MoveR('Y', lMoveStepY, m_bBlClick);
	m_nTimer[0] = SetTimer( 1, nStageMC600_Elapse, NULL );
	m_nTimer[1] = SetTimer( 2, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONXMoveMinus() 
{
	if( m_bIsMoving )
		return;
	// 像素距离
	int nLengthX = m_sizeReso.cx /4;
	// 实际步数
	int nScaleX = 0;
	CalculateStageUScale( 0, nLengthX, nScaleX );

	long lMoveStep = 0 -nScaleX;
	CString str;
	str.Format( "X move relative %ld steps", lMoveStep );
	Message(str);
	m_bIsMoving = TRUE;
	m_MC600.MoveR('X', lMoveStep);
	m_nTimer[0] = SetTimer( 1, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONXMovePlus() 
{
	if( m_bIsMoving )
		return;
	// 像素距离
	int nLengthX = m_sizeReso.cx /4;
	// 实际步数
	int nScaleX = 0;
	CalculateStageUScale( 0, nLengthX, nScaleX );

	long lMoveStep = nScaleX;
	CString str;
	str.Format( "X move relative %ld steps", lMoveStep );
	Message(str);
	m_bIsMoving = TRUE;
	m_MC600.MoveR('X', lMoveStep);
	m_nTimer[0] = SetTimer( 1, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONYMoveMinus() 
{
	if( m_bIsMoving )
		return;
	// 像素距离
	int nLengthY = m_sizeReso.cy /4;
	// 实际步数
	int nScaleY = 0;
	CalculateStageUScale( 1, nLengthY, nScaleY );

	long lMoveStep = 0 -nScaleY;
	CString str;
	str.Format( "Y move relative %ld steps", lMoveStep );
	Message(str);
	m_bIsMoving = TRUE;
	m_MC600.MoveR('Y', lMoveStep);
	m_nTimer[1] = SetTimer( 2, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONYMovePlus() 
{
	if( m_bIsMoving )
		return;
	// 像素距离
	int nLengthY = m_sizeReso.cy /4;
	// 实际步数
	int nScaleY = 0;
	CalculateStageUScale( 1, nLengthY, nScaleY );

	long lMoveStep = nScaleY;
	CString str;
	str.Format( "Y move relative %ld steps", lMoveStep );
	Message(str);
	m_bIsMoving = TRUE;
	m_MC600.MoveR('Y', lMoveStep);
	m_nTimer[1] = SetTimer( 2, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONZMoveMinus() 
{
	// 单步走每次0.5mm
	MoveZ( -0.5, TRUE );
}

void CPanelStage::OnBUTTONZMovePlus() 
{
	// 单步走每次0.5mm
	MoveZ( 0.5, TRUE );
}

void CPanelStage::OnBUTTONRMoveMinus() 
{
	if( m_bIsMoving )
		return;
	// 单步走每次5度
	// 实际步数
	long lMoveStep = 0 -(int)(5/m_dEquivalent[3]);
	CString str;
	str.Format( "R move relative %ld steps", lMoveStep );
	Message(str);
	m_bIsMoving = TRUE;
	m_MC600.MoveR('T', lMoveStep);
	m_nTimer[3] = SetTimer( 4, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONRMovePlus() 
{
	if( m_bIsMoving )
		return;
	// 单步走每次5度
	// 实际步数
	long lMoveStep = (int)(5/m_dEquivalent[3]);
	CString str;
	str.Format( "R move relative %ld steps", lMoveStep );
	Message(str);
	m_bIsMoving = TRUE;
	m_MC600.MoveR('T', lMoveStep);
	m_nTimer[3] = SetTimer( 4, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONXMoveTo() 
{
	if( m_bIsMoving )
		return;
	Message( "Move X" );
	Move(0);
}

void CPanelStage::OnBUTTONYMoveTo() 
{
	if( m_bIsMoving )
		return;
	Message( "Move Y" );
	Move(1);
}

void CPanelStage::OnBUTTONZMoveTo() 
{
	if( m_bIsMoving )
		return;
	Message( "Move Z" );
	Move(2);
}

void CPanelStage::OnBUTTONRMoveTo() 
{
	if( m_bIsMoving )
		return;
	Message( "Move R" );
	Move(3);
}

void CPanelStage::OnBUTTONMoveTo() 
{
	if( m_bIsMoving )
		return;
	CString strX, strY, strZ, strR, strT;
	GetDlgItem(IDC_EDIT_X_UserDisp)->GetWindowText(strX);
	GetDlgItem(IDC_EDIT_Y_UserDisp)->GetWindowText(strY);
	GetDlgItem(IDC_EDIT_Z_UserDisp)->GetWindowText(strZ);
	GetDlgItem(IDC_EDIT_R_UserDisp)->GetWindowText(strR);
	GetDlgItem(IDC_EDIT_T_UserDisp)->GetWindowText(strT);

	float fX = (float)atof(strX);
	float fY = (float)atof(strY);
	float fZ = (float)atof(strZ);
	float fR = (float)atof(strR);
	float fT = (float)atof(strT);
	// 都换成绝对坐标
	if( m_bRelative )
	{
		fX = fX + m_lCurStep[0] *m_dEquivalent[0];
		fY = fY + m_lCurStep[1] *m_dEquivalent[1];
		if( m_bWD )
			fZ = m_lCurStep[2] *m_dEquivalent[2] - fZ;
		else
			fZ = m_lCurStep[2] *m_dEquivalent[2] + fZ;
		fR = fR + m_lCurStep[3] *m_dEquivalent[3];
		fT = fT + m_lCurStep[4] *m_dEquivalent[4];
	}

//	Move4Axis(3, fX, fY, fZ, fR);
	///////////////////////////////////
	// 加入T轴后的调整
	// 只要Z向有变化，就提示用户是否能确保没有危险？
	//		如果用户确认，则保持T向当前角度/或变至需要角度
	//		如果用户无法确认，则先将T向归零
	// 有T向
	if( m_nStageType > 2 && m_bIsVinceReady )
	{
		// Z向向上移动大于0.5mm
		if( fZ -m_lCurStep[2] *m_dEquivalent[2] > 0.5 )
		{
			// 且倾斜角度大于5度
			if( fabs( fT ) > 5 )
			{
				if( MessageBox( m_strMsg[3], m_strMsg[0], MB_YESNO ) == IDNO )
					fT = 0.0;
			}
		}
		Move5Axis(3, fX, fY, fZ, fR, fT);
	}
	///////////////////////////////////
	else
		Move4Axis(3, fX, fY, fZ, fR);
}

void CPanelStage::Move( short anxi, float fPos )
{
	// 从外部设置单轴位置
	if( m_bIsMoving )
		return;

	CString str;
	int nID = 0;
	long lMoveStep = 0;
	switch( anxi )
	{
	case	0:
		Message("Move X");
		nID	= IDC_EDIT_X_UserDisp;
		str.Format( "%.2f", fPos );
		lMoveStep = (long)(fPos /m_dEquivalent[0]);
		m_MC600.MoveA('X', lMoveStep);
		m_nTimer[0] = SetTimer( 1, nStageMC600_Elapse, NULL );
		break;
	case	1:
		Message("Move Y");
		nID = IDC_EDIT_Y_UserDisp;
		str.Format( "%.2f", fPos );
		GetDlgItem(IDC_EDIT_Y_UserDisp)->GetWindowText(str);
		lMoveStep = (long)(fPos /m_dEquivalent[1]);
		m_MC600.MoveA('Y', lMoveStep);
		m_nTimer[1] = SetTimer( 2, nStageMC600_Elapse, NULL );
		break;
	case	2:
		Message("Move Z");
		nID = IDC_EDIT_Z_UserDisp;
		str.Format( "%.2f", fPos );
		MoveZ( fPos, FALSE );
		break;
	case	3:
		Message("Move R");
		nID = IDC_EDIT_R_UserDisp;
		str.Format( "%.1f", fPos );
		lMoveStep = (long)(fPos /m_dEquivalent[3]);
		// R向只有相对位置
		m_MC600.MoveR('T', lMoveStep);
//		m_MC600.MoveA('T', lMoveStep);
		m_nTimer[3] = SetTimer( 4, nStageMC600_Elapse, NULL );
		break;
	case	4:
		Message("Move T");
		nID = IDC_EDIT_T_UserDisp;
		str.Format( "%.1f", fPos );
		lMoveStep = (long)(fPos /m_dEquivalent[4]);
		m_Vince.Move( 1, 1, lMoveStep );
		m_nTimer[4] = SetTimer( 5, nStageMC600_Elapse, NULL );
		break;
	}
	GetDlgItem( nID )->SetWindowText(str);
	m_bIsMoving = TRUE;
}

void CPanelStage::Move( short anxi )
{
	// 从内部控制面板中设置单轴位置
	if( m_bIsMoving )
		return;

	long lMoveStep = 0;
	CString str;
	if( anxi == 0 )
	{
		Message("Move X");
		GetDlgItem(IDC_EDIT_X_UserDisp)->GetWindowText(str);	// mm
		lMoveStep = (long)(atof(str) /m_dEquivalent[0]);
		if( m_bRelative )
			m_MC600.MoveR('X', lMoveStep);
		else
			m_MC600.MoveA('X', lMoveStep);
		m_nTimer[0] = SetTimer( 1, nStageMC600_Elapse, NULL );
	}
	else if( anxi == 1 )
	{
		Message("Move Y");
		GetDlgItem(IDC_EDIT_Y_UserDisp)->GetWindowText(str);
		lMoveStep = (long)(atof(str) /m_dEquivalent[1]);
		if( m_bRelative )
			m_MC600.MoveR('Y', lMoveStep);
		else
			m_MC600.MoveA('Y', lMoveStep);
		m_nTimer[1] = SetTimer( 2, nStageMC600_Elapse, NULL );
	}
	else if( anxi == 2 )
	{
		Message("Move Z");
		GetDlgItem(IDC_EDIT_Z_UserDisp)->GetWindowText(str);
		MoveZ( (float)atof(str), m_bRelative );
	}
	else if( anxi == 3 )
	{
		Message("Move R");
		GetDlgItem(IDC_EDIT_R_UserDisp)->GetWindowText(str);
		lMoveStep = (long)(atof(str) /m_dEquivalent[3]);
		if( m_bRelative )	// R向只有相对位置
			m_MC600.MoveR('T', lMoveStep);
		else
			m_MC600.MoveA('T', lMoveStep);
		m_nTimer[3] = SetTimer( 4, nStageMC600_Elapse, NULL );
	}
	else
	{
		Message("Move T");
		GetDlgItem(IDC_EDIT_T_UserDisp)->GetWindowText(str);	// mm
		lMoveStep = (long)(atof(str) /m_dEquivalent[4]);
		if( m_bRelative )
			m_Vince.Move( 2, 1, lMoveStep );
		else
			m_Vince.Move( 1, 1, lMoveStep );
		m_nTimer[4] = SetTimer( 5, nStageMC600_Elapse, NULL );
	}
	m_bIsMoving = TRUE;
}

void CPanelStage::MoveZ( float fZ, BOOL bRelative )
{
	if( m_bIsMoving )
		return;
	float fZExp = fZ;
	// 都换成绝对坐标
	if( bRelative )
	{
		if( m_bWD )
			fZExp = m_lCurStep[2] *m_dEquivalent[2] - fZ;
		else
			fZExp = m_lCurStep[2] *m_dEquivalent[2] + fZ;
	}
	// 核查预期Z下的XY
	m_fGoal[0] = m_lCurStep[0] *m_dEquivalent[0];
	m_fGoal[1] = m_lCurStep[1] *m_dEquivalent[1];
	m_fGoal[2] = fZExp;
	m_fGoal[3] = m_lCurStep[3] *m_dEquivalent[3];
/*	CoherenceCheck_Z2( m_fGoal[0], m_fGoal[1], m_fGoal[2], FALSE );
	m_MC600.MoveA('X', (long)(m_fGoal[0] /m_dEquivalent[0]));
	m_MC600.MoveA('Y', (long)(m_fGoal[1] /m_dEquivalent[1]));
	m_nProcess = 3;
	m_nMoveSteps = 1;
	m_bIsMoving = TRUE;
	m_nTimer[0] = SetTimer( 1, nStageMC600_Elapse, NULL );
	m_nTimer[1] = SetTimer( 2, nStageMC600_Elapse, NULL );
*/
	///////////////////////////////////
	// 加入T轴后的调整
	// 只要Z向有变化，就提示用户是否能确保没有危险？
	//		如果用户确认，则保持T向当前角度/或变至需要角度
	//		如果用户无法确认，则先将T向归零
	// 有T向
	m_fGoal[4] = m_lCurStep[4] *m_dEquivalent[4];
	if( m_nStageType > 2 && m_bIsVinceReady )
	{
		// Z向向上移动大于0.5mm
		if( fZExp -m_lCurStep[2] *m_dEquivalent[2] > 0.5 )
		{
			// 且倾斜角度大于5度
			if( fabs( m_fGoal[4] ) > 5 )
			{
				if( MessageBox( m_strMsg[3], m_strMsg[0], MB_YESNO ) == IDNO )
					m_fGoal[4] = 0.0;
			}
		}
		Move5Axis(3, m_fGoal[0], m_fGoal[1], m_fGoal[2], m_fGoal[3], m_fGoal[4]);
	}
	///////////////////////////////////
	else
		Move4Axis(3, m_fGoal[0], m_fGoal[1], m_fGoal[2], m_fGoal[3]);
}

void CPanelStage::Move4Axis( int nProcess, float fX, float fY, float fZ, float fR )
{
	if( m_bIsMoving )
		return;
	// 核查预期Z下的XY
	CoherenceCheck_Z2(fX, fY, fZ, FALSE);
	// 更新位置显示
	CString str;
	str.Format( "%.3f", fX );
	GetDlgItem(IDC_EDIT_X_UserDisp)->SetWindowText(str);
	str.Format( "%.3f", fY );
	GetDlgItem(IDC_EDIT_Y_UserDisp)->SetWindowText(str);
	if( m_bWD )
		str.Format( "%.1f", m_fZ0Wd -fZ );
	else
		str.Format( "%.3f", fZ );
	GetDlgItem(IDC_EDIT_Z_UserDisp)->SetWindowText(str);
	str.Format( "%.1f", fR );
	GetDlgItem(IDC_EDIT_R_UserDisp)->SetWindowText(str);
	// 保存目标位置
	m_fGoal[0] = fX;
	m_fGoal[1] = fY;
	m_fGoal[2] = fZ;
	m_fGoal[3] = fR;
	// 移动自动台: 移动有先后，先XY，再Z，最后R
	Message("Move X/Y/Z/R to");
	m_bIsMoving	= TRUE;
	m_nProcess	= nProcess;
	m_nMoveSteps = 1;
	m_MC600.MoveA('X', (long)(m_fGoal[0] /m_dEquivalent[0]));
	m_MC600.MoveA('Y', (long)(m_fGoal[1] /m_dEquivalent[1]));
	m_nTimer[0] = SetTimer(1, nStageMC600_Elapse, NULL);
	m_nTimer[1] = SetTimer(2, nStageMC600_Elapse, NULL);
}

void CPanelStage::Move5Axis( int nProcess, float fX, float fY, float fZ, float fR, float fT )
{
	if( m_bIsMoving )
		return;
	// 核查预期Z下的XY
	CoherenceCheck_Z2(fX, fY, fZ, FALSE);
	// 更新位置显示
	CString str;
	str.Format( "%.3f", fX );
	GetDlgItem(IDC_EDIT_X_UserDisp)->SetWindowText(str);
	str.Format( "%.3f", fY );
	GetDlgItem(IDC_EDIT_Y_UserDisp)->SetWindowText(str);
	if( m_bWD )
		str.Format( "%.1f", m_fZ0Wd -fZ );
	else
		str.Format( "%.3f", fZ );
	GetDlgItem(IDC_EDIT_Z_UserDisp)->SetWindowText(str);
	str.Format( "%.1f", fR );
	GetDlgItem(IDC_EDIT_R_UserDisp)->SetWindowText(str);
	// 保存目标位置
	m_fGoal[0] = fX;
	m_fGoal[1] = fY;
	m_fGoal[2] = fZ;
	m_fGoal[3] = fR;
	m_fGoal[4] = fT;
	// 移动自动台: 移动有先后，先T，再XY，再Z，最后R
	Message("Move T/X/Y/Z/R to");
	m_bIsMoving	= TRUE;
	m_nProcess	= nProcess;
	m_nMoveSteps = 0;
	m_Vince.Move( 1, 1, (long)(m_fGoal[4] /m_dEquivalent[4]) );
	m_nTimer[4] = SetTimer( 5, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONSetUserOrigin() 
{
	SaveUserOrigin();
	SetUserOrigin();
}

void CPanelStage::SaveUserOrigin()
{
	// 保存工作原点绝对位置
	m_fCali[0] = (float)(m_lCurStep[0] *m_dEquivalent[0]);
	m_fCali[1] = (float)(m_lCurStep[1] *m_dEquivalent[1]);
	m_fCali[2] = (float)(m_lCurStep[2] *m_dEquivalent[2]);	// Z向不写了
	m_fCali[3] = (float)(m_lCurStep[3] *m_dEquivalent[3]);
	m_fCali[4] = (float)(m_lCurStep[4] *m_dEquivalent[4]);

	if( MessageBox( m_strMsg[2], m_strMsg[0], MB_YESNO ) == IDYES )
	{
		// 获取工作路径
		char path[256];
		::GetModuleFileName(0, path, 255);
		char *exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		strcat(path, "\\Stage\\StageMC600.ini");
		char szBuf[255];
		// 写入配置文件
		sprintf( szBuf, "%.2f,%.2f,%.2f,%.1f,%.1f", m_fCali[0], m_fCali[1], m_fCali[2], m_fCali[3], m_fCali[4] );
		WritePrivateProfileString( "StageMC600Cfg", "Cali", szBuf, path );
	}
}

void CPanelStage::SetUserOrigin()
{
	// 设置当前位置为运动轴工作原点
	m_MC600.SetHome( UserOrigin, 'X', '0' );
	m_MC600.SetHome( UserOrigin, 'Y', '0' );
	m_MC600.SetHome( UserOrigin, 'Z', '0' );
	m_MC600.SetHome( UserOrigin, 'T', '0' );
	m_Vince.SetOrg(1);
	Sleep(1000);
	for( int i=0; i<5; i++ )
		GetCurStep(i);
	GetDlgItem(IDC_EDIT_X_UserDisp)->SetWindowText("0.000");
	GetDlgItem(IDC_EDIT_Y_UserDisp)->SetWindowText("0.000");
	GetDlgItem(IDC_EDIT_Z_UserDisp)->SetWindowText("0.000");
	GetDlgItem(IDC_EDIT_R_UserDisp)->SetWindowText("0.0");
	GetDlgItem(IDC_EDIT_T_UserDisp)->SetWindowText("0.0");
}

void CPanelStage::OnBUTTONZero() 
{
	Zero();
}

void CPanelStage::Zero()
{
	if( m_bIsMoving )
		return;
	m_nProcess = 2;
	// 回扫描中心（即用户原点归零）
	if( m_nStageType > 2 && m_bIsVinceReady )
	{
		Message( "Go Origion T/X/Y/Z/R" );
		m_nMoveSteps = 0;
//		m_Vince.BackZero(1, 1);		// 机械中心
		m_Vince.Move( 1, 1, 0 );	// 自定义的用户原点
		m_nTimer[4] = SetTimer( 5, nStageMC600_Elapse, NULL );
	}
	else
	{
		Message( "Go Origion X/Y/Z/R" );
		m_nMoveSteps = 1;
		m_MC600.GoOrigion('X');
		m_MC600.GoOrigion('Y');
		m_nTimer[0] = SetTimer( 1, nStageMC600_Elapse, NULL );
		m_nTimer[1] = SetTimer( 2, nStageMC600_Elapse, NULL );
	}
	m_bIsMoving = TRUE;
}

void CPanelStage::OnBUTTONZeroX() 
{
	if( m_bIsMoving )
		return;
	Message( "GoOrigion X" );
	m_bIsMoving = TRUE;
	m_MC600.GoOrigion( 'X' );
}

void CPanelStage::OnBUTTONZeroY() 
{
	if( m_bIsMoving )
		return;
	Message( "GoOrigion Y" );
	m_bIsMoving = TRUE;
	m_MC600.GoOrigion( 'Y' );
}

void CPanelStage::OnBUTTONZeroZ() 
{
	if( m_bIsMoving )
		return;
	Message( "GoOrigion Z" );
	m_bIsMoving = TRUE;
	m_MC600.GoOrigion( 'Z' );
}

void CPanelStage::OnBUTTONZeroR() 
{
	if( m_bIsMoving )
		return;
	Message( "GoOrigion R" );
	m_bIsMoving = TRUE;
	m_MC600.GoOrigion( 'T' );
}

void CPanelStage::OnBUTTONStop() 
{
	Stop();
}

void CPanelStage::Stop()
{
	m_nProcess = -1;
	m_nMoveSteps = -1;
	Message( "Stop X/Y/Z/R move" );
	m_MC600.StopMove('X');
	m_MC600.StopMove('Y');
	m_MC600.StopMove('Z');
	m_MC600.StopMove('T');
	m_bIsMoving = FALSE;
	m_bRelative = m_bRelativeBak;
	::PostMessage( GetParent()->m_hWnd, WM_USER_MC600_UpdateValue, 0, 0 );

	m_Vince.MotorStop(1);
}

void CPanelStage::OnVentExit()
{
	switch( m_nVentSel )
	{
	case	0:	// 回机械原点
		Home(1);
		break;
	case	1:	// 回用户原点
		Zero();
		break;
	case	2:	// 不动
		break;
	}
}

void CPanelStage::OnBUTTONHome() 
{
	Home(1);
}

void CPanelStage::Home( int nProcess )
{
	if( m_bIsMoving )
		return;
	::PostMessage( GetParent()->m_hWnd, WM_USER_MC600_UpdateValue, 0, 1 );
	m_nProcess = nProcess;
	m_bIsMoving = TRUE;
	SetTimer( 11, 500, NULL );
}

void CPanelStage::OnBUTTONHomeX() 
{
	Home(21);
}

void CPanelStage::OnBUTTONHomeY() 
{
	Home(22);
}

void CPanelStage::OnBUTTONHomeZ() 
{
	Home(23);
}

void CPanelStage::OnBUTTONHomeR() 
{
	if( m_bOptR )	// 有光电开关
		Home(24);
}

BOOL CPanelStage::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		CString str = "";
		int nID = GetFocus()->GetDlgCtrlID();
		switch( nID )
		{
		case	IDC_EDIT_X_UserDisp:
			Move(0);
			break;
		case	IDC_EDIT_Y_UserDisp:
			Move(1);
			break;
		case	IDC_EDIT_Z_UserDisp:
			Move(2);
			break;
		case	IDC_EDIT_R_UserDisp:
			Move(3);
			break;
		case	IDC_EDIT_T_UserDisp:
			Move(4);
			break;

		case	IDC_Stage_Mag:
			{
				CString str;
				GetDlgItem(IDC_Stage_Mag)->GetWindowText(str);
				m_dMag = atof(str);
				MagChange();
			}
			break;

		case	IDC_LIST_Position:
			{
				Message("Click and enter in listctrl");
			}
			break;
		default:
			break;
		}
		if( nID == IDC_LIST_Position )
			return FALSE;
		else
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CPanelStage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if( nCtlColor == CTLCOLOR_STATIC )
	{
		int nID = pWnd->GetDlgCtrlID();
		if( nID == IDC_EDIT_X_CurStep )
		{
			if( m_nTimer[0] != 0 )
				pDC->SetTextColor(RGB(255,0,0));// 设置文本颜色  
			else
				pDC->SetTextColor(RGB(0,0,0));	// 设置文本颜色  
		}
		else if( nID == IDC_EDIT_Y_CurStep )
		{
			if( m_nTimer[1] != 0 )
				pDC->SetTextColor(RGB(255,0,0));// 设置文本颜色  
			else
				pDC->SetTextColor(RGB(0,0,0));	// 设置文本颜色  
		}
		else if( nID == IDC_EDIT_Z_CurStep )
		{
			if( m_nTimer[2] != 0 )
				pDC->SetTextColor(RGB(255,0,0));// 设置文本颜色  
			else
				pDC->SetTextColor(RGB(0,0,0));	// 设置文本颜色  
		}
		else if( nID == IDC_EDIT_R_CurStep )
		{
			if( m_nTimer[3] != 0 )
				pDC->SetTextColor(RGB(255,0,0));// 设置文本颜色  
			else
				pDC->SetTextColor(RGB(0,0,0));	// 设置文本颜色  
		}
		else if( nID == IDC_EDIT_T_CurStep )
		{
			if( m_nTimer[4] != 0 )
				pDC->SetTextColor(RGB(255,0,0));// 设置文本颜色  
			else
				pDC->SetTextColor(RGB(0,0,0));	// 设置文本颜色  
		}
	}
	return hbr;
}

void CPanelStage::OnBUTTONXMoveMinus2() 
{
	Message("X continues to move in a negative dir");
	m_bIsMoving = TRUE;
	m_MC600.MoveA('X', -150000);
	m_nTimer[0] = SetTimer( 1, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONXMovePlus2() 
{
	Message("X continues to move in a forward dir");
	m_bIsMoving = TRUE;
	m_MC600.MoveA('X', 150000);
	m_nTimer[0] = SetTimer( 1, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONYMoveMinus2() 
{
	Message("Y continues to move in a negative dir");
	m_bIsMoving = TRUE;
	m_MC600.MoveA('Y', -120000);
	m_nTimer[1] = SetTimer( 2, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONYMovePlus2() 
{
	Message("Y continues to move in a forward dir");
	m_bIsMoving = TRUE;
	m_MC600.MoveA('Y', 120000);
	m_nTimer[1] = SetTimer( 2, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONZMoveMinus2() 
{
	Message("Z continues to move in a negative dir");
	m_bIsMoving = TRUE;
	m_MC600.MoveA('Z', -200000);
	m_nTimer[2] = SetTimer( 3, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONZMovePlus2() 
{
	Message("Z continues to move in a forward dir");
	m_bIsMoving = TRUE;
	m_MC600.MoveA('Z', 400000);
	m_nTimer[2] = SetTimer( 3, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONRMoveMinus2() 
{
	Message("R continues to move in a negative dir");
	m_bIsMoving = TRUE;
	m_MC600.MoveA('T', -20000);
	m_nTimer[3] = SetTimer( 4, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONRMovePlus2() 
{
	Message("R continues to move in a forward dir");
	m_bIsMoving = TRUE;
	m_MC600.MoveA('T', 20000);
	m_nTimer[3] = SetTimer( 4, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnCHECKRelative() 
{
	m_bRelative = ((CButton*)(GetDlgItem(IDC_CHECK_Relative)))->GetCheck();
	if( m_bRelative ) 
		Message("Change move style to RELATIVE");
	else
		Message("Change move style to ABSOLUTE");
	m_bRelativeBak = m_bRelative;
}

void CPanelStage::OnCheckZWd() 
{
	m_bWD = ((CButton*)(GetDlgItem(IDC_CHECK_Z_WD)))->GetCheck();
	CString str;
	if( m_bWD ) 
	{
		str.Format( "%.1f", m_fZ0Wd -m_lCurStep[2] *m_dEquivalent[2] );
		Message("Change to display WD (mm)");
	}
	else
	{
		str.Format( "%.3f", m_lCurStep[2] *m_dEquivalent[2] );
		Message("Change to display Z (mm)");
	}
	GetDlgItem(IDC_EDIT_Z_UserDisp)->SetWindowText(str);
}

void CPanelStage::OnCHECKAdvance() 
{
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Advance)))->GetCheck();
	if( nCheck > 0 )
	{
		CDlgInput dlg;
		dlg.m_bPassword = TRUE;
		strcpy( dlg.m_cPathLocalize, m_cPathLocalize );
		if( (dlg.DoModal() != IDOK) || dlg.m_strInput != "kx-sem" )
		{
			((CButton*)(GetDlgItem(IDC_CHECK_Advance)))->SetCheck(0);
			return;
		}
		GetDlgItem(IDC_CHECK_Advance)->SetWindowText("<<");
		GetDlgItem(IDC_BUTTON_Home)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_HomeX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_HomeY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_HomeZ)->ShowWindow(SW_SHOW);
		if( m_bOptR )
			GetDlgItem(IDC_BUTTON_HomeR)->ShowWindow(SW_SHOW);
		else
			GetDlgItem(IDC_BUTTON_HomeR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HomeT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ZeroX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ZeroY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ZeroZ)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ZeroR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ZeroT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_X_Move_Minus2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_X_Move_Plus2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_Y_Move_Minus2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_Y_Move_Plus2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_Z_Move_Minus2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_Z_Move_Plus2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_T_Move_Minus2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_T_Move_Plus2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_SetUserOrigin)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_Backlash)->ShowWindow(SW_SHOW);
		SetWindowPos(&CWnd::wndTop, 0, 0,
					m_rcWindow.Width(), m_rcWindow.Height(), SWP_NOMOVE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_Advance)->SetWindowText(">>");
		GetDlgItem(IDC_BUTTON_Home)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HomeX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HomeY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HomeZ)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HomeR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HomeT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ZeroX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ZeroY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ZeroZ)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ZeroR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ZeroT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_X_Move_Minus2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_X_Move_Plus2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_Y_Move_Minus2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_Y_Move_Plus2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_Z_Move_Minus2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_Z_Move_Plus2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_T_Move_Minus2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_T_Move_Plus2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_SetUserOrigin)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_Backlash)->ShowWindow(SW_HIDE);
		SetWindowPos(&CWnd::wndTop, 0, 0, m_rcWindow.Width(),
					m_rcWindow.Height() -m_rcReport.Height() -10, SWP_NOMOVE);
	}
}

void CPanelStage::OnBUTTONBacklash() 
{

}

void CPanelStage::OnBUTTONSetup() 
{
	CRect rc;
	GetWindowRect(rc);
	CDlgSetup dlg;
	dlg.m_nPort = m_nPort;
	dlg.m_nPort2 = m_nPort2;
	dlg.m_fRyp = m_paramZRes.fRyp;
	dlg.m_fH = m_paramZRes.fH;
	dlg.m_bClick = m_bBlClick;
	dlg.m_bTrack = m_bBlTrack;
	dlg.m_nVentSel = m_nVentSel;
	dlg.m_strMsg[0] = m_strMsg[0];
	dlg.m_bAdvance = (rc.Height() > 600) ? TRUE : FALSE;
	dlg.m_nStageType = m_nStageType;
	strcpy( dlg.m_cPathLocalize, m_cPathLocalize );
	if( dlg.DoModal() == IDOK )
	{
		m_nPort = dlg.m_nPort;
		m_nPort2 = dlg.m_nPort2;
		m_paramZRes.fRyp = dlg.m_fRyp;
		m_paramZRes.fH = dlg.m_fH;
		m_bBlClick	= dlg.m_bClick;
		m_bBlTrack	= dlg.m_bTrack;
		m_nVentSel	= dlg.m_nVentSel;
		// 获取工作路径
		char path[255];
		::GetModuleFileName(0, path, 255);
		char *exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		strcat(path, "\\Stage\\StageMC600.ini");
		char szBuf[255];
		// 写入配置文件
		sprintf( szBuf, "%d,%d", m_nPort, m_nPort2 );
		WritePrivateProfileString( "StageMC600Cfg", "Port", szBuf, path );
		sprintf( szBuf, "%d", m_bBlClick );
		WritePrivateProfileString( "StageMC600Cfg", "BlClick", szBuf, path );
		sprintf( szBuf, "%d", m_bBlTrack );
		WritePrivateProfileString( "StageMC600Cfg", "BlTrack", szBuf, path );
		sprintf( szBuf, "%d", m_nVentSel );
		WritePrivateProfileString( "StageMC600Cfg", "VentSel", szBuf, path );
		if( m_paramZRes.fH < 5 )
			sprintf( szBuf, "%.1f,5.0", m_paramZRes.fRyp );
		else
			sprintf( szBuf, "%.1f,%.1f", m_paramZRes.fRyp, m_paramZRes.fH );
		WritePrivateProfileString( "StageMC600Zres2", "ResUser", szBuf, path );
		// 更新Z向范围值
		InitParam_ZRestraint_User( path );
		SetZLimit();
	}
}




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//CString CalculateStageUScale( int nScalePixelsX, int nScalePixelsY, DWORD &dwScaleX, DWORD &dwScaleY )
void CPanelStage::CalculateStageUScale( int nAxis,		// 是哪个轴
								 double	dScalePixels,	// 像素长度
								 int	&nScale )		// 实际移动步数
{
	// 放大倍数 = 屏幕物理尺寸 / 样品的实际物理尺寸
	// 例如：5寸照片（12.7mm * 8.9mm）时，
	//			3mm铜网（样品实际物理尺寸）在屏幕上实际测量为45mm，
	//			则放大倍数为15倍
	// 这里需要注意，屏幕实际测量的尺寸是和屏幕的参数有关的
	//			当显示器驱动安装后，屏幕的像素数和对应的物理尺寸可获得
	//			那么5寸照片的尺寸对应的像素数就可以得到
	//
	// 当自动台XY向单步走时，计划每次走当前屏幕的1/4
	// 比如：当前图像分辨率为1024*768，则X向每次走256像素，Y向每次走192像素
	//		也就是说：当前图像分辨率下，XY向每次走的屏幕物理尺寸都是固定值
	//		那么即可获得每一个放大倍数下，样品台单步实际走的物理尺寸
	//		也就可以换算成样品台单步走的pp值

	// 我的监视器为520mm*320mm，1920*1200 pixel
	double dRatio[4];
	dRatio[0] = m_dblPixelsPerMM_HORZ;	// 1920.0 /520.0;
	dRatio[1] = m_dblPixelsPerMM_VERT;	// 1200.0 /320.0;
	dRatio[2] = dRatio[3] = 1.0;
	// 屏幕物理尺寸 = 像素数 / Ratio，实际物理尺寸= 屏幕物理尺寸 / 放大倍数
	double dScale = dScalePixels / dRatio[nAxis] / m_dMag;
	dScale *= m_paramsSys.fPitch[nAxis];	// 解决X/Y方向单步走和双击跳转中心时的2倍问题
	nScale = (int)(dScale/m_dEquivalent[nAxis] +0.5);
	if( nScale < 1 )
		nScale = 1;
}

void CPanelStage::SetPixelsPerMM( double dX, double dY )
{
	m_dblPixelsPerMM_HORZ = dX;
	m_dblPixelsPerMM_VERT = dY;
}

void CPanelStage::CoherenceCheck_Mag()
{
	if( m_dMag > 450000 )
		m_dMag = 450000;
	if( m_dMag < 5 )
		m_dMag = 5;
}

void CPanelStage::OnDeltaposStageMagAdj(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString str;
	GetDlgItem(IDC_Stage_Mag)->GetWindowText(str);
	m_dMag = atof(str);
	if( m_dMag > 10000 )
	{
		if( pNMUpDown->iDelta > 0 )
			m_dMag -= 100;
		else
			m_dMag += 100;
	}
	else
	{
		if( pNMUpDown->iDelta > 0 )
			m_dMag -= 10;
		else
			m_dMag += 10;
	}
	MagChange();
	*pResult = 0;
}

void CPanelStage::MagChange()
{
	CoherenceCheck_Mag();
	CString str;
	str.Format("%.1f", m_dMag);
	GetDlgItem(IDC_Stage_Mag)->SetWindowText(str);
}

void CPanelStage::SetMag( double dMag )
{
	m_dMag = dMag;
	MagChange();
}

void CPanelStage::SetResolution( CSize szReso )
{
	m_sizeReso = szReso;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void CPanelStage::SetParam( int nIndex, double dParam )
{
	if( nIndex == 1 )
	{
		// 光电开关状态
		m_bPEswitch = dParam > 0 ? TRUE : FALSE;
	}
}

void CPanelStage::GetParam( int nIndex, float& f1, float& f2, float& f3, float& f4, float& f5 )
{
	switch( nIndex )
	{
	case	0:	// positive limit
		f1 = m_actualLimit[1];	// m_paramZRes.fLimitPos[0];
		f2 = m_actualLimit[3];	// m_paramZRes.fLimitPos[1];
		f3 = m_actualLimit[5];	// m_paramZRes.fLimitPos[2];
		f4 = 360.0;
		f5 = 75;
		break;
	case	1:	// negtive limit
		f1 = m_actualLimit[0];	// m_paramZRes.fLimitNeg[0];
		f2 = m_actualLimit[2];	// m_paramZRes.fLimitNeg[1];
		f3 = m_actualLimit[4];	// m_paramZRes.fLimitNeg[2];
		f4 = -360.0;
		f5 = -15;
		break;
	case	2:	// position
		f1 = m_lCurStep[0] *m_dEquivalent[0];
		f2 = m_lCurStep[1] *m_dEquivalent[1];
		f3 = m_lCurStep[2] *m_dEquivalent[2];
		f4 = m_lCurStep[3] *m_dEquivalent[3];
		f5 = m_lCurStep[4] *m_dEquivalent[4];
		break;

	case	3:
		f1 = m_fZ0Wd;
		break;
	}
}

void CPanelStage::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}


/////////////////////////////////////////////////////////////////////
// T轴

void CPanelStage::OnBUTTONTMoveMinus() 
{
	if( m_bIsMoving )
		return;
	// 单步走每次5度
	// 实际步数
	long lMoveStep = 0 -(int)(5/m_dEquivalent[4]);
	CString str;
	str.Format( "T move relative %ld steps", lMoveStep );
	Message(str);
	m_bIsMoving = TRUE;
	m_Vince.Move( 2, 1, lMoveStep );
	m_nTimer[4] = SetTimer( 5, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONTMovePlus() 
{
	if( m_bIsMoving )
		return;
	// 单步走每次5度
	// 实际步数
	long lMoveStep = (int)(5/m_dEquivalent[4]);
	CString str;
	str.Format( "T move relative %ld steps", lMoveStep );
	Message(str);
	m_bIsMoving = TRUE;
	m_Vince.Move( 2, 1, lMoveStep );
	m_nTimer[4] = SetTimer( 5, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONTMoveTo() 
{
	if( m_bIsMoving )
		return;
	Message( "Move T" );
//	Move(3);
}

void CPanelStage::OnBUTTONTMoveMinus2() 
{
	Message("T continues to move in a negative dir");
	m_bIsMoving = TRUE;
	m_Vince.SetCfgParams(Cfg_spd, 1, -6400);
	m_Vince.Move( 0, 1, 0 );
	m_nTimer[4] = SetTimer( 5, 2*nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONTMovePlus2() 
{
	Message("T continues to move in a forward dir");
	m_bIsMoving = TRUE;
	m_Vince.SetCfgParams(Cfg_spd, 1, 6400);
	m_Vince.Move( 0, 1, 0 );
	m_nTimer[4] = SetTimer( 5, 2*nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONZeroT() 
{
	if( m_bIsMoving )
		return;
	Message( "GoOrigion T" );
	m_bIsMoving = TRUE;
	m_Vince.BackZero(1, 1);
	m_nTimer[4] = SetTimer( 5, nStageMC600_Elapse, NULL );
}

void CPanelStage::OnBUTTONHomeT() 
{
// 	if( m_bIsMoving )
// 		return;
// 	Message( "GoHome T" );
// 	m_bIsMoving = TRUE;
// 	m_Vince.BackZero(1, 1);
// 	m_nTimer[4] = SetTimer( 5, nStageMC600_Elapse, NULL );
	Home(25);
}

Vince_Param_Status CPanelStage::GetStatus( short anxi )
{
	Vince_Param_Status sp = m_Vince.GetStatus(anxi);
	return sp;
}
