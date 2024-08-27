// Last Modify : 2010.05.20

//{{AFX_INCLUDES()
#include "semcorectrlpanel_stand1.h"
#include "semcorectrlpanel_stand7.h"
//}}AFX_INCLUDES

#if !defined(AFX_DLGIMPPANELACCESSORY_H__7F157A57_B22F_4665_8948_695E6B26530E__INCLUDED_)
#define AFX_DLGIMPPANELACCESSORY_H__7F157A57_B22F_4665_8948_695E6B26530E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImpPanelAccessory.h : header file
//

#include "MyGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelAccessory dialog

class CDlgImpPanelAccessory : public CDialog
{
// Construction
public:
	CDlgImpPanelAccessory(CWnd* pParent = NULL);   // standard constructor
	void	Localize(void);

	void	EnableCtrl( long lClassID, BOOL bEnable );

// Dialog Data
	//{{AFX_DATA(CDlgImpPanelAccessory)
	enum { IDD = IDD_IMP_PANEL_ACCESSORY };
	CButton	m_staticGroup;
	CSemCoreCtrlPanel_Stand1	m_ctrlDObjectiveLens;
	CSemCoreCtrlPanel_Stand7	m_ctrlDObjectiveLensSwitch;
	CSemCoreCtrlPanel_Stand1	m_ctrlDFocus;
	CSemCoreCtrlPanel_Stand7	m_ctrlDFocusSwitch;
	CSemCoreCtrlPanel_Stand1	m_ctrlYAdjust;
	CSemCoreCtrlPanel_Stand7	m_ctrlSACP;
	CSemCoreCtrlPanel_Stand7	m_ctrlImageReverse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImpPanelAccessory)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImpPanelAccessory)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPPANELACCESSORY_H__7F157A57_B22F_4665_8948_695E6B26530E__INCLUDED_)
