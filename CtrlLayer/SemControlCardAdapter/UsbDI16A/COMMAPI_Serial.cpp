// COMMAPI_Serial.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <memory.h>
#include <math.h>

#include <COMMAPI_Serial.h>
#include <USB_Functions.h>

#include <ftd2xx.h>

//#include "CriticalResource.h"
#include <afxmt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
DWORD	Serial_dwErrorCount = 0;
#endif


//=============================================================================
// Serial

//CriticalResource		m_mutexPortsSerial;
static	CCriticalSection		m_mutexPortsSerial;
static	BOOL	m_bIs232Ready;
DWORD	m_dwDevsNum;
char*	m_cBufPtrs[16];

/////////////////////////////////////////////////////////////////////////////
// 
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
//FT_STATUS FTDI_Write(LPVOID, DWORD, LPDWORD);

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
int FTDI_LoadDLL()
{
	m_hmodule = LoadLibrary("Ftd2xx.dll");	
	if(m_hmodule == NULL)
	{
		AfxMessageBox("Error: Can't Load ft8u245.dll");
		return 1;
	}

	m_pWrite = (PtrToWrite)GetProcAddress(m_hmodule, "FT_Write");
	if (m_pWrite == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_Write");
		return 2;
	}

	m_pRead = (PtrToRead)GetProcAddress(m_hmodule, "FT_Read");
	if (m_pRead == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_Read");
		return 3;
	}

	m_pOpen = (PtrToOpen)GetProcAddress(m_hmodule, "FT_Open");
	if (m_pOpen == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_Open");
		return 4;
	}

	m_pOpenEx = (PtrToOpenEx)GetProcAddress(m_hmodule, "FT_OpenEx");
	if (m_pOpenEx == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_OpenEx");
		return 5;
	}

	m_pListDevices = (PtrToListDevices)GetProcAddress(m_hmodule, "FT_ListDevices");
	if(m_pListDevices == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_ListDevices");
		return 6;
	}

	m_pClose = (PtrToClose)GetProcAddress(m_hmodule, "FT_Close");
	if (m_pClose == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_Close");
		return 7;
	}

	m_pResetDevice = (PtrToResetDevice)GetProcAddress(m_hmodule, "FT_ResetDevice");
	if (m_pResetDevice == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_ResetDevice");
		return 8;
	}

	m_pPurge = (PtrToPurge)GetProcAddress(m_hmodule, "FT_Purge");
	if (m_pPurge == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_Purge");
		return 9;
	}

	m_pSetTimeouts = (PtrToSetTimeouts)GetProcAddress(m_hmodule, "FT_SetTimeouts");
	if (m_pSetTimeouts == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_SetTimeouts");
		return 10;
	}

	m_pGetQueueStatus = (PtrToGetQueueStatus)GetProcAddress(m_hmodule, "FT_GetQueueStatus");
	if (m_pGetQueueStatus == NULL)
	{
		AfxMessageBox("Error: Can't Find FT_GetQueueStatus");
		return 11;
	}
	return 0;
}	


//****************************************************************************************
FT_STATUS FTDI_Read(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytesRead)
{
	if (!m_pRead)
	{
		AfxMessageBox("FT_Read is not valid!"); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pRead)(m_ftHandle, lpvBuffer, dwBuffSize, lpdwBytesRead);
}	


