// SEMDocument.cpp : implementation file
//
// Last Modify : 2009.04.21

#include "stdafx.h"
#include "XXX2.h"
#include "SEMDocument.h"

#include "DlgPrompt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSEMDocument

IMPLEMENT_DYNCREATE(CSEMDocument, CDocument)

CSEMDocument::CSEMDocument()
{
	m_DefaultMenu = NULL; // initialize to NULL
}

BOOL CSEMDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSEMDocument::~CSEMDocument()
{
	if (m_DefaultMenu)
		::DestroyMenu(m_DefaultMenu);
}


BEGIN_MESSAGE_MAP(CSEMDocument, CDocument)
	//{{AFX_MSG_MAP(CSEMDocument)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEMDocument diagnostics

#ifdef _DEBUG
void CSEMDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CSEMDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSEMDocument serialization

void CSEMDocument::Serialize(CArchive& ar)
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
// CSEMDocument commands

BOOL CSEMDocument::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	
	return TRUE;
}

BOOL CSEMDocument::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDocument::OnSaveDocument(lpszPathName);
}

void CSEMDocument::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDocument::OnCloseDocument();
}

void CSEMDocument::SetTitle(LPCTSTR lpszTitle) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDocument::SetTitle(lpszTitle);
}

BOOL CSEMDocument::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!IsModified())
		return TRUE;        // ok to continue

	// get name/title of document
	CString name = m_strTitle;
/*	if (m_strPathName.IsEmpty())
	{
		// get name based on caption
		name = m_strTitle;
		if (name.IsEmpty())
			VERIFY(name.LoadString(AFX_IDS_UNTITLED));
	}
	else
	{
		// get name based on file title of path name
		name = m_strPathName;
		if (afxData.bMarked4)
		{
			AfxGetFileTitle(m_strPathName, name.GetBuffer(_MAX_PATH), _MAX_PATH);
			name.ReleaseBuffer();
		}
	}

	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
*/
	CDlgPrompt dlg;
	dlg.m_byteType = (BYTE)0x08;
	dlg.m_strMsg.Format( GetResString( IDF_ASK_TO_SAVE ), name );
	int nRet = dlg.DoModal();
	switch( nRet )
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		// If so, either Save or Update, as appropriate
		if (!DoFileSave())
			return FALSE;       // don't continue
		break;

	case IDNO:
		// If not saving changes, revert the document
		break;

	case	IDOK:
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;    // keep going
//	return CDocument::SaveModified();
}

BOOL CSEMDocument::DoSave(LPCTSTR pszPathName, BOOL bReplace /*=TRUE*/)
{
	return TRUE;
}

HMENU CSEMDocument::GetDefaultMenu()  // get menu depending on state
{
	if (m_DefaultMenu)
		return m_DefaultMenu;

	return CDocument::GetDefaultMenu();
}
