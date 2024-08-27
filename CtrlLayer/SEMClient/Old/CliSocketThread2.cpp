// CliSocketThread2.cpp : implementation file
//

#include "stdafx.h"
#include "CliSocketThread2.h"

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

HANDLE g_hClientSocketThread2 = CreateEvent(NULL, TRUE, FALSE, "ClientSocketThread2");

// 将从服务端接收的命令串，分析出包含单一指令的字符串，放入缓冲区SendServer
DWORD WINAPI ThreadAnalyzeServer(LPVOID lpParam);
// 将包含单一指令的字符串从缓冲区SendServer中一一取出，发送给客户端
DWORD WINAPI ThreadRecvFromServer(LPVOID lpParam);

CRITICAL_SECTION	csFifoRecvServerMsg, csFifoRecvFromServer;
static	BOOL g_bEndClientThreads = FALSE;
int		g_nClientThreadCounts;

/////////////////////////////////////////////////
/////////////////////////////////////////////////

extern HWND g_hMain2;
CSEMSocket*	g_pClientSocket;	// 附加在这个线程上的CSocket

/////////////////////////////////////////////
int			g_nConnected;		// 已和服务端连接为1，未连接为0
WORD		g_wVersionNumber;
BYTE		g_byteStageStatus;

int			g_nGetAngleInit, g_nGetAngle, g_nSetAngle;
int			g_nStatusVacuum, g_nStatusGun, g_nTypeHV, g_nTypeSEM;
double		g_dGetMag, g_dGetWD, g_dGetBrt, g_dGetBrt2, g_dGetCst, g_dGetHV, g_dGetFila, g_dGetBeam, g_dGetBias, g_dGetEMI;
double		g_dSetMag, g_dSetWD, g_dSetBrt, g_dSetBrt2, g_dSetCst, g_dSetHV, g_dSetFila, g_dSetBeam, g_dSetBias;
CSize		g_szGetStig, g_szSetStig, g_szGetGrating;
KYKYCommMsg_Data5f	g_5fGetRange, g_5fGetPos, g_5fSetPos;
KYKYCommMsg_Data2f	g_2fGetMagRange;
KYKYCommMsg_DataGroup	g_groupPos;

int			g_nScanSpeed, g_nScanReso, g_nSetLineH, g_nSetLineV;
CPoint		g_ptSetScanSpot;
CRect		g_rcSetScanArea;
/////////////////////////////////////////////
char	g_szBufSend[1024];
KYKYCommMsg_ImgData g_msgImageFrameData;

/////////////////////////////////////////////////////////////////////////////
// CCliSocketThread2

IMPLEMENT_DYNCREATE(CCliSocketThread2, CWinThread)

CCliSocketThread2::CCliSocketThread2()
{
	g_pClientSocket = new CSEMSocket();     //!!!problem
	g_nConnected = 0;    //未连接

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

	g_szGetStig.cx		= 0;
	g_szGetStig.cy		= 0;
	g_szGetGrating.cx	= 0;
	g_szGetGrating.cy	= 0;
	g_ptSetScanSpot.x	= 0;
	g_ptSetScanSpot.y	= 0;

	g_5fGetRange.fValue1	= 0.0;
	g_5fGetRange.fValue2	= 0.0;
	g_5fGetRange.fValue3	= 0.0;
	g_5fGetRange.fValue4	= 0.0;
	g_5fGetRange.fValue5	= 0.0;
	g_5fGetPos.fValue1		= 0.0;
	g_5fGetPos.fValue2		= 0.0;
	g_5fGetPos.fValue3		= 0.0;
	g_5fGetPos.fValue4		= 0.0;
	g_5fGetPos.fValue5		= 0.0;
	g_2fGetMagRange.fValue1	= 0.0;
	g_2fGetMagRange.fValue2	= 0.0;
}

CCliSocketThread2::~CCliSocketThread2()
{
	delete g_pClientSocket;
	g_pClientSocket = NULL;
}

void Send_JsonString( Json::Value valueSend )
{
	Json::FastWriter writer;
	std::string strSend = writer.write(valueSend);
	const char *sendData = strSend.c_str();
	ZeroMemory( &g_szBufSend, sizeof(g_szBufSend) );
	int nLen = strlen(sendData);
//	sprintf( g_szBufSend, "%d:[%s],", nLen, sendData );
// 	nLen = strlen(g_szBufSend);
// 	g_szBufSend[nLen] = '\0';
	sprintf( g_szBufSend, "%d:[%s", nLen, sendData );
	nLen = strlen(g_szBufSend);
	g_szBufSend[nLen-1] = '\0';
	strcat(g_szBufSend, "],");
	nLen = strlen(g_szBufSend);
	g_szBufSend[nLen] = '\0';
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
		strcpy( group, strchr( id, '.')+1 );
		*strchr(id, '.') = '\0';
	}
	else
	{
//		return;
		strcpy( group, id );
	}
