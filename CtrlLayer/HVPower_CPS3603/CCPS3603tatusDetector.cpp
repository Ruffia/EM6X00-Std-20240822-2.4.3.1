#include "StdAfx.h"
#include "CCPS3603tatusDetector.h"
#include "3601/c3601.h"
#include "3601/c3601win.h"
#include <math.h>

#define minmax(x,l,r)   (((x)<(l))?(l):((x)>(r))?(r):(x))

CCPS3603tatusDetector& CCPS3603tatusDetector::Instance()
{
	static CCPS3603tatusDetector Inst;
	return Inst;
}


CCPS3603tatusDetector::CCPS3603tatusDetector(void)
{
	m_hWnd = NULL;

	memset(workdir,0,256);
	memset(ininame,0,256);
	memset(AppName,0,256);
	const char* sINI = "3603_control";
	strcpy(AppName,sINI);

	InitParam();

	g_AutoState	= AS_NULL;

	g_hFile3603	= NULL;
	g_nLoops3603 = 0;
	g_bReady	= FALSE;

	g_dblAutoStep_Accelerator1	= 500;
	g_dblAutoStep_Accelerator2	= 1000;
	g_dblAutoRated_Accelerator1	= 10000;
	g_dblAutoRated_Accelerator2	= 30000;
	g_dblAutoStep_Bias			= 10;
	g_dblAutoRated_Bias			= 100;
	g_dblAutoStep_Fila			= 0.100;// 100mA
	g_dblAutoStep_Fila2			= 0.010;// 10mA
	g_dblAutoRated_Fila			= 1.9;	// 1.9A

	g_dblAutoBias_Min			= 130;
	g_dblAutoBias_Max			= 170;
	g_bAutoBias					= FALSE;

	g_bLookForSaturationPoint	= FALSE;

	m_dwSleep = 250;
	m_bNotify2FilamentCurrentContorl = FALSE;   //通知灯丝电流控件UI
	m_bNotify2HVContorl = TRUE;                //通知高压控件UI

}


void CCPS3603tatusDetector::Init( HWND hWnd,int nPort )
{
	char sz[512] = {0};
	sprintf_s(sz,512,"void CCPS3603tatusDetector::Init( HWND hWnd,int nPort ), nPort = %d\r\n",nPort );
	OutputDebugString(sz);

	m_hWnd = hWnd;
	g_3603s.nPort = nPort;
}

BOOL CCPS3603tatusDetector::Start()
{
	DeviceOpen();
	if( !g_bReady )
		return 0;

	BOOL bStart = __super::Start();
	return bStart;
}


void CCPS3603tatusDetector::Release()
{
	DeviceClose();
	__super::Release();
}


DWORD CCPS3603tatusDetector::DoJob()
{
	if( g_AutoState < AS_NULL )
	{
		AutoProc();
	}

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
		//如果是灯丝清零，则直接操作，不需经过Step步骤
		if (abs(g_dblGoalFila) < EPSILON)
		{
			m_dVset_f = g_dblGoalFila;
			ChangeFilament(0);
			PostMessage( m_hWnd, WM_USER_CPS_AutoParam, AS_Rise_Fila, (LPARAM)(m_dVset_f*1000) );
			g_bStepFila = FALSE;
			g_lastStep = 0;
		}

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

	Sleep( 1 );

	return 0L;
}


void CCPS3603tatusDetector::InitParam()
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


// Error message
int CCPS3603tatusDetector::error(char *fmt, ... )
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


// Read configuration data from the <test dir>\3601.ini file
void CCPS3603tatusDetector::readini(void)
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

void CCPS3603tatusDetector::readinipar(char *buf, char *name, double *par)
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


void CCPS3603tatusDetector::writeini(void)
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
void CCPS3603tatusDetector::logprintf(char *fmt, ... )
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


///////////////////////////////////////////////////////////
void CCPS3603tatusDetector::ChangeAcc( int nPos, BOOL bHScroll )
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


void CCPS3603tatusDetector::ChangeBias( int nPos, BOOL bHScroll )
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


void CCPS3603tatusDetector::ChangeFilament( int nPos, BOOL bHScroll )
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

BOOL CCPS3603tatusDetector::DeviceOpen()
{
	OutputDebugString("CCPS3603tatusDetector---------- CCPS3603tatusDetector::DeviceOpen() \r\n");
	m_mutexPort.Lock();
	// 打开端口
	g_3603s.nConnectA = g_3603s.nConnectE = g_3603s.nConnectF = !C3601_OpenPort(g_3603s.nPort, DEF_BAUD_RATE);

	char sz[512] = {0};
	sprintf_s(sz,512,"g_3603s.nPort = %d,%s\r\n",g_3603s.nPort, g_3603s.nConnectA?"g_3603s.nConnectA = g_3603s.nConnectE = g_3603s.nConnectF = true":"g_3603s.nConnectA = g_3603s.nConnectE = g_3603s.nConnectF = false" );
	OutputDebugString(sz);

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

	PostMessage( m_hWnd, WM_USER_CPS_Status, 0x02, (LPARAM)( &g_3603s ) );

	ZeroMemory(sz,512);
	sprintf_s(sz,512,"%s\r\n","PostMessage( m_hWnd, WM_USER_CPS_Status, 0x02, (LPARAM)( &g_3603s ) );" );
	OutputDebugString(sz);

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


BOOL CCPS3603tatusDetector::DeviceClose()
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

	PostMessage( m_hWnd, WM_USER_CPS_Status, 0x01, 0x00 );

	if( g_hFile3603 != NULL )
		CloseHandle( g_hFile3603 );
	g_hFile3603	= NULL;

	return TRUE;
}


