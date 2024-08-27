// Comm3603.cpp : implementation file
//

#include "stdafx.h"
#include "Comm3603.h"

#include <afxmt.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD WINAPI CommThread(LPVOID lpParam);

HANDLE	g_hThread;
DWORD	g_dwThread;
HANDLE	g_hEventWork		= CreateEvent(NULL,FALSE, FALSE, _T("WORK"));
HANDLE	g_hEventTeminate	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATE"));

HWND				g_hMain;
CPS3603_Param		g_3603;
CPS3603_Status		g_3603s;
CPS3603_AutoState	g_AutoState	= AS_NULL;
CCriticalSection	m_mutexPort;

HANDLE	g_hFile3603	= NULL;
int		g_nLoops3603 = 0;
BOOL	g_bReady	= FALSE;
DWORD	g_lastMon, g_lastAuto, g_lastStep;

double	g_dblAutoStep_Accelerator1	= 500;
double	g_dblAutoStep_Accelerator2	= 1000;
double	g_dblAutoRated_Accelerator1	= 10000;
double	g_dblAutoRated_Accelerator2	= 30000;
double	g_dblAutoStep_Bias			= 10;
double	g_dblAutoRated_Bias			= 100;
double	g_dblAutoStep_Fila			= 0.100;// 100mA
double	g_dblAutoStep_Fila2			= 0.010;// 10mA
double	g_dblAutoRated_Fila			= 1.9;	// 1.9A

double	g_dblAutoBias_Min			= 130;
double	g_dblAutoBias_Max			= 170;
BOOL	g_bAutoBias					= FALSE;
DWORD	g_lastAutoBias;

// 2018.03 当设置值大时，后台一步一步加
BOOL	g_bStepAcc, g_bStepBias, g_bStepFila;
double	g_dblGoalAcc, g_dblGoalBias, g_dblGoalFila;
BOOL	g_bLookForSaturationPoint	= FALSE;

///////////////////////////////////////////////////////////
#define minmax(x,l,r)   (((x)<(l))?(l):((x)>(r))?(r):(x))

//////////////////////// Types //////////////////////
typedef enum tagAlarmtype
{
	okay,
	acc_temp,
	hv_temp,
	level
}alarmtype;
////////////////////// Function prototypes //////////////////////////

void showsts(void);
double detcorona(double i);
void alarm(alarmtype atype);

void curvtofile(void);
void logprintf(char *fmt, ... );
void readini(void);
void writeini(void);
void readinipar(char *buf, char *name, double *par);

void dlgprintf(int id, char *fmt, ... );
int error(char *fmt, ... );

////////////////////// Function prototypes //////////////////////////

void closeDlg();
void restartDlg();

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

void	ChangeAcc( int nPos, BOOL bHScroll = TRUE );
void	ChangeBias( int nPos, BOOL bHScroll = TRUE );
void	ChangeFilament( int nPos, BOOL bHScroll = TRUE );
///////////////////////////////////////////////////////////

BOOL	m_bCheckUseA, m_bCheckUseE, m_bCheckUseF;
BOOL	m_bEnA, m_bEnE, m_bEnF;

/////////////////////////////////////////////////////////////////////////////
// CComm3603

CComm3603::CComm3603()
{
	g_hMain	= NULL;
	InitParam();

	g_bStepAcc	= FALSE;
	g_bStepBias	= FALSE;
	g_bStepFila	= FALSE;
	g_dblGoalAcc	= 0.0;
	g_dblGoalBias	= 0.0;
	g_dblGoalFila	= 0.0;
}

CComm3603::~CComm3603()
{
}


///////////////////////////////////////////////////////////
char workdir[256];
char ininame[256];
char *AppName = "3603_control";

