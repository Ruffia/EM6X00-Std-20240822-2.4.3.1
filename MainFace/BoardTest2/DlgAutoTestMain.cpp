// DlgAutoTestMain.cpp : implementation file
//

#include "stdafx.h"
#include "insample0.h"
#include "DlgAutoTestMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestMain dialog


CDlgAutoTestMain::CDlgAutoTestMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoTestMain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoTestMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pTabDialog	= NULL;
	m_pBtnPower		= NULL;
	m_pBtnVac		= NULL;
	m_pBtnLns		= NULL;
	m_pBtnScn		= NULL;
	m_pBtnMci		= NULL;
	m_pBtnSpr		= NULL;
	m_pPagePower	= NULL;
	m_pPageVAC		= NULL;
	m_pPageLNS		= NULL;
	m_pPageSCN		= NULL;
	m_pPageMCI		= NULL;
	m_pPageSPR		= NULL;
}

CDlgAutoTestMain::~CDlgAutoTestMain()
{
	if( m_pBtnPower		!= NULL )
		delete m_pBtnPower;
	if( m_pBtnVac	!= NULL )
		delete m_pBtnVac;
	if( m_pBtnLns	!= NULL )
		delete m_pBtnLns;
	if( m_pBtnScn	!= NULL )
		delete m_pBtnScn;
	if( m_pBtnMci	!= NULL )
		delete m_pBtnMci;
	if( m_pBtnSpr	!= NULL )
		delete m_pBtnSpr;
	if( m_pPagePower		!= NULL )
	{
		m_pPagePower->DestroyWindow();
		delete m_pPagePower;
		m_pPagePower = NULL;
	}
	if( m_pPageVAC		!= NULL )
	{
		m_pPageVAC->DestroyWindow();
		delete m_pPageVAC;
		m_pPageVAC = NULL;
	}
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
	if( m_pTabDialog	!= NULL )
	{
		m_pTabDialog->DestroyWindow();
		delete m_pTabDialog;
		m_pTabDialog = NULL;
	}
}


void CDlgAutoTestMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoTestMain)
	DDX_Control(pDX, IDC_STATIC_AT_Cali_Main_TabPos, m_staticTabPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutoTestMain, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoTestMain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoTestMain message handlers

BOOL CDlgAutoTestMain::OnInitDialog() 
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
BOOL CDlgAutoTestMain::InitTabDialog()
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
	m_pBtnPower->SetCheck( 1 );
//	m_pBtnVac->SetCheck( 1 );
//	m_pBtnLns->SetCheck( 1 );
	
	return TRUE;
}

