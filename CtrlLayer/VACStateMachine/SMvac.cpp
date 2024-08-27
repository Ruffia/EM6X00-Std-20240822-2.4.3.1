// SMvac.cpp : implementation file
//

#include "stdafx.h"
#include "Vac.h"
#include "SMvac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMvac

CSMvac& CSMvac::Instance()
{
	static CSMvac Inst;
	return Inst;
}

CSMvac::CSMvac()
{
	m_pVac		= NULL;
	m_bReady	= FALSE;
}

CSMvac::~CSMvac()
{
	Close();
}


BOOL CSMvac::Close()
{
	if( m_pVac != NULL )
	{
		m_pVac->Close();
		m_pVac->KillThread();
		m_pVac	= NULL;
	}

	m_bReady	= FALSE;
	return TRUE;
}

BOOL CSMvac::Reset( CWnd* pWnd )
{
	Close();
	m_pVac = new CVac();
	ASSERT_VALID(m_pVac);

	if( m_pVac != NULL )
	{
		if( m_pVac->Reset( pWnd ) )
		{
			m_pVac->m_pThreadParams	= NULL;

			// Create Thread in a suspended state so we can set the Priority
			// before it starts getting away from us
			if( !m_pVac->CreateThread(CREATE_SUSPENDED))
			{
				delete m_pVac;
				m_pVac	= NULL;
			}
			else
			{
				//VERIFY(m_pVac->SetThreadPriority(THREAD_PRIORITY_IDLE));
				VERIFY(m_pVac->SetThreadPriority(THREAD_PRIORITY_NORMAL));
				//VERIFY(m_pVac->SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL));
				//VERIFY(m_pVac->SetThreadPriority(THREAD_PRIORITY_HIGHEST));

				// Now the thread can run wild
				m_pVac->ResumeThread();
				m_bReady = TRUE;
			}
		}
		else
			Close();
	}
	return m_bReady;
}

BOOL CSMvac::SetParams( int nType, DWORD dwParam )
{
	if( m_pVac != NULL )
		m_pVac->SetParams( nType, dwParam );
	return TRUE;
}

BOOL CSMvac::SetParams2( int nType, double dParam )
{
	if( m_pVac != NULL )
		m_pVac->SetParams2( nType, dParam );
	return TRUE;
}
