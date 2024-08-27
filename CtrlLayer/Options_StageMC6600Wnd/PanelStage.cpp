// PanelStage.cpp : implementation file
//

#include "stdafx.h"
#include "Options_StageMC6600Wnd.h"
#include "PanelStage.h"
#include "DlgInput.h"
#include "DlgSetup.h"
#include "DlgDriverInit.h"

#include <math.h>
#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CLIP_RANGE(value, min, max)  ( (value) > (max) ? (max) : (((value) < (min)) ? (min) : (value)) )

const UINT	nStageMC6600_Elapse	= 800;
const UINT	nStageMC6600_Track	= 1000;
const long	lSpeedTrackMax		= 400;
const float m_fTrackStep[6]		= {0.1,0.2,0.3,0.4,0.5,1.5};
long		t1,t2,t3;

HHOOK g_hook = NULL;

unsigned char SerialNum(short anxi)
{
	unsigned char ucAxis = 'X';
	switch (anxi)
	{
	case 0:
		ucAxis = 'X';
		break;
	case 1:
		ucAxis = 'Y';
		break;
    case 2:
		ucAxis = 'Z';
		break;
	case 3:
		ucAxis = 'R';
		break;
	case 4:
		ucAxis = 'T';
		break;
	}
	return ucAxis;
}

short NumSerial( unsigned char ucAxis )
{
	short anxi = 0;
	switch( ucAxis )
	{
	case	'X':
		anxi = 0;
		break;
	case	'Y':
		anxi = 1;
		break;
	case	'Z':
		anxi = 2;
		break;
	case	'R':
		anxi = 3;
		break;
	case	'T':
		anxi = 4;
		break;
	}
	return anxi;
}

BOOL NotZero( float fValue )
{
	if( fabs(fValue) < FLT_EPSILON )	// 非常小
		return FALSE;
	else
		return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//Read data in one line in *.ini.
//In one line the data like ***,*****,*****, they are seperated by comma.
const UINT INTSTYLE		= 1;
const UINT DOUBLESTYLE	= 2;
const UINT LONGSTYLE	= 3;
const UINT STRSTYLE		= 4;
const UINT CHARSTYLE	= 5;
const UINT FLOATSTYLE	= 6;

int ReadInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator)
{
	char szBuf1[255];
	char szBuf2[255];
	int* pint = (int*) pData;
	double* pdouble = (double*)pData;
	long* plong = (long*)pData;
	CString* pstr = (CString*)pData;
	char* pch = (char*)pData;
	float* pfloat = (float*)pData;
	int i = 0;
	if(GetPrivateProfileString(Section, Entry, "", szBuf1, 255, path) != 0)
	{
		for(i=0; i<nTotal; i++)
		{
			strcpy_s(szBuf2, szBuf1);
			if(strchr(szBuf2, Separator) != NULL)
			{
				strcpy_s(szBuf1, strchr(szBuf2, Separator)+1);
				*strchr(szBuf2, Separator) = '\0';
				switch( uDataStyle )
				{
				case	INTSTYLE:
					pint[i] = atoi(szBuf2);
					break;
				case	DOUBLESTYLE:
					pdouble[i] = atof(szBuf2);
					break;
				case	LONGSTYLE:
					plong[i] = atol(szBuf2);
					break;
				case	STRSTYLE:
					pstr[i] = szBuf2;
					break;
				case	CHARSTYLE:
					pch[i] = szBuf2[0];
					break;
				case	FLOATSTYLE:
					pfloat[i] = (float)atof(szBuf2);
					break;
				default:
					return 0;
				}
			}
			else
			{
				// This is the last data.
				switch( uDataStyle )
				{
				case	INTSTYLE:
					pint[i] = atoi(szBuf2);
					break;
				case	DOUBLESTYLE:
					pdouble[i] = atof(szBuf2);
					break;
				case	LONGSTYLE:
					plong[i] = atol(szBuf2);
					break;
				case	STRSTYLE:
					pstr[i] = szBuf2;
					break;
				case	CHARSTYLE:
					pch[i] = szBuf2[0];
					break;
				case	FLOATSTYLE:
					pfloat[i] = (float)atof(szBuf2);
					break;
				default:
					return 0;
				}
				if(i == nTotal-1)
				{
					nTotal = i+1;
				}
				else
				{
					nTotal = i;
				}
				break;
			}
		}
	}
	return i+1;
}

/////////////////////////////////////////////////////////////////////////////
// CPanelStage dialog

CPanelStage::CPanelStage(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelStage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelStage)
		// NOTE: the ClassWizard will add member initialization here
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

	m_hFileLog      = NULL;
	m_sizeReso.cx	= 1024;
	m_sizeReso.cy	= 768;
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
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_Position, OnDblclkListPosition)
	ON_BN_CLICKED(IDC_CHECK_Relative, OnBnClickedCheckRelative)
	ON_BN_CLICKED(IDC_CHECK_Advance, OnBnClickedCheckAdvance)
	ON_BN_CLICKED(IDC_CHECK_Z_WD, OnBnClickedCheckZWd)
	ON_BN_CLICKED(IDC_BUTTON_Home, OnBnClickedButtonHome)
	ON_BN_CLICKED(IDC_BUTTON_Zero, OnBnClickedButtonZero)
	ON_BN_CLICKED(IDC_BUTTON_Stop, OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_MoveTo, OnBnClickedButtonMoveto)
	ON_BN_CLICKED(IDC_BUTTON_ListChange, OnBnClickedButtonListChange)
	ON_BN_CLICKED(IDC_BUTTON_ListAdd, OnBnClickedButtonListadd)
	ON_BN_CLICKED(IDC_BUTTON_ListDel, OnBnClickedButtonListdel)
	ON_BN_CLICKED(IDC_BUTTON_SetUserOrigin, OnBnClickedButtonSetuserorigin)
	ON_BN_CLICKED(IDC_BUTTON_Setup, OnBnClickedButtonSetup)
	ON_BN_CLICKED(IDC_BUTTON_DriverInit, OnBnClickedButtonDriverInit)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_All, OnBnClickedCheckAll)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BUTTON_HomeX, IDC_BUTTON_HomeT, OnHomeChanged)
	ON_COMMAND_RANGE(IDC_BUTTON_ZeroX, IDC_BUTTON_ZeroT, OnZeroChanged)
	ON_COMMAND_RANGE(IDC_BUTTON_X_Move_Minus, IDC_BUTTON_T_Move_Minus, OnMoveMinusChanged)
	ON_COMMAND_RANGE(IDC_BUTTON_X_Move_Plus, IDC_BUTTON_T_Move_Plus, OnMovePlusChanged)
	ON_COMMAND_RANGE(IDC_BUTTON_X_Move_Minus2, IDC_BUTTON_T_Move_Minus2, OnMoveMinus2Changed)
	ON_COMMAND_RANGE(IDC_BUTTON_X_Move_Plus2, IDC_BUTTON_T_Move_Plus2, OnMovePlus2Changed)
	ON_COMMAND_RANGE(IDC_CHECK_X, IDC_CHECK_T, OnMotorChanged)
	ON_MESSAGE(WM_USER_MC6600_UpdateValue, OnUpdateMC6600Value)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelStage message handlers

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

	// TODO:  在此添加额外的初始化
	m_bIsMC6600Ready	= FALSE;
	m_nProcess			= 0;
	m_nMoveSteps		= -1;
	m_bBlClick			= FALSE;
	m_bBlTrack			= FALSE;
	m_bHighPrecCount	= FALSE;
	m_bXdir				= FALSE;
	m_bYdir				= FALSE;
	m_fSLength[0]		= 0.0;
	m_fSLength[1]		= 0.0;
	m_bSoftLimit		= TRUE;
	m_bMoveXY			= FALSE;

	int i = 0;
	for( i=0; i<5; i++ )
	{
		m_fCurStep[i]	= 0;
		m_bBL[i]		= FALSE;
		m_fCurStep[i]	= 0;
		m_fDis[i]		= 1000.0;
		m_fCali[i]		= 0.0;
		m_fGoal[i]		= 0.0;
		m_bEnable[i]	= 0;
		m_bIsMoving[i]	= FALSE;
		m_fDeadline[i]	= 0;
		m_fBLCount[i]	= FALSE;
		m_bEqual[i]		= FALSE;
		m_bOrigion[i]   = FALSE;
		m_bDirection[i]	= FALSE;
		m_bNum[i]		= TRUE;
		m_bToHome[i]	= FALSE;
		m_fMaxSpeed[i]	= 4;
		m_fMinSpeed[i]	= 0.5;
		m_bTrackState[i] = TRUE;
		m_bAxisInitia[i] = TRUE;
		m_bCommunicationFault[i] = FALSE;
		m_bMotorFault[i] = FALSE;
		m_fRockerStep[i] = 0.1;
		m_nPhaseInit[i]	 = -1;
	}
	m_bAxisInitia[3] = FALSE;
	m_fMaxSpeed[4]	= 0.4f;
	m_fMinSpeed[4]	= 0.05f;
	//摇杆
	/*m_fTrackStep	= 0.5f;
	m_fTrackStepR	= 1.5f;
	m_fTrackStepT	= 0.2f;*/
	m_fTrackStepAxis = 0.05;

/*	m_paramZRes.fTa = 0;
	m_paramZRes.fHb = 2;
	m_paramZRes.fRqy = 0;
	m_paramZRes.fDis = 0;
	for (int i=0;i<3;i++)
	{
		m_paramZRes.fTangCircent[i]  =0;
		m_paramZRes.fRightArcjudg[i] =0;
		m_paramZRes.fLeftCylinleft[i] =0;
		m_paramZRes.fLeftSquareleft[i] =0;
	}
*/
	for( i=0; i<10; i++ )
	{
		m_fSLimit[i]		= 0;
		m_factualLimit[i]	= 0;
		m_nTimer[i]			= 0;
	}

	//手动自动
	for( i=0; i<5; i++ )
		m_bMotoronoff[i] = FALSE;	// false为自动
	m_bMotoronoffAll= FALSE;
	m_bDbClick		= TRUE;			// 双击

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
	
	SetStatus(FALSE);

	//运动方式
	m_bRelative		= FALSE;
	((CButton*)GetDlgItem(IDC_CHECK_Relative))->SetCheck(0);

	m_dMag = 100;
	GetDlgItem(IDC_Stage_Mag)->SetWindowText( "100" );

	GetDlgItem(IDC_EDIT_X_UserDisp)->SetWindowText( "0.000" );
	GetDlgItem(IDC_EDIT_Y_UserDisp)->SetWindowText( "0.000" );
	GetDlgItem(IDC_EDIT_Z_UserDisp)->SetWindowText( "0.000" );
	GetDlgItem(IDC_EDIT_R_UserDisp)->SetWindowText( "0" );
	GetDlgItem(IDC_EDIT_T_UserDisp)->SetWindowText( "0" );

	// 设置位置列表
	List_SetHead(0, _T("Label"));
	List_SetHead(1, "X/mm");
	List_SetHead(2, "Y/mm");
	List_SetHead(3, "Z/mm");
	List_SetHead(4, "R/deg");
	List_SetHead(5, "T/deg");
	List_Init();

	//Connect();

	// 将对话框位置设置到客户区左上角
	GetWindowRect(m_rcWindow);
	GetDlgItem(IDC_EDIT_Report)->GetClientRect(m_rcReport);
	SetWindowPos(&CWnd::wndTop,
				8,		//GetSystemMetrics(SM_CXSCREEN) - rc.Width()-10,
				32*3,	//GetSystemMetrics(SM_CYSCREEN) /2 -rc.Height() /2,
				m_rcWindow.Width(),
				m_rcWindow.Height() -m_rcReport.Height() -15,	//rc.Height(),
				SWP_SHOWWINDOW);	//SWP_NOSIZE);

	// Place this code inside an initialization method in your implementation file (.cpp)
	g_hook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgHookProc, NULL, GetCurrentThreadId());
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CPanelStage::OnDestroy()
{
	int i=0;
	for( i=0; i<10; i++ )
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
	while( (pos=m_plistMC6600Pos->GetHeadPosition()) )
	{
		structMC6600Pos* pItem = m_plistMC6600Pos->GetAt( pos );
		m_plistMC6600Pos->RemoveHead();
		delete pItem;
	}
	delete m_plistMC6600Pos;
	if( m_bIsMC6600Ready )
	{
		for(i=0;i<5;i++)
		{
			theApp.m_StageMC6600.close(i);
			m_bEnable[i] = FALSE;
			m_bIsMoving[i] = FALSE;
			CStageMC6600Manager::Instance().m_bIsMoving[i] = m_bIsMoving[i];
		}
		Sleep(100);
		theApp.m_StageMC6600.Close();
	}
	if( m_hFileLog != NULL )
		CloseHandle( m_hFileLog );
	m_hFileLog	= NULL;
	
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

void CPanelStage::OnCancel()
{
	CDialog::OnCancel();
}

void CPanelStage::OnOK()
{
	//CDialog::OnOK();
}

void CPanelStage::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CPanelStage::SetLanguageID( WORD wID )
{
	m_wLanguageID = wID;
	char szBuf[20];
	if( wID == MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED) )
		sprintf_s( szBuf, "zh_CN" );
	else if( wID == MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT) )
		sprintf_s( szBuf, "en_US" );
	else if( wID == MAKELANGID(LANG_RUSSIAN,SUBLANG_DEFAULT) )
		sprintf_s( szBuf, "ru_RU" );
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\Stage\\StageMC6600.ini");
	// 写入配置文件
	WritePrivateProfileString( "StageMC6600Cfg", "Language", szBuf, path );
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
	strcat_s(path, "\\Stage\\StageMC6600.ini");
	char szLang[20];
	if(GetPrivateProfileString("StageMC6600Cfg", "Language", "", szLang, 20, path) != 0)
	{
		exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		if( strcmp( szLang, "zh_CN" ) == 0 )
			strcat_s(path, "\\Locale\\zh\\Stage_zh_CN.ini");
		else if( strcmp( szLang, "en_US" ) == 0 )
			strcat_s(path, "\\Locale\\en\\Stage_en_US.ini");
		else if( strcmp( szLang, "ru_RU" ) == 0 )
			strcat_s(path, "\\Locale\\ru\\Stage_ru_RU.ini");
		strcpy_s( m_cPathLocalize, path );
		char szBuf[255];
		int nLen = 20;
		// 界面元素
		if(GetPrivateProfileString("Label", "Cap", "", szBuf, nLen, path) != 0)
			SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Zero", "", szBuf, nLen, path) != 0)
			GetDlgItem(IDC_BUTTON_Zero)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Move", "", szBuf, nLen, path) != 0)
			GetDlgItem(IDC_BUTTON_MoveTo)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Stop", "", szBuf, nLen, path) != 0)
			GetDlgItem(IDC_BUTTON_Stop)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Relative", "", szBuf, nLen, path) != 0)
			GetDlgItem(IDC_CHECK_Relative)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Setup", "", szBuf, nLen, path) != 0)
			GetDlgItem(IDC_BUTTON_Setup)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Add", "", szBuf, nLen, path) != 0)
			GetDlgItem(IDC_BUTTON_ListAdd)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Delete", "", szBuf, nLen, path) != 0)
			GetDlgItem(IDC_BUTTON_ListDel)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "Change", "", szBuf, nLen, path) != 0)
			GetDlgItem(IDC_BUTTON_ListChange)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "DriIinit ", "", szBuf, nLen, path) != 0)
			GetDlgItem(IDC_BUTTON_DriverInit)->SetWindowText( szBuf );

		// 提示字符串
		nLen = 100;
		if(GetPrivateProfileString("Msg", "MsgCap", "", szBuf, nLen, path) != 0)
			m_strMsg[0] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgFindIndex", "", szBuf, nLen, path) != 0)
			m_strMsg[1] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgCenter", "", szBuf, nLen, path) != 0)
			m_strMsg[2] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgTSafe", "", szBuf, nLen, path) != 0)
			m_strMsg[3] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgSetMechOri ", "", szBuf, nLen, path) != 0)
			m_strMsg[4] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgExtreSafe ", "", szBuf, nLen, path) != 0)
			m_strMsg[5] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgFindIndex+ ", "", szBuf, nLen, path) != 0)
			m_strMsg[6] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgFindIndex- ", "", szBuf, nLen, path) != 0)
			m_strMsg[7] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgGndSafe ", "", szBuf, nLen, path) != 0)
			m_strMsg[8] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgLimitAbnormal ", "", szBuf, nLen, path) != 0)
			m_strMsg[9] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgLimitAbnormal+ ", "", szBuf, nLen, path) != 0)
			m_strMsg[10] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgLimitAbnormal- ", "", szBuf, nLen, path) != 0)
			m_strMsg[11] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgCapWarning", "", szBuf, nLen, path) != 0)
			m_strMsg[14] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgAxisNotInit", "", szBuf, nLen, path) != 0)
			m_strMsg[15] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgAxisAbnormal", "", szBuf, nLen, path) != 0)
			m_strMsg[16] = szBuf;
	}
}

