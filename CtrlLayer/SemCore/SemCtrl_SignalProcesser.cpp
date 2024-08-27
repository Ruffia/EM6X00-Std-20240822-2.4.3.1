// SemCtrl_SignalProcesser.cpp: implementation of the CSemCtrl_SignalProcesser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_SignalProcesser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_SignalProcesser::CSemCtrl_SignalProcesser()
{
	m_scfCtrl = DefaultCtrlFigures_SignalProcesser;

	m_lSignalConverterState	= 0;
	m_nClickedButtonID		= SignalProcesser_NULL;
}

CSemCtrl_SignalProcesser::~CSemCtrl_SignalProcesser()
{

}

BOOL CSemCtrl_SignalProcesser::Flush()
{
	WORD				wCtl;
	SEM_ControlMessage	hostMSG;
	USB_SEM_ControlMessage	usb_hostMSG;

	wCtl = FormatControlFlag();

	hostMSG.bMessageID		= (BYTE)Z80B_SignalProcesser;
	hostMSG.bCtlMSGLength	= (BYTE)2;
	hostMSG.bData[1]		= (BYTE)wCtl;

	usb_hostMSG.bMessageID		= (BYTE)Z80B_SignalProcesser;
	usb_hostMSG.bCtlMSGLength	= (BYTE)2;
	usb_hostMSG.bData[1]		= (BYTE)wCtl;

	BOOL blRet = FALSE;
    blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}

const CString& CSemCtrl_SignalProcesser::FormatText()
{
	// Format Display string
	if ( m_lSignalConverterState != 0 )
		m_strText = m_pLanguageMan->GetResString( IDS_STATE_OPEN );
	else
		m_strText = m_pLanguageMan->GetResString( IDS_STATE_CLOSE );

	return m_strText;
}

const CString& CSemCtrl_SignalProcesser::FormatText( VARIANT varSwitch )
{
	static CString strText;

	// Format Display string
	if ( varSwitch.lVal != 0 )
		strText = m_pLanguageMan->GetResString( IDS_STATE_OPEN );
	else
		strText = m_pLanguageMan->GetResString( IDS_STATE_CLOSE );

	return strText;
}

BYTE CSemCtrl_SignalProcesser::FormatControlFlag()
{
	BYTE	bControlFlag = 0;						// Z80B(PC9) -> PC10M_SE4 ( B5 B6 B7 )

	if ( m_lSignalConverterState != 0 )
		bControlFlag |= (BYTE)0x40;

	switch ( m_nClickedButtonID )
	{
	case SignalProcesser_Power:
		bControlFlag |= BIT_0;
		break;
	
	case SignalProcesser_Right:
		bControlFlag |= BIT_1;
		break;
	
	case SignalProcesser_Left:
		bControlFlag |= BIT_2;
		break;
	
	case SignalProcesser_Down:
		bControlFlag |= BIT_3;
		break;
	
	case SignalProcesser_Up:
		bControlFlag |= BIT_4;
		break;
	
	default:
		break;
	}
	
	m_nClickedButtonID = SignalProcesser_NULL;		// Reset for next Click
	return bControlFlag;
}
