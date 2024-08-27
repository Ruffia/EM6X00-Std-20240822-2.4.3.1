#if !defined(AFX_SEMCORECTRLPANEL_STAND6PPG_H__108A3F3E_53B1_414C_9AB6_E86CAEF118D0__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND6PPG_H__108A3F3E_53B1_414C_9AB6_E86CAEF118D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand6Ppg.h : Declaration of the CSemCoreCtrlPanel_Stand6PropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6PropPage : See SemCoreCtrlPanel_Stand6Ppg.cpp.cpp for implementation.

class CSemCoreCtrlPanel_Stand6PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand6PropPage)
	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand6PropPage)

// Constructor
public:
	CSemCoreCtrlPanel_Stand6PropPage();

// Dialog Data
	//{{AFX_DATA(CSemCoreCtrlPanel_Stand6PropPage)
	enum { IDD = IDD_PROPPAGE_SEMCORECTRLPANEL_STAND6 };
	BOOL	m_bCaptionVisible;
	CString	m_strXMin;
	CString	m_strXMax;
	CString	m_strCaption;
	BOOL	m_bDebugInfoVisible;
	BOOL	m_bGradeVisible;
	BOOL	m_bAutoVisible;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand6PropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND6PPG_H__108A3F3E_53B1_414C_9AB6_E86CAEF118D0__INCLUDED)
