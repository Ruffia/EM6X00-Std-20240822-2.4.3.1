// PanelStage.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "PanelStage.h"
#include "ScanView.h"
#include "ScanParameterManager.h"
#include "SEMCTRL_Label.h"
#include "StageXYManager.h"
#include "CommonFunctionManager.h"
#include "CFGFileManager.h"
#include "ControlLayer.h"
#include "GlobalParamSetting.h"
#include "ScaleManager.h"
#include "SemCoreAssemble.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "MCICardCommonSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
}

CPanelStage::~CPanelStage()
{
}


void CPanelStage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelStage)
	DDX_Control(pDX, IDC_Stage_Positions, m_listCtrlStagePos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanelStage, CDialog)
	//{{AFX_MSG_MAP(CPanelStage)
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
	ON_BN_CLICKED(IDC_Stage_Stop, OnStageStop)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelStage message handlers

void CPanelStage::Localize(void)
{ 
	if( m_hWnd == NULL )
		return;

	// ��Ʒ̨
//	GetDlgItem(IDC_Stage_Group)->SetWindowText(GetResString(IDS_Stage_Group)); 
	GetDlgItem(IDC_Stage_Absolute)->SetWindowText(GetResString(IDS_Stage_Absolute)); 
	GetDlgItem(IDC_Stage_Relative)->SetWindowText(GetResString(IDS_Stage_Relative)); 
	GetDlgItem(IDC_Stage_Pos_Add)->SetWindowText(GetResString(IDS_Stage_Pos_Add)); 
	GetDlgItem(IDC_Stage_Pos_Delete)->SetWindowText(GetResString(IDS_Stage_Pos_Delete)); 
	GetDlgItem(IDC_Stage_Pos_Update)->SetWindowText(GetResString(IDS_Stage_Pos_Update)); 
	GetDlgItem(IDC_Stage_Pos_Goto)->SetWindowText(GetResString(IDS_Stage_Pos_Goto)); 
	GetDlgItem(IDC_Stage_Calibrate)->SetWindowText(GetResString(IDS_Stage_Calibrate)); 
	GetDlgItem(IDC_Stage_Reset)->SetWindowText(GetResString(IDS_Stage_Reset)); 
	GetDlgItem(IDC_Stage_Stop)->SetWindowText(GetResString(IDS_Stage_Stop)); 

	SetWindowText(GetResString(IDS_PANEL_Stage)); 
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

	if( CStageXYManager::Instance().m_Stage.nHasAutoStage < 1 || !CStageXYManager::Instance().m_Stage.xy.bReady )
	{
		// �Զ�̨��ز�������ʹ��
/*		CRect rcSep, rc;
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
*/
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
	}
	StagePosLoad();

	if( !CStageXYManager::Instance().m_Stage.xy.bReady )
		GetDlgItem( IDC_Stage_Reset )->EnableWindow( FALSE );

	m_hIconStage1	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_Left));
	m_hIconStage2	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_Right));
	m_hIconStage3	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_Up));
	m_hIconStage4	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_Stage_Down));
	((CButton*)( GetDlgItem( IDC_Stage_XLeft ) ))->SetIcon( m_hIconStage1 );
	((CButton*)( GetDlgItem( IDC_Stage_XRight ) ))->SetIcon( m_hIconStage2 );
	((CButton*)( GetDlgItem( IDC_Stage_YTop ) ))->SetIcon( m_hIconStage3 );
	((CButton*)( GetDlgItem( IDC_Stage_YBottom ) ))->SetIcon( m_hIconStage4 );
	///////////////////////////////////////////////////////////////////////////

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
			GetDlgItem( nID )->GetWindowText( str );
			break;
		default:
			break;
		}
		if( str.IsEmpty() )
			return CDialog::PreTranslateMessage(pMsg);
		double dValue = atof( str );
		switch( nID )
		{
		case	IDC_Stage_X:
			StagePosGotoX(dValue);
			break;
		case	IDC_Stage_Y:
			StagePosGotoY(dValue);
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
	CStageXYManager::Instance().Stage_XY_GetPosition( CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x, CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y );
	// ������
	CString str;
	if( bDir )	// X���������ƶ������磺������ϵ������Ҽ�ͷ
	{
		if( CStageXYManager::Instance().m_Stage.xy.bRev )	// XY����
			str.Format( "%s%ld", CStageXYManager::Instance().m_Stage.xy.sYRD, nScaleX );
		else
			str.Format( "%s%ld", CStageXYManager::Instance().m_Stage.xy.sXRD, nScaleX );
	}
	else		// X���������ƶ������磺������ϵ��������ͷ
	{
		if( CStageXYManager::Instance().m_Stage.xy.bRev)	// XY����
			str.Format( "%s%ld", CStageXYManager::Instance().m_Stage.xy.sYLD, nScaleX );
		else
			str.Format( "%s%ld", CStageXYManager::Instance().m_Stage.xy.sXLD, nScaleX );
	}
	nScaleX = atoi( str );
	// �õ�Ŀǰ��ʵ��λ��mm
	BOOL bExceed = FALSE;
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
	{
		if( bRelative )
			nAbsX = CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y + nScaleX;
		else
			nAbsX = nScaleX;
		// ��������
		if( nAbsX > (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY )
		{
			bExceed	= TRUE;
			nAbsX	= (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY;
		}
		else if( nAbsX < -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY )
		{
			bExceed	= TRUE;
			nAbsX	= -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY;
		}
	}
	else
	{
		if( bRelative )
			nAbsX = CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x + nScaleX;
		else
			nAbsX = nScaleX;
		// ��������
		if( nAbsX > (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX )
		{
			bExceed = TRUE;
			nAbsX	= (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX;
		}
		else if( nAbsX < -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX )
		{
			bExceed	= TRUE;
			nAbsX	= -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX;
		}
	}
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
	{
		if( nAbsX < -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY )
			GetDlgItem( IDC_Stage_XLeft )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_XLeft )->EnableWindow( TRUE );
		if( nAbsX > (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY )
			GetDlgItem( IDC_Stage_XRight )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_XRight )->EnableWindow( TRUE );
	}
	else
	{
		if( nAbsX < -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX )
			GetDlgItem( IDC_Stage_XLeft )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_XLeft )->EnableWindow( TRUE );
		if( nAbsX > (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX )
			GetDlgItem( IDC_Stage_XRight )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_XRight )->EnableWindow( TRUE );
	}
	if( bExceed )
		AfxMessageBox( "X�����г̷�Χ��" );

	BeginWaitCursor(); // display the hourglass cursor
	char szBuffer[64];
//	str.Format( "SP 10000,10000" );
	str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	if( CStageXYManager::Instance().m_Stage.xy.bRev)	// XY����
		str.Format( "SHY;PA ,%ld;BGY", nAbsX );
	else
		str.Format( "SHX;PA %ld;BGX", nAbsX );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	theApp.DisplayMsg(str);

	EndWaitCursor(); // remove the hourglass cursor
	SetTimer( 1, nAutoStageXY_FIElapse, NULL );
}

void CPanelStage::StageMoveY( int nScaleY, BOOL bRelative, BOOL bDir )
{
	int nAbsY;
	// ������������û������ֵ�������г̷�Χ��������������ʾ�û�
	// ��ȡ�Զ�̨X/Y��ǰλ��������
	CStageXYManager::Instance().Stage_XY_GetPosition( CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x, CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y );
	// ������
	CString str;
	if( bDir )	// Y���������ƶ������磺������ϵ������¼�ͷ
	{
		if( CStageXYManager::Instance().m_Stage.xy.bRev )	// XY����
			str.Format( "%s%ld", CStageXYManager::Instance().m_Stage.xy.sXRD, nScaleY );
		else
			str.Format( "%s%ld", CStageXYManager::Instance().m_Stage.xy.sYRD, nScaleY );
	}
	else		// Y���������ƶ������磺������ϵ������ϼ�ͷ
	{
		if( CStageXYManager::Instance().m_Stage.xy.bRev)	// XY����
			str.Format( "%s%ld", CStageXYManager::Instance().m_Stage.xy.sXLD, nScaleY );
		else
			str.Format( "%s%ld", CStageXYManager::Instance().m_Stage.xy.sYLD, nScaleY );
	}
	nScaleY = atoi( str );
	// �õ�Ŀǰ��ʵ��λ��mm
	BOOL bExceed = FALSE;
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
	{
		if( bRelative )
			nAbsY = CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x + nScaleY;
		else
			nAbsY = nScaleY;
		// ��������
		if( nAbsY > (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX )
		{
			bExceed	= TRUE;
			nAbsY	= (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX;
		}
		else if( nAbsY < -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX )
		{
			bExceed = TRUE;
			nAbsY	= -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX;
		}
	}
	else
	{
		if( bRelative )
			nAbsY = CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y + nScaleY;
		else
			nAbsY = nScaleY;
		// ��������
		if( nAbsY > (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY )
		{
			bExceed	= TRUE;
			nAbsY	= (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY;
		}
		else if( nAbsY < -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY )
		{
			bExceed	= TRUE;
			nAbsY	= -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY;
		}
	}
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
	{
		if( nAbsY < -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX )
			GetDlgItem( IDC_Stage_YBottom )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_YBottom )->EnableWindow( TRUE );
		if( nAbsY > (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeX /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX )
			GetDlgItem( IDC_Stage_YTop )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_YTop )->EnableWindow( TRUE );
	}
	else
	{
		if( nAbsY < -(1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY )
			GetDlgItem( IDC_Stage_YBottom )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_YBottom )->EnableWindow( TRUE );
		if( nAbsY > (1.0 *CStageXYManager::Instance().m_Stage.xy.nRangeY /2 -0.1)*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY )
			GetDlgItem( IDC_Stage_YTop )->EnableWindow( FALSE );
		else
			GetDlgItem( IDC_Stage_YTop )->EnableWindow( TRUE );
	}
	if( bExceed )
		AfxMessageBox( "Y�����г̷�Χ��" );

	BeginWaitCursor(); // display the hourglass cursor
	char szBuffer[64];
//	str.Format( "SP 10000,10000" );
	str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	if( CStageXYManager::Instance().m_Stage.xy.bRev)	// XY����
		str.Format( "SHX;PA %ld;BGX", nAbsY );
	else
		str.Format( "SHY;PA ,%ld;BGY", nAbsY );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	theApp.DisplayMsg(str);

	EndWaitCursor(); // remove the hourglass cursor
	SetTimer( 1, nAutoStageXY_FIElapse, NULL );
}


void CPanelStage::OnStageXLeft() 
{
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView == NULL )
		return;
	// ���ؾ���
	int nLengthX = CMCICardCommonSetting::Instance().m_sizeReso.cx /4;
	// ʵ�ʲ���
	int nScaleX = 0;
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
		CalculateStageUScale( FALSE, FALSE, nLengthX, nScaleX );
	else
		CalculateStageUScale( TRUE, FALSE, nLengthX, nScaleX );
	///////////////////////////////////
	CString s;
	s.Format( "Length = %d, Scale = %d, [meter = %.3f (%s)]", nLengthX, nScaleX, CGlobalParamSetting::Instance().g_dfMobileUScaleParameter, CGlobalParamSetting::Instance().g_csUScaleUnit );
	theApp.DisplayMsg(s);
	///////////////////////////////////
	StageMoveX( nScaleX, TRUE, FALSE );
	return;

	char szBuffer[64];
	CString str;
	str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	CStageXYManager::Instance().Stage_XY_Command( "PR", nScaleX, TRUE, FALSE );
}

void CPanelStage::OnStageXRight() 
{
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView == NULL )
		return;
	// ���ؾ���
	int nLengthX = CMCICardCommonSetting::Instance().m_sizeReso.cx /4;
	// ʵ�ʲ���
	int nScaleX = 0;
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
		CalculateStageUScale( FALSE, TRUE, nLengthX, nScaleX );
	else
		CalculateStageUScale( TRUE, TRUE, nLengthX, nScaleX );
	///////////////////////////////////
	CString s;
	s.Format( "Length = %d, Scale = %d, [meter = %.3f (%s)]", nLengthX, nScaleX, CGlobalParamSetting::Instance().g_dfMobileUScaleParameter, CGlobalParamSetting::Instance().g_csUScaleUnit );
	theApp.DisplayMsg(s);
	///////////////////////////////////
	StageMoveX( nScaleX, TRUE, TRUE );
	return;

	char szBuffer[64];
	CString str;
	str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	CStageXYManager::Instance().Stage_XY_Command( "PR", nScaleX, TRUE, TRUE );
}

void CPanelStage::OnStageYTop() 
{
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView == NULL )
		return;
	// ���ؾ���
	int nLengthY = CMCICardCommonSetting::Instance().m_sizeReso.cy /4;
	// ʵ�ʲ���
	int nScaleY = 0;
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
		CalculateStageUScale( TRUE, FALSE, nLengthY, nScaleY );
	else
		CalculateStageUScale( FALSE, FALSE, nLengthY, nScaleY );
	///////////////////////////////////
	CString s;
	s.Format( "Length = %d, Scale = %d, [meter = %.3f (%s)]", nLengthY, nScaleY, CGlobalParamSetting::Instance().g_dfMobileUScaleParameter, CGlobalParamSetting::Instance().g_csUScaleUnit );
	theApp.DisplayMsg(s);
	///////////////////////////////////
	StageMoveY( nScaleY, TRUE, FALSE );
	return;

	char szBuffer[64];
	CString str;
	str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	CStageXYManager::Instance().Stage_XY_Command( "PR", nScaleY, FALSE, FALSE );
}

void CPanelStage::OnStageYBottom() 
{
	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView == NULL )
		return;
	// ���ؾ���
	int nLengthY = CMCICardCommonSetting::Instance().m_sizeReso.cy /4;
	// ʵ�ʲ���
	int nScaleY = 0;
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
		CalculateStageUScale( TRUE, TRUE, nLengthY, nScaleY );
	else
		CalculateStageUScale( FALSE, TRUE, nLengthY, nScaleY );
	///////////////////////////////////
	CString s;
	s.Format( "Length = %d, Scale = %d, [meter = %.3f (%s)]", nLengthY, nScaleY, CGlobalParamSetting::Instance().g_dfMobileUScaleParameter, CGlobalParamSetting::Instance().g_csUScaleUnit );
	theApp.DisplayMsg(s);
	///////////////////////////////////
	StageMoveY( nScaleY, TRUE, TRUE );
	return;

	char szBuffer[64];
	CString str;
	str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	CStageXYManager::Instance().Stage_XY_Command( "PR", nScaleY, FALSE, TRUE );
}

void CPanelStage::StagePosLoad()
{
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

    CStageXYManager::Instance().m_nStagePosTotal = GetPrivateProfileInt( "StagePos", "TOTAL", 0, path );
	if( CStageXYManager::Instance().m_nStagePosTotal == 0 )
		return;

	int nTotal = CStageXYManager::Instance().m_nStagePosTotal;
/*	if( nTotal > 30 )
		nTotal = 30;
	CString*	pstrPosX	= new CString[nTotal];
	CString*	pstrPosY	= new CString[nTotal];
	CString*	pstrName	= new CString[nTotal];
	CString*	pstrShiftX	= new CString[nTotal];
	CString*	pstrShiftY	= new CString[nTotal];

	char stabuff[20];
	sprintf(stabuff, "StagePos");
	if( ReadInOneLine(path, stabuff, "PosX", STRSTYLE, nTotal, pstrPosX) != nTotal )
	{
		delete [] pstrPosX;
		delete [] pstrPosY;
		delete [] pstrName;
		delete [] pstrShiftX;
		delete [] pstrShiftY;
		return;
	}
	if( ReadInOneLine(path, stabuff, "PosY", STRSTYLE, nTotal, pstrPosY) != nTotal )
	{
		delete [] pstrPosX;
		delete [] pstrPosY;
		delete [] pstrName;
		delete [] pstrShiftX;
		delete [] pstrShiftY;
		return;
	}
	if( ReadInOneLine(path, stabuff, "Name", STRSTYLE, nTotal, pstrName) != nTotal )
	{
		delete [] pstrPosX;
		delete [] pstrPosY;
		delete [] pstrName;
		delete [] pstrShiftX;
		delete [] pstrShiftY;
		return;
	}
	if( ReadInOneLine(path, stabuff, "ShiftX", STRSTYLE, nTotal, pstrShiftX) != nTotal )
	{
		delete [] pstrPosX;
		delete [] pstrPosY;
		delete [] pstrName;
		delete [] pstrShiftX;
		delete [] pstrShiftY;
		return;
	}
	if( ReadInOneLine(path, stabuff, "ShiftY", STRSTYLE, nTotal, pstrShiftY) != nTotal )
	{
		delete [] pstrPosX;
		delete [] pstrPosY;
		delete [] pstrName;
		delete [] pstrShiftX;
		delete [] pstrShiftY;
		return;
	}

	LV_ITEM lvi;
	for( int i=0; i<nTotal; i++ )
	{
		structStagePos* pPos = new structStagePos();
		pPos->dwPosX		= atol(pstrPosX[i]);
		pPos->dwPosY		= atol(pstrPosY[i]);
		pPos->strPosName	= pstrName[i];
		pPos->dwShiftX		= atol(pstrShiftX[i]);
		pPos->dwShiftY		= atol(pstrShiftY[i]);
		CStageXYManager::Instance().m_plistStagePos->AddTail( pPos );

		ZeroMemory(&lvi,sizeof(LV_ITEM));
		lvi.mask = LVIF_TEXT;
		lvi.iItem = m_listCtrlStagePos.GetItemCount();
		lvi.iSubItem = 0;
		(LPCTSTR&)lvi.pszText = pstrName[i];
		lvi.cchTextMax=strlen(lvi.pszText);
		m_listCtrlStagePos.InsertItem(&lvi);
		m_listCtrlStagePos.SetItem(&lvi);
	}

	delete [] pstrPosX;
	delete [] pstrPosY;
	delete [] pstrName;
	delete [] pstrShiftX;
	delete [] pstrShiftY;
*/
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
			CStageXYManager::Instance().m_plistStagePos->AddTail( pPos );

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

void CPanelStage::StagePosListUpdate()
{
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	// ����λ������
	int nTotal = CStageXYManager::Instance().m_nStagePosTotal;
	CString str;
	str.Format( "%d", nTotal );
	WritePrivateProfileString( "StagePos", "Total", str, path );

	POSITION	pos			= CStageXYManager::Instance().m_plistStagePos->GetHeadPosition();
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
		structStagePos* pPos = CStageXYManager::Instance().m_plistStagePos->GetNext( pos );
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
		structStagePos* pPos = CStageXYManager::Instance().m_plistStagePos->GetNext( pos );

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
	POSITION pos = CStageXYManager::Instance().m_plistStagePos->GetHeadPosition();
	while( pos )
	{
		structStagePos* pPos = CStageXYManager::Instance().m_plistStagePos->GetAt( pos );
		if( pPos->strPosName == str )
		{
			// XY�Զ�̨λ��
			char szBuffer[64];
			CString str;
			str.Format( "SP 10000,10000" );
			CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
			str.Format( "SHXY;PA %ld,%ld;BGXY", pPos->dwPosX, pPos->dwPosY );
			CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
			SetTimer( 1, nAutoStageXY_FIElapse, NULL );
			// ��λ��λ��
			VARIANT ClassID, SerialNumber, Xpos, Ypos, Flag;
			SerialNumber.lVal = -1;
			ClassID.lVal = SemControlPanelClass_ScanShift;
			Flag.lVal = 3;
			Xpos.lVal = pPos->dwShiftX;
			Ypos.lVal = pPos->dwShiftY;
			CSemCoreCtrlPanelMessageTransmiter::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
			break;
		}
		CStageXYManager::Instance().m_plistStagePos->GetNext( pos );
	}
}

void CPanelStage::OnStagePosAdd() 
{
	// ��ȡ�Զ�̨X/Y��ǰλ��������
	CStageXYManager::Instance().Stage_XY_GetPosition( CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x, CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y );

	// ����һ����λ��
	CString str;
	str.Format( "Position %d", ++CStageXYManager::Instance().m_nStagePosTotal );
	structStagePos* pPos = new structStagePos();
	// XY�Զ�̨λ�ò���
	pPos->dwPosX = CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x;
	pPos->dwPosY = CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y;
	// ����
	pPos->strPosName = str;
	// ��λ�Ʋ���
	VARIANT varData = CSemCoreAssemble::Instance().m_ScanShiftX.GetPos();
	pPos->dwShiftX	= varData.lVal;
	varData = CSemCoreAssemble::Instance().m_ScanShiftY.GetPos();
	pPos->dwShiftY	= varData.lVal;
	CStageXYManager::Instance().m_plistStagePos->AddTail( pPos );

	LV_ITEM lvi;
	ZeroMemory(&lvi,sizeof(LV_ITEM));
	lvi.mask = LVIF_TEXT;
	lvi.iItem = m_listCtrlStagePos.GetItemCount();
	lvi.iSubItem = 0;
	(LPCTSTR&)lvi.pszText = str;
	lvi.cchTextMax=strlen(lvi.pszText);
	m_listCtrlStagePos.InsertItem(&lvi);
	m_listCtrlStagePos.SetItem(&lvi);

	StagePosListUpdate();
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
	pos = CStageXYManager::Instance().m_plistStagePos->GetHeadPosition();
	while( pos )
	{
		structStagePos* pPos = CStageXYManager::Instance().m_plistStagePos->GetAt( pos );
		if( pPos->strPosName == str )
		{
			CStageXYManager::Instance().m_plistStagePos->RemoveAt( pos );
			delete pPos;
			break;
		}
		CStageXYManager::Instance().m_plistStagePos->GetNext( pos );
	}

	CStageXYManager::Instance().m_nStagePosTotal = CStageXYManager::Instance().m_plistStagePos->GetCount();
	StagePosListUpdate();
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
	pos = CStageXYManager::Instance().m_plistStagePos->GetHeadPosition();
	while( pos )
	{
		structStagePos* pPos = CStageXYManager::Instance().m_plistStagePos->GetAt( pos );
		if( pPos->strPosName == str )
		{
			GetDlgItem( IDC_Stage_X )->GetWindowText( str );
			pPos->dwPosX = (int)(atof(str) *1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX);	// �����׻����ʵ���ƶ�����
			GetDlgItem( IDC_Stage_Y )->GetWindowText( str );
			pPos->dwPosY = (int)(atof(str) *1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY);	// �����׻����ʵ���ƶ�����
			break;
		}
		CStageXYManager::Instance().m_plistStagePos->GetNext( pos );
	}

	StagePosListUpdate();
}

void CPanelStage::OnStageCalibrate() 
{
	// TODO: Add your control notification handler code here
	// ��դ����������Ʒ�����Ĳ��غ�ʱ��ʹ�ø��ַ�������Ʒ������������Ļ����
	// Ȼ��ʹ�øá�У׼�����ܣ�ϵͳ����ʱ�Ĺ�դ��λ�ü�¼�������ļ���
	// �Ժ�ÿ�θ�λ����ԭ�㣩�������ٶ���һ�ξ���λ��
	// �ߵ�֮�󣬶���õ�Ϊ0,0��DP 0,0��

	// ��ȡ�Զ�̨X/Y��ǰλ��������
	CStageXYManager::Instance().Stage_XY_GetPosition( CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x, CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y );

	// ��¼�������ļ���
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	long*	plongCali	= new long[2];
	plongCali[0]	= CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x;
	plongCali[1]	= CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y;
	char stabuff[20];
	sprintf(stabuff, "StagePos");
	CCommonFunctionManager::Instance().SaveInOneLine( path, stabuff, "Cali", LONGSTYLE, 2, plongCali );
	delete [] plongCali;

	// ����õ�Ϊ0,0
	CStageXYManager::Instance().m_DMCWin.Command( "DP 0,0" );

	// ��ʾ
	CString str;
	str.Format( "Write Cali=%ld,%ld to ini file", CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x, CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y );
	theApp.DisplayMsg( str );
}

void CPanelStage::OnStageReset() 
{
	// TODO: Add your control notification handler code here
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 0 && CStageXYManager::Instance().m_Stage.xy.bReady)
	{
		// 15.10 XY�Զ�̨
		CStageXYManager::Instance().Stage_XY_FindIndex();
	}
}

void CPanelStage::OnStageDP() 
{
	// TODO: Add your control notification handler code here
	// ���嵱ǰλ��Ϊԭ��
// 	CStageXYManager::Instance().m_DMCWin.Command( "DP 0,0" );
// 	CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x = 0;
// 	CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y = 0;

	// 190515 ʵ�飺���˶������У�ͨ����ѯ_BGn����֪��ĳһ�����Ƿ����˶�
	// MG_BGn��ʾ1��ʾ�����˶�����ʾ0��ʾ��ֹͣ�˶�
	char szBuffer[64];
	CString str;
	str.Format( "SHX;JG 100;BGX" );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	Sleep(1000);
	str.Format( "MG_BGX" );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	str.Format( "%d", atoi(szBuffer));
	str.Format( "MG_BGY" );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	str.Format( "%d", atoi(szBuffer));
	Sleep(1000);
	str.Format( "ST" );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	str.Format( "MG_BGX" );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	str.Format( "%d", atoi(szBuffer));
	str.Format( "MG_BGY" );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	str.Format( "%d", atoi(szBuffer));
}


void CPanelStage::OnStageStop()
{
	CStageXYManager::Instance().m_DMCWin.Command( "ST" );
	Sleep( 100 );
	CStageXYManager::Instance().m_DMCWin.Command( "MO" );
	theApp.DisplayMsg( "Stop and MotorOff" );
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
	POSITION pos = CStageXYManager::Instance().m_plistStagePos->GetHeadPosition();
	while( pos )
	{
		structStagePos* pPos = CStageXYManager::Instance().m_plistStagePos->GetAt( pos );
		if( pPos->strPosName == m_strName )
		{
			pPos->strPosName = str;
			break;
		}
		CStageXYManager::Instance().m_plistStagePos->GetNext( pos );
	}

	StagePosListUpdate();

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
// 		if( !CStageXYManager::Instance().m_Stage.xy.bReady
// 			&& !CStageXYManager::Instance().m_Stage.zrt.bReadyZ && !CStageXYManager::Instance().m_Stage.zrt.bReadyR && !CStageXYManager::Instance().m_Stage.zrt.bReadyT )
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
	CString str;
	GetDlgItem( IDC_Stage_X )->GetWindowText( str );
	double dX = atof( str );
	GetDlgItem( IDC_Stage_Y )->GetWindowText( str );
	double dY = atof( str );
	StagePosGotoX( dX );
	StagePosGotoY( dY );
}

void CPanelStage::StagePosGotoX( double dValue )
{
	// ���༭���������ֵ����
	int nScaleX;
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
	{
		nScaleX = (int)(dValue *1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY);	// �����׻����ʵ���ƶ�����
	}
	else
	{
		nScaleX = (int)(dValue *1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX);	// �����׻����ʵ���ƶ�����
	}
	if( nScaleX > 0 )
		StageMoveX( nScaleX, m_bStageRelative, TRUE );
	else
		StageMoveX( abs(nScaleX), m_bStageRelative, FALSE );
	return;

	CString str;
	GetDlgItem( IDC_Stage_X )->GetWindowText( str );
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
	{
		nScaleX = (int)(atof(str) *1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY);	// �����׻����ʵ���ƶ�����
		if( nScaleX > 0 )
			StageMoveY( nScaleX, m_bStageRelative, TRUE );
		else
			StageMoveY( nScaleX, m_bStageRelative, FALSE );
	}
	else
	{
		nScaleX = (int)(atof(str) *1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX);	// �����׻����ʵ���ƶ�����
		if( nScaleX > 0 )
			StageMoveX( nScaleX, m_bStageRelative, TRUE );
		else
			StageMoveX( nScaleX, m_bStageRelative, FALSE );
	}
	return;

	// ������������û������ֵ�������г̷�Χ��������������ʾ�û�
	// ��ȡ�Զ�̨X/Y��ǰλ��������
	CStageXYManager::Instance().Stage_XY_GetPosition( CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x, CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y );
	// �õ�Ŀǰ��ʵ��λ��mm
	int nActualX;
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
		nActualX = CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y / CStageXYManager::Instance().m_Stage.xy.nGratingY;
	else
		nActualX = CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x / CStageXYManager::Instance().m_Stage.xy.nGratingX;
	if( m_bStageRelative )
	{
		// �������
		if( (nActualX + nScaleX) > 74.5*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX		// 1450000
			|| (nActualX + nScaleX) < -74.5*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX )// -1450000 )
		{
			AfxMessageBox( "X�����г̷�Χ��" );
			return;
		}
	}
	else
	{
		// ��������
		if( nScaleX > 74.5*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX		// 1450000
			|| nScaleX < -74.5*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX )	// -1450000 )
		{
			AfxMessageBox( "X�����г̷�Χ��" );
			return;
		}
	}

	char szBuffer[64];
	str.Format( "SP 10000,10000" );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	if( m_bStageRelative )
		str.Format( "SHX;PR %ld;BGX", nScaleX );	// �������
	else
		str.Format( "SHX;PA %ld;BGX", nScaleX );	// ��������
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
}

void CPanelStage::StagePosGotoY( double dValue )
{
	// ���༭���������ֵ����
	int nScaleY;
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
	{
		nScaleY = (int)(dValue *1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingX);	// �����׻����ʵ���ƶ�����
	}
	else
	{
		nScaleY = (int)(dValue *1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY);	// �����׻����ʵ���ƶ�����
	}
	if( nScaleY > 0 )
		StageMoveY( nScaleY, m_bStageRelative, TRUE );
	else
		StageMoveY( abs(nScaleY), m_bStageRelative, FALSE );
	return;

	CString str;
	// ������������û������ֵ�������г̷�Χ��������������ʾ�û�
	// ��ȡ�Զ�̨X/Y��ǰλ��������
	CStageXYManager::Instance().Stage_XY_GetPosition( CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x, CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y );
	// �õ�Ŀǰ��ʵ��λ��mm
	int nActualY;
	if( CStageXYManager::Instance().m_Stage.xy.bRev )
		nActualY = CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x / CStageXYManager::Instance().m_Stage.xy.nGratingX;
	else
		nActualY = CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y / CStageXYManager::Instance().m_Stage.xy.nGratingY;
	if( m_bStageRelative )
	{
		// �������
		if( ( (nActualY + nScaleY) > 74.5*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY )		// 1450000 )
			|| ( (nActualY + nScaleY) < -74.5*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY )	// -1450000 )
			|| ( CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x < 0
				&& CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y < 0 
				&& (nActualY +nScaleY) < -30.0*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY ) )// -600000 ) )
		{
			AfxMessageBox( "Y�����г̷�Χ��" );
			return;
		}
	}
	else
	{
		// ��������
		if( nScaleY > 74.5*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY			// 1450000
			|| nScaleY < -74.5*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY		// -1450000
			|| ( CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.x < 0
				&& CStageXYManager::Instance().m_Stage.xy.ptCurPosAb.y < 0 
				&& nScaleY < -30.0*1000000 /CStageXYManager::Instance().m_Stage.xy.nGratingY ) )// -600000 ) )
		{
			AfxMessageBox( "Y�����г̷�Χ��" );
			return;
		}
	}

	char szBuffer[64];
	str.Format( "SP 10000,10000" );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	if( m_bStageRelative )
		str.Format( "SHY;PR ,%ld;BGY", nScaleY );	// �������
	else
		str.Format( "SHY;PA ,%ld;BGY", nScaleY );	// ��������
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
}

void CPanelStage::StagePosUpdate()
{
	long lX, lY;
	CStageXYManager::Instance().Stage_XY_GetPosition(lX, lY, FALSE);
	// ��ʵ���ƶ���������ɺ�����ʾ
	double dX = 1.0 *lX * CStageXYManager::Instance().m_Stage.xy.nGratingX / 1000000.0;
	double dY = 1.0 *lY * CStageXYManager::Instance().m_Stage.xy.nGratingY / 1000000.0;
	CString str;
	if(  CStageXYManager::Instance().m_Stage.xy.bRev )	// XY����
	{
		if(  CStageXYManager::Instance().m_Stage.xy.bRevX && !( CStageXYManager::Instance().m_Stage.xy.bRevY) )			// X����Y����
		{
			 CStageXYManager::Instance().m_Stage.xy.fDispPos[0] = dY;
			str.Format( "%.3f",  CStageXYManager::Instance().m_Stage.xy.fDispPos[0] );
			GetDlgItem( IDC_Stage_X )->SetWindowText( str );
			 CStageXYManager::Instance().m_Stage.xy.fDispPos[1] = -1 *dX;
			str.Format( "%.3f",  CStageXYManager::Instance().m_Stage.xy.fDispPos[1] );
			GetDlgItem( IDC_Stage_Y )->SetWindowText( str );
		}
		else if(  CStageXYManager::Instance().m_Stage.xy.bRevX &&  CStageXYManager::Instance().m_Stage.xy.bRevY )		// XY������
		{
			 CStageXYManager::Instance().m_Stage.xy.fDispPos[0] = -1 *dY;
			str.Format( "%.3f",  CStageXYManager::Instance().m_Stage.xy.fDispPos[0] );
			GetDlgItem( IDC_Stage_X )->SetWindowText( str );
			 CStageXYManager::Instance().m_Stage.xy.fDispPos[1] = -1 *dX;
			str.Format( "%.3f",  CStageXYManager::Instance().m_Stage.xy.fDispPos[1] );
			GetDlgItem( IDC_Stage_Y )->SetWindowText( str );
		}
		else if( !( CStageXYManager::Instance().m_Stage.xy.bRevX) &&  CStageXYManager::Instance().m_Stage.xy.bRevY )	// X����Y����
		{
			 CStageXYManager::Instance().m_Stage.xy.fDispPos[0] = -1 *dY;
			str.Format( "%.3f",  CStageXYManager::Instance().m_Stage.xy.fDispPos[0] );
			GetDlgItem( IDC_Stage_X )->SetWindowText( str );
			 CStageXYManager::Instance().m_Stage.xy.fDispPos[1] = dX;
			str.Format( "%.3f",  CStageXYManager::Instance().m_Stage.xy.fDispPos[1] );
			GetDlgItem( IDC_Stage_Y )->SetWindowText( str );
		}
		else
		{
			// XY������
			 CStageXYManager::Instance().m_Stage.xy.fDispPos[0] = dY;
			str.Format( "%.3f",  CStageXYManager::Instance().m_Stage.xy.fDispPos[0] );
			GetDlgItem( IDC_Stage_X )->SetWindowText( str );
			 CStageXYManager::Instance().m_Stage.xy.fDispPos[1] = dX;
			str.Format( "%.3f",  CStageXYManager::Instance().m_Stage.xy.fDispPos[1] );
			GetDlgItem( IDC_Stage_Y )->SetWindowText( str );
		}
	}
	else
	{
		// X
		if(  CStageXYManager::Instance().m_Stage.xy.bRevX )
			 CStageXYManager::Instance().m_Stage.xy.fDispPos[0] = -1 *dX;
		else
			 CStageXYManager::Instance().m_Stage.xy.fDispPos[0] = dX;
		str.Format( "%.3f",  CStageXYManager::Instance().m_Stage.xy.fDispPos[0] );
		GetDlgItem( IDC_Stage_X )->SetWindowText( str );
		// Y
		if(  CStageXYManager::Instance().m_Stage.xy.bRevY )
			 CStageXYManager::Instance().m_Stage.xy.fDispPos[1] = -1 *dY;
		else
			 CStageXYManager::Instance().m_Stage.xy.fDispPos[1] = dY;
		str.Format( "%.3f",  CStageXYManager::Instance().m_Stage.xy.fDispPos[1] );
		GetDlgItem( IDC_Stage_Y )->SetWindowText( str );
	}
}

void CPanelStage::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == 1 )
	{
		StagePosUpdate();

		// ����Զ�̨��ֹͣ�˶����ͽ���ˢ��
		char szBuffer[64];
		CString str;
		str.Format( "MG_BGX" );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		int nIsXMoving = atoi(szBuffer);
		str.Format( "MG_BGY" );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
		int nIsYMoving = atoi(szBuffer);
		if( !(nIsXMoving > 0 || nIsYMoving > 0) )	// XY�Զ�̨�����˶���
			KillTimer(1);
	}
	
	CDialog::OnTimer(nIDEvent);
}
