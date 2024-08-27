// SemCtrl_CondenserLens.cpp: implementation of the CSemCtrl_CondenserLens class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_CondenserLens.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_CondenserLens::CSemCtrl_CondenserLens()
{	
	m_scfCtrl	= DefaultCtrlFigures_CondenserLens;
	m_dRateC	= 0.0;
	m_bReverse	= FALSE;
}

CSemCtrl_CondenserLens::~CSemCtrl_CondenserLens()
{

}

BOOL CSemCtrl_CondenserLens::Flush()
{
	if ( !CSemBasisCtrl::Flush() )
		return FALSE;

	WORD				wusbCtl;
	USB_SEM_ControlMessage	usb_hostMSG;

	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// ��ϵͳ�и�ѹͳ��Ҫ����������
	// ��ѹ�����ı�ʱ���۹⾵�������ﾵ������ɨ����Ȧ������Ҫ�����仯
	// ԭ���͵�Z80�ĸ�ѹͳ��ֵҪ��Ϊϵ����������3��ֵ
	// 30KVʱ��ϵ��Ϊ1
//	wusbCtl = (WORD)(lCtlIndex * m_dRateC);// - m_scfCtrl.varMin.lVal);
	if( m_bReverse )
	{
		// 0 - 65535 ӳ�䵽 32768 - 0
//		wusbCtl = (WORD)( (m_scfCtrl.varMax.lVal +1 -lCtlIndex) /2 * m_dRateC);
		// 0 - 32767 ӳ�䵽 32768 - 0
		wusbCtl = (WORD)( 32768 -lCtlIndex *m_dRateC );//( (32768 -lCtlIndex) * m_dRateC);
	}
	else
	{
		// 0 - 65535 ӳ�䵽 32768 - 65535
//		wusbCtl = (WORD)( (m_scfCtrl.varMax.lVal +1 +lCtlIndex) /2 * m_dRateC);
		// 0 - 32767 ӳ�䵽 32768 - 65535
		wusbCtl = (WORD)( 32768 +lCtlIndex *m_dRateC );//( (lCtlIndex +32768) * m_dRateC);
	}
//	usb_hostMSG.bMessageID		= (BYTE)USB_CID_LNS_CondenserLens;
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_CondenserLens;	// Serial Comm operand low byte (X/Y Stig DAC circuit address)
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_16;		// Serial Comm operand high byte (16 bit write to SCN board)
	usb_hostMSG.bData[2]		= (BYTE)( wusbCtl & 0x00FF );
	usb_hostMSG.bData[3]		= (BYTE)( wusbCtl >> 8 );

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}

const CString& CSemCtrl_CondenserLens::FormatText()
{
	WORD				wusbCtl;
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;
//	const double dfToDisplay = (double)1.0/64.0;	//	const double dfToDisplay = (double)1000/65535;
	const double dfToDisplay = (double)1.0/32.0;

	if( m_bReverse )
	{
		// 0 - 65535 ӳ�䵽 32768 - 0
//		wusbCtl = (WORD)( (m_scfCtrl.varMax.lVal +1 -lCtlIndex) /2 * m_dRateC);
		// 0 - 32767 ӳ�䵽 32768 - 0
		wusbCtl = (WORD)( 32768 -lCtlIndex *m_dRateC );//( (32768 -lCtlIndex) * m_dRateC);
	}
	else
	{
		// 0 - 65535 ӳ�䵽 32768 - 65535
//		wusbCtl = (WORD)( (m_scfCtrl.varMax.lVal +1 +lCtlIndex) /2 * m_dRateC);
		// 0 - 32767 ӳ�䵽32768 - 65535
		wusbCtl = (WORD)( 32768 +lCtlIndex *m_dRateC );//( (lCtlIndex +32768) * m_dRateC);
	}

	// Format Display string
	if( m_nOpr > 0 )
	{
		//m_strText.Format( "%4.2f CTL:%4XH", lCtlIndex * dfToDisplay, lCtlIndex );
		m_strText.Format( "%4.2f [ %04X ]", lCtlIndex * dfToDisplay, wusbCtl );
	}
	else
	{
		//m_strText.Format( "%4.2f CTL:%4XH", lCtlIndex * dfToDisplay, lCtlIndex );
		m_strText.Format( "%4.2f", lCtlIndex * dfToDisplay );
	}

	return m_strText;
}


long CSemCtrl_CondenserLens::GetPosByValue(const double dValue)
{
	long lPos = 0;
	const double dfToDisplay = (double)1.0/32.0;
	if( m_nOpr > 0 )
	{
		lPos = dValue/dfToDisplay;
	}
	else
	{
        lPos = dValue/dfToDisplay;
	}

	return lPos;
}

const CString& CSemCtrl_CondenserLens::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;
	const double dfToDisplay = (double)1.0/64.0;
	//const double dfToDisplay = (double)1000/65535;

	// Format Display string
	if( (lCtlIndex * dfToDisplay) >= 1000.0 )
	{
		strText.Format( "%4.2f", lCtlIndex * dfToDisplay );
	}
	else
	{
		strText.Format( "%4.2f", lCtlIndex * dfToDisplay );
	}

	return strText;
}

/*
const int SET_PARAMS_DebugMode			= 1;	// ���õ���ģʽ
const int SET_PARAMS_Reverse			= 2;	// ���÷�ת
const int SET_PARAMS_DetPair			= 3;	// ����̽������
const int SET_PARAMS_Rate				= 4;	// ����ͳ������ϵ��
const int SET_PARAMS_Det2nd				= 5;	// ����̽�������еĵڶ�ͨ��
*/

void CSemCtrl_CondenserLens::SetParams( int nType, double dParam )// Type:���ͣ�Param:Ҫ���õĲ���ֵ
{
	switch( nType )
	{
	case	SET_PARAMS_Reverse:
		m_bReverse = (BOOL)dParam;
//		Flush();
		break;

	case	SET_PARAMS_Rate:
// 		if( dParam < 1 )
// 			m_dRateC = 1.0;
// 		else
			m_dRateC = sqrt( dParam /300.0 );
		Flush();
		break;

	case	100:	// OprAdvance
		m_nOpr	= (int)dParam;
		break;

	default:
		break;
	}
}

