#if !defined(AFX_SEMCORECTRLPANEL_STAND4PPG_H__F607AEEA_E7F5_4616_8901_A6F2A231E027__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND4PPG_H__F607AEEA_E7F5_4616_8901_A6F2A231E027__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand4Ppg.h : Declaration of the CSemCoreCtrlPanel_Stand4PropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand4PropPage : See SemCoreCtrlPanel_Stand4Ppg.cpp.cpp for implementation.

class CSemCoreCtrlPanel_Stand4PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand4PropPage)
	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand4PropPage)

// Constructor
public:
	CSemCoreCtrlPanel_Stand4PropPage();

// Dialog Data
	//{{AFX_DATA(CSemCoreCtrlPanel_Stand4PropPage)
	enum { IDD = IDD_PROPPAGE_SEMCORECTRLPANEL_STAND4 };
	CString	m_strCaption;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand4PropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND4PPG_H__F607AEEA_E7F5_4616_8901_A6F2A231E027__INCLUDED)