void CComm3603::InitParam()
{
	// 获取工作路径
	::GetModuleFileName(0, workdir, 255);
	LPTSTR pszFileName = _tcsrchr(workdir, '\\');
	*pszFileName = '\0';

	int err = C3601_DLLOpen(workdir);
	if( err )
	{
		error( "DLL Open error %x", err );
		return;
	}

	///////////// device specific parameters //////////////
	m_dVscale_a_abs		= 30000;	// absolut v scale, setpoint, Volts
	m_dVscale_a_s		= 30000;	// output v scale, setpoint, Volts
	m_dVoffset_a_s		= 0;		// output v offset, setpoint, Volts

	m_dVscale_a			= 30000;	// output v scale, meter, Volts
	m_dVoffset_a		= 0;		// output v offset, meter, Volts
	m_dIscale_a			= 500e-6;	// output i scale, meter, Ampers
	m_dIoffset_a		= 0;		// output i offset, meter, Ampers

	m_dVscale_e_abs		= 10000;	// absolut v scale, setpoint, Volts
	m_dVscale_e_s		= 10000;	// output v scale, setpoint, Volts
	m_dVoffset_e_s		= 0;		// output v offset, setpoint, Volts
	m_dVscale_e_s_r		= 1;		// output r=v/i ratio

	m_dVscale_e			= 10000;	// output v scale, meter, Volts
	m_dVoffset_e		= 0;		// output v offset, meter, Volts
	m_dIscale_e			= 500e-6;	// output i scale, meter, Ampers
	m_dIoffset_e		= 0;		// output i offset, meter, Ampers

	m_dIscale_f_abs		= 5;		// absolut i scale, setpoint, Ampers
	m_dIscale_f_s		= 5;		// output i scale, setpoint, Ampers
	m_dIoffset_f_s		= 0;		// output i offset, setpoint, Ampers

	m_dIscale_f			= 5;		// output i scale, meter, Ampers
	m_dIoffset_f		= 0;		// output i offset, meter, Ampers
	m_dVscale_f			= 5;		// output v scale, meter, Volts
	m_dVoffset_f		= 0;		// output v offset, meter, Volts

	m_dShutdowntemp		= 60;		// shutdown temperature, C
	m_nShutdowndelay	= 10;		// shutdown delay, s
	m_nUseshutdown		= 0;		// use shutdown if !=0

	PASSIVE_BIAS		= 0;		// bias output regulator is active (0) or passive (1)
	VOLTAGE_CONTROL_F	= 0;		// filament control uses current (0) or voltage (1)
	EXTRA_PARAMS		= 0;		// show extra parameters in control window (1)
	RESISTOR_CONTROL	= 0;		// bias control uses voltage reg. (0) or variable resistor (1)
	SEPARATED_FEEDBACK	= 1;		// accelerator has a single HV connetion (0) or separated Out and Feedback (1)

	// Limits
	m_dMinimumAcceleratorSet	= 0;
	m_dMaximumAcceleratorSet	= 50000;

	m_dMinimumBiasSet			= 500;
	m_dMaximumBiasSet			= 5000;

	m_dMinimumFilamentSet		= 0;
	m_dMaximumFilamentSet		= 2.200;
	///////////// device specific parameters //////////////

	readini();
	/////////////////////////////////////////////////////////////////

	m_nUsewd			= 1;
	m_nCycltestkey		= 0;
	m_noldalarmlevel	= -1;
	m_bnewlog			= FALSE;
	m_dVset_a = m_dVset_e = m_dVset_f = 0;

	m_bCheckUseA = m_bCheckUseE = m_bCheckUseF = TRUE;
	m_bEnA = m_bEnE = m_bEnF = FALSE;
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	g_3603s.nPort	= -1;
	g_3603s.nConnectA = g_3603s.nConnectE = g_3603s.nConnectF = 0;
	g_3603s.nVerA = g_3603s.nVerE = g_3603s.nVerF = 0;
	g_3603s.nPassiveBias		= PASSIVE_BIAS;
	g_3603s.nVoltageControlF	= VOLTAGE_CONTROL_F;
	g_3603s.nExtraParams		= EXTRA_PARAMS;
	g_3603s.nResistorControl	= RESISTOR_CONTROL;
	g_3603s.nSeparatedFeedback	= SEPARATED_FEEDBACK;

	g_3603.dVoutA = g_3603.dIoutA = 0.0;
	g_3603.dVoutE = g_3603.dIoutE = 0.0;
	g_3603.dVoutF = g_3603.dIoutF = 0.0;
	g_3603.dRf = g_3603.dPf = g_3603.dTf = 0.0;
	g_3603.dTa = g_3603.dThv = 0.0;
	g_3603.dCoronalevel = 0.0;
	/////////////////////////////////////////////////////////////////
}

// Read configuration data from the <test dir>\3601.ini file
void readini(void)
{
#define CFGLIM    10000
	char buf[CFGLIM];
	double tmp;
	FILE *f;
	
	sprintf(ininame, "%s\\3603.ini", workdir);
	if((f = fopen(ininame, "rb")) == NULL)
	{
		MessageBox(NULL, "No configuration file: default values will be set.", AppName, MB_OK | MB_ICONWARNING);
		return;
	}
	fread(buf, 1, sizeof(buf), f);
	
	buf[CFGLIM - 1] = 0;
	
	readinipar(buf, "VscaleAcceleratorAbs",  &m_dVscale_a_abs);
	
	readinipar(buf, "VscaleAcceleratorSet",  &m_dVscale_a_s);
	readinipar(buf, "VoffsetAcceleratorSet", &m_dVoffset_a_s);
	
	readinipar(buf, "VscaleAccelerator",  &m_dVscale_a);
	readinipar(buf, "VoffsetAccelerator", &m_dVoffset_a);
	readinipar(buf, "IscaleAccelerator",  &m_dIscale_a);
	readinipar(buf, "IoffsetAccelerator", &m_dIoffset_a);
	
	readinipar(buf, "VscaleBiasAbs",  &m_dVscale_e_abs);
	readinipar(buf, "VscaleBiasSet",  &m_dVscale_e_s);
	readinipar(buf, "VoffsetBiasSet", &m_dVoffset_e_s);
	readinipar(buf, "VscaleBiasSetR",  &m_dVscale_e_s_r);
	
	readinipar(buf, "VscaleBias",  &m_dVscale_e);
	readinipar(buf, "VoffsetBias", &m_dVoffset_e);
	readinipar(buf, "IscaleBias",  &m_dIscale_e);
	readinipar(buf, "IoffsetBias", &m_dIoffset_e);
	
	readinipar(buf, "IscaleFilamentAbs",  &m_dIscale_f_abs);
	readinipar(buf, "IscaleFilamentSet",  &m_dIscale_f_s);
	readinipar(buf, "IoffsetFilamentSet", &m_dIoffset_f_s);
	
	readinipar(buf, "IscaleFilament",  &m_dIscale_f);
	readinipar(buf, "IoffsetFilament", &m_dIoffset_f);
	readinipar(buf, "VscaleFilament",  &m_dVscale_f);
	readinipar(buf, "VoffsetFilament", &m_dVoffset_f);
	
	readinipar(buf, "ShutdownTemp", &m_dShutdowntemp);
	readinipar(buf, "UseLevelShutdown", &tmp); m_nUseshutdown = tmp > 0.5;
	
	readinipar(buf, "PASSIVE_BIAS", &tmp); PASSIVE_BIAS = tmp > 0.5;
	readinipar(buf, "VOLTAGE_CONTROL_F", &tmp); VOLTAGE_CONTROL_F = tmp > 0.5;
	readinipar(buf, "EXTRA_PARAMS", &tmp); EXTRA_PARAMS = tmp > 0.5;
	readinipar(buf, "RESISTOR_CONTROL", &tmp); RESISTOR_CONTROL = tmp > 0.5;
	readinipar(buf, "SEPARATED_FEEDBACK", &tmp); SEPARATED_FEEDBACK = tmp > 0.5;
	
	readinipar(buf, "MinimumAcceleratorSet",  &m_dMinimumAcceleratorSet);
	readinipar(buf, "MaximumAcceleratorSet",  &m_dMaximumAcceleratorSet);
	readinipar(buf, "MinimumBiasSet",  &m_dMinimumBiasSet);
	readinipar(buf, "MaximumBiasSet",  &m_dMaximumBiasSet);
	readinipar(buf, "MinimumFilamentSet",  &m_dMinimumFilamentSet);
	readinipar(buf, "MaximumFilamentSet",  &m_dMaximumFilamentSet);
	
	m_dMaximumAcceleratorSet = minmax(m_dMaximumAcceleratorSet, 0, m_dVscale_a_abs);
	m_dMinimumAcceleratorSet = minmax(m_dMinimumAcceleratorSet, 0, m_dMaximumAcceleratorSet);
	
	m_dMaximumBiasSet = minmax(m_dMaximumBiasSet, 0, m_dVscale_e_abs);
	m_dMinimumBiasSet = minmax(m_dMinimumBiasSet, 0, m_dMaximumBiasSet);
	
	m_dMaximumFilamentSet = minmax(m_dMaximumFilamentSet, 0, m_dIscale_f_abs);
	m_dMinimumFilamentSet = minmax(m_dMinimumFilamentSet, 0, m_dMaximumFilamentSet);
	
	fclose(f);
	
//	vscale_a_abs = (int)((vscale_a_s + 1000) / 10000) * 10000;
}

