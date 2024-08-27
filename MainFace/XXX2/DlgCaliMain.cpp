// DlgCaliMain.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgCaliMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliMain dialog


CDlgCaliMain::CDlgCaliMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCaliMain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCaliMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pTabDialog	= NULL;

	m_pBtnLns		= NULL;
	m_pBtnScn		= NULL;
	m_pBtnMci		= NULL;
	m_pBtnSpr		= NULL;
	m_pBtnCo		= NULL;

	m_pPageLNS		= NULL;
	m_pPageSCN		= NULL;
	m_pPageMCI		= NULL;
	m_pPageSPR		= NULL;
	m_pPageCo		= NULL;
}

CDlgCaliMain::~CDlgCaliMain()
{
	if( m_pBtnLns	!= NULL )
		delete m_pBtnLns;
	if( m_pBtnScn	!= NULL )
		delete m_pBtnScn;
	if( m_pBtnMci	!= NULL )
		delete m_pBtnMci;
	if( m_pBtnSpr	!= NULL )
		delete m_pBtnSpr;
	if( m_pBtnCo	!= NULL )
		delete m_pBtnCo;

	if( m_pPageLNS		!= NULL )
	{
		m_pPageLNS->DestroyWindow();
		delete m_pPageLNS;
		m_pPageLNS = NULL;
	}
	if( m_pPageSCN		!= NULL )
	{
		m_pPageSCN->DestroyWindow();
		delete m_pPageSCN;
		m_pPageSCN = NULL;
	}
	if( m_pPageMCI		!= NULL )
	{
		m_pPageMCI->DestroyWindow();
		delete m_pPageMCI;
		m_pPageMCI = NULL;
	}
	if( m_pPageSPR		!= NULL )
	{
		m_pPageSPR->DestroyWindow();
		delete m_pPageSPR;
		m_pPageSPR = NULL;
	}
	if( m_pPageCo		!= NULL )
	{
		m_pPageCo->DestroyWindow();
		delete m_pPageCo;
		m_pPageCo = NULL;
	}
	if( m_pTabDialog	!= NULL )
	{
		m_pTabDialog->DestroyWindow();
		delete m_pTabDialog;
		m_pTabDialog = NULL;
	}
}


void CDlgCaliMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCaliMain)
	DDX_Control(pDX, IDC_STATIC_AT_Cali_Main_TabPos, m_staticTabPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCaliMain, CDialog)
	//{{AFX_MSG_MAP(CDlgCaliMain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliMain message handlers

void CDlgCaliMain::Localize(void)
{
	if( m_hWnd == NULL )
		return;

// 	m_pBtnMon->SetWindowText(GetResString( IDS_BUTTON_Debug_Tab_Mon ));
// 	m_pBtnAdvance->SetWindowText(GetResString( IDS_BUTTON_Debug_Tab_Advance ));
// 	m_pBtnDebug->SetWindowText(GetResString( IDS_BUTTON_Debug_Tab_Debug ));
	
	if( m_pPageCo != NULL && m_pPageCo->m_hWnd != NULL )
		m_pPageCo->Localize();
}

