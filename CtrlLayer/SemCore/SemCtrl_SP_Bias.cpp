// SemCtrl_SP_Bias.cpp : implementation file
//

#include "stdafx.h"
#include "SemCtrl_SP_Bias.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSemCtrl_SP_Bias

CSemCtrl_SP_Bias::CSemCtrl_SP_Bias()
{
	m_scfCtrl = DefaultCtrlFigures_SP_Bias;
}

CSemCtrl_SP_Bias::~CSemCtrl_SP_Bias()
{
}

BOOL CSemCtrl_SP_Bias::Flush()
{
	return TRUE;
}

const CString& CSemCtrl_SP_Bias::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// Format Display string
	m_strText.Format( "%ld uA", lCtlIndex );

	return m_strText;
}

const CString& CSemCtrl_SP_Bias::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%ld uA", lCtlIndex );

	return strText;
}


long  CSemCtrl_SP_Bias::GetPosByValue(const double dValue)
{
	return (long)dValue;
}