static double detcorona(double i)
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


void CCPS3603tatusDetector::CommWith3603()
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

	PostMessage( m_hWnd, WM_USER_CPS_Param, (WPARAM)( &g_3603 ), 0 );
	PostMessage( m_hWnd, WM_USER_CPS_MSG, 0, m_nstsError );

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
void CCPS3603tatusDetector::Auto_Rise_Acc1()
{
	m_dVset_a += g_dblAutoStep_Accelerator1;
	if( m_dVset_a >= g_dblAutoRated_Accelerator2 )
	{
		m_dVset_a = g_dblAutoRated_Accelerator2;
		ChangeAcc(0);
		PostMessage( m_hWnd, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)m_dVset_a );
		g_AutoState = AS_Rise_Bias;
	}
	else if( m_dVset_a >= g_dblAutoRated_Accelerator1 )
	{
		m_dVset_a = g_dblAutoRated_Accelerator1;
		ChangeAcc(0);
		PostMessage( m_hWnd, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)m_dVset_a );
		g_AutoState = AS_Rise_Acc2;
	}
	else
	{
		ChangeAcc(0);
		PostMessage( m_hWnd, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)m_dVset_a );
	}
}

void CCPS3603tatusDetector::Auto_Rise_Acc2()
{
	m_dVset_a += g_dblAutoStep_Accelerator2;
	if( m_dVset_a >= g_dblAutoRated_Accelerator2 )
	{
		m_dVset_a = g_dblAutoRated_Accelerator2;
		ChangeAcc(0);
		PostMessage( m_hWnd, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)m_dVset_a );
		g_AutoState = AS_Rise_Bias;
	}
	else
	{
		ChangeAcc(0);
		PostMessage( m_hWnd, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)m_dVset_a );
	}
}

void CCPS3603tatusDetector::Auto_Rise_Bias()
{
	m_dVset_e += g_dblAutoStep_Bias;
	if( m_dVset_e >= g_dblAutoRated_Bias )
	{
		m_dVset_e = g_dblAutoRated_Bias;
		ChangeBias(0);
		PostMessage( m_hWnd, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)m_dVset_e );
		g_AutoState = AS_Rise_Fila;
	}
	else
	{
		ChangeBias(0);
		PostMessage( m_hWnd, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)m_dVset_e );
	}
}

void CCPS3603tatusDetector::Auto_Rise_Fila()
{
	m_dVset_f += g_dblAutoStep_Fila;
	if( m_dVset_f >= g_dblAutoRated_Fila )
	{
		m_dVset_f = g_dblAutoRated_Fila;
		ChangeFilament(0);
		PostMessage( m_hWnd, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)(m_dVset_f*1000) );
		g_AutoState = AS_NULL;	// 结束
		PostMessage( m_hWnd, WM_USER_CPS_AutoParam, g_AutoState, 0 );
	}
	else
	{
		ChangeFilament(0);
		PostMessage( m_hWnd, WM_USER_CPS_AutoParam, g_AutoState, (LPARAM)(m_dVset_f*1000) );
	}
}

void CCPS3603tatusDetector::AutoProc()
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
void CCPS3603tatusDetector::StepAcc()
{
	if (m_dVset_a > g_dblGoalAcc)
	{
		m_dVset_a -= g_dblAutoStep_Accelerator2;
		if( m_dVset_a < g_dblGoalAcc )
		{
			m_dVset_a = g_dblGoalAcc;
			ChangeAcc(0);
			if (m_bNotify2HVContorl)
			{
				PostMessage( m_hWnd, WM_USER_CPS_AutoParam, AS_Rise_Acc1, (LPARAM)m_dVset_a );
				m_bNotify2HVContorl = FALSE;
			}	
			g_bStepAcc = FALSE;
			g_lastStep = 0;
		}
		else
		{
			ChangeAcc(0);
			if (m_bNotify2HVContorl)
			{
				PostMessage( m_hWnd, WM_USER_CPS_AutoParam, AS_Rise_Acc1, (LPARAM)m_dVset_a );
			}
		}
	}
	else
	{
		m_dVset_a += g_dblAutoStep_Accelerator2;
		if( m_dVset_a > g_dblGoalAcc )
		{
			m_dVset_a = g_dblGoalAcc;
			ChangeAcc(0);
			if (m_bNotify2HVContorl)
			{
				PostMessage( m_hWnd, WM_USER_CPS_AutoParam, AS_Rise_Acc1, (LPARAM)m_dVset_a );
				m_bNotify2HVContorl = FALSE;
			}	
			g_bStepAcc = FALSE;
			g_lastStep = 0;
		}
		else
		{
			ChangeAcc(0);
			if (m_bNotify2HVContorl)
			{
				PostMessage( m_hWnd, WM_USER_CPS_AutoParam, AS_Rise_Acc1, (LPARAM)m_dVset_a );
			}
		}
	}
}

