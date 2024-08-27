// SerSocketThread.cpp : implementation file
//

#include "stdafx.h"
#include "TCPServer.h"
#include "SerSocketThread.h"

#include <afxtempl.h>
#include <cfifo.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


typedef struct tagKYKYComm_Recv_Package
{
	char szRecv[768];
} KYKYComm_Recv_Package;

typedef struct tagKYKYCommJSON_Recv
{
	std::string	strRecv;
} KYKYCommJSON_Recv;

CFIFO<KYKYComm_Recv_Package, KYKYComm_Recv_Package&>	m_fifoRecvClientMsg(200);	// 大：每一个可能包含多条命令
CFIFO<KYKYCommJSON_Recv, KYKYCommJSON_Recv&>	m_fifoSendToHost(500);		// 小：每一个包含单一命令

HANDLE g_hServerSocketThread3 = CreateEvent(NULL,TRUE,FALSE,"ServerSocketThread3");

// 将从客户端接收的命令串，分析出包含单一指令的字符串，放入缓冲区SendHost
DWORD WINAPI ThreadAnalyzeClient(LPVOID lpParam);
// 将包含单一指令的字符串从缓冲区SendHost中一一取出，发送给主程序
DWORD WINAPI ThreadSendToHost(LPVOID lpParam);

CRITICAL_SECTION	csSocket, csFifoRecvClientMsg, csFifoSendToHost;
static	BOOL m_bEndThreads = FALSE;
int		m_nThreadCounts;

/////////////////////////////////////////////////
/////////////////////////////////////////////////

HWND	g_destHandle;		// 主程序句柄
CSEMSocket m_serverSocket;	// 附加在这个线程上的CSocket

KYKYCommMsg_ImgData		m_MsgImgData;
KYKYCommMsg_Ver			m_MsgVersion;
// KYKYCommMsg_ImageFrame		m_MsgImg;

KYKYCommMsg_SendDataC1i m_MsgSendDataC1i;
KYKYCommMsg_SendDataC2i m_MsgSendDataC2i;
KYKYCommMsg_SendDataC1f m_MsgSendDataC1f;
KYKYCommMsg_SendDataC2f m_MsgSendDataC2f;
KYKYCommMsg_SendDataC5f m_MsgSendDataC5f;
KYKYCommMsg_Data2f m_2fData;
KYKYCommMsg_Data5f m_5fData;
KYKYCommMsg_Data2fGroup m_2fDataGroup;
KYKYCommMsg_Data5fGroup m_5fDataGroup;
char	m_szSend[1024];


#include <vector>
using namespace std ;
//vector<string> m_queryNames;
vector<short> m_queryNames, m_queryStage;
double	m_dMag, m_dWD, m_dBrt, m_dBrt2, m_dCst, m_dAcc, m_dFila, m_dBias, m_dEmi, m_dEmiMon;	// 10
double	m_dMagRange[2], m_dStageRange[2][5], m_dStagePos[5];
int		m_nSemType, m_nHVType, m_nScanRotAngleInit, m_nScanRotAngle, m_nVacStatus, m_nGunStatus, m_nStageStatus;
int		m_nStig[2];
CRect	m_rcScanArea;
CPoint	m_ptSpot;

/////////////////////////////////////////////////////////////////////////////
// CSerSocketThread

IMPLEMENT_DYNCREATE(CSerSocketThread, CWinThread)

CSerSocketThread::CSerSocketThread()
{
	m_MsgSendDataC1i.bHead1	= 0x99;
	m_MsgSendDataC1i.bHead2	= 0x88;
	m_MsgSendDataC2i.bHead1	= 0x99;
	m_MsgSendDataC2i.bHead2	= 0x88;
	m_MsgSendDataC1f.bHead1	= 0x99;
	m_MsgSendDataC1f.bHead2	= 0x88;
	m_MsgSendDataC2f.bHead1	= 0x99;
	m_MsgSendDataC2f.bHead2	= 0x88;
	m_MsgSendDataC5f.bHead1	= 0x99;
	m_MsgSendDataC5f.bHead2	= 0x88;

	m_dMag	= 0.0;
	m_dWD	= 0.0;
	m_dBrt	= 0.0;
	m_dBrt2	= 0.0;
	m_dCst	= 0.0;
	m_dAcc	= 0.0;
	m_dFila	= 0.0;
	m_dBias	= 0.0;
	m_dEmi	= 0.0;
	m_dEmiMon = 0.0;

	int i, j;
	for( i=0; i<2; i++ )
		m_dMagRange[i] = 0.0;
	for( i=0; i<5; i++ )
		m_dStagePos[i] = 0.0;
	for( i=0; i<2; i++ )
	{
		for( j=0; j<5; j++ )
			m_dStageRange[i][j] = 0.0;
	}
	for( i=0; i<2; i++ )
		m_nStig[i] = 0.0;

	m_nSemType	= 0;
	m_nHVType	= 0;
	m_nScanRotAngleInit	= 0;
	m_nScanRotAngle	= 0;
	m_nVacStatus	= 0;
	m_nGunStatus	= 0;
	m_nStageStatus	= 0;

	m_nThreadCounts	= 0;
	m_nPort			= -1;
}

CSerSocketThread::~CSerSocketThread()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		m_serverSocket.Close();
		m_hSocket = NULL;
	}
}

///////////////////////////////////////////////////////////
// 1.打开6X00主程序
// 2.JsonTcpTest中设置IP为192.168.1.58，Port为3000
//
// 3.JsonTcpTest中输入
// [{"jsonrpc":"2.0","id":"2","method":"params.sem.getnames","params":[]}]
//
// 4.SEMServer的OnMsgRead中收到如下字符串
// 71:[{"jsonrpc":"2.0","id":"2","method":"params.sem.getnames","params":[]}],
///////////////////////////////////////////////////////////

void Send_JsonString( Json::Value valueSend )
{
	Json::FastWriter writer;
	std::string strSend = writer.write(valueSend);
	const char *sendData = strSend.c_str();
	ZeroMemory( &m_szSend, sizeof(m_szSend) );
	int nLen = strlen(sendData);
// 	sprintf( m_szSend, "%d:%s,", nLen, sendData );
// 	nLen = strlen(m_szSend);
// 	m_szSend[nLen] = '\0';
	sprintf( m_szSend, "%d:[%s", nLen, sendData );
	nLen = strlen(m_szSend);
	m_szSend[nLen-1] = '\0';
	strcat(m_szSend, "],");
	nLen = strlen(m_szSend);
	m_szSend[nLen] = '\0';
	
// 	m_serverSocket.AsyncSelect(FD_CLOSE);
// 	ResetEvent(g_hServerSocketThread3);
//	EnterCriticalSection(&csSocket);
	nLen = m_serverSocket.Send(m_szSend, strlen(m_szSend));
//	LeaveCriticalSection(&csSocket);
// 	SetEvent(g_hServerSocketThread3);
// 	m_serverSocket.AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
	
	char output[1024];
	sprintf( output, "Server: Send_JsonString = %s\n", m_szSend );
	OutputDebugString( output );
}

void Send_SemParams( short nIndex )
{
	switch( nIndex )
	{
	case	KYKYComm_CMD_Get_MagRange:
		break;
	case	KYKYComm_CMD_Get_Magnification:
		break;
	case	KYKYComm_CMD_Get_WorkDistance:
		break;
	case	KYKYComm_CMD_Get_StigX:
		break;
	case	KYKYComm_CMD_Get_StigY:
		break;
	case	KYKYComm_CMD_Get_Brightness:
		break;
	case	KYKYComm_CMD_Get_Brightness2:
		break;
	case	KYKYComm_CMD_Get_Contrast:
		break;
	case	KYKYComm_CMD_Get_ScanRotAngleInit:
		break;
	case	KYKYComm_CMD_Get_ScanRotAngle:
		break;
	case	KYKYComm_CMD_Get_HighVoltage:
		break;
	case	KYKYComm_CMD_Get_Fila:
		break;
	case	KYKYComm_CMD_Get_Bias:
		break;
	case	KYKYComm_CMD_Get_BeamCurrent:
		break;
	case	KYKYComm_CMD_Get_EMI:
		break;

	case	KYKYComm_CMD_Get_VacStatus:
		break;
	case	KYKYComm_CMD_Get_GunStatus:
		break;

	case	KYKYComm_CMD_Sys_GetSEMType:
		break;
	case	KYKYComm_CMD_Sys_GetHighVoltageType:
		break;

	case	KYKYComm_CMD_Stage_GetRange:
		break;
	case	KYKYComm_CMD_Stage_GetPos:
		break;
	}
}

