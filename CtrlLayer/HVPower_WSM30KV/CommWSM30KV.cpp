// CommWSM30KV.cpp : implementation file
//

#include "stdafx.h"
#include "CommWSM30KV.h"

#include "../GlobalComponent/Serial.h"

#include <PMSKV.h>
#include <afxmt.h>
#include <afxtempl.h>
#include <cfifo.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	nWSM30KV_Sleep			100

DWORD WINAPI WSM30KVThreadWR(LPVOID lpParam);		// 发送及接收线程
DWORD WINAPI WSM30KVThreadSend(LPVOID lpParam);		// 发送线程
DWORD WINAPI WSM30KVThreadRead(LPVOID lpParam);		// 接收线程
DWORD WINAPI WSM30KVThreadInquire(LPVOID lpParam);	// 查询线程
DWORD WINAPI WSM30KVThreadAnalyze(LPVOID lpParam);	// 分析线程（将数据包分析为数字量）
DWORD WINAPI WSM30KVThreadConvert(LPVOID lpParam);	// 转换线程（将数字量转换为能在界面上显示的量）
DWORD WINAPI WSM30KVThreadLog(LPVOID lpParam);		// 日志线程
const	short	g_nWSM30KVThreadCountsAll = 5;		// 读写、查询、分析、转换、日志

static BOOL g_bWSM30KVEndThreads = FALSE;
CRITICAL_SECTION	csWSM30KVPort, csWSM30KVFifoHost, csWSM30KVFifoDev;

const	short	WSM30KV_HostFIFOSize	= 20;
const	short	WSM30KV_DevFIFOSize		= 20;
CFIFO<WSM30KV_Send_Msg, WSM30KV_Send_Msg&>	g_WSM30KVfifoHostMsg(WSM30KV_HostFIFOSize);
CFIFO<WSM30KV_Recv_Msg, WSM30KV_Recv_Msg&>	g_WSM30KVfifoDevMsg(WSM30KV_DevFIFOSize);

HANDLE	g_hWSM30KVThread;
DWORD	g_dwWSM30KVThread;
DWORD	WINAPI WSM30KVCommThread(LPVOID lpParam);
HANDLE	g_hWSM30KVEventWork		= CreateEvent(NULL,FALSE, FALSE, _T("WORKwsm30kv"));
HANDLE	g_hWSM30KVEventTeminate	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEwsm30kv"));

HWND	g_hWSM30KVMain;
CSerial	g_WSM30KV;
char	g_cWSM30KVSaved[512];
short	g_nWSM30KVSavedCounts, g_nWSM30KVThreadCounts;

Cfg_WSM30KV		g_WSM30KVcfg;
Status_WSM30KV	g_WSM30KVstatus;
Mon_WSM30KV		g_WSM30KVmon;
Setting_WSM30KV	g_WSM30KVset;
WORD	g_wWSM30KVReg01[nWSM30KVCount_Registers], g_wWSM30KVReg25[nWSM30KVCount_Registers];

HANDLE	g_hWSM30KVFile	= NULL;
int		g_nWSM30KVLoops;

/////////////////////////////
float	g_fWSM30KVAutoStep_ACL1		= (float)500.0;		// 0.5;	// 500 V;
float	g_fWSM30KVAutoStep_ACL2		= (float)1000.0;	// 1.0;	// 1000 V;
float	g_fWSM30KVAutoRated_ACL1	= (float)10000.0;	// 10.0;// 10000 V;
float	g_fWSM30KVAutoRated_ACL2	= (float)30000.0;	// 30.0;// 30000 V;
float	g_fWSM30KVAutoStep_EMS		= (float)10;
float	g_fWSM30KVAutoRated_EMS		= (float)30;	// 100;
float	g_fWSM30KVAutoStep_FIL		= (float)0.1;	// 0.001;// 100mA
float	g_fWSM30KVAutoRated_FIL		= (float)1.9;	// 0.003;// 1.9A
WSM30KV_AutoState	g_WSM30KVAutoState = WSM30KVAS_NULL;
DWORD	g_WSM30KVlastAuto;
/////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CCommWSM30KV

CCommWSM30KV::CCommWSM30KV()
{
	g_hWSM30KVMain = NULL;
	ZeroMemory( &g_WSM30KVcfg, sizeof(Cfg_WSM30KV) );
	ZeroMemory( &g_WSM30KVmon, sizeof(Mon_WSM30KV) );
	ZeroMemory( &g_WSM30KVset, sizeof(Setting_WSM30KV) );
	g_WSM30KVstatus.statusINT01.wStatus	= 0xFFFF;
	g_WSM30KVstatus.statusINT25.wStatus	= 0xFFFF;
	g_WSM30KVstatus.statusO01.wStatus	= 0xFFFF;
	g_WSM30KVstatus.statusO25.wStatus	= 0xFFFF;
	g_WSM30KVstatus.statusB01.wStatus	= 0xFFFF;
	g_WSM30KVstatus.statusB25.wStatus	= 0xFFFF;
	g_WSM30KVstatus.statusV01.wStatus	= 0xFFFF;
	g_WSM30KVstatus.statusV25.wStatus	= 0xFFFF;
	g_WSM30KVstatus.statusI01.wStatus	= 0xFFFF;
	g_WSM30KVstatus.statusI25.wStatus	= 0xFFFF;
	g_WSM30KVstatus.statusA25.wStatus	= 0xFFFF;
	g_WSM30KVstatus.status101.wStatus	= 0xFFFF;
	g_WSM30KVstatus.status125.wStatus	= 0xFFFF;
	g_WSM30KVstatus.status201.wStatus	= 0xFFFF;
	g_WSM30KVstatus.status225.wStatus	= 0xFFFF;
	g_WSM30KVstatus.status301.wStatus	= 0xFFFF;
	g_WSM30KVstatus.status325.wStatus	= 0xFFFF;
}

CCommWSM30KV::~CCommWSM30KV()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCommWSM30KV message handlers

