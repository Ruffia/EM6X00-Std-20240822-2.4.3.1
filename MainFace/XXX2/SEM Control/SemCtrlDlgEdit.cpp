// SemCtrlDlgEdit.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "SemCtrlDlgEdit.h"
#include "ControlLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSemCtrlDlgEdit dialog


CSemCtrlDlgEdit::CSemCtrlDlgEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CSemCtrlDlgEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSemCtrlDlgEdit)
	m_csText = _T("");
	//}}AFX_DATA_INIT

	m_ptPos = CPoint(0,0);
}


void CSemCtrlDlgEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSemCtrlDlgEdit)
	DDX_Text(pDX, IDC_EDIT_TEXT, m_csText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSemCtrlDlgEdit, CDialog)
	//{{AFX_MSG_MAP(CSemCtrlDlgEdit)
	ON_EN_CHANGE(IDC_EDIT_TEXT, OnChangeEditText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSemCtrlDlgEdit message handlers

void CSemCtrlDlgEdit::Localize(void)
{ 
	if( m_hWnd == NULL )
		return;

	SetWindowText(GetResString(IDS_SEMCTL_EDIT)); 
} 

BOOL CSemCtrlDlgEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE );

	Localize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSemCtrlDlgEdit::OnChangeEditText() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	static CSize szLastTextSize(0,0);
	CString csLastText = m_csText;
	
	UpdateData();

	CRect rcClear;
	long nClearLength = csLastText.GetLength() - m_csText.GetLength();

	if ( nClearLength <= 0 )
	{
		szLastTextSize = CSemVirtualMCICard::Instance().GraphTextOut( m_ptPos, m_csText );
	}
	else if ( nClearLength == 1 )
	{
		//rcClear.left	= m_ptPos.x + szLastTextSize.cx - 20;
		//rcClear.right	= m_ptPos.x + szLastTextSize.cx;
		rcClear.left	= m_ptPos.x + szLastTextSize.cx - 32;
		rcClear.right	= m_ptPos.x + szLastTextSize.cx + 1;
		rcClear.top		= m_ptPos.y;
		rcClear.bottom	= m_ptPos.y + szLastTextSize.cy;

		CSemVirtualMCICard::Instance().GraphClear( rcClear );
		szLastTextSize = CSemVirtualMCICard::Instance().GraphTextOut( m_ptPos, m_csText );
	}
	else
	{
		rcClear.left	= m_ptPos.x;
		rcClear.right	= m_ptPos.x + szLastTextSize.cx;
		rcClear.top		= m_ptPos.y;
		rcClear.bottom	= m_ptPos.y + szLastTextSize.cy;

		CSemVirtualMCICard::Instance().GraphClear( rcClear );
		szLastTextSize = CSemVirtualMCICard::Instance().GraphTextOut( m_ptPos, m_csText );
	}

}

void CSemCtrlDlgEdit::ClearTextBuff() 
{
	m_csText.Empty();
	UpdateData( FALSE );

	SetFocus();
}


