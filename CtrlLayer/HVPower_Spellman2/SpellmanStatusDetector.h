#pragma once
#include "StatusDetector.h"
#include "USB_Functions.h"

#define		BUFFER_LEN	65

class AFX_EXT_CLASS CSpellmanStatusDetector : public CStatusDetector
{
protected:
	CSpellmanStatusDetector(void);
	~CSpellmanStatusDetector(void){};

public:
	static CSpellmanStatusDetector& Instance();

	void	Init( HWND hWnd );
	virtual	BOOL	Start();

	virtual	void	Release();
	virtual	DWORD	DoJob();

	void	SetParam( int nIndex, int nValue );

protected:
	CString	HID_OpenHIDDevice();
	BOOL	HID_InitHIDDevice( CString strDeviceName );
	void	HID_CloseHIDDevice();

	void    Message();
	void    Message2();

	int     ConvertReceiveData( CString strData );
	void    ConvertCommand( CString strCmd );
	BOOL    WriteToDevice();
	CString ReadFromDevice();

	void    ReadRampRate( int nIndex, int nSleep );
	void    ReadRampRates( int nSleep );

	void    SetValue( int nIndex, int nValue );
	void    SetRampRates( int nIndex, int nValue );
	void    SetEnquiry( int nIndex, int nSleep );
	int     SetEnquiryStatus( int nSleep );
	int     SetEnquiryID();
	int     SetEnquiryVer();
	void    SetSwitch( int nIndex, int nSwitch );

	void    ClearSwitches();
	void    OpenSwitches();
	void    Unlock5KV();
	void    Reset771();
	void    CommWithSpellman( int nSleep );

	void    Auto_Rise_EHT1();
	void    Auto_Rise_EHT2();
	void    Auto_Rise_Heater();
	void    Auto_Rise_Beam();
	void	Auto_Rise_WaitEMI();
	void    AutoProc();

	void    StepAcc();
	void    StepFila2();
	void    StepFila1();
	void    SetSteps( int nIndex, int nValue );

protected:
	HWND      m_hWnd;
	HANDLE	  g_hEvent;

	DebugParam_Spellman g_Spellmans;
	Spellman_AutoState	g_AutoState;

	BOOL	g_bStepAcc, g_bStepBias, g_bStepFila;
	double	g_dblGoalAcc, g_dblGoalBias, g_dblGoalFila;
	DWORD	g_lastStep;
	BOOL    m_bNotify2FilamentCurrentContorl;   //通知灯丝电流控件UI
	BOOL    m_bNotify2HVContorl;                //通知高压控件UI

	double	g_dblAutoStep_EHT1;		// 500;
	double	g_dblAutoStep_EHT2;		// 1000;
	double	g_dblAutoRated_EHT1;	// 10000;
	double	g_dblAutoRated_EHT2;	// 30000;
	double	g_dblAutoStep_Beam;
	double	g_dblAutoRated_Beam;
	double	g_dblAutoStep_Heater1;	// 100mA
	double	g_dblAutoStep_Heater2;	// 10mA
	double	g_dblAutoRated_Heater;	// 1.9A

	HANDLE	g_hFile;
	int		g_nLoops;
	CString	g_strSpellman, g_strSpellmanSaved;

	//BOOL	g_bReady;			// 是否至少有一个设备联通
	BOOL	g_bPauseComm;		// 是否暂停与设备通讯
	BOOL	g_bPausePost;		// 是否暂停上送监测值（但不停止与设备通讯）
	BOOL	g_bPauseInquire;
	DWORD	g_lastMon, g_lastAuto;

	CString		g_strDeviceName;			// The system name for the remote button device
	BOOL		g_bOpenned;					// Is the ButtonsAndLights device operational?

	HANDLE		g_hWrite;					// Needed to write to a system device
	HANDLE		g_hRead;					// Needed to read a system device
	OVERLAPPED	g_overlapRead;

	BYTE		g_bufferRead[BUFFER_LEN];	// Data read from ButtonsAdnLights device, changed to 8 by DD 6-24-10
	BYTE		g_bufferWrite[BUFFER_LEN];	// Data written to ButtonsAndLights device

	double		g_dConstStep_PMT;
	double		g_dConstStep_II;
	double		g_dConstStep_Heater;
	double		g_dConstStep_EHT;
	double		g_dConstStep_Col;
	double		g_dConstStep_Scint;
	double		g_dConstStep_PMT3;
};
