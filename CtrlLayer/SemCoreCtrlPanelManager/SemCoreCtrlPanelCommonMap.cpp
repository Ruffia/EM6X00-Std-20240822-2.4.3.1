// SemCoreCtrlPanelCommonMap.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "SemCoreCtrlPanelCommonMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanelCommonMap

IMPLEMENT_DYNCREATE(CSemCoreCtrlPanelCommonMap, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanelCommonMap properties


CSemCoreCtrlPanelCommonMap::CSemCoreCtrlPanelCommonMap()
{
}

CSemCoreCtrlPanelCommonMap::~CSemCoreCtrlPanelCommonMap()
{
}


BEGIN_MESSAGE_MAP(CSemCoreCtrlPanelCommonMap, CWnd)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanelCommonMap)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanelCommonMap message handlers

void CSemCoreCtrlPanelCommonMap::Bind(const VARIANT& ClassID, const VARIANT& SerialNumber)
{
	m_varPanelClassID		= ClassID;
	m_varPanelSerialNumber	= SerialNumber;
}

void CSemCoreCtrlPanelCommonMap::Enable(const VARIANT& Enabled, const VARIANT& Flag)
{

}

void CSemCoreCtrlPanelCommonMap::SetRange(const VARIANT& Xmin, const VARIANT& Xmax, const VARIANT& Ymin, const VARIANT& Ymax)
{
	m_scfCtrlX.varMin = Xmin;
	m_scfCtrlX.varMax = Xmax;

	m_scfCtrlY.varMin = Ymin;
	m_scfCtrlY.varMax = Ymax;
}

void CSemCoreCtrlPanelCommonMap::SetPos(const VARIANT& Xpos, const VARIANT& Ypos, const VARIANT& Flag)
{
	m_scfCtrlX.varPos = Xpos;
	m_scfCtrlY.varPos = Ypos;
}
	
void CSemCoreCtrlPanelCommonMap::SyncPos(const VARIANT& Xpos, const VARIANT& Ypos, const VARIANT& Flag)
{
	m_scfCtrlX.varPos = Xpos;
	m_scfCtrlY.varPos = Ypos;
}

void CSemCoreCtrlPanelCommonMap::SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT& Flag)
{
	m_strX = strX;
	m_strY = strY;
}
