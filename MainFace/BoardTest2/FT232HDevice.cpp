// FT232HDevice.cpp : implementation file
//

#include "stdafx.h"
#include "insample0.h"
#include "FT232HDevice.h"

#include <afxmt.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCriticalSection		m_mutexPorts;

/////////////////////////////////////////////////////////////////////////////
// CFT232HDevice

CFT232HDevice::CFT232HDevice()
{
	m_bReady	= FALSE;
	m_dwDevsNum	= 0;
	for( int i=0; i<16; i++ )
		m_cBufPtrs[i] = new char[64];
}

CFT232HDevice::~CFT232HDevice()
{
	for( int i=0; i<16; i++ )
		delete m_cBufPtrs[i];
}


/////////////////////////////////////////////////////////////////////////////
// CFT232HDevice message handlers

//****************************************************************************************
int CFT232HDevice::FTDI_LoadDLL()
{
	m_hmodule = LoadLibrary("Ftd2xx.dll");	
	if(m_hmodule == NULL)
	{
//		AfxMessageBox("Error: Can't Load ft8u245.dll");
		return 1;
	}

	m_pWrite = (PtrToWrite)GetProcAddress(m_hmodule, "FT_Write");
	if (m_pWrite == NULL)
	{
//		AfxMessageBox("Error: Can't Find FT_Write");
		return 2;
	}

	m_pRead = (PtrToRead)GetProcAddress(m_hmodule, "FT_Read");
	if (m_pRead == NULL)
	{
//		AfxMessageBox("Error: Can't Find FT_Read");
		return 3;
	}

	m_pOpen = (PtrToOpen)GetProcAddress(m_hmodule, "FT_Open");
	if (m_pOpen == NULL)
	{
//		AfxMessageBox("Error: Can't Find FT_Open");
		return 4;
	}

	m_pOpenEx = (PtrToOpenEx)GetProcAddress(m_hmodule, "FT_OpenEx");
	if (m_pOpenEx == NULL)
	{
//		AfxMessageBox("Error: Can't Find FT_OpenEx");
		return 5;
	}

	m_pListDevices = (PtrToListDevices)GetProcAddress(m_hmodule, "FT_ListDevices");
	if(m_pListDevices == NULL)
	{
//		AfxMessageBox("Error: Can't Find FT_ListDevices");
		return 6;
	}

	m_pClose = (PtrToClose)GetProcAddress(m_hmodule, "FT_Close");
	if (m_pClose == NULL)
	{
//		AfxMessageBox("Error: Can't Find FT_Close");
		return 7;
	}

	m_pResetDevice = (PtrToResetDevice)GetProcAddress(m_hmodule, "FT_ResetDevice");
	if (m_pResetDevice == NULL)
	{
//		AfxMessageBox("Error: Can't Find FT_ResetDevice");
		return 8;
	}

	m_pPurge = (PtrToPurge)GetProcAddress(m_hmodule, "FT_Purge");
	if (m_pPurge == NULL)
	{
//		AfxMessageBox("Error: Can't Find FT_Purge");
		return 9;
	}

	m_pSetTimeouts = (PtrToSetTimeouts)GetProcAddress(m_hmodule, "FT_SetTimeouts");
	if (m_pSetTimeouts == NULL)
	{
//		AfxMessageBox("Error: Can't Find FT_SetTimeouts");
		return 10;
	}

	m_pGetQueueStatus = (PtrToGetQueueStatus)GetProcAddress(m_hmodule, "FT_GetQueueStatus");
	if (m_pGetQueueStatus == NULL)
	{
//		AfxMessageBox("Error: Can't Find FT_GetQueueStatus");
		return 11;
	}
	return 0;
}	


//****************************************************************************************
FT_STATUS CFT232HDevice::FTDI_Read(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytesRead)
{
	if (!m_pRead)
	{
		AfxMessageBox("FT_Read is not valid!"); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pRead)(m_ftHandle, lpvBuffer, dwBuffSize, lpdwBytesRead);
}	


