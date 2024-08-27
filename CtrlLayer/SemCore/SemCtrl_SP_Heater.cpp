// SemCtrl_SP_Heater.cpp : implementation file
//

#include "stdafx.h"
#include "SemCtrl_SP_Heater.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSemCtrl_SP_Heater

CSemCtrl_SP_Heater::CSemCtrl_SP_Heater()
{
	m_scfCtrl = DefaultCtrlFigures_SP_Heater;
}

CSemCtrl_SP_Heater::~CSemCtrl_SP_Heater()
{
}

BOOL CSemCtrl_SP_Heater::Flush()
{
	return TRUE;
}

const CString& CSemCtrl_SP_Heater::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// Format Display string
	m_strText.Format( "%.2f A", lCtlIndex * 0.01 );

	return m_strText;
}

const CString& CSemCtrl_SP_Heater::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%.2f A", lCtlIndex * 0.01 );

	return strText;
}