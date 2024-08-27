// DlgDebugMain.cpp : implementation file
//

#include "stdafx.h"
#include "xxx2.h"
#include "DlgDebugMain.h"
#include "ControlLayer.h"
#include "HVPowerDataManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugMain dialog


CDlgDebugMain::CDlgDebugMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDebugMain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDebugMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pTabDialog	= NULL;
	m_pBtnMon		= NULL;
	m_pBtnAdvance	= NULL;
//	m_pBtnDebug		= NULL;
	m_pPageMon		= NULL;
	m_pPageMon2		= NULL;
	m_pPageMon3		= NULL;
	m_pPageAdvance	= NULL;
//	m_pPageDebug	= NULL;
}

CDlgDebugMain::~CDlgDebugMain()
{
	m_Font.DeleteObject();
	if( m_pBtnMon	!= NULL )
	{
		delete m_pBtnMon;
		m_pBtnMon = NULL;
	}
	if( m_pBtnAdvance != NULL )
	{
		delete m_pBtnAdvance;
		m_pBtnAdvance = NULL;
	}
// 	if( m_pBtnDebug != NULL )
// 	{
// 		delete m_pBtnDebug;
// 		m_pBtnDebug = NULL;
// 	}
	if( m_pPageMon != NULL )
	{
		m_pPageMon->DestroyWindow();
		delete m_pPageMon;
		m_pPageMon = NULL;
	}
	if( m_pPageMon2 != NULL )
	{
		m_pPageMon2->DestroyWindow();
		delete m_pPageMon2;
		m_pPageMon2 = NULL;
	}
	if( m_pPageMon3 != NULL )
	{
		m_pPageMon3->DestroyWindow();
		delete m_pPageMon3;
		m_pPageMon3 = NULL;
	}
	if( m_pPageAdvance != NULL )
	{
		m_pPageAdvance->DestroyWindow();
		delete m_pPageAdvance;
		m_pPageAdvance = NULL;
	}
// 	if( m_pPageDebug != NULL )
// 	{
// 		m_pPageDebug->DestroyWindow();
// 		delete m_pPageDebug;
// 		m_pPageDebug = NULL;
// 	}
	if( m_pTabDialog	!= NULL )
	{
		m_pTabDialog->DestroyWindow();
		delete m_pTabDialog;
		m_pTabDialog = NULL;
	}
}

void CDlgDebugMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDebugMain)
	DDX_Control(pDX, IDC_STATIC_Debug_Main_TabPos, m_staticTabPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDebugMain, CDialog)
	//{{AFX_MSG_MAP(CDlgDebugMain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDebugMain message handlers

void CDlgDebugMain::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	m_pBtnMon->SetWindowText(GetResString( IDS_BUTTON_Debug_Tab_Mon ));
	m_pBtnAdvance->SetWindowText(GetResString( IDS_BUTTON_Debug_Tab_Advance ));
//	m_pBtnDebug->SetWindowText(GetResString( IDS_BUTTON_Debug_Tab_Debug ));

	if( m_pPageMon != NULL && m_pPageMon->m_hWnd != NULL )
		m_pPageMon->Localize();
	if( m_pPageMon2 != NULL && m_pPageMon2->m_hWnd != NULL )
		m_pPageMon2->Localize();
	if( m_pPageMon3 != NULL && m_pPageMon3->m_hWnd != NULL )
		m_pPageMon3->Localize();
	if( m_pPageAdvance != NULL && m_pPageAdvance->m_hWnd != NULL )
		m_pPageAdvance->Localize();
// 	if( m_pPageDebug != NULL && m_pPageDebug->m_hWnd != NULL )
// 		m_pPageDebug->Localize();

	SetWindowText(GetResString( IDS_Debug_Main ));
}

