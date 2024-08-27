//TCPServer.cpp    Last modified 2015.7.30

#include "StdAfx.h"
#include "TCPServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

HANDLE g_hListen = CreateEvent(NULL,FALSE,FALSE,_T("Listen"));

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTCPServer::CTCPServer()
{
}

CTCPServer::CTCPServer(HWND handle)
{
	m_nPort = KYKYComm_ListenPort1;
	m_destHandle = handle;
 	m_psocketListen = NULL;	
}

CTCPServer::~CTCPServer()
{
	if (m_psocketListen)
	{
		delete m_psocketListen;
	}
	m_psocketListen = NULL;

	if (g_hListen)
	{
		CloseHandle(g_hListen);
	}
	g_hListen = NULL;
}

BOOL CTCPServer::StartListen()
{
	m_psocketListen = new CSEMSocket(m_destHandle);   
	if (!AfxSocketInit())
	{
		AfxMessageBox("socket ³õÊ¼»¯Ê§°Ü£¡");
		delete m_psocketListen;
		m_psocketListen = NULL;
		return FALSE;
	}
	if (!m_psocketListen->Create(m_nPort))
	{
		AfxMessageBox("¼àÌýsocket´´½¨Ê§°Ü£¡");
		delete m_psocketListen;
		m_psocketListen = NULL;
		return FALSE;
	}
	if (!m_psocketListen->Listen(20))
	{
		AfxMessageBox("¼àÌýsocket¼àÌýÊ§°Ü£¡");
		delete m_psocketListen;
		m_psocketListen = NULL;
		return FALSE;
	}
	m_psocketListen->m_nPort = m_nPort;
	return TRUE;
}

void CTCPServer::StopListen()
{
	if ( !m_psocketListen)
	{
		m_psocketListen->Close();    // modify
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTCPServer message handlers

void CTCPServer::SetPort(int nPort)
{
	m_nPort = nPort;
}
