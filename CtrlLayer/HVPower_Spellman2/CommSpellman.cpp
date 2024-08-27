// CommSpellman.cpp : implementation file
//

#include "stdafx.h"
#include "CommSpellman.h"

#include <USB_Functions.h>
#include <afxmt.h>

static	CCriticalSection		m_mutexPorts;

DWORD WINAPI CommThread(LPVOID lpParam);

HANDLE	g_hThread;
DWORD	g_dwThread;
HANDLE	g_hEvent				= CreateEvent(NULL,FALSE, FALSE, _T("WORKspellman"));
HANDLE	g_hEventTeminate		= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEspellman"));

HWND	g_hMain;
//KYKYParam*	g_pParam	= NULL;
DebugParam_Spellman g_Spellmans;
Spellman_AutoState	g_AutoState = AS2_NULL;

double	g_dblAutoStep_EHT1		= 0.5;	// 500;
double	g_dblAutoStep_EHT2		= 1.0;	// 1000;
double	g_dblAutoRated_EHT1		= 10.0;	// 10000;
double	g_dblAutoRated_EHT2		= 30.0;	// 30000;
double	g_dblAutoStep_Beam		= 10;
double	g_dblAutoRated_Beam		= 100;
double	g_dblAutoStep_Heater	= 0.100;// 100mA
double	g_dblAutoRated_Heater	= 1.9;	// 1.9A

HANDLE	g_hFile	= NULL;
int		g_nLoops;
CString	g_strSpellman, g_strSpellmanSaved;

//BOOL	g_bReady		= FALSE;	// 是否至少有一个设备联通
BOOL	g_bPauseComm	= FALSE;	// 是否暂停与设备通讯
BOOL	g_bPausePost	= FALSE;	// 是否暂停上送监测值（但不停止与设备通讯）
BOOL	g_bPauseInquire	= FALSE;
DWORD	g_lastMon, g_lastAuto;

extern	CString	HID_FindHIDInterface( int nVID, int nPID, int nUsagePage );
extern	HANDLE	HID_OpenForRead( CString strFileName );
extern	HANDLE	HID_OpenForWrite( CString strFileName );

CString	HID_OpenHIDDevice();
BOOL	HID_InitHIDDevice( CString strDeviceName );
void	HID_CloseHIDDevice();

CString		g_strDeviceName	= _T("");				// The system name for the remote button device
BOOL		g_bOpenned		= FALSE;				// Is the ButtonsAndLights device operational?

#define		BUFFER_LEN	65

HANDLE		g_hWrite		= NULL;					// Needed to write to a system device
HANDLE		g_hRead			= NULL;					// Needed to read a system device
OVERLAPPED	g_overlapRead;

BYTE		g_bufferRead[BUFFER_LEN];				// Data read from ButtonsAdnLights device, changed to 8 by DD 6-24-10
BYTE		g_bufferWrite[BUFFER_LEN];				// Data written to ButtonsAndLights device

double		g_dConstStep_PMT	= 1350.0 /4095;
double		g_dConstStep_II		= 411.11 /4095;
double		g_dConstStep_Heater	= 4200.0 /4095 /1000;
double		g_dConstStep_EHT	= 33000.0/1023;
double		g_dConstStep_Col	= (471.06 +433.37)/1023;
double		g_dConstStep_Scint	= 13660.0 /1023;
double		g_dConstStep_PMT3	= 1350.0 /1023;
/////////////////////////////////////////////////////////////////////////////
// HID device
/////////////////////////////////////////////////////////////////////////////

CString HID_OpenHIDDevice()
{
	// I know this is a HID device with a specific VID and PID.
	// I also search for the UsagePage that defines the data reports
	// I return the system name for the device or NULL if the device is not plugged in
	CString strSystemName = _T("");
	// check for examples 1 and 4
	strSystemName = HID_FindHIDInterface( 0x04D8, 0x003F, 0xFF00 );
	if( strSystemName == "" )
		strSystemName = HID_FindHIDInterface( 0x04D8, 0x003F, 0xFF00 );		// Set in USB_descr.asm
	
	return strSystemName;
}

BOOL HID_InitHIDDevice( CString strDeviceName )
{
	///////////////////////////////////////////////////////////////////////////
	g_overlapRead.Offset		= 0;
	g_overlapRead.OffsetHigh	= 0;
	g_overlapRead.hEvent		= g_hEvent;
	///////////////////////////////////////////////////////////////////////////
	
    if (strDeviceName == "")
        return FALSE;
    else
	{
		g_hRead		= HID_OpenForRead(strDeviceName);
		g_hWrite	= HID_OpenForWrite(strDeviceName);
		g_bOpenned	= TRUE;
	}
	
	// 接收数据缓冲区清零
	memset( g_bufferRead, 0, sizeof(g_bufferRead) );
	memset( g_bufferWrite, 0, sizeof(g_bufferWrite) );
	
	return TRUE;
}

