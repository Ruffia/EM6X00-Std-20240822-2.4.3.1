// SemCtrl.cpp: implementation of the CSemCtrl class.
//
// Last Modify : 2011.07.19

/////////////////////////////////////////////////////////////////////////////
// 11.07.19 按不同机型设置不同的IoRead循环次数
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl.h"
// VAC State Machine
#include "SMvac.h"
#include "SemCoreAssemble.h"
// USB
#include "UsbDI16A.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// Construction/Destruction
CSemCtrl& CSemCtrl::Instance()
{
	static CSemCtrl Inst;
	return Inst;
}


void  CSemCtrl::Release()
{
	Close();
}

CSemCtrl::CSemCtrl()
{
	// USB
	m_bUSBReady		= FALSE;

	// 21.04
	m_nVACBoardType				= 0;	// 0:LRT VAC Board; 1:KYKY VAC Board；2为KYKY且使用LaB6+IPG
										// 11:场枪真空控制板；12场枪真空控制板（带Standby）
										// 21:PLC真空控制板；22:PLC真空控制板（带Standby）
}

CSemCtrl::~CSemCtrl()
{
	//Close();
}

BOOL CSemCtrl::Close()
{
	m_SemScan.StopScan();
	CMP::Instance().Close();

	// USB
	CUsbDI16A::Instance().USB_CloseCard();

	return TRUE;
}
/*
BOOL CSemCtrl::GetMSG( MicrocomputerID Z80ID, SEM_ControlMessage& mcMSG )
{
	if ( USB_isReady() || MCB_isReady() )
		return CMP::Instance().GetMSG( Z80ID, mcMSG );

	return FALSE;
}
*/
BOOL CSemCtrl::GetLastCommError( MicrocomputerID Z80ID, CString& csError )
{
	if ( m_bUSBReady )//USB_isReady() )
		return CMP::Instance().USB_GetLastError( csError );
//		return CMP::Instance().GetLastError( Z80ID, csError );

	return FALSE;
}

//09.03.23
BOOL CSemCtrl::InitLanguageManager( CLanguageManagerCtrl* pLanguageManCtrl )
{
	if (pLanguageManCtrl == NULL)
		return FALSE;

	CSemCoreAssemble::Instance().BindLanguageManager(pLanguageManCtrl);

	return TRUE;
}

void CSemCtrl::SetVACBoardType( int nType )
{
	m_nVACBoardType = nType;
}

// USB
int CSemCtrl::USB_Init()
{
	m_bUSBReady = TRUE;

    CUsbDI16A::Instance().SetVACBoardType( m_nVACBoardType );
	int nUSB_InitCard = CUsbDI16A::Instance().USB_InitCard();
	if ( nUSB_InitCard < 0 )
	{
		Close();
		m_bUSBReady = FALSE;
#if !defined(_DEBUG_WITHOUT_PCICARD_)
		return -1;
#endif //_DEBUG_WITHOUT_PCICARD_
	}

	if( !m_bUSBReady )
		return -1;

	if ( !CMP::Instance().USB_Init() )
	{
		Close();
		m_bUSBReady = FALSE;
		return -1;
	}

	return nUSB_InitCard;
}

int CSemCtrl::USB_ReinitCommPort()
{
	if( CUsbDI16A::Instance().USB_isCommPortReady() )
		CUsbDI16A::Instance().USB_CloseCommPort();
	Sleep(1);
	return CUsbDI16A::Instance().USB_InitCommPort();
}

int CSemCtrl::USB_ReinitImagePort()
{
	if( CUsbDI16A::Instance().USB_isImagePortReady() )
		CUsbDI16A::Instance().USB_CloseImagePort();
	Sleep(1);
	return CUsbDI16A::Instance().USB_InitImagePort();
}

BOOL CSemCtrl::USB_GetMSG( MicrocomputerID Z80ID, USB_SEM_ControlMessage& mcMSG )
{
	if ( m_bUSBReady )//USB_isReady() )
		return CMP::Instance().USB_GetMSG( /*Z80ID,*/ mcMSG );

	return FALSE;
}

