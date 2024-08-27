// SemCtrl_BseBySeDetectorSwitch.cpp : implementation of the CSemCtrl_BseBySeDetectorSwitch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_BseBySeDetectorSwitch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_BseBySeDetectorSwitch::CSemCtrl_BseBySeDetectorSwitch()
{
	m_scfCtrl = DefaultCtrlFigures_RobinsonDetectorSwitch;
}

CSemCtrl_BseBySeDetectorSwitch::~CSemCtrl_BseBySeDetectorSwitch()
{
}

BOOL CSemCtrl_BseBySeDetectorSwitch::Flush()
{
	WORD				wCtl;
	SEM_ControlMessage	hostMSG;

	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	wCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);

	hostMSG.bMessageID		= (BYTE)Z80B_BseBySeDetectorSwitch;
	hostMSG.bCtlMSGLength	= (BYTE)2;
	hostMSG.bData[1]		= (BYTE)wCtl;
	
	BOOL blRet = FALSE;
/*	if ( m_pMPComm != NULL )
		blRet = CMP::Instance().PostMSG( SEM_Z80ID_Z80B, hostMSG );
*/
	return blRet;
}

const CString& CSemCtrl_BseBySeDetectorSwitch::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// Format Display string
	if ( lCtlIndex != 0 )
		m_strText = m_pLanguageMan->GetResString( IDS_STATE_OPEN );
	else
		m_strText = m_pLanguageMan->GetResString( IDS_STATE_CLOSE );

	return m_strText;
}

const CString& CSemCtrl_BseBySeDetectorSwitch::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	if ( lCtlIndex != 0 )
		strText = m_pLanguageMan->GetResString( IDS_STATE_OPEN );
	else
		strText = m_pLanguageMan->GetResString( IDS_STATE_CLOSE );

	return strText;
}
