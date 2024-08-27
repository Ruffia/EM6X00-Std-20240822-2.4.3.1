// Ccg.cpp : implementation file
//

#include "stdafx.h"
#include "Ccg.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCcg

IMPLEMENT_DYNCREATE(CCcg, CWinThread)

CCcg::CCcg()
{
	m_bAutoDelete	= FALSE;

	// kill event starts out in the signaled state
	m_hEventKill	= CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventDead	= CreateEvent(NULL, TRUE, FALSE, NULL);

	m_cfg.bReady	= FALSE;

	m_hMain			= NULL;
	m_hFil			= NULL;
}

CCcg::~CCcg()
{
	Close();
	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);
}

BOOL CCcg::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	// loop but check for kill notification
	while( WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT )
		MainLoop();

	// avoid entering standard message loop by returning FALSE
	return FALSE;	// TRUE;
}

int CCcg::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCcg, CWinThread)
	//{{AFX_MSG_MAP(CCcg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCcg message handlers

void CCcg::Delete()
{
	// calling the base here won't do anything but it is a good habit
	CWinThread::Delete();
	
	// acknowledge receipt of kill notification
	VERIFY(SetEvent(m_hEventDead));
}

void CCcg::KillThread()
{
	// Note: this function is called in the context of other threads,
	//  not the thread itself.
	
	// reset the m_hEventKill which signals the thread to shutdown
	VERIFY(SetEvent(m_hEventKill));
	
	// allow thread to run at higher priority during kill process
	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	WaitForSingleObject(m_hEventDead, INFINITE);
	WaitForSingleObject(m_hThread, INFINITE);
	
	// now delete CWinThread object since no longer necessary
	delete this;
}

BOOL CCcg::Close()
{
	BOOL bReady = m_cfg.bReady;
	m_cfg.bReady = FALSE;
	if( bReady )
	{
		if( m_CCG.IsOpened() )
			m_CCG.Close();
	}

	if( m_hFil != NULL )
		CloseHandle( m_hFil );
	m_hFil	= NULL;
	return TRUE;
}

BOOL CCcg::Reset( CWnd* pWnd )
{
	if( m_cfg.bReady )
		Close();

	m_hMain = pWnd->m_hWnd;
//	Init();

	return TRUE;
}

BOOL CCcg::Init()
{
	m_cfg.bReady	= FALSE;
	m_cfg.dVac		= 0.0;

	m_lastTicks	= 0;
	// 串口端口设置
	if( m_CCG.Open( m_cfg.nPort ) )
	{
		m_cfg.bReady	= TRUE;
		m_lastTicks		= GetTickCount();
	}
	PostMessage( m_hMain, WM_USER_VAC_UpdateStatus, m_cfg.nType, m_cfg.bReady );

	/////////////////////////////////////////////
	// 日志
	if( m_hFil != NULL )
		CloseHandle( m_hFil );
	m_hFil	= NULL;
	// 设置日志
	TCHAR exeFullPath[255];
	::GetModuleFileName( NULL, exeFullPath, 255 );
	char *ptemp = strrchr( exeFullPath, '\\');
	ptemp[1] = 0;

	CString str, strTemp;
	CTime	timeCurrent = CTime::GetCurrentTime();
	strTemp = timeCurrent.Format("%Y%m%d%H%M");
	// 设置IP日志文件
	str.Format( "%szCCG%s.txt", exeFullPath, strTemp );
	m_hFil = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	m_nLoops = 1;
	// 写IP日志文件头
	if( m_hFil != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		str.Format( "\t%s", "CCG" );
		sprintf( Buf, "%s\n", str );
		WriteFile( m_hFil, Buf, strlen(Buf), &cbRet, NULL );
	}
	/////////////////////////////////////////////

	return TRUE;
}

void CCcg::ClearDevice()
{

}

void CCcg::SetParam( int nIndex, int nValue )
{
	switch( nIndex )
	{
	case	0:
		m_cfg.nPort	= nValue;
		break;
	case	1:
		m_cfg.nType	= nValue;
		break;
	}
}

void CCcg::MainLoop()
{
	if( !m_cfg.bReady )
	{
		Sleep( 1 );
		return;
	}
	
	DWORD thisTickCount = GetTickCount();
	///////////////////////////////////////////////////////
	// 常规监测
	///////////////////////////////////////////////////////
	if( m_lastTicks > 0 && thisTickCount - m_lastTicks > 1000 )
	{
		char dataSend[255];
		int nLen = 0;//ConvertComData22( "U01?", dataSend );
		m_CCG.SendData(dataSend, nLen);	//发送数据
		m_lastTicks = GetTickCount();
	}
	
	Sleep( 1 );
}