void CreateLog()
{
	if( g_hWSM30KVFile != NULL )
	{
		CloseHandle( g_hWSM30KVFile );
		g_hWSM30KVFile = NULL;
	}

	// 设置日志文件
	unsigned long cbRet;
	char Buf[1024] = {0};
	TCHAR exeFullPath[255];
	::GetModuleFileName( NULL, exeFullPath, 255 );
	char *ptemp = strrchr( exeFullPath, '\\');
	ptemp[1] = 0;
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
	sprintf( Buf, "%s\\Log\\WSM30KV%s.csv", exeFullPath, csCurrentTime );
	g_hWSM30KVFile = CreateFile(Buf, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	g_nWSM30KVLoops = 1;

	sprintf( Buf, "%s\r\n", "WSM30KV" );
	WriteFile( g_hWSM30KVFile, Buf, strlen(Buf), &cbRet, NULL );
	sprintf( Buf, "DateTime,%s%s%s%s%s%s%s%s%s%s%s%s%s%s\r\n",
		"sINT01,sINT25,", "sO01,sO25,", "sPB01,sPB25,", "sPV01,sPV25,", "sPI01,sPI25,", "sPA,", "sI101,sI125,sI201,sI225,sI301,sI325,",
		"SCIs,SCIo,SCIv,SCIi,", "COLs,COLo,COLv,COLi,", "PMTs,PMTo,PMTv,PMTi,",
		"ACLVs,ACLVo,ACLVv,", "ACLIs,ACLIo,ACLIi,","FILs,FILo,FILv,FILi,",
		"+3V31,+3V32,+5V1,+5V2,+15V1,+15V2,-15V1,-15V2,+24V1,+24V2,T1,T2");
	WriteFile( g_hWSM30KVFile, Buf, strlen(Buf), &cbRet, NULL );
}

void Record( LPCTSTR szRcd )
{
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%Y%m%d %H:%M:%S");
	
	unsigned long cbRet;
	char Buf[1024];
	sprintf(Buf, "[%s],%s\r\n", csCurrentTime, szRcd );
	WriteFile(g_hWSM30KVFile, Buf, strlen(Buf), &cbRet, NULL);
	
	g_nWSM30KVLoops++;
	if( g_nWSM30KVLoops > 10000 )
		CreateLog();
}

// 纵向冗余校验(LRC)的计算，对报文中的所有的连续8位字节相加，忽略任何进位，然后求出其二进制补码。
// LRC为一个8位域，那么每个会导致值大于255新的相加只是简单的将域的值在零“回绕”。因为没有第9位，进位被自动放弃。
// 生成一个LRC的过程为：
// 1. 不包括起始冒号和结束CRLF的报文中的所有字节相加到一个8位域，故此进位被丢弃
// 2. 从FF（全1）十六进制中减去域的最终值，产生1的补码（二进制反码）
// 3. 加1产生二进制补码

// 指ASCII字符转换为数值之后的字节相加取反加1：
// 例如：ASCII帧为：":01030000000AF2"（此为ModbusPoll软件生成的ASCII帧）
// LRC = 0x100 - ( 0x01 +0x03 +0x00 +0x00 +0x00 + 0x0A )
//		= 0x100 - 0x0E
//		= 0xF2
// 当8位LRC（2个ASCII字符）在报文中传送时，高位字符首先发送，然后是低位字符

unsigned char LRC( unsigned char *auchMsg, unsigned short usDataLen )
{
	unsigned char uchLRC = 0;
	while( usDataLen-- )
		uchLRC += *auchMsg++;
	return ((unsigned char)(-((char)uchLRC)));
}

BOOL VerifyLRC( char* data, BYTE *bData, unsigned short &usDataLen )
{
	// 入参data，已去掉起始符冒号及结束符回车换行
	// 校验反馈数据的验证码LRC
	// strncpy 没有自动加上终止符的，需要手动加上不然会出问题
	char cData[512] = {0}, cLRC[3] = {0};
	BYTE bLRC = 0x00;

	// 反馈帧中除校验码以外的数据
	int nLen = strlen(data);
	strncpy( cData, data, nLen -2 );
	cData[nLen -2] = '\0';

	// 反馈帧中的校验码
	strncpy( cLRC, data + nLen -2, 2 );
	cLRC[2] = '\0';

	//bLRC = cLRC[0]<<4 + cLRC[1];
	//sscanf( cLRC, "%02X", &(bLRC[0]) );

	// 将数据由ASCII字符转换为十六进制数
	usDataLen = strlen( cData ) /2;
	char cByte[3] = {0};
	for( unsigned short i=0; i<usDataLen; i++ )
	{
		strncpy( cByte, cData +i*2, 2 );
//		strncpy( cByte, cData +strlen(cData) -2*(i+1), 2 );
		cByte[2] = '\0';
		sscanf( cByte, "%02X", &(bData[i]) );
	}
	// 计算校验码
	BYTE uLRC = LRC( bData, usDataLen );
	char szULRC[16] = {0};
	sprintf_s(szULRC,16,"%02X",uLRC);
	return strcmp(szULRC,cLRC) == 0;
}

void AnalyzeData_ReadReg( BYTE* bBuf )
{
	if( bBuf[0] == byteWSM30KV_Sn01 )
	{
		// power#01通道
		if( bBuf[2] == nWSM30KVCount_Registers *2 )
		{
			for( int i=0; i<nWSM30KVCount_Registers; i++ )
				g_wWSM30KVReg01[i] = ( bBuf[i*2 +3] << 8 ) | bBuf[i*2 +4];
			if( g_wWSM30KVReg01[AddrWSM30KVR01_INTStatus -byteWSM30KV_Offset] != g_WSM30KVstatus.statusINT01.wStatus )
			{
				g_WSM30KVstatus.statusINT01.wStatus = g_wWSM30KVReg01[AddrWSM30KVR01_INTStatus -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR01_INTStatus, (LPARAM)(&(g_WSM30KVstatus.statusINT01)) );
			}
			if( g_wWSM30KVReg01[AddrWSM30KVR01_OutputStatus1 -byteWSM30KV_Offset] != g_WSM30KVstatus.statusO01.wStatus )
			{
				g_WSM30KVstatus.statusO01.wStatus = g_wWSM30KVReg01[AddrWSM30KVR01_OutputStatus1 -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR01_OutputStatus1, (LPARAM)(&(g_WSM30KVstatus.statusO01)) );
			}
			if( g_wWSM30KVReg01[AddrWSM30KVR01_ProtectStatusB -byteWSM30KV_Offset] != g_WSM30KVstatus.statusB01.wStatus )
			{
				g_WSM30KVstatus.statusB01.wStatus = g_wWSM30KVReg01[AddrWSM30KVR01_ProtectStatusB -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR01_ProtectStatusB, (LPARAM)(&(g_WSM30KVstatus.statusB01)) );
			}
			if( g_wWSM30KVReg01[AddrWSM30KVR01_ProtectStatusV -byteWSM30KV_Offset] != g_WSM30KVstatus.statusV01.wStatus )
			{
				g_WSM30KVstatus.statusV01.wStatus = g_wWSM30KVReg01[AddrWSM30KVR01_ProtectStatusV -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR01_ProtectStatusV, (LPARAM)(&(g_WSM30KVstatus.statusV01)) );
			}
			if( g_wWSM30KVReg01[AddrWSM30KVR01_ProtectStatusI -byteWSM30KV_Offset] != g_WSM30KVstatus.statusI01.wStatus )
			{
				g_WSM30KVstatus.statusI01.wStatus = g_wWSM30KVReg01[AddrWSM30KVR01_ProtectStatusI -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR01_ProtectStatusI, (LPARAM)(&(g_WSM30KVstatus.statusI01)) );
			}
			if( g_wWSM30KVReg01[AddrWSM30KVR01_InquireStatus1 -byteWSM30KV_Offset] != g_WSM30KVstatus.status101.wStatus )
			{
				g_WSM30KVstatus.status101.wStatus = g_wWSM30KVReg01[AddrWSM30KVR01_InquireStatus1 -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR01_InquireStatus1, (LPARAM)(&(g_WSM30KVstatus.status101)) );
			}
			if( g_wWSM30KVReg01[AddrWSM30KVR01_InquireStatus2 -byteWSM30KV_Offset] != g_WSM30KVstatus.status201.wStatus )
			{
				g_WSM30KVstatus.status201.wStatus = g_wWSM30KVReg01[AddrWSM30KVR01_InquireStatus2 -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR01_InquireStatus2, (LPARAM)(&(g_WSM30KVstatus.status201)) );
			}
			if( g_wWSM30KVReg01[AddrWSM30KVR01_InquireStatus3 -byteWSM30KV_Offset] != g_WSM30KVstatus.status301.wStatus )
			{
				g_WSM30KVstatus.status301.wStatus = g_wWSM30KVReg01[AddrWSM30KVR01_InquireStatus3 -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR01_InquireStatus3, (LPARAM)(&(g_WSM30KVstatus.status301)) );
			}
			g_WSM30KVmon.monVT.fP3V3[0]	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_FB_P3V3 -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *3.3);
			g_WSM30KVmon.monVT.fP5V[0]	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_FB_P5V -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *5.0);
			g_WSM30KVmon.monVT.fP15V[0]	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_FB_P15V -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *15.0);
			g_WSM30KVmon.monVT.fN15V[0]	= (float)(-1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_FB_N15V -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *15.0);
			g_WSM30KVmon.monVT.fP24V[0]	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_FB_P24V -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *24.0);
			g_WSM30KVmon.monVT.fTemp[0]	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_FB_Temp -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *250);
			g_WSM30KVmon.monFB.fSCIv	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_FB_SCI_V -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *g_fWSM30KVFS_SCI_mon_V);
			g_WSM30KVmon.monFB.fSCIi	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_FB_SCI_I -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *g_fWSM30KVFS_SCI_mon_I);
			g_WSM30KVmon.monFB.fACLv	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_FB_ACL_V -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *g_fWSM30KVFS_ACL_mon_V);
			g_WSM30KVmon.monFB.fACLi	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_FB_ACL_I -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *g_fWSM30KVFS_ACL_mon_I);
			g_WSM30KVmon.monFB.fCOLv	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_FB_COL_V -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *g_fWSM30KVFS_COL_mon_V -g_fWSM30KVFS_COL_mon_V/2);
			g_WSM30KVmon.monFB.fCOLi	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_FB_COL_I -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *g_fWSM30KVFS_COL_mon_I);
			g_WSM30KVmon.monFB.fPMTv	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_FB_PMT_V -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *g_fWSM30KVFS_PMT_mon_V);
			g_WSM30KVmon.monFB.fPMTi	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_FB_PMT_I -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *g_fWSM30KVFS_PMT_mon_V);
			g_WSM30KVmon.monOP.fSCIv	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_Given_SCI_V -byteWSM30KV_Offset] / g_wWSM30KVFS_2 *g_fWSM30KVFS_SCI_mon_V);
			g_WSM30KVmon.monOP.fACLv	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_Given_ACL_V -byteWSM30KV_Offset] / g_wWSM30KVFS_2 *g_fWSM30KVFS_ACL_mon_V);
			g_WSM30KVmon.monOP.fACLi	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_Given_ACL_I -byteWSM30KV_Offset] / g_wWSM30KVFS_2 *g_fWSM30KVFS_ACL_mon_I);
			g_WSM30KVmon.monOP.fCOLv	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_Given_COL_V -byteWSM30KV_Offset] / g_wWSM30KVFS_2 *g_fWSM30KVFS_COL_mon_V -g_fWSM30KVFS_COL_mon_V/2);
			g_WSM30KVmon.monOP.fPMTv	= (float)(1.0 *g_wWSM30KVReg01[AddrWSM30KVR01_Given_PMT_V -byteWSM30KV_Offset] / g_wWSM30KVFS_2 *g_fWSM30KVFS_PMT_mon_V);
		}
	}
	else if( bBuf[0] == byteWSM30KV_Sn25 )
	{
		// power#25通道
		if( bBuf[2] == nWSM30KVCount_Registers *2 )
		{
			for( int i=0; i<nWSM30KVCount_Registers; i++ )
				g_wWSM30KVReg25[i] = ( bBuf[i*2 +3] << 8 ) | bBuf[i*2 +4];
			if( g_wWSM30KVReg25[AddrWSM30KVR25_INTStatus -byteWSM30KV_Offset] != g_WSM30KVstatus.statusINT25.wStatus )
			{
				g_WSM30KVstatus.statusINT25.wStatus = g_wWSM30KVReg25[AddrWSM30KVR25_INTStatus -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR25_INTStatus +byteWSM30KV_Offset, (LPARAM)(&(g_WSM30KVstatus.statusINT25)) );
			}
			if( g_wWSM30KVReg25[AddrWSM30KVR25_OutputStatus1 -byteWSM30KV_Offset] != g_WSM30KVstatus.statusO25.wStatus )
			{
				g_WSM30KVstatus.statusO25.wStatus = g_wWSM30KVReg25[AddrWSM30KVR25_OutputStatus1 -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR25_OutputStatus1 +byteWSM30KV_Offset, (LPARAM)(&(g_WSM30KVstatus.statusO25)) );
			}
			if( g_wWSM30KVReg25[AddrWSM30KVR25_ProtectStatusB -byteWSM30KV_Offset] != g_WSM30KVstatus.statusB25.wStatus )
			{
				g_WSM30KVstatus.statusB25.wStatus = g_wWSM30KVReg25[AddrWSM30KVR25_ProtectStatusB -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR25_ProtectStatusB +byteWSM30KV_Offset, (LPARAM)(&(g_WSM30KVstatus.statusB25)) );
			}
			if( g_wWSM30KVReg25[AddrWSM30KVR25_ProtectStatusV -byteWSM30KV_Offset] != g_WSM30KVstatus.statusV25.wStatus )
			{
				g_WSM30KVstatus.statusV25.wStatus = g_wWSM30KVReg25[AddrWSM30KVR25_ProtectStatusV -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR25_ProtectStatusV +byteWSM30KV_Offset, (LPARAM)(&(g_WSM30KVstatus.statusV25)) );
			}
			if( g_wWSM30KVReg25[AddrWSM30KVR25_ProtectStatusI -byteWSM30KV_Offset] != g_WSM30KVstatus.statusI25.wStatus )
			{
				g_WSM30KVstatus.statusI25.wStatus = g_wWSM30KVReg25[AddrWSM30KVR25_ProtectStatusI -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR25_ProtectStatusI +byteWSM30KV_Offset, (LPARAM)(&(g_WSM30KVstatus.statusI25)) );
			}
			if( g_wWSM30KVReg25[AddrWSM30KVR25_ProtectStatusArc -byteWSM30KV_Offset] != g_WSM30KVstatus.statusA25.wStatus )
			{
				g_WSM30KVstatus.statusA25.wStatus = g_wWSM30KVReg25[AddrWSM30KVR25_ProtectStatusArc -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR25_ProtectStatusArc +byteWSM30KV_Offset, (LPARAM)(&(g_WSM30KVstatus.statusA25)) );
			}
			if( g_wWSM30KVReg25[AddrWSM30KVR25_InquireStatus1 -byteWSM30KV_Offset] != g_WSM30KVstatus.status125.wStatus )
			{
				g_WSM30KVstatus.status125.wStatus = g_wWSM30KVReg25[AddrWSM30KVR25_InquireStatus1 -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR01_InquireStatus1 +byteWSM30KV_Offset, (LPARAM)(&(g_WSM30KVstatus.status125)) );
			}
			if( g_wWSM30KVReg25[AddrWSM30KVR25_InquireStatus2 -byteWSM30KV_Offset] != g_WSM30KVstatus.status225.wStatus )
			{
				g_WSM30KVstatus.status225.wStatus = g_wWSM30KVReg25[AddrWSM30KVR25_InquireStatus2 -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR01_InquireStatus2 +byteWSM30KV_Offset, (LPARAM)(&(g_WSM30KVstatus.status225)) );
			}
			if( g_wWSM30KVReg25[AddrWSM30KVR25_InquireStatus3 -byteWSM30KV_Offset] != g_WSM30KVstatus.status325.wStatus )
			{
				g_WSM30KVstatus.status325.wStatus = g_wWSM30KVReg25[AddrWSM30KVR25_InquireStatus3 -byteWSM30KV_Offset];
				PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KVR01_InquireStatus3 +byteWSM30KV_Offset, (LPARAM)(&(g_WSM30KVstatus.status325)) );
			}
			g_WSM30KVmon.monVT.fP3V3[1]	= (float)(1.0 *g_wWSM30KVReg25[AddrWSM30KVR25_FB_P3V3 -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *3.3);
			g_WSM30KVmon.monVT.fP5V[1]	= (float)(1.0 *g_wWSM30KVReg25[AddrWSM30KVR25_FB_P5V -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *5.0);
			g_WSM30KVmon.monVT.fP15V[1]	= (float)(1.0 *g_wWSM30KVReg25[AddrWSM30KVR25_FB_P15V -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *15.0);
			g_WSM30KVmon.monVT.fN15V[1]	= (float)(-1.0 *g_wWSM30KVReg25[AddrWSM30KVR25_FB_N15V -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *15.0);
			g_WSM30KVmon.monVT.fP24V[1]	= (float)(1.0 *g_wWSM30KVReg25[AddrWSM30KVR25_FB_P24V -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *24.0);
			g_WSM30KVmon.monVT.fTemp[1]	= (float)(1.0 *g_wWSM30KVReg25[AddrWSM30KVR25_FB_Temp -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *250);
			g_WSM30KVmon.monFB.fFILv	= (float)(1.0 *g_wWSM30KVReg25[AddrWSM30KVR25_FB_FIL_V -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *g_fWSM30KVFS_FIL_mon_V);
			g_WSM30KVmon.monFB.fFILi	= (float)(1.0 *g_wWSM30KVReg25[AddrWSM30KVR25_FB_FIL_I -byteWSM30KV_Offset] / g_wWSM30KVFS_1 *g_fWSM30KVFS_FIL_mon_I);
			g_WSM30KVmon.monOP.fFILi	= (float)(1.0 *g_wWSM30KVReg25[AddrWSM30KVR25_Given_FIL_I -byteWSM30KV_Offset] / g_wWSM30KVFS_2 *g_fWSM30KVFS_FIL_mon_I);
		}
	}
	PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateMon, AddrWSM30KVR01_FB_P3V3, (LPARAM)(&(g_WSM30KVmon.monVT)) );
	PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateMon, AddrWSM30KVR01_FB_SCI_V, (LPARAM)(&(g_WSM30KVmon.monFB)) );
	PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateMon, AddrWSM30KVR01_Given_SCI_V, (LPARAM)(&(g_WSM30KVmon.monOP)) );
}

void AnalyzeData_WriteReg( BYTE* bBuf )
{
	// 设置参数值
	if( bBuf[0] == byteWSM30KV_Sn01 )
	{
		// power#01通道
	}
	else if( bBuf[0] == byteWSM30KV_Sn25 )
	{
		// power#25通道
	}
}

void AnalyzeData_WriteCoil( BYTE* bBuf )
{
	// 设置使能、复位信号
	if( bBuf[0] == byteWSM30KV_Sn01 )
	{
		// power#01通道
	}
	else if( bBuf[0] == byteWSM30KV_Sn25 )
	{
		// power#25通道
	}
}

void AnalyzeData_ShieldReg( BYTE* bBuf )
{
	if( bBuf[0] == byteWSM30KV_Sn01 )
	{
		// power#01通道
	}
	else if( bBuf[0] == byteWSM30KV_Sn25 )
	{
		// power#25通道
	}
}

void WSM30KV_ConvertReceiveData( int nLenRecv, char* data )
{
	if( nLenRecv < 1 )
		return;

	char cRemain[512] = {0}, cBuf[512] = {0};
	while( nLenRecv > 2 )
	{
		if( strchr( data, '\r\n') != NULL )
		{
			// 找到反馈帧尾，并验证LRC
			strcpy( cRemain, strchr(data, '\r\n') +2 );
			*strchr( data, '\r\n') = '\0';
			if( strchr( data, ':') != NULL )
			{
				strcpy( cBuf, strchr(data, ':') +1 );
				// Modbus ASCII帧的最大尺寸是513个字符，
				// 减去起始1字符、结束2字符、校验2字符后，即为全部数据个数
				BYTE bBuf[513-5] = {0};
				unsigned short usDataLen = 0;
				if( !VerifyLRC(cBuf, bBuf, usDataLen) )
				{
					strcpy( data, cRemain );
					nLenRecv = strlen(data);
					continue;	// ?????
				}
				switch( bBuf[1] )	// 0为地址，1为功能号
				{
				case	byteWSM30KV_FuncCodeR:
					AnalyzeData_ReadReg(bBuf);
					break;
				case	byteWSM30KV_FuncCodeWR:
					AnalyzeData_WriteReg(bBuf);
					break;
				case	byteWSM30KV_FuncCodeWC:
					AnalyzeData_WriteCoil(bBuf);
					break;
				case	byteWSM30KV_FuncCodeS:
					AnalyzeData_ShieldReg(bBuf);
					break;
				}
				strcpy( data, cRemain );
				nLenRecv = strlen(data);
			}
		}
		else
			break;
	}

	g_nWSM30KVSavedCounts = nLenRecv;
	if( nLenRecv > 0 )
	{
		strcpy( g_cWSM30KVSaved, data );
	}
}

void WSM30KV_ConvertSendData( BYTE bAddr, BYTE bFunc, WORD wAddr, WORD wValue, char* data )
{
	//			起始符（冒号）	从机地址	功能号	寄存器地址	数据
	// HEX:		1字节			1字节		1字节	2字节		2字节
	// ASCII:	1字符			2字符		2字符	4字符		4字符
	BYTE bData[7];
	bData[0] = bAddr;
	bData[1] = bFunc;	// 读寄存器、写单个寄存器、写单个线圈
	bData[2] = (BYTE)((wAddr & 0xFF00) >> 8);
	bData[3] = (BYTE)(wAddr & 0x00FF);
	bData[4] = (BYTE)((wValue & 0xFF00) >> 8);
	bData[5] = (BYTE)(wValue & 0x00FF);
	bData[6] = LRC( bData, 6 );
	sprintf( data, ":%02X%02X%04X%04X%02X\r\n", bAddr, bFunc, wAddr, wValue, bData[6] );
}

int WSM30KV_Send( int nSleep, BYTE bAddr, BYTE bFunc, WORD wAddr, WORD wValue )
{
	if( !g_WSM30KVcfg.bReady )
		return -1;
	WSM30KV_Send_Msg msg;
	WSM30KV_ConvertSendData( bAddr, bFunc, wAddr, wValue, msg.cData );
	msg.nLength = strlen(msg.cData);
	msg.cData[msg.nLength] = '\0';
	if( !g_WSM30KVfifoHostMsg.IsFIFOFull() )
	{
		if( bFunc != byteWSM30KV_FuncCodeR )
			g_WSM30KVfifoHostMsg.Input(msg);
		else
		{
			if( g_WSM30KVfifoHostMsg.GetFIFOCount() < WSM30KV_DevFIFOSize -2 )
				g_WSM30KVfifoHostMsg.Input(msg);
		}
	}
	return msg.nLength;
}

char* Float2Hex(const float f) 
{
	unsigned char *hex = (unsigned char *)&f;
	char* sHex = new char[9];
	memset(sHex,0,9);
	sprintf(sHex,"%02X%02X%02X%02X",hex[0],hex[1],hex[2],hex[3]);
	return sHex;
}

DWORD Float2DWORD(const float f) 
{
	unsigned char *hex = (unsigned char *)&f;
	char* sHex = new char[2*8 + 1];
	memset(sHex,0,17);
	sprintf(sHex,"%02X%02X%02X%02X",hex[3],hex[2],hex[1],hex[0]);	// 高位在前
	DWORD dValue = (DWORD)strtoul(sHex, NULL, 16);
	delete sHex;
	sHex = NULL;
	return dValue;
}

BOOL Change_ACL_V( int nValue, BOOL bFromMain )
{
	if( !g_WSM30KVcfg.bReady )
		return FALSE;
	g_WSM30KVset.fACLv = (float)(1.0 *nValue);
	if( g_WSM30KVset.fACLv > g_fWSM30KVFS_ACL_mon_V )
		g_WSM30KVset.fACLv = g_fWSM30KVFS_ACL_mon_V;
	if( g_WSM30KVset.fACLv < 0.0 )
		g_WSM30KVset.fACLv = 0.0;
	// Analog --> Digital
	WORD wValue = (WORD)(g_WSM30KVset.fACLv /g_fWSM30KVFS_ACL_mon_V *g_wWSM30KVFS_2);
	if( WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_Sn01, byteWSM30KV_FuncCodeWR, AddrWSM30KVR01_Given_ACL_V_M, wValue ) > 0 )
		return TRUE;
	else
		return FALSE;
}

BOOL Set_ACL_V( int nValue, BOOL bFromMain )
{
	return TRUE;
}

BOOL Change_ACL_I( int nValue, BOOL bFromMain )
{
	if( !g_WSM30KVcfg.bReady )
		return FALSE;
	g_WSM30KVset.fACLi = (float)(1.0 *nValue);
	if( g_WSM30KVset.fACLi > g_fWSM30KVFS_ACL_mon_I )
		g_WSM30KVset.fACLi = g_fWSM30KVFS_ACL_mon_I;
	if( g_WSM30KVset.fACLi < 0.0 )
		g_WSM30KVset.fACLi = 0.0;
	// Analog --> Digital
	WORD wValue = (WORD)(g_WSM30KVset.fACLi /g_fWSM30KVFS_ACL_mon_I *g_wWSM30KVFS_2);
	if( WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_Sn01, byteWSM30KV_FuncCodeWR, AddrWSM30KVR01_Given_ACL_I_M, wValue ) > 0 )
		return TRUE;
	else
		return FALSE;
}

