// SemCtrl_Brightness.cpp: implementation of the CSemCtrl_Brightness class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_Brightness.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_Brightness::CSemCtrl_Brightness()
{
	m_scfCtrl = DefaultCtrlFigures_Brightness;
}

CSemCtrl_Brightness::~CSemCtrl_Brightness()
{

}

BOOL CSemCtrl_Brightness::Flush()
{
	if ( !CSemBasisCtrl::Flush() )
		return FALSE;

	WORD				wusbCtl;
	USB_SEM_ControlMessage	usb_hostMSG;

	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

//	wusbCtl = (WORD)( -m_scfCtrl.varMin.lVal - lCtlIndex -1 );	// -2048 - +2047 对应到 4095 - 0
	wusbCtl = (WORD)( m_scfCtrl.varMax.lVal - lCtlIndex );		// -2047 - +2048 对应到 4095 - 0

	// -1023 - +1024 对应到 (4095-255)[0xEFF] - (2047-255)[0x700]，即中心为0xB00(2816)
//	wusbCtl = (WORD)( m_scfCtrl.varMax.lVal - lCtlIndex + (2047 -255) );

	// -511 - +512 对应到 (3071-127)[0xB7F] - (2047-127)[0x780]，即中心为0x980(2432)
//	wusbCtl = (WORD)( m_scfCtrl.varMax.lVal - lCtlIndex + (2047 -127) );
   ///////////////////////////////////////////////////////////////////////////

	usb_hostMSG.bType			= (BYTE)0;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;

	if( m_nDetPair == 2 )
		usb_hostMSG.bData[0]	= (BYTE)USB_Det45StoreDAC;
	else
		usb_hostMSG.bData[0]	= (BYTE)USB_Det03StoreDAC;

	usb_hostMSG.bData[1]		= (BYTE)TRUE;
	usb_hostMSG.bData[2]		= (BYTE)( wusbCtl & 0x00FF );	// low

	int nChn = 2 * m_nDetPair;
	if( m_nDet2nd > 0 )
		nChn += 1;
	if( nChn > 3 )
		nChn -= 4;
	usb_hostMSG.bData[3]		= (BYTE)( (wusbCtl >> 8) | (nChn << 6) );

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}

const CString& CSemCtrl_Brightness::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	WORD				wusbCtl;
// 	wusbCtl = (WORD)( -m_scfCtrl.varMin.lVal - lCtlIndex -1 );	// -2048 - +2047 对应到 4095 - 0
 	wusbCtl = (WORD)( m_scfCtrl.varMax.lVal - lCtlIndex );		// -2047 - +2048 对应到 4095 - 0

	// -1023 - +1024 对应到 (4095-255)[0xEFF] - (2047-255)[0x700]，即中心为0xB00(2816)
//	wusbCtl = (WORD)( m_scfCtrl.varMax.lVal - lCtlIndex + (2047 -255) );
	
	// -511 - +512 对应到 (3071-127)[0xB7F] - (2047-127)[0x780]，即中心为0x980(2432)
//	wusbCtl = (WORD)( m_scfCtrl.varMax.lVal - lCtlIndex + (2047 -127) );

	// Format Display string
	if ( m_nOpr > 0 )
	{
		m_strText.Format( "%+02.2f [ %03X ]", 
			(double)lCtlIndex * 100.0 / abs(m_scfCtrl.varMin.lVal), wusbCtl );
	}
	else
	{
		m_strText.Format( "%+02.2f", 
			(double)lCtlIndex * 100.0 / abs(m_scfCtrl.varMax.lVal) );
	}

	return m_strText;
}

const CString& CSemCtrl_Brightness::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	if ( lCtlIndex < 0 )
	{
		strText.Format( "%+02.2f", 
			(double)lCtlIndex * 100.0 / abs(m_scfCtrl.varMin.lVal) );
	}
	else
	{
		strText.Format( "%+02.2f", 
			(double)lCtlIndex * 100.0 / abs(m_scfCtrl.varMax.lVal) );
	}

	return strText;
}

/*
const int SET_PARAMS_DebugMode			= 1;	// 设置调试模式
const int SET_PARAMS_Reverse			= 2;	// 设置反转
const int SET_PARAMS_DetPair			= 3;	// 设置探测器组
const int SET_PARAMS_Rate				= 4;	// 设置统调比例系数
const int SET_PARAMS_Det2nd				= 5;	// 设置探测器组中的第二通道
*/
void CSemCtrl_Brightness::SetParams( int nType, double dParam )// Type:类型；Param:要设置的参数值
{
	switch( nType )
	{
	case	SET_PARAMS_DetPair:
		m_nDetPair	= (int)dParam;
//		Flush();
		break;

	case	SET_PARAMS_Det2nd:
		m_nDet2nd	= (int)dParam;
//		Flush();
		break;

	case	100:	// OprAdvance
		m_nOpr	= (int)dParam;
		break;

	default:
		break;
	}
}

long CSemCtrl_Brightness::GetPosByValue(const double dValue)
{
	long lPos = 0;
	if ( m_nOpr > 0 )
	{
		lPos = Round(abs(m_scfCtrl.varMin.lVal) * dValue /100.0 , 0);
	}
	else
	{
		lPos = Round(abs(m_scfCtrl.varMax.lVal) * dValue /100.0 , 0);
	}

	return lPos;
}