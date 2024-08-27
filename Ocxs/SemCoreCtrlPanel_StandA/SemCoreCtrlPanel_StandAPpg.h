#if !defined(AFX_SEMCORECTRLPANEL_STANDAPPG_H__803EA34F_D7AA_4A7A_9637_190223C5BDF0__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STANDAPPG_H__803EA34F_D7AA_4A7A_9637_190223C5BDF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_StandAPpg.h : Declaration of the CSemCoreCtrlPanel_StandAPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandAPropPage : See SemCoreCtrlPanel_StandAPpg.cpp.cpp for implementation.

class CSemCoreCtrlPanel_StandAPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_StandAPropPage)
	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_StandAPropPage)

// Constructor
public:
	CSemCoreCtrlPanel_StandAPropPage();

// Dialog Data
	//{{AFX_DATA(CSemCoreCtrlPanel_StandAPropPage)
	enum { IDD = IDD_PROPPAGE_SEMCORECTRLPANEL_STANDA };
	BOOL	m_bCaptionVisible;
	CString	m_strCaption;
	BOOL	m_bDebugInfoVisible;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSemCoreCtrlPanel_StandAPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STANDAPPG_H__803EA34F_D7AA_4A7A_9637_190223C5BDF0__INCLUDED)
