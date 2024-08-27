// PLCvacBoard.cpp : implementation file
//

#include "stdafx.h"
#include "PLCvacBoard.h"
#include <PLCvacBrd.h>

#include <afxsock.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD WINAPI CommThreadPLCvacBoard(LPVOID lpParam);
HANDLE	g_hThreadPLCvacBrd;
DWORD	g_dwThreadPLCvacBrd;
HANDLE	g_hEventWorkPLCvacBoard		= CreateEvent(NULL,FALSE, FALSE, _T("WORKPLCvacBoard"));
HANDLE	g_hEventTeminatePLCvacBoard	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEPLCvacBoard"));

HWND	g_hMain;
Cfg_PLCvacBrd	g_cfgPLCvacBrd;
Status_PLCvacBrd	g_StatusPLCvacBrd;

HANDLE	g_hFilePLCvacBrd	= NULL;
int		g_nLoopsPLCvacBrd;

SOCKET sockfd;
struct sockaddr_in local_addr,remote_addr;

/////////////////////////////////////////////////////////////////////////////
// CPLCvacBoard

CPLCvacBoard::CPLCvacBoard()
{
	g_hMain	= NULL;
	ZeroMemory( &g_cfgPLCvacBrd, sizeof(Cfg_PLCvacBrd) );
	ZeroMemory( &g_StatusPLCvacBrd, sizeof(Status_PLCvacBrd) );
}

CPLCvacBoard::~CPLCvacBoard()
{
}

/////////////////////////////////////////////////////////////////////////////
// CPLCvacBoard message handlers
void PLC_RecordInit()
{
	if( g_hFilePLCvacBrd != NULL )
		CloseHandle( g_hFilePLCvacBrd );
	g_hFilePLCvacBrd = NULL;
	// 设置日志文件
	TCHAR exeFullPath[255];
	::GetModuleFileName( NULL, exeFullPath, 255 );
	char *ptemp = strrchr( exeFullPath, '\\');
	ptemp[1] = 0;

	CString csCurrentTime, str;
	CTime	timeCurrent = CTime::GetCurrentTime();
	csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
	str.Format( "%s\\Log\\PLCvacBrd%s.csv", exeFullPath, csCurrentTime );
	g_hFilePLCvacBrd = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	g_nLoopsPLCvacBrd = 1;

	if( g_hFilePLCvacBrd != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		str.Format( "\t%s\r\n", "PLCvacBrd" );
		sprintf( Buf, "%s\r\n", str );
		WriteFile( g_hFilePLCvacBrd, Buf, strlen(Buf), &cbRet, NULL );
		sprintf( Buf, "Time, CCG, TMP time, TMP speed, TMP temperature, V1, V2, V4, V5, TMP, CCG, CCD, CCGErr, MPErr, TMPpause, TMPfail, TMPErr, VacErr, ChambVacOK\r\n" );
		WriteFile( g_hFilePLCvacBrd, Buf, strlen(Buf), &cbRet, NULL );
	}
}

void PLC_Record( LPCTSTR lpszMessage )
{
	if( g_hFilePLCvacBrd == NULL )
		return;

	unsigned long cbRet;
	char Buf[255];
	CString str;

	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
	str.Format( "[%s],%s", csCurrentTime, lpszMessage );

	sprintf(Buf, "%s\r\n", str );
	WriteFile(g_hFilePLCvacBrd, Buf, strlen(Buf), &cbRet, NULL);

	g_nLoopsPLCvacBrd++;
	if( g_nLoopsPLCvacBrd > 10000 )
		PLC_RecordInit();
}

int PLC_ReadFrom( char FAR * buf, int len )
{
	int flags = 0;
	int nRet = recv(sockfd, buf, len, flags);

	char szRecv[2048] = {0}, temp[10] = {0};
	sprintf(szRecv, "PLCvacBrd: recv = ");
	for(int i=0; i<nRet; i++)
	{
		sprintf(temp, "0x%02X ", (unsigned char)buf[i]);
		strcat(szRecv, temp);
	}
//	PLC_Record(szRecv);
	return nRet;
}

int PLC_WriteTo( const char FAR * buf, int len )
{
	char szSend[255] = {0}, temp[10] = {0};
	sprintf(szSend, "PLCvacBrd: send = ");
	for(int i = 0; i < len; i++)
	{
		sprintf(temp, "0x%02X ", (unsigned char)buf[i]);
		strcat(szSend, temp);
	}
//	PLC_Record(szSend);

	int flags = 0;
	return send(sockfd, buf, len, flags);
}

