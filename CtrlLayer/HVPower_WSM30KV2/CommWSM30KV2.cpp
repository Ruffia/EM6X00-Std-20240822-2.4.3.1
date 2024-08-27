// CommWSM30KV2.cpp : implementation file
//

#include "stdafx.h"
#include "CommWSM30KV2.h"

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

#define	nWSM30KV2_Sleep			100

DWORD WINAPI WSM30KV2ThreadWR(LPVOID lpParam);		// 发送及接收线程
DWORD WINAPI WSM30KV2ThreadSend(LPVOID lpParam);	// 发送线程
DWORD WINAPI WSM30KV2ThreadRead(LPVOID lpParam);	// 接收线程
DWORD WINAPI WSM30KV2ThreadInquire(LPVOID lpParam);	// 查询线程
DWORD WINAPI WSM30KV2ThreadAnalyze(LPVOID lpParam);	// 分析线程（将数据包分析为数字量）
DWORD WINAPI WSM30KV2ThreadConvert(LPVOID lpParam);	// 转换线程（将数字量转换为能在界面上显示的量）
DWORD WINAPI WSM30KV2ThreadLog(LPVOID lpParam);		// 日志线程
const	short	g_nWSM30KV2ThreadCountsAll = 5;		// 读写、查询、分析、转换、日志

static BOOL g_bWSM30KV2EndThreads = FALSE;
CRITICAL_SECTION	csWSM30KV2Port, csWSM30KV2FifoHost, csWSM30KV2FifoDev;

const	short	WSM30KV2_HostFIFOSize	= 20;
const	short	WSM30KV2_DevFIFOSize	= 20;
CFIFO<WSM30KV_Send_Msg, WSM30KV_Send_Msg&>	g_WSM30KV2fifoHostMsg(WSM30KV2_HostFIFOSize);
CFIFO<WSM30KV_Recv_Msg, WSM30KV_Recv_Msg&>	g_WSM30KV2fifoDevMsg(WSM30KV2_DevFIFOSize);

HANDLE	g_hWSM30KV2Thread;
DWORD	g_dwWSM30KV2Thread;
DWORD	WINAPI WSM30KV2CommThread(LPVOID lpParam);
HANDLE	g_hWSM30KV2EventWork		= CreateEvent(NULL,FALSE, FALSE, _T("WORKwsm30kv2"));
HANDLE	g_hWSM30KV2EventTeminate	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEwsm30kv2"));

HWND	g_hWSM30KV2Main;
CSerial	g_WSM30KV2;
char	g_cWSM30KV2Saved[512];
short	g_nWSM30KV2SavedCounts, g_nWSM30KV2ThreadCounts;

Cfg_WSM30KV			g_WSM30KV2cfg;
Status_WSM30KV2		g_WSM30KV2status;
Mon_WSM30KV2		g_WSM30KV2mon;
Setting_WSM30KV2	g_WSM30KV2set;
WORD	g_wWSM30KV2Reg[nWSM30KV2Count_Registers];

HANDLE	g_hWSM30KV2File	= NULL;
int		g_nWSM30KV2Loops;

/////////////////////////////
float	g_fWSM30KV2AutoStep_ACL1	= (float)500.0;		// V;
float	g_fWSM30KV2AutoStep_ACL2	= (float)1000.0;	// V;
float	g_fWSM30KV2AutoRated_ACL1	= (float)10000.0;	// V;
float	g_fWSM30KV2AutoRated_ACL2	= (float)30000.0;	// V;
float	g_fWSM30KV2AutoStep_EMS		= (float)10;
float	g_fWSM30KV2AutoRated_EMS	= (float)30;	// 100;
float	g_fWSM30KV2AutoStep_FIL		= (float)0.1;	// 0.001;// 100mA
float	g_fWSM30KV2AutoRated_FIL	= (float)1.9;	// A
WSM30KV2_AutoState	g_WSM30KV2AutoState = WSM30KV2AS_NULL;
DWORD	g_WSM30KV2lastAuto;
/////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CCommWSM30KV2

CCommWSM30KV2::CCommWSM30KV2()
{
	g_hWSM30KV2Main = NULL;
	ZeroMemory( &g_WSM30KV2cfg, sizeof(Cfg_WSM30KV) );
	ZeroMemory( &g_WSM30KV2mon, sizeof(Mon_WSM30KV2) );
	ZeroMemory( &g_WSM30KV2set, sizeof(Setting_WSM30KV2) );
	g_WSM30KV2status.statusO.wStatus	= 0xFFFF;
	g_WSM30KV2status.statusB.wStatus	= 0xFFFF;
	g_WSM30KV2status.statusV.wStatus	= 0xFFFF;
	g_WSM30KV2status.statusI.wStatus	= 0xFFFF;
	g_WSM30KV2status.statusA.wStatus	= 0xFFFF;
	g_WSM30KV2status.status1.wStatus	= 0xFFFF;
	g_WSM30KV2status.status2.wStatus	= 0xFFFF;
	g_WSM30KV2status.status3.wStatus	= 0xFFFF;
}

CCommWSM30KV2::~CCommWSM30KV2()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCommWSM30KV2 message handlers