*/
	if( !(param.isMember("Name")) )
		return;
	std::string strName = param["Name"].asString();
	const char *name = strName.c_str();
	if( strcmp(name, "MagRange") != 0 )
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
				g_dGetHV = param["Value"].asDouble();
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
		}
	}
	else
	{
		if( param.isMember("Min") && param.isMember("Max") )
		{
			g_2fGetMagRange.fValue1 = param["Min"].asDouble();
			g_2fGetMagRange.fValue2 = param["Max"].asDouble();
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
				g_nConnected = 1;
				PostMessage(g_hMain2, WM_USER_KYKYComm_Connect_Status, KYKYComm_CMD_Sys_Connect_Success, 1);
			}
		}
		else if( strcmp(name, "Disconnect") == 0 )
		{
			BOOL bValue = param["Value"].asBool();
			if( bValue )
			{
				g_nConnected = 0;
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
				g_5fGetRange.fValue1 = max -min;
			else if( strcmp(name, "RangeY") == 0 )
				g_5fGetRange.fValue2 = max -min;
			else if( strcmp(name, "RangeZ") == 0 )
				g_5fGetRange.fValue3 = max -min;
			else if( strcmp(name, "RangeR") == 0 )
				g_5fGetRange.fValue4 = max -min;
			else if( strcmp(name, "RangeT") == 0 )
				g_5fGetRange.fValue5 = max -min;
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
		if( g_bEndClientThreads )
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
			do 
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
							strcpy( szBuf, strstr(msg.szRecv, "}}]") +3 );
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
							strcpy( szBuf, strstr(msg.szRecv, "}]}]") +4 );
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
					strcpy( szAnalyze, strchr( msg.szRecv, '{') );
					exd = strrchr( szAnalyze, ']' );
					if( exd != NULL )
						*exd = '\0';
					KYKYCommClientJSON_Recv recvClient;
					recvClient.strRecv = szAnalyze;
//					EnterCriticalSection(&csFifoSendToHost);
					g_fifoRecvFromServer.Input(recvClient);
//					LeaveCriticalSection(&csFifoSendToHost);
				}
				strcpy( msg.szRecv, szBuf );
			} while (strchr( msg.szRecv, '}]') != NULL);
		}
		LeaveCriticalSection(&csFifoRecvServerMsg);

		Sleep(1);
	}
	while( g_nClientThreadCounts > 1 )
		Sleep(1);
	g_nClientThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit FEThreadAnalyzeClient, counts = %d", g_nClientThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

