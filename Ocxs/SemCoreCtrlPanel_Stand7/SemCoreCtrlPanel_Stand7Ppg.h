#if !defined(AFX_SEMCORECTRLPANEL_STAND7PPG_H__7479DF7B_C5F9_4DF9_83A3_BF3BB5E7AEF6__INCLUDED_)
#define AFX_SEMCORECTRLPANEL_STAND7PPG_H__7479DF7B_C5F9_4DF9_83A3_BF3BB5E7AEF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SemCoreCtrlPanel_Stand7Ppg.h : Declaration of the CSemCoreCtrlPanel_Stand7PropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand7PropPage : See SemCoreCtrlPanel_Stand7Ppg.cpp.cpp for implementation.

class CSemCoreCtrlPanel_Stand7PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSemCoreCtrlPanel_Stand7PropPage)
	DECLARE_OLECREATE_EX(CSemCoreCtrlPanel_Stand7PropPage)

// Constructor
public:
	CSemCoreCtrlPanel_Stand7PropPage();

// Dialog Data
	//{{AFX_DATA(CSemCoreCtrlPanel_Stand7PropPage)
	enum { IDD = IDD_PROPPAGE_SEMCORECTRLPANEL_STAND7 };
	BOOL	m_bCaptionVisible;
	CString	m_strCaption;
	BOOL	m_bDebugInfoVisible;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSemCoreCtrlPanel_Stand7PropPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCORECTRLPANEL_STAND7PPG_H__7479DF7B_C5F9_4DF9_83A3_BF3BB5E7AEF6__INCLUDED)
