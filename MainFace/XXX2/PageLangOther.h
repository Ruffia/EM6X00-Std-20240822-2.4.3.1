// Last Modify : 2010.07.02

/////////////////////////////////////////////////////////////////////////////
// 10.07.02 增加优先存储格式
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGELANGOTHER_H__502A17DA_D7D3_4A31_A5BC_D88ACA95BA61__INCLUDED_)
#define AFX_PAGELANGOTHER_H__502A17DA_D7D3_4A31_A5BC_D88ACA95BA61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageLangOther.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageLangOther dialog

class CPageLangOther : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageLangOther)

// Construction
public:
	CPageLangOther();
	~CPageLangOther();
	void	Localize(void);
	CFont	m_Font;

// Dialog Data
	//{{AFX_DATA(CPageLangOther)
	enum { IDD = IDD_3K2_PAGE_LANGUAGE_OTHER };
	int		m_nPortHV;
	int		m_nPortCCG;
	int		m_nPortIPG;
	//}}AFX_DATA

	int		m_nLanguage, m_nPriorSaveType;
	BOOL	m_bOpenImageAfterSnap, m_bLogAll;
	BOOL	m_bShowScreenMag, m_bShowDate, m_bShowNote, m_bShowCCD;

	int		m_nLanguageBak, m_nPriorSaveTypeBak;
	BOOL	m_bOpenImageAfterSnapBak, m_bLogAllBak;
	BOOL	m_bShowScreenMagBak, m_bShowDateBak, m_bShowNoteBak, m_bShowCCDBak;
	int		m_nPortHVbak, m_nPortCCGbak, m_nPortIPGbak;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageLangOther)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageLangOther)
	virtual BOOL OnInitDialog();
	afx_msg void OnPageLangCheckOpenImageAfterSnap();
	afx_msg void OnCHECKDebugLogAll();
	afx_msg void OnSelchangeCOMBODebugCPS3603Port();
	afx_msg void OnSelchangeCOMBODebugCCGPort();
	afx_msg void OnSelchangeCOMBODebugIPGPort();
	afx_msg void OnWriteOneKey();
	afx_msg void OnBtnDefault();
	afx_msg void OnCHECKSwScrMag();
	afx_msg void OnCHECKOpenCCD();
	afx_msg void OnCHECKSwDate();
	afx_msg void OnCHECKSwNote();
	//}}AFX_MSG
	afx_msg void OnRadioLanguage(UINT nID);
	afx_msg void OnRadioPriorSaveType(UINT nID);
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGELANGOTHER_H__502A17DA_D7D3_4A31_A5BC_D88ACA95BA61__INCLUDED_)
