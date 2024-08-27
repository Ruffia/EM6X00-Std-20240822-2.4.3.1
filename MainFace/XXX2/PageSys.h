#if !defined(AFX_PAGESYS_H__448A8AF0_BD30_4245_B61C_D4A6340505B2__INCLUDED_)
#define AFX_PAGESYS_H__448A8AF0_BD30_4245_B61C_D4A6340505B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSys.h : header file
//

#include "FontCombo.h"

/////////////////////////////////////////////////////////////////////////////
// CPageSys dialog

class CPageSys : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageSys)

// Construction
public:
	CPageSys();
	~CPageSys();
	void	Localize(void);
	CFont	m_Font;

	SYS_Options	m_sysOpt;

// Dialog Data
	//{{AFX_DATA(CPageSys)
	enum { IDD = IDD_PAGE_SYS };
	CComboBox	m_comboLineWidth;
	CComboBox	m_comboUScaleTextSize;
	CComboBox	m_comboLabelTextSize;
	CComboBox	m_comboEditTextSize;
	CFontCombo	m_comboUScaleText;
	CFontCombo	m_comboLabelText;
	CFontCombo	m_comboEditText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageSys)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageSys)
	virtual BOOL OnInitDialog();
	afx_msg void OnPAGESYSColorBtnBackSel();
	afx_msg void OnPAGESYSColorBtnEditTextSel();
	afx_msg void OnPAGESYSColorBtnLabelTextSel();
	afx_msg void OnPAGESYSColorBtnLabelLogoTextSel();
	afx_msg void OnPAGESYSColorBtnUScaleTextSel();
	afx_msg void OnPAGESYSColorBtnUScaleLineSel();
	afx_msg void OnPAGESYSColorBtnOscGridSel();
	afx_msg void OnPAGESYSColorBtnOscGraphSel();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPAGESYSColorBtnDefault();
	afx_msg void OnSelchangePAGESYSFontComboBoxEditTextName();
	afx_msg void OnSelchangePAGESYSFontComboBoxLabelTextName();
	afx_msg void OnSelchangePAGESYSFontComboBoxUScaleTextName();
	afx_msg void OnPAGESYSFontBtnDefault();
	afx_msg void OnPAGESYSColorBtnTransSel();
	afx_msg void OnSelchangePAGESYSColorComboBoxLineWidth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESYS_H__448A8AF0_BD30_4245_B61C_D4A6340505B2__INCLUDED_)
