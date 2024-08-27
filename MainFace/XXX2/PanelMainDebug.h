// Last Modify : 2009.03.12

#if !defined(AFX_PANELMAINDEBUG_H__7C0F4977_343D_477B_96A4_1F21C477BF14__INCLUDED_)
#define AFX_PANELMAINDEBUG_H__7C0F4977_343D_477B_96A4_1F21C477BF14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelMainDebug.h : header file
//

#include "PanelMCI.h"
#include "PanelSL.h"
#include "PanelVS.h"
#include "DlgImpPanelDebug2.h"
#include "DlgImpPanelDebugA.h"

/////////////////////////////////////////////////////////////////////////////
// CPanelMainDebug dialog

class CPanelMainDebug : public CDialogBar
{
// Construction
public:
	CPanelMainDebug();   // standard constructor
	~CPanelMainDebug();
	virtual BOOL Create( CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID );

	BOOL	CreateCtlDlgs();
	BOOL	Init();
	void	AdjustMainDlg();
	void	AdjustCtlDlgPos(CWnd* pCtlDlg);
	void	AdjustAllCtlDlg();
	void	AdjustCtrlsPos();
	void	ChangeActivePage(CDialog* pNewActivePage);
	int		SelchangePage( int nIndex );
	BOOL	IsActivePage( int nIndex );
	void	Localize(void);

	PageAssamblePanel*	m_pPageTable;
	int					m_nPageTableLength;
	int					m_nSelectPage;

	CDialog*			m_pActivePage;
	CDlgImpPanelDebug2	m_PageDebug2;
	CDlgImpPanelDebugA	m_PageDebugA;
	CPanelMCI			m_PagePanelMCI;
	CPanelSL			m_PagePanelSL;
	CPanelVS			m_PagePanelVS;

// Dialog Data
	//{{AFX_DATA(CPanelMainDebug)
	enum { IDD = IDD_3K2_PANEL_MAIN_DEBUG };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelMainDebug)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CPanelMainDebug)
	afx_msg LONG OnInitDialog ( UINT, LONG );
	afx_msg void OnLastStep();
	afx_msg void OnNextStep();
	afx_msg void OnResetCommunicate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELMAINDEBUG_H__7C0F4977_343D_477B_96A4_1F21C477BF14__INCLUDED_)