void CreateLog()
{
	if( g_hWSM30KV2File != NULL )
	{
		CloseHandle( g_hWSM30KV2File );
		g_hWSM30KV2File = NULL;
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
	sprintf( Buf, "%s\\Log\\WSM30KV2-%s.csv", exeFullPath, csCurrentTime );
	g_hWSM30KV2File = CreateFile(Buf, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	g_nWSM30KV2Loops = 1;

	sprintf( Buf, "%s\r\n", "WSM30KV2" );
	WriteFile( g_hWSM30KV2File, Buf, strlen(Buf), &cbRet, NULL );
// 	sprintf( Buf, "DateTime,%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
// 		"sOutput,", "sProtectB,", "sProtectV,", "sProtectI,", "sProtectArc,", "status1,status2,status3,",
// 		"SCI set,SCI out,SCI mon v,SCI mon i,", "COL set,COL out,COL mon v,COL mon i,", "PMT set,PMT out,PMT mon v,PMT mon i,",
// 		"ACL V set,ACL V out,ACL V mon v,", "ACL I set,ACL I out,ACL I mon i,", "FIL set,FIL out,FIL mon i,", "BIAS set,BIAS out,BIAS mon v,"
// 		"+3V3,+5V,+15V,-15V,+24V,T");
	CString str = "DateTime,";
			str += "sOutput, sProtectB, sProtectV, sProtectI, sProtectArc,";
			str += "status1,status2,status3,";
			str += "SCI set,SCI out,SCI mon v,SCI mon i,";
			str += "COL set,COL out,COL mon v,COL mon i,";
			str += "PMT set,PMT out,PMT mon v,PMT mon i,";
			str += "ACL V set,ACL V out,ACL V mon v,";
			str += "ACL I set,ACL I out,ACL I mon i,";
			str += "FIL set,FIL out,FIL mon i,";
			str += "BIAS set,BIAS out,BIAS mon v,";
			str += "+3V3,+5V,+15V,-15V,+24V,T";
	sprintf( Buf, "%s\r\n", str );
	WriteFile( g_hWSM30KV2File, Buf, strlen(Buf), &cbRet, NULL );
}

void Record( LPCTSTR szRcd )
{
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%Y%m%d %H:%M:%S");

	unsigned long cbRet;
	char Buf[1024];
	sprintf(Buf, "[%s],%s\r\n", csCurrentTime, szRcd );
	WriteFile(g_hWSM30KV2File, Buf, strlen(Buf), &cbRet, NULL);

	g_nWSM30KV2Loops++;
	if( g_nWSM30KV2Loops > 10000 )
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
	strncpy( cLRC, data +nLen -2, 2 );
	cLRC[2] = '\0';

//	sscanf( cLRC, "%02X", &bLRC );

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
	if( bBuf[0] == byteWSM30KV2_Sn )
	{
		// power#01通道
		if( bBuf[2] == nWSM30KV2Count_Registers *2 )
		{
			for( int i=0; i<nWSM30KV2Count_Registers; i++ )
				g_wWSM30KV2Reg[i] = ( bBuf[i*2 +3] << 8 ) | bBuf[i*2 +4];
			if( g_wWSM30KV2Reg[AddrWSM30KV2R_OutputStatus1 -byteWSM30KV2_Offset] != g_WSM30KV2status.statusO.wStatus )
			{
				g_WSM30KV2status.statusO.wStatus = g_wWSM30KV2Reg[AddrWSM30KV2R_OutputStatus1 -byteWSM30KV2_Offset];
				PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KV2R_OutputStatus1, (LPARAM)(&(g_WSM30KV2status.statusO)) );
			}
			if( g_wWSM30KV2Reg[AddrWSM30KV2R_ProtectStatusB -byteWSM30KV2_Offset] != g_WSM30KV2status.statusB.wStatus )
			{
				g_WSM30KV2status.statusB.wStatus = g_wWSM30KV2Reg[AddrWSM30KV2R_ProtectStatusB -byteWSM30KV2_Offset];
				PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KV2R_ProtectStatusB, (LPARAM)(&(g_WSM30KV2status.statusB)) );
			}
			if( g_wWSM30KV2Reg[AddrWSM30KV2R_ProtectStatusV -byteWSM30KV2_Offset] != g_WSM30KV2status.statusV.wStatus )
			{
				g_WSM30KV2status.statusV.wStatus = g_wWSM30KV2Reg[AddrWSM30KV2R_ProtectStatusV -byteWSM30KV2_Offset];
				PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KV2R_ProtectStatusV, (LPARAM)(&(g_WSM30KV2status.statusV)) );
			}
			if( g_wWSM30KV2Reg[AddrWSM30KV2R_ProtectStatusI -byteWSM30KV2_Offset] != g_WSM30KV2status.statusI.wStatus )
			{
				g_WSM30KV2status.statusI.wStatus = g_wWSM30KV2Reg[AddrWSM30KV2R_ProtectStatusI -byteWSM30KV2_Offset];
				PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KV2R_ProtectStatusI, (LPARAM)(&(g_WSM30KV2status.statusI)) );
			}
			if( g_wWSM30KV2Reg[AddrWSM30KV2R_ProtectStatusA -byteWSM30KV2_Offset] != g_WSM30KV2status.statusA.wStatus )
			{
				g_WSM30KV2status.statusA.wStatus = g_wWSM30KV2Reg[AddrWSM30KV2R_ProtectStatusA -byteWSM30KV2_Offset];
				PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KV2R_ProtectStatusA, (LPARAM)(&(g_WSM30KV2status.statusA)) );
			}
			if( g_wWSM30KV2Reg[AddrWSM30KV2R_InquireStatus1 -byteWSM30KV2_Offset] != g_WSM30KV2status.status1.wStatus )
			{
				g_WSM30KV2status.status1.wStatus = g_wWSM30KV2Reg[AddrWSM30KV2R_InquireStatus1 -byteWSM30KV2_Offset];
				PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KV2R_InquireStatus1, (LPARAM)(&(g_WSM30KV2status.status1)) );
			}
			if( g_wWSM30KV2Reg[AddrWSM30KV2R_InquireStatus2 -byteWSM30KV2_Offset] != g_WSM30KV2status.status2.wStatus )
			{
				g_WSM30KV2status.status2.wStatus = g_wWSM30KV2Reg[AddrWSM30KV2R_InquireStatus2 -byteWSM30KV2_Offset];
				PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KV2R_InquireStatus2, (LPARAM)(&(g_WSM30KV2status.status2)) );
			}
			if( g_wWSM30KV2Reg[AddrWSM30KV2R_InquireStatus3 -byteWSM30KV2_Offset] != g_WSM30KV2status.status3.wStatus )
			{
				g_WSM30KV2status.status3.wStatus = g_wWSM30KV2Reg[AddrWSM30KV2R_InquireStatus3 -byteWSM30KV2_Offset];
				PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_UpdateStatus, AddrWSM30KV2R_InquireStatus3, (LPARAM)(&(g_WSM30KV2status.status3)) );
			}
			g_WSM30KV2mon.monVT.fP3V3	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_P3V3 -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *3.3);
			g_WSM30KV2mon.monVT.fP5V	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_P5V -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *5.0);
			g_WSM30KV2mon.monVT.fP15V	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_P15V -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *15.0);
			g_WSM30KV2mon.monVT.fN15V	= (float)(-1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_N15V -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *15.0);
			g_WSM30KV2mon.monVT.fP24V	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_P24V -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *24.0);
			g_WSM30KV2mon.monVT.fTemp	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_Temp -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *250);

			g_WSM30KV2mon.monFB.fBIASv	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_BIAS_V -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *g_fWSM30KV2FS_BIAS_mon_V);
			g_WSM30KV2mon.monFB.fFILi	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_FIL_I -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *g_fWSM30KV2FS_FIL_mon_I);
			g_WSM30KV2mon.monFB.fACLv	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_ACL_V -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *g_fWSM30KV2FS_ACL_mon_V);
			g_WSM30KV2mon.monFB.fACLi	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_ACL_I -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *g_fWSM30KV2FS_ACL_mon_I);
			g_WSM30KV2mon.monFB.fSCIv	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_SCI_V -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *g_fWSM30KV2FS_SCI_mon_V);
			g_WSM30KV2mon.monFB.fSCIi	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_SCI_I -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *g_fWSM30KV2FS_SCI_mon_I);
			g_WSM30KV2mon.monFB.fPMTv	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_PMT_V -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *g_fWSM30KV2FS_PMT_mon_V);
			g_WSM30KV2mon.monFB.fPMTi	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_PMT_I -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *g_fWSM30KV2FS_PMT_mon_I);
			g_WSM30KV2mon.monFB.fCOLv	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_COL_V -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *g_fWSM30KV2FS_COL_mon_V);// -g_fWSM30KV2FS_COL_mon_V/2);
			g_WSM30KV2mon.monFB.fCOLi	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_FB_COL_I -byteWSM30KV2_Offset] / g_wWSM30KV2FS_1 *g_fWSM30KV2FS_COL_mon_I);

			g_WSM30KV2mon.monOP.fBIASv	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_Given_BIAS_V -byteWSM30KV2_Offset] / g_wWSM30KV2FS_2 *g_fWSM30KV2FS_BIAS_mon_V);
			g_WSM30KV2mon.monOP.fFILi	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_Given_FIL_I -byteWSM30KV2_Offset] / g_wWSM30KV2FS_2 *g_fWSM30KV2FS_FIL_mon_I);
			g_WSM30KV2mon.monOP.fACLv	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_Given_ACL_V -byteWSM30KV2_Offset] / g_wWSM30KV2FS_2 *g_fWSM30KV2FS_ACL_mon_V);
			g_WSM30KV2mon.monOP.fSCIv	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_Given_SCI_V -byteWSM30KV2_Offset] / g_wWSM30KV2FS_2 *g_fWSM30KV2FS_SCI_mon_V);
			g_WSM30KV2mon.monOP.fACLi	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_Given_ACL_I -byteWSM30KV2_Offset] / g_wWSM30KV2FS_2 *g_fWSM30KV2FS_ACL_mon_I);
			g_WSM30KV2mon.monOP.fCOLv	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_Given_COL_V -byteWSM30KV2_Offset] / g_wWSM30KV2FS_2 *g_fWSM30KV2FS_COL_mon_V);// -g_fWSM30KV2FS_COL_mon_V/2);
			g_WSM30KV2mon.monOP.fPMTv	= (float)(1.0 *g_wWSM30KV2Reg[AddrWSM30KV2R_Given_PMT_V -byteWSM30KV2_Offset] / g_wWSM30KV2FS_2 *g_fWSM30KV2FS_PMT_mon_V);
		}
	}
	PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_UpdateMon, AddrWSM30KV2R_FB_P3V3, (LPARAM)(&(g_WSM30KV2mon.monVT)) );
	PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_UpdateMon, AddrWSM30KV2R_FB_BIAS_V, (LPARAM)(&(g_WSM30KV2mon.monFB)) );
	PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_UpdateMon, AddrWSM30KV2R_Given_BIAS_V, (LPARAM)(&(g_WSM30KV2mon.monOP)) );
}

