// VirtualDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "Display.h"
#include "VirtualDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVirtualDisplay

CVirtualDisplay::CVirtualDisplay()
{
	m_pDisplay		= NULL;
	m_dblZoom		= 1.0;
	m_sizeReso		= CSize(512,512);
	m_nusbFunction	= 1;
}

CVirtualDisplay::~CVirtualDisplay()
{
	Close();
}

/////////////////////////////////////////////////////////////////////////////
// CVirtualDisplay message handlers

HDIB CVirtualDisplay::GetDib()
{ 
	if ( !isReady() )
		return NULL;

	if ( m_pDisplay == NULL )
		return NULL;

	return m_pDisplay->CopyImageDib();
}


BOOL CVirtualDisplay::Close( )
{
	if ( m_pDisplay != NULL )
	{
		m_pDisplay->Close();
		m_pDisplay->KillThread();
		m_pDisplay = NULL;
	}

	m_bReady = FALSE;

	return TRUE;
}

BOOL CVirtualDisplay::Reset( CWnd* pWnd )
{
	Close( );

	m_pDisplay = new CDisplay();

	ASSERT_VALID(m_pDisplay);
	if ( m_pDisplay != NULL )
	{
		m_pDisplay->SetZoom( m_dblZoom );
		m_pDisplay->SetFunction( m_nusbFunction );
		m_pDisplay->SetResolution( m_sizeReso );

		if ( m_pDisplay->Reset( pWnd ) )
		{
			m_pDisplay->m_pThreadParams = NULL;

			// Create Thread in a suspended state so we can set the Priority
			// before it starts getting away from us
			if (!m_pDisplay->CreateThread(CREATE_SUSPENDED))
			{
				delete m_pDisplay;
				m_pDisplay = NULL;
			}
			else
			{
				//VERIFY(m_pDisplay->SetThreadPriority(THREAD_PRIORITY_IDLE));
//				VERIFY(m_pDisplay->SetThreadPriority(THREAD_PRIORITY_NORMAL));
				//VERIFY(m_pDisplay->SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL));
				VERIFY(m_pDisplay->SetThreadPriority(THREAD_PRIORITY_HIGHEST));

				// Now the thread can run wild
				m_pDisplay->ResumeThread();  // remark by LMH 2001-8-7 Test for ÂýÉ¨ËÀ»ú T9

				m_bReady = TRUE;
			}
		}
		else
		{
			Close();
		}
	}

	return m_bReady;
}

BOOL CVirtualDisplay::IsShouldReset()
{
	if ( m_pDisplay != NULL )
		return m_pDisplay->IsShouldReset();

	return FALSE;
}

BOOL CVirtualDisplay::WriteCommImageData( LPVOID lpBuff, long lBuffSize, CRect& rcFrameWindow )
{
	if ( m_pDisplay == NULL )
		return FALSE;

	if ( !(m_pDisplay->IsReady()) )
		return FALSE;

	m_pDisplay->ImageDibWrite( lpBuff, lBuffSize, rcFrameWindow );

	return TRUE;
}

void CVirtualDisplay::SetResolution(CSize szReso)
{
	m_sizeReso = szReso;

	if (m_pDisplay != NULL)
		m_pDisplay->SetResolution( m_sizeReso );
}

void CVirtualDisplay::SetZoom(double dblZoom)
{
	m_dblZoom = dblZoom;

	if (m_pDisplay != NULL)
		m_pDisplay->SetZoom( m_dblZoom );
}