// 将包含单一指令的字符串从缓冲区RecvFromServer中一一取出，发送给客户端
DWORD WINAPI ThreadRecvFromServer(LPVOID lpParam)
{
	while (TRUE)
	{
		if( g_bEndClientThreads )
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
	g_nClientThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit FEThreadSendToHost, counts = %d", g_nClientThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

void StartThreads()		// (HWND hMain)
{
//	g_destHandle = hMain;
	g_bEndClientThreads = FALSE;
	HANDLE hThread1, hThread2;
//	InitializeCriticalSection(&csSocket);
	InitializeCriticalSection(&csFifoRecvServerMsg);
	InitializeCriticalSection(&csFifoRecvFromServer);
	
	hThread1 = CreateThread(NULL, 0, ThreadAnalyzeServer, NULL, 0, NULL);
	g_nClientThreadCounts++;
	
	hThread2 = CreateThread(NULL, 0, ThreadRecvFromServer, NULL, 0, NULL);
	g_nClientThreadCounts++;
}

BOOL CloseThreads()
{
	ResetEvent(g_hClientSocketThread2);	
	g_pClientSocket->AsyncSelect(FD_CLOSE);
	g_bEndClientThreads = TRUE;
//	Sleep(2000);
	while(true)
	{
		if( g_nClientThreadCounts < 1 )
			break;
	}
//	DeleteCriticalSection(&csSocket);
	DeleteCriticalSection(&csFifoRecvServerMsg);
	DeleteCriticalSection(&csFifoRecvFromServer);

	SetEvent( g_hClientSocketThread2 );
	WaitForSingleObject( g_hClientSocketThread2, INFINITE );
	return TRUE;
}

BOOL CCliSocketThread2::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	if (!AfxSocketInit())
	{
		return CWinThread::InitInstance();
	}

	if (!g_pClientSocket->Create())
	{
		AfxMessageBox("socket创建失败！"); 
		return CWinThread::InitInstance();
	}

	if (g_pClientSocket->Connect(m_strIPAddr, KYKYComm_ListenPort1))
	{
		g_pClientSocket->m_pThrd = this;
		g_pClientSocket->m_nPort = KYKYComm_ListenPort1;
		g_pClientSocket->AsyncSelect(FD_READ|FD_WRITE|FD_CLOSE);
		g_nConnected = 1;
	}
	else
		TRACE("命令线程连接失败！\n");
	if( g_nConnected > 0 )
	{
		StartThreads();	//(g_hMain2);
		return TRUE;
	}
	return CWinThread::InitInstance();
}

int CCliSocketThread2::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	if( g_nConnected > 0 )
		CloseThreads();
// 		WaitForSingleObject(g_hClientSocketThread2, INFINITE);    //通过等待，解决了在断开连接时程序崩溃的问题
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCliSocketThread2, CWinThread)
	//{{AFX_MSG_MAP(CCliSocketThread2)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_Thread_Read, OnMsgRead)
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_ClientThreadSend_Command, OnMsgSend_Command)
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_ClientThreadSend_Param, OnMsgSend_Param)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCliSocketThread2 message handlers

void CCliSocketThread2::SetServerIP(CString sIP)
{
	m_strIPAddr = sIP;
}

void CCliSocketThread2::Reset(LPVOID ptr)
{
//	m_pMainWnd = (CWnd*)ptr; 
}

// 得到了从Server端发来的信息？？？？？
// 根据接收到的数据里的命令字或字节数，提取相应信息
void CCliSocketThread2::OnMsgRead(WPARAM wParam,LPARAM lParam)
{
	// 接收数据
	// 先把接收数据事件关掉
	// 关闭接收数据，避免后续接收数据不停传来，造成处理堵塞，这样读写一次完成
// 	ResetEvent(g_hClientSocketThread2);	
// 	g_pClientSocket->AsyncSelect(FD_CLOSE);
	KYKYCommClient_Recv_Package msg;
//	EnterCriticalSection(&csSocket);
	int nRecv = g_pClientSocket->Receive(msg.szRecv, 512);
	msg.szRecv[nRecv] = '\0';
//	DeleteCriticalSection(&csSocket);
	if( nRecv > 0 )
	{
//		EnterCriticalSection(&csFifoRecvClientMsg);
		g_fifoRecvServerMsg.Input(msg);
//		DeleteCriticalSection(&csFifoRecvClientMsg);
	}
	char output[1024] = {0};
	sprintf( output, "Server: MsgRead = %s\n", msg.szRecv );
//	OutputDebugString(output);
	//重新打开接收数据事件
// 	g_pClientSocket->AsyncSelect(FD_READ|FD_WRITE|FD_CLOSE);
// 	SetEvent(g_hClientSocketThread2);

/*	// 接收数据
	// 先把接收数据事件关掉
	// 关闭接收数据，避免后续接收数据不停传来，造成处理堵塞，这样读写一次完成
	ResetEvent(g_hClientSocketThread2);	
	g_pClientSocket->AsyncSelect(FD_CLOSE);
	char szBufRecv[1024], szBuf[1024];
	int nRecvLen = g_pClientSocket->Receive(szBufRecv, 512); 
	szBufRecv[nRecvLen] = '\0';

	/////////////////////////////////////////////////////////////////
	if( strchr(szBufRecv, ':') != NULL )
	{
		strcpy( szBuf, strchr( szBufRecv, ':')+1 );
		*strchr(szBufRecv, ':') = '\0';
		int nLen = atoi(szBufRecv);
		int nRealLen = strlen(szBuf) -1;	// 字符串字节数（去掉结尾的','）
		if( nRealLen < nLen )
		{
			// 实际收到字节数小于JSON字符串应有长度，直接备份后返回
			strcpy( m_szRecvBak, szBuf );
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
			strcpy( szBuf, strchr( szBufRecv, '{' ) );	// szBuf中是完整的JSON字符串
			std::string strRecv = szBuf;
			Recv_TranslateJson( strRecv );
		}
	}
	//重新打开接收数据事件
	g_pClientSocket->AsyncSelect(FD_READ|FD_WRITE|FD_CLOSE);
	SetEvent(g_hClientSocketThread2);*/	
}

