//  TCPServer.h   Last modified 2015.7.30

#if !defined(AFX_TCPSERVER_H__0B45430C_3294_4666_90BB_B0F5040EAAC1__INCLUDED_)
#define AFX_TCPSERVER_H__0B45430C_3294_4666_90BB_B0F5040EAAC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\SEMSocket/SemSocket.h"
#include "CommFunctions.h"

class CSEMSocket;
class CSerSocketThread;
class AFX_EXT_CLASS CTCPServer
{
public:
	CTCPServer();
    CTCPServer(HWND handle);
	~CTCPServer();
	BOOL	StartListen();	// 成功返回0，错误返回错误码
	void	StopListen();

private:
	int		m_nPort;

public:
	void	SetPort(int nPort);
	CSEMSocket*		m_psocketListen;		// 监听CSocket
//	CSEMImgSocket*	m_psocketListenImage;	// 监听图像数据
	HWND	m_destHandle;
};

// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_TCPSERVER_H__0B45430C_3294_4666_90BB_B0F5040EAAC1__INCLUDED_)