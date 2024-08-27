// SemCtrl_PC10SE6.cpp: implementation of the CSemCtrl_PC10SE6 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_PC10SE6.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_PC10SE6::CSemCtrl_PC10SE6()
{
	m_scfCtrl = DefaultCtrlFigures_PC10SE6;

	m_lSPState	= 0;
}

CSemCtrl_PC10SE6::~CSemCtrl_PC10SE6()
{

}

BYTE CSemCtrl_PC10SE6::FormatControlFlag()
{
	BYTE	bControlFlag = 0;
	BYTE	wCtlCS = 0;
	BYTE	wCtlSP = 0;

	// Setup CS ControlFlag
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;
	switch ( lCtlIndex )
	{
	case 0:
		wCtlCS = BIT_0;
		break;
	
	case 1:
		wCtlCS = BIT_1;
		break;
	
	case 2:
		wCtlCS = BIT_2;
		break;
	
	case 3:
		wCtlCS = BIT_3;
		break;
	
	default:
		wCtlCS = BIT_0;
		break;
	}
	//======2004-5-12_����ָ��Ϊ���߼�=====================
	// �޸ı�ʾ��2004-5-12_����ָ��Ϊ���߼�
	// �޸����ڣ�2004��5��12��������
	// �޸��ˣ�������
	// �޸�ԭ��2004��2��12�� �����ָ��
	//		����ָ��Ϊ"���߼�"����ԭ��������"���߼�"���� 

	// �����޸�ǰ���븱��
    /*----------------------------------------------------- 
	// Setup SP ControlFlag
	switch ( m_lSPState )
	{
	case SignalProcesser_Close:
		wCtlSP = 0;
		break;
	
	case SignalProcesser_LowPassFilter:
		wCtlSP = BIT_4 | BIT_6;
		break;
	
	case SignalProcesser_SignalEnhancement:
		wCtlSP = BIT_4;
		break;
	
	default:
		wCtlSP = 0;
		break;
	}

	bControlFlag = wCtlCS | wCtlSP;
	-----------------------------------------------------*/
	
	// �����޸ĺ����
	//-----------------------------------------------------
	// Setup SP ControlFlag
	switch ( m_lSPState )
	{
	case SignalProcesser_Close:
		wCtlSP = 0;
		break;
	
	case SignalProcesser_LowPassFilter:
		wCtlSP = BIT_4 | BIT_6;
		break;
	
	case SignalProcesser_SignalEnhancement:
		wCtlSP = BIT_4;
		break;
	
	default:
		wCtlSP = 0;
		break;
	}

	wCtlCS = (~wCtlCS) & 0x0F;
	bControlFlag = wCtlCS | wCtlSP;
	bControlFlag |= BIT_5 | BIT_7;
	//-----------------------------------------------------
	//======2004-5-12_����ָ��Ϊ���߼�=====================


	return bControlFlag;
}

BOOL CSemCtrl_PC10SE6::Flush()
{
	WORD				wCtl;
	SEM_ControlMessage	hostMSG;
	USB_SEM_ControlMessage	usb_hostMSG;

	wCtl = FormatControlFlag();

	hostMSG.bMessageID		= (BYTE)Z80B_PC10_SE6;
	hostMSG.bCtlMSGLength	= (BYTE)2;
	hostMSG.bData[1]		= (BYTE)wCtl;

	usb_hostMSG.bMessageID		= (BYTE)Z80B_PC10_SE6;
	usb_hostMSG.bCtlMSGLength	= (BYTE)2;
	usb_hostMSG.bData[1]		= (BYTE)wCtl;

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}

const CString& CSemCtrl_PC10SE6::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();

	return FormatText( varPos );
}

const CString& CSemCtrl_PC10SE6::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	switch ( lCtlIndex )
	{
	case 0:
		m_strText = _T("A/B");
		break;
	
	case 1:
		m_strText = _T("C");
		break;
	
	case 2:
		m_strText = _T("D");
		break;
	
	case 3:
		m_strText = _T("E");
		break;
	
	default:
		m_strText = _T("A/B");
		break;
	}

	return strText;
}