void HID_CloseHIDDevice()
{
	if( g_hRead != NULL )
	{
		CloseHandle( g_hRead );
		g_hRead = NULL;
	}
	if( g_hWrite != NULL )
	{
		CloseHandle( g_hWrite );
		g_hWrite = NULL;
	}

	ResetEvent( g_hEvent );

	g_bOpenned = FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// HID device End
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
// CCommSpellman

CCommSpellman::CCommSpellman()
{
	g_hMain	= NULL;

	g_Spellmans.status.bReady		= FALSE;
	g_Spellmans.status.bCollector	= FALSE;
	g_Spellmans.status.bScint		= FALSE;
	g_Spellmans.status.bEHT		= FALSE;
	g_Spellmans.status.bHeater	= FALSE;
	g_Spellmans.status.bPMT		= FALSE;

	g_Spellmans.pS.dVC			= 0.0;
	g_Spellmans.pS.dVE			= 0.0;
	g_Spellmans.pS.dIB			= 0.0;
	g_Spellmans.pS.dIH			= 0.0;
	g_Spellmans.pS.dVP			= 0.0;

	g_Spellmans.pM.dVC			= 0.0;
	g_Spellmans.pM.dVE			= 0.0;
	g_Spellmans.pM.dIB			= 0.0;
	g_Spellmans.pM.dIH			= 0.0;
	g_Spellmans.pM.dVP			= 0.0;
	g_Spellmans.pM.dII			= 0.0;

	g_Spellmans.pM2.dVC			= 0.0;
	g_Spellmans.pM2.dVE			= 0.0;
	g_Spellmans.pM2.dIB			= 0.0;
	g_Spellmans.pM2.dIH			= 0.0;
	g_Spellmans.pM2.dVP			= 0.0;
}

CCommSpellman::~CCommSpellman()
{
}

/////////////////////////////////////////////////////////////////////////////
// CCommSpellman message handlers
/*
// 从字符串的左边截取n个字符
char * left(char *dst,char *src, int n)
{
	char *p = src;
	char *q = dst;
	int len = strlen(src);
	if(n>len) n = len;
//	p += (len-n);			// 从右边第n个字符开始
	while(n--) *(q++) = *(p++);
	*(q++)='\0';			// 有必要吗？很有必要
	return dst;
}

// 从字符串的中间截取n个字符
char * mid(char *dst,char *src, int n,int m) // n为长度，m为位置
{
	char *p = src;
	char *q = dst;
	int len = strlen(src);
	if(n>len) n = len-m;	// 从第m个到最后
	if(m<0) m=0;			// 从第一个开始
	if(m>len) return NULL;
	p += m;
	while(n--) *(q++) = *(p++);
	*(q++)='\0';			// 有必要吗？很有必要
	return dst;
}

// 从字符串的右边截取n个字符
char * right(char *dst,char *src, int n)
{
	char *p = src;
	char *q = dst;
	int len = strlen(src);
	if(n>len) n = len;
	p += (len-n);			// 从右边第n个字符开始
	while(*(q++) = *(p++));
	return dst;
}

char *substring(char *dst,char *src,int len,int start)
{
	char *p=dst;
	char *q=src;
	int length=strlen(src);
	if(start>=length||start<0) return NULL;
	if(len>length) len=length-start;
	q+=start;
	while(len--)
	{
		*(p++)=*(q++);
	}
	*(p++)='\0';
	return dst;
}

char valueToHexCh(const int value)
{
	char result = '\0';
	if(value >= 0 && value <= 9){
		result = (char)(value + 48);		//48为ascii编码的‘0’字符编码值
	}
	else if(value >= 10 && value <= 15){
		result = (char)(value - 10 + 65);	//减去10则找出其在16进制的偏移量，65为ascii的'A'的字符编码值
	}
	else{
		;
	}

	return result;
}

int strToHex(char *ch, char *hex)
{
	int high,low;
	int tmp = 0;
	if(ch == NULL || hex == NULL){
		return -1;
	}

	if(strlen(ch) == 0){
		return -2;
	}

	while(*ch){
		tmp = (int)*ch;
		high = tmp >> 4;
		low = tmp & 15;
		*hex++ = valueToHexCh(high);	//先写高字节
		*hex++ = valueToHexCh(low);		//其次写低字节
		ch++;
	}
	*hex = '\0';
	return 0;
}

int hexCharToValue(const char ch)
{
	int result = 0;
	//获取16进制的高字节位数据
	if(ch >= '0' && ch <= '9'){
		result = (int)(ch - '0');
	}
	else if(ch >= 'a' && ch <= 'z'){
		result = (int)(ch - 'a') + 10;
	}
	else if(ch >= 'A' && ch <= 'Z'){
		result = (int)(ch - 'A') + 10;
	}
	else{
		result = -1;
	}
	return result;
}

int hexToStr(char *hex, char *ch)
{
	int high,low;
	int tmp = 0;
	if(hex == NULL || ch == NULL){
		return -1;
	}

	if(strlen(hex) %2 == 1){
		return -2;
	}

	while(*hex){
		high = hexCharToValue(*hex);
		if(high < 0){
			*ch = '\0';
			return -3;
		}
		hex++; //指针移动到下一个字符上
		low = hexCharToValue(*hex);
		if(low < 0){
			*ch = '\0';
			return -3;
		}
		tmp = (high << 4) + low;
		*ch++ = (char)tmp;
		hex++;
	}
	*ch = '\0';
	return 0;
}

int char2int(char data[3]) 
{ 
	int a, b, c; 
	if (data[0]>='0' && data[0]<='9') a=data[0]-'0'; 
	else if(data[0]>='a' && data[0]<='f') a=data[0]-'a'+10; 
	else if(data[0]>='A' && data[0]<='F') a=data[0]-'A'+10; 
	else return -1; 

	if (data[1]>='0' && data[1]<='9') b=data[1]-'0'; 
	else if(data[1]>='a' && data[1]<='f') b=data[1]-'a'+10; 
	else if(data[1]>='A' && data[1]<='F') b=data[1]-'A'+10; 
	else return -1; 

	if (data[2]>='0' && data[2]<='9') c=data[2]-'0'; 
	else if(data[2]>='a' && data[2]<='f') c=data[2]-'a'+10; 
	else if(data[2]>='A' && data[2]<='F') c=data[2]-'A'+10; 
	else return -1; 

	return a*16*16 +b*16 +c; 
}

int ConvertReceiveData( char* data )
{
	char cCR = '\r';
	char *pdest = strchr( data, cCR );
	int nPos = pdest -data +1;
	return 0;
}

void ConvertReceiveData()
{
	char cCmd = g_bufferRead[1];
	switch( cCmd )
	{
	case	'G':
		break;
	default:
		break;
	}
}
*/

void Message()
{
	if( g_hFile != NULL )
	{
		unsigned long cbRet;
		char Buf[255];

		CString str, strTemp;
//		str.Format( "0x%02X\t0x%03X\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.3f\t%.3f\t%.2f\t%.2f\t%.2f",
		str.Format( "0x%02X,0x%05X, %7.3lf,%7.3lf,%7.3lf, %9.3lf,%9.3lf,%9.3lf, %7.3lf,%7.3lf,%9.3lf, %5.3lf,%5.3lf,%5.3lf, %7.3lf,%7.3lf,%7.3lf, %6.2lf",
					g_Spellmans.status.bstatusSwitch, g_Spellmans.status.dwstatusRegister,
					g_Spellmans.pS.dVC, g_Spellmans.pM.dVC, g_Spellmans.pM3.dVC,
					g_Spellmans.pS.dVE, g_Spellmans.pM.dVE, g_Spellmans.pM3.dVE,
					g_Spellmans.pS.dIB, g_Spellmans.pM.dIB, g_Spellmans.pM3.dIB,
					g_Spellmans.pS.dIH, g_Spellmans.pM.dIH, g_Spellmans.pM3.dIH,
					g_Spellmans.pS.dVP, g_Spellmans.pM.dVP, g_Spellmans.pM3.dVP,
					g_Spellmans.pM.dII );

		CTime	timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime = timeCurrent.Format("%Y/%m/%d %H:%M:%S");
		strTemp.Format( "[%s], %s", csCurrentTime, str );
		sprintf(Buf, "%s\r\n", strTemp );
		WriteFile(g_hFile, Buf, strlen(Buf), &cbRet, NULL);

		g_nLoops++;
		if( g_nLoops > 10000 )
		{
			CloseHandle( g_hFile );
			// 设置日志文件
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			char *ptemp = strrchr( exeFullPath, '\\');
			ptemp[1] = 0;
			
			CTime	timeCurrent = CTime::GetCurrentTime();
			csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
			str.Format( "%s\\Log\\Spell771-%s.csv", exeFullPath, csCurrentTime );
			g_hFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoops = 1;

			unsigned long cbRet;
			char Buf[255];
			str.Format( "\t%s\r\n", "EBM30N12/771" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFile, Buf, strlen(Buf), &cbRet, NULL );
//			str.Format( "\tSwitch\tStatus\tCollector s(V)\tCollector m(V)\tEHT s(KV)\tEHT m(KV)\tBeam s(uA)\tBeam m(uA)\tHeat s(A)\tHeat m(A)\tPMT s(V)\tPMT m(V)\tBeam Current(uA)" );
//			str.Format( ",Switch,Status,Collector s(V),Collector m(V),EHT s(KV),EHT m(KV),Beam s(uA),Beam m(uA),Heat s(A),Heat m(A),PMT s(V),PMT m(V),Beam Current(uA)" );
			str.Format( "Time, Switch,Status, C s(V),C m(V),C m2(V), E s(V),E m(V),E m2(V), B s(uA),B m(uA),S m(V), H s(A),H m(A),H m2(A), P s(V),P m(V),P m2(V), Beam Current(uA)" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFile, Buf, strlen(Buf), &cbRet, NULL );
		}
	}
}

void Message2()
{
	if( g_hFile != NULL )
	{
		unsigned long cbRet;
		char Buf[255];

		CString str;
		str.Format( "Collector Ramp rates = %.2f V/s", g_Spellmans.pM2.dVC );
		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFile, Buf, strlen(Buf), &cbRet, NULL);
		str.Format( "EHT Ramp rates = %.2f V/s", g_Spellmans.pM2.dVE );
		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFile, Buf, strlen(Buf), &cbRet, NULL);
		str.Format( "Beam Ramp rates = %.2f uA/s", g_Spellmans.pM2.dIB );
		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFile, Buf, strlen(Buf), &cbRet, NULL);
		str.Format( "Heater Ramp rates = %.2f A/s", g_Spellmans.pM2.dIH );
		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFile, Buf, strlen(Buf), &cbRet, NULL);
		str.Format( "PMT Ramp rates = %.2f V/s", g_Spellmans.pM2.dVP );
		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFile, Buf, strlen(Buf), &cbRet, NULL);
		str = " ";
		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFile, Buf, strlen(Buf), &cbRet, NULL);
	}
}

