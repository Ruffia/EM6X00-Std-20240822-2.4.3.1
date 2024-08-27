#if !defined(AFX_SEMCORECTRLPANEL_STAND3PPG_H__C113D17B_C6B6_4EB6_AF36_9A09165DCD49__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND3PPG_H__C113D17B_C6B6_4EB6_AF36_9A09165DCD49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand3Ppg.h : Declaration of the CSemCoreCtrlPanel_Stand3PropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3PropPage : See SemCoreCtrlPanel_Stand3Ppg.cpp.cpp for implementation.

class CSemCoreCtrlPanel_Stand3PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand3PropPage)
	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand3PropPage)

// Constructor
public:
	CSemCoreCtrlPanel_Stand3PropPage();

// Dialog Data
	//{{AFX_DATA(CSemCoreCtrlPanel_Stand3PropPage)
	enum { IDD = IDD_PROPPAGE_SEMCORECTRLPANEL_STAND3 };
	BOOL	m_bCaptionVisible;
	CString	m_strCaption;
	BOOL	m_bDebugInfoVisible;
	BOOL	m_bGradeVisible;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand3PropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND3PPG_H__C113D17B_C6B6_4EB6_AF36_9A09165DCD49__INCLUDED)