void Send_DoubleParam( BYTE bcommand )
{
	Json::Value valueSend, valueParam, valueArray;
	switch (bcommand)
	{
	case	KYKYComm_CMD_Get_Magnification:
		valueParam["Name"]	= "Mag";
		valueParam["Value"]	= ((int)(m_dMag *10 +0.5)) /10.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_WorkDistance:
		valueParam["Name"]	= "WD";
		valueParam["Value"]	= ((int)(m_dWD *1000000 +0.5)) /1000000.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_Brightness:
		valueParam["Name"]	= "Brightness";
		valueParam["Value"]	= ((int)(m_dBrt *100 +0.5)) /100.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_Brightness2:
		valueParam["Name"]	= "Brightness2";
		valueParam["Value"]	= ((int)(m_dBrt2 *100 +0.5)) /100.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_Contrast:
		valueParam["Name"]	= "Contrast";
		valueParam["Value"]	= ((int)(m_dCst *100 +0.5)) /100.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_HighVoltage:
		valueParam["Name"]	= "Acc";
		valueParam["Value"]	= ((int)(m_dAcc *1000 +0.5)) /1000.0;	// KV
		valueArray.append( valueParam );
		valueSend["id"]		= "hv.getvalue";
		break;
	case	KYKYComm_CMD_Get_Fila:
		valueParam["Name"]	= "Filament";
		valueParam["Value"]	= ((int)(m_dFila *1000 +0.5)) /1000.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "hv.getvalue";
		break;
	case	KYKYComm_CMD_Get_Bias:
		valueParam["Name"]	= "Bias";
		valueParam["Value"]	= ((int)(m_dBias *1000 +0.5)) /1000.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "hv.getvalue";
		break;
	case	KYKYComm_CMD_Get_BeamCurrent:
		valueParam["Name"]	= "Emission";
		valueParam["Value"]	= ((int)(m_dEmi *100 +0.5)) /100.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "hv.getvalue";
		break;
	case	KYKYComm_CMD_Get_EMI:
		valueParam["Name"]	= "EmissionMonitor";
		valueParam["Value"]	= ((int)(m_dEmiMon *100 +0.5)) /100.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "hv.getvalue";
		break;
	}
	valueSend["result"] = valueArray;
	valueSend["method"]	= "sem.getvalue";
// 	valueSend["id"]="MyId";   
// 	valueSend["jsonrpc"]="2.0";
	Send_JsonString( valueSend );
}

void Send_DoubleParams( BYTE bcommand )
{
	Json::Value valueSend, valueParam, valueArray;
	switch (bcommand)
	{
	case	KYKYComm_CMD_Get_MagRange:
		valueSend["method"] = "sem.getvalue";
		valueSend["id"]		= "status.getvalue";
		valueParam["Name"]	= "MagRange";
		valueParam["Min"]	= ((int)(m_dMagRange[0] *100 +0.5)) /100.0;
		valueParam["Max"]	= ((int)(m_dMagRange[1] *100 +0.5)) /100.0;
		valueArray.append( valueParam );
		break;
	case	KYKYComm_CMD_Stage_GetRange:
		valueSend["method"]	= "stage.getvalue";
		valueSend["id"]		= "stage.getaxes";
		valueParam["Name"]	= "RangeX";
		valueParam["Min"]	= ((int)(m_dStageRange[1][0] *1000 +0.5)) /1000.0;
		valueParam["Max"]	= ((int)(m_dStageRange[0][0] *1000 +0.5)) /1000.0;
		valueArray.append( valueParam );
		valueParam["Name"]	= "RangeY";
		valueParam["Min"]	= ((int)(m_dStageRange[1][1] *1000 +0.5)) /1000.0;
		valueParam["Max"]	= ((int)(m_dStageRange[0][1] *1000 +0.5)) /1000.0;
		valueArray.append( valueParam );
		valueParam["Name"]	= "RangeZ";
		valueParam["Min"]	= ((int)(m_dStageRange[1][2] *1000 +0.5)) /1000.0;
		valueParam["Max"]	= ((int)(m_dStageRange[0][2] *1000 +0.5)) /1000.0;
		valueArray.append( valueParam );
		valueParam["Name"]	= "RangeR";
		valueParam["Min"]	= ((int)(m_dStageRange[1][3] *100 +0.5)) /100.0;
		valueParam["Max"]	= ((int)(m_dStageRange[0][3] *100 +0.5)) /100.0;
		valueArray.append( valueParam );
		valueParam["Name"]	= "RangeT";
		valueParam["Min"]	= ((int)(m_dStageRange[1][4] *100 +0.5)) /100.0;
		valueParam["Max"]	= ((int)(m_dStageRange[0][4] *100 +0.5)) /100.0;
		valueArray.append( valueParam );
		break;
	case	KYKYComm_CMD_Stage_GetPos:
		valueSend["method"]	= "stage.getvalue";
		valueSend["id"]		= "stage.getpos";
		valueParam["Name"]	= "PosX";
		valueParam["Value"]	= m_dStagePos[0];
		valueArray.append( valueParam );
		valueParam["Name"]	= "PosY";
		valueParam["Value"]	= m_dStagePos[1];
		valueArray.append( valueParam );
		valueParam["Name"]	= "PosZ";
		valueParam["Value"]	= m_dStagePos[2];
		valueArray.append( valueParam );
		valueParam["Name"]	= "PosR";
		valueParam["Value"]	= m_dStagePos[3];
		valueArray.append( valueParam );
		valueParam["Name"]	= "PosT";
		valueParam["Value"]	= m_dStagePos[4];
		valueArray.append( valueParam );
		break;
	}
	valueSend["result"] = valueArray;

// 	valueSend["id"]="MyId";   
// 	valueSend["jsonrpc"]="2.0";
	Send_JsonString( valueSend );
}

void Send_IntParam( BYTE bcommand )
{
	Json::Value valueSend, valueParam, valueArray;
	switch (bcommand)
	{
	case	KYKYComm_CMD_Sys_GetSEMType:
		valueParam["Name"]	= "SEM";
		valueParam["Value"]	= m_nSemType;
		valueArray.append( valueParam );
		valueSend["method"]	= "sem.gettype";
		valueSend["id"]		= "sys.getvalue";
		break;
	case	KYKYComm_CMD_Sys_GetHighVoltageType:
		valueParam["Name"]	= "HighVoltage";
		valueParam["Value"]	= m_nHVType;
		valueArray.append( valueParam );
		valueSend["method"]	= "sem.gettype";
		valueSend["id"]		= "sys.getvalue";
		break;

	case	KYKYComm_CMD_Get_ScanRotAngleInit:
		valueParam["Name"]	= "ScanRotOffset1";
		valueParam["Value"]	= m_nScanRotAngleInit;
		valueArray.append( valueParam );
		valueSend["method"]	= "sem.getvalue";
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_ScanRotAngle:
		valueParam["Name"]	= "ScanRotAngle";
		valueParam["Value"]	= m_nScanRotAngle;
		valueArray.append( valueParam );
		valueSend["method"]	= "sem.getvalue";
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_VacStatus:
		valueParam["Name"]	= "VacStatus";
		valueParam["Value"]	= m_nVacStatus;
		valueArray.append( valueParam );
		valueSend["method"]	= "sem.getvalue";
		valueSend["id"]		= "status.getvalue";
		break;
	case	KYKYComm_CMD_Get_GunStatus:
		valueParam["Name"]	= "GunStatus";
		valueParam["Value"]	= m_nGunStatus;
		valueArray.append( valueParam );
		valueSend["method"]	= "sem.getvalue";
		valueSend["id"]		= "status.getvalue";
		break;

	case	KYKYComm_CMD_Stage_GetStatus:
		valueParam["Name"]	= "Status";
		valueParam["Value"]	= m_nStageStatus;
		valueArray.append( valueParam );
		valueSend["method"]	= "stage.getvalue";
		valueSend["id"]		= "stage.getvalue";
		break;
	}
	valueSend["result"] = valueArray;
// 	valueSend["id"]="MyId";   
// 	valueSend["jsonrpc"]="2.0";
	Send_JsonString( valueSend );
}

