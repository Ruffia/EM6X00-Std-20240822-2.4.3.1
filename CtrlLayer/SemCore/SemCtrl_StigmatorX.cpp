// SemCtrl_StigmatorX.cpp: implementation of the CSemCtrl_StigmatorX class.
//
// Last Modify : 2006.08.04
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_StigmatorX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_StigmatorX::CSemCtrl_StigmatorX()
{
	m_scfCtrl = DefaultCtrlFigures_StigmatorX;

	m_dCaliOffset = 0.0;
	CSemBasisCtrl::ReadINI( INI_CaliOffset, INIIndex_StigX, &m_dCaliOffset );
}

CSemCtrl_StigmatorX::~CSemCtrl_StigmatorX()
{

}

BOOL CSemCtrl_StigmatorX::Flush()
{
	if ( !CSemBasisCtrl::Flush() )
		return FALSE;

	WORD				wCtl, wusbCtl;
	USB_SEM_ControlMessage	usb_hostMSG;

	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	//======2006.08.04_��������ɢ========================
	// �޸ı�ʾ��2006.08.04_��������ɢ
	// �޸����ڣ�2006��8��4��������
	// �޸��ˣ�J.Y.Ma
	// �޸�ԭ��
	//		�ڴ򿪸�������ɢ���غ�
	//		ͼ������ʾ��ʮ�ֲ��X���������������ɢ��X����ָʾ�����෴
	// �����޸�ǰ���븱��
	//----------------------------------------------------- 
	wCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);
	//-----------------------------------------------------*/
	// �����޸ĺ����
	/*-----------------------------------------------------
//	wCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);
	wCtl = (WORD)(m_scfCtrl.varMax.lVal - lCtlIndex);
//	wCtl = (WORD)(abs(lCtlIndex - m_scfCtrl.varMax.lVal));
	//-----------------------------------------------------
	*///======2006.08.04_��������ɢ========================

//	wusbCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);
//	wusbCtl = (WORD)((lCtlIndex +m_dCaliOffset - m_scfCtrl.varMin.lVal)*16);	// -128 - 127 ���� 0 - 4095
	wusbCtl = (WORD)(lCtlIndex +m_dCaliOffset - m_scfCtrl.varMin.lVal);
//	usb_hostMSG.bMessageID		= (BYTE)USB_CID_SCN_Stigmation;
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SCN_Stigmation;	// Serial Comm operand low byte (X/Y Stig DAC circuit address)
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_SCN_Write_16;	// Serial Comm operand high byte (16 bit write to SCN board)
	usb_hostMSG.bData[2]		= (BYTE)( wusbCtl & 0x00FF );
	usb_hostMSG.bData[3]		= (BYTE)( wusbCtl >> 8 );

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}

const CString& CSemCtrl_StigmatorX::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	WORD				wusbCtl;
//	wusbCtl = (WORD)((lCtlIndex +m_dCaliOffset - m_scfCtrl.varMin.lVal)*16);	// -128 - 127 ���� 0 - 4095
	wusbCtl = (WORD)(lCtlIndex +m_dCaliOffset - m_scfCtrl.varMin.lVal);
	// Format Display string
	if( m_nOpr > 0 )
		m_strText.Format( "%+05d     [ %04X ]", lCtlIndex, wusbCtl );
	else
		m_strText.Format( "%+05d", lCtlIndex );

	return m_strText;
}

const CString& CSemCtrl_StigmatorX::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%+05d", lCtlIndex );

	return strText;
}

void CSemCtrl_StigmatorX::SetParams( int nType, double dParam )// Type:���ͣ�Param:Ҫ���õĲ���ֵ
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
