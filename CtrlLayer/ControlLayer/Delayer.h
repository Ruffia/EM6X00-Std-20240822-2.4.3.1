#pragma once
#include "USB_Functions.h"

class AFX_EXT_CLASS CDelayer
{
public:
	static CDelayer& Instance();
	void  Delay( int msecond );

protected:
	CDelayer();

protected:
	HANDLE	m_hTimeDelay;
};