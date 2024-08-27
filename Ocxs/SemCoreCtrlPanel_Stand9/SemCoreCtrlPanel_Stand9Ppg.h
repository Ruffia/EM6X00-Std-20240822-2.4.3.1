#if !defined(AFX_SEMCORECTRLPANEL_STAND9PPG_H__E5FCD949_3F68_4A03_AC1F_08E7D781C3BA__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND9PPG_H__E5FCD949_3F68_4A03_AC1F_08E7D781C3BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand9Ppg.h : Declaration of the CSemCoreCtrlPanel_Stand9PropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand9PropPage : See SemCoreCtrlPanel_Stand9Ppg.cpp.cpp for implementation.

class CSemCoreCtrlPanel_Stand9PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand9PropPage)
	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand9PropPage)

// Constructor
public:
	CSemCoreCtrlPanel_Stand9PropPage();

// Dialog Data
	//{{AFX_DATA(CSemCoreCtrlPanel_Stand9PropPage)
	enum { IDD = IDD_PROPPAGE_SEMCORECTRLPANEL_STAND9 };
	BOOL	m_bCaptionVisible;
	CString	m_strCaption;
	BOOL	m_bDebugInfoVisible;
	BOOL	m_bNewDisplayMode;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand9PropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND9PPG_H__E5FCD949_3F68_4A03_AC1F_08E7D781C3BA__INCLUDED)