void AnalyzeData_WriteReg( BYTE* bBuf )
{
	// 设置参数值
	if( bBuf[0] == byteWSM30KV2_Sn )
	{
		// power#01通道
	}
}

void AnalyzeData_WriteCoil( BYTE* bBuf )
{
	// 设置使能、复位信号
	if( bBuf[0] == byteWSM30KV2_Sn )
	{
		// power#01通道
	}
}

void AnalyzeData_ShieldReg( BYTE* bBuf )
{
	if( bBuf[0] == byteWSM30KV2_Sn )
	{
		// power#01通道
	}
}

void WSM30KV2_ConvertReceiveData( int nLenRecv, char* data )
{
	if( nLenRecv < 1 )
		return;

	char cRemain[512], cBuf[512];
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
				case	byteWSM30KV2_FuncCodeR:
					AnalyzeData_ReadReg(bBuf);
					break;
				case	byteWSM30KV2_FuncCodeWR:
					AnalyzeData_WriteReg(bBuf);
					break;
				case	byteWSM30KV2_FuncCodeWC:
					AnalyzeData_WriteCoil(bBuf);
					break;
				case	byteWSM30KV2_FuncCodeS:
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

	g_nWSM30KV2SavedCounts = nLenRecv;
	if( nLenRecv > 0 )
	{
		strcpy( g_cWSM30KV2Saved, data );
	}
}

void WSM30KV2_ConvertSendData( BYTE bAddr, BYTE bFunc, WORD wAddr, WORD wValue, char* data )
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

int WSM30KV2_Send( int nSleep, BYTE bAddr, BYTE bFunc, WORD wAddr, WORD wValue )
{
	if( !g_WSM30KV2cfg.bReady )
		return -1;
	WSM30KV_Send_Msg msg;
	WSM30KV2_ConvertSendData( bAddr, bFunc, wAddr, wValue, msg.cData );
	msg.nLength = strlen(msg.cData);
	msg.cData[msg.nLength] = '\0';
	if( !g_WSM30KV2fifoHostMsg.IsFIFOFull() )
	{
		if( bFunc != byteWSM30KV2_FuncCodeR )
			g_WSM30KV2fifoHostMsg.Input(msg);
		else
		{
			if( g_WSM30KV2fifoHostMsg.GetFIFOCount() < WSM30KV2_DevFIFOSize -2 )
				g_WSM30KV2fifoHostMsg.Input(msg);
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
	if( !g_WSM30KV2cfg.bReady )
		return FALSE;
	g_WSM30KV2set.fACLv = (float)(1.0 *nValue);
	if( g_WSM30KV2set.fACLv > g_fWSM30KV2FS_ACL_mon_V )
		g_WSM30KV2set.fACLv = g_fWSM30KV2FS_ACL_mon_V;
	if( g_WSM30KV2set.fACLv < 0.0 )
		g_WSM30KV2set.fACLv = 0.0;
	// Analog --> Digital
	WORD wValue = (WORD)(g_WSM30KV2set.fACLv /g_fWSM30KV2FS_ACL_mon_V *g_wWSM30KV2FS_2);
	if( WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeWR, AddrWSM30KV2R_Given_ACL_V, wValue ) > 0 )
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
	if( !g_WSM30KV2cfg.bReady )
		return FALSE;
	g_WSM30KV2set.fACLi = (float)(1.0 *nValue);
	if( g_WSM30KV2set.fACLi > g_fWSM30KV2FS_ACL_mon_I )
		g_WSM30KV2set.fACLi = g_fWSM30KV2FS_ACL_mon_I;
	if( g_WSM30KV2set.fACLi < 0.0 )
		g_WSM30KV2set.fACLi = 0.0;
	// Analog --> Digital
	WORD wValue = (WORD)(g_WSM30KV2set.fACLi /g_fWSM30KV2FS_ACL_mon_I *g_wWSM30KV2FS_2);
	if( WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeWR, AddrWSM30KV2R_Given_ACL_I, wValue ) > 0 )
		return TRUE;
	else
		return FALSE;
}

BOOL Change_FIL( int nValue, BOOL bFromMain )
{
	if( !g_WSM30KV2cfg.bReady )
		return FALSE;
	g_WSM30KV2set.fFILi = (float)(nValue /1000.0);		// mA --> A
	if( g_WSM30KV2set.fFILi > g_fWSM30KV2FS_FIL_mon_I )
		g_WSM30KV2set.fFILi = g_fWSM30KV2FS_FIL_mon_I;
	if( g_WSM30KV2set.fFILi < 0.0 )
		g_WSM30KV2set.fFILi = 0.0;
	// Analog --> Digital
	WORD wValue = (WORD)(g_WSM30KV2set.fFILi /g_fWSM30KV2FS_FIL_mon_I *g_wWSM30KV2FS_2);
	if( WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeWR, AddrWSM30KV2R_Given_FIL_I, wValue ) > 0 )
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
	if( !g_WSM30KV2cfg.bReady )
		return FALSE;
	g_WSM30KV2set.fPMTv = (float)(1.0 *nValue);
	if( g_WSM30KV2set.fPMTv > g_fWSM30KV2FS_PMT_mon_V )
		g_WSM30KV2set.fPMTv = g_fWSM30KV2FS_PMT_mon_V;
	if( g_WSM30KV2set.fPMTv < 0.0 )
		g_WSM30KV2set.fPMTv = 0.0;
	// Analog --> Digital
	WORD wValue = (WORD)(g_WSM30KV2set.fPMTv /g_fWSM30KV2FS_PMT_mon_V *g_wWSM30KV2FS_2);
	if( WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeWR, AddrWSM30KV2R_Given_PMT_V, wValue ) > 0 )
		return TRUE;
	else
		return FALSE;
}

