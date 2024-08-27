
// #ifndef __IonPumps_H
// #define __IonPumps_H

#if !defined(AFX_IonPumps_)
#define AFX_IonPumps_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <VACobtain.h>
#include "SerialPort.h"

HWND	hMain;

UINT	WINAPI	ThreadProcIPG(LPVOID lpParam);
UINT	WINAPI	ThreadProcIPC(LPVOID lpParam);
UINT	WINAPI	ThreadProcCCG(LPVOID lpParam);
int	APIENTRY	MainWndProcIPG(HWND hDlg, int uMsg, WPARAM wParam, LPARAM lParam);
int	APIENTRY	MainWndProcIPC(HWND hDlg, int uMsg, WPARAM wParam, LPARAM lParam);
int	APIENTRY	MainWndProcCCG(HWND hDlg, int uMsg, WPARAM wParam, LPARAM lParam);

HWND	hThreadWinIPG;
HWND	hThreadWinIPC;
HWND	hThreadWinCCG;
CWinThread*	pThreadIPG;
CWinThread*	pThreadIPC;
CWinThread*	pThreadCCG;

LPCSTR AppNameIPG	= TEXT("IPG");
LPCSTR AppHdrIPG	= TEXT("IPGWindow");
LPCSTR AppNameIPC	= TEXT("IPC");
LPCSTR AppHdrIPC	= TEXT("IPCWindow");
LPCSTR AppNameCCG	= TEXT("CCG");
LPCSTR AppHdrCCG	= TEXT("CCGWindow");

CfgVAC_IP_CCG	cfgIPG;
CfgVAC_IP_CCG	cfgIPC;
CfgVAC_IP_CCG	cfgCCG;

CSerialPort	spIPG;
CSerialPort	spIPC;
CSerialPort	spCCG;

CString	strDataIPG;
CString	strDataIPC;
CString	strDataCCG;

#endif
