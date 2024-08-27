#pragma once
#include "CommFunctions.h"

class AFX_EXT_CLASS CMPOData
{
public:
	CMPOData();
	~CMPOData();

public:
	static CMPOData& Instance();
	
public:
	double	m_dSpeed;
	double	m_dMag, m_dWD, m_dBrt, m_dBrt2, m_dCst;
	double	m_dAcc, m_dFila, m_dBias, m_dEmi, m_dEmiMon;

	int     m_nScanRotAngle, m_nVacStatus, m_nGunStatus, m_nIsStopScan;
	int		m_nStageStatus;

	KYKYCommMsg_Data5f m_5fData;
	double	m_dMagRange[2], m_dFOV[3];
	double	m_dStageRange[2][5], m_dStagePos[5];
	int		m_nStig[2], m_nReso[2];
};