void Send_IntParams( BYTE bcommand )
{
	Json::Value valueSend, valueArray, valueParam;
	switch (bcommand)
	{
	case	KYKYComm_CMD_Get_StigX:
		valueParam["Name"]	= "StigX";
		valueParam["Value"]	= m_nStig[0];
		valueArray.append( valueParam );
		valueSend["method"]	= "sem.getvalue";
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_StigY:
		valueParam["Name"]	= "StigY";
		valueParam["Value"]	= m_nStig[1];
		valueArray.append( valueParam );
		valueSend["method"]	= "sem.getvalue";
		valueSend["id"]		= "common.getvalue";
		break;
	}
	valueSend["result"] = valueArray;
// 	valueSend["id"]="MyId";   
// 	valueSend["jsonrpc"]="2.0";
	Send_JsonString( valueSend );
}

void Send_SemGetValueArray()
{
	Json::Value valueSend, valueParam, arrayObj;
	int nSize = m_queryNames.size();
	for( int i=0; i<nSize; i++ )
	{
		switch( m_queryNames[i] )
		{
		case	KYKYComm_CMD_Get_Magnification:
			valueParam["Name"]	= "Mag";
			valueParam["Value"]	= m_dMag;
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_WorkDistance:
			valueParam["Name"]	= "WD";
			valueParam["Value"]	= m_dWD;
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_Brightness:
			valueParam["Name"]	= "Brightness";
			valueParam["Value"]	= m_dBrt;
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_Brightness2:
			valueParam["Name"]	= "Brightness2";
			valueParam["Value"]	= m_dBrt2;
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_Contrast:
			valueParam["Name"]	= "Contrast";
			valueParam["Value"]	= m_dCst;
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_StigX:
			valueParam["Name"]	= "StigX";
			valueParam["Value"]	= m_nStig[0];
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_StigY:
			valueParam["Name"]	= "StigY";
			valueParam["Value"]	= m_nStig[1];
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_ScanRotAngleInit:
			valueParam["Name"]	= "ScanRotOffset1";
			valueParam["Value"]	= m_nScanRotAngleInit;
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_ScanRotAngle:
			valueParam["Name"]	= "ScanRotAngle";
			valueParam["Value"]	= m_nScanRotAngle;
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_HighVoltage:
			valueParam["Name"]	= "Acc";
			valueParam["Value"]	= m_dAcc;
			arrayObj.append( valueParam );
			valueSend["id"]		= "hv.getvalue";
			break;
		case	KYKYComm_CMD_Get_Fila:
			valueParam["Name"]	= "Filament";
			valueParam["Value"]	= m_dFila;
			arrayObj.append( valueParam );
			valueSend["id"]		= "hv.getvalue";
			break;
		case	KYKYComm_CMD_Get_Bias:
			valueParam["Name"]	= "Bias";
			valueParam["Value"]	= m_dBias;
			arrayObj.append( valueParam );
			valueSend["id"]		= "hv.getvalue";
			break;
		case	KYKYComm_CMD_Get_BeamCurrent:
			valueParam["Name"]	= "Emission";
			valueParam["Value"]	= m_dEmi;
			arrayObj.append( valueParam );
			valueSend["id"]		= "hv.getvalue";
			break;
		case	KYKYComm_CMD_Get_EMI:
			valueParam["Name"]	= "EmissionMonitor";
			valueParam["Value"]	= m_dEmiMon;
			arrayObj.append( valueParam );
			valueSend["id"]		= "hv.getvalue";
			break;
		case	KYKYComm_CMD_Get_VacStatus:
			valueParam["Name"]	= "VacStatus";
			valueParam["Value"]	= m_nVacStatus;
			arrayObj.append( valueParam );
			valueSend["id"]		= "status.getvalue";
			break;
		case	KYKYComm_CMD_Get_GunStatus:
			valueParam["Name"]	= "GunStatus";
			valueParam["Value"]	= m_nGunStatus;
			arrayObj.append( valueParam );
			valueSend["id"]		= "status.getvalue";
			break;
		}
	}
	valueSend["result"] = arrayObj;
	valueSend["method"]	= "sem.getvalue";
// 	valueSend["id"]="MyId";   
// 	valueSend["jsonrpc"]="2.0";
	Send_JsonString( valueSend );
}

void Recv_SemGetType( const char* param )
{
// 	char output[255];
// 	sprintf( output, "Server: post GetType %s\n", param );
// 	OutputDebugString( output );
	short nGetCmd = -1;
	if( strcmp(param, "SEM") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Sys_GetSEMType;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Sys_GetSEMType, 0);
	}
	else if( strcmp(param, "HighVoltage") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Sys_GetHighVoltageType;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Sys_GetHighVoltageType, 0);
	}
	if( nGetCmd > 0 )
		Send_IntParam( nGetCmd );
}

void Recv_SemGetTypeArray( Json::Value arrayObj )
{
	std::string strParams;
	UINT nSize = arrayObj.size();
	for( UINT i=0; i<nSize; i++ )
	{
		if( arrayObj[i].isMember("Name") )
		{
			strParams = arrayObj[i]["Name"].asString();
			const char *param = strParams.c_str();
			Recv_SemGetType( param );
		}
	}
}

short Recv_SemGetValue( const char* param, BOOL bSend )
{
// 	char output[255];
// 	sprintf( output, "Server: post GetValue %s\n", param );
// 	OutputDebugString( output );
	short nGetCmd = -1;
	if( strcmp(param, "MagRange") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_MagRange;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_MagRange, 0);
	}
	else if( strcmp(param, "Mag") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_Magnification;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_Magnification, 0);
	}
	else if( strcmp(param, "WD") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_WorkDistance;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_WorkDistance, 0);
	}
	else if( strcmp(param, "StigX") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_StigX;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_StigX, 0);
	}
	else if( strcmp(param, "StigY") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_StigY;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_StigY, 0);
	}
	else if( strcmp(param, "Brightness") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_Brightness;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_Brightness, 0);
	}
	else if( strcmp(param, "Brightness2") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_Brightness2;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_Brightness2, 0);
	}
	else if( strcmp(param, "Contrast") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_Contrast;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_Contrast, 0);
	}

	else if( strcmp(param, "ScanRotAngle") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_ScanRotAngle;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_ScanRotAngle, 0);
	}
	else if( strcmp(param, "ScanRotOffset1") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_ScanRotAngleInit;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_ScanRotAngleInit, 0);
	}

	else if( strcmp(param, "Acc") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_HighVoltage;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_HighVoltage, 0);
	}
	else if( strcmp(param, "Filament") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_Fila;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_Fila, 0);
	}
	else if( strcmp(param, "Bias") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_Bias;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_Bias, 0);
	}
	else if( strcmp(param, "Emission") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_BeamCurrent;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_BeamCurrent, 0);
	}
	else if( strcmp(param, "EmissionMonitor") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_EMI;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_EMI, 0);
	}

	else if( strcmp(param, "VacStatus") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_VacStatus;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_VacStatus, 0);
	}
	else if( strcmp(param, "GunStatus") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_GunStatus;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_GunStatus, 0);
	}

	if( bSend && nGetCmd > -1 )
	{
		// 需要立即发送数据给客户端
		switch( nGetCmd )
		{
		case	KYKYComm_CMD_Get_Magnification:
		case	KYKYComm_CMD_Get_Brightness:
		case	KYKYComm_CMD_Get_Brightness2:
		case	KYKYComm_CMD_Get_Contrast:
		case	KYKYComm_CMD_Get_WorkDistance:
		case	KYKYComm_CMD_Get_HighVoltage:
		case	KYKYComm_CMD_Get_Bias:
		case	KYKYComm_CMD_Get_BeamCurrent:
		case	KYKYComm_CMD_Get_Fila:
		case	KYKYComm_CMD_Get_EMI:
			Send_DoubleParam( nGetCmd );
			break;
		case	KYKYComm_CMD_Get_MagRange:
			Send_DoubleParams( nGetCmd );
			break;
		case	KYKYComm_CMD_Get_ScanRotAngleInit:
		case	KYKYComm_CMD_Get_ScanRotAngle:
		case	KYKYComm_CMD_Get_VacStatus:
		case	KYKYComm_CMD_Get_GunStatus:
			Send_IntParam( nGetCmd );
			break;
		case	KYKYComm_CMD_Get_StigX:
		case	KYKYComm_CMD_Get_StigY:
			Send_IntParams( nGetCmd );
			break;
		}
	}
	return nGetCmd;
}

