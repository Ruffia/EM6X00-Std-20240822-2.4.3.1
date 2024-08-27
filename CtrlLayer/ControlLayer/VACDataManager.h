#pragma once
#include "BoardVAC.h"
#include "CommBoardVAC.h"
#include "ExportBSDFunc.h"
#include "USB_Functions.h"
#include "PLCvacBrd.h"

class AFX_EXT_CLASS CVACDataManager 
{
public:
	static CVACDataManager& Instance();
	
	// 什么时候可以加高压
	BOOL  GetHVInt();

	BOOL  CheckVAC();
	BOOL  CheckVACAndCCG();
	BOOL  CheckVACConnection(CWnd* pWnd);
	void  VAC_Reset(const USB_ScanType scanType);

	void  CHECKVacuumTCc( int nCheckf, int nCheckc );
	void  CHECKVacuumTCf( int nCheckf, int nCheckc );

	void  CHECKFPGArelayEn4( int nCheck );
	void  CHECKFPGArelayEn56( int nCheck );
	void  CHECKFPGArelayEn2( int nCheck );
	void  CHECKFPGArelayEn1( int nCheck );
	void  CHECKV1status( BOOL bV1 );

	void  SetParam_Power();
	void  SetParam_Chamb();
	void  SetParam_Gun();

	void  CFGfile_InitVacParams( char* path );
	void  CFGfile_InitCCGParams( char* path ); 
	void  LoadVACFactor( char * path ); 

protected:
	CVACDataManager();

public:
	SMvac_Param				m_VacParamMain;
	SMvac_TCAmp				m_VacTCAmp;

	CfgVAC_IP_CCG			m_cfgVacCCG;
	int						m_nCCGconnects;
	DWORD	                m_dwV1CtrlMode;	// 16.07 V1阀控制方式

	CfgVAC_IP_CCG			m_cfgVacIPG;

	CfgVAC_IP_CCG			m_cfgVacIPC;

	/////////////////////////////////////////////
	// 新真空控制板
	CfgVAC_Ports			m_portBoard;
	CCommBoardVAC			m_BoardVAC;
	VACstatus6K_Board		m_statusBoard, m_statusBoardBak;
	VACvalue6K_CCG			m_valueCCG;
	VACvalue6K_TC			m_valueTC;
	VACvalue6K_Thresholds	m_thresholdsVAC;

	/////////////////////////////////////////////
	// PLC真空控制板
	Cfg_PLCvacBrd			m_cfgPLCvacBrd;
	Status_PLCvacBrd		m_StatusPLCvacBrd, m_StatusPLCvacBrdBak;
};
