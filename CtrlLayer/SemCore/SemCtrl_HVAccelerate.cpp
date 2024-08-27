// SemCtrl_HVAccelerate.cpp: implementation of the CSemCtrl_HVAccelerate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_HVAccelerate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_HVAccelerate::CSemCtrl_HVAccelerate()
{
//	m_scfCtrl = DefaultCtrlFigures_HVAccelerate;

	m_scfCtrl.varMin.dblVal = (DOUBLE)0;
	m_scfCtrl.varMax.dblVal = (DOUBLE)30;
	m_scfCtrl.varPos.dblVal = (DOUBLE)0;
}

CSemCtrl_HVAccelerate::~CSemCtrl_HVAccelerate()
{

}

const CString& CSemCtrl_HVAccelerate::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	double dblCtlIndex = varPos.dblVal;

	// Format Display string
	m_strText.Format( "%2.1f", dblCtlIndex );

	return m_strText;
}

