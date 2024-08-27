// SemCtrl_CPS_FilamentCurrent.cpp: implementation of the CSemCtrl_CPS_FilamentCurrent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_CPS_FilamentCurrent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL GetMyProfileDoubleEx( LPCTSTR lpKey, LPCTSTR lpSubKey, double& value);

/////////////////////////////////////////////////////////////////////////////
// CSemCtrl_CPS_FilamentCurrent

CSemCtrl_CPS_FilamentCurrent::CSemCtrl_CPS_FilamentCurrent()
{
	m_scfCtrl = DefaultCtrlFigures_CPS_FilamentCurrent;

	m_dIScale_ABS	= 5.0;
	m_dIScale_Set	= 6.25;
	m_dIOffset_Set	= 0;

	m_dIScale		= 6.25;
	m_dIOffset		= 0;
	m_dVScale		= 5;
	m_dVOffset		= 0;

	m_lUserMode		= 1;
	m_lEnable		= 0;

	m_lOldCtlIndex	= 0;

	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "IscaleFilamentAbs", m_dIScale_ABS );
	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "IscaleFilamentSet", m_dIScale_Set );
	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "IoffsetFilamentSet", m_dIOffset_Set );

	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "IscaleFilament", m_dIScale );
	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "IoffsetFilament", m_dIOffset );
	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "VscaleFilament", m_dVScale );
	GetMyProfileDoubleEx(CtrlFigures_RegRootKey, "VoffsetFilament", m_dVOffset );
}

CSemCtrl_CPS_FilamentCurrent::~CSemCtrl_CPS_FilamentCurrent()
{
}

BOOL CSemCtrl_CPS_FilamentCurrent::Flush()
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

const CString& CSemCtrl_CPS_FilamentCurrent::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// Format Display string
	m_strText.Format( "%.2f A", lCtlIndex * 0.01 );

	return m_strText;
}

const CString& CSemCtrl_CPS_FilamentCurrent::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%.2f A", lCtlIndex * 0.01 );

	return strText;
}

long CSemCtrl_CPS_FilamentCurrent::GetPosByValue(const double dValue)
{
	long lCtrlIndex = dValue * 100;
	return lCtrlIndex;
}
