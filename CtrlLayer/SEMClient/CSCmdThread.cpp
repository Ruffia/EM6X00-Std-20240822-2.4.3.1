// CSCmdThread.cpp : implementation file
//

#include "stdafx.h"
#include "CSCmdThread.h"

#include <afxtempl.h>
#include <cfifo.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


typedef struct tagKYKYCommClient_Recv_Package
{
	char szRecv[768];
} KYKYCommClient_Recv_Package;

typedef struct tagKYKYCommClientJSON_Recv
{
	std::string	strRecv;
} KYKYCommClientJSON_Recv;

CFIFO<KYKYCommClient_Recv_Package, KYKYCommClient_Recv_Package&>	g_fifoRecvServerMsg(200);	// 大：每一个可能包含多条命令
CFIFO<KYKYCommClientJSON_Recv, KYKYCommClientJSON_Recv&>	g_fifoRecvFromServer(500);		// 小：每一个包含单一命令

HANDLE g_hCSocketThreadCmd = CreateEvent(NULL, TRUE, FALSE, "CSocketThreadCmd");

// 将从服务端接收的命令串，分析出包含单一指令的字符串，放入缓冲区SendServer
DWORD WINAPI ThreadAnalyzeServer(LPVOID lpParam);
// 将包含单一指令的字符串从缓冲区SendServer中一一取出，发送给客户端
DWORD WINAPI ThreadRecvFromServer(LPVOID lpParam);

CRITICAL_SECTION	csFifoRecvServerMsg, csFifoRecvFromServer;
static	BOOL g_bEndCScmdThreads = FALSE;
int		g_nCScmdThreadCounts;

/////////////////////////////////////////////////
/////////////////////////////////////////////////

extern HWND g_hMain2;
CCSCmd*	g_pCSocketCmd;	// 附加在这个线程上的CSocket

/////////////////////////////////////////////
int			g_nConnectedCmd;		// 已和服务端连接为1，未连接为0
WORD		g_wVersionNumber;
BYTE		g_byteStageStatus;

int			g_nGetAngleInit, g_nGetAngle, g_nSetAngle;
int			g_nStatusVacuum, g_nStatusGun, g_nTypeHV, g_nTypeSEM, g_nIsStopScan;
// g_dGetSpeed与g_szGetReso是可从服务端获取的当前的速度值（uS）与分辨率值（X和Y分别多少像素）
double		g_dGetSpeed, g_dGetMag, g_dGetWD, g_dGetBrt, g_dGetBrt2, g_dGetCst, g_dGetHV, g_dGetFila, g_dGetBeam, g_dGetBias, g_dGetEMI;
double		g_dSetMag, g_dSetWD, g_dSetBrt, g_dSetBrt2, g_dSetCst, g_dSetHV, g_dSetFila, g_dSetBeam, g_dSetBias;
CSize		g_szGetReso, g_szGetStig, g_szSetStig, g_szGetGrating;
KYKYCommMsg_Data5f	g_5fGetRange[2], g_5fGetPos, g_5fSetPos, g_5fGetFOV;
KYKYCommMsg_Data2f	g_2fGetMagRange;
KYKYCommMsg_DataGroup	g_groupPos;

// g_nSetSpeed与g_nSetReso是可从客户端设置的速度档位和分辨率档位
int			g_nSetSpeed, g_nSetReso, g_nSetLineH, g_nSetLineV;
CPoint		g_ptSetScanSpot;
CRect		g_rcSetScanArea;
/////////////////////////////////////////////
char	g_szBufSend[1024];

/////////////////////////////////////////////////////////////////////////////
// CCSCmdThread

IMPLEMENT_DYNCREATE(CCSCmdThread, CWinThread)

CCSCmdThread::CCSCmdThread()
{
	g_pCSocketCmd = new CCSCmd();     //!!!problem
	g_nConnectedCmd = 0;    //未连接

	g_dGetSpeed	= 1.0;
	g_dGetMag	= 0.0;
	g_dGetWD	= 0.0;
	g_dGetBrt	= 0.0;
	g_dGetBrt2	= 0.0;
	g_dGetCst	= 0.0;
	g_dGetHV	= 0.0;
	g_dGetFila	= 0.0;
	g_dGetBeam	= 0.0;
	g_dGetBias	= 0.0;
	g_dGetEMI	= 0.0;

	g_nGetAngleInit	= 0;
	g_nGetAngle		= 0;
	g_nTypeHV		= -1;
	g_nTypeSEM		= -1;
	g_nStatusVacuum	= 0;
	g_nStatusGun	= 0;
	g_byteStageStatus	= 0x0;
	g_wVersionNumber	= 0x0;
	g_nIsStopScan	= 0;

	g_szGetReso.cx		= 1024;
	g_szGetReso.cy		= 768;
	g_szGetStig.cx		= 0;
	g_szGetStig.cy		= 0;
	g_szGetGrating.cx	= 0;
	g_szGetGrating.cy	= 0;
	g_ptSetScanSpot.x	= 0;
	g_ptSetScanSpot.y	= 0;

	g_5fGetRange[0].fValue1	= 0.0;
	g_5fGetRange[0].fValue2	= 0.0;
	g_5fGetRange[0].fValue3	= 0.0;
	g_5fGetRange[0].fValue4	= 0.0;
	g_5fGetRange[0].fValue5	= 0.0;
	g_5fGetRange[1].fValue1	= 0.0;
	g_5fGetRange[1].fValue2	= 0.0;
	g_5fGetRange[1].fValue3	= 0.0;
	g_5fGetRange[1].fValue4	= 0.0;
	g_5fGetRange[1].fValue5	= 0.0;
	g_5fGetPos.fValue1		= 0.0;
	g_5fGetPos.fValue2		= 0.0;
	g_5fGetPos.fValue3		= 0.0;
	g_5fGetPos.fValue4		= 0.0;
	g_5fGetPos.fValue5		= 0.0;
	g_2fGetMagRange.fValue1	= 0.0;
	g_2fGetMagRange.fValue2	= 0.0;
	g_5fGetFOV.fValue1		= 0.0;
	g_5fGetFOV.fValue2		= 0.0;
	g_5fGetFOV.fValue3		= 0.0;
}

CCSCmdThread::~CCSCmdThread()
{
	delete g_pCSocketCmd;
	g_pCSocketCmd = NULL;
}

void Send_JsonString( Json::Value valueSend )
{
	Json::FastWriter writer;
	std::string strSend = writer.write(valueSend);
	const char *sendData = strSend.c_str();
	ZeroMemory( &g_szBufSend, sizeof(g_szBufSend) );
	int nLen = strlen(sendData);
//	sprintf_s( g_szBufSend, "%d:[%s],", nLen, sendData );
// 	nLen = strlen(g_szBufSend);
// 	g_szBufSend[nLen] = '\0';
	sprintf_s( g_szBufSend, "%d:[%s", nLen, sendData );
	nLen = strlen(g_szBufSend);
	g_szBufSend[nLen-1] = '\0';
	strcat_s(g_szBufSend, "],");
	nLen = strlen(g_szBufSend);
	g_szBufSend[nLen] = '\0';
//	OutputDebugString(g_szBufSend);
}
	
