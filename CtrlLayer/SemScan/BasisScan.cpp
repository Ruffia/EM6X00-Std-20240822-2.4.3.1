// BasisScan.cpp : implementation file
//

#include "stdafx.h"
#include "BasisScan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CBasisScan
IMPLEMENT_DYNCREATE(CBasisScan, CWinThread)

CBasisScan::CBasisScan()
{
	m_bAutoDelete	= FALSE;

	m_boolOneOff	= FALSE;
	m_boolFinished	= FALSE;

	// kill event starts out in the signaled state
	m_hEventKill = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CBasisScan::~CBasisScan()
{
	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);
}

BOOL CBasisScan::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	while (WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT)
		MainLoop();

	// avoid entering standard message loop by returning FALSE
	return FALSE;  // TRUE;
}

int CBasisScan::ExitInstance()
{
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CBasisScan, CWinThread)
	//{{AFX_MSG_MAP(CBasisScan)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CBasisScan message handlers
BOOL CBasisScan::InitData()
{
	return TRUE;
}


void CBasisScan::Delete()
{
	CWinThread::Delete();
	VERIFY(SetEvent(m_hEventDead));
}

void CBasisScan::KillThread()
{
	VERIFY(SetEvent(m_hEventKill));
	WaitForSingleObject(m_hEventDead, INFINITE);
	
	WaitForSingleObject(m_hThread, INFINITE);
	
	delete this;
}

#ifdef _DEBUG_RECORD_ERRINFO_

BOOL CBasisScan::PostErrorInformation( CString& csError )
{
	if ( m_fifoError.IsFIFOFull() )
	{
		CString		sspTemp;
		m_fifoError.Output( sspTemp );
	}

	CString	csTickCount;
	CString csTime = CTime::GetCurrentTime().Format("[%H:%M:%S]");
	DWORD thisTickCount = timeGetTime();
	csTickCount.Format( "(0x%08X) ", thisTickCount );
	csTime	+= csTickCount;
	csError = csTime + csError;

	return m_fifoError.Input( csError ) != (POSITION)(-1);
}

BOOL CBasisScan::GetErrorInformation( CString& csError )
{
	if ( !m_fifoError.IsFIFOEmpty() )
	{
		CString		sspTemp;

		if ( m_fifoError.Output( sspTemp ) )
		{
			csError = sspTemp;

			return TRUE;
		}
	}

	return FALSE;
}

#endif //_DEBUG_RECORD_ERRINFO_