BOOL Change_SCI( int nValue, BOOL bFromMain )
{
	if( !g_WSM30KV2cfg.bReady )
		return FALSE;
	WORD wValue = 0x0;
	if( nValue > 0 )
		wValue = g_wWSM30KV2FS_2;
	g_WSM30KV2set.fSCIv = g_fWSM30KV2FS_SCI_mon_V;
//	g_WSM30KV2set.fSCIv = (float)(wValue *g_fWSM30KV2FS_SCI_mon_V /g_wWSM30KV2FS_2);
	if( WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeWR, AddrWSM30KV2R_Given_SCI_V, wValue ) > 0 )
		return TRUE;
	else
		return FALSE;
}

BOOL Change_COL( int nValue, BOOL bFromMain )
{
	if( !g_WSM30KV2cfg.bReady )
		return FALSE;
	WORD wValue = 0x0;
	if( nValue > 0 )
		wValue = (WORD)(300.0 /g_fWSM30KV2FS_COL_mon_V *g_wWSM30KV2FS_2);
	g_WSM30KV2set.fCOLv = 300.0f;
	if( WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeWR, AddrWSM30KV2R_Given_COL_V, wValue ) > 0 )
		return TRUE;
	else
		return FALSE;
}

BOOL Change_BIAS( int nValue, BOOL bFromMain )
{
	if( !g_WSM30KV2cfg.bReady )
		return FALSE;
	g_WSM30KV2set.fBIASv = (float)(1.0 *nValue);
	if( g_WSM30KV2set.fBIASv > g_fWSM30KV2FS_BIAS_mon_V )
		g_WSM30KV2set.fBIASv = g_fWSM30KV2FS_BIAS_mon_V;
	if( g_WSM30KV2set.fBIASv < 0.0 )
		g_WSM30KV2set.fBIASv = 0.0;
	// Analog --> Digital
	WORD wValue = (WORD)(g_WSM30KV2set.fBIASv /g_fWSM30KV2FS_BIAS_mon_V *g_wWSM30KV2FS_2);
	if( WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeWR, AddrWSM30KV2R_Given_BIAS_V, wValue ) > 0 )
		return TRUE;
	else
		return FALSE;
}

BOOL setenable( int nIndex, int nCheck )
{
	if( !g_WSM30KV2cfg.bReady )
		return FALSE;
	WORD wValue = 0x0000;
	if( nCheck > 0 )
		wValue = 0xFF00;

	int nStatus = 0;
	if( nIndex < 18 )
	{
		BYTE bAddr = byteWSM30KV2_Sn;
		WORD wAddr = 0x0000;
		switch( nIndex )
		{
		case	11:	// BIAS
			wAddr	= AddrWSM30KV2C_ReqOn_Power1;
			nStatus	= g_WSM30KV2status.statusO.status.BIAS;
			break;
		case	12:	// FIL
			wAddr	= AddrWSM30KV2C_ReqOn_Power2;
			nStatus	= g_WSM30KV2status.statusO.status.FIL;
			break;
		case	13:	// ACL
			wAddr	= AddrWSM30KV2C_ReqOn_Power3;
			nStatus	= g_WSM30KV2status.statusO.status.ACL;
			break;
		case	14:	// SCI
			wAddr	= AddrWSM30KV2C_ReqOn_Power4;
			nStatus	= g_WSM30KV2status.statusO.status.SCI;
			break;
		case	15:	// EMS
			wAddr	= AddrWSM30KV2C_ReqOn_Power5;
			nStatus	= g_WSM30KV2status.statusO.status.EMS;
			break;
		case	16:	// COL
			wAddr	= AddrWSM30KV2C_ReqOn_Power6;
			nStatus	= g_WSM30KV2status.statusO.status.COL;
			break;
		case	17:	// PMT
			wAddr	= AddrWSM30KV2C_ReqOn_Power7;
			nStatus	= g_WSM30KV2status.statusO.status.PMT;
			break;
		}
		if( nStatus != nCheck )
		{
			if( WSM30KV2_Send( nWSM30KV2_Sleep, bAddr, byteWSM30KV2_FuncCodeWC, wAddr, wValue ) > 0 )
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
		if( WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_SnBroadcast, byteWSM30KV2_FuncCodeWR, AddrWSM30KV2R_InquireStatus1, wValue ) > 0 )
			return TRUE;
		else
			return FALSE;
	}
}

BOOL setpause( int nIndex, int nCheck )
{
	if( !g_WSM30KV2cfg.bReady )
		return FALSE;
	WORD wValue = 0x0000;
	if( nCheck > 0 )
		wValue = 0xFF00;

	if( nIndex < 28 )
	{
		BYTE bAddr = byteWSM30KV2_Sn;
		WORD wAddr = 0x0000;
		switch( nIndex )
		{
		case	21:	// BIAS
			wAddr	= AddrWSM30KV2C_ReqPause_Power1;
			break;
		case	22:	// FIL
			wAddr	= AddrWSM30KV2C_ReqPause_Power2;
			break;
		case	23:	// ACL
			wAddr	= AddrWSM30KV2C_ReqPause_Power3;
			break;
		case	24:	// SCI
			wAddr	= AddrWSM30KV2C_ReqPause_Power4;
			break;
		case	25:	// EMS
			wAddr	= AddrWSM30KV2C_ReqPause_Power5;
			break;
		case	26:	// COL
			wAddr	= AddrWSM30KV2C_ReqPause_Power6;
			break;
		case	27:	// PMT
			wAddr	= AddrWSM30KV2C_ReqPause_Power7;
			break;
		}
		if( WSM30KV2_Send( nWSM30KV2_Sleep, bAddr, byteWSM30KV2_FuncCodeWC, wAddr, wValue ) > 0 )
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if( WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeWC, AddrWSM30KV2C_ReqPause_Power1, wValue )
			&& WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeWC, AddrWSM30KV2C_ReqPause_Power2, wValue )
			&& WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeWC, AddrWSM30KV2C_ReqPause_Power3, wValue )
			&& WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeWC, AddrWSM30KV2C_ReqPause_Power4, wValue )
			&& WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeWC, AddrWSM30KV2C_ReqPause_Power5, wValue )
			&& WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeWC, AddrWSM30KV2C_ReqPause_Power6, wValue )
			&& WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeWC, AddrWSM30KV2C_ReqPause_Power7, wValue ) )
			return TRUE;
		else
			return FALSE;
	}
}