// 向MCI板写数据
// bOpcode:		操作码
// blOperand:	是否有操作数
// wData:		16 bits操作数
BOOL CSemCtrl::USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData )
{
//	return CUsbDI16A::Instance().USB_WriteToBoard( bOpcode, blOperand, wData );

	if( !m_bUSBReady )
		return FALSE;

	USB_SEM_ControlMessage	usb_hostMSG;
	usb_hostMSG.bType			= 0x00;	// to MCI
	usb_hostMSG.bCtlMSGLength	= 0x04;
	usb_hostMSG.bData[0]		= bOpcode;
	usb_hostMSG.bData[1]		= blOperand;
	usb_hostMSG.bData[2]		= (BYTE)( wData & 0x00FF );
	usb_hostMSG.bData[3]		= (BYTE)( wData >> 8 );

	return CMP::Instance().USB_PostMSGEx( usb_hostMSG );
}


// 通过MCI向其它板卡写数据/从其它板卡读数据
// byteLo:	低8位操作数
// byteHi:	高8位操作数
void CSemCtrl::USB_CommToBoards( BYTE byteLo, BYTE byteHi )
{
//	CUsbDI16A::Instance().USB_CommToBoards( byteLo, byteHi );

	if( !m_bUSBReady )
		return;

	USB_SEM_ControlMessage	usb_hostMSG;
	usb_hostMSG.bType			= 0x01;	// to sub boards
	usb_hostMSG.bCtlMSGLength	= 0x02;
	usb_hostMSG.bData[0]		= byteLo;
	usb_hostMSG.bData[1]		= byteHi;

	CMP::Instance().USB_PostMSGEx( usb_hostMSG );
}

void CSemCtrl::USB_CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 )
{
//	CUsbDI16A::Instance().USB_CommToBoards2( bOp1, bOp2, bOp3, bOp4 );

	if( !m_bUSBReady )
		return;

	USB_SEM_ControlMessage	usb_hostMSG;
	usb_hostMSG.bType			= 0x01;	// to sub boards
	usb_hostMSG.bCtlMSGLength	= 0x04;
	usb_hostMSG.bData[0]		= bOp1;
	usb_hostMSG.bData[1]		= bOp2;
	usb_hostMSG.bData[2]		= bOp3;
	usb_hostMSG.bData[3]		= bOp4;

	CMP::Instance().USB_PostMSGEx( usb_hostMSG );
}

void CSemCtrl::USB_CommToBoards3( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, BYTE bOp5, BYTE bOp6 )
{
//	CUsbDI16A::Instance().USB_CommToBoards3( bOp1, bOp2, bOp3, bOp4, bOp5, bOp6 );

	if( !m_bUSBReady )
		return;

	USB_SEM_ControlMessage	usb_hostMSG;
	usb_hostMSG.bType			= 0x01;	// to sub boards
	usb_hostMSG.bCtlMSGLength	= 0x06;
	usb_hostMSG.bData[0]		= bOp1;
	usb_hostMSG.bData[1]		= bOp2;
	usb_hostMSG.bData[2]		= bOp3;
	usb_hostMSG.bData[3]		= bOp4;
	usb_hostMSG.bData[4]		= bOp5;
	usb_hostMSG.bData[5]		= bOp6;

	CMP::Instance().USB_PostMSGEx( usb_hostMSG );
}

BOOL CSemCtrl::USB_GetInterruptNumber( unsigned int& nID )
{
// 	unsigned char input[4];
// 	USB_IoPortReadAll(&input);
// 	if( input[3] == 0x02 )	// port D: 0x01
// 		nID = input[1];		// port B: interrupt number
	return TRUE;
}

