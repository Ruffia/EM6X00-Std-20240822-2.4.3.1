#if !defined(AFX_PANELBSD_H__99C95E2B_D348_4EA6_9BD5_0D4148A5E9FD__INCLUDED_)
#define AFX_PANELBSD_H__99C95E2B_D348_4EA6_9BD5_0D4148A5E9FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelBSD.h : header file
//

#include "CommBSD.h"

/////////////////////////////////////////////////////////////////////////////
// CPanelBSD dialog

class CPanelBSD : public CDialog
{
	HWND		m_hMain;
	CCommBSD	m_BSD;

// Construction
public:
	CPanelBSD(CWnd* pParent = NULL);   // standard constructor
	void	InitHWND( HWND hWnd );
	void	Connect();
	void	Relink();

	void	SetStatus(BOOL bEnable);
	void	InitParams();
	void	BSD_Fine_Change( int nPos );
	void	BSD_Brt_Change( int nPos );

	int		m_nPort;		// ͨѶ�˿ں�
	BOOL	m_bIsReady;		// ͨѶ�˿��Ƿ��Ѵ�����
	char	m_cPathLocalize[256];
	BOOL	m_bFineRev;		// �Աȶȷ���

	///////////////////////////////////
	// ���ػ����
	WORD	m_wLanguageID;				// ��������
	void	SetLanguageID( WORD wID );	// ���������õ���������
	void	Localize();					// ���ػ�
	CString	m_strMsg[10];	// char* m_szMsg[10];// �ַ������飨ָ��char���͵�ָ�����飩
	///////////////////////////////////

// Dialog Data
	//{{AFX_DATA(CPanelBSD)
	enum { IDD = IDD_PANEL_BSD };
	CScrollBar	m_scrollFine;
	CScrollBar	m_scrollBrt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelBSD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPanelBSD)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeCOMBOPort();
	afx_msg void OnChannelsChanged();
	afx_msg void OnCoarseChanged();
	afx_msg void OnCHECKFine();
	afx_msg void OnRelink();
	afx_msg void OnCHECKInOut();
	//}}AFX_MSG
	afx_msg LRESULT OnUpdateBSD(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELBSD_H__99C95E2B_D348_4EA6_9BD5_0D4148A5E9FD__INCLUDED_)
