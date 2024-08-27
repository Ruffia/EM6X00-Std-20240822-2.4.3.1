// SemCtrl_SP_HV.cpp : implementation file
//

#include "stdafx.h"
#include "SemCtrl_SP_HV.h"
#include "CoadjustParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSemCtrl_SP_HV

CSemCtrl_SP_HV::CSemCtrl_SP_HV()
{
	//WORD	nCtrlDataSize = 100;

	// 2005-2-2_ͳ��
	WORD nCtrlDataSize = LoadSemHVAdjustParam( HVCoadjustParamFilename, 
		CtrlDataSizeMax_HV,m_wHVCtlData, m_wHVAdjustData );

	m_scfCtrl = DefaultCtrlFigures_SP_HV;
	m_scfCtrl.varMax.lVal = nCtrlDataSize - 1;

	m_pCL	= NULL;
	m_pOL	= NULL;
}

CSemCtrl_SP_HV::~CSemCtrl_SP_HV()
{
}

BOOL CSemCtrl_SP_HV::Flush()
{
	return TRUE;
}

const CString& CSemCtrl_SP_HV::FormatText()
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

const CString& CSemCtrl_SP_HV::FormatText( VARIANT varPos )
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


BOOL CSemCtrl_SP_HV::Bind( CSemCtrl_HVAccelerate*	pHVAccelerate )
{
	if ( pHVAccelerate != NULL )
	{
		m_pHVAccelerate	= pHVAccelerate;
		m_bReady		= TRUE;
	}
	
	return m_bReady;
}

BOOL CSemCtrl_SP_HV::Bind( CSemCtrl_CondenserLens*	pCL )
{
	if ( pCL != NULL )
	{
		m_pCL		= pCL;
		m_bReady	= TRUE;
	}

	return m_bReady;
}

BOOL CSemCtrl_SP_HV::Bind( CSemCtrl_ObjectiveLens*	pOL )
{
	if ( pOL != NULL )
	{
		m_pOL		= pOL;
		m_bReady	= TRUE;
	}

	return m_bReady;
}

VARIANT CSemCtrl_SP_HV::SetPos( VARIANT varPos )
{
	CSemBasisCtrl::SetPos( varPos );

//	if ( m_pHVAccelerate != NULL )
	{
		VARIANT varPos = GetPos();
		long lCtlIndex = varPos.lVal;

		VARIANT varAcceleratePos;
		varAcceleratePos.dblVal = m_wHVCtlData[ lCtlIndex ]/10.0;
		m_pHVAccelerate->SetPos( varAcceleratePos );

		// ��ϵͳ�и�ѹͳ��Ҫ����������
		// ��ѹ�����ı�ʱ���۹⾵�������ﾵ������ɨ����Ȧ������Ҫ�����仯
		// ԭ���͵�Z80�ĸ�ѹͳ��ֵҪ��Ϊϵ����������3��ֵ
		// 30KVʱ��ϵ��Ϊ1
		m_pCL->SetParams( SET_PARAMS_Rate, m_wHVCtlData[ lCtlIndex ] );
		m_pOL->SetParams( SET_PARAMS_Rate, m_wHVCtlData[ lCtlIndex ] );
	}

	return m_scfCtrl.varPos;
}