BOOL CSemCtrl::USB_ClearPortDB()
{
	// 清除B/D端口的数据
    // software sends acknowledgement to MCI by sending
	// "clear_DB" command, opcode=ox47, no operand.
 
	if( !m_bUSBReady )
		return FALSE;

	USB_SEM_ControlMessage	usb_hostMSG;
	usb_hostMSG.bType			= 0x00;	// to MCI
	usb_hostMSG.bCtlMSGLength	= 0x04;
	usb_hostMSG.bData[0]		= USB_ClearDB;
	usb_hostMSG.bData[1]		= (BYTE)FALSE;
	usb_hostMSG.bData[2]		= 0x00;
	usb_hostMSG.bData[3]		= 0x00;

	return CMP::Instance().USB_PostMSGEx( usb_hostMSG );
}

BOOL CSemCtrl::USB_Interrupt()
{
	unsigned int nID = 0;
	USB_GetInterruptNumber( nID );
	switch( nID )
	{
	case	0:	// SPR board
		break;
	case	1:	// VAC board
		break;
	case	2:	// SCN board
		break;
	default:
		// ReadComprs
		USB_CommToBoards2( USB_CID_VAC_ReadThermoCCGComp, USB_TM_VAC_Read_16, 0x0, 0x0 );
		break;
	}
	USB_ClearPortDB();
	return TRUE;
}

// 通过MCI向其它板卡发起参数回读
BOOL CSemCtrl::USB_Loopback()
{
	// by DD  Jan 24, 2012:
	// Force MCI local loopback to assure alternative RB data from board comm will work.
	// Each time this button is clicked (presuming Scanning has stopped), board_comm block
	// will force 16 words of 0xABCD into To_USB FIFO to be sent to host.
	USB_CommToBoards2( 0x5A, 0xAD, 0x0, 0x0 );
	return TRUE;
}

DWORD CSemCtrl::USB_ReadBoardData( unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred )
{
	if ( m_bUSBReady )//USB_isReady() )
		CUsbDI16A::Instance().USB_LoopbackWords( FramePoints, pnFrameData, BytesTransferred, -1 );
	return 0;
}

BOOL CSemCtrl::USB_LoopbackAndReadData( unsigned short nModID, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred, short nNeedLog )
{
	if ( !m_bUSBReady )//!USB_isReady() )
		return FALSE;

	// set pix_clock to 2
	USB_WriteToBoard( USB_SetPixelRate, TRUE, 2 );

	// 清空FIFO
	USB_ClearFIFO();

	// 发起从FIFO中读状态数据等
	switch( nModID )
	{
	case	0:
		// by DD  Jan 24, 2012:
		// Force MCI local loopback to assure alternative RB data from board comm will work.
		// Each time this button is clicked (presuming Scanning has stopped), board_comm block
		// will force 16 words of 0xABCD into To_USB FIFO to be sent to host.
		USB_CommToBoards2( 0x5A, 0xAD, 0x0, 0x0 );
		Sleep( 25 );
		// 读到数据，并刷新界面状态显示
		CUsbDI16A::Instance().USB_LoopbackWords( FramePoints, pnFrameData, BytesTransferred, nNeedLog );
/*		USB_CommToBoards(	0x5A,		// OP1 = Circuit ID for force 16 r/b words
							0xAD );		// OP2 = Transfer type A for module D(MCI)
		USB_CommToBoards(	0x0,		// OP3
							0x0 );		// OP4
*/		break;
	case	1:
		// ReadComprs
//		USB_CommToBoards2( USB_CID_VAC_ReadThermoCCGComp, USB_TM_VAC_Read_16, 0x0, 0x0 );
		// 读到数据，并刷新界面状态显示
		CUsbDI16A::Instance().USB_LoopbackWords2( nModID, FramePoints, pnFrameData, BytesTransferred, nNeedLog );
		Sleep( 5 );
/*		USB_CommToBoards(	USB_CID_VAC_ReadThermoCCGComp,	// OP1 = ckt addr. of comparator read register on VAC board
							USB_TM_VAC_Read_16 );			// OP2 = 0xA3 (16 bit read from VAC board)
		USB_CommToBoards(	0x0,							// OP3 = 0 (No data to be written)
							0x0 );							// OP4 = 0 (No data to be written)
*/		break;
	case	USB_TTTT_INT:
		USB_Interrupt();
		Sleep( 5 );
		// 读到数据，并刷新界面状态显示
		CUsbDI16A::Instance().USB_LoopbackWords( FramePoints, pnFrameData, BytesTransferred, nNeedLog );
		break;
	case	USB_TTTT_Switches:
		// read selected external switch state
		USB_CommToBoards2( USB_CID_VAC_ReadSelExtSwitchState, USB_TM_VAC_Read_16, 0x0, 0x0 );
		Sleep( 5 );
		// 读到数据，并刷新界面状态显示
		CUsbDI16A::Instance().USB_LoopbackWords( FramePoints, pnFrameData, BytesTransferred, nNeedLog );
		break;
	default:
		// 读到数据，并刷新界面状态显示
		CUsbDI16A::Instance().USB_LoopbackWords( FramePoints, pnFrameData, BytesTransferred, nNeedLog );
		break;
	}

	return TRUE;
}