void readinipar(char *buf, char *name, double *par)
{
	char *p;
	int len, nf;
	
	len = strlen(name);
	
	do
	{
		p = strstr(buf, name);
		if(p)
		{
			nf = sscanf(p, "%*s %lf\n", par);
			if(nf == 0)
				break;
		}
		else
			break;
		buf = p + len;
	}
	while(!isspace(*(p + len)));
	
	if(p == NULL || nf != 1)
		error("No <%s> parameter found in configuration file: default value %f will be set.", name, *par);
}


void writeini(void)
{
	FILE *f;
	sprintf(ininame, "%s\\3603.ini", workdir);
	if((f = fopen(ininame, "wb")) == NULL)
		return;
	fprintf(f, "VscaleBias %lf\n", &m_dVscale_e);
	fprintf(f, "IscaleBias %lf\n", &m_dIscale_e);
	fclose(f);
}

// Printf to log file
void logprintf(char *fmt, ... )
{
	va_list argptr;
	static char str[256];
	char fname[256];
	FILE *f;

	sprintf(fname, "%s\\3601.log", workdir);
	if((f = fopen(fname, "a")) == NULL)
		return;

	va_start(argptr, fmt);
	vsprintf(str, fmt, argptr);
	fprintf(f, "%s", str);
	va_end(argptr);

	fclose(f);
}

#ifdef _MSC_VER

/*
 * vsscanf for MSC
 *
 * Written 5/2003 by <mgix@mgix.com>
 *
 * This code is in the Public Domain
 *
 */

#include <malloc.h>

int vsscanf(const char  *buffer, const char  *format, va_list argPtr)
{
	// Get an upper bound for the # of args
	size_t count = 0;
	const char *p = format;
	size_t stackSize;
	void **newStack;
	int result;
	void *savedESP;

	while(1)
	{
		char c = *(p++);
		if(c==0) break;
		if(c=='%' && (p[0]!='*' && p[0]!='%')) ++count;
	}	// Make a local stack
	stackSize = (2+count)*sizeof(void*);
	newStack = (void**)alloca(stackSize);	// Fill local stack the way sscanf likes it
	newStack[0] = (void*)buffer;
	newStack[1] = (void*)format;
	memcpy(newStack+2, argPtr, count*sizeof(void*));	// Warp into system sscanf with new stack
	_asm
	{
		mov     savedESP, esp;
		mov     esp, newStack;
		call    sscanf;
		mov     esp, savedESP;
		mov     result, eax;
	}
	return result;
}

#endif // _MSC_VER


// Error message
int error(char *fmt, ... )
{
	va_list argptr;
	static char str[256];
	int res;

	//connectok = 0;
	if(m_nDebugmode)	// no error messages in debug mode
		return 0;
	va_start(argptr, fmt);
	vsprintf(str, fmt, argptr);
	res = MessageBox(NULL, str, AppName, MB_OK | MB_ICONEXCLAMATION);
	va_end(argptr);
	return(res);
}

