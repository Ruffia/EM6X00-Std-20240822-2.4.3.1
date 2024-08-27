// SemCtrl_YAdjust.cpp: implementation of the CSemCtrl_YAdjust class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_YAdjust.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_YAdjust::CSemCtrl_YAdjust()
{
	m_scfCtrl = DefaultCtrlFigures_YAdjust;
}

CSemCtrl_YAdjust::~CSemCtrl_YAdjust()
{
}

BOOL CSemCtrl_YAdjust::Flush()
{
	WORD				wCtl;
	SEM_ControlMessage	hostMSG;
	USB_SEM_ControlMessage	usb_hostMSG;

	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	wCtl = (WORD)(m_scfCtrl.varMax.lVal - lCtlIndex);

	hostMSG.bMessageID		= (BYTE)Z80A_YAdjust;
	hostMSG.bCtlMSGLength	= (BYTE)2;
	hostMSG.bData[1]		= (BYTE)wCtl;

	usb_hostMSG.bMessageID		= (BYTE)Z80A_YAdjust;
	usb_hostMSG.bCtlMSGLength	= (BYTE)2;
	usb_hostMSG.bData[1]		= (BYTE)wCtl;

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
	
	return blRet;
}

const CString& CSemCtrl_YAdjust::FormatText()
{
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// Format Display string
	m_strText.Format( "%03.1f", 
		(double)(lCtlIndex - m_scfCtrl.varMin.lVal) * 100.0 / (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal) );

	return m_strText;
}

const CString& CSemCtrl_YAdjust::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%03.1f", 
		(double)(lCtlIndex - m_scfCtrl.varMin.lVal) * 100.0 / (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal) );

	return strText;
}