//****************************************************************************************
FT_STATUS CFT232HDevice::FTDI_Write(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytes)
{
	if (!m_pWrite)
	{
		AfxMessageBox("FT_Write is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pWrite)(m_ftHandle, lpvBuffer, dwBuffSize, lpdwBytes);
}	





//****************************************************************************************
FT_STATUS CFT232HDevice::FTDI_Open(PVOID pvDevice)
{
	if (!m_pOpen)
	{
		AfxMessageBox("FT_Open is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pOpen)(pvDevice, &m_ftHandle );
}	

//****************************************************************************************
FT_STATUS CFT232HDevice::FTDI_OpenEx(PVOID pArg1, DWORD dwFlags)
{
	if (!m_pOpenEx)
	{
		AfxMessageBox("FT_OpenEx is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pOpenEx)(pArg1, dwFlags, &m_ftHandle);
}	


//****************************************************************************************
FT_STATUS CFT232HDevice::FTDI_ListDevices(PVOID pArg1, PVOID pArg2, DWORD dwFlags)
{
	if (!m_pListDevices)
	{
		AfxMessageBox("FT_ListDevices is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pListDevices)(pArg1, pArg2, dwFlags);
}	


//****************************************************************************************
FT_STATUS CFT232HDevice::FTDI_Close()
{
	if (!m_pClose)
	{
		AfxMessageBox("FT_Close is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pClose)(m_ftHandle);
}	



//****************************************************************************************
FT_STATUS CFT232HDevice::FTDI_ResetDevice()
{
	if (!m_pResetDevice)
	{
		AfxMessageBox("FT_ResetDevice is not valid!"); 
		return FT_INVALID_HANDLE;
	}
	
	return (*m_pResetDevice)(m_ftHandle);
}	



//****************************************************************************************
FT_STATUS CFT232HDevice::FTDI_Purge(ULONG dwMask)
{
	if (!m_pPurge)
	{
		AfxMessageBox("FT_Purge is not valid!"); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pPurge)(m_ftHandle, dwMask);
}	



//****************************************************************************************
FT_STATUS CFT232HDevice::FTDI_SetTimeouts(ULONG dwReadTimeout, ULONG dwWriteTimeout)
{
	if (!m_pSetTimeouts)
	{
		AfxMessageBox("FT_SetTimeouts is not valid!"); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pSetTimeouts)(m_ftHandle, dwReadTimeout, dwWriteTimeout);
}	


//****************************************************************************************
FT_STATUS CFT232HDevice::FTDI_GetQueueStatus(LPDWORD lpdwAmountInRxQueue)
{
	if (!m_pGetQueueStatus)
	{
		AfxMessageBox("FT_GetQueueStatus is not valid!"); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pGetQueueStatus)(m_ftHandle, lpdwAmountInRxQueue);
}	





//****************************************************************************************
//****************************************************************************************
//****************************************************************************************
BOOL CFT232HDevice::Serial_Load( CString& strError )
{
	int nRet = FTDI_LoadDLL();
	FTDI_Close();	// must be closed to perform the ListDevices() function
	switch( nRet )
	{
	case 1:
		strError	= "Error: Can't Load ft8u245.dll";
		break;
	case 2:
		strError	= "Error: Can't Find FT_Write";
		break;
	case 3:
		strError	= "Error: Can't Find FT_Read";
		break;
	case 4:
		strError	= "Error: Can't Find FT_Open";
		break;
	case 5:
		strError	= "Error: Can't Find FT_OpenEx";
		break;
	case 6:
		strError	= "Error: Can't Find FT_ListDevices";
		break;
	case 7:
		strError	= "Error: Can't Find FT_Close";
		break;
	case 8:
		strError	= "Error: Can't Find FT_ResetDevice";
		break;
	case 9:
		strError	= "Error: Can't Find FT_Purge";
		break;
	case 10:
		strError	= "Error: Can't Find FT_SetTimeouts";
		break;
	case 11:
		strError	= "Error: Can't Find FT_GetQueueStatus";
		break;
	case 12:
		strError	= "";
		break;
	default:
		break;
	}
	if( nRet > 0 )
		return FALSE;
	else
		return TRUE;
}

BOOL CFT232HDevice::Serial_Open( int index )
{
	FT_STATUS ftStatus = FTDI_Open((LPVOID)index);	//load default device 0
	if( ftStatus == FT_OK )
	{
 		FTDI_ResetDevice();
//		UCHAR BitMode;
//		ftStatus = FT_GetBitMode(m_ftHandle, &BitMode);
		ftStatus = FT_SetBitMode(m_ftHandle, 0xff, 0);
		Sleep(10);
		ftStatus = FT_SetLatencyTimer(m_ftHandle, 16);		// 时间参数设置大则上来的数据慢，默认16ms
//		ftStatus = FT_SetUSBParameters(m_ftHandle,0x10000, 0x10000);			// 设置传输缓冲区大小，默认为4096 bytes
//		ftStatus = FT_SetFlowControl(m_ftHandle, FT_FLOW_RTS_CTS, 0x0, 0x0);	// 设置控制流，串口时需要设置
		ftStatus = FTDI_Purge(FT_PURGE_RX || FT_PURGE_TX);
		Sleep(10);
		ftStatus = FTDI_SetTimeouts(3000, 3000);//extend timeout while board finishes reset

// 		FTDI_Purge(FT_PURGE_RX || FT_PURGE_TX);
// 		FTDI_ResetDevice();
// 		FTDI_SetTimeouts(3000, 3000);//extend timeout while board finishes reset
		m_bReady = TRUE;
		return TRUE;
	}
	else
		return FALSE;
}

void CFT232HDevice::Serial_Close()
{
	FTDI_Close();
	FreeLibrary(m_hmodule);
}

int CFT232HDevice::Serial_ListDevicesCounts()
{
	//search for Descriptions or Serial Numbers depending on the current mode
	FT_STATUS ftStatus;
	ftStatus = FTDI_ListDevices(&m_dwDevsNum, NULL, FT_LIST_NUMBER_ONLY);
	if( ftStatus != FT_OK )
		return -1;
	else
		return (int)m_dwDevsNum;
}

char* CFT232HDevice::Serial_GetDeviceSerialNumber( int nIndex )
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

BOOL CFT232HDevice::Serial_Write( void *pData, DWORD dwBufferSize, DWORD &dwBytesWritten )
{
	if( !m_bReady )
		return FALSE;
	BOOL bRet = FALSE;
	FT_STATUS ftStatus = FTDI_Write( pData, dwBufferSize, &dwBytesWritten );
	if( ftStatus == FT_OK )
		bRet = TRUE;
	return bRet;
}

int CFT232HDevice::Serial_Read( void *Buffer, DWORD &dwBytesReturned )
{
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
	if( !m_bReady )
		return -1;
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

CString CFT232HDevice::Serial_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData )
{
	CString str = "";
	if( !m_bReady )
		return str;

//	TRACE( "Serial_Write: bOpcode = 0x%02X, blOperand = 0x%02X, wData = 0x%04X\n", bOpcode, blOperand, wData );
	/*/////////////////////////////////
	Ⅰ. Opcode only, no operand
		帧头	长度	cC	opcode	帧尾
		CA		04		F4	××	0A
	Ⅱ. Operand follow opcode
		帧头	长度	cC	opcode	operand					帧尾
		CA		06		F5	××	××(low) 	××(high)	0A
	/////////////////////////////////*/
 	m_mutexPorts.Lock();
	DWORD dwBytesWritten, i, dwLen = 0;
	CString strTemp;
	BYTE bSnd[frameLen];
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

	for( i=0; i<dwLen; i++ )
	{
		strTemp.Format( "0x%02X, ", bSnd[i] );
		str += strTemp;
	}
	strTemp.Format( "written total = %ld. ", dwBytesWritten );
	str += strTemp;

	m_mutexPorts.Unlock();
	return str;
}

CString CFT232HDevice::Serial_CommToBoards( BYTE bOp1, BYTE bOp2 )
{
	CString str = "";
	if( !m_bReady )
		return str;
 	m_mutexPorts.Lock();
	
	CString strTemp;
	DWORD dwBytesWritten;
	BYTE bSnd[frameLen];
	bSnd[fHead]	= byte_HeadSnd;
	bSnd[fLen]	= byte_LenSnd2;
	bSnd[fCC]	= byte_cCOpr;
	bSnd[fOpc]	= byte_cCSub;
	bSnd[fOpr1]	= bOp1;
	bSnd[fOpr2]	= bOp2;
	bSnd[fTail]	= byte_TailSnd;
	Serial_Write(bSnd, frameLen, dwBytesWritten );
	
	for( DWORD i=0; i<frameLen; i++ )
	{
		strTemp.Format( "0x%02X, ", bSnd[i] );
		str += strTemp;
	}
	strTemp.Format( "written total = %ld", dwBytesWritten );
	str += strTemp;
	
 	m_mutexPorts.Unlock();
	return str;
}

CString CFT232HDevice::Serial_CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 )
{
	CString str = "";
	if( !m_bReady )
		return str;

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
 	m_mutexPorts.Lock();

	CString strTemp;
	DWORD dwBytesWritten;
	BYTE bSnd[frameLen];
	bSnd[fHead]	= byte_HeadSnd;
	bSnd[fLen]	= byte_LenSnd2;
	bSnd[fCC]	= byte_cCOpr;
	bSnd[fOpc]	= byte_cCSub;
	bSnd[fOpr1]	= bOp1;
	bSnd[fOpr2]	= bOp2;
	bSnd[fTail]	= byte_TailSnd;
	DWORD dwBytesWritten2;
	BYTE bSnd2[frameLen];
	bSnd2[fHead]	= byte_HeadSnd;
	bSnd2[fLen]		= byte_LenSnd2;
	bSnd2[fCC]		= byte_cCOpr;
	bSnd2[fOpc]		= byte_cCSub;
	bSnd2[fOpr1]	= bOp3;
	bSnd2[fOpr2]	= bOp4;
	bSnd2[fTail]	= byte_TailSnd;

	Serial_Write(bSnd, frameLen, dwBytesWritten );
	// To assure that software execution is not overrunning the board communication time window,
	// software waits at least 10uS between >> sending sets of instruction clusters.
	int i;
	for(i=0; i<10000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);

// 	bSnd[fOpr1]	= bOp3;
// 	bSnd[fOpr2]	= bOp4;
// 	Serial_Write(bSnd, frameLen, dwBytesWritten );
	Serial_Write(bSnd2, frameLen, dwBytesWritten2 );
	// To assure that software execution is not overrunning the board communication time window,
	// software waits at least 10uS between >> sending sets of instruction clusters.
	for(i=0; i<10000; i++)					// 1000000次开平方运算16ms，10000次开平方运算16us
		double x = sqrt(4.);
	m_mutexPorts.Unlock();

	for( i=0; i<frameLen; i++ )
	{
		strTemp.Format( "0x%02X, ", bSnd[i] );
		str += strTemp;
	}
	strTemp.Format( "written total = %ld. ", dwBytesWritten );
	str += strTemp;

	for( i=0; i<frameLen; i++ )
	{
		strTemp.Format( "0x%02X, ", bSnd2[i] );
		str += strTemp;
	}
	strTemp.Format( "written total = %ld. ", dwBytesWritten2 );
	str += strTemp;
/*
	BYTE bSnd[frameLen*2];
	bSnd[fHead]	= byte_HeadSnd;
	bSnd[fLen]	= byte_LenSnd2;
	bSnd[fCC]	= byte_cCOpr;
	bSnd[fOpc]	= byte_cCSub;
	bSnd[fOpr1]	= bOp1;
	bSnd[fOpr2]	= bOp2;
	bSnd[fTail]	= byte_TailSnd;
	bSnd[frameLen +fHead]	= byte_HeadSnd;
	bSnd[frameLen +fLen]	= byte_LenSnd2;
	bSnd[frameLen +fCC]		= byte_cCOpr;
	bSnd[frameLen +fOpc]	= byte_cCSub;
	bSnd[frameLen +fOpr1]	= bOp3;
	bSnd[frameLen +fOpr2]	= bOp4;
	bSnd[frameLen +fTail]	= byte_TailSnd;
	Serial_Write(bSnd, frameLen*2, dwBytesWritten );
	for( int i=0; i<frameLen*2; i++ )
	{
		strTemp.Format( "%02x, ", bSnd[i] );
		str += strTemp;
	}
	strTemp.Format( "written total = %ld. ", dwBytesWritten );
	str += strTemp;
*/
	return str;
}

CString CFT232HDevice::Serial_CommToBoards3( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, BYTE bOp5, BYTE bOp6 )
{
	CString str = "";
	if( !m_bReady )
		return str;

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
 	m_mutexPorts.Lock();

	CString strTemp;
	DWORD dwBytesWritten;
	BYTE bSnd[frameLen];
	bSnd[fHead]	= byte_HeadSnd;
	bSnd[fLen]	= byte_LenSnd2;
	bSnd[fCC]	= byte_cCOpr;
	bSnd[fOpc]	= byte_cCSub;
	bSnd[fOpr1]	= bOp1;
	bSnd[fOpr2]	= bOp2;
	bSnd[fTail]	= byte_TailSnd;
	Serial_Write(bSnd, frameLen, dwBytesWritten );

	DWORD i;
	for( i=0; i<frameLen; i++ )
	{
		strTemp.Format( "0x%02X, ", bSnd[i] );
		str += strTemp;
	}
	strTemp.Format( "written total = %ld", dwBytesWritten );
	str += strTemp;

	bSnd[fOpr1]	= bOp3;
	bSnd[fOpr2]	= bOp4;
	Serial_Write(bSnd, frameLen, dwBytesWritten );

	for( i=0; i<frameLen; i++ )
	{
		strTemp.Format( "0x%02X, ", bSnd[i] );
		str += strTemp;
	}
	strTemp.Format( "written total = %ld", dwBytesWritten );
	str += strTemp;

	bSnd[fOpr1]	= bOp5;
	bSnd[fOpr2]	= bOp6;
	Serial_Write(bSnd, frameLen, dwBytesWritten );

	for( i=0; i<frameLen; i++ )
	{
		strTemp.Format( "0x%02X, ", bSnd[i] );
		str += strTemp;
	}
	strTemp.Format( "written total = %ld", dwBytesWritten );
	str += strTemp;

 	m_mutexPorts.Unlock();
	return str;
}

void CFT232HDevice::Serial_CommToBoards_ReadADC( int nProcessID, BYTE bModID, BYTE bCID, BOOL bADCtype, int nADCchn, BYTE bTM, WORD wData )
{

}