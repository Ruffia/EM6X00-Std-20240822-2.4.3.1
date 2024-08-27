//TCPServer.cpp    Last modified 2022.02.17

#include "StdAfx.h"
#include "TCPServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

HANDLE g_hListenCmd = CreateEvent(NULL,FALSE,FALSE,_T("ListenCmd"));
HANDLE g_hListenImg = CreateEvent(NULL,FALSE,FALSE,_T("ListenImg"));

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTCPServer::CTCPServer()
{
}

CTCPServer::CTCPServer(HWND handle)
{
	m_nPortCmd	= KYKYComm_ListenPort1;
	m_nPortImg	= KYKYComm_ListenPort2;
	m_destHandle		= handle;
 	m_psocketListenCmd	= NULL;	
	m_psocketListenImg	= NULL;	
}

CTCPServer::~CTCPServer()
{
	if (m_psocketListenCmd)
		delete m_psocketListenCmd;
	m_psocketListenCmd = NULL;

	if (g_hListenCmd)
		CloseHandle(g_hListenCmd);
	g_hListenCmd = NULL;

	if (m_psocketListenImg)
		delete m_psocketListenImg;
	m_psocketListenImg = NULL;

	if (g_hListenImg)
		CloseHandle(g_hListenImg);
	g_hListenImg = NULL;
}

BOOL CTCPServer::StartListen( BOOL bCmd )
{
	if( bCmd )
	{
		m_psocketListenCmd = new CSSCmd(m_destHandle);   
		if (!AfxSocketInit())
		{
			AfxMessageBox("socket ³õÊ¼»¯Ê§°Ü£¡");
			delete m_psocketListenCmd;
			m_psocketListenCmd = NULL;
			return FALSE;
		}
		if (!m_psocketListenCmd->Create(m_nPortCmd))
		{
			AfxMessageBox("¼àÌýsocket´´½¨Ê§°Ü£¡");
			delete m_psocketListenCmd;
			m_psocketListenCmd = NULL;
			return FALSE;
		}
		if (!m_psocketListenCmd->Listen(20))
		{
			AfxMessageBox("¼àÌýsocket¼àÌýÊ§°Ü£¡");
			delete m_psocketListenCmd;
			m_psocketListenCmd = NULL;
			return FALSE;
		}
		m_psocketListenCmd->m_nPort = m_nPortCmd;
		return TRUE;
	}
	else
	{
		m_psocketListenImg = new CSSImg(m_destHandle);   
		if (!AfxSocketInit())
		{
			AfxMessageBox("socket ³õÊ¼»¯Ê§°Ü£¡");
			delete m_psocketListenImg;
			m_psocketListenImg = NULL;
			return FALSE;
		}
		if (!m_psocketListenImg->Create(m_nPortImg))
		{
			AfxMessageBox("¼àÌýsocket´´½¨Ê§°Ü£¡");
			delete m_psocketListenImg;
			m_psocketListenImg = NULL;
			return FALSE;
		}
		if (!m_psocketListenImg->Listen(20))
		{
			AfxMessageBox("¼àÌýsocket¼àÌýÊ§°Ü£¡");
			delete m_psocketListenImg;
			m_psocketListenImg = NULL;
			return FALSE;
		}
		m_psocketListenImg->m_nPort = m_nPortImg;
		return TRUE;
	}
}

void CTCPServer::StopListen( BOOL bCmd )
{
	if( bCmd )
	{
		if ( !m_psocketListenCmd)
			m_psocketListenCmd->Close();	// modify
	}
	else
	{
		if ( !m_psocketListenImg)
			m_psocketListenImg->Close();	// modify
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTCPServer message handlers

void CTCPServer::SetPort( int nPort, BOOL bCmd )
{
	if( bCmd )
		m_nPortCmd = nPort;
	else
		m_nPortImg = nPort;
}