void CCPS3603tatusDetector::StepFila2()
{
	if (m_dVset_f > g_dblGoalFila)
	{
		m_dVset_f -= g_dblAutoStep_Fila2;
		if (m_dVset_f < g_dblGoalFila)
		{
			m_dVset_f = g_dblGoalFila;
			ChangeFilament(0);
			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_CPS_AutoParam, AS_Rise_Fila, (LPARAM)(m_dVset_f*1000) );
				m_bNotify2FilamentCurrentContorl = FALSE;
			}
			g_bStepFila = FALSE;
			g_lastStep = 0;
		}
		else
		{
			ChangeFilament(0);
			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_CPS_AutoParam, AS_Rise_Fila, (LPARAM)(m_dVset_f*1000) );
			}
		}
	}
	else
	{
		m_dVset_f += g_dblAutoStep_Fila2;
		if( m_dVset_f > g_dblGoalFila )
		{
			m_dVset_f = g_dblGoalFila;
			ChangeFilament(0);
			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_CPS_AutoParam, AS_Rise_Fila, (LPARAM)(m_dVset_f*1000) );
				m_bNotify2FilamentCurrentContorl = FALSE;
			}
			g_bStepFila = FALSE;
			g_lastStep = 0;
		}
		else
		{
			ChangeFilament(0);
			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_CPS_AutoParam, AS_Rise_Fila, (LPARAM)(m_dVset_f*1000) );
			}
		}
	}
}


void CCPS3603tatusDetector::StepFila1()
{
	if (m_dVset_f > g_dblGoalFila)
	{
		m_dVset_f -= g_dblAutoStep_Fila;
		if( m_dVset_f < g_dblGoalFila )
		{
			m_dVset_f = g_dblGoalFila;
			ChangeFilament(0);
			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_CPS_AutoParam, AS_Rise_Fila, (LPARAM)(m_dVset_f*1000) );
			}
			g_bStepFila = FALSE;
			g_lastStep = 0;
		}
		else
		{
			ChangeFilament(0);
			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_CPS_AutoParam, AS_Rise_Fila, (LPARAM)(m_dVset_f*1000) );
			}
		}
	}
	else
	{
		m_dVset_f += g_dblAutoStep_Fila;
		if( m_dVset_f > g_dblGoalFila )
		{
			m_dVset_f = g_dblGoalFila;
			ChangeFilament(0);
			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_CPS_AutoParam, AS_Rise_Fila, (LPARAM)(m_dVset_f*1000) );
			}
			g_bStepFila = FALSE;
			g_lastStep = 0;
		}
		else
		{
			ChangeFilament(0);
			if (m_bNotify2FilamentCurrentContorl)
			{
				PostMessage( m_hWnd, WM_USER_CPS_AutoParam, AS_Rise_Fila, (LPARAM)(m_dVset_f*1000) );
			}
		}
	}
}


void CCPS3603tatusDetector::SetParam( int nIndex, int nValue )
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
		{
			if( nValue > 10000 && nValue > m_dVset_a )
			{
				g_dblGoalAcc	= nValue;
				g_bStepAcc		= TRUE;
				g_lastStep		= GetTickCount();
			}
			else
			{
				g_dblGoalAcc	= nValue;
				g_bStepAcc		= TRUE;
				g_lastStep		= GetTickCount();
				//ChangeAcc( 0 );
			}
		}
		break;
	case	11:
		m_dVset_e		= nValue /10.0;
		ChangeBias( 0 );
		break;
	case	12:
		{
			if( !g_bLookForSaturationPoint
				&& nValue > 1800 && nValue > m_dVset_f *1000 )	// 2.2A
			{
				g_dblGoalFila	= nValue /1000.0;
				g_bStepFila		= TRUE;
				g_lastStep		= GetTickCount();
			}
			else
			{
				g_dblGoalFila	= nValue /1000.0;
				g_bStepFila		= TRUE;
				g_lastStep		= GetTickCount();
			}
		}

		break;

	case    13:
		m_bNotify2HVContorl	= (BOOL)nValue;
		break;
	case    14:
		m_bNotify2FilamentCurrentContorl	= (BOOL)nValue;
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

void CCPS3603tatusDetector::GetParam( int nIndex, int& nValue )
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

BOOL CCPS3603tatusDetector::InitDevice( HWND pWnd, int nPort )
{
	m_hWnd = m_hWnd;
	g_3603s.nPort = nPort;
	return TRUE;
}

BOOL CCPS3603tatusDetector::IsReady()
{
	return g_bReady;
}

BOOL CCPS3603tatusDetector::CloseDevice()
{
	PostMessage( m_hWnd, WM_USER_CPS_Status, 0x01, 0x00 );
	return TRUE;
}
