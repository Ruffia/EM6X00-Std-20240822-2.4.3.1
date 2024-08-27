// SemCoreCtrlPanelList.h: interface for the CSemCoreCtrlPanelList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCORECTRLPANELLIST_H__DA12ED27_1F1E_4F47_8419_011BCB013B92__INCLUDED_)
#define AFX_SEMCORECTRLPANELLIST_H__DA12ED27_1F1E_4F47_8419_011BCB013B92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSemCoreCtrlPanelCommonMap;

const long SemCoreCtrlPanelNumberMax = 100;
const long LinkTableNULL = -1;

class CSemCoreCtrlPanelList  
{
	typedef struct tagPanelMapNode
	{
		CSemCoreCtrlPanelCommonMap*	pPanelMap;
		long						lNext;
	}PanelMapNode;
	
	PanelMapNode	m_NodeBuffer[SemCoreCtrlPanelNumberMax];
	
	long	m_FreeLinkTableHead, m_FreeLinkTableTail, m_FreeLinkTableLength;
	long	m_UsedLinkTableHead, m_UsedLinkTableTail, m_UsedLinkTableLength;

public:
	CSemCoreCtrlPanelList();
	~CSemCoreCtrlPanelList();

	BOOL Add( CSemCoreCtrlPanelCommonMap* pPanelMap );
	BOOL Delete( CSemCoreCtrlPanelCommonMap* pPanelMap );
	long GetCurrentFreeNodeID();

	long GetFirstNodeID();
	CSemCoreCtrlPanelCommonMap*  GetNextNode( long& lNodeID );
	BOOL IsAvaidNode( long lNodeID );
};

#endif // !defined(AFX_SEMCORECTRLPANELLIST_H__DA12ED27_1F1E_4F47_8419_011BCB013B92__INCLUDED_)
