// SemCtrl_Contrast.cpp: implementation of the CSemCtrl_Contrast class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_Contrast.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_Contrast::CSemCtrl_Contrast()
{
	m_scfCtrl = DefaultCtrlFigures_Contrast;
}

CSemCtrl_Contrast::~CSemCtrl_Contrast()
{
}

BOOL CSemCtrl_Contrast::Flush()
{
	if ( !CSemBasisCtrl::Flush() )
		return FALSE;

	WORD				wusbCtl;
	USB_SEM_ControlMessage	usb_hostMSG;

	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

//	wusbCtl = (WORD)( -m_scfCtrl.varMin.lVal - lCtlIndex -1 );	// -32768 - +32767 对应到 65535 - 0
	wusbCtl = (WORD)((m_scfCtrl.varMax.lVal - lCtlIndex)*256);	// 0 - 255 对应到 65535 - 0
//	usb_hostMSG.bMessageID		= (BYTE)USB_CID_SPR_General_Purpose_DACs;
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)6;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SPR_General_Purpose_DACs;	// Command ID
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_SPR_Write_24;				// Mod ID
	usb_hostMSG.bData[2]		= (BYTE)( wusbCtl >> 8 );					// high 8bit
	usb_hostMSG.bData[3]		= (BYTE)0x10;								// DAC channel
	usb_hostMSG.bData[4]		= (BYTE)0x0;
	usb_hostMSG.bData[5]		= (BYTE)( wusbCtl & 0x00FF );				// low 8bit

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
	
	return blRet;
}

const CString& CSemCtrl_Contrast::FormatText()
{
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	WORD				wusbCtl;
//	wusbCtl = (WORD)( -m_scfCtrl.varMin.lVal - lCtlIndex -1 );	// -32768 - +32767 对应到 65535 - 0
	wusbCtl = (WORD)((m_scfCtrl.varMax.lVal - lCtlIndex)*256);	// 0 - 255 对应到 65535 - 0

	// Format Display string
	if( m_nOpr > 0 )
		m_strText.Format( "%03.3f [ %04X ]", 
			(double)(lCtlIndex - m_scfCtrl.varMin.lVal) * 100.0 / (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal), wusbCtl );
	else
		m_strText.Format( "%03.3f", 
			(double)(lCtlIndex - m_scfCtrl.varMin.lVal) * 100.0 / (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal) );

	return m_strText;
}

const CString& CSemCtrl_Contrast::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%03.2f", 
		(double)(lCtlIndex - m_scfCtrl.varMin.lVal) * 100.0 / (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal) );

	return strText;
}

void CSemCtrl_Contrast::SetParams( int nType, double dParam )// Type:类型；Param:要设置的参数值
{
	switch( nType )
	{
	case	100:	// OprAdvance
		m_nOpr	= (int)dParam;
		break;

	default:
		break;
	}
}


long CSemCtrl_Contrast::GetPosByValue(const double dValue)
{
	long lPos = 0;
	if ( m_nOpr > 0 )
	{
		lPos = (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal) * dValue /100.0;
	}
	else
	{
		lPos = (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal) * dValue /100.0;
	}

	return lPos;
}