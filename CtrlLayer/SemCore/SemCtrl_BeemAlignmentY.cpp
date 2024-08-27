// SemCtrl_BeemAlignmentY.cpp: implementation of the CSemCtrl_BeemAlignmentY class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_BeemAlignmentY.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_BeemAlignmentY::CSemCtrl_BeemAlignmentY()
{
	m_scfCtrl = DefaultCtrlFigures_BeemAlignmentY;

	m_dCaliOffset = 0.0;
	CSemBasisCtrl::ReadINI( INI_CaliOffset, INIIndex_AlignY, &m_dCaliOffset );
}

CSemCtrl_BeemAlignmentY::~CSemCtrl_BeemAlignmentY()
{

}

BOOL CSemCtrl_BeemAlignmentY::Flush()
{
	if ( !CSemBasisCtrl::Flush() )
		return FALSE;

	WORD				wusbCtl;
	USB_SEM_ControlMessage	usb_hostMSG;

	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

//	wusbCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);
//	wusbCtl = (WORD)((lCtlIndex +m_dCaliOffset- m_scfCtrl.varMin.lVal)*256);	// -128 - 127 扩大到 0 - 65535
	wusbCtl = (WORD)((lCtlIndex +m_dCaliOffset- m_scfCtrl.varMin.lVal)*2);
//	usb_hostMSG.bMessageID		= (BYTE)USB_CID_LNS_YAlignment;
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_YAlignment;	// Serial Comm operand low byte (X/Y Stig DAC circuit address)
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_16;	// Serial Comm operand high byte (16 bit write to SCN board)
	usb_hostMSG.bData[2]		= (BYTE)( wusbCtl & 0x00FF );
	usb_hostMSG.bData[3]		= (BYTE)( wusbCtl >> 8 );

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}

const CString& CSemCtrl_BeemAlignmentY::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	WORD				wusbCtl;
//	wusbCtl = (WORD)((lCtlIndex +m_dCaliOffset- m_scfCtrl.varMin.lVal)*256);	// -128 - 127 扩大到 0 - 65535
	wusbCtl = (WORD)((lCtlIndex +m_dCaliOffset- m_scfCtrl.varMin.lVal)*2);
	// Format Display string
	if( m_nOpr > 0 )
		m_strText.Format( "%+06d     [ %04X ]", lCtlIndex, wusbCtl );
	else
		m_strText.Format( "%+06d", lCtlIndex );

	return m_strText;
}

const CString& CSemCtrl_BeemAlignmentY::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%+06d", lCtlIndex );

	return strText;
}

void CSemCtrl_BeemAlignmentY::SetParams( int nType, double dParam )// Type:类型；Param:要设置的参数值
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