void CPanelStage::Connect()
{
	ZeroMemory(&m_paramsSpeed,sizeof(MC6600_Params_Speed));
	ZeroMemory(&m_paramsCurrent,sizeof(MC6600_Params_Current));
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\Stage\\StageMC6600.ini");

	char szBuf[255], szBufRootKey[20];
	sprintf_s(szBufRootKey, "StageMC6600Cfg");
	// 通讯端口
	m_nPort = 1;
	if(GetPrivateProfileString(szBufRootKey, "Port", "", szBuf, 15, path) != 0)
		m_nPort	= atoi( szBuf );
	int i, nCount = 5;
	//轴向
	char* pchData = new char[nCount *2];
	if( ReadInOneLine(path, szBufRootKey, "Axial", CHARSTYLE, nCount, pchData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramAddr.cAxial[i] = pchData[i];
	}
	delete [] pchData;
	//从站地址
	int* pnData = new int[nCount];
	if( ReadInOneLine(path, szBufRootKey, "Address", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramAddr.byteAddr[i] = pnData[i];
	}
	//驱动方向
	if( ReadInOneLine(path, szBufRootKey, "DriveDir", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramDriveDir.nDir[i] = pnData[i];
	}
	//初始化用
	if( ReadInOneLine(path, szBufRootKey, "AxisInitia", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_bAxisInitia[i] = pnData[i];
	}
	//通讯故障用
	/*if( ReadInOneLine(path, szBufRootKey, "CommunicationFault", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_bCommunicationFault[i] = pnData[i];
	}*/
	//电机故障用
	if( ReadInOneLine(path, szBufRootKey, "MotorFault", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
		{
			m_paramFault.bMotorFault[i] = pnData[i];
			m_bMotorFault[i] = pnData[i];
		}
	}
	delete [] pnData;

	float* pfData = new float[nCount *2];
	//摇杆步距
	if(ReadInOneLine(path, szBufRootKey, "RockerStep", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_fRockerStep[i] = pfData[i];
	}

	//极限方向-A
	if( ReadInOneLine(path, szBufRootKey, "LimitDirA", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramLimitDir.fA[i] = pfData[i];
	}
	//极限方向-B
	if( ReadInOneLine(path, szBufRootKey, "LimitDirB", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramLimitDir.fB[i] = pfData[i];
	}

	//轴向转速
	if( ReadInOneLine(path, szBufRootKey, "Pospeed", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		float fSpeed[5];
		for( i=0; i<nCount; i++ )
		{
			fSpeed[i] = pfData[i];
			fSpeed[i] = CLIP_RANGE(fSpeed[i],m_fMinSpeed[i],m_fMaxSpeed[i]);
			m_paramsSpeed.fSpeedInit[i] = fSpeed[i];
		}
	}
	//Track转速
	if( ReadInOneLine(path, szBufRootKey, "TrackPospeed", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		float fTrackspeed[5];
		for( i=0; i<nCount; i++ )
		{
			fTrackspeed[i] = pfData[i];
			fTrackspeed[i] = CLIP_RANGE(fTrackspeed[i],m_fMinSpeed[i],m_fMaxSpeed[i]);
			m_paramsSpeed.fSpeedTrack[i] = fTrackspeed[i];
		}
	}
	//死区精度
	if( ReadInOneLine(path, szBufRootKey, "DeadLine", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_fDeadline[i] = pfData[i];
	}
	//中心位置
	if( ReadInOneLine(path, szBufRootKey, "Cali", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_fCali[i] = pfData[i];
	}
	// 软限位值
	nCount = 10;
	if( ReadInOneLine(path, szBufRootKey, "Limits", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		int nTotal = nCount /2;
		for( i=0; i<nTotal; i++ )
		{
			m_paramZRes.fLimitNeg[i] = pfData[i *2];
			m_paramZRes.fLimitPos[i] = pfData[i *2 +1];
		}
		for( i=0; i<nCount; i++ )
			m_factualLimitH[i] = pfData[i];
	}
	delete [] pfData;

	//是否有软限位
	if(GetPrivateProfileString(szBufRootKey, "SLimit ", "", szBuf, 255, path) != 0)
		m_bSoftLimit = (BOOL)(atoi(szBuf));
	if(GetPrivateProfileString(szBufRootKey, "Z0wd", "", szBuf, 255, path) != 0)
		m_paramZRes.fZ0wd = (float)atof(szBuf);
	if(GetPrivateProfileString(szBufRootKey, "RTo", "", szBuf, 255, path) != 0)
		m_paramZRes.fTo = (float)atof(szBuf);
	if(GetPrivateProfileString(szBufRootKey, "RInclina", "", szBuf, 255, path) != 0)
		m_paramZRes.fInclina = (float)atof(szBuf);
	if(GetPrivateProfileString(szBufRootKey, "ZSample_H", "", szBuf, 255, path) != 0)
		m_paramZRes.fHs = (float)atof(szBuf);
	// 放气前的选项
	if(GetPrivateProfileString(szBufRootKey, "VentSel", "", szBuf, 255, path) != 0)
		m_nVentSel = atoi(szBuf);

	// 是否有老控制器
	m_paramStyle.bOld = FALSE;
	m_paramStyle.nAxis = 4;
	m_paramStyle.ucAxis = 'T';
	if(GetPrivateProfileString(szBufRootKey, "Style", "", szBuf, 255, path) != 0)
	{
		char szBuf2[50];
		if(strchr(szBuf, ',') != NULL)
		{
			strcpy_s(szBuf2, strchr(szBuf, ',')+1);
			*strchr(szBuf, ',') = '\0';
			m_paramStyle.bOld = (BOOL)(atoi(szBuf));
			m_paramStyle.nAxis = atoi(szBuf2);
			m_paramStyle.ucAxis = SerialNum( m_paramStyle.nAxis );
		}
	}
	// 样品台类型
	m_paramStyle.nType = 0;
	if(GetPrivateProfileString(szBufRootKey, "Type", "", szBuf, 15, path) != 0)
		m_paramStyle.nType = atoi( szBuf );
	if( m_paramStyle.nType > 1 )	// Klein台子的R轴无限位
	{
		GetDlgItem(IDC_BUTTON_R_Move_Minus2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_R_Move_Plus2)->ShowWindow(SW_HIDE);
	}

	//20210618-HY：软限位适用于极限+扫描原点
	CString str;
	if(m_bSoftLimit)
	{
		for(i=0;i<5;i++)
		{
			m_paramZRes.fLimitNeg[i] = m_paramZRes.fLimitNeg[i] - m_fCali[i];
			m_paramZRes.fLimitPos[i] = m_paramZRes.fLimitPos[i] - m_fCali[i];
			m_factualLimit[i*2] = m_factualLimitH[i*2] - m_fCali[i];
			m_factualLimit[i*2+1] = m_factualLimitH[i*2+1] - m_fCali[i];
			str.Format("m_factualLimit[%d]: %.3f, m_factualLimit[%d]: %.3f",i,m_factualLimit[i*2],i+1,m_factualLimit[i*2+1]);
			Message(str);
		}
	}

	theApp.m_StageMC6600.InitHWND( this->m_hWnd );
		
	Localize();
	InitParam_SysParams();
	InitParam_Backlash( path );
	InitParam_ZRestraint( path );
	InitParam_Calibration( path );
	InitParam_SetWizard( path );
	InitParam_SetDefaultPID( path );
	theApp.m_StageMC6600.SlaveAddrSend(m_paramAddr.byteAddr);
	theApp.m_StageMC6600.Pospeed(m_paramsSpeed.fSpeedInit);
	theApp.m_StageMC6600.SetOldStyle( m_paramStyle.bOld, m_paramStyle.ucAxis );
	theApp.m_StageMC6600.Open(m_nPort);
}

void CPanelStage::InitParam_SysParams()
{
	m_dEquivalent[0]	= 1;
	m_dEquivalent[1]	= 1;
	switch(m_paramStyle.nType)
	{
	case	1:
	case	3:
		// large stage
		m_dEquivalent[2]	= 1;
		m_dEquivalent[3]	= 0.1;
		break;
	case	0:
	case	2:
		// small stage
		m_dEquivalent[2]	= 2;
		m_dEquivalent[3]	= 0.2;
		break;
	}
	m_dEquivalent[4]	= 4/9.;
}

void CPanelStage::InitParam_ZRestraint( char* path )
{
	char szBuf[255], szBufRootKey[20];
	sprintf_s(szBufRootKey, "StageMC6600Zres");
	
	float* pfData = new float[3];
	int i, nCount = 2;
	if( ReadInOneLine(path, szBufRootKey, "ResUser", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		// 样品高度H
		m_paramZRes.fHy = pfData[0];
		// 样品半径Ryp
		m_paramZRes.fRy = pfData[1];
	}
	if( ReadInOneLine(path, szBufRootKey, "ResZ", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		// 安全物距
		m_paramZRes.fSafeWD	= pfData[0];
		// 二次电子探头下缘据物镜面Z下沉
		m_paramZRes.fZsink	= pfData[1];
	}
	nCount = 3;
	if( ReadInOneLine(path, szBufRootKey, "ResR", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		// 探头半径
		m_paramZRes.fRtb	= pfData[0];
		// 方形样品座半宽
		m_paramZRes.fRx0	= pfData[1];
		m_paramZRes.fRy0	= pfData[2];
	}
	if( ReadInOneLine(path, szBufRootKey, "ResPosW", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		//物镜坐标
		for( i=0; i<nCount; i++ )
			m_paramZRes.fLeftObjectlow[i] = pfData[i];
	}
	if( ReadInOneLine(path, szBufRootKey, "ResPosT", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		//物镜坐标
		for( i=0; i<nCount; i++ )
			m_paramZRes.fProbeCenCoor[i] = pfData[i];
	}
	delete [] pfData;

	if(GetPrivateProfileString(szBufRootKey, "ResT", "", szBuf, 255, path) != 0)
		m_paramZRes.fHt = (float)atof(szBuf);

	// Z标定 = Z0wd -safeWD -Hy
	m_paramZRes.fCali[2] = m_paramZRes.fZ0wd -m_paramZRes.fSafeWD -m_paramZRes.fHy;
	// Zbr1 = Zbr1 = fT0 -Hy
	m_paramZRes.fZbr[1] = m_paramZRes.fTo -m_paramZRes.fHy ;
	// Zbr0 = fZ0wd -Hy -Zsink
	m_paramZRes.fZbr[0] = m_paramZRes.fZ0wd  -m_paramZRes.fHy -m_paramZRes.fZsink;
}

void CPanelStage::CoherenceCheck_Z( float& fXExp, float& fYExp, float fZExp, BOOL bAfter )
{
	// 距离机械原点的偏移量
	float fZExpAct = (float)(fZExp +m_fCali[2]+m_paramZRes.fRy*sin(m_paramZRes.fTa)+m_paramZRes.fHy);
	// 判断处于哪个避让面
	CString str;
	if(fZExpAct < m_paramZRes.fZbr[0])  // 无需避让
	{
		m_factualLimit[0] = m_paramZRes.fLimitNeg[0];
		m_factualLimit[2] = m_paramZRes.fLimitNeg[1];
	
	    m_factualLimit[1] = m_paramZRes.fLimitPos[0];
	    m_factualLimit[3] = m_paramZRes.fLimitPos[1];

		if( fXExp > m_factualLimit[0] )
		{
			fXExp = m_factualLimit[0];
		}
		if( fYExp > m_factualLimit[2] )
		{
			fYExp = m_factualLimit[2];
		}
		if( fXExp > m_factualLimit[0] || fYExp > m_factualLimit[2] )
		{
			str.Format("X/Y out of bounds,need to move XY to: %.2f, %.2f",fXExp,fYExp);
			Message(str);
		}
		str.Format("Need to move XY to: %.2f, %.2f",fXExp,fYExp);
		Message(str);
	}
	else if( fZExpAct >  m_paramZRes.fZbr[0] &&  fZExpAct < m_paramZRes.fProbeCenCoor[2])
	{
		// 第一避让面
		if(m_paramZRes.fTa <= 0)
		{
		}
	}
	else if( fZExpAct > m_paramZRes.fProbeCenCoor[2] && fZExpAct < m_paramZRes.fCali[2])
	{
	}
	else
	{
		CString str;
		str.Format("Z out of bounds!");
		Message(str);
	}
}

void CPanelStage::InitParam_Backlash( char* path )
{
	ZeroMemory(&m_paramBl, sizeof(MC6600_Params_Bl));
	char szBuf[255], szBufRootKey[20];
	sprintf_s(szBufRootKey, "StageMC6600Cfg");

	short i, nCount = 5;
	int* pnData = new int[nCount];
	float* pfData = new float[nCount];
	// 消间隙方向
	if( ReadInOneLine(path, szBufRootKey, "BlDir", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramBl.nDir[i] = (short)pnData[i];
	}
	// 回程系数
	if( ReadInOneLine(path, szBufRootKey, "BlRetCoefficient", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramBl.nCoeff[i] = pnData[i];
	}
	// 间隙量
	if( ReadInOneLine(path, szBufRootKey, "BlCount", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramBl.fCount[i] = (float)(pfData[i] * m_paramBl.nCoeff[i] *m_dEquivalent[i]);
	}
	delete [] pnData;
	delete [] pfData;

	// 双击和跟踪时是否消间隙
	if(GetPrivateProfileString(szBufRootKey, "BlClick", "", szBuf, 255, path) != 0)
		m_bBlClick = (BOOL)(atoi(szBuf));
	if(GetPrivateProfileString(szBufRootKey, "BlTrack", "", szBuf, 255, path) != 0)
		m_bBlTrack = (BOOL)(atoi(szBuf));
	sprintf_s( szBuf, "Backlash X: %d, %lf", m_paramBl.nDir[0], m_paramBl.fCount[0] );
	Message(szBuf);
	sprintf_s( szBuf, "Backlash Y: %d, %lf", m_paramBl.nDir[1], m_paramBl.fCount[1] );
	Message(szBuf);
	sprintf_s( szBuf, "Backlash Z: %d, %lf", m_paramBl.nDir[2], m_paramBl.fCount[2] );
	Message(szBuf);
	sprintf_s( szBuf, "Backlash R: %d, %lf", m_paramBl.nDir[3], m_paramBl.fCount[3] );
	Message(szBuf);
	sprintf_s( szBuf, "Backlash T: %d, %lf\r\n", m_paramBl.nDir[4], m_paramBl.fCount[4] );
	Message(szBuf);
// 	sprintf_s( szBuf, "Backlash: click=%d,track=%d", m_bBlClick, m_bBlTrack );
// 	Message(szBuf);
// 	sprintf_s( szBuf, "Backlash: click=%d,track=%d", m_bBlClick, m_bBlTrack );
//	Message(szBuf);
	for( i=0; i<5; i++ )
		theApp.m_StageMC6600.SetBacklashParams( i, m_paramBl.nDir[i], m_paramBl.fCount[i] );
}

void CPanelStage::InitParam_Calibration( char* path )
{
	char szBuf[255], szBufRootKey[20];
	sprintf_s(szBufRootKey, "StageMC6600Cfg");

	m_fCalibrationZ =3;
	if(GetPrivateProfileString(szBufRootKey, "CalibrationZ0 ", "", szBuf, 15, path) != 0)
		m_fCalibrationZ	= (float)atof( szBuf );

	m_fCalibrationR =30;
	if(GetPrivateProfileString(szBufRootKey, "CalibrationR0 ", "", szBuf, 15, path) != 0)
		m_fCalibrationR	= (float)atof( szBuf );

	m_fCalibrationT =6;
	if(GetPrivateProfileString(szBufRootKey, "CalibrationT0 ", "", szBuf, 15, path) != 0)
		m_fCalibrationT	= (float)atof( szBuf );
}

void CPanelStage::InitParam_SetWizard( char* path )
{
	char szBufRootKey[20];
	sprintf_s(szBufRootKey, "StageMC6600Cfg");

	short i, nCount = 5;
	int* pnData = new int[nCount];
	// 电机类型
	if( ReadInOneLine(path, szBufRootKey, "MotorType", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramWizard.nSetMotorType[i] = pnData[i];
	}
	// 编码器线数
	if( ReadInOneLine(path, szBufRootKey, "EncoderLines", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramWizard.nEncoderLines[i] = pnData[i];
	}
	// 控制方式
	if( ReadInOneLine(path, szBufRootKey, "ControlMode", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramWizard.nControlMode[i] = pnData[i];
	}
	// 控制信号
	if( ReadInOneLine(path, szBufRootKey, "ControlSignal", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramWizard.nControlSignal[i] = pnData[i];
	}
	// 最大电流
	if( ReadInOneLine(path, szBufRootKey, "MaxCurrent", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramWizard.nMaxCurrent[i] = pnData[i];
	}
	delete [] pnData;
}

void CPanelStage::InitParam_SetDefaultPID( char* path )
{
	char szBufRootKey[20];
	sprintf_s(szBufRootKey, "StageMC6600Cfg");

	short i, nCount = 5;
	float* pfData = new float[nCount];
	// PID默认参数
	// P
	if( ReadInOneLine(path, szBufRootKey, "DefaultP", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
		{
			m_paramDefaultPID.fP[i] = pfData[i];
			m_paramCurPID.fP[i] = 0.0f;
		}
	}
	// I
	if( ReadInOneLine(path, szBufRootKey, "DefaultI", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
		{
			m_paramDefaultPID.fI[i] = pfData[i];
			m_paramCurPID.fI[i] = 0.0f;
		}
	}
	// D
	if( ReadInOneLine(path, szBufRootKey, "DefaultD", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
		{
			m_paramDefaultPID.fD[i] = pfData[i];
			m_paramCurPID.fD[i] = 0.0f;
		}
	}
	delete [] pfData;
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

	// 设置StageMC6600日志文件
	str.Format( "%s\\Log\\StageMC6600-%s.log", path, strTemp );
	if( m_hFileLog != NULL )
		CloseHandle( m_hFileLog );
	m_hFileLog	= NULL;
	m_hFileLog = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	m_nLoopsLog = 1;
	// 写StageMC6600日志文件头
	if( m_hFileLog != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		str.Format( "\t%s\r\n", "Log StageMC6600" );
		sprintf_s( Buf, "%s\r\n", str );
		WriteFile( m_hFileLog, Buf, strlen(Buf), &cbRet, NULL );
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
	GetDlgItem(IDC_CHECK_Advance)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DriverInit)->EnableWindow(TRUE);
	if( !bEnable )
		return;
	for(int i=0;i<5;i++)
		if((!m_bCommunicationFault[i]) || (!m_bMotorFault[i]))
		{
			GetDlgItem(IDC_EDIT_X_UserDisp+i)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_X_Move_Minus+i)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_X_Move_Plus+i)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_X_Move_Minus2+i)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_X_Move_Plus2+i)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_HomeX+i)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ZeroX+i)->EnableWindow(FALSE);
		}
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
	char Buf[1024] = {0};
	CString str;
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
	str.Format( "[%s],%s", csCurrentTime, lpszMessage );

	sprintf_s(Buf, "%s\r\n", str );
	WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);

	m_nLoopsLog++;
	if( m_nLoopsLog > 50000 )
	{
		CloseHandle( m_hFileLog );
		InitLog();
	}
}

LRESULT CPanelStage::OnUpdateMC6600Value(WPARAM wParam, LPARAM lParam)
{
	CString str;
	switch( wParam )
	{
	case	0:
		m_bIsMC6600Ready = (BOOL)lParam;
		if(m_bIsMC6600Ready)
		{
			for(int i=0;i<5;i++)
				m_bEnable[i] = TRUE;
			SetStatus(TRUE);
			InitLog();
			str.Format("Open MC6600 COM %d successfully!", m_nPort);
		}
		else
		{   
			for(int i=0;i<5;i++)
			{
				theApp.m_StageMC6600.close(i);
				m_bEnable[i] = FALSE;
			}
			SetStatus(FALSE);
			m_bIsMC6600Ready = FALSE;
			str.Format("Open MC6600 COM %d Failed!",m_nPort);
			::PostMessage( GetParent()->m_hWnd, WM_USER_MC6600_UpdateValue, 0, 0 );
		}
		Message(str);
		break;
	
	case	1:
	case	2:
	case	3:
	case	4:
	case	5:
		{
			short anxi = (short)wParam -1;
			//GetCurStep(anxi);
			if( anxi < 3)
				str.Format( "%.3f", m_fCurStep[anxi]/m_dEquivalent[anxi] );
			else
				str.Format( "%.2f", m_fCurStep[anxi]/m_dEquivalent[anxi] );
			GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText(str);
			GetDlgItem(IDC_EDIT_X_CurStep +anxi)->Invalidate();
		}
		break;
	case  6:
		// 读取到Params_Pos
		m_paramsCurrent = *((MC6600_Params_Current*)lParam);
		SyncParamsPos();
		FindIndex();
		break;
	case 7:
		m_paramFault = *((MC6600_Params_Fault*)lParam);
		for(int i=0;i<5;i++)
			m_bCommunicationFault[i] = m_paramFault.bCommunicationFault[i];
		SaveToINI(5);
		break;

	case	11:
		m_paramsCurrent = *((MC6600_Params_Current*)lParam);
		SyncParamsPos();
		break;
	case 12:
		m_paramCurPID = *((MC6600_Params_PID*)lParam);
		break;
	}
	return 0;
}

void CPanelStage::SyncParamsPos()
{
	CString str;
	//str.Format("X pos = %.3f mm", m_paramsCurrent.fSteps[0] /m_dEquivalent[0] * m_paramDriveDir.nDir[0]);
	//Message(str);
	//str.Format("Y pos = %.3f mm", m_paramsCurrent.fSteps[1] /m_dEquivalent[1] * m_paramDriveDir.nDir[1]);
	//Message(str);
	//str.Format("Z pos = %.3f mm", m_paramsCurrent.fSteps[2] /m_dEquivalent[2] * m_paramDriveDir.nDir[2]);
	//Message(str);
	//str.Format("R pos = %.1f deg", m_paramsCurrent.fSteps[3] /m_dEquivalent[3] * m_paramDriveDir.nDir[3]);
	//Message(str);
	//str.Format("T pos = %.1f deg\r\n", m_paramsCurrent.fSteps[4] /m_dEquivalent[4] * m_paramDriveDir.nDir[4]);
	//Message(str);

	// 运动时，或者某轴手动模式下，需要及时更新位置值
	char szBuf[255] = {0};
	unsigned short ucAxis;
	int i=0;
	for(i=0; i<5; i++ )
	{
		m_fCurStep[i] = m_paramsCurrent.fSteps[i];
	}
	for(i=0; i<5; i++ )
	{
    	if( m_bIsMoving[i] )
		{
			if(m_bHighPrecCount)
			{
				if( i<3 )
					sprintf_s( szBuf, "%.3f, %.5f", m_paramsCurrent.fSteps[i] /m_dEquivalent[i] * m_paramDriveDir.nDir[i],m_paramsCurrent.fSteps[i] /m_dEquivalent[i] * m_paramDriveDir.nDir[i]);
				else
					sprintf_s( szBuf, "%.2f, %.5f", m_paramsCurrent.fSteps[i] /m_dEquivalent[i] * m_paramDriveDir.nDir[i],m_paramsCurrent.fSteps[i] /m_dEquivalent[i] * m_paramDriveDir.nDir[i]);
			}
			else
			{
				if( i<3 )
					sprintf_s( szBuf, "%.3f", m_paramsCurrent.fSteps[i] /m_dEquivalent[i] * m_paramDriveDir.nDir[i]);
				else
					sprintf_s( szBuf, "%.2f", m_paramsCurrent.fSteps[i] /m_dEquivalent[i] * m_paramDriveDir.nDir[i]);
			}

			GetDlgItem( IDC_EDIT_X_CurStep +i )->SetWindowText( szBuf );
			ucAxis = SerialNum(i);
			/*str.Format("%c pos = %.3f mm", ucAxis,m_paramsCurrent.fSteps[i] /m_dEquivalent[i] * m_paramDriveDir.nDir[i]);
			Message(str);*/
		}
	}
}

float CPanelStage::RDriveConversion(float fRCurrent, float fRTarget)
{
	float fPosDiff1 = 0;
	float fPosDiff2 = 0;
	float fRet = fRTarget;
	fPosDiff1 = fabs(fRTarget - fRCurrent);
	fPosDiff2 = fRTarget - fRCurrent; 
	if(fPosDiff1 > 180*m_dEquivalent[3])
	{
		if(fPosDiff2 > 180*m_dEquivalent[3])
			fRet = fRTarget - 360*m_dEquivalent[3];
		else
			fRet = fRTarget + 360*m_dEquivalent[3];
	}
	return fRet;
}

CString CPanelStage::RDriveConversion_Disp( float fJudge, float fFormat )
{
	// fJudge为判断条件时使用，fFormat为格式化字符串时使用
	CString str;
	short anxi = 3;
	if(m_bHighPrecCount)
	{
		if(fJudge/m_dEquivalent[anxi] > 179)
			str.Format( "%.2f, %.5f", (fFormat/m_dEquivalent[anxi] - 360) *m_paramDriveDir.nDir[anxi], (fFormat/m_dEquivalent[anxi] - 360) *m_paramDriveDir.nDir[anxi] );
		else if(fJudge/m_dEquivalent[anxi] < -179)
			str.Format( "%.2f, %.5f", (fFormat/m_dEquivalent[anxi] + 360) *m_paramDriveDir.nDir[anxi], (fFormat/m_dEquivalent[anxi] + 360) *m_paramDriveDir.nDir[anxi] );
		else if(fJudge/m_dEquivalent[anxi] >359)
			str.Format( "%d, %d", (int)(fFormat/m_dEquivalent[anxi])% 360  *m_paramDriveDir.nDir[anxi], (int)(fFormat/m_dEquivalent[anxi])% 360 *m_paramDriveDir.nDir[anxi] );
		else if(fJudge/m_dEquivalent[anxi] <-359)
			str.Format( "%d, %d", (int)(fFormat/m_dEquivalent[anxi])% (-360) *m_paramDriveDir.nDir[anxi], (int)(fFormat/m_dEquivalent[anxi])% (-360) *m_paramDriveDir.nDir[anxi] );
		else
			str.Format( "%.2f, %.5f", fFormat/m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi], fFormat/m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi] );
	}
	else
	{
		if(fJudge/m_dEquivalent[anxi] > 179)
			str.Format( "%.2f", (fFormat/m_dEquivalent[anxi]- 360) *m_paramDriveDir.nDir[anxi] );
		else if(fJudge/m_dEquivalent[anxi] < -179)
			str.Format( "%.2f", (fFormat/m_dEquivalent[anxi]+ 360) *m_paramDriveDir.nDir[anxi] );
		else if(fJudge/m_dEquivalent[anxi] > 359)
			str.Format( "%d", int(fFormat/m_dEquivalent[anxi])% 360 *m_paramDriveDir.nDir[anxi] );
		else if(fJudge/m_dEquivalent[anxi] < -359)
			str.Format( "%d", int(fFormat/m_dEquivalent[anxi])% (-360) *m_paramDriveDir.nDir[anxi] );
		else
			str.Format( "%.2f", fFormat/m_dEquivalent[anxi] );
	}
	return str;
}

CString CPanelStage::DisplayOutput(short anxi, float fValue, float fGoal)
{
	CString str;
	float fShowPos1 = fValue / m_dEquivalent[anxi] * m_paramDriveDir.nDir[anxi];
	float fShowPos2 = fGoal / m_dEquivalent[anxi];
	float fShowPos3 = fValue / m_dEquivalent[anxi];

	if(m_bHighPrecCount)
	{
		if(anxi < 3)
			str.Format( "%.3f, %.5f", fShowPos1, fShowPos1 );
		else
			str.Format( "%.2f, %.5f",fShowPos1, fShowPos1);
	}
	else
	{
		if(anxi < 3)
			str.Format( "%.3f", fShowPos1 );
		else
			str.Format( "%.2f",fShowPos1);
	}

	return str;
}

void CPanelStage::MoveMinusPlus( int nIndex )
{
	// XY轴单步走
	char szBuf[255];
	// 像素距离
	int nLength;
	short anxi;
	switch( nIndex )
	{
	case	0:	// X Minus
	case	1:	// X Plus
		nLength = m_sizeReso.cx /4;
		anxi = 0;
		break;
	case	2:	// Y Minus
	case	3:	// Y Plus
		nLength = m_sizeReso.cy /4;
		anxi = 1;
		break;
	case	4:	// Z Minus
	case	5:	// Z Plus
		anxi = 2;
		break;
	case	6:	// R Minus
	case	7:	// R Plus
		anxi = 3;
		break;
	case	8:	// T Minus
	case	9:	// T Plus
		anxi = 4;
		break;
	}
	unsigned char ucAxis = SerialNum( anxi );
	if(m_bMotoronoff[anxi] || m_bMotoronoffAll)
	{
		if(m_bMotoronoff[anxi])
		{
			m_bMotoronoff[anxi] = FALSE;
			MantoAuto(anxi);
		}
		else if(m_bMotoronoffAll)
		{
			m_bMotoronoffAll = FALSE;
			MantoAutoAll();
		}
	}
	else if(!m_bEnable[anxi])
	{
		theApp.m_StageMC6600.Enable(anxi);
		m_bEnable[anxi] = TRUE;
	}
	// 实际码数
	float fMoveStep, fScale = 0;
	if( nIndex < 4 )	// X & Y
		CalculateStageUScale( 0, nLength, fScale );
	switch( nIndex )
	{
	case	0:	// X Minus
	case	2:	// Y Minus
		sprintf_s( szBuf, "%c move negative %.3f mm", ucAxis, fScale/m_dEquivalent[anxi] );
		m_bDirection[anxi] = FALSE;
		break;
	case	1:	// X Plus
	case	3:	// Y Plus
		sprintf_s( szBuf, "%c move positive %.3f mm", ucAxis, fScale/m_dEquivalent[anxi] );
		m_bDirection[anxi] = TRUE;
		break;
	case	4:	// Z Minus
		sprintf_s( szBuf, "%c move negative %.3f mm", ucAxis, 0.005/m_dEquivalent[anxi] );
		m_bDirection[anxi] = FALSE;
		break;
	case	6:	// R Minus
	case	8:	// T Minus
		sprintf_s( szBuf, "%c move negative %.3f mm", ucAxis, 1/m_dEquivalent[anxi] );
		m_bDirection[anxi] = FALSE;
		break;
	case	5:	// Z Plus
		sprintf_s( szBuf, "%c move positive %.3f mm", ucAxis, 0.005/m_dEquivalent[anxi] );
		m_bDirection[anxi] = TRUE;
		break;
	case	7:	// R Plus
	case	9:	// T Plus
		sprintf_s( szBuf, "%c move positive %.3f mm", ucAxis, 1/m_dEquivalent[anxi] );
		m_bDirection[anxi] = TRUE;
		break;
	}
	Message(szBuf);
	if( m_paramDriveDir.nDir[anxi] < 0 )
	{
		if(m_bDirection[anxi])
			m_bDirection[anxi] = FALSE;
		else
			m_bDirection[anxi] = TRUE;
	}
	switch( nIndex )
	{
	case	1:	// X Plus
	case	2:	// Y Minus
		fMoveStep = fScale *m_paramDriveDir.nDir[anxi];
		break;
	case	0:	// X Minus
	case	3:	// Y Plus
		fMoveStep = (0 -fScale) *m_paramDriveDir.nDir[anxi];
		break;
	case	4:	// Z Minus
		fMoveStep = -0.005 *m_paramDriveDir.nDir[anxi];
		break;
	case	6:	// R Minus
	case	8:	// T Minus
		fMoveStep = -1.0 *m_paramDriveDir.nDir[anxi];
		break;
	case	5:	// Z Plus
		fMoveStep = 0.005 *m_paramDriveDir.nDir[anxi];
		break;
	case	7:	// R Plus
	case	9:	// T Plus
		fMoveStep = 1.0 *m_paramDriveDir.nDir[anxi];
		break;
	}
	if(fMoveStep > 0)
	{
		if(m_paramBl.nDir[anxi] > 0 && NotZero(m_paramBl.fCount[anxi]) )
			m_bBL[anxi]=TRUE;
	}
	else
	{
		if(m_paramBl.nDir[anxi] < 0 && NotZero(m_paramBl.fCount[anxi]) )
			m_bBL[anxi]=TRUE;
	}
	fMoveStep += m_fCurStep[anxi];
	/*if(anxi == 3)
		fMoveStep = RDriveConversion(m_fCurStep[anxi],fMoveStep);*/
	m_fGoal[anxi] = fMoveStep;
	if( nIndex < 6 )	// XYZ
		sprintf_s( szBuf, "%.3f", fMoveStep/m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi] );
	else
		sprintf_s( szBuf, "%.2f", fMoveStep/m_dEquivalent[anxi]*m_paramDriveDir.nDir[anxi] );
	GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText(szBuf);
	m_bIsMoving[anxi] = TRUE;
	CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
	m_fBLCount[anxi] = theApp.m_StageMC6600.MoveDir(ucAxis, fMoveStep);
// 	theApp.m_StageMC6600.Move( ucAxis, fMoveStep, m_bRelative);
// 	m_bIsMoving = FALSE;
// 	theApp.m_StageMC6600.SendCmd(cmd , ucAxis);
	m_nTimer[anxi] = SetTimer(anxi +1, nStageMC6600_Elapse, NULL);
}

void CPanelStage::OnMoveMinusChanged(UINT nID)
{
	MoveMinusPlus( (nID -IDC_BUTTON_X_Move_Minus) *2 );
}

void CPanelStage::OnMovePlusChanged(UINT nID)
{
	MoveMinusPlus( (nID -IDC_BUTTON_X_Move_Plus) *2 +1 );
}

void CPanelStage::MoveContinues( int nIndex )
{
	short anxi;
	switch( nIndex )
	{
	case	0:	// X Minus 2
	case	1:	// X Plus 2
		anxi = 0;
		break;
	case	2:	// Y Minus 2
	case	3:	// Y Plus 2
		anxi = 1;
		break;
	case	4:	// Z Minus 2
	case	5:	// Z Plus 2
		anxi = 2;
		break;
	case	6:	// R Minus 2
	case	7:	// R Plus 2
		anxi = 3;
		break;
	case	8:	// T Minus 2
	case	9:	// T Plus 2
		anxi = 4;
		break;
	}
	if(m_bMotoronoff[anxi] || m_bMotoronoffAll)
	{
		if(m_bMotoronoff[anxi])
		{
			m_bMotoronoff[anxi] = FALSE;
			MantoAuto(anxi);
		}
		else if(m_bMotoronoffAll)
		{
			m_bMotoronoffAll = FALSE;
			MantoAutoAll();
		}
	}
	else if(!m_bEnable[anxi])
	{
		theApp.m_StageMC6600.Enable(anxi);
		m_bEnable[anxi] = TRUE;
	}
	unsigned char ucAxis = SerialNum( anxi );
	char szBuf[255];
	switch( nIndex )
	{
	case	0:	// X Minus
	case	2:	// Y Minus
	case	4:	// Z Minus
	case	6:	// R Minus
	case	8:	// T Minus
		if(m_paramDriveDir.nDir[anxi] > 0)
			m_bDirection[anxi] = FALSE;
		else
			m_bDirection[anxi] = TRUE;
		sprintf_s( szBuf, "%c continues to move in a negative dir", ucAxis );
		break;
	case	1:	// X Plus
	case	3:	// Y Plus
	case	5:	// Z Plus
	case	7:	// R Plus
	case	9:	// T Plus
		if(m_paramDriveDir.nDir[anxi] > 0)
			m_bDirection[anxi] = TRUE;
		else
			m_bDirection[anxi] = FALSE;
		sprintf_s( szBuf, "%c continues to move in a positive dir", ucAxis );
		break;
	}
	Message( szBuf );
	m_bIsMoving[anxi] = TRUE;
	CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
	if( m_bDirection[anxi] )
		m_fGoal[anxi] = 1000;
	else
		m_fGoal[anxi] = -1000;
	theApp.m_StageMC6600.MoveLimit(ucAxis, m_fGoal[anxi]);
	
// 	m_fGoal[anxi] = -28;
// 	CString str;
// 	str.Format( " %f", m_fGoal[anxi]/m_dEquivalent[anxi] );
// 	GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText(str);
	m_nTimer[anxi] = SetTimer( anxi +1, nStageMC6600_Elapse, NULL );
}

void CPanelStage::OnMoveMinus2Changed(UINT nID)
{
	MoveContinues( (nID -IDC_BUTTON_X_Move_Minus2) *2 );
}

void CPanelStage::OnMovePlus2Changed(UINT nID)
{
	MoveContinues( (nID -IDC_BUTTON_X_Move_Plus2) *2 +1 );
}

void CPanelStage::HomeAxis( short anxi )
{
	if(m_bMotoronoff[anxi] || m_bMotoronoffAll)
	{
		if(m_bMotoronoff[anxi])
		{
			m_bMotoronoff[anxi] = FALSE;
			MantoAuto(anxi);
		}
		else if(m_bMotoronoffAll)
		{
			m_bMotoronoffAll = FALSE;
			MantoAutoAll();
		}
	}
	//确保处于使能状态
	else if(!m_bEnable[anxi])
	{
		theApp.m_StageMC6600.Enable(anxi);
		m_bEnable[anxi] = TRUE;
	}
	unsigned char ucAxis = SerialNum( anxi );
	char szBuf[255];
	sprintf_s( szBuf, "GoHome %c", ucAxis );
	Message( szBuf );
	m_bIsMoving[anxi]	= TRUE;
	CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
	m_bToHome[anxi]		= TRUE;
	//GetCurStep(anxi);
	theApp.m_StageMC6600.LimRelease(1, ucAxis);

	float fSteps = 0.0f;
	switch( anxi )
	{
	case	0:	// X
		m_nPhaseInit[anxi] = 0;
		fSteps = 200.0 *m_paramDriveDir.nDir[anxi];
		break;
	case	1:	// Y
		m_nPhaseInit[anxi] = 0;
		fSteps = 200.0 *m_paramDriveDir.nDir[anxi];
		break;
	case	2:	// Z
		m_nPhaseInit[anxi] = 1;
		fSteps = -100.0 *m_paramDriveDir.nDir[anxi];
		break;
	case	3:	// R
		m_nPhaseInit[anxi] = 1;
		fSteps = -100.0 *m_paramDriveDir.nDir[anxi];
		break;
	case	4:	// T
		m_nPhaseInit[anxi] = 1;
		fSteps = -100.0 *m_paramDriveDir.nDir[anxi];
		break;
	}
	theApp.m_StageMC6600.GoHome(ucAxis, fSteps);
	sprintf_s( szBuf, "GoHome %c, fSteps %.5f", ucAxis,fSteps*m_paramDriveDir.nDir[anxi]);
	Message( szBuf );
	m_nTimer[anxi] = SetTimer( anxi +1, nStageMC6600_Elapse, NULL );
}

void CPanelStage::OnHomeChanged(UINT nID)
{
	HomeAxis( nID -IDC_BUTTON_HomeX );
}

void CPanelStage::ZeroAxis( short anxi )
{
	if(m_bMotoronoff[anxi] || m_bMotoronoffAll)
	{
		if(m_bMotoronoff[anxi])
		{
			m_bMotoronoff[anxi] = FALSE;
			MantoAuto(anxi);
		}
		else if(m_bMotoronoffAll)
		{
			m_bMotoronoffAll = FALSE;
			MantoAutoAll();
		}
	}
	else if(!m_bEnable[anxi])
	{
		theApp.m_StageMC6600.Enable(anxi);
		m_bEnable[anxi] = TRUE;
	}
	unsigned char ucAxis = SerialNum( anxi );
	char szBuf[255];
	sprintf_s( szBuf, "GoOrigion %c", ucAxis );
	Message( szBuf );
	m_bOrigion[anxi]	= TRUE;
	m_bToHome[anxi]		= TRUE;
	m_bIsMoving[anxi]	= TRUE;
	CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
	//GetCurStep(anxi);
	theApp.m_StageMC6600.LimRelease(1, ucAxis);

	float fSteps = 0.0f;
	switch( anxi )
	{
	case	0:	// X
		m_nPhaseInit[anxi] = 0;
		fSteps = 200.0 *m_paramDriveDir.nDir[anxi];
		break;
	case	1:	// Y
		m_nPhaseInit[anxi] = 0;
		fSteps = 200.0 *m_paramDriveDir.nDir[anxi];
		break;
	case	2:	// Z
		m_nPhaseInit[anxi] = 1;
		fSteps = -100.0 *m_paramDriveDir.nDir[anxi];
		break;
	case	3:	// R
		m_nPhaseInit[anxi] = 1;
		fSteps = -100.0 *m_paramDriveDir.nDir[anxi];
		break;
	case	4:	// T
		m_nPhaseInit[anxi] = 1;
		fSteps = -100.0 *m_paramDriveDir.nDir[anxi];
		break;
	}
	theApp.m_StageMC6600.GoHome(ucAxis, fSteps);
	m_nTimer[anxi] = SetTimer( anxi +1, nStageMC6600_Elapse, NULL );
}

void CPanelStage::OnZeroChanged(UINT nID)
{
	ZeroAxis( nID -IDC_BUTTON_ZeroX );
}

void CPanelStage::MantoAuto(short anxi)
{
	//切换控件状态
	int state = ((CButton*)GetDlgItem(IDC_CHECK_X))->GetCheck();
	((CButton*)GetDlgItem(IDC_CHECK_X))->SetCheck(!state);
	unsigned short ucAxis = SerialNum(anxi);
	theApp.m_StageMC6600.Move(ucAxis,m_fCurStep[anxi],m_bRelative);
	m_fCurStep[anxi] = m_fGoal[anxi]/m_paramDriveDir.nDir[anxi];
	KillTimer(anxi+6);
	m_nTimer[anxi] = 0;
	Sleep(100);
	theApp.m_StageMC6600.Enable(anxi);
	m_bEnable[anxi] = TRUE;
	GetDlgItem(IDC_EDIT_X_CurStep +anxi)->Invalidate();	
	CString str;
	if( anxi < 3 )
		str.Format( "%.3f", m_fGoal[anxi]/m_dEquivalent[anxi] );
	else
		str.Format( "%.2f", m_fGoal[anxi]/m_dEquivalent[anxi] );
	GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText(str);
	if(anxi == 3)	//限位无效
		theApp.m_StageMC6600.LimRelease(1,'R');
}

void CPanelStage::MantoAutoAll()
{
	int i =0;
	for(i=0;i<5;i++)
	{
		KillTimer(i+6);
		m_nTimer[i] = 0;
		//theApp.m_StageMC6600.CurrPosR(i);
		int state = ((CButton*)GetDlgItem(IDC_CHECK_X+i))->GetCheck();
		((CButton*)GetDlgItem(IDC_CHECK_X+i))->SetCheck(!state);

		unsigned short ucAxis = SerialNum(i);
		theApp.m_StageMC6600.Move(ucAxis,m_fCurStep[i],m_bRelative);
		Sleep(100);
		theApp.m_StageMC6600.Enable(i);
		m_bEnable[i] = TRUE;
		GetDlgItem(IDC_EDIT_X_CurStep +i)->Invalidate();	
		CString str;
		if( i < 3 )
			str.Format( "%.3f", m_fGoal[i]/m_dEquivalent[i] );
		else
			str.Format( "%.2f", m_fGoal[i]/m_dEquivalent[i] );
		GetDlgItem(IDC_EDIT_X_UserDisp +i)->SetWindowText(str);
	}
	theApp.m_StageMC6600.LimRelease(1,'R');
}

void CPanelStage::JudgeMotorOff(short anxi,BOOL bMotoronoff)
{
	if(bMotoronoff)
	{
		if(anxi == 3)	//限位有效
			theApp.m_StageMC6600.LimRelease(0,'R');

		theApp.m_StageMC6600.close(anxi);
		m_bEnable[anxi] = FALSE;
		CStageMC6600Manager::Instance().m_bIsMoving[anxi] = TRUE;
		m_nTimer[anxi] = SetTimer(anxi+6,nStageMC6600_Elapse,NULL);
	}
	else
	{
		//theApp.m_StageMC6600.CurrPosR(anxi);
		unsigned short ucAxis = SerialNum(anxi);
		theApp.m_StageMC6600.Move(ucAxis,m_fCurStep[anxi],m_bRelative);
		m_fCurStep[anxi] = m_fGoal[anxi]/m_paramDriveDir.nDir[anxi];
		KillTimer(anxi+6);
		m_nTimer[anxi] = 0;
		Sleep(100);
		theApp.m_StageMC6600.Enable(anxi);
		m_bEnable[anxi] = TRUE;
		GetDlgItem(IDC_EDIT_X_CurStep +anxi)->Invalidate();	
		CString str;
		if( anxi < 3 )
			str.Format( "%.3f", m_fGoal[anxi]/m_dEquivalent[anxi] );
		else
			str.Format( "%.2f", m_fGoal[anxi]/m_dEquivalent[anxi] );

		GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText(str);
		if(anxi == 3)	//限位无效
			theApp.m_StageMC6600.LimRelease(1,'R');
	}
}

void CPanelStage::MotorOnOff( short anxi )
{
	m_bMotoronoff[anxi] = !m_bMotoronoff[anxi];
	JudgeMotorOff(anxi,m_bMotoronoff[anxi]);
}

void CPanelStage::OnMotorChanged(UINT nID)
{
	MotorOnOff(nID - IDC_CHECK_X);
}

void CPanelStage::MoveAxis_FromThird( short anxi, float fStep )
{
	MoveAxis( anxi, fStep *m_paramDriveDir.nDir[anxi] );
	char szBuf[255];
	unsigned char ucAxis = SerialNum( anxi );
	sprintf_s( szBuf, "Move %c Axis from third: %.3f", fStep *m_paramDriveDir.nDir[anxi] );
	Message( szBuf );
}

void CPanelStage::MoveAxis( short anxi, float fStep )
{
	if(m_bMotoronoff[anxi] || m_bMotoronoffAll)
	{
		if(m_bMotoronoff[anxi] )
		{
			m_bMotoronoff[anxi] = FALSE;
			MantoAuto(anxi);
		}
		else if(m_bMotoronoffAll)
		{
			m_bMotoronoffAll = FALSE;
			MantoAutoAll();
		}
	}
	else if(!m_bEnable[anxi])
	{
		theApp.m_StageMC6600.Enable(anxi);
		m_bEnable[anxi] = TRUE;
		Sleep(100);
	}
	unsigned char ucAxis = SerialNum( anxi );
	CString str;
	str.Format( "Move %c", ucAxis );
	Message( str );

	m_fGoal[anxi] = fStep;
	if( anxi < 3 )
		str.Format( "%.3f", fStep /m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi] );
	else
		str.Format( "%.2f", fStep/m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi] );
	GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText(str);
	m_bIsMoving[anxi] = TRUE;
	CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
	if( anxi < 2 )	// XY 轴
		m_fSLength[anxi] = fStep;
	if(m_bSoftLimit)
	{
		m_fSLimit[anxi *2] = (float)(m_factualLimit[anxi *2]* m_dEquivalent[anxi]);
		m_fSLimit[anxi *2 +1] = (float)(m_factualLimit[anxi *2 +1]* m_dEquivalent[anxi]);
		if(fStep *m_paramDriveDir.nDir[anxi] < m_fSLimit[anxi *2] )
		{
			theApp.m_StageMC6600.MoveLimit(ucAxis, m_fSLimit[anxi *2] *m_paramDriveDir.nDir[anxi]);
			m_fGoal[anxi] = m_fSLimit[anxi *2] *m_paramDriveDir.nDir[anxi];
			m_bDirection[anxi] = FALSE;
		}
		else if( fStep *m_paramDriveDir.nDir[anxi] > m_fSLimit[anxi *2 +1])
		{
			theApp.m_StageMC6600.MoveLimit(ucAxis, m_fSLimit[anxi *2 +1] *m_paramDriveDir.nDir[anxi]);
			m_fGoal[anxi] = m_fSLimit[anxi *2 +1] *m_paramDriveDir.nDir[anxi];
			m_bDirection[anxi] = TRUE;
		}
		else
		{
			if(anxi < 2 && (fabs(m_fSLength[anxi] - m_fCurStep[anxi]) <= m_paramBl.fCount[anxi]/m_paramBl.nCoeff[anxi]) )
			{
				// XY轴
				if( m_fSLength[anxi] < m_fCurStep[anxi] )
				{
					m_fSLength[anxi] = m_fSLength[anxi] +m_paramBl.fCount[anxi];
					theApp.m_StageMC6600.MoveLimit(ucAxis, m_fSLength[anxi]);
					Sleep(100);
					//GetCurStep(anxi);
				}
				else if( m_fSLength[anxi] > m_fCurStep[anxi] )
				{
					m_fSLength[anxi] = m_fSLength[anxi] -m_paramBl.fCount[anxi];
					theApp.m_StageMC6600.MoveLimit(ucAxis, m_fSLength[anxi]);
					Sleep(100);
					//GetCurStep(anxi);
				}
				if(fStep - m_fSLength[anxi] > 0)
				{
					m_bDirection[anxi] = TRUE;
					if(m_paramBl.nDir[anxi] > 0 && NotZero(m_paramBl.fCount[anxi]))
						m_bBL[anxi] = TRUE;
				}
				else if(fStep - m_fSLength[anxi] < 0)
				{
					m_bDirection[anxi] = FALSE;
					if(m_paramBl.nDir[anxi] < 0 && NotZero(m_paramBl.fCount[anxi]))
						m_bBL[anxi] = TRUE;
				}
				else
					return;
			}
			else
			{
				if(fStep - m_fCurStep[anxi] > 0)
				{
					m_bDirection[anxi] = TRUE;
					if(m_paramBl.nDir[anxi] > 0 && NotZero(m_paramBl.fCount[anxi]))
						m_bBL[anxi] = TRUE;
				}
				else if(fStep - m_fCurStep[anxi] < 0)
				{
					m_bDirection[anxi] = FALSE;
					if(m_paramBl.nDir[anxi] < 0 && NotZero(m_paramBl.fCount[anxi]))
						m_bBL[anxi] = TRUE;
				}
				else
					return;
			}
			str.Format("Move %c to %.5f",ucAxis,fStep / m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi]);
			Message( str );
			m_fBLCount[anxi] = theApp.m_StageMC6600.MoveDir(ucAxis, fStep);
		}
	}
	else
	{
		if(anxi < 2 && (fabs(m_fSLength[anxi] - m_fCurStep[anxi]) <= m_paramBl.fCount[anxi]/m_paramBl.nCoeff[anxi]) )
		{
			if( m_fSLength[anxi] < m_fCurStep[anxi] )
			{
				m_fSLength[anxi] = m_fSLength[anxi] +m_paramBl.fCount[anxi];
				theApp.m_StageMC6600.MoveLimit(ucAxis, m_fSLength[anxi]);
				Sleep(100);
				//GetCurStep(anxi);
			}
			else if( m_fSLength[anxi] > m_fCurStep[anxi] )
			{
				m_fSLength[anxi] = m_fSLength[anxi] -m_paramBl.fCount[anxi];
				theApp.m_StageMC6600.MoveLimit(ucAxis, m_fSLength[anxi]);
				Sleep(100);
				//GetCurStep(anxi);
			}
			if(fStep - m_fSLength[anxi] > 0)
			{
				m_bDirection[anxi] = TRUE;
				if(m_paramBl.nDir[anxi] > 0 && NotZero(m_paramBl.fCount[anxi]))
					m_bBL[anxi]=TRUE;
			}
			else if(fStep - m_fSLength[anxi] < 0)
			{
				m_bDirection[anxi] = FALSE;
				if(m_paramBl.nDir[anxi] < 0 && NotZero(m_paramBl.fCount[anxi]))
					m_bBL[anxi]=TRUE;
			}
			else
				return;
		}
		else
		{
			if(fStep - m_fCurStep[anxi] > 0)
			{
				m_bDirection[anxi] = TRUE;
				if(m_paramBl.nDir[anxi] > 0 && NotZero(m_paramBl.fCount[anxi]))
					m_bBL[anxi]=TRUE;
			}
			else if(fStep - m_fCurStep[anxi] < 0)
			{
				m_bDirection[anxi] = FALSE;
				if(m_paramBl.nDir[anxi] < 0 && NotZero(m_paramBl.fCount[anxi]))
					m_bBL[anxi]=TRUE;
			}
			else
				return;
		}
		str.Format("Move %c to %.5f",ucAxis,fStep / m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi]);
		Message( str );
		m_fBLCount[anxi] = theApp.m_StageMC6600.MoveDir(ucAxis, fStep);
	}
	m_nTimer[anxi] = SetTimer( anxi +1, nStageMC6600_Elapse, NULL );
}

void CPanelStage::MoveInput( short anxi )
{
	CString str;
	GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->GetWindowText(str);	// mm
	float fMoveStep = (float)(atof(str) * m_dEquivalent[anxi]);
	if( m_bRelative )
	{
		if(m_paramDriveDir.nDir[anxi] > 0)
			fMoveStep += m_fCurStep[anxi];
		else
			fMoveStep -= m_fCurStep[anxi];
	}
	/*if(anxi == 3)
		fMoveStep = RDriveConversion(m_fCurStep[anxi],fMoveStep);*/
	fMoveStep = fMoveStep * m_paramDriveDir.nDir[anxi];
	MoveAxis( anxi, fMoveStep );
}

void CPanelStage::GetCurStep( short anxi )
{
	/*unsigned char ucAxis = SerialNum( anxi );
	float lStep = 0;
	lStep = theApp.m_StageMC6600.GetCurPosition( ucAxis);
	
	CString str;
	str = DisplayOutput(anxi, lStep, m_fGoal[anxi]);
	GetDlgItem(IDC_EDIT_X_CurStep +anxi)->SetWindowText(str);
	m_fCurStep[anxi] = lStep;*/
// 	str.Format( "%.6f", m_fCurStep[anxi]/m_dEquivalent[anxi] );
// 	GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText(str);
}

BOOL CPanelStage::QueryMoving()
{
	int i=0;
	for(i=0;i<5;i++)
	{
		if(m_bIsMoving[i])
			return TRUE;
	}
	if(m_nProcess!=0)
		return TRUE;
	else
		return FALSE;
}

void CPanelStage::GetParam( int nIndex, float& f1, float& f2, float& f3, float& f4, float& f5 )
{
	switch( nIndex )
	{
	case	0:	// positive limit
		f1 = m_factualLimit[1];	// m_paramZRes.fLimitPos[0];
		f2 = m_factualLimit[3];	// m_paramZRes.fLimitPos[1];
		f3 = m_paramZRes.fLimitPos[2];
		f4 = 180.0;
		f5 = m_paramZRes.fLimitPos[4];
		break;
	case	1:	// negtive limit
		f1 = m_factualLimit[0];	// m_paramZRes.fLimitNeg[0];
		f2 = m_factualLimit[2];	// m_paramZRes.fLimitNeg[1];
		f3 = m_paramZRes.fLimitNeg[2];
		f4 = -180.0;
		f5 = m_paramZRes.fLimitNeg[4];
		break;
	case	2:	// position
		f1 = (float)(m_fCurStep[0] /m_dEquivalent[0] *m_paramDriveDir.nDir[0]);
		f2 = (float)(m_fCurStep[1] /m_dEquivalent[1] *m_paramDriveDir.nDir[1]);
		f3 = (float)(m_fCurStep[2] /m_dEquivalent[2] *m_paramDriveDir.nDir[2]);
		f4 = (float)(m_fCurStep[3] /m_dEquivalent[3] *m_paramDriveDir.nDir[3]);
		f5 = (float)(m_fCurStep[4] /m_dEquivalent[4] *m_paramDriveDir.nDir[4]);
		break;

	case	3:
		f1 = m_paramZRes.fZ0wd;
		break;
	}
}

void CPanelStage::OnBnClickedCheckRelative()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bRelative = ((CButton*)(GetDlgItem(IDC_CHECK_Relative)))->GetCheck();
	if( m_bRelative ) 
		Message("Change move style to RELATIVE");
	else
		Message("Change move style to ABSOLUTE");
//	m_bRelativeBak = m_bRelative;
}

void CPanelStage::OnBnClickedCheckAdvance()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_Advance)))->GetCheck();
	if( nCheck > 0 )
	{
		CDlgInput dlg;
		dlg.m_bPassword = TRUE;
		strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
		if( (dlg.DoModal() != IDOK) || dlg.m_strInput != "kx-sem" )
		{
			((CButton*)(GetDlgItem(IDC_CHECK_Advance)))->SetCheck(0);
			return;
		}
		GetDlgItem(IDC_CHECK_Advance)->SetWindowText("<<");
		GetDlgItem(IDC_BUTTON_DriverInit)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_HomeX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_HomeY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_HomeZ)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_BUTTON_HomeR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_HomeT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ZeroX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ZeroY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ZeroZ)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_BUTTON_ZeroR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ZeroT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_Home)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_SetUserOrigin)->ShowWindow(SW_SHOW);
		m_bHighPrecCount = TRUE;
		SetWindowPos(&CWnd::wndTop, 0, 0,
			m_rcWindow.Width(), m_rcWindow.Height(), SWP_NOMOVE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_Advance)->SetWindowText(">>");
		GetDlgItem(IDC_BUTTON_DriverInit)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HomeX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HomeY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HomeZ)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_BUTTON_HomeR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HomeT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ZeroX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ZeroY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ZeroZ)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_BUTTON_ZeroR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ZeroT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_Home)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_SetUserOrigin)->ShowWindow(SW_HIDE);
		m_bHighPrecCount = FALSE;
		SetWindowPos(&CWnd::wndTop, 0, 0, m_rcWindow.Width(),
			m_rcWindow.Height() -m_rcReport.Height() -15, SWP_NOMOVE);
	}
	//Invalidate(FALSE);
}

void CPanelStage::OnBnClickedCheckZWd()
{
	// TODO: 在此添加控件通知处理程序代码
}

BOOL CPanelStage::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		int nID = GetFocus()->GetDlgCtrlID();
		switch( nID )
		{
		case	IDC_EDIT_X_UserDisp:
		case	IDC_EDIT_Y_UserDisp:
		case	IDC_EDIT_Z_UserDisp:
		case	IDC_EDIT_R_UserDisp:
		case	IDC_EDIT_T_UserDisp:
			MoveInput( nID -IDC_EDIT_X_UserDisp);
			break;
		case	IDC_LIST_Position:
			Message("Click and enter in listctrl");
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
	//设置edit控件
	if( nCtlColor == CTLCOLOR_STATIC )  //CTLCOLOR_STATIC静态控件
	{
		int nID = pWnd->GetDlgCtrlID();
		switch( nID )
		{
		case	IDC_EDIT_X_CurStep:
		case	IDC_EDIT_Y_CurStep:
		case	IDC_EDIT_Z_CurStep:
		case	IDC_EDIT_R_CurStep:
		case	IDC_EDIT_T_CurStep:
			{
				int anxi = nID -IDC_EDIT_X_CurStep;
				if( m_nTimer[anxi] != 0 || m_nTimer[anxi +5] != 0 )
					pDC->SetTextColor(RGB(255,0,0));// 设置文本颜色  
				else
					pDC->SetTextColor(RGB(0,0,0));	// 设置文本颜色  
			}
			break;
		}
	}
	return hbr;
}

void CPanelStage::OnBnClickedButtonZero()
{
	Zero();
}

void CPanelStage::Zero()
{
	// 回扫描中心（即用户原点归零）
	Message( "Go Origion Z/T/X-Y/R" );
	// 保存目标位置
	float lSteps = 0.0f;
	int i = 0;
	for(i=0; i<5; i++)
		m_fGoal[i] = lSteps;
	// 更新位置显示
	CString str;
	for(i=0; i<5; i++)
	{
		m_fGoal[i] = 0;
		if( i < 3 )
			str.Format( "%.3f", m_fGoal[i]/m_dEquivalent[i] );
		else
			str.Format( "%.2f", m_fGoal[i]/m_dEquivalent[i] );
//		str = DisplayOutput1(i,m_fGoal[i],m_fGoal[i]);
		GetDlgItem(IDC_EDIT_X_UserDisp +i)->SetWindowText(str);
	}
	for(i=0; i<5; i++)
	{
		//GetCurStep(i);
		m_bEqual[i] = FALSE;
//		if(m_fGoal[i] != m_fCurStep[i])
		if( fabs(m_fGoal[i] -m_fCurStep[i]) > FLT_EPSILON )
			m_bEqual[i] = TRUE;
	}
	for(i=0;i<5;i++)
	{
		if(m_bMotoronoff[i])
		{
			m_bMotoronoff[i] = FALSE;
			MantoAuto(i);
		}
	}
	if(m_bMotoronoffAll)
	{
		m_bMotoronoffAll = FALSE;
		MantoAutoAll();
	}

	// Z 轴
	if(m_fGoal[2] - m_fCurStep[2] > 0)
	{
		m_bDirection[2] = TRUE;
		if(m_paramBl.nDir[2] > 0 && NotZero(m_paramBl.fCount[2]) )
			m_bBL[2]=TRUE;
	}
	else if(m_fGoal[2] - m_fCurStep[2] < 0)
	{
		m_bDirection[2] = FALSE;
		if(m_paramBl.nDir[2] < 0 && NotZero(m_paramBl.fCount[2]) )
			m_bBL[2]=TRUE;
	}
	if(!m_bEnable[2])
	{
		theApp.m_StageMC6600.Enable(2);
		m_bEnable[2] = TRUE;
	}
	/*for(i=0; i<5; i++)
		m_bIsMoving[i] = TRUE;*/

	m_nProcess = -1;
	m_nMoveSteps = 0;
    MoveSteps();
}

void CPanelStage::OnBnClickedButtonStop()
{
	Stop();
}

void CPanelStage::Stop()
{
//	KillTimer(lStabilityTestID);
	//R轴限位禁用
	theApp.m_StageMC6600.LimRelease(1,'R');
	//失能-读取当前位置-发送等于当前位置的目标位置-使能 
	Message( "Stop X/Y/Z/R/T move" );
	BOOL Moveing[5];
	int i = 0;
	for(i=0; i<5; i++)
		Moveing[i] = FALSE;
	for(i=0; i<5; i++)
	{
		theApp.m_StageMC6600.close(i);
		//是否需要killtimer（i+1）；
		m_bEnable[i] = FALSE;
		KillTimer(i+1);
		if(m_bIsMoving[i])
		{
			Moveing[i]= TRUE;
			m_nPhaseInit[i]  = -1;
		}
		m_bIsMoving[i] = FALSE;
		CStageMC6600Manager::Instance().m_bIsMoving[i] = m_bIsMoving[i];
	}
	m_nProcess		= 0;
	m_nMoveSteps	= -1;

	CString str;
	//float m_stopgoal;
	for(i=0; i<5; i++)
	{
		if( Moveing[i] )
		{
			//m_stopgoal = theApp.m_StageMC6600.CurrPosR(i);
			unsigned short ucAxis = SerialNum(i);
			theApp.m_StageMC6600.Move(ucAxis,m_fCurStep[i],m_bRelative);
			m_fGoal[i] = m_fCurStep[i];
			m_bToHome[i] = FALSE;
			m_bOrigion[i] = FALSE;
			if(i < 3)
				str.Format( "%.3f", m_fGoal[i]/m_dEquivalent[i] *m_paramDriveDir.nDir[i] );
			else
				str.Format( "%.2f", m_fGoal[i]/m_dEquivalent[i] *m_paramDriveDir.nDir[i] );
//			str = DisplayOutput1(i,m_stopgoal,m_fGoal[i]);
			GetDlgItem(IDC_EDIT_X_UserDisp +i)->SetWindowText(str);
			theApp.m_StageMC6600.Enable(i);
			m_bEnable[i] = TRUE;
			Moveing[i] = FALSE;
			m_bBL[i] = FALSE;
			m_nTimer[i] = SetTimer(i+1, nStageMC6600_Elapse, NULL);
		}
		else
		{
			theApp.m_StageMC6600.Enable(i);
			m_bEnable[i] = TRUE;
		}
	}
}

void CPanelStage::OnBnClickedButtonMoveto()
{
// 	if( m_bIsMoving )
// 		return;
	int flag = 0;
	CString str;
	float fPos[5];
	for( int i=0; i<5; i++ )
	{
		GetDlgItem(IDC_EDIT_X_UserDisp +i)->GetWindowText(str);
		fPos[i] = (float)(atof(str) *m_paramDriveDir.nDir[i]);   //mm
		// 都换成绝对坐标
		if( m_bRelative )
			fPos[i] = (float)(fPos[i] + m_fCurStep[i] /m_dEquivalent[i]);
		/*if(i == 3)
			fPos[i] = RDriveConversion(m_fCurStep[i],fPos[i]*m_dEquivalent[i]) /m_dEquivalent[i];*/
	}
	if(flag == 1)         //添加flag以选择是否有约束，初始参数不一致的约束
		CoherenceCheck_Z(fPos[0], fPos[1], fPos[2], 0);
	Move5Axis( fPos[0], fPos[1], fPos[2], fPos[3], fPos[4]);
}

void CPanelStage::Move5Axis_FromThird( float fX, float fY, float fZ, float fR, float fT )
{
	Move5Axis( fX *m_paramDriveDir.nDir[0],
				fY *m_paramDriveDir.nDir[1],
				fZ *m_paramDriveDir.nDir[2],
				fR *m_paramDriveDir.nDir[3],
				fT *m_paramDriveDir.nDir[4] );
	char szBuf[255];
	sprintf_s( szBuf, "Move 5 Axis from third: %.3f, %.3f, %.2f, %.1f, %.2f",
				fX *m_paramDriveDir.nDir[0], fY *m_paramDriveDir.nDir[1], fZ *m_paramDriveDir.nDir[2],
				fR *m_paramDriveDir.nDir[3], fT *m_paramDriveDir.nDir[4] );
	Message( szBuf );
}

void CPanelStage::Move2Axis_FromThird( float fX, float fY)
{
	Move2Axis( fX *m_paramDriveDir.nDir[0],	fY *m_paramDriveDir.nDir[1]);
	char szBuf[255];
	sprintf_s( szBuf, "Move 2 Axis from third: %.3f, %.3f",	fX *m_paramDriveDir.nDir[0], fY *m_paramDriveDir.nDir[1] );
	Message( szBuf );
}

void CPanelStage::Move5Axis_JudgeDirMove( short anxi )
{
	if(m_bSoftLimit)
	{
		m_fSLimit[anxi *2]		= (float)(m_factualLimit[anxi *2]* m_dEquivalent[anxi]);
		m_fSLimit[anxi *2 +1]	= (float)(m_factualLimit[anxi *2 +1]* m_dEquivalent[anxi]);
		if(m_fGoal[anxi] *m_paramDriveDir.nDir[anxi] < m_fSLimit[anxi *2] )
		{
			m_fGoal[anxi] = m_fSLimit[anxi *2] *m_paramDriveDir.nDir[anxi];
			m_bDirection[anxi] = FALSE;
		}
		else if( m_fGoal[anxi] *m_paramDriveDir.nDir[anxi] > m_fSLimit[anxi *2 +1])
		{
			m_fGoal[anxi] = m_fSLimit[anxi *2 +1] *m_paramDriveDir.nDir[anxi];
			m_bDirection[anxi] = TRUE;
		}
		else
		{
			// bSoftLimit时，还有可能出现下面的情况吗？
			if(m_fGoal[anxi] - m_fCurStep[anxi] > 0)
			{
				m_bDirection[anxi] = TRUE;
				if(m_paramBl.nDir[anxi] > 0 && NotZero(m_paramBl.fCount[anxi]))
					m_bBL[anxi]=TRUE;
			}
			else if(m_fGoal[anxi] - m_fCurStep[anxi] < 0)
			{
				m_bDirection[anxi] = FALSE;
				if(m_paramBl.nDir[anxi] < 0 && NotZero(m_paramBl.fCount[anxi]))
					m_bBL[anxi]=TRUE;
			}
		}
	}
	else
	{
		if(m_fGoal[anxi] - m_fCurStep[anxi] > 0)
		{
			m_bDirection[anxi] = TRUE;
			if(m_paramBl.nDir[anxi] > 0 && NotZero(m_paramBl.fCount[anxi]))
				m_bBL[anxi]=TRUE;
		}
		else if(m_fGoal[anxi] - m_fCurStep[anxi] < 0)
		{
			m_bDirection[anxi] = FALSE;
			if(m_paramBl.nDir[anxi] < 0 && NotZero(m_paramBl.fCount[anxi]))
				m_bBL[anxi]=TRUE;
		}
	}

	if(m_bMotoronoff[anxi] || m_bMotoronoffAll)
	{
		if(m_bMotoronoff[anxi])
		{
			m_bMotoronoff[anxi] = FALSE;
			MantoAuto(anxi);
		}
		else if(m_bMotoronoffAll)
		{
			m_bMotoronoffAll = FALSE;
			MantoAutoAll();
		}
	}

	else if(!m_bEnable[anxi])
	{
		theApp.m_StageMC6600.Enable(anxi);
		m_bEnable[anxi] = TRUE;
	}
	unsigned char ucAxis = SerialNum( anxi );
	char szBuf[255];
	sprintf_s( szBuf, "Move %c to", ucAxis );
	Message( szBuf );
	m_bIsMoving[anxi] = TRUE;
	CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
	switch( anxi )
	{
	case	2:	// Z
		m_nMoveSteps = 0;
		break;
	case	4:	// T
		m_nMoveSteps = 1;
		break;
	case	3:	// R
		m_nMoveSteps = 2;
		break;
	case	0:	// X
		m_nMoveSteps = 3;
		break;
	case	1:	// Y
		m_nMoveSteps = 3;
		break;
	}
	MoveSteps();
}

void CPanelStage::Move2Axis(float fX, float fY) //参数为mm
{
// 	if(m_bIsMoving)
// 		return;
	// 保存目标位置
	m_fGoal[0] = (float)(fX * m_dEquivalent[0]);
	m_fGoal[1] = (float)(fY * m_dEquivalent[1]);

	if(m_bSoftLimit)
	{
		for(int i=0;i<5;i++)
		{
			m_fSLimit[i *2]		= (float)(m_factualLimit[i *2]* m_dEquivalent[i]);
			m_fSLimit[i *2 +1]	= (float)(m_factualLimit[i *2 +1]* m_dEquivalent[i]);
			if(m_fGoal[i]*m_paramDriveDir.nDir[i] < m_fSLimit[i *2] )
			{
				m_fGoal[i] = m_fSLimit[i *2]*m_paramDriveDir.nDir[i];
				m_bDirection[i] = FALSE;
			}
			else if( m_fGoal[i]*m_paramDriveDir.nDir[i] > m_fSLimit[i *2 +1])
			{
				m_fGoal[i] = m_fSLimit[i *2 +1]*m_paramDriveDir.nDir[i];
				m_bDirection[i] = TRUE;
			}
		}
	}

	// 更新位置显示
	CString str;
	str.Format( "%.4f", fX *m_paramDriveDir.nDir[0] );
	GetDlgItem(IDC_EDIT_X_UserDisp)->SetWindowText(str);
	str.Format( "%.4f", fY *m_paramDriveDir.nDir[1] );
	GetDlgItem(IDC_EDIT_Y_UserDisp)->SetWindowText(str);

	for(int i=0;i<2;i++)
	{
		m_bEqual[i] = FALSE;
//		if(m_fGoal[i] != m_fCurStep[i])
		if( fabs(m_fGoal[i] -m_fCurStep[i]) > FLT_EPSILON )
		{
			m_bEqual[i] = TRUE;
			m_bIsMoving[i] = TRUE;
			m_nTimer[i] = 0;
		}
	}
	m_nProcess = -1;

	if(m_bEqual[0]&& m_bMotorFault[0] && m_bCommunicationFault[0])
		Move5Axis_JudgeDirMove( 0 );
	else
	{
		if(m_bEqual[1]&&m_bMotorFault[1] && m_bCommunicationFault[1])
			Move5Axis_JudgeDirMove( 1 );
		else
		{
			m_nProcess = 0;
			return;
		}
	}
}

void CPanelStage::Move5Axis(float fX, float fY, float fZ, float fR, float fT) //参数为mm
{
// 	if(m_bIsMoving)
// 		return;
	// 保存目标位置
	m_fGoal[0] = (float)(fX * m_dEquivalent[0]);
	m_fGoal[1] = (float)(fY * m_dEquivalent[1]);
	m_fGoal[2] = (float)(fZ * m_dEquivalent[2]);
	m_fGoal[3] = (float)(fR * m_dEquivalent[3]);
	m_fGoal[4] = (float)(fT * m_dEquivalent[4]);

	if(m_bSoftLimit)
	{
		for(int i=0;i<5;i++)
		{
			m_fSLimit[i *2]		= (float)(m_factualLimit[i *2]* m_dEquivalent[i]);
			m_fSLimit[i *2 +1]	= (float)(m_factualLimit[i *2 +1]* m_dEquivalent[i]);
			if(m_fGoal[i]*m_paramDriveDir.nDir[i] < m_fSLimit[i *2] )
			{
				m_fGoal[i] = m_fSLimit[i *2]*m_paramDriveDir.nDir[i];
				m_bDirection[i] = FALSE;
			}
			else if( m_fGoal[i]*m_paramDriveDir.nDir[i] > m_fSLimit[i *2 +1])
			{
				m_fGoal[i] = m_fSLimit[i *2 +1]*m_paramDriveDir.nDir[i];
				m_bDirection[i] = TRUE;
			}
		}
	}
	
	// 更新位置显示
	CString str;
	str.Format( "%.4f", fX *m_paramDriveDir.nDir[0] );
	GetDlgItem(IDC_EDIT_X_UserDisp)->SetWindowText(str);
	str.Format( "%.4f", fY *m_paramDriveDir.nDir[1] );
	GetDlgItem(IDC_EDIT_Y_UserDisp)->SetWindowText(str);
	str.Format( "%.4f", fZ *m_paramDriveDir.nDir[2] );
	GetDlgItem(IDC_EDIT_Z_UserDisp)->SetWindowText(str);
	str.Format( "%.3f", fR *m_paramDriveDir.nDir[3] );
	GetDlgItem(IDC_EDIT_R_UserDisp)->SetWindowText(str);
	str.Format( "%.3f", fT *m_paramDriveDir.nDir[4] );
	GetDlgItem(IDC_EDIT_T_UserDisp)->SetWindowText(str);

	for(int i=0;i<5;i++)
	{
		m_bEqual[i] = FALSE;
//		if(m_fGoal[i] != m_fCurStep[i])
		if( fabs(m_fGoal[i] -m_fCurStep[i]) > FLT_EPSILON )
		{
			m_bEqual[i] = TRUE;
			m_bIsMoving[i] = TRUE;
			CStageMC6600Manager::Instance().m_bIsMoving[i] = m_bIsMoving[i];
			m_nTimer[i] = 0;
		}
	}
	m_nProcess = -1;
	if(m_bEqual[2]&& m_bMotorFault[2] && m_bCommunicationFault[2])
		Move5Axis_JudgeDirMove( 2 );
	else
	{
		if(m_bEqual[4]&& m_bMotorFault[4] && m_bCommunicationFault[4])
			Move5Axis_JudgeDirMove( 4 );
		else
		{
			if(m_bEqual[3]&& m_bMotorFault[3] && m_bCommunicationFault[3])
				Move5Axis_JudgeDirMove( 3 );
			else
			{
				if(m_bEqual[0]&&m_bMotorFault[0] && m_bCommunicationFault[0])
					Move5Axis_JudgeDirMove( 0 );
				else
				{
					if(m_bEqual[1]&& m_bMotorFault[1] && m_bCommunicationFault[1])
						Move5Axis_JudgeDirMove( 1 );
					else
					{
						m_nProcess =0;
						return;
					}
				}
			}
		}
	}
}

void CPanelStage::MoveSteps_Axis( short anxi )
{
	unsigned char ucAxis = SerialNum( anxi );
	if( m_nTimer[anxi] == 0 )
	{
		switch( anxi )
		{
		case	2:	// Z
			m_nMoveSteps = 1;
			break;
		case	4:	// T
			m_nMoveSteps = 2;
			break;
		case	3:	// R
			m_nMoveSteps = 3;
			break;
		case	0:	// X
			m_nMoveSteps = 4;
			break;
		case	1:	// Y
			m_nMoveSteps = 4;
			break;
		}
		if( m_nProcess == 1 )	// Home
		{
			m_bIsMoving[anxi] = TRUE;
			CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
			m_bToHome[anxi] = TRUE;
			switch( anxi )
			{
			case	4:	// T
			case	3:	// R
			case	2:	// Z
				if(m_paramDriveDir.nDir[anxi] > 0)
					m_bDirection[anxi] = FALSE;
				else
					m_bDirection[anxi] = TRUE;
				m_nPhaseInit[anxi] = 1;
				break;
			case	0:	// X
			case	1:	// Y
				if(m_paramDriveDir.nDir[anxi] > 0)
					m_bDirection[anxi] = TRUE;
				else
					m_bDirection[anxi] = FALSE;
				m_nPhaseInit[anxi] = 0;
				break;
			}
			//GetCurStep(anxi);
			theApp.m_StageMC6600.LimRelease(1, ucAxis);
			if( m_bDirection[anxi] )
				m_fGoal[anxi] = 200;
			else
				m_fGoal[anxi] = -200;
			theApp.m_StageMC6600.GoHome(ucAxis, m_fGoal[anxi]);
			Sleep(100);
			m_nTimer[anxi] = SetTimer( anxi +1, nStageMC6600_Elapse, NULL );
			if(m_bMoveXY)
			{
				m_bMoveXY = FALSE;
				MoveSteps_Axis(1);
			}
		}
	}
}

void CPanelStage::MoveSteps_JudgeDirMove( short anxi )
{
	if(m_nTimer[anxi] == 0)
	{
		unsigned char ucAxis = SerialNum( anxi );
		switch( anxi )
		{
		case	4:	// T
			m_nMoveSteps = 2;
			break;
		case	3:	// R
			m_nMoveSteps = 3;
			break;
		case	0:	// X
			m_nMoveSteps = 4;
			break;
		case	1:	// Y
			m_nMoveSteps = 4;
			break;
		}
		if(m_bEqual[anxi] && m_nProcess == -1 && m_bMotorFault[anxi] && m_bCommunicationFault[anxi])
		{
			if(m_fGoal[anxi] - m_fCurStep[anxi] > 0)
			{
				m_bDirection[anxi] = TRUE;
				if(m_paramBl.nDir[anxi] > 0 && NotZero(m_paramBl.fCount[anxi]))
					m_bBL[anxi]=TRUE;
			}
			else if(m_fGoal[anxi] - m_fCurStep[anxi] < 0)
			{
				m_bDirection[anxi] = FALSE;
				if(m_paramBl.nDir[anxi] < 0 && NotZero(m_paramBl.fCount[anxi]))
					m_bBL[anxi]=TRUE;
			}
			if(!m_bEnable[anxi])
			{
				theApp.m_StageMC6600.Enable(anxi);
				m_bEnable[anxi] = TRUE;
			}
			char szBuf[255];
			sprintf_s( szBuf, "Move %c To", ucAxis );
			Message( szBuf );
			m_bIsMoving[anxi] = TRUE;
			CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
			m_fBLCount[anxi] = theApp.m_StageMC6600.MoveDir(ucAxis, m_fGoal[anxi]);
			m_nTimer[anxi] = SetTimer( anxi +1, nStageMC6600_Elapse, NULL );
			if(m_bMoveXY)
			{
				m_bMoveXY = FALSE;
				MoveSteps_JudgeDirMove(1);
			}
		}
		else
		{
			m_bIsMoving[anxi] = FALSE;
			CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
			MoveSteps();
		}
	}
}

void CPanelStage::MoveSteps()
{
	if( m_nProcess > 0 )
	{
		CString str;
		if( m_nMoveSteps == 0 )
		{
			// Home：先Z
			// 正在进行第零步：移动Z轴
			if(m_bAxisInitia[2])
			{
				if(m_bCommunicationFault[2] && m_bMotorFault[2])
					MoveSteps_Axis( 2 );
				else
				{
					//MessageBox("Z 轴未正确进行初始化,Z 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "Z %s, Z %s", m_strMsg[15], m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
					m_nMoveSteps = 1; 
					MoveSteps();
				}
			}
			else
			{
				if(m_bCommunicationFault[2] && m_bMotorFault[2])
				{
					str = DisplayOutput(2, m_fCurStep[2], m_fGoal[2]);
					GetDlgItem(IDC_EDIT_X_CurStep +2)->SetWindowText(str);
				}
				else
				{
					//MessageBox("Z 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "Z %s", m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
				}
				m_nMoveSteps = 1; 
				MoveSteps();
			}
		}
		else if( m_nMoveSteps == 1 )
		{
			// Home：先T
			// 正在进行第一步：移动T轴
			if(m_bAxisInitia[4])
			{
				if(m_bCommunicationFault[4] && m_bMotorFault[4])
					MoveSteps_Axis( 4 );
				else
				{
					//MessageBox("T 轴未正确进行初始化,T 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "T %s, T %s", m_strMsg[15], m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
					m_nMoveSteps = 2; 
					MoveSteps();
				}
			}
			else
			{
				if(m_bCommunicationFault[4] && m_bMotorFault[4])
				{
					//读取当前位置
					str = DisplayOutput(4, m_fCurStep[4], m_fGoal[4]);
					GetDlgItem(IDC_EDIT_X_CurStep +4)->SetWindowText(str);
				}
				else
				{
					//MessageBox("T 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "T %s", m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
				}
				m_nMoveSteps = 2; 
				MoveSteps();
			}
		}
		else if( m_nMoveSteps == 2 )
		{
			if(m_bAxisInitia[3])
			{
				if(m_bCommunicationFault[3] && m_bMotorFault[3])
					MoveSteps_Axis( 3 );
				else
				{
					//MessageBox("R 轴未正确进行初始化,R 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "R %s, R %s", m_strMsg[15], m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
					m_nMoveSteps = 3; 
					MoveSteps();
				}
			}
			else
			{
				if(m_bCommunicationFault[3] && m_bMotorFault[3])
				{
					//读取当前位置
					str = DisplayOutput(3, m_fCurStep[3], m_fGoal[3]);
					GetDlgItem(IDC_EDIT_X_CurStep +3)->SetWindowText(str);
				}
				else
				{
					//MessageBox("R 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "R %s", m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
				}
				m_nMoveSteps = 3; 
				MoveSteps();
			}
		}
		else if( m_nMoveSteps == 3 )
		{
			// Home ：先X
			// 正在进行第二步：移动X&Y轴
			if(m_bAxisInitia[0] && m_bAxisInitia[1])
			{
				if(m_bCommunicationFault[0] && m_bMotorFault[0] && m_bCommunicationFault[1] && m_bMotorFault[1])
				{
					m_bMoveXY = TRUE;
					MoveSteps_Axis( 0 );
				}
				else if(m_bCommunicationFault[0] && m_bMotorFault[0] )
				{
					m_bMoveXY = FALSE;
					MoveSteps_Axis( 0 );
					//MessageBox("Y 轴未正确进行初始化,Y 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "Y %s, Y %s", m_strMsg[15], m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
				}
				else if(m_bCommunicationFault[1] && m_bMotorFault[1])
				{
					m_bMoveXY = FALSE;
					MoveSteps_Axis( 1 );
					//MessageBox("X 轴未正确进行初始化,X 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "X %s, X %s", m_strMsg[15], m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
				}
				else
				{
					//MessageBox("X&Y 轴未正确进行初始化,X&Y 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "X&Y %s, X&Y %s", m_strMsg[15], m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
					m_nMoveSteps = 4; 
					MoveSteps();
				}
			}
			else if(m_bAxisInitia[0])
			{
				if(m_bCommunicationFault[0] && m_bMotorFault[0] )
				{
					m_bMoveXY = FALSE;
					MoveSteps_Axis( 0 );
				}
				else
				{
					//MessageBox("X 轴未正确进行初始化,X 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "X %s, X %s", m_strMsg[15], m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
					m_nMoveSteps = 4; 
					MoveSteps();
				}
			}
			else if(m_bAxisInitia[1])
			{
				if(m_bCommunicationFault[1] && m_bMotorFault[1] )
				{
					m_bMoveXY = FALSE;
					MoveSteps_Axis( 1 );
				}
				else
				{
					//MessageBox("Y 轴未正确进行初始化,Y 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "Y %s, Y %s", m_strMsg[15], m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
					m_nMoveSteps = 4; 
					MoveSteps();
				}
			}
			else
			{
				if(m_bCommunicationFault[0] && m_bMotorFault[0]&& m_bCommunicationFault[1] && m_bMotorFault[1])
				{
					//读取当前位置
					str = DisplayOutput(0, m_fCurStep[0], m_fGoal[0]);
					GetDlgItem(IDC_EDIT_X_CurStep +0)->SetWindowText(str);
					Sleep(100);
					str = DisplayOutput(1, m_fCurStep[1], m_fGoal[1]);
					GetDlgItem(IDC_EDIT_X_CurStep +1)->SetWindowText(str);				
				}
				else if(m_bCommunicationFault[0] && m_bMotorFault[0])
				{
					str = DisplayOutput(0, m_fCurStep[0], m_fGoal[0]);
					GetDlgItem(IDC_EDIT_X_CurStep +0)->SetWindowText(str);					
					//MessageBox("Y 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "Y %s", m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
				}
				else if(m_bCommunicationFault[1] && m_bMotorFault[1])
				{
					str = DisplayOutput(1, m_fCurStep[1], m_fGoal[1]);
					GetDlgItem(IDC_EDIT_X_CurStep +1)->SetWindowText(str);
					//MessageBox("X 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "X %s", m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
				}
				else
				{
					//MessageBox("X&Y 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "X&Y %s", m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
				}
				m_nMoveSteps = 4; 
				MoveSteps();
			}
		}
		else if( m_nMoveSteps == 4 )
		{
			if( m_nProcess == 1 )
			{
				// 5轴运动都已结束，给出提示
				Message("Finish");
				m_nMoveSteps = -1;
				m_nProcess = 0;
			}
		}
	}
	else if(m_nProcess < 0)
	{
		if( m_nMoveSteps == 0 )
		{
			// Z
			if(m_nTimer[2] == 0)
			{
				m_nMoveSteps = 1;
				if(m_nProcess == -1  && m_bMotorFault[2] && m_bCommunicationFault[2])
				{
					m_bIsMoving[2] = TRUE;
					CStageMC6600Manager::Instance().m_bIsMoving[2] = m_bIsMoving[2];
					m_fBLCount[2] = theApp.m_StageMC6600.MoveDir('Z', m_fGoal[2]);
					m_nTimer[2] = SetTimer( 3, nStageMC6600_Elapse, NULL );
				}
				else
				{
					m_bIsMoving[2] = FALSE;
					CStageMC6600Manager::Instance().m_bIsMoving[2] = m_bIsMoving[2];
					MoveSteps();
				}
			}
		}
		else if( m_nMoveSteps == 1 )
		{
			// T
			MoveSteps_JudgeDirMove( 4 );
		}
		else if( m_nMoveSteps == 2 )
		{
			// R
			MoveSteps_JudgeDirMove( 3 );
		}
		else if( m_nMoveSteps == 3 )
		{
			// X 
			if(m_bEqual[0] && m_bEqual[1])
			{
				m_bMoveXY = TRUE;
				MoveSteps_JudgeDirMove( 0 );
			}
			else if(m_bEqual[0])
				MoveSteps_JudgeDirMove( 0 );
			else if(m_bEqual[1])
				MoveSteps_JudgeDirMove( 1 );
		}
		else if( m_nMoveSteps == 4 )
		{
			if(m_nProcess == -1)
			{
				// 5轴运动都已结束，给出提示
				Message("Finish");
				m_nMoveSteps = -1;
				m_nProcess = 0;
			}
		}
	}
}

void CPanelStage::DiffCenter(short anxi)
{
	m_bIsMoving[anxi] = TRUE;
	unsigned char ucAxis = SerialNum(anxi);

	m_fGoal[anxi] = (float)(m_fCali[anxi] *m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi]);
	theApp.m_StageMC6600.GoOrigion(ucAxis, m_fGoal[anxi]);
	Sleep(100);
	char szBuf[255];
	sprintf_s( szBuf, "%f", m_fCali[anxi] );
	GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText(szBuf);
	//m_nTimer[anxi] = SetTimer( anxi+1, nStageMC6600_Elapse, NULL );
}

void CPanelStage::SaveToINI( int nIndex )
{
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\Stage\\StageMC6600.ini");
	char szBuf[255];
	// 写入配置文件
	switch( nIndex )
	{
	case	0:	// 保存工作原点绝对位置SaveUserOrigin
		sprintf_s( szBuf, "%.4f,%.4f,%.4f,%.3f,%.3f", m_fCali[0], m_fCali[1], m_fCali[2], m_fCali[3], m_fCali[4] );
		WritePrivateProfileString( "StageMC6600Cfg", "Cali", szBuf, path );
		break;
	case	1:	// ToHome
		sprintf_s( szBuf, "%.2f,%.2f,%.2f,%.2f,%.2f", 
						m_paramLimitDir.fA[0],
						m_paramLimitDir.fA[1],
						m_paramLimitDir.fA[2],
						m_paramLimitDir.fA[3],
						m_paramLimitDir.fA[4] );
		WritePrivateProfileString( "StageMC6600Cfg", "LimitDirA ", szBuf, path );
		sprintf_s( szBuf, "%.2f,%.2f,%.2f,%.2f,%.2f", 
						m_paramLimitDir.fB[0],
						m_paramLimitDir.fB[1],
						m_paramLimitDir.fB[2],
						m_paramLimitDir.fB[3],
						m_paramLimitDir.fB[4] );
		WritePrivateProfileString( "StageMC6600Cfg", "LimitDirB ", szBuf, path );
		break;
	case	2:	// OnBnClickedButtonDriverInit: SoftLimit
		sprintf_s( szBuf, "%d", m_bSoftLimit);
		WritePrivateProfileString( "StageMC6600Cfg", "SLimit", szBuf, path );
		break;
	case	3:	// OnBnClickedButtonDriverInit: OnOK	
		sprintf_s( szBuf, "%d,%d,%d,%d,%d",
						m_paramAddr.byteAddr[0],
						m_paramAddr.byteAddr[1],
						m_paramAddr.byteAddr[2],
						m_paramAddr.byteAddr[3],
						m_paramAddr.byteAddr[4] );
		WritePrivateProfileString( "StageMC6600Cfg", "Address", szBuf, path );
		//设置向导
		sprintf_s( szBuf, "%d,%d,%d,%d,%d",
						m_paramWizard.nSetMotorType[0],
						m_paramWizard.nSetMotorType[1],
						m_paramWizard.nSetMotorType[2],
						m_paramWizard.nSetMotorType[3],
						m_paramWizard.nSetMotorType[4] );
		WritePrivateProfileString( "StageMC6600Cfg", "MotorType ", szBuf, path );
		sprintf_s( szBuf, "%d,%d,%d,%d,%d",
						m_bMotorFault[0],
						m_bMotorFault[1],
						m_bMotorFault[2],
						m_bMotorFault[3],
						m_bMotorFault[4] );
		WritePrivateProfileString( "StageMC6600Cfg", "MotorFault ", szBuf, path );
		sprintf_s( szBuf, "%d,%d,%d,%d,%d",
						m_paramWizard.nEncoderLines[0],
						m_paramWizard.nEncoderLines[1],
						m_paramWizard.nEncoderLines[2],
						m_paramWizard.nEncoderLines[3],
						m_paramWizard.nEncoderLines[4] );
		WritePrivateProfileString( "StageMC6600Cfg", "EncoderLines ", szBuf, path );
		sprintf_s( szBuf, "%d,%d,%d,%d,%d",
						m_paramWizard.nControlMode[0],
						m_paramWizard.nControlMode[1],
						m_paramWizard.nControlMode[2],
						m_paramWizard.nControlMode[3],
						m_paramWizard.nControlMode[4] );
		WritePrivateProfileString( "StageMC6600Cfg", "ControlMode ", szBuf, path );
		sprintf_s( szBuf, "%d,%d,%d,%d,%d",
						m_paramWizard.nControlSignal[0],
						m_paramWizard.nControlSignal[1],
						m_paramWizard.nControlSignal[2],
						m_paramWizard.nControlSignal[3],
						m_paramWizard.nControlSignal[4] );
		WritePrivateProfileString( "StageMC6600Cfg", "ControlSignal ", szBuf, path );
		sprintf_s( szBuf, "%d,%d,%d,%d,%d",
						m_paramWizard.nMaxCurrent[0],
						m_paramWizard.nMaxCurrent[1],
						m_paramWizard.nMaxCurrent[2],
						m_paramWizard.nMaxCurrent[3],
						m_paramWizard.nMaxCurrent[4] );
		WritePrivateProfileString( "StageMC6600Cfg", "MaxCurrent ", szBuf, path );
		break;
	case	4:	// OnBnClickedButtonSetup
		sprintf_s( szBuf, "%d", m_nPort);
		WritePrivateProfileString( "StageMC6600Cfg", "Port", szBuf, path );
		sprintf_s( szBuf, "%d", m_bBlClick );
		WritePrivateProfileString( "StageMC6600Cfg", "BlClick", szBuf, path );
		sprintf_s( szBuf, "%d", m_bBlTrack );
		WritePrivateProfileString( "StageMC6600Cfg", "BlTrack", szBuf, path );
		sprintf_s( szBuf, "%d", m_nVentSel );
		WritePrivateProfileString( "StageMC6600Cfg", "VentSel", szBuf, path );
		if( m_paramZRes.fHy < 5 )
			sprintf_s( szBuf, "%.1f,5.0", m_paramZRes.fRy );
		else
			sprintf_s( szBuf, "%.1f,%.1f", m_paramZRes.fRy, m_paramZRes.fHy );
		WritePrivateProfileString( "StageMC6600Zres", "ResUser", szBuf, path );
		sprintf_s( szBuf, "%.1f,%.1f", m_paramZRes.fSafeWD, m_paramZRes.fZsink );
		WritePrivateProfileString( "StageMC6600Zres", "ResZ", szBuf, path );
		sprintf_s( szBuf, "%.1f", m_paramZRes.fHt );
		WritePrivateProfileString( "StageMC6600Zres", "ResT", szBuf, path );
		sprintf_s( szBuf, "%.1f", m_paramZRes.fHs );
		WritePrivateProfileString( "StageMC6600Cfg", "ZSample_H", szBuf, path );
		break;
	case 5:
		sprintf_s( szBuf, "%d,%d,%d,%d,%d", m_bCommunicationFault[0], m_bCommunicationFault[1], m_bCommunicationFault[2], m_bCommunicationFault[3], m_bCommunicationFault[4] );
		WritePrivateProfileString( "StageMC6600Cfg", "CommunicationFault", szBuf, path );
		break;
	}
}

void CPanelStage::OnBnClickedButtonSetuserorigin()
{
	// TODO: 在此添加控件通知处理程序代码
	//将当前位置存储为用户原点
	if(m_bHighPrecCount)
	{
		SaveUserOrigin();
	}
	else
	{
		SetUserOrigin();
	}
}

void CPanelStage::SaveUserOrigin()
{
	if( MessageBox( m_strMsg[2], m_strMsg[0], MB_YESNO ) == IDYES )
	{
		// 将当前位置值清零，即设为零点
		for(int i =0;i<5;i++)
		{
			// 更新当前位置值显示
			CStageMC6600Manager::Instance().m_bIsMoving[i] =  TRUE;
			theApp.m_StageMC6600.LocaZero(i);
			Sleep(100);
			theApp.m_StageMC6600.Reset(i);
			m_fGoal[i] = 0.0;
			if(i<3)
			{
				GetDlgItem(IDC_EDIT_X_UserDisp +i)->SetWindowText("0.000");
				GetDlgItem(IDC_EDIT_X_CurStep +i)->SetWindowText("0.000,0.00000");
			}
			else
			{
				GetDlgItem(IDC_EDIT_X_UserDisp +i)->SetWindowText("0.00");
				GetDlgItem(IDC_EDIT_X_CurStep +i)->SetWindowText("0.00,0.00000");
			}
		}
		// 保存工作原点绝对位置
		m_fCali[0] = (float)(m_fCurStep[0] /m_dEquivalent[0] * m_paramDriveDir.nDir[0]);
		m_fCali[1] = (float)(m_fCurStep[1] /m_dEquivalent[1] * m_paramDriveDir.nDir[1]);
		m_fCali[2] = (float)(m_fCurStep[2] /m_dEquivalent[2] * m_paramDriveDir.nDir[2]);	// Z向不写了
		m_fCali[3] = (float)(m_fCurStep[3] /m_dEquivalent[3] * m_paramDriveDir.nDir[3]);
		m_fCali[4] = (float)(m_fCurStep[4] /m_dEquivalent[4] * m_paramDriveDir.nDir[4]);
		SaveToINI( 0 );
	}
}

void CPanelStage::SetUserOrigin()
{
	for(int i =0;i<5;i++)
		m_bOrigion[i] = TRUE;
	Message( "GoOrigion T/Z/X/Y" );

	m_bToHome[2] = TRUE;
	m_bIsMoving[2] = TRUE;
	CStageMC6600Manager::Instance().m_bIsMoving[2] = m_bIsMoving[2];
	m_nMoveSteps =0;
	m_nProcess = 1;
	MoveSteps();
}

void CPanelStage::OnBnClickedButtonHome()
{
	// TODO: 在此添加控件通知处理程序代码
	Home();
}

void CPanelStage::Home()
{
	Message( "GoHome Z/T/X-Y/R" );

	m_bToHome[2] = TRUE;
	m_bIsMoving[2] = TRUE;
	CStageMC6600Manager::Instance().m_bIsMoving[2] = m_bIsMoving[2];
	m_nMoveSteps =0;
	m_nProcess = 1;
	//R轴限位启用
	theApp.m_StageMC6600.LimRelease(0,'R');
	MoveSteps();
}

void CPanelStage::OnBnClickedButtonDriverInit()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rec;
	GetWindowRect(rec);
	CDlgDriverInit dlg;
	dlg.m_nAxial = m_paramAddr.cAxial[0];
	dlg.m_nAddress = m_paramAddr.byteAddr[0];
	dlg.m_bSoftLimit = m_bSoftLimit;
	dlg.m_Style = m_paramStyle;
	strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
	int i = 0;
	for(i=0;i<5;i++)
	{
		dlg.m_nSetMotorType[i]	= m_paramWizard.nSetMotorType[i];
		dlg.m_nEncoderLines[i]	= m_paramWizard.nEncoderLines[i];
		dlg.m_nControlSignal[i]	= m_paramWizard.nControlSignal[i];
		dlg.m_nControlMode[i]	= m_paramWizard.nControlMode[i];
		dlg.m_nMaxCurrent[i]	= m_paramWizard.nMaxCurrent[i];
		dlg.m_bRecordMotorStatus[i] = m_bMotorFault[i];
	}
	
	/*for(int i=0;i<5;i++)
		dlg.fCurSetP[i] = m_paramCurSetP.fCurSetP[i];
	for(int i=0;i<5;i++)
		dlg.fCurSetI[i] = m_paramCurSetI.fCurSetI[i];
	for(int i=0;i<5;i++)
		dlg.fCurSetD[i] = m_paramCurSetD.fCurSetD[i];*/
	for(i=0;i<5;i++)
	{
		dlg.m_fDefaultP[i]	= m_paramDefaultPID.fP[i];
		dlg.m_fDefaultI[i]	= m_paramDefaultPID.fI[i];
		dlg.m_fDefaultD[i]	= m_paramDefaultPID.fD[i];
		dlg.m_fCurP[i]		= m_paramCurPID.fP[i];
		dlg.m_fCurI[i]		= m_paramCurPID.fI[i];
		dlg.m_fCurD[i]		= m_paramCurPID.fD[i];
		//开启线程
		CStageMC6600Manager::Instance().m_bIsMoving[i] = TRUE;
	}

	INT_PTR dlgstatus = dlg.DoModal();
	if(dlgstatus == IDC_BUTTON_Slimit )
	{
		m_bSoftLimit = dlg.m_bSoftLimit;
		SaveToINI( 2 );
	}
	
	else if( dlgstatus == IDOK )
	{
		m_nAxial = dlg.m_nAxial;
		m_nAddress = dlg.m_nAddress;
		for(i=0;i<5;i++)
		{
			m_paramWizard.nSetMotorType[i]	= dlg.m_nSetMotorType[i];
			m_paramWizard.nEncoderLines[i]	= dlg.m_nEncoderLines[i];
			m_paramWizard.nControlSignal[i]	= dlg.m_nControlSignal[i];
			m_paramWizard.nControlMode[i]	= dlg.m_nControlMode[i];
			m_paramWizard.nMaxCurrent[i]	= dlg.m_nMaxCurrent[i];
			m_bMotorFault[i]				= dlg.m_bRecordMotorStatus[i];
			m_paramCurPID.fP[i]				= dlg.m_fCurP[i];
			m_paramCurPID.fI[i]				= dlg.m_fCurI[i];
			m_paramCurPID.fD[i]				= dlg.m_fCurD[i];
		}
		for(i=0;i<5;i++)
		{
			if(i == m_nAxial)
			{
				m_paramAddr.byteAddr[i]=m_nAddress;
				break;
			}
		}
		SaveToINI( 3 );
		//关闭线程
		CStageMC6600Manager::Instance().m_bIsMoving[i] = FALSE;
	}
	else if(dlgstatus == IDCANCEL)
	{
		for(i=0;i<5;i++)
		{
			m_paramCurPID.fP[i] = dlg.m_fCurP[i];
			m_paramCurPID.fI[i] = dlg.m_fCurI[i];
			m_paramCurPID.fD[i] = dlg.m_fCurD[i];
		}
		//关闭线程
		CStageMC6600Manager::Instance().m_bIsMoving[i] = FALSE;
	}
}

