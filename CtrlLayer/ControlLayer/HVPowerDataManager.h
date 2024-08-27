#pragma once
#include "USB_Functions.h"

class AFX_EXT_CLASS CHVPowerDataManager
{
public:
	static	CHVPowerDataManager& Instance();
	
	void	ResetCPS3603Param();
	void	ResetCPS3603Status();

	void	ResetSpellmanDebugParam();
	void	HVPower_SetEnable_HVType( int nCheck, int nID, USB_ScanType eScanType, BOOL blOverhaulState );
	void	VAC_Reset_ByHVType( const USB_ScanType scanType );

	void	HVPower_Init(CWnd* pWnd);
	void	HVPower_Init_ByHVType(CWnd* pWnd);
	void	HVPower_SetEnable( int nID, int nCheck, const USB_ScanType scanType, BOOL blOverhaulState ); 
	CString	HVPower_UpdateHVdisp();

	void	HV_UpdateStatusSwitches(const BYTE bSSbak );
	void	ChangeHighVoltage_771( BOOL bNotify, VARIANT ClassID, VARIANT SerialNumber, VARIANT &Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSet, BOOL bSync, USB_ScanType eScanType );
	void	ChangeHighVoltage_3603( BOOL bNotify, VARIANT ClassID, VARIANT SerialNumber, VARIANT &Xpos, VARIANT Ypos, VARIANT Flag, BOOL bSet, USB_ScanType eScanType ); 

	void	CFGFile_InitCPS3603s( char * path ); 

protected:
	CHVPowerDataManager();
	void	_InitSpellmanParamater();

public:
	DebugParam_Spellman m_Spellmans;

	CPS3603_Param	m_CPS3603;
	CPS3603_Status	m_CPS3603s;
	double			m_dHV;//, m_dHV2, m_dHV3;	// 分别记录高压、偏压（771束流设置值）、灯丝加热电流的设置值

	DWORD	m_dwHVtype;		// 15.12 高压电源型号
};
