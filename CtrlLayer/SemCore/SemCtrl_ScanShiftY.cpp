// SemCtrl_ScanShiftY.cpp: implementation of the CSemCtrl_ScanShiftY class.
//
// Last Modify : 2007.12.11
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_ScanShiftY.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_ScanShiftY::CSemCtrl_ScanShiftY()
{
	m_scfCtrl = DefaultCtrlFigures_ScanShiftY;

	m_dCaliOffset = 0.0;
	CSemBasisCtrl::ReadINI( INI_CaliOffset, INIIndex_ShiftY, &m_dCaliOffset );
}

CSemCtrl_ScanShiftY::~CSemCtrl_ScanShiftY()
{

}

BOOL CSemCtrl_ScanShiftY::Flush()
{
	if ( !CSemBasisCtrl::Flush() )
		return FALSE;

	WORD				wCtl, wusbCtl;
	SEM_ControlMessage	hostMSG;
	USB_SEM_ControlMessage	usb_hostMSG;

	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	//======2007.12.11_��λ��========================
	// �޸ı�ʾ��2007.12.11_��λ��
	// �޸����ڣ�2007��12��11��
	// �޸��ˣ�J.Y.Ma
	// �޸�ԭ��
	//		ͼ������Ļ����X����������е�λ�Ƶ�X����ָʾ�����෴
	// �����޸�ǰ���븱��
	//----------------------------------------------------- 
	wCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);
//	-----------------------------------------------------*/
	// �����޸ĺ����
	//-----------------------------------------------------
//	wCtl = (WORD)(abs(lCtlIndex - m_scfCtrl.varMax.lVal));
	//-----------------------------------------------------
	//======2007.12.11_��λ��========================

	hostMSG.bMessageID		= (BYTE)Z80B_EMoveY;
	hostMSG.bCtlMSGLength	= (BYTE)2;
	hostMSG.bData[1]		= (BYTE)wCtl;

//	wusbCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);
//	wusbCtl = (WORD)((lCtlIndex +m_dCaliOffset - m_scfCtrl.varMin.lVal)*16);	// -128 - 127 ���� 0 - 4095
//	wusbCtl = (WORD)((m_scfCtrl.varMax.lVal - lCtlIndex -m_dCaliOffset)*16);	// -128 - 127 ���� 4095 - 0
//	wusbCtl = (WORD)(m_scfCtrl.varMax.lVal - lCtlIndex -m_dCaliOffset);			// -2048 - 2047 ��Ӧ 4095 - 0
	wusbCtl = (WORD)(m_scfCtrl.varMax.lVal - lCtlIndex -m_dCaliOffset);			// -2047 - 2048 ��Ӧ 4095 - 0
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SCN_Yshift_Crossover;	// Serial Comm operand low byte (X/Y Stig DAC circuit address)
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_SCN_Write_16;			// Serial Comm operand high byte (16 bit write to SCN board)
	usb_hostMSG.bData[2]		= (BYTE)( wusbCtl & 0x00FF );
	usb_hostMSG.bData[3]		= (BYTE)( wusbCtl >> 8 );

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}

const CString& CSemCtrl_ScanShiftY::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	WORD				wusbCtl;
//	wusbCtl = (WORD)((lCtlIndex +m_dCaliOffset - m_scfCtrl.varMin.lVal)*16);	// -128 - 127 ���� 0 - 4095
//	wusbCtl = (WORD)((m_scfCtrl.varMax.lVal - lCtlIndex -m_dCaliOffset)*16);	// -128 - 127 ���� 4095 - 0
//	wusbCtl = (WORD)(m_scfCtrl.varMax.lVal - lCtlIndex -m_dCaliOffset);			// -2048 - 2047 ��Ӧ 4095 - 0
	wusbCtl = (WORD)(m_scfCtrl.varMax.lVal - lCtlIndex -m_dCaliOffset);			// -2047 - 2048 ��Ӧ 4095 - 0
	// Format Display string
	if( m_nOpr > 0 )
		m_strText.Format( "%+05d     [ %04X ]", lCtlIndex, wusbCtl );
	else
		m_strText.Format( "%+05d", lCtlIndex );

	return m_strText;
}

const CString& CSemCtrl_ScanShiftY::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%+05d", lCtlIndex );

	return strText;
}

void CSemCtrl_ScanShiftY::SetParams( int nType, double dParam )// Type:���ͣ�Param:Ҫ���õĲ���ֵ
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
