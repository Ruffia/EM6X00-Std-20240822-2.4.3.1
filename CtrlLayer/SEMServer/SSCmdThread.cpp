// SSCmdThread.cpp : implementation file
//

#include "stdafx.h"
//#include "TCPServer.h"
#include "SSCmdThread.h"
#include "WorkQueue.h"
#include "WorkItemAnalyzeClientMsg.h"
#include "KYKYCommFIFO.h"
#include "MPOData.h"

#include <afxtempl.h>
#include <cfifo.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


HANDLE g_hSSocketThreadCmd = CreateEvent(NULL,TRUE,FALSE,"SSocketThreadCmd");

// 将从客户端接收的命令串，分析出包含单一指令的字符串，放入缓冲区SendHost
DWORD WINAPI ThreadAnalyzeClient(LPVOID lpParam);
// 将包含单一指令的字符串从缓冲区SendHost中一一取出，发送给主程序
DWORD WINAPI ThreadSendToHost(LPVOID lpParam);

CRITICAL_SECTION	csSSocketCmd, csFifoRecvClientMsg, csFifoSendToHost;
static	BOOL m_bEndSScmdThreads = FALSE;
int		m_nSScmdThreadCounts;

/////////////////////////////////////////////////
/////////////////////////////////////////////////

HWND	g_destHandle;		// 主程序句柄
CSSCmd m_SSocketCmd;	// 附加在这个线程上的CSocket

// KYKYCommMsg_ImgFrameData		m_MsgImgData;
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
int		m_nSemType, m_nHVType, m_nScanRotAngleInit;
CRect	m_rcScanArea;
CPoint	m_ptSpot;

/////////////////////////////////////////////////////////////////////////////
// CSSCmdThread

IMPLEMENT_DYNCREATE(CSSCmdThread, CWinThread)

CSSCmdThread::CSSCmdThread()
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

	m_nSemType	= 0;
	m_nHVType	= 0;
	m_nScanRotAngleInit	= 0;

	m_nSScmdThreadCounts= 0;
	m_nPort		= -1;
}

CSSCmdThread::~CSSCmdThread()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		m_SSocketCmd.Close();
		m_hSocket = NULL;
	}
}

///////////////////////////////////////////////////////////
// 1.打开6X00主程序
// 2.JsonTcpTest中设置IP为127.0.0.1，Port为3000
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
//	ZeroMemory( &m_szSend, sizeof(m_szSend) );
	char szSend[1024] = {0};
	int nLen1 = strlen(sendData);
	/////////////////////////////////////////////
	// PE或Oxford需要的JSON格式(不经过SEMClient.dll),
	// JsonTcpTest可以正确识别到
// 	sprintf_s( m_szSend, "%d:%s,", nLen, sendData );
// 	nLen = strlen(m_szSend);
// 	m_szSend[nLen] = '\0';
	sprintf_s( szSend, "%d:%s,", nLen1, sendData );
	int nLen2 = strlen(szSend);
	szSend[nLen2] = '\0';
	/////////////////////////////////////////////
	/////////////////////////////////////////////
	// SEMClient.dll需要的JSON格式，JsonTcpTest无法正确识别
// 	sprintf_s( m_szSend, "%d:[%s", nLen, sendData );
// 	nLen = strlen(m_szSend);
// 	m_szSend[nLen-1] = '\0';
// 	strcat(m_szSend, "],");
// 	nLen = strlen(m_szSend);
// 	m_szSend[nLen] = '\0';
	/////////////////////////////////////////////
	
// 	m_SSocketCmd.AsyncSelect(FD_CLOSE);
// 	ResetEvent(g_hSSocketThreadCmd);
//	EnterCriticalSection(&csSSocketCmd);
	int nLen3 = m_SSocketCmd.Send(szSend, strlen(szSend));	// m_szSend, strlen(m_szSend));
//	LeaveCriticalSection(&csSSocketCmd);
// 	SetEvent(g_hSSocketThreadCmd);
// 	m_SSocketCmd.AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
	
	char output[1024];
	sprintf_s( output, "Server: Send_JsonString [%d] = %s\n", nLen3, szSend );
	OutputDebugString( output );
}