void CPanelStage::OnBnClickedButtonSetup()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rc;
	GetWindowRect(rc);
	CDlgSetup dlg;
	dlg.m_nPort = m_nPort;
	dlg.m_fRy = m_paramZRes.fRy;
	dlg.m_fHy = m_paramZRes.fHy;
	dlg.m_fHt = m_paramZRes.fHt;
	dlg.m_fHs = m_paramZRes.fHs;
	dlg.m_fZsink = m_paramZRes.fZsink;
	dlg.m_fSafeWD = m_paramZRes.fSafeWD;
	dlg.m_bClick = m_bBlClick;
	dlg.m_bTrack = m_bBlTrack;
	dlg.m_nVentSel = m_nVentSel;
	dlg.m_strMsg[0] = m_strMsg[0];
	dlg.m_bDbClick = m_bDbClick;
	dlg.m_bAdvance = (rc.Height() > 600) ? TRUE : FALSE;
	strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
	switch( dlg.DoModal() )
	{
	case	IDOK:
		{
			m_nPort = dlg.m_nPort;
			m_paramZRes.fRy = dlg.m_fRy;
			m_paramZRes.fHy = dlg.m_fHy;
			m_paramZRes.fHt = dlg.m_fHt;
			m_paramZRes.fHs = dlg.m_fHs;
			m_paramZRes.fZsink = dlg.m_fZsink;
			m_paramZRes.fSafeWD = dlg.m_fSafeWD;
			m_bBlClick	= dlg.m_bClick;
			m_bBlTrack	= dlg.m_bTrack;
			m_nVentSel	= dlg.m_nVentSel;
			for(int i =0;i<5;i++)
			{
				if(i == m_nAxial)
				{
					m_paramAddr.byteAddr[i]=m_nAddress;
					break;
				}
			}
			m_bDbClick	= dlg.m_bDbClick;
			SaveToINI( 4 );
		}
		break;
	case	IDC_BUTTON_Relink:
		{
			if( m_bIsMC6600Ready )
			{
				for(int i=0;i<5;i++)
				{
					theApp.m_StageMC6600.close(i);
					m_bEnable[i] = FALSE;
					m_bIsMoving[i] = FALSE;
					CStageMC6600Manager::Instance().m_bIsMoving[i] = m_bIsMoving[i];
				}
				Sleep(100);
				theApp.m_StageMC6600.Close();
			}
			Connect();
		}
		break;
	}
}

