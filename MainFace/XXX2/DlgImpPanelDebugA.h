// Last Modify : 2010.05.20

//{{AFX_INCLUDES()
#include "semcorectrlpanel_stand1.h"
#include "semcorectrlpanel_stand6.h"
#include "semcorectrlpanel_stand7.h"
//}}AFX_INCLUDES

#if !defined(AFX_DLGIMPPANELDEBUGA_H__03C4D707_CDC8_44BB_B6B7_F6C7645ED81C__INCLUDED_)
#define AFX_DLGIMPPANELDEBUGA_H__03C4D707_CDC8_44BB_B6B7_F6C7645ED81C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImpPanelDebugA.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelDebugA dialog

class CDlgImpPanelDebugA : public CDialog
{
// Construction
public:
	CDlgImpPanelDebugA(CWnd* pParent = NULL);   // standard constructor
	void	Localize(void);
	
	void	EnableCtrl( long lClassID, BOOL bEnable );

// Dialog Data
	//{{AFX_DATA(CDlgImpPanelDebugA)
	enum { IDD = IDD_IMP_PANEL_DEBUGA };
	CSemCoreCtrlPanel_Stand1	m_ctrlDObjectiveLens;
	CSemCoreCtrlPanel_Stand7	m_ctrlDObjectiveLensSwitch;
	CSemCoreCtrlPanel_Stand1	m_ctrlDFocus;
	CSemCoreCtrlPanel_Stand7	m_ctrlDFocusSwitch;
	CSemCoreCtrlPanel_Stand6	m_ctrlObjectiveLens;
	CSemCoreCtrlPanel_Stand1	m_ctrlYAdjust;
	CSemCoreCtrlPanel_Stand7	m_ctrlSACP;
	CSemCoreCtrlPanel_Stand7	m_ctrlImageReverse;
	CSemCoreCtrlPanel_Stand1	m_ctrlRobinsonDetector;
	CSemCoreCtrlPanel_Stand7	m_ctrlRobinsonDetectorSwitch;
	CSemCoreCtrlPanel_Stand1	m_ctrlSignal2Brightness;
	CSemCoreCtrlPanel_Stand1	m_ctrlBrightness;
	int		m_nCS1State;
	int		m_nCS2State;
	int		m_nSPState;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImpPanelDebugA)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImpPanelDebugA)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnReportPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	afx_msg void OnCS1Change();
	afx_msg void OnCS2Change();
	afx_msg void OnSPChange();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPPANELDEBUGA_H__03C4D707_CDC8_44BB_B6B7_F6C7645ED81C__INCLUDED_)