int ConvertReceiveData( CString strData )
{
	if( strData.IsEmpty() )
		return -1;

	CString str, strC, strType, strValue;
	int nPos, nRet = 0;
	str = strData;
/*	strC.Format( "\012" );
	nPos = str.Find( strC );
	int nLength = str.GetLength();
	if( nPos < nLength -2 )
	{
		// 回车在字符串中间
		g_strSpellmanSaved = str.Right( nLength -nPos -2 );
		str = str.Left( nPos );
	}
	else
		g_strSpellmanSaved = "";
	if( str.GetLength() < 5 )
		return 1;	// 数据量不足
*/
	strType = str.Left( 1 );	// G,V, O,P, C/E/B/H/J/I, c/e/b/h/j

	strValue = str.Mid(1,3);
	char *bufNum = strValue.GetBuffer(4);
	sscanf(bufNum, "%x", &nPos);

	if( strType == "G" )		// Gun supply identification
	{
		g_Spellmans.status.strID = str.Right( str.GetLength() -1 );
//		PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 2, 0 );
		PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 2, (LPARAM)(&(g_Spellmans.status)) );
		nRet = 1;
	}
	else if( strType == "V" )	// Software Version identification
	{
		g_Spellmans.status.strVer = str.Right( str.GetLength() -1 );
//		PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 3, 0 );
		PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 3, (LPARAM)(&(g_Spellmans.status)) );
		nRet = 2;
	}
	else if( strType == "O" )	// On/Off/Status
	{
		strValue = str.Mid( 1, str.GetLength() -1 );
		int nLength = strValue.GetLength();
		char *bufStatus = strValue.GetBuffer( nLength +1 );
		sscanf(bufStatus, "%X", &nPos );
//		if( nLength > 20 )
		{
			g_Spellmans.status.dwstatusRegister	= nPos & 0xFFFFF;
			g_Spellmans.status.bstatusSwitch	= nPos >> 20;
		}
/*		else
		{
			g_Spellmans.status.dwstatusRegister	= nPos & 0xFFF;
			g_Spellmans.status.bstatusSwitch	= nPos >> 12;
		}
*/		g_Spellmans.status.bPMT			= (g_Spellmans.status.bstatusSwitch & 0x10) == 0x10;
		g_Spellmans.status.bHeater		= (g_Spellmans.status.bstatusSwitch & 0x08) == 0x08;
		g_Spellmans.status.bEHT			= (g_Spellmans.status.bstatusSwitch & 0x04) == 0x04;
		g_Spellmans.status.bScint		= (g_Spellmans.status.bstatusSwitch & 0x02) == 0x02;
		g_Spellmans.status.bCollector	= (g_Spellmans.status.bstatusSwitch & 0x01) == 0x01;
		nRet = 3;
	}
	else if( strType == "P" )
		nRet = 4;
	else if( strType == "C" )	// Collector voltage program
	{
		g_Spellmans.pM.dVC	= (nPos -2048) *0.2;
		nRet = 11;
	}
	else if( strType == "E" )	// EHT voltage program
	{
		g_Spellmans.pM.dVE	= nPos *10.0;
		nRet = 12;
	}
	else if( strType == "B" )	// Beam current program
	{
		g_Spellmans.pM.dIB	= nPos *0.1;
		nRet = 13;
	}
	else if( strType == "H" )	// Heater current program
	{
		g_Spellmans.pM.dIH	= nPos *0.001;
		nRet = 14;
	}
	else if( strType == "J" )	// PMT voltage program
	{
		g_Spellmans.pM.dVP	= nPos *g_dConstStep_PMT;
		nRet = 15;
	}
	else if( strType == "I" )	// Beam current monitor
	{
		g_Spellmans.pM.dII	= nPos *g_dConstStep_II;
		nRet = 16;
	}
	/////////////////////////////////////////////
	else if( strType == "c" )	// Collector ramp rates
	{
		g_Spellmans.pM2.dVC	= nPos *0.2;
		nRet = 21;
	}
	else if( strType == "e" )	// EHT ramp rates
	{
		g_Spellmans.pM2.dVE	= nPos *10;
		nRet = 22;
	}
	else if( strType == "b" )	// Beam ramp rates
	{
		g_Spellmans.pM2.dIB	= nPos *0.1;
		nRet = 23;
	}
	else if( strType == "h" )	// Heater ramp rates
	{
		g_Spellmans.pM2.dIH	= nPos *0.001;
		nRet = 24;
	}
	else if( strType == "j" )	// PMT ramp rates
	{
		g_Spellmans.pM2.dVP	= nPos *0.33;
		nRet = 25;
	}
	/////////////////////////////////////////////
	else if( strType == "f" )	// Heater current monitor
	{
		g_Spellmans.pM3.dIH	= nPos *g_dConstStep_Heater;
		nRet = 31;
	}
	else if( strType == "a" )	// EHT voltage monitor
	{
		g_Spellmans.pM3.dVE	= nPos *g_dConstStep_EHT;
		nRet = 32;
	}
	else if( strType == "k" )	// Collector voltage monitor
	{
		g_Spellmans.pM3.dVC	= 471.06 -nPos *g_dConstStep_Col;	// 0.88;
		nRet = 33;
	}
	else if( strType == "s" )	// Scinitillator voltage monitor
	{
		g_Spellmans.pM3.dIB	= nPos *g_dConstStep_Scint;
		nRet = 34;
	}
	else if( strType == "p" )	// PMT voltage monitor
	{
		g_Spellmans.pM3.dVP	= nPos *g_dConstStep_PMT3;
		nRet = 35;
	}
	return nRet;
}

