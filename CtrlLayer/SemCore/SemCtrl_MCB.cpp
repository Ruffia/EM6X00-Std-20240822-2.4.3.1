// SemCtrl_MCB.cpp: implementation of the CSemCtrl_MCB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_MCB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_MCB::CSemCtrl_MCB()
{
	m_oseParam.bFunction		= OSE_Fn_ObjectiveLens;
	m_oseParam.bStep			= 1;
	m_oseParam.bNotifyTimer	= 50;
	m_oseParam.bPulseDivier	= 0;
	m_oseParam.nCtrlMin		= 0;
	m_oseParam.nCtrlMax		= 255;

	m_scfCtrl = DefaultCtrlFigures_OSE;
	m_scfCtrl.varPos.lVal  = m_oseParam.bFunction;
}

CSemCtrl_MCB::~CSemCtrl_MCB()
{
}

BOOL CSemCtrl_MCB::Flush()
{
	WORD				wCtl;
	SEM_ControlMessage	hostMSG;
	USB_SEM_ControlMessage	usb_hostMSG;

	hostMSG.bCtlMSGLength	= (BYTE)9;
	hostMSG.bMessageID		= (BYTE)USB_OSE_SetupParam;
	hostMSG.bData[1]		= (BYTE)m_oseParam.bFunction;
	hostMSG.bData[2]		= (BYTE)m_oseParam.bStep;
	hostMSG.bData[3]		= (BYTE)m_oseParam.bNotifyTimer;
	hostMSG.bData[4]		= (BYTE)m_oseParam.bPulseDivier;
					   wCtl = (WORD)m_oseParam.nCtrlMin;
	hostMSG.bData[5]		= (BYTE)( wCtl & 0x00FF );
	hostMSG.bData[6]		= (BYTE)( wCtl >> 8 );
					   wCtl = (WORD)m_oseParam.nCtrlMax;
	hostMSG.bData[7]		= (BYTE)( wCtl & 0x00FF );
	hostMSG.bData[8]		= (BYTE)( wCtl >> 8 );

	usb_hostMSG.bCtlMSGLength	= (BYTE)9;
	usb_hostMSG.bMessageID		= (BYTE)USB_OSE_SetupParam;
	usb_hostMSG.bData[1]		= (BYTE)m_oseParam.bFunction;
	usb_hostMSG.bData[2]		= (BYTE)m_oseParam.bStep;
	usb_hostMSG.bData[3]		= (BYTE)m_oseParam.bNotifyTimer;
	usb_hostMSG.bData[4]		= (BYTE)m_oseParam.bPulseDivier;
					   wCtl = (WORD)m_oseParam.nCtrlMin;
	usb_hostMSG.bData[5]		= (BYTE)( wCtl & 0x00FF );
	usb_hostMSG.bData[6]		= (BYTE)( wCtl >> 8 );
					   wCtl = (WORD)m_oseParam.nCtrlMax;
	usb_hostMSG.bData[7]		= (BYTE)( wCtl & 0x00FF );
	usb_hostMSG.bData[8]		= (BYTE)( wCtl >> 8 );

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}

const CString& CSemCtrl_MCB::FormatText()
{
	m_strText = FormatText( GetPos() );

	return m_strText;
}

const CString& CSemCtrl_MCB::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	switch(lCtlIndex)
	{
	case OSE_Fn_ObjectiveLens:
		strText = _T("�ﾵ");
		break;

	case OSE_Fn_CondenserLens:
		strText = _T("�۹⾵");
		break;

	case OSE_Fn_Brightness:
		strText = _T("����");
		break;

	case OSE_Fn_Contrast:
		strText = _T("�Աȶ�");
		break;

	case OSE_Fn_ScanShiftX:
		strText = _T("��λ��X");
		break;
	
	case OSE_Fn_ScanShiftY:
		strText = _T("��λ��Y");
		break;

	case OSE_Fn_StigmatorX:
		strText = _T("����ɢX");
		break;

	case OSE_Fn_StigmatorY:
		strText = _T("����ɢY");
		break;

	case OSE_Fn_BeamAlignmentX:
		strText = _T("�����X");
		break;

	case OSE_Fn_BeamAlignmentY:
		strText = _T("�����Y");
		break;

	default:
		strText = _T("��ť����");
		break;
	}

	return strText;
}

VARIANT CSemCtrl_MCB::SetUp( const OSEncoderParam& oseParam )
{	
	m_oseParam = oseParam; 
	m_scfCtrl.varPos.lVal  = m_oseParam.bFunction;
	
	return GetPos();
}
