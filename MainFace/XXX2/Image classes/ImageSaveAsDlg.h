#if !defined(AFX_IMAGESAVEASDLG_H__1CB176D3_983D_4E53_9452_AF7D2911E544__INCLUDED_)
#define AFX_IMAGESAVEASDLG_H__1CB176D3_983D_4E53_9452_AF7D2911E544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageSaveAsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageSaveAsDlg dialog

class CImageSaveAsDlg : public CFileDialog
{
	DECLARE_DYNAMIC(CImageSaveAsDlg)

public:
	CImageSaveAsDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

protected:
	virtual void OnTypeChange();

	//{{AFX_MSG(CImageSaveAsDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGESAVEASDLG_H__1CB176D3_983D_4E53_9452_AF7D2911E544__INCLUDED_)