////////////////////////////////////////////////////////////
//Add pages (include button and dialog) to TabDialog
BOOL CDlgAutoTestMain::AddPagesToTabDialog()
{
	/////////////////////////////////////////////////////////////////
	//create first button
	m_pBtnPower = new CMacRadioButton();
	RECT rectPower;
	rectPower.left		= BTNPower_LOCATION.x;
	rectPower.right		= BTNPower_LOCATION.x+BUTTON_WIDTH;
	rectPower.top		= BTNPower_LOCATION.y;
	rectPower.bottom	= BTNPower_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnPower->Create("POWER", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectPower, m_pTabDialog, IDC_BUTTON_AT_Cali_Main_Tab_Power);
	m_pBtnPower->SetColor( RGB(128,0,0) );

	//create first dialog
	m_pPagePower = new CDlgAutoTestPower(m_pTabDialog);
	if(m_pPagePower->Create(IDD_AutoTest_Power, m_pTabDialog) == FALSE)
		return FALSE;

	//add first page
	m_pTabDialog->AddPage(m_pPagePower, m_pBtnPower);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//Create second button
	m_pBtnVac = new CMacRadioButton();
	RECT rectVac;
	rectVac.left	= BTNVac_LOCATION.x;
	rectVac.right	= BTNVac_LOCATION.x+BUTTON_WIDTH;
	rectVac.top		= BTNVac_LOCATION.y;
	rectVac.bottom	= BTNVac_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnVac->Create("VAC", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectVac, m_pTabDialog, IDC_BUTTON_AT_Cali_Main_Tab_VAC);
	m_pBtnVac->SetColor( RGB(0,0,255) );

	//create second dialog
	m_pPageVAC = new CDlgAutoTestVAC(m_pTabDialog);
	if(m_pPageVAC->Create(IDD_AutoTest_VAC, m_pTabDialog) == FALSE)
		return FALSE;

	//add second page
	m_pTabDialog->AddPage(m_pPageVAC, m_pBtnVac);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//Create third button
	m_pBtnLns = new CMacRadioButton();
	RECT rectLns;
	rectLns.left	= BTNLns_LOCATION.x;
	rectLns.right	= BTNLns_LOCATION.x+BUTTON_WIDTH;
	rectLns.top		= BTNLns_LOCATION.y;
	rectLns.bottom	= BTNLns_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnLns->Create("LNS", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectLns, m_pTabDialog, IDC_BUTTON_AT_Cali_Main_Tab_LNS);
	m_pBtnLns->SetColor( RGB(0,128,0) );

	//create third dialog
	m_pPageLNS = new CDlgAutoTestLNS(m_pTabDialog);
	if(m_pPageLNS->Create(IDD_AutoTest_LNS, m_pTabDialog) == FALSE)
		return FALSE;

	//add third page
	m_pTabDialog->AddPage(m_pPageLNS, m_pBtnLns);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//Create fourth button
	m_pBtnScn = new CMacRadioButton();
	RECT rectScn;
	rectScn.left	= BTNScn_LOCATION.x;
	rectScn.right	= BTNScn_LOCATION.x+BUTTON_WIDTH;
	rectScn.top		= BTNScn_LOCATION.y;
	rectScn.bottom	= BTNScn_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnScn->Create("SCN", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectScn, m_pTabDialog, IDC_BUTTON_AT_Cali_Main_Tab_SCN);
	m_pBtnScn->SetColor( RGB(255,128,0) );

	//create third dialog
	m_pPageSCN = new CDlgAutoTestSCN(m_pTabDialog);
	if(m_pPageSCN->Create(IDD_AutoTest_SCN, m_pTabDialog) == FALSE)
		return FALSE;

	//add third page
	m_pTabDialog->AddPage(m_pPageSCN, m_pBtnScn);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//Create fifth button
	m_pBtnMci = new CMacRadioButton();
	RECT rectMci;
	rectMci.left	= BTNMci_LOCATION.x;
	rectMci.right	= BTNMci_LOCATION.x+BUTTON_WIDTH;
	rectMci.top		= BTNMci_LOCATION.y;
	rectMci.bottom	= BTNMci_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnMci->Create("MCI", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectMci, m_pTabDialog, IDC_BUTTON_AT_Cali_Main_Tab_MCI);
	m_pBtnMci->SetColor( RGB(255,128,0) );

	//create fifth dialog
	m_pPageMCI = new CDlgAutoTestMCI(m_pTabDialog);
	if(m_pPageMCI->Create(IDD_AutoTest_MCI, m_pTabDialog) == FALSE)
		return FALSE;

	//add fifth page
	m_pTabDialog->AddPage(m_pPageMCI, m_pBtnMci);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//Create sixth button
	m_pBtnSpr = new CMacRadioButton();
	RECT rectSpr;
	rectSpr.left	= BTNSpr_LOCATION.x;
	rectSpr.right	= BTNSpr_LOCATION.x+BUTTON_WIDTH;
	rectSpr.top		= BTNSpr_LOCATION.y;
	rectSpr.bottom	= BTNSpr_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnSpr->Create("SPR", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectSpr, m_pTabDialog, IDC_BUTTON_AT_Cali_Main_Tab_SPR);
	m_pBtnSpr->SetColor( RGB(128,0,0) );

	//create sixth dialog
	m_pPageSPR = new CDlgAutoTestSPR(m_pTabDialog);
	if(m_pPageSPR->Create(IDD_AutoTest_SPR, m_pTabDialog) == FALSE)
		return FALSE;

	//add sixth page
	m_pTabDialog->AddPage(m_pPageSPR, m_pBtnSpr);
	/////////////////////////////////////////////////////////////////

	return TRUE;
}

void CDlgAutoTestMain::InitParams()
{
	m_pPagePower->InitParams();
	m_pPageVAC->InitParams();
	m_pPageLNS->InitParams();
	m_pPageSCN->InitParams();
	m_pPageMCI->InitParams();
	m_pPageSPR->InitParams();
}