#if !defined(AFX_SEMCORECTRLPANEL_STAND2PPG_H__1ABD152B_06A7_4B82_8FF3_CB71D6B6CBBA__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND2PPG_H__1ABD152B_06A7_4B82_8FF3_CB71D6B6CBBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand2Ppg.h : Declaration of the CSemCoreCtrlPanel_Stand2PropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand2PropPage : See SemCoreCtrlPanel_Stand2Ppg.cpp.cpp for implementation.

class CSemCoreCtrlPanel_Stand2PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand2PropPage)
	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand2PropPage)

// Constructor
public:
	CSemCoreCtrlPanel_Stand2PropPage();

// Dialog Data
	//{{AFX_DATA(CSemCoreCtrlPanel_Stand2PropPage)
	enum { IDD = IDD_PROPPAGE_SEMCORECTRLPANEL_STAND2 };
	BOOL	m_bCaptionVisible;
	CString	m_strXMin;
	CString	m_strXMax;
	CString	m_strCaption;
	BOOL	m_bDebugInfoVisible;
	CString	m_strYMax;
	CString	m_strYMin;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand2PropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND2PPG_H__1ABD152B_06A7_4B82_8FF3_CB71D6B6CBBA__INCLUDED)
