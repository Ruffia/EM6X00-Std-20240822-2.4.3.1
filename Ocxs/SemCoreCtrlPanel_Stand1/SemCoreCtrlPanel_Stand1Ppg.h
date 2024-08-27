#if !defined(AFX_SEMCORECTRLPANEL_STAND1PPG_H__D6B80C73_E18F_4871_A551_0675EE85A279__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND1PPG_H__D6B80C73_E18F_4871_A551_0675EE85A279__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand1Ppg.h : Declaration of the CSemCoreCtrlPanel_Stand1PropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1PropPage : See SemCoreCtrlPanel_Stand1Ppg.cpp.cpp for implementation.

class CSemCoreCtrlPanel_Stand1PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand1PropPage)
	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand1PropPage)

// Constructor
public:
	CSemCoreCtrlPanel_Stand1PropPage();

// Dialog Data
	//{{AFX_DATA(CSemCoreCtrlPanel_Stand1PropPage)
	enum { IDD = IDD_PROPPAGE_SEMCORECTRLPANEL_STAND1 };
	BOOL	m_bCaptionVisible;
	CString	m_strXMin;
	CString	m_strXMax;
	CString	m_strCaption;
	BOOL	m_bDebugInfoVisible;
	int		m_nStyle;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand1PropPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND1PPG_H__D6B80C73_E18F_4871_A551_0675EE85A279__INCLUDED)