BOOL Change_FIL( int nValue, BOOL bFromMain )
{
	if( !g_WSM30KVcfg.bReady )
		return FALSE;
	g_WSM30KVset.fFILi = (float)(nValue /1000.0);		// mA --> A
	if( g_WSM30KVset.fFILi > g_fWSM30KVFS_FIL_mon_I )
		g_WSM30KVset.fFILi = g_fWSM30KVFS_FIL_mon_I;
	if( g_WSM30KVset.fFILi < 0.0 )
		g_WSM30KVset.fFILi = 0.0;
	// Analog --> Digital
	WORD wValue = (WORD)(g_WSM30KVset.fFILi /g_fWSM30KVFS_FIL_mon_I *g_wWSM30KVFS_2);
	if( WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_Sn25, byteWSM30KV_FuncCodeWR, AddrWSM30KVR25_Given_FIL_I_M, wValue ) > 0 )
		return TRUE;
	else
		return FALSE;
}

BOOL Set_FIL( int nValue, BOOL bFromMain )
{
	return TRUE;
}

BOOL Change_PMT( int nValue, BOOL bFromMain )
{
	if( !g_WSM30KVcfg.bReady )
		return FALSE;
	g_WSM30KVset.fPMTv = (float)(1.0 *nValue);
	if( g_WSM30KVset.fPMTv > g_fWSM30KVFS_PMT_mon_V )
		g_WSM30KVset.fPMTv = g_fWSM30KVFS_PMT_mon_V;
	if( g_WSM30KVset.fPMTv < 0.0 )
		g_WSM30KVset.fPMTv = 0.0;
	// Analog --> Digital
	WORD wValue = (WORD)(g_WSM30KVset.fPMTv /g_fWSM30KVFS_PMT_mon_V *g_wWSM30KVFS_2);
	if( WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_Sn01, byteWSM30KV_FuncCodeWR, AddrWSM30KVR01_Given_PMT_V_M, wValue ) > 0 )
		return TRUE;
	else
		return FALSE;
}

