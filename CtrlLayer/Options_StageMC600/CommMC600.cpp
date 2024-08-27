// CommMC600.cpp : implementation file
//

#include "stdafx.h"
#include "CommMC600.h"

#include "Serial.h"
#include <afxmt.h>
#include <Comm_MC600.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCriticalSection		m_mutexBox600;

#define nBox600_Sleep		20
#define	nRet_OK				100
#define nRet_Ready			101
#define nRet_E01			102
#define nRet_E02			103

DWORD WINAPI CommThreadBox600(LPVOID lpParam);

HANDLE	g_hThreadBox600;
DWORD	g_dwThreadBox600;
HANDLE	g_hEventWorkBox600		= CreateEvent(NULL,FALSE, FALSE, _T("WORKbox600"));
HANDLE	g_hEventTeminateBox600	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEbox600"));

HWND	g_hMain;
CSerial	g_Box600;
char	g_szBox600Saved[255];

HANDLE	g_hFileBox600 = NULL;
HANDLE	g_hFileTest	= NULL;
int		g_nLoopsBox600, g_nLoopsTest600;
DWORD	g_lastMon600	= 0;

BOOL	g_bBox600Open		= FALSE;	// 是否至少有一个设备联通

// long	g_lSpeedX, g_lSpeedY, g_lSpeedZ;
// long	g_lSpeedAccX, g_lSpeedAccY, g_lSpeedAccZ;
// long	g_lSpeedInitX, g_lSpeedInitY, g_lSpeedInitZ;
// long	g_lStepX, g_lStepY, g_lStepZ;

MC600_Params_Sys	g_ParamSys;
MC600_Params_Speed	g_ParamSpeed;
MC600_Params_Pos	g_ParamPos;
MC600_Params_Bl		g_ParamBl;
BOOL				g_bBacklash[4];
int					g_nDirX, g_nDirY;

int		Box600_WriteToDevice( int nSleep, char* data );
int		Box600_ReadFromDevice( int nSleep, char* data );
int		Box600_ConvertReceiveData( char* data );
int		Box600_Convert_Position( char* szAxis, char* szVal );
int		Box600_Convert_SetUserOrigin( char* szAxis, char* szVal );
int		Box600_Convert_Speed( char* szAxis, char* szVal );
int		Box600_Convert_SpeedInit( char* szAxis, char* szVal );
int		Box600_Convert_SpeedAcc( char* szAxis, char* szVal );
int		Box600_Convert_SpeedHome( char* szAxis, char* szVal );
int		Box600_Convert_StagePitch( char* szAxis, char* szVal );
int		Box600_Convert_StageSteps( char* szAxis, char* szVal );
int		Box600_Convert_StageRat( char* szAxis, char* szVal );
int		Box600_Convert_StageDistance( char* szAxis, char* szVal );
void	Box600_UpdateDir( unsigned char ucAxis, long lSteps );
void	Box600_AfterStop( int nRet );
int		Box600_SendCmd( int nSleep, char* cmd );
void	InitBox600Params( int nSleep );
int		CommWithBox600( int nSleep );

/////////////////////////////////////////////////////////////////////////////
// CCommMC600

CCommMC600::CCommMC600()
{
	g_hMain	= NULL;
// 	g_lSpeedX = g_lSpeedY = g_lSpeedZ = 0;
// 	g_lSpeedAccX = g_lSpeedAccY = g_lSpeedAccZ = 0;
// 	g_lSpeedInitX = g_lSpeedInitY = g_lSpeedInitZ = 0;
// 	g_lStepX = g_lStepY = g_lStepZ = 0;
	for( int i=0; i<4; i++ )
	{
		g_ParamSpeed.fSpeed[i]		= 0.0;
		g_ParamSpeed.fSpeedInit[i]	= 0.0;
		g_ParamSpeed.fSpeedAcc[i]	= 0.0;
		g_ParamSpeed.fSpeedHome[i]	= 0.0;
		g_ParamPos.fSteps[i]		= 0.0;
		g_ParamPos.fOrigin[i]		= 0.0;
		g_ParamBl.nDir[i]			= 0;
		g_ParamBl.nCount[i]			= 0;
		g_bBacklash[i]				= FALSE;
	}
	g_nDirX = 0;
	g_nDirY = 0;
}

