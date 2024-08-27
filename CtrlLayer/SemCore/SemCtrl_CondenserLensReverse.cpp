// SemCtrl_CondenserLensReverse.cpp: implementation of the CSemCtrl_CondenserLensReverse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_CondenserLensReverse.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_CondenserLensReverse::CSemCtrl_CondenserLensReverse()
{
	m_scfCtrl = DefaultCtrlFigures_CondenserLensReverse;

}

CSemCtrl_CondenserLensReverse::~CSemCtrl_CondenserLensReverse()
{

}

BOOL CSemCtrl_CondenserLensReverse::Flush()
{
	WORD				wCtl;
	SEM_ControlMessage	hostMSG;
	USB_SEM_ControlMessage	usb_hostMSG;

	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	wCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);

	hostMSG.bMessageID		= (BYTE)Z80B_CondenserLensReverse;
	hostMSG.bCtlMSGLength	= (BYTE)2;
	hostMSG.bData[1]		= (BYTE)wCtl;

	usb_hostMSG.bMessageID		= (BYTE)Z80B_CondenserLensReverse;
	usb_hostMSG.bCtlMSGLength	= (BYTE)2;
	usb_hostMSG.bData[1]		= (BYTE)wCtl;

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}

const CString& CSemCtrl_CondenserLensReverse::FormatText()
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

const CString& CSemCtrl_CondenserLensReverse::FormatText( VARIANT varPos )
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
