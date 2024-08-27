// SemCtrl_AutoFocus.cpp: implementation of the CSemCtrl_AutoFocus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_WD.h"
#include "SemCtrl_ObjectiveLens.h"
#include "SemCtrl_AutoFocus.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_AutoFocus::CSemCtrl_AutoFocus()
{
	m_scfCtrl = DefaultCtrlFigures_AutoFocus;

	m_pFinalLens	= NULL;
}

CSemCtrl_AutoFocus::~CSemCtrl_AutoFocus()
{

}

BOOL CSemCtrl_AutoFocus::Bind( CSemCtrl_ObjectiveLens*	pFinalLens )
{
	if ( pFinalLens != NULL )
	{
		m_pFinalLens		= pFinalLens;
	}

	return TRUE;
}

BOOL CSemCtrl_AutoFocus::Flush()
{
	WORD				wFinalLensCtl;
	SEM_ControlMessage	hostMSG;

	if (m_pFinalLens != NULL)
	{
		VARIANT varPos = m_pFinalLens->GetPos();
		long lCtlIndex = varPos.lVal;

		wFinalLensCtl = (WORD)(lCtlIndex - m_pFinalLens->m_scfCtrl.varMin.lVal);
	}

	hostMSG.bMessageID		= (BYTE)Z80B_AutoObjectiveLens;
	hostMSG.bCtlMSGLength	= (BYTE)5;
	hostMSG.bData[1]		= (BYTE)( wFinalLensCtl & 0x00FF );
	hostMSG.bData[2]		= (BYTE)( wFinalLensCtl >> 8 );
	hostMSG.bData[3]		= (BYTE) 0x01;
	hostMSG.bData[4]		= (BYTE) 0x01;
	
	BOOL blRet = FALSE;
/*	if ( m_pMPComm != NULL )
		blRet = CMP::Instance().PostMSG( SEM_Z80ID_Z80B, hostMSG );
*/
	return blRet;
}