BOOL CSemCtrl::USB_BoardRead2432( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred )
{
	if ( USB_isReady() )
		return CUsbDI16A::Instance().USB_BoardRead2432( bOp1, bOp2, bOp3, bOp4, FramePoints, pnFrameData, BytesTransferred );
	return FALSE;
}


// Debug
DWORD CSemCtrl::USB_ConfigureEx(void *pOutMask, void *pData, void *pTristateMask)
{
	return CUsbDI16A::Instance().USB_ConfigureEx( pOutMask, pData, pTristateMask );
}

DWORD CSemCtrl::USB_ClearFIFO()
{
	return CUsbDI16A::Instance().USB_ClearFIFO();
}

DWORD CSemCtrl::USB_StreamOpen( unsigned long bIsRead )
{
	return CUsbDI16A::Instance().USB_StreamOpen( bIsRead );
}

DWORD CSemCtrl::USB_StreamFrame( unsigned long FramePoints, unsigned short *pFrameData, unsigned long *BytesTransferred, short nNeedLog )
{
	return CUsbDI16A::Instance().USB_ImageInputStream1( FramePoints, pFrameData, BytesTransferred, nNeedLog );
}

DWORD CSemCtrl::USB_StreamClose()
{
	return CUsbDI16A::Instance().USB_StreamClose();
}

DWORD CSemCtrl::USB_VideoON()
{
	return CUsbDI16A::Instance().USB_VideoON();
}

DWORD CSemCtrl::USB_VideoOFF()
{
	return CUsbDI16A::Instance().USB_VideoOFF();
}

void CSemCtrl::USB_GetPortInfo( BYTE* pPort )
{
	CUsbDI16A::Instance().USB_GetPortInfo( pPort );
}


// Serial FT232H
BOOL CSemCtrl::Serial_ReadbackData( int nMID, int nCID, unsigned short* pData, int nChn /* = 0 */, BOOL bSel /* = FALSE */ )
{
	if( nMID < 1 )
	{
		if( nMID < 0 )
			return CUsbDI16A::Instance().USB_GetLoopbackTest( pData );
		else
			return CUsbDI16A::Instance().USB_GetMCIPwr( bSel, nChn, pData );
	}
	else
	{
		if( nCID == USB_CID_SCN_ReadBoardID )				// 所有ReadBoadID的CID都一样
			return CUsbDI16A::Instance().USB_GetBoardID( nMID, pData );
		else if( nCID == USB_CID_VAC_INT_Source_Register )	// VAC & SPR的Read INT Source Register的CID都一样
			return CUsbDI16A::Instance().USB_GetISR( nMID, pData );
		else if( nCID == USB_CID_VAC_ReadThermoCCGComp )
			return CUsbDI16A::Instance().USB_GetGaugeCompr( pData );
//		else if( nCID == USB_CID_SPR_ReadExternalSwitch )
	}
	return TRUE;
}