BOOL WriteToDevice()
{
	DWORD dwBytesWritten, dwErr;
	BOOL bRetWrite = WriteFile( g_hWrite, g_bufferWrite, BUFFER_LEN, &dwBytesWritten, NULL );
	if( !bRetWrite )
		dwErr = GetLastError();
	return bRetWrite;
}

CString ReadFromDevice()
{
	CString strData = "";
	DWORD dwErr, dwBytesRead = 0;

	BOOL bRetRead = ReadFile(g_hRead, g_bufferRead, BUFFER_LEN, &dwBytesRead, &g_overlapRead);
	if( !bRetRead )
		dwErr = GetLastError();

	if( bRetRead )
	{
		CString str;
		for( DWORD i=1; i<dwBytesRead; i++ )
		{
			if( g_bufferRead[i] == 0x0D )
				break;
			str.Format( "%c", g_bufferRead[i] );
			strData += str;
		}
	}
	return strData;
}

void ReadRampRate( int nIndex, int nSleep )
{
	if( !g_Spellmans.status.bReady )
		return;

	m_mutexPorts.Lock();
	// 发送查询命令
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('?');
	switch( nIndex )
	{
	case	0:	// Collector
		g_bufferWrite[2] = (BYTE)('c');
		break;
	case	1:	// EHT
		g_bufferWrite[2] = (BYTE)('e');
		break;
	case	2:	// Beam
		g_bufferWrite[2] = (BYTE)('b');
		break;
	case	3:	// Heater
		g_bufferWrite[2] = (BYTE)('h');
		break;
	case	4:	// PMT
		g_bufferWrite[2] = (BYTE)('j');
		break;
	}
	g_bufferWrite[3] = 0x0D;
	for( int i=0; i<10; i++ )
		g_bufferRead[i] = 0;
	WriteToDevice();

	Sleep( nSleep );
	// 接收回复
	// *hhh: where * is either C for Collector, E for EHT, B for Beam or H for Heater
	//		J for PMT or I for the Beam current monitor
	//		hhh is a 12 bit number representing the raw value to be sent to the DAC
	// Example: - Beam Current Monitor "?Ic"
	// Reply:	I7D0c = Beam current is 200uA
	CString strData = ReadFromDevice();
	// 分析数据
	ConvertReceiveData( strData );
	m_mutexPorts.Unlock();
}

void ReadRampRates( int nSleep )
{
	if( !g_Spellmans.status.bReady )
		return;

	ReadRampRate( 0, nSleep );
	ReadRampRate( 1, nSleep );
	ReadRampRate( 2, nSleep );
	ReadRampRate( 3, nSleep );
	ReadRampRate( 4, nSleep );
	PostMessage( g_hMain, WM_USER_SPELLMAN_Param2, 1, (LPARAM)(&(g_Spellmans.pM2)) );
	Message2();
}

void SetValue( int nIndex, int nValue )
{
	if( !g_Spellmans.status.bReady )
		return;

	m_mutexPorts.Lock();
	CString str;
	switch( nIndex )
	{
	case	0:	// Collector
		str.Format( "C%03X", nValue +2048 );
		break;
	case	1:	// EHT
		str.Format( "E%03X", nValue *10 );
		break;
	case	2:	// Beam
		str.Format( "B%03X", nValue );
		break;
	case	3:	// Heater
		str.Format( "H%03X", nValue );
		break;
	case	4:	// PMT
		str.Format( "J%03X", nValue );
		break;
	}
	g_bufferWrite[0] = 0;
	for( int i=0; i<4; i++ )
		g_bufferWrite[i+1] = (BYTE)(str[i]);
	g_bufferWrite[5] = 0x0D;
	WriteToDevice();
	m_mutexPorts.Unlock();
}

void SetRampRates( int nIndex, int nValue )
{
	if( !g_Spellmans.status.bReady )
		return;

	m_mutexPorts.Lock();
	CString str;
	switch( nIndex )
	{
	case	0:	// Collector
		str.Format( "c%03X", nValue );
		break;
	case	1:	// EHT
		str.Format( "e%03X", nValue );
		break;
	case	2:	// Beam
		str.Format( "b%03X", nValue );
		break;
	case	3:	// Heater
		str.Format( "h%03X", nValue );
		break;
	case	4:	// PMT
		str.Format( "j%03X", nValue );
		break;
	}
	g_bufferWrite[0] = 0;
	for( int i=0; i<4; i++ )
		g_bufferWrite[i+1] = (BYTE)(str[i]);
	g_bufferWrite[5] = 0x0D;
	WriteToDevice();
	m_mutexPorts.Unlock();
	ReadRampRate( nIndex, 10 );
}

