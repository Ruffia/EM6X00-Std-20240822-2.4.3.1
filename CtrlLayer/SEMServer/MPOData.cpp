#include "StdAfx.h"
#include "MPOData.h"

CMPOData::CMPOData()
{
	m_dSpeed	= 1.0;
	m_dMag		= 0.0;
	m_dWD		= 0.0;
	m_dBrt		= 0.0;
	m_dBrt2		= 0.0;
	m_dCst		= 0.0;
	m_dAcc		= 0.0;
	m_dFila		= 0.0;
	m_dBias		= 0.0;
	m_dEmi		= 0.0;
	m_dEmiMon	= 0.0;

	m_nScanRotAngle	= 0;
	m_nVacStatus	= 0;
	m_nGunStatus	= 0;
	m_nStageStatus	= 0;
	m_nIsStopScan	= 0;

	int i, j;
	for( i=0; i<2; i++ )
		m_dMagRange[i] = 0.0;

	for( i=0; i<3; i++ )
		m_dFOV[i] = 0.0;

	for( i=0; i<5; i++ )
		m_dStagePos[i] = 0.0;

	for( i=0; i<2; i++ )
	{
		for( j=0; j<5; j++ )
			m_dStageRange[i][j] = 0.0;
	}

	for( i=0; i<2; i++ )
	{
		m_nStig[i] = 0;
		m_nReso[i] = 0;
	}
}

CMPOData::~CMPOData()
{

}


CMPOData& CMPOData::Instance()
{
	static CMPOData workQueue;
	return workQueue;
}