void Recv_SemGetValue( Json::Value param )
{
/*	// 分组
	char *exd = strrchr(id, '.');
	if(exd != NULL)
		*exd = '\0';
	char group[50];
	if( strchr(id, '.') != NULL )
	{
		strcpy_s( group, strchr( id, '.')+1 );
		*strchr(id, '.') = '\0';
	}
	else
	{
//		return;
		strcpy_s( group, id );
	}
*/
	if( !(param.isMember("Name")) )
		return;
	std::string strName = param["Name"].asString();
	const char *name = strName.c_str();
	if( strcmp(name, "MagRange") == 0 )
	{
		if( param.isMember("Min") && param.isMember("Max") )
		{
			g_2fGetMagRange.fValue1 = param["Min"].asDouble();
			g_2fGetMagRange.fValue2 = param["Max"].asDouble();
		}
	}
	else if( strcmp(name, "FOV") == 0 )
	{
		if( param.isMember("X") && param.isMember("Y") && param.isMember("Unit") )
		{
			g_5fGetFOV.fValue1 = param["X"].asDouble();
			g_5fGetFOV.fValue2 = param["Y"].asDouble();
			g_5fGetFOV.fValue3 = param["Unit"].asDouble();
		}
	}
	else
	{
		if( param.isMember("Value") )
		{
			if( strcmp(name, "SEM") == 0 )
				g_nTypeSEM = param["Value"].asInt();
			else if( strcmp(name, "HighVoltage") == 0 )
				g_nTypeHV = param["Value"].asInt();
			else if( strcmp(name, "Mag") == 0 )
				g_dGetMag = param["Value"].asDouble();
			else if( strcmp(name, "WD") == 0 )
				g_dGetWD = param["Value"].asDouble();
			else if( strcmp(name, "StigX") == 0 )
				g_szGetStig.cx = param["Value"].asInt();
			else if( strcmp(name, "StigY") == 0 )
				g_szGetStig.cy = param["Value"].asInt();
			else if( strcmp(name, "Brightness") == 0 )
				g_dGetBrt = param["Value"].asDouble();
			else if( strcmp(name, "Brightness2") == 0 )
				g_dGetBrt2 = param["Value"].asDouble();
			else if( strcmp(name, "Contrast") == 0 )
				g_dGetCst = param["Value"].asDouble();
			else if( strcmp(name, "ScanRotOffset1") == 0 )
				g_nGetAngleInit = param["Value"].asDouble();
			else if( strcmp(name, "ScanRotAngle") == 0 )
				g_nGetAngle = param["Value"].asDouble();
			else if( strcmp(name, "Acc") == 0 )
				g_dGetHV = param["Value"].asDouble();	// 从服务端得到是KV
			else if( strcmp(name, "Filament") == 0 )
				g_dGetFila = param["Value"].asDouble();
			else if( strcmp(name, "Bias") == 0 )
				g_dGetBias = param["Value"].asDouble();
			else if( strcmp(name, "Emission") == 0 )
				g_dGetBeam = param["Value"].asDouble();
			else if( strcmp(name, "EmissionMonitor") == 0 )
				g_dGetEMI = param["Value"].asDouble();
			else if( strcmp(name, "VacStatus") == 0 )
				g_nStatusVacuum = param["Value"].asInt();
			else if( strcmp(name, "GunStatus") == 0 )
				g_nStatusGun = param["Value"].asInt();
			else if( strcmp(name, "Speed") == 0 )
				g_dGetSpeed = param["Value"].asDouble();
			else if( strcmp(name, "ResoX") == 0 )
				g_szGetReso.cx = param["Value"].asInt();
			else if( strcmp(name, "ResoY") == 0 )
				g_szGetReso.cy = param["Value"].asInt();
			else if( strcmp(name, "IsStopScan") == 0 )
				g_nIsStopScan = param["Value"].asInt();
		}
	}
}

void Recv_SemGetValues( Json::Value root )
{
	if( root["result"].isArray() )
	{
		const Json::Value arrayObj = root["result"];
		UINT i, nSize = arrayObj.size();
		for( i=0; i<nSize; i++ )
		{
			const Json::Value param = arrayObj[i];
			Recv_SemGetValue( param );
		}
	}
	else
	{
		const Json::Value param = root["result"];
		Recv_SemGetValue( param );
	}
}

void Recv_AppSetCommand( Json::Value param )
{
	std::string strMsg;
	if( param.isMember("Name") )
	{
		strMsg = param["Name"].asString();
		const char *name = strMsg.c_str();
		if( strcmp(name, "Connect") == 0 )
		{
			BOOL bValue = param["Value"].asBool();
			if( bValue )
			{
				g_nConnectedCmd = 1;
				PostMessage(g_hMain2, WM_USER_KYKYComm_Connect_Status, KYKYComm_CMD_Sys_Connect_Success, 1);
			}
		}
		else if( strcmp(name, "Disconnect") == 0 )
		{
			BOOL bValue = param["Value"].asBool();
			if( bValue )
			{
				g_nConnectedCmd = 0;
				PostMessage(g_hMain2, WM_USER_KYKYComm_Connect_Status, KYKYComm_CMD_Sys_Disconnect, 0);
			}
		}
		else if( strcmp(name, "AutoBC") == 0 )
		{
			BOOL bValue = param["Value"].asBool();
			if( bValue )
			{
				PostMessage(g_hMain2, WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Func_AutoBC, 0);
			}
		}
		else if( strcmp(name, "ShutdownBeam") == 0 )
		{
			BOOL bValue = param["Value"].asBool();
			if( bValue )
			{
				PostMessage(g_hMain2, WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Func_ShutdownBeam, 0);
			}
		}
	}
}

void Recv_AppSetCommands( Json::Value root )
{
	if( root["result"].isArray() )
	{
		const Json::Value arrayObj = root["result"];
		UINT i, nSize = arrayObj.size();
		for( i=0; i<nSize; i++ )
		{
			const Json::Value param = arrayObj[i];
			Recv_AppSetCommand( param );
		}
	}
	else
	{
		const Json::Value param = root["result"];
		Recv_AppSetCommand( param );
	}
}

void Recv_SemGetType( Json::Value param )
{
	if( !(param.isMember("Name")) )
		return;
	std::string strName = param["Name"].asString();
	const char *name = strName.c_str();
	if( param.isMember("Value") )
	{
		if( strcmp(name, "SEM") == 0 )
			g_nTypeSEM = param["Value"].asInt();
		else if( strcmp(name, "HighVoltage") == 0 )
			g_nTypeHV = param["Value"].asInt();
	}
}

void Recv_SemGetTypes( Json::Value root )
{
	if( root["result"].isArray() )
	{
		const Json::Value arrayObj = root["result"];
		UINT i, nSize = arrayObj.size();
		for( i=0; i<nSize; i++ )
		{
			const Json::Value param = arrayObj[i];
			Recv_SemGetType( param );
		}
	}
	else
	{
		const Json::Value param = root["result"];
		Recv_SemGetType( param );
	}
}

void Recv_StageGetParam( Json::Value param )
{
	std::string strName;
	const char *name;
	if( param.isMember("Name") )
	{
		strName = param["Name"].asString();
		name = strName.c_str();
		if( param.isMember("Value") )
		{
			if( strstr( name, "Status") != NULL )
				g_byteStageStatus = param["Value"].asInt();
			else if( strstr(name, "PosX") )
				g_5fGetPos.fValue1 = param["Value"].asDouble();
			else if( strcmp(name, "PosY") == 0 )
				g_5fGetPos.fValue2 = param["Value"].asDouble();
			else if( strcmp(name, "PosZ") == 0 )
				g_5fGetPos.fValue3 = param["Value"].asDouble();
			else if( strcmp(name, "PosR") == 0 )
				g_5fGetPos.fValue4 = param["Value"].asDouble();
			else if( strcmp(name, "PosT") == 0 )
				g_5fGetPos.fValue5 = param["Value"].asDouble();
		}
		else if( param.isMember("Min") && param.isMember("Max") )
		{
			double min	= param["Min"].asDouble();
			double max	= param["Max"].asDouble();
			if( strcmp(name, "RangeX") == 0 )
			{
				g_5fGetRange[0].fValue1 = min;
				g_5fGetRange[1].fValue1 = max;
			}
			else if( strcmp(name, "RangeY") == 0 )
			{
				g_5fGetRange[0].fValue2 = min;
				g_5fGetRange[1].fValue2 = max;
			}
			else if( strcmp(name, "RangeZ") == 0 )
			{
				g_5fGetRange[0].fValue3 = min;
				g_5fGetRange[1].fValue3 = max;
			}
			else if( strcmp(name, "RangeR") == 0 )
			{
				g_5fGetRange[0].fValue4 = min;
				g_5fGetRange[1].fValue4 = max;
			}
			else if( strcmp(name, "RangeT") == 0 )
			{
				g_5fGetRange[0].fValue5 = min;
				g_5fGetRange[1].fValue5 = max;
			}
		}
		else if( param.isMember("ValueX") && param.isMember("ValueY") )
		{
			g_szGetGrating.cx = param["ValueX"].asInt();
			g_szGetGrating.cy = param["ValueY"].asInt();
		}
	}
}

