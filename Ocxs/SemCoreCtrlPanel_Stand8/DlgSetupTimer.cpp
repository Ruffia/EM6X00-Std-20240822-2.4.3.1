// DlgSetupTimer.cpp : implementation file
//

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand8.h"
#include "DlgSetupTimer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupTimer dialog


CDlgSetupTimer::CDlgSetupTimer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetupTimer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetupTimer)
	m_nTimer = 0;
	//}}AFX_DATA_INIT
}


void CDlgSetupTimer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetupTimer)
	DDX_Control(pDX, IDC_DIALOG_SEMCORECTRLPANEL_STAND8_STATIC_Warning, m_ctrlWarning);
	DDX_Text(pDX, IDC_DIALOG_SEMCORECTRLPANEL_STAND8_EDIT_Speed, m_nTimer);
	DDV_MinMaxUInt(pDX, m_nTimer, 150, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetupTimer, CDialog)
	//{{AFX_MSG_MAP(CDlgSetupTimer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetupTimer message handlers

BOOL CDlgSetupTimer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	COLORREF rgbFlash1 = 0x0000FF;
	COLORREF rgbFlash2 = CC_SYSCOLOR(COLOR_WINDOWTEXT);

	m_ctrlWarning.SetTextColor( CC_SYSCOLOR(COLOR_WINDOWTEXT) );
	m_ctrlWarning.SetBkColor( CC_SYSCOLOR(COLOR_BTNFACE) );
	m_ctrlWarning.SetBkBlinkColors( CC_SYSCOLOR(COLOR_INFOBK), CC_SYSCOLOR(COLOR_INFOTEXT) );
	m_ctrlWarning.SetTextBlinkColors( rgbFlash1, rgbFlash2 );
	m_ctrlWarning.StartBlink(CC_BLINK_TEXT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