void Recv_SemGetValueArray( Json::Value arrayObj )
{
	m_queryNames.clear();
	std::string strParams;
	UINT nSize = arrayObj.size();
	int nCmdIndex = -1;
	for( UINT i=0; i<nSize; i++ )
	{
		if( arrayObj[i].isMember("Name") )
		{
			strParams = arrayObj[i]["Name"].asString();
			const char *param = strParams.c_str();
			nCmdIndex = Recv_SemGetValue( param, FALSE );
			m_queryNames.push_back( nCmdIndex );
		}
	}
	
	if( !m_queryNames.empty() )
		Send_SemGetValueArray();
}

void Recv_SemSetValue( Json::Value param )
{
	if( param.isMember("Name") && param.isMember("Value") )
	{
		std::string strName = param["Name"].asString();
		const char* name = strName.c_str();
		if( strstr(name, "Mag") != NULL )
		{
			m_MsgSendDataC1f.fValue = (float)(param["Value"].asDouble());
			m_MsgSendDataC1f.bCommand = KYKYComm_CMD_Set_Magnification;
			PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_Magnification, LPARAM(&m_MsgSendDataC1f) );
		}
		else if( strstr(name, "WD") != NULL )
		{
			m_MsgSendDataC1f.fValue = (float)(param["Value"].asDouble());
			m_MsgSendDataC1f.bCommand = KYKYComm_CMD_Set_WorkDistance;
			PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_WorkDistance, LPARAM(&m_MsgSendDataC1f) );
		}
		else if( strstr(name, "Brightness") != NULL )
		{
			m_MsgSendDataC1f.fValue = (float)(param["Value"].asDouble());
			m_MsgSendDataC1f.bCommand = KYKYComm_CMD_Set_Brightness;
			PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_Brightness, LPARAM(&m_MsgSendDataC1f) );
		}
		else if( strstr(name, "Brightness2") != NULL )
		{
			m_MsgSendDataC1f.fValue = (float)(param["Value"].asDouble());
			m_MsgSendDataC1f.bCommand = KYKYComm_CMD_Set_Brightness2;
			PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_Brightness2, LPARAM(&m_MsgSendDataC1f) );
		}
		else if( strstr(name, "Contrast") != NULL )
		{
			m_MsgSendDataC1f.fValue = (float)(param["Value"].asDouble());
			m_MsgSendDataC1f.bCommand = KYKYComm_CMD_Set_Contrast;
			PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_Contrast, LPARAM(&m_MsgSendDataC1f) );
		}
		else if( strstr(name, "ScanRotAngle") != NULL )
		{
			m_MsgSendDataC1i.nValue = param["Value"].asInt();
			m_MsgSendDataC1i.bCommand = KYKYComm_CMD_Set_ScanRotAngle;
			PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_ScanRotAngle, LPARAM(&m_MsgSendDataC1i) );
		}
		else if( strstr(name, "Acc") != NULL )
		{
			m_MsgSendDataC1f.fValue = (float)(param["Value"].asDouble() *1000.0);
			m_MsgSendDataC1f.bCommand = KYKYComm_CMD_Set_HighVoltage;
			PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_HighVoltage, LPARAM(&m_MsgSendDataC1f) );
		}
		else if( strstr(name, "Filament") != NULL )
		{
			m_MsgSendDataC1f.fValue = (float)(param["Value"].asDouble());
			m_MsgSendDataC1f.bCommand = KYKYComm_CMD_Set_Fila;
			PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_Fila, LPARAM(&m_MsgSendDataC1f) );
		}
		else if( strstr(name, "Bias") != NULL )
		{
			m_MsgSendDataC1f.fValue = (float)(param["Value"].asDouble());
			m_MsgSendDataC1f.bCommand = KYKYComm_CMD_Set_Bias;
			PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_Bias, LPARAM(&m_MsgSendDataC1f) );
		}
		else if( strstr(name, "Emission") != NULL )
		{
			m_MsgSendDataC1f.fValue = (float)(param["Value"].asDouble());
			m_MsgSendDataC1f.bCommand = KYKYComm_CMD_Set_BeamCurrent;
			PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_BeamCurrent, LPARAM(&m_MsgSendDataC1f) );
		}
		else if( strstr(name, "StigX") != NULL )
		{
			m_MsgSendDataC1i.nValue = param["Value"].asInt();
			m_MsgSendDataC1i.bCommand = KYKYComm_CMD_Set_StigX;
			PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_StigX, LPARAM(&m_MsgSendDataC1i) );
		}
		else if( strstr(name, "StigY") != NULL )
		{
			m_MsgSendDataC1i.nValue = param["Value"].asInt();
			m_MsgSendDataC1i.bCommand = KYKYComm_CMD_Set_StigY;
			PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_StigY, LPARAM(&m_MsgSendDataC1i) );
		}
	}
}

void Recv_SemSetValues( Json::Value root )
{
	char output[255];
	if( root["params"].isArray() )
	{
		Json::Value arrayObj = root["params"];
		UINT i, nSize = arrayObj.size();
		for( i=0; i<nSize; i++ )
		{
			Json::Value param = arrayObj[i];
			Recv_SemSetValue( param );
		}
	}
	else
	{
		Json::Value param = root["params"];
		Recv_SemSetValue( param );
	}
}

short Recv_StageGetParam( const char* param, BOOL bSend )
{
	short nGetCmd = -1;
	if( strcmp(param, "Status") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Stage_GetStatus;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Stage_GetStatus, 0);
	}
	else if( strcmp(param, "Range") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Stage_GetRange;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Stage_GetRange, 0);
	}
	else if( strcmp(param, "Pos") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Stage_GetPos;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Stage_GetPos, 0);
	}
	
	if( bSend && nGetCmd > -1 )
	{
		// 需要立即发送数据给客户端
		switch( nGetCmd )
		{
		case	KYKYComm_CMD_Stage_GetStatus:
			Send_IntParam( nGetCmd );
			break;
		case	KYKYComm_CMD_Stage_GetRange:
		case	KYKYComm_CMD_Stage_GetPos:
			Send_DoubleParams( nGetCmd );
			break;
		}
	}
	return nGetCmd;
}

void Send_StageGetValueArray()
{
	Json::Value valueSend, valueParam, arrayObj;
	int nSize = m_queryStage.size();
	for( int i=0; i<nSize; i++ )
	{
		switch( m_queryStage[i] )
		{
		case	KYKYComm_CMD_Stage_GetStatus:
			valueParam["Name"]	= "Status";
			valueParam["Value"]	= m_nStageStatus;
			arrayObj.append( valueParam );
			valueSend["id"]		= "stage.getstatus";
			break;
		case	KYKYComm_CMD_Stage_GetRange:
			valueParam["Name"]	= "RangeX";
			valueParam["Min"]	= ((int)(m_dStageRange[1][0] *1000 +0.5)) /1000.0;
			valueParam["Max"]	= ((int)(m_dStageRange[0][0] *1000 +0.5)) /1000.0;
			arrayObj.append( valueParam );
			valueParam["Name"]	= "RangeY";
			valueParam["Min"]	= ((int)(m_dStageRange[1][1] *1000 +0.5)) /1000.0;
			valueParam["Max"]	= ((int)(m_dStageRange[0][1] *1000 +0.5)) /1000.0;
			arrayObj.append( valueParam );
			valueParam["Name"]	= "RangeZ";
			valueParam["Min"]	= ((int)(m_dStageRange[1][2] *1000 +0.5)) /1000.0;
			valueParam["Max"]	= ((int)(m_dStageRange[0][2] *1000 +0.5)) /1000.0;
			arrayObj.append( valueParam );
			valueParam["Name"]	= "RangeR";
			valueParam["Min"]	= ((int)(m_dStageRange[1][3] *100 +0.5)) /100.0;
			valueParam["Max"]	= ((int)(m_dStageRange[0][3] *100 +0.5)) /100.0;
			arrayObj.append( valueParam );
			valueParam["Name"]	= "RangeT";
			valueParam["Min"]	= ((int)(m_dStageRange[1][4] *100 +0.5)) /100.0;
			valueParam["Max"]	= ((int)(m_dStageRange[0][4] *100 +0.5)) /100.0;
			arrayObj.append( valueParam );
			valueSend["id"]		= "stage.getaxes";
			break;
		case	KYKYComm_CMD_Stage_GetPos:
			valueParam["Name"]	= "PosX";
			valueParam["Value"]	= m_dStagePos[0];
			arrayObj.append( valueParam );
			valueParam["Name"]	= "PosY";
			valueParam["Value"]	= m_dStagePos[1];
			arrayObj.append( valueParam );
			valueParam["Name"]	= "PosZ";
			valueParam["Value"]	= m_dStagePos[2];
			arrayObj.append( valueParam );
			valueParam["Name"]	= "PosR";
			valueParam["Value"]	= m_dStagePos[3];
			arrayObj.append( valueParam );
			valueParam["Name"]	= "PosT";
			valueParam["Value"]	= m_dStagePos[4];
			arrayObj.append( valueParam );
			valueSend["id"]		= "stage.getpos";
			break;
		}
	}
	valueSend["result"] = arrayObj;
	valueSend["method"]	= "stage.getvalue";
// 	valueSend["id"]="MyId";   
// 	valueSend["jsonrpc"]="2.0";
	Send_JsonString( valueSend );
}