//****************************************************************************************
FT_STATUS FTDI_Write(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytes)
{
	if (!m_pWrite)
	{
		AfxMessageBox("FT_Write is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pWrite)(m_ftHandle, lpvBuffer, dwBuffSize, lpdwBytes);
}	





//****************************************************************************************
FT_STATUS FTDI_Open(PVOID pvDevice)
{
	if (!m_pOpen)
	{
		AfxMessageBox("FT_Open is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pOpen)(pvDevice, &m_ftHandle );
}	

//****************************************************************************************
FT_STATUS FTDI_OpenEx(PVOID pArg1, DWORD dwFlags)
{
	if (!m_pOpenEx)
	{
		AfxMessageBox("FT_OpenEx is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pOpenEx)(pArg1, dwFlags, &m_ftHandle);
}	


//****************************************************************************************
FT_STATUS FTDI_ListDevices(PVOID pArg1, PVOID pArg2, DWORD dwFlags)
{
	if (!m_pListDevices)
	{
		AfxMessageBox("FT_ListDevices is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pListDevices)(pArg1, pArg2, dwFlags);
}	


//****************************************************************************************
FT_STATUS FTDI_Close()
{
	if (!m_pClose)
	{
		AfxMessageBox("FT_Close is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pClose)(m_ftHandle);
}	



//****************************************************************************************
FT_STATUS FTDI_ResetDevice()
{
	if (!m_pResetDevice)
	{
		AfxMessageBox("FT_ResetDevice is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pResetDevice)(m_ftHandle);
}	



//****************************************************************************************
FT_STATUS FTDI_Purge(ULONG dwMask)
{
	if (!m_pPurge)
	{
		AfxMessageBox("FT_Purge is not valid!"); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pPurge)(m_ftHandle, dwMask);
}	



//****************************************************************************************
FT_STATUS FTDI_SetTimeouts(ULONG dwReadTimeout, ULONG dwWriteTimeout)
{
	if (!m_pSetTimeouts)
	{
		AfxMessageBox("FT_SetTimeouts is not valid!"); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pSetTimeouts)(m_ftHandle, dwReadTimeout, dwWriteTimeout);
}	


//****************************************************************************************
FT_STATUS FTDI_GetQueueStatus(LPDWORD lpdwAmountInRxQueue)
{
	if (!m_pGetQueueStatus)
	{
		AfxMessageBox("FT_GetQueueStatus is not valid!"); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pGetQueueStatus)(m_ftHandle, lpdwAmountInRxQueue);
}	



int Serial_ListDevicesCounts()
{
	//search for Descriptions or Serial Numbers depending on the current mode
	FT_STATUS ftStatus;
	ftStatus = FTDI_ListDevices(&m_dwDevsNum, NULL, FT_LIST_NUMBER_ONLY);
	if( ftStatus != FT_OK )
		return -1;
	else
		return (int)m_dwDevsNum;
}

char* Serial_GetDeviceSerialNumber( int nIndex )
{
	if( (m_dwDevsNum < 1) || (nIndex > m_dwDevsNum-1) )
		return NULL;
	
	FT_STATUS ftStatus;
	ftStatus = FTDI_ListDevices(m_cBufPtrs, &m_dwDevsNum, FT_LIST_ALL|FT_OPEN_BY_SERIAL_NUMBER);
	if( FT_SUCCESS(ftStatus) )
		return m_cBufPtrs[nIndex];
	else
		return NULL;
}


BOOL Serial_OpenDevice( void )
{
	int nRet = FTDI_LoadDLL();
	if( nRet > 0 )
		return FALSE;

	m_dwDevsNum	= 0;
	// 找FT232设备
	int numDevs = Serial_ListDevicesCounts();
	if( numDevs > -1 )
	{
		CString str;
		str.Format("%d FT232 device(s) attached:\n", numDevs);
		TRACE(str);

		if( numDevs > 0 )
		{
			int i, ft232Index = -1;
			for( i=0; i<numDevs; i++)
			{
				str.Format("   #%d Serial Number: %s \n", i, Serial_GetDeviceSerialNumber(i) );
				TRACE(str);
				FT_HANDLE ftHandle;
				FT_DEVICE ftDevice;
				FT_STATUS ftStatus;
				DWORD deviceID;
				char SerialNumber[16];
				char Description[64];
				ftStatus = FT_Open(i, &ftHandle);
				if(ftStatus != FT_OK)
				{
					// FT_Open failed return;
					continue;
				}
				ftStatus = FT_GetDeviceInfo( ftHandle, &ftDevice, &deviceID, SerialNumber, Description, NULL );
				if (ftStatus == FT_OK)
				{
					str.Format("   #%d DeviceID: 0x%08X \n", i, deviceID );
					TRACE(str);
					str.Format("   #%d Description: %s \n", i, Description );
					TRACE(str);
					str.Format("   #%d ftDevice: %ld \n", i, ftDevice );
					TRACE(str);
					if (ftDevice == FT_DEVICE_232H)
					{
						// device is FT232H
						ft232Index = i;
						FT_Close(ftHandle);
						m_bIs232Ready = TRUE;
						break;
					}
				}
				FT_Close(ftHandle);
			}
			if( ft232Index < 0 )
				return FALSE;

			FT_STATUS ftStatus = FTDI_Open((LPVOID)ft232Index);
			if( ftStatus != FT_OK )
			{
#ifdef _DEBUG
				afxDump << "\nInitCard-- ";
				afxDump << "Error in scanning for FTDI devices: No devices found." << "\n";
				afxDump << "\n		ErrorCount = " << Serial_dwErrorCount++ << "\n";
#endif
				return FALSE;
			}
			else
			{
				FTDI_ResetDevice();
				FTDI_Purge(FT_PURGE_RX || FT_PURGE_TX);
				FTDI_ResetDevice();
				FTDI_SetTimeouts(3000, 3000);//extend timeout while board finishes reset
				return TRUE;
			}
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// COMMAPI
void Serial_ResetCard() 
{
	FTDI_ResetDevice();
	FTDI_Purge(FT_PURGE_RX || FT_PURGE_TX);
	FTDI_ResetDevice();
	FTDI_SetTimeouts(3000, 3000);//extend timeout while board finishes reset
	Sleep(150);
}

BOOL Serial_InitCard_once( void )
{
	// send version and check returns of port B,D
	Serial_WriteToBoard( USB_ReadVersion, FALSE, 0 );
	Sleep(100);

	BYTE bBuf[1024];
	DWORD	dwBytesReturned = 0;
	int nRet = Serial_Read( bBuf, dwBytesReturned );
	if( (nRet == 0) && dwBytesReturned > 0 )
	{
		CString str, strMsg;
		strMsg.Format( "Read port B & D in Init, received %ld bytes: ", dwBytesReturned );
		for(DWORD i=0; i<dwBytesReturned; i++)
		{
			str.Format("0x%02X, ", bBuf[i]);
			strMsg += str;
		}
		TRACE("%s\n", strMsg);
		/*/////////////////////////////////
		帧头	长度	cD		cB		帧尾
		80		04		××	××	0E
		/////////////////////////////////*/
		// port D: 0x01, means MCI power on
 		if( (dwBytesReturned < 5) || (bBuf[0] != byte_HeadINT) || (bBuf[2] == 0x07) )
 			return FALSE;
		else
		{
			m_bIs232Ready = TRUE;
			return TRUE;
		}

//		ReadDataFromINI();

		// switch analog power supplies ON
		BYTE bOP =  USB_TTTT_Write_08 << 4;
		bOP |= (BYTE)USB_MID_SPR;
		Serial_CommToBoards2(	USB_CID_SPR_AnalogPower_Enable,
								bOP,
								0,
								0x01 );

		m_bIs232Ready = TRUE;

		// set interrupt mask register, mask all off
		Serial_WriteToBoard( USB_SetIMR, TRUE, 0 );
		// set interrupt mask register, enable INT0,1,2,3
		Serial_WriteToBoard( USB_SetIMR, TRUE, 0x02 );	// 只接收VAC板中断	//0x07 );
		// set INT enable register on VAC board
		Serial_CommToBoards2( USB_CID_VAC_INT_Enable_Register,
								USB_TM_VAC_Write_16,
								0xFF, 0x0F );
		// set INT enable register on SPR board
// 		Serial_CommToBoards2( USB_CID_SPR_INT_Enable_Register,
// 								USB_TM_SPR_Write_08,
// 								0x00, 0xFF );

		// set mainslock off
		Serial_WriteToBoard( USB_MainsLock_OFF, FALSE, 0 );
		Serial_WriteToBoard( USB_Video_ON, FALSE, 0 );

		// set detector pair
		Serial_WriteToBoard( USB_DetPairSelection, TRUE, 0x00 );
		return TRUE;
	}

	return FALSE;
}

int Serial_Diag_Init( int nVACBoardType )
{
	if( !m_bIs232Ready )
		return -1;

	int nRet = 0;
	///////////////////////////////////////////////////////
	// MCI初始化参数
	// set interrupt mask register, mask all off
	Serial_WriteToBoard( USB_SetIMR, TRUE, 0 );
	// set mainslock off
	Serial_WriteToBoard( USB_MainsLock_OFF, FALSE, 0 );
	// set video on
	Serial_WriteToBoard( USB_Video_ON, FALSE, 0 );
	// set detector pair
	Serial_WriteToBoard( USB_DetPairSelection, TRUE, 0x00 );
	///////////////////////////////////////////////////////

	unsigned short nRegister, nCount, nCountTotal = 10;
	///////////////////////////////////////////////////////
	// SPR初始化参数
	nRegister = 0;
	nCount = 0;
	do
	{
		Serial_ReadBoardID( 4, &nRegister );
		nCount++;
	} while( nRegister == 0x0 && nCount < nCountTotal );
	if( nRegister == 0xA4 )
	{
		// switch analog power supplies ON
		BYTE bOP =  USB_TTTT_Write_08 << 4;
		bOP |= (BYTE)USB_MID_SPR;
		Serial_CommToBoards2(	USB_CID_SPR_AnalogPower_Enable,
								bOP,
								0,
								0x01 );
		// set INT enable register on SPR board
// 		Serial_CommToBoards2(	USB_CID_SPR_INT_Enable_Register,
// 								USB_TM_SPR_Write_08,
// 								0x00, 0xFF );
	}
	else
		nRet |= 0x08;
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// SCN初始化参数
	nRegister = 0;
	nCount = 0;
	do
	{
		Serial_ReadBoardID( 1, &nRegister );
		nCount++;
	} while( nRegister == 0x0 && nCount < nCountTotal );
	if( nRegister == 0xA1 )
	{
		BYTE bytePos = 0x40 | 0x12;	// 18
		Serial_CommToBoards2( USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x19;		// 25
		Serial_CommToBoards2( USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x17;		// 23
		Serial_CommToBoards2( USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x1E;		// 30
		Serial_CommToBoards2( USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x13;		// 19
		Serial_CommToBoards2( USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x1A;		// 26
		Serial_CommToBoards2( USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
	}
	else
		nRet |= 0x01;
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// LNS初始化参数
	nRegister = 0;
	nCount = 0;
	do
	{
		Serial_ReadBoardID( 2, &nRegister );
		nCount++;
	} while( nRegister == 0x0 && nCount < nCountTotal );
	if( nRegister == 0xA2 )
	{
// 		BYTE bytePos = 0x40 | 0x12;	// 18
// 		Serial_CommToBoards2( USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
	}
	else
		nRet |= 0x02;
	///////////////////////////////////////////////////////
	if( nVACBoardType > 0  )
		return nRet;	// 统一真空控制板

	///////////////////////////////////////////////////////
	// VAC初始化参数
	nRegister = 0;
	nCount = 0;
	do
	{
		Serial_ReadBoardID( 3, &nRegister );
		nCount++;
	} while( nRegister == 0x0 && nCount < nCountTotal );
	if( nRegister == 0xA3 )
	{
		// set interrupt mask register, enable INT0,1,2,3
		Serial_WriteToBoard( USB_SetIMR, TRUE, 0x02 );	// 只接收VAC板中断	//0x07 );
		// 15.04.03 如果设置IER后直接读VAC的ISR，则回读值为0x0F7C
		//			即除了未定义的位，其它所有位都有中断
		// 为避免这种情况，初始化时设置VAC的IER前先回读一次其ISR
		//			此时回读值会为0x0000，可以不做任何处理
		unsigned short* pISR = new unsigned short[2];
		memset(pISR,0,2);
		Serial_ReadISR( 3, pISR );
		delete[] pISR;
		// set INT enable register on VAC board
		Serial_CommToBoards2(	USB_CID_VAC_INT_Enable_Register,
								USB_TM_VAC_Write_16,
								0xFF, 0x0F );
		// "0 (Rly Tst A), 29 (Rly Tst B)" = High
		BYTE bytePos = 0x80 | 0x0;	// 0
		Serial_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x1D;		// 29
		Serial_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		// "Test Relays 0,1,2,3,5,6" = check on
		bytePos = 0x80 | 0x01;		// 1 (Test Rly 0)
		Serial_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x02;		// 2 (Test Rly 1)
		Serial_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x03;		// 3 (Test Rly 2)
		Serial_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x04;		// 4 (Test Rly 3)
		Serial_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x06;		// 6 (Test Rly 5)
		Serial_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x07;		// 7 (Test Rly 6)
		Serial_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		// "En TC0,1,2,3" = High (enables TC1,2,3,4 on VAC diagram)
		bytePos = 0x80 | 0x13;		// 19 (En TC1 Ref.)
		Serial_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x14;		// 20 (En TC2 Ref.)
		Serial_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x15;		// 21 (En TC3 Ref.)
		Serial_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x16;		// 22 (En TC4 Ref.)
		Serial_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
	}
	else
		nRet |= 0x04;
	///////////////////////////////////////////////////////
	return nRet;
}

int WINAPI Serial_InitCard( int nVACBoardType )
{
	static int nInitTimes = 0;
	for(int i=0; i<16; i++ )
		m_cBufPtrs[i] = new char[64];

	if ( !Serial_OpenDevice() )
	{
		Serial_CloseCard();
		return -1;
	}

	while ( !Serial_InitCard_once() )
	{
		if (nInitTimes++ < 10)
			Serial_ResetCard();
		else 
		{
			nInitTimes = 0;
			m_bIs232Ready = FALSE;
			return -1;
		}
	};

	int nRet = Serial_Diag_Init( nVACBoardType );
	return nRet;
}

BOOL WINAPI Serial_CloseCard( void )
{
	if( m_bIs232Ready )
	{
		// set interrupt mask register, mask all off
		Serial_WriteToBoard( USB_SetIMR, TRUE, 0 );
		// Clear port B & D
		Serial_WriteToBoard( USB_ClearDB, FALSE, 0 );
		// stop scan
		Serial_WriteToBoard( USB_StopScan, FALSE, 0 );
		// video off
		Serial_WriteToBoard( USB_Video_OFF, FALSE, 0 );
		m_bIs232Ready = FALSE;
	}

	for(int i=0; i<16; i++ )
	{
		if( m_cBufPtrs[i] != NULL )
		{
			delete m_cBufPtrs[i];
			m_cBufPtrs[i] = NULL;
		}
	}
	FTDI_Close();
	FreeLibrary(m_hmodule);

	return TRUE;
}

BOOL WINAPI Serial_Write( void *pData, DWORD dwBufferSize, DWORD &dwBytesWritten )
{
	if( !m_bIs232Ready )
		return FALSE;

	BOOL bRet = FALSE;
	FT_STATUS ftStatus = FTDI_Write( pData, dwBufferSize, &dwBytesWritten );
	if( ftStatus == FT_OK )
		bRet = TRUE;
	return bRet;
}

int WINAPI Serial_Read( void *Buffer, DWORD &dwBytesReturned )
{
	if( !m_bIs232Ready )
		return 3;
// 	BOOL bRet = FALSE;
// 	DWORD dwRetInBuf;
// 	FT_STATUS ftStatus = FTDI_GetQueueStatus( &dwRetInBuf );
// 	if( dwRetInBuf > 0 )
// 	{
// 		ftStatus = FTDI_Read(Buffer, dwRetInBuf<1024 ? dwRetInBuf : 1024, &dwBytesReturned);
// 		if((ftStatus == FT_OK) || ((ftStatus == FT_IO_ERROR) && (dwBytesReturned > 0)) /* Read timeout */)
// 			bRet = TRUE;
// 	}
// 	return bRet;
//	if( !m_bReady )
//		return -1;
	int nRet = 0;
	DWORD dwRetInBuf;
	FT_STATUS ftStatus = FTDI_GetQueueStatus( &dwRetInBuf );
	if( dwRetInBuf > 0 )
	{
		ftStatus = FTDI_Read(Buffer, dwRetInBuf<1024 ? dwRetInBuf : 1024, &dwBytesReturned);
		if( ftStatus == FT_OK )
			nRet = 0;
		else if( (ftStatus == FT_IO_ERROR) && (dwBytesReturned > 0) )	//* Read timeout */)
			nRet = 2;
	}
	else
		nRet = 1;
	return nRet;
}

BOOL WINAPI Serial_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData )
{
	if( !m_bIs232Ready )
		return FALSE;
//	TRACE( "Serial_Write: bOpcode = 0x%02X, blOperand = 0x%02X, wData = 0x%04X\n", bOpcode, blOperand, wData );
	/*/////////////////////////////////
	I. Opcode only, no operand
		帧头	长度	cC	opcode	帧尾
		CA		04		F4	××	0A
	II. Operand follow opcode
		帧头	长度	cC	opcode	operand					帧尾
		CA		06		F5	××	××(low) 	××(high)	0A
	/////////////////////////////////*/
	m_mutexPortsSerial.Lock();
	DWORD dwBytesWritten = 0, dwLen = 0;;
	BYTE bSnd[frameLen] = {0};
	bSnd[fHead]	= byte_HeadSnd;

	if( blOperand )
	{
		// 有操作数的命令
		dwLen		= frameLen;
		bSnd[fLen]	= byte_LenSnd2;
		bSnd[fCC]	= byte_cCOpr;
		bSnd[fOpc]	= bOpcode;
		bSnd[fOpr1]	= (BYTE)( wData & 0x00FF );	// Low byte operand
		bSnd[fOpr2]	= (BYTE)( wData >> 8 );		// High byte operand
		bSnd[fTail]	= byte_TailSnd;
	}
	else
	{
		// 无操作数的命令
		dwLen		= frameLen2;
		bSnd[fLen]	= byte_LenSnd1;
		bSnd[fCC]	= byte_cCOpc;
		bSnd[fOpc]	= bOpcode;
		bSnd[frameLen2 -1]	= byte_TailSnd;
	}
	Serial_Write(bSnd, dwLen, dwBytesWritten );
//	Sleep( 20 );
	m_mutexPortsSerial.Unlock();

	return TRUE;
}

void WINAPI Serial_CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 )
{
	if( !m_bIs232Ready )
		return;
//	TRACE( "Serial_Comm2: op1 - 4 = 0x%02X, 0x%02X, 0x%02X, 0x%02X\n", bOp1, bOp2, bOp3, bOp4 );
	// Write/read via MCI to/from sub boards.
	/*/////////////////////////////////
	Step1：(always required)
		帧头	长度	cC	opcode	Operand1	帧尾
		CA		06		F5	F1	×× 	××	0A
	Step2: ( always required)
		帧头	长度	cC	opcode	Operand2	帧尾
		CA		06		F5	F1	××	××	0A
	Step3: (if necessary)
		帧头	长度	cC	opcode	Operand3	帧尾
		CA		06		F5	F1	×× 	××	0A
	/////////////////////////////////*/
	m_mutexPortsSerial.Lock();

	DWORD i, dwBytesWritten;
	BYTE bSnd[frameLen];
	bSnd[fHead]	= byte_HeadSnd;
	bSnd[fLen]	= byte_LenSnd2;
	bSnd[fCC]	= byte_cCOpr;
	bSnd[fOpc]	= byte_cCSub;
	bSnd[fOpr1]	= bOp1;
	bSnd[fOpr2]	= bOp2;
	bSnd[fTail]	= byte_TailSnd;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	// To assure that software execution is not overrunning the board communication time window,
	// software waits at least 10uS between >> sending sets of instruction clusters.
	DWORD TickCount1 = GetTickCount();
	for(i=0; i<2000000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
//	Sleep(2000);
	DWORD TickCount2 = GetTickCount();


	bSnd[fOpr1]	= bOp3;
	bSnd[fOpr2]	= bOp4;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	// To assure that software execution is not overrunning the board communication time window,
	// software waits at least 10uS between >> sending sets of instruction clusters.
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
/*
	int nTimes = 0;
	while( nTimes < 10 )
	{
		DWORD dwLen = 0;
		bSnd[fHead]	= byte_HeadSnd;
		// 无操作数的命令
		dwLen		= frameLen2;
		bSnd[fLen]	= byte_LenSnd1;
		bSnd[fCC]	= byte_cCOpc;
		bSnd[fOpc]	= USB_ReadVersion;
		bSnd[frameLen2 -1]	= byte_TailSnd;
		Serial_Write(bSnd, dwLen, dwBytesWritten );
		Sleep(20);
		
		BYTE bData[256];
		DWORD dwBytesReturned;
		int nRet = Serial_Read( bData, dwBytesReturned );
		if( (nRet == 0 ) && (dwBytesReturned == frameLen2)
			&& (bData[fHead] == byte_HeadRec) && (bData[fLen] == byte_LenSnd1) )
		{
			if( bData[fLen+2] == 0x01 && bData[fLen+1] == 0x03 )	// cB,cD = 1,3 means board comm ready
				break;
		}
		nTimes++;
	}
	//	TRACE( "Comm2 AFTER times = %d\n", nTimes );
	if( nTimes == 10 )
	{
//		Serial_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
		// 无操作数的命令
		bSnd[fLen]	= byte_LenSnd1;
		bSnd[fCC]	= byte_cCOpc;
		bSnd[fOpc]	= USB_MCI_COMM_RESET;
		bSnd[frameLen2 -1]	= byte_TailSnd;
		Serial_Write(bSnd, frameLen2, dwBytesWritten );
		TRACE( "Comm Reset\n" );
	}
*/
//	Sleep( 50 );
// 	for(i=0; i<30000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
// 		double x = sqrt(4.);
	m_mutexPortsSerial.Unlock();
}

void WINAPI Serial_CommToBoards3( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, BYTE bOp5, BYTE bOp6 )
{
	if( !m_bIs232Ready )
		return;
//	TRACE( "Serial_Comm3: op1 - 6 = 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\n", bOp1, bOp2, bOp3, bOp4, bOp5, bOp6 );
	/*/////////////////////////////////
	Step1：(always required)
		帧头	长度	cC	opcode	Operand1	帧尾
		CA		06		F5	F1	×× 	××	0A
	Step2: ( always required)
		帧头	长度	cC	opcode	Operand2	帧尾
		CA		06		F5	F1	××	××	0A
	Step3: (if necessary)
		帧头	长度	cC	opcode	Operand3	帧尾
		CA		06		F5	F1	×× 	××	0A
	/////////////////////////////////*/
	m_mutexPortsSerial.Lock();

	DWORD i, dwBytesWritten;
	BYTE bSnd[frameLen];
	bSnd[fHead]	= byte_HeadSnd;
	bSnd[fLen]	= byte_LenSnd2;
	bSnd[fCC]	= byte_cCOpr;
	bSnd[fOpc]	= byte_cCSub;
	bSnd[fOpr1]	= bOp1;
	bSnd[fOpr2]	= bOp2;
	bSnd[fTail]	= byte_TailSnd;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	// To assure that software execution is not overrunning the board communication time window,
	// software waits at least 10uS between >> sending sets of instruction clusters.
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

	bSnd[fOpr1]	= bOp3;
	bSnd[fOpr2]	= bOp4;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	// To assure that software execution is not overrunning the board communication time window,
	// software waits at least 10uS between >> sending sets of instruction clusters.
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

	bSnd[fOpr1]	= bOp5;
	bSnd[fOpr2]	= bOp6;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	// To assure that software execution is not overrunning the board communication time window,
	// software waits at least 10uS between >> sending sets of instruction clusters.
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

//	Sleep( 50 );
// 	for(i=0; i<30000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
// 		double x = sqrt(4.);
	m_mutexPortsSerial.Unlock();
}

void WINAPI Serial_CommToBoardsDirect( BYTE byteLen, BYTE* pbData )
{
	if( !m_bIs232Ready )
		return;
	// Write via MCI to subboards.
//	TRACE( "\nLen = %d, ID = 0x%02X\n", byteLen, pbData[0] );
	if( byteLen == 4 )
		Serial_CommToBoards2( pbData[0], pbData[1], pbData[2], pbData[3] );
	else// if( byteLen == 6 )
		Serial_CommToBoards3( pbData[0], pbData[1], pbData[2], pbData[3], pbData[4], pbData[5] );
}

BOOL WINAPI Serial_WriteToBoardDirect( BYTE bOpcode, BOOL blOperand, BYTE byteLo, BYTE byteHi )
{
	if( !m_bIs232Ready )
		return FALSE;
//	TRACE( "Serial_Write_Direct: bOpcode = 0x%02X, blOperand = 0x%02X, byteLo = 0x%02X, byteHi = 0x%02X\n", bOpcode, blOperand, byteLo, byteHi );
	/*/////////////////////////////////
	I. Opcode only, no operand
		帧头	长度	cC	opcode	帧尾
		CA		04		F4	××	0A
	II. Operand follow opcode
		帧头	长度	cC	opcode	operand					帧尾
		CA		06		F5	××	××(low) 	××(high)	0A
	/////////////////////////////////*/
	m_mutexPortsSerial.Lock();
	DWORD dwBytesWritten, dwLen = 0;;
	BYTE bSnd[frameLen];
	bSnd[fHead]	= byte_HeadSnd;

	if( blOperand )
	{
		// 有操作数的命令
		dwLen		= frameLen;
		bSnd[fLen]	= byte_LenSnd2;
		bSnd[fCC]	= byte_cCOpr;
		bSnd[fOpc]	= bOpcode;
		bSnd[fOpr1]	= byteLo;
		bSnd[fOpr2]	= byteHi;
		bSnd[fTail]	= byte_TailSnd;
	}
	else
	{
		// 无操作数的命令
		dwLen		= frameLen2;
		bSnd[fLen]	= byte_LenSnd1;
		bSnd[fCC]	= byte_cCOpc;
		bSnd[fOpc]	= bOpcode;
		bSnd[frameLen2 -1]	= byte_TailSnd;
	}
	Serial_Write(bSnd, dwLen, dwBytesWritten );

	m_mutexPortsSerial.Unlock();

	if( dwBytesWritten != 6 )
		return FALSE;
	else
		return TRUE;
	return TRUE;
}

BOOL WINAPI Serial_ReadPortInfo( BYTE* pPort )
{
	if( !m_bIs232Ready )
		return FALSE;
//	Serial_WriteToBoard( USB_ReadVersion, FALSE, 0 );
//	Sleep( 50 );

	/*/////////////////////////////////
	I. Opcode only, no operand
		帧头	长度	cC	opcode	帧尾
		CA		04		F4	××	0A
	II. Operand follow opcode
		帧头	长度	cC	opcode	operand					帧尾
		CA		06		F5	××	××(low) 	××(high)	0A
	/////////////////////////////////*/
	m_mutexPortsSerial.Lock();
	DWORD i, dwBytesWritten, dwLen = 0;;
	BYTE bSnd[frameLen];
	bSnd[fHead]	= byte_HeadSnd;
	// 无操作数的命令
	dwLen		= frameLen2;
	bSnd[fLen]	= byte_LenSnd1;
	bSnd[fCC]	= byte_cCOpc;
	bSnd[fOpc]	= USB_ReadVersion;
	bSnd[frameLen2 -1]	= byte_TailSnd;
	Serial_Write(bSnd, dwLen, dwBytesWritten );
//	Sleep(50);
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

	BOOL bRet = FALSE;
	BYTE bData[256];
	ZeroMemory(bData, sizeof(bData));
	DWORD dwBytesReturned;
	int nRet = Serial_Read( bData, dwBytesReturned );
	if( (nRet == 0 ) && (dwBytesReturned == frameLen2)
		&& (bData[fHead] == byte_HeadRec) && (bData[fLen] == byte_LenSnd1) )
	{
		pPort[0] = bData[fLen+1];
		pPort[1] = bData[fLen+2];
		bRet = TRUE;
	}
	m_mutexPortsSerial.Unlock();
	return bRet;
}

BOOL WINAPI Serial_ReadMCIPwr( BOOL bAll, int nChn, unsigned short* pPwr )
{
	if( !m_bIs232Ready )
		return FALSE;
	FTDI_Purge(FT_PURGE_TX);
	BYTE bData[256];
	ZeroMemory(bData, sizeof(bData));
	DWORD dwBytesReturned;
	if( bAll )
	{
		// 一次回读所有通道
//		WORD wData = 0xF0;
//		for( DWORD i=0; i<8; i++ )
//			Serial_WriteToBoard( USB_Read_Pwr, TRUE, wData +i );
		Serial_WriteToBoard( USB_Read_Pwr, TRUE, 0xF2 );
		Serial_WriteToBoard( USB_Read_Pwr, TRUE, 0xF6 );
		Serial_WriteToBoard( USB_Read_Pwr, TRUE, 0xF7 );
		Serial_WriteToBoard( USB_Read_Pwr, TRUE, 0xF5 );
		Serial_WriteToBoard( USB_Read_Pwr, TRUE, 0xF1 );
		Serial_WriteToBoard( USB_Read_Pwr, TRUE, 0xF0 );
		Serial_WriteToBoard( USB_Read_Pwr, TRUE, 0xF3 );
		Serial_WriteToBoard( USB_Read_Pwr, TRUE, 0xF4 );
		Sleep( 50 );
		int nRet = Serial_Read( bData, dwBytesReturned );
		if( (nRet == 0) && (dwBytesReturned == frameLen2*8) )
		{
			int nIndex = 0;
			for( DWORD i=0; i<dwBytesReturned; i++ )
			{
				if( (bData[i] == byte_HeadRec) && (bData[i+1] == byte_LenSnd1) && (bData[i+4] == byte_TailRec) )
				{
//					pPwr[nIndex] = ((bData[i+2] & 0x0F) << 8) | bData[i+3];
					pPwr[nIndex] = (bData[i+2] << 8) | bData[i+3];
					nIndex++;
					i = i +4;
				}
			}
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		// 回读指定通道
		Serial_WriteToBoard( USB_Read_Pwr, TRUE, (WORD)(0xF0 +nChn) );
		Sleep( 50 );
		int nRet = Serial_Read( bData, dwBytesReturned );
		if( (nRet == 0) && (dwBytesReturned == frameLen2) )
		{
			if( (bData[fHead] == byte_HeadRec) && (bData[fLen] == byte_LenSnd1) && (bData[frameLen2-1] == byte_TailRec) )
			{
				*pPwr = ((bData[fLen+1] & 0x0F) << 8) | bData[fLen+2];
				return TRUE;
			}
		}
		return FALSE;
	}
}

BOOL WINAPI Serial_ReadISR( int nMID, unsigned short* pISR )
{
	if( !m_bIs232Ready )
		return FALSE;
	// Read INT source register
	BYTE bOp1 = 0x0;
	BYTE bOp2 = 0x0;
	switch( nMID )
	{
	case	1:
	case	2:
		break;
	case	3:
		bOp1	= USB_CID_VAC_INT_Source_Register;
		bOp2	= USB_TM_VAC_Read_16;
		break;
	case	4:
		bOp1	= USB_CID_SPR_INT_Source_Register;
		bOp2	= USB_TM_SPR_Read_08;
		break;
	default:
		break;
	}
	if( bOp1 == 0x0 )
		return FALSE;

//	Serial_CommToBoards2( bOp1, bOp2, 0, 0 );
//	Sleep( 50 );
	m_mutexPortsSerial.Lock();
	FTDI_Purge(FT_PURGE_TX);
	DWORD i, dwBytesWritten;
	BYTE bSnd[frameLen];
	bSnd[fHead]	= byte_HeadSnd;
	bSnd[fLen]	= byte_LenSnd2;
	bSnd[fCC]	= byte_cCOpr;
	bSnd[fOpc]	= byte_cCSub;
	bSnd[fOpr1]	= bOp1;
	bSnd[fOpr2]	= bOp2;
	bSnd[fTail]	= byte_TailSnd;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	bSnd[fOpr1]	= 0;
	bSnd[fOpr2]	= 0;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	Sleep(50);

	BOOL bRet = FALSE;
	unsigned short nData[256];
	ZeroMemory(nData, sizeof(nData));
	DWORD dwBytesReturned;
	int nRet = Serial_Read( nData, dwBytesReturned );
	if( (nRet == 0) && (dwBytesReturned > 0) )
	{
		for( i=0; i<dwBytesReturned/2; i++ )
		{
			if( nData[i] == 0xD22D )
			{
				pISR[0] = nData[i+1];
				pISR[1] = nData[i+2];
				bRet = TRUE;
			}
		}
	}
	m_mutexPortsSerial.Unlock();
	return bRet;
}

BOOL WINAPI Serial_ReadBoardID( int nMID, unsigned short* pBoardID )
{
	if( !m_bIs232Ready )
		return FALSE;
	BYTE bOp1 = 0x0;
	BYTE bOp2 = 0x0;
	switch( nMID )
	{
	case	1:
		bOp1	= USB_CID_SCN_ReadBoardID;
		bOp2	= USB_TM_SCN_Read_08;
		break;
	case	2:
		bOp1	= USB_CID_LNS_ReadBoardID;
		bOp2	= USB_TM_LNS_Read_08;
		break;
	case	3:
		bOp1	= USB_CID_VAC_ReadBoardID;
		bOp2	= USB_TM_VAC_Read_08;
		break;
	case	4:
		bOp1	= USB_CID_SPR_ReadBoardID;
		bOp2	= USB_TM_SPR_Read_08;
		break;
	default:
		break;
	}
	if( bOp1 == 0x0 )
		return FALSE;

//	Serial_CommToBoards2( bOp1, bOp2, 0, 0 );
//	Sleep( 50 );
	m_mutexPortsSerial.Lock();
	FTDI_Purge(FT_PURGE_TX);
	DWORD i, dwBytesWritten;
	BYTE bSnd[frameLen];
	bSnd[fHead]	= byte_HeadSnd;
	bSnd[fLen]	= byte_LenSnd2;
	bSnd[fCC]	= byte_cCOpr;
	bSnd[fOpc]	= byte_cCSub;
	bSnd[fOpr1]	= bOp1;
	bSnd[fOpr2]	= bOp2;
	bSnd[fTail]	= byte_TailSnd;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	bSnd[fOpr1]	= 0;
	bSnd[fOpr2]	= 0;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

	Sleep( 30 );
	BOOL bRet = FALSE;
	unsigned short nData[256];
	ZeroMemory(nData, sizeof(nData));
	DWORD dwBytesReturned;
	int nRet = Serial_Read( nData, dwBytesReturned );
	if( (nRet == 0) && (dwBytesReturned > 0) )
	{
		for( i=0; i<dwBytesReturned/2; i++ )
		{
			if( nData[i] == 0xD22D )
			{
				pBoardID[0] = nData[i+1];
				bRet = TRUE;
			}
		}
	}
	m_mutexPortsSerial.Unlock();
	return bRet;
}

BOOL WINAPI Serial_ReadGaugeCompr( unsigned short* pGauge )
{
	if( !m_bIs232Ready )
		return FALSE;
	m_mutexPortsSerial.Lock();
	FTDI_Purge(FT_PURGE_TX | FT_PURGE_RX);
	DWORD i, dwBytesWritten;
	BYTE bSnd[frameLen];
	bSnd[fHead]	= byte_HeadSnd;
	bSnd[fLen]	= byte_LenSnd2;
	bSnd[fCC]	= byte_cCOpr;
	bSnd[fOpc]	= byte_cCSub;
	bSnd[fOpr1]	= USB_CID_VAC_ReadThermoCCGComp;
	bSnd[fOpr2]	= USB_TM_VAC_Read_16;
	bSnd[fTail]	= byte_TailSnd;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	bSnd[fOpr1]	= 0;
	bSnd[fOpr2]	= 0;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	Sleep(50);

	BOOL bRet = FALSE;
	unsigned short nData[256];
	ZeroMemory(nData, sizeof(nData));
	DWORD dwBytesReturned;
	int nRet = Serial_Read( nData, dwBytesReturned );
	if( (nRet == 0) && (dwBytesReturned > 0) )
	{
		for( i=0; i<dwBytesReturned/2; i++ )
		{
			if( nData[i] == 0xD22D )
			{
				pGauge[0] = nData[i+1];
				bRet = TRUE;
				break;
			}
		}
	}
	m_mutexPortsSerial.Unlock();
	return bRet;
}

BOOL WINAPI Serial_ReadExternalSwitch( unsigned short* pSwitch )
{
	if( !m_bIs232Ready )
		return FALSE;
	m_mutexPortsSerial.Lock();
	FTDI_Purge(FT_PURGE_TX | FT_PURGE_RX);
	DWORD i, dwBytesWritten;
	BYTE bSnd[frameLen];
	bSnd[fHead]	= byte_HeadSnd;
	bSnd[fLen]	= byte_LenSnd2;
	bSnd[fCC]	= byte_cCOpr;
	bSnd[fOpc]	= byte_cCSub;
	bSnd[fOpr1]	= USB_CID_SPR_ReadExternalSwitch;
	bSnd[fOpr2]	= USB_TM_SPR_Read_08;
	bSnd[fTail]	= byte_TailSnd;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	bSnd[fOpr1]	= 0;
	bSnd[fOpr2]	= 0;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	Sleep(50);

	BOOL bRet = FALSE;
	unsigned short nData[256];
	ZeroMemory(nData, sizeof(nData));
	DWORD dwBytesReturned;
	int nRet = Serial_Read( nData, dwBytesReturned );
	if( (nRet == 0) && (dwBytesReturned > 0) )
	{
		for( i=0; i<dwBytesReturned/2; i++ )
		{
			if( nData[i] == 0xD22D )
			{
				pSwitch[0] = nData[i+1];
				bRet = TRUE;
				break;
			}
		}
	}
	m_mutexPortsSerial.Unlock();
	return bRet;
}

DWORD WINAPI Serial_ReadForVAC( unsigned short nModID, unsigned short* pFrameData, unsigned long *BytesTransferred )
{
	if( !m_bIs232Ready )
		return FALSE;
	m_mutexPortsSerial.Lock();
	FTDI_Purge(FT_PURGE_TX | FT_PURGE_RX);
	DWORD i, dwBytesWritten;
	BYTE bSnd[frameLen];
	bSnd[fHead]	= byte_HeadSnd;
	bSnd[fLen]	= byte_LenSnd2;
	bSnd[fCC]	= byte_cCOpr;
	bSnd[fOpc]	= byte_cCSub;

	bSnd[fOpr1]	= USB_CID_VAC_ReadThermoCCGComp;
	bSnd[fOpr2]	= USB_TM_VAC_Read_16;
	bSnd[fTail]	= byte_TailSnd;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	bSnd[fOpr1]	= 0;
	bSnd[fOpr2]	= 0;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	Sleep(50);

	bSnd[fOpr1]	= USB_CID_SPR_ReadExternalSwitch;
	bSnd[fOpr2]	= USB_TM_SPR_Read_08;
	bSnd[fTail]	= byte_TailSnd;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	bSnd[fOpr1]	= 0;
	bSnd[fOpr2]	= 0;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	Sleep(50);

	DWORD dwBytesReturned;
	int nRet = Serial_Read( pFrameData, dwBytesReturned );
	if( (nRet == 0) && (dwBytesReturned > 0) )
	{
		*BytesTransferred = (unsigned short)dwBytesReturned;
	}
	m_mutexPortsSerial.Unlock();
	return nRet;
}

BOOL WINAPI Serial_ReadLoopbackTest( unsigned short* pData )
{
	if( !m_bIs232Ready )
		return FALSE;
//	Serial_CommToBoards2( 0x5A, 0xAD, 0x0, 0x0 );
	m_mutexPortsSerial.Lock();
	FTDI_Purge(FT_PURGE_TX);
	DWORD i, dwBytesWritten;
	BYTE bSnd[frameLen];
	bSnd[fHead]	= byte_HeadSnd;
	bSnd[fLen]	= byte_LenSnd2;
	bSnd[fCC]	= byte_cCOpr;
	bSnd[fOpc]	= byte_cCSub;
	bSnd[fOpr1]	= 0x5A;
	bSnd[fOpr2]	= 0xAD;
	bSnd[fTail]	= byte_TailSnd;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	bSnd[fOpr1]	= 0;
	bSnd[fOpr2]	= 0;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

	Sleep(30);
	BOOL bRet = FALSE;
	DWORD dwBytesReturned;
	int nRet = Serial_Read( &(pData[1]), dwBytesReturned );
	if( (nRet == 0) && (dwBytesReturned > 0) )
	{
		pData[0] = (unsigned short)(dwBytesReturned/2);
		bRet = TRUE;
	}
	m_mutexPortsSerial.Unlock();
	return bRet;
}

BOOL WINAPI	Serial_BoardRead2432( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred )
{
	if( !m_bIs232Ready )
		return FALSE;
	ZeroMemory(pnFrameData, sizeof(pnFrameData));
//	Serial_CommToBoards2( bOp1, bOp2, bOp3, bOp4 );
	m_mutexPortsSerial.Lock();
	FTDI_Purge(FT_PURGE_TX);
	DWORD i, dwBytesWritten;
	BYTE bSnd[frameLen];
	bSnd[fHead]	= byte_HeadSnd;
	bSnd[fLen]	= byte_LenSnd2;
	bSnd[fCC]	= byte_cCOpr;
	bSnd[fOpc]	= byte_cCSub;
	bSnd[fOpr1]	= bOp1;
	bSnd[fOpr2]	= bOp2;
	bSnd[fTail]	= byte_TailSnd;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	bSnd[fOpr1]	= bOp3;
	bSnd[fOpr2]	= bOp4;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	for(i=0; i<20000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

	Sleep(30);
	BOOL bRet = FALSE;
	int nRet = Serial_Read( &(pnFrameData[1]), *BytesTransferred );
	if( (nRet == 0) && (*BytesTransferred > 0) )
	{
		pnFrameData[0] = (*BytesTransferred)/2;
		bRet = TRUE;
	}
	m_mutexPortsSerial.Unlock();
	return bRet;
}