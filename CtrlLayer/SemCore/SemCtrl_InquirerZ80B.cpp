// SemCtrl_InquirerZ80B.cpp: implementation of the CSemCtrl_InquirerZ80B class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_InquirerZ80B.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_InquirerZ80B::CSemCtrl_InquirerZ80B()
{
	m_scfCtrl = DefaultCtrlFigures_InquirerContent;

}

CSemCtrl_InquirerZ80B::~CSemCtrl_InquirerZ80B()
{

}

BOOL CSemCtrl_InquirerZ80B::Flush()
{
	SEM_ControlMessage	hostMSG;
	USB_SEM_ControlMessage	usb_hostMSG;
	static	BOOL		blAlternative = TRUE;

	if (blAlternative)
	{
		//查询选通状态
		hostMSG.bMessageID		= (BYTE)Z80B_Inquirer;
		hostMSG.bCtlMSGLength	= (BYTE)1;
		usb_hostMSG.bMessageID		= (BYTE)Z80B_Inquirer;
		usb_hostMSG.bCtlMSGLength	= (BYTE)1;
	}
	else
	{
		//查询灯丝加热电流值
		hostMSG.bMessageID		= (BYTE)Z80B_HeatingFilamentElectricity;
		hostMSG.bCtlMSGLength	= (BYTE)1;
		usb_hostMSG.bMessageID		= (BYTE)Z80B_HeatingFilamentElectricity;
		usb_hostMSG.bCtlMSGLength	= (BYTE)1;
	}
	blAlternative = !blAlternative;

	BOOL blRet = FALSE;
	return blRet;
}