void SetEnquiry( int nIndex, int nSleep )
{
	if( !g_Spellmans.status.bReady )
		return;

	m_mutexPorts.Lock();
	// 发送查询命令
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('?');
	switch( nIndex )
	{
	case	0:	// Collector voltage program
		g_bufferWrite[2] = (BYTE)('C');
		break;
	case	1:	// EHT voltage program
		g_bufferWrite[2] = (BYTE)('E');
		break;
	case	2:	// Beam current program
		g_bufferWrite[2] = (BYTE)('B');
		break;
	case	3:	// Heater current program
		g_bufferWrite[2] = (BYTE)('H');
		break;
	case	4:	// PMT voltage program
		g_bufferWrite[2] = (BYTE)('J');
		break;
	case	5:	// Beam current monitor
		g_bufferWrite[2] = (BYTE)('I');
		break;

	case	6:	// Heater current monitor
		g_bufferWrite[2] = (BYTE)('f');
		break;
	case	7:	// EHT voltage monitor
		g_bufferWrite[2] = (BYTE)('a');
		break;
	case	8:	// Collector voltage monitor
		g_bufferWrite[2] = (BYTE)('k');
		break;
	case	9:	// Scintillator voltage monitor
		g_bufferWrite[2] = (BYTE)('s');
		break;
	case	10:	// PMT voltage monitor
		g_bufferWrite[2] = (BYTE)('p');
		break;
	}
	g_bufferWrite[3] = 0x0D;
	for( int i=0; i<10; i++ )
		g_bufferRead[i] = 0;
	WriteToDevice();

	Sleep( nSleep );
	// 接收回复
	// *hhh: where * is either C for Collector, E for EHT, B for Beam or H for Heater
	//		J for PMT or I for the Beam current monitor
	//		hhh is a 12 bit number representing the raw value to be sent to the DAC
	// Example: - Beam Current Monitor "?Ic"
	// Reply:	I7D0c = Beam current is 200uA
	CString strData = ReadFromDevice();
	// 分析数据
	ConvertReceiveData( strData );
	m_mutexPorts.Unlock();
}

int SetEnquiryStatus( int nSleep )
{
	if( !g_Spellmans.status.bReady )
		return -1;

	// On/Off/Status Enquiry - "?Oc"
	// Command:		?O
	// Response:	OhhHHHHH
	// Where	hh	represents the On/Off commands as Collector/Scintillator/EHT/Heater/PMT
	//			HH	represents the status register as below
	//				18:	Filament Timeout
	//				17:	Filament Regulation Trip
	//				16:	PMT Regulation Trip
	//				15:	Scintillator Regulation Trip
	//				14:	Collector Regulation Trip
	//				13:	EHT Regulation Trip
	//				12:	Internal Rail Fault
	//				11:	Scintillator interlock
	//				10:	Collector interlock
	//				9:	Accelerator Over Voltage Trip
	//				8:	Accelerator Over Current Trip
	//				7:	PMT interlock
	//				6:	Over Temperature Trip
	//				5:	Arc has occurred
	//				4:	Trip (EHT Tripped Off), (4 sen of over current or 16 Arc groups)
	//				3:	Heater Open Circuit
	//				2:	EHT interlock
	//				1:	Vacuum interlock
	//				0:	Power on reset
	m_mutexPorts.Lock();
	// 发送查询命令
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('?');
	g_bufferWrite[2] = (BYTE)('O');
	g_bufferWrite[3] = 0x0D;
	for( int i=0; i<10; i++ )
		g_bufferRead[i] = 0;
	WriteToDevice();

	Sleep( nSleep );

	// 接收回复
	CString strData = ReadFromDevice();
	// 分析数据
	int nRet;
	if( ConvertReceiveData( strData ) == 3 )
		nRet = 0;
	else
		nRet = 1;
	m_mutexPorts.Unlock();
	return nRet;
}

int SetEnquiryID()
{
	if( !g_Spellmans.status.bReady )
		return -1;

	// Request for identification
	// Gun supply identification - "?Gc"
	// Command:		?G	(return code for Type identification)
	// Response:	GEBM30N12_771
	// 发送查询命令
	m_mutexPorts.Lock();
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('?');
	g_bufferWrite[2] = (BYTE)('G');
	g_bufferWrite[3] = 0x0D;
	for( int i=0; i<10; i++ )
		g_bufferRead[i] = 0;
	WriteToDevice();

	Sleep( 10 );

	// 接收回复
	CString strData = ReadFromDevice();
	// 分析数据
	int nRet;
	if( ConvertReceiveData( strData ) == 1 )
	{
		if( g_Spellmans.status.strID == "EBM30N12" )
			nRet = 0;
		else
			nRet = 1;
	}
	else
		nRet = 2;
	m_mutexPorts.Unlock();
	return nRet;
}

int SetEnquiryVer()
{
	if( !g_Spellmans.status.bReady )
		return -1;

	// Request for identification
	// Software Version identification - "?Vc"
	// Command:		?V	(return code for software Version identification)
	// Response:	V01.0c = A particular Version

	m_mutexPorts.Lock();
	// 发送查询命令
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('?');
	g_bufferWrite[2] = (BYTE)('V');
	g_bufferWrite[3] = 0x0D;
	for( int i=0; i<10; i++ )
		g_bufferRead[i] = 0;
	WriteToDevice();

	Sleep( 10 );

	// 接收回复
	CString strData = ReadFromDevice();
	// 分析数据
	int nRet;
	if( ConvertReceiveData( strData ) == 2 )
		nRet = 0;
	else
		nRet = 1;
	m_mutexPorts.Unlock();
	return nRet;
}

void SetSwitch( int nIndex, int nSwitch )
{
	if( !g_Spellmans.status.bReady )
		return;

	// On/Off Command - "Ohhc"
	// Command:		Ohh		Where h is the ASCII representation of a hexadecimal number
	// Response:	None
	// Where h is the ASCII representation of a hexadecimal number
	//				7:	X
	//				6:	X
	//				5:	X
	//				4:	PMT
	//				3:	Heater
	//				2:	EHT
	//				1:	Scintillator
	//				0:	Collector
	// 0 indicates that the supply is OFF
	// 1 indicates that the supply is ON
	BYTE bSwitchSet = (g_Spellmans.status.bPMT << 4) | (g_Spellmans.status.bHeater << 3) | (g_Spellmans.status.bEHT << 2) | (g_Spellmans.status.bScint << 1) | g_Spellmans.status.bCollector;
	BYTE bSwitch = g_Spellmans.status.bstatusSwitch;
	if( nSwitch > 0 )
	{
		// 打开开关
		bSwitch |= (1<<nIndex);
	}
	else
	{
		// 关闭开关
		bSwitch &= (~(1<<nIndex));
	}
	if( bSwitchSet == bSwitch )
		return;

	CString str;
	str.Format( "%02X", bSwitch );
	int nTimes = 0;
	BOOL bRet = FALSE;
	// 更新开关设置
	do{
		m_mutexPorts.Lock();
		g_bufferWrite[0] = 0;
		g_bufferWrite[1] = (BYTE)('O');
		g_bufferWrite[2] = (BYTE)(str[0]);
		g_bufferWrite[3] = (BYTE)(str[1]);
		g_bufferWrite[4] = 0x0D;
		bRet = WriteToDevice();
		m_mutexPorts.Unlock();
		if( bRet )
			break;
		nTimes++;
	} while( nTimes < 10 );
	if( nTimes == 10 )
		return;
	Sleep(50);
	// 再检测开关状态
	nTimes = 0;
	do{
		SetEnquiryStatus(20);
		if( g_Spellmans.status.bstatusSwitch == bSwitchSet )
			break;
		nTimes++;
	} while( nTimes < 20 );
//	if( nTimes != 20 )
//		PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 4, 0 );
		PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 4, (LPARAM)(&(g_Spellmans.status)) );
}

