// SemCtrl_SP_PMT.cpp : implementation file
//

#include "stdafx.h"
#include "SemCtrl_SP_PMT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSemCtrl_SP_PMT

CSemCtrl_SP_PMT::CSemCtrl_SP_PMT()
{
	m_scfCtrl = DefaultCtrlFigures_SP_PMT;
}

CSemCtrl_SP_PMT::~CSemCtrl_SP_PMT()
{
}

BOOL CSemCtrl_SP_PMT::Flush()
{
	return TRUE;
}

const CString& CSemCtrl_SP_PMT::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;
	
	// Format Display string
	if( m_nOpr > 0 )
		m_strText.Format( "%.2f [ %03X ]", lCtlIndex * 100.0 /m_scfCtrl.varMax.lVal, lCtlIndex*10 );
	else
		m_strText.Format( "%.2f", lCtlIndex * 100.0 /m_scfCtrl.varMax.lVal );
	
	return m_strText;
}

const CString& CSemCtrl_SP_PMT::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;
	
	// Format Display string
	strText.Format( "%.2f", lCtlIndex * 100.0 /m_scfCtrl.varMax.lVal );
	
	return strText;
}

void CSemCtrl_SP_PMT::SetParams( int nType, double dParam )// Type:类型；Param:要设置的参数值
{
	switch( nType )
	{
	case	100:	// OprAdvance
		m_nOpr	= (int)dParam;
		break;
		
	default:
		break;
	}
}


long  CSemCtrl_SP_PMT::GetPosByValue(const double dValue)
{
	long lPos = 0;
	if (m_nOpr > 0)
	{
		lPos = dValue * m_scfCtrl.varMax.lVal/100.0;
	}
	else
	{
		lPos = dValue * m_scfCtrl.varMax.lVal/100.0;
	}

	return lPos;
}