CCommMC600::~CCommMC600()
{
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
int Box600_WriteToDevice( int nSleep, char* data )
{
	if( !g_bBox600Open )
		return -1;
	int nLength = strlen(data);
	int nLenSend = g_Box600.SendData(data, nLength);
	Sleep(nSleep);
	return nLenSend;
}

int Box600_ReadFromDevice( int nSleep, char* data )
{
	if( !g_bBox600Open )
		return -1;

	int nLen, nLenRecv = 0;
	strcpy( data, g_szBox600Saved );
	nLenRecv = strlen(data);
	g_szBox600Saved[0] = '\0';

	char buf[255];
	int nIndex = 0;
	do{
		Sleep(10);
		nLen = g_Box600.ReadData( buf, 100 );
		buf[nLen] = '\0';
		strcat( data, buf );
		nLenRecv += nLen;
		nIndex++;
	}while(	(strrchr(buf, '\r') == NULL) && nIndex < 10 );
	return nLenRecv;
}

int Box600_Convert_Position( char* szAxis, char* szVal )
{
	float fSteps = (float)atof( szVal );
	int nRet = -1;
	if( szAxis[0] == 'X' )
	{
		g_ParamPos.fSteps[0] = fSteps;
		nRet = 11;
	}
	else if( szAxis[0] == 'Y' )
	{
		g_ParamPos.fSteps[1] = fSteps;
		nRet = 12;
	}
	else if( szAxis[0] == 'Z' )
	{
		g_ParamPos.fSteps[2] = fSteps;
		nRet = 13;
	}
	else if( szAxis[0] == 'T' )
	{
		g_ParamPos.fSteps[3] = fSteps;
		nRet = 14;
	}
	return nRet;
}

int Box600_Convert_SetUserOrigin( char* szAxis, char* szVal )
{
	float fOrigin = (float)atof( szVal );
	int nRet = -1;
	if( szAxis[0] == 'X' )
	{
		g_ParamPos.fOrigin[0] = fOrigin;
		nRet = 15;
	}
	else if( szAxis[0] == 'Y' )
	{
		g_ParamPos.fOrigin[1] = fOrigin;
		nRet = 16;
	}
	else if( szAxis[0] == 'Z' )
	{
		g_ParamPos.fOrigin[2] = fOrigin;
		nRet = 17;
	}
	else if( szAxis[0] == 'T' )
	{
		g_ParamPos.fOrigin[3] = fOrigin;
		nRet = 18;
	}
	return nRet;
}

int Box600_Convert_Speed( char* szAxis, char* szVal )
{
	float fSpeed = (float)atof( szVal );
	int nRet = -1;
	if( szAxis[0] == 'X' )
	{
		g_ParamSpeed.fSpeed[0] = fSpeed;
		nRet = 20;
	}
	else if( szAxis[0] == 'Y' )
	{
		g_ParamSpeed.fSpeed[1] = fSpeed;
		nRet = 21;
	}
	else if( szAxis[0] == 'Z' )
	{
		g_ParamSpeed.fSpeed[2] = fSpeed;
		nRet = 22;
	}
	else if( szAxis[0] == 'T' )
	{
		g_ParamSpeed.fSpeed[3] = fSpeed;
		nRet = 23;
	}
	return nRet;
}

int Box600_Convert_SpeedInit( char* szAxis, char* szVal )
{
	float fSpeed = (float)atof( szVal );
	int nRet = -1;
	if( szAxis[0] == 'X' )
	{
		g_ParamSpeed.fSpeedInit[0] = fSpeed;
		nRet = 25;
	}
	else if( szAxis[0] == 'Y' )
	{
		g_ParamSpeed.fSpeedInit[1] = fSpeed;
		nRet = 26;
	}
	else if( szAxis[0] == 'Z' )
	{
		g_ParamSpeed.fSpeedInit[2] = fSpeed;
		nRet = 27;
	}
	else if( szAxis[0] == 'T' )
	{
		g_ParamSpeed.fSpeedInit[3] = fSpeed;
		nRet = 28;
	}
	return nRet;
}

int Box600_Convert_SpeedAcc( char* szAxis, char* szVal )
{
	float fSpeed = (float)atof( szVal );
	int nRet = -1;
	if( szAxis[0] == 'X' )
	{
		g_ParamSpeed.fSpeedAcc[0] = fSpeed;
		nRet = 30;
	}
	else if( szAxis[0] == 'Y' )
	{
		g_ParamSpeed.fSpeedAcc[1] = fSpeed;
		nRet = 31;
	}
	else if( szAxis[0] == 'Z' )
	{
		g_ParamSpeed.fSpeedAcc[2] = fSpeed;
		nRet = 32;
	}
	else if( szAxis[0] == 'T' )
	{
		g_ParamSpeed.fSpeedAcc[3] = fSpeed;
		nRet = 33;
	}
	return nRet;
}

int Box600_Convert_SpeedHome( char* szAxis, char* szVal )
{
	float fSpeed = (float)atof( szVal );
	int nRet = -1;
	if( szAxis[0] == 'X' )
	{
		g_ParamSpeed.fSpeedHome[0] = fSpeed;
		nRet = 35;
	}
	else if( szAxis[0] == 'Y' )
	{
		g_ParamSpeed.fSpeedHome[1] = fSpeed;
		nRet = 36;
	}
	else if( szAxis[0] == 'Z' )
	{
		g_ParamSpeed.fSpeedHome[2] = fSpeed;
		nRet = 37;
	}
	else if( szAxis[0] == 'T' )
	{
		g_ParamSpeed.fSpeedHome[3] = fSpeed;
		nRet = 38;
	}
	return nRet;
}


int Box600_Convert_StagePitch( char* szAxis, char* szVal )
{
	float fPitch = (float)atof( szVal );
	int nRet = -1;
	if( szAxis[0] == 'X' )
	{
		g_ParamSys.fPitch[0] = fPitch;
		nRet = 41;
	}
	else if( szAxis[0] == 'Y' )
	{
		g_ParamSys.fPitch[1] = fPitch;
		nRet = 42;
	}
	else if( szAxis[0] == 'Z' )
	{
		g_ParamSys.fPitch[2] = fPitch;
		nRet = 43;
	}
	else if( szAxis[0] == 'T' )
	{
		g_ParamSys.fPitch[3] = fPitch;
		nRet = 44;
	}
	return nRet;
}

int Box600_Convert_StageSteps( char* szAxis, char* szVal )
{
	float fSteps = (float)atof( szVal );
	int nRet = -1;
	if( szAxis[0] == 'X' )
	{
		g_ParamSys.fStepsRev[0] = fSteps;
		nRet = 45;
	}
	else if( szAxis[0] == 'Y' )
	{
		g_ParamSys.fStepsRev[1] = fSteps;
		nRet = 46;
	}
	else if( szAxis[0] == 'Z' )
	{
		g_ParamSys.fStepsRev[2] = fSteps;
		nRet = 47;
	}
	else if( szAxis[0] == 'T' )
	{
		g_ParamSys.fStepsRev[3] = fSteps;
		nRet = 48;
	}
	return nRet;
}

int Box600_Convert_StageRat( char* szAxis, char* szVal )
{
	float fRat = (float)atof( szVal );
	int nRet = -1;
	if( szAxis[0] == 'X' )
	{
		g_ParamSys.fRat[0] = fRat;
		nRet = 51;
	}
	else if( szAxis[0] == 'Y' )
	{
		g_ParamSys.fRat[1] = fRat;
		nRet = 52;
	}
	else if( szAxis[0] == 'Z' )
	{
		g_ParamSys.fRat[2] = fRat;
		nRet = 53;
	}
	else if( szAxis[0] == 'T' )
	{
		g_ParamSys.fRat[3] = fRat;
		nRet = 54;
	}
	return nRet;
}

int Box600_Convert_StageDistance( char* szAxis, char* szVal )
{
	float fDistance = (float)atof( szVal );
	int nRet = -1;
	if( szAxis[0] == 'X' )
	{
		g_ParamSys.fDistance[0] = fDistance;
		nRet = 55;
	}
	else if( szAxis[0] == 'Y' )
	{
		g_ParamSys.fDistance[1] = fDistance;
		nRet = 56;
	}
	else if( szAxis[0] == 'Z' )
	{
		g_ParamSys.fDistance[2] = fDistance;
		nRet = 57;
	}
	else if( szAxis[0] == 'T' )
	{
		g_ParamSys.fDistance[3] = fDistance;
		nRet = 58;
	}
	return nRet;
}

int Box600_Convert_SetUserPositiveLimit( char* szAxis, char* szVal )
{
	int nRet = -1;
	return nRet;
}

int Box600_Convert_SetUserNegtiveLimit( char* szAxis, char* szVal )
{
	int nRet = -1;
	return nRet;
}

int Box600_ConvertReceiveData( char* data )
{
	int nLen = strlen(data);
	if( nLen < 2 )
	{
		strcpy( g_szBox600Saved, data );
		return -1;	// 不够一帧数据
	}

// 	int nCount = 1;//nLen /byte_LvLenRec;	// 一次收到多少帧数据
// 	int i = 0;
// 	CString str;
// 	for( int j=0; j<nCount; j++ )
// 	{
// 	}
	char szBuf[255], szAxis[5];
	// 去掉回车符
	if( strchr(data, '\r') != NULL )
	{
//		sprintf( szBuf, "Split 2 = %s", strchr( data, '\r\n' )+1 );
		strcpy( g_szBox600Saved, strchr(data, '\r')+1 );
		sprintf( szBuf, "Split 2 = %s", g_szBox600Saved );
//		RecordVacuum( 5, szBuf );
		*strchr(data, '\r') = '\0';
	}
	else
	{
		strcpy( g_szBox600Saved, data );
		return -1;
	}

	int nRet = -1;
	if( strrchr(data, ',') != NULL )
	{
		// SETUSERNEGATIVELIMIT? X,-45000.1,OK
		// 有逗号，可能是各速度/各位置回读
		// 逗号前的字符串代表不同含义
		strcpy( szBuf, strrchr( data, ',' )+1 );		// szBuf中是OK
		*(strrchr(data, ',')) = '\0';					// data中是指令+?+空格+轴号+','+数值

		if( strrchr(data, ',') != NULL )
		{
			// SETUSERNEGATIVELIMIT? X,-45000.1
			strcpy( szBuf, strrchr( data, ',' )+1 );	// szBuf中是最后的数值
			*(strrchr(data, ',')) = '\0';				// data中是指令+?+空格+轴号
			if( strrchr(data, '?') != NULL )
			{
				// 有?，可能是各参数查询指令组的返回值
				// ?前的字符串为指令
				strcpy( szAxis, strrchr(data, '?' )+2 );// szAxis中是轴号
				*(strrchr(data, '?')) = '\0';			// data中是指令
			}
			else
				return -1;
		}
		else
		{
			// 无?，可能是位置值：轴号+空格+数值
			// 是主动传回的位置值，则说明运动已停止
			if( strrchr(data,' ') != NULL )
			{
				strcpy( szBuf, strrchr( data, ' ' )+1 );// szBuf中是最后的数值
				float fPos = (float)atof( szBuf );
				*(strrchr(data, ' ')) = '\0';			// data中是轴号
				return Box600_Convert_Position( data, szBuf ) +100;
			}
			else
				return -1;
		}

		int nCmdLen = strlen( data );
		if( nCmdLen < 2 )
			return -1;
		if( strcmp( data, "POSITION" ) == 0 )
			return Box600_Convert_Position( szAxis, szBuf );
		else if( strcmp( data, "SETSTAGEPITCH" ) == 0 )
			return Box600_Convert_StagePitch( szAxis, szBuf );
		else if( strcmp( data, "SETSTAGESTEPSREV" ) == 0 )
			return Box600_Convert_StageSteps( szAxis, szBuf );
		else if( strcmp( data, "SETSTAGEDRIVERAT" ) == 0 )
			return Box600_Convert_StageRat( szAxis, szBuf );
		else if( strcmp( data, "SETSTAGEDISTANCE" ) == 0 )
			return Box600_Convert_StageDistance( szAxis, szBuf );

		else if( strcmp( data, "SETSPEED" ) == 0 )
			return Box600_Convert_Speed( szAxis, szBuf );
		else if( strcmp( data, "SETINITSPEED" ) == 0 )
			return Box600_Convert_SpeedInit( szAxis, szBuf );
		else if( strcmp( data, "SETACCSPEED" ) == 0 )
			return Box600_Convert_SpeedAcc( szAxis, szBuf );
		else if( strcmp( data, "SETHOMESPEED" ) == 0 )
			return Box600_Convert_SpeedHome( szAxis, szBuf );
		else if( strcmp( data, "SETUSERORIGIN" ) == 0 )
			return Box600_Convert_SetUserOrigin( szAxis, szBuf );

		else if( strcmp( data, "SETUSERPOSITIVELIMIT" ) == 0 )
			return Box600_Convert_SetUserPositiveLimit( szAxis, szBuf );
		else if( strcmp( data, "SETUSERNEGATIVELIMIT" )  == 0 )
			return Box600_Convert_SetUserNegtiveLimit( szAxis, szBuf );
		else
			return nRet;
	}
	else
	{
		// 无逗号，可能是OK/READY/E01/E02
		if( strcmp( data, "OK" ) == 0 )
			nRet = nRet_OK;
		else if( strcmp( data, "READY" ) == 0 )
			nRet = nRet_Ready;
		else if( strcmp( data, "E01" ) == 0 )
			nRet = nRet_E01;
		else if( strcmp( data, "E02" ) == 0 )
			nRet = nRet_E02;
		else
		{
			// 其它信息
			nRet = nRet_OK -1;
		}
		return nRet;
	}
	return 0;
}

void Box600_UpdateDir( unsigned char ucAxis, long lSteps )
{
	if( ucAxis == 'X' )
	{
		if( lSteps < 0 )
			g_nDirX = -1;
		else
			g_nDirX = 1;
	}
	else if( ucAxis == 'Y' )
	{
		if( lSteps < 0 )
			g_nDirY = -1;
		else
			g_nDirY = 1;
	}
}

void Box600_AfterStop( int nRet )
{
	switch( nRet )
	{
	case	111:
		if( g_bBacklash[0] )
		{
			PostMessage( g_hMain, WM_USER_MC600_UpdateValue, nRet, g_ParamPos.fSteps[0] );
			char data[100];
			if( g_ParamBl.nDir[0] < 0 )
				sprintf( data, "GoPosition X,O,R,P,%d", g_ParamBl.nCount[0] );	// 向正向走
			else
				sprintf( data, "GoPosition X,O,R,N,%d", g_ParamBl.nCount[0] );	// 向负向走
			Box600_UpdateDir( 'X', g_ParamBl.nDir[0] );
			Box600_SendCmd( nBox600_Sleep, data );
			g_bBacklash[0] = FALSE;
		}
		else
		{
			PostMessage( g_hMain, WM_USER_MC600_UpdateValue, nRet, g_ParamPos.fSteps[0] );
			g_nDirX = 0;
		}
		break;
	case	112:
		if( g_bBacklash[1] )
		{
			PostMessage( g_hMain, WM_USER_MC600_UpdateValue, nRet, g_ParamPos.fSteps[1] );
			char data[100];
			if( g_ParamBl.nDir[1] < 0 )
				sprintf( data, "GoPosition Y,O,R,P,%d", g_ParamBl.nCount[1] );	// 向正向走
			else
				sprintf( data, "GoPosition Y,O,R,N,%d", g_ParamBl.nCount[1] );	// 向负向走
			Box600_UpdateDir( 'Y', g_ParamBl.nDir[1] );
			Box600_SendCmd( nBox600_Sleep, data );
			g_bBacklash[1] = FALSE;
		}
		else
		{
			PostMessage( g_hMain, WM_USER_MC600_UpdateValue, nRet, g_ParamPos.fSteps[1] );
			g_nDirY = 0;
		}
		break;
	case	113:
		if( g_bBacklash[2] )
		{
			char data[100];
			if( g_ParamBl.nDir[2] < 0 )
				sprintf( data, "GoPosition Z,O,R,P,%d", g_ParamBl.nCount[2] );	// 向正向走
			else
				sprintf( data, "GoPosition Z,O,R,N,%d", g_ParamBl.nCount[2] );	// 向负向走
			Box600_SendCmd( nBox600_Sleep, data );
			g_bBacklash[2] = FALSE;
		}
		else
			PostMessage( g_hMain, WM_USER_MC600_UpdateValue, nRet, g_ParamPos.fSteps[2] );
		break;
	case	114:
		if( g_bBacklash[3] )
		{
			char data[100];
			if( g_ParamBl.nDir[3] < 0 )
				sprintf( data, "GoPosition T,O,R,P,%d", g_ParamBl.nCount[3] );	// 向正向走
			else
				sprintf( data, "GoPosition T,O,R,N,%d", g_ParamBl.nCount[3] );	// 向负向走
			Box600_SendCmd( nBox600_Sleep, data );
			g_bBacklash[3] = FALSE;
		}
		else
			PostMessage( g_hMain, WM_USER_MC600_UpdateValue, nRet, g_ParamPos.fSteps[3] );
		break;
	}
}

int Box600_SendCmd( int nSleep, char* cmd )
{
	if( !g_bBox600Open )
		return -1;

	int nRet = 0;
	m_mutexBox600.Lock();
	char dataSend[255], dataRecv[255];
	if( cmd[0] != 'N' )
		sprintf( dataSend, "%s\r", cmd );
	else
		sprintf( dataSend, "\r" );
	Box600_WriteToDevice( nSleep, dataSend );

	int nLength = Box600_ReadFromDevice( nSleep, dataRecv );
	if( nLength > 0 )
	{
		nRet = Box600_ConvertReceiveData( dataRecv );
		if( nRet > 110 && nRet < 115 )	// 主动返回位置信息，说明运动已停止
			Box600_AfterStop( nRet );
	}
	m_mutexBox600.Unlock();
	return nRet;
}

void InitBox600Params( int nSleep )
{
	// 单位为pp
	Box600_SendCmd( nSleep, "SetUnit X,s" );
	Box600_SendCmd( nSleep, "SetUnit Y,s" );
	Box600_SendCmd( nSleep, "SetUnit Z,s" );
	Box600_SendCmd( nSleep, "SetUnit T,s" );
	// 设置默认运行速度
	Box600_SendCmd( nSleep, "SetSpeed X,960" );
	Box600_SendCmd( nSleep, "SetSpeed Y,960" );
	Box600_SendCmd( nSleep, "SetSpeed Z,800" );
	Box600_SendCmd( nSleep, "SetSpeed T,827" );
}

void SyncBox600Params( int nSleep )
{
	// 读取各参数
	Box600_SendCmd( nSleep, "SetStagePitch? X" );
	Box600_SendCmd( nSleep, "SetStagePitch? Y" );
	Box600_SendCmd( nSleep, "SetStagePitch? Z" );
	Box600_SendCmd( nSleep, "SetStagePitch? T" );
	Box600_SendCmd( nSleep, "SetStageStepsRev? X" );
	Box600_SendCmd( nSleep, "SetStageStepsRev? Y" );
	Box600_SendCmd( nSleep, "SetStageStepsRev? Z" );
	Box600_SendCmd( nSleep, "SetStageStepsRev? T" );
	Box600_SendCmd( nSleep, "SetStageDriveRat? X" );
	Box600_SendCmd( nSleep, "SetStageDriveRat? Y" );
	Box600_SendCmd( nSleep, "SetStageDriveRat? Z" );
	Box600_SendCmd( nSleep, "SetStageDriveRat? T" );
	Box600_SendCmd( nSleep, "SetStageDistance? X" );
	Box600_SendCmd( nSleep, "SetStageDistance? Y" );
	Box600_SendCmd( nSleep, "SetStageDistance? Z" );
	Box600_SendCmd( nSleep, "SetStageDistance? T" );

	Box600_SendCmd( nSleep, "SetSpeed? X" );
	Box600_SendCmd( nSleep, "SetSpeed? Y" );
	Box600_SendCmd( nSleep, "SetSpeed? Z" );
	Box600_SendCmd( nSleep, "SetSpeed? T" );
	Box600_SendCmd( nSleep, "SetInitSpeed? X" );
	Box600_SendCmd( nSleep, "SetInitSpeed? Y" );
	Box600_SendCmd( nSleep, "SetInitSpeed? Z" );
	Box600_SendCmd( nSleep, "SetInitSpeed? T" );
	Box600_SendCmd( nSleep, "SetAccSpeed? X" );
	Box600_SendCmd( nSleep, "SetAccSpeed? Y" );
	Box600_SendCmd( nSleep, "SetAccSpeed? Z" );
	Box600_SendCmd( nSleep, "SetAccSpeed? T" );
	Box600_SendCmd( nSleep, "SetHomeSpeed? X" );
	Box600_SendCmd( nSleep, "SetHomeSpeed? Y" );
	Box600_SendCmd( nSleep, "SetHomeSpeed? Z" );
	Box600_SendCmd( nSleep, "SetHomeSpeed? T" );
}

void LoadKYKYdefault( int nSleep )
{
	if( !g_bBox600Open )
		return;
	// 运动轴使用单位
	Box600_SendCmd( nSleep, "SetUnit X,s" );
	Box600_SendCmd( nSleep, "SetUnit Y,s" );
	Box600_SendCmd( nSleep, "SetUnit Z,s" );
	Box600_SendCmd( nSleep, "SetUnit T,s" );
	Sleep(nSleep);
	// 台子类型
	Box600_SendCmd( nSleep, "SetStageStyle X,T" );
	Box600_SendCmd( nSleep, "SetStageStyle Y,T" );
	Box600_SendCmd( nSleep, "SetStageStyle Z,T" );
	Box600_SendCmd( nSleep, "SetStageStyle T,R" );
	Sleep(nSleep);
	// 每转脉冲数
	Box600_SendCmd( nSleep, "SetStageStepsRev X,1600" );
	Box600_SendCmd( nSleep, "SetStageStepsRev Y,1600" );
	Box600_SendCmd( nSleep, "SetStageStepsRev Z,1600" );
	Box600_SendCmd( nSleep, "SetStageStepsRev T,1600" );
	Sleep(nSleep);
	// 丝杠导程
	Box600_SendCmd( nSleep, "SetStagePitch X,0.5" );
	Sleep(nSleep);
	Box600_SendCmd( nSleep, "SetStagePitch Y,0.5" );
	Sleep(nSleep);
	Box600_SendCmd( nSleep, "SetStagePitch Z,0.416667" );	// 40/96 = 0.416667
	Sleep(nSleep);
	Box600_SendCmd( nSleep, "SetStagePitch T,30" );
	Sleep(nSleep);
	// 传动比
	Box600_SendCmd( nSleep, "SetStageDriveRat X,180" );
	Sleep(nSleep);
	Box600_SendCmd( nSleep, "SetStageDriveRat Y,180" );
	Sleep(nSleep);
	Box600_SendCmd( nSleep, "SetStageDriveRat Z,180" );
	Sleep(nSleep);
	Box600_SendCmd( nSleep, "SetStageDriveRat T,6.2143" );	// 87/14 = 6.214286
	Sleep(nSleep);
	// 满行程
	Box600_SendCmd( nSleep, "SetStageDistance X,256000" );
	Box600_SendCmd( nSleep, "SetStageDistance Y,160000" );
	Box600_SendCmd( nSleep, "SetStageDistance Z,178000" );
	Box600_SendCmd( nSleep, "SetStageDistance T,10218" );
	Sleep(nSleep);
	// 归零模式：P为采用物理方式归零
	Box600_SendCmd( nSleep, "SetOrigin X,P" );
	Box600_SendCmd( nSleep, "SetOrigin Y,P" );
	Box600_SendCmd( nSleep, "SetOrigin Z,P" );
	Box600_SendCmd( nSleep, "SetOrigin T,P" );
	Sleep(nSleep);
	// 归零方式：模式2为光电开关归零
	Box600_SendCmd( nSleep, "Setsechhomemode X,2" );
	Box600_SendCmd( nSleep, "Setsechhomemode Y,2" );
	Box600_SendCmd( nSleep, "Setsechhomemode Z,2" );
	Box600_SendCmd( nSleep, "Setsechhomemode T,2" );
	Sleep(nSleep);
	// 软限位使能状态：E为打开软限位
	Box600_SendCmd( nSleep, "SetUserLimitAble X,E" );
	Box600_SendCmd( nSleep, "SetUserLimitAble Y,E" );
	Box600_SendCmd( nSleep, "SetUserLimitAble Z,E" );
	Box600_SendCmd( nSleep, "SetUserLimitAble T,D" );
	Sleep(nSleep);
	// 正向极限位置
	Box600_SendCmd( nSleep, "SetUserPositiveLimit X,124800" );	// 39mm
	Box600_SendCmd( nSleep, "SetUserPositiveLimit Y,78500" );	// 24.53mm
	Box600_SendCmd( nSleep, "SetUserPositiveLimit Z,122870" );	// 32.0mm
	Box600_SendCmd( nSleep, "SetUserPositiveLimit T,10080" );
	Sleep(nSleep);
	// 负向极限位置
	Box600_SendCmd( nSleep, "SetUserNegativeLimit X,-131200" );	// 41mm
	Box600_SendCmd( nSleep, "SetUserNegativeLimit Y,-82500" );	// 25.78mm
	Box600_SendCmd( nSleep, "SetUserNegativeLimit Z,-55675" );	// 14.5mm
	Box600_SendCmd( nSleep, "SetUserNegativeLimit T,-10080" );
	Sleep(nSleep);
	// 常速度
	Box600_SendCmd( nSleep, "SetSpeed X,960" );
	Box600_SendCmd( nSleep, "SetSpeed Y,960" );
	Box600_SendCmd( nSleep, "SetSpeed Z,800" );
	Box600_SendCmd( nSleep, "SetSpeed T,827" );
	Sleep(nSleep);
	// 初速度
	Box600_SendCmd( nSleep, "SetInitSpeed X,800" );
	Box600_SendCmd( nSleep, "SetInitSpeed Y,800" );
	Box600_SendCmd( nSleep, "SetInitSpeed Z,640" );
	Box600_SendCmd( nSleep, "SetInitSpeed T,138" );
	Sleep(nSleep);
	// 加速度
	Box600_SendCmd( nSleep, "SetAccSpeed X,12000" );
	Box600_SendCmd( nSleep, "SetAccSpeed Y,12000" );
	Box600_SendCmd( nSleep, "SetAccSpeed Z,12000" );
	Box600_SendCmd( nSleep, "SetAccSpeed T,12427" );
	Sleep(nSleep);
	// 归零速度
	Box600_SendCmd( nSleep, "SetHomeSpeed X,800" );
	Box600_SendCmd( nSleep, "SetHomeSpeed Y,800" );
	Box600_SendCmd( nSleep, "SetHomeSpeed Z,640" );
	Box600_SendCmd( nSleep, "SetHomeSpeed T,138" );
	Sleep(nSleep);

	SyncBox600Params(nSleep);
	PostMessage( g_hMain, WM_USER_MC600_UpdateValue, 1, (LPARAM)(&g_ParamSys) );
	PostMessage( g_hMain, WM_USER_MC600_UpdateValue, 2, (LPARAM)(&g_ParamSpeed) );
	PostMessage( g_hMain, WM_USER_MC600_UpdateValue, 3, (LPARAM)(&g_ParamPos) );
}

int CommWithBox600( int nSleep )
{
	if( !g_bBox600Open )
		return -1;

	int nRet = 0;
	m_mutexBox600.Lock();
	char dataRecv[100];
	int nLength = Box600_ReadFromDevice( nSleep, dataRecv );
	if( nLength > 0 )
	{
		nRet = Box600_ConvertReceiveData( dataRecv );
		if( nRet > 110 && nRet < 115)	// 主动返回位置信息，说明运动已停止
			Box600_AfterStop( nRet );
	}
	m_mutexBox600.Unlock();
	return nRet;
}

DWORD WINAPI CommThreadBox600(LPVOID lpParam)
{
	// 打开端口
	g_bBox600Open	= g_Box600.Open( (short)lpParam, 19200 );

	if( g_bBox600Open )
	{
		int nRet = Box600_SendCmd( nBox600_Sleep, "Hello" );
		Sleep( 1000 );
		if( nRet != 100 )
			g_bBox600Open = FALSE;
		else
		{
			/////////////////////////////////////////////
/*			// 设置日志
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			char *ptemp = strrchr( exeFullPath, '\\');
			ptemp[1] = 0;

			CString str, strTemp;
			CTime	timeCurrent = CTime::GetCurrentTime();
			strTemp = timeCurrent.Format("%Y%m%d%H%M");

			// 设置Box日志文件
			str.Format( "%sLog\\Box%s.csv", exeFullPath, strTemp );
			if( g_hFileBox != NULL )
				CloseHandle( g_hFileBox );
			g_hFileBox	= NULL;
			g_hFileBox = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoopsBox = 1;
			// 写Box日志文件头
			if( g_hFileBox != NULL )
			{
				unsigned long cbRet;
				char Buf[255];
				str.Format( "\t%s\r\n", "Box" );
				sprintf( Buf, "%s\r\n", str );
				WriteFile( g_hFileBox, Buf, strlen(Buf), &cbRet, NULL );
			}
*/			InitBox600Params(nBox600_Sleep);
			SyncBox600Params(nBox600_Sleep);
		}
	}
	PostMessage( g_hMain, WM_USER_MC600_UpdateValue, 0, g_bBox600Open );
	if( g_bBox600Open )
	{
		PostMessage( g_hMain, WM_USER_MC600_UpdateValue, 1, (LPARAM)(&g_ParamSys) );
		PostMessage( g_hMain, WM_USER_MC600_UpdateValue, 2, (LPARAM)(&g_ParamSpeed) );
		PostMessage( g_hMain, WM_USER_MC600_UpdateValue, 3, (LPARAM)(&g_ParamPos) );
	}

	while( g_bBox600Open && WaitForSingleObject(g_hEventTeminateBox600,200) != WAIT_OBJECT_0 )
	{
//		if( !g_bPauseComm )
		{
			CommWithBox600(nBox600_Sleep);
		}
		Sleep( 1 );
	}

	g_Box600.Close();
	g_bBox600Open	= g_Box600.IsOpened();
	if( g_hFileBox600 != NULL )
		CloseHandle( g_hFileBox600 );
	g_hFileBox600	= NULL;

	SetEvent( g_hEventWorkBox600 );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCommMC600 message handlers
void CCommMC600::InitHWND( HWND hWnd )
{
	g_hMain = hWnd;
}

BOOL CCommMC600::Open( short port )
{
	g_szBox600Saved[0] = '\0';

	/////////////////////////////////////////////////////////////////
	ResetEvent( g_hEventWorkBox600 );
	ResetEvent( g_hEventTeminateBox600 );

	g_hThreadBox600	= CreateThread( NULL, 0, CommThreadBox600, (LPVOID)(port), 0, &g_dwThreadBox600 );
	if( g_hThreadBox600 == NULL )
		return FALSE;

	CloseHandle(g_hThreadBox600);
	return TRUE;
}

BOOL CCommMC600::Close()
{
	if( g_bBox600Open )
	{
		SetEvent( g_hEventTeminateBox600 );
		WaitForSingleObject( g_hEventWorkBox600, INFINITE );
		g_bBox600Open	= FALSE;
	}
	return TRUE;
}

BOOL CCommMC600::GetConnectState()
{
	return g_bBox600Open;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CCommMC600::LoadDefault( BOOL bFactory )
{
	if( bFactory )
		Box600_SendCmd( nBox600_Sleep, "LoadDefault" );
	else
		LoadKYKYdefault( 350 );
}

float CCommMC600::GetPosition( unsigned char ucAxis )
{
	char data[100];
	sprintf( data, "Position? %c", ucAxis );
	Box600_SendCmd( nBox600_Sleep, data );
	float fSteps = 0.0;
	switch( ucAxis )
	{
	case	'X':
		fSteps = g_ParamPos.fSteps[0];
		break;
	case	'Y':
		fSteps = g_ParamPos.fSteps[1];
		break;
	case	'Z':
		fSteps = g_ParamPos.fSteps[2];
		break;
	case	'T':
		fSteps = g_ParamPos.fSteps[3];
		break;
	}
	return fSteps;
}

float CCommMC600::GetSpeed( short nIndex, unsigned char ucAxis )
{
	if( nIndex > SpeedHome )
		return 0.0;
	char cmd[100], data[255];
	switch( nIndex )
	{
	case	Speed:
		sprintf(cmd, "SetSpeed? ");
		break;
	case	SpeedInit:
		sprintf(cmd, "SetInitSpeed? ");
		break;
	case	SpeedAcc:
		sprintf(cmd, "SetAccSpeed? ");
		break;
	case	SpeedHome:
		sprintf(cmd, "SetHomeSpeed? ");
		break;
	}
	sprintf( data, "%s%c", cmd, ucAxis );
	Box600_SendCmd( nBox600_Sleep, data );
	float fSpeed = 0.0;
	if( nIndex == Speed )
	{
		switch( ucAxis )
		{
		case	'X':
			fSpeed = g_ParamSpeed.fSpeed[0];
			break;
		case	'Y':
			fSpeed = g_ParamSpeed.fSpeed[1];
			break;
		case	'Z':
			fSpeed = g_ParamSpeed.fSpeed[2];
			break;
		case	'T':
			fSpeed = g_ParamSpeed.fSpeed[3];
			break;
		}
	}
	else if( nIndex == SpeedInit )
	{
		switch( ucAxis )
		{
		case	'X':
			fSpeed = g_ParamSpeed.fSpeedInit[0];
			break;
		case	'Y':
			fSpeed = g_ParamSpeed.fSpeedInit[1];
			break;
		case	'Z':
			fSpeed = g_ParamSpeed.fSpeedInit[2];
			break;
		case	'T':
			fSpeed = g_ParamSpeed.fSpeedInit[3];
			break;
		}
	}
	else if( nIndex == SpeedAcc )
	{
		switch( ucAxis )
		{
		case	'X':
			fSpeed = g_ParamSpeed.fSpeedAcc[0];
			break;
		case	'Y':
			fSpeed = g_ParamSpeed.fSpeedAcc[1];
			break;
		case	'Z':
			fSpeed = g_ParamSpeed.fSpeedAcc[2];
			break;
		case	'T':
			fSpeed = g_ParamSpeed.fSpeedAcc[3];
			break;
		}
	}
	else if( nIndex == SpeedHome )
	{
		switch( ucAxis )
		{
		case	'X':
			fSpeed = g_ParamSpeed.fSpeedAcc[0];
			break;
		case	'Y':
			fSpeed = g_ParamSpeed.fSpeedAcc[1];
			break;
		case	'Z':
			fSpeed = g_ParamSpeed.fSpeedAcc[2];
			break;
		case	'T':
			fSpeed = g_ParamSpeed.fSpeedAcc[3];
			break;
		}
	}
	return fSpeed;
}

void CCommMC600::GetHome( short nIndex, unsigned char ucAxis )
{
	char cmd[100], data[255];
	switch( nIndex )
	{
	case	HomeMode:
		sprintf(cmd, "SetSechHomeMode? ");
		break;
	case	UserOrigin:
		sprintf(cmd, "SetUserOrigin? ");
		break;
	default:
		sprintf(cmd, "");
		break;
	}
	sprintf( data, "%s%c", cmd, ucAxis );
	if( nIndex < UserOrigin +1 )
		Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::GetLimit( short nIndex, unsigned char ucAxis )
{
	char cmd[100], data[255];
	switch( nIndex )
	{
	case	LimitAble:
		sprintf(cmd, "SetUserLimitAble? ");
		break;
	case	LimitPositive:
		sprintf(cmd, "SetUserPositiveLimit? ");
		break;
	case	LimitNegative:
		sprintf(cmd, "SetUserNegativeLimit? ");
		break;
	default:
		sprintf(cmd, "");
		break;
	}
	sprintf( data, "%s%c", cmd, ucAxis );
	if( nIndex < LimitNegative +1 )
		Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::GetSys( short nIndex, unsigned char ucAxis )
{
	char cmd[100], data[255];
	switch( nIndex )
	{
	case	Unit:
		sprintf(cmd, "SetUnit? ");
		break;
	case	Style:
		sprintf(cmd, "SetStageStyle? ");
		break;
	case	WorkState:
		sprintf(cmd, "SetStageWorkState? ");
		break;
	case	Pitch:
		sprintf(cmd, "SetStagePitch? ");
		break;
	case	StepsRev:
		sprintf(cmd, "SetStageStepsRev? ");
		break;
	case	DriveRat:
		sprintf(cmd, "SetDriveRat? ");
		break;
	case	Distance:
		sprintf(cmd, "SetStageDistance? ");
		break;
	default:
		sprintf(cmd, "");
		break;
	}
	sprintf( data, "%s%c", cmd, ucAxis );
	if( nIndex < Distance +1 )
		Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::SetSpeed( short nIndex, unsigned char ucAxis, float fSpeed )
{
	char cmd[100], data[255];
	switch( nIndex )
	{
	case	Speed:
		sprintf(cmd, "SetSpeed ");
		break;
	case	SpeedInit:
		sprintf(cmd, "SetInitSpeed ");
		break;
	case	SpeedAcc:
		sprintf(cmd, "SetAccSpeed ");
		break;
	case	SpeedHome:
		sprintf(cmd, "SetHomeSpeed ");
		break;
	default:
		sprintf(cmd, "");
		break;
	}
	sprintf( data, "%s%c,%.1f", cmd, ucAxis, fSpeed );
	if( nIndex < SpeedHome +1 )
		Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::SetHome( short nIndex, unsigned char ucAxis, unsigned char ucMode )
{
	char cmd[100], data[255];
	switch( nIndex )
	{
	case	Origin:
		sprintf(cmd, "SetOrigin ");
		break;
	case	HomeMode:
		sprintf(cmd, "SetSechHomeMode ");
		break;
	case	UserOrigin:
		sprintf(cmd, "SetUserOrigin ");
		break;
	default:
		sprintf(cmd, "");
		break;
	}
	sprintf( data, "%s%c,%c", cmd, ucAxis, ucMode );
	if( nIndex < UserOrigin +1 )
		Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::SetLimit( short nIndex, unsigned char ucAxis, unsigned char ucAble, float fLimit )
{
	char cmd[100], data[255];
	switch( nIndex )
	{
	case	LimitAble:
		sprintf(cmd, "SetUserLimitAble ");
		break;
	case	LimitPositive:
		sprintf(cmd, "SetUserPositiveLimit ");
		break;
	case	LimitNegative:
		sprintf(cmd, "SetUserNegativeLimit ");
		break;
	default:
		sprintf(cmd, "");
		break;
	}
	if( nIndex == LimitAble )
		sprintf( data, "%s%c,%c", cmd, ucAxis, ucAble );
	else
		sprintf( data, "%s%c,%.1f", cmd, ucAxis, fLimit );
	if( nIndex < LimitNegative +1 )
		Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::SetSys( short nIndex, unsigned char ucAxis, unsigned char ucSys, float fSys )
{
	char cmd[100], data[255];
	switch( nIndex )
	{
	case	Unit:
		sprintf(cmd, "SetUnit ");
		break;
	case	Style:
		sprintf(cmd, "SetStageStyle ");
		break;
	case	WorkState:
		sprintf(cmd, "SetStageWorkState ");
		break;
	case	Pitch:
		sprintf(cmd, "SetStagePitch ");
		break;
	case	StepsRev:
		sprintf(cmd, "SetStageStepsRev ");
		break;
	case	DriveRat:
		sprintf(cmd, "SetDriveRat ");
		break;
	case	Distance:
		sprintf(cmd, "SetStageDistance ");
		break;
	default:
		sprintf(cmd, "");
		break;
	}
	if( nIndex < Pitch )
		sprintf( data, "%s%c,%c", cmd, ucAxis, ucSys );
	else
		sprintf( data, "%s%c,%.1f", cmd, ucAxis, fSys );
	if( nIndex < Distance +1 )
		Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::BackZero( unsigned char ucAxis, unsigned char ucMode )
{
	// 定型后可不用
	char data[100];
	if( ucMode == 'O' )
		sprintf( data, "GoOrigion %c", ucAxis );
	else
		sprintf( data, "GoHome %c", ucAxis );
	Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::GoOrigion( unsigned char ucAxis )
{
	// 返回工作原点
	char data[100];
	sprintf( data, "GoOrigion %c", ucAxis );
	Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::GoHome( unsigned char ucAxis )
{
	// 返回机械原点（我们使用的是光电归零）
	char data[100];
	sprintf( data, "GoHome %c", ucAxis );
	Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::GoAxisHome( unsigned char ucX, unsigned char ucY, unsigned char ucZ, unsigned char ucT )
{
	// 多轴同时归零（返回机械原点），参数为Y或N
	char data[100];
	sprintf( data, "GonAxisHome %c,%c,%c,%c", ucX, ucY, ucZ, ucT );
	Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::MoveR( unsigned char ucAxis, long lSteps, BOOL bBacklash )
{
	if( bBacklash )
		CoherenceCheck_Backlash( ucAxis, lSteps, TRUE );
	char data[100];
	if( lSteps < 0 )
		sprintf( data, "GoPosition %c,O,R,N,%ld", ucAxis, 0 -lSteps );
	else
		sprintf( data, "GoPosition %c,O,R,P,%ld", ucAxis, lSteps );
	Box600_UpdateDir( ucAxis, lSteps );
	Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::MoveA( unsigned char ucAxis, long lSteps, BOOL bBacklash )
{
	if( bBacklash )
		CoherenceCheck_Backlash( ucAxis, lSteps, FALSE );
	char data[100];
	if( lSteps < 0 )
		sprintf( data, "GoPosition %c,O,A,N,%ld", ucAxis, 0 -lSteps );
	else
		sprintf( data, "GoPosition %c,O,A,P,%ld", ucAxis, lSteps );
	Box600_UpdateDir( ucAxis, lSteps );
	Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::GoPosition( unsigned char ucAxis, char cState, char cMode, char cDir, float fCount )
{
	char data[100];
	sprintf( data, "GoPosition %c,%c,%c,%c,%.1f", ucAxis, cState, cMode, cDir, fCount );
	Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::StopMove( unsigned char ucAxis )
{
	char data[100];
	sprintf( data, "Stop %c", ucAxis );
	Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::GoInterpolationLine2( unsigned char ucAxis1, float fS1, unsigned char ucAxis2, float fS2 )
{
	char data[100];
	sprintf( data, "GoInterpolationline2 %c,%.1f,%c,%.1f", ucAxis1, fS1, ucAxis2, fS2 );
	Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::GoInterpolationLine3( unsigned char ucAxis1, float fS1, unsigned char ucAxis2, float fS2, unsigned char ucAxis3, float fS3 )
{
	char data[100];
	sprintf( data, "GoInterpolationline3 %c,%.1f,%c,%.1f,%c,%.1f", ucAxis1, fS1, ucAxis2, fS2, ucAxis3, fS3 );
	Box600_SendCmd( nBox600_Sleep, data );
}

void CCommMC600::SetBacklashParams( short nIndex, short nDir, int nCount )
{
	// 每次向特定方向移动时，要多移动一个间隙量，然后向反方向走回一个间隙量
	g_ParamBl.nDir[nIndex]		= nDir;
	g_ParamBl.nCount[nIndex]	= nCount;
}

void CCommMC600::CoherenceCheck_Backlash( unsigned char ucAxis, long& lSteps, BOOL bRelative )
{
	short nIndex = -1;
	switch( ucAxis )
	{
	case	'X':
		nIndex = 0;
		break;
	case	'Y':
		nIndex = 1;
		break;
	case	'Z':
		nIndex = 2;
		break;
	case	'T':
		nIndex = 3;
		break;
	}
	long lThreshold = 0;	// 运行相对位置，只需看相对位置值的正负即可（与0比较）
	if( !bRelative )
		// 运行绝对位置，要先判断相对于当前位置是要向哪个方向运动
		lThreshold = g_ParamPos.fSteps[nIndex];
	if( lSteps < lThreshold && g_ParamBl.nDir[nIndex] < 0 )
	{
		g_bBacklash[nIndex] = TRUE;
		lSteps -= g_ParamBl.nCount[nIndex];
	}
	else if( lSteps > lThreshold && g_ParamBl.nDir[nIndex] > 0 )
	{
		g_bBacklash[nIndex] = TRUE;
		lSteps += g_ParamBl.nCount[nIndex];
	}
	else
		g_bBacklash[nIndex] = FALSE;
}

int CCommMC600::GetDir( unsigned char ucAxis )
{
	int nDir = 0;
	if( ucAxis == 'X' )
		nDir = g_nDirX;
	else if( ucAxis == 'Y' )
		nDir = g_nDirY;
	return nDir;
}