void CCliSocketThread2::OnMsgSend_Command(WPARAM wParam, LPARAM lParam)
// 对于客户端来说，只需要发送命令
{
	// 向服务端发送命令，以得到客户端需要的信息
	//发送数据
	//先把发送数据事件关掉
	ResetEvent(g_hClientSocketThread2);	
	g_pClientSocket->AsyncSelect(FD_CLOSE);		

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
	else if( bCmd > KYKYComm_CMD_ScanCtrl_Freeze -1 && bCmd < KYKYComm_CMD_Get_MagRange )
	{
		// 扫描参数：type, speed, resolution
		switch( bCmd )
		{
		case	KYKYComm_CMD_ScanCtrl_Speed1:
			valueParam["Name"]	= "Speed1";
			valueSend["method"]	= "scan.setcmd";
			break;
		case	KYKYComm_CMD_ScanCtrl_Speed2:
			valueParam["Name"]	= "Speed1";
			valueSend["method"]	= "scan.setcmd";
			break;
		case	KYKYComm_CMD_ScanCtrl_Speed3:
			valueParam["Name"]	= "Speed1";
			valueSend["method"]	= "scan.setcmd";
			break;
		case	KYKYComm_CMD_ScanCtrl_Speed4:
			valueParam["Name"]	= "Speed1";
			valueSend["method"]	= "scan.setcmd";
			break;
		case	KYKYComm_CMD_ScanCtrl_Reso1:
			valueParam["Name"]	= "Reso1";
			valueSend["method"]	= "scan.setcmd";
			break;
		case	KYKYComm_CMD_ScanCtrl_Reso2:
			valueParam["Name"]	= "Reso1";
			valueSend["method"]	= "scan.setcmd";
			break;
		case	KYKYComm_CMD_ScanCtrl_Normal:
			valueParam["Name"]	= "Normal";
			valueSend["method"]	= "scan.start.normal";
			break;
		case	KYKYComm_CMD_ScanCtrl_Freeze:
			valueParam["Name"]	= "Freeze";
			valueSend["method"]	= "scan.start.freeze";
			break;
		case	KYKYComm_CMD_ScanCtrl_Sync:
			valueParam["Name"]	= "Sync";
			valueSend["method"]	= "scan.start.sync";
			break;
		}
		valueSend["id"]		= "scan.setcmd";
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
			valueParam["Name"]	= "Range";
			valueSend["method"]	= "stage.getvalue";
			valueSend["id"]		= "stage.getvalue";
			break;
		case	KYKYComm_CMD_Stage_GetPos:
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
	nSendData = g_pClientSocket->Send(g_szBufSend, strlen(g_szBufSend));

	g_pClientSocket->AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
	SetEvent(g_hClientSocketThread2);
}

void CCliSocketThread2::OnMsgSend_Param(WPARAM wParam, LPARAM lParam)
// 对于客户端来说，只需要发送命令
{
	// 向服务端发送命令及参数，以得到客户端需要的信息
	//发送数据
	//先把发送数据事件关掉
	ResetEvent(g_hClientSocketThread2);	
	g_pClientSocket->AsyncSelect(FD_CLOSE);		

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
				valueSend["method"]	= "scan.start.area";
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
				valueSend["method"]	= "scan.start.spot";
			}
			break;
		case	KYKYComm_CMD_ScanCtrl_Line_HORZ:
			valueParam["Name"]	= "LineH";
			valueParam["Value"]	= g_nSetLineH;
			valueSend["params"]	= valueParam;
			valueSend["method"]	= "scan.start.lineh";
			break;
		case	KYKYComm_CMD_ScanCtrl_Line_VERT:
			valueParam["Name"]	= "LineV";
			valueParam["Value"]	= g_nSetLineV;
			valueSend["params"]	= valueParam;
			valueSend["method"]	= "scan.start.linev";
			break;
		case	KYKYComm_CMD_ScanCtrl_Speed:
			valueParam["Name"]	= "Speed";
			valueParam["Value"]	= g_nScanSpeed;
			valueSend["params"]	= valueParam;
			valueSend["method"]	= "scan.setcmd.speed";
			break;
		case	KYKYComm_CMD_ScanCtrl_Reso:
			valueParam["Name"]	= "Reso";
			valueParam["Value"]	= g_nScanReso;
			valueSend["params"]	= valueParam;
			valueSend["method"]	= "scan.setcmd.reso";
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
				item["Name"]	= "y";
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
			valueParam["Name"]	= "Beam";
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
				sprintf( szAxis, "%c", g_groupPos.cIndex );
				valueParam["Axis"]	= szAxis;
				valueParam["Pos"]	= g_groupPos.fValue;
				valueSend["params"]	= valueParam;
				valueSend["id"]		= "stage.setvalue";
				valueSend["method"]	= "stage.setvalue";
				valueSend["jsonrpc"]="2.0";
			}
			break;
		case	KYKYComm_CMD_Stage_SetPos2:
			{
				Json::Value	arrayObj, item;

				item["Axis"]	= "x";
				item["Pos"]		= g_5fSetPos.fValue1;
				arrayObj.append(item);
				item["Axis"]	= "y";
				item["Pos"]		= g_5fSetPos.fValue2;
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

				item["Axis"]	= "x";
				item["Pos"]		= g_5fSetPos.fValue1;
				arrayObj.append(item);
				item["Axis"]	= "y";
				item["Pos"]		= g_5fSetPos.fValue2;
				arrayObj.append(item);
				item["Axis"]	= "z";
				item["Pos"]		= g_5fSetPos.fValue3;
				arrayObj.append(item);
				item["Axis"]	= "r";
				item["Pos"]		= g_5fSetPos.fValue4;
				arrayObj.append(item);
				item["Axis"]	= "t";
				item["Pos"]		= g_5fSetPos.fValue5;
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
	nSendData = g_pClientSocket->Send(g_szBufSend, strlen(g_szBufSend));

	g_pClientSocket->AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
	SetEvent(g_hClientSocketThread2);
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

bool CCliSocketThread2::GetServerStatus(int* status)
{
	*status = g_nConnected;
	return true;
}

bool CCliSocketThread2::GetVersion( WORD* ver )
{
	*ver = g_wVersionNumber;
	return true;
}

///////////////////////////////////////////////////////////
bool CCliSocketThread2::GetDoubleParam(int index, double* param)
{
	switch( index )
	{
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
		*param = g_dGetHV /1000;
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

bool CCliSocketThread2::GetDoubleParams(int index, double* param1, double* param2)
{
	switch( index )
	{
	case	KYKYComm_CMD_Get_MagRange:
		*param1 = g_2fGetMagRange.fValue1;
		*param2 = g_2fGetMagRange.fValue2;
		break;
	case	KYKYComm_CMD_Get_Stigs:
		*param1 = (double)g_szGetStig.cx;
		*param2 = (double)g_szGetStig.cy;
		break;
	}
	return true;
}

bool CCliSocketThread2::GetIntParam(int index, int* param)
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

	case	KYKYComm_CMD_Stage_GetStatus:
		*param = (int)g_byteStageStatus;
		break;
	}
	return true;
}

bool CCliSocketThread2::SetDoubleParam(int index, double param)
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
		g_dSetHV = param *1000;
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
	return true;
}

