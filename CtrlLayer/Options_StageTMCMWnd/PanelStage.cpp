// PanelStage.cpp : implementation file
//

#include "stdafx.h"
#include "Options_StageTMCMWnd.h"
#include "PanelStage.h"
#include "DlgSetup.h"
#include "DlgInput.h"
#include <math.h>
#include <float.h>
#include "DlgLimitParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CLIP_RANGE(value, min, max)  ( (value) > (max) ? (max) : (((value) < (min)) ? (min) : (value)) )

const UINT	nStageTMCM_Elapse	= 800;
const UINT	nStageTMCM_Track	= 1000;
const long	lSpeedTrackMax		= 400;

HHOOK g_hook = NULL;

unsigned char SerialNum(short motor)
{
	unsigned char ucMotor = 'X';
	switch (motor)
	{
	case 0:
		ucMotor = 'X';
		break;
	case 1:
		ucMotor = 'Y';
		break;
    case 2:
		ucMotor = 'Z';
		break;
	case 3:
		ucMotor = 'R';
		break;
	case 4:
		ucMotor = 'T';
		break;
	}
	return ucMotor;
}

short NumSerial( unsigned char ucMotor )
{
	short motor = 0;
	switch( ucMotor )
	{
	case	'X':
		motor = 0;
		break;
	case	'Y':
		motor = 1;
		break;
	case	'Z':
		motor = 2;
		break;
	case	'R':
		motor = 3;
		break;
	case	'T':
		motor = 4;
		break;
	default:
		motor = 5;
		break;
	}
	return motor;
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
// 	ON_BN_CLICKED(IDC_CHECK_Z_WD, OnBnClickedCheckZWd)
 	ON_BN_CLICKED(IDC_BUTTON_Home, OnBnClickedButtonHome)
	ON_BN_CLICKED(IDC_BUTTON_Zero, OnBnClickedButtonZero)
	ON_BN_CLICKED(IDC_BUTTON_Stop, OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_MoveTo, OnBnClickedButtonMoveto)
	ON_BN_CLICKED(IDC_BUTTON_ListAdd, OnBnClickedButtonListadd)
	ON_BN_CLICKED(IDC_BUTTON_ListDel, OnBnClickedButtonListdel)
	ON_BN_CLICKED(IDC_BUTTON_ListChange, OnBnClickedButtonListchange)
	ON_BN_CLICKED(IDC_BUTTON_SetUserOrigin, OnBnClickedButtonSetuserorigin)
	ON_BN_CLICKED(IDC_BUTTON_Setup, OnBnClickedButtonSetup)
	ON_BN_CLICKED(IDC_BUTTON_LimitParam, OnBnClickedButtonLimitparam)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BUTTON_HomeX, IDC_BUTTON_HomeT, OnHomeChanged)
	ON_COMMAND_RANGE(IDC_BUTTON_ZeroX, IDC_BUTTON_ZeroT, OnZeroChanged)
	ON_COMMAND_RANGE(IDC_BUTTON_X_Move_Minus, IDC_BUTTON_T_Move_Minus, OnMoveMinusChanged)
	ON_COMMAND_RANGE(IDC_BUTTON_X_Move_Plus, IDC_BUTTON_T_Move_Plus, OnMovePlusChanged)
	ON_COMMAND_RANGE(IDC_BUTTON_X_Move_Minus2, IDC_BUTTON_T_Move_Minus2, OnMoveMinus2Changed)
	ON_COMMAND_RANGE(IDC_BUTTON_X_Move_Plus2, IDC_BUTTON_T_Move_Plus2, OnMovePlus2Changed)
	ON_MESSAGE(WM_USER_TMCM_UpdateValue, OnUpdateTMCMValue)
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
	m_bIsTMCMReady		= FALSE;
	m_nProcess			= 0;
	m_nMoveSteps		= -1;
	m_bBlClick			= FALSE;
	m_bBlTrack			= FALSE;
	m_bIsMove			= FALSE;
	m_bHighPrecCount	= FALSE;
	m_bXdir				= FALSE;
	m_bYdir				= FALSE;
	m_fSLength[0]		= 0.0;
	m_fSLength[1]		= 0.0;
	m_bSoftLimit		= TRUE;
	m_bMoveXY			= FALSE;
	int i=0; 
	for( i=0; i<5; i++ )
	{
		m_fCurStep[i]	= 0;
		m_bBL[i]		= FALSE;
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
		m_fMaxCurSpeed[i]		= 130;
		m_fMinCurSpeed[i]		= 20;
		m_fMaxStandbySpeed[i]	=10;
		m_fMinStandbySpeed[i]	=1;
		/*if(i<2)
			m_fMaxPospeed[i]		= 32000;
		else if(i<3)
			m_fMaxPospeed[i]		= 6400;
		else if(i<4)
			m_fMaxPospeed[i]		= 16000;
		else
			m_fMaxPospeed[i]		= 16000;
		
		m_fMinPospeed[i]		= 1600;
		m_fMaxAccspeed[i]		= 6400;
		m_fMinAccspeed[i]		= 1600;
		m_fMaxDecspeed[i]		= 6400;
		m_fMinDecspeed[i]		= 1600;*/
		m_fMaxTrackSpeed[i]		= 3200;
		m_fMinTrackSpeed[i]		= 1600;
		m_bTrackState[i]		= TRUE;
		m_bAxisInitia[i]		= TRUE;
		m_bCommunicationFault[i] = FALSE;
		m_bMotorFault[i]		= TRUE;
		m_fRockerStep[i]		= 0.1;
		m_nPhaseInit[i]			= -1;
	}
	m_bAxisInitia[3]	= FALSE;
//	m_fMaxTrackSpeed[4]	= 0.4f;
//	m_fMinTrackSpeed[4]	= 0.05f;
	//摇杆
	/*m_fTrackStep		= 1600;
	m_fTrackStepR		= 2400;
	m_fTrackStepT		= 800;*/
	m_fTrackStepAxis	= 160;

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
	for( i=0;i<10;i++)
	{
		m_fSLimit[i]		= 0;
		m_factualLimit[i]	= 0;
		m_nTimer[i]			= 0;
	}
	m_bDbClick		= TRUE;		// 双击
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

	m_dMag = 100.0;
//	GetDlgItem(IDC_Stage_Mag)->SetWindowText( "100" );

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