BOOL CDlgCaliMain::OnInitDialog() 
{
	CDialog::OnInitDialog();
//	EnableToolTips(TRUE);		//允许在线提示
	
	// TODO: Add extra initialization here
	InitTabDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////
// Initialize the TabDialog
BOOL CDlgCaliMain::InitTabDialog()
{
	//create the TabDialog
	m_pTabDialog = new CDlgAutoTestTab(IDD_AutoTest_TabDlg, this);
	if (m_pTabDialog->Create(IDD_AutoTest_TabDlg, this) == FALSE)
		return FALSE;
	
	if(!AddPagesToTabDialog())
		return FALSE;
	
	//get TabDialog's position from the static control
	CRect rect;
	m_staticTabPos.GetWindowRect(&rect);
	ScreenToClient((LPRECT) rect);
	
	//set the TabDialog's positon
	m_pTabDialog->SetWindowPos(this, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);
	
	//initialize the showing of TabDialog
	m_pTabDialog->InitPagesShow(0);
	m_pBtnLns->SetCheck( 1 );
	
	return TRUE;
}

////////////////////////////////////////////////////////////
//Add pages (include button and dialog) to TabDialog
BOOL CDlgCaliMain::AddPagesToTabDialog()
{
	/////////////////////////////////////////////////////////////////
	//Create button
	m_pBtnLns = new CMacRadioButton();
	RECT rectLns;
	rectLns.left	= BTNLns_LOCATION.x;
	rectLns.right	= BTNLns_LOCATION.x+BUTTON_WIDTH;
	rectLns.top		= BTNLns_LOCATION.y;
	rectLns.bottom	= BTNLns_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnLns->Create("LNS", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectLns, m_pTabDialog, IDC_BUTTON_AT_Cali_Main_Tab_LNS);
	m_pBtnLns->SetColor( RGB(0,128,0) );

	//create dialog
	m_pPageLNS = new CDlgCaliLNS(m_pTabDialog);
	if(m_pPageLNS->Create(IDD_Cali_LNS, m_pTabDialog) == FALSE)
		return FALSE;

	//add page
	m_pTabDialog->AddPage(m_pPageLNS, m_pBtnLns);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//Create button
	m_pBtnScn = new CMacRadioButton();
	RECT rectScn;
	rectScn.left	= BTNScn_LOCATION.x;
	rectScn.right	= BTNScn_LOCATION.x+BUTTON_WIDTH;
	rectScn.top		= BTNScn_LOCATION.y;
	rectScn.bottom	= BTNScn_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnScn->Create("SCN", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectScn, m_pTabDialog, IDC_BUTTON_AT_Cali_Main_Tab_SCN);
	m_pBtnScn->SetColor( RGB(255,128,0) );

	//create dialog
	m_pPageSCN = new CDlgCaliSCN(m_pTabDialog);
	if(m_pPageSCN->Create(IDD_Cali_SCN, m_pTabDialog) == FALSE)
		return FALSE;

	//add page
	m_pTabDialog->AddPage(m_pPageSCN, m_pBtnScn);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//Create button
	m_pBtnMci = new CMacRadioButton();
	RECT rectMci;
	rectMci.left	= BTNMci_LOCATION.x;
	rectMci.right	= BTNMci_LOCATION.x+BUTTON_WIDTH;
	rectMci.top		= BTNMci_LOCATION.y;
	rectMci.bottom	= BTNMci_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnMci->Create("MCI", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectMci, m_pTabDialog, IDC_BUTTON_AT_Cali_Main_Tab_MCI);
	m_pBtnMci->SetColor( RGB(255,128,0) );

	//create dialog
	m_pPageMCI = new CDlgCaliMCI(m_pTabDialog);
	if(m_pPageMCI->Create(IDD_Cali_MCI, m_pTabDialog) == FALSE)
		return FALSE;

	//add page
	m_pTabDialog->AddPage(m_pPageMCI, m_pBtnMci);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//Create button
	m_pBtnSpr = new CMacRadioButton();
	RECT rectSpr;
	rectSpr.left	= BTNSpr_LOCATION.x;
	rectSpr.right	= BTNSpr_LOCATION.x+BUTTON_WIDTH;
	rectSpr.top		= BTNSpr_LOCATION.y;
	rectSpr.bottom	= BTNSpr_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnSpr->Create("SPR", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectSpr, m_pTabDialog, IDC_BUTTON_AT_Cali_Main_Tab_SPR);
	m_pBtnSpr->SetColor( RGB(128,0,0) );

	//create dialog
	m_pPageSPR = new CDlgCaliSPR(m_pTabDialog);
	if(m_pPageSPR->Create(IDD_Cali_SPR, m_pTabDialog) == FALSE)
		return FALSE;

	//add page
	m_pTabDialog->AddPage(m_pPageSPR, m_pBtnSpr);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//Create button
	m_pBtnCo = new CMacRadioButton();
	RECT rectCo;
	rectCo.left		= BTNCo_LOCATION.x;
	rectCo.right	= BTNCo_LOCATION.x+BUTTON_WIDTH;
	rectCo.top		= BTNCo_LOCATION.y;
	rectCo.bottom	= BTNCo_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnCo->Create("Coadjust", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectCo, m_pTabDialog, IDC_BUTTON_AT_Cali_Main_Tab_Co);
	m_pBtnCo->SetColor( RGB(0,0,255) );

	//create dialog
	m_pPageCo = new CDlgCaliCoadjust(m_pTabDialog);
	if(m_pPageCo->Create(IDD_Cali_Coadjust, m_pTabDialog) == FALSE)
		return FALSE;

	//add page
	m_pTabDialog->AddPage(m_pPageCo, m_pBtnCo);
	/////////////////////////////////////////////////////////////////

	return TRUE;
}

void CDlgCaliMain::InitParams()
{
	m_pPageLNS->InitParams();
	m_pPageSCN->InitParams();
	m_pPageMCI->InitParams();
	m_pPageSPR->InitParams();
	m_pPageCo->InitParams();
}

void CDlgCaliMain::UpdateCaliData( int nIndex )
{
	if( nIndex == 4 )
		m_pPageCo->UpdateCaliData();
}