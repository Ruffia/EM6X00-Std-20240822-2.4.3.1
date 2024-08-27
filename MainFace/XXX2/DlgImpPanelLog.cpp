// DlgImpPanelLog.cpp : implementation file
//
// Last Modify : 2010.09.02

/////////////////////////////////////////////////////////////////////////////
// 10.09.02 调试窗口一打开，就自动记录所有操作日志
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XXX2.h"
#include "DlgImpPanelLog.h"

#include "SemCtrlDlgRenewSelect.h"
#include "AppLogManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelLog dialog


CDlgImpPanelLog::CDlgImpPanelLog(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImpPanelLog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImpPanelLog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgImpPanelLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImpPanelLog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImpPanelLog, CDialog)
	//{{AFX_MSG_MAP(CDlgImpPanelLog)
	ON_BN_CLICKED(IDC_PANEL_LOG_Btn_Clear, OnLogClear)
	ON_BN_CLICKED(IDC_PANEL_LOG_Btn_Save, OnLogSave)
	ON_BN_CLICKED(IDC_PANEL_LOG_Btn_Select, OnLogSelect)
	ON_BN_CLICKED(IDC_PANEL_LOG_Btn_Msg, OnLogMsg)
	ON_BN_CLICKED(IDC_PANEL_LOG_Btn_Msg_RG, OnPANELLOGBtnMsgRG)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelLog message handlers

void CDlgImpPanelLog::Localize(void)
{ 
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_PANEL_LOG_Btn_Clear)->SetWindowText(GetResString(IDS_PANEL_LOG_Btn_Clear)); 
	GetDlgItem(IDC_PANEL_LOG_Btn_Save)->SetWindowText(GetResString(IDS_PANEL_LOG_Btn_Save)); 
	GetDlgItem(IDC_PANEL_LOG_Btn_Select)->SetWindowText(GetResString(IDS_PANEL_LOG_Btn_Select)); 
	GetDlgItem(IDC_PANEL_LOG_Btn_Msg)->SetWindowText(GetResString(IDS_PANEL_LOG_Btn_Msg)); 
	SetWindowText(GetResString(IDS_STATIC_IMP_PANEL_Group_Log)); 
} 

BOOL CDlgImpPanelLog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_PANEL_LOG_EDIT_Log) );
	peditLog->SetLimitText( 4294967294 );

	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgImpPanelLog::OnDestroy() 
{
	CloseAutoLog();
	CDialog::OnDestroy();

	// TODO: Add your message handler code here

}

void CDlgImpPanelLog::Message(LPCTSTR lpszMessage)
{
	CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_PANEL_LOG_EDIT_Log) );
	if( peditLog == NULL )
		return;

	CString csMessage = CString(lpszMessage) + _T("\r\n");
	int len = peditLog->GetWindowTextLength();
	peditLog->SetSel(len,len);
	peditLog->ReplaceSel( (LPCTSTR)csMessage );

	// 10.09.02
	if( m_fAutoLogFile.m_hFile != CFile::hFileNull )
	{
		m_fAutoLogFile.Write( csMessage, csMessage.GetLength() );
	}
	// 10.09.02

	m_nLoopCounts++;
	if( m_nLoopCounts > 10000 )
	{
		CloseAutoLog();
		peditLog->SetSel(0,len);
		peditLog->ReplaceSel( (LPCTSTR)csMessage );
		InitAutoLog();
	}
}