void Recv_StageGetParams( Json::Value root )
{
	if( root["result"].isArray() )
	{
		const Json::Value arrayObj = root["result"];
		UINT i, nSize = arrayObj.size();
		for( i=0; i<nSize; i++ )
		{
			const Json::Value param = arrayObj[i];
			Recv_StageGetParam( param );
		}
	}
	else
	{
		const Json::Value param = root["result"];
		Recv_StageGetParam( param );
	}
}

void Recv_TranslateJson(const std::string strData)
{
	// 解析json用Json::Reader
	Json::Reader *readerinfo = new Json::Reader(Json::Features::strictMode());
	// Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array...
	Json::Value root;       
	if (readerinfo->parse(strData, root))
	{
		std::string strJsonrpc, strID, strMethod, strParams;
		if (root.isMember("jsonrpc"))
			strJsonrpc = root["jsonrpc"].asString();

		if (root.isMember("method"))
		{
			strMethod = root["method"].asString();
			const char *method = strMethod.c_str();
			if( strstr( method, "stage" ) != NULL )
				Recv_StageGetParams( root );
			else if( strstr( method, "getvalue" ) != NULL )
				Recv_SemGetValues( root );
			else if( strstr( method, "app" ) != NULL )
				Recv_AppSetCommands( root );
			else if( strstr( method, "gettype" ) != NULL )
				Recv_SemGetTypes( root );
		}
	}

	::delete readerinfo;
	readerinfo = NULL;
}

// 将从服务端接收的命令串，分析出包含单一指令的字符串，放入缓冲区RecvFromServer
DWORD WINAPI ThreadAnalyzeServer(LPVOID lpParam)
{
//	char* sFlag = (char*)(lpParam);
//	CTestSpellmanRSDlg* pDlg = (CTestSpellmanRSDlg*)(lpParam);
	char szBuf[768] = {0};
	char szAnalyze[512] = {0};
	char *exd;
	while (TRUE)
	{
		if( g_bEndCScmdThreads )
		{
// 			LeaveCriticalSection(&csFifoHost);
			break;
		}

		EnterCriticalSection(&csFifoRecvServerMsg);
		if( !g_fifoRecvServerMsg.IsFIFOEmpty() )
		{
			KYKYCommClient_Recv_Package msg;
			g_fifoRecvServerMsg.Output(msg);
//			LeaveCriticalSection(&csFifoRecvClientMsg);
/*			do 
			{
				// [{"method":"sem.getvalue","params":{"Name":"WD"}}]
				// [{"method":"sem.getvalue","params":[{"Name":"Mag"},{"Name":"WD"}]}]
				if( strstr( msg.szRecv, "}]") == NULL )
					break;
				if( strstr( msg.szRecv, "}}]") != NULL )
				{
					exd = strstr(msg.szRecv, "}}]");	// +3;
					int result = exd -msg.szRecv +1;	// position
					if( exd != NULL )
					{
						if( result +3 < strlen(msg.szRecv) )
						{
							strcpy_s( szBuf, strstr(msg.szRecv, "}}]") +3 );
							*(exd +3) = '\0';
						}
						else
							szBuf[0] = '\0';
					}
				}
				else if( strstr(msg.szRecv, "}]}]") != NULL )
				{
					exd = strstr(msg.szRecv, "}]}]");	// +4;
					int result = exd -msg.szRecv +1;	// position
					if( exd != NULL )
					{
						if( result +4 < strlen(msg.szRecv) )
						{
							strcpy_s( szBuf, strstr(msg.szRecv, "}]}]") +4 );
							*(exd +4) = '\0';
						}
						else
							szBuf[0] = '\0';
					}
				}
				else
					szBuf[0] = '\0';
				if( strchr( msg.szRecv, '{') != NULL )
				{
					strcpy_s( szAnalyze, strchr( msg.szRecv, '{') );
					exd = strrchr( szAnalyze, ']' );
					if( exd != NULL )
						*exd = '\0';
					KYKYCommClientJSON_Recv recvClient;
					recvClient.strRecv = szAnalyze;
//					EnterCriticalSection(&csFifoSendToHost);
					g_fifoRecvFromServer.Input(recvClient);
//					LeaveCriticalSection(&csFifoSendToHost);
				}
				strcpy_s( msg.szRecv, szBuf );
			} while (strchr( msg.szRecv, '}]') != NULL);
*/
			///////////////////////////////////////////////////////////////////
			do 
			{
				// 85:{"id":"app.execute","method":"app.execute","result":{"Name":"Connect","Value":true}}
				if( strstr( msg.szRecv, "}") == NULL )
					break;
				if( strstr( msg.szRecv, "}}") != NULL )
				{
					exd = strstr(msg.szRecv, "}}");		// +2;
					int result = exd -msg.szRecv +1;	// position
					if( exd != NULL )
					{
						if( result +2 < strlen(msg.szRecv) )
						{
							strcpy_s( szBuf, strstr(msg.szRecv, "}}") +2 );
							*(exd +2) = '\0';
						}
						else
							szBuf[0] = '\0';
					}
				}
				else if( strstr(msg.szRecv, "}]}]") != NULL )
				{
					exd = strstr(msg.szRecv, "}]}]");	// +4;
					int result = exd -msg.szRecv +1;	// position
					if( exd != NULL )
					{
						if( result +4 < strlen(msg.szRecv) )
						{
							strcpy_s( szBuf, strstr(msg.szRecv, "}]}]") +4 );
							*(exd +4) = '\0';
						}
						else
							szBuf[0] = '\0';
					}
				}
				else
					szBuf[0] = '\0';
				if( strchr( msg.szRecv, '{') != NULL )
				{
					strcpy_s( szAnalyze, strchr( msg.szRecv, '{') );
					exd = strrchr( szAnalyze, '}' ) +1;
					if( exd != NULL )
						*exd = '\0';
					KYKYCommClientJSON_Recv recvClient;
					recvClient.strRecv = szAnalyze;
//					EnterCriticalSection(&csFifoSendToHost);
					g_fifoRecvFromServer.Input(recvClient);
//					LeaveCriticalSection(&csFifoSendToHost);
				}
				strcpy_s( msg.szRecv, szBuf );
			} while (strchr( msg.szRecv, '}') != NULL);
		}
		LeaveCriticalSection(&csFifoRecvServerMsg);

		Sleep(1);
	}
	while( g_nCScmdThreadCounts > 1 )
		Sleep(1);
	g_nCScmdThreadCounts--;
	char cDebug[255];
	sprintf_s( cDebug, "exit FEThreadAnalyzeClient, counts = %d", g_nCScmdThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

// 将包含单一指令的字符串从缓冲区RecvFromServer中一一取出，发送给客户端
DWORD WINAPI ThreadRecvFromServer(LPVOID lpParam)
{
	while (TRUE)
	{
		if( g_bEndCScmdThreads )
		{
// 			LeaveCriticalSection(&csFifoHost);
			break;
		}

		EnterCriticalSection(&csFifoRecvFromServer);
		if( !g_fifoRecvFromServer.IsFIFOEmpty() )
		{
			KYKYCommClientJSON_Recv msg;
			g_fifoRecvFromServer.Output(msg);
//			LeaveCriticalSection(&csFifoSendToHost);
			Recv_TranslateJson( msg.strRecv );
		}
		LeaveCriticalSection(&csFifoRecvFromServer);

		Sleep(1);
	}
	g_nCScmdThreadCounts--;
	char cDebug[255];
	sprintf_s( cDebug, "exit FEThreadSendToHost, counts = %d", g_nCScmdThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

void StartThreads()		// (HWND hMain)
{
//	g_destHandle = hMain;
	g_bEndCScmdThreads = FALSE;
	HANDLE hThread1, hThread2;
//	InitializeCriticalSection(&csSocket);
	InitializeCriticalSection(&csFifoRecvServerMsg);
	InitializeCriticalSection(&csFifoRecvFromServer);

	hThread1 = CreateThread(NULL, 0, ThreadAnalyzeServer, NULL, 0, NULL);
	g_nCScmdThreadCounts++;

	hThread2 = CreateThread(NULL, 0, ThreadRecvFromServer, NULL, 0, NULL);
	g_nCScmdThreadCounts++;
}

BOOL CloseThreads()
{
	ResetEvent(g_hCSocketThreadCmd);	
	g_pCSocketCmd->AsyncSelect(FD_CLOSE);
	g_bEndCScmdThreads = TRUE;
//	Sleep(2000);
	while(true)
	{
		if( g_nCScmdThreadCounts < 1 )
			break;
	}
//	DeleteCriticalSection(&csSocket);
	DeleteCriticalSection(&csFifoRecvServerMsg);
	DeleteCriticalSection(&csFifoRecvFromServer);

	SetEvent( g_hCSocketThreadCmd );
	WaitForSingleObject( g_hCSocketThreadCmd, INFINITE );
	return TRUE;
}

BOOL CCSCmdThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	if (!AfxSocketInit())
	{
		return CWinThread::InitInstance();
	}

	if (!g_pCSocketCmd->Create())
	{
		AfxMessageBox("socket创建失败！"); 
		return CWinThread::InitInstance();
	}

	if (g_pCSocketCmd->Connect(m_strIPAddr, KYKYComm_ListenPort1))
	{
		g_pCSocketCmd->m_pThrd = this;
		g_pCSocketCmd->m_nPort = KYKYComm_ListenPort1;
		g_pCSocketCmd->AsyncSelect(FD_READ|FD_WRITE|FD_CLOSE);
		g_nConnectedCmd = 1;
	}
	else
		TRACE("命令线程连接失败！\n");
	if( g_nConnectedCmd > 0 )
	{
		StartThreads();	//(g_hMain2);
		return TRUE;
	}
	return CWinThread::InitInstance();
}

int CCSCmdThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	if( g_nConnectedCmd > 0 )
		CloseThreads();
// 		WaitForSingleObject(g_hCSocketThreadCmd, INFINITE);    //通过等待，解决了在断开连接时程序崩溃的问题
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCSCmdThread, CWinThread)
	//{{AFX_MSG_MAP(CCSCmdThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_Thread_Read, OnMsgRead)
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_ClientThreadSend_Command, OnMsgSend_Command)
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_ClientThreadSend_Param, OnMsgSend_Param)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSCmdThread message handlers

