// SemCtrlPanelManager.cpp: implementation of the CSemCtrlPanelManager class.
//
// Last Modify : 2008.06.04
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrlPanelManager.h"
#include "SemCoreCtrlPanelFunctionHandler.h"
using  namespace nsSemCoreCtrlPanel;
#include "SemCoreCtrlPanelCoadjustHandler.h"
using  namespace nsSemCoreCtrlPanelCoadjust;
#include "Factory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrlPanelManager::CSemCtrlPanelManager()
: m_mutexPanelList(FALSE, NULL)
{
	m_bReady	= FALSE;
}

CSemCtrlPanelManager::~CSemCtrlPanelManager()
{

}


CSemCtrlPanelManager& CSemCtrlPanelManager::Instance()
{
	static CSemCtrlPanelManager Inst;
	return Inst;
}

BOOL CSemCtrlPanelManager::Bind( )
{
	m_bReady	= TRUE;
	CString csFilename = _T("Coadjust_WD.txt");
	char szBuf[100];
	DWORD dwSize = 100;
	HKEY hk;
	if( RegOpenKeyEx( HKEY_CURRENT_USER, 
		"Software\\KYKY", 
		0, 
		KEY_ALL_ACCESS,
		&hk ) == ERROR_SUCCESS )
	{
		if( RegQueryValueEx( hk, 
			"Coadjust param filename",
			NULL,
			NULL,
			(LPBYTE)szBuf,
			&dwSize) == ERROR_SUCCESS )
			csFilename.Format( "%s", szBuf );
		RegCloseKey(hk);
	}

	return m_bReady;
}

BOOL CSemCtrlPanelManager::UnRegist( CSemCoreCtrlPanelCommonMap* pSemCoreCtrlPanelMap )
{
	BOOL	boolRet = FALSE;
	CSingleLock sLock( &m_mutexPanelList );

	sLock.Lock();
	boolRet = m_PanelList.Delete( pSemCoreCtrlPanelMap );
	sLock.Unlock();

	return boolRet;
}

DWORD CSemCtrlPanelManager::SemCoreCtrlPanelMessageProc(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	if ( SerialNumber.lVal != -1 )
	{
		if ( !m_PanelList.IsAvaidNode( SerialNumber.lVal ) )
			return -1;
	}

	if ( isReady() )
		ControlSemCore( ClassID, Xpos, Ypos, Flag );

	SyncSemCoreCtrlPanel( ClassID, SerialNumber, Xpos, Ypos, Flag );

	SemControlPanelClassID classID = (SemControlPanelClassID)ClassID.lVal;
	ISemCoreCtrlPanelCoadjustHandler* pHandler = Factory<ISemCoreCtrlPanelCoadjustHandler,SemControlPanelClassID>::Instance().BuildProduct(classID);
	if (pHandler)
	{
		pHandler->Adjust(this,ClassID, SerialNumber, Xpos, Ypos, Flag);	
		delete pHandler; pHandler = NULL;
	}

	return 0;
}


DWORD CSemCtrlPanelManager::SemCoreCtrlPanel_SyncPos(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	if ( SerialNumber.lVal != -1 )
	{
		if ( !m_PanelList.IsAvaidNode( SerialNumber.lVal ) )
			return -1;
	}

	if ( isReady() )
		ControlSemCore( ClassID, Xpos, Ypos, Flag );

	return 0;
}


BOOL CSemCtrlPanelManager::SyncSemCoreCtrlPanelRange(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, 
	const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax)
{
	BOOL bRet = FALSE;

	long lNodeID = m_PanelList.GetFirstNodeID();
	CSemCoreCtrlPanelCommonMap*  pPanelMap;

	while( lNodeID != LinkTableNULL )
	{
		pPanelMap = m_PanelList.GetNextNode( lNodeID );
		long lClassID		= pPanelMap->GetPanelClassID().lVal;
		long lSerialNumber	= pPanelMap->GetPanelSerialNumber().lVal;

		if( lClassID == ClassID.lVal )
		{
			if( lSerialNumber != SerialNumber.lVal )
			{
				pPanelMap->SetRange( Xmin, Xmax, Ymin, Ymax );
			}
			else
			{
				bRet = TRUE;
			}
		}
	}
	
	return bRet;
}


