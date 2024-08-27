// ThreadRecvImage.h: interface for the CThreadRecvImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADRECVIMAGE_H__A4BC029A_3925_4163_9B67_B6D565619DEB__INCLUDED_)
#define AFX_THREADRECVIMAGE_H__A4BC029A_3925_4163_9B67_B6D565619DEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxsock.h>
#include "CommFunctions.h" 

typedef struct
{
	SOCKET	sock;
	LPVOID	ptr;	

}PARAMRECV;

class CThreadRecvImage  
{
public:
	CThreadRecvImage();
	virtual ~CThreadRecvImage();

	void	StopRecvThread();
	void	SetServerIP(char * sIP);

//	CThreadRecvImage(LPVOID ptr);
	static	DWORD	WINAPI	ThreadRequestImage(LPVOID lParam);
	void	RequestImage();

private:
	//接收线程结束符
	volatile BOOL	m_bEndRecvThread;
	
	//指向窗口的句柄
	CWnd*	m_pWnd;
	char	m_chIPAddr[15];
};

#endif // !defined(AFX_THREADRECVIMAGE_H__A4BC029A_3925_4163_9B67_B6D565619DEB__INCLUDED_)