void CPanelStage::CalculateStageUScale( int nAxis,		// 是哪个轴
									   double	dScalePixels,	// 像素长度
									   float	&nScale )		// 实际移动步数
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
	dRatio[1] = m_dblPixelsPerMM_VERT;	// 1080.0 /320.0;
	dRatio[2] = dRatio[3] = 1.0;
	// 屏幕物理尺寸 = 像素数 / Ratio，实际物理尺寸= 屏幕物理尺寸 / 放大倍数
	double dScale = dScalePixels / dRatio[nAxis] / m_dMag;
	//dScale *= m_paramsSys.fPitch[nAxis];	// 解决X/Y方向单步走和双击跳转中心时的2倍问题
	nScale = (float)(dScale*(m_dEquivalent[nAxis]));
// 	if( nScale < 1 )
// 		nScale = 1;
}

void CPanelStage::SetPixelsPerMM( double dX, double dY )
{
	m_dblPixelsPerMM_HORZ = dX;
	m_dblPixelsPerMM_VERT = dY;
}

void CPanelStage::SetMag( double dMag )
{
	m_dMag = dMag;
	MagChange();
}

void CPanelStage::MagChange()
{
	CoherenceCheck_Mag();
	CString str;
	str.Format("%.1f", m_dMag);
	GetDlgItem(IDC_Stage_Mag)->SetWindowText(str);
}