BOOL CSemCtrlPanelManager::SyncSemCoreCtrlPanel(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	BOOL bRet = FALSE;

	long lNodeID = m_PanelList.GetFirstNodeID();
	CSemCoreCtrlPanelCommonMap*  pPanelMap;

	while( lNodeID != LinkTableNULL )
	{
		pPanelMap = m_PanelList.GetNextNode( lNodeID );
		long lClassID		= pPanelMap->GetPanelClassID().lVal;
		long lSerialNumber	= pPanelMap->GetPanelSerialNumber().lVal;

		if( lClassID == ClassID.lVal )
		{
			if( lSerialNumber != SerialNumber.lVal )//== -1 )//!= SerialNumber.lVal )//09.03.10
			{
				pPanelMap->SyncPos( Xpos, Ypos, Flag );
				SyncSemCoreCtrlPanelText( ClassID, Flag, pPanelMap );
			}
			else
			{
				bRet = TRUE;

				//pPanelMap->SyncPos( Xpos, Ypos, Flag );
				SyncSemCoreCtrlPanelText( ClassID, Flag, pPanelMap );
			}
		}
	}
	
	return bRet;
}

BOOL CSemCtrlPanelManager::SyncSemCoreCtrlPanelText(const VARIANT FAR& ClassID, const VARIANT FAR& Flag, CSemCoreCtrlPanelCommonMap*  pPanelMap)
{
	BOOL bRet = TRUE;
	SemControlPanelClassID classID = (SemControlPanelClassID)ClassID.lVal;
	ISemCoreCtrlPanelHandler* pHandler = Factory<ISemCoreCtrlPanelHandler,SemControlPanelClassID>::Instance().BuildProduct(classID);
	if (pHandler)
	{
		pHandler->SyncText(pPanelMap,Flag);	
		delete pHandler; pHandler = NULL;
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}

BOOL CSemCtrlPanelManager::ControlSemCore(const VARIANT FAR& ClassID, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag)
{
	BOOL bRet = TRUE;

	SemControlPanelClassID classID = (SemControlPanelClassID)ClassID.lVal;
	ISemCoreCtrlPanelHandler* pHandler = Factory<ISemCoreCtrlPanelHandler,SemControlPanelClassID>::Instance().BuildProduct(classID);
	if (pHandler)
	{
		pHandler->SetPos(Xpos,Ypos,Flag);	
		delete pHandler; pHandler = NULL;
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}

BOOL CSemCtrlPanelManager::Regist( CSemCoreCtrlPanelCommonMap* pSemCoreCtrlPanelMap, long lPanelClassID )
{
	VARIANT  ClassID;
	VARIANT  SerialNumber;
	CSingleLock sLock( &m_mutexPanelList );

	sLock.Lock();

	long lSN = m_PanelList.GetCurrentFreeNodeID();
	if ( lSN == LinkTableNULL )
	{
		sLock.Unlock();
		return FALSE;
	}

	ClassID.lVal		= lPanelClassID;
	SerialNumber.ulVal  = lSN;
	pSemCoreCtrlPanelMap->Bind( ClassID, SerialNumber );
	m_PanelList.Add( pSemCoreCtrlPanelMap );

	sLock.Unlock();

	// Î´Íê´ýÐø
	VARIANT  Flag;
	Flag.ulVal = FlagCtrlX;
	SemControlPanelClassID classID = (SemControlPanelClassID)ClassID.lVal;
	ISemCoreCtrlPanelHandler* pHandler = Factory<ISemCoreCtrlPanelHandler,SemControlPanelClassID>::Instance().BuildProduct(classID);
	if (pHandler)
	{
		pHandler->SyncPos(pSemCoreCtrlPanelMap);	
		delete pHandler; pHandler = NULL;
	}
	
	SyncSemCoreCtrlPanelText( ClassID, Flag, pSemCoreCtrlPanelMap );
	
	return TRUE;
}

BOOL CSemCtrlPanelManager::EnableSemCoreCtrlPanel(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Enabled, const VARIANT FAR& Flag)
{
	BOOL bRet = FALSE;

	long lNodeID = m_PanelList.GetFirstNodeID();
	CSemCoreCtrlPanelCommonMap*  pPanelMap;

	while( lNodeID != LinkTableNULL )
	{
		pPanelMap = m_PanelList.GetNextNode( lNodeID );
		long lClassID		= pPanelMap->GetPanelClassID().lVal;
		long lSerialNumber	= pPanelMap->GetPanelSerialNumber().lVal;

		if( lClassID == ClassID.lVal )
		{
			if( lSerialNumber != SerialNumber.lVal )
			{
				pPanelMap->Enable(Enabled, Flag);
			}
			else
			{
				bRet = TRUE;
			}
		}
	}
	
	return bRet;
}
