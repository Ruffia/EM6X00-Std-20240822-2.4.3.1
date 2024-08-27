// SemCtrl_WD.cpp: implementation of the CSemCtrl_WD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_Magnifier.h"
#include "SemCtrl_WD.h"
#include "CoadjustParam.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// 15.05.14 新统调公式
const	double	Co_6200_a	= -1.253 * pow((double)10, (int)-11);
const	double	Co_6200_b	= 2.176 * pow((double)10, (int)-8);
const	double	Co_6200_c	= 1.225 * pow((double)10,(int)-6);

// const	double	Co_6900_a	= -6.566 * pow(10, -12);
// const	double	Co_6900_b	= 2.392 * pow(10, -8);
// const	double	Co_6900_c	= 1.061 * pow(10,-6);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 2005-2-2_统调
/*
//WorkDistance <--> ObjectiveLens electric-current : ObjectiveLens EC Ctl
//用于计算工作距离
const WORD CSemCtrl_WD::m_nObjectiveLensToWDIndex[] = 
{
//	0,		1,		2,		3,		4,		5,		6,		7,		8,		9,

	0x62FF,	0x64FF,	0x65FF,	0x66FF,	0x68FF,	0x69FF,	0x6AFF,	0x6CFF,	0x6EFF,	0x70FF,
	0x72FF,	0x74FF,	0x76FF,	0x78FF,	0x7AFF,	0x7EFF,	0x82FF,	0x86FF,	0x8AFF,	0x8EFF, 
	0x94FF,	0x9AFF,	0xA0FF,	0xA6FF,	0xACFF,	0xB2FF, 0xFFFF,
};

//WorkDistance <--> ObjectiveLens electric-current : WorkDistance ( Unit : mm )
//用于显示工作距离
const WORD CSemCtrl_WD::m_nWDIndexToWD[]=
{
//	0,		1,		2,		3,		4,		5,		6,		7,		8,		9,

	28,		27,		26,		25,		24,		23,		22,		21,		20,		19,
	18,		17,		16,		15,		14,		13,		12,		11,		10,		9,
	8,		7,		6,		5,		4,		3,		2,
};
*/

CSemCtrl_WD::CSemCtrl_WD()
{
	m_scfCtrl = DefaultCtrlFigures_WorkDistance;

	// Coadjust
	double*		pdouble	= new double[3];
	if( CSemBasisCtrl::ReadINI( INI_Coadjust, INIIndex_WD, pdouble ) )
	{
		m_scfCtrl.varMin.dblVal = (DOUBLE)pdouble[0];
		m_scfCtrl.varMax.dblVal = (DOUBLE)pdouble[1];
		m_scfCtrl.varPos.dblVal = (DOUBLE)12.0;
	}
	if( CSemBasisCtrl::ReadINI( INI_Coadjust, INIIndex_Factors, pdouble ) )
	{
		m_dCoFactorA	= pdouble[0];
		m_dCoFactorB	= pdouble[1];
		m_dCoFactorC	= pdouble[2];
	}
	else
	{
		m_dCoFactorA	= 0.0;
		m_dCoFactorB	= 0.0;
		m_dCoFactorC	= 0.0;
	}
	delete [] pdouble;

	m_dCurrentHV	= 0.0;
	m_nCoHVstage	= -1;

	m_bReady	= FALSE;
	m_pMag		= NULL;
}

CSemCtrl_WD::~CSemCtrl_WD()
{

}


BOOL CSemCtrl_WD::Bind( CSemCtrl_Magnifier*	pMag )
{
	//CSemCtrl_Magnifier * pMagSafety = DYNAMIC_DOWNCAST(CSemCtrl_Magnifier, pMag);
	
	if ( pMag != NULL )
	{
		m_pMag		= pMag;
		m_bReady	= TRUE;
	}

	return m_bReady;
}

BOOL CSemCtrl_WD::Bind( CSemCtrl_ObjectiveLens*	pOL )
{
	//CSemCtrl_WD * pWDSafety = DYNAMIC_DOWNCAST(CSemCtrl_WD, pWD);

	if ( pOL != NULL )
	{
		m_pOL		= pOL;
	}

	return m_bReady;
}

