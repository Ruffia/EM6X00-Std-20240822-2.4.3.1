// SemCtrl_StigmatorY.cpp: implementation of the CSemCtrl_StigmatorY class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_StigmatorY.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_StigmatorY::CSemCtrl_StigmatorY()
{
	m_scfCtrl = DefaultCtrlFigures_StigmatorY;

	m_dCaliOffset = 0.0;
	CSemBasisCtrl::ReadINI( INI_CaliOffset, INIIndex_StigY, &m_dCaliOffset );
}

CSemCtrl_StigmatorY::~CSemCtrl_StigmatorY()
{

}

BOOL CSemCtrl_StigmatorY::Flush()
{
	if ( !CSemBasisCtrl::Flush() )
		return FALSE;

	WORD				wusbCtl;
	USB_SEM_ControlMessage	usb_hostMSG;

	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

//	wusbCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);
//	wusbCtl = (WORD)((lCtlIndex +m_dCaliOffset - m_scfCtrl.varMin.lVal)*16);	// -128 - 127 扩大到 0 - 4095
	wusbCtl = (WORD)(lCtlIndex +m_dCaliOffset - m_scfCtrl.varMin.lVal);
//	usb_hostMSG.bMessageID		= (BYTE)USB_CID_SCN_Stigmation;
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SCN_Stigmation;	// Serial Comm operand low byte (X/Y Stig DAC circuit address)
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_SCN_Write_16;	// Serial Comm operand high byte (16 bit write to SCN board)
	usb_hostMSG.bData[2]		= (BYTE)( wusbCtl & 0x00FF );
	usb_hostMSG.bData[3]		= (BYTE)( (wusbCtl >> 8 ) | 0x80 );

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}

const CString& CSemCtrl_StigmatorY::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	WORD				wusbCtl;
//	wusbCtl = (WORD)((lCtlIndex +m_dCaliOffset - m_scfCtrl.varMin.lVal)*16);	// -128 - 127 扩大到 0 - 4095
	wusbCtl = (WORD)(lCtlIndex +m_dCaliOffset - m_scfCtrl.varMin.lVal);
	// Format Display string
	if( m_nOpr > 0 )
		m_strText.Format( "%+05d     [ %04X ]", lCtlIndex, wusbCtl );
	else
		m_strText.Format( "%+05d", lCtlIndex );

	return m_strText;
}

const CString& CSemCtrl_StigmatorY::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%+05d", lCtlIndex );

	return strText;
}

void CSemCtrl_StigmatorY::SetParams( int nType, double dParam )// Type:类型；Param:要设置的参数值
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