BOOL reset( int nIndex, int nCheck )
{
	if( !g_WSM30KV2cfg.bReady )
		return FALSE;
	WORD wValue = 0x0000;
	if( nCheck > 0 )
		wValue = 0xFF00;

	if( nIndex < 38 )
	{
		BYTE bAddr = byteWSM30KV2_Sn;
		WORD wAddr = 0x0000;
		switch( nIndex )
		{
		case	31:	// BIAS
			wAddr	= AddrWSM30KV2C_ReqReset_Power1;
			break;
		case	32:	// FIL
			wAddr	= AddrWSM30KV2C_ReqReset_Power2;
			break;
		case	33:	// ACL
			wAddr	= AddrWSM30KV2C_ReqReset_Power3;
			break;
		case	34:	// SCI
			wAddr	= AddrWSM30KV2C_ReqReset_Power4;
			break;
		case	35:	// EMS
			wAddr	= AddrWSM30KV2C_ReqReset_Power5;
			break;
		case	36:	// COL
			wAddr	= AddrWSM30KV2C_ReqReset_Power6;
			break;
		case	37:	// PMT
			wAddr	= AddrWSM30KV2C_ReqReset_Power7;
			break;
		}
		if( WSM30KV2_Send( nWSM30KV2_Sleep, bAddr, byteWSM30KV2_FuncCodeWC, wAddr, wValue ) > 0 )
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		// 广播
		if( wValue != 0 )
			wValue = 0x00FF;
		if( WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_SnBroadcast, byteWSM30KV2_FuncCodeWR, AddrWSM30KV2R_InquireStatus3, wValue ) > 0 )
			return TRUE;
		else
			return FALSE;
	}
}

BOOL WSM30KV2CommTest( int nSleep )
{
	BOOL bRet = FALSE;
	char buf[255];
	WSM30KV2_ConvertSendData( byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeR, AddrWSM30KV2R_OutputStatus1 -1, nWSM30KV2Count_Registers, buf );
	g_WSM30KV2.SendDataEx2( buf, strlen(buf) );
	Sleep(nSleep);

	int nTimes = 0;
	g_nWSM30KV2SavedCounts = 0;
	char data[1024];
	WSM30KV_Recv_Msg msgRead;
	do{
		msgRead.nLength = g_WSM30KV2.ReadData( msgRead.cData, 512 );
		msgRead.cData[msgRead.nLength] = '\0';
		if( g_nWSM30KV2SavedCounts > 0 )	// 拼数据
		{
			strcpy( data, g_cWSM30KV2Saved );
			strcat( data +g_nWSM30KV2SavedCounts, msgRead.cData );
		}
		else
			strcpy( data, msgRead.cData );
		WSM30KV2_ConvertReceiveData( strlen(data), data );
		if( g_WSM30KV2mon.monVT.fP3V3 > 3 )
			break;
		nTimes++;
		Sleep(nSleep);
	} while( nTimes < 50 );
	if( nTimes < 50 )
	{
		bRet = TRUE;
		g_nWSM30KV2SavedCounts = 0;
	}
	return bRet;
}