void Recv_StageGetParamArray( Json::Value arrayObj )
{
	m_queryStage.clear();
	std::string strParams;
	UINT nSize = arrayObj.size();
	int nCmdIndex = -1;
	for( UINT i=0; i<nSize; i++ )
	{
		if( arrayObj[i].isMember("Name") )
		{
			strParams = arrayObj[i]["Name"].asString();
			const char *param = strParams.c_str();
			nCmdIndex = Recv_StageGetParam( param, FALSE );
			m_queryStage.push_back( nCmdIndex );
		}
	}
	
	if( !m_queryStage.empty() )
		Send_StageGetValueArray();
}

void Recv_StageSetParam( Json::Value root, const char* method )
{
	if( root["params"].isArray() )
	{
		Json::Value params = root["params"];
		UINT i, nSize = params.size();
		m_MsgSendDataC5f.Values.fValue1 = 100.0;
		m_MsgSendDataC5f.Values.fValue2 = 100.0;
		m_MsgSendDataC5f.Values.fValue3 = 100.0;
		m_MsgSendDataC5f.Values.fValue4 = 100.0;
		m_MsgSendDataC5f.Values.fValue5 = 100.0;
		for( i=0; i<nSize; i++ )
		{
			if( params[i].isMember("Axis") && params[i].isMember("Pos") )
			{
				float fPos = (float)(params[i]["Pos"].asDouble());
				std::string strName = params[i]["Axis"].asString();
				const char* name = strName.c_str();
				if( strstr(name, "x") != NULL )
					m_MsgSendDataC5f.Values.fValue1 = fPos;
				else if( strstr(name, "y") != NULL )
					m_MsgSendDataC5f.Values.fValue2 = fPos;
				else if( strstr(name, "z") != NULL )
					m_MsgSendDataC5f.Values.fValue3 = fPos;
				else if( strstr(name, "r") != NULL )
					m_MsgSendDataC5f.Values.fValue4 = fPos;
				else if( strstr(name, "t") != NULL )
					m_MsgSendDataC5f.Values.fValue5 = fPos;
			}
		}
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Stage_SetPos5, LPARAM(&m_MsgSendDataC5f) );
	}
	else
	{
		if( root["params"].isMember("Axis") && root["params"].isMember("Pos") )
		{
			float fPos = (float)(root["params"]["Pos"].asDouble());
			std::string strName = root["params"]["Axis"].asString();
			const char* name = strName.c_str();
			char cAxis = name[0];
			switch( cAxis )
			{
			case	'x':
				m_MsgSendDataC1f.bCommand	= 0;
				m_MsgSendDataC1f.fValue		= fPos;
				break;
			case	'y':
				m_MsgSendDataC1f.bCommand	= 1;
				m_MsgSendDataC1f.fValue		= fPos;
				break;
			case	'z':
				m_MsgSendDataC1f.bCommand	= 2;
				m_MsgSendDataC1f.fValue		= fPos;
				break;
			case	'r':
				m_MsgSendDataC1f.bCommand	= 3;
				m_MsgSendDataC1f.fValue		= fPos;
				break;
			case	't':
				m_MsgSendDataC1f.bCommand	= 4;
				m_MsgSendDataC1f.fValue		= fPos;
				break;
			}
			PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Stage_SetPos, LPARAM(&m_MsgSendDataC1f) );
		}
	}
}

void Recv_StageSetCmd( const char* method )
{
	if( strcmp(method, "Stop") == 0 )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Stage_Stop, 0);
	else if( strcmp(method, "GoHome") == 0 )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Stage_GoHome, 0);
}

void Recv_StageSetCmdArray( Json::Value arrayObj )
{
	std::string strParams;
	UINT nSize = arrayObj.size();
	for( UINT i=0; i<nSize; i++ )
	{
		if( arrayObj[i].isMember("Name") )
		{
			strParams = arrayObj[i]["Name"].asString();
			const char *param = strParams.c_str();
			Recv_StageSetCmd( param );
		}
	}
}

void Recv_AppSetCommand( const char* param )
{
// 	char output[255];
// 	sprintf( output, "Server: post AppSetCmd %s\n", param );
// 	OutputDebugString( output );
	if( strcmp(param, "AutoBC") == 0 )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Func_AutoBC, 0);
	else if( strcmp(param, "Disconnect") == 0 )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Sys_Disconnect, 0);
	else if( strcmp(param, "ShutdownBeam") == 0 )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Func_ShutdownBeam, 0);
}

void Recv_AppSetCommandArray( Json::Value arrayObj )
{
	std::string strParams;
	UINT nSize = arrayObj.size();
	for( UINT i=0; i<nSize; i++ )
	{
		if( arrayObj[i].isMember("Name") )
		{
			strParams = arrayObj[i]["Name"].asString();
			const char *param = strParams.c_str();
			Recv_AppSetCommand( param );
		}
	}
}

void Recv_SetScanParam( const char* method, Json::Value param )
{
	UINT i, nSize = param.size();
	int nValue = 0;
	if( strstr(method, "spot") != 0 )
	{
		std::string strName;
		for( i=0; i<nSize; i++ )
		{
			if( param[i].isMember("Name") && param[i].isMember("Value") )
			{
				nValue = param[i]["Value"].asInt();
				strName = param[i]["Name"].asString();
				const char *name = strName.c_str();
				if( strcmp(name, "SpotX") == 0 )
					m_ptSpot.x = nValue;
				else if( strcmp(name, "SpotY") == 0 )
					m_ptSpot.y = nValue;
			}
		}
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_Spot, LPARAM(&m_ptSpot));
	}
	else if( strstr(method, "area") != 0 )
	{
		std::string strName;
		for( i=0; i<nSize; i++ )
		{
			if( param[i].isMember("Name") && param[i].isMember("Value") )
			{
				nValue = param[i]["Value"].asInt();
				strName = param[i]["Name"].asString();
				const char *name = strName.c_str();
				if( strcmp(name, "Left") == 0 )
					m_rcScanArea.left	= nValue;
				else if( strcmp(name, "Top") == 0 )
					m_rcScanArea.top	= nValue;
				else if( strcmp(name, "Right") == 0 )
					m_rcScanArea.right	= nValue;
				else if( strcmp(name, "Bottom") == 0 )
					m_rcScanArea.bottom	= nValue;
			}
		}
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_ActiveArea, (LPARAM)(&m_rcScanArea));
	}
}