void ClearSwitches()
{
	if( !g_Spellmans.status.bReady )
		return;

	if( g_Spellmans.status.bstatusSwitch == 0x00 )
		return;

	m_mutexPorts.Lock();
	// 开关清零
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('O');
	g_bufferWrite[2] = (BYTE)('0');
	g_bufferWrite[3] = (BYTE)('0');
	g_bufferWrite[4] = 0x0D;
	WriteToDevice();
	m_mutexPorts.Unlock();
}

void OpenSwitches()
{
	if( !g_Spellmans.status.bReady )
		return;

	if( g_Spellmans.status.bstatusSwitch == 0x1F )
		return;

	BYTE bSwitchSet = 0x1F;
	int nTimes = 0;
	BOOL bRet = FALSE;
	// 更新开关设置
	do{
		m_mutexPorts.Lock();
		g_bufferWrite[0] = 0;
		g_bufferWrite[1] = (BYTE)('O');
		g_bufferWrite[2] = (BYTE)('1');
		g_bufferWrite[3] = (BYTE)('F');
		g_bufferWrite[4] = 0x0D;
		bRet = WriteToDevice();
		m_mutexPorts.Unlock();
		if( bRet )
			break;
		nTimes++;
	} while( nTimes < 10 );
	// 再检测开关状态
	nTimes = 0;
	do{
		SetEnquiryStatus(20);
		if( g_Spellmans.status.bstatusSwitch == bSwitchSet )
			break;
		nTimes++;
	} while( nTimes < 10 );
//	PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 4, 0 );
	PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 4, (LPARAM)(&(g_Spellmans.status)) );
}

void Unlock5KV()
{
	if( !g_Spellmans.status.bReady )
		return;

	m_mutexPorts.Lock();
	// 解除5KV的限制
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('G');
	g_bufferWrite[2] = (BYTE)('P');
	g_bufferWrite[3] = (BYTE)('0');
	g_bufferWrite[4] = 0x0D;
	WriteToDevice();
	m_mutexPorts.Unlock();
}

void ConvertCommand( CString strCmd )
{
	if( !g_Spellmans.status.bReady )
		return;

	int i, nLen = strCmd.GetLength();
	if( nLen > BUFFER_LEN )
		return;
	m_mutexPorts.Lock();
	g_bufferWrite[0] = 0;
	for( i=0; i<nLen; i++ )
	{
		char ch = strCmd[i];
		g_bufferWrite[i+1] = (BYTE)ch;
	}
	g_bufferWrite[i+1] = 0x0D;
	WriteToDevice();
	m_mutexPorts.Unlock();
}

void Reset771()
{
	if( !g_Spellmans.status.bReady )
		return;

	m_mutexPorts.Lock();
	// 复位设备
	g_bufferWrite[0] = 0;
	g_bufferWrite[1] = (BYTE)('R');
	g_bufferWrite[2] = 0x0D;
	WriteToDevice();
	m_mutexPorts.Unlock();
}

void CommWithSpellman( int nSleep )
{
	if( !g_Spellmans.status.bReady )
		return;

	SetEnquiry( 0, nSleep );
	SetEnquiry( 1, nSleep );
	SetEnquiry( 2, nSleep );
	SetEnquiry( 3, nSleep );
	SetEnquiry( 4, nSleep );
	SetEnquiry( 5, nSleep );
	SetEnquiry( 6, nSleep );
	SetEnquiry( 7, nSleep );
	SetEnquiry( 8, nSleep );
	SetEnquiry( 9, nSleep );
	SetEnquiry( 10, nSleep );
	PostMessage( g_hMain, WM_USER_SPELLMAN_Param, 0, (LPARAM)(&(g_Spellmans.pM)) );
	PostMessage( g_hMain, WM_USER_SPELLMAN_Param3, 0, (LPARAM)(&(g_Spellmans.pM3)) );

	// 2s查看一次状态值
	DWORD thisTickCount = GetTickCount();
//	if( thisTickCount -g_lastMon > 2000 )
	{
		// 备份
 		BYTE switchBak	 = g_Spellmans.status.bstatusSwitch;
 		DWORD registerBak = g_Spellmans.status.dwstatusRegister;
		if( SetEnquiryStatus( nSleep ) < 1 )
		{
			// 状态有变化才向主程序投递更新消息
	 		if( (switchBak != g_Spellmans.status.bstatusSwitch)
	 			|| (registerBak != g_Spellmans.status.dwstatusRegister) )
//	 			PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 4, 0 );
				PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 4, (LPARAM)(&(g_Spellmans.status)) );
		}
		g_lastMon = GetTickCount();
	}
	Message();
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Auto Functions
void Auto_Rise_EHT1()
{
	g_Spellmans.pS.dVE += g_dblAutoStep_EHT1;
	if( g_Spellmans.pS.dVE >= g_dblAutoRated_EHT2 )
	{
		g_Spellmans.pS.dVE = g_dblAutoRated_EHT2;
		SetValue( 1, (int)(g_Spellmans.pS.dVE *100 /10) );
		PostMessage( g_hMain, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)(g_Spellmans.pS.dVE*10) );
		g_AutoState = AS2_Rise_Heater;
	}
	else if( g_Spellmans.pS.dVE >= g_dblAutoRated_EHT1 )
	{
		g_Spellmans.pS.dVE = g_dblAutoRated_EHT1;
		SetValue( 1, (int)(g_Spellmans.pS.dVE *100 /10) );
		PostMessage( g_hMain, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)(g_Spellmans.pS.dVE*10) );
		g_AutoState = AS2_Rise_EHT2;
	}
	else
	{
		SetValue( 1, (int)(g_Spellmans.pS.dVE *100 /10) );
		PostMessage( g_hMain, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)(g_Spellmans.pS.dVE*10) );
	}
}

