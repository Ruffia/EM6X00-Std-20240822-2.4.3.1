#include "stdafx.h"
#include "SemCoreCtrlPanelMessageTransmiter.h"
#include "SemCtrlPanelManager.h"
#include "Factory.h"
#include "SemCoreCtrlPanelMessageProc.h"
using namespace SemCoreCtrlPanelMessageProcessor;

CSemCoreCtrlPanelMessageTransmiter& CSemCoreCtrlPanelMessageTransmiter::Instance()
{
	static CSemCoreCtrlPanelMessageTransmiter Inst;
	return Inst;
}


CSemCoreCtrlPanelMessageTransmiter::CSemCoreCtrlPanelMessageTransmiter()
{
	m_pLanguageManagerCtrl = NULL;
}


DWORD CSemCoreCtrlPanelMessageTransmiter::SemCoreCtrlPanelMessageProc(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCtrlPanelManager::Instance().SemCoreCtrlPanelMessageProc( ClassID, SerialNumber, Xpos, Ypos, Flag );
	BOOL bXpos = Xpos.lVal != 0 ;

	SemControlPanelClassID ctrlClssID = (SemControlPanelClassID)ClassID.lVal;
	ISemCoreCtrlPanelMessageProc* pProc = Factory<ISemCoreCtrlPanelMessageProc,SemControlPanelClassID>::Instance().BuildProduct(ctrlClssID);
	if (pProc)
	{
		pProc->DisplayMsg(m_pLanguageManagerCtrl,ClassID,bXpos);
		delete pProc; pProc = NULL;
	}
	else
	{
		pProc = Factory<ISemCoreCtrlPanelMessageProc,SemControlPanelClassID>::Instance().BuildProduct(SemControlPanelClass_NULL);
		if (pProc)
		{
			pProc->DisplayMsg(m_pLanguageManagerCtrl,ClassID,bXpos);
			delete pProc; pProc = NULL;
		}
	}

	return 0;
}


DWORD CSemCoreCtrlPanelMessageTransmiter::SemCoreCtrlPanel_SetPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	CSemCtrlPanelManager::Instance().SemCoreCtrlPanel_SyncPos( ClassID, SerialNumber, Xpos, Ypos, Flag );
	return 0;
}