VARIANT CSemCtrl_WD::GetPos()
{
	VARIANT varPos = CSemBasisCtrl::GetPos();
//	long lWDIndex = varPos.lVal;

//	varPos.lVal = (long)m_nWDIndexToWD[ lWDIndex ];

	return varPos;
}

BOOL CSemCtrl_WD::LookupPos( WORD wObjectiveLensPos )
{
	long	i, lWDIndex, nCtrlDataSize;
	nCtrlDataSize = m_scfCtrl.varMax.lVal;

// 2005-2-2_统调
//	for( i=0; i<CtrlDataSize_WorkDistance; i++ )
	for( i=0; i<nCtrlDataSize; i++ )
	{
		if( wObjectiveLensPos <= m_nObjectiveLensToWDIndex[ i ] )
			break;
	}
	lWDIndex = i;

// 2005-2-2_统调
//	if( lWDIndex >= CtrlDataSize_WorkDistance )
	if( lWDIndex >= nCtrlDataSize )
		return FALSE;

	VARIANT varPos = CSemBasisCtrl::GetPos();
	if ( lWDIndex != varPos.lVal )
	{
		varPos.lVal = lWDIndex;	
		SetPos( varPos );
		
		if( m_bReady )
			m_pMag->UpdateWDIndex( lWDIndex );
	}

	return TRUE;
}

BOOL CSemCtrl_WD::Co_UpdatePosByM( double dM )
{
	// 6200: WD = C0 * C1 / M -120
	double dPos = Co_6200_C0 * Co_6200_C1 /dM /10 -12;
	VARIANT varPos = CSemBasisCtrl::GetPos();
	if ( dPos > 0.0 && dPos != varPos.dblVal )
	{
		// 只要WD变化，就统调MagMin
		varPos.dblVal = dPos;
		varPos = SetPos( varPos );
		if( m_bReady && dPos >= m_scfCtrl.varMin.dblVal && dPos <= m_scfCtrl.varMax.dblVal )
			m_pMag->Co_UpdatePosByM( dM );
	}
	return TRUE;
}

BOOL CSemCtrl_WD::Co_UpdatePosByOL( double dOL )
{
	// 6900和新6200
	// OLdisp3X = 1 / sqrt( a *WD *WD + b *WD + c)
	// a *WD *WD + b *WD +c = 1 / (OL*OL)
	// a *WD *WD + b *WD +C = 0
	// WD = ( -b + sqrt( b*b - 4*a*c) ) / (2*a)
	double dWD = 0.0;
	double dPos = 0.0;
	if( m_nType < 2 )
	{
		// 6200
		double C = Co_6200_c - 1.0 / (dOL *dOL);
		dWD = ( Co_6200_b *(-1.0) + sqrt( Co_6200_b *Co_6200_b -4 *Co_6200_a *C) ) / (2 *Co_6200_a);
		dPos = dWD /10.0;
	}
	else
	{
		// 6200 & 6900
		double C = m_dCoFactorC - 1.0 / (dOL *dOL);
		double delta = m_dCoFactorB *m_dCoFactorB -4 *m_dCoFactorA *C;
		if( delta > 0 )
		{
			dWD = ( m_dCoFactorB *(-1.0) + sqrt( delta ) ) / (2 *m_dCoFactorA);
			dPos = dWD;
		}
		else
			return FALSE;
	}

	VARIANT varPos = CSemBasisCtrl::GetPos();
	if ( dPos != varPos.dblVal )	// 只要WD变化，就统调MagMin
	{
		varPos.dblVal = dPos;
		varPos = SetPos( varPos );
		if( m_bReady && dPos >= m_scfCtrl.varMin.dblVal && dPos <= m_scfCtrl.varMax.dblVal )
		{
			m_pMag->Co_UpdatePosByWD( dWD );
			char szBuf[255];
			sprintf( szBuf, "WD = %.2f\r\n", dWD );
			OutputDebugString( szBuf );
		}

		int nCoHVstage = 0;
		if( dPos < 7.5 )
			nCoHVstage = 0;
		else if( dPos < 12.5 )
			nCoHVstage = 1;
		else if( dPos < 17.5 )
			nCoHVstage = 2;
		else if( dPos < 22.5 )
			nCoHVstage = 3;
		else if( dPos < 27.5 )
			nCoHVstage = 4;
		else
			nCoHVstage = 5;
		if( m_nCoHVstage != nCoHVstage )
		{
			// 通知物镜，要重新加载高压统调系数
			m_nCoHVstage = nCoHVstage;
			m_pOL->SetParams( SET_PARAMS_Rate, m_dCurrentHV );
		}
	}
	return TRUE;
}