void CPanelStage::CoherenceCheck_Mag()
{
	if( m_dMag > 450000 )
		m_dMag = 450000;
	if( m_dMag < 5 )
		m_dMag = 5;
}

void CPanelStage::SetResolution( CSize szReso )
{
	m_sizeReso = szReso;
}

void CPanelStage::MoveToCenter( int nCenterX, int nCenterY, int nX, int nY )
{
	if(!m_bMotorFault[0] || !m_bCommunicationFault[0] || !m_bMotorFault[1] || !m_bCommunicationFault[1] || !m_bDbClick)
		return;

	double dLengthX = abs( nX -nCenterX );
	double dLengthY = abs( nY -nCenterY );
	// 实际步数
	float nScaleX = 0;
	CalculateStageUScale( 0, dLengthX, nScaleX );
	float nScaleY = 0;
	CalculateStageUScale( 1, dLengthY, nScaleY );

	float fMoveStepX, fMoveStepY;
	if( nX > nCenterX )
		fMoveStepX = 0 -nScaleX;
	else
		fMoveStepX = nScaleX;

	if( nY > nCenterY )
		fMoveStepY = nScaleY;
	else
		fMoveStepY = 0 -nScaleY;
	CString str;
	str.Format( "move to center: %.3f, %.3f", fMoveStepX, fMoveStepY );
	Message(str);
	
	fMoveStepX = fMoveStepX *m_paramDriveDir.nDir[0];
	fMoveStepY = fMoveStepY *m_paramDriveDir.nDir[1];

	fMoveStepX = fMoveStepX + m_fCurStep[0];
	fMoveStepY = fMoveStepY + m_fCurStep[1];
	m_fGoal[0] = fMoveStepX;
	m_fGoal[1] = fMoveStepY;
	
	for(int i=0;i<2;i++)
	{
		m_bIsMoving[i] = TRUE;
		CStageMC6600Manager::Instance().m_bIsMoving[i] = m_bIsMoving[i];
	}

	str.Format( "%.4f", fMoveStepX/m_dEquivalent[0] *m_paramDriveDir.nDir[0] );
	GetDlgItem(IDC_EDIT_X_UserDisp)->SetWindowText(str);
	str.Format( "%.4f", fMoveStepY/m_dEquivalent[1] *m_paramDriveDir.nDir[1] );
	GetDlgItem(IDC_EDIT_Y_UserDisp)->SetWindowText(str);

	//双击时是否消间隙
	if(m_bBlClick)
	{
		if(fMoveStepX - m_fCurStep[0] > 0)
		{
			m_bDirection[0] = TRUE;
			if(m_paramBl.nDir[0] > 0 && NotZero(m_paramBl.fCount[0]))
				m_bBL[0]=TRUE;
		}
		else if(fMoveStepX - m_fCurStep[0] < 0)
		{
			m_bDirection[0] = FALSE;
			if(m_paramBl.nDir[0] < 0 && NotZero(m_paramBl.fCount[0]))
				m_bBL[0]=TRUE;
		}
		else
		{
			return;
		}
		m_fBLCount[0] = theApp.m_StageMC6600.MoveDir('X', fMoveStepX);
		m_nTimer[0] = SetTimer( 1, nStageMC6600_Elapse, NULL );

		if(fMoveStepY - m_fCurStep[1] > 0)
		{
			m_bDirection[1] = TRUE;
			if(m_paramBl.nDir[1] > 0 && NotZero(m_paramBl.fCount[1]))
				m_bBL[1]=TRUE;
		}
		else if(fMoveStepY - m_fCurStep[1] < 0)
		{
			m_bDirection[1] = FALSE;
			if(m_paramBl.nDir[1] < 0 && NotZero(m_paramBl.fCount[1]))
				m_bBL[1]=TRUE;
		}
		else
		{
			return;
		}
		m_fBLCount[1] = theApp.m_StageMC6600.MoveDir('Y', fMoveStepY);
		m_nTimer[1] = SetTimer( 2, nStageMC6600_Elapse, NULL );
	}
	else
	{
		theApp.m_StageMC6600.Move('X', fMoveStepX ,m_bRelative);
		m_nTimer[0] = SetTimer(1,nStageMC6600_Elapse,NULL);

		theApp.m_StageMC6600.Move('Y', fMoveStepY, m_bRelative);
		m_nTimer[1] = SetTimer(2,nStageMC6600_Elapse,NULL);
	}
}