int PLC_SendCmd( BYTE byteOpc, BYTE byteOpr )
{
	char dataSend[byte_PLCvacBRDLenSend1 +1];
	sprintf( dataSend, "%c%c%c\r", byte_PLCvacBRDHead, byteOpc, byteOpr );
	return PLC_WriteTo( dataSend, strlen(dataSend) );
}

/////////////////////////////////////////////////////////////////////
int PLC_Recv_VACstatus( BYTE* data )
{
	// 返回操作数为：0xXXXX
	int i = 0;
	WORD wStatus = (data[i +2] << 8) | data[i +3];
	if( wStatus != g_StatusPLCvacBrd.statusVac.wStatus )
	{
		g_StatusPLCvacBrd.statusVac.wStatus = wStatus;
		PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_InquireVACstatus, (LPARAM)(&(g_StatusPLCvacBrd.statusVac)) );
	}
	return 0;
}

int PLC_Recv_VACvalve( BYTE* data )
{
	// 返回操作数为：0xXXXX
	int i = 0;
	WORD wStatus = (data[i +2] << 8) | data[i +3];
	if( wStatus != g_StatusPLCvacBrd.statusValve.wValve )
	{
		g_StatusPLCvacBrd.statusValve.wValve = wStatus;
		PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_InquireVACvalve, (LPARAM)(&(g_StatusPLCvacBrd.statusValve)) );
	}
	return 0;
}

int PLC_Recv_VACthreshold( BYTE* data )
{
	// 返回操作数为：0xXXXX XXXX XXXX
	int i = 0;
	WORD wData1 = (data[i +2] << 8) | data[i +3];
	WORD wData2 = (data[i +4] << 8) | data[i +5];
	WORD wData3 = (data[i +6] << 8) | data[i +7];
	g_cfgPLCvacBrd.dThreshold_PreVac = 1.0 *wData1 / pow(10,3);
	g_cfgPLCvacBrd.dThreshold_VacOK = 1.0 *wData2 / pow(10,7);
	g_cfgPLCvacBrd.dThreshold_VacNotOK = 1.0 *wData3 / pow(10,7);
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_InquireVACthreshold, (LPARAM)(&g_cfgPLCvacBrd) );
	return 0;
}

int PLC_Recv_Status( BYTE* data )
{
	// 返回操作数为：
	//				参数1,2：	真空状态
	//				参数3,4：	阀门状态
	//				参数5,6：	CCG值
	//				参数7,8：	分子泵转速值
	//				参数9,10：	分子泵温度值
	//				参数11,12：	分子泵运行时间值
	//				参数13：	01为钨灯丝；02为场枪
	//				参数14：	01为允许开启NAV
	int i = 0;
	g_StatusPLCvacBrd.statusVac.wStatus = (data[i +3] << 8) | data[i +2];
	g_StatusPLCvacBrd.statusValve.wValve = (data[i +5] << 8) | data[i +4];

	WORD wData = (data[i +6] << 8) | data[i +7];
	// U = (ADC/27648.0) *10.0	// ?????
	// P = 10 ^ (1.667 *U -d)	// d = 11.46
	g_StatusPLCvacBrd.dPccg = pow( 10, 1.667 * (10.0 *wData /27648) -11.46 );

	g_StatusPLCvacBrd.tmp.wR = (data[i +8] << 8) | data[i +9];
	g_StatusPLCvacBrd.tmp.wD = (data[i +10] << 8) | data[i +11];
	g_StatusPLCvacBrd.tmp.wT = (data[i +12] << 8) | data[i +13];

	g_StatusPLCvacBrd.nType = data[i +14];
	g_StatusPLCvacBrd.bNAV = (BOOL)(data[15]);

	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_InquireStatus, (LPARAM)(&g_StatusPLCvacBrd) );
	return 0;
}

int PLC_Recv_SwitchMode( BYTE* data )
{
	int nMode = 1;	// 自动模式
	if( data[2] == 0x01 )
		nMode = 0;
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_SwitchMode, (LPARAM)(&nMode) );
	return 0;
}

int PLC_Recv_PumpVent_SEM( BYTE* data )
{
	int nPumpVent_SEM = data[2];
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_PumpVent_SEM, (LPARAM)(&nPumpVent_SEM) );
	return 0;
}

