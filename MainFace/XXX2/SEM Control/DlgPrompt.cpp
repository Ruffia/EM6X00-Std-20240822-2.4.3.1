// DlgPrompt.cpp : implementation file
//
// Last Modify : 2009.04.21

#include "stdafx.h"
#include "XXX2.h"
#include "DlgPrompt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TYPE_BTN_OK				(BYTE)0x01
#define	TYPE_BTN_OKCANCEL		(BYTE)0x02
#define	TYPE_BTN_YESNO			(BYTE)0x04
#define	TYPE_BTN_YESNOCANCEL	(BYTE)0x08

#define	TYEP_ICO_ERROR			(BYTE)0x10
#define	TYPE_ICO_QUESTION		(BYTE)0x20
#define	TYPE_ICO_WARNING		(BYTE)0x40
#define	TYPE_ICO_INFOMATION		(BYTE)0x80

#define	FLAGBIT( flag, flag_bit )	( ( (flag) & (flag_bit) ) == (flag_bit) )

/////////////////////////////////////////////////////////////////////////////
// CDlgPrompt dialog


CDlgPrompt::CDlgPrompt(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPrompt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPrompt)
	m_strMsg	= _T("");
	//}}AFX_DATA_INIT
	m_byteType	= (BYTE)0x01;
}


void CDlgPrompt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPrompt)
	DDX_Text(pDX, IDC_STATIC_MSG_PROMPT, m_strMsg);
	DDV_MaxChars(pDX, m_strMsg, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPrompt, CDialog)
	//{{AFX_MSG_MAP(CDlgPrompt)
	ON_BN_CLICKED(IDYES, OnYes)
	ON_BN_CLICKED(IDNO, OnNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPrompt message handlers

void CDlgPrompt::Localize(void)
{ 
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDOK)->SetWindowText(GetResString(IDS_OK));
	GetDlgItem(IDCANCEL)->SetWindowText(GetResString(IDS_CANCEL));
	GetDlgItem(IDYES)->SetWindowText(GetResString(IDS_YES));
	GetDlgItem(IDNO)->SetWindowText(GetResString(IDS_NO));

	SetWindowText( m_strTitle );
} 

BOOL CDlgPrompt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_strTitle	= theApp.m_strLogoDisp;
	AdjustCtrlsPos();
	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPrompt::AdjustCtrlsPos()
{
	CRect rc;
	CWnd* pWnd = GetDlgItem(IDC_PROMPT_STATIC_RECT);
	ASSERT(pWnd != NULL);
	pWnd->GetWindowRect((LPRECT) rc);
	ScreenToClient((LPRECT) rc);

	CRect wndRect;
	pWnd = GetDlgItem(IDOK);
	ASSERT(pWnd != NULL);
	pWnd->GetWindowRect((LPRECT) wndRect);

	int nLeft, nTop, nWidth, nHeight;

	if( FLAGBIT( m_byteType, TYPE_BTN_OK ) )
	{
		//只有确定按钮，居中显示
		nLeft	= rc.left + (rc.Width() - wndRect.Width()) /2;
		nTop	= rc.top;
		nWidth	= wndRect.Width();
		nHeight	= wndRect.Height();
		( GetDlgItem( IDOK ) )->MoveWindow( nLeft, nTop, nWidth, nHeight );

		( GetDlgItem( IDCANCEL ) )->ShowWindow( FALSE );
		( GetDlgItem( IDYES ) )->ShowWindow( FALSE );
		( GetDlgItem( IDNO ) )->ShowWindow( FALSE );
	}
	else if( FLAGBIT( m_byteType, TYPE_BTN_OKCANCEL ) )
	{
		( GetDlgItem( IDYES ) )->ShowWindow( FALSE );
		( GetDlgItem( IDNO ) )->ShowWindow( FALSE );
	}
	else if( FLAGBIT( m_byteType, TYPE_BTN_YESNO ) )
	{
		( GetDlgItem( IDOK ) )->ShowWindow( FALSE );
		( GetDlgItem( IDCANCEL ) )->ShowWindow( FALSE );
	}
	else if( FLAGBIT( m_byteType, TYPE_BTN_YESNOCANCEL ) )
	{
		//有三个按钮，横向平均显示
		nLeft	= rc.left;
		nTop	= rc.bottom -wndRect.Height();
		nWidth	= wndRect.Width();
		nHeight	= wndRect.Height();
		( GetDlgItem( IDYES ) )->MoveWindow( nLeft, nTop, nWidth, nHeight );
		nLeft	= rc.left + (rc.Width() - nWidth) /2;
		( GetDlgItem( IDNO ) )->MoveWindow( nLeft, nTop, nWidth, nHeight );
		nLeft	= rc.right -nWidth;
		( GetDlgItem( IDCANCEL ) )->MoveWindow( nLeft, nTop, nWidth, nHeight );

		( GetDlgItem( IDOK ) )->ShowWindow( FALSE );
	}

	//如果文字一行可以显示，将标签风格改为纵向居中显示(center vertically)
	if( m_strMsg.GetLength() < 40 )
	{
		pWnd = GetDlgItem(IDC_STATIC_MSG_PROMPT);
		ASSERT(pWnd != NULL);
		pWnd->ModifyStyle(0, SS_CENTERIMAGE);
	}
}

void CDlgPrompt::OnYes() 
{
	// TODO: Add your control notification handler code here
	EndDialog(IDYES);
}

void CDlgPrompt::OnNo() 
{
	// TODO: Add your control notification handler code here
	EndDialog(IDNO);
}