void Auto_Rise_EHT2()
{
	g_Spellmans.pS.dVE += g_dblAutoStep_EHT2;
	if( g_Spellmans.pS.dVE >= g_dblAutoRated_EHT2 )
	{
		g_Spellmans.pS.dVE = g_dblAutoRated_EHT2;
		SetValue( 1, (int)(g_Spellmans.pS.dVE *100 /10) );
		PostMessage( g_hMain, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)(g_Spellmans.pS.dVE*10) );
		g_AutoState = AS2_Rise_Heater;
	}
	else
	{
		SetValue( 1, (int)(g_Spellmans.pS.dVE *100 /10) );
		PostMessage( g_hMain, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)(g_Spellmans.pS.dVE*10) );
	}
}

void Auto_Rise_Beam()
{
	g_Spellmans.pS.dIB += g_dblAutoStep_Beam;
	if( g_Spellmans.pS.dIB >= g_dblAutoRated_Beam )
	{
		g_Spellmans.pS.dIB = g_dblAutoRated_Beam;
		SetValue( 2, (int)(g_Spellmans.pS.dIB *10) );
		PostMessage( g_hMain, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)g_Spellmans.pS.dIB );
		g_AutoState = AS2_NULL;	// 结束
		PostMessage( g_hMain, WM_USER_SPELLMAN_AutoParam, g_AutoState, 0 );
	}
	else
	{
		SetValue( 2, (int)(g_Spellmans.pS.dIB *10) );
		PostMessage( g_hMain, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)g_Spellmans.pS.dIB );
	}
}

void Auto_Rise_Heater()
{
	g_Spellmans.pS.dIH += g_dblAutoStep_Heater;
	if( g_Spellmans.pS.dIH >= g_dblAutoRated_Heater )
	{
		g_Spellmans.pS.dIH = g_dblAutoRated_Heater;
		SetValue( 3, (int)(g_Spellmans.pS.dIH *1000) );
		PostMessage( g_hMain, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)(g_Spellmans.pS.dIH*1000) );
		g_AutoState = AS2_Rise_Beam;
	}
	else
	{
		SetValue( 3, (int)(g_Spellmans.pS.dIH *1000) );
		PostMessage( g_hMain, WM_USER_SPELLMAN_AutoParam, g_AutoState, (LPARAM)(g_Spellmans.pS.dIH*1000) );
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
	case	AS2_Rise_EHT1:
		if( thisTickCount - g_lastAuto > 200 )
		{
			Auto_Rise_EHT1();
			g_lastAuto = GetTickCount();
		}
		break;
	case	AS2_Rise_EHT2:
		if( thisTickCount - g_lastAuto > 400 )
		{
			Auto_Rise_EHT2();
			g_lastAuto = GetTickCount();
		}
		break;
	case	AS2_Rise_Beam:
		if( thisTickCount - g_lastAuto > 400 )
		{
			Auto_Rise_Beam();
			g_lastAuto = GetTickCount();
		}
		break;
	case	AS2_Rise_Heater:
		if( thisTickCount - g_lastAuto > 400 )
		{
			Auto_Rise_Heater();
			g_lastAuto = GetTickCount();
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


DWORD WINAPI CommThread(LPVOID lpParam)
{
	g_strDeviceName = HID_OpenHIDDevice();
	if( g_strDeviceName == "" )
	{
		g_Spellmans.status.bReady = FALSE;
		return 0;
	}

	if( HID_InitHIDDevice( g_strDeviceName ) )
		g_Spellmans.status.bReady = TRUE;

	if( g_Spellmans.status.bReady )
	{
		/////////////////////////////////////////////////////////
		// Request for identification
		// Gun supply identification - "?Gc"
		// Command:		?G	(return code for Type identification)
		// Response:	EBM30N12
		m_mutexPorts.Lock();
		g_bufferWrite[1] = (BYTE)('?');
		g_bufferWrite[2] = (BYTE)('G');
		g_bufferWrite[3] = 0x0D;

		DWORD nTimes = 0;
		CString strAck = "GEBM30N12";
		do{
			for(int i=0; i<10; i++ )
				g_bufferRead[i] = 0;
			WriteToDevice();

			Sleep(100);
			CString strData = ReadFromDevice();
//			if( strData == strAck )
			if( strData.Find(strAck) > -1 )
			{
				ConvertReceiveData( strData );
				break;
			}
			nTimes++;
		} while( nTimes < 10 );
		m_mutexPorts.Unlock();

		if( nTimes == 10 )
			g_Spellmans.status.bReady	= FALSE;
		else
			g_Spellmans.status.bReady	= TRUE;

		/////////////////////////////////////////////////////////
		// Request for identification
		// Software Version identification - "?Vc"
		// Command:		?V	(return code for software Version identification)
		// Response:	V01.0c = A particular Version
		m_mutexPorts.Lock();
		// 发送查询命令
		g_bufferWrite[0] = 0;
		g_bufferWrite[1] = (BYTE)('?');
		g_bufferWrite[2] = (BYTE)('V');
		g_bufferWrite[3] = 0x0D;
		for(int i=0; i<10; i++ )
			g_bufferRead[i] = 0;
		WriteToDevice();

		Sleep( 10 );
		// 接收回复
		CString strData = ReadFromDevice();
		// 分析数据
		ConvertReceiveData( strData );
		m_mutexPorts.Unlock();
		Reset771();
	}
	PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 1, g_Spellmans.status.bReady );

	g_AutoState	= AS2_NULL;
	if( g_Spellmans.status.bReady )
	{
		// 日志文件
		TCHAR exeFullPath[255];
		::GetModuleFileName( NULL, exeFullPath, 255 );
		char *ptemp = strrchr( exeFullPath, '\\');
		ptemp[1] = 0;
		
		CString str;
		CTime	timeCurrent = CTime::GetCurrentTime();
		CString strTime = timeCurrent.Format("%Y%m%d%H%M");
		str.Format( "%s\\Log\\Spell771-%s.csv", exeFullPath, strTime );
		if( g_hFile != NULL )
			CloseHandle( g_hFile );
		g_hFile	= NULL;
		g_hFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		g_nLoops = 1;
		// 写日志文件头
		if( g_hFile != NULL )
		{
			unsigned long cbRet;
			char Buf[255];
			str.Format( "\t%s\r\n", "EBM30N12/771" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFile, Buf, strlen(Buf), &cbRet, NULL );
		}
		// 先查询一次所有加速度
		int nSleep = 20;
		ReadRampRates( nSleep );
		///////////////////////////////////////////////////
		// For test
// 		SetRampRates( 0, 640 );	// Collector 800, 160V/s
// 		SetRampRates( 1, 480 );	// EHT 600, 6000V/s
// 		SetRampRates( 2, 640 );	// Beam 800, 80uA/s
// 		SetRampRates( 3, 640 );	// Heater 800, 0.8A/s
// 		SetRampRates( 4, 400 );	// PMT 788, 260V/s
// 		PostMessage( g_hMain, WM_USER_SPELLMAN_Param2, 1, (LPARAM)(&(g_Spellmans.pM2)) );
// 		Message2();
// 		ReadRampRates( nSleep );
		///////////////////////////////////////////////////
		if( g_hFile != NULL )
		{
			unsigned long cbRet;
			char Buf[255];
//			str.Format( "\tSwitch\tStatus\tCollector s(V)\tCollector m(V)\tEHT s(KV)\tEHT m(KV)\tBeam s(uA)\tBeam m(uA)\tHeat s(A)\tHeat m(A)\tPMT s(V)\tPMT m(V)\tBeam Current(uA)" );
//			str.Format( ",Switch,Status,Collector s(V),Collector m(V),EHT s(KV),EHT m(KV),Beam s(uA),Beam m(uA),Heat s(A),Heat m(A),PMT s(V),PMT m(V),Beam Current(uA)" );
			str.Format( "Time, Switch,Status, C s(V),C m(V),C m2(V), E s(V),E m(V),E m2(V), B s(uA),B m(uA),S m(V), H s(A),H m(A),H m2(A), P s(V),P m(V),P m2(V), Beam Current(uA)" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFile, Buf, strlen(Buf), &cbRet, NULL );
		}
		// 同步一次所有参数值
		if( SetEnquiryStatus( nSleep ) < 1 )
//			PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 4, 0 );
			PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 4, (LPARAM)(&(g_Spellmans.status)) );
		SetEnquiry( 0, nSleep );
		SetEnquiry( 1, nSleep );
		SetEnquiry( 2, nSleep );
		SetEnquiry( 3, nSleep );
		SetEnquiry( 4, nSleep );
		SetEnquiry( 5, nSleep );
		PostMessage( g_hMain, WM_USER_SPELLMAN_Param, 1, (LPARAM)(&(g_Spellmans.pM)) );
		Message();
		g_lastMon	= GetTickCount();
		g_lastAuto	= GetTickCount();
	}
	
	while( g_Spellmans.status.bReady && WaitForSingleObject(g_hEventTeminate,200) != WAIT_OBJECT_0 )
	{
		if( !g_bPauseComm )
		{
			if( g_AutoState < AS2_NULL )
			{
				AutoProc();
//				Sleep(1);
			}
			// 500ms通讯一次
			CommWithSpellman(20);
		}
		Sleep( 1 );
	}
	
	g_Spellmans.status.bReady	= FALSE;
	if( g_hFile != NULL )
		CloseHandle( g_hFile );
	g_hFile	= NULL;
	
	SetEvent( g_hEvent );
	return 0;
}