BOOL CDlgDebugMain::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTabDialog();
	Localize();

	m_Font.CreateFont(-12, 0, 0, 0, FW_NORMAL, 0, 0, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");

	SetFont( &m_Font, TRUE );
	CWnd *pw = GetWindow(GW_CHILD);
	while(pw != NULL)
	{   
		pw->SetFont( &m_Font);
		pw = pw->GetWindow(GW_HWNDNEXT);
	};
	m_pBtnMon->SetFont( &m_Font );
	m_pBtnAdvance->SetFont( &m_Font );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////
// Initialize the TabDialog
BOOL CDlgDebugMain::InitTabDialog()
{
	//create the TabDialog
	m_pTabDialog = new CDlgDebugTab(IDD_Debug_TabDlg, this);
	if (m_pTabDialog->Create(IDD_Debug_TabDlg, this) == FALSE)
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
	m_pBtnMon->SetCheck(1);
	
	return TRUE;
}

////////////////////////////////////////////////////////////
//Add pages (include button and dialog) to TabDialog
BOOL CDlgDebugMain::AddPagesToTabDialog()
{
	/////////////////////////////////////////////////////////////////
	//create first button
	m_pBtnMon = new CMacRadioButton();
	RECT rectMon;
	rectMon.left	= BTNMon_LOCATION.x;
	rectMon.right	= BTNMon_LOCATION.x+BUTTON_WIDTH;
	rectMon.top		= BTNMon_LOCATION.y;
	rectMon.bottom	= BTNMon_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnMon->Create("Monitor", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectMon, m_pTabDialog, IDC_BUTTON_Debug_Tab_Mon);
	m_pBtnMon->SetColor( RGB(0,0,255) );

	//create first dialog
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		m_pPageMon = new CDlgDebugMon(m_pTabDialog);
		if(m_pPageMon->Create(IDD_Debug_Mon, m_pTabDialog) == FALSE)
			return FALSE;

		//add first page
		m_pTabDialog->AddPage(m_pPageMon, m_pBtnMon);
	}
	else
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
		{
			m_pPageMon3 = new CDlgDebugMon3(m_pTabDialog);
			if(m_pPageMon3->Create(IDD_Debug_Mon3, m_pTabDialog) == FALSE)
				return FALSE;

			//add first page
			m_pTabDialog->AddPage(m_pPageMon3, m_pBtnMon);
		}
		else
		{
			m_pPageMon2 = new CDlgDebugMon2(m_pTabDialog);
			if(m_pPageMon2->Create(IDD_Debug_Mon2, m_pTabDialog) == FALSE)
				return FALSE;

			//add first page
			m_pTabDialog->AddPage(m_pPageMon2, m_pBtnMon);
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//Create second button
	m_pBtnAdvance = new CMacRadioButton();
	RECT rectAdvance;
	rectAdvance.left	= BTNAdvance_LOCATION.x;
	rectAdvance.right	= BTNAdvance_LOCATION.x+BUTTON_WIDTH;
	rectAdvance.top		= BTNAdvance_LOCATION.y;
	rectAdvance.bottom	= BTNAdvance_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnAdvance->Create("Advance", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectAdvance, m_pTabDialog, IDC_BUTTON_Debug_Tab_Advance);
	m_pBtnAdvance->SetColor( RGB(0,128,0) );

	//create second dialog
	m_pPageAdvance = new CDlgDebugAdvance(m_pTabDialog);
	if(m_pPageAdvance->Create(IDD_Debug_Advance, m_pTabDialog) == FALSE)
		return FALSE;

	//add second page
	m_pTabDialog->AddPage(m_pPageAdvance, m_pBtnAdvance);
	/////////////////////////////////////////////////////////////////
/*
	/////////////////////////////////////////////////////////////////
	//Create third button
	m_pBtnDebug = new CMacRadioButton();
	RECT rectDebug;
	rectDebug.left		= BTNDebug_LOCATION.x;
	rectDebug.right		= BTNDebug_LOCATION.x+BUTTON_WIDTH;
	rectDebug.top		= BTNDebug_LOCATION.y;
	rectDebug.bottom	= BTNDebug_LOCATION.y+BUTTON_HEIGHT;

	m_pBtnDebug->Create("Debug", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE | BS_FLAT, rectDebug, m_pTabDialog, IDC_BUTTON_Debug_Tab_Debug);
	m_pBtnDebug->SetColor( RGB(128,0,0) );

	//create third dialog
	m_pPageDebug = new CSemCtrlDlgDebug(m_pTabDialog);
	if(m_pPageDebug->Create(IDD_SEMCTL_DEBUG, m_pTabDialog) == FALSE)
		return FALSE;

	//add third page
	m_pTabDialog->AddPage(m_pPageDebug, m_pBtnDebug);
	/////////////////////////////////////////////////////////////////
*/
	return TRUE;
}
