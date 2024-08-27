// CommTestServerDlg.h : header file
//

#if !defined(AFX_COMMTESTSERVERDLG_H__574CA44B_0F4C_415C_BDF0_85B857E5CC32__INCLUDED_)
#define AFX_COMMTESTSERVERDLG_H__574CA44B_0F4C_415C_BDF0_85B857E5CC32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TCPServer.h"
#include "CommFunctions.h"

/////////////////////////////////////////////////////////////////////////////
// CCommTestServerDlg dialog

class CCommTestServerDlg : public CDialog
{
// Construction
public:
	CCommTestServerDlg(CWnd* pParent = NULL);	// standard constructor

	/////////////////////////////////////////////
	// Remote
	CWinThread*		m_commandThd;	// 客户端接收和发送消息的线程
	CTCPServer*		m_pServer;		// 发送控制命令
	WORD	versionNumber;
	BOOL	bListenStarted;
	CRect	rcLast;
	int		nRMType;
	int		nMagnification;
	int		nScanRotAng;
	
	void	KYKYComm_EndConnectThread();
	void	KYKYComm_SendMag();
	void	KYKYComm_SendSRotAng();
	void	KYKYComm_SendReso();
	/////////////////////////////////////////////
private:
	volatile BOOL	bBatchSendEnd;
	/////////////////////////////////////////////

// Dialog Data
	//{{AFX_DATA(CCommTestServerDlg)
	enum { IDD = IDD_COMMTESTSERVER_DIALOG };
	int		m_nMag;
	int		m_nRotAngle;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommTestServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCommTestServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnBUTTONListenStart();
	afx_msg void OnBUTTONListenStop();
	afx_msg void OnDeltaposSPINMag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSPINRotAngle(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnKYKYComm_ReplyAsk(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKYKYComm_ConnectThread(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMTESTSERVERDLG_H__574CA44B_0F4C_415C_BDF0_85B857E5CC32__INCLUDED_)
