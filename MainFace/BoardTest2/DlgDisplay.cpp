// DlgDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "insample0.h"
#include "DlgDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDisplay dialog


CDlgDisplay::CDlgDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDisplay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDisplay)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDisplay)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDisplay, CDialog)
	//{{AFX_MSG_MAP(CDlgDisplay)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDisplay message handlers