void CDlgImpPanelLog::OnLogSelect() 
{
	// TODO: Add your command handler code here
	CSemCtrlDlgRenewSelect	dlg;

	////////////////////////////////////////////////////////////////////
	dlg.m_boolHV						=	CAppLogManager::Instance().m_boolHVLog;
	dlg.m_boolScanShift					=	CAppLogManager::Instance().m_boolScanShiftLog;
	dlg.m_boolBeemAlignment				=	CAppLogManager::Instance().m_boolBeemAlignmentLog;
	dlg.m_boolBrightness				=	CAppLogManager::Instance().m_boolBrightnessLog;
	dlg.m_boolContrast					=	CAppLogManager::Instance().m_boolContrastLog;
	dlg.m_boolStigmator					=	CAppLogManager::Instance().m_boolStigmatorLog;
	dlg.m_boolObjectiveLens				=	CAppLogManager::Instance().m_boolObjectiveLensLog;
	dlg.m_boolCondenserLens				=	CAppLogManager::Instance().m_boolCondenserLensLog;
	dlg.m_boolFilamentCurrent			=	CAppLogManager::Instance().m_boolFilamentCurrentLog;
	dlg.m_boolMagnifier					=	CAppLogManager::Instance().m_boolMagnifierLog;
	dlg.m_boolWorkDistance				=	CAppLogManager::Instance().m_boolWorkDistanceLog;
	dlg.m_boolObjectiveLensReverse		=	CAppLogManager::Instance().m_boolObjectiveLensReverseLog;
	dlg.m_boolCondenserLensReverse		=	CAppLogManager::Instance().m_boolCondenserLensReverseLog;
	dlg.m_boolAuxiliaryStigmation		=	CAppLogManager::Instance().m_boolAuxiliaryStigmationLog;
	dlg.m_boolDCondenserLens			=	CAppLogManager::Instance().m_boolDCondenserLensLog;
	dlg.m_boolScanRotate				=	CAppLogManager::Instance().m_boolScanRotateLog;
	dlg.m_boolSACP						=	CAppLogManager::Instance().m_boolSACPLog;
	dlg.m_boolDFocus					=	CAppLogManager::Instance().m_boolDFocusLog;
	dlg.m_boolDFocusSwitch				=	CAppLogManager::Instance().m_boolDFocusSwitchLog;
	dlg.m_boolDObjectiveLens			=	CAppLogManager::Instance().m_boolDObjectiveLensLog;
	dlg.m_boolDObjectiveLensSwitch		=	CAppLogManager::Instance().m_boolDObjectiveLensSwitchLog;
	dlg.m_boolRobinsonDetectorSwitch	=	CAppLogManager::Instance().m_boolRobinsonDetectorSwitchLog;
	dlg.m_boolSignal2Brightness			=	CAppLogManager::Instance().m_boolSignal2BrightnessLog;
	dlg.m_boolSignal2Contrast			=	CAppLogManager::Instance().m_boolSignal2ContrastLog;
	dlg.m_boolYAdjust					=	CAppLogManager::Instance().m_boolYAdjustLog;
	dlg.m_boolImageReverse				=	CAppLogManager::Instance().m_boolImageReverseLog;
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	dlg.m_boolIsRecord = TRUE;
	if ( dlg.DoModal() != IDOK )
		return;
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	CAppLogManager::Instance().m_boolHVLog						=	dlg.m_boolHV;
	CAppLogManager::Instance().m_boolScanShiftLog				=	dlg.m_boolScanShift;
	CAppLogManager::Instance().m_boolBeemAlignmentLog			=	dlg.m_boolBeemAlignment;
	CAppLogManager::Instance().m_boolBrightnessLog				=	dlg.m_boolBrightness;
	CAppLogManager::Instance().m_boolContrastLog				=	dlg.m_boolContrast;
	CAppLogManager::Instance().m_boolStigmatorLog				=	dlg.m_boolStigmator;
	CAppLogManager::Instance().m_boolObjectiveLensLog			=	dlg.m_boolObjectiveLens;
	CAppLogManager::Instance().m_boolCondenserLensLog			=	dlg.m_boolCondenserLens;
	CAppLogManager::Instance().m_boolFilamentCurrentLog			=	dlg.m_boolFilamentCurrent;
	CAppLogManager::Instance().m_boolMagnifierLog				=	dlg.m_boolMagnifier;
	CAppLogManager::Instance().m_boolWorkDistanceLog			=	dlg.m_boolWorkDistance;
	CAppLogManager::Instance().m_boolObjectiveLensReverseLog	=	dlg.m_boolObjectiveLensReverse;
	CAppLogManager::Instance().m_boolCondenserLensReverseLog	=	dlg.m_boolCondenserLensReverse;
	CAppLogManager::Instance().m_boolAuxiliaryStigmationLog		=	dlg.m_boolAuxiliaryStigmation;
	CAppLogManager::Instance().m_boolDCondenserLensLog			=	dlg.m_boolDCondenserLens;
	CAppLogManager::Instance().m_boolScanRotateLog				=	dlg.m_boolScanRotate;
	CAppLogManager::Instance().m_boolSACPLog					=	dlg.m_boolSACP;
	CAppLogManager::Instance().m_boolDFocusLog					=	dlg.m_boolDFocus;
	CAppLogManager::Instance().m_boolDFocusSwitchLog			=	dlg.m_boolDFocusSwitch;
	CAppLogManager::Instance().m_boolDObjectiveLensLog			=	dlg.m_boolDObjectiveLens;
	CAppLogManager::Instance().m_boolDObjectiveLensSwitchLog	=	dlg.m_boolDObjectiveLensSwitch;
	CAppLogManager::Instance().m_boolRobinsonDetectorSwitchLog	=	dlg.m_boolRobinsonDetectorSwitch;
	CAppLogManager::Instance().m_boolSignal2BrightnessLog		=	dlg.m_boolSignal2Brightness;
	CAppLogManager::Instance().m_boolSignal2ContrastLog			=	dlg.m_boolSignal2Contrast;
	CAppLogManager::Instance().m_boolYAdjustLog					=	dlg.m_boolYAdjust;
	CAppLogManager::Instance().m_boolImageReverseLog			=	dlg.m_boolImageReverse;
	////////////////////////////////////////////////////////////////////
}