BOOL Change_SCI( int nValue, BOOL bFromMain )
{
	if( !g_WSM30KVcfg.bReady )
		return FALSE;
	WORD wValue = 0x0;
	if( nValue > 0 )
		wValue = g_wWSM30KVFS_2;
	g_WSM30KVset.fSCIv = g_fWSM30KVFS_SCI_mon_V;
//	g_WSM30KVset.fSCIv = (float)(wValue *g_fWSM30KVFS_SCI_mon_V /g_wWSM30KVFS_2);
	if( WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_Sn01, byteWSM30KV_FuncCodeWR, AddrWSM30KVR01_Given_SCI_V_M, wValue ) > 0 )
		return TRUE;
	else
		return FALSE;
}

BOOL Change_COL( int nValue, BOOL bFromMain )
{
	if( !g_WSM30KVcfg.bReady )
		return FALSE;
	WORD wValue = 0x0;
	if( nValue > 0 )
	{
		wValue = (WORD)(1.0 *(300 +g_fWSM30KVFS_COL_mon_V/2) /g_fWSM30KVFS_COL_mon_V *g_wWSM30KVFS_2);
//		wValue = (WORD)(1.0 *(30 +g_fWSM30KVFS_COL_mon_V/2) /g_fWSM30KVFS_COL_mon_V *g_wWSM30KVFS_2);
		g_WSM30KVset.fCOLv = 300;	//30;	// 300
	}
	else
	{
		wValue = (WORD)(g_wWSM30KVFS_2/2);	// 正负范围，0是中间值
		g_WSM30KVset.fCOLv = 0;
	}
	if( WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_Sn01, byteWSM30KV_FuncCodeWR, AddrWSM30KVR01_Given_COL_V_M, wValue ) > 0 )
		return TRUE;
	else
		return FALSE;
}

