// SemCtrl_GunCurrent.cpp: implementation of the CSemCtrl_GunCurrent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_GunCurrent.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_GunCurrent::CSemCtrl_GunCurrent()
{
	m_scfCtrl = DefaultCtrlFigures_GunCurrent;

}

CSemCtrl_GunCurrent::~CSemCtrl_GunCurrent()
{

}

const CString& CSemCtrl_GunCurrent::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// Format Display string
	m_strText.Format( "%03d", lCtlIndex );

	return m_strText;
}

const CString& CSemCtrl_GunCurrent::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%03d", lCtlIndex );

	return strText;
}


BOOL CSemCtrl_GunCurrent::Bind( CSemCtrl_GunCurrentAccelerate*	pGunCurrentAccelerate )
{
	if ( pGunCurrentAccelerate != NULL )
	{
		m_pGunCurrentAccelerate	= pGunCurrentAccelerate;
		m_bReady				= TRUE;
	}

	return m_bReady;
}

VARIANT CSemCtrl_GunCurrent::SetPos( VARIANT varPos )
{
	CSemBasisCtrl::SetPos( varPos );

	if ( m_pGunCurrentAccelerate != NULL )
	{
		/*
		///////////////////////////////////////////////////////////
		//           -------
		// ---------- <-a-> ----------
		const double ReserveSide	= 0.2;	// [0.1~0.9]
		const long lX0 = 100;				// 50 ~ 150
		const long lX1 = 130;				// 100 ~ 200
		///////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////
		const long lMin = m_scfCtrl.varMin.lVal;
		const long lMax = m_scfCtrl.varMax.lVal;
		const long lPos = m_scfCtrl.varPos.lVal - m_scfCtrl.varMin.lVal;

		VARIANT varMin, varMax;
		m_pGunCurrentAccelerate->GetRange( varMin, varMax );
		const double	dblMin = varMin.dblVal;
		const double	dblMax = varMax.dblVal;
		///////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////
		const double dblA_Side	= ReserveSide * (dblMax - dblMin) / ((lMax - lMin) - (lX1 - lX0));
		const double dblA		= (1 - ReserveSide) * (dblMax - dblMin) / (lX1 - lX0);
		///////////////////////////////////////////////////////////

		VARIANT varAcceleratePos;
		///////////////////////////////////////////////////////////
		if ( lPos < lX0 )
		{
			varAcceleratePos.dblVal = dblMin + lPos * dblA_Side;
		}
		else if ( lPos < lX1 )
		{
			varAcceleratePos.dblVal = dblMin + lX0 * dblA_Side + (lPos-lX0) * dblA;
		}
		else
		{
			varAcceleratePos.dblVal = dblMin + lX0 * dblA_Side + (lX1-lX0) * dblA + (lPos-lX1) * dblA_Side;
		}
		///////////////////////////////////////////////////////////

		m_pGunCurrentAccelerate->SetPos( varAcceleratePos );
		*/

		VARIANT varAcceleratePos;
		const long lX0 = 70;				// 50 ~ 150
		const long lX1 = 120;				// 100 ~ 200
		///////////////////////////////////////////////////////////
		if ( m_scfCtrl.varPos.lVal < lX0 )
		{
			varAcceleratePos.dblVal = lX0;
		}
		else if ( m_scfCtrl.varPos.lVal > lX1 )
		{
			varAcceleratePos.dblVal = lX1;
		}
		else
		{
			varAcceleratePos.dblVal = m_scfCtrl.varPos.lVal;
		}
		///////////////////////////////////////////////////////////
		m_pGunCurrentAccelerate->SetPos( varAcceleratePos );
	}

	return m_scfCtrl.varPos;
}