int PLC_Recv_Standby( BYTE* data )
{
	int nStandby = data[2];
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_Standby, (LPARAM)(&nStandby) );
	return 0;
}

int PLC_Recv_PumpVent_FEM( BYTE* data )
{
	int nPumpVent_FEM = data[2];
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_PumpVent_FEM, (LPARAM)(&nPumpVent_FEM) );
	return 0;
}

int PLC_Recv_V1( BYTE* data )
{
	int nV1 = data[2];
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_V1, (LPARAM)(&nV1) );
	return 0;
}

int PLC_Recv_Manual_V1( BYTE* data )
{
	int nManual_V1 = data[2];
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_Manual_V1, (LPARAM)(&nManual_V1) );
	return 0;
}

int PLC_Recv_Manual_V2( BYTE* data )
{
	int nManual_V2 = data[2];
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_Manual_V2, (LPARAM)(&nManual_V2) );
	return 0;
}

int PLC_Recv_Manual_V3( BYTE* data )
{
	int nManual_V3 = data[2];
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_Manual_V3, (LPARAM)(&nManual_V3) );
	return 0;
}

int PLC_Recv_Manual_V4( BYTE* data )
{
	int nManual_V4 = data[2];
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_Manual_V4, (LPARAM)(&nManual_V4) );
	return 0;
}

int PLC_Recv_Manual_TMP( BYTE* data )
{
	int nManual_TMP = data[2];
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_Manual_TMP, (LPARAM)(&nManual_TMP) );
	return 0;
}

int PLC_Recv_CCD( BYTE* data )
{
	int nV1 = data[2];
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_CCD, (LPARAM)(&nV1) );
	return 0;
}

int PLC_Recv_WarnReset( BYTE* data )
{
	int nWarnReset = data[2];
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_WarnReset, (LPARAM)(&nWarnReset) );
	return 0;
}

int PLC_Recv_NAV( BYTE* data )
{
	int nNAV = data[2];
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_NAV, (LPARAM)(&nNAV) );
	return 0;
}
/////////////////////////////////////////////////////////////////////

BOOL IsValidFrame( BYTE bFunc, int nLen )
{
	BOOL bRet = TRUE;
	switch( bFunc )
	{
	case	byte_PLCvacBRDopc_SwitchMode:
	case	byte_PLCvacBRDopc_PumpVent_SEM:
	case	byte_PLCvacBRDopc_V1:
	case	byte_PLCvacBRDopc_Manual_V1:
	case	byte_PLCvacBRDopc_Manual_V2:
	case	byte_PLCvacBRDopc_Manual_V3:
	case	byte_PLCvacBRDopc_Manual_V4:
	case	byte_PLCvacBRDopc_Manual_TMP:
	case	byte_PLCvacBRDopc_CCD:
	case	byte_PLCvacBRDopc_NAV:
	case	byte_PLCvacBRDopc_WarnReset:
		if( nLen < 4 )
			bRet = FALSE;
		break;

	case	byte_PLCvacBRDopc_InquireVACstatus:
	case	byte_PLCvacBRDopc_InquireVACvalve:
		if( nLen < 5 )
			bRet = FALSE;
		break;
	case	byte_PLCvacBRDopc_InquireVACthreshold:
		if( nLen < 9 )
			bRet = FALSE;
		break;
	case	byte_PLCvacBRDopc_InquireStatus:
		if( nLen < 17 )
			bRet = FALSE;
		break;

	case	byte_PLCvacBRDopc_InquireCCG:
	case	byte_PLCvacBRDopc_InquireTMPr:
	case	byte_PLCvacBRDopc_InquireTMPd:
	case	byte_PLCvacBRDopc_InquireTMPt:
		if( nLen < 5 )
			bRet = FALSE;
		break;
	}
	return bRet;
}