void CCSCmdThread::SetServerIP(CString sIP)
{
	m_strIPAddr = sIP;
}

void CCSCmdThread::Reset(LPVOID ptr)
{
//	m_pMainWnd = (CWnd*)ptr; 
}

// 得到了从Server端发来的信息？？？？？
// 根据接收到的数据里的命令字或字节数，提取相应信息
void CCSCmdThread::OnMsgRead(WPARAM wParam,LPARAM lParam)
{
	// 接收数据
	// 先把接收数据事件关掉
	// 关闭接收数据，避免后续接收数据不停传来，造成处理堵塞，这样读写一次完成
// 	ResetEvent(g_hCSocketThreadCmd);	
// 	g_pCSocketCmd->AsyncSelect(FD_CLOSE);
	KYKYCommClient_Recv_Package msg;
//	EnterCriticalSection(&csSocket);
	int nRecv = g_pCSocketCmd->Receive(msg.szRecv, 512);
	msg.szRecv[nRecv] = '\0';
//	DeleteCriticalSection(&csSocket);
	if( nRecv > 0 )
	{
//		EnterCriticalSection(&csFifoRecvClientMsg);
		g_fifoRecvServerMsg.Input(msg);
//		DeleteCriticalSection(&csFifoRecvClientMsg);
	}
	char output[1024] = {0};
	sprintf_s( output, "Client: MsgRead = %s\n", msg.szRecv );
	OutputDebugString(output);
	//重新打开接收数据事件
// 	g_pCSocketCmd->AsyncSelect(FD_READ|FD_WRITE|FD_CLOSE);
// 	SetEvent(g_hCSocketThreadCmd);

/*	// 接收数据
	// 先把接收数据事件关掉
	// 关闭接收数据，避免后续接收数据不停传来，造成处理堵塞，这样读写一次完成
	ResetEvent(g_hCSocketThreadCmd);	
	g_pCSocketCmd->AsyncSelect(FD_CLOSE);
	char szBufRecv[1024], szBuf[1024];
	int nRecvLen = g_pCSocketCmd->Receive(szBufRecv, 512); 
	szBufRecv[nRecvLen] = '\0';

	/////////////////////////////////////////////////////////////////
	if( strchr(szBufRecv, ':') != NULL )
	{
		strcpy_s( szBuf, strchr( szBufRecv, ':')+1 );
		*strchr(szBufRecv, ':') = '\0';
		int nLen = atoi(szBufRecv);
		int nRealLen = strlen(szBuf) -1;	// 字符串字节数（去掉结尾的','）
		if( nRealLen < nLen )
		{
			// 实际收到字节数小于JSON字符串应有长度，直接备份后返回
			strcpy_s( m_szRecvBak, szBuf );
		}
		else
		{
			if( strrchr( szBuf, '}') != NULL )
			{
				char *exd = strrchr(szBuf, '}') +1;
				if(exd != NULL)
					*exd = '\0';	// szBuf中是完整的JSON字符串
				std::string strRecv = szBuf;
				Recv_TranslateJson( strRecv );
			}
		}
	}
	/////////////////////////////////////////////////////////////////

	if( strrchr( szBufRecv, '}') != NULL )
	{
		char *exd = strrchr(szBufRecv, '}') +1;
		if(exd != NULL)
			*exd = '\0';
		if( strchr( szBufRecv, '{') != NULL )
		{
			strcpy_s( szBuf, strchr( szBufRecv, '{' ) );	// szBuf中是完整的JSON字符串
			std::string strRecv = szBuf;
			Recv_TranslateJson( strRecv );
		}
	}
	//重新打开接收数据事件
	g_pCSocketCmd->AsyncSelect(FD_READ|FD_WRITE|FD_CLOSE);
	SetEvent(g_hCSocketThreadCmd);*/	
}

