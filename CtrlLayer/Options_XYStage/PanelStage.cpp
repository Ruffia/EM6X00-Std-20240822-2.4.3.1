// PanelStage.cpp : implementation file
//

#include "stdafx.h"
#include "Options_XYStage.h"
#include "PanelStage.h"
#include "DlgProgress.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const	CString g_csRegEntry_nAutoStage						= _T("hasAutoStage");
const	unsigned	short	g_nAutoStageController			= 1;

const UINT nAutoStageXY_FI11	= 131;
const UINT nAutoStageXY_FI12	= 132;
const UINT nAutoStageXY_FI21	= 133;
const UINT nAutoStageXY_FI22	= 134;
const UINT nAutoStageXY_FI31	= 135;
const UINT nAutoStageXY_FI32	= 136;
const UINT nAutoStageXY_FI4		= 137;
const UINT nAutoStageTrack		= 138;
const UINT nAutoStageClearXY	= 150;
const UINT nAutoStageXY_FIElapse= 300;
#define	STAGE_TRACE_POINT_RADIUS	4

/////////////////////////////////////////////////////////////////////////////
// CPanelStage dialog


CPanelStage::CPanelStage(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelStage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelStage)
	m_dMag = 100.0;
	//}}AFX_DATA_INIT
	m_hIconStage1	= NULL;
	m_hIconStage2	= NULL;
	m_hIconStage3	= NULL;
	m_hIconStage4	= NULL;

	m_sizeReso.cx	= 1024;
	m_sizeReso.cy	= 768;

	m_plistStagePos			= new ListStagePos;
	m_pAutoProgressDlg		= NULL;

	m_Stage.nHasAutoStage	= 0;
	m_Stage.xy.bReady		= FALSE;
	m_Stage.xy.ptCurPosAb.x	= 0;
	m_Stage.xy.ptCurPosAb.y	= 0;

	m_bCatch				= FALSE;
	// 12.06.21 �Զ�̨
	m_bStageOrigin			= FALSE;
	m_bStageOriginHitTest	= FALSE;
	m_bStageTrack			= FALSE;
	m_bShiftDown			= FALSE;
	m_ptStagePointParam.x	= m_sizeReso.cx / 2;
	m_ptStagePointParam.y	= m_sizeReso.cy / 2;
	m_ptStageTrackParam.x	= m_sizeReso.cx / 2;
	m_ptStageTrackParam.y	= m_sizeReso.cy / 2;
	m_csUScaleUnit = "";
	m_dfMobileUScaleParameter = -1;

}

CPanelStage::~CPanelStage()
{
}

void CPanelStage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelStage)
	DDX_Control(pDX, IDC_Stage_Mag_Adj, m_spinMag);
	DDX_Control(pDX, IDC_Stage_Positions, m_listCtrlStagePos);
	DDX_Text(pDX, IDC_Stage_Mag, m_dMag);
	DDV_MinMaxDouble(pDX, m_dMag, 1., 250000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanelStage, CDialog)
	//{{AFX_MSG_MAP(CPanelStage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_Stage_Absolute, OnStageAbsolute)
	ON_BN_CLICKED(IDC_Stage_Relative, OnStageRelative)
	ON_BN_CLICKED(IDC_Stage_Pos_Goto, OnStagePosGoto)
	ON_BN_CLICKED(IDC_Stage_Pos_Add, OnStagePosAdd)
	ON_BN_CLICKED(IDC_Stage_Pos_Delete, OnStagePosDelete)
	ON_BN_CLICKED(IDC_Stage_Pos_Update, OnStagePosUpdate)
	ON_BN_CLICKED(IDC_Stage_Calibrate, OnStageCalibrate)
	ON_BN_CLICKED(IDC_Stage_XLeft, OnStageXLeft)
	ON_BN_CLICKED(IDC_Stage_XRight, OnStageXRight)
	ON_BN_CLICKED(IDC_Stage_YTop, OnStageYTop)
	ON_BN_CLICKED(IDC_Stage_YBottom, OnStageYBottom)
	ON_NOTIFY(NM_DBLCLK, IDC_Stage_Positions, OnStagePositionsDblclk)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_Stage_Positions, OnStagePositionsBeginlabeledit)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_Stage_Positions, OnStagePositionsEndlabeledit)
	ON_NOTIFY(NM_CLICK, IDC_Stage_Positions, OnStagePositionsClick)
	ON_BN_CLICKED(IDC_Stage_Reset, OnStageReset)
	ON_BN_CLICKED(IDC_Stage_DP, OnStageDP)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Stage_Mag_Adj, OnDeltaposStageMagAdj)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_Stage_Advance, OnStageAdvance)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelStage message handlers

CString CPanelStage::GetResString( UINT nID )
{
	CString str = _T("");
	str.LoadString( nID );
	return str;
}

void CPanelStage::Localize(void)
{ 
	if( m_hWnd == NULL )
		return;

	// ��Ʒ̨
//	GetDlgItem(IDC_Stage_Group)->SetWindowText(GetResString(IDS_Stage_Group)); 
	GetDlgItem(IDC_Stage_Absolute)->SetWindowText(GetResString(IDS_Stage_Absolute)); 
	GetDlgItem(IDC_Stage_Relative)->SetWindowText(GetResString(IDS_Stage_Relative)); 
//	GetDlgItem(IDC_Stage_Pos_Add)->SetWindowText(GetResString(IDS_Stage_Pos_Add)); 
//	GetDlgItem(IDC_Stage_Pos_Delete)->SetWindowText(GetResString(IDS_Stage_Pos_Delete)); 
	GetDlgItem(IDC_Stage_Pos_Update)->SetWindowText(GetResString(IDS_Stage_Pos_Update)); 
	GetDlgItem(IDC_Stage_Pos_Goto)->SetWindowText(GetResString(IDS_Stage_Pos_Goto)); 
	GetDlgItem(IDC_Stage_Calibrate)->SetWindowText(GetResString(IDS_Stage_Calibrate)); 
	GetDlgItem(IDC_Stage_Reset)->SetWindowText(GetResString(IDS_Stage_Reset)); 

//	SetWindowText(GetResString(IDS_PANEL_Stage)); 
} 

void CPanelStage::DisplayMsg(LPCTSTR lpszMessage)
{
	CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_Stage_Log) );
	if( peditLog == NULL )
		return;

	CString csMessage = CString(lpszMessage) + _T("\r\n");
	int len = peditLog->GetWindowTextLength();
	peditLog->SetSel(len,len);
	peditLog->ReplaceSel( (LPCTSTR)csMessage );
}

BOOL CPanelStage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	///////////////////////////////////////////////////////////////////////////
	// 15.10 XY�Զ�̨��س�ʼ����
	GetDlgItem( IDC_Stage_X )->SetWindowText( "0.00" );
	GetDlgItem( IDC_Stage_Y )->SetWindowText( "0.00" );
	m_bStageRelative = FALSE;
	((CButton*)(GetDlgItem( IDC_Stage_Absolute )))->SetCheck( TRUE );
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvc.iSubItem	= 0;
	lvc.pszText		= "";
	lvc.cx			= 80;
	lvc.fmt			= LVCFMT_LEFT;
	m_listCtrlStagePos.InsertColumn(0,&lvc);
	StagePosLoad();

	m_hIconStage1	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_Left));
	m_hIconStage2	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_Right));
	m_hIconStage3	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_Up));
	m_hIconStage4	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_Down));
	((CButton*)( GetDlgItem( IDC_Stage_XLeft ) ))->SetIcon( m_hIconStage1 );
	((CButton*)( GetDlgItem( IDC_Stage_XRight ) ))->SetIcon( m_hIconStage2 );
	((CButton*)( GetDlgItem( IDC_Stage_YTop ) ))->SetIcon( m_hIconStage3 );
	((CButton*)( GetDlgItem( IDC_Stage_YBottom ) ))->SetIcon( m_hIconStage4 );
	///////////////////////////////////////////////////////////////////////////
	int nScalePixels;
	CalculateUScale2( m_sizeReso.cx, m_dMag, nScalePixels );

	Localize();

	// ���Ի���λ�����õ��ͻ������Ͻ�
	CRect rc;
	GetClientRect( &rc );
	ClientToScreen( &rc );
	SetWindowPos(&CWnd::wndTop,
		10,//GetSystemMetrics(SM_CXSCREEN) - rc.Width()-10,
		32*3,//GetSystemMetrics(SM_CYSCREEN) /2 -rc.Height() /2,
		rc.Width(), rc.Height(), SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPanelStage::OnDestroy() 
{
	m_DMCWin.Close();

	POSITION pos;
	while( (pos=m_plistStagePos->GetHeadPosition()) )
	{
		structStagePos* pItem = m_plistStagePos->GetAt( pos );
		m_plistStagePos->RemoveHead();
		delete pItem;
	}
	delete m_plistStagePos;

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
}

void CPanelStage::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == nAutoStageXY_FI11 )
	{
		if( m_Stage.xy.nMtd < 1 )
			AutoStageXY_FI_XQ();
		else if( m_Stage.xy.nMtd < 2 )
			AutoStageXY_FI_Process11();
		else
			AutoStageXY_FI2_Process11();
	}
	else if( nIDEvent == nAutoStageXY_FI12 )
	{
		if( m_Stage.xy.nMtd < 2 )
			AutoStageXY_FI_Process12();
		else
			AutoStageXY_FI2_Process12();
	}
	else if( nIDEvent == nAutoStageXY_FI21 )
	{
		if( m_Stage.xy.nMtd < 2 )
			AutoStageXY_FI_Process21();
		else
			AutoStageXY_FI2_Process21();
	}
	else if( nIDEvent == nAutoStageXY_FI22 )
	{
		if( m_Stage.xy.nMtd < 2 )
			AutoStageXY_FI_Process22();
		else
			AutoStageXY_FI2_Process22();
	}
	else if( nIDEvent == nAutoStageXY_FI31 )
	{
		if( m_Stage.xy.nMtd < 2 )
			AutoStageXY_FI_Process31();
		else
			AutoStageXY_FI2_Process31();
	}
	else if( nIDEvent == nAutoStageXY_FI32 )
	{
		if( m_Stage.xy.nMtd < 2 )
			AutoStageXY_FI_Process32();
		else
			AutoStageXY_FI2_Process32();
	}
	else if( nIDEvent == nAutoStageXY_FI4 )
		AutoStageXY_FI_Process4();
	else if( nIDEvent == nAutoStageTrack )
		AutoStageXY_FI_TrackSecurity();
	else if( nIDEvent == nAutoStageClearXY )
		AutoStage_Clear_XY();
	///////////////////////////////////////

	CDialog::OnTimer(nIDEvent);
}

BOOL CPanelStage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		CString str = "";
		int nID = GetFocus()->GetDlgCtrlID();
		switch( nID )
		{
		case	IDC_Stage_X:
		case	IDC_Stage_Y:
		case	IDC_Stage_Mag:
			GetDlgItem( nID )->GetWindowText( str );
			break;
		default:
			break;
		}
		if( str.IsEmpty() )
			return CDialog::PreTranslateMessage(pMsg);
		switch( nID )
		{
		case	IDC_Stage_X:
			StagePosGotoX();
			break;
		case	IDC_Stage_Y:
			StagePosGotoY();
			break;
		case	IDC_Stage_Mag:
			UpdateData();
			MagChange();
			break;

		default:
			break;
		}

		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


