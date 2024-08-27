// SemCtrl_Channel2C.cpp : implementation of the CSemCtrl_Channel2C class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_Channel2C.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_Channel2C::CSemCtrl_Channel2C()
{
	m_scfCtrl = DefaultCtrlFigures_Channel2C;
}

CSemCtrl_Channel2C::~CSemCtrl_Channel2C()
{
}

BOOL CSemCtrl_Channel2C::Flush()
{
	WORD				wCtl;
	SEM_ControlMessage	hostMSG;

	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	wCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);

	hostMSG.bMessageID		= (BYTE)Z80B_Channel_C;
	hostMSG.bCtlMSGLength	= (BYTE)2;
	hostMSG.bData[1]		= (BYTE)wCtl;
	
	BOOL blRet = FALSE;
/*	if ( m_pMPComm != NULL )
		blRet = CMP::Instance().PostMSG( SEM_Z80ID_Z80B, hostMSG );
*/
	return blRet;
}

const CString& CSemCtrl_Channel2C::FormatText()
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

const CString& CSemCtrl_Channel2C::FormatText( VARIANT varPos )
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

