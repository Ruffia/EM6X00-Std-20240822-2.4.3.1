// Last Modify : 2010.09.25

#if !defined(AFX_DLGIMPPANELTHUMB_H__940D69ED_4259_4277_A65A_14212DF40CF7__INCLUDED_)
#define AFX_DLGIMPPANELTHUMB_H__940D69ED_4259_4277_A65A_14212DF40CF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImpPanelThumb.h : header file
//

#include "ThumbnailListCtrlEx.h"
#include "ximage.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelThumb dialog

class CDlgImpPanelThumb : public CDialog
{
// Construction
public:
	CDlgImpPanelThumb(CWnd* pParent = NULL);   // standard constructor
	~CDlgImpPanelThumb();
	void	AdjustCtrlsPos();
	void	Localize(void);

	void	Update_ImageList_ListCtrl(UINT uCount);
	void	Reset_ImageList_ListCtrl();

	void	AddItem(CxImage* pImage, CString strName);
	void	AddItem( CThumbImageItem* pThumbImageItem );
	void	DeleteItem( CString strTitle );
	void	ShowItem( CThumbImageItem* pThumbImageItem );
	void	ChangeItemName( CString strOldName, CString strNewName );

	BOOL	DoSave( CThumbImageItem* pImageItem );
	// 2021.06 原始数据存储
	void	SaveImageDataSrc( CThumbImageItem* pImageItem );

	CImageList	m_ImageListThumb;
	CFont		m_Font;

// Dialog Data
	//{{AFX_DATA(CDlgImpPanelThumb)
	enum { IDD = IDD_IMP_PANEL_THUMB };
	CThumbnailListCtrlEx	m_ListCtrlThumb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImpPanelThumb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImpPanelThumb)
	virtual BOOL OnInitDialog();
	afx_msg void OnThumbSave();
	afx_msg void OnThumbDelete();
	afx_msg void OnThumbView();
	afx_msg void OnDblclkLISTThumb(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnThumbSelAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPPANELTHUMB_H__940D69ED_4259_4277_A65A_14212DF40CF7__INCLUDED_)
