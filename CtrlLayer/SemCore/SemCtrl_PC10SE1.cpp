// SemCtrl_PC10SE1.cpp: implementation of the CSemCtrl_PC10SE1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_PC10SE1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_PC10SE1::CSemCtrl_PC10SE1()
{
	m_scfCtrl = DefaultCtrlFigures_PC10SE1;

}

CSemCtrl_PC10SE1::~CSemCtrl_PC10SE1()
{

}

BYTE CSemCtrl_PC10SE1::FormatControlFlag()
{
	WORD				wCtlCS;

	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	//wCtl = (WORD)(lCtlIndex - m_scfCtrl.varMin.lVal);
	switch ( lCtlIndex )
	{
	case 0:
		wCtlCS = BIT_1;
		break;
	
	case 1:
		wCtlCS = BIT_2;
		break;
	
	case 2:
		wCtlCS = BIT_3;
		break;
	
	case 3:
		wCtlCS = BIT_5;
		break;
	
	default:
		wCtlCS = BIT_1;
		break;
	}
	wCtlCS = ~wCtlCS; // 2004-5-12_控制指令为反逻辑

	return (BYTE)wCtlCS;
}

BOOL CSemCtrl_PC10SE1::Flush()
{
	WORD				wCtl;
	SEM_ControlMessage	hostMSG;
	USB_SEM_ControlMessage	usb_hostMSG;

	wCtl = FormatControlFlag();

	hostMSG.bMessageID		= (BYTE)Z80B_PC10_SE1;
	hostMSG.bCtlMSGLength	= (BYTE)2;
	hostMSG.bData[1]		= (BYTE)wCtl;

	usb_hostMSG.bMessageID		= (BYTE)Z80B_PC10_SE1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)2;
	usb_hostMSG.bData[1]		= (BYTE)wCtl;

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}

const CString& CSemCtrl_PC10SE1::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();

	return FormatText( varPos );
}

const CString& CSemCtrl_PC10SE1::FormatText( VARIANT varPos )
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
