// Last Modify : 2010.07.02

/////////////////////////////////////////////////////////////////////////////
// 10.07.02 增加打开窗口的图像预览 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMFILEDIALOG_H__CEB8D6F3_C25F_4AE1_B6DA_4F9781C56E0E__INCLUDED_)
#define AFX_SEMFILEDIALOG_H__CEB8D6F3_C25F_4AE1_B6DA_4F9781C56E0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SEMFileDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSEMFileDialog dialog

class CSEMFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CSEMFileDialog)

public:
	CSEMFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL,
		DWORD dwSize = 0,
		BOOL bVistaStyle = TRUE);
	~CSEMFileDialog();

	void	Localize(void);
	CFont	m_Font;
	BOOL	m_bOpenFileDialog;
	CxImage	*image;					//main image

protected:
	//{{AFX_MSG(CSEMFileDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnCHECKPreviewShowPrev();
	afx_msg void OnCHECKSaveSrc();
	//}}AFX_MSG
	afx_msg void OnTypeChange();
	afx_msg void OnFileNameChange();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMFILEDIALOG_H__CEB8D6F3_C25F_4AE1_B6DA_4F9781C56E0E__INCLUDED_)
