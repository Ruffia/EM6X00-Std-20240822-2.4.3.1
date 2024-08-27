#pragma once
#include "StatusDetector.h"
#include "USB_Functions.h"

class AFX_EXT_CLASS CCPS3603tatusDetector : public CStatusDetector
{
protected:
	CCPS3603tatusDetector(void);
	~CCPS3603tatusDetector(void){};

public:
	static CCPS3603tatusDetector& Instance();

	void Init( HWND hWnd,int nPort );
	virtual BOOL Start();

	virtual void Release();
	virtual DWORD DoJob();

	void SetParam( int nIndex, int nValue );
	void GetParam( int nIndex, int& nValue );
	BOOL IsReady();

protected:
	void CommWith3603();

	void InitParam();
	int error(char *fmt, ... );

	void readini(void);
	void readinipar(char *buf, char *name, double *par);
	void writeini(void);
	void logprintf(char *fmt, ... );

	void ChangeAcc( int nPos, BOOL bHScroll = TRUE  );
	void ChangeBias( int nPos, BOOL bHScroll  = TRUE );
	void ChangeFilament( int nPos, BOOL bHScroll  = TRUE );
	BOOL DeviceOpen();
	BOOL DeviceClose();

	void Auto_Rise_Acc1();
	void Auto_Rise_Acc2();
	void Auto_Rise_Bias();
	void Auto_Rise_Fila();
	void AutoProc();
	void StepAcc();
	void StepFila2();
	void StepFila1();
	BOOL InitDevice( HWND pWnd, int nPort );
	BOOL CloseDevice();

protected:
	HWND      m_hWnd;

	CPS3603_Param		g_3603;
	CPS3603_Status		g_3603s;
	CPS3603_AutoState	g_AutoState;
	CCriticalSection	m_mutexPort;

	HANDLE	g_hFile3603;
	int		g_nLoops3603;
	BOOL	g_bReady;
	DWORD	g_lastMon, g_lastAuto, g_lastStep;

	double	g_dblAutoStep_Accelerator1;
	double	g_dblAutoStep_Accelerator2;
	double	g_dblAutoRated_Accelerator1;
	double	g_dblAutoRated_Accelerator2;
	double	g_dblAutoStep_Bias;
	double	g_dblAutoRated_Bias;
	double	g_dblAutoStep_Fila;// 100mA
	double	g_dblAutoStep_Fila2	;// 10mA
	double	g_dblAutoRated_Fila;	// 1.9A

	double	g_dblAutoBias_Min;
	double	g_dblAutoBias_Max;
	BOOL	g_bAutoBias;
	DWORD	g_lastAutoBias;

	// 2018.03 当设置值大时，后台一步一步加
	BOOL	g_bStepAcc, g_bStepBias, g_bStepFila;
	double	g_dblGoalAcc, g_dblGoalBias, g_dblGoalFila;
	BOOL	g_bLookForSaturationPoint;


	//int		m_nConnectok_a, m_nConnectok_e, m_nConnectok_f, m_nRemoteok;	// connection ok flags
	int		m_nStop, m_nPause, m_nUsewd;	// force to stop, no attempts to retry

	int		m_nDebugmode;			// debug mode: if disabled, will try to
	// restore communication instead error messages
	double	m_dVset_a, m_dVset_e, m_dVset_f;

	int		m_nCycltestkey;			// Use cyclic test

	///////////// device specific parameters //////////////
	double	m_dVscale_a_abs;		// absolut v scale, setpoint, Volts
	double	m_dVscale_a_s;			// output v scale, setpoint, Volts
	double	m_dVoffset_a_s;			// output v offset, setpoint, Volts

	double	m_dVscale_a;			// output v scale, meter, Volts
	double	m_dVoffset_a;			// output v offset, meter, Volts
	double	m_dIscale_a;			// output i scale, meter, Ampers
	double	m_dIoffset_a;			// output i offset, meter, Ampers

	double	m_dVscale_e_abs;		// absolut v scale, setpoint, Volts
	double	m_dVscale_e_s;			// output v scale, setpoint, Volts
	double	m_dVoffset_e_s;			// output v offset, setpoint, Volts
	double	m_dVscale_e_s_r;		// output r=v/i ratio

	double	m_dVscale_e;			// output v scale, meter, Volts
	double	m_dVoffset_e;			// output v offset, meter, Volts
	double	m_dIscale_e;			// output i scale, meter, Ampers
	double	m_dIoffset_e;			// output i offset, meter, Ampers

	double	m_dIscale_f_abs;		// absolut i scale, setpoint, Ampers
	double	m_dIscale_f_s;			// output i scale, setpoint, Ampers
	double	m_dIoffset_f_s;			// output i offset, setpoint, Ampers

	double	m_dIscale_f;			// output i scale, meter, Ampers
	double	m_dIoffset_f;			// output i offset, meter, Ampers
	double	m_dVscale_f;			// output v scale, meter, Volts
	double	m_dVoffset_f;			// output v offset, meter, Volts

	double	m_dShutdowntemp;		// shutdown temperature, C
	int		m_nShutdowndelay;		// shutdown delay, s
	int		m_nUseshutdown;			// use shutdown if !=0

	int		m_nRangeMax_a;			// number of slider's positions
	int		m_nRangeMax_e;			// number of slider's positions
	int		m_nRangeMax_f;			// number of slider's positions

	int		m_nstsError;			// error status, returned by last call of a C3601_Get/Setxxx()
	int		m_nstsDevice;			// device status, returned by last call of C3601_GetStatusFlags()
	int		m_nstsFault;			// device fault status, returned by last call of C3601_GetFaultFlags()

	int		PASSIVE_BIAS;			// bias output regulator is active (0) or passive (1)
	int		VOLTAGE_CONTROL_F;		// filament control uses current (0) or voltage (1)
	int		EXTRA_PARAMS;			// show extra parameters in control window (1)
	int		RESISTOR_CONTROL;		// bias control uses voltage reg. (0) or variable resistor (1)
	int		SEPARATED_FEEDBACK;		// accelerator has a single HV connetion (0) or separated Out and Feedback (1)

	// Limits
	double	m_dMinimumAcceleratorSet;
	double	m_dMaximumAcceleratorSet;

	double	m_dMinimumBiasSet;
	double	m_dMaximumBiasSet;

	double	m_dMinimumFilamentSet;
	double	m_dMaximumFilamentSet;
	///////////// device specific parameters //////////////

	int		m_noldalarmlevel;
	double	m_dvavr, m_diavr, m_dtavr;
	int		m_ntickcnt;
	BOOL	m_bnewlog;
	int		m_nstopdealy;

	BOOL	m_bCheckUseA, m_bCheckUseE, m_bCheckUseF;
	BOOL	m_bEnA, m_bEnE, m_bEnF;
	BOOL    m_bNotify2FilamentCurrentContorl;   //通知灯丝电流控件UI
	BOOL    m_bNotify2HVContorl;                //通知高压控件UI

	char workdir[256];
	char ininame[256];
	char AppName[256];
};
