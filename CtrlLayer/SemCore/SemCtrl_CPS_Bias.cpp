// SemCtrl_CPS_Bias.cpp: implementation of the CSemCtrl_CPS_Bias class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_CPS_Bias.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL GetMyProfileDoubleEx( LPCTSTR lpKey, LPCTSTR lpSubKey, double& value);

/////////////////////////////////////////////////////////////////////////////
// CSemCtrl_CPS_Bias

CSemCtrl_CPS_Bias::CSemCtrl_CPS_Bias()
{
	m_scfCtrl = DefaultCtrlFigures_CPS_Bias;

	m_dVScale_ABS	= 500;
	m_dVScale_Set	= 2150;
	m_dVOffset_Set	= 0;
	m_dVscale_Set_r	= 2.84;		// output r=v/i ratio

	m_dVScale		= 2150;
	m_dVOffset		= 0;
	m_dIScale		= 500e-6;
	m_dIOffset		= 0;

	m_lUserMode		= 1;
	m_lEnable		= 0;

	m_lOldCtlIndex	= 0;

	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "VscaleBiasAbs", m_dVScale_ABS );
	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "VscaleBiasSet", m_dVScale_Set );
	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "VoffsetBiasSet", m_dVOffset_Set );
	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "VscaleBiasSetR", m_dVscale_Set_r );

	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "VscaleBias", m_dVScale );
	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "VoffsetBias", m_dVOffset );
	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "IscaleBias", m_dIScale );
	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "IoffsetBias", m_dIOffset );
}

CSemCtrl_CPS_Bias::~CSemCtrl_CPS_Bias()
{
}

BOOL CSemCtrl_CPS_Bias::Flush()
{
	if( !m_lEnable )
		return FALSE;

	// GetPos
	VARIANT varPos = GetPos();
	if( m_lOldCtlIndex == varPos.lVal )
		return FALSE;

	m_lOldCtlIndex = varPos.lVal;
	long lCtlIndex = varPos.lVal;

	BOOL blRet = FALSE;

	return blRet;
}

const CString& CSemCtrl_CPS_Bias::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// Format Display string
	m_strText.Format( "%d V", lCtlIndex );

	return m_strText;
}

const CString& CSemCtrl_CPS_Bias::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%d V", lCtlIndex );

	return strText;
}


long  CSemCtrl_CPS_Bias::GetPosByValue(const double dValue)
{
	long lCtlIndex = dValue;
	return lCtlIndex;
}