void Recv_SetScanCommand( const char* method, int nValue )
{
// 	char output[255];
// 	sprintf( output, "Server: post ScanSetCmd %s\n", param );
// 	OutputDebugString( output );
	if( strstr(method, "speed") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_Speed, nValue);
	else if( strstr(method, "reso") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_Reso, nValue);
	else if( strstr(method, "normal") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_Normal, nValue);
	else if( strstr(method, "freeze") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_Freeze, nValue);
	else if( strstr(method, "lineh") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_Line_HORZ, nValue);
	else if( strstr(method, "linev") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_Line_VERT, nValue);
	else if( strstr(method, "sync") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_Sync, nValue);
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
		
		if (root.isMember("id"))
			strID = root["id"].asString();
		
		if (root.isMember("method"))
		{
			strMethod = root["method"].asString();
			const char *method = strMethod.c_str();
			
			if( strcmp(method, "sem.gettype") == 0 )
			{
				if( root["params"].isArray() )
				{
					const Json::Value arrayObj = root["params"];
					Recv_SemGetTypeArray( arrayObj );
				}
				else if( root["params"].isMember("Name") )
				{
					strParams = root["params"]["Name"].asString();
					const char *param = strParams.c_str();
					Recv_SemGetType( param );
				}
			}
			else if( strcmp(method, "sem.getvalue") == 0 )
			{
				if( root["params"].isArray() )
				{
					const Json::Value arrayObj = root["params"];
					Recv_SemGetValueArray( arrayObj );
				}
				else if( root["params"].isMember("Name") )
				{
					strParams = root["params"]["Name"].asString();
					const char *param = strParams.c_str();
					Recv_SemGetValue( param, TRUE );
				}
			}
			else if( strcmp(method, "sem.setvalue") == 0 )
				Recv_SemSetValues( root );
			else if( strcmp(method, "app.execute") == 0 )
			{
				if( root["params"].isArray() )
				{
					const Json::Value arrayObj = root["params"];
					Recv_AppSetCommandArray( arrayObj );
				}
				else if( root["params"].isMember("Name") )
				{
					strParams = root["params"]["Name"].asString();
					const char *param = strParams.c_str();
					Recv_AppSetCommand( param );
				}
			}
			else if( strstr(method, "stage.getvalue") != NULL )
			{
				if( root["params"].isArray() )
				{
					const Json::Value arrayObj = root["params"];
					Recv_StageGetParamArray( arrayObj );
				}
				else if( root["params"].isMember("Name") )
				{
					strParams = root["params"]["Name"].asString();
					const char *param = strParams.c_str();
					Recv_StageGetParam( param, TRUE );
				}
			}
			else if( strstr(method, "stage.setvalue") != NULL )
				Recv_StageSetParam( root, method );
			else if( strstr(method, "stage.cmd") != NULL )
			{
				if( root["params"].isArray() )
				{
					const Json::Value arrayObj = root["params"];
					Recv_StageSetCmdArray( arrayObj );
				}
				else if( root["params"].isMember("Name") )
				{
					strParams = root["params"]["Name"].asString();
					const char *param = strParams.c_str();
					Recv_StageSetCmd( param );
				}
			}
			else if( strstr(method, "scan") != NULL )
			{
				if( root["params"].isArray() )
				{
					const Json::Value arrayObj = root["params"];
					Recv_SetScanParam( method, arrayObj );
				}
				else if( root["params"].isMember("Name") && root["params"].isMember("Value") )
				{
					int nValue = root["params"]["Value"].asInt();
					Recv_SetScanCommand( method, nValue );
				}
				else
					Recv_SetScanCommand( method, 0 );
			}
		}
	}
	
	::delete readerinfo;
	readerinfo = NULL;
}

// 将从客户端接收的命令串，分析出包含单一指令的字符串，放入缓冲区SendHost
DWORD WINAPI ThreadAnalyzeClient(LPVOID lpParam)
{
//	char* sFlag = (char*)(lpParam);
//	CTestSpellmanRSDlg* pDlg = (CTestSpellmanRSDlg*)(lpParam);
	char szBuf[768] = {0};
	char szAnalyze[512] = {0};
	char *exd;
	while (TRUE)
	{
		if( m_bEndThreads )
		{
// 			LeaveCriticalSection(&csFifoHost);
			break;
		}

		EnterCriticalSection(&csFifoRecvClientMsg);
		if( !m_fifoRecvClientMsg.IsFIFOEmpty() )
		{
			KYKYComm_Recv_Package msg;
			m_fifoRecvClientMsg.Output(msg);
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
					KYKYCommJSON_Recv recvClient;
					recvClient.strRecv = szAnalyze;
//					EnterCriticalSection(&csFifoSendToHost);
					m_fifoSendToHost.Input(recvClient);
//					LeaveCriticalSection(&csFifoSendToHost);
				}
				strcpy( msg.szRecv, szBuf );
			} while (strchr( msg.szRecv, '}]') != NULL);
/*			do 
			{
				if( strstr( msg.szRecv, "}]") == NULL )
					break;
				char *exd = strstr(msg.szRecv, "}]");// +2;
				int result = exd -msg.szRecv +1;	// position
				if( exd != NULL )
				{
					if( result +2 < strlen(msg.szRecv) )
					{
						strcpy( szBuf, strstr(msg.szRecv, "}]") +2 );
						*(exd +2) = '\0';
					}
					else
						szBuf[0] = '\0';
				}
				else
					szBuf[0] = '\0';
				if( strchr( msg.szRecv, '{') != NULL )
				{
					strcpy( szAnalyze, strchr( msg.szRecv, '{') );
					exd = strrchr( szAnalyze, ']' );
					if( exd != NULL )
						*exd = '\0';
					KYKYCommJSON_Recv recvClient;
					recvClient.strRecv = szAnalyze;
//					EnterCriticalSection(&csFifoSendToHost);
					m_fifoSendToHost.Input(recvClient);
//					LeaveCriticalSection(&csFifoSendToHost);
				}
				strcpy( msg.szRecv, szBuf );
			} while (strchr( msg.szRecv, '}]') != NULL);*/
//			if( m_nSavedCounts > 0 )	// 拼数据
//				memcpy( data, m_dataSaved, m_nSavedCounts );
//			memcpy( data +m_nSavedCounts, msg.bData, msg.bCtrlMSGLength );
//			AnalyzeData( data, m_nSavedCounts +msg.bCtrlMSGLength );
		}
		LeaveCriticalSection(&csFifoRecvClientMsg);

		Sleep(1);
	}
	while( m_nThreadCounts > 1 )
		Sleep(1);
	m_nThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadAnalyzeClient, counts = %d", m_nThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

// 将包含单一指令的字符串从缓冲区SendHost中一一取出，发送给主程序
DWORD WINAPI ThreadSendToHost(LPVOID lpParam)
{
	while (TRUE)
	{
		if( m_bEndThreads )
		{
// 			LeaveCriticalSection(&csFifoHost);
			break;
		}

		EnterCriticalSection(&csFifoSendToHost);
		if( !m_fifoSendToHost.IsFIFOEmpty() )
		{
			KYKYCommJSON_Recv msg;
			m_fifoSendToHost.Output(msg);
//			LeaveCriticalSection(&csFifoSendToHost);
			Recv_TranslateJson( msg.strRecv );
		}
		LeaveCriticalSection(&csFifoSendToHost);

		Sleep(1);
	}
	m_nThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadSendToHost, counts = %d", m_nThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

void StartThreads(HWND hMain)
{
	g_destHandle = hMain;
	m_bEndThreads = FALSE;
	HANDLE hThread1, hThread2;
//	InitializeCriticalSection(&csSocket);
	InitializeCriticalSection(&csFifoRecvClientMsg);
	InitializeCriticalSection(&csFifoSendToHost);

	hThread1 = CreateThread(NULL, 0, ThreadAnalyzeClient, NULL, 0, NULL);
	m_nThreadCounts++;

	hThread2 = CreateThread(NULL, 0, ThreadSendToHost, NULL, 0, NULL);
	m_nThreadCounts++;
}

BOOL CloseThreads()
{
	ResetEvent(g_hServerSocketThread3);
	m_serverSocket.AsyncSelect(FD_CLOSE);
	m_bEndThreads = TRUE;
//	Sleep(2000);
	while(true)
	{
		if( m_nThreadCounts < 1 )
			break;
	}
//	DeleteCriticalSection(&csSocket);
	DeleteCriticalSection(&csFifoRecvClientMsg);
	DeleteCriticalSection(&csFifoSendToHost);

	SetEvent( g_hServerSocketThread3 );
	WaitForSingleObject( g_hServerSocketThread3, INFINITE );
	return TRUE;
}

BOOL CSerSocketThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	if (!AfxSocketInit())	// 初始化CSocket必须调用的
	{
		return CWinThread::InitInstance();	// 立刻退出
	}
	// 对服务端Socket附加的socket句柄
	if (m_serverSocket.Attach(m_hSocket))	// 重新Attach之前传入的Socket
	{
		m_serverSocket.m_pThrd = this;		// 告诉CSocket它附加工作得线程
//		m_serverSocket.m_thrdIndex = m_thrdIndex;
		m_serverSocket.AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);

		return TRUE;	// 这样线程就不会立刻退出
	}
	return CWinThread::InitInstance();		// 立刻退出
}

int CSerSocketThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	CloseThreads();
//	WaitForSingleObject(g_hServerSocketThread3, INFINITE);
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSerSocketThread, CWinThread)
	//{{AFX_MSG_MAP(CSerSocketThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_Thread_Read, OnMsgRead)
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_Thread_Send, OnMsgSend)
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_Thread_Send_Img, OnMsgSendImg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerSocketThread message handlers

void CSerSocketThread::OnMsgRead(WPARAM wParam,LPARAM lParam)
{
	// 接收数据
	// 先把接收数据事件关掉
	// 关闭接收数据，避免后续接收数据不停传来，造成处理堵塞，这样读写一次完成
// 	ResetEvent(g_hServerSocketThread3);
// 	m_serverSocket.AsyncSelect(FD_CLOSE);

	KYKYComm_Recv_Package msg;
//	EnterCriticalSection(&csSocket);
	int nRecv = m_serverSocket.Receive(msg.szRecv, 512);
	msg.szRecv[nRecv] = '\0';
//	DeleteCriticalSection(&csSocket);
	if( nRecv > 0 )
	{
//		EnterCriticalSection(&csFifoRecvClientMsg);
		m_fifoRecvClientMsg.Input(msg);
//		DeleteCriticalSection(&csFifoRecvClientMsg);
	}
	char output[1024] = {0};
	sprintf( output, "Server: MsgRead = %s", msg.szRecv );
	OutputDebugString(output);
/*
	char szBufRecv[768], szBuf[768], szAnalyze[512], output[1024];
	int nRecv = m_serverSocket.Receive(szBufRecv, 512);
	sprintf( output, "Server: MsgRead = %s", szBufRecv );
	OutputDebugString(output);
	/////////////////////////////////////////////////////////////////
	// [{"method":"sem.getvalue","params":{"Name":"Mag"}}][{"method":
	// "sem.getvalue","params":{"Name":"wd"}}][{"method":"sem.getvalue",
	// "params":{"Name":"wd"}}][{"method":"sem.getvalue","params":{"Name":
	// "Acc"}}][{"method":"sem.getvalue","params":{"Name":"Acc"}}][{"method":
	// "sem.getvalue","params":{"Name":"ScanRotAngle"}}][{"method":"sem.getvalue",
	// "params":{"Name":"Contrast"}}][{"method":"sem.getvalue","params":{"Name":
	// "Brightness"}}][{"method":"stage.getstatus","params":[]}][{"method":"stage.getpos5","params":[]}]
	/////////////////////////////////////////////////////////////////
	do 
	{
		if( strstr( szBufRecv, "}]") == NULL )
			break;
		char *exd = strstr(szBufRecv, "}]") +2;
		if( exd != NULL )
		{
			strcpy( szBuf, strstr(szBufRecv, "}]") +2 );
			*exd = '\0';
		}
		if( strchr( szBufRecv, '{') != NULL )
		{
			strcpy( szAnalyze, strchr( szBufRecv, '{') );
			exd = strrchr( szAnalyze, ']' );
			if( exd != NULL )
				*exd = '\0';
			std::string strRecv = szAnalyze;
			Recv_TranslateJson( strRecv );
		}
		strcpy( szBufRecv, szBuf );
	} while (strchr( szBufRecv, '}]') != NULL);
*/
// 	m_serverSocket.AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
// 	SetEvent(g_hServerSocketThread3);		
}

void CSerSocketThread::OnMsgSend(WPARAM wParam, LPARAM lParam)
{
	//发送数据
	BYTE bcommand = BYTE(wParam);    //commmand
// 	m_serverSocket.AsyncSelect(FD_CLOSE);
// 	ResetEvent(g_hServerSocketThread3);
	switch (bcommand)
	{
	case	KYKYComm_CMD_Sys_Connect_Success:
	case	KYKYComm_CMD_Sys_Disconnect:
	case	KYKYComm_CMD_Stage_Stop:
	case	KYKYComm_CMD_Stage_GoHome:
	case	KYKYComm_CMD_Func_AutoBC:
		{
			Send_SetCommand( bcommand );
		}
		break;

	case	KYKYComm_CMD_Sys_GetVersion:
		{
			m_MsgVersion.bHead = 0x5a;
			m_MsgVersion.bCommand = bcommand;
			m_MsgVersion.versionNum = WORD(lParam); 
//			nSendlen = m_serverSocket.Send(&m_MsgVersion,sizeof(m_MsgVersion)); 
		}	
		break;
/*		
	case	KYKYComm_CMD_ScanCtrl_Reso:
		{
			m_MsgSendDataS2i.szSize = *(CSize *)(lParam);
			m_MsgSendDataS2i.bHead = 0x5a;
			m_MsgSendDataS2i.bCommand = bcommand;
//			nSendlen = m_serverSocket.Send(&m_MsgSendDataS2i,sizeof(m_MsgSendDataS2i));
		}
		break;
*/
	case	KYKYComm_CMD_Sys_GetSEMType:
	case	KYKYComm_CMD_Sys_GetHighVoltageType:
		{
			Update_IntParam( bcommand, *(int *)(lParam) );
		}
		break;

	case	KYKYComm_CMD_Get_Magnification:
	case	KYKYComm_CMD_Get_WorkDistance:
	case	KYKYComm_CMD_Get_Brightness:
	case	KYKYComm_CMD_Get_Brightness2:
	case	KYKYComm_CMD_Get_Contrast:
	case	KYKYComm_CMD_Get_HighVoltage:
	case	KYKYComm_CMD_Get_Fila:
	case	KYKYComm_CMD_Get_Bias:
	case	KYKYComm_CMD_Get_BeamCurrent:
	case	KYKYComm_CMD_Get_EMI:
		{
			Update_DoubleParam( bcommand, *(double *)(lParam) );
		}
		break;

	case	KYKYComm_CMD_Get_ScanRotAngleInit:
	case	KYKYComm_CMD_Get_ScanRotAngle:
		{
			Update_IntParam( bcommand, *(int *)(lParam) );
		}
		break;

	case	KYKYComm_CMD_Get_VacStatus:
	case	KYKYComm_CMD_Get_GunStatus:
	case	KYKYComm_CMD_Get_StigX:
	case	KYKYComm_CMD_Get_StigY:
		{
			Update_IntParam( bcommand, *(int *)(lParam) );
		}
		break;

/*	case	KYKYComm_CMD_Get_Stigmator:
		{
			Update_IntParams( bcommand, *(CSize *)(lParam) );
		}
		break;
*/
		///////////////////////////////////////////////////
		///////////////////////////////////////////////////
		///////////////////////////////////////////////////
	case	KYKYComm_CMD_Stage_GetStatus:
		{
			Update_IntParam( bcommand, *(int *)(lParam) );
		}
		break;

	case	KYKYComm_CMD_Get_MagRange:
		{
			m_2fData = *(KYKYCommMsg_Data2f *)(lParam);
			Update_DoubleParams( bcommand );
		}
		break;
/*	case	KYKYComm_CMD_Stage_GetPosXY:
		{
			m_2fData = *(KYKYCommMsg_Data2f *)(lParam);
			Update_DoubleParams( bcommand );
		}
		break;

	case	KYKYComm_CMD_Stage_GetRange:
		{
			m_2fDataGroup = *(KYKYCommMsg_Data2fGroup *)(lParam);
			Update_DoubleParams( bcommand );
		}
		break;
*/
	case	KYKYComm_CMD_Stage_GetRange:
		{
			m_5fDataGroup = *(KYKYCommMsg_Data5fGroup *)(lParam);
			Update_DoubleParams( bcommand );
		}
		break;
	case	KYKYComm_CMD_Stage_GetPos:
		{
			m_5fData = *(KYKYCommMsg_Data5f *)(lParam);
			Update_DoubleParams( bcommand );
		}
		break;

		///////////////////////////////////////////////////
		///////////////////////////////////////////////////
		///////////////////////////////////////////////////
/*	case	KYKYComm_CMD_Image_Sync:
		{
			m_MsgImgData = *PKYKYCommMsg_ImgData(lParam);
			int nsize = m_MsgImgData.rcDest.Width()*m_MsgImgData.rcDest.Height();
//			nSendlen = m_serverSocket.Send(&m_MsgImgData,16+nsize);
		}
		break;
*/	}

// 	SetEvent(g_hServerSocketThread3);
// 	m_serverSocket.AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
}

