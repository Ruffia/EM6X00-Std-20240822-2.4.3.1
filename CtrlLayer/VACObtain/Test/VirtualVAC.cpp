// VirtualVAC.cpp : implementation file
//

#include "stdafx.h"
#include "IonPump.h"
#include "Ccg.h"
#include "VirtualVAC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVirtualVAC

CVirtualVAC::CVirtualVAC()
{
	m_pIPG	= NULL;
	m_pIPC	= NULL;
	m_pCCG	= NULL;
	m_nInit	= -1;

	for( int i=0; i<3; i++ )
		m_nPort[i] = 0;
}

CVirtualVAC::~CVirtualVAC()
{
}

/////////////////////////////////////////////////////////////////////////////
// CVirtualVAC message handlers

BOOL CVirtualVAC::ClearDevice()
{
	if( m_pIPG != NULL )
		m_pIPG->ClearDevice();
	if( m_pIPC != NULL )
		m_pIPC->ClearDevice();
	if( m_pCCG != NULL )
		m_pCCG->ClearDevice();
	Close(1);
	Close(2);
	Close(3);
	return TRUE;
}

BOOL CVirtualVAC::Close( int nType )
{
	switch( nType )
	{
	case	1:
		if( m_pIPG != NULL )
		{
			m_pIPG->Close();
			m_pIPG->KillThread();
			m_pIPG = NULL;
		}
		break;
	case	2:
		if( m_pIPC != NULL )
		{
			m_pIPC->Close();
			m_pIPC->KillThread();
			m_pIPC = NULL;
		}
		break;
	case	3:
		if( m_pCCG != NULL )
		{
			m_pCCG->Close();
			m_pCCG->KillThread();
			m_pCCG = NULL;
		}
		break;
	}

	m_nInit = -1;
	return TRUE;
}

BOOL CVirtualVAC::Reset( CWnd* pWnd )
{
	Close(1);
	Close(2);
	Close(3);
	m_pIPG = new CIonPump();
	ASSERT_VALID(m_pIPG);
	m_pIPC = new CIonPump();
	ASSERT_VALID(m_pIPC);
// 	m_pCCG = new CCcg();
// 	ASSERT_VALID(m_pCCG);

	if( m_pIPG != NULL )
	{
		m_pIPG->SetParam( 1, 1 );	// set type
		if( m_pIPG->Reset( pWnd ) )
		{
			m_pIPG->m_pThreadParams		= NULL;

			// Create Thread in a suspended state so we can set the Priority
			// before it starts getting away from us
			if( !m_pIPG->CreateThread(CREATE_SUSPENDED))
			{
				delete m_pIPG;
				m_pIPG	= NULL;
			}
			else
			{
				//VERIFY(m_pIPG->SetThreadPriority(THREAD_PRIORITY_IDLE));
				VERIFY(m_pIPG->SetThreadPriority(THREAD_PRIORITY_NORMAL));
				//VERIFY(m_pIPG->SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL));
				//VERIFY(m_pIPG->SetThreadPriority(THREAD_PRIORITY_HIGHEST));

				m_pIPG->SetParam( 0, m_nPort[0] );
				m_pIPG->Init();

				// Now the thread can run wild
				m_pIPG->ResumeThread();
				m_nInit = 1;
			}
		}
		else
			Close(1);
	}
	if( m_pIPC != NULL )
	{
		m_pIPC->SetParam( 1, 2 );	// set type
		if( m_pIPC->Reset( pWnd ) )
		{
			m_pIPC->m_pThreadParams		= NULL;

			// Create Thread in a suspended state so we can set the Priority
			// before it starts getting away from us
			if( !m_pIPC->CreateThread(CREATE_SUSPENDED))
			{
				delete m_pIPC;
				m_pIPC	= NULL;
			}
			else
			{
				//VERIFY(m_pIPC->SetThreadPriority(THREAD_PRIORITY_IDLE));
				VERIFY(m_pIPC->SetThreadPriority(THREAD_PRIORITY_NORMAL));
				//VERIFY(m_pIPC->SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL));
				//VERIFY(m_pIPC->SetThreadPriority(THREAD_PRIORITY_HIGHEST));

				m_pIPC->SetParam( 0, m_nPort[1] );
				m_pIPC->Init();

				// Now the thread can run wild
				m_pIPC->ResumeThread();
				m_nInit = 1;
			}
		}
		else
			Close(2);
	}
/*	if( m_pCCG != NULL )
	{
		m_pCCG->SetParam( 1, 3 );
		if( m_pCCG->Reset( pWnd ) )
		{
			m_pCCG->m_pThreadParams		= NULL;

			// Create Thread in a suspended state so we can set the Priority
			// before it starts getting away from us
			if( !m_pCCG->CreateThread(CREATE_SUSPENDED))
			{
				delete m_pCCG;
				m_pCCG	= NULL;
			}
			else
			{
				//VERIFY(m_pCCG->SetThreadPriority(THREAD_PRIORITY_IDLE));
				VERIFY(m_pCCG->SetThreadPriority(THREAD_PRIORITY_NORMAL));
				//VERIFY(m_pCCG->SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL));
				//VERIFY(m_pCCG->SetThreadPriority(THREAD_PRIORITY_HIGHEST));

				m_pCCG->SetParam( 0, m_nPort[2] );
				m_pCCG->Init();

				// Now the thread can run wild
				m_pCCG->ResumeThread();
				m_nInit = 1;
			}
		}
		else
			Close(3);
	}*/
	return m_nInit;
}

void CVirtualVAC::SetParam( int index, int nValue )
{
	switch( index )
	{
	case	1:
		m_nPort[0]	= nValue;
		break;
	case	2:
		m_nPort[1]	= nValue;
		break;
	case	3:
		m_nPort[2]	= nValue;
		break;
	}
}