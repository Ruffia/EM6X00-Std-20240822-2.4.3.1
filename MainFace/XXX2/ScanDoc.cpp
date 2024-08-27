// ScanDoc.cpp : implementation file
// Author : J.Y.Ma
// Last Modify : 2009.04.21

#include "stdafx.h"
#include "XXX2.h"
#include "ScanDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScanDoc

IMPLEMENT_DYNCREATE(CScanDoc, CSEMDocument)

CScanDoc::CScanDoc()
{
}

BOOL CScanDoc::OnNewDocument()
{
//	CString	csTitle;
//	csTitle.Format( "SemScan%s%03d",
//		theApp.m_csCurrentDate, theApp.m_nSerialNumber );
	SetTitle( _T("SemScan") );

	if (!CSEMDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CScanDoc::~CScanDoc()
{
}


BEGIN_MESSAGE_MAP(CScanDoc, CSEMDocument)
	//{{AFX_MSG_MAP(CScanDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanDoc diagnostics

#ifdef _DEBUG
void CScanDoc::AssertValid() const
{
	CSEMDocument::AssertValid();
}

void CScanDoc::Dump(CDumpContext& dc) const
{
	CSEMDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScanDoc serialization

void CScanDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CScanDoc commands

// 09.03.05
HMENU CScanDoc::GetDefaultMenu()  // get menu depending on state
{
	return CSEMDocument::GetDefaultMenu();
}