//	Connect();

	// 将对话框位置设置到客户区左上角
	GetWindowRect(m_rcWindow);
	GetDlgItem(IDC_EDIT_Report)->GetClientRect(m_rcReport);
	SetWindowPos(&CWnd::wndTop,
				8,		//GetSystemMetrics(SM_CXSCREEN) - rc.Width()-10,
				32*3,	//GetSystemMetrics(SM_CYSCREEN) /2 -rc.Height() /2,
				m_rcWindow.Width(),
				m_rcWindow.Height() -m_rcReport.Height() -20,	//rc.Height(),
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
	while( (pos=m_plistTMCMPos->GetHeadPosition()) )
	{
		structTMCMPos* pItem = m_plistTMCMPos->GetAt( pos );
		m_plistTMCMPos->RemoveHead();
		delete pItem;
	}
	delete m_plistTMCMPos;
	if( m_bIsTMCMReady )
	{
		for(i=0;i<5;i++)
		{
			m_bIsMoving[i] = FALSE;
			CStageTMCMManager::Instance().m_bIsMoving[i] = m_bIsMoving[i];
		}
		Sleep(100);
		theApp.m_StageTMCM.Close();
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
//	CDialog::OnOK();
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
	strcat_s(path, "\\Stage\\StageTMCM.ini");
	// 写入配置文件
	WritePrivateProfileString( "StageTMCMCfg", "Language", szBuf, path );
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
	strcat_s(path, "\\Stage\\StageTMCM.ini");
	char szLang[20];
	if(GetPrivateProfileString("StageTMCMCfg", "Language", "", szLang, 20, path) != 0)
	{
		exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		if( strcmp( szLang, "zh_CN" ) == 0)
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
		if(GetPrivateProfileString("Label", "DriIinit", "", szBuf, nLen, path) != 0)
			GetDlgItem(IDC_BUTTON_LimitParam)->SetWindowText( szBuf );

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
		if(GetPrivateProfileString("Msg", "MsgSetMechOri", "", szBuf, nLen, path) != 0)
			m_strMsg[4] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgExtreSafe", "", szBuf, nLen, path) != 0)
			m_strMsg[5] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgFindIndex+", "", szBuf, nLen, path) != 0)
			m_strMsg[6] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgFindIndex-", "", szBuf, nLen, path) != 0)
			m_strMsg[7] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgGndSafe", "", szBuf, nLen, path) != 0)
			m_strMsg[8] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgLimitAbnormal", "", szBuf, nLen, path) != 0)
			m_strMsg[9] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgLimitAbnormal+", "", szBuf, nLen, path) != 0)
			m_strMsg[10] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgLimitAbnormal-", "", szBuf, nLen, path) != 0)
			m_strMsg[11] = szBuf;
		if(GetPrivateProfileString("Msg", "MsgWarning ", "", szBuf, nLen, path) != 0)
			m_strMsg[12] = szBuf;
		if(GetPrivateProfileString("Msg", "ZHeightWarning ", "", szBuf, nLen, path) != 0)
			m_strMsg[13] = szBuf;
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
	ZeroMemory(&m_paramSpeed,sizeof(TMCM_Params_Speed));
	ZeroMemory(&m_paramCurrent,sizeof(TMCM_Params_Current));
	// 获取工作路径
	char path[256], szBufRootKey[20];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\Stage\\StageTMCM.ini");
	char szBuf[255];
	// 通讯端口
	m_nPort = 1;
	if(GetPrivateProfileString("StageTMCMCfg", "Port", "", szBuf, 15, path) != 0)
		m_nPort	= atoi( szBuf );

	int i, nCount = 5;
	float* pfData = new float[nCount *2];
	int* pnData = new int[nCount];

	sprintf_s(szBufRootKey, "StageTMCMCfg");
	//基础参数设置
	if( ReadInOneLine(path, szBufRootKey, "AxisCurSpeed", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		float fCurSpeed[5]={0};
		for( i=0; i<nCount; i++ )
		{
			fCurSpeed[i] = pfData[i];
			fCurSpeed[i] = CLIP_RANGE(fCurSpeed[i],m_fMinCurSpeed[i],m_fMaxCurSpeed[i]);
			m_paramAxis.fAxisCurSpeed[i] = fCurSpeed[i];
		}
	}
	if( ReadInOneLine(path, szBufRootKey, "AxisStandbySpeed", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		float fStandSpeed[5]={0};
		for( i=0; i<nCount; i++ )
		{
			fStandSpeed[i] = pfData[i];
			fStandSpeed[i] = CLIP_RANGE(fStandSpeed[i],m_fMinStandbySpeed[i],m_fMaxStandbySpeed[i]);
			m_paramAxis.fAxisStandbySpeed[i] = fStandSpeed[i];
		}
	}
	if( ReadInOneLine(path, szBufRootKey, "MaxSpeedPos", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramSpeed.fMaxSpeedPos[i] = pfData[i];
	}
	if( ReadInOneLine(path, szBufRootKey, "MaxSpeedAcc", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramSpeed.fMaxSpeedAcc[i] = pfData[i];
	}
		if( ReadInOneLine(path, szBufRootKey, "MaxSpeedDec", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramSpeed.fMaxSpeedDec[i] = pfData[i];
	}
	if( ReadInOneLine(path, szBufRootKey, "MinSpeedPos", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramSpeed.fMinSpeedPos[i] = pfData[i];
	}
	if( ReadInOneLine(path, szBufRootKey, "MinSpeedAcc", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramSpeed.fMinSpeedAcc[i] = pfData[i];
	}
		if( ReadInOneLine(path, szBufRootKey, "MinSpeedDec", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramSpeed.fMinSpeedDec[i] = pfData[i];
	}
	if( ReadInOneLine(path, szBufRootKey, "SpeedPos", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		float fPosSpeed[5];
		for( i=0; i<nCount; i++ )
		{
			fPosSpeed[i] = pfData[i];
			fPosSpeed[i] = CLIP_RANGE(fPosSpeed[i],m_paramSpeed.fMinSpeedPos[i],m_paramSpeed.fMaxSpeedPos[i]);
			m_paramSpeed.fSpeedPos[i] = fPosSpeed[i];
		}
	}
	if( ReadInOneLine(path, szBufRootKey, "SpeedAcc", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		float fAccSpeed[5];
		for( i=0; i<nCount; i++ )
		{
			fAccSpeed[i] = pfData[i];
			fAccSpeed[i] = CLIP_RANGE(fAccSpeed[i],m_paramSpeed.fMinSpeedAcc[i],m_paramSpeed.fMaxSpeedAcc[i]);
			m_paramSpeed.fSpeedAcc[i] = fAccSpeed[i];
		}
	}
	if( ReadInOneLine(path, szBufRootKey, "SpeedDec", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		float fDecSpeed[5];
		for( i=0; i<nCount; i++ )
		{
			fDecSpeed[i] = pfData[i];
			fDecSpeed[i] = CLIP_RANGE(fDecSpeed[i],m_paramSpeed.fMinSpeedDec[i],m_paramSpeed.fMaxSpeedDec[i]);
			m_paramSpeed.fSpeedDec[i] = fDecSpeed[i];
		}
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
	//调试用
	//电机故障用
	if( ReadInOneLine(path, szBufRootKey, "MotorFault", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_bMotorFault[i] = pnData[i];
	}

	//摇杆步距
	if(ReadInOneLine(path, szBufRootKey, "RockerStep", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_fRockerStep[i] = pfData[i];
	}

	//Track转速
	if( ReadInOneLine(path, szBufRootKey, "TrackPospeed", FLOATSTYLE, nCount, pfData, ',' ) == nCount )
	{
		float fTrackspeed[5];
		for( i=0; i<nCount; i++ )
		{
			fTrackspeed[i] = pfData[i];
			fTrackspeed[i] = CLIP_RANGE(fTrackspeed[i],m_fMinTrackSpeed[i],m_fMaxTrackSpeed[i]);
			m_paramSpeed.fSpeedTrack[i] = fTrackspeed[i];
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

	//限位设置
	if( ReadInOneLine(path, szBufRootKey, "LeftLimit", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramLimitParam.nLeftLimit[i] = pnData[i];
	}
	if( ReadInOneLine(path, szBufRootKey, "RightLimit", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramLimitParam.nRightLimit[i] = pnData[i];
	}
	if( ReadInOneLine(path, szBufRootKey, "ExchangeLimit", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramLimitParam.nExchangeLimit[i] = pnData[i];
	}
	if( ReadInOneLine(path, szBufRootKey, "LeftLimitLevel", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramLimitParam.nLeftLevelReversal[i] = (short)pnData[i];
	}
	if( ReadInOneLine(path, szBufRootKey, "RightLimitLevel", INTSTYLE, nCount, pnData, ',' ) == nCount )
	{
		for( i=0; i<nCount; i++ )
			m_paramLimitParam.nRightLevelReversal[i] = (short)pnData[i];
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
	delete [] pnData;

	//是否有软限位
	if(GetPrivateProfileString(szBufRootKey, "SLimit", "", szBuf, 255, path) != 0)
		m_bSoftLimit = (BOOL)(atoi(szBuf));
	if(GetPrivateProfileString(szBufRootKey, "Z0wd", "", szBuf, 255, path) != 0)
		m_paramZRes.fZ0wd = (float)atof(szBuf);
	if(GetPrivateProfileString(szBufRootKey, "RTo", "", szBuf, 255, path) != 0)
		m_paramZRes.fTo = (float)atof(szBuf);
	if(GetPrivateProfileString(szBufRootKey, "RInclina", "", szBuf, 255, path) != 0)
		m_paramZRes.fInclina = (float)atof(szBuf);
	// 放气前的选项
	if(GetPrivateProfileString(szBufRootKey, "VentSel", "", szBuf, 255, path) != 0)
		m_nVentSel = atoi(szBuf);

	// 第几版控制器
	m_paramStyle.nVersion = 1;
	if(GetPrivateProfileString(szBufRootKey, "Version", "", szBuf, 255, path) != 0)
		m_paramStyle.nVersion = atoi(szBuf);

	//样品台型号
	m_paramStyle.nType = 0;
	if(GetPrivateProfileString(szBufRootKey, "Type", "", szBuf, 15, path) != 0)
		m_paramStyle.nType = atoi( szBuf );
	if(m_bSoftLimit)
	{
		for(int i=0;i<5;i++)
		{
			m_paramZRes.fLimitNeg[i] = m_paramZRes.fLimitNeg[i] - m_fCali[i];
			m_paramZRes.fLimitPos[i] = m_paramZRes.fLimitPos[i] - m_fCali[i];
			m_factualLimit[i*2] = m_factualLimitH[i*2] - m_fCali[i];
			m_factualLimit[i*2+1] = m_factualLimitH[i*2+1] - m_fCali[i];
		}
	}

	GetDlgItem(IDC_BUTTON_R_Move_Minus2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_R_Move_Plus2)->ShowWindow(SW_HIDE);

	theApp.m_StageTMCM.InitHWND( this->m_hWnd );

	Localize();
	InitParam_SysParams();
	InitParam_Backlash( path );
	InitParam_ZRestraint( path );
	InitParam_Calibration( path );
	theApp.m_StageTMCM.Pospeed(m_paramSpeed.fSpeedPos,m_paramSpeed.fSpeedAcc,m_paramSpeed.fSpeedDec);
	theApp.m_StageTMCM.LimitParam(m_paramLimitParam.nLeftLimit,m_paramLimitParam.nRightLimit,m_paramLimitParam.nExchangeLimit);
	theApp.m_StageTMCM.LimitLevelParam(m_paramLimitParam.nLeftLevelReversal,m_paramLimitParam.nRightLevelReversal);
	theApp.m_StageTMCM.Current(m_paramAxis.fAxisCurSpeed,m_paramAxis.fAxisStandbySpeed);
	theApp.m_StageTMCM.SetVersion(m_paramStyle.nVersion);
	theApp.m_StageTMCM.Open(m_nPort);
}

void CPanelStage::InitParam_SysParams()
{
	int i;
	for(i=0;i<5;i++)
	{
		if(i<3)
			m_paramSys.fPitch[i] = 2;
		else if(i < 4)
		{
			if(m_paramStyle.nType > 0)
				m_paramSys.fPitch[i] = 4;   	//大台子
			else
				m_paramSys.fPitch[i] = 7.2;		//小台子
		}
		else
		{
			if(m_paramStyle.nType > 0)
				m_paramSys.fPitch[i] = 4;   	//大台子
			else
				m_paramSys.fPitch[i] = 360./46;	//小台子
		}

		m_paramSys.fRat[i] = 90;
		m_paramSys.fStepAngle[i] = 1.8f;
		m_paramSys.nAxisSubDivide[i] = 32;
		m_paramSys.fStepsRev[i] = 360/m_paramSys.fStepAngle[i]*m_paramSys.nAxisSubDivide[i];
	}

	for(i=0;i<5;i++)
		m_dEquivalent[i]	= m_paramSys.fStepsRev[i]/ m_paramSys.fPitch[i];   //每mm/deg对应的脉冲数
}

void CPanelStage::InitParam_ZRestraint( char* path )
{
	char szBuf[255], szBufRootKey[20];
	sprintf_s(szBufRootKey, "StageTMCMZres");

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
	ZeroMemory(&m_paramBl, sizeof(TMCM_Params_Bl));
	char szBuf[255], szBufRootKey[20];
	sprintf_s(szBufRootKey, "StageTMCMCfg");

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
		theApp.m_StageTMCM.SetBacklashParams( i, m_paramBl.nDir[i], m_paramBl.fCount[i] );
}

void CPanelStage::InitParam_Calibration( char* path )
{
	char szBuf[255], szBufRootKey[20];
	sprintf_s(szBufRootKey, "StageTMCMCfg");
	//考虑到T轴旋转，默认值为6
	m_fCalibrationZ = 6;
	if(GetPrivateProfileString(szBufRootKey, "CalibrationZ0 ", "", szBuf, 15, path) != 0)
		m_fCalibrationZ	= (float)atof( szBuf );
	/*if(m_fCalibrationZ < 6)
		m_fCalibrationZ = 6;
	m_fZHeight = m_fCalibrationZ;*/
	m_fCalibrationR =5;
	if(GetPrivateProfileString(szBufRootKey, "CalibrationR0 ", "", szBuf, 15, path) != 0)
		m_fCalibrationR	= (float)atof( szBuf );
	m_fCalibrationT =6;
	if(GetPrivateProfileString(szBufRootKey, "CalibrationT0 ", "", szBuf, 15, path) != 0)
		m_fCalibrationT	= (float)atof( szBuf );
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

	// 设置StageTMCM日志文件
	str.Format( "%s\\Log\\StageTMCM-%s.log", path, strTemp );
	if( m_hFileLog != NULL )
		CloseHandle( m_hFileLog );
	m_hFileLog	= NULL;
	m_hFileLog = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	m_nLoopsLog = 1;
	// 写StageTMCM日志文件头
	if( m_hFileLog != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		str.Format( "\t%s\r\n", "Log StageTMCM" );
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
	GetDlgItem(IDC_BUTTON_LimitParam)->EnableWindow(TRUE);
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

	CString csMessage = CString(lpszMessage) +"\r\n";
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

	sprintf_s(Buf, "%s\r\n", str );
	WriteFile(m_hFileLog, Buf, strlen(Buf), &cbRet, NULL);

	m_nLoopsLog++;
	if( m_nLoopsLog > 50000 )
	{
		CloseHandle( m_hFileLog );
		InitLog();
	}
}

LRESULT CPanelStage::OnUpdateTMCMValue(WPARAM wParam, LPARAM lParam)
{
	CString str;
	switch( wParam )
	{
	case	0:
		m_bIsTMCMReady = (BOOL)lParam;
		if(m_bIsTMCMReady)
		{
			SetStatus(TRUE);
			InitLog();
			str.Format("Open TMCM COM %d successfully!", m_nPort);
		}
		else
		{   
			SetStatus(FALSE);
			m_bIsTMCMReady = FALSE;
			str.Format("Open TMCM COM %d Failed!",m_nPort);
			::PostMessage( GetParent()->m_hWnd, WM_USER_TMCM_UpdateValue, 0, 0 );
		}
		Message(str);
		break;

	case	1:
	case	2:
	case	3:
	case	4:
	case	5:
		{
			short motor = (short)wParam -1;
			GetCurStep(motor);
			if( motor < 3)
				str.Format( "%.3f", m_fCurStep[motor]/m_dEquivalent[motor] );
			else
				str.Format( "%.2f", m_fCurStep[motor]/m_dEquivalent[motor] );
			GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText(str);
			GetDlgItem(IDC_EDIT_X_CurStep +motor)->Invalidate();
		}
		break;
	case  6:
		// 读取到Params_Pos
		m_paramCurrent = *((TMCM_Params_Current*)lParam);
		SyncParamsPos();
		FindIndex();
		break;
	case 7:
		m_paramFault = *((TMCM_Params_Fault*)lParam);
		for(int i=0;i<5;i++)
			m_bCommunicationFault[i] = m_paramFault.bCommunicationFault[i];
		SaveToINI(1);
		break;
	case 11:
		m_paramCurrent = *((TMCM_Params_Current*)lParam);
		SyncParamsPos();
		break;
	case 12:
		m_paramLimitStatus = *((TMCM_Params_LimitStatus*)lParam);
		break;
	}
	return 0;
}

void CPanelStage::SyncParamsPos()
{
	CString str;
	//str.Format("X pos = %.3f mm，nl = %d,nR=%d", m_paramCurrent.fSteps[0] /m_dEquivalent[0] * m_paramDriveDir.nDir[0],m_paramLimitStatus.nL[0],m_paramLimitStatus.nR[0]);
	//Message(str);
	//str.Format("Y pos = %.3f mm，nl = %d,nR=%d", m_paramCurrent.fSteps[1] /m_dEquivalent[1] * m_paramDriveDir.nDir[1],m_paramLimitStatus.nL[1],m_paramLimitStatus.nR[1]);
	//Message(str);
	//str.Format("Z pos = %.3f mm，nl = %d,nR=%d", m_paramCurrent.fSteps[2] /m_dEquivalent[2]  * m_paramDriveDir.nDir[2],m_paramLimitStatus.nL[2],m_paramLimitStatus.nR[2]);
	//Message(str);
	//str.Format("R pos = %.1f deg，nl = %d,nR=%d", m_paramCurrent.fSteps[3] /m_dEquivalent[3]  * m_paramDriveDir.nDir[3],m_paramLimitStatus.nL[3],m_paramLimitStatus.nR[3]);
	//Message(str);
	//str.Format("T pos = %.1f deg，nl = %d,nR=%d\r\n", m_paramCurrent.fSteps[4] /m_dEquivalent[4]  * m_paramDriveDir.nDir[4],m_paramLimitStatus.nL[4],m_paramLimitStatus.nR[4]);
	//Message(str);
		// 运动时，或者某轴手动模式下，需要及时更新位置值
	char szBuf[255] = {0};
	unsigned short ucAxis;
	int i=0;
	for(i=0; i<5; i++ )
	{
		m_fCurStep[i] = m_paramCurrent.fSteps[i];
	}
	for(i=0; i<5; i++ )
	{
		if( m_bIsMoving[i] )
		{
			if(m_bHighPrecCount)
			{
				if( i<3 )
					sprintf_s( szBuf, "%.3f, %.5f", m_paramCurrent.fSteps[i] /m_dEquivalent[i] * m_paramDriveDir.nDir[i],m_paramCurrent.fSteps[i] /m_dEquivalent[i] * m_paramDriveDir.nDir[i]);
				else
					sprintf_s( szBuf, "%.2f, %.5f", m_paramCurrent.fSteps[i] /m_dEquivalent[i] * m_paramDriveDir.nDir[i],m_paramCurrent.fSteps[i] /m_dEquivalent[i] * m_paramDriveDir.nDir[i]);
			}
			else
			{
				if( i<3 )
					sprintf_s( szBuf, "%.3f", m_paramCurrent.fSteps[i] /m_dEquivalent[i] * m_paramDriveDir.nDir[i]);
				else
					sprintf_s( szBuf, "%.2f", m_paramCurrent.fSteps[i] /m_dEquivalent[i] * m_paramDriveDir.nDir[i]);
			}
			
			GetDlgItem( IDC_EDIT_X_CurStep +i )->SetWindowText( szBuf );
			ucAxis = SerialNum(i);
			/*str.Format("%c pos = %.3f mm", ucAxis,m_paramCurrent.fSteps[i] /m_dEquivalent[i] * m_paramDriveDir.nDir[i]);
			Message(str);*/
		}
	}
}

void CPanelStage::MoveMinusPlus( int nIndex )
{
	// XY轴单步走
	char szBuf[255];
	// 像素距离
	int nLength;
	short motor = 0;
	switch( nIndex )
	{
	case	0:	// X Minus
	case	1:	// X Plus
		nLength = m_sizeReso.cx /4;
		motor = 0;
		break;
	case	2:	// Y Minus
	case	3:	// Y Plus
		nLength = m_sizeReso.cy /4;
		motor = 1;
		break;
	case	4:	// Z Minus
	case	5:	// Z Plus
		motor = 2;
		break;
	case	6:	// R Minus
	case	7:	// R Plus
		motor = 3;
		break;
	case	8:	// T Minus
	case	9:	// T Plus
		motor = 4;
		break;
	}
	unsigned char ucMotor = SerialNum( motor );
	//if(!m_bEnable[motor])
		//theApp.m_StageTMCM.Enable(motor);
	// 实际pps
	float fMoveStep, fScale = 0;
	if( nIndex < 4 )	// X & Y
		CalculateStageUScale( 0, nLength, fScale );
	switch( nIndex )
	{
	case	0:	// X Minus
	case	2:	// Y Minus
		sprintf_s( szBuf, "%c move negative %.3f mm", ucMotor, fScale/m_dEquivalent[motor] );
		m_bDirection[motor] = FALSE;
		break;
	case	1:	// X Plus
	case	3:	// Y Plus
		sprintf_s( szBuf, "%c move positive %.3f mm", ucMotor, fScale/m_dEquivalent[motor] );
		m_bDirection[motor] = TRUE;
		break;
	case	4:	// Z Minus
	case	6:	// R Minus
	case	8:	// T Minus
		sprintf_s( szBuf, "%c move negative %.3f mm", ucMotor, 1600/m_dEquivalent[motor] );
		m_bDirection[motor] = FALSE;
		break;
	case	5:	// Z Plus
	case	7:	// R Plus
	case	9:	// T Plus
		sprintf_s( szBuf, "%c move positive %.3f mm", ucMotor, 1600/m_dEquivalent[motor] );
		m_bDirection[motor] = TRUE;
		break;
	}
	Message(szBuf);
	if(m_paramDriveDir.nDir[motor]<0)
	{
		if(m_bDirection[motor])
			m_bDirection[motor] = FALSE;
		else
			m_bDirection[motor] = TRUE;
	}
	switch( nIndex )
	{
	case	1:	// X Plus
	case	2:	// Y Minus
		fMoveStep = fScale*m_paramDriveDir.nDir[motor];
		break;
	case	0:	// X Minus
	case	3:	// Y Plus
		fMoveStep = (0 -fScale)*m_paramDriveDir.nDir[motor];
		break;
	case	4:	// Z Minus
	case	6:	// R Minus
	case	8:	// T Minus
		fMoveStep = -1600.0*m_paramDriveDir.nDir[motor];
		break;
	case	5:	// Z Plus
	case	7:	// R Plus
	case	9:	// T Plus
		fMoveStep = 1600.0*m_paramDriveDir.nDir[motor];
		break;
	}
	if(fMoveStep >0)
	{
		if(m_paramBl.nDir[motor] > 0 && NotZero(m_paramBl.fCount[motor]) )
			m_bBL[motor]=TRUE;
	}
	else
	{
		if(m_paramBl.nDir[motor] < 0 && NotZero(m_paramBl.fCount[motor]) )
			m_bBL[motor]=TRUE;
	}

	fMoveStep += m_fCurStep[motor];
	/*if(motor ==3)
	{
		fMoveStep = RDriveConversion(m_fCurStep[motor],fMoveStep);
	}*/
	m_fGoal[motor] = fMoveStep;
	if( nIndex < 6 )	// XYZ
		sprintf_s( szBuf, "%.3f", fMoveStep/m_dEquivalent[motor]*m_paramDriveDir.nDir[motor]);
	else
		sprintf_s( szBuf, "%.2f", fMoveStep/m_dEquivalent[motor]*m_paramDriveDir.nDir[motor]);
	GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText(szBuf);
	m_bIsMoving[motor] = TRUE;
	CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
	//电机上电
	switch(motor)
	{
	case 3:
		theApp.m_StageTMCM.PowerOfforOn(0,3);
		break;
	}
	m_fBLCount[motor] = theApp.m_StageTMCM.MoveDir(motor, fMoveStep);
// 	theApp.m_StageTMCM.Move( ucMotor, fMoveStep, m_bRelative);
// 	m_bIsMoving = FALSE;
// 	theApp.m_StageTMCM.SendCmd(cmd , ucMotor);
	m_nTimer[motor] = SetTimer(motor +1, nStageTMCM_Elapse, NULL);
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
	short motor;
	switch( nIndex )
	{
	case	0:	// X Minus 2
	case	1:	// X Plus 2
		motor = 0;
		break;
	case	2:	// Y Minus 2
	case	3:	// Y Plus 2
		motor = 1;
		break;
	case	4:	// Z Minus 2
	case	5:	// Z Plus 2
		motor = 2;
		break;
	case	6:	// R Minus 2
	case	7:	// R Plus 2
		motor = 3;
		break;
	case	8:	// T Minus 2
	case	9:	// T Plus 2
		motor = 4;
		break;
	}
	unsigned char ucMotor = SerialNum( motor );
	char szBuf[255];
	switch( nIndex )
	{
	case	0:	// X Minus
	case	2:	// Y Minus
	case	4:	// Z Minus
	case	6:	// R Minus
	case	8:	// T Minus
		if(m_paramDriveDir.nDir[motor] >0)
			m_bDirection[motor] = FALSE ;
		else
			m_bDirection[motor] = TRUE ;
		sprintf_s( szBuf, "%c continues to move in a negative dir", ucMotor );
		break;
	case	1:	// X Plus
	case	3:	// Y Plus
	case	5:	// Z Plus
	case	7:	// R Plus
	case	9:	// T Plus
		if(m_paramDriveDir.nDir[motor] >0)
			m_bDirection[motor] = TRUE ;
		else
			m_bDirection[motor] = FALSE ;
		sprintf_s( szBuf, "%c continues to move in a positive dir", ucMotor );
		break;
	}
	Message( szBuf );
	m_bIsMoving[motor] = TRUE;
	CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
	m_fGoal[motor] = 500000;
	if( nIndex < 8 )
	{
		// XYZR 轴
		if( m_bDirection[motor] )
			m_fGoal[motor] = 500000;
		else
			m_fGoal[motor] = -500000;
		theApp.m_StageTMCM.MoveLimit(motor, m_fGoal[motor] );
	}
	else
	{
		// T 轴
		if( m_bDirection[motor] )	
			m_fGoal[motor] = 150000;
		else
			m_fGoal[motor] = -150000;
		theApp.m_StageTMCM.MoveLimit(motor, m_fGoal[motor]);
	}

// 	m_fGoal[motor] = -28;
// 	CString str;
// 	str.Format( " %f", m_fGoal[motor]/m_dEquivalent[motor] );
// 	GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText(str);
	m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Elapse, NULL );
}

void CPanelStage::OnMoveMinus2Changed(UINT nID)
{
	MoveContinues( (nID -IDC_BUTTON_X_Move_Minus2) *2 );
}

void CPanelStage::OnMovePlus2Changed(UINT nID)
{
	MoveContinues( (nID -IDC_BUTTON_X_Move_Plus2) *2 +1 );
}

void CPanelStage::HomeAxis( short motor )
{
	unsigned char ucMotor = SerialNum( motor );
	char szBuf[255];
	sprintf_s( szBuf, "GoHome %c", ucMotor );
	Message( szBuf );
	m_bIsMoving[motor]	= TRUE;
	CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
	m_bToHome[motor]	= TRUE;
	//GetCurStep(motor);

	float fSteps = 0.0f;
	switch( motor )
	{
	case	0:	// X
	case	1:	// Y
		m_nPhaseInit[motor] = 0;
		fSteps = 500000.0*m_paramDriveDir.nDir[motor];
		break;
	case	2:	// Z
		m_nPhaseInit[motor] = 1;
		fSteps = -500000.0*m_paramDriveDir.nDir[motor];
		break;
	case	3:	// R
		m_nPhaseInit[motor] = 1;
		fSteps = 500000.0*m_paramDriveDir.nDir[motor];
		break;
	case	4:	// T
		m_nPhaseInit[motor] = 1;
		fSteps = -150000.0*m_paramDriveDir.nDir[motor];
		break;
	}
	theApp.m_StageTMCM.GoHome(motor, fSteps);
	sprintf_s( szBuf, "GoHome %c, fSteps %.5f", ucMotor,fSteps*m_paramDriveDir.nDir[motor]);
	Message( szBuf );
	m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Elapse, NULL );
}

void CPanelStage::OnHomeChanged(UINT nID)
{
	HomeAxis( nID -IDC_BUTTON_HomeX );
}

void CPanelStage::ZeroAxis( short motor )
{
	unsigned char ucMotor = SerialNum( motor );
	char szBuf[255];
	sprintf_s( szBuf, "GoOrigion %c", ucMotor );
	Message( szBuf );
	m_bOrigion[motor]	= TRUE;
	m_bToHome[motor]	= TRUE;
	m_bIsMoving[motor]	= TRUE;
	CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
	//GetCurStep(motor);
//	theApp.m_StageTMCM.LimRelease(1, ucMotor);

	float fSteps = 0.0f;
	switch( motor )
	{
	case	0:	// X
		fSteps = 500000.0*m_paramDriveDir.nDir[motor];
		m_nPhaseInit[motor] = 0;
		break;
	case	1:	// Y
		fSteps = 500000.0*m_paramDriveDir.nDir[motor];
		m_nPhaseInit[motor] = 0;
		break;
	case	2:	// Z
		fSteps = -500000.0*m_paramDriveDir.nDir[motor];
		m_nPhaseInit[motor] = 1;
		break;
	case	3:	// R
		fSteps = -500000.0*m_paramDriveDir.nDir[motor];
		m_nPhaseInit[motor] = 1;
		break;
	case	4:	// T
		fSteps = -150000.0*m_paramDriveDir.nDir[motor];
		m_nPhaseInit[motor] = 1;
		break;
	}
	theApp.m_StageTMCM.GoOrigion(motor, fSteps);
	m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Elapse, NULL );
}

void CPanelStage::OnZeroChanged(UINT nID)
{
	ZeroAxis( nID -IDC_BUTTON_ZeroX );
}

void CPanelStage::MoveAxis_FromThird( short anxi, float fStep )
{
	MoveAxis( anxi, fStep *m_paramDriveDir.nDir[anxi] );
	char szBuf[255];
	unsigned char ucAxis = SerialNum( anxi );
	sprintf_s( szBuf, "Move %c Axis from third: %.3f", fStep *m_paramDriveDir.nDir[anxi] );
	Message( szBuf );
}

void CPanelStage::MoveAxis( short motor, float fStep )
{
	unsigned char ucMotor = SerialNum( motor );
	CString str;
	str.Format( "Move %c", ucMotor );
	Message( str );

	m_fGoal[motor] = fStep;
	if( motor < 3 )
		str.Format( "%.3f", fStep/m_dEquivalent[motor]*m_paramDriveDir.nDir[motor] );
	/*else if(motor <4)
	{
		if(m_fGoal[motor]/m_dEquivalent[motor] > 179)
			str.Format( "%.2f", (m_fGoal[motor]/m_dEquivalent[motor]- 360)*m_paramDriveDir.nDir[motor] );
		else if(m_fGoal[motor]/m_dEquivalent[motor] < -179)
			str.Format( "%.2f", (m_fGoal[motor]/m_dEquivalent[motor]+ 360)*m_paramDriveDir.nDir[motor] );
		else if(fStep/m_dEquivalent[motor] > 359)
			str.Format( "%d", int(fStep/m_dEquivalent[motor])% 360 *m_paramDriveDir.nDir[motor] );
		else if(fStep/m_dEquivalent[motor] < -359)
			str.Format( "%d", int(fStep/m_dEquivalent[motor])% (-360) *m_paramDriveDir.nDir[motor] );
		else
			str.Format( "%.2f", fStep/m_dEquivalent[motor] *m_paramDriveDir.nDir[motor]);
	}*/
	else
		str.Format( "%.2f", fStep/m_dEquivalent[motor]*m_paramDriveDir.nDir[motor] );
	GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText(str);
	m_bIsMoving[motor] = TRUE;
	CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
	//R轴上电
	theApp.m_StageTMCM.PowerOfforOn(0,3);
	if( motor < 2 )	// XY 轴
		m_fSLength[motor] = fStep;
	if(m_bSoftLimit)
	{
		m_fSLimit[motor *2] = (float)(m_factualLimit[motor *2]* m_dEquivalent[motor]); //0，2，4，6，8为负向极限
		m_fSLimit[motor *2 +1] = (float)(m_factualLimit[motor *2 +1]* m_dEquivalent[motor]);
		if(fStep*m_paramDriveDir.nDir[motor] < m_fSLimit[motor *2] )
		{
			theApp.m_StageTMCM.MoveLimit(motor, m_fSLimit[motor *2]*m_paramDriveDir.nDir[motor]);
			m_fGoal[motor] = m_fSLimit[motor *2]*m_paramDriveDir.nDir[motor];
			m_bDirection[motor] = FALSE;
		}
		else if( fStep*m_paramDriveDir.nDir[motor] > m_fSLimit[motor *2 +1])
		{
			theApp.m_StageTMCM.MoveLimit(motor, m_fSLimit[motor *2 +1]*m_paramDriveDir.nDir[motor]);
			m_fGoal[motor] = m_fSLimit[motor *2 +1]*m_paramDriveDir.nDir[motor];
			m_bDirection[motor] = TRUE;
		}
		else
		{
			if(motor < 2 && (fabs(m_fSLength[motor] - m_fCurStep[motor]) <= m_paramBl.fCount[motor]/m_paramBl.nCoeff[motor]) )
			{
				// XY轴
				if( m_fSLength[motor] < m_fCurStep[motor] )
				{
					m_fSLength[motor] = m_fSLength[motor] +m_paramBl.fCount[motor];
					theApp.m_StageTMCM.MoveLimit(motor, m_fSLength[motor]);
					Sleep(100);
					//GetCurStep(motor);
				}
				else if( m_fSLength[motor] > m_fCurStep[motor] )
				{
					m_fSLength[motor] = m_fSLength[motor] -m_paramBl.fCount[motor];
					theApp.m_StageTMCM.MoveLimit(motor, m_fSLength[motor]);
					Sleep(100);
					//GetCurStep(motor);
				}
				if(fStep - m_fSLength[motor] > 0)
				{
					m_bDirection[motor] = TRUE;
					if(m_paramBl.nDir[motor] > 0 && NotZero(m_paramBl.fCount[motor]))
						m_bBL[motor] = TRUE;
				}
				else if(fStep - m_fSLength[motor] < 0)
				{
					m_bDirection[motor] = FALSE;
					if(m_paramBl.nDir[motor] < 0 && NotZero(m_paramBl.fCount[motor]))
						m_bBL[motor] = TRUE;
				}
				else
					return;
			}
			else
			{
				if(fStep - m_fCurStep[motor] > 0)
				{
					m_bDirection[motor] = TRUE;
					if(m_paramBl.nDir[motor] > 0 && NotZero(m_paramBl.fCount[motor]))
						m_bBL[motor] = TRUE;
				}
				else if(fStep - m_fCurStep[motor] < 0)
				{
					m_bDirection[motor] = FALSE;
					if(m_paramBl.nDir[motor] < 0 && NotZero(m_paramBl.fCount[motor]))
						m_bBL[motor] = TRUE;
				}
				else
					return;
			}
			m_fBLCount[motor] = theApp.m_StageTMCM.MoveDir(motor, fStep);
		}
	}
	else
	{
		if(motor < 2 && (fabs(m_fSLength[motor] - m_fCurStep[motor]) <= m_paramBl.fCount[motor]/m_paramBl.nCoeff[motor]) )
		{
			if( m_fSLength[motor] < m_fCurStep[motor] )
			{
				m_fSLength[motor] = m_fSLength[motor] +m_paramBl.fCount[motor];
				theApp.m_StageTMCM.MoveLimit(motor, m_fSLength[motor]);
				Sleep(100);
				//GetCurStep(motor);
			}
			else if( m_fSLength[motor] > m_fCurStep[motor] )
			{
				m_fSLength[motor] = m_fSLength[motor] -m_paramBl.fCount[motor];
				theApp.m_StageTMCM.MoveLimit(motor, m_fSLength[motor]);
				Sleep(100);
				//GetCurStep(motor);
			}
			if(fStep - m_fSLength[motor] > 0)
			{
				m_bDirection[motor] = TRUE;
				if(m_paramBl.nDir[motor] > 0 && NotZero(m_paramBl.fCount[motor]))
					m_bBL[motor]=TRUE;
			}
			else if(fStep - m_fSLength[motor] < 0)
			{
				m_bDirection[motor] = FALSE;
				if(m_paramBl.nDir[motor] < 0 && NotZero(m_paramBl.fCount[motor]))
					m_bBL[motor]=TRUE;
			}
			else
				return;
		}
		else
		{
			if(fStep - m_fCurStep[motor] > 0)
			{
				m_bDirection[motor] = TRUE;
				if(m_paramBl.nDir[motor] > 0 && NotZero(m_paramBl.fCount[motor]))
					m_bBL[motor]=TRUE;
			}
			else if(fStep - m_fCurStep[motor] < 0)
			{
				m_bDirection[motor] = FALSE;
				if(m_paramBl.nDir[motor] < 0 && NotZero(m_paramBl.fCount[motor]))
					m_bBL[motor]=TRUE;
			}
			else
				return;
		}
		m_fBLCount[motor] = theApp.m_StageTMCM.MoveDir(motor, fStep);
	}
	m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Elapse, NULL );
}

void CPanelStage::MoveInput( short motor )
{
	CString str;
	GetDlgItem(IDC_EDIT_X_UserDisp +motor)->GetWindowText(str);	// mm
	float fMoveStep = (float)(atof(str) * m_dEquivalent[motor]);
	if( m_bRelative )
	{
		if(m_paramDriveDir.nDir[motor] > 0)
			fMoveStep += m_fCurStep[motor];
		else
			fMoveStep -= m_fCurStep[motor];
	}
	/*if(motor ==3)
	{
		fMoveStep = RDriveConversion(m_fCurStep[motor],fMoveStep);
	}*/
	fMoveStep = fMoveStep * m_paramDriveDir.nDir[motor];
	//判断Z轴位置是否满足T轴运行条件
	/*if(motor > 3)
	{
		m_fZHeight = m_fCalibrationZ + m_fCurStep[2] / m_dEquivalent[2] * m_paramDriveDir.nDir[2];
		if(m_fZHeight < 6)
		{
			char szBuf[255];
			sprintf_s( szBuf, "The Z-axis height is too low");	
			Message(szBuf);
			sprintf_s( szBuf, "%s",  m_strMsg[13] );
			MessageBox( szBuf, m_strMsg[0], MB_OK | MB_ICONERROR );
		}
		else
		  MoveAxis( motor, fMoveStep );
	}
	else
	   MoveAxis( motor, fMoveStep );*/
	//机械结构修改，Z和T轴无约束
	MoveAxis( motor, fMoveStep );
}

void CPanelStage::GetCurStep( short motor )
{
	unsigned char ucMotor = SerialNum( motor );
	float lStep = 0;
	lStep = theApp.m_StageTMCM.GetCurPosition( motor);

	CString str;
	str = DisplayOutput(motor,lStep,m_fGoal[motor]);

	GetDlgItem(IDC_EDIT_X_CurStep +motor)->SetWindowText(str);
	m_fCurStep[motor] = lStep;
// 	str.Format( "%.6f", m_fCurStep[anxi]/m_dEquivalent[anxi] );
// 	GetDlgItem(IDC_EDIT_X_UserDisp +anxi)->SetWindowText(str);
}

BOOL CPanelStage::QueryMoving()
{
	for(int i=0;i<5;i++)
	{
		if(m_bIsMoving[i])
			return TRUE;
	}
	if( m_nProcess != 0 )
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

CString CPanelStage::DisplayOutput(short motor, float fValue, float fGoal)
{
	CString str;
	float fShowPos1 = fValue / m_dEquivalent[motor] * m_paramDriveDir.nDir[motor];
	float fShowPos2 = fGoal / m_dEquivalent[motor];
	float fShowPos3 = fValue / m_dEquivalent[motor];

	if(m_bHighPrecCount)
	{
		if(motor < 3)
			str.Format( "%.3f, %.5f", fShowPos1, fShowPos1 );
		/*else if(motor < 4)
		{
			if(fShowPos2 > 179)
				str.Format( "%.2f, %.5f", (fShowPos3- 360) * m_paramDriveDir.nDir[motor],(fShowPos3- 360) * m_paramDriveDir.nDir[motor]);
			else if(fShowPos2 < -179)
				str.Format( "%.2f, %.5f", (fShowPos3+ 360) * m_paramDriveDir.nDir[motor] ,(fShowPos3+ 360) * m_paramDriveDir.nDir[motor]);
			else if(fShowPos2 > 359)
				str.Format( "%d, %d", int(fShowPos3)% 360 * m_paramDriveDir.nDir[motor],int(fShowPos3)% 360 * m_paramDriveDir.nDir[motor]);
			else if(fShowPos2 < -359)
				str.Format( "%d, %d", int(fShowPos3)% (-360)* m_paramDriveDir.nDir[motor], int(fShowPos3)% (-360)* m_paramDriveDir.nDir[motor]);
			else
				str.Format( "%.2f, %.5f", fShowPos1,fShowPos1 );
		}*/
		else
			str.Format( "%.2f, %.5f",fShowPos1, fShowPos1);
	}
	else
	{
		if(motor < 3)
			str.Format( "%.3f", fShowPos1 );
		/*else if(motor < 4)
		{
			if(fShowPos2 > 179)
				str.Format( "%.2f", (fShowPos3- 360) * m_paramDriveDir.nDir[motor]);
			else if(fShowPos2 < -179)
				str.Format( "%.2f", (fShowPos3+ 360) * m_paramDriveDir.nDir[motor] );
			else if(fShowPos2 > 359)
				str.Format( "%d", int(fShowPos3)% 360 * m_paramDriveDir.nDir[motor]);
			else if(fShowPos2 < -359)
				str.Format( "%d", int(fShowPos3)% (-360)* m_paramDriveDir.nDir[motor]);
			else
				str.Format( "%.2f", fShowPos1);
		}*/
		else
			str.Format( "%.2f",fShowPos1);
	}
	
	return str;
}

void CPanelStage::OnBnClickedButtonZero()
{
	Zero();
}

void CPanelStage::Zero()
{
	// 回扫描中心（即用户原点归零）
	Message( "Go Origion X/Y/Z/R/T" );
	int i=0;
	// 保存目标位置
	/*float lSteps = 0.0f;
	int i=0; 
	for(i=0; i<5; i++)
		m_fGoal[i] = lSteps;*/
	// 更新位置显示
	CString str;
	for(i=0; i<5; i++)
	{
		m_fGoal[i] = 0.0f;
		if( i < 3 )
			str.Format( "%.3f", m_fGoal[i]/m_dEquivalent[i] );
		else
			str.Format( "%.2f", m_fGoal[i]/m_dEquivalent[i] );
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
// 	if(!m_bEnable[2])
// 		theApp.m_StageTMCM.Enable(2);
	for(i=0; i<5; i++)
	{
		m_bIsMoving[i] = TRUE;
		CStageTMCMManager::Instance().m_bIsMoving[i] = m_bIsMoving[i];
	}

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
	theApp.m_StageTMCM.LimitStatus(13,3,1);
	//下发停止命令
	Message( "Stop X/Y/Z/R/T move" );
	BOOL Moveing[5];
	int i=0; 
	for(i=0; i<5; i++)
		Moveing[i] = FALSE;
	for(i=0; i<5; i++)
	{
		if(m_bIsMoving[i])
		{
			Moveing[i]= TRUE;
			m_nPhaseInit[i]  = -1;
		}
		/*m_bIsMoving[i] = FALSE;
		CStageTMCMManager::Instance().m_bIsMoving[i] = m_bIsMoving[i];*/
	}
	m_nProcess		= 0;
	m_nMoveSteps	= -1;

	CString str;
	for(i=0; i<5; i++)
	{
	    if( Moveing[i] )
		{
			theApp.m_StageTMCM.Stop(i);
			m_bToHome[i] = FALSE;
			m_bOrigion[i] = FALSE;
			Moveing[i] = FALSE;
			m_bBL[i] = FALSE;
			m_nTimer[i] = SetTimer(i+6, 5000, NULL);
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
		fPos[i] = (float)(atof(str)*m_paramDriveDir.nDir[i]);   //mm
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

void CPanelStage::Move5Axis_JudgeDirMove( short motor )
{
	if(m_bSoftLimit)
	{
		m_fSLimit[motor *2]		= (float)(m_factualLimit[motor *2]* m_dEquivalent[motor]);
		m_fSLimit[motor *2 +1]	= (float)(m_factualLimit[motor *2 +1]* m_dEquivalent[motor]);
		if(m_fGoal[motor] * m_paramDriveDir.nDir[motor] < m_fSLimit[motor *2] )
		{
			m_fGoal[motor] = m_fSLimit[motor *2]* m_paramDriveDir.nDir[motor];
			m_bDirection[motor] = FALSE;
		}
		else if( m_fGoal[motor]* m_paramDriveDir.nDir[motor] > m_fSLimit[motor *2 +1])
		{
			m_fGoal[motor] = m_fSLimit[motor *2 +1]* m_paramDriveDir.nDir[motor];
			m_bDirection[motor] = TRUE;
		}
		else
		{
			// bSoftLimit时，还有可能出现下面的情况吗？
			if(m_fGoal[motor] - m_fCurStep[motor] > 0)
			{
				m_bDirection[motor] = TRUE;
				if(m_paramBl.nDir[motor] > 0 && NotZero(m_paramBl.fCount[motor]))
					m_bBL[motor]=TRUE;
			}
			else if(m_fGoal[motor] - m_fCurStep[motor] < 0)
			{
				m_bDirection[motor] = FALSE;
				if(m_paramBl.nDir[motor] < 0 && NotZero(m_paramBl.fCount[motor]))
					m_bBL[motor]=TRUE;
			}
		}
	}
	else
	{
		if(m_fGoal[motor] - m_fCurStep[motor] > 0)
		{
			m_bDirection[motor] = TRUE;
			if(m_paramBl.nDir[motor] > 0 && NotZero(m_paramBl.fCount[motor]))
				m_bBL[motor]=TRUE;
		}
		else if(m_fGoal[motor] - m_fCurStep[motor] < 0)
		{
			m_bDirection[motor] = FALSE;
			if(m_paramBl.nDir[motor] < 0 && NotZero(m_paramBl.fCount[motor]))
				m_bBL[motor]=TRUE;
		}
	}

	//if(!m_bEnable[motor])
		//theApp.m_StageTMCM.Enable(motor);
	unsigned char ucMotor = SerialNum( motor );
	char szBuf[255];
	sprintf_s( szBuf, "Move %c to", ucMotor );
	Message( szBuf );
	m_bIsMoving[motor] = TRUE;
	CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
	switch( motor )
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
			if(m_fGoal[i] * m_paramDriveDir.nDir[i]< m_fSLimit[i *2] )
			{
				m_fGoal[i] = m_fSLimit[i *2]* m_paramDriveDir.nDir[i];
				m_bDirection[i] = FALSE;
			}
			else if( m_fGoal[i]* m_paramDriveDir.nDir[i] > m_fSLimit[i *2 +1])
			{
				m_fGoal[i] = m_fSLimit[i *2 +1]* m_paramDriveDir.nDir[i];
				m_bDirection[i] = TRUE;
			}
		}
	}

	// 更新位置显示
	CString str;
	str.Format( "%.4f", fX * m_paramDriveDir.nDir[0]);
	GetDlgItem(IDC_EDIT_X_UserDisp)->SetWindowText(str);
	str.Format( "%.4f", fY * m_paramDriveDir.nDir[1]);
	GetDlgItem(IDC_EDIT_Y_UserDisp)->SetWindowText(str);
	str.Format( "%.4f", fZ * m_paramDriveDir.nDir[2]);
	GetDlgItem(IDC_EDIT_Z_UserDisp)->SetWindowText(str);
	str.Format( "%.3f", fR * m_paramDriveDir.nDir[3]);
	GetDlgItem(IDC_EDIT_R_UserDisp)->SetWindowText(str);
	str.Format( "%.3f", fT * m_paramDriveDir.nDir[4]);
	GetDlgItem(IDC_EDIT_T_UserDisp)->SetWindowText(str);

	for(int i=0;i<5;i++)
	{
		m_bEqual[i] = FALSE;
//		if(m_fGoal[i] != m_fCurStep[i])
		if( fabs(m_fGoal[i] -m_fCurStep[i]) > FLT_EPSILON )
		{
			m_bEqual[i] = TRUE;
			m_bIsMoving[i] = TRUE;
			CStageTMCMManager::Instance().m_bIsMoving[i] = m_bIsMoving[i];
			m_nTimer[i] = 0;
		}
	}
	m_nProcess = -1;

	if(m_bEqual[2] && m_bMotorFault[2] && m_bCommunicationFault[2])
		Move5Axis_JudgeDirMove( 2 );
	else
	{
		/*m_fZHeight = m_fCalibrationZ + m_fCurStep[2] / m_dEquivalent[2] *m_paramDriveDir.nDir[2] ;
		if(m_bEqual[4] && m_fZHeight >= 6 && m_bMotorFault[4] && m_bCommunicationFault[4])
		{
			Move5Axis_JudgeDirMove( 4 );
		}*/
		if(m_bEqual[4] && m_bMotorFault[4] && m_bCommunicationFault[4])
			Move5Axis_JudgeDirMove( 4 );
		else
		{
			/*if(m_bMotorFault[4] && m_bCommunicationFault[4] && m_fZHeight < 6 && m_bEqual[4])
			{
				char szBuf[255];
				sprintf_s( szBuf, "The Z-axis height is too low");	
				Message(szBuf);
				sprintf_s( szBuf, "%s",  m_strMsg[13] );
				MessageBox( szBuf, m_strMsg[0], MB_OK | MB_ICONERROR );
			}*/
			if(m_bEqual[3] && m_bMotorFault[3] && m_bCommunicationFault[3])
				Move5Axis_JudgeDirMove( 3 );
			else
			{
				if(m_bEqual[0] && m_bMotorFault[0] && m_bCommunicationFault[0])
					Move5Axis_JudgeDirMove( 0 );
				else
				{
					if(m_bEqual[1] && m_bMotorFault[1] && m_bCommunicationFault[1])
						Move5Axis_JudgeDirMove( 1 );
					else
					{
						m_nProcess = 0;
						return;
					}
				}
			}
		}
	}
}

void CPanelStage::MoveSteps_Axis( short motor )
{
	unsigned char ucMotor = SerialNum( motor );
	if( m_nTimer[motor] == 0 )
	{
		switch( motor )
		{
		case	4:	// T
			m_nMoveSteps = 1;
			break;
		case	2:	// Z
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
			m_bIsMoving[motor] = TRUE;
			CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
			m_bToHome[motor] = TRUE;
			switch( motor )
			{
			case	2:	// Z
			case	3:	// R
			case	4:	// T
				if(m_paramDriveDir.nDir[motor]>0)
					m_bDirection[motor] = FALSE;
				else
					m_bDirection[motor] = TRUE;
				m_nPhaseInit[motor] = 1;
				break;
			case	0:	// X
			case	1:	// Y
				if(m_paramDriveDir.nDir[motor]>0)
					m_bDirection[motor] = TRUE;
				else
					m_bDirection[motor] = FALSE;
				m_nPhaseInit[motor] = 0;
				break;
			}
			//GetCurStep(motor);
			if( m_bDirection[motor] )
				m_fGoal[motor] = 640000;
			else
				m_fGoal[motor] = -640000;
			//R轴上电
			switch(motor)
			{
			case 3:
				theApp.m_StageTMCM.PowerOfforOn(0,3);
				break;
			}
			theApp.m_StageTMCM.GoHome(motor, m_fGoal[motor]);
			m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Elapse, NULL );
			if(m_bMoveXY)
			{
				m_bMoveXY = FALSE;
				MoveSteps_Axis(1);
			}
		}
	}
}

void CPanelStage::Move5Axis_FromThird( float fX, float fY, float fZ, float fR, float fT )
{
	Move5Axis( fX * m_paramDriveDir.nDir[0],
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
	sprintf_s( szBuf, "Move 2 Axis from third: %.3f, %.3f",	
		fX *m_paramDriveDir.nDir[0], fY *m_paramDriveDir.nDir[1] );
	Message( szBuf );
}

void CPanelStage::MoveSteps_JudgeDirMove( short motor )
{
	if(m_nTimer[motor] == 0)
	{
		unsigned char ucAxis = SerialNum( motor );
		switch( motor )
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
		if(m_bEqual[motor] && m_nProcess == -1 && m_bMotorFault[motor] && m_bCommunicationFault[motor])
		{
			if(m_fGoal[motor] - m_fCurStep[motor] > 0)
			{
				m_bDirection[motor] = TRUE;
				if(m_paramBl.nDir[motor] > 0 && NotZero(m_paramBl.fCount[motor]))
					m_bBL[motor]=TRUE;
			}
			else if(m_fGoal[motor] - m_fCurStep[motor] < 0)
			{
				m_bDirection[motor] = FALSE;
				if(m_paramBl.nDir[motor] < 0 && NotZero(m_paramBl.fCount[motor]))
					m_bBL[motor]=TRUE;
			}
			//if(!m_bEnable[motor])
				//theApp.m_StageTMCM.Enable(motor);
			char szBuf[255];
			/*if(motor > 3)
			{
				m_fZHeight = m_fCalibrationZ + m_fCurStep[2] / m_dEquivalent[2] * m_paramDriveDir.nDir[2];
				if(m_fZHeight < 6)
				{
					sprintf_s( szBuf, "The Z-axis height is too low");	
					Message(szBuf);
					sprintf_s( szBuf, "%s",  m_strMsg[13] );
					MessageBox( szBuf, m_strMsg[0], MB_OK | MB_ICONERROR );
					m_bIsMoving[motor] = FALSE;
					MoveSteps();
				}
				else
				{
					sprintf_s( szBuf, "Move %c To", ucAxis );
					Message( szBuf );
					m_fBLCount[motor] = theApp.m_StageTMCM.MoveDir(motor, m_fGoal[motor]);
					m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Elapse, NULL );
				}
			}
			else*/
			{
				sprintf_s( szBuf, "Move %c To", ucAxis );
				Message( szBuf );
				m_bIsMoving[motor] = TRUE;
				CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
				//R轴上电
				switch(motor)
				{
				case 3:
					theApp.m_StageTMCM.PowerOfforOn(0,3);
					break;
				}
				m_fBLCount[motor] = theApp.m_StageTMCM.MoveDir(motor, m_fGoal[motor]);
				m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Elapse, NULL );
				if(m_bMoveXY)
				{
					m_bMoveXY = FALSE;
					MoveSteps_JudgeDirMove(1);
				}
			}
		}
		else
		{
			m_bIsMoving[motor] = FALSE;
			CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
			MoveSteps();
		}
	}
}

void CPanelStage::MoveSteps()
{
	if( m_nProcess > 0 )
	{
		CString str, strMsgWarning, strMsgAxisInit, strMsgAxis;
		if( m_nMoveSteps == 0 )
		{
			// Home：先T
			// 正在进行第零步：移动T轴
			if(m_bAxisInitia[4])
			{
				if(m_bCommunicationFault[4] && m_bMotorFault[4])
					MoveSteps_Axis( 4 );
				else
				{
					//MessageBox("T 轴未正确进行初始化,T 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "T %s, T %s", m_strMsg[15], m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
					m_nMoveSteps = 1; 
					MoveSteps();
				}
			}
			else
			{
				if(m_bCommunicationFault[4] && m_bMotorFault[4])
				{
					//GetCurStep(4);
					str = DisplayOutput(4, m_fCurStep[4], m_fGoal[4]);
					GetDlgItem(IDC_EDIT_X_CurStep +4)->SetWindowText(str);
				}
				else
				{
					//MessageBox("T 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "T %s", m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
				}
				m_nMoveSteps = 1; 
				MoveSteps();
			}	
		}
		else if( m_nMoveSteps == 1 )
		{
			// Home：先Z
			// 正在进行第一步：移动Z轴
			if(m_bAxisInitia[2])
			{
				if(m_bCommunicationFault[2] && m_bMotorFault[2])
					MoveSteps_Axis( 2 );
				else
				{
					//MessageBox("Z 轴未正确进行初始化,Z 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "Z %s, Z %s", m_strMsg[15], m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
					m_nMoveSteps = 2; 
					MoveSteps();
				}
			}
			else
			{
				if(m_bCommunicationFault[2] && m_bMotorFault[2])
				{
					//读取当前位置
					//GetCurStep(2);
					str = DisplayOutput(2, m_fCurStep[2], m_fGoal[2]);
					GetDlgItem(IDC_EDIT_X_CurStep +2)->SetWindowText(str);
				}
				else
				{
					//MessageBox("Z 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "Z %s", m_strMsg[16] );
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
					//MessageBox("R 轴未正确进行初始化,X 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
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
					//GetCurStep(3);
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
					//GetCurStep(0);
					str = DisplayOutput(0, m_fCurStep[0], m_fGoal[0]);
					GetDlgItem(IDC_EDIT_X_CurStep +0)->SetWindowText(str);
					Sleep(100);
					//GetCurStep(1);
					str = DisplayOutput(1, m_fCurStep[1], m_fGoal[1]);
					GetDlgItem(IDC_EDIT_X_CurStep +1)->SetWindowText(str);
				}
				else if(m_bCommunicationFault[0] && m_bMotorFault[0])
				{
					//GetCurStep(0);
					str = DisplayOutput(0, m_fCurStep[0], m_fGoal[0]);
					GetDlgItem(IDC_EDIT_X_CurStep +0)->SetWindowText(str);
					//MessageBox("Y 轴异常", "警告", MB_OK|MB_ICONEXCLAMATION );
					str.Format( "Y %s", m_strMsg[16] );
					MessageBox( str, m_strMsg[14], MB_OK|MB_ICONEXCLAMATION );
				}
				else if(m_bCommunicationFault[1] && m_bMotorFault[1])
				{
					//GetCurStep(1);
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
				if(m_nProcess == -1 && m_bMotorFault[2] && m_bCommunicationFault[2])
				{
					m_bIsMoving[2] = TRUE;
					CStageTMCMManager::Instance().m_bIsMoving[2] = m_bIsMoving[2];
					m_fBLCount[2] = theApp.m_StageTMCM.MoveDir(2, m_fGoal[2]);
					m_nTimer[2] = SetTimer( 3, nStageTMCM_Elapse, NULL );
				}
				else
				{
					m_bIsMoving[2] = FALSE;
					CStageTMCMManager::Instance().m_bIsMoving[2] = m_bIsMoving[2];
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

void CPanelStage::DiffCenter(short motor)
{
	m_bIsMoving[motor] = TRUE;
	unsigned char ucMotor = SerialNum(motor);

	m_fGoal[motor] = (float)(m_fCali[motor] *m_dEquivalent[motor]*m_paramDriveDir.nDir[motor]);
	theApp.m_StageTMCM.GoOrigion(motor, m_fGoal[motor]);
	char szBuf[255];
	sprintf_s( szBuf, "%f", m_fCali[motor] );
	GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText(szBuf);
	//m_nTimer[motor] = SetTimer( motor+1, nStageTMCM_Elapse, NULL );
}

void CPanelStage::SaveToINI( int nIndex )
{
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\Stage\\StageTMCM.ini");
	char szBuf[255];
	// 写入配置文件
	switch( nIndex )
	{
	case	0:	// 保存工作原点绝对位置SaveUserOrigin
		sprintf_s( szBuf, "%.4f,%.4f,%.4f,%.3f,%.3f", m_fCali[0], m_fCali[1], m_fCali[2], m_fCali[3], m_fCali[4] );
		WritePrivateProfileString( "StageTMCMCfg", "Cali", szBuf, path );
		break;
	case 1:
		sprintf_s( szBuf, "%d,%d,%d,%d,%d", m_bCommunicationFault[0], m_bCommunicationFault[1], m_bCommunicationFault[2], m_bCommunicationFault[3], m_bCommunicationFault[4] );
		WritePrivateProfileString( "StageTMCMCfg", "CommunicationFault", szBuf, path );
		break;
	case	2:	// OnBnClickedButtonDriverInit: SoftLimit
		sprintf_s( szBuf, "%d", m_bSoftLimit);
		WritePrivateProfileString( "StageTMCMCfg", "SLimit", szBuf, path );
		break;
	case 3:		//OnBnClickedButtonLimitParam: Limit disabling and swapping
		sprintf_s( szBuf, "%d,%d,%d,%d,%d", 
				m_paramLimitParam.nLeftLimit[0],
				m_paramLimitParam.nLeftLimit[1],
				m_paramLimitParam.nLeftLimit[2],
				m_paramLimitParam.nLeftLimit[3],
				m_paramLimitParam.nLeftLimit[4] );
		WritePrivateProfileString( "StageTMCMCfg", "LeftLimit ", szBuf, path );
		sprintf_s( szBuf, "%d,%d,%d,%d,%d", 
				m_paramLimitParam.nRightLimit[0],
				m_paramLimitParam.nRightLimit[1],
				m_paramLimitParam.nRightLimit[2],
				m_paramLimitParam.nRightLimit[3],
				m_paramLimitParam.nRightLimit[4] );
		WritePrivateProfileString( "StageTMCMCfg", "RightLimit ", szBuf, path );
		sprintf_s( szBuf, "%d,%d,%d,%d,%d", 
				m_paramLimitParam.nExchangeLimit[0],
				m_paramLimitParam.nExchangeLimit[1],
				m_paramLimitParam.nExchangeLimit[2],
				m_paramLimitParam.nExchangeLimit[3],
				m_paramLimitParam.nExchangeLimit[4] );
		WritePrivateProfileString( "StageTMCMCfg", "ExchangeLimit ", szBuf, path );
		sprintf_s( szBuf, "%d,%d,%d,%d,%d", 
				m_paramLimitParam.nLeftLevelReversal[0],
				m_paramLimitParam.nLeftLevelReversal[1],
				m_paramLimitParam.nLeftLevelReversal[2],
				m_paramLimitParam.nLeftLevelReversal[3],
				m_paramLimitParam.nLeftLevelReversal[4] );
		WritePrivateProfileString( "StageTMCMCfg", "LeftLimitLevel ", szBuf, path );
		sprintf_s( szBuf, "%d,%d,%d,%d,%d", 
				m_paramLimitParam.nRightLevelReversal[0],
				m_paramLimitParam.nRightLevelReversal[1],
				m_paramLimitParam.nRightLevelReversal[2],
				m_paramLimitParam.nRightLevelReversal[3],
				m_paramLimitParam.nRightLevelReversal[4] );
		WritePrivateProfileString( "StageTMCMCfg", "RightLimitLevel ", szBuf, path );
		break;
	case	4:	// OnBnClickedButtonSetup
		sprintf_s( szBuf, "%d", m_nPort);
		WritePrivateProfileString( "StageTMCMCfg", "Port", szBuf, path );
		sprintf_s( szBuf, "%d", m_bBlClick );
		WritePrivateProfileString( "StageTMCMCfg", "BlClick", szBuf, path );
		sprintf_s( szBuf, "%d", m_bBlTrack );
		WritePrivateProfileString( "StageTMCMCfg", "BlTrack", szBuf, path );
		sprintf_s( szBuf, "%d", m_nVentSel );
		WritePrivateProfileString( "StageTMCMCfg", "VentSel", szBuf, path );
		sprintf_s( szBuf, "%d", m_bSoftLimit );
		WritePrivateProfileString( "StageTMCMCfg", "SLimit", szBuf, path );
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
			CStageTMCMManager::Instance().m_bIsMoving[i] =  TRUE;
			theApp.m_StageTMCM.LocaZero(i);
			Sleep(100);
			theApp.m_StageTMCM.Reset(i);
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
	Message( "GoOrigion X/Y/Z/R/T" );

	m_bToHome[4] = TRUE;
	m_bIsMoving[4] = TRUE;
	CStageTMCMManager::Instance().m_bIsMoving[4] = m_bIsMoving[4];

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
	Message( "GoHome X/Y/Z/R/T" );

	m_bToHome[4] = TRUE;
	m_bIsMoving[4] = TRUE;
	CStageTMCMManager::Instance().m_bIsMoving[4] = m_bIsMoving[4];

	m_nMoveSteps =0;
	m_nProcess = 1;
	//R轴左限位启用
	theApp.m_StageTMCM.LimitStatus(13,3,0);
	MoveSteps();
}

void CPanelStage::OnBnClickedButtonLimitparam()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	CDlgLimitParam dlg;
	strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
	for(i=0;i<5;i++)
	{
		dlg.m_nLeftLimit[i] = m_paramLimitParam.nLeftLimit[i];
		dlg.m_nRightLimit[i] = m_paramLimitParam.nRightLimit[i];
		dlg.m_nExchangeLimit[i] = m_paramLimitParam.nExchangeLimit[i];
		dlg.m_nLeftLimitLevel[i] = m_paramLimitParam.nLeftLevelReversal[i];
		dlg.m_nRightLimitLevel[i] = m_paramLimitParam.nRightLevelReversal[i];
	}
	if( dlg.DoModal() == IDOK )
	{
		for(i=0;i<5;i++)
		{
			if( m_paramLimitParam.nLeftLimit[i] != dlg.m_nLeftLimit[i] )
			{
				m_paramLimitParam.nLeftLimit[i] = dlg.m_nLeftLimit[i];
				theApp.m_StageTMCM.LimitStatus(13, i, m_paramLimitParam.nLeftLimit[i]);
			}
			if( m_paramLimitParam.nRightLimit[i] != dlg.m_nRightLimit[i] )
			{
				m_paramLimitParam.nRightLimit[i] = dlg.m_nRightLimit[i];
				theApp.m_StageTMCM.LimitStatus(12, i, m_paramLimitParam.nRightLimit[i]);
			}
			if( m_paramLimitParam.nExchangeLimit[i] != dlg.m_nExchangeLimit[i] )
			{
				m_paramLimitParam.nExchangeLimit[i] = dlg.m_nExchangeLimit[i];
				theApp.m_StageTMCM.LimitExchange(i, m_paramLimitParam.nExchangeLimit[i]);
			}
			if(m_paramLimitParam.nLeftLevelReversal[i] != dlg.m_nLeftLimitLevel[i])
			{
				m_paramLimitParam.nLeftLevelReversal[i] = dlg.m_nLeftLimitLevel[i];
				theApp.m_StageTMCM.LimitLevel(25,i,m_paramLimitParam.nLeftLevelReversal[i]);
			}
			if(m_paramLimitParam.nRightLevelReversal[i] != dlg.m_nRightLimitLevel[i])
			{
				m_paramLimitParam.nRightLevelReversal[i] = dlg.m_nRightLimitLevel[i];
				theApp.m_StageTMCM.LimitLevel(24,i,m_paramLimitParam.nRightLevelReversal[i]);
			}
		}
		SaveToINI(3);
	}
}

void CPanelStage::OnBnClickedButtonSetup()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rc;
	GetWindowRect(rc);
	CDlgSetup dlg;
	dlg.m_nPort = m_nPort;
	dlg.m_bClick = m_bBlClick;
	dlg.m_bTrack = m_bBlTrack;
	dlg.m_nVentSel = m_nVentSel;
	dlg.m_bSoftLimit = m_bSoftLimit;
	dlg.m_strMsg[0] = m_strMsg[0];
	dlg.m_bDbClick = m_bDbClick;
	dlg.m_bAdvance = (rc.Height() > 600) ? TRUE : FALSE;
	strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
	switch(dlg.DoModal())
	{
	case IDOK:
		m_nPort		= dlg.m_nPort;
		m_bBlClick	= dlg.m_bClick;
		m_bBlTrack	= dlg.m_bTrack;
		m_nVentSel	= dlg.m_nVentSel;
		m_bSoftLimit = dlg.m_bSoftLimit;
		m_bDbClick	= dlg.m_bDbClick;
		SaveToINI( 4 );
		break;
	case IDC_BUTTON_Relink:
		{
			if( m_bIsTMCMReady )
			{
				for(int i=0;i<5;i++)
				{
					m_bIsMoving[i] = FALSE;
					CStageTMCMManager::Instance().m_bIsMoving[i] = m_bIsMoving[i];
				}
				Sleep(100);
				theApp.m_StageTMCM.Close();
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
// 	GetDlgItem(IDC_Stage_Mag)->SetWindowText(str);
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
		CStageTMCMManager::Instance().m_bIsMoving[i] = m_bIsMoving[i];
	}

	str.Format( "%.4f", fMoveStepX/m_dEquivalent[0] *m_paramDriveDir.nDir[0]);
	GetDlgItem(IDC_EDIT_X_UserDisp)->SetWindowText(str);
	str.Format( "%.4f", fMoveStepY/m_dEquivalent[1] *m_paramDriveDir.nDir[1]);
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

		m_fBLCount[0] = theApp.m_StageTMCM.MoveDir(0, fMoveStepX);
		m_nTimer[0] = SetTimer( 1, nStageTMCM_Elapse, NULL );

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
		m_fBLCount[1] = theApp.m_StageTMCM.MoveDir(1, fMoveStepY);
		m_nTimer[1] = SetTimer( 2, nStageTMCM_Elapse, NULL );
	}
	else
	{
		theApp.m_StageTMCM.Move(0, fMoveStepX ,m_bRelative);
		m_nTimer[0] = SetTimer(1,nStageTMCM_Elapse,NULL);

		theApp.m_StageTMCM.Move(1, fMoveStepY, m_bRelative);
		m_nTimer[1] = SetTimer(2,nStageTMCM_Elapse,NULL);
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
	else if(nIDEvent > 5 && nIDEvent < 11)
		Timer_Stop((short)nIDEvent -6);
	/*else if(nIDEvent > 99 && nIDEvent < 105)
		Timer_AutoMove((short)nIDEvent -6);*/
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

void CPanelStage::Grounding()
{
	//theApp.m_StageTMCM.close(anxi);
	short motor = 2;
	char szBuf[255];
	sprintf_s( szBuf, "grounding occurs");	//出现接地
	Message(szBuf);
	Stop();
	for(int i=0;i<5;i++)
	{
		KillTimer(i+1);
		m_nTimer[i]=0;
		m_bIsMoving[i] = FALSE;
		CStageTMCMManager::Instance().m_bIsMoving[i] = m_bIsMoving[i];
	}
	sprintf_s( szBuf, "%s",  m_strMsg[12] );
	MessageBox( szBuf, m_strMsg[0], MB_OK | MB_ICONERROR );
}

void CPanelStage::ToHome_XY( unsigned char ucMotor, short motor )
{
	char szBuf[255];
	//GetCurStep(motor);

	//float* fState = theApp.m_StageTMCM.StatusRead(motor);
	//m_paramLimitDir.fL[motor] = *fState;
	//m_paramLimitDir.fR[motor] = *(fState+1);
	if(motor == 3)
	{
		if((m_fGoal[motor] - m_fCurStep[motor])  * m_paramDriveDir.nDir[motor] >= 0)
			m_paramLimitStatus.nL[motor] = 1;
		else if((m_fGoal[motor] - m_fCurStep[motor])  * m_paramDriveDir.nDir[motor] < 0)
			m_paramLimitStatus.nR[motor] = 1;
	}

	if(!m_paramLimitStatus.nL[motor] || !m_paramLimitStatus.nR[motor])	// fState == 10 || fState == 11)
	{
		/*KillTimer(motor +1);
		m_nTimer[motor] = 0;*/
// 		m_paramLimitDir.fR[motor] = 0;

		if(m_bNum[motor]  && m_nPhaseInit[motor] ==0)
		{
			//已到正向限位
			sprintf_s( szBuf, "Axis %c goes to the positive limit", ucMotor );	// 到正向极限
			Message( szBuf );
			// 清零并到负向极限
			sprintf_s( szBuf, "Axis %c reset and reach the negative limit", ucMotor );
			Message( szBuf );
			theApp.m_StageTMCM.Reset(motor);
			Sleep(100);
			theApp.m_StageTMCM.GoHome(motor, -500000 *m_paramDriveDir.nDir[motor]);
			m_nPhaseInit[motor] = 1;
			Sleep(100);		
		}
		else if(m_bNum[motor] && m_nPhaseInit[motor] == 1 && m_fCurStep[motor]*m_paramDriveDir.nDir[motor] < -128000)
		{
			sprintf_s( szBuf, "Axis %c goes to the negative limit: %.6f", ucMotor, m_fCurStep[motor]/m_dEquivalent[motor] );
			Message( szBuf );
			//GetCurStep(motor); 
			m_fDis[motor] = (float)(m_fCurStep[motor]*0.5);
			theApp.m_StageTMCM.GoHome(motor, m_fDis[motor]);
			// 到负向极限
			m_bNum[motor] = FALSE;
			m_nPhaseInit[motor] = 2;
		}
//		theApp.m_StageTMCM.Enable(motor);
		Sleep(100);
		//m_nTimer[motor] = SetTimer( motor+1, nStageTMCM_Elapse, NULL );
		//m_bNum[motor] = !(m_bNum[motor]);
	}
	else if(!m_bNum[motor] && m_nPhaseInit[motor] == 2 &&  fabs(m_fCurStep[motor]-m_fDis[motor]) < m_fDeadline[motor]*m_dEquivalent[motor] )
	{
		if( m_bOrigion[motor] )
		{
			if( fabs(m_fDis[motor] / m_dEquivalent[motor] -m_fCali[motor]*m_paramDriveDir.nDir[motor])< m_fDeadline[motor] )
			{
				sprintf_s( szBuf, "Axis %c goes to the original", ucMotor );	// 到扫描原点
				Message( szBuf );
				KillTimer(motor+1);
				m_nTimer[motor] = 0;
				theApp.m_StageTMCM.LocaZero(motor);
				Sleep(100);
				theApp.m_StageTMCM.Reset(motor);
				m_fGoal[motor] = 0.0;
				m_fCurStep[motor] = 0.0;
				//theApp.m_StageTMCM.GetCurPosition(motor);
				m_bOrigion[motor] =FALSE;
				m_bToHome[motor] =FALSE;
				m_bNum[motor] = TRUE;
				m_nTimer[motor] = SetTimer( motor+1, nStageTMCM_Elapse, NULL );
				GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText("0.0");
				m_nPhaseInit[motor] = -1;
				if(m_nProcess > 0)
					MoveSteps();
				if(m_bSoftLimit)
				{
					m_paramZRes.fLimitNeg[motor] = m_paramZRes.fLimitNeg[motor] - m_fCali[motor];
					m_paramZRes.fLimitPos[motor] = m_paramZRes.fLimitPos[motor] - m_fCali[motor];
					m_factualLimit[motor*2] = m_factualLimit[motor*2] - m_fCali[motor];
					m_factualLimit[motor*2+1] = m_factualLimit[motor*2+1] - m_fCali[motor];
				}
			}
			else
			{
				sprintf_s( szBuf, "Axis %c goes to the HOME-scan", ucMotor );	// 到机械原点-扫描
				Message( szBuf );
				theApp.m_StageTMCM.LocaZero(motor);
				Sleep(100);
				theApp.m_StageTMCM.Reset(motor);
				m_fCurStep[motor] = 0.0;
				DiffCenter(motor);
				m_fDis[motor] = (float)(m_fCali[motor]*m_dEquivalent[motor]*m_paramDriveDir.nDir[motor]);
			}
		}
		else
		{
			sprintf_s( szBuf, "Axis %c goes to the HOME-mechanical", ucMotor );	// 到机械原点-机械
			Message( szBuf );
			theApp.m_StageTMCM.LocaZero(motor);
			Sleep(100);
			theApp.m_StageTMCM.Reset(motor);
//			theApp.m_StageTMCM.Enable(motor);
			m_fCurStep[motor] = 0.0;
			sprintf_s( szBuf, "Kill Timer %c", ucMotor );
			Message( szBuf );
			KillTimer(motor+1);
			m_nTimer[motor] = 0;
			m_bToHome[motor] =FALSE;
			m_fGoal[motor] = 0.0;
			GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText("0.000");
//			GetCurStep(motor);
// 			GetDlgItem(IDC_EDIT_X_CurStep +motor)->Invalidate();
			m_nTimer[motor] = SetTimer( motor+1, nStageTMCM_Elapse, NULL );
			m_bNum[motor] = TRUE;
			m_bIsMoving[motor] = FALSE;
			m_nPhaseInit[motor] = -1;
			if(m_nProcess > 0  && !m_bIsMoving[0] && !m_bIsMoving[1])
				MoveSteps();
			// 写入配置文件
			SaveToINI( 1 );
			if(m_bSoftLimit)
			{
				m_paramZRes.fLimitNeg[motor] = m_paramZRes.fLimitNeg[motor] + m_fCali[motor];
				m_paramZRes.fLimitPos[motor] = m_paramZRes.fLimitPos[motor] + m_fCali[motor];
				m_factualLimit[motor*2] = m_factualLimitH[motor*2] + m_fCali[motor];
				m_factualLimit[motor*2+1] = m_factualLimitH[motor*2+1] + m_fCali[motor];
			}
		}
	}
}

void CPanelStage::ToHome_ZRT( unsigned char ucMotor, short motor )
{
	char szBuf[255];
	//GetCurStep(motor);

	//float* fState = theApp.m_StageTMCM.StatusRead(motor);
	//m_paramLimitDir.fL[motor] = *fState;
	//m_paramLimitDir.fR[motor] = *(fState+1);
	if(motor == 3)
	{
		if((m_fGoal[motor] - m_fCurStep[motor])  * m_paramDriveDir.nDir[motor] >= 0)
			m_paramLimitStatus.nL[motor] = 1;
		else if((m_fGoal[motor] - m_fCurStep[motor])  * m_paramDriveDir.nDir[motor] < 0)
			m_paramLimitStatus.nR[motor] = 1;
	}

	if(!m_paramLimitStatus.nL[motor] || !m_paramLimitStatus.nR[motor])	// fState == 10 || fState == 11)
	{
		/*KillTimer(motor+1);
		m_nTimer[motor] = 0;*/

		if(m_bNum[motor]  && m_nPhaseInit[motor] == 1)
		{
			// 到负向极限
			sprintf_s( szBuf, "Axis %c goes to the negative limit: %.6f", ucMotor, m_fCurStep[motor]/m_dEquivalent[motor] );
			Message( szBuf );
			//GetCurStep(motor); 
			if( motor > 3 )
				m_fDis[motor] = (float)(m_fCurStep[motor] + m_fCalibrationT*m_dEquivalent[motor]*m_paramDriveDir.nDir[motor]);
			else if( motor > 2 )
				m_fDis[motor] = (float)(m_fCurStep[motor] + m_fCalibrationR*m_dEquivalent[motor] *m_paramDriveDir.nDir[motor]);	
			else
				m_fDis[motor] = (float)(m_fCurStep[motor] + m_fCalibrationZ*m_dEquivalent[motor]*m_paramDriveDir.nDir[motor]);
			Message("Get the target position");	// 获取目标位置

			theApp.m_StageTMCM.GoHome(motor, m_fDis[motor]);
			m_nPhaseInit[motor] = 2;
			m_bNum[motor] = FALSE;
			Sleep(100);
			//m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Elapse, NULL );
		}
	}
	else if( !m_bNum[motor] && m_nPhaseInit[motor] ==2 &&  fabs(m_fCurStep[motor]-m_fDis[motor])< m_fDeadline[motor]*m_dEquivalent[motor] )
	{
		if( m_bOrigion[motor] )
		{
			if( fabs(m_fDis[motor] / m_dEquivalent[motor] -m_fCali[motor]*m_paramDriveDir.nDir[motor])< m_fDeadline[motor] )
			{
				sprintf_s( szBuf, "Axis %c goes to the original", ucMotor );	// 到扫描原点
				Message( szBuf );
				KillTimer(motor+1);
				m_nTimer[motor] = 0;
				theApp.m_StageTMCM.LocaZero(motor);
				Sleep(100);
				theApp.m_StageTMCM.Reset(motor);
				m_fGoal[motor] = 0;
				//theApp.m_StageTMCM.GetCurPosition(motor);
				m_fCurStep[motor] = 0.0;
				m_bOrigion[motor] =FALSE;
				m_bToHome[motor] =FALSE;
				m_bNum[motor] = TRUE;
				m_nTimer[motor] = SetTimer( motor+1, nStageTMCM_Elapse, NULL );
				GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText("0.0");
				m_nPhaseInit[motor] = -1;
				if(m_nProcess > 0)
					MoveSteps();
				//R轴左限位禁用
				if(motor == 3)
					theApp.m_StageTMCM.LimitStatus(13,motor,1);
				if(m_bSoftLimit)
				{
					m_paramZRes.fLimitNeg[motor] = m_paramZRes.fLimitNeg[motor] - m_fCali[motor];
					m_paramZRes.fLimitPos[motor] = m_paramZRes.fLimitPos[motor] - m_fCali[motor];
					m_factualLimit[motor*2] = m_factualLimitH[motor*2] - m_fCali[motor];
					m_factualLimit[motor*2+1] = m_factualLimitH[motor*2+1] - m_fCali[motor];
				}
			}
			else
			{
				sprintf_s( szBuf, "Axis %c goes to the HOME-scan", ucMotor );	// 到机械原点-扫描
				Message( szBuf );
				theApp.m_StageTMCM.LocaZero(motor);
				Sleep(100);
				theApp.m_StageTMCM.Reset(motor);
				m_fCurStep[motor] = 0.0;
				DiffCenter(motor);
				m_fDis[motor] = (float)(m_fCali[motor]*m_dEquivalent[motor]*m_paramDriveDir.nDir[motor]);
			}
		}
		else
		{
			sprintf_s( szBuf, "Axis %c goes to the HOME-mechanical", ucMotor );	// 到机械原点-机械
			Message( szBuf );
			theApp.m_StageTMCM.LocaZero(motor);
			Sleep(100);
			theApp.m_StageTMCM.Reset(motor);
			m_fCurStep[motor] = 0.0;
			sprintf_s( szBuf, "Kill Timer %c", ucMotor );
			Message( szBuf );
			KillTimer(motor +1);
			m_nTimer[motor] = 0;
			m_bToHome[motor] =FALSE;
			m_fGoal[motor] = 0.0;
			if( motor > 2 )
				GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText("0.00");
			else
				GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText("0.000");
// 			GetCurStep(motor);
// 			GetDlgItem(IDC_EDIT_X_CurStep +motor)->Invalidate();
			m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Elapse, NULL );
			m_bNum[motor] = TRUE;
			m_bIsMoving[motor] = FALSE;
			m_nPhaseInit[motor] = -1;
			//R轴左限位禁用
			if(motor == 3)
				theApp.m_StageTMCM.LimitStatus(13,motor,1);
			if(m_nProcess > 0  && !m_bIsMoving[0] && !m_bIsMoving[1])
				MoveSteps();
			// 写入配置文件
			SaveToINI( 1 );
			if(m_bSoftLimit)
			{
				m_paramZRes.fLimitNeg[motor] = m_paramZRes.fLimitNeg[motor] + m_fCali[motor];
				m_paramZRes.fLimitPos[motor] = m_paramZRes.fLimitPos[motor] + m_fCali[motor];
				m_factualLimit[motor*2] = m_factualLimitH[motor*2] + m_fCali[motor];
				m_factualLimit[motor*2+1] = m_factualLimitH[motor*2+1] + m_fCali[motor];
			}
		}
	}
}
/*
void CPanelStage::ToHome_T( unsigned char ucMotor, short motor )
{
	char szBuf[255];
	GetCurStep(motor);
	float* fState = theApp.m_StageTMCM.LimitStatusRead(motor);
	m_paramLimitDir.nL[motor] = *fState;
	m_paramLimitDir.nR[motor] = *(fState+1);

	if(!m_paramLimitDir.nL[motor] || !m_paramLimitDir.nR[motor])	// fState == 10 || fState == 11)
	{
		// 到负向极限
		sprintf_s( szBuf, "Axis %c goes to the negative limit: %.6f", ucMotor, m_fCurStep[motor]/m_dEquivalent[motor] );
		Message( szBuf );
		KillTimer(motor+1);
		m_nTimer[motor] = 0;
		
		if(m_bNum[motor])
		{
			GetCurStep(motor); 
			m_fDis[motor] = (float)(m_fCurStep[motor] + m_fCalibrationT*m_dEquivalent[motor]);
			Message("Get the target position");	// 获取目标位置
			
			theApp.m_StageTMCM.GoHome(motor, m_fDis[motor]);
			Sleep(50);
			m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Elapse, NULL );
		}
	}
	if( fabs(m_fCurStep[motor]-m_fDis[motor])< m_fDeadline[motor]*m_dEquivalent[motor] )
	{
		if( m_bOrigion[motor] )
		{
			if( fabs(m_fDis[motor] / m_dEquivalent[motor] )-fabs(m_fCali[motor])< m_fDeadline[motor] )
			{
				sprintf_s( szBuf, "Axis %c goes to the original", ucMotor );	// 到扫描原点
				Message( szBuf );
				KillTimer(motor+1);
				m_nTimer[motor] = 0;
				theApp.m_StageTMCM.Reset(motor);
				Sleep(500);
				theApp.m_StageTMCM.LocaZero(motor);
// 				Sleep(50);
// 				theApp.m_StageTMCM.LimitDisable(motor);
				m_fGoal[motor] = 0;
				//theApp.m_StageTMCM.GetCurPosition(motor);
				m_bOrigion[motor] =FALSE;
				m_bToHome[motor] =FALSE;
				m_bNum[motor] = TRUE;
				m_nTimer[motor] = SetTimer( motor+1, nStageTMCM_Elapse, NULL );
				GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText("0.0");
				if(m_nProcess > 0)
					MoveSteps();
			}
			else
			{
				sprintf_s( szBuf, "Axis %c goes to the HOME-scan", ucMotor );	// 到机械原点-扫描
				Message( szBuf );
				
				theApp.m_StageTMCM.Reset(motor);
				Sleep(500);
				theApp.m_StageTMCM.LocaZero(motor);
				Sleep(50);
				
				DiffCenter(motor);
				m_fDis[motor] = (float)(m_fCali[motor]*m_dEquivalent[motor]);
			}
		}
		else
		{
			sprintf_s( szBuf, "Axis %c goes to the HOME-mechanical", ucMotor );	// 到机械原点-机械
			Message( szBuf );
			
			theApp.m_StageTMCM.Reset(motor);
			Sleep(500);
			theApp.m_StageTMCM.LocaZero(motor);
			sprintf_s( szBuf, "Kill Timer %c", ucMotor );
// 			Sleep(50);
// 			theApp.m_StageTMCM.LimitDisable(motor);
			Message( szBuf );
			KillTimer(motor +1);
			m_nTimer[motor] = 0;
			m_bToHome[motor] =FALSE;
			m_fGoal[motor] = 0;
			
			GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText("0.00");
			
// 			GetCurStep(motor);
// 			GetDlgItem(IDC_EDIT_X_CurStep +motor)->Invalidate();
			m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Elapse, NULL );
			m_bNum[motor] = TRUE;
			m_bIsMoving[motor] = FALSE;
			if(m_nProcess > 0)
				MoveSteps();
			// 写入配置文件
			SaveToINI( 1 );
		}
	}
}
*/
void CPanelStage::NonToHome( unsigned char ucMotor, short motor )
{
	float lStep = m_fGoal[motor];
	//GetCurStep(motor);

	//float* fState = theApp.m_StageTMCM.StatusRead(motor);
	//m_paramLimitDir.fL[motor] = *fState;
	//m_paramLimitDir.fR[motor] = *(fState+1);
	if(m_paramStyle.nVersion <1)
	{
		if(motor > 3)
			m_paramLimitStatus.nL[motor] = 1;
	}
	//R轴一个限位，向左驱动查询左限位，向右驱动查询右限位
	if(motor == 3)
	{
		m_paramLimitStatus.nL[motor] = 1;
		m_paramLimitStatus.nR[motor] = 1;
	}
	
	char szBuf[255];
	if(!m_paramLimitStatus.nL[motor] || !m_paramLimitStatus.nR[motor])	// fState == 10 || fState == 11)
	{
		//GetCurStep(motor);
		if(!m_paramLimitStatus.nL[motor]  )	// == 左-负
		{
			KillTimer(motor+1);
			m_nTimer[motor] = 0;
			sprintf_s( szBuf, "Kill Timer %c", ucMotor );
			Message( szBuf );
			Sleep(1000);	// 延时1秒
			//m_bIsMoving[motor] = FALSE;
			//CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
			m_bBL[motor] = FALSE;
			SafeLimit(ucMotor, m_paramLimitStatus.nL[motor]);
		}
		else	// == 右-正
		{
			KillTimer(motor+1);
			m_nTimer[motor] = 0;
			sprintf_s( szBuf, "Kill Timer %c", ucMotor );
			Message( szBuf );
			Sleep(1000);	// 延时1秒
			//m_bIsMoving[motor] = FALSE;
			//CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
			m_bBL[motor] = FALSE;
			SafeLimit(ucMotor, m_paramLimitStatus.nR[motor]);
		}
	}
	if(m_bBL[motor])
	{
		//m_fBLCount[motor] = theApp.m_StageTMCM.CoherenceCheck_Backlash(ucMotor,lStep,FALSE);
		if( fabs(m_fBLCount[motor] -m_fCurStep[motor]) <= m_paramBl.fCount[motor] /3. )
		{
			 theApp.m_StageTMCM.MoveLimit(motor, lStep);
			 m_bBL[motor] = FALSE;
		}
	}
	else
	{
		if( fabs(lStep -m_fCurStep[motor]) <= m_fDeadline[motor]*m_dEquivalent[motor] )
		{
			//GetCurStep(motor);
			//m_fGoal[motor] = theApp.m_StageTMCM.GetCurPosition(motor)*m_paramDriveDir.nDir[motor];
			theApp.m_StageTMCM.Move(ucMotor,m_fCurStep[motor],m_bRelative);
			m_fGoal[motor] = m_fCurStep[motor]*m_paramDriveDir.nDir[motor];
			KillTimer(motor+1);
			m_nTimer[motor] = 0;
			m_bEqual[motor] = FALSE;
			//Sleep(500);
			if(m_bHighPrecCount)
			{
				if( motor < 3)
					sprintf_s( szBuf, "%.3f, %.5f", m_fGoal[motor] /m_dEquivalent[motor], m_fGoal[motor] /m_dEquivalent[motor] );
				else
					sprintf_s( szBuf, "%.2f, %.5f", m_fGoal[motor] /m_dEquivalent[motor], m_fGoal[motor] /m_dEquivalent[motor] );
			}
			else
			{
				if( motor < 3 )
					sprintf_s( szBuf, "%.3f", m_fGoal[motor] /m_dEquivalent[motor] );
				else
					sprintf_s( szBuf, "%.2f", m_fGoal[motor] /m_dEquivalent[motor] );
			}

			GetDlgItem(IDC_EDIT_X_CurStep +motor)->SetWindowText(szBuf);
			m_fCurStep[motor] = m_fGoal[motor]/m_paramDriveDir.nDir[motor];

			sprintf_s( szBuf, "KillTimer %c", ucMotor );
			Message( szBuf );
			//Sleep(500);	// 延时1秒
			m_bIsMoving[motor] = FALSE;
			CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
			//R轴断电
			switch(motor)
			{
			case 3:
				theApp.m_StageTMCM.PowerOfforOn(1,3);
				break;
			}

			if(m_nProcess < 0)
				MoveSteps();

			GetDlgItem(IDC_EDIT_X_CurStep +motor)->Invalidate();	
		}
	}
}

void CPanelStage::Timer_Axis( short motor )
{
	unsigned char ucMotor = SerialNum( motor );

	//接地
	if(m_paramLimitStatus.nGL>0)	//== 12)
		Grounding();

	if(m_bToHome[motor])
	{
		switch( motor )
		{
		case	0:
		case	1:
			ToHome_XY( ucMotor, motor );
			break;
		case	2:
		case	3:
		case	4:
			ToHome_ZRT( ucMotor, motor );
			break;
		/*case	4:
			ToHome_T( ucMotor, motor );
			break;*/
		}
	}
	else
		NonToHome( ucMotor, motor );
}

void CPanelStage::Timer_Stop( short motor )
{
	unsigned char ucMotor = SerialNum( motor );
	m_fPreCurpos = m_fCurStep[motor];

	//float* fState = theApp.m_StageTMCM.StatusRead(motor);
	//m_paramLimitDir.fL[motor] = *fState;
	//m_paramLimitDir.fR[motor] = *(fState+1);
	if(motor == 3)
	{
		m_paramLimitStatus.nL[motor] = 1;
		m_paramLimitStatus.nR[motor] = 1;
	}

	//GetCurStep(motor);

	char szBuf[255];
	if(!m_paramLimitStatus.nL[motor] || !m_paramLimitStatus.nR[motor])	// fState == 10 || fState == 11)
	{
		//GetCurStep(motor);
		if(!m_paramLimitStatus.nL[motor]  )	// == 左-负
		{
			m_bIsMoving[motor] = FALSE;
			CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
			sprintf_s( szBuf, "Kill Timer %c", ucMotor );
			Message( szBuf );
			KillTimer(motor+6);
			m_nTimer[motor] = 0;
			m_bBL[motor] = FALSE;
			SafeLimit(ucMotor, m_paramLimitStatus.nL[motor]);
		}
		else	// == 右-正
		{
			m_bIsMoving[motor] = FALSE;
			CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
			sprintf_s( szBuf, "Kill Timer %c", ucMotor );
			Message( szBuf );
			KillTimer(motor+6);
			m_nTimer[motor] = 0;
			m_bBL[motor] = FALSE;
			SafeLimit(ucMotor, m_paramLimitStatus.nR[motor]);
		}
	}
	else
	{
		if( ! NotZero(m_fPreCurpos-m_fCurStep[motor]))
		{
			if(m_bHighPrecCount)
			{
				if( motor < 3)
					sprintf_s( szBuf, "%.3f, %.5f", m_fCurStep[motor] /m_dEquivalent[motor]*m_paramDriveDir.nDir[motor], m_fCurStep[motor] /m_dEquivalent[motor]*m_paramDriveDir.nDir[motor] );
				/*else if( motor <4)
				{
					if(m_fCurStep[motor]/m_dEquivalent[motor] > 179)
						sprintf_s( szBuf, "%.2f, %.5f", (m_fCurStep[motor]/m_dEquivalent[motor] - 360)*m_paramDriveDir.nDir[motor], (m_fCurStep[motor]/m_dEquivalent[motor] - 360)*m_paramDriveDir.nDir[motor]);
					else if(m_fCurStep[motor]/m_dEquivalent[motor] < -179)
						sprintf_s( szBuf, "%.2f, %.5f", (m_fCurStep[motor]/m_dEquivalent[motor] + 360)*m_paramDriveDir.nDir[motor], (m_fCurStep[motor]/m_dEquivalent[motor] + 360)*m_paramDriveDir.nDir[motor]);
					else if(m_fCurStep[motor]/m_dEquivalent[motor] > 359)
						sprintf_s( szBuf, "%d, %d", (int)(m_fCurStep[motor]/m_dEquivalent[motor])% 360*m_paramDriveDir.nDir[motor], (int)(m_fCurStep[motor]/m_dEquivalent[motor])% 360*m_paramDriveDir.nDir[motor]);
					else if(m_fCurStep[motor]/m_dEquivalent[motor] < -359)
						sprintf_s( szBuf, "%d, %d", (int)(m_fCurStep[motor]/m_dEquivalent[motor])% (-360)*m_paramDriveDir.nDir[motor], (int)(m_fCurStep[motor]/m_dEquivalent[motor])% (-360)*m_paramDriveDir.nDir[motor]);
					else
						sprintf_s( szBuf, "%.2f, %.5f", m_fCurStep[motor] /m_dEquivalent[motor]*m_paramDriveDir.nDir[motor], m_fCurStep[motor] /m_dEquivalent[motor]*m_paramDriveDir.nDir[motor] );
				}*/
				else
					sprintf_s( szBuf, "%.2f, %.5f", m_fCurStep[motor] /m_dEquivalent[motor]*m_paramDriveDir.nDir[motor], m_fCurStep[motor] /m_dEquivalent[motor]*m_paramDriveDir.nDir[motor] );
			}
			else
			{
				if( motor < 3 )
					sprintf_s( szBuf, "%.3f", m_fCurStep[motor] /m_dEquivalent[motor]*m_paramDriveDir.nDir[motor] );
				/*else if( motor <4)
				{
					if(m_fCurStep[motor]/m_dEquivalent[motor] > 179)
						sprintf_s( szBuf, "%.2f", (m_fCurStep[motor]/m_dEquivalent[motor] - 360)*m_paramDriveDir.nDir[motor]);
					else if(m_fCurStep[motor]/m_dEquivalent[motor] < -179)
						sprintf_s( szBuf, "%.2f", (m_fCurStep[motor]/m_dEquivalent[motor] + 360)*m_paramDriveDir.nDir[motor]);
					else if(m_fCurStep[motor]/m_dEquivalent[motor] > 359)
						sprintf_s( szBuf, "%d", (int)(m_fCurStep[motor]/m_dEquivalent[motor])% 360*m_paramDriveDir.nDir[motor]);
					else if(m_fCurStep[motor]/m_dEquivalent[motor] < -359)
						sprintf_s( szBuf, "%d", (int)(m_fCurStep[motor]/m_dEquivalent[motor])% (-360)*m_paramDriveDir.nDir[motor]);
					else
						sprintf_s( szBuf, "%.2f", m_fCurStep[motor] /m_dEquivalent[motor]*m_paramDriveDir.nDir[motor] );
				}*/
				else
					sprintf_s( szBuf, "%.2f", m_fCurStep[motor] /m_dEquivalent[motor]*m_paramDriveDir.nDir[motor] );
			}
			GetDlgItem(IDC_EDIT_X_CurStep +motor)->SetWindowText(szBuf);

			//m_fGoal[motor] = theApp.m_StageTMCM.GetCurPosition(motor);
			m_fGoal[motor] = m_fCurStep[motor];
			CString str;
			if( motor < 3 )
				str.Format( "%.3f", m_fGoal[motor]/m_dEquivalent[motor]*m_paramDriveDir.nDir[motor] );
			else
				str.Format( "%.2f", m_fGoal[motor]/m_dEquivalent[motor]*m_paramDriveDir.nDir[motor] );
			GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText(str);

			sprintf_s( szBuf, "KillTimer %c", ucMotor );
			Message( szBuf );
			KillTimer(motor+6);
			m_nTimer[motor] = 0;
			m_bIsMoving[motor] = FALSE;
			CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
			//R轴断电
			switch(motor)
			{
			case 3:
				theApp.m_StageTMCM.PowerOfforOn(1,3);
				break;
			}
			GetDlgItem(IDC_EDIT_X_CurStep +motor)->Invalidate();	
		}
	}
}

void CPanelStage::Timer_KillTimer( UINT nIDEvent )
{
	short motor, nTimerIndex;
	switch( nIDEvent )
	{
	case	11:
	case	12:
		motor		= 0;
		nTimerIndex = 5;
		break;
	case	21:
	case	22:
		motor		= 1;
		nTimerIndex = 6;
		break;
	case	31:
	case	32:
		motor		= 2;
		nTimerIndex = 7;
		break;
	case	41:
	case	42:
		motor		= 3;
		nTimerIndex = 8;
		break;
	case	51:
	case	52:
		motor		= 4;
		nTimerIndex = 9;
		break;
	}
	KillTimer( nIDEvent );
	m_nTimer[nTimerIndex] = 0;
	unsigned char ucAxis = SerialNum( motor );
	char szBuf[255];
	sprintf_s( szBuf, "KillTimer-%c", ucAxis );
	Message( szBuf );
}

void CPanelStage::Timer_Tracklocus( UINT nIDEvent, short motor )
{
	if( m_bTrackState[motor] )
	{
		if( (nIDEvent % 2) != 0 )	// 11/21/31/41/51
		{
			if(!m_bDirection[motor])
				Timer_KillTimer( nIDEvent );
			SetTracklocusAxis( motor, TRUE);
		}
		else						// 12/22/32/42/52
		{
			if(m_bDirection[motor])
				Timer_KillTimer( nIDEvent );
			SetTracklocusAxis( motor, FALSE);
		}
	}
	else
	{
		Timer_KillTimer( nIDEvent );
		SetTracklocusAxis( motor, FALSE );
	}
}

void CPanelStage::StartPos(short motor)
{
	//float* fState = theApp.m_StageTMCM.StatusRead(motor);
	//m_paramLimitDir.fL[motor] = *fState;
//	m_paramLimitDir.fR[motor] = *(fState+1);
	if(motor == 3)
	{
		m_paramLimitStatus.nL[motor] = 1;
		m_paramLimitStatus.nR[motor] = 1;
	}

	if(!m_paramLimitStatus.nL[motor] || !m_paramLimitStatus.nR[motor])	// fState == 10 || fState == 11)
		StartPos_LimitDir( motor);
	//fState = theApp.m_StageTMCM.StatusRead(motor);
	//m_paramLimitDir.fL[motor] = *fState;
	//m_paramLimitDir.fR[motor] = *(fState+1);
	theApp.m_StageTMCM.LimitStatusRead(motor);
	if(motor == 3)
	{
		m_paramLimitStatus.nL[motor] = 1;
		m_paramLimitStatus.nR[motor] = 1;
	}
	if(!m_paramLimitStatus.nL[motor] || !m_paramLimitStatus.nR[motor])	// fState == 10 || fState == 11)
		StartPos(motor);

	Sleep(100);
	//float fStep = theApp.m_StageTMCM.CurrPosO(motor);
	float fStep = m_fCurStep[motor];
	CString str;
	str = DisplayOutput(motor ,fStep,fStep);

	GetDlgItem(IDC_EDIT_X_CurStep +motor)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText(str);
	//m_fCurStep[motor] = fStep;
}

void CPanelStage::StartPos_LimitDir( short motor)
{
	float fLimitDir = 0.0f;
	if( m_paramLimitStatus.nL[motor])			// == 10
		fLimitDir = 1;
	else
		fLimitDir = -1;

	unsigned char ucMotor = SerialNum(motor);
	char szBuf[255] = {0};
	if( fLimitDir > 0 )			// 1
	{
		sprintf_s( szBuf, "%c %s", ucMotor, m_strMsg[6] );
		if( MessageBox( szBuf, m_strMsg[0], MB_YESNO ) == IDYES )
			Message("It is in the positive limit position");	// 处于正向极限位置
	}
	else if( fLimitDir < 0 )	// -1
	{
		sprintf_s( szBuf, "%c %s", ucMotor, m_strMsg[7] );
		if( MessageBox( szBuf, m_strMsg[0], MB_YESNO ) == IDYES )
			Message("It is in the negative limit position");	// 处于负向极限位置
	}
//	theApp.m_StageTMCM.LimRelease(1,ucMotor);
	Sleep(50);
	m_fGoal[motor] = theApp.m_StageTMCM.BackOff(ucMotor,fLimitDir);
	m_fCurStep[motor] = m_fGoal[motor];
//	theApp.m_StageTMCM.Enable(motor);
	Sleep(50);
	m_bIsMoving[motor] = TRUE;
	CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
	Sleep(50);
//	theApp.m_StageTMCM.LimRelease(0,ucMotor);

	CString str;
	if( motor < 3 )
		str.Format( "%.3f", m_fGoal[motor]/m_dEquivalent[motor] *m_paramDriveDir.nDir[motor]);
	else
		str.Format( "%.2f", m_fGoal[motor]/m_dEquivalent[motor] *m_paramDriveDir.nDir[motor]);
	GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText(str);
	m_nTimer[motor] = SetTimer( motor + 1, nStageTMCM_Elapse, NULL );
}

void CPanelStage::FindIndex()
{
	short motor = 5;
	int i;
	//float fGState = theApp.m_StageTMCM.GroundStatusRead();

	//接地
	if(m_paramLimitStatus.nGL>0)	//== 12)
		Grounding();

	Sleep(100);
	// 先回机械原点，再回扫描中心，并定义其为用户原点
	if( MessageBox( m_strMsg[1], m_strMsg[0], MB_OKCANCEL ) == IDOK )
	{
		// 先回机械原点
		Message("Start FindIndex");
		SetUserOrigin();
	}
	else
	{
		//R轴限位禁用
		theApp.m_StageTMCM.LimitStatus(13,3,1);
		for( i=0; i<5; i++ )
		{
			if( m_bCommunicationFault[i] && m_bMotorFault[i]  )	// 调试用
				StartPos(i);
		}
	}
	if(!QueryMoving())
	{
		for( i=0; i<5; i++ )
		{
			m_fGoal[i] = m_fCurStep[i];
			CStageTMCMManager::Instance().m_bIsMoving[i] = FALSE;
		}
	}
	//R轴断电
	theApp.m_StageTMCM.PowerOfforOn(1,3);
}

void CPanelStage::Safe_Axis( unsigned char ucMotor, int fState, int nType )
{
	short motor = NumSerial( ucMotor );

	if( nType < 1 )
		Sleep(50);
	if( nType < 1 )
	{
		// SafeLimit: 触碰到极限
		if(!m_paramLimitStatus.nL[motor])		//== 11)
			m_fGoal[motor] = theApp.m_StageTMCM.BackOff(ucMotor, -1);
		else if(!m_paramLimitStatus.nR[motor])
			m_fGoal[motor] = theApp.m_StageTMCM.BackOff(ucMotor, 1);
	}

//	theApp.m_StageTMCM.Enable(motor);
	m_bIsMoving[motor] = TRUE;
	CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
	if( nType < 1 )
		Sleep(1000);
	else
		Sleep(100);
//	theApp.m_StageTMCM.LimRelease(0,ucMotor);
	CString str;
	if( motor < 3 )
		str.Format( "%.3f", m_fGoal[motor]/m_dEquivalent[motor]*m_paramDriveDir.nDir[motor] );
	/*else if( motor < 4 )
	{
		if(m_fGoal[motor]/m_dEquivalent[motor] > 179)
			str.Format( "%.2f, %.5f", (m_fGoal[motor]/m_dEquivalent[motor] - 360) *m_paramDriveDir.nDir[motor]);
		else if(m_fGoal[motor]/m_dEquivalent[motor] < -179)
			str.Format( "%.2f, %.5f", (m_fGoal[motor]/m_dEquivalent[motor] + 360)*m_paramDriveDir.nDir[motor]);
		else if(m_fGoal[motor]/m_dEquivalent[motor] > 359)
			str.Format( "%d", int(m_fGoal[motor]/m_dEquivalent[motor])% 360 *m_paramDriveDir.nDir[motor]);
		else if(m_fGoal[motor]/m_dEquivalent[motor] < -359)
			str.Format( "%d", int(m_fGoal[motor]/m_dEquivalent[motor])% (-360) *m_paramDriveDir.nDir[motor]);
		else
			str.Format( "%.2f", m_fGoal[motor]/m_dEquivalent[motor]*m_paramDriveDir.nDir[motor] );
	}*/
	else
		str.Format( "%.2f", m_fGoal[motor]/m_dEquivalent[motor]*m_paramDriveDir.nDir[motor] );
	GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText(str);
	//GetCurStep(motor);
	m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Elapse, NULL );
}

void CPanelStage::SafeLimit(unsigned char ucMotor, int fState)
{
	char szBuf[255] = {0};
	sprintf_s( szBuf, "%c %s", ucMotor, m_strMsg[5] );
	// 触碰到极限，是否回退
	if( MessageBox(szBuf, m_strMsg[0], MB_YESNO ) != IDYES )
		return;
	Safe_Axis( ucMotor, fState, 0 );
}

void CPanelStage::SetTrackParamAxis( short motor, int nSpeed )
{
	// 跟踪：设置速度，并进行运动
	// 根据速度值正负判断运动方向
	// 如果与当前运动方向不一致，要停止运动、设置速度(绝对值)、开始运动
	// 如果与当前运动方向一致，只需设置速度(绝对值)

	int nPositive = abs(nSpeed);
	nPositive = (nPositive / 200) *5;	// 将速度分档
	if( nPositive > lSpeedTrackMax )
		nPositive = lSpeedTrackMax;

	if( motor < 1 )	// X
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
	unsigned char ucMotor = SerialNum( motor );
	char szBuf[255];
	if( abs(nSpeed) < 5 || nPositive == 0 )	// 与跟踪点之间差值较小，停止运动
	{
		//theApp.m_StageTMCM.close(motor);
		theApp.m_StageTMCM.CurrPosR(motor);
		sprintf_s( szBuf, "Stop motion in derection %c", ucMotor );	// X方向停止运动
		Message( szBuf );
	}
	else
	{
		BOOL bIsSame = FALSE;
		if( fabs(m_paramSpeed.fSpeedPos[motor] -nPositive/2.) < FLT_EPSILON  )
			bIsSame = TRUE;
		m_paramSpeed.fSpeedPos[motor] = (float)(nPositive/2.);
		if( motor < 1 )	// X
		{
			if( !m_bXdir && nSpeed < 0 )
			{
				sprintf_s( szBuf, "%c motion in original direction", ucMotor );	// X 原方向运动
				Message( szBuf );
			}
			else
			{
				sprintf_s( szBuf, "%c motion in the opposite direction", ucMotor );	// X 反方向运动
				Message( szBuf );
			}
		}
		else
		{
			if( m_bYdir && nSpeed < 0 )
			{
				sprintf_s( szBuf, "%c motion in original direction", ucMotor );	// X 原方向运动
				Message( szBuf );
			}
			else
			{
				sprintf_s( szBuf, "%c motion in the opposite direction", ucMotor );	// X 反方向运动
				Message( szBuf );
			}
		}
		if( !bIsSame )
		{
			theApp.m_StageTMCM.SetSpeed( ucMotor, m_paramSpeed.fSpeedPos[motor] );
			sprintf_s( szBuf, "Set Speed %c = %f", ucMotor, m_paramSpeed.fSpeedPos[motor] );
			Message( szBuf );
		}
		float fSteps;
		if( nSpeed < 0 )
		{
			if( motor < 1 )	// X
				fSteps =-512000.0f;
			else
				fSteps = 512000.0f;
		}
		else
		{
			if( motor < 1 )	// X
				fSteps = 512000.0f;
			else
				fSteps = -512000.0f;
		}
		fSteps = fSteps *  m_paramDriveDir.nDir[motor];
		theApp.m_StageTMCM.MoveDir(motor, fSteps, FALSE);
//		m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Elapse, NULL );
		sprintf_s( szBuf, "%c start moving", ucMotor );	// X 开始运动
		Message( szBuf );
	}
	m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Elapse, NULL );
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
	unsigned char ucMotor;
	for(int i=0;i<2;i++)
	{
		ucMotor = SerialNum(i);
		theApp.m_StageTMCM.SetSpeed(ucMotor, m_paramSpeed.fSpeedPos[i]);
	}
}

void CPanelStage::StopTrackAxis1( short motor )
{
	unsigned char ucAxis = SerialNum( motor );
	char szBuf[255];
	m_bBL[motor]=TRUE;
	m_bIsMoving[motor] = TRUE;
	CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
	if( m_paramBl.nDir[motor] < 0 )
		m_fBLCount[motor] = m_fCurStep[motor] - m_paramBl.fCount[motor];
	else
		m_fBLCount[motor] = m_fCurStep[motor] + m_paramBl.fCount[motor];
	theApp.m_StageTMCM.MoveDir(motor, m_fBLCount[motor], FALSE);

	Sleep(40);
	sprintf_s( szBuf, "%.4f", m_fGoal[motor]/m_dEquivalent[motor] * m_paramDriveDir.nDir[motor] );
	GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText( szBuf );
	if( m_paramBl.nDir[motor] < 0 )
		sprintf_s( szBuf, "Modified %c position display with - backlash direction", ucAxis );	// 带-间隙方向，修改X位置显示
	else
		sprintf_s( szBuf, "Modified %c position display with + backlash direction", ucAxis );	// 带+间隙方向，修改%c位置显示
	Message( szBuf );
}

void CPanelStage::StopTrackAxis2( short motor )
{
	unsigned char ucAxis = SerialNum( motor );
	char szBuf[255];
	theApp.m_StageTMCM.MoveDir(motor, m_fCurStep[motor], FALSE);
	//theApp.m_StageTMCM.Enable(motor);
	if( m_paramBl.nDir[motor] > 0 )	// X轴非间隙方向
		sprintf_s( szBuf, "Axis %c non-backlash direction 1", ucAxis );
	else
		sprintf_s( szBuf, "Axis %c non-backlash direction 2", ucAxis );
	Message( szBuf );
}

void CPanelStage::StopTrack()
{
	int i=0; 
	char szBuf[255];
	unsigned char ucMotor;
	for( i=0; i<2; i++ )
	{
		GetCurStep(i);
		m_fGoal[i] = m_fCurStep[i];
		ucMotor = SerialNum( i );
		sprintf_s( szBuf, "Get the current position of axis %c", ucMotor );	// 获取当前%c轴位置
		Message( szBuf );
	}
	if(m_bBlTrack)
	{
		if(  (!m_bXdir &&  m_paramDriveDir.nDir[0]>0) || (m_bXdir && m_paramDriveDir.nDir[0]<0) )
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
		
		if((!m_bYdir &&  m_paramDriveDir.nDir[1]>0) || (m_bYdir &&  m_paramDriveDir.nDir[1]<0) )
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
			ucMotor = SerialNum( i );
			theApp.m_StageTMCM.MoveDir(i, m_fCurStep[i], FALSE);
			//theApp.m_StageTMCM.Enable(i);
			Sleep(40);
		}
	}

//	Message("不带间隙，X/Y位置显示");
	Message("Without backlash, display position of X/Y");
	for( i=0; i<2; i++ )
	{
		sprintf_s( szBuf, "%.4f", m_fGoal[i]/m_dEquivalent[i]* m_paramDriveDir.nDir[i] );
		GetDlgItem(IDC_EDIT_X_UserDisp +i)->SetWindowText( szBuf );
	}
	RestoreSpeed();
}

// 摇杆
void CPanelStage::SetTracklocusAxis1( short motor, int nSpeed )
{
	// X/R/T
	nSpeed -= 512;
	nSpeed = nSpeed * m_paramDriveDir.nDir[motor];
	m_bTrackState[motor] = TRUE;
	unsigned char ucAxis = SerialNum( motor );
	char szBuf[255];
	if( abs(nSpeed) < 5  )	// 与跟踪点之间差值较小，停止运动
	{
		m_bTrackState[motor] = FALSE;
		sprintf_s( szBuf, "Offset of %c is too small, do not move", ucAxis );	// 移动量过小，不移动
		Message( szBuf );
	}
	else
	{
// 		m_StageTMCM.SetSpeed( ucAxis, m_paramSpeed.fSpeedTrack[motor] );
		m_bIsMoving[motor] = TRUE;
		CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
		if(nSpeed > 0 && abs(nSpeed) > 5)
		{
			m_bDirection[motor] = TRUE;
// 			m_StageTMCM.MoveDir(ucAxis, 50, FALSE);
			KillTimer( (motor +1)*10 +2 );
			m_nTimer[motor +5] = SetTimer( (motor +1)*10 +1, nStageTMCM_Track, NULL );
		}
		if(nSpeed < 0 && abs(nSpeed) > 5)
		{
			m_bDirection[motor] = FALSE;
// 			m_StageTMCM.MoveDir(ucAxis, -50, FALSE);
			KillTimer( (motor +1) *10 +1 );
			m_nTimer[motor +5] = SetTimer( (motor +1)*10 +2, nStageTMCM_Track, NULL );
		}
		theApp.m_StageTMCM.SetSpeed( ucAxis, m_paramSpeed.fSpeedTrack[motor] );
	}
}

void CPanelStage::SetTracklocusAxis2( short motor, int nSpeed )
{
	// Y/Z
	nSpeed -= 512;
	nSpeed = nSpeed * m_paramDriveDir.nDir[motor];
	m_bTrackState[motor] = TRUE;
	unsigned char ucAxis = SerialNum( motor );
	char szBuf[255];
	if( abs(nSpeed) < 5  )	// 与跟踪点之间差值较小，停止运动
	{
		m_bTrackState[motor] = FALSE;
		sprintf_s( szBuf, "Offset of %c is too small, do not move", ucAxis );	// 移动量过小，不移动
		Message( szBuf );
	}
	else
	{
// 		m_StageTMCM.SetSpeed( ucAxis, m_paramSpeed.fSpeedTrack[motor] );
		m_bIsMoving[motor] = TRUE;
		CStageTMCMManager::Instance().m_bIsMoving[motor] = m_bIsMoving[motor];
		if(nSpeed < 0 && abs(nSpeed) > 5)
		{
			m_bDirection[motor] = TRUE;
// 			m_StageTMCM.MoveDir(ucAxis, 50, FALSE);
			KillTimer( (motor +1)*10 +2 );
			m_nTimer[motor +5] = SetTimer( (motor +1)*10 +1, nStageTMCM_Track, NULL );
		}
		if(nSpeed > 0 && abs(nSpeed) > 5)
		{
			m_bDirection[motor] = FALSE;
// 			m_StageTMCM.MoveDir(ucAxis, -50, FALSE);
			KillTimer( (motor +1) *10 +1 );
			m_nTimer[motor +5] = SetTimer( (motor +1)*10 +2, nStageTMCM_Track, NULL );
		}
		theApp.m_StageTMCM.SetSpeed( ucAxis, m_paramSpeed.fSpeedTrack[motor] );
	}
}

void CPanelStage::SetTracklocus1( int nSpeedX, int nSpeedY, int nSpeedR )
{
	if( !m_bIsTMCMReady )
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
	if( !m_bIsTMCMReady )
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

void CPanelStage::SetTracklocusAxis( short motor, BOOL bDirect )
{
	CString str;
	unsigned char ucMotor = SerialNum( motor );
	if( m_bTrackState[motor] )
	{
		//GetCurStep(motor);
		////不同放大倍数，移动步距不同
		//CalculateStageUScale(0,m_sizeReso.cy /4,m_fTrackStepAxis);
		/*char sz[512] = {0};
		ZeroMemory(sz,512);
		sprintf_s(sz,512,"time2 = %ld\r\n",t2-t1 );
		OutputDebugString(sz);*/
		//方案一
		if(motor < 1)
			//不同放大倍数，移动步距不同
			CalculateStageUScale(0,m_sizeReso.cx * m_fRockerStep[0],m_fTrackStepAxis);
		else if(motor < 2)
			//不同放大倍数，移动步距不同
			CalculateStageUScale(0,m_sizeReso.cy * m_fRockerStep[1],m_fTrackStepAxis);
		else
			m_fTrackStepAxis = m_fRockerStep[motor] * m_dEquivalent[motor];

		if(bDirect)
			m_fGoal[motor] = (float)(m_fCurStep[motor] + m_fTrackStepAxis);
		else
			m_fGoal[motor] = (float)(m_fCurStep[motor] -m_fTrackStepAxis);

		theApp.m_StageTMCM.MoveDir(motor, m_fGoal[motor], FALSE);
		char szBuf[255];
		sprintf_s( szBuf, "m_fCurStep[motor] = %.3f,m_fGoal[motor] = %.3f,m_fTrackStepAxis = %.3f",m_fCurStep[motor], m_fGoal[motor],m_fTrackStepAxis);
		Message( szBuf );
		//方案二
		/*if(bDirect)
			m_fGoal[motor] = m_fGoal[motor] + m_fTrackStep;
		else
			m_fGoal[motor] = m_fGoal[motor] - m_fTrackStep;
		unsigned char ucMotor = SerialNum( motor );
		m_StageTMCM.MoveDir(ucMotor, m_fGoal[motor], FALSE);*/
	}
	else
	{
		m_nTimer[motor] = SetTimer( motor +1, nStageTMCM_Track, NULL );
		//获取当前位置并下发
		m_fGoal[motor] = m_fCurStep[motor];
		theApp.m_StageTMCM.MoveDir(motor, m_fGoal[motor], FALSE);
		////高低电平交换
		//theApp.m_StageTMCM.LimitLevel(25,motor,1);
		//theApp.m_StageTMCM.LimitLevel(24,motor,1);
		if( motor < 2 )
			str.Format( "%.4f", m_fGoal[motor]/m_dEquivalent[motor] *m_paramDriveDir.nDir[motor] );
		else
			str.Format( "%.2f", m_fGoal[motor]/m_dEquivalent[motor] *m_paramDriveDir.nDir[motor] );
		GetDlgItem(IDC_EDIT_X_UserDisp +motor)->SetWindowText(str);
		/*for(int i =0;i<5;i++)
		{
			if(i != motor)
			{
				theApp.m_StageTMCM.LimitLevel(25,motor,1);
				theApp.m_StageTMCM.LimitLevel(24,motor,1);
			}
		}*/
	}
}

void CPanelStage::RestoreSpeedlocus()
{
	unsigned char ucAxis;
	for(int i=0;i<5;i++)
	{
		ucAxis = SerialNum(i);
		theApp.m_StageTMCM.SetSpeed(ucAxis, m_paramSpeed.fSpeedPos[i]);
	}
}

void CPanelStage::StopTracklocus()
{
	//获取当前位置，并使计数、电机都停止-高低电平交换
	for(int i =0;i<5;i++)
	{
		/*theApp.m_StageTMCM.LimitLevel(25,i,0);
		theApp.m_StageTMCM.LimitLevel(24,i,0);*/
		m_bTrackState[i] =FALSE;
	}
	RestoreSpeedlocus();
}

void CPanelStage::OnBnClickedCheckAdvance()
{
	// TODO: 在此添加控件通知处理程序代码
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
		GetDlgItem(IDC_BUTTON_HomeX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_HomeY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_HomeZ)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_BUTTON_HomeR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_HomeT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ZeroX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ZeroY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ZeroZ)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_BUTTON_ZeroR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ZeroT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_SetUserOrigin)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_LimitParam)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_Home)->ShowWindow(SW_SHOW);
		m_bHighPrecCount = TRUE;
		SetWindowPos(&CWnd::wndTop, 0, 0,
			m_rcWindow.Width(), m_rcWindow.Height(), SWP_NOMOVE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_Advance)->SetWindowText(">>");
		GetDlgItem(IDC_BUTTON_HomeX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HomeY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HomeZ)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_BUTTON_HomeR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HomeT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ZeroX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ZeroY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ZeroZ)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_BUTTON_ZeroR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ZeroT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_SetUserOrigin)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_LimitParam)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_Home)->ShowWindow(SW_HIDE);
		m_bHighPrecCount = FALSE;
		SetWindowPos(&CWnd::wndTop, 0, 0, m_rcWindow.Width(),
			m_rcWindow.Height() -m_rcReport.Height() -20, SWP_NOMOVE);
	}
}

void CPanelStage::AutoMove(float fstep, short motor )
{
	float fMoveStep = fstep * m_dEquivalent[motor];
	if( m_bRelative )
	{
		if(m_paramDriveDir.nDir[motor] > 0)
			fMoveStep += m_fCurStep[motor];
		else
			fMoveStep -= m_fCurStep[motor];
	}
	/*if(motor ==3)
	{
		fMoveStep = RDriveConversion(m_fCurStep[motor],fMoveStep);
	}*/
	fMoveStep = fMoveStep * m_paramDriveDir.nDir[motor];
	//判断Z轴位置是否满足T轴运行条件
	/*if(motor > 3)
	{
		m_fZHeight = m_fCalibrationZ + m_fCurStep[2] / m_dEquivalent[2] * m_paramDriveDir.nDir[2];
		if(m_fZHeight < 6)
		{
			char szBuf[255];
			sprintf_s( szBuf, "The Z-axis height is too low");	
			Message(szBuf);
			sprintf_s( szBuf, "%s",  m_strMsg[13] );
			MessageBox( szBuf, m_strMsg[0], MB_OK | MB_ICONERROR );
		}
		else
			MoveAxis( motor, fMoveStep );
	}
	else
		MoveAxis( motor, fMoveStep );*/
	//机械结构修改，Z和T无约束
	MoveAxis( motor, fMoveStep );
}