int PLC_ConvertReceiveData( int nLen, char* data )
{
	if( nLen < 4 )
		return 11;	// 不够一帧数据

	int i = 0;
// 	if( data[i] != byte_PLCvacBRDHead )
// 		return 12;	// 帧头不对
// 	if( data[nLen] != byte_PLCvacBRDTail )
// 		return 13;	// 帧尾不对

	BYTE bFunc = data[i+1];
	BYTE* pbData = new BYTE[ nLen +1];
	for( i=0; i<nLen; i++ )
		pbData[i] = data[i];
	if( !IsValidFrame( bFunc, nLen ) )
	{
		delete [] pbData;
		return 14;	// 不是完整的响应帧
	}

	switch( bFunc )
	{
	case	byte_PLCvacBRDopc_InquireVACstatus:
		PLC_Recv_VACstatus( pbData );
		break;
	case	byte_PLCvacBRDopc_InquireVACvalve:
		PLC_Recv_VACvalve( pbData );
		break;
	case	byte_PLCvacBRDopc_InquireVACthreshold:
		PLC_Recv_VACthreshold( pbData );
		break;
	case	byte_PLCvacBRDopc_InquireStatus:
		PLC_Recv_Status( pbData );
		break;

	case	byte_PLCvacBRDopc_SwitchMode:
		PLC_Recv_SwitchMode( pbData );
		break;
	case	byte_PLCvacBRDopc_PumpVent_SEM:
		PLC_Recv_PumpVent_SEM( pbData );
		break;
	case	byte_PLCvacBRDopc_V1:
		PLC_Recv_V1( pbData );
		break;

	case	byte_PLCvacBRDopc_Manual_V1:
		PLC_Recv_Manual_V1( pbData );
		break;
	case	byte_PLCvacBRDopc_Manual_V2:
		PLC_Recv_Manual_V2( pbData );
		break;
	case	byte_PLCvacBRDopc_Manual_V3:
		PLC_Recv_Manual_V3( pbData );
		break;
	case	byte_PLCvacBRDopc_Manual_V4:
		PLC_Recv_Manual_V4( pbData );
		break;
	case	byte_PLCvacBRDopc_Manual_TMP:
		PLC_Recv_Manual_TMP( pbData );
		break;

	case	byte_PLCvacBRDopc_CCD:
		PLC_Recv_CCD( pbData );
		break;
	case	byte_PLCvacBRDopc_WarnReset:
		PLC_Recv_WarnReset( pbData );
		break;

	case	byte_PLCvacBRDopc_NAV:
		PLC_Recv_NAV( pbData );
		break;
	}
	delete [] pbData;
	return 0;
}

int PLC_CommWithDevice( int nSleep )
{
	if( !(g_cfgPLCvacBrd.bReady) )
		return 1;

	if( PLC_SendCmd( byte_PLCvacBRDopc_InquireStatus, byte_PLCvacBRDopr_Default ) < 1 )
		return 2;

	char dataRecv[2048] = {0};
	int nLenRecv = PLC_ReadFrom( dataRecv, 1024 );

	int nRet = 3;
	if( nLenRecv > 0 )
		nRet = PLC_ConvertReceiveData( nLenRecv, dataRecv );
	return nRet;
}

int PLC_CommTest( int nSleep )
{
	int nRet = 1;
	if( PLC_SendCmd( byte_PLCvacBRDopc_InquireStatus, byte_PLCvacBRDopr_Default ) > 0 )
	{
		char dataRecv[2048] = {0};
		int nLenRecv = PLC_ReadFrom( dataRecv, 1024 );
		if( nLenRecv > 0 )
			nRet = PLC_ConvertReceiveData( nLenRecv, dataRecv );
	}
	return nRet;
}

DWORD WINAPI CommThreadPLCvacBoard(LPVOID lpParam)
{
	if( !(g_cfgPLCvacBrd.bCreate) || !(g_cfgPLCvacBrd.bConnect) )
		return 0;
	// 先测试通讯是否正常
	if( PLC_CommTest(10) > 0 )
		g_cfgPLCvacBrd.bReady = FALSE;
	else
	{
		g_cfgPLCvacBrd.bReady = TRUE;
		PLC_RecordInit();
	}
	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, 3, g_cfgPLCvacBrd.bReady );
	if( g_cfgPLCvacBrd.bReady )
	{
		// 切换钨灯丝
		PLC_SendCmd( byte_PLCvacBRDopc_PumpVent_SEM, byte_PLCvacBRDopr_SEMpvSwitch );
		// 自动模式
		PLC_SendCmd( byte_PLCvacBRDopc_SwitchMode, byte_PLCvacBRDopr_ModeAuto );
		// 获取当前阈值
		PLC_SendCmd( byte_PLCvacBRDopc_InquireVACthreshold, byte_PLCvacBRDopr_Default );
	}

	while( g_cfgPLCvacBrd.bReady && WaitForSingleObject(g_hEventTeminatePLCvacBoard, 500) != WAIT_OBJECT_0 )
	{
//		if( !g_bPauseComm )
		{
			PLC_CommWithDevice(10);
		}
	}
	
	shutdown(sockfd,2);
	closesocket(sockfd);
	g_cfgPLCvacBrd.bReady	= FALSE;
	if( g_hFilePLCvacBrd != NULL )
		CloseHandle( g_hFilePLCvacBrd );
	g_hFilePLCvacBrd	= NULL;
	SetEvent( g_hEventWorkPLCvacBoard );
	return 0;
}