void CPanelStage::OnVentExit()
{
	switch( m_nVentSel )
	{
	case	0:	// 回机械原点
		OnBnClickedButtonHome();
		break;
	case	1:	// 回用户原点
		OnBnClickedButtonSetuserorigin();
		break;
	case	2:	// 不动
		break;
	}
}

void CPanelStage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( nIDEvent > 0  && nIDEvent < 6 )
		Timer_Axis( (short)nIDEvent -1 );
	else if( nIDEvent > 5 && nIDEvent < 11 )
		Timer_CurPos( (short)nIDEvent -6 );
	else
	{
		switch( nIDEvent )
		{
		case	11:
		case	12:
			Timer_Tracklocus( nIDEvent, 0 );
			break;
		case	21:
		case	22:
			Timer_Tracklocus( nIDEvent, 1 );
			break;
		case	31:
		case	32:
			Timer_Tracklocus( nIDEvent, 2 );
			break;
		case	41:
		case	42:
			Timer_Tracklocus( nIDEvent, 3 );
			break;
		case	51:
		case	52:
			Timer_Tracklocus( nIDEvent, 4 );
			break;
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CPanelStage::Grounding( unsigned char ucAxis, short anxi )
{
	theApp.m_StageMC6600.close(anxi);
	m_bEnable[anxi] = FALSE;
	KillTimer(anxi+1);
	m_nTimer[anxi] = 0;
	char szBuf[255];
	sprintf_s( szBuf, "%c grounding occurs", ucAxis );	//出现接地
	Message(szBuf);
	m_bBL[anxi] = FALSE;
//	char sl = ucAxis;
// 	if( MessageBox(sl+m_strMsg[8], m_strMsg[0], MB_YESNO ) != IDYES )
//		return;
	Safe_Axis( ucAxis, -1, 1 );
}

void CPanelStage::ToHome_XY( unsigned char ucAxis, short anxi )
{
	char szBuf[255];
	/*GetCurStep(anxi);
	float fState = theApp.m_StageMC6600.StatusRead(ucAxis);*/
	if(anxi == 3)
		m_paramsCurrent.nStatus[3] = 0;
	//是否在限位状态
	if(m_paramsCurrent.nStatus[anxi] > 9 && m_paramsCurrent.nStatus[anxi] < 12)	// fState == 10 || fState == 11)
	{
		//处于安全，限位断电
		theApp.m_StageMC6600.close(anxi);
		m_bEnable[anxi] = FALSE;
		KillTimer(anxi+1);
		m_nTimer[anxi] = 0;
		if(m_bNum[anxi] && m_nPhaseInit[anxi] == 0)
		{
			// 清零并到负向极限
			sprintf_s( szBuf, "Axis %c goes to the positive limit", ucAxis );	// 到正向极限
			Message( szBuf );
			theApp.m_StageMC6600.GoHome(ucAxis, -200 *m_paramDriveDir.nDir[anxi] );
			Sleep(50);
			theApp.m_StageMC6600.Reset(anxi);
			m_nPhaseInit[anxi] = 1;
			sprintf_s( szBuf, "Axis %c reset and reach the negative limit", ucAxis );
			Message( szBuf );

			if(m_paramsCurrent.nStatus[anxi] < 11 )	// == 10
			{
				m_paramLimitDir.fA[anxi] = 1 *m_paramDriveDir.nDir[anxi];
				m_paramLimitDir.fB[anxi] = -1 *m_paramDriveDir.nDir[anxi];
			}
			else				// == 11
			{
				m_paramLimitDir.fA[anxi] = -1 *m_paramDriveDir.nDir[anxi];
				m_paramLimitDir.fB[anxi] = 1 *m_paramDriveDir.nDir[anxi];
			}
		}
		else if(m_bNum[anxi] && m_nPhaseInit[anxi] == 1 &&  m_fCurStep[anxi]*m_paramDriveDir.nDir[anxi] < -40)
		{
			sprintf_s( szBuf, "Axis %c goes to the negative limit: %.6f", ucAxis, m_fCurStep[anxi]/m_dEquivalent[anxi]* m_paramDriveDir.nDir[anxi] );
			Message( szBuf );
			// 到机械原点
			m_fDis[anxi] = (float)(m_fCurStep[anxi]*0.5);
			theApp.m_StageMC6600.GoHome(ucAxis, m_fDis[anxi]);	
			m_bNum[anxi] = FALSE;
			m_nPhaseInit[anxi] = 2;
		}
		theApp.m_StageMC6600.Enable(anxi);
		m_bEnable[anxi] = TRUE;
		m_nTimer[anxi] = SetTimer( anxi+1, nStageMC6600_Elapse, NULL );
		
	}
	else if( !m_bNum[anxi] && m_nPhaseInit[anxi] == 2 && fabs(m_fCurStep[anxi]-m_fDis[anxi]) < m_fDeadline[anxi]*m_dEquivalent[anxi] )
	{
		if( m_bOrigion[anxi] )
		{
			if( fabs(m_fDis[anxi] /m_dEquivalent[anxi] -m_fCali[anxi]* m_paramDriveDir.nDir[anxi]) < m_fDeadline[anxi] )
			{
				sprintf_s( szBuf, "Axis %c goes to the original", ucAxis );	// 到扫描原点
				Message( szBuf );
				KillTimer(anxi+1);
				m_nTimer[anxi] = 0;
				theApp.m_StageMC6600.close(anxi);
				m_bEnable[anxi] = FALSE;
				Sleep(100);
				theApp.m_StageMC6600.LocaZero(anxi);
				Sleep(100);
				theApp.m_StageMC6600.Reset(anxi);
				m_fGoal[anxi] = 0.0;
				//theApp.m_StageMC6600.CurrPosR(anxi);
				m_fCurStep[anxi] = 0.0;
				Sleep(100);
				theApp.m_StageMC6600.Enable(anxi);
				Sleep(100);
				m_bEnable[anxi] = TRUE;
				m_bOrigion[anxi] =FALSE;
				m_bToHome[anxi] =FALSE;
				m_bNum[anxi] = TRUE;
				m_nTimer[anxi] = SetTimer( anxi+1, nStageMC6600_Elapse, NULL );
				GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText("0.0");
				m_nPhaseInit[anxi] = -1;
				if(m_nProcess > 0)
					MoveSteps();
				if(m_bSoftLimit)
				{
					m_paramZRes.fLimitNeg[anxi] = m_paramZRes.fLimitNeg[anxi] - m_fCali[anxi];
					m_paramZRes.fLimitPos[anxi] = m_paramZRes.fLimitPos[anxi] - m_fCali[anxi];
					m_factualLimit[anxi*2] = m_factualLimitH[anxi*2] - m_fCali[anxi];
					m_factualLimit[anxi*2+1] = m_factualLimitH[anxi*2+1] - m_fCali[anxi];
				}
			}
			else
			{
				sprintf_s( szBuf, "Axis %c goes to the HOME-scan", ucAxis );	// 到机械原点-扫描
				Message( szBuf );
				theApp.m_StageMC6600.close(anxi);
				m_bEnable[anxi] = FALSE;
				Sleep(100);
				theApp.m_StageMC6600.LocaZero(anxi);
				Sleep(100);
				theApp.m_StageMC6600.Reset(anxi);
				m_fCurStep[anxi] = 0.0;
				Sleep(100);
				theApp.m_StageMC6600.LimRelease(0,ucAxis);
				Sleep(100);
				theApp.m_StageMC6600.Enable(anxi);
				m_bEnable[anxi] = TRUE;
				DiffCenter(anxi);
				m_fDis[anxi] = (float)(m_fCali[anxi] *m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi]);
			}
		}
		else
		{
			sprintf_s( szBuf, "Axis %c goes to the HOME-mechanical", ucAxis );	// 到机械原点-机械
			Message( szBuf );
			theApp.m_StageMC6600.close(anxi);
			m_bEnable[anxi] = FALSE;
			Sleep(100);
			theApp.m_StageMC6600.LocaZero(anxi);
			Sleep(100);
			theApp.m_StageMC6600.Reset(anxi);
			m_fCurStep[anxi] = 0.0;
			Sleep(100);
			theApp.m_StageMC6600.Enable(anxi);
			m_bEnable[anxi] = TRUE;
			theApp.m_StageMC6600.LimRelease(0,ucAxis);
			sprintf_s( szBuf, "Kill Timer %c", ucAxis );
			Message( szBuf );
			KillTimer(anxi+1);
			m_nTimer[anxi] = 0;
			m_bToHome[anxi] =FALSE;
			m_fGoal[anxi] = 0.0;
			GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText("0.000");
// 			GetCurStep(anxi);
// 			GetDlgItem(IDC_EDIT_X_CurStep +anxi)->Invalidate();
			m_nTimer[anxi] = SetTimer( anxi+1, nStageMC6600_Elapse, NULL );
			m_bNum[anxi] = TRUE;
			m_bIsMoving[anxi] = FALSE;
			m_nPhaseInit[anxi] = -1;
			if(m_nProcess > 0 && !m_bIsMoving[0] && !m_bIsMoving[1])
				MoveSteps();
			// 写入配置文件
			SaveToINI( 1 );
			if(m_bSoftLimit)
			{
				m_paramZRes.fLimitNeg[anxi] = m_paramZRes.fLimitNeg[anxi] + m_fCali[anxi];
				m_paramZRes.fLimitPos[anxi] = m_paramZRes.fLimitPos[anxi] + m_fCali[anxi];
				m_factualLimit[anxi*2] = m_factualLimitH[anxi*2] + m_fCali[anxi];
				m_factualLimit[anxi*2+1] = m_factualLimitH[anxi*2+1] + m_fCali[anxi];
			}
		}
	}
}

void CPanelStage::ToHome_ZRT( unsigned char ucAxis, short anxi )
{
	char szBuf[255];
	/*GetCurStep(anxi);
	float fState = theApp.m_StageMC6600.StatusRead(ucAxis);*/
	if(anxi == 3)
		m_paramsCurrent.nStatus[3] = 0;
	if(m_paramsCurrent.nStatus[anxi] > 9 && m_paramsCurrent.nStatus[anxi] < 12)	// fState == 10 || fState == 11)
	{
			//处于安全，限位断电
		theApp.m_StageMC6600.close(anxi);
		m_bEnable[anxi] = FALSE;
		KillTimer(anxi+1);
		m_nTimer[anxi] = 0;
		if(m_bNum[anxi] && m_nPhaseInit[anxi] == 1)
		{
			// 到负向极限
			sprintf_s( szBuf, "Axis %c goes to the negative limit: %.2f", ucAxis, m_fCurStep[anxi]/m_dEquivalent[anxi] );
			Message( szBuf );

			if( anxi < 3 )
				m_fDis[anxi] = (float)(m_fCurStep[anxi] + m_fCalibrationZ*m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi]);
			else if(anxi < 4)
				m_fDis[anxi] = (float)(m_fCurStep[anxi] + m_fCalibrationR*m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi]);
			else
				m_fDis[anxi] = (float)(m_fCurStep[anxi] + m_fCalibrationT*m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi]);

			Message("Get the target position");	// 获取目标位置
			theApp.m_StageMC6600.GoHome(ucAxis, m_fDis[anxi]);

			if(m_paramsCurrent.nStatus[anxi] < 11)	// == 10
			{
				m_paramLimitDir.fA[anxi] = -1 *m_paramDriveDir.nDir[anxi];
				m_paramLimitDir.fB[anxi] = 1 *m_paramDriveDir.nDir[anxi];
			}
			else			// == 11
			{
				m_paramLimitDir.fB[anxi] = -1 *m_paramDriveDir.nDir[anxi];
				m_paramLimitDir.fA[anxi] = 1 *m_paramDriveDir.nDir[anxi];
			}
			m_nPhaseInit[anxi] = 2;
			m_bNum[anxi] = FALSE;
		}
			
		theApp.m_StageMC6600.Enable(anxi);
		m_bEnable[anxi] = TRUE;
		Sleep(100);
		m_nTimer[anxi] = SetTimer( anxi +1, nStageMC6600_Elapse, NULL );
	}
	else if( !m_bNum[anxi] && m_nPhaseInit[anxi] == 2 && fabs(m_fCurStep[anxi]-m_fDis[anxi])< m_fDeadline[anxi]*m_dEquivalent[anxi] )
	{
		if( m_bOrigion[anxi] )
		{
			if( fabs(m_fDis[anxi] / m_dEquivalent[anxi] -m_fCali[anxi]* m_paramDriveDir.nDir[anxi])< m_fDeadline[anxi] )
			{
				sprintf_s( szBuf, "Axis %c goes to the original", ucAxis );	// 到扫描原点
				Message( szBuf );
				KillTimer(anxi+1);
				m_nTimer[anxi] = 0;
				theApp.m_StageMC6600.close(anxi);
				m_bEnable[anxi] = FALSE;
				Sleep(100);
				theApp.m_StageMC6600.LocaZero(anxi);
				Sleep(100);
				theApp.m_StageMC6600.Reset(anxi);
				m_fGoal[anxi] = 0;
				//theApp.m_StageMC6600.CurrPosR(anxi);
				m_fCurStep[anxi] = 0.0;
				m_bOrigion[anxi] =FALSE;
				m_bToHome[anxi] =FALSE;
				m_bNum[anxi] = TRUE;
				m_nTimer[anxi] = SetTimer( anxi+1, nStageMC6600_Elapse, NULL );
				GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText("0.0");
				m_nPhaseInit[anxi] = -1;
				if(m_nProcess > 0)
					MoveSteps();
				//R轴左限位无效
				if(anxi == 3)
					theApp.m_StageMC6600.LimRelease(1,ucAxis);
				if(m_bSoftLimit)
				{
					m_paramZRes.fLimitNeg[anxi] = m_paramZRes.fLimitNeg[anxi] - m_fCali[anxi];
					m_paramZRes.fLimitPos[anxi] = m_paramZRes.fLimitPos[anxi] - m_fCali[anxi];
					m_factualLimit[anxi*2] = m_factualLimitH[anxi*2] - m_fCali[anxi];
					m_factualLimit[anxi*2+1] = m_factualLimitH[anxi*2+1] - m_fCali[anxi];
				}
			}
			else
			{
				sprintf_s( szBuf, "Axis %c goes to the HOME-scan", ucAxis );	// 到机械原点-扫描
				Message( szBuf );
				theApp.m_StageMC6600.close(anxi);
				m_bEnable[anxi] = FALSE;
				Sleep(100);
				theApp.m_StageMC6600.LocaZero(anxi);
				Sleep(100);
				theApp.m_StageMC6600.Reset(anxi);

				m_fCurStep[anxi] = 0.0;
				Sleep(100);
				theApp.m_StageMC6600.LimRelease(0,ucAxis);
				Sleep(100);
				theApp.m_StageMC6600.Enable(anxi);
				m_bEnable[anxi] = TRUE;
				DiffCenter(anxi);
				m_fDis[anxi] = (float)(m_fCali[anxi]*m_dEquivalent[anxi]* m_paramDriveDir.nDir[anxi]);
			}
		}
		else
		{
			sprintf_s( szBuf, "Axis %c goes to the HOME-mechanical", ucAxis );	// 到机械原点-机械
			Message( szBuf );
			theApp.m_StageMC6600.close(anxi);
			m_bEnable[anxi] = FALSE;
			Sleep(100);
			theApp.m_StageMC6600.LocaZero(anxi);
			Sleep(100);
			theApp.m_StageMC6600.Reset(anxi);
			m_fCurStep[anxi] = 0.0;
			Sleep(100);
			theApp.m_StageMC6600.Enable(anxi);
			m_bEnable[anxi] = TRUE;
			theApp.m_StageMC6600.LimRelease(0,ucAxis);
			sprintf_s( szBuf, "Kill Timer %c", ucAxis );
			Message( szBuf );
			KillTimer(anxi +1);
			m_nTimer[anxi] = 0;
			m_bToHome[anxi] =FALSE;
			m_fGoal[anxi] = 0.0;
			if( anxi > 2 )
				GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText("0.00");
			else
				GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText("0.000");
// 			GetCurStep(anxi);
// 			GetDlgItem(IDC_EDIT_X_CurStep +anxi)->Invalidate();
			m_nTimer[anxi] = SetTimer( anxi +1, nStageMC6600_Elapse, NULL );
			m_bNum[anxi] = TRUE;
			m_bIsMoving[anxi] = FALSE;
			m_nPhaseInit[anxi] = -1;
			//R轴左限位无效
			if(anxi == 3)
				theApp.m_StageMC6600.LimRelease(1,ucAxis);
			if(m_nProcess > 0 && !m_bIsMoving[0] && !m_bIsMoving[1])
				MoveSteps();
			// 写入配置文件
			SaveToINI( 1 );
			if(m_bSoftLimit)
			{
				m_paramZRes.fLimitNeg[anxi] = m_paramZRes.fLimitNeg[anxi] + m_fCali[anxi];
				m_paramZRes.fLimitPos[anxi] = m_paramZRes.fLimitPos[anxi] + m_fCali[anxi];
				m_factualLimit[anxi*2] = m_factualLimitH[anxi*2] + m_fCali[anxi];
				m_factualLimit[anxi*2+1] = m_factualLimitH[anxi*2+1] + m_fCali[anxi];
			}
		}
	}
}