double CSemCtrl_WD::GetOLValue( double dWD )
{
	// 通常不使用
	// 在第三方软件设置工作距离时使用：需要反向计算物镜值
	double dOL = 1.0;
	if( m_nType > 1)
	{
		// 只考虑6900的情况
		// OLdisp3X = 1 / sqrt( a *WD *WD + b *WD + c)
		// a *WD *WD + b *WD +c = 1 / (OL*OL)
		// a *WD *WD + b *WD +C = 0
		// WD = ( -b + sqrt( b*b - 4*a*C) ) / (2*a)
		dOL = 1 / sqrt( m_dCoFactorA *dWD *dWD +m_dCoFactorB *dWD +m_dCoFactorC );
	}
	return dOL;
}

BOOL CSemCtrl_WD::CoherenceCheck()
{
	BOOL blRet = TRUE;

	if ( m_scfCtrl.varMax.dblVal < m_scfCtrl.varMin.dblVal )
	{
		m_scfCtrl.varMax.dblVal = m_scfCtrl.varMin.dblVal;
		blRet = FALSE;
	}

	if ( m_scfCtrl.varPos.dblVal > m_scfCtrl.varMax.dblVal )
	{
		m_scfCtrl.varPos.dblVal = m_scfCtrl.varMax.dblVal;
		blRet = FALSE;
	}

	if ( m_scfCtrl.varPos.dblVal < m_scfCtrl.varMin.dblVal )
	{
		m_scfCtrl.varPos.dblVal = m_scfCtrl.varMin.dblVal;
		blRet = FALSE;
	}

	return blRet;
}

const CString& CSemCtrl_WD::FormatText()
{
	VARIANT varPos = CSemBasisCtrl::GetPos();
	m_strText.Format( "%2.1f mm", varPos.dblVal );

/*	varPos = GetPos();
	long lWD = varPos.lVal;
	long lWD = (long)m_nWDIndexToWD[ lWDIndex ];
	m_strText.Format( "%2.1f mm", (float)lWD/10.0 );
*/
	return m_strText;
}

const CString& CSemCtrl_WD::FormatText( VARIANT varPos )
{
	static CString strText;

	strText.Format( "%2.1f mm", varPos.dblVal );

	return strText;
}

void CSemCtrl_WD::SetParams( int nType, double dParam )// Type:类型；Param:要设置的参数值
{
	switch( nType )
	{
	case	11:
		m_dCoFactorA = dParam;
		break;
	case	12:
		m_dCoFactorB = dParam;
		break;
	case	13:
		m_dCoFactorC = dParam;
		break;

	case	101:	// SemType
		m_nType	= (int)dParam;
		if( m_nType == 12 )
			m_scfCtrl.varPos.dblVal = (DOUBLE)15.0;
		break;

	case	201:
		// 高压值改变时，由物镜告诉WD，当前高压值是多少
		m_dCurrentHV = dParam;
		break;
	default:
		break;
	}
}

void CSemCtrl_WD::GetParams( int nType, double& dParam )
{
	switch( nType )
	{
	case	0:
		{
			// 根据WD值反算OL显示值
			double dWD = dParam;
			dParam = GetOLValue( dWD );
		}
		break;

	case	1:
		// 告诉物镜，当前WD值是多少，看看是否需要更新高压统调系数
		{
			VARIANT varPos = CSemBasisCtrl::GetPos();
			dParam = varPos.dblVal;
		}
		break;
	}
}