void CDlgImpPanelLog::OnLogMsg() 
{
	// TODO: Add your control notification handler code here
	CAppLogManager::Instance().m_boolLogMsgPcMc = !(CAppLogManager::Instance().m_boolLogMsgPcMc);
}

void CDlgImpPanelLog::OnLogClear() 
{
	CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_PANEL_LOG_EDIT_Log) );
	if( peditLog == NULL )
		return;

	CString csMessage = _T("");
	int len = peditLog->GetWindowTextLength();
	peditLog->SetSel(0,len);
	peditLog->ReplaceSel( (LPCTSTR)csMessage );
}

void CDlgImpPanelLog::OnLogSave() 
{
	// TODO: Add your control notification handler code here
	CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_PANEL_LOG_EDIT_Log) );
	if( peditLog == NULL )
		return;

	CString	strFilename;	
	strFilename.Format( "SemLog%s%03d",
		theApp.m_csCurrentDate, theApp.m_nSerialNumber );
	//文件类型
	CString strFiletype;
	strFiletype = GetResString( IDS_SD_LOG_FILE_TYPE );
//	char szFilter[] = "KYKY-EM3200日志文件 (*.SemLog)|*.SemLog||";
	TCHAR szFilter[100];
	sprintf( szFilter, "%s", strFiletype );
	CFileDialog dlg(FALSE, "SemLog", strFilename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						szFilter, NULL);
	if( dlg.DoModal() == IDOK )
	{
		CFile flLog;
		flLog.Open( dlg.GetPathName(), CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite );
		CString strLog;
		GetDlgItemText( IDC_PANEL_LOG_EDIT_Log, strLog );
		flLog.Write( strLog, strLog.GetLength() );
		flLog.Close();
	}
}

