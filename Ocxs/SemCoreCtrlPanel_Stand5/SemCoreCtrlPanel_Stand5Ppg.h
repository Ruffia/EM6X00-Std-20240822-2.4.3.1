#if !defined(AFX_SEMCORECTRLPANEL_STAND5PPG_H__63020074_733A_4EB0_A379_197684FFBD15__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND5PPG_H__63020074_733A_4EB0_A379_197684FFBD15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand5Ppg.h : Declaration of the CSemCoreCtrlPanel_Stand5PropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5PropPage : See SemCoreCtrlPanel_Stand5Ppg.cpp.cpp for implementation.

class CSemCoreCtrlPanel_Stand5PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand5PropPage)
	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand5PropPage)

// Constructor
public:
	CSemCoreCtrlPanel_Stand5PropPage();

// Dialog Data
	//{{AFX_DATA(CSemCoreCtrlPanel_Stand5PropPage)
	enum { IDD = IDD_PROPPAGE_SEMCORECTRLPANEL_STAND5 };
	CString	m_strCaption;
	int		m_nStyle;
	short	m_nScaleDecimals;
	short	m_nValueDecimals;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand5PropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioInt();
	afx_msg void OnRadioFloat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND5PPG_H__63020074_733A_4EB0_A379_197684FFBD15__INCLUDED)