void CSerSocketThread::OnMsgSendImg(WPARAM wParam, LPARAM lParam)
{
	// 发送图像数据
	BYTE bcommand = BYTE(wParam);    //commmand
// 	m_serverSocket.AsyncSelect(FD_CLOSE);
// 	ResetEvent(g_hServerSocketThread3);
	switch (bcommand)
	{
	case	KYKYComm_CMD_Image_Sync:
		{
			m_MsgImgData = *PKYKYCommMsg_ImgData(lParam);
			int nsize = m_MsgImgData.rcDest.Width()*m_MsgImgData.rcDest.Height();
			int nSendlen = m_serverSocket.Send(&m_MsgImgData,16+nsize);
		}
		break;
	}
// 	SetEvent(g_hServerSocketThread3);
// 	m_serverSocket.AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
}

void CSerSocketThread::Update_DoubleParam( BYTE bcommand, double dValue )
{
	switch (bcommand)
	{
	case	KYKYComm_CMD_Get_Magnification:
		m_dMag = dValue;
		break;
	case	KYKYComm_CMD_Get_WorkDistance:
		m_dWD = dValue;
		break;
	case	KYKYComm_CMD_Get_Brightness:
		m_dBrt = dValue;
		break;
	case	KYKYComm_CMD_Get_Brightness2:
		m_dBrt2 = dValue;
		break;
	case	KYKYComm_CMD_Get_Contrast:
		m_dCst = dValue;
		break;
	case	KYKYComm_CMD_Get_HighVoltage:
		m_dAcc = dValue /1000.0;
		break;
	case	KYKYComm_CMD_Get_Fila:
		m_dFila = dValue;
		break;
	case	KYKYComm_CMD_Get_Bias:
		m_dBias = dValue;
		break;
	case	KYKYComm_CMD_Get_BeamCurrent:
		m_dEmi = dValue;
		break;
	case	KYKYComm_CMD_Get_EMI:
		m_dEmiMon = dValue;
		break;
	}
}

void CSerSocketThread::Update_DoubleParams( BYTE bcommand )//, KYKYCommMsg_Data5f 5fValue )
{
	switch (bcommand)
	{
	case	KYKYComm_CMD_Get_MagRange:
		m_dMagRange[0]	= m_2fData.fValue1;
		m_dMagRange[1]	= m_2fData.fValue2;
		break;
/*	case	KYKYComm_CMD_Stage_GetRange:
		m_dStageRange[0][0]	= m_5fDataGroup.fValueX[0];
		m_dStageRange[0][1]	= m_5fDataGroup.fValueY[0];
		m_dStageRange[1][0]	= m_5fDataGroup.fValueX[1];
		m_dStageRange[1][1]	= m_5fDataGroup.fValueY[1];
		break;
	case	KYKYComm_CMD_Stage_GetPosXY:
		m_dStagePos[0]	= m_5fData.fValue1;
		m_dStagePos[1]	= m_5fData.fValue2;
		break;
*/	case	KYKYComm_CMD_Stage_GetRange:
		m_dStageRange[0][0]	= m_5fDataGroup.fValueX[0];
		m_dStageRange[0][1]	= m_5fDataGroup.fValueY[0];
		m_dStageRange[0][2]	= m_5fDataGroup.fValueZ[0];
		m_dStageRange[0][3]	= m_5fDataGroup.fValueR[0];
		m_dStageRange[0][4]	= m_5fDataGroup.fValueT[0];
		m_dStageRange[1][0]	= m_5fDataGroup.fValueX[1];
		m_dStageRange[1][1]	= m_5fDataGroup.fValueY[1];
		m_dStageRange[1][2]	= m_5fDataGroup.fValueZ[1];
		m_dStageRange[1][3]	= m_5fDataGroup.fValueR[1];
		m_dStageRange[1][4]	= m_5fDataGroup.fValueT[1];
		break;
	case	KYKYComm_CMD_Stage_GetPos:
		m_dStagePos[0]	= m_5fData.fValue1;
		m_dStagePos[1]	= m_5fData.fValue2;
		m_dStagePos[2]	= m_5fData.fValue3;
		m_dStagePos[3]	= m_5fData.fValue4;
		m_dStagePos[4]	= m_5fData.fValue5;
		break;
	}
}

void CSerSocketThread::Update_IntParam( BYTE bcommand, int nValue )
{
	switch (bcommand)
	{
	case	KYKYComm_CMD_Sys_GetSEMType:
		m_nSemType	= nValue;
		break;
	case	KYKYComm_CMD_Sys_GetHighVoltageType:
		m_nHVType	= nValue;
		break;

	case	KYKYComm_CMD_Get_StigX:
		m_nStig[0]	= nValue;
		break;
	case	KYKYComm_CMD_Get_StigY:
		m_nStig[1]	= nValue;
		break;

	case	KYKYComm_CMD_Get_ScanRotAngleInit:
		m_nScanRotAngleInit	= nValue;
		break;
	case	KYKYComm_CMD_Get_ScanRotAngle:
		m_nScanRotAngle	= nValue;
		break;
	case	KYKYComm_CMD_Get_VacStatus:
		m_nVacStatus	= nValue;
		break;
	case	KYKYComm_CMD_Get_GunStatus:
		m_nGunStatus	= nValue;
		break;

	case	KYKYComm_CMD_Stage_GetStatus:
		m_nStageStatus	= nValue;
		break;
	}
}
/*
void CSerSocketThread::Update_IntParams( BYTE bcommand, CSize szValue )
{
	switch (bcommand)
	{
	case	KYKYComm_CMD_Get_Stigmators:
		m_nStig[0]	= szValue.cx;
		m_nStig[1]	= szValue.cy;
		break;
	}
}
*/
void CSerSocketThread::Send_SetCommand( BYTE bcommand )
{
	Json::Value valueSend, valueParam;
	switch (bcommand)
	{
	case	KYKYComm_CMD_Sys_Connect_Success:
		if( m_nPort == KYKYComm_ListenPort1 )
		{
			StartThreads(m_destHandle);
			valueSend["method"]	= "app.execute";
			valueSend["id"]		= "app.execute";
		}
		else
		{
			valueSend["method"]	= "app.execute.img";
			valueSend["id"]		= "app.execute.img";
		}
		valueParam["Name"]	= "Connect";
		valueParam["Value"]	= true;
		break;
	case	KYKYComm_CMD_Sys_Disconnect:
		valueSend["method"]	= "app.execute";
		valueSend["id"]		= "app.execute";
		valueParam["Name"]	= "Disconnect";
		valueParam["Value"]	= true;
		break;
	case	KYKYComm_CMD_Func_AutoBC:
		valueSend["method"]	= "app.execute";
		valueSend["id"]		= "app.execute";
		valueParam["Name"]	= "AutoBC";
		valueParam["Value"]	= NULL;
		break;
	case	KYKYComm_CMD_Stage_Stop:
		valueSend["method"]	= "stage.cmd";
		valueSend["id"]		= "stage.cmd";
		valueParam["Name"]	= "Stop";
		valueParam["Value"]	= NULL;
		break;
	case	KYKYComm_CMD_Stage_GoHome:
		valueSend["method"]	= "stage.cmd";
		valueSend["id"]		= "stage.cmd";
		valueParam["Name"]	= "GoHome";
		valueParam["Value"]	= NULL;
		break;
	}
	valueSend["result"] = valueParam;
// 	valueSend["id"]="MyId";   
// 	valueSend["jsonrpc"]="2.0";
	Send_JsonString( valueSend );
}
