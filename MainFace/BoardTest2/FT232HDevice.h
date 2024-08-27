#if !defined(AFX_FT232HDEVICE_H__0EE03288_15B3_4ABC_8A5B_2891360B01D4__INCLUDED_)
#define AFX_FT232HDEVICE_H__0EE03288_15B3_4ABC_8A5B_2891360B01D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FT232HDevice.h : header file
//

#include "ftd2xx.h"

//=============================================================================
// Serial
const	BYTE	byte_HeadSnd	= 0xCA;
const	BYTE	byte_HeadRec	= 0x80;
const	BYTE	byte_HeadINT	= 0x80;
const	BYTE	byte_LenSnd1	= 0x04;
const	BYTE	byte_LenSnd2	= 0x06;
const	BYTE	byte_TailSnd	= 0x0A;
const	BYTE	byte_TailRec	= 0x0E;
const	BYTE	byte_TailINT	= 0x0E;
const	BYTE	byte_cCOpc		= 0xF4;
const	BYTE	byte_cCOpr		= 0xF5;
const	BYTE	byte_cCSub		= 0xF1;

#define	fHead	0
#define fLen	1
#define fCC		2
#define fOpc	3
#define fOpr1	4
#define fOpr2	5
#define fTail	6
const	int frameLen	= 7;
const	int frameLen2	= 5;
//=============================================================================

/////////////////////////////////////////////////////////////////////////////
// CFT232HDevice window

class CFT232HDevice
{
	// Construction
public:
	CFT232HDevice();

	// Attributes
public:
	BOOL	m_bReady;

	DWORD	m_dwDevsNum;
	char*	m_cBufPtrs[16];

// Operations
public:

	// Implementation
public:
	virtual ~CFT232HDevice();

	//****************************************************************************************
	HMODULE m_hmodule;
	FT_HANDLE m_ftHandle;
	int FTDI_LoadDLL();

	typedef FT_STATUS (WINAPI *PtrToOpen)(PVOID, FT_HANDLE *); 
	PtrToOpen m_pOpen; 
	FT_STATUS FTDI_Open(PVOID);

	typedef FT_STATUS (WINAPI *PtrToOpenEx)(PVOID, DWORD, FT_HANDLE *); 
	PtrToOpenEx m_pOpenEx; 
	FT_STATUS FTDI_OpenEx(PVOID, DWORD);

	typedef FT_STATUS (WINAPI *PtrToListDevices)(PVOID, PVOID, DWORD);
	PtrToListDevices m_pListDevices; 
	FT_STATUS FTDI_ListDevices(PVOID, PVOID, DWORD);

	typedef FT_STATUS (WINAPI *PtrToClose)(FT_HANDLE);
	PtrToClose m_pClose;
	FT_STATUS FTDI_Close();

	typedef FT_STATUS (WINAPI *PtrToRead)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
	PtrToRead m_pRead;
	FT_STATUS FTDI_Read(LPVOID, DWORD, LPDWORD);

	typedef FT_STATUS (WINAPI *PtrToWrite)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
	PtrToWrite m_pWrite;
	FT_STATUS FTDI_Write(LPVOID, DWORD, LPDWORD);

	typedef FT_STATUS (WINAPI *PtrToResetDevice)(FT_HANDLE);
	PtrToResetDevice m_pResetDevice;
	FT_STATUS FTDI_ResetDevice();

	typedef FT_STATUS (WINAPI *PtrToPurge)(FT_HANDLE, ULONG);
	PtrToPurge m_pPurge;
	FT_STATUS FTDI_Purge(ULONG);

	typedef FT_STATUS (WINAPI *PtrToSetTimeouts)(FT_HANDLE, ULONG, ULONG);
	PtrToSetTimeouts m_pSetTimeouts;
	FT_STATUS FTDI_SetTimeouts(ULONG, ULONG);

	typedef FT_STATUS (WINAPI *PtrToGetQueueStatus)(FT_HANDLE, LPDWORD);
	PtrToGetQueueStatus m_pGetQueueStatus;
	FT_STATUS FTDI_GetQueueStatus(LPDWORD);
	//****************************************************************************************

	BOOL	Serial_Load( CString& strError );
	BOOL	Serial_Open( int index );
	void	Serial_Close();
	BOOL	Serial_Write( void *pData, DWORD dwBufferSize, DWORD &dwBytesWritten );
	int		Serial_Read( void *Buffer, DWORD &dwBytesReturned );
	CString	Serial_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData );
	CString	Serial_CommToBoards( BYTE bOp1, BYTE bOp2 );
	CString	Serial_CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 );
	CString	Serial_CommToBoards3( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, BYTE bOp5, BYTE bOp6 );
	void	Serial_CommToBoards_ReadADC( int nProcessID, BYTE bModID, BYTE bCID, BOOL bADCtype, int nADCchn, BYTE bTM, WORD wData );

	int		Serial_ListDevicesCounts();
	char*	Serial_GetDeviceSerialNumber( int nIndex );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FT232HDEVICE_H__0EE03288_15B3_4ABC_8A5B_2891360B01D4__INCLUDED_)