void Send_DoubleParam( BYTE bcommand )
{
	Json::Value valueSend, valueParam, valueArray;
	switch (bcommand)
	{
	case	KYKYComm_CMD_Get_Magnification:
		valueParam["Name"]	= "Mag";
		valueParam["Value"]	= ((int)(CMPOData::Instance().m_dMag *10 +0.5)) /10.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_WorkDistance:
		valueParam["Name"]	= "WD";
		valueParam["Value"]	= ((int)(CMPOData::Instance().m_dWD *1000000 +0.5)) /1000000.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_Brightness:
		valueParam["Name"]	= "Brightness";
		valueParam["Value"]	= CMPOData::Instance().m_dBrt > 0 ? ((int)(CMPOData::Instance().m_dBrt *100 +0.5)) /100.0 : ((int)(CMPOData::Instance().m_dBrt *100 -0.5)) /100.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_Brightness2:
		valueParam["Name"]	= "Brightness2";
		valueParam["Value"]	= CMPOData::Instance().m_dBrt2 > 0 ? ((int)(CMPOData::Instance().m_dBrt2 *100 +0.5)) /100.0 : ((int)(CMPOData::Instance().m_dBrt2 *100 -0.5)) /100.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_Contrast:
		valueParam["Name"]	= "Contrast";
		valueParam["Value"]	= ((int)(CMPOData::Instance().m_dCst *100 +0.5)) /100.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_HighVoltage:
		valueParam["Name"]	= "Acc";
		valueParam["Value"]	= ((int)(CMPOData::Instance().m_dAcc *1000 +0.5)) /1000.0;	// KV
		valueArray.append( valueParam );
		valueSend["id"]		= "hv.getvalue";
		break;
	case	KYKYComm_CMD_Get_Fila:
		valueParam["Name"]	= "Filament";
		valueParam["Value"]	= ((int)(CMPOData::Instance().m_dFila *1000 +0.5)) /1000.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "hv.getvalue";
		break;
	case	KYKYComm_CMD_Get_Bias:
		valueParam["Name"]	= "Bias";
		valueParam["Value"]	= ((int)(CMPOData::Instance().m_dBias *1000 +0.5)) /1000.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "hv.getvalue";
		break;
	case	KYKYComm_CMD_Get_BeamCurrent:
		valueParam["Name"]	= "Emission";
		valueParam["Value"]	= ((int)(CMPOData::Instance().m_dEmi *100 +0.5)) /100.0;
		valueArray.append( valueParam );
		valueSend["id"]		= "hv.getvalue";
		break;
	case	KYKYComm_CMD_Get_EMI:
		valueParam["Name"]	= "EmissionMonitor";
		valueParam["Value"]	= ((int)(CMPOData::Instance().m_dEmiMon *100 +0.5)) /100.0;
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
		valueParam["Min"]	= ((int)(CMPOData::Instance().m_dMagRange[0] *100 +0.5)) /100.0;
		valueParam["Max"]	= ((int)(CMPOData::Instance().m_dMagRange[1] *100 +0.5)) /100.0;
		valueArray.append( valueParam );
		break;
	case	KYKYComm_CMD_Get_FieldOfView:
		valueSend["method"] = "sem.getvalue";
		valueSend["id"]		= "common.getvalue";
		valueParam["Name"]	= "FOV";
		valueParam["X"]		= ((int)(CMPOData::Instance().m_dFOV[0] *100 +0.5)) /100.0;
		valueParam["Y"]		= ((int)(CMPOData::Instance().m_dFOV[1] *100 +0.5)) /100.0;
		valueParam["Unit"]	= ((int)(CMPOData::Instance().m_dFOV[2] *100 +0.5)) /100.0;
		valueArray.append( valueParam );
		break;
	case	KYKYComm_CMD_Stage_GetRange:
		valueSend["method"]	= "stage.getvalue";
		valueSend["id"]		= "stage.getaxes";
		valueParam["Name"]	= "RangeX";
		valueParam["Min"]	= ((int)(CMPOData::Instance().m_dStageRange[1][0] *1000 -0.5)) /1000.0;
		valueParam["Max"]	= ((int)(CMPOData::Instance().m_dStageRange[0][0] *1000 +0.5)) /1000.0;
		valueArray.append( valueParam );
		valueParam["Name"]	= "RangeY";
		valueParam["Min"]	= ((int)(CMPOData::Instance().m_dStageRange[1][1] *1000 -0.5)) /1000.0;
		valueParam["Max"]	= ((int)(CMPOData::Instance().m_dStageRange[0][1] *1000 +0.5)) /1000.0;
		valueArray.append( valueParam );
		valueParam["Name"]	= "RangeZ";
		valueParam["Min"]	= ((int)(CMPOData::Instance().m_dStageRange[1][2] *1000 -0.5)) /1000.0;
		valueParam["Max"]	= ((int)(CMPOData::Instance().m_dStageRange[0][2] *1000 +0.5)) /1000.0;
		valueArray.append( valueParam );
		valueParam["Name"]	= "RangeR";
		valueParam["Min"]	= ((int)(CMPOData::Instance().m_dStageRange[1][3] *100 -0.5)) /100.0;
		valueParam["Max"]	= ((int)(CMPOData::Instance().m_dStageRange[0][3] *100 +0.5)) /100.0;
		valueArray.append( valueParam );
		valueParam["Name"]	= "RangeT";
		valueParam["Min"]	= ((int)(CMPOData::Instance().m_dStageRange[1][4] *100 -0.5)) /100.0;
		valueParam["Max"]	= ((int)(CMPOData::Instance().m_dStageRange[0][4] *100 +0.5)) /100.0;
		valueArray.append( valueParam );
		break;
	case	KYKYComm_CMD_Stage_GetPos:
		valueSend["method"]	= "stage.getvalue";
		valueSend["id"]		= "stage.getpos";
		valueParam["Name"]	= "PosX";
		valueParam["Value"]	= CMPOData::Instance().m_dStagePos[0] > 0 ? ((int)(CMPOData::Instance().m_dStagePos[0] *1000 +0.5)) /1000.0 : ((int)(CMPOData::Instance().m_dStagePos[0] *1000 -0.5)) /1000.0;
		valueArray.append( valueParam );
		valueParam["Name"]	= "PosY";
		valueParam["Value"]	= CMPOData::Instance().m_dStagePos[1] > 0 ? ((int)(CMPOData::Instance().m_dStagePos[1] *1000 +0.5)) /1000.0 : ((int)(CMPOData::Instance().m_dStagePos[1] *1000 -0.5)) /1000.0;
		valueArray.append( valueParam );
		valueParam["Name"]	= "PosZ";
		valueParam["Value"]	= CMPOData::Instance().m_dStagePos[2] > 0 ? ((int)(CMPOData::Instance().m_dStagePos[2] *1000 +0.5)) /1000.0 : ((int)(CMPOData::Instance().m_dStagePos[2] *1000 -0.5)) /1000.0;
		valueArray.append( valueParam );
		valueParam["Name"]	= "PosR";
		valueParam["Value"]	= CMPOData::Instance().m_dStagePos[3] > 0 ? ((int)(CMPOData::Instance().m_dStagePos[3] *100 +0.5)) /100.0 : ((int)(CMPOData::Instance().m_dStagePos[3] *100 -0.5)) /100.0;
		valueArray.append( valueParam );
		valueParam["Name"]	= "PosT";
		valueParam["Value"]	= CMPOData::Instance().m_dStagePos[4] > 0 ? ((int)(CMPOData::Instance().m_dStagePos[4] *100 +0.5)) /100.0 : ((int)(CMPOData::Instance().m_dStagePos[4] *100 -0.5)) /100.0;
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

	case	KYKYComm_CMD_Get_ScanCtrl_Speed:
		valueParam["Name"]	= "Speed";
		valueParam["Value"]	= CMPOData::Instance().m_dSpeed;
		valueArray.append( valueParam );
		valueSend["method"]	= "scan.getvalue";
		valueSend["id"]		= "scan.getvalue";
		break;

	case	KYKYComm_CMD_ScanCtrl_IsStopScan:
		valueParam["Name"]	= "IsStopScan";
		valueParam["Value"]	= CMPOData::Instance().m_nIsStopScan;
		valueArray.append( valueParam );
		valueSend["method"]	= "scan.getvalue";
		valueSend["id"]		= "scan.getvalue";
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
		valueParam["Value"]	= CMPOData::Instance().m_nScanRotAngle;
		valueArray.append( valueParam );
		valueSend["method"]	= "sem.getvalue";
		valueSend["id"]		= "common.getvalue";
		break;

	case	KYKYComm_CMD_Get_StigX:
		valueParam["Name"]	= "StigX";
		valueParam["Value"]	= CMPOData::Instance().m_nStig[0];
		valueArray.append( valueParam );
		valueSend["method"]	= "sem.getvalue";
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_StigY:
		valueParam["Name"]	= "StigY";
		valueParam["Value"]	= CMPOData::Instance().m_nStig[1];
		valueArray.append( valueParam );
		valueSend["method"]	= "sem.getvalue";
		valueSend["id"]		= "common.getvalue";
		break;

	case	KYKYComm_CMD_Get_VacStatus:
		valueParam["Name"]	= "VacStatus";
		valueParam["Value"]	= CMPOData::Instance().m_nVacStatus;
		valueArray.append( valueParam );
		valueSend["method"]	= "sem.getvalue";
		valueSend["id"]		= "status.getvalue";
		break;
	case	KYKYComm_CMD_Get_GunStatus:
		valueParam["Name"]	= "GunStatus";
		valueParam["Value"]	= CMPOData::Instance().m_nGunStatus;
		valueArray.append( valueParam );
		valueSend["method"]	= "sem.getvalue";
		valueSend["id"]		= "status.getvalue";
		break;

	case	KYKYComm_CMD_Stage_GetStatus:
		valueParam["Name"]	= "Status";
		valueParam["Value"]	= CMPOData::Instance().m_nStageStatus;
		valueArray.append( valueParam );
		valueSend["method"]	= "stage.getvalue";
		valueSend["id"]		= "stage.getstatus";
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
	case	KYKYComm_CMD_Get_Stigs:
		valueParam["Name"]	= "StigX";
		valueParam["Value"]	= CMPOData::Instance().m_nStig[0];
		valueArray.append( valueParam );
		valueParam["Name"]	= "StigY";
		valueParam["Value"]	= CMPOData::Instance().m_nStig[1];
		valueArray.append( valueParam );
		valueSend["method"]	= "sem.getvalue";
		valueSend["id"]		= "common.getvalue";
		break;
	case	KYKYComm_CMD_Get_ScanCtrl_Reso:
		valueParam["Name"]	= "ResoX";
		valueParam["Value"]	= CMPOData::Instance().m_nReso[0];
		valueArray.append( valueParam );
		valueParam["Name"]	= "ResoY";
		valueParam["Value"]	= CMPOData::Instance().m_nReso[1];
		valueArray.append( valueParam );
		valueSend["method"]	= "scan.getvalue";
		valueSend["id"]		= "scan.getvalue";
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
			valueParam["Value"]	= ((int)(CMPOData::Instance().m_dMag *10 +0.5)) /10.0;
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_WorkDistance:
			valueParam["Name"]	= "WD";
			valueParam["Value"]	= ((int)(CMPOData::Instance().m_dWD *1000000 +0.5)) /1000000.0;
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_Brightness:
			valueParam["Name"]	= "Brightness";
			valueParam["Value"]	= CMPOData::Instance().m_dBrt > 0 ? ((int)(CMPOData::Instance().m_dBrt *100 +0.5)) /100.0 : ((int)(CMPOData::Instance().m_dBrt *100 -0.5)) /100.0;
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_Brightness2:
			valueParam["Name"]	= "Brightness2";
			valueParam["Value"]	= CMPOData::Instance().m_dBrt2 > 0 ? ((int)(CMPOData::Instance().m_dBrt2 *100 +0.5)) /100.0 : ((int)(CMPOData::Instance().m_dBrt2 *100 -0.5)) /100.0;
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_Contrast:
			valueParam["Name"]	= "Contrast";
			valueParam["Value"]	= ((int)(CMPOData::Instance().m_dCst *100 +0.5)) /100.0;
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_StigX:
			valueParam["Name"]	= "StigX";
			valueParam["Value"]	= CMPOData::Instance().m_nStig[0];
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_StigY:
			valueParam["Name"]	= "StigY";
			valueParam["Value"]	= CMPOData::Instance().m_nStig[1];
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
			valueParam["Value"]	= CMPOData::Instance().m_nScanRotAngle;
			arrayObj.append( valueParam );
			valueSend["id"]		= "common.getvalue";
			break;
		case	KYKYComm_CMD_Get_HighVoltage:
			valueParam["Name"]	= "Acc";
			valueParam["Value"]	= ((int)(CMPOData::Instance().m_dAcc *1000 +0.5)) /1000.0;	// KV
			arrayObj.append( valueParam );
			valueSend["id"]		= "hv.getvalue";
			break;
		case	KYKYComm_CMD_Get_Fila:
			valueParam["Name"]	= "Filament";
			valueParam["Value"]	= ((int)(CMPOData::Instance().m_dFila *1000 +0.5)) /1000.0;
			arrayObj.append( valueParam );
			valueSend["id"]		= "hv.getvalue";
			break;
		case	KYKYComm_CMD_Get_Bias:
			valueParam["Name"]	= "Bias";
			valueParam["Value"]	= ((int)(CMPOData::Instance().m_dBias *1000 +0.5)) /1000.0;
			arrayObj.append( valueParam );
			valueSend["id"]		= "hv.getvalue";
			break;
		case	KYKYComm_CMD_Get_BeamCurrent:
			valueParam["Name"]	= "Emission";
			valueParam["Value"]	= ((int)(CMPOData::Instance().m_dEmi *100 +0.5)) /100.0;
			arrayObj.append( valueParam );
			valueSend["id"]		= "hv.getvalue";
			break;
		case	KYKYComm_CMD_Get_EMI:
			valueParam["Name"]	= "EmissionMonitor";
			valueParam["Value"]	= ((int)(CMPOData::Instance().m_dEmiMon *100 +0.5)) /100.0;
			arrayObj.append( valueParam );
			valueSend["id"]		= "hv.getvalue";
			break;
		case	KYKYComm_CMD_Get_VacStatus:
			valueParam["Name"]	= "VacStatus";
			valueParam["Value"]	= CMPOData::Instance().m_nVacStatus;
			arrayObj.append( valueParam );
			valueSend["id"]		= "status.getvalue";
			break;
		case	KYKYComm_CMD_Get_GunStatus:
			valueParam["Name"]	= "GunStatus";
			valueParam["Value"]	= CMPOData::Instance().m_nGunStatus;
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
// 	sprintf_s( output, "Server: post GetType %s\n", param );
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
// 	sprintf_s( output, "Server: post GetValue %s\n", param );
// 	OutputDebugString( output );
	short nGetCmd = -1;
	if( strcmp(param, "Mag") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_Magnification;
		//PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_Magnification, 0);
		Send_DoubleParam( KYKYComm_CMD_Get_Magnification );
	}
	else if( strcmp(param, "WD") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_WorkDistance;
		//PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_WorkDistance, 0);
		Send_DoubleParam( KYKYComm_CMD_Get_WorkDistance );
	}
	else if( strcmp(param, "Acc") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_HighVoltage;
		//PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_HighVoltage, 0);
		Send_DoubleParam(KYKYComm_CMD_Get_HighVoltage);
	}
	else if( strcmp(param, "VacStatus") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_VacStatus;
		//PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_VacStatus, 0);
		Send_IntParam(KYKYComm_CMD_Get_VacStatus);
	}
	else if( strcmp(param, "GunStatus") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_GunStatus;
		//PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_GunStatus, 0);
		Send_IntParam(KYKYComm_CMD_Get_GunStatus);
	}
	else if( strcmp(param, "ScanRotAngle") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_ScanRotAngle;
		//PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_ScanRotAngle, 0);
		Send_IntParam(KYKYComm_CMD_Get_ScanRotAngle);
	}
	else if( strcmp(param, "Emission") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_BeamCurrent;
		//PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_BeamCurrent, 0);
		Send_DoubleParam(KYKYComm_CMD_Get_BeamCurrent);
	}

	else if( strcmp(param, "MagRange") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_MagRange;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_MagRange, 0);
	}
	else if( strcmp(param, "FOV") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_FieldOfView;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_FieldOfView, 0);
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
	else if( strcmp(param, "ScanRotOffset1") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_ScanRotAngleInit;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_ScanRotAngleInit, 0);
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
	else if( strcmp(param, "EmissionMonitor") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Get_EMI;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_EMI, 0);
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
		else if( strstr(name, "Acc") != NULL )
		{
			// 从客户端得到的是KV，转变为V发送给服务端
			m_MsgSendDataC1f.fValue = (float)(param["Value"].asDouble() *1000.0);
			m_MsgSendDataC1f.bCommand = KYKYComm_CMD_Set_HighVoltage;
			PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_HighVoltage, LPARAM(&m_MsgSendDataC1f) );
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
		//PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Stage_GetStatus, 0);
		Send_IntParam(KYKYComm_CMD_Stage_GetStatus);
	}
	else if( strcmp(param, "Pos") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Stage_GetPos;
		//PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Stage_GetPos, 0);
		Send_DoubleParams(KYKYComm_CMD_Stage_GetPos);
	}
	else if( strcmp(param, "Range") == 0 )
	{
		nGetCmd = KYKYComm_CMD_Stage_GetRange;
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Stage_GetRange, 0);
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
			valueParam["Value"]	= CMPOData::Instance().m_nStageStatus;
			arrayObj.append( valueParam );
			valueSend["id"]		= "stage.getstatus";
			break;
		case	KYKYComm_CMD_Stage_GetRange:
			valueParam["Name"]	= "RangeX";
			valueParam["Min"]	= ((int)(CMPOData::Instance().m_dStageRange[1][0] *1000 -0.5)) /1000.0;
			valueParam["Max"]	= ((int)(CMPOData::Instance().m_dStageRange[0][0] *1000 +0.5)) /1000.0;
			arrayObj.append( valueParam );
			valueParam["Name"]	= "RangeY";
			valueParam["Min"]	= ((int)(CMPOData::Instance().m_dStageRange[1][1] *1000 -0.5)) /1000.0;
			valueParam["Max"]	= ((int)(CMPOData::Instance().m_dStageRange[0][1] *1000 +0.5)) /1000.0;
			arrayObj.append( valueParam );
			valueParam["Name"]	= "RangeZ";
			valueParam["Min"]	= ((int)(CMPOData::Instance().m_dStageRange[1][2] *1000 -0.5)) /1000.0;
			valueParam["Max"]	= ((int)(CMPOData::Instance().m_dStageRange[0][2] *1000 +0.5)) /1000.0;
			arrayObj.append( valueParam );
			valueParam["Name"]	= "RangeR";
			valueParam["Min"]	= ((int)(CMPOData::Instance().m_dStageRange[1][3] *100 -0.5)) /100.0;
			valueParam["Max"]	= ((int)(CMPOData::Instance().m_dStageRange[0][3] *100 +0.5)) /100.0;
			arrayObj.append( valueParam );
			valueParam["Name"]	= "RangeT";
			valueParam["Min"]	= ((int)(CMPOData::Instance().m_dStageRange[1][4] *100 -0.5)) /100.0;
			valueParam["Max"]	= ((int)(CMPOData::Instance().m_dStageRange[0][4] *100 +0.5)) /100.0;
			arrayObj.append( valueParam );
			valueSend["id"]		= "stage.getaxes";
			break;
		case	KYKYComm_CMD_Stage_GetPos:
			valueParam["Name"]	= "PosX";
			valueParam["Value"]	= CMPOData::Instance().m_dStagePos[0] > 0 ? ((int)(CMPOData::Instance().m_dStagePos[0] *1000 +0.5)) /1000.0 : ((int)(CMPOData::Instance().m_dStagePos[0] *1000 -0.5)) /1000.0;
			arrayObj.append( valueParam );
			valueParam["Name"]	= "PosY";
			valueParam["Value"]	= CMPOData::Instance().m_dStagePos[1] > 0 ? ((int)(CMPOData::Instance().m_dStagePos[1] *1000 +0.5)) /1000.0 : ((int)(CMPOData::Instance().m_dStagePos[1] *1000 -0.5)) /1000.0;
			arrayObj.append( valueParam );
			valueParam["Name"]	= "PosZ";
			valueParam["Value"]	= CMPOData::Instance().m_dStagePos[2] > 0 ? ((int)(CMPOData::Instance().m_dStagePos[2] *1000 +0.5)) /1000.0 : ((int)(CMPOData::Instance().m_dStagePos[2] *1000 -0.5)) /1000.0;
			arrayObj.append( valueParam );
			valueParam["Name"]	= "PosR";
			valueParam["Value"]	= CMPOData::Instance().m_dStagePos[3] > 0 ? ((int)(CMPOData::Instance().m_dStagePos[3] *100 +0.5)) /100.0 : ((int)(CMPOData::Instance().m_dStagePos[3] *100 -0.5)) /100.0;
			arrayObj.append( valueParam );
			valueParam["Name"]	= "PosT";
			valueParam["Value"]	= CMPOData::Instance().m_dStagePos[4] > 0 ? ((int)(CMPOData::Instance().m_dStagePos[4] *100 +0.5)) /100.0 : ((int)(CMPOData::Instance().m_dStagePos[4] *100 -0.5)) /100.0;
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
			if( params[i].isMember("Name") && params[i].isMember("Value") )
			{
				float fPos = (float)(params[i]["Value"].asDouble());
				std::string strName = params[i]["Name"].asString();
				const char* name = strName.c_str();
				if( strstr(name, "PosX") != NULL )
					m_MsgSendDataC5f.Values.fValue1 = fPos;
				else if( strstr(name, "PosY") != NULL )
					m_MsgSendDataC5f.Values.fValue2 = fPos;
				else if( strstr(name, "PosZ") != NULL )
					m_MsgSendDataC5f.Values.fValue3 = fPos;
				else if( strstr(name, "PosR") != NULL )
					m_MsgSendDataC5f.Values.fValue4 = fPos;
				else if( strstr(name, "PosT") != NULL )
					m_MsgSendDataC5f.Values.fValue5 = fPos;
			}
		}
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Stage_SetPos5, LPARAM(&m_MsgSendDataC5f) );
	}
	else
	{
		if( root["params"].isMember("Name") && root["params"].isMember("Value") )
		{
			float fPos = (float)(root["params"]["Value"].asDouble());
			std::string strName = root["params"]["Name"].asString();
			const char* name = strName.c_str();
			char cAxis = name[3];
			switch( cAxis )
			{
			case	'X':
				m_MsgSendDataC1f.bCommand	= 0;
				m_MsgSendDataC1f.fValue		= fPos;
				break;
			case	'Y':
				m_MsgSendDataC1f.bCommand	= 1;
				m_MsgSendDataC1f.fValue		= fPos;
				break;
			case	'Z':
				m_MsgSendDataC1f.bCommand	= 2;
				m_MsgSendDataC1f.fValue		= fPos;
				break;
			case	'R':
				m_MsgSendDataC1f.bCommand	= 3;
				m_MsgSendDataC1f.fValue		= fPos;
				break;
			case	'T':
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
// 	sprintf_s( output, "Server: post AppSetCmd %s\n", param );
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
// 	sprintf_s( output, "Server: post ScanSetCmd %s\n", param );
// 	OutputDebugString( output );
	if( strstr(method, "speed") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_ScanCtrl_Speed, nValue);
	else if( strstr(method, "reso") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Set_ScanCtrl_Reso, nValue);
	else if( strstr(method, "normal") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_Normal, nValue);
	else if( strstr(method, "freeze") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_Freeze, nValue);
	else if( strstr(method, "lineh") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_Line_HORZ, nValue);
	else if( strstr(method, "linev") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_Line_VERT, nValue);
	else if( strstr(method, "start.sync") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_Sync, nValue);
	else if( strstr(method, "img.frame") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Image_GetFrameData, nValue);
	else if( strstr(method, "img.sync") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Image_Sync, nValue);
}

void Recv_GetScanParam( const char* method )
{
	if( strstr(method, "speed") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_ScanCtrl_Speed, 0 );
	else if( strstr(method, "reso") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_Get_ScanCtrl_Reso, 0 );
	else if( strstr(method, "isstop") != NULL )
		PostMessage(g_destHandle, WM_USER_KYKYComm_Recv, KYKYComm_CMD_ScanCtrl_IsStopScan, 0 );
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
			
			if( strstr(method, "stage.getvalue") != NULL )
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
			else if( strcmp(method, "hv.setvalue") == 0 )
				Recv_SemSetValues( root );
			else if( strcmp(method, "sem.gettype") == 0 )
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
				if( strstr(method, "set") != NULL )
				{
					if( root["params"].isArray() )
					{
						const Json::Value arrayObj = root["params"];
						Recv_SetScanParam( method, arrayObj );
					}
					else if( root["params"].isMember("Name") && root["params"].isMember("Value") )
					{
						int nValue = root["params"]["Value"].asInt();
						Recv_SetScanCommand( method, nValue );	// start scan, set speed/reso
					}
					else
						Recv_SetScanCommand( method, 0 );		// start scan
				}
				else if( strstr(method, "get") != NULL )		// get speed/reso
					Recv_GetScanParam( method );
				else
					Recv_SetScanCommand( method, 0 );			// Image Sync
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
	while (TRUE)
	{
		if( m_bEndSScmdThreads )
		{
// 			LeaveCriticalSection(&csFifoHost);
			break;
		}

		EnterCriticalSection(&csFifoRecvClientMsg);
		CFIFO<KYKYComm_Recv_Package, KYKYComm_Recv_Package&>& m_fifoRecvClientMsg = CommFIFO::Instance().GetRecvClientMsgFIFO();

		if( !m_fifoRecvClientMsg.IsFIFOEmpty() )
		{
			KYKYComm_Recv_Package msg;
			m_fifoRecvClientMsg.Output(msg);
			WorkItemAnalyzeClientMsg* pItem = new WorkItemAnalyzeClientMsg(msg);
			CWorkQueue::Instance().InsertWorkItem(pItem);
		}
		LeaveCriticalSection(&csFifoRecvClientMsg);

		Sleep(1);
	}
	while( m_nSScmdThreadCounts > 1 )
		Sleep(1);
	m_nSScmdThreadCounts--;
	char cDebug[255];
	sprintf_s( cDebug, "exit ThreadAnalyzeClient, counts = %d", m_nSScmdThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

// 将包含单一指令的字符串从缓冲区SendHost中一一取出，发送给主程序
DWORD WINAPI ThreadSendToHost(LPVOID lpParam)
{
	while (TRUE)
	{
		if( m_bEndSScmdThreads )
		{
// 			LeaveCriticalSection(&csFifoHost);
			break;
		}

		EnterCriticalSection(&csFifoSendToHost);
		CFIFO<KYKYCommJSON_Recv, KYKYCommJSON_Recv&>& m_fifoSendToHost = CommFIFO::Instance().GetSendToHostFIFO();
		if( !m_fifoSendToHost.IsFIFOEmpty() )
		{
			KYKYCommJSON_Recv msg;
			m_fifoSendToHost.Output(msg);

			char sz[2048] = {0};
			sprintf_s(sz,2048,"m_fifoSendToHost.GetFIFOCount() = %d,msg.strRecv = %s",m_fifoSendToHost.GetFIFOCount(),msg.strRecv.c_str());
//			LeaveCriticalSection(&csFifoSendToHost);
			Recv_TranslateJson( msg.strRecv );
		}
		LeaveCriticalSection(&csFifoSendToHost);

		Sleep(1);
	}
	m_nSScmdThreadCounts--;
	char cDebug[255];
	sprintf_s( cDebug, "exit ThreadSendToHost, counts = %d", m_nSScmdThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

void StartThreads(HWND hMain)
{
	g_destHandle = hMain;
	m_bEndSScmdThreads = FALSE;
	HANDLE hThread1, hThread2;
//	InitializeCriticalSection(&csSSocketCmd);
	InitializeCriticalSection(&csFifoRecvClientMsg);
	InitializeCriticalSection(&csFifoSendToHost);

	CWorkQueue::Instance().Create(5);

	hThread1 = CreateThread(NULL, 0, ThreadAnalyzeClient, NULL, 0, NULL);
	m_nSScmdThreadCounts++;

	hThread2 = CreateThread(NULL, 0, ThreadSendToHost, NULL, 0, NULL);
	m_nSScmdThreadCounts++;
}

BOOL CloseThreads()
{
	ResetEvent(g_hSSocketThreadCmd);
	m_SSocketCmd.AsyncSelect(FD_CLOSE);
	m_bEndSScmdThreads = TRUE;
//	Sleep(2000);
	while(true)
	{
		if( m_nSScmdThreadCounts < 1 )
			break;
	}
//	DeleteCriticalSection(&csSSocketCmd);
	DeleteCriticalSection(&csFifoRecvClientMsg);
	DeleteCriticalSection(&csFifoSendToHost);

	SetEvent( g_hSSocketThreadCmd );
	WaitForSingleObject( g_hSSocketThreadCmd, INFINITE );
	CWorkQueue::Instance().Destroy();
	return TRUE;
}
/*
void Init()
{
	g_nCommTimerMode = 0;
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	char szBuf[15];
	if(GetPrivateProfileString("Remote", "TimerMode", "0", szBuf, 15, path) != 0)
		g_nCommTimerMode = atoi(szBuf);
}
*/
BOOL CSSCmdThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	if (!AfxSocketInit())	// 初始化CSocket必须调用的
	{
		return CWinThread::InitInstance();	// 立刻退出
	}
	// 对服务端Socket附加的socket句柄
	if (m_SSocketCmd.Attach(m_hSocket))	// 重新Attach之前传入的Socket
	{
		m_SSocketCmd.m_pThrd = this;		// 告诉CSocket它附加工作得线程
//		m_SSocketCmd.m_thrdIndex = m_thrdIndex;
		m_SSocketCmd.AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
		//Init();
		
		return TRUE;	// 这样线程就不会立刻退出
	}
	return CWinThread::InitInstance();		// 立刻退出
}

int CSSCmdThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	CloseThreads();
//	WaitForSingleObject(g_hSSocketThreadCmd, INFINITE);
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSSCmdThread, CWinThread)
	//{{AFX_MSG_MAP(CSSCmdThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_Thread_Read, OnMsgRead)
	ON_THREAD_MESSAGE(WM_USER_KYKYComm_Thread_Send, OnMsgSend)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSCmdThread message handlers

void CSSCmdThread::OnMsgRead(WPARAM wParam,LPARAM lParam)
{
	// 接收数据
	// 先把接收数据事件关掉
	// 关闭接收数据，避免后续接收数据不停传来，造成处理堵塞，这样读写一次完成
// 	ResetEvent(g_hSSocketThreadCmd);
// 	m_SSocketCmd.AsyncSelect(FD_CLOSE);

	KYKYComm_Recv_Package msg;
//	EnterCriticalSection(&csSSocketCmd);
	int nRecv = m_SSocketCmd.Receive(msg.szRecv, 512);
	msg.szRecv[nRecv] = '\0';
//	DeleteCriticalSection(&csSSocketCmd);
	if( nRecv > 0 )
	{
//		EnterCriticalSection(&csFifoRecvClientMsg);
		CFIFO<KYKYComm_Recv_Package, KYKYComm_Recv_Package&>& m_fifoRecvClientMsg = CommFIFO::Instance().GetRecvClientMsgFIFO();
		m_fifoRecvClientMsg.Input(msg);
//		DeleteCriticalSection(&csFifoRecvClientMsg);
	}
	char output[1024] = {0};
	sprintf_s( output, "Server: MsgRead = %s, %ld", msg.szRecv, GetTickCount() );
	OutputDebugString(output);
/*
	char szBufRecv[768], szBuf[768], szAnalyze[512], output[1024];
	int nRecv = m_SSocketCmd.Receive(szBufRecv, 512);
	sprintf_s( output, "Server: MsgRead = %s", szBufRecv );
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
			strcpy_s( szBuf, strstr(szBufRecv, "}]") +2 );
			*exd = '\0';
		}
		if( strchr( szBufRecv, '{') != NULL )
		{
			strcpy_s( szAnalyze, strchr( szBufRecv, '{') );
			exd = strrchr( szAnalyze, ']' );
			if( exd != NULL )
				*exd = '\0';
			std::string strRecv = szAnalyze;
			Recv_TranslateJson( strRecv );
		}
		strcpy_s( szBufRecv, szBuf );
	} while (strchr( szBufRecv, '}]') != NULL);
*/
// 	m_SSocketCmd.AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
// 	SetEvent(g_hSSocketThreadCmd);		
}

void CSSCmdThread::OnMsgSend(WPARAM wParam, LPARAM lParam)
{
	//发送数据
	BYTE bcommand = BYTE(wParam);    //commmand
// 	m_SSocketCmd.AsyncSelect(FD_CLOSE);
// 	ResetEvent(g_hSSocketThreadCmd);
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
//			nSendlen = m_SSocketCmd.Send(&m_MsgVersion,sizeof(m_MsgVersion)); 
		}	
		break;
/*		
	case	KYKYComm_CMD_ScanCtrl_Reso:
		{
			m_MsgSendDataS2i.szSize = *(CSize *)(lParam);
			m_MsgSendDataS2i.bHead = 0x5a;
			m_MsgSendDataS2i.bCommand = bcommand;
//			nSendlen = m_SSocketCmd.Send(&m_MsgSendDataS2i,sizeof(m_MsgSendDataS2i));
		}
		break;
*/
	case	KYKYComm_CMD_Sys_GetSEMType:
	case	KYKYComm_CMD_Sys_GetHighVoltageType:
		{
			Update_IntParam( bcommand, *(int *)(lParam) );
		}
		break;

	case	KYKYComm_CMD_Get_ScanCtrl_Reso:
		{
			CSize szReso = *(CSize *)(lParam);
			CMPOData::Instance().m_nReso[0] = szReso.cx;
			CMPOData::Instance().m_nReso[1] = szReso.cy;
			Send_IntParams( KYKYComm_CMD_Get_ScanCtrl_Reso );
		}
		break;
	case	KYKYComm_CMD_Get_ScanCtrl_Speed:
		Update_IntParam( bcommand, *(unsigned short *)(lParam) );
		break;
	case	KYKYComm_CMD_ScanCtrl_IsStopScan:
		Update_IntParam( bcommand, *(int *)(lParam) );
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
	case	KYKYComm_CMD_Get_FieldOfView:
		{
			CMPOData::Instance().m_5fData = *(KYKYCommMsg_Data5f *)(lParam);
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
			CMPOData::Instance().m_5fData = *(KYKYCommMsg_Data5f *)(lParam);
			Update_DoubleParams( bcommand );
		}
		break;

		///////////////////////////////////////////////////
		///////////////////////////////////////////////////
		///////////////////////////////////////////////////
/*	case	KYKYComm_CMD_Image_Sync:
		{
			m_MsgImgData = *PKYKYCommMsg_ImgFrameData(lParam);
			int nsize = m_MsgImgData.rcDest.Width()*m_MsgImgData.rcDest.Height();
//			nSendlen = m_SSocketCmd.Send(&m_MsgImgData,16+nsize);
		}
		break;
*/	}

// 	SetEvent(g_hSSocketThreadCmd);
// 	m_SSocketCmd.AsyncSelect(FD_WRITE|FD_READ|FD_CLOSE);
}

void CSSCmdThread::Update_DoubleParam( BYTE bcommand, double dValue )
{
	switch (bcommand)
	{
	case	KYKYComm_CMD_Get_Magnification:
		CMPOData::Instance().m_dMag = dValue;
		break;
	case	KYKYComm_CMD_Get_WorkDistance:
		CMPOData::Instance().m_dWD = dValue;
		break;
	case	KYKYComm_CMD_Get_Brightness:
		CMPOData::Instance().m_dBrt = dValue;
		break;
	case	KYKYComm_CMD_Get_Brightness2:
		CMPOData::Instance().m_dBrt2 = dValue;
		break;
	case	KYKYComm_CMD_Get_Contrast:
		CMPOData::Instance().m_dCst = dValue;
		break;
	case	KYKYComm_CMD_Get_HighVoltage:
		CMPOData::Instance().m_dAcc = dValue /1000.0;	// 从服务端得到是V，转变为KV发给客户端
		break;
	case	KYKYComm_CMD_Get_Fila:
		CMPOData::Instance().m_dFila = dValue;
		break;
	case	KYKYComm_CMD_Get_Bias:
		CMPOData::Instance().m_dBias = dValue;
		break;
	case	KYKYComm_CMD_Get_BeamCurrent:
		CMPOData::Instance().m_dEmi = dValue;
		break;
	case	KYKYComm_CMD_Get_EMI:
		CMPOData::Instance().m_dEmiMon = dValue;
		break;
	}
	Send_DoubleParam( bcommand );
}

void CSSCmdThread::Update_DoubleParams( BYTE bcommand )//, KYKYCommMsg_Data5f 5fValue )
{
	switch (bcommand)
	{
	case	KYKYComm_CMD_Get_MagRange:
		CMPOData::Instance().m_dMagRange[0]	= m_2fData.fValue1;
		CMPOData::Instance().m_dMagRange[1]	= m_2fData.fValue2;
		break;
	case	KYKYComm_CMD_Get_FieldOfView:
		CMPOData::Instance().m_dFOV[0]		= m_5fData.fValue1;
		CMPOData::Instance().m_dFOV[1]		= m_5fData.fValue2;
		CMPOData::Instance().m_dFOV[2]		= m_5fData.fValue3;
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
		CMPOData::Instance().m_dStageRange[0][0]	= m_5fDataGroup.fValueX[0];
		CMPOData::Instance().m_dStageRange[0][1]	= m_5fDataGroup.fValueY[0];
		CMPOData::Instance().m_dStageRange[0][2]	= m_5fDataGroup.fValueZ[0];
		CMPOData::Instance().m_dStageRange[0][3]	= m_5fDataGroup.fValueR[0];
		CMPOData::Instance().m_dStageRange[0][4]	= m_5fDataGroup.fValueT[0];
		CMPOData::Instance().m_dStageRange[1][0]	= m_5fDataGroup.fValueX[1];
		CMPOData::Instance().m_dStageRange[1][1]	= m_5fDataGroup.fValueY[1];
		CMPOData::Instance().m_dStageRange[1][2]	= m_5fDataGroup.fValueZ[1];
		CMPOData::Instance().m_dStageRange[1][3]	= m_5fDataGroup.fValueR[1];
		CMPOData::Instance().m_dStageRange[1][4]	= m_5fDataGroup.fValueT[1];
		break;
	case	KYKYComm_CMD_Stage_GetPos:
		CMPOData::Instance().m_dStagePos[0]	= m_5fData.fValue1;
		CMPOData::Instance().m_dStagePos[1]	= m_5fData.fValue2;
		CMPOData::Instance().m_dStagePos[2]	= m_5fData.fValue3;
		CMPOData::Instance().m_dStagePos[3]	= m_5fData.fValue4;
		CMPOData::Instance().m_dStagePos[4]	= m_5fData.fValue5;
		break;
	}
	Send_DoubleParams( bcommand );
}

void CSSCmdThread::Update_IntParam( BYTE bcommand, int nValue )
{
	switch (bcommand)
	{
	case	KYKYComm_CMD_Sys_GetSEMType:
		m_nSemType	= nValue;
		break;
	case	KYKYComm_CMD_Sys_GetHighVoltageType:
		m_nHVType	= nValue;
		break;

	case	KYKYComm_CMD_Get_ScanCtrl_Speed:
		CMPOData::Instance().m_dSpeed	= 1.0 *nValue /2;
		break;
	case	KYKYComm_CMD_ScanCtrl_IsStopScan:
		CMPOData::Instance().m_nIsStopScan = nValue;
		break;

	case	KYKYComm_CMD_Get_StigX:
		CMPOData::Instance().m_nStig[0]	= nValue;
		break;
	case	KYKYComm_CMD_Get_StigY:
		CMPOData::Instance().m_nStig[1]	= nValue;
		break;

	case	KYKYComm_CMD_Get_ScanRotAngleInit:
		m_nScanRotAngleInit	= nValue;
		break;
	case	KYKYComm_CMD_Get_ScanRotAngle:
		CMPOData::Instance().m_nScanRotAngle	= nValue;
		break;
	case	KYKYComm_CMD_Get_VacStatus:
		CMPOData::Instance().m_nVacStatus	= nValue;
		break;
	case	KYKYComm_CMD_Get_GunStatus:
		CMPOData::Instance().m_nGunStatus	= nValue;
		break;

	case	KYKYComm_CMD_Stage_GetStatus:
		CMPOData::Instance().m_nStageStatus	= nValue;
		break;
	}
	Send_IntParam( bcommand );
}
/*
void CSSCmdThread::Update_IntParams( BYTE bcommand, CSize szValue )
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
void CSSCmdThread::Send_SetCommand( BYTE bcommand )
{
	Json::Value valueSend, valueParam;
	switch (bcommand)
	{
	case	KYKYComm_CMD_Sys_Connect_Success:
		StartThreads(m_destHandle);
		valueSend["method"]	= "app.execute";
		valueSend["id"]		= "app.execute";
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
