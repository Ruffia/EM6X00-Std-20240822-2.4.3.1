// SemCtrl_RobinsonDetectorSwitch.cpp: implementation of the CSemCtrl_RobinsonDetectorSwitch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_RobinsonDetectorSwitch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_RobinsonDetectorSwitch::CSemCtrl_RobinsonDetectorSwitch()
{
	m_scfCtrl = DefaultCtrlFigures_RobinsonDetectorSwitch;
}

CSemCtrl_RobinsonDetectorSwitch::~CSemCtrl_RobinsonDetectorSwitch()
{
}

BOOL CSemCtrl_RobinsonDetectorSwitch::Flush()
{
	WORD				wCtl;
	SEM_ControlMessage	hostMSG;
	USB_SEM_ControlMessage	usb_hostMSG;

	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	wCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);

	hostMSG.bMessageID		= (BYTE)Z80B_RobinsonDetectorSwitch;
	hostMSG.bCtlMSGLength	= (BYTE)2;
	hostMSG.bData[1]		= (BYTE)wCtl;

	usb_hostMSG.bMessageID		= (BYTE)Z80B_RobinsonDetectorSwitch;
	usb_hostMSG.bCtlMSGLength	= (BYTE)2;
	usb_hostMSG.bData[1]		= (BYTE)wCtl;

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}

const CString& CSemCtrl_RobinsonDetectorSwitch::FormatText()
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

const CString& CSemCtrl_RobinsonDetectorSwitch::FormatText( VARIANT varPos )
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
