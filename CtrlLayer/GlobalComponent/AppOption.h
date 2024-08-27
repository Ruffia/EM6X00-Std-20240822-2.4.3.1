#pragma once
#include "USB_Functions.h"


class AFX_EXT_CLASS CAppOption  
{
public:
	static CAppOption& Instance();

	//判断系统设置是否改变
	BOOL  SystemSettingsChanged(const SYS_Options& sysOpt);

	int   ReadFooterSettings( char* path );
	void  SetFooterSettingsDefault();

protected:
	CAppOption();
	void  _ReadFooter_TimeFormat( char* buf );

public:
	SYS_Options				m_sysOpt;
	Footer_Options			m_footerOpt;
};