void CDlgImpPanelLog::AdjustCtrlsPos()
{
	if( m_hWnd == NULL )
		return;

	//根据屏幕分辨率动态调整对话框布局
	CRect rc, wndRect;
	GetClientRect( &rc );

	int nLeft = rc.left +20;
	int nTop = rc.top +5;
	int nWidth = rc.Width() /7;
	int nHeight = rc.Height() /5;
	//日志记录项目按钮
	CButton* pSelect = (CButton*)( GetDlgItem( IDC_PANEL_LOG_Btn_Select ) );
	if( pSelect != NULL )
		pSelect->MoveWindow( nLeft, nTop, nWidth, nHeight, TRUE );

	nTop += (nHeight +5);
	//底层通讯记录按钮
	CButton* pMsg = (CButton*)( GetDlgItem( IDC_PANEL_LOG_Btn_Msg ) );
	if( pMsg != NULL )
		pMsg->MoveWindow( nLeft, nTop, nWidth*2/3, nHeight, TRUE );
	//远程获取通讯记录按钮
	CButton* pMsgRG = (CButton*)( GetDlgItem( IDC_PANEL_LOG_Btn_Msg_RG ) );
	if( pMsgRG != NULL )
	{
		pMsgRG->MoveWindow( nLeft +nWidth*2/3, nTop, nWidth/3, nHeight, TRUE );
		pMsgRG->SetCheck(1);
	}

	nTop += (nHeight +5);
	//清空日志记录按钮
	CButton* pClear = (CButton*)( GetDlgItem( IDC_PANEL_LOG_Btn_Clear ) );
	if( pClear != NULL )
		pClear->MoveWindow( nLeft, nTop, nWidth, nHeight, TRUE );

	nTop += (nHeight +5);
	//保存日志按钮
	CButton* pSave = (CButton*)( GetDlgItem( IDC_PANEL_LOG_Btn_Save ) );
	if( pSave != NULL )
		pSave->MoveWindow( nLeft, nTop, nWidth, nHeight, TRUE );

	nLeft = rc.left + rc.Width() /5;
	//日志编辑框
	CEdit* pLog = (CEdit*)( GetDlgItem( IDC_PANEL_LOG_EDIT_Log ) );
	if( pLog != NULL )
		pLog->MoveWindow( nLeft, rc.top +3, rc.right -nLeft -3, rc.Height() -6, TRUE );
}

// 10.09.02 调试窗口一打开，就自动记录所有操作日志
void CDlgImpPanelLog::InitAutoLog()
{
	CString strFileName = m_fAutoLogFile.GetFileName();
	if( !strFileName.IsEmpty() )
		return;

	// 日志文件
	char appfilepath[255];
	::GetModuleFileName( NULL, appfilepath, 255 );
	char *ptemp = strrchr( appfilepath, '\\');
	ptemp[1] = 0;
	CString strAutoLogFileName;
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString strTime = timeCurrent.Format("%Y%m%d%H%M");
	strAutoLogFileName.Format( "%sLog\\AutoLogFile%s.log", appfilepath, strTime );
	m_fAutoLogFile.Open( strAutoLogFileName, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite );
	m_nLoopCounts = 0;

	if( m_fAutoLogFile.m_hFile != CFile::hFileNull )
	{
		CEdit* peditLog = (CEdit*)(GetDlgItem( IDC_PANEL_LOG_EDIT_Log) );
		if( peditLog != NULL )
		{
			CString strLog;
			GetDlgItemText( IDC_PANEL_LOG_EDIT_Log, strLog );
			m_fAutoLogFile.Write( strLog, strLog.GetLength() );
		}
	}

	// 底层通讯记录
//	( (CButton*)(GetDlgItem(IDC_PANEL_LOG_Btn_Msg)) )->SetCheck( TRUE );

	// 日志选项
	CAppLogManager::Instance().SetupLogs( TRUE );
}

void CDlgImpPanelLog::CloseAutoLog()
{
	// 日志文件
	if( m_fAutoLogFile.m_hFile != CFile::hFileNull )
		m_fAutoLogFile.Close();

	// 底层通讯记录
	( (CButton*)(GetDlgItem(IDC_PANEL_LOG_Btn_Msg)) )->SetCheck( FALSE );

	// 日志选项
	CAppLogManager::Instance().SetupLogs( FALSE );
}
// 10.09.02 调试窗口一打开，就自动记录所有操作日志

void CDlgImpPanelLog::OnPANELLOGBtnMsgRG() 
{
	// TODO: Add your control notification handler code here
	CAppLogManager::Instance().m_boolLogMsgRemoteGet = !(CAppLogManager::Instance().m_boolLogMsgRemoteGet);
}
