// SemCtrl_AutoStigmator.cpp: implementation of the CSemCtrl_AutoStigmator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_WD.h"
#include "SemCtrl_ObjectiveLens.h"
#include "SemCtrl_StigmatorX.h"
#include "SemCtrl_StigmatorY.h"
#include "SemCtrl_AutoStigmator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_AutoStigmator::CSemCtrl_AutoStigmator()
{
	m_scfCtrl = DefaultCtrlFigures_AutoStigmator;

	m_pStigmatorX	= NULL;
	m_pStigmatorY	= NULL;
}

CSemCtrl_AutoStigmator::~CSemCtrl_AutoStigmator()
{

}


BOOL CSemCtrl_AutoStigmator::Bind( CSemCtrl_StigmatorX*	pStigmatorX )
{
	if ( pStigmatorX != NULL )
	{
		m_pStigmatorX		= pStigmatorX;

		if( (m_pStigmatorX != NULL) && (m_pStigmatorY != NULL) )
			m_bReady			= TRUE;
	}

	return m_bReady;
}

BOOL CSemCtrl_AutoStigmator::Bind( CSemCtrl_StigmatorY*	pStigmatorY )
{
	if ( pStigmatorY != NULL )
	{
		m_pStigmatorY		= pStigmatorY;

		if( (m_pStigmatorX != NULL) && (m_pStigmatorY != NULL) )
			m_bReady			= TRUE;
	}

	return m_bReady;
}

BOOL CSemCtrl_AutoStigmator::Flush()
{
	WORD				wStigmatorCtlX;
	WORD				wStigmatorCtlY;
	SEM_ControlMessage	hostMSG;
	VARIANT				varPos;
	long				lCtlIndex;

	if (m_pStigmatorX != NULL)
	{
		varPos			= m_pStigmatorX->GetPos();
		lCtlIndex		= varPos.lVal;
		wStigmatorCtlX	= (WORD)(lCtlIndex - m_pStigmatorX->m_scfCtrl.varMin.lVal);
	}
	if (m_pStigmatorY != NULL)
	{
		varPos			= m_pStigmatorY->GetPos();
		lCtlIndex		= varPos.lVal;
		wStigmatorCtlY	= (WORD)(lCtlIndex - m_pStigmatorY->m_scfCtrl.varMin.lVal);
	}

	hostMSG.bMessageID		= (BYTE)Z80B_AutoStigmator;
	hostMSG.bCtlMSGLength	= (BYTE)5;
	hostMSG.bData[1]		= (BYTE) wStigmatorCtlX;
	hostMSG.bData[2]		= (BYTE) wStigmatorCtlY;
	hostMSG.bData[3]		= (BYTE) 0x01;
	hostMSG.bData[4]		= (BYTE) 0x01;

	BOOL blRet = FALSE;
/*	if ( m_pMPComm != NULL )
		blRet = CMP::Instance().PostMSG( SEM_Z80ID_Z80B, hostMSG );
*/
	return blRet;
}