void CCSCmdThread::OnMsgSend_Command(WPARAM wParam, LPARAM lParam)
// 对于客户端来说，只需要发送命令
{
	// 向服务端发送命令，以得到客户端需要的信息
	//发送数据
	//先把发送数据事件关掉
	ResetEvent(g_hCSocketThreadCmd);	
	g_pCSocketCmd->AsyncSelect(FD_CLOSE);		

	BYTE bCmd = BYTE(lParam);
	Json::Value valueSend, valueParam;
	if( bCmd > KYKYComm_CMD_Sys_Password && bCmd < KYKYComm_CMD_Image_Sync )
	{
		// 系统参数：Connect, Disconnect, Version, SEMType, HVType
		switch( bCmd )
		{
		case	KYKYComm_CMD_Sys_Connect_Success:
			valueParam["Name"]	= "Connect";
			valueSend["method"]	= "app.execute";
			valueSend["id"]		= "app";
			break;
		case	KYKYComm_CMD_Sys_Disconnect:
			valueParam["Name"]	= "Disconnect";
			valueSend["method"]	= "app.execute";
			valueSend["id"]		= "app";
			break;

		case	KYKYComm_CMD_Sys_GetSEMType:
			valueParam["Name"]	= "SEM";
			valueSend["method"]	= "sem.gettype";
			valueSend["id"]		= "sys.getvalue";
			break;
		case	KYKYComm_CMD_Sys_GetHighVoltageType:
			valueParam["Name"]	= "HighVoltage";
			valueSend["method"]	= "sem.gettype";
			valueSend["id"]		= "sys.getvalue";
			break;
		}
	}
	else if( bCmd > KYKYComm_CMD_Image_Sync -1 && bCmd < KYKYComm_CMD_ScanCtrl_Freeze )
	{
		g_nIsStopScan		= 0;
		// Image
		switch( bCmd )
		{
		case	KYKYComm_CMD_Image_Sync:
			valueParam["Name"]	= "ImageSync";
			valueSend["method"]	= "scan.img.sync";
			valueSend["id"]		= "scan.img";
			break;
		case	KYKYComm_CMD_Image_GetFrameData:
			valueParam["Name"]	= "ImageFrame";
			valueSend["method"]	= "scan.img.frame";
			valueSend["id"]		= "scan.img";
			break;
		}
	}
	else if( bCmd > KYKYComm_CMD_ScanCtrl_Freeze -1 && bCmd < KYKYComm_CMD_Get_MagRange )
	{
		// 扫描参数：type, speed, resolution
		switch( bCmd )
		{
/*		case	KYKYComm_CMD_ScanCtrl_Speed1:
			valueParam["Name"]	= "Speed1";
			valueSend["method"]	= "scan.setcmd";
			valueSend["id"]		= "scan.setcmd";
			break;
		case	KYKYComm_CMD_ScanCtrl_Speed2:
			valueParam["Name"]	= "Speed1";
			valueSend["method"]	= "scan.setcmd";
			valueSend["id"]		= "scan.setcmd";
			break;
		case	KYKYComm_CMD_ScanCtrl_Speed3:
			valueParam["Name"]	= "Speed1";
			valueSend["method"]	= "scan.setcmd";
			valueSend["id"]		= "scan.setcmd";
			break;
		case	KYKYComm_CMD_ScanCtrl_Speed4:
			valueParam["Name"]	= "Speed1";
			valueSend["method"]	= "scan.setcmd";
			valueSend["id"]		= "scan.setcmd";
			break;
*/		case	KYKYComm_CMD_Get_ScanCtrl_Reso:
			valueParam["Name"]	= "Reso";
			valueSend["method"]	= "scan.getvalue.reso";
			valueSend["id"]		= "scan.getvalue.reso";
			break;
		case	KYKYComm_CMD_Get_ScanCtrl_Speed:
			valueParam["Name"]	= "Speed";
			valueSend["method"]	= "scan.getvalue.speed";
			valueSend["id"]		= "scan.getvalue.speed";
			break;
		case	KYKYComm_CMD_ScanCtrl_Normal:
			valueParam["Name"]	= "Normal";
			valueSend["method"]	= "scan.start.normal";
			valueSend["id"]		= "scan.setcmd";
			g_nIsStopScan		= 0;
			break;
		case	KYKYComm_CMD_ScanCtrl_Freeze:
			valueParam["Name"]	= "Freeze";
			valueSend["method"]	= "scan.start.freeze";
			valueSend["id"]		= "scan.setcmd";
			break;
		case	KYKYComm_CMD_ScanCtrl_Sync:
			valueParam["Name"]	= "Sync";
			valueSend["method"]	= "scan.start.sync";
			valueSend["id"]		= "scan.setcmd";
			g_nIsStopScan		= 0;
			break;
		case	KYKYComm_CMD_ScanCtrl_IsStopScan:
			valueParam["Name"]	= "IsStopScan";
			valueSend["method"]	= "scan.getvalue.isstop";
			valueSend["id"]		= "scan.getvalue.isstop";
			break;
		}
	}
	else if( bCmd > KYKYComm_CMD_Get_MagRange -1 && bCmd < KYKYComm_CMD_Get_Magnification )
	{
		// 系统参数：MagRange, ScanRotAngleInit, VacStatus, GunStatus
		switch( bCmd )
		{
		case	KYKYComm_CMD_Get_MagRange:
			valueParam["Name"]	= "MagRange";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_ScanRotAngleInit:
			valueParam["Name"]	= "ScanRotOffset1";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_GunStatus:
			valueParam["Name"]	= "GunStatus";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "status.getvalue";
			break;
		case	KYKYComm_CMD_Get_VacStatus:
			valueParam["Name"]	= "VacStatus";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "status.getvalue";
			break;
		}
	}
	else if( bCmd > KYKYComm_CMD_Get_Magnification -1 && bCmd < KYKYComm_CMD_Set_Magnification )
	{
		// 常规参数：Mag, WD, RotAngle, StigX/Y, B, C, B2
		switch( bCmd )
		{
		case	KYKYComm_CMD_Get_Brightness:
			valueParam["Name"]	= "Brightness";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_Brightness2:
			valueParam["Name"]	= "Brightness2";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_Contrast:
			valueParam["Name"]	= "Contrast";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_Magnification:
			valueParam["Name"]	= "Mag";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_ScanRotAngle:
			valueParam["Name"]	= "ScanRotAngle";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_WorkDistance:
			valueParam["Name"]	= "WD";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_Stigs:
			{
				Json::Value	item;
				item["Name"]	= "StigX";
				valueParam.append(item);
				item["Name"]	= "StigY";
				valueParam.append(item);
				valueSend["method"]	= "sem.getvalue";
				valueSend["id"]		= "common.getvalue";
			}
			break;
		case	KYKYComm_CMD_Get_StigX:
			valueParam["Name"]	= "StigX";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_StigY:
			valueParam["Name"]	= "StigY";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_FieldOfView:
			valueParam["Name"]	= "FOV";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "common.getvalue";
			break;
		}
	}
	else if( bCmd > KYKYComm_CMD_Get_HighVoltage -1 && bCmd < KYKYComm_CMD_Stage_GetStatus )
	{
		// 高压参数：Acc, Fila, Beam, Bias, EMI
		switch( bCmd )
		{
		case	KYKYComm_CMD_Get_HighVoltage:
			valueParam["Name"]	= "Acc";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "hv.getvalue";
			break;
		case	KYKYComm_CMD_Get_Fila:
			valueParam["Name"]	= "Filament";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "hv.getvalue";
			break;
		case	KYKYComm_CMD_Get_BeamCurrent:
			valueParam["Name"]	= "Emission";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "hv.getvalue";
			break;
		case	KYKYComm_CMD_Get_Bias:
			valueParam["Name"]	= "Bias";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "hv.getvalue";
			break;
		case	KYKYComm_CMD_Get_EMI:
			valueParam["Name"]	= "EmissionMonitor";
			valueSend["method"]	= "sem.getvalue";
			valueSend["id"]		= "hv.getvalue";
			break;
		}
	}
	else if( bCmd > KYKYComm_CMD_Stage_GetStatus -1 && bCmd < KYKYComm_CMD_Func_AutoBC )
	{
		// 自动台参数：Status, Range, Pos, Stop, GoHome
		switch( bCmd )
		{
		case	KYKYComm_CMD_Stage_GetStatus:
			valueParam["Name"]	= "Status";
			valueSend["method"]	= "stage.getvalue";
			valueSend["id"]		= "stage.getvalue";
			break;
		case	KYKYComm_CMD_Stage_GetRange:
		case	KYKYComm_CMD_Stage_GetRange2:
		case	KYKYComm_CMD_Stage_GetRange5:
			valueParam["Name"]	= "Range";
			valueSend["method"]	= "stage.getvalue";
			valueSend["id"]		= "stage.getvalue";
			break;
		case	KYKYComm_CMD_Stage_GetPos:
		case	KYKYComm_CMD_Stage_GetPos2:
		case	KYKYComm_CMD_Stage_GetPos5:
			valueParam["Name"]	= "Pos";
			valueSend["method"]	= "stage.getvalue";
			valueSend["id"]		= "stage.getvalue";
			break;
		case	KYKYComm_CMD_Stage_Stop:
			valueParam["Name"]	= "Stop";
			valueSend["method"]	= "stage.cmd";
			valueSend["id"]		= "stage.cmd";
			break;
		case	KYKYComm_CMD_Stage_GoHome:
			valueParam["Name"]	= "GoHome";
			valueSend["method"]	= "stage.cmd";
			valueSend["id"]		= "stage.cmd";
			break;
		}
	}
	else if( bCmd > KYKYComm_CMD_Func_AutoBC -1 && bCmd < KYKYComm_CMD_Func_ShutdownBeam +1 )
	{
		// 应用功能：ABC, clearoff
		switch( bCmd )
		{
		case	KYKYComm_CMD_Func_AutoBC:
			valueParam["Name"]	= "AutoBC";
			valueSend["method"]	= "app.execute";
			valueSend["id"]		= "app.cmd";
			break;
		case	KYKYComm_CMD_Func_ShutdownBeam:
			valueParam["Name"]	= "ShutdownBeam";
			valueSend["method"]	= "app.execute";
			valueSend["id"]		= "app.cmd";
			break;
		}
	}
	valueSend["params"] = valueParam;
	Send_JsonString( valueSend );

	int nSendData = strlen(g_szBufSend);
	nSendData = g_pCSocketCmd->Send(g_szBufSend, strlen(g_szBufSend));

	g_pCSocketCmd->AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
	SetEvent(g_hCSocketThreadCmd);
}