///////////////////////////////////////////////////////////////////////////////
//
// XY�Զ�̨��ز���
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CPanelStage::Stage_XY_Init()
{
	// Initialize the DMCWin object
	m_DMCWin.SetController(g_nAutoStageController);	// Controller 1 in the Windows registry
	m_DMCWin.SethWnd(this->m_hWnd);	//GetSafeHwnd());			// Our hWnd;

	// Open a connection to the controller 
	m_DMCWin.Open();

	//
	// NOTE: We could have omitted the calls to SetController and SethWnd and
	//       called Open with arguments:
	//
	//    m_DMCWin.Open(nController, GetSafeHwnd());
	//

	if (m_DMCWin.IsConnected())
	{
/*		// Display the controller version information
		char szVersion[128];

		if (m_DMCWin.GetVersion(szVersion, sizeof(szVersion)) == 0)
			pEdit->SetWindowText(szVersion);*/
		// ��е��λ
/*		CString str;
		m_DMCWin.Command( "HM;BG", szBuffer, sizeof(szBuffer) );
		// �ƶ������ģ�50mm��̨�ӣ������ƶ���25mm����
		str.Format( "PA %ld,%ld;BG", g_nCenter /g_nGrating, g_nCenter /g_nGrating );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
*/
		m_DMCWin.Command( "MO" );
		m_DMCWin.Command( "SHXY" );
		m_DMCWin.Command( "AC 1000000,1000000" );
		m_DMCWin.Command( "DC 16000000,16000000" );
		m_DMCWin.Command( "BL -2147483648,-2147483648" );
		m_DMCWin.Command( "FL 2147483647,2147483647" );
		m_Stage.xy.ptCurPosAb = CPoint(10000000,10000000);

		m_Stage.xy.bReady = TRUE;
	}
	else
	{
		MessageBox( GetResString(IDS_Stage_Device_NOT_Found), "Stage", MB_OK |MB_ICONWARNING );
		m_Stage.xy.bReady = FALSE;
	}

	if( /*!m_Stage.nHasAutoStage || */!m_Stage.xy.bReady )
	{
		// �Զ�̨��ز�������ʹ��
		CRect rcSep, rc;
		GetDlgItem( IDC_Stage_Group )->GetWindowRect( &rcSep );
//		GetDlgItem( IDC_Stage_Group )->ShowWindow( FALSE );
		
		CWnd *pw = GetWindow(GW_CHILD);
		while(pw != NULL)
		{
			pw->GetWindowRect( &rc );
			if( rcSep.PtInRect(rc.TopLeft()) )
				pw->EnableWindow( FALSE );
			pw = pw->GetWindow(GW_HWNDNEXT);
		};
/*
		GetDlgItem( IDC_Stage_X )->EnableWindow( FALSE );
		GetDlgItem( IDC_Stage_XLeft )->EnableWindow( FALSE );
		GetDlgItem( IDC_Stage_XRight )->EnableWindow( FALSE );
		GetDlgItem( IDC_Stage_Y )->EnableWindow( FALSE );
		GetDlgItem( IDC_Stage_YTop )->EnableWindow( FALSE );
		GetDlgItem( IDC_Stage_YBottom )->EnableWindow( FALSE );
		GetDlgItem( IDC_Stage_Positions )->EnableWindow( FALSE );
		GetDlgItem( IDC_Stage_Pos_Add )->EnableWindow( FALSE );
		GetDlgItem( IDC_Stage_Pos_Delete )->EnableWindow( FALSE );
		GetDlgItem( IDC_Stage_Pos_Goto )->EnableWindow( FALSE );
		GetDlgItem( IDC_Stage_Calibrate )->EnableWindow( FALSE );
*/	}
	if( !m_Stage.xy.bReady )
		GetDlgItem( IDC_Stage_Reset )->EnableWindow( FALSE );

	// ��ȡ����·��
	TCHAR exeFullPath[255];
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	char path[255];
	sprintf(path, "%s", exeFullPath);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	char	szBuf1[50], szBuf2[50], szBufRootSection[20];
	sprintf(szBufRootSection, "StageCfg");
	// ��դ��
	if(GetPrivateProfileString(szBufRootSection, "Gratings", "0", szBuf1, 255, path) != 0)
	{
		strcpy(szBuf2, szBuf1);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf1, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_Stage.xy.nGratingX = atoi(szBuf2);
			m_Stage.xy.nGratingY = atoi(szBuf1);
		}
	}
	// �г�
	if(GetPrivateProfileString(szBufRootSection, "Ranges", "0", szBuf1, 255, path) != 0)
	{
		strcpy(szBuf2, szBuf1);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf1, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_Stage.xy.nRangeX = atoi(szBuf2);
			m_Stage.xy.nRangeY = atoi(szBuf1);
		}
	}
	// ��ԭ�㷽��
	m_Stage.xy.nMtd = GetPrivateProfileInt( szBufRootSection, "Mtd", 0, path );
	// XY�Ƿ���
	m_Stage.xy.bRev = GetPrivateProfileInt( szBufRootSection, "XY", 0, path );
	// X/Y�ķ���
	if(GetPrivateProfileString(szBufRootSection, "Dir", "0", szBuf1, 255, path) != 0)
	{
		strcpy(szBuf2, szBuf1);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf1, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			if( atol(szBuf2) > 0 )
			{
				m_Stage.xy.sXLD	= "-";	// ����Ϊ-
				m_Stage.xy.sXRD	= "+";	// ����Ϊ+
			}
			else
			{
				m_Stage.xy.sXLD	= "+";
				m_Stage.xy.sXRD	= "-";
			}
			if( atol(szBuf1) > 0 )
			{
				m_Stage.xy.sYLD	= "-";	// ����Ϊ-
				m_Stage.xy.sYRD	= "+";	// ����Ϊ+
			}
			else
			{
				m_Stage.xy.sYLD	= "+";
				m_Stage.xy.sYRD	= "-";
			}
		}
	}
	else
	{
		m_Stage.xy.sXLD	= "-";
		m_Stage.xy.sXRD	= "+";
		m_Stage.xy.sYLD	= "-";
		m_Stage.xy.sYRD	= "+";
	}

/*	if(GetPrivateProfileString(szBufRootSection, "Dir2", "0", szBuf1, 255, path) != 0)
	{
		strcpy(szBuf2, szBuf1);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf1, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_Stage.bIndex_XD = atol(szBuf2);
			m_Stage.bIndex_YD = atol(szBuf1);
		}
	}
	else
	{
		m_Stage.bIndex_XD = 1;
		m_Stage.bIndex_YD = 1;
	}
*/	// ��ԭ���һ��������ݴ˵ó����Ӧ�ж��ĸ���־λ
	if(GetPrivateProfileString(szBufRootSection, "Cmd1", "0", szBuf1, 255, path) != 0)
	{
		m_Stage.xy.sCmdFI1.Format( "%s", szBuf1 );
		strcpy(szBuf2, szBuf1);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf1, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			if( atol(szBuf2) > 0 )
			{
				m_Stage.xy.sCmdLX = "MG_LFX";	// X forward limit switch
				m_Stage.xy.sCmdLX2= "MG_LRX";
			}
			else
			{
				m_Stage.xy.sCmdLX = "MG_LRX";	// X reverse limit switch
				m_Stage.xy.sCmdLX2= "MG_LFX";
			}

//			strcpy(szBuf2, strchr(szBuf1, ',')+1);
			if( atol(szBuf2) > 0 )
			{
				m_Stage.xy.sCmdLY = "MG_LFY";	// Y forward limit switch
				m_Stage.xy.sCmdLY2= "MG_LRY";
			}
			else
			{
				m_Stage.xy.sCmdLY = "MG_LRY";	// Y reverse limit switch
				m_Stage.xy.sCmdLY2= "MG_LFY";
			}
		}
	}
	else
	{
		m_Stage.xy.sCmdFI1	= "-5000,5000";
		m_Stage.xy.sCmdLX	= "MG_LRX";
		m_Stage.xy.sCmdLY	= "MG_LFY";
		m_Stage.xy.sCmdLX2	= "MG_LFX";
		m_Stage.xy.sCmdLY2	= "MG_LRY";
	}
	// ��ԭ��ڶ�������1���ٶȣ�
	if(GetPrivateProfileString(szBufRootSection, "Cmd21", "0", szBuf1, 255, path) != 0)
		m_Stage.xy.sCmdFI21.Format( "%s", szBuf1 );
	// ��ԭ��ڶ�������2����ת��
	if(GetPrivateProfileString(szBufRootSection, "Cmd22", "0", szBuf1, 255, path) != 0)
		m_Stage.xy.sCmdFI22.Format( "%s", szBuf1 );
// 	{
// 		strcpy(szBuf2, szBuf1);
// 		if(strchr(szBuf2, ',') != NULL)
// 		{
// 			strcpy(szBuf1, strchr(szBuf2, ',')+1);
// 			*strchr(szBuf2, ',') = '\0';
// 			m_Stage.xy.sCmdFI22.Format( "%ld,,%ld", atol(szBuf2), atol(szBuf1) );
// 		}
// 	}
	// ��ԭ�����������
	if(GetPrivateProfileString(szBufRootSection, "Cmd3", "0", szBuf1, 255, path) != 0)
		m_Stage.xy.sCmdFI3.Format( "%s", szBuf1 );
	// �����λ����
	if(GetPrivateProfileString(szBufRootSection, "Cmd41", "0", szBuf1, 255, path) != 0)
		m_Stage.xy.sCmdLimitX.Format( "%s", szBuf1 );
	if(GetPrivateProfileString(szBufRootSection, "Cmd42", "0", szBuf1, 255, path) != 0)
		m_Stage.xy.sCmdLimitY.Format( "%s", szBuf1 );
}

void CPanelStage::Stage_XY_FindIndex()
{
	if( m_Stage.nHasAutoStage > 0 && m_Stage.xy.bReady )
	{
		CString str;
		str.Format("Grating: X = %d nm, Y = %d nm", m_Stage.xy.nGratingX, m_Stage.xy.nGratingY );
		DisplayMsg( str );
		str.Format("Stage reverse: %d", m_Stage.xy.bRev );
		DisplayMsg( str );
		str.Format("Stage XLD=%s, XRD=%s, YLD=%s, YRD=%s", m_Stage.xy.sXLD, m_Stage.xy.sXRD, m_Stage.xy.sYLD, m_Stage.xy.sYRD );
		DisplayMsg( str );
		str.Format("Stage CMD1 = %s, LX = %s, LY = %s", m_Stage.xy.sCmdFI1, m_Stage.xy.sCmdLX, m_Stage.xy.sCmdLY );
		DisplayMsg( str );
		str.Format("Stage CMD21 (SP) = %s", m_Stage.xy.sCmdFI21 );
		DisplayMsg( str );
		str.Format("Stage CMD22 (PR) = %s", m_Stage.xy.sCmdFI22 );
		DisplayMsg( str );
		str.Format("Stage CMD3 (JG OF FIXY) = %s", m_Stage.xy.sCmdFI3 );
		DisplayMsg( str );
		// ������Index
		AutoStageXY_FI_Init();
	}
}

int CPanelStage::Stage_XY_GetPosition( long& lX, long& lY )
{
	char szBuf1[64], szBuf2[64];
	m_DMCWin.Command( "TP", szBuf1, sizeof(szBuf1) );

	strcpy(szBuf2, szBuf1);
	if(strchr(szBuf2, ',') != NULL)
	{
		strcpy(szBuf1, strchr(szBuf2, ',')+1);
		*strchr(szBuf2, ',') = '\0';
		lX = atol(szBuf2);

		lY = atol(szBuf1);
/*		if(strchr(szBuf1, ',') != NULL)
		{
			strcpy(szBuf2, strchr(szBuf1, ',')+1);
			*strchr(szBuf1, ',') = '\0';
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				lY = atol(szBuf2);
			}
		}*/
	}

	CString str;
	str.Format( "pos X=%ld, Y=%ld", lX, lY );
	DisplayMsg( str );

	return 0;
}