void CCommSpellman::SetParam( int nIndex, int nValue )
{
	switch( nIndex )
	{
	case	11:
		g_bPauseComm	= (BOOL)nValue;
		break;
	case	12:
		g_bPausePost	= (BOOL)nValue;
		break;
	case	13:
		g_bPauseInquire	= (BOOL)nValue;
		break;

	case	16:
		g_AutoState		= (Spellman_AutoState)nValue;
		if( g_AutoState == AS2_Rise_EHT1 && g_Spellmans.pS.dVE > g_dblAutoRated_EHT1 )
			g_AutoState = AS2_Rise_EHT2;
		else if( g_AutoState == AS2_Rise_EHT1 && ((int)g_Spellmans.pS.dVE == (int)g_dblAutoRated_EHT2) )
			g_AutoState = AS2_Rise_Heater;
		break;

	case	17:
		g_dblAutoRated_EHT2		= nValue /1000.0;
		break;
	case	18:
		g_dblAutoRated_Beam		= nValue;
		break;
	case	19:
		g_dblAutoRated_Heater	= nValue /100.0;
		break;

	case	21:	// Collector
	case	22:	// Scint
	case	23:	// EHT
	case	24:	// Heater
	case	25:	// PMT
		SetSwitch( nIndex -21, nValue );
		break;

	case	31:	// Collector
		g_Spellmans.pS.dVC	= nValue *0.2;
		SetValue( nIndex -31, nValue );
		break;
	case	32:	// EHT
		g_Spellmans.pS.dVE	= nValue *10.0 /100;
		SetValue( nIndex -31, nValue );
		break;
	case	33:	// Beam
		g_Spellmans.pS.dIB	= nValue *0.1;
		SetValue( nIndex -31, nValue );
		break;
	case	34:	// Heater
		g_Spellmans.pS.dIH	= nValue *0.001;
		SetValue( nIndex -31, nValue );
		break;
	case	35:	// PMT
		g_Spellmans.pS.dVP	= nValue *g_dConstStep_PMT;
		SetValue( nIndex -31, nValue );
		break;

	case	41:	// Collector
	case	42:	// EHT
	case	43:	// Beam
	case	44:	// Heater
	case	45:	// PMT
		SetRampRates( nIndex -41, nValue );
		break;

	case	51:
		SetEnquiryID();
		break;
	case	52:
		SetEnquiryVer();
		break;
	case	53:
		if( SetEnquiryStatus( 10 ) < 1 )
//			PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 4, 0 );
			PostMessage( g_hMain, WM_USER_SPELLMAN_Status, 4, (LPARAM)(&(g_Spellmans.status)) );
		break;
	case	54:
		ReadRampRates( 10 );
		break;

	case	55:
		Unlock5KV();
		break;
	case	56:
		Reset771();
		break;

	case	61:
		ClearSwitches();
		break;
	case	62:
		OpenSwitches();
		break;
	}
}

void CCommSpellman::SetCommand( CString strCmd )
{
	ConvertCommand(strCmd);
}

void CCommSpellman::SetSharedParam( void* pParam )
{
//	g_pParam	= (KYKYParam*)pParam;
}

BOOL CCommSpellman::Init( HWND hWnd )
{
	g_hMain = hWnd;

	g_strSpellman		= "";
	g_strSpellmanSaved	= "";

	/////////////////////////////////////////////////////////////////
	ResetEvent( g_hEvent );
	ResetEvent( g_hEventTeminate );

	g_hThread	= CreateThread( NULL, 0, CommThread, NULL, 0, &g_dwThread );
	if( g_hThread == NULL )
		return FALSE;

	CloseHandle(g_hThread);
	/////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL CCommSpellman::Close()
{
	if( g_Spellmans.status.bReady )
	{
		SetEvent( g_hEventTeminate );
		WaitForSingleObject( g_hEvent, INFINITE );
		g_Spellmans.status.bReady	= FALSE;
	}
	HID_CloseHIDDevice();
	return TRUE;
}
