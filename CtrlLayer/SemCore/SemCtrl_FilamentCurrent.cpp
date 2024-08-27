// SemCtrl_FilamentCurrent.cpp: implementation of the CSemCtrl_FilamentCurrent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_FilamentCurrent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_FilamentCurrent::CSemCtrl_FilamentCurrent()
{
	m_scfCtrl = DefaultCtrlFigures_FilamentCurrent;

}

CSemCtrl_FilamentCurrent::~CSemCtrl_FilamentCurrent()
{

}

BOOL CSemCtrl_FilamentCurrent::Flush()
{
	WORD				wCtl;
	SEM_ControlMessage	hostMSG;
	USB_SEM_ControlMessage	usb_hostMSG;

	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	wCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);

	hostMSG.bMessageID		= (BYTE)Z80B_FilamentI;
	hostMSG.bCtlMSGLength	= (BYTE)2;
	hostMSG.bData[1]		= (BYTE)wCtl;

	usb_hostMSG.bMessageID		= (BYTE)Z80B_FilamentI;
	usb_hostMSG.bCtlMSGLength	= (BYTE)2;
	usb_hostMSG.bData[1]		= (BYTE)wCtl;

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}

const CString& CSemCtrl_FilamentCurrent::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// Format Display string
	m_strText.Format( "%d uA", lCtlIndex * 15 );

	return m_strText;
}

const CString& CSemCtrl_FilamentCurrent::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%d uA", lCtlIndex * 15 );

	return strText;
}
