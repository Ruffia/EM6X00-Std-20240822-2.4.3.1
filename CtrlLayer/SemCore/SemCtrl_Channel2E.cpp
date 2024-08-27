// SemCtrl_Channel2E.cpp : implementation of the CSemCtrl_Channel2E class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_Channel2E.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_Channel2E::CSemCtrl_Channel2E()
{
	m_scfCtrl = DefaultCtrlFigures_Channel2E;
}

CSemCtrl_Channel2E::~CSemCtrl_Channel2E()
{
}

BOOL CSemCtrl_Channel2E::Flush()
{
	WORD				wCtl;
	SEM_ControlMessage	hostMSG;

	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	wCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);

	hostMSG.bMessageID		= (BYTE)Z80B_Channel_E;
	hostMSG.bCtlMSGLength	= (BYTE)2;
	hostMSG.bData[1]		= (BYTE)wCtl;
	
	BOOL blRet = FALSE;
/*	if ( m_pMPComm != NULL )
		blRet = CMP::Instance().PostMSG( SEM_Z80ID_Z80B, hostMSG );
*/
	return blRet;
}

const CString& CSemCtrl_Channel2E::FormatText()
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

const CString& CSemCtrl_Channel2E::FormatText( VARIANT varPos )
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