void CCSCmdThread::OnMsgSend_Param(WPARAM wParam, LPARAM lParam)
// 对于客户端来说，只需要发送命令
{
	// 向服务端发送命令及参数，以得到客户端需要的信息
	//发送数据
	//先把发送数据事件关掉
	ResetEvent(g_hCSocketThreadCmd);	
	g_pCSocketCmd->AsyncSelect(FD_CLOSE);		

	Json::Value valueSend, valueParam;
	BYTE bCmd = BYTE(wParam);
	if( bCmd > KYKYComm_CMD_ScanCtrl_Freeze -1 && bCmd < KYKYComm_CMD_Get_MagRange )
	{
		switch( bCmd )
		{
		case	KYKYComm_CMD_ScanCtrl_ActiveArea:
			{
				Json::Value	arrayObj, item;
				item["Name"]	= "Left";
				item["Value"]	= g_rcSetScanArea.left;
				arrayObj.append(item);
				item["Name"]	= "Top";
				item["Value"]	= g_rcSetScanArea.top;
				arrayObj.append(item);
				item["Name"]	= "Right";
				item["Value"]	= g_rcSetScanArea.right;
				arrayObj.append(item);
				item["Name"]	= "Bottom";
				item["Value"]	= g_rcSetScanArea.bottom;
				arrayObj.append(item);
				valueSend["params"] = arrayObj;
				valueSend["method"]	= "scan.setstart.area";
				g_nIsStopScan		= 0;
			}
			break;
		case	KYKYComm_CMD_ScanCtrl_Spot:
			{
				Json::Value	arrayObj, item;
				item["Name"]	= "SpotX";
				item["Value"]	= g_ptSetScanSpot.x;
				arrayObj.append(item);
				item["Name"]	= "SpotY";
				item["Value"]	= g_ptSetScanSpot.y;
				arrayObj.append(item);
				valueSend["params"] = arrayObj;
				valueSend["method"]	= "scan.setstart.spot";
				g_nIsStopScan		= 0;
			}
			break;
		case	KYKYComm_CMD_ScanCtrl_Line_HORZ:
			valueParam["Name"]	= "LineH";
			valueParam["Value"]	= g_nSetLineH;
			valueSend["params"]	= valueParam;
			valueSend["method"]	= "scan.setstart.lineh";
			g_nIsStopScan		= 0;
			break;
		case	KYKYComm_CMD_ScanCtrl_Line_VERT:
			valueParam["Name"]	= "LineV";
			valueParam["Value"]	= g_nSetLineV;
			valueSend["params"]	= valueParam;
			valueSend["method"]	= "scan.setstart.linev";
			g_nIsStopScan		= 0;
			break;
		case	KYKYComm_CMD_Set_ScanCtrl_Speed:
			valueParam["Name"]	= "Speed";
			valueParam["Value"]	= g_nSetSpeed;
			valueSend["params"]	= valueParam;
			valueSend["method"]	= "scan.setvalue.speed";
			break;
		case	KYKYComm_CMD_Set_ScanCtrl_Reso:
			valueParam["Name"]	= "Reso";
			valueParam["Value"]	= g_nSetReso;
			valueSend["params"]	= valueParam;
			valueSend["method"]	= "scan.setvalue.reso";
			break;
		}
		valueSend["id"]			= "scan.setcmd";
		valueSend["jsonrpc"]	="2.0";
	}
	else if( bCmd > KYKYComm_CMD_Set_Magnification -1 && bCmd < KYKYComm_CMD_Get_HighVoltage )
	{
		switch( bCmd )
		{
		case	KYKYComm_CMD_Set_Magnification:
			valueParam["Name"]	= "Mag";
			valueParam["Value"]	= g_dSetMag;
			break;
		case	KYKYComm_CMD_Set_WorkDistance:
			valueParam["Name"]	= "WD";
			valueParam["Value"]	= g_dSetWD;
			break;
		case	KYKYComm_CMD_Set_Brightness:
			valueParam["Name"]	= "Brightness";
			valueParam["Value"]	= g_dSetBrt;
			break;
		case	KYKYComm_CMD_Set_Contrast:
			valueParam["Name"]	= "Contrast";
			valueParam["Value"]	= g_dSetCst;
			break;
		case	KYKYComm_CMD_Set_ScanRotAngle:
			valueParam["Name"]	= "ScanRotAngle";
			valueParam["Value"]	= g_nSetAngle;
			break;
		case	KYKYComm_CMD_Set_Stigs:
			{
				Json::Value	item;
				item["Name"]	= "StigX";
				item["Value"]	= g_szSetStig.cx;
				valueParam.append(item);
				item["Name"]	= "StigY";
				item["Value"]	= g_szSetStig.cy;
				valueParam.append(item);
			}
			break;
		case	KYKYComm_CMD_Set_StigX:
			valueParam["Name"]	= "StigX";
			valueParam["Value"]	= g_szSetStig.cx;
			break;
		case	KYKYComm_CMD_Set_StigY:
			valueParam["Name"]	= "StigY";
			valueParam["Value"]	= g_szSetStig.cy;
			break;
		}
		valueSend["params"]		= valueParam;
		valueSend["id"]			= "common.setvalue";
		valueSend["method"]		= "sem.setvalue";
		valueSend["jsonrpc"]	="2.0";
	}
	else if( bCmd > KYKYComm_CMD_Get_HighVoltage -1 && bCmd < KYKYComm_CMD_Stage_GetStatus )
	{
		switch( bCmd )
		{
		case	KYKYComm_CMD_Set_HighVoltage:
			valueParam["Name"]	= "Acc";
			valueParam["Value"]	= g_dSetHV;
			break;
		case	KYKYComm_CMD_Set_Fila:
			valueParam["Name"]	= "Filament";
			valueParam["Value"]	= g_dSetFila;
			break;
		case	KYKYComm_CMD_Set_BeamCurrent:
			valueParam["Name"]	= "Emission";
			valueParam["Value"]	= g_dSetBeam;
			break;
		case	KYKYComm_CMD_Set_Bias:
			valueParam["Name"]	= "Bias";
			valueParam["Value"]	= g_dSetBias;
			break;
		}
		valueSend["params"]		= valueParam;
		valueSend["id"]			= "hv.setvalue";
		valueSend["method"]		= "hv.setvalue";
		valueSend["jsonrpc"]	="2.0";
	}
	else if( bCmd > KYKYComm_CMD_Stage_SetPos -1 && bCmd < KYKYComm_CMD_Stage_SetPos5 +1 )
	{
		switch( bCmd )
		{
		case	KYKYComm_CMD_Stage_SetPos:
			{
				char szAxis[2];
				sprintf_s( szAxis, "Pos%c", g_groupPos.cIndex );
				valueParam["Name"]	= szAxis;
				valueParam["Value"]	= g_groupPos.fValue;
				valueSend["params"]	= valueParam;
				valueSend["id"]		= "stage.setvalue";
				valueSend["method"]	= "stage.setvalue";
				valueSend["jsonrpc"]="2.0";
			}
			break;
		case	KYKYComm_CMD_Stage_SetPos2:
			{
				Json::Value	arrayObj, item;

				item["Name"]	= "PosX";
				item["Value"]	= g_5fSetPos.fValue1;
				arrayObj.append(item);
				item["Name"]	= "PosY";
				item["Value"]	= g_5fSetPos.fValue2;
				arrayObj.append(item);
				valueSend["params"]	=arrayObj;

				valueSend["id"]		="stage.setvalue";   
				valueSend["method"]	="stage.setvalue";
				valueSend["jsonrpc"]="2.0";
			}
			break;
		case	KYKYComm_CMD_Stage_SetPos5:
			{
				Json::Value	arrayObj, item;

				item["Name"]	= "PosX";
				item["Value"]	= g_5fSetPos.fValue1;
				arrayObj.append(item);
				item["Name"]	= "PosY";
				item["Value"]	= g_5fSetPos.fValue2;
				arrayObj.append(item);
				item["Name"]	= "PosZ";
				item["Value"]	= g_5fSetPos.fValue3;
				arrayObj.append(item);
				item["Name"]	= "PosR";
				item["Value"]	= g_5fSetPos.fValue4;
				arrayObj.append(item);
				item["Name"]	= "PosT";
				item["Value"]	= g_5fSetPos.fValue5;
				arrayObj.append(item);
				valueSend["params"]	=arrayObj;

				valueSend["id"]		="stage.setvalue";   
				valueSend["method"]	="stage.setvalue";
				valueSend["jsonrpc"]="2.0";
			}
			break;
		}
	}
	Send_JsonString( valueSend );

	int nSendData = strlen(g_szBufSend);
	nSendData = g_pCSocketCmd->Send(g_szBufSend, strlen(g_szBufSend));

	g_pCSocketCmd->AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
	SetEvent(g_hCSocketThreadCmd);
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

bool CCSCmdThread::GetServerStatus(int* status)
{
	*status = g_nConnectedCmd;
	return true;
}

bool CCSCmdThread::GetVersion( WORD* ver )
{
	*ver = g_wVersionNumber;
	return true;
}

///////////////////////////////////////////////////////////
bool CCSCmdThread::GetDoubleParam(int index, double* param)
{
	switch( index )
	{
	case	KYKYComm_CMD_Get_ScanCtrl_Speed:
		*param = g_dGetSpeed;
		break;

	case	KYKYComm_CMD_Get_Magnification:
		*param = g_dGetMag;
		break;
	case	KYKYComm_CMD_Get_WorkDistance:
		*param = g_dGetWD;
		break;
	case	KYKYComm_CMD_Get_Brightness:
		*param = g_dGetBrt;
		break;
	case	KYKYComm_CMD_Get_Contrast:
		*param = g_dGetCst;
		break;

	case	KYKYComm_CMD_Get_StigX:
		*param = g_szGetStig.cx;
		break;
	case	KYKYComm_CMD_Get_StigY:
		*param = g_szGetStig.cy;
		break;

	case	KYKYComm_CMD_Get_ScanRotAngleInit:
		*param = g_nGetAngleInit;
		break;
	case	KYKYComm_CMD_Get_ScanRotAngle:
		*param = g_nGetAngle;
		break;

	case	KYKYComm_CMD_Get_HighVoltage:
		*param = g_dGetHV;
		break;
	case	KYKYComm_CMD_Get_Fila:
		*param = g_dGetFila;
		break;
	case	KYKYComm_CMD_Get_BeamCurrent:
		*param = g_dGetBeam;
		break;
	case	KYKYComm_CMD_Get_Bias:
		*param = g_dGetBias;
		break;
	case	KYKYComm_CMD_Get_EMI:
		*param = g_dGetEMI;
		break;
	}
	return true;
}

bool CCSCmdThread::GetDoubleParams(int index, double* param1, double* param2)
{
	switch( index )
	{
	case	KYKYComm_CMD_Get_MagRange:
		*param1 = g_2fGetMagRange.fValue1;
		*param2 = g_2fGetMagRange.fValue2;
		break;
	case	KYKYComm_CMD_Get_FieldOfView:
		*param1 = g_5fGetFOV.fValue1;
		*param2 = g_5fGetFOV.fValue2;
		break;
	case	KYKYComm_CMD_Get_Stigs:
		*param1 = (double)g_szGetStig.cx;
		*param2 = (double)g_szGetStig.cy;
		break;
	case	KYKYComm_CMD_Get_ScanCtrl_Reso:
		*param1 = (double)g_szGetReso.cx;
		*param2 = (double)g_szGetReso.cy;
		break;
	}
	return true;
}

bool CCSCmdThread::GetIntParam(int index, int* param)
{
	switch( index )
	{
	case	KYKYComm_CMD_Sys_GetSEMType:
		*param = g_nTypeSEM;
		break;
	case	KYKYComm_CMD_Sys_GetHighVoltageType:
		*param = g_nTypeHV;
		break;
	case	KYKYComm_CMD_Get_VacStatus:
		*param = g_nStatusVacuum;
		break;
	case	KYKYComm_CMD_Get_GunStatus:
		*param = g_nStatusGun;
		break;

	case	KYKYComm_CMD_Get_FieldOfView:
		*param = (int)(g_5fGetFOV.fValue3);
		break;

	case	KYKYComm_CMD_Stage_GetStatus:
		*param = (int)g_byteStageStatus;
		break;

	case	KYKYComm_CMD_ScanCtrl_IsStopScan:
		*param = g_nIsStopScan;
		break;
	}
	return true;
}

bool CCSCmdThread::SetDoubleParam(int index, double param)
{
	switch( index )
	{
	case	KYKYComm_CMD_Set_Magnification:
		g_dSetMag = param;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_Magnification, LPARAM(&g_dSetMag) );
		break;
	case	KYKYComm_CMD_Set_WorkDistance:
		g_dSetWD = param;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_WorkDistance, LPARAM(&g_dSetWD) );
		break;
	case	KYKYComm_CMD_Set_Brightness:
		g_dSetBrt = param;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_Brightness, LPARAM(&g_dSetBrt) );
		break;
	case	KYKYComm_CMD_Set_Contrast:
		g_dSetCst = param;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_Contrast, LPARAM(&g_dSetCst) );
		break;
	case	KYKYComm_CMD_Set_StigX:
		g_szSetStig.cx = (int)param;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_StigX, LPARAM(&(g_szSetStig.cx)) );
		break;
	case	KYKYComm_CMD_Set_StigY:
		g_szSetStig.cy = (int)param;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_StigY, LPARAM(&(g_szSetStig.cy)) );
		break;

	case	KYKYComm_CMD_Set_ScanRotAngle:
		g_nSetAngle = (int)param;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_ScanRotAngle, LPARAM(&g_nSetAngle) );
		break;

	case	KYKYComm_CMD_Set_HighVoltage:
		g_dSetHV = param;	// 从客户端得到的是KV
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_HighVoltage, LPARAM(&g_dSetHV) );
		break;
	case	KYKYComm_CMD_Set_Fila:
		g_dSetFila = param;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_Fila, LPARAM(&g_dSetFila) );
		break;
	case	KYKYComm_CMD_Set_BeamCurrent:
		g_dSetBeam = param;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_BeamCurrent, LPARAM(&g_dSetBeam) );
		break;
	case	KYKYComm_CMD_Set_Bias:
		g_dSetBias = param;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_Bias, LPARAM(&g_dSetBias) );
		break;
	}
	Sleep(20);
	return true;
}

