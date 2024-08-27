// SemCtrl_InquirerZ80A.cpp: implementation of the CSemCtrl_InquirerZ80A class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_InquirerZ80A.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_InquirerZ80A::CSemCtrl_InquirerZ80A()
{
	m_scfCtrl = DefaultCtrlFigures_InquirerContent;

}

CSemCtrl_InquirerZ80A::~CSemCtrl_InquirerZ80A()
{

}

BOOL CSemCtrl_InquirerZ80A::Flush()
{
	SEM_ControlMessage	hostMSG;
	USB_SEM_ControlMessage	usb_hostMSG;

	hostMSG.bMessageID		= (BYTE)Z80A_Inquirer;
	hostMSG.bCtlMSGLength	= (BYTE)1;

	usb_hostMSG.bMessageID		= (BYTE)Z80A_Inquirer;
	usb_hostMSG.bCtlMSGLength	= (BYTE)1;

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}