void CPanelStage::NonToHome( unsigned char ucAxis, short anxi )
{
	float lStep = m_fGoal[anxi];
	char szBuf[255] = {0};
//	GetCurStep(anxi);
//	float fState = theApp.m_StageMC6600.StatusRead(ucAxis);
	//R轴限位屏蔽
	if(anxi == 3)
		m_paramsCurrent.nStatus[3] = 0;
	
	if(m_paramsCurrent.nStatus[anxi] > 9 && m_paramsCurrent.nStatus[anxi] < 12)	// fState == 10 || fState == 11)
	{
		//AfxMessageBox("触碰极限,进行回退！");
		theApp.m_StageMC6600.close(anxi);
		m_bEnable[anxi] = FALSE;
		//GetCurStep(anxi);
		if(m_paramsCurrent.nStatus[anxi] < 11 )	// == 10
		{
			if((m_bDirection[anxi] && m_paramLimitDir.fA[anxi] ==1) ||  (!m_bDirection[anxi] && m_paramLimitDir.fA[anxi] == -1) )
			{
				KillTimer(anxi+1);
				m_nTimer[anxi] = 0;
				sprintf_s( szBuf, "Kill Timer %c", ucAxis );
				Message( szBuf );
				Sleep(100);	// 延时1秒
				m_bIsMoving[anxi] = FALSE;
				//是否回影响回读值？
				CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
				m_bBL[anxi] = FALSE;
				SafeLimit(ucAxis, m_paramsCurrent.nStatus[anxi]);
			}
			else 
			{
				sprintf_s( szBuf, "%c %s :%.6f", ucAxis, m_strMsg[9], m_fCurStep[anxi]/m_dEquivalent[anxi] );	// 极限异常
				Message( szBuf );
				KillTimer(anxi+1);
				m_nTimer[anxi] = 0;
				theApp.m_StageMC6600.LimRelease(1,ucAxis);
				if(m_paramLimitDir.fA[anxi] ==1)
				{
					m_fGoal[anxi] = theApp.m_StageMC6600.BackOff(ucAxis,1);
					theApp.m_StageMC6600.Enable(anxi);
					m_bEnable[anxi] = TRUE;
					//GetDlgItem(IDC_EDIT_X_CurStep +anxi)->SetWindowText(szBuf);
					sprintf_s( szBuf, "%c %s", ucAxis, m_strMsg[10] );	// 正向极限异常
					MessageBox( szBuf, m_strMsg[0], MB_OK | MB_ICONERROR );
				}
				else
				{
					m_fGoal[anxi] = theApp.m_StageMC6600.BackOff(ucAxis,-1);
					theApp.m_StageMC6600.Enable(anxi);
					m_bEnable[anxi] = TRUE;
					sprintf_s( szBuf, "%c %s", ucAxis, m_strMsg[11] );	// 负向极限异常
					MessageBox( szBuf, m_strMsg[0], MB_OK | MB_ICONERROR );
				}
				Sleep(50);
				theApp.m_StageMC6600.LimRelease(0,ucAxis);
				Sleep(50);
				/*Sleep(50);
				theApp.m_StageMC6600.LimRelease(0,ucAxis);
				m_nTimer[anxi] = SetTimer( anxi +1, nStageMC6600_Elapse, NULL );*/
			}
		}
		else	// == 11
		{
			if((m_bDirection[anxi] && m_paramLimitDir.fB[anxi] ==1) ||  (!m_bDirection[anxi] && m_paramLimitDir.fB[anxi] == -1) )
			{
				KillTimer(anxi+1);
				m_nTimer[anxi] = 0;
				sprintf_s( szBuf, "Kill Timer %c", ucAxis );
				Message( szBuf );
				Sleep(100);	// 延时1秒
				m_bIsMoving[anxi] = FALSE;
				CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
				m_bBL[anxi] = FALSE;
				SafeLimit(ucAxis,m_paramsCurrent.nStatus[anxi]);
			}
			else
			{
				sprintf_s( szBuf, "%c %s :%.6f", ucAxis, m_strMsg[9], m_fCurStep[anxi] );	// 极限异常
				Message( szBuf );
				KillTimer(anxi+1);
				m_nTimer[anxi] = 0;
				theApp.m_StageMC6600.LimRelease(1,ucAxis);
				if(m_paramLimitDir.fB[anxi] ==1)
				{
					m_fGoal[anxi] = theApp.m_StageMC6600.BackOff(ucAxis,1);
					theApp.m_StageMC6600.Enable(anxi);
					m_bEnable[anxi] = TRUE;
					sprintf_s( szBuf, "%c %s", ucAxis, m_strMsg[10] );	// 正向极限异常
					MessageBox( szBuf, m_strMsg[0], MB_OK | MB_ICONERROR );
				}
				else
				{
					m_fGoal[anxi] = theApp.m_StageMC6600.BackOff(ucAxis,-1);
					theApp.m_StageMC6600.Enable(anxi);
					m_bEnable[anxi] = TRUE;
					sprintf_s( szBuf, "%c %s", ucAxis, m_strMsg[11] );	// 负向极限异常
					MessageBox( szBuf, m_strMsg[0], MB_OK | MB_ICONERROR );
				}
				Sleep(50);
				theApp.m_StageMC6600.LimRelease(0,ucAxis);
				Sleep(50);
				/*Sleep(50);
				theApp.m_StageMC6600.LimRelease(0,ucAxis);
				m_nTimer[anxi] = SetTimer( anxi+1, nStageMC6600_Elapse, NULL );*/
			}
		}
	}

	else if(m_bBL[anxi])
	{
		//m_fBLCount[anxi] = theApp.m_StageMC6600.CoherenceCheck_Backlash(ucAxis,lStep,FALSE);
		if( fabs(m_fBLCount[anxi] -m_fCurStep[anxi]) <= m_paramBl.fCount[anxi] /3. )
		{
			 theApp.m_StageMC6600.MoveLimit(ucAxis, lStep);
			 m_bBL[anxi] = FALSE;
		}
	}
	else
	{
/*		if( m_fCurStep[anxi] /lStep > 0.8)
		{
			float Ivalue ;
			GetDlgItem(IDC_EDIT_IX +anxi)->GetWindowText(str);	// mm
			Ivalue = (long)(atof(str) *1000.0);
			theApp.m_StageMC6600.ItoModify(ucAxis,Ivalue );
		}
*/		if( fabs(lStep -m_fCurStep[anxi]) <= m_fDeadline[anxi]*m_dEquivalent[anxi] )
		{
			//theApp.m_StageMC6600.close(anxi);
			//GetCurStep(anxi);
			theApp.m_StageMC6600.Move(ucAxis,m_fCurStep[anxi],m_bRelative);
			m_fGoal[anxi] = m_fCurStep[anxi] *m_paramDriveDir.nDir[anxi];
			KillTimer(anxi+1);
			m_nTimer[anxi] = 0;
			m_bEqual[anxi] = FALSE;
			sprintf_s( szBuf, "KillTimer %c", ucAxis );
			Message( szBuf );
			//Sleep(500);
			//sprintf_s( szBuf, " %f", m_fGoal[0]/m_dEquivalent[0] );
			if(m_bHighPrecCount)
			{
				if( anxi < 3)
					sprintf_s( szBuf, "%.3f, %.5f", m_fGoal[anxi] /m_dEquivalent[anxi], m_fGoal[anxi] /m_dEquivalent[anxi] );
				else
					sprintf_s( szBuf, "%.2f, %.5f", m_fGoal[anxi] /m_dEquivalent[anxi], m_fGoal[anxi] /m_dEquivalent[anxi] );
			}
			else
			{
				if( anxi < 3 )
					sprintf_s( szBuf, "%.3f", m_fGoal[anxi] /m_dEquivalent[anxi] );
				else
					sprintf_s( szBuf, "%.2f", m_fGoal[anxi] /m_dEquivalent[anxi] );
			}

			GetDlgItem(IDC_EDIT_X_CurStep +anxi)->SetWindowText(szBuf);
			m_fCurStep[anxi] = m_fGoal[anxi] /m_paramDriveDir.nDir[anxi];

			//Sleep(500);	// 延时1秒
			m_bIsMoving[anxi] = FALSE;
			CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
			if(m_nProcess < 0)
				MoveSteps();
			GetDlgItem(IDC_EDIT_X_CurStep +anxi)->Invalidate();	
		}
	}
}
/*
void CPanelStage::NonToHome( unsigned char ucAxis, short anxi )
{
	float lStep = m_fGoal[anxi];
//	GetCurStep(anxi);
//	float fState = theApp.m_StageMC6600.StatusRead(ucAxis);
	char szBuf[255];
	if( anxi != 3 )
	{
//		if(fState > 9 && fState < 12)	// fState == 10 || fState == 11)
		if(m_paramsCurrent.fStatus[anxi] > 9 && m_paramsCurrent.fStatus[anxi] < 12)	// fState == 10 || fState == 11)
		{
			//AfxMessageBox("触碰极限,进行回退！");
			theApp.m_StageMC6600.close(anxi);
//			GetCurStep(anxi);
//			if(fState < 11 )	// == 10
			if(m_paramsCurrent.fStatus[anxi] < 11 )	// == 10
			{
				if((m_bDirection[anxi] && m_paramLimitDir.fA[anxi] ==1) ||  (!m_bDirection[anxi] && m_paramLimitDir.fA[anxi] == -1) )
				{
					KillTimer(anxi+1);
					m_nTimer[anxi] = 0;
					sprintf_s( szBuf, "Kill Timer %c", ucAxis );
					Message( szBuf );
					Sleep(1000);	// 延时1秒
					m_bIsMoving[anxi] = FALSE;
					m_bBL[anxi] = FALSE;
					SafeLimit(ucAxis, m_paramsCurrent.fStatus[anxi]);	//fState);
				}
				else if(!m_bDirection[anxi])
				{
				}
				else 
				{
					sprintf_s( szBuf, "%c %s :%.6f", ucAxis, m_strMsg[9], m_fCurStep[anxi]/m_dEquivalent[anxi] );	// 极限异常
					Message( szBuf );
					KillTimer(anxi+1);
					m_nTimer[anxi] = 0;
					theApp.m_StageMC6600.LimRelease(1,ucAxis);
					if(m_paramLimitDir.fA[anxi] ==1)
					{
						m_fGoal[anxi] = theApp.m_StageMC6600.BackOff(ucAxis,1);
						theApp.m_StageMC6600.Enable(anxi);
						GetDlgItem(IDC_EDIT_X_CurStep +anxi)->SetWindowText(szBuf);
						sprintf_s( szBuf, "%c %s", ucAxis, m_strMsg[10] );	// 正向极限异常
						MessageBox( szBuf, m_strMsg[0], MB_OK | MB_ICONERROR );
					}
					else
					{
						m_fGoal[anxi] = theApp.m_StageMC6600.BackOff(ucAxis,-1);
						theApp.m_StageMC6600.Enable(anxi);
						sprintf_s( szBuf, "%c %s", ucAxis, m_strMsg[11] );	// 负向极限异常
						MessageBox( szBuf, m_strMsg[0], MB_OK | MB_ICONERROR );
					}
					Sleep(50);
					theApp.m_StageMC6600.LimRelease(0,ucAxis);
					/*Sleep(50);
					//theApp.m_StageMC6600.LimRelease(0,ucAxis);
					//m_nTimer[anxi] = SetTimer( anxi +1, nStageMC6600_Elapse, NULL );
				}
			}
			else	// == 11
			{
				if((m_bDirection[anxi] && m_paramLimitDir.fB[anxi] ==1) ||  (!m_bDirection[anxi] && m_paramLimitDir.fB[anxi] == -1) )
				{
					KillTimer(anxi+1);
					m_nTimer[anxi] = 0;
					sprintf_s( szBuf, "Kill Timer %c", ucAxis );
					Message( szBuf );
					Sleep(1000);	// 延时1秒
					m_bIsMoving[anxi] = FALSE;
					m_bBL[anxi] = FALSE;
					SafeLimit(ucAxis, m_paramsCurrent.fStatus[anxi]);	//fState);
				}
				else if(!m_bDirection[anxi])
				{
				}
				else
				{
					sprintf_s( szBuf, "%c %s :%.6f", ucAxis, m_strMsg[9], m_fCurStep[anxi] );	// 极限异常
					Message( szBuf );
					KillTimer(anxi+1);
					m_nTimer[anxi] = 0;
					theApp.m_StageMC6600.LimRelease(1,ucAxis);
					if(m_paramLimitDir.fB[anxi] ==1)
					{
						m_fGoal[anxi] = theApp.m_StageMC6600.BackOff(ucAxis,1);
						theApp.m_StageMC6600.Enable(anxi);
						sprintf_s( szBuf, "%c %s", ucAxis, m_strMsg[10] );	// 正向极限异常
						MessageBox( szBuf, m_strMsg[0], MB_OK | MB_ICONERROR );
					}
					else
					{
						m_fGoal[anxi] = theApp.m_StageMC6600.BackOff(ucAxis,-1);
						theApp.m_StageMC6600.Enable(anxi);
						sprintf_s( szBuf, "%c %s", ucAxis, m_strMsg[11] );	// 负向极限异常
						MessageBox( szBuf, m_strMsg[0], MB_OK | MB_ICONERROR );
					}
					Sleep(50);
					theApp.m_StageMC6600.LimRelease(0,ucAxis);
					/*Sleep(50);
					//theApp.m_StageMC6600.LimRelease(0,ucAxis);
					//m_nTimer[anxi] = SetTimer( anxi+1, nStageMC6600_Elapse, NULL );
				}
			}
		}
	}
	if(m_bBL[anxi])
	{
		//m_fBLCount[anxi] = theApp.m_StageMC6600.CoherenceCheck_Backlash(ucAxis,lStep,FALSE);
		if( fabs(m_fBLCount[anxi] -m_fCurStep[anxi]) <= m_paramBl.fCount[anxi] /3. )
		{
			 theApp.m_StageMC6600.MoveLimit(ucAxis, lStep);
			 m_bBL[anxi] = FALSE;
		}
	}
	else
	{
		//if( m_fCurStep[anxi] /lStep > 0.8)
		//{
		//	float Ivalue ;
		//	GetDlgItem(IDC_EDIT_IX +anxi)->GetWindowText(str);	// mm
		//	Ivalue = (long)(atof(str) *1000.0);
		//	theApp.m_StageMC6600.ItoModify(ucAxis,Ivalue );
		//}
		if( fabs(lStep -m_fCurStep[anxi]) <= m_fDeadline[anxi]*m_dEquivalent[anxi] )
		{
			theApp.m_StageMC6600.close(anxi);
//			GetCurStep(anxi);
			if( fabs(lStep -m_fCurStep[anxi]) <= m_fDeadline[anxi]*m_dEquivalent[anxi] )
			{
				m_fGoal[anxi] = theApp.m_StageMC6600.CurrPosR(anxi) *m_paramDriveDir.nDir[anxi];
				KillTimer(anxi+1);
				m_nTimer[anxi] = 0;
				//Sleep(500);
// 				sprintf_s( szBuf, " %f", m_fGoal[0]/m_dEquivalent[0] );
				if(m_bHighPrecCount)
				{
					if( anxi < 3)
						sprintf_s( szBuf, "%.3f, %.5f", m_fGoal[anxi] /m_dEquivalent[anxi], m_fGoal[anxi] /m_dEquivalent[anxi] );
					else if( anxi < 4 )
//						sprintf_s( szBuf, "%s", RDriveConversion_Disp(m_fGoal[anxi], m_fGoal[anxi]) );
					{
						if(m_fGoal[anxi]/m_dEquivalent[anxi] > 179)
							sprintf_s( szBuf, "%.2f, %.5f", m_fGoal[anxi]/m_dEquivalent[anxi] - 360, m_fGoal[anxi]/m_dEquivalent[anxi] - 360);
						else if(m_fGoal[anxi]/m_dEquivalent[anxi] < -179)
							sprintf_s( szBuf, "%.2f, %.5f", m_fGoal[anxi]/m_dEquivalent[anxi] + 360, m_fGoal[anxi]/m_dEquivalent[anxi] + 360);
						else if(m_fGoal[anxi]/m_dEquivalent[anxi] > 359)
							sprintf_s( szBuf, "%d, %d", (int)(m_fGoal[anxi]/m_dEquivalent[anxi])% 360, (int)(m_fGoal[anxi]/m_dEquivalent[anxi])% 360);
						else if(m_fGoal[anxi]/m_dEquivalent[anxi] < -359)
							sprintf_s( szBuf, "%d, %d", (int)(m_fGoal[anxi]/m_dEquivalent[anxi])% (-360), (int)(m_fGoal[anxi]/m_dEquivalent[anxi])% (-360));
						else
							sprintf_s( szBuf, "%.2f, %.5f", m_fGoal[anxi] /m_dEquivalent[anxi], m_fGoal[anxi] /m_dEquivalent[anxi] );
					}
					else
						sprintf_s( szBuf, "%.2f, %.5f", m_fGoal[anxi] /m_dEquivalent[anxi], m_fGoal[anxi] /m_dEquivalent[anxi] );
				}
				else
				{
					if( anxi < 3 )
						sprintf_s( szBuf, "%.3f", m_fGoal[anxi] /m_dEquivalent[anxi] );
					else if( anxi < 4 )
//						sprintf_s( szBuf, "%s", RDriveConversion_Disp(m_fGoal[anxi], m_fGoal[anxi]) );
					{
						if(m_fGoal[anxi]/m_dEquivalent[anxi] > 179)
							sprintf_s( szBuf, "%.2f", m_fGoal[anxi]/m_dEquivalent[anxi] - 360);
						else if(m_fGoal[anxi]/m_dEquivalent[anxi] < -179)
							sprintf_s( szBuf, "%.2f", m_fGoal[anxi]/m_dEquivalent[anxi] + 360);
						else if(m_fGoal[anxi]/m_dEquivalent[anxi] > 359)
							sprintf_s( szBuf, "%d", (int)(m_fGoal[anxi]/m_dEquivalent[anxi])% 360);
						else if(m_fGoal[anxi]/m_dEquivalent[anxi] < -359)
							sprintf_s( szBuf, "%d", (int)(m_fGoal[anxi]/m_dEquivalent[anxi])% (-360));
						else
							sprintf_s( szBuf, "%.2f", m_fGoal[anxi] /m_dEquivalent[anxi] );
					}
					else
						sprintf_s( szBuf, "%.2f", m_fGoal[anxi] /m_dEquivalent[anxi] );
				}

				GetDlgItem(IDC_EDIT_X_CurStep +anxi)->SetWindowText(szBuf);
				m_fCurStep[anxi] = m_fGoal[anxi] /m_paramDriveDir.nDir[anxi];
				
				sprintf_s( szBuf, "KillTimer %c", ucAxis );
				Message( szBuf );
				Sleep(1000);	// 延时1秒
				m_bIsMoving[anxi] = FALSE;

				if(m_nProcess < 0)
					MoveSteps();

				GetDlgItem(IDC_EDIT_X_CurStep +anxi)->Invalidate();	
		   }
		   else
		   {
			   theApp.m_StageMC6600.Enable(anxi);
		   }
		}
		Sleep(500);
	}
}
*/
void CPanelStage::Timer_Axis( short anxi )
{
	unsigned char ucAxis = SerialNum( anxi );
	//float state;
	//state = theApp.m_StageMC6600.StatusRead(ucAxis);
	if(m_paramsCurrent.nStatus[anxi] > 11)	//== 12)
		Grounding( ucAxis, anxi );
	if(m_bToHome[anxi])
	{
		switch( anxi )
		{
		case	0:
		case	1:
			ToHome_XY( ucAxis, anxi );
			break;
		case	2:
		case	3:
		case	4:
			ToHome_ZRT( ucAxis, anxi );
			break;
		}
	}
	else
		NonToHome( ucAxis, anxi );
}

void CPanelStage::Timer_CurPos( short anxi )
{
	char szBuf[255] = {0};
	//GetCurStep(anxi);
	unsigned char ucAxis = SerialNum( anxi );
	//m_fGoal[anxi] = theApp.m_StageMC6600.GetCurPosition(ucAxis)*m_paramDriveDir.nDir[anxi];
	m_fGoal[anxi] = m_fCurStep[anxi] *m_paramDriveDir.nDir[anxi];
	if(m_bHighPrecCount)
	{
		if( anxi < 3)
			sprintf_s( szBuf, "%.3f, %.5f", m_fGoal[anxi] /m_dEquivalent[anxi], m_fGoal[anxi] /m_dEquivalent[anxi]);
		else
			sprintf_s( szBuf, "%.2f, %.5f", m_fGoal[anxi] /m_dEquivalent[anxi], m_fGoal[anxi] /m_dEquivalent[anxi] );
	}
	else
	{
		if( anxi < 3 )
			sprintf_s( szBuf, "%.3f", m_fGoal[anxi] /m_dEquivalent[anxi] );
		else
			sprintf_s( szBuf, "%.2f", m_fGoal[anxi] /m_dEquivalent[anxi] );
	}
	GetDlgItem(IDC_EDIT_X_CurStep +anxi)->SetWindowText(szBuf);
}

void CPanelStage::Timer_KillTimer( UINT nIDEvent )
{
	short anxi, nTimerIndex;
	switch( nIDEvent )
	{
	case	11:
	case	12:
		anxi		= 0;
		nTimerIndex = 5;
		break;
	case	21:
	case	22:
		anxi		= 1;
		nTimerIndex = 6;
		break;
	case	31:
	case	32:
		anxi		= 2;
		nTimerIndex = 7;
		break;
	case	41:
	case	42:
		anxi		= 3;
		nTimerIndex = 8;
		break;
	case	51:
	case	52:
		anxi		= 4;
		nTimerIndex = 9;
		break;
	}
	KillTimer( nIDEvent );
	m_nTimer[nTimerIndex] = 0;
	unsigned char ucAxis = SerialNum( anxi );
	char szBuf[255];
	sprintf_s( szBuf, "KillTimer-%c", ucAxis );
	Message( szBuf );
}

void CPanelStage::Timer_Tracklocus( UINT nIDEvent, short anxi )
{
	if( m_bTrackState[anxi] )
	{
		if( (nIDEvent % 2) != 0 )	// 11/21/31/41/51
		{
			if(!m_bDirection[anxi])
				Timer_KillTimer( nIDEvent );
			SetTracklocusAxis( anxi, TRUE);
		}
		else						// 12/22/32/42/52
		{
			if(m_bDirection[anxi])
				Timer_KillTimer( nIDEvent );
			SetTracklocusAxis( anxi, FALSE);
		}
	}
	else
	{
		Timer_KillTimer( nIDEvent );
		SetTracklocusAxis( anxi, FALSE );
	}
}

void CPanelStage::StartPos(short anxi)
{
	unsigned char ucAxis = SerialNum(anxi);

	//float fState = theApp.m_StageMC6600.StatusRead(ucAxis);
	//R轴限位屏蔽
	if(anxi == 3)
		m_paramsCurrent.nStatus[3] = 0;

	if(m_paramsCurrent.nStatus[anxi] > 9 && m_paramsCurrent.nStatus[anxi] < 12)	// fState == 10 || fState == 11)
		StartPos_LimitDir( anxi, ucAxis );
	//fState = theApp.m_StageMC6600.StatusRead(ucAxis);
	if(anxi == 3)
		m_paramsCurrent.nStatus[3] = 0;
	if(m_paramsCurrent.nStatus[anxi] > 9 && m_paramsCurrent.nStatus[anxi] < 12)	// fState == 10 || fState == 11)
		StartPos(anxi);

	Sleep(100);
	//float fStep = theApp.m_StageMC6600.CurrPosO(ucAxis);
	float fStep = m_fCurStep[anxi];
	CString str;
	str = DisplayOutput(anxi, fStep, fStep); 
	GetDlgItem(IDC_EDIT_X_CurStep +anxi)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText(str);
	//m_fCurStep[anxi] = fStep;
}