double detcorona(double i)
{
	static double tconst = 5, lasti, lastdi1, det;
	double nlim, plim, di1, di2, dd;
	
	if(i == 0)
		return det = 0;
	if(i == lasti)
		return det;
	
	plim = i * 0.1; nlim = -plim;
	
	di1 = i - lasti;
	if(di1 > 0 && lastdi1 > 0 || di1 < 0 && lastdi1 < 0)
	{
		//di1 = 0;
		di2 = 0;
	}
	else
	{
		if(di1 > plim) di1 = plim;
		else if(di1 < nlim) di1 = nlim;
		di2 = di1 - lastdi1;
		if(di2 > plim) di2 = plim;
		else if(di2 < nlim) di2 = nlim;
	}
	
	dd = fabs(di2) - det;
	det += dd / tconst;
	
	lasti = i;
	lastdi1 = di1;
	
	return det;
}
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
void ChangeAcc( int nPos, BOOL bHScroll )
{
// 	double dv = nPos * m_dVscale_a_abs / m_nRangeMax_a;
// 	if(dv != m_dVset_a)
// 		m_dVset_a = minmax(dv, m_dMinimumAcceleratorSet, m_dMaximumAcceleratorSet);
	
	int nuse_a;
#if(USERMODE)
	nuse_a = 1;
#else
	nuse_a = m_bCheckUseA;
#endif

	m_mutexPort.Lock();
	if( nuse_a )
	{
		double dvsout_a = 0.0;
		int nenab_a = 0;
		if(m_bEnA)
		{
			dvsout_a = m_dVset_a;
			nenab_a	= 1;
		}
		else
			dvsout_a = 0;
		C3601_SetValue(g_3603s.nPort, DEV_A, (dvsout_a + m_dVoffset_a_s) / m_dVscale_a_s, nenab_a);
	}
	m_mutexPort.Unlock();
}

void ChangeBias( int nPos, BOOL bHScroll )
{
// 	double dv = nPos * m_dVscale_e_abs / m_nRangeMax_e;
// 	if(dv != m_dVset_e)
// 		m_dVset_e = minmax(dv, m_dMinimumBiasSet, m_dMaximumBiasSet);

	int nuse_e;
#if(USERMODE)
	nuse_e = 1;
#else
	nuse_e = m_bCheckUseE;
#endif

	m_mutexPort.Lock();
	if(nuse_e)
	{
		double dvsout_e = 0.0;
		if( PASSIVE_BIAS )
			dvsout_e = m_dVset_e;
		else
		{
			if(m_bEnE)
				dvsout_e = m_dVset_e;
			else
				dvsout_e = 0;
		}

		if( RESISTOR_CONTROL )
			C3601_SetValue(g_3603s.nPort, DEV_E, (dvsout_e + m_dVoffset_e_s) / m_dVscale_e_s / m_dVscale_e_s_r, m_bEnE || m_bEnF);
		else
			C3601_SetValue(g_3603s.nPort, DEV_E, (dvsout_e + m_dVoffset_e_s) / m_dVscale_e_s, m_bEnE || m_bEnF);
	}
	m_mutexPort.Unlock();
}

void ChangeFilament( int nPos, BOOL bHScroll )
{
// 	double dv = nPos * m_dIscale_f_abs / m_nRangeMax_f;
// 	if(dv != m_dVset_f)
// 		m_dVset_f = minmax(dv, m_dMinimumFilamentSet, m_dMaximumFilamentSet);

	int nuse_f;
#if(USERMODE)
	nuse_f = 1;
#else
	nuse_f = m_bCheckUseF;
#endif

	m_mutexPort.Lock();
	if(nuse_f)
	{
		double dvsout_f = 0.0;
		if(m_bEnF)
			dvsout_f = m_dVset_f;
		else
			dvsout_f = 0;
		C3601_SetValue(g_3603s.nPort, DEV_F, (dvsout_f + m_dIoffset_f_s) / m_dIscale_f_s, m_bEnE || m_bEnF);
	}
	m_mutexPort.Unlock();
}
///////////////////////////////////////////////////////////

