// SemCtrl_Signal2Contrast.cpp: implementation of the CSemCtrl_Signal2Contrast class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_Signal2Contrast.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_Signal2Contrast::CSemCtrl_Signal2Contrast()
{
	m_scfCtrl = DefaultCtrlFigures_Signal2Contrast;
}

CSemCtrl_Signal2Contrast::~CSemCtrl_Signal2Contrast()
{
}

BOOL CSemCtrl_Signal2Contrast::Flush()
{
	WORD				wCtl;
	SEM_ControlMessage	hostMSG;
	USB_SEM_ControlMessage	usb_hostMSG;

	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	wCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);

	hostMSG.bMessageID		= (BYTE)Z80B_Signal2Contrast;
	hostMSG.bCtlMSGLength	= (BYTE)2;
	hostMSG.bData[1]		= (BYTE)wCtl;

	usb_hostMSG.bMessageID		= (BYTE)Z80B_Signal2Contrast;
	usb_hostMSG.bCtlMSGLength	= (BYTE)2;
	usb_hostMSG.bData[1]		= (BYTE)wCtl;

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
	
	return blRet;
}

const CString& CSemCtrl_Signal2Contrast::FormatText()
{
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// Format Display string
	m_strText.Format( "%03.1f", 
		(double)(lCtlIndex - m_scfCtrl.varMin.lVal) * 100.0 / (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal) );

	return m_strText;
}

const CString& CSemCtrl_Signal2Contrast::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%03.1f", 
		(double)(lCtlIndex - m_scfCtrl.varMin.lVal) * 100.0 / (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal) );

	return strText;
}