void CPanelStage::StartPos_LimitDir( short anxi, unsigned char ucAxis )
{
	float fLimitDir = 0.0f;
	if(m_paramsCurrent.nStatus[anxi] < 11 )			// == 10
		fLimitDir = m_paramLimitDir.fA[anxi];
	else
		fLimitDir = m_paramLimitDir.fB[anxi];

	char szBuf[255] = {0};
	if( fLimitDir > 0 )			// 1
	{
		sprintf_s( szBuf, "%c %s", ucAxis, m_strMsg[6] );
		if( MessageBox( szBuf, m_strMsg[0], MB_YESNO ) == IDYES )
			Message("It is in the positive limit position");	// 处于正向极限位置
	}
	else if( fLimitDir < 0 )	// -1
	{
		sprintf_s( szBuf, "%c %s", ucAxis, m_strMsg[7] );
		if( MessageBox( szBuf, m_strMsg[0], MB_YESNO ) == IDYES )
			Message("It is in the negative limit position");	// 处于负向极限位置
	}
	theApp.m_StageMC6600.LimRelease(1,ucAxis);
	Sleep(50);
	m_fGoal[anxi] = theApp.m_StageMC6600.BackOff(ucAxis,fLimitDir);
	//m_fCurStep[anxi] = m_fGoal[anxi];
	theApp.m_StageMC6600.Enable(anxi);
	m_bEnable[anxi] = TRUE;
	Sleep(50);
	m_bIsMoving[anxi] = TRUE;
	CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
	Sleep(50);
	theApp.m_StageMC6600.LimRelease(0,ucAxis);

	CString str;
//	str = DisplayOutput1(anxi,m_fGoal[anxi],m_fGoal[anxi]); 
	if( anxi < 3 )
		str.Format( "%.3f", m_fGoal[anxi]/m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi] );
	else
		str.Format( "%.2f", m_fGoal[anxi]/m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi] );
	GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText(str);
	m_nTimer[anxi] = SetTimer( anxi + 1, nStageMC6600_Elapse, NULL );
}

void CPanelStage::FindIndex()
{
// 	if( m_bIsMoving )
// 		return;
	unsigned char ucAxis;
	int i;
	//float fState;
	for(i=0; i<5; i++)
	{
		ucAxis = SerialNum(i);
		//fState= theApp.m_StageMC6600.StatusRead(ucAxis);

		if(m_paramsCurrent.nStatus[i] > 11)
			Grounding( ucAxis, i );
		else
		{
			theApp.m_StageMC6600.Enable(i);
			m_bEnable[i] = TRUE;
		}
	}

	// 先回机械原点，再回扫描中心，并定义其为用户原点
	if( MessageBox( m_strMsg[1], m_strMsg[0], MB_OKCANCEL ) == IDOK )
	{
		// 先回机械原点
		Message("Start FindIndex");
		//R轴限位启用
		theApp.m_StageMC6600.LimRelease(0,'R');
		SetUserOrigin();
	}
	else
	{
		//R轴限位禁用
		theApp.m_StageMC6600.LimRelease(1,'R');
		for( i=0; i<5; i++ )
		{
			if( m_bMotorFault[i] && m_bCommunicationFault[i] )	// 调试用
				StartPos(i);
		}
	}
	if(!QueryMoving())
	{
		for( i=0; i<5; i++ )
		{
			m_fGoal[i] = m_fCurStep[i];
			CStageMC6600Manager::Instance().m_bIsMoving[i] = FALSE;
		}
	}
}

void CPanelStage::Safe_Axis( unsigned char ucAxis, float fState, int nType )
{
	short anxi = NumSerial( ucAxis );
	theApp.m_StageMC6600.LimRelease(1,ucAxis);
	if( nType < 1 )
		Sleep(50);
	if( nType < 1 )
	{
		// SafeLimit: 触碰到极限
		if(fState > 10)		//== 11)
			m_fGoal[anxi] = theApp.m_StageMC6600.BackOff(ucAxis, m_paramLimitDir.fB[anxi]);
		else if(fState > 9)	//== 10)
			m_fGoal[anxi] = theApp.m_StageMC6600.BackOff(ucAxis, m_paramLimitDir.fA[anxi]);
	}
	else
	{
		// SafeGnd: 出现接地
		if(m_bDirection[anxi] > 0)
			m_fGoal[anxi] = theApp.m_StageMC6600.BackOff(ucAxis, 1);
		else if(m_bDirection[anxi] < 0)
			m_fGoal[anxi] = theApp.m_StageMC6600.BackOff(ucAxis, -1);
		if( anxi == 2 )
			Message("Z performing a rollback");	// Z执行回退
	}
	theApp.m_StageMC6600.Enable(anxi);
	m_bEnable[anxi] = TRUE;
	m_bIsMoving[anxi] = TRUE;
	CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
	if( nType < 1 )
		Sleep(1000);
	else
		Sleep(100);
	theApp.m_StageMC6600.LimRelease(0,ucAxis);
	CString str;
//	str = DisplayOutput1(anxi,m_fGoal[anxi],m_fGoal[anxi]);
	if( anxi < 3 )
		str.Format( "%.3f", m_fGoal[anxi]/m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi] );
	else
		str.Format( "%.2f", m_fGoal[anxi]/m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi] );
	GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText(str);
	//GetCurStep(anxi);
	m_nTimer[anxi] = SetTimer( anxi +1, nStageMC6600_Elapse, NULL );
}

void CPanelStage::SafeLimit(unsigned char ucAxis, float fState)
{
	char szBuf[255] = {0};
	sprintf_s( szBuf, "%c %s", ucAxis, m_strMsg[5] );
	// 触碰到极限，是否回退
	if( MessageBox(szBuf, m_strMsg[0], MB_YESNO ) != IDYES )
		return;
	Safe_Axis( ucAxis, fState, 0 );
}


// 拖拽
void CPanelStage::SetTrackParamAxis( short anxi, int nSpeed )
{
	// 跟踪：设置速度，并进行运动
	// 根据速度值正负判断运动方向
	// 如果与当前运动方向不一致，要停止运动、设置速度(绝对值)、开始运动
	// 如果与当前运动方向一致，只需设置速度(绝对值)

	int nPositive = abs(nSpeed);
	nPositive = (nPositive / 50) *10;	// 将速度分档
	if( nPositive > lSpeedTrackMax )
		nPositive = lSpeedTrackMax;

	if( anxi < 1 )	// X
	{
		if(nSpeed > 0)
			m_bXdir = TRUE;
		else
			m_bXdir = FALSE;
	}
	else
	{
		if(nSpeed > 0)
			m_bYdir = FALSE;
		else
			m_bYdir = TRUE;
	}
	unsigned char ucAxis = SerialNum( anxi );
	char szBuf[255];
	if( abs(nSpeed) < 5 || nPositive == 0 )	// 与跟踪点之间差值较小，停止运动
	{
		theApp.m_StageMC6600.close(anxi);
		//theApp.m_StageMC6600.CurrPosR(anxi);
		theApp.m_StageMC6600.Move(ucAxis,m_fCurStep[anxi],m_bRelative);
		theApp.m_StageMC6600.Enable(anxi);
		sprintf_s( szBuf, "Stop motion in direction %c", ucAxis );	// X方向停止运动
		Message( szBuf );
	}
	else
	{
		BOOL bIsSame = FALSE;
		if( fabs(m_paramsSpeed.fSpeed[anxi] -nPositive/60.) < FLT_EPSILON  )
			bIsSame = TRUE;
		m_paramsSpeed.fSpeed[anxi] = (float)(nPositive/60.);
		if( anxi < 1 )	// X
		{
			if( !m_bXdir && nSpeed < 0 )
			{
				sprintf_s( szBuf, "%c motion in original direction", ucAxis );	// X 原方向运动
				Message( szBuf );
			}
			else
			{
				sprintf_s( szBuf, "%c motion in the opposite direction", ucAxis );	// X 反方向运动
				Message( szBuf );
			}
		}
		else
		{
			if( m_bYdir && nSpeed < 0 )
			{
				sprintf_s( szBuf, "%c motion in original direction", ucAxis );	// X 原方向运动
				Message( szBuf );
			}
			else
			{
				sprintf_s( szBuf, "%c motion in the opposite direction", ucAxis );	// X 反方向运动
				Message( szBuf );
			}
		}
		if( !bIsSame )
		{
			theApp.m_StageMC6600.SetSpeed( ucAxis, m_paramsSpeed.fSpeed[anxi] );
			sprintf_s( szBuf, "Set Speed %c = %f", ucAxis, m_paramsSpeed.fSpeed[anxi] );
			Message( szBuf );
		}
		float fSteps = 0.0;
		if( nSpeed < 0 )
		{
			if( anxi < 1 )	// X
				fSteps = -100.0f;
			else
				fSteps = 50.0f;
		}
		else
		{
			if( anxi < 1 )	// X
				fSteps = 100.0f;
			else
				fSteps = -50.0f;
		}
		fSteps = fSteps * m_paramDriveDir.nDir[anxi];

		theApp.m_StageMC6600.MoveDir(ucAxis, fSteps, FALSE);
		//开启线程
		m_bIsMoving[anxi] = TRUE;
		CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
//		m_nTimer[anxi] = SetTimer( anxi +1, nStageMC6600_Elapse, NULL );
	}
	m_nTimer[anxi] = SetTimer( anxi +1, nStageMC6600_Elapse, NULL );
	Sleep(40);
}

void CPanelStage::SetTrackParam( int nSpeedX, int nSpeedY )
{
	// X向
	SetTrackParamAxis( 0, nSpeedX );
	// Y向
	SetTrackParamAxis( 1, nSpeedY );
}

void CPanelStage::RestoreSpeed()
{
	// 恢复速度
	unsigned char ucAxis;
	for(int i=0;i<2;i++)
	{
		ucAxis = SerialNum(i);
		theApp.m_StageMC6600.SetSpeed(ucAxis, m_paramsSpeed.fSpeedInit[i]);
	}
}

void CPanelStage::StopTrackAxis1( short anxi )
{
	unsigned char ucAxis = SerialNum( anxi );
	char szBuf[255];
	m_bBL[anxi]=TRUE;
	m_bIsMoving[anxi] = TRUE;
	CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
	if( m_paramBl.nDir[anxi] < 0 )
		m_fBLCount[anxi] = m_fCurStep[anxi] - m_paramBl.fCount[anxi];
	else
		m_fBLCount[anxi] = m_fCurStep[anxi] + m_paramBl.fCount[anxi];
	theApp.m_StageMC6600.MoveDir(ucAxis, m_fBLCount[anxi], FALSE);
	if(!m_bEnable[anxi])
	{
		theApp.m_StageMC6600.Enable(anxi);
		m_bEnable[anxi] = TRUE;
	}
	Sleep(40);
	sprintf_s( szBuf, "%.4f", m_fGoal[anxi]/m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi] );
	GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText( szBuf );
	if( m_paramBl.nDir[anxi] < 0 )
		sprintf_s( szBuf, "Modified %c position display with - backlash direction", ucAxis );	// 带-间隙方向，修改X位置显示
	else
		sprintf_s( szBuf, "Modified %c position display with + backlash direction", ucAxis );	// 带+间隙方向，修改%c位置显示
	Message( szBuf );
	//关闭线程
	m_bIsMoving[anxi] = FALSE;
	CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
}

void CPanelStage::StopTrackAxis2( short anxi )
{
	unsigned char ucAxis = SerialNum( anxi );
	char szBuf[255];
	theApp.m_StageMC6600.MoveDir(ucAxis, m_fCurStep[anxi], FALSE);
	theApp.m_StageMC6600.Enable(anxi);
	m_bEnable[anxi] = TRUE;
	if( m_paramBl.nDir[anxi] > 0 )	// X轴非间隙方向
		sprintf_s( szBuf, "Axis %c non-backlash direction 1", ucAxis );
	else
		sprintf_s( szBuf, "Axis %c non-backlash direction 2", ucAxis );
	Message( szBuf );
}

void CPanelStage::StopTrack()
{
	int i = 0;
	for( i=0; i<2; i++ )
	{
		theApp.m_StageMC6600.close(i);
		m_bEnable[i] =FALSE;
	}
	RestoreSpeed();

	char szBuf[255];
	unsigned char ucAxis;
	for( i=0; i<2; i++ )
	{
		//GetCurStep(i);
		m_fGoal[i] = m_fCurStep[i];
		ucAxis = SerialNum( i );
		sprintf_s( szBuf, "Get the current position of axis %c", ucAxis );	// 获取当前%c轴位置
		Message( szBuf );
	}
	if(m_bBlTrack)
	{
		if( (!m_bXdir && m_paramDriveDir.nDir[0] > 0) || (m_bXdir && m_paramDriveDir.nDir[0] < 0))
		{
			m_bDirection[0] = FALSE;
			if(m_paramBl.nDir[0] < 0 && NotZero(m_paramBl.fCount[0]))
				StopTrackAxis1( 0 );
			else if(m_paramBl.nDir[0] > 0)
				StopTrackAxis2( 0 );
		}
		else
		{
			m_bDirection[0] = TRUE;
			if(m_paramBl.nDir[0] > 0 && NotZero(m_paramBl.fCount[0]))
				StopTrackAxis1( 0 );
			else if(m_paramBl.nDir[0] < 0)
				StopTrackAxis2( 0 );
		}
		
		if( (!m_bYdir && m_paramDriveDir.nDir[1] > 0) || (m_bYdir && m_paramDriveDir.nDir[1] < 0) )
		{
			m_bDirection[1] = FALSE;
			if(m_paramBl.nDir[1] < 0 && NotZero(m_paramBl.fCount[1]))
				StopTrackAxis1( 1 );
			else  if(m_paramBl.nDir[1] > 0)
				StopTrackAxis2( 1 );
		}
		else
		{
			m_bDirection[1] = TRUE;
			if(m_paramBl.nDir[1] > 0 && NotZero(m_paramBl.fCount[1]))
				StopTrackAxis1( 1 );
			else
				StopTrackAxis2( 1 );
		}
	}
	else
	{
//		Message("不带间隙，下发当前位置为目标值");
		Message("Without backlash, the current position is the target value");
		for( i=0; i<2; i++ )
		{
			ucAxis = SerialNum( i );
			theApp.m_StageMC6600.MoveDir(ucAxis, m_fCurStep[i], FALSE);
			theApp.m_StageMC6600.Enable(i);
			m_bEnable[i] = TRUE;
			Sleep(40);
		}
	}

//	Message("不带间隙，X/Y位置显示");
	Message("Without backlash, display position of X/Y");
	for( i=0; i<2; i++ )
	{
		sprintf_s( szBuf, "%.4f", m_fGoal[i]/m_dEquivalent[i] *m_paramDriveDir.nDir[i] );
		GetDlgItem(IDC_EDIT_X_UserDisp +i)->SetWindowText( szBuf );
	}
}

// 摇杆
void CPanelStage::SetTracklocusAxis1( short anxi, int nSpeed )
{
	// X/R/T
	nSpeed -= 512;
	nSpeed = nSpeed * m_paramDriveDir.nDir[anxi];
	m_bTrackState[anxi] = TRUE;
	unsigned char ucAxis = SerialNum( anxi );
	char szBuf[255];
	if( abs(nSpeed) < 5  )	// 与跟踪点之间差值较小，停止运动
	{
		m_bTrackState[anxi] = FALSE;
		sprintf_s( szBuf, "Offset of %c is too small, do not move", ucAxis );	// 移动量过小，不移动
		Message( szBuf );
	}
	else
	{
// 		m_StageMC6600.SetSpeed( ucAxis, m_paramsSpeed.fSpeedTrack[anxi] );
		m_bIsMoving[anxi] = TRUE;
		CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
		if(nSpeed > 0 && abs(nSpeed) > 5)
		{
			m_bDirection[anxi] = TRUE;
// 			m_StageMC6600.MoveDir(ucAxis, 50, FALSE);
			KillTimer( (anxi +1) *10 +2 );
			m_nTimer[anxi +5] = SetTimer( (anxi +1)*10 +1, nStageMC6600_Track, NULL );
		}
		if(nSpeed < 0 && abs(nSpeed) > 5)
		{
			m_bDirection[anxi] = FALSE;
// 			m_StageMC6600.MoveDir(ucAxis, -50, FALSE);
			KillTimer( (anxi +1) *10 +1 );
			m_nTimer[anxi +5] = SetTimer( (anxi +1)*10 +2, nStageMC6600_Track, NULL );
		}
		theApp.m_StageMC6600.SetSpeed( ucAxis, m_paramsSpeed.fSpeedTrack[anxi] );
	}
}

void CPanelStage::SetTracklocusAxis2( short anxi, int nSpeed )
{
	// Y/Z
	nSpeed -= 512;
	nSpeed = nSpeed * m_paramDriveDir.nDir[anxi];
	m_bTrackState[anxi] = TRUE;
	unsigned char ucAxis = SerialNum( anxi );
	char szBuf[255];
	if( abs(nSpeed) < 5  )	// 与跟踪点之间差值较小，停止运动
	{
		m_bTrackState[anxi] = FALSE;
		sprintf_s( szBuf, "Offset of %c is too small, do not move", ucAxis );	// 移动量过小，不移动
		Message( szBuf );
	}
	else
	{
// 		m_StageMC6600.SetSpeed( ucAxis, m_paramsSpeed.fSpeedTrack[anxi] );
		m_bIsMoving[anxi] = TRUE;
		CStageMC6600Manager::Instance().m_bIsMoving[anxi] = m_bIsMoving[anxi];
		if(nSpeed < 0 && abs(nSpeed) > 5)
		{
			m_bDirection[anxi] = TRUE;
// 			m_StageMC6600.MoveDir(ucAxis, 50, FALSE);
			KillTimer( (anxi +1) *10 +2 );
			m_nTimer[anxi +5] = SetTimer( (anxi +1)*10 +1, nStageMC6600_Track, NULL );
		}
		if(nSpeed > 0 && abs(nSpeed) > 5)
		{
			m_bDirection[anxi] = FALSE;
// 			m_StageMC6600.MoveDir(ucAxis, -50, FALSE);
			KillTimer( (anxi +1) *10 +1 );
			m_nTimer[anxi +5] = SetTimer( (anxi +1)*10 +2, nStageMC6600_Track, NULL );
		}
		theApp.m_StageMC6600.SetSpeed( ucAxis, m_paramsSpeed.fSpeedTrack[anxi] );
	}
}

void CPanelStage::SetTracklocus1( int nSpeedX, int nSpeedY, int nSpeedR )
{
	if( !m_bIsMC6600Ready )
		return;
	char szBuf[255];
	sprintf_s( szBuf, "SetTracklocus1: X=%d, Y=%d, R=%d", nSpeedX, nSpeedY, nSpeedR );
	Message( szBuf );
	// 如果Speed为0值，则该轴不动作
	// X向
	if( nSpeedX != 0 )
		SetTracklocusAxis1( 0, nSpeedX );
	// Y向
	if( nSpeedY != 0 )
		SetTracklocusAxis2( 1, nSpeedY );
	// R向
	if( nSpeedR != 0 )
		SetTracklocusAxis1( 3, nSpeedR );
} 

void CPanelStage::SetTracklocus2( int nSpeedT, int nSpeedZ )
{
	if( !m_bIsMC6600Ready )
		return;
	char szBuf[255];
	sprintf_s( szBuf, "SetTracklocus2: T=%d, Z=%d", nSpeedT, nSpeedZ );
	Message( szBuf );
	// 如果Speed为0值，则该轴不动作
	// T向
	if( nSpeedT != 0 )
		SetTracklocusAxis1( 4, nSpeedT );
	// Z向
	if( nSpeedZ != 0 )
		SetTracklocusAxis2( 2, nSpeedZ );
} 

void CPanelStage::SetTracklocusAxis( short anxi, BOOL bDirect )
{
	CString str;
	unsigned char ucAxis = SerialNum( anxi );
	if( m_bTrackState[anxi] )
	{
		//GetCurStep(anxi);
		//方案一
		if(anxi < 1)
			//不同放大倍数，移动步距不同
			CalculateStageUScale(0,m_sizeReso.cx * m_fRockerStep[0],m_fTrackStepAxis);
		else if(anxi < 2)
			//不同放大倍数，移动步距不同
			CalculateStageUScale(0,m_sizeReso.cy * m_fRockerStep[1],m_fTrackStepAxis);
		else
			m_fTrackStepAxis = m_fRockerStep[anxi] * m_dEquivalent[anxi];
		
		if(bDirect)
			m_fGoal[anxi] = (float)(m_fCurStep[anxi] + m_fTrackStepAxis);
		else
			m_fGoal[anxi] = (float)(m_fCurStep[anxi] -m_fTrackStepAxis);
		str.Format("anxi:%d, m_fCurStep:%f, m_fTrackStep:%f\n",anxi, m_fCurStep[anxi], m_fTrackStepAxis);
		Message(str);

		theApp.m_StageMC6600.MoveDir(ucAxis, m_fGoal[anxi], FALSE);
		//方案二
		/*if(bDirect)
			m_fGoal[anxi] = m_fGoal[anxi] + m_fTrackStep;
		else
			m_fGoal[anxi] = m_fGoal[anxi] - m_fTrackStep;
		unsigned char ucAxis = SerialNum( anxi );
		m_StageMC6600.MoveDir(ucAxis, m_fGoal[anxi], FALSE);*/
	}
	else
	{
		m_nTimer[anxi] = SetTimer( anxi +1, nStageMC6600_Track, NULL );
		//获取当前位置并下发
		m_fGoal[anxi] = m_fCurStep[anxi];
		theApp.m_StageMC6600.MoveDir(ucAxis, m_fGoal[anxi], FALSE);
		if( anxi < 2 )
			str.Format( "%.4f", m_fGoal[anxi]/m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi] );
		else
			str.Format( "%.2f", m_fGoal[anxi]/m_dEquivalent[anxi] *m_paramDriveDir.nDir[anxi] );
		GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText(str);
	}
}

void CPanelStage::RestoreSpeedlocus()
{
	unsigned char ucAxis;
	for(int i=0;i<5;i++)
	{
		ucAxis = SerialNum(i);
		theApp.m_StageMC6600.SetSpeed(ucAxis, m_paramsSpeed.fSpeedInit[i]);
	}
}

void CPanelStage::StopTracklocus()
{
	for(int i =0;i<5;i++)
		m_bTrackState[i] =FALSE;
	RestoreSpeedlocus();
}

void CPanelStage::OnBnClickedCheckAll()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bMotoronoffAll = !m_bMotoronoffAll;
	int i;
	if(m_bMotoronoffAll)
	{
		for(i=0;i<5;i++)
		{
			theApp.m_StageMC6600.close(i);
			m_bEnable[i] = FALSE;
			CStageMC6600Manager::Instance().m_bIsMoving[i] = TRUE;
			m_nTimer[i] = SetTimer(i+6,1000,NULL);
		}
		theApp.m_StageMC6600.LimRelease(0,'R');
	}
	else
	{
		for(i=0;i<5;i++)
		{
			KillTimer(i+6);
			m_nTimer[i] = 0;
			//theApp.m_StageMC6600.CurrPosR(i);
			unsigned short ucAxis = SerialNum(i);
			theApp.m_StageMC6600.Move(ucAxis,m_fCurStep[i],m_bRelative);
			Sleep(100);
			theApp.m_StageMC6600.Enable(i);
			m_bEnable[i] = TRUE;
			GetDlgItem(IDC_EDIT_X_CurStep +i)->Invalidate();	
			CString str;
			if( i < 3 )
				str.Format( "%.3f", m_fGoal[i]/m_dEquivalent[i] );
			else
				str.Format( "%.2f", m_fGoal[i]/m_dEquivalent[i] );

			GetDlgItem(IDC_EDIT_X_UserDisp +i)->SetWindowText(str);
		}
		theApp.m_StageMC6600.LimRelease(1,'R');
	}
}