BOOL DeviceOpen()
{
	m_mutexPort.Lock();
	// 打开端口
	g_3603s.nConnectA = g_3603s.nConnectE = g_3603s.nConnectF = !C3601_OpenPort(g_3603s.nPort, DEF_BAUD_RATE);

	if(g_3603s.nConnectA)	// try to communicate
	{
		g_3603s.nVerA = C3601_GetVersion(g_3603s.nPort, DEV_A);
		if(g_3603s.nVerA <= 0)
			g_3603s.nConnectA = 0;
	}
	if(g_3603s.nConnectE)	// try to communicate
	{
		g_3603s.nVerE = C3601_GetVersion(g_3603s.nPort, DEV_E);
		if(g_3603s.nVerE <= 0)
			g_3603s.nConnectE = 0;
	}
	if(g_3603s.nConnectF)	// try to communicate
	{
		g_3603s.nVerF = C3601_GetVersion(g_3603s.nPort, DEV_F);
		if(g_3603s.nVerF <= 0)
			g_3603s.nConnectF = 0;
	}

	m_nRangeMax_a = (int)(m_dVscale_a_abs / 500);		// 500V fixed clicks

	if(m_dVscale_e_abs < 1500)
		m_nRangeMax_e = (int)(m_dVscale_e_abs / 10);	// 10V fixed clicks
	else if(m_dVscale_e_abs < 2500)
		m_nRangeMax_e = (int)(m_dVscale_e_abs / 20);	// 20V fixed clicks
	else if(m_dVscale_e_abs < 3500)
		m_nRangeMax_e = (int)(m_dVscale_e_abs / 25);	// 25V fixed clicks
	else if(m_dVscale_e_abs < 5000)
		m_nRangeMax_e = (int)(m_dVscale_e_abs / 50);	// 50V fixed clicks
	else
		m_nRangeMax_e = (int)(m_dVscale_e_abs / 100);	// 100V fixed clicks

	m_nRangeMax_f = (int)(m_dIscale_f_abs / 0.1);		// 0.1A fixed clicks

	C3601_Reset(g_3603s.nPort, DEV_A, 0);
	C3601_Reset(g_3603s.nPort, DEV_E, 0);
	C3601_Reset(g_3603s.nPort, DEV_F, 0);

	Sleep(1000);

	C3601_SetValue(g_3603s.nPort, DEV_A, 0, 0);
	C3601_SetValue(g_3603s.nPort, DEV_E, 0, 0);
	C3601_SetValue(g_3603s.nPort, DEV_F, 0, 0);

	if(g_3603s.nConnectA)
		C3601_SetWD(g_3603s.nPort, DEV_A, m_nUsewd ? 1000 : 0);
	if(g_3603s.nConnectE)
		C3601_SetWD(g_3603s.nPort, DEV_E, m_nUsewd ? 1000 : 0);
	if(g_3603s.nConnectF)
		C3601_SetWD(g_3603s.nPort, DEV_F, m_nUsewd ? 1000 : 0);
	m_mutexPort.Unlock();

	PostMessage( g_hMain, WM_USER_CPS_Status, 0x02, (LPARAM)( &g_3603s ) );

	g_AutoState	= AS_NULL;
	g_bReady = ( g_3603s.nConnectA || g_3603s.nConnectE || g_3603s.nConnectF );
	if( g_bReady )
	{
		g_lastAuto	= GetTickCount();
		g_lastMon	= GetTickCount();
		/////////////////////////////////////////////
		// 设置日志
		TCHAR exeFullPath[255];
		::GetModuleFileName( NULL, exeFullPath, 255 );
		char *ptemp = strrchr( exeFullPath, '\\');
		ptemp[1] = 0;
		
		CString str, strTemp;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d%H%M");
		// 设置3603日志文件
		str.Format( "%s\\Log\\3603-%s.csv", exeFullPath, strTemp );
		if( g_hFile3603 != NULL )
			CloseHandle( g_hFile3603 );
		g_hFile3603	= NULL;
		g_hFile3603 = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		g_nLoops3603 = 1;
		// 写IPG日志文件头
		if( g_hFile3603 != NULL )
		{
			unsigned long cbRet;
			char Buf[255];
			str.Format( "\t%s", "CPS3603" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFile3603, Buf, strlen(Buf), &cbRet, NULL );
			if( g_3603s.nConnectA )
			{
				str = "Accelerator is connected";
				sprintf( Buf, "%s\r\n", str );
				WriteFile( g_hFile3603, Buf, strlen(Buf), &cbRet, NULL );
			}
			if( g_3603s.nConnectE )
			{
				str = "Bias is connected";
				sprintf( Buf, "%s\r\n", str );
				WriteFile( g_hFile3603, Buf, strlen(Buf), &cbRet, NULL );
			}
			if( g_3603s.nConnectF )
			{
				str = "Filament is connected";
				sprintf( Buf, "%s\r\n", str );
				WriteFile( g_hFile3603, Buf, strlen(Buf), &cbRet, NULL );
			}
//			str.Format( "HH:MM:SS\tAcc set\tAcc V\tAcc I\tCorona\tFila set\tFila V\tFila I\tBias set\tBias V\tBias I\tP\tR\tT");
			str.Format( "HH:MM:SS,Acc set,Acc V,Acc I,Corona,Fila set,Fila V,Fila I,Bias set,Bias V,Bias I,P,R,T");
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFile3603, Buf, strlen(Buf), &cbRet, NULL );
		}
		/////////////////////////////////////////////
	}
	return TRUE;
}

BOOL DeviceClose()
{
	m_mutexPort.Lock();
	/////////////////////////////////////////////
	if(g_3603s.nConnectA)
	{
		C3601_SetValue(g_3603s.nPort, DEV_A, 0, 0);
		g_3603s.nConnectA = 0;
	}
	if(g_3603s.nConnectE)
	{
		C3601_SetValue(g_3603s.nPort, DEV_E, 0, 0);
		g_3603s.nConnectE = 0;
	}
	if(g_3603s.nConnectF)
	{
		C3601_SetValue(g_3603s.nPort, DEV_F, 0, 0);
		g_3603s.nConnectF = 0;
	}
	C3601_ClosePort(g_3603s.nPort);
	/////////////////////////////////////////////
	m_mutexPort.Unlock();

	if( g_hFile3603 != NULL )
		CloseHandle( g_hFile3603 );
	g_hFile3603	= NULL;

	return TRUE;
}

