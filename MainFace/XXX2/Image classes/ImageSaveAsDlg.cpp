// ImageSaveAsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "3200Imp.h"

#include "DLGS.H"
#include "ImageSaveAsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageSaveAsDlg

IMPLEMENT_DYNAMIC(CImageSaveAsDlg, CFileDialog)

CImageSaveAsDlg::CImageSaveAsDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(CImageSaveAsDlg, CFileDialog)
	//{{AFX_MSG_MAP(CImageSaveAsDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageSaveAsDlg

void CImageSaveAsDlg::OnTypeChange()
{
	int nIndex = CFileDialog::m_ofn.nFilterIndex - 1;
	int nDocType = theApp.GetTypeFromIndex( nIndex, m_bOpenFileDialog );
	CString csCurrentExt = theApp.GetExtFromType(nDocType).Mid(2,3);

	CString csFileExt = CFileDialog::GetFileExt();
	
	if( csFileExt != csCurrentExt )
	{
		CString csFileTitle = CFileDialog::GetFileTitle(); 
		if ( csFileTitle.IsEmpty() )	csFileTitle = _T("*");
		CString csFileName = csFileTitle + _T(".") + csCurrentExt;
		
		CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, edt1, (LPCSTR)csFileName );
	}
}

