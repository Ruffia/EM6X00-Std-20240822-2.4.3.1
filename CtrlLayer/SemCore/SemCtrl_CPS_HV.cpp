// SemCtrl_CPS_HV.cpp: implementation of the CSemCtrl_CPS_HV class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_CPS_HV.h"
#include "CoadjustParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern BOOL GetMyProfileDoubleEx( LPCTSTR lpKey, LPCTSTR lpSubKey, double& value);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_CPS_HV::CSemCtrl_CPS_HV()
{
	//WORD	nCtrlDataSize = 1;
	
	// 2005-2-2_统调
	WORD nCtrlDataSize = LoadSemHVAdjustParam( HVCoadjustParamFilename, 
		CtrlDataSizeMax_HV, m_wHVCtlData, m_wHVAdjustData );

	m_scfCtrl = DefaultCtrlFigures_CPS_HV;
	m_scfCtrl.varMax.vt = VT_UI4;
	m_scfCtrl.varMax.lVal = (long)(nCtrlDataSize-1);

	m_pCL	= NULL;
	m_pOL	= NULL;
}

CSemCtrl_CPS_HV::~CSemCtrl_CPS_HV()
{
}

BOOL CSemCtrl_CPS_HV::Flush()
{
	return TRUE;
}

const CString& CSemCtrl_CPS_HV::FormatText()
{
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// Format Display string
	if( m_wHVCtlData[ lCtlIndex ] < 100 )
	{
		m_strText.Format( "%2.1f KV", m_wHVCtlData[ lCtlIndex ]/10.0 );
	}
	else
	{
		m_strText.Format( "%d KV", m_wHVCtlData[ lCtlIndex ]/10 );
	}

	return m_strText;
}

const CString& CSemCtrl_CPS_HV::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	if( m_wHVCtlData[ lCtlIndex ] < 100 )
	{
		strText.Format( "%2.1f KV", m_wHVCtlData[ lCtlIndex ]/10.0 );
	}
	else
	{
		strText.Format( "%d KV", m_wHVCtlData[ lCtlIndex ]/10 );
	}	

	return strText;
}


BOOL CSemCtrl_CPS_HV::Bind( CSemCtrl_HVAccelerate*	pHVAccelerate )
{
	if ( pHVAccelerate != NULL )
	{
		m_pHVAccelerate	= pHVAccelerate;
		m_bReady		= TRUE;
	}

	return m_bReady;
}

BOOL CSemCtrl_CPS_HV::Bind( CSemCtrl_CondenserLens*	pCL )
{
	if ( pCL != NULL )
	{
		m_pCL		= pCL;
		m_bReady	= TRUE;
	}

	return m_bReady;
}

BOOL CSemCtrl_CPS_HV::Bind( CSemCtrl_ObjectiveLens*	pOL )
{
	if ( pOL != NULL )
	{
		m_pOL		= pOL;
		m_bReady	= TRUE;
	}

	return m_bReady;
}

VARIANT CSemCtrl_CPS_HV::SetPos( VARIANT varPos )
{
	CSemBasisCtrl::SetPos( varPos );

	if ( m_pHVAccelerate != NULL )
	{
		VARIANT varPos = GetPos();
		long lCtlIndex = varPos.lVal;

		VARIANT varAcceleratePos;
		varAcceleratePos.dblVal = m_wHVCtlData[ lCtlIndex ]/10.0;
		m_pHVAccelerate->SetPos( varAcceleratePos );

		// 新系统中高压统调要在软件上完成
		// 高压发生改变时，聚光镜电流、物镜电流和扫描线圈电流都要发生变化
		// 原来送到Z80的高压统调值要作为系数送至上述3个值
		// 30KV时该系数为1
		m_pCL->SetParams( SET_PARAMS_Rate, m_wHVCtlData[ lCtlIndex ] );
		m_pOL->SetParams( SET_PARAMS_Rate, m_wHVCtlData[ lCtlIndex ] );
// 		BOOL blRet = FALSE;
// 		if ( m_pMPComm != NULL )
// 		{
// 			blRet = CMP::Instance().CPS_Comm_SetValue( DEV_A, (m_wHVCtlData[ lCtlIndex ]/10.0*1000 + m_dVOffset_Set) / m_dVScale_Set, m_lEnable );
// 		}
	}

	return m_scfCtrl.varPos;
}