BOOL setenable( int nIndex, int nCheck )
{
	if( !g_WSM30KVcfg.bReady )
		return FALSE;
	WORD wValue = 0x0000;
	if( nCheck > 0 )
		wValue = 0xFF00;

	int nStatus = 0;
	if( nIndex < 17 )
	{
		BYTE bAddr = byteWSM30KV_Sn01;
		WORD wAddr = 0x0000;
		switch( nIndex )
		{
		case	11:	// SCI
			wAddr	= AddrWSM30KVC01_ReqOn_Power1;
			nStatus	= g_WSM30KVstatus.statusO01.status.SCI;
			break;
		case	12:	// ACL
			wAddr	= AddrWSM30KVC01_ReqOn_Power2;
			nStatus	= g_WSM30KVstatus.statusO01.status.ACL_FIL;
			break;
		case	13:	// EMS
			wAddr	= AddrWSM30KVC01_ReqOn_Power3;
			nStatus	= g_WSM30KVstatus.statusO01.status.EMS;
			break;
		case	14:	// COL
			wAddr	= AddrWSM30KVC01_ReqOn_Power5;
			nStatus	= g_WSM30KVstatus.statusO01.status.COL;
			break;
		case	15:	// PMT
			wAddr	= AddrWSM30KVC01_ReqOn_Power6;
			nStatus	= g_WSM30KVstatus.statusO01.status.PMT;
			break;
		case	16:	// FIL
			bAddr	= byteWSM30KV_Sn25;
			wAddr	= AddrWSM30KVC25_ReqOn_Power2;
			nStatus	= g_WSM30KVstatus.statusO25.status.ACL_FIL;
			break;
		}
		if( nStatus != nCheck )
		{
			if( WSM30KV_Send( nWSM30KV_Sleep, bAddr, byteWSM30KV_FuncCodeWC, wAddr, wValue ) > 0 )
				return TRUE;
			else
				return FALSE;
		}
		else
			return TRUE;
	}
	else
	{
		// 广播
		if( wValue != 0 )
			wValue = 0x00FF;
		if( WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_SnBroadcast, byteWSM30KV_FuncCodeWR, AddrWSM30KVR01_InquireStatus1, wValue ) > 0 )
			return TRUE;
		else
			return FALSE;
	}
}

BOOL setpause( int nIndex, int nCheck )
{
	if( !g_WSM30KVcfg.bReady )
		return FALSE;
	WORD wValue = 0x0000;
	if( nCheck > 0 )
		wValue = 0xFF00;

	if( nIndex < 27 )
	{
		BYTE bAddr = byteWSM30KV_Sn01;
		WORD wAddr = 0x0000;
		switch( nIndex )
		{
		case	21:	// SCI
			wAddr	= AddrWSM30KVC01_ReqPause_Power1;
			break;
		case	22:	// ACL
			wAddr	= AddrWSM30KVC01_ReqPause_Power2;
			break;
		case	23:	// EMS
			wAddr	= AddrWSM30KVC01_ReqPause_Power3;
			break;
		case	24:	// COL
			wAddr	= AddrWSM30KVC01_ReqPause_Power5;
			break;
		case	25:	// PMT
			wAddr	= AddrWSM30KVC01_ReqPause_Power6;
			break;
		case	26:	// FIL
			bAddr	= byteWSM30KV_Sn25;
			wAddr	= AddrWSM30KVC25_ReqPause_Power2;
			break;
		}
		if( WSM30KV_Send( nWSM30KV_Sleep, bAddr, byteWSM30KV_FuncCodeWC, wAddr, wValue ) > 0 )
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if( WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_Sn01, byteWSM30KV_FuncCodeWC, AddrWSM30KVC01_ReqPause_Power1, wValue )
			&& WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_Sn01, byteWSM30KV_FuncCodeWC, AddrWSM30KVC01_ReqPause_Power2, wValue )
			&& WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_Sn01, byteWSM30KV_FuncCodeWC, AddrWSM30KVC01_ReqPause_Power3, wValue )
			&& WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_Sn01, byteWSM30KV_FuncCodeWC, AddrWSM30KVC01_ReqPause_Power5, wValue )
			&& WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_Sn01, byteWSM30KV_FuncCodeWC, AddrWSM30KVC01_ReqPause_Power6, wValue )
			&& WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_Sn25, byteWSM30KV_FuncCodeWC, AddrWSM30KVC25_ReqPause_Power2, wValue ) )
			return TRUE;
		else
			return FALSE;
	}
}

BOOL reset( int nIndex, int nCheck )
{
	if( !g_WSM30KVcfg.bReady )
		return FALSE;
	WORD wValue = 0x0000;
	if( nCheck > 0 )
		wValue = 0xFF00;

	if( nIndex < 37 )
	{
		BYTE bAddr = byteWSM30KV_Sn01;
		WORD wAddr = 0x0000;
		switch( nIndex )
		{
		case	31:	// SCI
			wAddr	= AddrWSM30KVC01_ReqReset_Power1;
			break;
		case	32:	// ACL
			wAddr	= AddrWSM30KVC01_ReqReset_Power2;
			break;
		case	33:	// EMS
			wAddr	= AddrWSM30KVC01_ReqReset_Power3;
			break;
		case	34:	// COL
			wAddr	= AddrWSM30KVC01_ReqReset_Power5;
			break;
		case	35:	// PMT
			wAddr	= AddrWSM30KVC01_ReqReset_Power6;
			break;
		case	36:	// FIL
			bAddr	= byteWSM30KV_Sn25;
			wAddr	= AddrWSM30KVC25_ReqReset_Power2;
			break;
		}
		if( WSM30KV_Send( nWSM30KV_Sleep, bAddr, byteWSM30KV_FuncCodeWC, wAddr, wValue ) > 0 )
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		// 广播
		if( wValue != 0 )
			wValue = 0x00FF;
		if( WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_SnBroadcast, byteWSM30KV_FuncCodeWR, AddrWSM30KVR01_InquireStatus3, wValue ) > 0 )
			return TRUE;
		else
			return FALSE;
	}
}

BOOL WSM30KVCommTest( int nSleep )
{
	BOOL bRet = FALSE;
	char buf[255];
	WSM30KV_ConvertSendData( byteWSM30KV_Sn01, byteWSM30KV_FuncCodeR, AddrWSM30KVR01_OutputStatus1 -1, 66, buf );
	g_WSM30KV.SendDataEx2( buf, strlen(buf) );
	Sleep(nSleep);

	int nTimes = 0;
	g_nWSM30KVSavedCounts = 0;
	char data[1024];
	WSM30KV_Recv_Msg msgRead;
	do{
		msgRead.nLength = g_WSM30KV.ReadData( msgRead.cData, 512 );
		msgRead.cData[msgRead.nLength] = '\0';
		if( g_nWSM30KVSavedCounts > 0 )	// 拼数据
		{
			strcpy( data, g_cWSM30KVSaved );
			strcat( data +g_nWSM30KVSavedCounts, msgRead.cData );
		}
		else
			strcpy( data, msgRead.cData );
		WSM30KV_ConvertReceiveData( strlen(data), data );
		if( g_WSM30KVmon.monVT.fP3V3[0] > 3 )
			break;
		nTimes++;
		Sleep(nSleep);
	} while( nTimes < 50 );
	if( nTimes < 50 )
	{
		bRet = TRUE;
		g_nWSM30KVSavedCounts = 0;
	}
	return bRet;
}