DWORD WINAPI WSM30KV2ThreadInquire(LPVOID lpParam)
{
	while (TRUE)
	{
		if( g_bWSM30KV2EndThreads )
		{
			break;
		}
//		EnterCriticalSection(&csFifoHost);
//		LeaveCriticalSection(&csFifoHost);
		WSM30KV2_Send( nWSM30KV2_Sleep, byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeR, AddrWSM30KV2R_OutputStatus1 -1, nWSM30KV2Count_Registers );
		Sleep(500);
	}
	g_nWSM30KV2ThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadInquire, counts = %d\n", g_nWSM30KV2ThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}
 
DWORD WINAPI WSM30KV2ThreadWR(LPVOID lpParam)
{
	while (TRUE)
	{
		if( g_bWSM30KV2EndThreads )
		{
//			LeaveCriticalSection(&csWSM30KV2Port);
// 			LeaveCriticalSection(&csFifoHost);
			break;
		}
		
		EnterCriticalSection(&csWSM30KV2Port);
//		EnterCriticalSection(&csFifoHost);
		
		if( !g_WSM30KV2fifoHostMsg.IsFIFOEmpty() )
		{
			// 发送数据
			WSM30KV_Send_Msg msgSend;
			g_WSM30KV2fifoHostMsg.Output(msgSend);
			// 读取应答
			BOOL bTimeout = FALSE;
			WSM30KV_Recv_Msg msgRead;
			int nSend = g_WSM30KV2.SendDataEx2(msgSend.cData, msgSend.nLength);
			if( nSend < 1 )
				continue;
			Sleep(nWSM30KV2_Sleep);
			DWORD dwEndTime, dwStartTime = GetTickCount();
			do
			{
				dwEndTime = GetTickCount();
				if( dwEndTime -dwStartTime > 1000 )
					bTimeout = TRUE;
				if( bTimeout )
					break;
				msgRead.cData[0] = '\0';
				msgRead.nLength = g_WSM30KV2.ReadData( msgRead.cData, WSM30KV_ReadFromDevice_DataSize );
//				EnterCriticalSection(&csWSM30KV2FifoDev);
				if( msgRead.nLength > 0 && !g_WSM30KV2fifoDevMsg.IsFIFOFull() )
				{
					msgRead.cData[msgRead.nLength] = '\0';
					int len = strlen(msgRead.cData);
					g_WSM30KV2fifoDevMsg.Input(msgRead);
					break;
				}
			} while( msgRead.nLength < 1 );
		}
		
		LeaveCriticalSection(&csWSM30KV2Port);
//		LeaveCriticalSection(&csFifoHost);
//		LeaveCriticalSection(&csWSM30KV2FifoDev);
		
		Sleep(10);
	}
	while( g_nWSM30KV2ThreadCounts > g_nWSM30KV2ThreadCountsAll -1 )
		Sleep(1);
	g_nWSM30KV2ThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadSend, counts = %d\n", g_nWSM30KV2ThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

DWORD WINAPI WSM30KV2ThreadSend(LPVOID lpParam)
{
	while (TRUE)
	{
		if( g_bWSM30KV2EndThreads )
		{
//			LeaveCriticalSection(&csWSM30KV2Port);
// 			LeaveCriticalSection(&csFifoHost);
			break;
		}
		
		EnterCriticalSection(&csWSM30KV2Port);
//		EnterCriticalSection(&csFifoHost);
		
		if( !g_WSM30KV2fifoHostMsg.IsFIFOEmpty() )
		{
			WSM30KV_Send_Msg msg;
			g_WSM30KV2fifoHostMsg.Output(msg);
			g_WSM30KV2.SendDataEx2(msg.cData, msg.nLength);
		}
		
		LeaveCriticalSection(&csWSM30KV2Port);
//		LeaveCriticalSection(&csFifoHost);
		
		Sleep(10);
	}
	while( g_nWSM30KV2ThreadCounts > g_nWSM30KV2ThreadCountsAll )
		Sleep(1);
	g_nWSM30KV2ThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadSend, counts = %d\n", g_nWSM30KV2ThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

DWORD WINAPI WSM30KV2ThreadRead(LPVOID lpParam)
{
	char* sFlag = (char*)(lpParam);
	while (TRUE)
	{
		if( g_bWSM30KV2EndThreads )
		{
//			LeaveCriticalSection(&csWSM30KV2Port);
// 			LeaveCriticalSection(&csFifoHost);
			break;
		}
		
		EnterCriticalSection(&csWSM30KV2Port);
//		EnterCriticalSection(&csFifoDev);
		
		WSM30KV_Recv_Msg msg;
//		msg.cData[0] = '\0';
		msg.nLength = g_WSM30KV2.ReadData( msg.cData, WSM30KV_ReadFromDevice_DataSize );
		if( msg.nLength > 0 && !g_WSM30KV2fifoDevMsg.IsFIFOFull() )
		{
			msg.cData[msg.nLength] = '\0';
			g_WSM30KV2fifoDevMsg.Input(msg);
		}

		LeaveCriticalSection(&csWSM30KV2Port);
//		LeaveCriticalSection(&csFifoDev);
		
		Sleep(400);
	}
	while( g_nWSM30KV2ThreadCounts > g_nWSM30KV2ThreadCountsAll -1 )
		Sleep(1);
	g_nWSM30KV2ThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadRead, counts = %d\n", g_nWSM30KV2ThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

DWORD WINAPI WSM30KV2ThreadAnalyze(LPVOID lpParam)
{
//	char* sFlag = (char*)(lpParam);
//	CTestSpellmanRSDlg* pDlg = (CTestSpellmanRSDlg*)(lpParam);
	while (TRUE)
	{
		if( g_bWSM30KV2EndThreads )
		{
// 			LeaveCriticalSection(&csPort);
// 			LeaveCriticalSection(&csFifoHost);
			break;
		}
		
		EnterCriticalSection(&csWSM30KV2FifoDev);
		if( !g_WSM30KV2fifoDevMsg.IsFIFOEmpty() )
		{
			WSM30KV_Recv_Msg msg;
			g_WSM30KV2fifoDevMsg.Output(msg);
			char data[1024] = {0};
			if( g_nWSM30KV2SavedCounts > 0 )	// 拼数据
			{
				strcpy( data, g_cWSM30KV2Saved );
				strcat( data +g_nWSM30KV2SavedCounts, msg.cData );
			}
			else
				strcpy( data, msg.cData );
			WSM30KV2_ConvertReceiveData( strlen(data), data );
		}
		LeaveCriticalSection(&csWSM30KV2FifoDev);
		
		Sleep(200);
	}
	while( g_nWSM30KV2ThreadCounts > g_nWSM30KV2ThreadCountsAll -2 )
		Sleep(1);
	g_nWSM30KV2ThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadAnalyze, counts = %d\n", g_nWSM30KV2ThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

DWORD WINAPI WSM30KV2ThreadConvert(LPVOID lpParam)
{
	while (TRUE)
	{
		if( g_bWSM30KV2EndThreads )
		{
			break;
		}
		Sleep(nWSM30KV2_Sleep);
	}
	while( g_nWSM30KV2ThreadCounts > g_nWSM30KV2ThreadCountsAll -3 )
		Sleep(1);
	g_nWSM30KV2ThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadConvert, counts = %d\n", g_nWSM30KV2ThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

DWORD WINAPI WSM30KV2ThreadLog(LPVOID lpParam)
{
	while (TRUE)
	{
		if( g_bWSM30KV2EndThreads )
		{
			break;
		}
		char szBuf[1024], szBufStatus[255], szBufValues[255], szBufVT[255];
		sprintf( szBufStatus, "%04X,%04X,%04X,%04X,%04X,%04X,%04X,%04X,", 
			g_WSM30KV2status.statusO.wStatus,
			g_WSM30KV2status.statusB.wStatus,
			g_WSM30KV2status.statusV.wStatus,
			g_WSM30KV2status.statusI.wStatus,
			g_WSM30KV2status.statusA.wStatus,
			g_WSM30KV2status.status1.wStatus,
			g_WSM30KV2status.status2.wStatus,
			g_WSM30KV2status.status3.wStatus );
		sprintf( szBufValues, "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,",
			g_WSM30KV2set.fSCIv, g_WSM30KV2mon.monOP.fSCIv, g_WSM30KV2mon.monFB.fSCIv, g_WSM30KV2mon.monFB.fSCIi,
			g_WSM30KV2set.fCOLv, g_WSM30KV2mon.monOP.fCOLv, g_WSM30KV2mon.monFB.fCOLv, g_WSM30KV2mon.monFB.fCOLi,
			g_WSM30KV2set.fPMTv, g_WSM30KV2mon.monOP.fPMTv, g_WSM30KV2mon.monFB.fPMTv, g_WSM30KV2mon.monFB.fPMTi,
			g_WSM30KV2set.fACLv, g_WSM30KV2mon.monOP.fACLv, g_WSM30KV2mon.monFB.fACLv,
			g_WSM30KV2set.fACLi, g_WSM30KV2mon.monOP.fACLi, g_WSM30KV2mon.monFB.fACLi,
			g_WSM30KV2set.fFILi, g_WSM30KV2mon.monOP.fFILi, g_WSM30KV2mon.monFB.fFILi,
			g_WSM30KV2set.fBIASv, g_WSM30KV2mon.monOP.fBIASv, g_WSM30KV2mon.monFB.fBIASv );
		sprintf( szBufVT, "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f",
			g_WSM30KV2mon.monVT.fP3V3,
			g_WSM30KV2mon.monVT.fP5V,
			g_WSM30KV2mon.monVT.fP15V,
			g_WSM30KV2mon.monVT.fN15V,
			g_WSM30KV2mon.monVT.fP24V,
			g_WSM30KV2mon.monVT.fTemp );
		sprintf( szBuf, "%s%s%s", szBufStatus, szBufValues, szBufVT );
		Record(szBuf);
		Sleep(1000);
	}
	while( g_nWSM30KV2ThreadCounts > g_nWSM30KV2ThreadCountsAll -4 )
		Sleep(1);
	g_nWSM30KV2ThreadCounts--;
	char cDebug[255];
	sprintf( cDebug, "exit ThreadLog, counts = %d\n", g_nWSM30KV2ThreadCounts );
	OutputDebugString( cDebug );
	return 0;
}

void StartThreads()
{
	HANDLE hThread1, hThread2, hThread3, hThread4, hThread5, hThread6;
	InitializeCriticalSection(&csWSM30KV2Port);
	InitializeCriticalSection(&csWSM30KV2FifoHost);
	InitializeCriticalSection(&csWSM30KV2FifoDev);

/*	char* sSend = "WSM30KV2ThreadSend";
	hThread1 = CreateThread(NULL, 0, WSM30KV2ThreadSend, (LPVOID)sSend, 0, NULL);
	g_nWSM30KV2ThreadCounts++;

	char* sRead = "WSM30KV2ThreadRead";
	hThread2 = CreateThread(NULL, 0, WSM30KV2ThreadRead, (LPVOID)sRead, 0, NULL);
	g_nWSM30KV2ThreadCounts++;
*/
	char* sRW = "WSM30KV2ThreadWR";
	hThread2 = CreateThread(NULL, 0, WSM30KV2ThreadWR, (LPVOID)sRW, 0, NULL);
	g_nWSM30KV2ThreadCounts++;

	char* sAnalyze = "WSM30KV2ThreadAnalyze";
	hThread3 = CreateThread(NULL, 0, WSM30KV2ThreadAnalyze, (LPVOID)sAnalyze, 0, NULL);
	g_nWSM30KV2ThreadCounts++;

	char* sInquire = "WSM30KV2ThreadInquire";
	hThread4 = CreateThread(NULL, 0, WSM30KV2ThreadInquire, (LPVOID)sInquire, 0, NULL);
	g_nWSM30KV2ThreadCounts++;

	char* sDisplay = "WSM30KV2ThreadConvert";
	hThread5 = CreateThread(NULL, 0, WSM30KV2ThreadConvert, (LPVOID)sDisplay, 0, NULL);
	g_nWSM30KV2ThreadCounts++;

	char* sLog = "WSM30KV2ThreadLog";
	hThread6 = CreateThread(NULL, 0, WSM30KV2ThreadLog, (LPVOID)sLog, 0, NULL);
	g_nWSM30KV2ThreadCounts++;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// Auto Functions
void Auto_Rise_ACL1()
{
	g_WSM30KV2set.fACLv += g_fWSM30KV2AutoStep_ACL1;
	if( g_WSM30KV2set.fACLv >= g_fWSM30KV2AutoRated_ACL2 )
	{
		g_WSM30KV2set.fACLv = g_fWSM30KV2AutoRated_ACL2;
		Change_ACL_V((int)(g_WSM30KV2set.fACLv), FALSE );
		PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KV2AutoState, (LPARAM)(g_WSM30KV2set.fACLv) );
		g_WSM30KV2AutoState = WSM30KV2AS_Rise_EMS;
	}
	else if( g_WSM30KV2set.fACLv >= g_fWSM30KV2AutoRated_ACL1 )
	{
		g_WSM30KV2set.fACLv = g_fWSM30KV2AutoRated_ACL1;
		Change_ACL_V((int)(g_WSM30KV2set.fACLv), FALSE );
		PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KV2AutoState, (LPARAM)(g_WSM30KV2set.fACLv) );
		g_WSM30KV2AutoState = WSM30KV2AS_Rise_ACL2;
	}
	else
	{
		Change_ACL_V((int)(g_WSM30KV2set.fACLv), FALSE );
		PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KV2AutoState, (LPARAM)(g_WSM30KV2set.fACLv) );
	}
}

void Auto_Rise_ACL2()
{
	g_WSM30KV2set.fACLv += g_fWSM30KV2AutoStep_ACL2;
	if( g_WSM30KV2set.fACLv >= g_fWSM30KV2AutoRated_ACL2 )
	{
		g_WSM30KV2set.fACLv = g_fWSM30KV2AutoRated_ACL2;
		Change_ACL_V((int)(g_WSM30KV2set.fACLv), FALSE );
		PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KV2AutoState, (LPARAM)(g_WSM30KV2set.fACLv) );
		g_WSM30KV2AutoState = WSM30KV2AS_Rise_EMS;
	}
	else
	{
		Change_ACL_V((int)(g_WSM30KV2set.fACLv), FALSE );
		PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KV2AutoState, (LPARAM)(g_WSM30KV2set.fACLv) );
	}
}

void Auto_Rise_EMS()
{
	g_WSM30KV2set.fACLi += g_fWSM30KV2AutoStep_EMS;
	if( g_WSM30KV2set.fACLi >= g_fWSM30KV2AutoRated_EMS )
	{
		g_WSM30KV2set.fACLi = g_fWSM30KV2AutoRated_EMS;
		Change_ACL_I( (int)(g_WSM30KV2set.fACLi), FALSE );
		PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KV2AutoState, (LPARAM)g_WSM30KV2set.fACLi );
		g_WSM30KV2AutoState = WSM30KV2AS_NULL;	// 结束
		PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KV2AutoState, 0 );
	}
	else
	{
		Change_ACL_I( (int)(g_WSM30KV2set.fACLi), FALSE );
		PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KV2AutoState, (LPARAM)g_WSM30KV2set.fACLi );
	}
}

