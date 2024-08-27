#include "stdafx.h"
#include "MCICardOption.h"
#include "MCICardParam.h"


CMCICardOption& CMCICardOption::Instance()
{
	static CMCICardOption Inst;
	return Inst;
}


CMCICardOption::CMCICardOption()
{
	m_sysOpt.color[SysOpt_clrTransparent] = g_clrUSB_Transparent;

#ifdef GRAPHIC_Transparent
	m_sysOpt.color[SysOpt_clrBack]		= g_clrUSB_Transparent;
#else
	m_sysOpt.color[SysOpt_clrBack]		= g_clrUSB_Background;
#endif

	///////////////////////////////////////////////////////////////////
	m_sysOpt.color[SysOpt_clrEditText]			= g_clrUSB_EditText;
	m_sysOpt.color[SysOpt_clrLabelText]			= g_clrUSB_LabelText;
	m_sysOpt.color[SysOpt_clrLabelLogoText]		= g_clrUSB_LabelLogoText;
	m_sysOpt.color[SysOpt_clrOscillographGrid]	= RGB(0,255,0);
	m_sysOpt.color[SysOpt_clrOscillographGraph]	= g_clrUSB_MobileUScale;

	/////////////////////////////////////////////////////////////////
	// Footer 19.02.18
	for(int i=0; i<11; i++ )
		m_footerOpt.opt[i].bVisible = FALSE;

	m_sysOpt.color[SysOpt_clrFrameRate] = RGB(255,0,0);
}