bool CCliSocketThread2::SetDoubleParams(int index, double param1, double param2)
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
	return true;
}

bool CCliSocketThread2::SetIntParam(int index, int param)
{
	switch( index )
	{
	case	KYKYComm_CMD_ScanCtrl_Speed:
		g_nScanSpeed = param;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_ScanCtrl_Speed, LPARAM(&g_nScanSpeed) );
		break;
	case	KYKYComm_CMD_ScanCtrl_Reso:
		g_nScanReso = param;
		PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_ScanCtrl_Reso, LPARAM(&g_nScanReso) );
		break;
	}
	return true;
}

bool CCliSocketThread2::SetCommand(int cmd)
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

bool CCliSocketThread2::StageGetStatus( int* status )
{
	*status = (int)g_byteStageStatus;
	return true;
}

bool CCliSocketThread2::StageGetGratings( int* xSt, int* ySt )
{
	*xSt	= (int)(g_szGetGrating.cx);
	*ySt	= (int)(g_szGetGrating.cy);
	return true;
}

bool CCliSocketThread2::StageGetDoubleParam( int index, int axis, float* param )
{
	switch( index )
	{
	case	KYKYComm_CMD_Stage_GetRange:
		{
			switch( axis )
			{
			case	0:
				*param = g_5fGetRange.fValue1;
				break;
			case	1:
				*param = g_5fGetRange.fValue2;
				break;
			case	2:
				*param = g_5fGetRange.fValue3;
				break;
			case	3:
				*param = g_5fGetRange.fValue4;
				break;
			case	4:
				*param = g_5fGetRange.fValue5;
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

bool CCliSocketThread2::StageGetDoubleParam2( int index, float* xSt, float* ySt )
{
	switch( index )
	{
	case	KYKYComm_CMD_Stage_GetRange2:
		{
			*xSt = g_5fGetRange.fValue1;
			*ySt = g_5fGetRange.fValue2;
		}
		break;
	case	KYKYComm_CMD_Stage_GetPos2:
		{
			*xSt = g_5fGetPos.fValue1;
			*ySt = g_5fGetPos.fValue2;
		}
		break;
	}
	return true;
}

bool CCliSocketThread2::StageGetDoubleParam5( int index, float* xSt, float* ySt, float* zSt, float* rSt, float* tSt )
{
	switch( index )
	{
	case	KYKYComm_CMD_Stage_GetRange5:
		{
			*xSt = g_5fGetRange.fValue1;
			*ySt = g_5fGetRange.fValue2;
			*zSt = g_5fGetRange.fValue3;
			*rSt = g_5fGetRange.fValue4;
			*tSt = g_5fGetRange.fValue5;
		}
		break;
	case	KYKYComm_CMD_Stage_GetPos5:
		{
			*xSt = g_5fGetPos.fValue1;
			*ySt = g_5fGetPos.fValue2;
			*zSt = g_5fGetPos.fValue3;
			*rSt = g_5fGetPos.fValue4;
			*tSt = g_5fGetPos.fValue5;
		}
		break;
	}
	return true;
}

bool CCliSocketThread2::StageSetPos( int axis, float pos )
{
	switch( axis )
	{
	case	0:
		g_5fSetPos.fValue1	= pos;
		g_groupPos.cIndex	= 'x';
		break;
	case	1:
		g_5fSetPos.fValue2	= pos;
		g_groupPos.cIndex	= 'y';
		break;
	case	2:
		g_5fSetPos.fValue3	= pos;
		g_groupPos.cIndex	= 'z';
		break;
	case	3:
		g_5fSetPos.fValue4	= pos;
		g_groupPos.cIndex	= 'r';
		break;
	case	4:
		g_5fSetPos.fValue5	= pos;
		g_groupPos.cIndex	= 't';
		break;
	}
	g_groupPos.fValue = pos;
	PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Stage_SetPos, LPARAM(&g_groupPos) );
	return true;
}

bool CCliSocketThread2::StageSetPos2( float xSt, float ySt )
{
	g_5fSetPos.fValue1 = xSt;
	g_5fSetPos.fValue2 = ySt;
	PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Stage_SetPos5, LPARAM(&g_5fSetPos) );
	return true;
}

bool CCliSocketThread2::StageSetPos5( float xSt, float ySt, float zSt, float rSt, float tSt )
{
	g_5fSetPos.fValue1 = xSt;
	g_5fSetPos.fValue2 = ySt;
	g_5fSetPos.fValue3 = zSt;
	g_5fSetPos.fValue4 = rSt;
	g_5fSetPos.fValue5 = tSt;
	PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_Stage_SetPos5, LPARAM(&g_5fSetPos) );
	return true;
}





bool CCliSocketThread2::ScanStartSpot( int x, int y )
{
	g_ptSetScanSpot.x = x;
	g_ptSetScanSpot.y = y;
	PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_ScanCtrl_Spot, LPARAM(&g_ptSetScanSpot) );
	return true;
}

bool CCliSocketThread2::ScanStartHLine( int y )
{
	g_nSetLineH = y;
	PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_ScanCtrl_Line_HORZ, LPARAM(&g_nSetLineH) );
	return true;
}

bool CCliSocketThread2::ScanStartVLine( int x )
{
	g_nSetLineV = x;
	PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_ScanCtrl_Line_VERT, LPARAM(&g_nSetLineV) );
	return true;
}

bool CCliSocketThread2::ScanStartArea(int left, int top, int right, int bottom)
{
	g_rcSetScanArea.left	= left;
	g_rcSetScanArea.top		= top;
	g_rcSetScanArea.right	= right;
	g_rcSetScanArea.bottom	= bottom;
	PostThreadMessage(WM_USER_KYKYComm_ClientThreadSend_Param, KYKYComm_CMD_ScanCtrl_ActiveArea, LPARAM(&g_rcSetScanArea) );
	return true;
}
