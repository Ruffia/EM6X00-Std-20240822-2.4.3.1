// WSM30KVTestDlg.h : header file
//

#if !defined(AFX_WSM30KVTESTDLG_H__DBB482BC_D9CC_4FD2_AD8C_7AEC5C4384DD__INCLUDED_)
#define AFX_WSM30KVTESTDLG_H__DBB482BC_D9CC_4FD2_AD8C_7AEC5C4384DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <PMSKV.h>
#include "CommWSM30KV.h"

/////////////////////////////////////////////////////////////////////////////
// CWSM30KVTestDlg dialog

class CWSM30KVTestDlg : public CDialog
{
// Construction
public:
	CWSM30KVTestDlg(CWnd* pParent = NULL);	// standard constructor
	~CWSM30KVTestDlg();
	void	InitWSM30KV();

	CBrush	m_brushHLgreen;
	CBrush	m_brushHLred;

	CCommWSM30KV	m_WSM30KV;
	Cfg_WSM30KV		m_cfgWSM30KV;
	Status_WSM30KV	m_statusWSM30KV;
	Mon_WSM30KV		m_monWSM30KV;
	Setting_WSM30KV	m_setWSM30KV;

	void	UpdateStatus_01( WPARAM wParam, LPARAM lParam );
	void	UpdateStatus_25( WPARAM wParam, LPARAM lParam );
	void	UpdateInquire_01( WPARAM wParam, LPARAM lParam );
	void	UpdateInquire_25( WPARAM wParam, LPARAM lParam );
	void	UpdateMon_VT( WPARAM wParam, LPARAM lParam );
	void	UpdateMon_FB( WPARAM wParam , LPARAM lParam );
	void	UpdateMon_Given( WPARAM wParam , LPARAM lParam );

	void	Display_Status_Output( CDC* pDC, HBRUSH& hbr, int nID );
	void	Display_Status_Protect01( CDC* pDC, HBRUSH& hbr, int nID );
	void	Display_Status_Protect25( CDC* pDC, HBRUSH& hbr, int nID );
	void	Display_Status_ProtectOV( CDC* pDC, HBRUSH& hbr, int nID );
	void	Display_Status_ProtectOC( CDC* pDC, HBRUSH& hbr, int nID );

	void	Change_ACL_V( int nPos );
	void	Change_ACL_I( int nPos );
	void	Change_FIL_I( int nPos );
	void	Change_PMT_V( int nPos );

// Dialog Data
	//{{AFX_DATA(CWSM30KVTestDlg)
	enum { IDD = IDD_WSM30KVTEST_DIALOG };
	CButton	m_checkV1;
	CButton	m_checkFIL;
	CButton	m_checkEMS;
	CButton	m_checkACL;
	CScrollBar	m_scrollPMTv;
	CScrollBar	m_scrollFILi;
	CScrollBar	m_scrollACLv;
	CScrollBar	m_scrollACLi;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWSM30KVTestDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWSM30KVTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBUTTONWSM30KVLink();
	afx_msg void OnCHECKWSM30KVV1Test();
	afx_msg void OnCheckWsm30kvAcl();
	afx_msg void OnCheckWsm30kvEms();
	afx_msg void OnCheckWsm30kvFil();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBUTTONTest();
	afx_msg void OnCheckWsm30kvSci();
	afx_msg void OnCheckWsm30kvCol();
	afx_msg void OnCheckWsm30kvPmt();
	afx_msg void OnBUTTONWSM30KVClearFault();
	//}}AFX_MSG
	afx_msg LRESULT OnWSM30KVSetChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWSM30KVUpdateSet(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWSM30KVUpdateMon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWSM30KVUpdateStatus(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSM30KVTESTDLG_H__DBB482BC_D9CC_4FD2_AD8C_7AEC5C4384DD__INCLUDED_)