DWORD WINAPI WSM30KVThreadInquire(LPVOID lpParam)
{
	while (TRUE)
	{
		if( g_bWSM30KVEndThreads )
		{
			break;
		}
//		EnterCriticalSection(&csFifoHost);
//		LeaveCriticalSection(&csFifoHost);
		WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_Sn01, byteWSM30KV_FuncCodeR, AddrWSM30KVR01_OutputStatus1 -1, 66 );
		WSM30KV_Send( nWSM30KV_Sleep, byteWSM30KV_Sn25, byteWSM30KV_FuncCodeR, AddrWSM30KVR25_OutputStatus1 -1, 66 );
		Sleep(500);
	}
	g_nWSM30KVThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadInquire, counts = %d\n", g_nWSM30KVThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}
 
DWORD WINAPI WSM30KVThreadWR(LPVOID lpParam)
{
	while (TRUE)
	{
		if( g_bWSM30KVEndThreads )
		{
//			LeaveCriticalSection(&csWSM30KVPort);
// 			LeaveCriticalSection(&csFifoHost);
			break;
		}
		
		EnterCriticalSection(&csWSM30KVPort);
//		EnterCriticalSection(&csFifoHost);
		
		if( !g_WSM30KVfifoHostMsg.IsFIFOEmpty() )
		{
			// 发送数据
			WSM30KV_Send_Msg msgSend;
			g_WSM30KVfifoHostMsg.Output(msgSend);
			// 读取应答
			BOOL bTimeout = FALSE;
			WSM30KV_Recv_Msg msgRead;
			int nSend = g_WSM30KV.SendDataEx2(msgSend.cData, msgSend.nLength);
			if( nSend < 1 )
				continue;
			Sleep(nWSM30KV_Sleep);
			DWORD dwEndTime, dwStartTime = GetTickCount();
			do
			{
				dwEndTime = GetTickCount();
				if( dwEndTime -dwStartTime > 1000 )
					bTimeout = TRUE;
				if( bTimeout )
					break;
				msgRead.cData[0] = '\0';
				msgRead.nLength = g_WSM30KV.ReadData( msgRead.cData, WSM30KV_ReadFromDevice_DataSize );
				if( msgRead.nLength > 0 && !g_WSM30KVfifoDevMsg.IsFIFOFull() )
				{
					msgRead.cData[msgRead.nLength] = '\0';
					int len = strlen(msgRead.cData);
					g_WSM30KVfifoDevMsg.Input(msgRead);
					break;
				}
			} while( msgRead.nLength < 1 );
		}
		
		LeaveCriticalSection(&csWSM30KVPort);
//		LeaveCriticalSection(&csFifoHost);
		
		Sleep(10);
	}
	while( g_nWSM30KVThreadCounts > g_nWSM30KVThreadCountsAll -1 )
		Sleep(1);
	g_nWSM30KVThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadSend, counts = %d\n", g_nWSM30KVThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

DWORD WINAPI WSM30KVThreadSend(LPVOID lpParam)
{
	while (TRUE)
	{
		if( g_bWSM30KVEndThreads )
		{
//			LeaveCriticalSection(&csWSM30KVPort);
// 			LeaveCriticalSection(&csFifoHost);
			break;
		}
		
		EnterCriticalSection(&csWSM30KVPort);
//		EnterCriticalSection(&csFifoHost);
		
		if( !g_WSM30KVfifoHostMsg.IsFIFOEmpty() )
		{
			WSM30KV_Send_Msg msg;
			g_WSM30KVfifoHostMsg.Output(msg);
			g_WSM30KV.SendDataEx2(msg.cData, msg.nLength);
		}
		
		LeaveCriticalSection(&csWSM30KVPort);
//		LeaveCriticalSection(&csFifoHost);
		
		Sleep(10);
	}
	while( g_nWSM30KVThreadCounts > g_nWSM30KVThreadCountsAll )
		Sleep(1);
	g_nWSM30KVThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadSend, counts = %d\n", g_nWSM30KVThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

DWORD WINAPI WSM30KVThreadRead(LPVOID lpParam)
{
	char* sFlag = (char*)(lpParam);
	while (TRUE)
	{
		if( g_bWSM30KVEndThreads )
		{
//			LeaveCriticalSection(&csWSM30KVPort);
// 			LeaveCriticalSection(&csFifoHost);
			break;
		}
		
		EnterCriticalSection(&csWSM30KVPort);
//		EnterCriticalSection(&csFifoDev);
		
		WSM30KV_Recv_Msg msg;
//		msg.cData[0] = '\0';
		msg.nLength = g_WSM30KV.ReadData( msg.cData, WSM30KV_ReadFromDevice_DataSize );
		if( msg.nLength > 0 && !g_WSM30KVfifoDevMsg.IsFIFOFull() )
		{
			msg.cData[msg.nLength] = '\0';
			g_WSM30KVfifoDevMsg.Input(msg);
		}

		LeaveCriticalSection(&csWSM30KVPort);
//		LeaveCriticalSection(&csFifoDev);
		
		Sleep(400);
	}
	while( g_nWSM30KVThreadCounts > g_nWSM30KVThreadCountsAll -1 )
		Sleep(1);
	g_nWSM30KVThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadRead, counts = %d\n", g_nWSM30KVThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

DWORD WINAPI WSM30KVThreadAnalyze(LPVOID lpParam)
{
//	char* sFlag = (char*)(lpParam);
//	CTestSpellmanRSDlg* pDlg = (CTestSpellmanRSDlg*)(lpParam);
	while (TRUE)
	{
		if( g_bWSM30KVEndThreads )
		{
// 			LeaveCriticalSection(&csPort);
// 			LeaveCriticalSection(&csFifoHost);
			break;
		}
		
//		EnterCriticalSection(&csFifoDev);
		if( !g_WSM30KVfifoDevMsg.IsFIFOEmpty() )
		{
			WSM30KV_Recv_Msg msg;
			g_WSM30KVfifoDevMsg.Output(msg);
			char data[1024] = {0};
			if( g_nWSM30KVSavedCounts > 0 )	// 拼数据
			{
				strcpy( data, g_cWSM30KVSaved );
				strcat( data +g_nWSM30KVSavedCounts, msg.cData );
			}
			else
				strcpy( data, msg.cData );
			WSM30KV_ConvertReceiveData( strlen(data), data );
		}
//		LeaveCriticalSection(&csFifoDev);
		
		Sleep(200);
	}
	while( g_nWSM30KVThreadCounts > g_nWSM30KVThreadCountsAll -2 )
		Sleep(1);
	g_nWSM30KVThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadAnalyze, counts = %d\n", g_nWSM30KVThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

DWORD WINAPI WSM30KVThreadConvert(LPVOID lpParam)
{
	while (TRUE)
	{
		if( g_bWSM30KVEndThreads )
		{
			break;
		}
		Sleep(nWSM30KV_Sleep);
	}
	while( g_nWSM30KVThreadCounts > g_nWSM30KVThreadCountsAll -3 )
		Sleep(1);
	g_nWSM30KVThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadConvert, counts = %d\n", g_nWSM30KVThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

DWORD WINAPI WSM30KVThreadLog(LPVOID lpParam)
{
	while (TRUE)
	{
		if( g_bWSM30KVEndThreads )
		{
			break;
		}
		char szBuf[1024], szBufStatus[255], szBufValues[255], szBufVT[255];
		sprintf( szBufStatus, "%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,", 
			g_WSM30KVstatus.statusINT01.wStatus, g_WSM30KVstatus.statusINT25.wStatus,
			g_WSM30KVstatus.statusO01.wStatus, g_WSM30KVstatus.statusO25.wStatus,
			g_WSM30KVstatus.statusB01.wStatus, g_WSM30KVstatus.statusB25.wStatus,
			g_WSM30KVstatus.statusV01.wStatus, g_WSM30KVstatus.statusV25.wStatus,
			g_WSM30KVstatus.statusI01.wStatus, g_WSM30KVstatus.statusI25.wStatus,
			g_WSM30KVstatus.statusA25.wStatus,
			g_WSM30KVstatus.status101.wStatus, g_WSM30KVstatus.status125.wStatus,
			g_WSM30KVstatus.status201.wStatus, g_WSM30KVstatus.status125.wStatus,
			g_WSM30KVstatus.status301.wStatus, g_WSM30KVstatus.status325.wStatus );
		sprintf( szBufValues, "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,",
			g_WSM30KVset.fSCIv, g_WSM30KVmon.monOP.fSCIv, g_WSM30KVmon.monFB.fSCIv, g_WSM30KVmon.monFB.fSCIi,
			g_WSM30KVset.fCOLv, g_WSM30KVmon.monOP.fCOLv, g_WSM30KVmon.monFB.fCOLv, g_WSM30KVmon.monFB.fCOLi,
			g_WSM30KVset.fPMTv, g_WSM30KVmon.monOP.fPMTv, g_WSM30KVmon.monFB.fPMTv, g_WSM30KVmon.monFB.fPMTi,
			g_WSM30KVset.fACLv, g_WSM30KVmon.monOP.fACLv, g_WSM30KVmon.monFB.fACLv,
			g_WSM30KVset.fACLi, g_WSM30KVmon.monOP.fACLi, g_WSM30KVmon.monFB.fACLi,
			g_WSM30KVset.fFILi, g_WSM30KVmon.monOP.fFILi, g_WSM30KVmon.monFB.fFILv, g_WSM30KVmon.monFB.fFILi );
		sprintf( szBufVT, "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f",
			g_WSM30KVmon.monVT.fP3V3[0], g_WSM30KVmon.monVT.fP3V3[1],
			g_WSM30KVmon.monVT.fP5V[0], g_WSM30KVmon.monVT.fP5V[1],
			g_WSM30KVmon.monVT.fP15V[0], g_WSM30KVmon.monVT.fP15V[1],
			g_WSM30KVmon.monVT.fN15V[0], g_WSM30KVmon.monVT.fN15V[1],
			g_WSM30KVmon.monVT.fP24V[0], g_WSM30KVmon.monVT.fP24V[1],
			g_WSM30KVmon.monVT.fTemp[0], g_WSM30KVmon.monVT.fTemp[1] );
		sprintf( szBuf, "%s%s%s", szBufStatus, szBufValues, szBufVT );
		Record(szBuf);
		Sleep(1000);
	}
	while( g_nWSM30KVThreadCounts > g_nWSM30KVThreadCountsAll -4 )
		Sleep(1);
	g_nWSM30KVThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadLog, counts = %d\n", g_nWSM30KVThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

void StartThreads()
{
	HANDLE hThread1, hThread2, hThread3, hThread4, hThread5, hThread6;
	InitializeCriticalSection(&csWSM30KVPort);
	InitializeCriticalSection(&csWSM30KVFifoHost);
	InitializeCriticalSection(&csWSM30KVFifoDev);

/*	char* sSend = "WSM30KVThreadSend";
	hThread1 = CreateThread(NULL, 0, WSM30KVThreadSend, (LPVOID)sSend, 0, NULL);
	g_nWSM30KVThreadCounts++;

	char* sRead = "WSM30KVThreadRead";
	hThread2 = CreateThread(NULL, 0, WSM30KVThreadRead, (LPVOID)sRead, 0, NULL);
	g_nWSM30KVThreadCounts++;
*/
	char* sRW = "WSM30KVThreadWR";
	hThread2 = CreateThread(NULL, 0, WSM30KVThreadWR, (LPVOID)sRW, 0, NULL);
	g_nWSM30KVThreadCounts++;

	char* sAnalyze = "WSM30KVThreadAnalyze";
	hThread3 = CreateThread(NULL, 0, WSM30KVThreadAnalyze, (LPVOID)sAnalyze, 0, NULL);
	g_nWSM30KVThreadCounts++;

	char* sInquire = "WSM30KVThreadInquire";
	hThread4 = CreateThread(NULL, 0, WSM30KVThreadInquire, (LPVOID)sInquire, 0, NULL);
	g_nWSM30KVThreadCounts++;

	char* sDisplay = "WSM30KVThreadConvert";
	hThread5 = CreateThread(NULL, 0, WSM30KVThreadConvert, (LPVOID)sDisplay, 0, NULL);
	g_nWSM30KVThreadCounts++;

	char* sLog = "WSM30KVThreadLog";
	hThread6 = CreateThread(NULL, 0, WSM30KVThreadLog, (LPVOID)sLog, 0, NULL);
	g_nWSM30KVThreadCounts++;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Auto Functions
void Auto_Rise_ACL1()
{
	g_WSM30KVset.fACLv += g_fWSM30KVAutoStep_ACL1;
	if( g_WSM30KVset.fACLv >= g_fWSM30KVAutoRated_ACL2 )
	{
		g_WSM30KVset.fACLv = g_fWSM30KVAutoRated_ACL2;
		Change_ACL_V((int)(g_WSM30KVset.fACLv), FALSE );
		PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KVAutoState, (LPARAM)(g_WSM30KVset.fACLv) );
		g_WSM30KVAutoState = WSM30KVAS_Rise_FIL;
	}
	else if( g_WSM30KVset.fACLv >= g_fWSM30KVAutoRated_ACL1 )
	{
		g_WSM30KVset.fACLv = g_fWSM30KVAutoRated_ACL1;
		Change_ACL_V((int)(g_WSM30KVset.fACLv), FALSE );
		PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KVAutoState, (LPARAM)(g_WSM30KVset.fACLv) );
		g_WSM30KVAutoState = WSM30KVAS_Rise_ACL2;
	}
	else
	{
		Change_ACL_V((int)(g_WSM30KVset.fACLv), FALSE );
		PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KVAutoState, (LPARAM)(g_WSM30KVset.fACLv) );
	}
}

void Auto_Rise_ACL2()
{
	g_WSM30KVset.fACLv += g_fWSM30KVAutoStep_ACL2;
	if( g_WSM30KVset.fACLv >= g_fWSM30KVAutoRated_ACL2 )
	{
		g_WSM30KVset.fACLv = g_fWSM30KVAutoRated_ACL2;
		Change_ACL_V((int)(g_WSM30KVset.fACLv), FALSE );
		PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KVAutoState, (LPARAM)(g_WSM30KVset.fACLv) );
		g_WSM30KVAutoState = WSM30KVAS_Rise_FIL;
	}
	else
	{
		Change_ACL_V((int)(g_WSM30KVset.fACLv), FALSE );
		PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KVAutoState, (LPARAM)(g_WSM30KVset.fACLv) );
	}
}

void Auto_Rise_ESM()
{
	g_WSM30KVset.fACLi += g_fWSM30KVAutoStep_EMS;
	if( g_WSM30KVset.fACLi >= g_fWSM30KVAutoRated_EMS )
	{
		g_WSM30KVset.fACLi = g_fWSM30KVAutoRated_EMS;
		Change_ACL_I( (int)(g_WSM30KVset.fACLi), FALSE );
		PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KVAutoState, (LPARAM)g_WSM30KVset.fACLi );
		g_WSM30KVAutoState = WSM30KVAS_NULL;	// 结束
		PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KVAutoState, 0 );
	}
	else
	{
		Change_ACL_I( (int)(g_WSM30KVset.fACLi), FALSE );
		PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KVAutoState, (LPARAM)g_WSM30KVset.fACLi );
	}
}

