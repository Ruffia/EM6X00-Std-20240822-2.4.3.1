// SemCoreCtrlPanelList.cpp: implementation of the CSemCoreCtrlPanelList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCoreCtrlPanelCommonMap.h"
#include "SemCoreCtrlPanelList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCoreCtrlPanelList::CSemCoreCtrlPanelList()
{
	m_FreeLinkTableHead		= 0;
	m_FreeLinkTableTail		= SemCoreCtrlPanelNumberMax-1;
	m_FreeLinkTableLength	= SemCoreCtrlPanelNumberMax;

	m_UsedLinkTableHead		= LinkTableNULL;
	m_UsedLinkTableTail		= LinkTableNULL;
	m_UsedLinkTableLength	= 0;

	for(long i=0; i<SemCoreCtrlPanelNumberMax; i++)
	{
		m_NodeBuffer[i].pPanelMap = NULL;
		m_NodeBuffer[i].lNext = i+1;
	}
	m_NodeBuffer[ m_FreeLinkTableTail ].lNext = LinkTableNULL;
}

CSemCoreCtrlPanelList::~CSemCoreCtrlPanelList()
{
}

BOOL CSemCoreCtrlPanelList::Add( CSemCoreCtrlPanelCommonMap* pPanelMap )
{
	if ( m_FreeLinkTableLength <= 0 ) return FALSE;

	///////////////////////////////////////////////
	// Save pPanelMap in lNode
	///////////////////////////////////////////////
	long lNode = m_FreeLinkTableHead;
	m_NodeBuffer[lNode].pPanelMap = pPanelMap;

	///////////////////////////////////////////////
	// Remove lNode from FreeLinkTable
	///////////////////////////////////////////////
	if ( m_FreeLinkTableLength > 1 ) // Not in tail
	{
		m_FreeLinkTableHead	= m_NodeBuffer[lNode].lNext;
		m_FreeLinkTableLength --;
	}
	else
	{
		m_FreeLinkTableHead	= LinkTableNULL;
		m_FreeLinkTableTail	= LinkTableNULL;
		m_FreeLinkTableLength = 0;
	}

	///////////////////////////////////////////////
	// Add lNode to UsedLinkTable
	///////////////////////////////////////////////
	if ( m_UsedLinkTableLength > 0 ) // Not at first
	{
		m_NodeBuffer[m_UsedLinkTableTail].lNext = lNode;
		m_NodeBuffer[lNode].lNext	= LinkTableNULL;
		m_UsedLinkTableTail			= lNode;
		m_UsedLinkTableLength ++;
	}
	else
	{
		m_UsedLinkTableHead			= lNode;
		m_NodeBuffer[lNode].lNext	= LinkTableNULL;
		m_UsedLinkTableTail			= lNode;
		m_UsedLinkTableLength = 1;
	}

	return TRUE;
}

BOOL CSemCoreCtrlPanelList::Delete( CSemCoreCtrlPanelCommonMap* pPanelMap )
{
	if (m_UsedLinkTableLength <= 0) return FALSE;

	///////////////////////////////////////////////
	// Find pPanelMap in UsedLinkTable
	///////////////////////////////////////////////
	long lNode = m_UsedLinkTableHead;
	long lPriorNode = m_UsedLinkTableHead;

	while( lNode != LinkTableNULL )
	{
		if ( pPanelMap == m_NodeBuffer[lNode].pPanelMap )
			break;
		else
		{
			lPriorNode = lNode;
			lNode = m_NodeBuffer[lNode].lNext;
		}
	};
	if ( lNode == LinkTableNULL ) return FALSE;

	///////////////////////////////////////////////
	// Remove lNode from UsedLinkTable
	///////////////////////////////////////////////
	if ( lNode == m_UsedLinkTableHead ) // At head
	{
		m_UsedLinkTableHead	= m_NodeBuffer[lNode].lNext;
		m_UsedLinkTableLength --;
		
		if( m_UsedLinkTableLength == 0)
			m_UsedLinkTableTail	= LinkTableNULL;
	}
	else
	{
		m_NodeBuffer[lPriorNode].lNext = m_NodeBuffer[lNode].lNext;
		m_UsedLinkTableLength --;

		if( m_NodeBuffer[lNode].lNext == LinkTableNULL )
			m_UsedLinkTableTail	= lPriorNode;
	}

	///////////////////////////////////////////////
	// Clean pPanelMap in lNode
	///////////////////////////////////////////////
	m_NodeBuffer[lNode].pPanelMap = NULL;


	///////////////////////////////////////////////
	// Add lNode to FreeLinkTable
	///////////////////////////////////////////////
	if ( m_FreeLinkTableLength > 0 ) // Not in tail
	{
		m_NodeBuffer[m_FreeLinkTableTail].lNext = lNode;
		m_NodeBuffer[lNode].lNext	= LinkTableNULL;
		m_FreeLinkTableTail			= lNode;

		m_FreeLinkTableLength ++;
	}
	else
	{
		m_FreeLinkTableHead	= lNode;
		m_FreeLinkTableTail	= lNode;
		
		m_NodeBuffer[lNode].lNext	= LinkTableNULL;

		m_FreeLinkTableLength = 1;
	}

	return TRUE;
}

long CSemCoreCtrlPanelList::GetCurrentFreeNodeID()
{
	return m_FreeLinkTableHead;
}

long CSemCoreCtrlPanelList::GetFirstNodeID()
{
	return m_UsedLinkTableHead;
}

CSemCoreCtrlPanelCommonMap*  CSemCoreCtrlPanelList::GetNextNode( long& lNodeID )
{
	if ( lNodeID == LinkTableNULL ) return NULL;

	CSemCoreCtrlPanelCommonMap*  pPanelMap = m_NodeBuffer[lNodeID].pPanelMap;

	lNodeID = m_NodeBuffer[lNodeID].lNext;

	return pPanelMap;
}

BOOL CSemCoreCtrlPanelList::IsAvaidNode( long lNodeID )
{
	if ( lNodeID >= SemCoreCtrlPanelNumberMax )
		return FALSE;

	if ( m_NodeBuffer[lNodeID].pPanelMap == NULL )
		return FALSE;

	return TRUE;
}