void CommWith3603()
{
	if( !g_bReady )
		return;

//	double dv, di, dv_a, di_a, dt_a = 0, dt_hv = 0;
//	double dvsout_a, dvsout_e, dvsout_f;
//	double dv_e, di_e;
	double di_e, di_a;

	int nuse_a, nuse_e, nuse_f;
#if(USERMODE)
	nuse_a = nuse_e = nuse_f = 1;
#else
	nuse_a = m_bCheckUseA;
	nuse_e = m_bCheckUseE;
	nuse_f = m_bCheckUseF;
#endif

	m_mutexPort.Lock();
	if(nuse_e)
	{
		g_3603.dVoutE = C3601_GetVoltage(g_3603s.nPort, DEV_E, &m_nstsError) * m_dVscale_e - m_dVoffset_e;
		g_3603.dIoutE = C3601_GetCurrent(g_3603s.nPort, DEV_E, &m_nstsError) * m_dIscale_e - m_dIoffset_e;
		if( PASSIVE_BIAS )
			di_e = g_3603.dIoutE;
		g_3603.dThv = C3601_GetTempearture(g_3603s.nPort, DEV_E, &m_nstsError);
	}
	
	if(nuse_a)
	{
		g_3603.dVoutA = C3601_GetVoltage(g_3603s.nPort, DEV_A, &m_nstsError) * m_dVscale_a - m_dVoffset_a;
		di_a = g_3603.dIoutA = C3601_GetCurrent(g_3603s.nPort, DEV_A, &m_nstsError) * m_dIscale_a - m_dIoffset_a;
		g_3603.dCoronalevel = detcorona(g_3603.dIoutA) * 1e9;	// full scale (1000 units) is 1 uA
		g_3603.dTa = C3601_GetTempearture(g_3603s.nPort, DEV_A, &m_nstsError);
	}
	
	if(nuse_f)
	{
		double rf;
		
		g_3603.dVoutF = C3601_GetVoltage(g_3603s.nPort, DEV_F, &m_nstsError) * m_dVscale_f - m_dVoffset_f;
		g_3603.dIoutF = C3601_GetCurrent(g_3603s.nPort, DEV_F, &m_nstsError) * m_dIscale_f - m_dIoffset_f;
		g_3603.dPf = g_3603.dVoutF *g_3603.dIoutF;
		if(g_3603.dIoutF > 10e-3 && g_3603.dVoutF > 1e-3)
		{
			g_3603.dRf = g_3603.dVoutF / g_3603.dIoutF;
			rf = g_3603.dRf - 0.097;
			g_3603.dTf = (g_3603.dRf / 0.22 - 1) / 0.0046 + g_3603.dThv;
		}
		else
		{
			g_3603.dRf = 0;
			g_3603.dTf = g_3603.dThv;
		}
		
		if(!nuse_e)
			g_3603.dThv = C3601_GetTempearture(g_3603s.nPort, DEV_E, &m_nstsError);
	}
	// 自动偏压调节
	if( !g_bAutoBias && g_3603.dVoutA > 200 && g_3603.dIoutF > 1.9 )
	{
		// 有高压、有束流时，启动自偏压调节
		g_bAutoBias = TRUE;
		g_lastAutoBias = GetTickCount();
	}
	else if( g_3603.dVoutA < 200 || g_3603.dIoutF < 1.9 )
		g_bAutoBias = FALSE;
	m_mutexPort.Unlock();

	PostMessage( g_hMain, WM_USER_CPS_Param, (WPARAM)( &g_3603 ), 0 );
	PostMessage( g_hMain, WM_USER_CPS_MSG, 0, m_nstsError );

	if( g_hFile3603 != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		CString str;
		CTime	timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime = timeCurrent.Format("%Y/%m/%d %H:%M:%S");
//		str.Format( "[%s]\t%9.3lf\t%9.3lf\t%7.3lf\t%9.3lf\t%6.3lf\t%6.3lf\t%6.3lf\t%8.3lf\t%8.3lf\t%8.3lf\t%6.3lf\t%6.3lf\t%8.3lf",
		str.Format( "[%s],%9.3lf,%9.3lf,%7.3lf,%9.3lf,%6.3lf,%6.3lf,%6.3lf,%8.3lf,%8.3lf,%8.3lf,%6.3lf,%6.3lf,%8.3lf",
					csCurrentTime,
					m_dVset_a,							// Acc set
					g_3603.dVoutA, g_3603.dIoutA * 1e6,	// Acc
					g_3603.dCoronalevel,
					m_dVset_f,							// Fila set
					g_3603.dVoutF, g_3603.dIoutF,		// Fila
					m_dVset_e,							// Bias set
					g_3603.dVoutE, g_3603.dIoutE * 1e6,	// Bias
					g_3603.dPf, g_3603.dRf, g_3603.dTf);
		sprintf( Buf, "%s\r\n", str );
		WriteFile( g_hFile3603, Buf, strlen(Buf), &cbRet, NULL );

		g_nLoops3603++;
		if( g_nLoops3603 > 10000 )
		{
			CloseHandle( g_hFile3603 );
			// 设置日志文件
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			char *ptemp = strrchr( exeFullPath, '\\');
			ptemp[1] = 0;
			CTime	timeCurrent = CTime::GetCurrentTime();
			csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
			str.Format( "%s\\Log\\3603-%s.csv", exeFullPath, csCurrentTime );
			g_hFile3603 = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoops3603 = 1;
			
			str.Format( "\t%s", "CPS3603" );
			sprintf( Buf, "%s\n", str );
			WriteFile( g_hFile3603, Buf, strlen(Buf), &cbRet, NULL );
//			str.Format( "HH:MM:SS\tAcc set\tAcc V\tAcc I\tCorona\tFila set\tFila V\tFila I\tBias set\tBias V\tBias I\tP\tR\tT");
			str.Format( "HH:MM:SS,Acc set,Acc V,Acc I,Corona,Fila set,Fila V,Fila I,Bias set,Bias V,Bias I,P,R,T");
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFile3603, Buf, strlen(Buf), &cbRet, NULL );
		}
	}
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Auto Functions
void Auto_Rise_Acc1()
{
	m_dVset_a += g_dblAutoStep_Accelerator1;
	if( m_dVset_a >= g_dblAutoRated_Accelerator2 )
	{
		m_dVset_a = g_dblAutoRated_Accelerator2;
		ChangeAcc(0);
		PostMessage( g_hMain, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)m_dVset_a );
		g_AutoState = AS_Rise_Bias;
	}
	else if( m_dVset_a >= g_dblAutoRated_Accelerator1 )
	{
		m_dVset_a = g_dblAutoRated_Accelerator1;
		ChangeAcc(0);
		PostMessage( g_hMain, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)m_dVset_a );
		g_AutoState = AS_Rise_Acc2;
	}
	else
	{
		ChangeAcc(0);
		PostMessage( g_hMain, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)m_dVset_a );
	}
}