void Auto_Rise_FIL()
{
	g_WSM30KVset.fFILi += g_fWSM30KVAutoStep_FIL;
	if( g_WSM30KVset.fFILi >= g_fWSM30KVAutoRated_FIL )
	{
		g_WSM30KVset.fFILi = g_fWSM30KVAutoRated_FIL;
		Change_FIL( (int)(g_WSM30KVset.fFILi *1000), FALSE );
		PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KVAutoState, (LPARAM)(g_WSM30KVset.fFILi*1000) );
		g_WSM30KVAutoState = WSM30KVAS_Rise_EMS;
	}
	else
	{
		Change_FIL( (int)(g_WSM30KVset.fFILi *1000), FALSE );
		PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KVAutoState, (LPARAM)(g_WSM30KVset.fFILi*1000) );
	}
}

void AutoProc()
{
	DWORD thisTickCount = GetTickCount();
	// 灯丝1
	switch( g_WSM30KVAutoState )
	{
		/////////////////////////////////////////
		// Auto Rise
		/////////////////////////////////////////
	case	WSM30KVAS_Rise_ACL1:
		if( thisTickCount - g_WSM30KVlastAuto > 200 )
		{
			Auto_Rise_ACL1();
			g_WSM30KVlastAuto = GetTickCount();
		}
		break;
	case	WSM30KVAS_Rise_ACL2:
		if( thisTickCount - g_WSM30KVlastAuto > 400 )
		{
			Auto_Rise_ACL2();
			g_WSM30KVlastAuto = GetTickCount();
		}
		break;
	case	WSM30KVAS_Rise_EMS:
		if( thisTickCount - g_WSM30KVlastAuto > 400 )
		{
			Auto_Rise_ESM();
			g_WSM30KVlastAuto = GetTickCount();
		}
		break;
	case	WSM30KVAS_Rise_FIL:
		if( thisTickCount - g_WSM30KVlastAuto > 400 )
		{
			Auto_Rise_FIL();
			g_WSM30KVlastAuto = GetTickCount();
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


DWORD WINAPI WSM30KVCommThread(LPVOID lpParam)
{
	// 打开端口
	g_WSM30KVcfg.bReady = g_WSM30KV.Open( g_WSM30KVcfg.nPort, 115200 );
	if( g_WSM30KVcfg.bReady )
	{
		if( !WSM30KVCommTest(nWSM30KV_Sleep) )
			g_WSM30KVcfg.bReady = FALSE;
		if( g_WSM30KVcfg.bReady )
		{
			CreateLog();
			StartThreads();
			g_WSM30KVlastAuto = GetTickCount();
			reset( 37, 1 );
		}
	}
	PostMessage( g_hWSM30KVMain, WM_USER_SPELLMAN_PMSKV_UpdateStatus, 1, g_WSM30KVcfg.bReady );

	while( g_WSM30KVcfg.bReady && WaitForSingleObject(g_hWSM30KVEventTeminate,200) != WAIT_OBJECT_0 )
	{
//		if( !g_bPauseComm )
		{
			if( g_WSM30KVAutoState < WSM30KVAS_NULL )
			{
				AutoProc();
				Sleep(1);
			}
		}
		Sleep( 1 );
	}

	g_WSM30KV.Close();
	g_WSM30KVcfg.bReady	= g_WSM30KV.IsOpened();
	if( g_hWSM30KVFile != NULL )
		CloseHandle( g_hWSM30KVFile );
	g_hWSM30KVFile	= NULL;

	SetEvent( g_hWSM30KVEventWork );
	return 0;
}

void CCommWSM30KV::SetParam( int nIndex, int nValue )
{
	switch( nIndex )
	{
	case	0:
		g_WSM30KVcfg.nPort	= nValue;
		break;
	case	1:	// ACL V
//		Set_ACL_V( nValue, TRUE );
		Change_ACL_V( nValue, TRUE );
		break;
	case	2:	// ACL I
		Change_ACL_I( nValue, TRUE );
		break;
	case	3:	// FIL
//		Set_FIL( nValue, TRUE );
		Change_FIL( nValue, TRUE );
		break;
	case	4:	// PMT
		Change_PMT( nValue, TRUE );
		break;
	case	5:	// SCI
		Change_SCI( nValue, TRUE );
		break;
	case	6:	// COL
		Change_COL( nValue, TRUE );
		break;

	case	11:	// SCI
	case	12:	// ACL
	case	13:	// EMS
	case	14:	// COL
	case	15:	// PMT
	case	16:	// FIL
	case	17:	// ALL
		setenable( nIndex, nValue );
		break;

	case	21:	// SCI
	case	22:	// ACL
	case	23:	// EMS
	case	24:	// COL
	case	25:	// PMT
	case	26:	// FIL
	case	27:	// ALL
		setpause( nIndex, nValue );
		break;

	case	31:	// SCI
	case	32:	// ACL
	case	33:	// EMS
	case	34:	// COL
	case	35:	// PMT
	case	36:	// FIL
	case	37:	// ALL
		reset( nIndex, nValue );
		break;

	case	41:
		g_fWSM30KVAutoRated_ACL2	= (float)nValue;	// (float)(nValue /1000.0);
		break;
	case	42:
		g_fWSM30KVAutoRated_EMS		= (float)nValue;
		break;
	case	43:
		g_fWSM30KVAutoRated_FIL		= (float)(nValue /100.0);
		break;
	case	49:
		g_WSM30KVAutoState	= (WSM30KV_AutoState)nValue;
		if( g_WSM30KVAutoState == WSM30KVAS_Rise_ACL1 && g_WSM30KVset.fACLv > g_fWSM30KVAutoRated_ACL1 )
			g_WSM30KVAutoState = WSM30KVAS_Rise_ACL2;
		else if( g_WSM30KVAutoState == WSM30KVAS_Rise_ACL1
			&& ((int)(g_WSM30KVset.fACLv *10) == (int)(g_fWSM30KVAutoRated_ACL2 *10)) )
			g_WSM30KVAutoState = WSM30KVAS_Rise_EMS;
		break;

	case	100:
		// test
		{
			char data[100];
//			WSM30KV_ConvertSendData( byteWSM30KV_Sn01, byteWSM30KV_FuncCodeR, AddrWSM30KVR01_OutputStatus1, 0x0011, data );
			BYTE bData[20];
			bData[0] = byteWSM30KV_Sn01;
			bData[1] = byteWSM30KV_FuncCodeR;
			bData[2] = 0x02;
			bData[3] = 0x00;
			bData[4] = 0x11;
			bData[5] = LRC( bData, 5 );
			sprintf( data, ":%02X%02X%02X%02X%02X%02X\r\n", bData[0], bData[1], bData[2], bData[3], bData[4], bData[5] );
			WSM30KV_ConvertReceiveData( strlen(data), data );
			bData[0] = byteWSM30KV_Sn25;
			bData[1] = byteWSM30KV_FuncCodeR;
			bData[2] = 0x02;
			bData[3] = 0x00;
			bData[4] = 0x02;
			bData[5] = LRC( bData, 5 );
			sprintf( data, ":%02X%02X%02X%02X%02X%02X\r\n", bData[0], bData[1], bData[2], bData[3], bData[4], bData[5] );
			WSM30KV_ConvertReceiveData( strlen(data), data );

			bData[0] = byteWSM30KV_Sn01;
			bData[1] = byteWSM30KV_FuncCodeR;
			bData[2] = 0x06;
			bData[3] = 0x00;
			bData[4] = 0x01;
			bData[5] = 0x00;
			bData[6] = 0x01;
			bData[7] = 0x00;
			bData[8] = 0x10;
			bData[9] = LRC( bData, 9 );
			sprintf( data, ":%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n", bData[0], bData[1], bData[2], bData[3], bData[4], bData[5], bData[6], bData[7], bData[8], bData[9] );
			WSM30KV_ConvertReceiveData( strlen(data), data );
			bData[0] = byteWSM30KV_Sn25;
			bData[1] = byteWSM30KV_FuncCodeR;
			bData[2] = 0x08;
			bData[3] = 0x00;
			bData[4] = 0x04;
			bData[5] = 0x00;
			bData[6] = 0x01;
			bData[7] = 0x00;
			bData[8] = 0x10;
			bData[9] = 0x00;
			bData[10]= 0x00;
			bData[11] = LRC( bData, 11 );
			sprintf( data, ":%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n", bData[0], bData[1], bData[2], bData[3], bData[4], bData[5], bData[6], bData[7], bData[8], bData[9], bData[10], bData[11] );
			WSM30KV_ConvertReceiveData( strlen(data), data );
		}
		break;
	}
}

void CCommWSM30KV::InitHWND( HWND hWnd )
{
	g_hWSM30KVMain = hWnd;
}

BOOL CCommWSM30KV::Init()
{
	// Test
/*	char data[100];
	sprintf( data, ":0105044AFF00AD\r\n" );
//	WSM30KV_ConvertReceiveData( 0x01, 0x044A, strlen(data), data );
// 	g_strWSM30KV	= "";
// 	g_strWSM30KVSaved	= "";
*/
	/////////////////////////////////////////////////////////////////
	ResetEvent( g_hWSM30KVEventWork );
	ResetEvent( g_hWSM30KVEventTeminate );

	g_hWSM30KVThread	= CreateThread( NULL, 0, WSM30KVCommThread, NULL, 0, &g_dwWSM30KVThread );
	if( g_hWSM30KVThread == NULL )
		return FALSE;

	CloseHandle(g_hWSM30KVThread);
	return TRUE;
}

BOOL CCommWSM30KV::Close()
{
	if( g_WSM30KVcfg.bReady )
	{
		SetEvent( g_hWSM30KVEventTeminate );
		WaitForSingleObject( g_hWSM30KVEventWork, INFINITE );
		g_WSM30KVcfg.bReady	= FALSE;
		g_WSM30KVcfg.nPort	= -1;
	}

	return TRUE;
}
