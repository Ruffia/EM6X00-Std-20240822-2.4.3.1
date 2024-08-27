#pragma once
#include "USB_Functions.h"
#include "LanguageManagerCtrl.h"

class AFX_EXT_CLASS CSemCoreCtrlPanelMessageTransmiter
{
public:
	static CSemCoreCtrlPanelMessageTransmiter& Instance();

	void Attach(CLanguageManagerCtrl* pLanguageManagerCtrl)
	{
		m_pLanguageManagerCtrl = pLanguageManagerCtrl;
	}

	DWORD SemCoreCtrlPanelMessageProc(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);

	DWORD SemCoreCtrlPanel_SetPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	
protected:
	CSemCoreCtrlPanelMessageTransmiter();

protected:
	CLanguageManagerCtrl* m_pLanguageManagerCtrl;
};