void Auto_Rise_Acc2()
{
	m_dVset_a += g_dblAutoStep_Accelerator2;
	if( m_dVset_a >= g_dblAutoRated_Accelerator2 )
	{
		m_dVset_a = g_dblAutoRated_Accelerator2;
		ChangeAcc(0);
		PostMessage( g_hMain, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)m_dVset_a );
		g_AutoState = AS_Rise_Bias;
	}
	else
	{
		ChangeAcc(0);
		PostMessage( g_hMain, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)m_dVset_a );
	}
}

void Auto_Rise_Bias()
{
	m_dVset_e += g_dblAutoStep_Bias;
	if( m_dVset_e >= g_dblAutoRated_Bias )
	{
		m_dVset_e = g_dblAutoRated_Bias;
		ChangeBias(0);
		PostMessage( g_hMain, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)m_dVset_e );
		g_AutoState = AS_Rise_Fila;
	}
	else
	{
		ChangeBias(0);
		PostMessage( g_hMain, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)m_dVset_e );
	}
}

void Auto_Rise_Fila()
{
	m_dVset_f += g_dblAutoStep_Fila;
	if( m_dVset_f >= g_dblAutoRated_Fila )
	{
		m_dVset_f = g_dblAutoRated_Fila;
		ChangeFilament(0);
		PostMessage( g_hMain, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)(m_dVset_f*1000) );
		g_AutoState = AS_NULL;	// 结束
		PostMessage( g_hMain, WM_USER_CPS_AutoParam, g_AutoState, 0 );
	}
	else
	{
		ChangeFilament(0);
		PostMessage( g_hMain, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)(m_dVset_f*1000) );
	}
}

