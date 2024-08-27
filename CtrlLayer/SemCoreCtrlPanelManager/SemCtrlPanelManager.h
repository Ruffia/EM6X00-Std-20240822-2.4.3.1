// SemCtrlPanelManager.h: interface for the CSemCtrlPanelManager class.
//
// Last Modify : 2009.03.10
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRLPANELMANAGER_H__7E2078BD_337E_433F_B949_6C1FD3A79D1B__INCLUDED_)
#define AFX_SEMCTRLPANELMANAGER_H__7E2078BD_337E_433F_B949_6C1FD3A79D1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemCoreAssemble.h"
#include "SemCoreCtrlPanelCommonMap.h"
#include "SemCoreCtrlPanelList.h"
#include <afxmt.h>


class AFX_EXT_CLASS CSemCtrlPanelManager  
{
	CSemCoreCtrlPanelList	m_PanelList;
	CMutex					m_mutexPanelList;

	BOOL					m_bReady;

protected:
	CSemCtrlPanelManager();
	virtual ~CSemCtrlPanelManager();

public:
	static CSemCtrlPanelManager& Instance();

	BOOL isReady(){ return m_bReady; }
	BOOL Bind( );

	BOOL Regist( CSemCoreCtrlPanelCommonMap* pSemCoreCtrlPanelMap, long lPanelClassID );
	BOOL UnRegist( CSemCoreCtrlPanelCommonMap* pSemCoreCtrlPanelMap );

	DWORD SemCoreCtrlPanelMessageProc(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	BOOL EnableSemCoreCtrlPanel(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Enabled, const VARIANT FAR& Flag);

	DWORD SemCoreCtrlPanel_SyncPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);

	BOOL SyncSemCoreCtrlPanel(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber,
		const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);

	BOOL ControlSemCore(const VARIANT FAR& ClassID, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);

	BOOL SyncSemCoreCtrlPanelRange(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, 
		const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax);

	BOOL SyncSemCoreCtrlPanelText(const VARIANT FAR& ClassID, const VARIANT FAR& Flag, CSemCoreCtrlPanelCommonMap*  pPanelMap);

};

#endif // !defined(AFX_SEMCTRLPANELMANAGER_H__7E2078BD_337E_433F_B949_6C1FD3A79D1B__INCLUDED_)