bool CCSCmdThread::SetDoubleParams(int index, double param1, double param2)
{
	switch( index )
	{
	case	KYKYComm_CMD_Set_Stigs:
		g_szSetStig.cx = (int)param1;
		g_szSetStig.cy = (int)param2;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_StigX, LPARAM(&(g_szSetStig.cx)) );
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_StigY, LPARAM(&(g_szSetStig.cy)) );
		break;
	}
	Sleep(20);
	return true;
}

bool CCSCmdThread::SetIntParam(int index, int param)
{
	switch( index )
	{
	case	KYKYComm_CMD_Set_ScanCtrl_Speed:
		g_nSetSpeed = param;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_ScanCtrl_Speed, LPARAM(&g_nSetSpeed) );
		break;
	case	KYKYComm_CMD_Set_ScanCtrl_Reso:
		g_nSetReso = param;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Set_ScanCtrl_Reso, LPARAM(&g_nSetReso) );
		break;
	}
	Sleep(20);
	return true;
}

bool CCSCmdThread::SetCommand(int cmd)
{
	switch( cmd )
	{
	case	KYKYComm_CMD_Func_AutoBC:
		break;
	case	KYKYComm_CMD_Func_ShutdownBeam:
		break;
	}
	return true;
}
///////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

bool CCSCmdThread::StageGetStatus( int* status )
{
	*status = (int)g_byteStageStatus;
	return true;
}

