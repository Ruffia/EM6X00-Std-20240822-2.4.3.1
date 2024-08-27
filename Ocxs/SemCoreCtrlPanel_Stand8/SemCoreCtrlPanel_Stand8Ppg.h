#if !defined(AFX_SEMCORECTRLPANEL_STAND8PPG_H__713D8557_4C81_4A28_BE4B_A2F960315166__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND8PPG_H__713D8557_4C81_4A28_BE4B_A2F960315166__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand8Ppg.h : Declaration of the CSemCoreCtrlPanel_Stand8PropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8PropPage : See SemCoreCtrlPanel_Stand8Ppg.cpp.cpp for implementation.

class CSemCoreCtrlPanel_Stand8PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand8PropPage)
	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand8PropPage)

// Constructor
public:
	CSemCoreCtrlPanel_Stand8PropPage();

// Dialog Data
	//{{AFX_DATA(CSemCoreCtrlPanel_Stand8PropPage)
	enum { IDD = IDD_PROPPAGE_SEMCORECTRLPANEL_STAND8 };
	BOOL	m_bCaptionVisible;
	CString	m_strCaption;
	BOOL	m_bDebugInfoVisible;
	CString	m_strProgressMessage;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand8PropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND8PPG_H__713D8557_4C81_4A28_BE4B_A2F960315166__INCLUDED)