BOOL CPLCvacBoard::Init( HWND hWnd )
{
	g_hMain = hWnd;

	// 创建socket
	AfxSocketInit();
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if( sockfd < 0 )
	{
		// Socket创建异常...
		g_cfgPLCvacBrd.bCreate = FALSE;
		PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, 0, g_cfgPLCvacBrd.bCreate );
		shutdown(sockfd,2);
		closesocket(sockfd);
		return FALSE;
	}
	else
	{
		g_cfgPLCvacBrd.bCreate = TRUE;
		char szPortPLC[5], szPortPC[5], szIP[16];
/*		sprintf( szPortPC, "%d", PLCvacBoard_PortPC );
		local_addr.sin_family = AF_INET;
		local_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
		local_addr.sin_port = htons(_ttoi(szPortPC));
*/
		sprintf( szPortPLC, "%d", PLCvacBoard_PortPLC );
		sprintf( szIP, "192.168.2.1" );
		remote_addr.sin_family = AF_INET;
		remote_addr.sin_addr.s_addr = inet_addr(szIP);
		remote_addr.sin_port = htons(_ttoi(szPortPLC));

		if( connect(sockfd,(struct sockaddr*)&remote_addr, sizeof(struct sockaddr_in)) < 0 )
		{
			// 连接异常
			g_cfgPLCvacBrd.bConnect = FALSE;
			PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, 1, g_cfgPLCvacBrd.bConnect );
			shutdown(sockfd,2);
			closesocket(sockfd);
			return FALSE;
		}
		else
		{
			g_cfgPLCvacBrd.bConnect = TRUE;

			ResetEvent( g_hEventWorkPLCvacBoard );
			ResetEvent( g_hEventTeminatePLCvacBoard );
			g_hThreadPLCvacBrd = CreateThread(NULL, 0, CommThreadPLCvacBoard, NULL, 0, &g_dwThreadPLCvacBrd);
			if( g_hThreadPLCvacBrd == NULL )
			{
				PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, 2, 0 );
				return FALSE;
			}
			CloseHandle( g_hThreadPLCvacBrd );
		}
	}
	return TRUE;
}

BOOL CPLCvacBoard::Close()
{
	if( g_cfgPLCvacBrd.bReady )
	{
		SetEvent( g_hEventTeminatePLCvacBoard );
		WaitForSingleObject( g_hEventWorkPLCvacBoard, INFINITE );
	}
	ZeroMemory( &g_cfgPLCvacBrd, sizeof(Cfg_PLCvacBrd) );
	ZeroMemory( &g_StatusPLCvacBrd, sizeof(Status_PLCvacBrd) );
	return TRUE;
}

BOOL CPLCvacBoard::SetCmd( BYTE bFunc, BYTE bOpr )
{
	if( PLC_SendCmd( bFunc, bOpr ) < 1 )
		return FALSE;
	return TRUE;
}

BOOL CPLCvacBoard::SetThresholds( BYTE bOpr, double dValue )
{
	WORD sum = 0x0;
	switch( bOpr )
	{
	case	byte_PLCvacBRDopr_VacPre:
		g_cfgPLCvacBrd.dThreshold_PreVac = dValue;
		sum = (WORD)(dValue *pow(10,3));
		break;
	case	byte_PLCvacBRDopr_VacOK:
		g_cfgPLCvacBrd.dThreshold_VacOK = dValue;
		sum = (WORD)(dValue *pow(10,7));
		break;
	case	byte_PLCvacBRDopr_VacNotOK:
		g_cfgPLCvacBrd.dThreshold_VacNotOK = dValue;
		sum = (WORD)(dValue *pow(10,7));
		break;
	}
	char dataSend[25] = {0};
	sprintf( dataSend, "%c%c%c%c%c\r", byte_PLCvacBRDHead, byte_PLCvacBRDopc_SetThreshold, bOpr, (BYTE)((sum & 0xFF00) >> 8), (BYTE)(sum & 0x00FF) );
	PLC_WriteTo( dataSend, strlen(dataSend) );
	return TRUE;
}