void AutoProc()
{
	DWORD thisTickCount = GetTickCount();
	// 灯丝1
	switch( g_AutoState )
	{
		/////////////////////////////////////////
		// Auto Rise
		/////////////////////////////////////////
	case	AS_Rise_Acc1:
		if( thisTickCount - g_lastAuto > 200 )
		{
			Auto_Rise_Acc1();
			g_lastAuto = GetTickCount();
		}
		break;
	case	AS_Rise_Acc2:
		if( thisTickCount - g_lastAuto > 400 )
		{
			Auto_Rise_Acc2();
			g_lastAuto = GetTickCount();
		}
		break;
	case	AS_Rise_Bias:
		if( thisTickCount - g_lastAuto > 400 )
		{
			Auto_Rise_Bias();
			g_lastAuto = GetTickCount();
		}
		break;
	case	AS_Rise_Fila:
		if( thisTickCount - g_lastAuto > 400 )
		{
			Auto_Rise_Fila();
			g_lastAuto = GetTickCount();
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Step Functions
void StepAcc()
{
	m_dVset_a += g_dblAutoStep_Accelerator2;
	if( m_dVset_a > g_dblGoalAcc )
	{
		m_dVset_a = g_dblGoalAcc;
		ChangeAcc(0);
		g_bStepAcc = FALSE;
		g_lastStep = 0;
	}
	else
		ChangeAcc(0);
}

void StepFila2()
{
	m_dVset_f += g_dblAutoStep_Fila2;
	if( m_dVset_f > g_dblGoalFila )
	{
		m_dVset_f = g_dblGoalFila;
		ChangeFilament(0);
		g_bStepFila = FALSE;
		g_lastStep = 0;
	}
	else
		ChangeFilament(0);
}

void StepFila1()
{
	m_dVset_f += g_dblAutoStep_Fila;
	if( m_dVset_f > g_dblGoalFila )
	{
		m_dVset_f = g_dblGoalFila;
		ChangeFilament(0);
		g_bStepFila = FALSE;
		g_lastStep = 0;
	}
	else
		ChangeFilament(0);
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

DWORD WINAPI CommThread(LPVOID lpParam)
{
	DeviceOpen();
 	if( !g_bReady )
 		return 0;

	while(WaitForSingleObject(g_hEventTeminate,0) != WAIT_OBJECT_0)
	{
		if( g_AutoState < AS_NULL )
		{
			AutoProc();
//			Sleep(1);
		}
/*		else
		{
			if( g_bAutoBias )
			{
				// 每隔5秒钟判断一下束流值是否在预期范围内，不在则调整
				DWORD thisTickCount = GetTickCount();
				if( thisTickCount -g_lastAutoBias > 5000 )
				{
					if( g_3603.dIoutA < g_dblAutoBias_Min )
					{
						// 束流值小于预期范围低值，则降偏压
						m_dVset_e -= g_dblAutoStep_Bias;
						if( m_dVset_e < 0 )
							m_dVset_e = 0;
						ChangeBias(0);
					}
					if( g_3603.dIoutA > g_dblAutoBias_Max )
					{
						// 束流值大于预期范围高值，则加偏压
						m_dVset_e += g_dblAutoStep_Bias;
						if( m_dVset_e > m_dVscale_e_abs )
							m_dVset_a = m_dVscale_e_abs;
						ChangeBias(0);
					}
					g_lastAutoBias = GetTickCount();
					Sleep(1);
				}
			}
		}
*/
		DWORD thisTickCount = GetTickCount();
		if( g_bStepAcc )
		{
			// 启动分步加高压
			if( thisTickCount -g_lastStep > 400 )
			{
				StepAcc();
				g_lastStep = GetTickCount();
			}
		}
		if( g_bStepFila )
		{
			// 启动分步加灯丝
			if( thisTickCount -g_lastStep > 400 )
			{
				if( m_dVset_f > 2.0 )
					StepFila2();
				else
					StepFila1();
				g_lastStep = GetTickCount();
			}
		}

		// 通讯一次，读一次各路回读值。将所有回读值放在一个数据结构体中上传给主程序
		if( thisTickCount -g_lastMon > 245 )
		{
			CommWith3603();
			g_lastMon = GetTickCount();
		}
		Sleep(1);
	}

	DeviceClose();
	SetEvent( g_hEventWork );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CComm3603 message handlers

void CComm3603::SetParam( int nIndex, int nValue )
{
	switch( nIndex )
	{
	case	1:
		g_3603s.nPort	= nValue;
		break;
	case	2:
		m_bCheckUseA	= (BOOL)nValue;
		break;
	case	3:
		m_bCheckUseE	= (BOOL)nValue;
		break;
	case	4:
		m_bCheckUseF	= (BOOL)nValue;
		break;
	case	5:
		m_bEnA			= (BOOL)nValue;
		ChangeAcc(0);
		break;
	case	6:
		m_bEnE			= (BOOL)nValue;
		ChangeBias(0);
		break;
	case	7:
		m_bEnF			= (BOOL)nValue;
		ChangeFilament(0);
		break;

	case	10:
		if( nValue > 10000 && nValue > m_dVset_a )
		{
			g_dblGoalAcc	= nValue;
			g_bStepAcc		= TRUE;
			g_lastStep		= GetTickCount();
		}
		else
		{
			m_dVset_a		= nValue;
			ChangeAcc( 0 );
		}
		break;
	case	11:
		m_dVset_e		= nValue /10.0;
		ChangeBias( 0 );
		break;
	case	12:
		if( !g_bLookForSaturationPoint
			&& nValue > 1800 && nValue > m_dVset_f *1000 )	// 2.2A
		{
			g_dblGoalFila	= nValue /1000.0;
			g_bStepFila		= TRUE;
			g_lastStep		= GetTickCount();
		}
		else
		{
			m_dVset_f		= nValue /1000.0;
			ChangeFilament( 0 );
		}
		break;

	case	21:
		g_AutoState		= (CPS3603_AutoState)nValue;
		if( g_AutoState == AS_Rise_Acc1 && m_dVset_a > g_dblAutoRated_Accelerator1 )
			g_AutoState = AS_Rise_Acc2;
		g_bStepAcc		= FALSE;
		g_bStepFila		= FALSE;
		break;

	case	25:
		g_dblAutoRated_Accelerator2	= nValue;
		break;
	case	26:
		g_dblAutoRated_Bias			= nValue;
		break;
	case	27:
		g_dblAutoRated_Fila			= nValue /100.0;
		break;

	case	31:
		// 自偏压范围为预期值 +/- 20%
		g_dblAutoBias_Min	= nValue - nValue *0.2;
		g_dblAutoBias_Max	= nValue + nValue *0.2;
		break;

	case	35:
		g_bLookForSaturationPoint = (BOOL)nValue;
		g_bStepAcc		= FALSE;
		g_bStepFila		= FALSE;
		g_lastStep		= 0;
	}
}

void CComm3603::GetParam( int nIndex, int& nValue )
{
	// 返回值与界面控件设置的步数相对应
	switch( nIndex )
	{
	case	1:
		nValue	= g_3603s.nPort;
		break;
	case	10:
		nValue	= (int)m_dVset_a;
		break;
	case	11:
		nValue	= (int)m_dVset_e;
		break;
	case	12:
		nValue	= (int)(m_dVset_f *100);
		break;
	}
}

BOOL CComm3603::InitDevice( CWnd* pWnd )
{
	g_hMain = pWnd->m_hWnd;

	ResetEvent( g_hEventWork );
	ResetEvent( g_hEventTeminate );

	g_hThread	= CreateThread( NULL, 0, CommThread, NULL, 0, &g_dwThread );
	if( g_hThread == NULL )
		return FALSE;

	CloseHandle(g_hThread);
	return TRUE;
}

BOOL CComm3603::IsReady()
{
	return g_bReady;
}

BOOL CComm3603::CloseDevice()
{
	if( g_bReady )
	{
		SetEvent( g_hEventTeminate );
		WaitForSingleObject( g_hEventWork, INFINITE );
		g_bReady = FALSE;
	}
	PostMessage( g_hMain, WM_USER_CPS_Status, 0x01, 0x00 );

	return TRUE;
}
