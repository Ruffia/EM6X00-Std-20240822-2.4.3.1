// SemCtrl_GunCurrentAccelerate.cpp: implementation of the CSemCtrl_GunCurrentAccelerate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_GunCurrentAccelerate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_GunCurrentAccelerate::CSemCtrl_GunCurrentAccelerate()
{
//	m_scfCtrl = DefaultCtrlFigures_GunCurrentAccelerate;

	m_scfCtrl.varMin.dblVal = (DOUBLE)70;
	m_scfCtrl.varMax.dblVal = (DOUBLE)120;
	m_scfCtrl.varPos.dblVal = (DOUBLE)70;
}

CSemCtrl_GunCurrentAccelerate::~CSemCtrl_GunCurrentAccelerate()
{

}

const CString& CSemCtrl_GunCurrentAccelerate::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	double dblCtlIndex = varPos.dblVal;

	// Format Display string
	m_strText.Format( "%03.2f", dblCtlIndex );

	return m_strText;
}
