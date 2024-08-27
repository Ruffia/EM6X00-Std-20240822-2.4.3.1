//  TCPServer.h   Last modified 2022.02.17

#if !defined(AFX_TCPSERVER_H__0B45430C_3294_4666_90BB_B0F5040EAAC1__INCLUDED_)
#define AFX_TCPSERVER_H__0B45430C_3294_4666_90BB_B0F5040EAAC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SSCmd.h"
#include "SSImg.h"
#include "CommFunctions.h"

class CSSCmd;
class CSSCmdThread;
class CSSImg;
class CSSImgThread;
class AFX_EXT_CLASS CTCPServer
{
public:
	CTCPServer();
    CTCPServer(HWND handle);
	~CTCPServer();
	BOOL	StartListen(BOOL bCmd = TRUE);	// 成功返回0，错误返回错误码
	void	StopListen(BOOL bCmd = TRUE);

private:
	int		m_nPortCmd, m_nPortImg;

public:
	void	SetPort(int nPort, BOOL bCmd = TRUE);
	CSSCmd*	m_psocketListenCmd;	// 监听命令参数
	CSSImg*	m_psocketListenImg;	// 监听图像数据
	HWND	m_destHandle;
};

// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_TCPSERVER_H__0B45430C_3294_4666_90BB_B0F5040EAAC1__INCLUDED_)