bool CCSCmdThread::StageGetGratings( int* xSt, int* ySt )
{
	*xSt	= (int)(g_szGetGrating.cx);
	*ySt	= (int)(g_szGetGrating.cy);
	return true;
}

bool CCSCmdThread::StageGetDoubleParam( int index, int axis, float* param )
{
	switch( index )
	{
	case	KYKYComm_CMD_Stage_GetRange:
		{
			switch( axis )
			{
			case	0:
				param[0] = g_5fGetRange[0].fValue1;
				param[1] = g_5fGetRange[1].fValue1;
				break;
			case	1:
				param[0] = g_5fGetRange[0].fValue2;
				param[1] = g_5fGetRange[1].fValue2;
				break;
			case	2:
				param[0] = g_5fGetRange[0].fValue3;
				param[1] = g_5fGetRange[1].fValue3;
				break;
			case	3:
				param[0] = g_5fGetRange[0].fValue4;
				param[1] = g_5fGetRange[1].fValue4;
				break;
			case	4:
				param[0] = g_5fGetRange[0].fValue5;
				param[1] = g_5fGetRange[1].fValue5;
				break;
			}
		}
		break;
	case	KYKYComm_CMD_Stage_GetPos:
		{
			switch( axis )
			{
			case	0:
				*param = g_5fGetPos.fValue1;
				break;
			case	1:
				*param = g_5fGetPos.fValue2;
				break;
			case	2:
				*param = g_5fGetPos.fValue3;
				break;
			case	3:
				*param = g_5fGetPos.fValue4;
				break;
			case	4:
				*param = g_5fGetPos.fValue5;
				break;
			}
		}
		break;
	}
	return true;
}

bool CCSCmdThread::StageGetDoubleParam2( int index, float* param )
{
	switch( index )
	{
	case	KYKYComm_CMD_Stage_GetRange2:
		{
			param[0] = g_5fGetRange[0].fValue1;
			param[1] = g_5fGetRange[1].fValue1;
			param[2] = g_5fGetRange[0].fValue2;
			param[3] = g_5fGetRange[1].fValue2;
		}
		break;
	case	KYKYComm_CMD_Stage_GetPos2:
		{
			param[0] = g_5fGetPos.fValue1;
			param[1] = g_5fGetPos.fValue2;
		}
		break;
	}
	return true;
}

bool CCSCmdThread::StageGetDoubleParam5( int index, float* param )
{
	switch( index )
	{
	case	KYKYComm_CMD_Stage_GetRange5:
		{
			param[0] = g_5fGetRange[0].fValue1;
			param[1] = g_5fGetRange[1].fValue1;
			param[2] = g_5fGetRange[0].fValue2;
			param[3] = g_5fGetRange[1].fValue2;
			param[4] = g_5fGetRange[0].fValue3;
			param[5] = g_5fGetRange[1].fValue3;
			param[6] = g_5fGetRange[0].fValue4;
			param[7] = g_5fGetRange[1].fValue4;
			param[8] = g_5fGetRange[0].fValue5;
			param[9] = g_5fGetRange[1].fValue5;
		}
		break;
	case	KYKYComm_CMD_Stage_GetPos5:
		{
			param[0] = g_5fGetPos.fValue1;
			param[1] = g_5fGetPos.fValue2;
			param[2] = g_5fGetPos.fValue3;
			param[3] = g_5fGetPos.fValue4;
			param[4] = g_5fGetPos.fValue5;
		}
		break;
	}
	return true;
}

bool CCSCmdThread::StageSetPos( int axis, float pos )
{
	switch( axis )
	{
	case	0:
		g_5fSetPos.fValue1	= pos;
		g_groupPos.cIndex	= 'X';
		break;
	case	1:
		g_5fSetPos.fValue2	= pos;
		g_groupPos.cIndex	= 'Y';
		break;
	case	2:
		g_5fSetPos.fValue3	= pos;
		g_groupPos.cIndex	= 'Z';
		break;
	case	3:
		g_5fSetPos.fValue4	= pos;
		g_groupPos.cIndex	= 'R';
		break;
	case	4:
		g_5fSetPos.fValue5	= pos;
		g_groupPos.cIndex	= 'T';
		break;
	}
	g_groupPos.fValue = pos;
	PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Stage_SetPos, LPARAM(&g_groupPos) );
	return true;
}

bool CCSCmdThread::StageSetPos2( float xSt, float ySt )
{
	g_5fSetPos.fValue1 = xSt;
	g_5fSetPos.fValue2 = ySt;
	PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Stage_SetPos5, LPARAM(&g_5fSetPos) );
	return true;
}

bool CCSCmdThread::StageSetPos5( float xSt, float ySt, float zSt, float rSt, float tSt )
{
	g_5fSetPos.fValue1 = xSt;
	g_5fSetPos.fValue2 = ySt;
	g_5fSetPos.fValue3 = zSt;
	g_5fSetPos.fValue4 = rSt;
	g_5fSetPos.fValue5 = tSt;
	PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Stage_SetPos5, LPARAM(&g_5fSetPos) );
	return true;
}





bool CCSCmdThread::ScanStartSpot( int x, int y )
{
	g_ptSetScanSpot.x = x;
	g_ptSetScanSpot.y = y;
	PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_ScanCtrl_Spot, LPARAM(&g_ptSetScanSpot) );
	return true;
}

bool CCSCmdThread::ScanStartHLine( int y )
{
	g_nSetLineH = y;
	PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_ScanCtrl_Line_HORZ, LPARAM(&g_nSetLineH) );
	return true;
}

bool CCSCmdThread::ScanStartVLine( int x )
{
	g_nSetLineV = x;
	PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_ScanCtrl_Line_VERT, LPARAM(&g_nSetLineV) );
	return true;
}

bool CCSCmdThread::ScanStartArea(int left, int top, int right, int bottom)
{
	g_rcSetScanArea.left	= left;
	g_rcSetScanArea.top		= top;
	g_rcSetScanArea.right	= right;
	g_rcSetScanArea.bottom	= bottom;
	PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_ScanCtrl_ActiveArea, LPARAM(&g_rcSetScanArea) );
	return true;
}
