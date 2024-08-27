#include "stdafx.h"
#include "SemCtrl_AdjustParameter.h"

const SemCtrlFigures DefaultCtrlFigures_AdjustmentParameter	= {{0,0,0,0, (long)10}, {0,0,0,0, (long)30}, {0,0,0,0, (long)0}};

CSemCtrl_AdjustParameter::CSemCtrl_AdjustParameter()
{
	m_scfCtrl = DefaultCtrlFigures_AdjustmentParameter;
	m_scfCtrl.varPos.lVal = 10;
}

CSemCtrl_AdjustParameter::~CSemCtrl_AdjustParameter()
{
}

const CString& CSemCtrl_AdjustParameter::FormatText()
{
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	m_strText.Format( "%0.1f", (double)lCtlIndex / 10.0);

	return m_strText;
}

const CString& CSemCtrl_AdjustParameter::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%.1f", (double)(lCtlIndex - m_scfCtrl.varMin.lVal) * 10.0/ (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal) );

	return strText;
}