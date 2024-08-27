// VirtualVACs.cpp : implementation file
//

#include "stdafx.h"
#include "VirtualVACs.h"

#include <VACobtain.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
extern	UINT	/*WINAPI*/	ThreadProcIPG(LPVOID lpParam);
extern	UINT	/*WINAPI*/	ThreadProcIPC(LPVOID lpParam);
extern	UINT	/*WINAPI*/	ThreadProcCCG(LPVOID lpParam);
extern	int	APIENTRY	MainWndProcIPG(HWND hDlg, int uMsg, WPARAM wParam, LPARAM lParam);
extern	int	APIENTRY	MainWndProcIPC(HWND hDlg, int uMsg, WPARAM wParam, LPARAM lParam);
extern	int	APIENTRY	MainWndProcCCG(HWND hDlg, int uMsg, WPARAM wParam, LPARAM lParam);
extern	HANDLE ghIPGThreadTerminated;

extern	UINT	InitIPG();

extern	HWND		ghMain, ghThreadWinIPG, ghThreadWinIPC, ghThreadWinCCG;
extern	CWinThread*	gpThreadIPG, gpThreadIPC, gpThreadCCG;

extern	CfgVAC_IP_CCG	g_cfgIPG, g_cfgIPC, g_cfgCCG;
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// CVirtualVACs

CVirtualVACs::CVirtualVACs()
{
}

CVirtualVACs::~CVirtualVACs()
{
}

/////////////////////////////////////////////////////////////////////////////
// CVirtualVACs message handlers

BOOL CVirtualVACs::ClearDevice()
{
	Close(1);
	Close(2);
	Close(3);
// 	if( m_pIPG != NULL )
// 		m_pIPG->ClearDevice();
// 	if( m_pIPC != NULL )
// 		m_pIPC->ClearDevice();
// 	if( m_pCCG != NULL )
// 		m_pCCG->ClearDevice();
	return TRUE;
}

BOOL CVirtualVACs::Close( int nType )
{
	switch( nType )
	{
	case	1:
		if( gpThreadIPG != NULL )
		{
			PostMessage(ghThreadWinIPG, WM_CLOSE, 0, 0);
			WaitForSingleObject( ghIPGThreadTerminated, INFINITE );
		}
		break;
	case	2:
		if( gpThreadIPC != NULL )
			PostMessage(ghThreadWinIPC, WM_QUIT, 0, 0);
		break;
	case	3:
		if( gpThreadCCG != NULL )
			PostMessage(ghThreadWinCCG, WM_QUIT, 0, 0);
		break;
	}

// 	m_nInit = -1;
	return TRUE;
}

BOOL CVirtualVACs::Reset( CWnd* pWnd )
{
	Close(1);
	Close(2);
	Close(3);
/*
	TCHAR exeFullPath[255];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	char path[255];
	sprintf(path, "%s", exeFullPath);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	int nPort[3];
	char szBuf[20], szBuf1[20], szBuf2[20];
	// 串口端口读取
	DWORD dwReadCfg = GetPrivateProfileString("Settings", "CommPorts", "", szBuf, 15, path);
	if( dwReadCfg != 0 )
	{
		strcpy(szBuf1, szBuf);
		strcpy(szBuf2, szBuf);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf2, strrchr(szBuf, ',')+1);	// szBuf2中是CCG端口
			*strrchr(szBuf, ',') = '\0';			// szBuf中是IPG & IPC端口
			strcpy(szBuf1, strrchr(szBuf, ',')+1);	// szBuf1中是IPC端口
			*strchr(szBuf, ',') = '\0';				// szBuf中是IPG端口
			nPort[0] = atoi( szBuf );		// IPG
			nPort[1] = atoi( szBuf1 );		// IPC
			nPort[2] = atoi( szBuf2 );		// CCG
		}
	}
	else
	{
		nPort[0] = 3;
		nPort[1] = 4;
		nPort[2] = 5;
	}
*/
	ghMain = pWnd->m_hWnd;
	gpThreadIPG = AfxBeginThread(ThreadProcIPG, 0);
	SetTimer(ghThreadWinIPG, 1, 2000, 0);
	return FALSE;//m_nInit;
}

void CVirtualVACs::SetParam( int index, int nValue )
{
	switch( index )
	{
	case	1:
		g_cfgIPG.nPort	= nValue;
		break;
	case	2:
		g_cfgIPC.nPort	= nValue;
		break;
	case	3:
		g_cfgCCG.nPort	= nValue;
		break;
	}
}