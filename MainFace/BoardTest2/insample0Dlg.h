// insample0Dlg.h : header file
//

#if !defined(AFX_INSAMPLE0DLG_H__BBBEB9F9_B94F_4B5D_BA42_96F952F4BE34__INCLUDED_)
#define AFX_INSAMPLE0DLG_H__BBBEB9F9_B94F_4B5D_BA42_96F952F4BE34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MacButtons.h"

#include "TabDialog.h"
#include "PageMCI.h"
#include "PageSL.h"
#include "PageVS.h"

#include "DlgDiagLogGraph.h"
#include "DlgAutoTestMain.h"

/////////////////////////////////////////////////////////////////////////////
// CInsample0Dlg dialog

class CInsample0Dlg : public CDialog
{
// Construction
public:
	CInsample0Dlg(CWnd* pParent = NULL);	// standard constructor
	~CInsample0Dlg();

	void	Init();
	void	Init2();
	void	Close();
	void	UpdateConfig();
	void	UpdateListString( CString listStr);
	void	UpdateListReadbackString( CString listStr );

	void	TestReadPort();
	void	TestReadFIFO();

	UINT	m_nTestTimes;
	UINT	m_nRec0Times;
	HANDLE	m_hFil;

	BOOL	m_bDiag;
	void	Diagnose( BOOL bDiag );
	CString	Diag_SCN_LNS();
	CString	Diag_VAC();
	CString	Diag_SPR();
	CString	Diag_MCI();
	CString	Diag_Logging();
	CString	Diag_Logging2();

	CDlgDiagLogGraph	m_dlgGraph;
	CDlgAutoTestMain	m_dlgAutotest;

// Dialog Data
	//{{AFX_DATA(CInsample0Dlg)
	enum { IDD = IDD_INSAMPLE0_DIALOG };
	CMacButton	m_btnResetComm;
	CMacCheckBox	m_btnDiag;
	CListBox	m_listReport;
	CListBox	m_listReadback;
	CMacCheckBox	m_btnTestLEDs;
	CStatic	m_dlgPos;
	CMacButton	m_btnCancel;
	CMacButton	m_btnOK;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsample0Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CInsample0Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTestLEDs();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDiagnose();
	afx_msg void OnResetCommunicate();
	afx_msg void OnCHECKTestReadPort();
	afx_msg void OnCHECKTestReadFIFO();
	afx_msg void OnDiagnoseSetup();
	afx_msg void OnClearList();
	afx_msg void OnClearFIFO();
	afx_msg void OnAutoTest();
	//}}AFX_MSG
	afx_msg LRESULT OnSerialRecv(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSerialSend(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	BOOL InitTabDialog();

	BOOL AddPagesToTabDialog();

private:
	CTabDialog*			m_pTabDialog;
	CMacRadioButton*	m_pBtnMCI;
	CMacRadioButton*	m_pBtnScnLns;
	CMacRadioButton*	m_pBtnVacSpr;
	CPageMCI*			m_pPageMCI;
	CPageSL*			m_pPageSL;
	CPageVS*			m_pPageVS;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSAMPLE0DLG_H__BBBEB9F9_B94F_4B5D_BA42_96F952F4BE34__INCLUDED_)
