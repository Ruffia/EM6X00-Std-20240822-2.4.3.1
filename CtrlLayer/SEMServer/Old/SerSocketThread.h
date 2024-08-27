#if !defined(AFX_SERSOCKETTHREAD_H__2838C1FA_6A30_4F8E_80B4_27E5CF794852__INCLUDED_)
#define AFX_SERSOCKETTHREAD_H__2838C1FA_6A30_4F8E_80B4_27E5CF794852__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SerSocketThread3.h : header file
//

#include "TCPServer.h"

#include "Include\\Json\\Json.h"

/////////////////////////////////////////////////////////////////////////////
// CSerSocketThread thread

class CTCPServer;
class CSerSocketThread : public CWinThread
{
	DECLARE_DYNCREATE(CSerSocketThread)
protected:
	CSerSocketThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
//	int		m_thrdIndex;

//	BYTE	m_byteRecv[3];
// 	BYTE	m_byteRecv[256];
// 	BYTE	m_byteConnected[2];	// 表示连接成功的回复

	HWND	m_destHandle;		// 主程序句柄
	SOCKET	m_hSocket;			// Socket句柄
	int		m_nPort;
/*
	void	Recv_TranslateJson( const std::string strData );
	void	Send_JsonString( Json::Value valueSend );
	void	Send_JsonString2( Json::Value valueSend );
	char*	char_mid(char *dst,char *src, int n,int m);
*/
	void	Update_DoubleParam( BYTE bcommand, double dValue );
	void	Update_DoubleParams( BYTE bcommand );//, KYKYCommMsg_Data5f 5fValue );
	void	Update_IntParam( BYTE bcommand, int nValue );
//	void	Update_IntParams( BYTE bcommand, CSize szValue );
	void	Send_SetCommand( BYTE bcommand );
/*
	void	Recv_SemGetType( const char* param );
	void	Recv_SemGetValue( const char* param );
	void	Recv_SemSetValue( Json::Value root );
	void	Recv_StageGetParam( const char* method );
	void	Recv_StageSetParam( Json::Value root, const char* method );
	void	Recv_StageSetCmd( const char* method );
	void	Recv_AppSetCommand( const char* param );
*/
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerSocketThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSerSocketThread();

	// Generated message map functions
	//{{AFX_MSG(CSerSocketThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void	OnMsgRead(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnMsgSend(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnMsgSendImg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERSOCKETTHREAD3_H__2838C1FA_6A30_4F8E_80B4_27E5CF794852__INCLUDED_)