void CPanelStage::Stage_XY_Command( CString strCmd, int nValue, BOOL bIsX, BOOL bDir )
{
	// һ������˶�
	CString str;
	if( bIsX )
	{
		if( bDir )	// X���������ƶ������磺������ϵ������Ҽ�ͷ
		{
			if( m_Stage.xy.bRev )	// XY����
				str.Format( "SHY;%s ,%s%ld;BGY", strCmd, m_Stage.xy.sYRD, nValue );
			else
				str.Format( "SHX;%s %s%ld;BGX", strCmd, m_Stage.xy.sXRD, nValue );
		}
		else		// X���������ƶ������磺������ϵ��������ͷ
		{
			if( m_Stage.xy.bRev)	// XY����
				str.Format( "SHY;%s ,%s%ld;BGY", strCmd, m_Stage.xy.sYLD, nValue );
			else
				str.Format( "SHX;%s %s%ld;BGX", strCmd, m_Stage.xy.sXLD, nValue );
		}
	}
	else
	{
		if( bDir )	// Y���������ƶ������磺������ϵ������¼�ͷ
		{
			if( m_Stage.xy.bRev )	// XY����
				str.Format( "SHX;%s %s%ld;BGX", strCmd, m_Stage.xy.sXRD, nValue );
			else
				str.Format( "SHY;%s ,%s%ld;BGY", strCmd, m_Stage.xy.sYRD, nValue );
		}
		else		// Y���������ƶ������磺������ϵ������ϼ�ͷ
		{
			if( m_Stage.xy.bRev)	// XY����
				str.Format( "SHX;%s %s%ld;BGX", strCmd, m_Stage.xy.sXLD, nValue );
			else
				str.Format( "SHY;%s ,%s%ld;BGY", strCmd, m_Stage.xy.sYLD, nValue );
		}
	}

	char szBuffer[64];
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	Stage_XY_GetPosition( m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
}

void CPanelStage::Stage_XY_Command2( CString strCmd, int nValueX, int nValueY, BOOL bDirX, BOOL bDirY, BOOL bSerial )
{
	// ������ͬʱ�˶�
	CString strTemp, str;
	if( m_Stage.xy.bRev )	// XY����
		strTemp.Format( "%s %s%ld,%s%ld", strCmd, bDirY ? m_Stage.xy.sXRD : m_Stage.xy.sXLD, nValueY,
											bDirX ? m_Stage.xy.sYRD : m_Stage.xy.sYLD, nValueX );
	else
		strTemp.Format( "%s %s%ld,%s%ld", strCmd, bDirX ? m_Stage.xy.sXRD : m_Stage.xy.sXLD, nValueX,
												bDirY ? m_Stage.xy.sYRD : m_Stage.xy.sYLD, nValueY );
// 	if( bSerial )
// 		str.Format( "SHXY;%s;BGXY", strTemp );
// 	else
// 		str = strTemp;
	str.Format( "SHXY;%s;BGXY", strTemp );
	DisplayMsg( str );

	char szBuffer[64];
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	Stage_XY_GetPosition( m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CString CPanelStage::CalculateUScale2( int nScreenWidth, double dfMag, int& nScalePixels )
{
	CString strRet;
	int		u_scale;

	////////////////////////////////////////////////////////////
	// Mag = 1, 
	// PhotoWidth = 120 mm		<==>	ScreenWidth = 1024 Pixel
	// u_scale = 10 mm			<==>	ScaleWidth	= ? Pixel
	////////////////////////////////////////////////////////////
	// ScaleWidth		= u_scale * ScreenWidth / PhotoWidth
	// SreenScaleParam	= ScreenWidth / PhotoWidth = 1024 / 120
	////////////////////////////////////////////////////////////
	const double SreenScaleParam = 1024.0 / 120.0 / (1024.0 /nScreenWidth);
//	const double SreenScaleParam = nScreenWidth / 120.0;	//1024.0 / 120.0;	// 11.05
	CString str;
	char szBuffer[64];
	// ���̶�����߳��̶���ͼ��ֱ��ʵ�1/4���ڣ����ڵױ�Ŀ��Ҳ��ע
	// ����ע�ߴ罫��һ����1��10��100����������ֵ
	double dScale = (nScreenWidth /4) / (dfMag *SreenScaleParam );
	if( dScale > 0.999 )
	{
		u_scale = (int)dScale;
		strRet.Format("%3d mm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam + 0.5);
		m_csUScaleUnit = _T("mm");
		m_dfMobileUScaleParameter = dfMag*SreenScaleParam;

		m_Stage.xy.nStageSPX = 10000;
		m_Stage.xy.nStageSPY = 10000;
	}
	else if( dScale > 0.0999 )
	{
		u_scale = (int)(dScale *1000) /100 *100;
		strRet.Format("%3d ��m", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);
		m_csUScaleUnit = _T("��m");
		m_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;

		m_Stage.xy.nStageSPX = 8000;
		m_Stage.xy.nStageSPY = 8000;
	}
	else if( dScale > 0.00999 )
	{
		u_scale = (int)(dScale *1000 +0.05) /10 *10;
		strRet.Format("%3d ��m", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);
		m_csUScaleUnit = _T("��m");
		m_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;

		m_Stage.xy.nStageSPX = 4000;
		m_Stage.xy.nStageSPY = 4000;
	}
	else if( dScale > 0.00099 )
	{
		u_scale = (int)(dScale *1000 +0.05);
		strRet.Format("%3d ��m", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);
		m_csUScaleUnit = _T("��m");
		m_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;

		m_Stage.xy.nStageSPX = 1000;
		m_Stage.xy.nStageSPY = 1000;
	}
	else if( dScale > 0.000099 )
	{
		u_scale = (int)(dScale *1000000 +0.05) /100 *100;
		strRet.Format("%3d nm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000000 + 0.5);
		m_csUScaleUnit = _T("nm");
		m_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000000;

		m_Stage.xy.nStageSPX = 800;
		m_Stage.xy.nStageSPY = 800;
	}
	else if( dScale > 0.0000099 )
	{
		u_scale = (int)(dScale *1000000 +0.05) /10 *10;
		strRet.Format("%3d nm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000000 + 0.5);
		m_csUScaleUnit = _T("nm");
		m_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000000;

		m_Stage.xy.nStageSPX = 100;
		m_Stage.xy.nStageSPY = 100;
	}
	else if( dScale > 0.00000099 )
	{
		u_scale = (int)(dScale *1000000 +0.05);
		strRet.Format("%3d nm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000000 + 0.5);
		m_csUScaleUnit = _T("nm");
		m_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000000;

		m_Stage.xy.nStageSPX = 50;
		m_Stage.xy.nStageSPY = 50;
	}
	str.Format( "SP %ld,%ld", m_Stage.xy.nStageSPX, m_Stage.xy.nStageSPY );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	return strRet;
}

//CString CalculateStageUScale( int nScalePixelsX, int nScalePixelsY, DWORD &dwScaleX, DWORD &dwScaleY )
CString CPanelStage::CalculateStageUScale( BOOL	bIsX,			// ��X��Y��1 is X, 0 is Y
							 BOOL	bDir,			// ����1 is positive, 0 is negative
							 int	nScalePixels,	// ���س���
							 int	&nScale )		// ʵ���ƶ�����
{
	if ( m_dfMobileUScaleParameter == -1 )
		return m_csUScaleUnit;

	// ʵ������ƶ�����
	double dbScale = nScalePixels / m_dfMobileUScaleParameter;
//	if( bIsX )
//		dbScale *= 2;
	// ��������׵�λ
	if( m_csUScaleUnit == "cm" )
		dbScale *= 10000000;
	else if( m_csUScaleUnit == "mm" )
		dbScale *= 1000000;
	else	// if( m_csUScaleUnit ==  "um" )
		dbScale *= 1000;

	// ʵ������ƶ�����
	if( bIsX )
		nScale = (int)(dbScale/m_Stage.xy.nGratingX);
	else
		nScale = (int)(dbScale/m_Stage.xy.nGratingY);
/*
	// ���¾���λ��
	if( bIsX )
	{
		if( bDir )
			m_ptCurPosAb.x += dbScale;
		else
			m_ptCurPosAb.x -= dbScale;
	}
	else
	{
		if( bDir )
			m_ptCurPosAb.y += dbScale;
		else
			m_ptCurPosAb.y -= dbScale;
	}
*/
	return m_csUScaleUnit;
}

void CPanelStage::OnStageAbsolute() 
{
	GetDlgItem( IDC_Stage_Pos_Add )->EnableWindow( TRUE );
	GetDlgItem( IDC_Stage_Pos_Delete )->EnableWindow( TRUE );
	GetDlgItem( IDC_Stage_Pos_Update )->EnableWindow( TRUE );
	GetDlgItem( IDC_Stage_Positions )->EnableWindow( TRUE );
	m_bStageRelative = FALSE;
}

void CPanelStage::OnStageRelative() 
{
	GetDlgItem( IDC_Stage_Pos_Add )->EnableWindow( FALSE );
	GetDlgItem( IDC_Stage_Pos_Delete )->EnableWindow( FALSE );
	GetDlgItem( IDC_Stage_Pos_Update )->EnableWindow( FALSE );
	GetDlgItem( IDC_Stage_Positions )->EnableWindow( FALSE );
	int nCount = m_listCtrlStagePos.GetItemCount();
	// ȫ�����ѡ�б�־
	for( int i=0; i<nCount; i++ )
		m_listCtrlStagePos.SetItemState(i, 0, LVIS_SELECTED);
	m_bStageRelative = TRUE;
}

void CPanelStage::StageMoveX( int nScaleX, BOOL bRelative, BOOL bDir )
{
	int nAbsX;
	// ������������û������ֵ�������г̷�Χ��������������ʾ�û�
	// ��ȡ�Զ�̨X/Y��ǰλ��������
	Stage_XY_GetPosition( m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
	// ������
	CString str;
	if( bDir )	// X���������ƶ������磺������ϵ������Ҽ�ͷ
	{
		if( m_Stage.xy.bRev )	// XY����
			str.Format( "%s%ld", m_Stage.xy.sYRD, nScaleX );
		else
			str.Format( "%s%ld", m_Stage.xy.sXRD, nScaleX );
	}
	else		// X���������ƶ������磺������ϵ��������ͷ
	{
		if( m_Stage.xy.bRev)	// XY����
			str.Format( "%s%ld", m_Stage.xy.sYLD, nScaleX );
		else
			str.Format( "%s%ld", m_Stage.xy.sXLD, nScaleX );
	}
	nScaleX = atoi( str );
	// �õ�Ŀǰ��ʵ��λ��mm
	BOOL bExceed = FALSE;
	if( m_Stage.xy.bRev )
	{
		if( bRelative )
			nAbsX = m_Stage.xy.ptCurPosAb.y + nScaleX;
		else
			nAbsX = nScaleX;
		// ��������
		if( nAbsX > (1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY )
		{
			bExceed	= TRUE;
			nAbsX	= (1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY;
		}
		else if( nAbsX < -(1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY )
		{
			bExceed	= TRUE;
			nAbsX	= -(1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY;
		}
	}
	else
	{
		if( bRelative )
			nAbsX = m_Stage.xy.ptCurPosAb.x + nScaleX;
		else
			nAbsX = nScaleX;
		// ��������
		if( nAbsX > (1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX )
		{
			bExceed = TRUE;
			nAbsX	= (1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX;
		}
		else if( nAbsX < -(1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX )
		{
			bExceed	= TRUE;
			nAbsX	= -(1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX;
		}
	}
	if( m_Stage.xy.bRev )
	{
		if( nAbsX < -(1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY )
			GetDlgItem( IDC_Stage_XLeft )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_XLeft )->EnableWindow( TRUE );
		if( nAbsX > (1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY )
			GetDlgItem( IDC_Stage_XRight )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_XRight )->EnableWindow( TRUE );
	}
	else
	{
		if( nAbsX < -(1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX )
			GetDlgItem( IDC_Stage_XLeft )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_XLeft )->EnableWindow( TRUE );
		if( nAbsX > (1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX )
			GetDlgItem( IDC_Stage_XRight )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_XRight )->EnableWindow( TRUE );
	}
	if( bExceed )
		AfxMessageBox( "X�����г̷�Χ��" );

	BeginWaitCursor(); // display the hourglass cursor
	char szBuffer[64];
//	str.Format( "SP 10000,10000" );
	str.Format( "SP %ld,%ld", m_Stage.xy.nStageSPX, m_Stage.xy.nStageSPY );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	if( m_Stage.xy.bRev)	// XY����
		str.Format( "SHY;PA ,%ld;BGY", nAbsX );
	else
		str.Format( "SHX;PA %ld;BGX", nAbsX );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	EndWaitCursor(); // remove the hourglass cursor
}

void CPanelStage::StageMoveY( int nScaleY, BOOL bRelative, BOOL bDir )
{
	int nAbsY;
	// ������������û������ֵ�������г̷�Χ��������������ʾ�û�
	// ��ȡ�Զ�̨X/Y��ǰλ��������
	Stage_XY_GetPosition( m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
	// ������
	CString str;
	if( bDir )	// Y���������ƶ������磺������ϵ������¼�ͷ
	{
		if( m_Stage.xy.bRev )	// XY����
			str.Format( "%s%ld", m_Stage.xy.sXRD, nScaleY );
		else
			str.Format( "%s%ld", m_Stage.xy.sYRD, nScaleY );
	}
	else		// Y���������ƶ������磺������ϵ������ϼ�ͷ
	{
		if( m_Stage.xy.bRev)	// XY����
			str.Format( "%s%ld", m_Stage.xy.sXLD, nScaleY );
		else
			str.Format( "%s%ld", m_Stage.xy.sYLD, nScaleY );
	}
	nScaleY = atoi( str );
	// �õ�Ŀǰ��ʵ��λ��mm
	BOOL bExceed = FALSE;
	if( m_Stage.xy.bRev )
	{
		if( bRelative )
			nAbsY = m_Stage.xy.ptCurPosAb.x + nScaleY;
		else
			nAbsY = nScaleY;
		// ��������
		if( nAbsY > (1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX )
		{
			bExceed	= TRUE;
			nAbsY	= (1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX;
		}
		else if( nAbsY < -(1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX )
		{
			bExceed = TRUE;
			nAbsY	= -(1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX;
		}
	}
	else
	{
		if( bRelative )
			nAbsY = m_Stage.xy.ptCurPosAb.y + nScaleY;
		else
			nAbsY = nScaleY;
		// ��������
		if( nAbsY > (1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY )
		{
			bExceed	= TRUE;
			nAbsY	= (1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY;
		}
		else if( nAbsY < -(1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY )
		{
			bExceed	= TRUE;
			nAbsY	= -(1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY;
		}
	}
	if( m_Stage.xy.bRev )
	{
		if( nAbsY < -(1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX )
			GetDlgItem( IDC_Stage_YBottom )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_YBottom )->EnableWindow( TRUE );
		if( nAbsY > (1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX )
			GetDlgItem( IDC_Stage_YTop )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_YTop )->EnableWindow( TRUE );
	}
	else
	{
		if( nAbsY < -(1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY )
			GetDlgItem( IDC_Stage_YBottom )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_YBottom )->EnableWindow( TRUE );
		if( nAbsY > (1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY )
			GetDlgItem( IDC_Stage_YTop )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_YTop )->EnableWindow( TRUE );
	}
	if( bExceed )
		AfxMessageBox( "Y�����г̷�Χ��" );

	BeginWaitCursor(); // display the hourglass cursor
	char szBuffer[64];
//	str.Format( "SP 10000,10000" );
	str.Format( "SP %ld,%ld", m_Stage.xy.nStageSPX, m_Stage.xy.nStageSPY );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	if( m_Stage.xy.bRev)	// XY����
		str.Format( "SHX;PA %ld;BGX", nAbsY );
	else
		str.Format( "SHY;PA ,%ld;BGY", nAbsY );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	EndWaitCursor(); // remove the hourglass cursor
}

void CPanelStage::OnStageXLeft() 
{
	// ���ؾ���
	int nLengthX = m_sizeReso.cx /4;
	// ʵ�ʲ���
	int nScaleX = 0;
	if( m_Stage.xy.bRev )
		CalculateStageUScale( FALSE, FALSE, nLengthX, nScaleX );
	else
		CalculateStageUScale( TRUE, FALSE, nLengthX, nScaleX );
	StageMoveX( nScaleX, TRUE, FALSE );
	return;

	char szBuffer[64];
	CString str;
	str.Format( "SP %ld,%ld", m_Stage.xy.nStageSPX, m_Stage.xy.nStageSPY );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	Stage_XY_Command( "PR", nScaleX, TRUE, FALSE );
}

void CPanelStage::OnStageXRight() 
{
	// ���ؾ���
	int nLengthX = m_sizeReso.cx /4;
	// ʵ�ʲ���
	int nScaleX = 0;
	if( m_Stage.xy.bRev )
		CalculateStageUScale( FALSE, TRUE, nLengthX, nScaleX );
	else
		CalculateStageUScale( TRUE, TRUE, nLengthX, nScaleX );
	StageMoveX( nScaleX, TRUE, TRUE );
	return;

	char szBuffer[64];
	CString str;
	str.Format( "SP %ld,%ld", m_Stage.xy.nStageSPX, m_Stage.xy.nStageSPY );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	Stage_XY_Command( "PR", nScaleX, TRUE, TRUE );
}

void CPanelStage::OnStageYTop() 
{
	// ���ؾ���
	int nLengthY = m_sizeReso.cy /4;
	// ʵ�ʲ���
	int nScaleY = 0;
	if( m_Stage.xy.bRev )
		CalculateStageUScale( TRUE, FALSE, nLengthY, nScaleY );
	else
		CalculateStageUScale( FALSE, FALSE, nLengthY, nScaleY );
	StageMoveY( nScaleY, TRUE, FALSE );
	return;

	char szBuffer[64];
	CString str;
	str.Format( "SP %ld,%ld", m_Stage.xy.nStageSPX, m_Stage.xy.nStageSPY );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	Stage_XY_Command( "PR", nScaleY, FALSE, FALSE );
}

void CPanelStage::OnStageYBottom() 
{
	// ���ؾ���
	int nLengthY = m_sizeReso.cy /4;
	// ʵ�ʲ���
	int nScaleY = 0;
	if( m_Stage.xy.bRev )
		CalculateStageUScale( TRUE, TRUE, nLengthY, nScaleY );
	else
		CalculateStageUScale( FALSE, TRUE, nLengthY, nScaleY );
	StageMoveY( nScaleY, TRUE, TRUE );
	return;

	char szBuffer[64];
	CString str;
	str.Format( "SP %ld,%ld", m_Stage.xy.nStageSPX, m_Stage.xy.nStageSPY );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	Stage_XY_Command( "PR", nScaleY, FALSE, TRUE );
}

void CPanelStage::StagePosLoad()
{
	// ��ȡ����·��
	TCHAR exeFullPath[255];
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	char path[255];
	sprintf(path, "%s", exeFullPath);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

    m_nStagePosTotal = GetPrivateProfileInt( "StagePos", "TOTAL", 0, path );
	if( m_nStagePosTotal == 0 )
		return;

	int nTotal = m_nStagePosTotal;

	LV_ITEM lvi;
	char	szBuf1[255], szBuf2[255], szBufRootSection[20], szBufRootKey[20];
	sprintf(szBufRootSection, "StagePos");
	for( int i=0; i<nTotal; i++ )
	{
		sprintf(szBufRootKey, "pos%d", i);
		if(GetPrivateProfileString(szBufRootSection, szBufRootKey, "", szBuf1, 255, path) != 0)
		{
			structStagePos* pPos = new structStagePos();
			strcpy(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				pPos->strPosName = szBuf2;
			}
			strcpy(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				pPos->dwPosX = atol(szBuf2);
			}
			strcpy(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				pPos->dwPosY = atol(szBuf2);
			}
			strcpy(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				pPos->dwShiftX = atol(szBuf2);
			}
			pPos->dwShiftY = atol(szBuf1);
			m_plistStagePos->AddTail( pPos );

			ZeroMemory(&lvi,sizeof(LV_ITEM));
			lvi.mask = LVIF_TEXT;
			lvi.iItem = m_listCtrlStagePos.GetItemCount();
			lvi.iSubItem = 0;
			(LPCTSTR&)lvi.pszText = pPos->strPosName;
			lvi.cchTextMax=strlen(lvi.pszText);
			m_listCtrlStagePos.InsertItem(&lvi);
			m_listCtrlStagePos.SetItem(&lvi);
		}
	}
}

void CPanelStage::StagePosUpdate()
{
	// ��ȡ����·��
	TCHAR exeFullPath[255];
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	char path[255];
	sprintf(path, "%s", exeFullPath);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	// ����λ������
	int nTotal = m_nStagePosTotal;
	CString str;
	str.Format( "%d", nTotal );
	WritePrivateProfileString( "StagePos", "Total", str, path );

	POSITION	pos			= m_plistStagePos->GetHeadPosition();
	int i = 0;
	LV_ITEM lvi;
	m_listCtrlStagePos.DeleteAllItems();
/*	CString*	pstrPosX	= new CString[nTotal];
	CString*	pstrPosY	= new CString[nTotal];
	CString*	pstrName	= new CString[nTotal];
	CString*	pstrShiftX	= new CString[nTotal];
	CString*	pstrShiftY	= new CString[nTotal];

	// �������
	while( pos )
	{
		structStagePos* pPos = m_plistStagePos->GetNext( pos );
		str.Format( "%ld", pPos->dwPosX );
		pstrPosX[i]		= str;
		str.Format( "%ld", pPos->dwPosY );
		pstrPosY[i]		= str;
		pstrName[i]		= pPos->strPosName;
		str.Format( "%ld", pPos->dwShiftX );
		pstrShiftX[i]	= str;
		str.Format( "%ld", pPos->dwShiftY );
		pstrShiftY[i]	= str;
		
		ZeroMemory(&lvi,sizeof(LV_ITEM));
		lvi.mask = LVIF_TEXT;
		lvi.iItem = m_listCtrlStagePos.GetItemCount();
		lvi.iSubItem = 0;
		(LPCTSTR&)lvi.pszText = pstrName[i];
		lvi.cchTextMax=strlen(lvi.pszText);
		m_listCtrlStagePos.InsertItem(&lvi);
		m_listCtrlStagePos.SetItem(&lvi);

		i++;
	}

	// д�������ļ�
	char stabuff[20];
	sprintf(stabuff, "StagePos");
	SaveInOneLine( path, stabuff, "PosX", STRSTYLE, nTotal, pstrPosX );
	SaveInOneLine( path, stabuff, "PosY", STRSTYLE, nTotal, pstrPosY );
	SaveInOneLine( path, stabuff, "Name", STRSTYLE, nTotal, pstrName );
	SaveInOneLine( path, stabuff, "ShiftX", STRSTYLE, nTotal, pstrShiftX );
	SaveInOneLine( path, stabuff, "ShiftY", STRSTYLE, nTotal, pstrShiftY );

	delete [] pstrPosX;
	delete [] pstrPosY;
	delete [] pstrName;
	delete [] pstrShiftX;
	delete [] pstrShiftY;
*/
	CString strPos;
	while( pos )
	{
		structStagePos* pPos = m_plistStagePos->GetNext( pos );

		// �������
		ZeroMemory(&lvi,sizeof(LV_ITEM));
		lvi.mask = LVIF_TEXT;
		lvi.iItem = m_listCtrlStagePos.GetItemCount();
		lvi.iSubItem = 0;
		(LPCTSTR&)lvi.pszText = pPos->strPosName;
		lvi.cchTextMax=strlen(lvi.pszText);
		m_listCtrlStagePos.InsertItem(&lvi);
		m_listCtrlStagePos.SetItem(&lvi);

		// д�������ļ�
		str.Format( "pos%d", i );
		strPos.Format( "%s,%ld,%ld,%ld,%ld", pPos->strPosName, pPos->dwPosX, pPos->dwPosY, pPos->dwShiftX, pPos->dwShiftY );
		WritePrivateProfileString( "StagePos", str, strPos, path );

		i++;
	}
}

void CPanelStage::StagePosGoto( int nItem )
{
	// ��ȡָ��λ������
	CString str;
	str = m_listCtrlStagePos.GetItemText( nItem, 0 );

	// �ҵ�ָ��λ�ò��ƶ�XY�Զ�̨
	POSITION pos = m_plistStagePos->GetHeadPosition();
	while( pos )
	{
		structStagePos* pPos = m_plistStagePos->GetAt( pos );
		if( pPos->strPosName == str )
		{
			// XY�Զ�̨λ��
			char szBuffer[64];
			CString str;
			str.Format( "SP 10000,10000" );
			m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
			str.Format( "SHXY;PA %ld,%ld;BGXY", pPos->dwPosX, pPos->dwPosY );
			m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
/*			// ��λ��λ��
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			ClassID.lVal = SemControlPanelClass_ScanShift;
			Flag.lVal = 3;
			Xpos.lVal = pPos->dwShiftX;
			Ypos.lVal = pPos->dwShiftY;
			SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
*/			break;
		}
		m_plistStagePos->GetNext( pos );
	}
}

void CPanelStage::OnStagePosAdd() 
{
	// ��ȡ�Զ�̨X/Y��ǰλ��������
	Stage_XY_GetPosition( m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );

	// ����һ����λ��
	CString str;
	str.Format( "Pos %d", ++m_nStagePosTotal );
	structStagePos* pPos = new structStagePos();
	// XY�Զ�̨λ�ò���
	pPos->dwPosX = m_Stage.xy.ptCurPosAb.x;
	pPos->dwPosY = m_Stage.xy.ptCurPosAb.y;
	// ����
	pPos->strPosName = str;
/*	// ��λ�Ʋ���
	VARIANT varData = m_SemCtrl.m_SemCore.m_ScanShiftX.GetPos();
	pPos->dwShiftX	= varData.lVal;
	varData = m_SemCtrl.m_SemCore.m_ScanShiftY.GetPos();
	pPos->dwShiftY	= varData.lVal;
*/	m_plistStagePos->AddTail( pPos );

	LV_ITEM lvi;
	ZeroMemory(&lvi,sizeof(LV_ITEM));
	lvi.mask = LVIF_TEXT;
	lvi.iItem = m_listCtrlStagePos.GetItemCount();
	lvi.iSubItem = 0;
	(LPCTSTR&)lvi.pszText = str;
	lvi.cchTextMax=strlen(lvi.pszText);
	m_listCtrlStagePos.InsertItem(&lvi);
	m_listCtrlStagePos.SetItem(&lvi);

	StagePosUpdate();
}

void CPanelStage::OnStagePosDelete() 
{
	// ��û��ѡ��λ��
	POSITION pos = m_listCtrlStagePos.GetFirstSelectedItemPosition();
	if( pos == NULL )
		return;

	// ��ȡָ��λ�����ֲ�ɾ��
	CString str;
	int nItem = -1;
	while( pos )
		nItem = m_listCtrlStagePos.GetNextSelectedItem(pos);
	str = m_listCtrlStagePos.GetItemText( nItem, 0 );
	m_listCtrlStagePos.DeleteItem( nItem );
	// �ҵ�ָ��λ�ò����б���ɾ��
	pos = m_plistStagePos->GetHeadPosition();
	while( pos )
	{
		structStagePos* pPos = m_plistStagePos->GetAt( pos );
		if( pPos->strPosName == str )
		{
			m_plistStagePos->RemoveAt( pos );
			delete pPos;
			break;
		}
		m_plistStagePos->GetNext( pos );
	}

	m_nStagePosTotal = m_plistStagePos->GetCount();
	StagePosUpdate();
}

void CPanelStage::OnStagePosUpdate() 
{
	// ��û��ѡ��λ��
	POSITION pos = m_listCtrlStagePos.GetFirstSelectedItemPosition();
	if( pos == NULL )
		return;

	// ��ȡָ��λ������
	CString str;
	int nItem = -1;
	while( pos )
		nItem = m_listCtrlStagePos.GetNextSelectedItem(pos);
	str = m_listCtrlStagePos.GetItemText( nItem, 0 );
	// �ҵ�ָ��λ�ò�����λ������
	pos = m_plistStagePos->GetHeadPosition();
	while( pos )
	{
		structStagePos* pPos = m_plistStagePos->GetAt( pos );
		if( pPos->strPosName == str )
		{
			GetDlgItem( IDC_Stage_X )->GetWindowText( str );
			pPos->dwPosX = (int)(atof(str) *1000000 /m_Stage.xy.nGratingX);	// �����׻����ʵ���ƶ�����
			GetDlgItem( IDC_Stage_Y )->GetWindowText( str );
			pPos->dwPosY = (int)(atof(str) *1000000 /m_Stage.xy.nGratingY);	// �����׻����ʵ���ƶ�����
			break;
		}
		m_plistStagePos->GetNext( pos );
	}

	StagePosUpdate();
}

void CPanelStage::OnStageAdvance() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_Stage_Advance)))->GetCheck();
	if( nCheck )
	{
		GetDlgItem( IDC_Stage_Calibrate )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_Stage_Advance )->SetWindowText( "<<" );
	}
	else
	{
		GetDlgItem( IDC_Stage_Calibrate )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_Stage_Advance )->SetWindowText( ">>" );
	}
}

void CPanelStage::OnStageCalibrate() 
{
	// TODO: Add your control notification handler code here
	// ��դ����������Ʒ�����Ĳ��غ�ʱ��ʹ�ø��ַ�������Ʒ������������Ļ����
	// Ȼ��ʹ�øá�У׼�����ܣ�ϵͳ����ʱ�Ĺ�դ��λ�ü�¼�������ļ���
	// �Ժ�ÿ�θ�λ����ԭ�㣩�������ٶ���һ�ξ���λ��
	// �ߵ�֮�󣬶���õ�Ϊ0,0��DP 0,0��

	// ��ȡ�Զ�̨X/Y��ǰλ��������
	Stage_XY_GetPosition( m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );

	// ��¼�������ļ���
	// ��ȡ����·��
	TCHAR exeFullPath[255];
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	char path[255];
	sprintf(path, "%s", exeFullPath);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	CString str;
	str.Format( "%ld,%ld", m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
	WritePrivateProfileString( "StagePos", "Cali", str, path );
/*	long*	plongCali	= new long[2];
	plongCali[0]	= m_Stage.xy.ptCurPosAb.x;
	plongCali[1]	= m_Stage.xy.ptCurPosAb.y;
	char stabuff[20];
	sprintf(stabuff, "StagePos");
	SaveInOneLine( path, stabuff, "Cali", LONGSTYLE, 2, plongCali );
	delete [] plongCali;
*/
	// ����õ�Ϊ0,0
	m_DMCWin.Command( "DP 0,0" );

	// ��ʾ
	str.Format( "Write Cali=%ld,%ld to ini file", m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
	DisplayMsg( str );
}

void CPanelStage::OnStageReset() 
{
	// TODO: Add your control notification handler code here
	if( m_Stage.nHasAutoStage > 0 )
	{
		// 15.10 XY�Զ�̨
		Stage_XY_FindIndex();
	}
}

void CPanelStage::OnStageDP() 
{
	// TODO: Add your control notification handler code here
	m_DMCWin.Command( "DP 0,0" );
	m_Stage.xy.ptCurPosAb.x = 0;
	m_Stage.xy.ptCurPosAb.y = 0;
}

void CPanelStage::OnStagePositionsDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	StagePosGoto( pDispInfo->item.mask );

	*pResult = 0;
}

void CPanelStage::OnStagePositionsBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_strName = m_listCtrlStagePos.GetItemText( pDispInfo->item.iItem, 0 );
	
	*pResult = 0;
}

void CPanelStage::OnStagePositionsEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString str = pDispInfo->item.pszText;//m_listCtrlStagePos.GetItemText( pDispInfo->item.iItem, 0 );
	if( str == m_strName )
		return;	// ����δ�޸�
	
	// �ҵ�ָ��λ�ò�����λ������
	POSITION pos = m_plistStagePos->GetHeadPosition();
	while( pos )
	{
		structStagePos* pPos = m_plistStagePos->GetAt( pos );
		if( pPos->strPosName == m_strName )
		{
			pPos->strPosName = str;
			break;
		}
		m_plistStagePos->GetNext( pos );
	}

	StagePosUpdate();

	*pResult = 0;
}

void CPanelStage::OnStagePositionsClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CPanelStage::SetupDebugParam( BOOL bDebugStage )
{
	if( bDebugStage )
	{
		GetDlgItem( IDC_Stage_Calibrate )->ShowWindow( SW_SHOW );
// 		GetDlgItem( IDC_Stage_Reset )->ShowWindow( SW_SHOW );
// 		if( !m_Stage.xy.bReady
// 			&& !m_Stage.zrt.bReadyZ && !m_Stage.zrt.bReadyR && !m_Stage.zrt.bReadyT )
// 			GetDlgItem( IDC_Stage_Reset )->EnableWindow( FALSE );
	}
	else
	{
		GetDlgItem( IDC_Stage_Calibrate )->ShowWindow( SW_HIDE );
//		GetDlgItem( IDC_Stage_Reset )->ShowWindow( SW_HIDE );
	}
}

void CPanelStage::OnStagePosGoto() 
{
	// 13.02.05 ����Ϣ��Ӧ��Ϊֻ���ֶ�����λ�ò��������ת���ѱ���λ�õ���תֱ����˫������
	if( m_Stage.xy.bRev )
	{
		StagePosGotoY();
		StagePosGotoX();
	}
	else
	{
		StagePosGotoX();
		StagePosGotoY();
	}
}

void CPanelStage::StagePosGotoX()
{
	// ���༭���������ֵ����
	CString str;
	int nScaleX;
	if( m_Stage.xy.bRev )
	{
		GetDlgItem( IDC_Stage_X )->GetWindowText( str );
		nScaleX = (int)(atof(str) *1000000 /m_Stage.xy.nGratingY);	// �����׻����ʵ���ƶ�����
	}
	else
	{
		GetDlgItem( IDC_Stage_X )->GetWindowText( str );
		nScaleX = (int)(atof(str) *1000000 /m_Stage.xy.nGratingX);	// �����׻����ʵ���ƶ�����
	}
	if( nScaleX > 0 )
		StageMoveX( nScaleX, m_bStageRelative, TRUE );
	else
		StageMoveX( abs(nScaleX), m_bStageRelative, FALSE );
	return;

	// ������������û������ֵ�������г̷�Χ��������������ʾ�û�
	// ��ȡ�Զ�̨X/Y��ǰλ��������
	Stage_XY_GetPosition( m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
	// �õ�Ŀǰ��ʵ��λ��mm
	int nActualX;
	if( m_Stage.xy.bRev )
		nActualX = m_Stage.xy.ptCurPosAb.y / m_Stage.xy.nGratingY;
	else
		nActualX = m_Stage.xy.ptCurPosAb.x / m_Stage.xy.nGratingX;
	if( m_bStageRelative )
	{
		// �������
		if( (nActualX + nScaleX) > 74.5*1000000 /m_Stage.xy.nGratingX		// 1450000
			|| (nActualX + nScaleX) < -74.5*1000000 /m_Stage.xy.nGratingX )// -1450000 )
		{
			AfxMessageBox( "X�����г̷�Χ��" );
			return;
		}
	}
	else
	{
		// ��������
		if( nScaleX > 74.5*1000000 /m_Stage.xy.nGratingX		// 1450000
			|| nScaleX < -74.5*1000000 /m_Stage.xy.nGratingX )	// -1450000 )
		{
			AfxMessageBox( "X�����г̷�Χ��" );
			return;
		}
	}

	char szBuffer[64];
	str.Format( "SP 10000,10000" );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	if( m_bStageRelative )
		str.Format( "SHX;PR %ld;BGX", nScaleX );	// �������
	else
		str.Format( "SHX;PA %ld;BGX", nScaleX );	// ��������
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
}

void CPanelStage::StagePosGotoY()
{
	// ���༭���������ֵ����
	CString str;
	int nScaleY;
	if( m_Stage.xy.bRev )
	{
		GetDlgItem( IDC_Stage_Y )->GetWindowText( str );
		nScaleY = (int)(atof(str) *1000000 /m_Stage.xy.nGratingX);	// �����׻����ʵ���ƶ�����
	}
	else
	{
		GetDlgItem( IDC_Stage_Y )->GetWindowText( str );
		nScaleY = (int)(atof(str) *1000000 /m_Stage.xy.nGratingY);	// �����׻����ʵ���ƶ�����
	}
	if( nScaleY > 0 )
		StageMoveY( nScaleY, m_bStageRelative, TRUE );
	else
		StageMoveY( abs(nScaleY), m_bStageRelative, FALSE );
	return;

	// ������������û������ֵ�������г̷�Χ��������������ʾ�û�
	// ��ȡ�Զ�̨X/Y��ǰλ��������
	Stage_XY_GetPosition( m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
	// �õ�Ŀǰ��ʵ��λ��mm
	int nActualY;
	if( m_Stage.xy.bRev )
		nActualY = m_Stage.xy.ptCurPosAb.x / m_Stage.xy.nGratingX;
	else
		nActualY = m_Stage.xy.ptCurPosAb.y / m_Stage.xy.nGratingY;
	if( m_bStageRelative )
	{
		// �������
		if( ( (nActualY + nScaleY) > 74.5*1000000 /m_Stage.xy.nGratingY )		// 1450000 )
			|| ( (nActualY + nScaleY) < -74.5*1000000 /m_Stage.xy.nGratingY )	// -1450000 )
			|| ( m_Stage.xy.ptCurPosAb.x < 0
				&& m_Stage.xy.ptCurPosAb.y < 0 
				&& (nActualY +nScaleY) < -30.0*1000000 /m_Stage.xy.nGratingY ) )// -600000 ) )
		{
			AfxMessageBox( "Y�����г̷�Χ��" );
			return;
		}
	}
	else
	{
		// ��������
		if( nScaleY > 74.5*1000000 /m_Stage.xy.nGratingY			// 1450000
			|| nScaleY < -74.5*1000000 /m_Stage.xy.nGratingY		// -1450000
			|| ( m_Stage.xy.ptCurPosAb.x < 0
				&& m_Stage.xy.ptCurPosAb.y < 0 
				&& nScaleY < -30.0*1000000 /m_Stage.xy.nGratingY ) )// -600000 ) )
		{
			AfxMessageBox( "Y�����г̷�Χ��" );
			return;
		}
	}

	char szBuffer[64];
	str.Format( "SP 10000,10000" );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	if( m_bStageRelative )
		str.Format( "SHY;PR ,%ld;BGY", nScaleY );	// �������
	else
		str.Format( "SHY;PA ,%ld;BGY", nScaleY );	// ��������
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
}








void CPanelStage::AutoStageXY_FI_Init()
{
	CDlgProgress		dlg;
	dlg.m_csTitle		= GetResString( IDS_Stage_SD_TITLE_AUTO_FindIndex );
	dlg.m_iRangeTo		= 40;
	dlg.m_csMessage		= GetResString( IDS_Stage_STRING_AUTO_FindIndex );

	m_bStageFinishX		= FALSE;
	m_bStageFinishY		= FALSE;

	char szBuffer[64];
	CString str;
	if( m_Stage.xy.nMtd < 1 )
	{
		m_lastStage = GetTickCount();
		// �����ϵ縴λ����
		str.Format( "XQ #AUTO" );
	}
	else
	{
		// ������ԭ��
		// ��һ����ײ���г̿��أ�����Y����
		str.Format( "SHY;JG %s;BGY", m_Stage.xy.sCmdFI1 );
	}
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	DisplayMsg( str );
	SetTimer( nAutoStageXY_FI11, nAutoStageXY_FIElapse, NULL );

	m_pAutoProgressDlg	= &dlg;
	dlg.DoModal();
	m_pAutoProgressDlg	= NULL;

	AutoStageXY_FI_Finish( FALSE );
}

void CPanelStage::AutoStageXY_FI_XQ()
{
	// �õ���ǰλ��
	long lPosX, lPosY;
	Stage_XY_GetPosition( lPosX, lPosY );
	if(  (abs(lPosY - m_Stage.xy.ptCurPosAb.y) < 10)
		&& (abs(lPosX - m_Stage.xy.ptCurPosAb.x) < 10) )
	{
		DWORD thisTickCount = GetTickCount();
		if( thisTickCount -m_lastStage > 5000 )	// 3sû�ж�
			AutoStageXY_FI_Finish( TRUE );
	}
	else
	{
		m_Stage.xy.ptCurPosAb.x	= lPosX;
		m_Stage.xy.ptCurPosAb.y	= lPosY;
		m_lastStage = GetTickCount();
	}
}

void CPanelStage::AutoStageXY_FI_Process11()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", m_Stage.xy.sCmdFI1 );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// ��ѯ��־λ
	m_DMCWin.Command( (char*)((const char*)m_Stage.xy.sCmdLY), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLY = atoi( str );
	// �õ���ǰλ��
	long lPosX, lPosY;
	Stage_XY_GetPosition( lPosX, lPosY );
	// ���λ����1���ڱ仯С��10������Ϊ���ñ�־λ����ͬ��ֱ��ײ���г̿��أ�
	if(  abs(lPosY - m_Stage.xy.ptCurPosAb.y) < 10 )
		nLY = 0;
	if( nLY != 0 )
		m_Stage.xy.ptCurPosAb.y	= lPosY;

	if( nLY == 0 )
	{
		KillTimer( nAutoStageXY_FI11 );	// ֹͣ��һ��Y����ײ�г̿���
		// �������ڵ�λ��
		m_Stage.xy.ptCurPosAb.x	= lPosX;
		m_Stage.xy.ptCurPosAb.y	= lPosY;

		m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		DisplayMsg( "ST at the beginning of process 21" );
		Sleep( 500 );
		m_DMCWin.Command( "SHXY" );
		DisplayMsg( "SHXY at the beginning of process 21: return to center" );
		Sleep( 500 );

		// �ڶ�����Y���������м��ߣ�����������Լ2mm
		str.Format( "JG %s", m_Stage.xy.sCmdFI21 );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

		str.Format( "SHY;PR %s;BGY", m_Stage.xy.sCmdFI22 );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		DisplayMsg( str );
		SetTimer( nAutoStageXY_FI21, nAutoStageXY_FIElapse, NULL );
	}
}

void CPanelStage::AutoStageXY_FI_Process12()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", m_Stage.xy.sCmdFI1 );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// ��ѯ��־λ
	m_DMCWin.Command( (char*)((const char*)m_Stage.xy.sCmdLX), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLX = atoi( str );
	// �õ���ǰλ��
	long lPosX, lPosY;
	Stage_XY_GetPosition( lPosX, lPosY );
	// ���λ����1���ڱ仯С��10������Ϊ���ñ�־λ����ͬ��ֱ��ײ���г̿��أ�
	if(  abs(lPosX - m_Stage.xy.ptCurPosAb.x) < 10 )
		nLX = 0;
	if( nLX != 0 )
		m_Stage.xy.ptCurPosAb.x	= lPosX;

	if( nLX == 0 )
	{
		KillTimer( nAutoStageXY_FI12 );	// ֹͣ��һ��ײ�г̿���
		// �������ڵ�λ��
		m_Stage.xy.ptCurPosAb.x	= lPosX;
		m_Stage.xy.ptCurPosAb.y	= lPosY;

		m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		DisplayMsg( "ST at the beginning of process 22" );
		Sleep( 500 );
		m_DMCWin.Command( "SHXY" );
		DisplayMsg( "SHXY at the beginning of process 22: return to center" );
		Sleep( 500 );

		// �ڶ��������������м��ߣ�����������Լ2mm
		str.Format( "JG %s", m_Stage.xy.sCmdFI21 );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

		str.Format( "SHX;PR %s;BGX", m_Stage.xy.sCmdFI22 );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		DisplayMsg( str );
		SetTimer( nAutoStageXY_FI22, nAutoStageXY_FIElapse, NULL );
	}
}

void CPanelStage::AutoStageXY_FI_Process21()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", m_Stage.xy.sCmdFI21 );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	Stage_XY_GetPosition( lPosX, lPosY );

	if(  !m_bStageFinishY && abs(lPosY - m_Stage.xy.ptCurPosAb.y) < 100 )
		m_bStageFinishY = TRUE;
	else
		m_Stage.xy.ptCurPosAb.y = lPosY;

	if( m_bStageFinishY )
	{
		// Y�����ѵ���������Լ2mm��
		KillTimer( nAutoStageXY_FI21 );
		Sleep( 100 );

		// ��������ʹ�ýϵ͵��ٶ�Ѱ�����Index
		str.Format( "SHY;JG %s;FIY;BGY", m_Stage.xy.sCmdFI3 );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		DisplayMsg( str );
		SetTimer( nAutoStageXY_FI31, nAutoStageXY_FIElapse, NULL );
	}
}

void CPanelStage::AutoStageXY_FI_Process22()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", m_Stage.xy.sCmdFI21 );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	Stage_XY_GetPosition( lPosX, lPosY );

	if( !m_bStageFinishX && abs(lPosX - m_Stage.xy.ptCurPosAb.x) < 100 )
		m_bStageFinishX = TRUE;
	else
		m_Stage.xy.ptCurPosAb.x = lPosX;

	if( m_bStageFinishX )
	{
		// X�����ѵ���������Լ2mm��
		KillTimer( nAutoStageXY_FI22 );
		Sleep( 100 );

		// ��������ʹ�ýϵ͵��ٶ�Ѱ�����Index
		str.Format( "SHX;JG %s;FIX;BGX", m_Stage.xy.sCmdFI3 );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		DisplayMsg( str );
		SetTimer( nAutoStageXY_FI32, nAutoStageXY_FIElapse, NULL );
	}
}

void CPanelStage::AutoStageXY_FI_Process31()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", m_Stage.xy.sCmdFI3 );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long nPosX, nPosY;
	Stage_XY_GetPosition( nPosX, nPosY );

	if( abs(nPosY) < 10 )
	{
		KillTimer( nAutoStageXY_FI31 );
		Sleep( 500 );

		// X����ײ�г̿���
		str.Format( "SHX;JG %s;BGX", m_Stage.xy.sCmdFI1 );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		DisplayMsg( str );
		SetTimer( nAutoStageXY_FI12, nAutoStageXY_FIElapse, NULL );
	}
}

void CPanelStage::AutoStageXY_FI_Process32()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", m_Stage.xy.sCmdFI3 );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	Stage_XY_GetPosition( lPosX, lPosY );

	if( abs(lPosX) < 10 )
	{
		KillTimer( nAutoStageXY_FI32 );
		Sleep( 100 );

		// ���Ĳ���У׼
		BOOL bVisible = ((GetDlgItem(IDC_Stage_Calibrate)->GetStyle() & WS_VISIBLE) != 0);
		if( bVisible )
		{
			// ����ʱ��ֻ�ҵ�Index��ֹͣ
			AutoStageXY_FI_Finish( TRUE );
			return;
		}
		// ���������һ����Indexʱ�����߾��Ծ���ΪУ׼ֵ
		// ��ȡ����·��
		TCHAR exeFullPath[255];
		GetModuleFileName(NULL,exeFullPath,MAX_PATH);
		char path[255];
		sprintf(path, "%s", exeFullPath);
		char *exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		strcat(path, "\\KYKYcfg.ini");

		m_Stage.xy.ptCurPosAb.x = 0;
		m_Stage.xy.ptCurPosAb.y = 0;
		char	szBuf1[50], szBuf2[50], szBufRootSection[20];
		sprintf(szBufRootSection, "StagePos");
		if(GetPrivateProfileString(szBufRootSection, "Cali", "0", szBuf1, 255, path) != 0)
		{
			strcpy(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				m_Stage.xy.ptCurPosAb.x = atol(szBuf2);
				m_Stage.xy.ptCurPosAb.y = atol(szBuf1);
			}
		}
		if( m_Stage.xy.ptCurPosAb.x != 0 || m_Stage.xy.ptCurPosAb.y != 0 )
		{
			str.Format( "SHXY;PA %ld,%ld;BGXY", m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
			m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
			DisplayMsg( str );
			SetTimer( nAutoStageXY_FI4, nAutoStageXY_FIElapse, NULL );
		}
		else
			AutoStageXY_FI_Finish( TRUE );
	}
}

void CPanelStage::AutoStageXY_FI2_Process11()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", m_Stage.xy.sCmdFI1 );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// ��ѯ��־λ
	m_DMCWin.Command( (char*)((const char*)m_Stage.xy.sCmdLY), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLY = atoi( str );
	// �õ���ǰλ��
	long lPosX, lPosY;
	Stage_XY_GetPosition( lPosX, lPosY );
	// ���λ����1���ڱ仯С��10������Ϊ���ñ�־λ����ͬ��ֱ��ײ���г̿��أ�
	if(  abs(lPosY - m_Stage.xy.ptCurPosAb.y) < 10 )
		nLY = 0;
	if( nLY != 0 )
		m_Stage.xy.ptCurPosAb.y	= lPosY;

	if( nLY == 0 )
	{
		KillTimer( nAutoStageXY_FI11 );	// ֹͣ��һ��Y����ײ�г̿���
		Sleep( 100 );

		m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		DisplayMsg( "ST at the beginning of process 21" );
		Sleep( 500 );
		m_DMCWin.Command( "SHXY" );
		DisplayMsg( "SHXY at the beginning of process 21: return to center" );
		Sleep( 500 );

		// ����ԭ��
		m_DMCWin.Command( "DP 0,0" );
		DisplayMsg( "DP 0,0" );
		// �������ڵ�λ��
		m_Stage.xy.ptCurPosAb.x	= lPosX;
		m_Stage.xy.ptCurPosAb.y	= 0;

		// �ڶ�����Y������ײ�г̿���
		str.Format( "SHY;JG %s;BGY", m_Stage.xy.sCmdFI21 );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		DisplayMsg( str );
		SetTimer( nAutoStageXY_FI21, nAutoStageXY_FIElapse, NULL );
	}
}

void CPanelStage::AutoStageXY_FI2_Process12()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", m_Stage.xy.sCmdFI1 );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// ��ѯ��־λ
	m_DMCWin.Command( (char*)((const char*)m_Stage.xy.sCmdLX), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLX = atoi( str );
	// �õ���ǰλ��
	long lPosX, lPosY;
	Stage_XY_GetPosition( lPosX, lPosY );
	// ���λ����1���ڱ仯С��10������Ϊ���ñ�־λ����ͬ��ֱ��ײ���г̿��أ�
	if(  abs(lPosX - m_Stage.xy.ptCurPosAb.x) < 10 )
		nLX = 0;
	if( nLX != 0 )
		m_Stage.xy.ptCurPosAb.x	= lPosX;

	if( nLX == 0 )
	{
		KillTimer( nAutoStageXY_FI12 );	// ֹͣ��һ��X����ײ�г̿���
		Sleep( 100 );

		m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		DisplayMsg( "ST at the beginning of process 22" );
		Sleep( 500 );
		m_DMCWin.Command( "SHXY" );
		DisplayMsg( "SHXY at the beginning of process 22: return to center" );
		Sleep( 500 );
		// ����ԭ��
		m_DMCWin.Command( "DP 0,0" );
		DisplayMsg( "DP 0,0" );
		// �������ڵ�λ��
		m_Stage.xy.ptCurPosAb.x	= 0;
		m_Stage.xy.ptCurPosAb.y	= lPosY;

		// �ڶ�����X������ײ�г̿���
		str.Format( "SHX;JG %s;BGX", m_Stage.xy.sCmdFI21 );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		DisplayMsg( str );
		SetTimer( nAutoStageXY_FI22, nAutoStageXY_FIElapse, NULL );
	}
}

void CPanelStage::AutoStageXY_FI2_Process21()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", m_Stage.xy.sCmdFI21 );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// ��ѯ��־λ
	m_DMCWin.Command( (char*)((const char*)m_Stage.xy.sCmdLY2), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLY = atoi( str );
	// �õ���ǰλ��
	long lPosX, lPosY;
	Stage_XY_GetPosition( lPosX, lPosY );
	// ���λ����1���ڱ仯С��10������Ϊ���ñ�־λ����ͬ��ֱ��ײ���г̿��أ�
	if(  abs(lPosY - m_Stage.xy.ptCurPosAb.y) < 10 )
		nLY = 0;
	if( nLY != 0 )
		m_Stage.xy.ptCurPosAb.y	= lPosY;
	
	if( nLY == 0 )
	{
		KillTimer( nAutoStageXY_FI21 );	// ֹͣ�ڶ���Y������ײ�г̿���
		Sleep( 100 );
		// �������ڵ�λ��
		m_Stage.xy.ptCurPosAb.x	= lPosX;
		m_Stage.xy.ptCurPosAb.y	= lPosY;

		m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		DisplayMsg( "ST at the beginning of process 31" );
		Sleep( 500 );
		m_DMCWin.Command( "SHXY" );
		DisplayMsg( "SHXY at the beginning of process 31: return to center" );
		Sleep( 500 );

		// ���������������ߵ�Y�����г̵�һ��
		str.Format( "SP %s", m_Stage.xy.sCmdFI1 );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		str.Format( "SHY;PAY=%ld;BGY", lPosY /2 );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		DisplayMsg( str );
		SetTimer( nAutoStageXY_FI31, nAutoStageXY_FIElapse, NULL );
	}
}

void CPanelStage::AutoStageXY_FI2_Process22()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", m_Stage.xy.sCmdFI21 );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	// ��ѯ��־λ
	m_DMCWin.Command( (char*)((const char*)m_Stage.xy.sCmdLX2), szBuffer, sizeof(szBuffer) );
	str.Format( "%s", szBuffer );
	int nLX = atoi( str );
	// �õ���ǰλ��
	long lPosX, lPosY;
	Stage_XY_GetPosition( lPosX, lPosY );
	// ���λ����1���ڱ仯С��10������Ϊ���ñ�־λ����ͬ��ֱ��ײ���г̿��أ�
	if(  abs(lPosX - m_Stage.xy.ptCurPosAb.x) < 10 )
		nLX = 0;
	if( nLX != 0 )
		m_Stage.xy.ptCurPosAb.x	= lPosX;
	
	if( nLX == 0 )
	{
		KillTimer( nAutoStageXY_FI22 );	// ֹͣ�ڶ���X������ײ�г̿���
		Sleep( 100 );
		// �������ڵ�λ��
		m_Stage.xy.ptCurPosAb.x	= lPosX;
		m_Stage.xy.ptCurPosAb.y	= lPosY;

		m_DMCWin.Command( "ST", szBuffer, sizeof(szBuffer) );
		DisplayMsg( "ST at the beginning of process 32" );
		Sleep( 500 );
		m_DMCWin.Command( "SHXY" );
		DisplayMsg( "SHXY at the beginning of process 32: return to center" );
		Sleep( 500 );

		// ���������������ߵ�X�����г̵�һ��
		str.Format( "SP %s", m_Stage.xy.sCmdFI1 );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		str.Format( "SHX;PAX=%ld;BGX", lPosX /2 );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		DisplayMsg( str );
		SetTimer( nAutoStageXY_FI32, nAutoStageXY_FIElapse, NULL );
	}
}

void CPanelStage::AutoStageXY_FI2_Process31()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", m_Stage.xy.sCmdFI1 );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	Stage_XY_GetPosition( lPosX, lPosY );

	if(  !m_bStageFinishY && abs(lPosY - m_Stage.xy.ptCurPosAb.y) < 100 )
		m_bStageFinishY = TRUE;
	else
		m_Stage.xy.ptCurPosAb.y = lPosY;

	if( m_bStageFinishY )
	{
		KillTimer( nAutoStageXY_FI31 );	// �ѵ������Ĵ�
		Sleep( 500 );

		// X����ײ�г̿���
		CString str;
		str.Format( "SHX;JG %s;BGX", m_Stage.xy.sCmdFI1 );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		DisplayMsg( str );
		SetTimer( nAutoStageXY_FI12, nAutoStageXY_FIElapse, NULL );
	}
}

void CPanelStage::AutoStageXY_FI2_Process32()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP %s", m_Stage.xy.sCmdFI1 );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	Stage_XY_GetPosition( lPosX, lPosY );

	if( !m_bStageFinishX && abs(lPosX - m_Stage.xy.ptCurPosAb.x) < 100 )
		m_bStageFinishX = TRUE;
	else
		m_Stage.xy.ptCurPosAb.x = lPosX;

	if( m_bStageFinishX )
	{
		KillTimer( nAutoStageXY_FI32 );	// �ѵ������Ĵ�
		Sleep( 100 );
		// ����ԭ��
		m_DMCWin.Command( "DP 0,0" );
		DisplayMsg( "DP 0,0" );

		// ���Ĳ���У׼
		BOOL bVisible = ((GetDlgItem(IDC_Stage_Calibrate)->GetStyle() & WS_VISIBLE) != 0);
		if( bVisible )
		{
			// ����ʱ��ֻ�ҵ�Index��ֹͣ
			AutoStageXY_FI_Finish( TRUE );
			return;
		}
		// ���������һ����Indexʱ�����߾��Ծ���ΪУ׼ֵ
		// ��ȡ����·��
		TCHAR exeFullPath[255];
		GetModuleFileName(NULL,exeFullPath,MAX_PATH);
		char path[255];
		sprintf(path, "%s", exeFullPath);
		char *exd = strrchr(path, (int)'\\');
		if(exd != NULL)
			*exd = '\0';
		strcat(path, "\\KYKYcfg.ini");

		m_Stage.xy.ptCurPosAb.x = 0;
		m_Stage.xy.ptCurPosAb.y = 0;
		char	szBuf1[50], szBuf2[50], szBufRootSection[20];
		sprintf(szBufRootSection, "StagePos");
		if(GetPrivateProfileString(szBufRootSection, "Cali", "0", szBuf1, 255, path) != 0)
		{
			strcpy(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				m_Stage.xy.ptCurPosAb.x = atol(szBuf2);
				m_Stage.xy.ptCurPosAb.y = atol(szBuf1);
			}
		}
		if( m_Stage.xy.ptCurPosAb.x != 0 || m_Stage.xy.ptCurPosAb.y != 0 )
		{
			str.Format( "SHXY;PA %ld,%ld;BGXY", m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
			m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
			DisplayMsg( str );
			SetTimer( nAutoStageXY_FI4, nAutoStageXY_FIElapse, NULL );
		}
		else
			AutoStageXY_FI_Finish( TRUE );
	}
}

void CPanelStage::AutoStageXY_FI_Process4()
{
	long nPosX, nPosY;
	Stage_XY_GetPosition( nPosX, nPosY );

	long nDiffX	= m_Stage.xy.ptCurPosAb.x - nPosX;
	long nDiffY	= m_Stage.xy.ptCurPosAb.y - nPosY;
	if( -10 < nDiffX && nDiffX < 10 && -10 < nDiffY && nDiffY < 10 )
	{
		// ����ԭ��
		m_DMCWin.Command( "DP 0,0" );
		DisplayMsg( "DP 0,0" );
		// �����λ
//		char szBuffer[64];
		CString str;
		str.Format( "%s", m_Stage.xy.sCmdLimitX );
//		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		DisplayMsg( str );
		str.Format( "%s", m_Stage.xy.sCmdLimitY );
//		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		DisplayMsg( str );
		AutoStageXY_FI_Finish( TRUE );
	}
}

void CPanelStage::AutoStageXY_FI_Finish( BOOL bAutoFinish )
{
	KillTimer( nAutoStageXY_FI11 );
	KillTimer( nAutoStageXY_FI12 );
	KillTimer( nAutoStageXY_FI21 );
	KillTimer( nAutoStageXY_FI22 );
	KillTimer( nAutoStageXY_FI31 );
	KillTimer( nAutoStageXY_FI32 );
	KillTimer( nAutoStageXY_FI4 );

	if( bAutoFinish )
	{
		// �Զ��ҵ�ԭ�㣬����˳�
		if ( m_pAutoProgressDlg != NULL )
			m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
	}

	m_DMCWin.Command( "ST" );
	DisplayMsg( "ST in finish" );
	Sleep( 100 );
	m_DMCWin.Command( "MO" );
	DisplayMsg( "MO in finish" );
	m_DMCWin.Command( "SHXY" );
	DisplayMsg( "SHXY in finish" );
	Sleep( 100 );
	Stage_XY_GetPosition( m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
}

void CPanelStage::AutoStageXY_FI_TrackSecurity()
{
	// ��ȡ�Զ�̨X/Y��ǰλ��������
	Stage_XY_GetPosition( m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
	// �õ�Ŀǰ��ʵ��λ��mm
	BOOL bExceed = FALSE;
	int nActualX, nActualY;
	if( m_Stage.xy.bRev )
	{
		nActualX = m_Stage.xy.ptCurPosAb.y;
		// ��������
		if( nActualX > (1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY				// 1450000
			|| nActualX < -(1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY )			// -1450000
			bExceed = TRUE;
	}
	else
	{
		nActualX = m_Stage.xy.ptCurPosAb.x;
		// ��������
		if( nActualX > (1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX				// 1450000
			|| nActualX < -(1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX )			// -1450000 )
			bExceed = TRUE;
	}
	if( m_Stage.xy.bRev )
	{
		nActualY = m_Stage.xy.ptCurPosAb.x;
		// ��������
		if( nActualY > (1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX				// 1450000
			|| nActualY < -(1.0 *m_Stage.xy.nRangeX /2 -0.1)*1000000 /m_Stage.xy.nGratingX )			// -1450000 )
			bExceed = TRUE;
	}
	else
	{
		nActualY = m_Stage.xy.ptCurPosAb.y;
		// ��������
		if( nActualY > (1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY				// 1450000
			|| nActualY < -(1.0 *m_Stage.xy.nRangeY /2 -0.1)*1000000 /m_Stage.xy.nGratingY )			// -1450000
			bExceed = TRUE;
	}
	if( bExceed )
	{
		KillTimer( nAutoStageTrack );
		m_DMCWin.Command( "ST" );
		DisplayMsg( "ST in AutoStageXY_FI_TrackSecurity" );
		Sleep( 100 );
		m_DMCWin.Command( "MO" );
		DisplayMsg( "MO in AutoStageXY_FI_TrackSecurity" );
		m_DMCWin.Command( "SHXY" );
		DisplayMsg( "SHXY in AutoStageXY_FI_TrackSecurity" );
		Sleep( 100 );
		Stage_XY_GetPosition( m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
		AfxMessageBox( "�����г̷�Χ��" );
		if ( m_bCatch )
		{
			m_bCatch = FALSE;
			ReleaseCapture();
			m_bStageTrack = FALSE;

			// ������ʶ���˳�����ģʽ
			CPoint ptPoint	= m_ptStageTrackParam;
			if( ptPoint.x < STAGE_TRACE_POINT_RADIUS )
				ptPoint.x = STAGE_TRACE_POINT_RADIUS;
			if( ptPoint.x > m_sizeReso.cx - STAGE_TRACE_POINT_RADIUS )
				ptPoint.x = m_sizeReso.cx - STAGE_TRACE_POINT_RADIUS;
			if( ptPoint.y < STAGE_TRACE_POINT_RADIUS )
				ptPoint.y = STAGE_TRACE_POINT_RADIUS;
			if( ptPoint.y > m_sizeReso.cy - STAGE_TRACE_POINT_RADIUS )
				ptPoint.y = m_sizeReso.cy - STAGE_TRACE_POINT_RADIUS;
			CRect rect(ptPoint.x-STAGE_TRACE_POINT_RADIUS,ptPoint.y-STAGE_TRACE_POINT_RADIUS,
				ptPoint.x+STAGE_TRACE_POINT_RADIUS,ptPoint.y+STAGE_TRACE_POINT_RADIUS);
			GraphDrawCircle( ptPoint, m_ptPosSave, ::GetSysColor(COLOR_3DFACE) );
			GraphDrawCircle( ptPoint, m_ptPosSave, ::GetSysColor(COLOR_3DFACE), FALSE );
			GraphClear( rect, ::GetSysColor(COLOR_3DFACE) );
		}
	}
}

void CPanelStage::AutoStage_Clear( BOOL bZRT )
{
	if( !bZRT && m_Stage.nHasAutoStage > 0 && m_Stage.xy.bReady )
	{
		CDlgProgress		dlg;
		dlg.m_csTitle		= "Axis X/Y clearing";
		dlg.m_iRangeTo		= 20;
		dlg.m_csMessage		= "Axis X/Y is clearing now, please wait...";

		m_bStageFinishX		= FALSE;
		m_bStageFinishY		= FALSE;

		// ��һ����XY�����
		char szBuffer[64];
		CString str;
		str.Format( "SP 5000,5000" );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		str.Format( "SHXY;PA 0,0;BGXY" );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		DisplayMsg( str );
		SetTimer( nAutoStageClearXY, nAutoStageXY_FIElapse, NULL );

		m_pAutoProgressDlg	= &dlg;
		dlg.DoModal();
		m_pAutoProgressDlg	= NULL;

		AutoStage_Clear_Finish( FALSE, FALSE );
	}
}

void CPanelStage::AutoStage_Clear_XY()
{
	char szBuffer[64];
	CString str;
	str.Format( "SP 5000,5000" );
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );

	long lPosX, lPosY;
	Stage_XY_GetPosition( lPosX, lPosY );

	if(  !m_bStageFinishX && abs(lPosX - m_Stage.xy.ptCurPosAb.x) < 100 )
		m_bStageFinishX = TRUE;
	else
		m_Stage.xy.ptCurPosAb.x = lPosX;

	if(  !m_bStageFinishY && abs(lPosY - m_Stage.xy.ptCurPosAb.y) < 100 )
		m_bStageFinishY = TRUE;
	else
		m_Stage.xy.ptCurPosAb.y = lPosY;

	if( m_bStageFinishX && m_bStageFinishY )
	{
		// XY�����ѹ���
		KillTimer( nAutoStageClearXY );
		Sleep( 100 );
		AutoStage_Clear_Finish( FALSE, TRUE );
	}
}

void CPanelStage::AutoStage_Clear_Finish( BOOL bStep, BOOL bAutoFinish )
{
	if( !bStep )
		KillTimer( nAutoStageClearXY );
	if( bAutoFinish )
	{
		// XYT���������
		if( m_pAutoProgressDlg != NULL )
			m_pAutoProgressDlg->SendMessage(WM_COMMAND, IDOK);
	}
}

void CPanelStage::CoherenceCheck()
{
	if( m_dMag > 250000 )
		m_dMag = 250000;
	if( m_dMag < 1 )
		m_dMag = 1;
}

void CPanelStage::OnDeltaposStageMagAdj(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData();
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
	CoherenceCheck();
	UpdateData(FALSE);

	int nScalePixels;
	CalculateUScale2( m_sizeReso.cx, m_dMag, nScalePixels );
}

void CPanelStage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
/*	
	// TODO: Add your message handler code here
	CRect rc;
	GetDlgItem(IDC_Stage_View)->GetWindowRect(rc);	// ȡ�ÿؼ�����Ļ�е�λ��
	ScreenToClient(rc);								// ת�����ڶԻ����е�λ��
	CBrush brushFrame;
	brushFrame.CreateSolidBrush(RGB(0,0,0));
	CBrush *poldbrush = dc.SelectObject( &brushFrame );
	dc.FillRect(rc, &brushFrame);
	dc.SelectObject( poldbrush );
	brushFrame.DeleteObject();
/*
	CPoint ptCenter = rc.CenterPoint();
	int nRadius = 5;
	CRect rcGrapic = CRect(ptCenter.x -nRadius, ptCenter.y -nRadius, ptCenter.x +nRadius, ptCenter.y +nRadius);
	CBrush brush;
	brush.CreateSolidBrush( RGB(0,255,0));
	poldbrush = dc.SelectObject( &brush );
	dc.Ellipse( ptCenter.x -nRadius, ptCenter.y -nRadius, ptCenter.x +nRadius, ptCenter.y +nRadius );
	dc.SelectObject( poldbrush );
	brush.DeleteObject();
*/
	// Do not call CDialog::OnPaint() for painting messages
}

void CPanelStage::GraphDrawCircle( CPoint ptCenter,	// ���ٵ�Բ��
							 CPoint ptEnd,			// ������β��
							 COLORREF color,		// ��ˢ/����ɫ
							 BOOL bDrawArrow,		// �Ƿ񻭼�ͷ
							 int nRadius )			// ���ٵ�Բ�α�ʶ�뾶
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	GetDlgItem(IDC_Stage_View)->GetWindowRect(rc);
	ScreenToClient( rc );
	//��������Ļ���ڼ��ݵ��ڴ��豸����
	CDC bmpDC;
	bmpDC.CreateCompatibleDC( &dc );
	//�����������ڴ��豸�����л�ͼ��λͼ��������ѡ���ڴ��豸����
	CBitmap& bitmap = *(new CBitmap);
	bitmap.CreateCompatibleBitmap( &dc, rc.Width(),	rc.Height() );
	CBitmap* poldBmp = bmpDC.SelectObject( &bitmap );
	//��Ŀ�������Ա�ɫ���
	bmpDC.FillRect( &CRect(rc.left, rc.top, rc.Width(), rc.Height()), &CBrush(::GetSysColor(COLOR_3DFACE)) );
	//�����豸��������ԭʼ��
	CPoint point = bmpDC.SetWindowOrg( 0, 0 );

/*	CBrush brBrushBk, *poldbrushBk;
	brBrushBk.CreateSolidBrush( ::GetSysColor(COLOR_3DFACE) );
	poldbrushBk = dc.SelectObject( &brBrushBk );
	dc.FillRect( (LPCRECT)rc, &brBrushBk );
	dc.SelectObject( poldbrushBk );
	brBrushBk.DeleteObject();
*/	
	// �����ٵ�Բ
	CRect rcGrapic = CRect(ptCenter.x -nRadius, ptCenter.y -nRadius, ptCenter.x +nRadius, ptCenter.y +nRadius);
	/////////////////////////////////////////////////////////////////
	CBrush brBrush, *poldbrush;
	brBrush.CreateSolidBrush( color );
	poldbrush = bmpDC.SelectObject( &brBrush );
	CPen pen, *poldpen;
	pen.CreatePen(PS_SOLID, 2, color);
	poldpen = bmpDC.SelectObject( &pen );
	/////////////////////////////////////////////////////////////////
	int nBkMode = bmpDC.SetBkMode( TRANSPARENT );

	bmpDC.Ellipse( ptCenter.x -nRadius, ptCenter.y -nRadius, ptCenter.x +nRadius, ptCenter.y +nRadius );

	if( bDrawArrow )
	{
		// ��������
		/////////////////////////////////////////////////////////////////

		bmpDC.MoveTo ( ptCenter.x, ptCenter.y );
		bmpDC.LineTo ( ptEnd.x, ptEnd.y );

		/////////////////////////////////////////////////////////////////
		// ����ͷ
		/////////////////////////////////////////////////////////////////
		/* ��ͷ����һ��������,���Զ���һ������Ϊ3��CPoint����,����һ����ˢ,
		Ȼ��Ϳ�����Polygon������������������;
		����ȷ����ͷ�����γ���ͷ�����������������,�����·���ȷ��:
		�����ͷ�ߵ����Ϊbegin,��ͷ����Ϊend
		double slopy , cosy , siny;
		double Par = 10.0;  //length of Arrow
		slopy = atan2((begin.y - end.y), (begin.x - end.x));
		cosy = cos(slopy);
		siny = sin(slopy);

		��������Ϊ
		end.x + int(Par * cosy - (Par / 2.0 * siny)),
		end.y + int(Par * siny + (Par / 2.0 * cosy));

		end.x + int(Par * cosy + Par / 2.0 * siny),
		end.y - int(Par / 2.0 * cosy - Par * siny)*/

		double slopy , cosy , siny;
		double Par = nRadius *2;  //length of Arrow
		slopy = atan2(double(ptCenter.y - ptEnd.y), double(ptCenter.x - ptEnd.x));
		cosy = cos(slopy);
		siny = sin(slopy);
		CPoint pt[3];
		pt[0].x = ptEnd.x + int(Par * cosy - (Par / 2.0 * siny));
		pt[0].y = ptEnd.y + int(Par * siny + (Par / 2.0 * cosy));
		pt[1].x = ptEnd.x + int(Par * cosy + Par / 2.0 * siny);
		pt[1].y = ptEnd.y - int(Par / 2.0 * cosy - Par * siny);
		pt[2] = ptEnd;

		bmpDC.Polygon( pt, 3 );

		/////////////////////////////////////////////////////////////////
	}
	bmpDC.SelectObject( poldbrush );
	brBrush.DeleteObject();
	bmpDC.SelectObject( poldpen );
	pen.DeleteObject();
	bmpDC.SetBkMode( nBkMode );
	dc.BitBlt( rc.left, rc.top, rc.Width(), rc.Height(), &bmpDC, 0, 0, SRCCOPY );

	//�ָ��豸��������ԭ�е�ԭʼ��
	bmpDC.SetWindowOrg( point );
	bmpDC.SelectObject( poldBmp );
	delete &bitmap;
	Invalidate(FALSE);
}

void CPanelStage::GraphClear( const CRect rcDest,	const COLORREF color )
{
	CPaintDC dc(this); // device context for painting
	CBrush brBrush, *poldbrush;
	brBrush.CreateSolidBrush( color );
	poldbrush = dc.SelectObject( &brBrush );

	CRect rcGraph = rcDest;
	dc.FillRect( (LPCRECT)rcGraph, &brBrush );

	dc.SelectObject( poldbrush );
	brBrush.DeleteObject();
	Invalidate(FALSE);
}

void CPanelStage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	GetDlgItem(IDC_Stage_View)->GetWindowRect(rc);	// ȡ�ÿؼ�����Ļ�е�λ��
	ScreenToClient(rc);								// ת�����ڶԻ����е�λ��
 	if( !rc.PtInRect( point ) )
 	{
 		CDialog::OnLButtonDown(nFlags, point);
 		return;
 	}

	m_bStageTrack	= TRUE;
	m_bCatch		= TRUE;
	SetCapture();

	m_ptPosSave				= point;	//ScreenToGraph( point );
	m_ptStageTrackParam		= m_ptPosSave;
	m_ptPosNew				= m_ptPosSave;

	// ��һ����ʶ��׼���������ģʽ
	CPoint ptPoint	= m_ptStageTrackParam;
	if( ptPoint.x < STAGE_TRACE_POINT_RADIUS )
		ptPoint.x = STAGE_TRACE_POINT_RADIUS;
	if( ptPoint.x > m_sizeReso.cx - STAGE_TRACE_POINT_RADIUS )
		ptPoint.x = m_sizeReso.cx - STAGE_TRACE_POINT_RADIUS;
	if( ptPoint.y < STAGE_TRACE_POINT_RADIUS )
		ptPoint.y = STAGE_TRACE_POINT_RADIUS;
	if( ptPoint.y > m_sizeReso.cy - STAGE_TRACE_POINT_RADIUS )
		ptPoint.y = m_sizeReso.cy - STAGE_TRACE_POINT_RADIUS;
	GraphDrawCircle( ptPoint, m_ptPosSave, RGB(0, 255, 0), FALSE );

	// ���ó�ʼ���ٲ���
	double dMag = m_dMag;
	if( dMag < 1000 )
	{
		m_nStageTrackStepX = 50;
		m_nStageTrackStepY = 50;
	}
	else if( dMag < 5000 )
	{
		m_nStageTrackStepX = 20;
		m_nStageTrackStepY = 20;
	}
	else
	{
		m_nStageTrackStepX = 10;
		m_nStageTrackStepY = 10;
	}

	CString str;
	// ����
	str.Format( "JG %ld,%ld;BGXY", m_nStageTrackStepX, m_nStageTrackStepY );
// 	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	// �������ٱ���
	SetTimer( nAutoStageTrack, nAutoStageXY_FIElapse, NULL );

	CDialog::OnLButtonDown(nFlags, point);
}

void CPanelStage::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bCatch )
	{
		// ֹͣ��Ʒ̨�ƶ�
		m_DMCWin.Command( "ST" );
		DisplayMsg( "ST in OnMButtonUp" );
		m_bCatch = FALSE;
		ReleaseCapture();
		m_bStageTrack = FALSE;
		KillTimer( nAutoStageTrack );

		// ������ʶ���˳�����ģʽ
		CPoint ptPoint	= m_ptStageTrackParam;
		if( ptPoint.x < STAGE_TRACE_POINT_RADIUS )
			ptPoint.x = STAGE_TRACE_POINT_RADIUS;
		if( ptPoint.x > m_sizeReso.cx - STAGE_TRACE_POINT_RADIUS )
			ptPoint.x = m_sizeReso.cx - STAGE_TRACE_POINT_RADIUS;
		if( ptPoint.y < STAGE_TRACE_POINT_RADIUS )
			ptPoint.y = STAGE_TRACE_POINT_RADIUS;
		if( ptPoint.y > m_sizeReso.cy - STAGE_TRACE_POINT_RADIUS )
			ptPoint.y = m_sizeReso.cy - STAGE_TRACE_POINT_RADIUS;
		CRect rect(ptPoint.x-STAGE_TRACE_POINT_RADIUS,ptPoint.y-STAGE_TRACE_POINT_RADIUS,
			ptPoint.x+STAGE_TRACE_POINT_RADIUS,ptPoint.y+STAGE_TRACE_POINT_RADIUS);
		GraphDrawCircle( ptPoint, m_ptPosSave, ::GetSysColor(COLOR_3DFACE) );
		GraphDrawCircle( ptPoint, m_ptPosSave, ::GetSysColor(COLOR_3DFACE), FALSE );
		GraphClear( rect, ::GetSysColor(COLOR_3DFACE) );
		return;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CPanelStage::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bCatch )
	{
		//SemCtrlMouse_Update
		if( (nFlags & MK_LBUTTON) == MK_LBUTTON )		//09.03.20
		{
			// �����м����϶�������Ʒ̨����ģʽ�£��������ٲ���
			if( m_bStageTrack && m_Stage.nHasAutoStage > 0 && m_Stage.xy.bReady )
				SetStageTrackParam( point );
			return;
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CPanelStage::SetStageTrackParam(CPoint point)
{
	/////////////////////////////////////////////
	CPoint ptPos0 =	m_ptPosSave;
	CPoint ptPos1 = point;	// ScreenToGraph( point );
	CPoint ptPoint	= m_ptStageTrackParam;
	m_ptPosNew	= point;
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// ����
	BOOL bSameX, bSameY;
	bSameX = bSameY = FALSE;
	if( ptPos1.x == ptPoint.x )
		bSameX = TRUE;
	if( ptPos1.y == ptPoint.y )
		bSameY = TRUE;
	GraphDrawCircle( ptPoint, ptPos0, ::GetSysColor(COLOR_3DFACE) );
//	GraphClear( CRect(ptPos0.x, ptPos0.y, ptPoint.x, ptPoint.y), ::GetSysColor(COLOR_3DFACE) );
	CRect rect(ptPoint.x-STAGE_TRACE_POINT_RADIUS,ptPoint.y-STAGE_TRACE_POINT_RADIUS,
		ptPoint.x+STAGE_TRACE_POINT_RADIUS,ptPoint.y+STAGE_TRACE_POINT_RADIUS);
//	GraphClear( rect, ::GetSysColor(COLOR_3DFACE) );
	GraphDrawCircle( ptPoint, ptPos1, RGB(0, 255, 0), TRUE, STAGE_TRACE_POINT_RADIUS );
	/////////////////////////////////////////////

	double dScaleX, dScaleY;
	/////////////////////////////////////////////
	// ����X���༰����
	if( (ptPos0.x == ptPoint.x) || (ptPos1.x == ptPoint.x) )
		dScaleX = 1.0;
	else
		dScaleX = 1.0 * abs(ptPos1.x - ptPoint.x) / abs(ptPos0.x - ptPoint.x);
	m_nStageTrackStepX = (int)(abs(m_nStageTrackStepX) *dScaleX);
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	// ����Y���༰����
	if( (ptPos0.y == ptPoint.y) || (ptPos1.y == ptPoint.y) )
		dScaleY = 1.0;
	else
		dScaleY = 1.0 * abs(ptPos1.y - ptPoint.y) / abs(ptPos0.y - ptPoint.y);
	m_nStageTrackStepY = (int)(abs(m_nStageTrackStepY) *dScaleY);
	/////////////////////////////////////////////

	// ����λ��
	m_ptPosSave = ptPos1;

	// ���ø��ٲ���
// 	Stage_XY_Command2( "JG", bSameX ? 0 : m_nStageTrackStepX, bSameY ? 0 : m_nStageTrackStepY,
// 		(ptPos1.x<ptPoint.x) ? FALSE : TRUE, (ptPos1.y<ptPoint.y) ? FALSE : TRUE, FALSE );
}
