// CommVAC.cpp : implementation file
//

#include "stdafx.h"
#include "CommVAC.h"

#include "../GlobalComponent/Serial.h"
#include <VACobtain.h>
#include "CommonVACFunction.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HANDLE	g_hThreadIPG, g_hThreadIPC, g_hThreadBoard;
DWORD	g_dwThreadIPG, g_dwThreadIPC, g_dwThreadBoard;
HANDLE	g_hEventIPG				= CreateEvent(NULL,FALSE, FALSE, _T("WORKipg"));
HANDLE	g_hEventTeminateIPG		= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEipg"));
HANDLE	g_hEventIPC				= CreateEvent(NULL,FALSE, FALSE, _T("WORKipc"));
HANDLE	g_hEventTeminateIPC		= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEipc"));
HANDLE	g_hEventCCG				= CreateEvent(NULL,FALSE, FALSE, _T("WORKccg"));
HANDLE	g_hEventTeminateCCG		= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEccg"));
HANDLE	g_hEventWorkBoard		= CreateEvent(NULL,FALSE, FALSE, _T("WORKboard"));
HANDLE	g_hEventTeminateBoard	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEboard"));

HWND	g_hMain;
CSerial	g_IPG, g_IPC, g_board;
CfgVAC_IP_CCG	g_cfgIPG, g_cfgIPC, g_cfgCCG, g_cfgBoard;
StatusFEGBrdVAC_board	g_boardStatusServer, g_boardStatusMain;

CString g_strIPG, g_strIPC, g_strBoard;
CString g_strIPGSaved, g_strIPCSaved, g_strBoardSaved;
CString	g_strWinIPG, g_strWinIPC;

HANDLE	g_hFileIPG	= NULL;
HANDLE	g_hFileIPC	= NULL;
HANDLE	g_hFileCCG	= NULL;
HANDLE	g_hFileBoard= NULL;
int		g_nLoopsIPG, g_nLoopsIPC, g_nLoopsBoard;

BOOL	g_bReady		= FALSE;	// �Ƿ�������һ���豸��ͨ
BOOL	g_bPauseComm	= FALSE;	// �Ƿ���ͣ���豸ͨѶ
BOOL	g_bPausePost	= FALSE;	// �Ƿ���ͣ���ͼ��ֵ������ֹͣ���豸ͨѶ��
BOOL	g_bPauseInquire	= FALSE;
int		g_nIs1845		= -1;		// �����1����1845������տ��ư�ÿ���IPG�������2�������ϵͳ�Ż�������տ��ư�ÿ���CCG
BOOL	g_bIsDual		= FALSE;	// ���ӱÿ������Ƿ���˫ͨ����
BOOL	g_nIPtype		= 1;		// ���ӱÿ���������ʽ������ʽ��Ӱ��ͨѶЭ�飩��Ĭ��Ϊ��ʽ

/////////////////////////////////////////////////////////////////////////////
// CCommVAC

CCommVAC::CCommVAC()
{
	g_hMain	= NULL;

	ZeroMemory( &g_cfgIPG, sizeof(CfgVAC_IP_CCG) );
	ZeroMemory( &g_cfgIPC, sizeof(CfgVAC_IP_CCG) );
	ZeroMemory( &g_cfgCCG, sizeof(CfgVAC_IP_CCG) );
	g_cfgIPG.nType	= 1;
	g_cfgIPC.nType	= 2;
	g_cfgCCG.nType	= 3;
/*	g_cfgIPG.bReady	= FALSE;
	g_cfgIPC.bReady	= FALSE;
	g_cfgCCG.bReady	= FALSE;
	g_cfgIPG.dVac	= 0.0;
	g_cfgIPC.dVac	= 0.0;
	g_cfgCCG.dVac	= 0.0;
*/
	ZeroMemory( &g_cfgBoard, sizeof(CfgVAC_IP_CCG) );
	g_cfgBoard.nType	= 4;
	ZeroMemory( &g_boardStatusServer, sizeof(StatusFEGBrdVAC_board) );
	ZeroMemory( &g_boardStatusMain, sizeof(StatusFEGBrdVAC_board) );
}

CCommVAC::~CCommVAC()
{
}


void CCommVAC::SetParam( int nIndex, int nValue )
{
	switch( nIndex )
	{
	case	1:
		g_cfgIPG.nPort	= nValue;
		if( g_cfgIPG.nPort == g_cfgIPC.nPort )
			g_bIsDual	= TRUE;
		else
			g_bIsDual	= FALSE;
		break;
	case	2:
		g_cfgIPC.nPort	= nValue;
		if( g_cfgIPG.nPort == g_cfgIPC.nPort )
			g_bIsDual	= TRUE;
		else
			g_bIsDual	= FALSE;
		break;
	case	3:
		g_cfgCCG.nPort	= nValue;
		break;
	case	4:
		g_cfgBoard.nPort= nValue;
		break;
	case	5:	// IPGʹ���ĸ�ͨ��
		g_strWinIPG.Format( "8%d2", nValue );
		break;
	case	6:	// IPCʹ���ĸ�ͨ��
		g_strWinIPC.Format( "8%d2", nValue );
		break;

	case	11:
		g_bPauseComm	= (BOOL)nValue;
		break;
	case	12:
		g_bPausePost	= (BOOL)nValue;
		break;
	case	13:
		g_bPauseInquire	= (BOOL)nValue;
		break;

	case	20:
		g_nIs1845		= nValue;		// 1845��IPG��Board��ȡ�����ϵͳ�Ż���CCG��Board��ȡ
		break;
	case	21:
		g_nIPtype		= nValue;		// ���ӱÿ�����ʽ��
		break;

	case	101:
	case	103:
		g_boardStatusMain.byteVent	= (BYTE)nValue;
		break;
	case	102:
		g_boardStatusMain.byteV1	= (BYTE)nValue;
		break;

	case	(100 +0x11):
	case	(100 +0x21):
	case	(100 +0x91):
		break;
	}
}

void CCommVAC::InitHWND( HWND hWnd )
{
	g_hMain = hWnd;
}