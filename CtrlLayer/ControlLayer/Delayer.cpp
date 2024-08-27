#include "stdafx.h"
#include "Delayer.h"

CDelayer& CDelayer::Instance()
{
	static CDelayer Inst;
	return Inst;
}


CDelayer::CDelayer()
{
	///////////////////////////////////////////////////////////
	m_hTimeDelay = CreateEvent( NULL, FALSE, TRUE, "TimeDelay" );
}


void CDelayer::Delay( int msecond )
{
	WaitForSingleObject( m_hTimeDelay, msecond );
}