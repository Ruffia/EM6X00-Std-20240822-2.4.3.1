#pragma once
#include "USB_Functions.h"


class CMCICardOption  
{
public:
	static CMCICardOption& Instance();

protected:
	CMCICardOption();


public:
	SYS_Options				m_sysOpt;
	Footer_Options			m_footerOpt;
};
