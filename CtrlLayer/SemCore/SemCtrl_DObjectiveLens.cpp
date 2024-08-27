// SemCtrl_DObjectiveLens.cpp: implementation of the CSemCtrl_DObjectiveLens class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_DObjectiveLens.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_DObjectiveLens::CSemCtrl_DObjectiveLens()
{
	m_scfCtrl = DefaultCtrlFigures_DObjectiveLens;
	m_lSwitch = 0;
}

CSemCtrl_DObjectiveLens::~CSemCtrl_DObjectiveLens()
{
}

BOOL CSemCtrl_DObjectiveLens::Flush()
{
	WORD				wCtl;
	SEM_ControlMessage	hostMSG;

	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	wCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);

	hostMSG.bMessageID		= (BYTE)Z80A_DObjectiveLens;
	hostMSG.bCtlMSGLength	= (BYTE)4;
	hostMSG.bData[1]		= (BYTE)m_lSwitch;
	hostMSG.bData[2]		= (BYTE)wCtl;
	hostMSG.bData[3]		= (BYTE)( wCtl >> 8 );
	
	BOOL blRet = FALSE;
/*	if ( m_pMPComm != NULL )
		blRet = CMP::Instance().PostMSG( SEM_Z80ID_Z80A, hostMSG );
*/	
	return blRet;
}

const CString& CSemCtrl_DObjectiveLens::FormatText()
{
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// Format Display string
	m_strText.Format( "%03.1f", 
		(double)(lCtlIndex - m_scfCtrl.varMin.lVal) * 100.0 / (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal) );

	return m_strText;
}

const CString& CSemCtrl_DObjectiveLens::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%03.1f", 
		(double)(lCtlIndex - m_scfCtrl.varMin.lVal) * 100.0 / (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal) );

	return strText;
}

const CString& CSemCtrl_DObjectiveLens::FormatSwitchText()
{
	// Format Display string
	if ( m_lSwitch != 0 )
		m_strSwitchText = m_pLanguageMan->GetResString( IDS_STATE_OPEN );
	else
		m_strSwitchText = m_pLanguageMan->GetResString( IDS_STATE_CLOSE );

	return m_strSwitchText;
}

const CString& CSemCtrl_DObjectiveLens::FormatSwitchText( VARIANT varSwitch )
{
	static CString strSwitchText;

	// Format Display string
	if ( varSwitch.lVal != 0 )
		strSwitchText = m_pLanguageMan->GetResString( IDS_STATE_OPEN );
	else
		strSwitchText = m_pLanguageMan->GetResString( IDS_STATE_CLOSE );

	return strSwitchText;
}
