// CommTestClientDlg.h : header file
//

#if !defined(AFX_COMMTESTCLIENTDLG_H__608DDB06_033F_454F_9A11_77E9D2D7A111__INCLUDED_)
#define AFX_COMMTESTCLIENTDLG_H__608DDB06_033F_454F_9A11_77E9D2D7A111__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KYKYSEMComm.h"
#include "RemoteFunctions.h"

/////////////////////////////////////////////////////////////////////////////
// CCommTestClientDlg dialog

//CKYKYClientWrapperApp SEMClient;

class CCommTestClientDlg : public CDialog
{
// Construction
public:
	CCommTestClientDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CCommTestClientDlg();

	void	Disconnect(BOOL bActive);  //主动or 被动

	void	SetConnectStatus(BOOL bEnable);
	void	SetSendStatus(BOOL bEnable);
	void	Message(LPCTSTR lpszMessage);

	WORD	m_wVersionNumber;
	BOOL	m_bConnected, m_bCmdConnected;     //命令socket

	CKYKYSEMComm	m_Client;

// Dialog Data
	//{{AFX_DATA(CCommTestClientDlg)
	enum { IDD = IDD_COMMTESTCLIENT_DIALOG };
	CIPAddressCtrl	m_IPAddr;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommTestClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCommTestClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBUTTONConnect();
	afx_msg void OnBUTTONDisconnect();
	afx_msg void OnRADIOScanCtrlSpeed1();
	afx_msg void OnRADIOScanCtrlSpeed2();
	afx_msg void OnRADIOScanCtrlSpeed3();
	afx_msg void OnRADIOScanCtrlSpeed4();
	afx_msg void OnRADIOScanCtrlReso1();
	afx_msg void OnRADIOScanCtrlReso2();
	afx_msg void OnRADIOScanCtrlReso3();
	afx_msg void OnRADIOScanCtrlReso4();
	afx_msg void OnRADIOScanCtrlType1();
	afx_msg void OnRADIOScanCtrlType2();
	afx_msg void OnRADIOScanCtrlType3();
	afx_msg void OnRADIOScanCtrlType4();
	afx_msg void OnRADIOScanCtrlType5();
	afx_msg void OnRADIOScanCtrlType6();
	afx_msg void OnRADIOScanCtrlType7();
	afx_msg void OnRADIOQueryVersion();
	afx_msg void OnClose();
	afx_msg void OnRADIOQueryMag();
	afx_msg void OnRADIOQueryRotAngle();
	afx_msg void OnRADIOQueryWD();
	afx_msg void OnRADIOQueryHV();
	afx_msg void OnRADIOQueryEMI();
	//}}AFX_MSG
	afx_msg LRESULT OnReceiveData(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnGetConnectStatus(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMTESTCLIENTDLG_H__608DDB06_033F_454F_9A11_77E9D2D7A111__INCLUDED_)