void Auto_Rise_FIL()
{
	g_WSM30KV2set.fFILi += g_fWSM30KV2AutoStep_FIL;
	if( g_WSM30KV2set.fFILi >= g_fWSM30KV2AutoRated_FIL )
	{
		g_WSM30KV2set.fFILi = g_fWSM30KV2AutoRated_FIL;
		Change_FIL( (int)(g_WSM30KV2set.fFILi *1000), FALSE );
		PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KV2AutoState, (LPARAM)(g_WSM30KV2set.fFILi*1000) );
		g_WSM30KV2AutoState = WSM30KV2AS_Rise_ACL1;
	}
	else
	{
		Change_FIL( (int)(g_WSM30KV2set.fFILi *1000), FALSE );
		PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_SETPARAM, g_WSM30KV2AutoState, (LPARAM)(g_WSM30KV2set.fFILi*1000) );
	}
}

void AutoProc()
{
	DWORD thisTickCount = GetTickCount();
	// 灯丝1
	switch( g_WSM30KV2AutoState )
	{
		/////////////////////////////////////////
		// Auto Rise
		/////////////////////////////////////////
	case	WSM30KV2AS_Rise_ACL1:
		if( thisTickCount - g_WSM30KV2lastAuto > 200 )
		{
			Auto_Rise_ACL1();
			g_WSM30KV2lastAuto = GetTickCount();
		}
		break;
	case	WSM30KV2AS_Rise_ACL2:
		if( thisTickCount - g_WSM30KV2lastAuto > 400 )
		{
			Auto_Rise_ACL2();
			g_WSM30KV2lastAuto = GetTickCount();
		}
		break;
	case	WSM30KV2AS_Rise_EMS:
		if( thisTickCount - g_WSM30KV2lastAuto > 400 )
		{
			Auto_Rise_EMS();
			g_WSM30KV2lastAuto = GetTickCount();
		}
		break;
	case	WSM30KV2AS_Rise_FIL:
		if( thisTickCount - g_WSM30KV2lastAuto > 400 )
		{
			Auto_Rise_FIL();
			g_WSM30KV2lastAuto = GetTickCount();
		}
		break;
	}
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


DWORD WINAPI WSM30KV2CommThread(LPVOID lpParam)
{
	// 打开端口
	g_WSM30KV2cfg.bReady = g_WSM30KV2.Open( g_WSM30KV2cfg.nPort, 115200 );
	if( g_WSM30KV2cfg.bReady )
	{
		if( !WSM30KV2CommTest(nWSM30KV2_Sleep) )
			g_WSM30KV2cfg.bReady = FALSE;
		if( g_WSM30KV2cfg.bReady )
		{
			CreateLog();
			StartThreads();
			g_WSM30KV2lastAuto = GetTickCount();
			reset( 38, 1 );
		}
	}
	PostMessage( g_hWSM30KV2Main, WM_USER_SPELLMAN_PMSKV_UpdateStatus, 1, g_WSM30KV2cfg.bReady );

	while( g_WSM30KV2cfg.bReady && WaitForSingleObject(g_hWSM30KV2EventTeminate,200) != WAIT_OBJECT_0 )
	{
//		if( !g_bPauseComm )
		{
			if( g_WSM30KV2AutoState < WSM30KV2AS_NULL )
			{
				AutoProc();
				Sleep(1);
			}
		}
		Sleep( 1 );
	}

	g_WSM30KV2.Close();
	g_WSM30KV2cfg.bReady	= g_WSM30KV2.IsOpened();
	if( g_hWSM30KV2File != NULL )
		CloseHandle( g_hWSM30KV2File );
	g_hWSM30KV2File	= NULL;
	SetEvent( g_hWSM30KV2EventWork );
	return 0;
}

void CCommWSM30KV2::SetParam( int nIndex, int nValue )
{
	switch( nIndex )
	{
	case	0:
		g_WSM30KV2cfg.nPort	= nValue;
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
	case	7:	// BIAS
		Change_BIAS( nValue, TRUE );
		break;

	case	11:	// BIAS
	case	12:	// FIL
	case	13:	// ACL
	case	14:	// SCI
	case	15:	// EMS
	case	16:	// COL
	case	17:	// PMT
	case	18:	// ALL
		setenable( nIndex, nValue );
		break;

	case	21:	// BIAS
	case	22:	// FIL
	case	23:	// ACL
	case	24:	// SCI
	case	25:	// EMS
	case	26:	// COL
	case	27:	// PMT
	case	28:	// ALL
		setpause( nIndex, nValue );
		break;

	case	31:	// BIAS
	case	32:	// FIL
	case	33:	// ACL
	case	34:	// SCI
	case	35:	// EMS
	case	36:	// COL
	case	37:	// PMT
	case	38:	// ALL
		reset( nIndex, nValue );
		break;

	case	41:
		g_fWSM30KV2AutoRated_ACL2	= (float)nValue;	// (float)(nValue /1000.0);
		break;
	case	42:
		g_fWSM30KV2AutoRated_EMS	= (float)nValue;
		break;
	case	43:
		g_fWSM30KV2AutoRated_FIL	= (float)(nValue /100.0);
		break;

	case	44:
		g_fWSM30KV2AutoStep_ACL1	= (float)nValue;
		break;
	case	45:
		g_fWSM30KV2AutoStep_ACL2	= (float)nValue;
		break;
	case	46:
		g_fWSM30KV2AutoStep_EMS		= (float)nValue;
		break;
	case	47:
		g_fWSM30KV2AutoStep_FIL		= (float)(nValue /1000.0);
		break;

	case	49:
		g_WSM30KV2AutoState	= (WSM30KV2_AutoState)nValue;
/*		if( g_WSM30KV2AutoState == WSM30KV2AS_Rise_ACL1 && g_WSM30KV2set.fACLv > g_fWSM30KV2AutoRated_ACL1 )
			g_WSM30KV2AutoState = WSM30KV2AS_Rise_ACL2;
		else if( g_WSM30KV2AutoState == WSM30KV2AS_Rise_ACL1
			&& ((int)(g_WSM30KV2set.fACLv *10) == (int)(g_fWSM30KV2AutoRated_ACL2 *10)) )
			g_WSM30KV2AutoState = WSM30KV2AS_Rise_EMS;
*/		// 威思曼新电源，为避免触发OC及OV，调整加的顺序：先FIL，再ACL，最后EMS
		if( g_WSM30KV2AutoState == WSM30KV2AS_Rise_FIL && g_WSM30KV2set.fFILi > g_fWSM30KV2AutoRated_FIL )
			g_WSM30KV2AutoState = WSM30KV2AS_Rise_ACL1;
		else if( g_WSM30KV2AutoState == WSM30KV2AS_Rise_ACL1 && g_WSM30KV2set.fACLv > g_fWSM30KV2AutoRated_ACL1 )
			g_WSM30KV2AutoState = WSM30KV2AS_Rise_ACL2;
		else if( g_WSM30KV2AutoState == WSM30KV2AS_Rise_ACL1
			&& ((int)(g_WSM30KV2set.fACLv *10) == (int)(g_fWSM30KV2AutoRated_ACL2 *10)) )
			g_WSM30KV2AutoState = WSM30KV2AS_Rise_EMS;
		break;

	case	100:
		// test
		{
			char data[100];
//			WSM30KV2_ConvertSendData( byteWSM30KV2_Sn, byteWSM30KV2_FuncCodeR, AddrWSM30KV2R_OutputStatus1, 0x0011, data );
			BYTE bData[20];
			bData[0] = byteWSM30KV2_Sn;
			bData[1] = byteWSM30KV2_FuncCodeR;
			bData[2] = 0x02;
			bData[3] = 0x00;
			bData[4] = 0x11;
			bData[5] = LRC( bData, 5 );
			sprintf( data, ":%02X%02X%02X%02X%02X%02X\r\n", bData[0], bData[1], bData[2], bData[3], bData[4], bData[5] );
			WSM30KV2_ConvertReceiveData( strlen(data), data );
			bData[0] = byteWSM30KV2_Sn;
			bData[1] = byteWSM30KV2_FuncCodeR;
			bData[2] = 0x02;
			bData[3] = 0x00;
			bData[4] = 0x02;
			bData[5] = LRC( bData, 5 );
			sprintf( data, ":%02X%02X%02X%02X%02X%02X\r\n", bData[0], bData[1], bData[2], bData[3], bData[4], bData[5] );
			WSM30KV2_ConvertReceiveData( strlen(data), data );

			bData[0] = byteWSM30KV2_Sn;
			bData[1] = byteWSM30KV2_FuncCodeR;
			bData[2] = 0x06;
			bData[3] = 0x00;
			bData[4] = 0x01;
			bData[5] = 0x00;
			bData[6] = 0x01;
			bData[7] = 0x00;
			bData[8] = 0x10;
			bData[9] = LRC( bData, 9 );
			sprintf( data, ":%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n", bData[0], bData[1], bData[2], bData[3], bData[4], bData[5], bData[6], bData[7], bData[8], bData[9] );
			WSM30KV2_ConvertReceiveData( strlen(data), data );
			bData[0] = byteWSM30KV2_Sn;
			bData[1] = byteWSM30KV2_FuncCodeR;
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
			WSM30KV2_ConvertReceiveData( strlen(data), data );
		}
		break;
	}
}

void CCommWSM30KV2::InitHWND( HWND hWnd )
{
	g_hWSM30KV2Main = hWnd;
}

BOOL CCommWSM30KV2::Init()
{
	// Test
/*	char data[100];
	sprintf( data, ":0105044AFF00AD\r\n" );
//	WSM30KV2_ConvertReceiveData( 0x01, 0x044A, strlen(data), data );
// 	g_strWSM30KV2	= "";
// 	g_strWSM30KV2Saved	= "";
*/
	/////////////////////////////////////////////////////////////////
	ResetEvent( g_hWSM30KV2EventWork );
	ResetEvent( g_hWSM30KV2EventTeminate );

	g_hWSM30KV2Thread	= CreateThread( NULL, 0, WSM30KV2CommThread, NULL, 0, &g_dwWSM30KV2Thread );
	if( g_hWSM30KV2Thread == NULL )
		return FALSE;

	CloseHandle(g_hWSM30KV2Thread);
	return TRUE;
}

BOOL CCommWSM30KV2::Close()
{
	if( g_WSM30KV2cfg.bReady )
	{
		SetEvent( g_hWSM30KV2EventTeminate );
		WaitForSingleObject( g_hWSM30KV2EventWork, INFINITE );
		g_WSM30KV2cfg.bReady	= FALSE;
		g_WSM30KV2cfg.nPort	= -1;
	}

	return TRUE;
}
