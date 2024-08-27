// USB_Send.cpp : implementation file
//

#include "stdafx.h"
#include "USB_Send.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUSB_Send

IMPLEMENT_DYNCREATE(CUSB_Send, CWinThread)

CUSB_Send::CUSB_Send()
{
	m_bAutoDelete	= FALSE;
	m_blClose		= FALSE;
	m_blPause		= FALSE;

	// kill event starts out in the signaled state
	m_hEventKill = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_p232Device	= NULL;
	m_hMain			= NULL;
	m_nListIndex	= 0;
}

CUSB_Send::~CUSB_Send()
{
	m_blClose = TRUE;

	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);
}

BOOL CUSB_Send::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	// loop but check for kill notification
	while (WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT)
		MainLoop();

	// avoid entering standard message loop by returning FALSE
	return FALSE;  // TRUE;
}

int CUSB_Send::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUSB_Send, CWinThread)
	//{{AFX_MSG_MAP(CUSB_Send)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSB_Send message handlers

void CUSB_Send::Delete()
{
	// calling the base here won't do anything but it is a good habit
	CWinThread::Delete();

	// acknowledge receipt of kill notification
	VERIFY(SetEvent(m_hEventDead));
}

void CUSB_Send::KillThread()
{
	// Note: this function is called in the context of other threads,
	//  not the thread itself.

	// reset the m_hEventKill which signals the thread to shutdown
	VERIFY(SetEvent(m_hEventKill));
//	m_blClose = TRUE;

	// allow thread to run at higher priority during kill process
	//	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	WaitForSingleObject(m_hEventDead, INFINITE);
	WaitForSingleObject(m_hThread, INFINITE);

	// now delete CWinThread object since no longer necessary
	delete this;
}

void CUSB_Send::MainLoop()
{
	if ( !m_fifoHostMSG.IsFIFOEmpty() )	// 先看是否有命令消息需要发送
	{
		USB_SEM_ControlMessage	hostMSG;

		if ( !CopyMSGfromHost( hostMSG ) )
		{
			Sleep(1);
			return;
		}

		//发送hostMSG
		DWORD dwBytesWritten = 0;
		m_p232Device->Serial_Write( hostMSG.bData, hostMSG.bCtlMSGLength, dwBytesWritten );

		// Delete the sent MSG from Host.
		// 结束处理
		CTimer				myTimer;
		myTimer.Reset( dwZ80COMMTimer_WaitEnd );
		while( !m_blClose )
		{
			if ( GetMSGfromHost( hostMSG ) )
			{
//				State = USB_Transceiver_Listen;
				break;
			}
			else if ( myTimer.IsTimeout() )
			{
#ifdef _DEBUG_ADD_ERRINFO_
				CString	csError;
				csError.Format( "GetMSGfromHost() Error! msgID = %4LX", hostMSG.bMessageID );
				PostErrorInformation( csError );
#endif
				break;
			}
			else
			{
				Sleep(1);
			}
		};
	}
	Sleep(1);
}

/////////////////////////////////////////////////////////////////////////////
// CUSB_Send  down-interface
BOOL CUSB_Send::NotifyMSGInfo( USB_SEM_SIO_Package& sspTemp, UINT nFlag = 0 )
{
	CString csInfo, str;
	for( int i=0; i<sspTemp.msg.bCtlMSGLength; i++ )
	{
		str.Format( "%02X, ", sspTemp.msg.bData[i] );
		csInfo += str;
	}

	if( sspTemp.msg.bType == 0 )
		csInfo = _T("MCI:") + csInfo;
	else
		csInfo = _T("SUB:") + csInfo;

	if ( nFlag == 0 )
		csInfo = _T("P-") + csInfo;
	else 
		csInfo = _T("S-") + csInfo;

	PostErrorInformation( csInfo );

	return TRUE;
}

BOOL CUSB_Send::CopyMSGfromHost( USB_SEM_ControlMessage& hostMSG )
{
	if ( !m_fifoHostMSG.IsFIFOEmpty() )
	{
		USB_SEM_SIO_Package		sspTemp;

		if ( m_fifoHostMSG.CopyHead( sspTemp ) )
		{
			hostMSG = sspTemp.msg;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CUSB_Send::GetMSGfromHost( USB_SEM_ControlMessage& hostMSG )
{
	if ( !m_fifoHostMSG.IsFIFOEmpty() )
	{
		USB_SEM_SIO_Package		sspTemp;

		if ( m_fifoHostMSG.Output( sspTemp ) )
		{
			hostMSG = sspTemp.msg;
			m_SendRate.CountLoopRate();
			
			NotifyMSGInfo( sspTemp, 1 );
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CUSB_Send::PostErrorInformation( CString& csError )
{
	if ( m_fifoError.IsFIFOFull() )
	{
		CString		sspTemp;
		m_fifoError.Output( sspTemp );
	}

	CString	csTickCount;
	CString csTime = CTime::GetCurrentTime().Format("[%H:%M:%S]");
	const DWORD thisTickCount = timeGetTime();
	//csTickCount.Format( "(0x%08X) ", thisTickCount );
	csTickCount.Format( "[%010Ld] ", thisTickCount );
	csTime	+= csTickCount;
	csError = csTime + csError;

	BOOL bRet;
	if( m_fifoError.Input( csError ) != (POSITION)(-1) )
		bRet = TRUE;
	else
		bRet = FALSE;
	PostMessage( m_hMain, WM_USB_SERIAL_SEND, 0, 0 );
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// CUSB_Send  up-interface

BOOL CUSB_Send::PostMSGtoFPGA( USB_SEM_ControlMessage& hostMSG )
{
	static BOOL		IsFIFOFullDisplaied = FALSE;
	// debug
	m_PostRate.CountLoopRate();

	if ( !m_fifoHostMSG.IsFIFOFull() )
	{
		////////////////////////////////////////////////
		if ( IsFIFOFullDisplaied )
		{
			CString	csError;
			csError.Format(" Overflow Relieve: Host->MCI MSG-FIFO free!");
			PostErrorInformation( csError );

			IsFIFOFullDisplaied = FALSE;
		}
		////////////////////////////////////////////////

		////////////////////////////////////////////////
		// If busy, pause Inquirer
		////////////////////////////////////////////////
		if ( m_fifoHostMSG.GetFIFOCount() > 0 )
		{
/*			if( m_MicrocomputerID == SEM_Z80ID_Z80B )
			if( (hostMSG.bMessageID == (BYTE)Z80B_Inquirer) ||
				(hostMSG.bMessageID == (BYTE)Z80B_HeatingFilamentElectricity) )
			{
				return TRUE;
			}

			if ( (m_MicrocomputerID == SEM_Z80ID_Z80A)
				&& (hostMSG.bMessageID == (BYTE)Z80A_Inquirer) )
			{
				return TRUE;
			}
*/		}
		////////////////////////////////////////////////

		////////////////////////////////////////////////
		// Add message package to MSG FIFO
		////////////////////////////////////////////////
		USB_SEM_SIO_Package		sspTemp;
		
		sspTemp.msg = hostMSG;

		POSITION posRet = m_fifoHostMSG.Input( sspTemp );
		if( posRet != (POSITION)(-1) )
		{
			// m_PostRate.CountLoopRate();
			NotifyMSGInfo( sspTemp );
			return TRUE;
		}
		////////////////////////////////////////////////
	}
#ifdef _DEBUG_ADD_ERRINFO_
	else
	{
		BOOL boolNotify = TRUE;

		////////////////////////////////////////////////
		if ( boolNotify && !IsFIFOFullDisplaied )
		{
			CString	csError;
			csError.Format(" Overflow Error: Host->MCI MSG-FIFO Full!");
			PostErrorInformation( csError );

			IsFIFOFullDisplaied = TRUE;
		}
		////////////////////////////////////////////////
	}
#endif

	m_FifoOverflowRate.CountLoopRate();

	return FALSE;
}

BOOL CUSB_Send::GetLastErrorInformation( CString& csError )
{
	if ( !m_fifoError.IsFIFOEmpty() )
	{
		CString		sspTemp;

		if ( m_fifoError.Output( sspTemp ) )
		{
			csError = sspTemp;

			return TRUE;
		}
	}

	return FALSE;
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void CUSB_Send::SetSharedParams( int nIndex, void* pParams )
{
	if( nIndex == 3 )
		m_pSPlist = (SharedParams_Record_List*)pParams;
}

BOOL CUSB_Send::PostMCIOpcr( int nProcessID, BYTE bOpcode, BOOL blOperand, WORD wData )
{
//	TRACE( "Serial_Write: bOpcode = 0x%02X, blOperand = 0x%02X, wData = 0x%04X\n", bOpcode, blOperand, wData );
	/*/////////////////////////////////
	Ⅰ. Opcode only, no operand
		帧头	长度	cC	opcode	帧尾
		CA		04		F4	××	0A
	Ⅱ. Operand follow opcode
		帧头	长度	cC	opcode	operand					帧尾
		CA		06		F5	××	××(low) 	××(high)	0A
	/////////////////////////////////*/
	// 组织数据包
	USB_SEM_ControlMessage	usb_hostMSG;
	usb_hostMSG.bType			= 0x00;	// to MCI
	usb_hostMSG.bData[fHead]	= byte_HeadSnd;
	if( blOperand )
	{
		// 有操作数的命令
		usb_hostMSG.bCtlMSGLength	= frameLen;
		usb_hostMSG.bData[fLen]		= byte_LenSnd2;
		usb_hostMSG.bData[fCC]		= byte_cCOpr;
		usb_hostMSG.bData[fOpc]		= bOpcode;
		usb_hostMSG.bData[fOpr1]	= (BYTE)( wData & 0x00FF );	// Low byte operand
		usb_hostMSG.bData[fOpr2]	= (BYTE)( wData >> 8 );		// High byte operand
		usb_hostMSG.bData[fTail]	= byte_TailSnd;
		// 分析命令码，看是否需要加入回读序列
		AddToReadbackSerial( nProcessID, 0, bOpcode, usb_hostMSG.bData[fOpr1] );
	}
	else
	{
		// 无操作数的命令
		usb_hostMSG.bCtlMSGLength		= frameLen2;
		usb_hostMSG.bData[fLen]			= byte_LenSnd1;
		usb_hostMSG.bData[fCC]			= byte_cCOpc;
		usb_hostMSG.bData[fOpc]			= bOpcode;
		usb_hostMSG.bData[frameLen2 -1]	= byte_TailSnd;
	}

	// 发送数据包
	PostMSGtoFPGA( usb_hostMSG );
	return TRUE;
}

BOOL CUSB_Send::PostSubOpcr( int nProcessID, BYTE bOp1, BYTE bOp2 )
{
	/////////////////////////////////
	// 组织数据包
	USB_SEM_ControlMessage	usb_hostMSG;
	usb_hostMSG.bType			= 0x01;		// to sub boards
	usb_hostMSG.bCtlMSGLength	= frameLen;

	usb_hostMSG.bData[fHead]	= byte_HeadSnd;
	usb_hostMSG.bData[fLen]		= byte_LenSnd2;
	usb_hostMSG.bData[fCC]		= byte_cCOpr;
	usb_hostMSG.bData[fOpc]		= byte_cCSub;
	usb_hostMSG.bData[fOpr1]	= bOp1;
	usb_hostMSG.bData[fOpr2]	= bOp2;
	usb_hostMSG.bData[fTail]	= byte_TailSnd;

	// 分析命令码，看是否需要加入回读序列
	AddToReadbackSerial( nProcessID, 1, bOp1, bOp2 );

	// 发送数据包
	PostMSGtoFPGA( usb_hostMSG );
	return TRUE;

// 	m_mutexPorts.Lock();
}

BOOL CUSB_Send::PostSubOpcr2( int nProcessID, BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 )
{
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
	// 组织数据包
	USB_SEM_ControlMessage	usb_hostMSG;
	usb_hostMSG.bType			= 0x01;	// to sub boards
	usb_hostMSG.bCtlMSGLength	= 0x0E;	// 2*7 = 14 (0x0E)

	usb_hostMSG.bData[fHead]	= byte_HeadSnd;
	usb_hostMSG.bData[fLen]		= byte_LenSnd2;
	usb_hostMSG.bData[fCC]		= byte_cCOpr;
	usb_hostMSG.bData[fOpc]		= byte_cCSub;
	usb_hostMSG.bData[fOpr1]	= bOp1;
	usb_hostMSG.bData[fOpr2]	= bOp2;
	usb_hostMSG.bData[fTail]	= byte_TailSnd;

	usb_hostMSG.bData[frameLen +fHead]	= byte_HeadSnd;
	usb_hostMSG.bData[frameLen +fLen]	= byte_LenSnd2;
	usb_hostMSG.bData[frameLen +fCC]	= byte_cCOpr;
	usb_hostMSG.bData[frameLen +fOpc]	= byte_cCSub;
	usb_hostMSG.bData[frameLen +fOpr1]	= bOp3;
	usb_hostMSG.bData[frameLen +fOpr2]	= bOp4;
	usb_hostMSG.bData[frameLen +fTail]	= byte_TailSnd;

	// 分析命令码，看是否需要加入回读序列
	AddToReadbackSerial( nProcessID, 1, bOp1, bOp2 );

	// 发送数据包
	PostMSGtoFPGA( usb_hostMSG );
	return TRUE;

// 	m_mutexPorts.Lock();
}

BOOL CUSB_Send::PostSubOpcr3( int nProcessID, BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, BYTE bOp5, BYTE bOp6 )
{
	// 组织数据包
	USB_SEM_ControlMessage	usb_hostMSG;
	usb_hostMSG.bType			= 0x01;	// to sub boards
	usb_hostMSG.bCtlMSGLength	= 0x15;	// 3*7 = 21 (0x15)

	usb_hostMSG.bData[fHead]	= byte_HeadSnd;
	usb_hostMSG.bData[fLen]		= byte_LenSnd2;
	usb_hostMSG.bData[fCC]		= byte_cCOpr;
	usb_hostMSG.bData[fOpc]		= byte_cCSub;
	usb_hostMSG.bData[fOpr1]	= bOp1;
	usb_hostMSG.bData[fOpr2]	= bOp2;
	usb_hostMSG.bData[fTail]	= byte_TailSnd;

	usb_hostMSG.bData[frameLen +fHead]	= byte_HeadSnd;
	usb_hostMSG.bData[frameLen +fLen]	= byte_LenSnd2;
	usb_hostMSG.bData[frameLen +fCC]	= byte_cCOpr;
	usb_hostMSG.bData[frameLen +fOpc]	= byte_cCSub;
	usb_hostMSG.bData[frameLen +fOpr1]	= bOp3;
	usb_hostMSG.bData[frameLen +fOpr2]	= bOp4;
	usb_hostMSG.bData[frameLen +fTail]	= byte_TailSnd;

	usb_hostMSG.bData[frameLen*2 +fHead]	= byte_HeadSnd;
	usb_hostMSG.bData[frameLen*2 +fLen]		= byte_LenSnd2;
	usb_hostMSG.bData[frameLen*2 +fCC]		= byte_cCOpr;
	usb_hostMSG.bData[frameLen*2 +fOpc]		= byte_cCSub;
	usb_hostMSG.bData[frameLen*2 +fOpr1]	= bOp5;
	usb_hostMSG.bData[frameLen*2 +fOpr2]	= bOp6;
	usb_hostMSG.bData[frameLen*2 +fTail]	= byte_TailSnd;

	// 分析命令码，看是否需要加入回读序列
	AddToReadbackSerial( nProcessID, 1, bOp1, bOp2 );

	// 发送数据包
	PostMSGtoFPGA( usb_hostMSG );
	return TRUE;
}

BOOL CUSB_Send::PostSubReadADC( int nProcessID, BYTE bModID, BYTE bCID, int nADCnum, int nADCchn, BYTE bTM, WORD wData, BOOL bAnalyze )
{
	// 组织数据包
	USB_SEM_ControlMessage	usb_hostMSG;
	usb_hostMSG.bType			= 0x01;	// to sub boards
	usb_hostMSG.bCtlMSGLength	= 0x0E;	// 2*7 = 14 (0x0E)

	usb_hostMSG.bData[fHead]	= byte_HeadSnd;
	usb_hostMSG.bData[fLen]		= byte_LenSnd2;
	usb_hostMSG.bData[fCC]		= byte_cCOpr;
	usb_hostMSG.bData[fOpc]		= byte_cCSub;
	usb_hostMSG.bData[fOpr1]	= bCID;
	usb_hostMSG.bData[fOpr2]	= bTM;
	usb_hostMSG.bData[fTail]	= byte_TailSnd;

	usb_hostMSG.bData[frameLen +fHead]	= byte_HeadSnd;
	usb_hostMSG.bData[frameLen +fLen]	= byte_LenSnd2;
	usb_hostMSG.bData[frameLen +fCC]	= byte_cCOpr;
	usb_hostMSG.bData[frameLen +fOpc]	= byte_cCSub;
	usb_hostMSG.bData[frameLen +fOpr1]	= (BYTE)(wData & 0xFF);
	usb_hostMSG.bData[frameLen +fOpr2]	= (BYTE)((wData /256) & 0xFF);	// Shift data down 8 bits before assigning to OP4
	usb_hostMSG.bData[frameLen +fTail]	= byte_TailSnd;
	
	// 分析命令码，看是否需要加入回读序列
	m_pSPlist[m_nListIndex].nType	= 0;
	m_pSPlist[m_nListIndex].nNum	= nADCnum;
	m_pSPlist[m_nListIndex].nMID	= bModID;
	m_pSPlist[m_nListIndex].nChn	= nADCchn;
	m_pSPlist[m_nListIndex].bAnalyze= bAnalyze;
	m_nListIndex++;
	if( m_nListIndex > 127 )
		m_nListIndex = 0;
	
	// 发送数据包
	PostMSGtoFPGA( usb_hostMSG );
	return TRUE;
}

void CUSB_Send::AddToReadbackSerial( int nProcessID, BOOL bType, BYTE bOp1, BYTE bOp2 )
{
	if( bType )
	{
		BOOL bNeed = FALSE;
		// 向子板卡发送的命令
		switch( bOp1 )	// CID
		{
		case	USB_CID_VAC_ReadThermoCCGComp:		// 0x94
		case	USB_CID_VAC_ReadSelExtSwitchState:	// 0x98
		case	USB_CID_VAC_INT_Source_Register:	// 0xA4: Read INT source register on VAC & SPR board
		case	USB_CID_SPR_PowerStatus_Readback:	// 0x80
		case	USB_CID_SPR_ReadExternalSwitch:		// 0x92
			bNeed = TRUE;
			break;
		default:
			break;
		}
		if( !bNeed )
			return;

		BYTE bType = 0;	// 0:ADC; 1:gauge compr; 2:VAC switch; 3:SPR switch; 4:SPR power; 5:Board ID; 6:ISR; 7:MCI Pwr
		switch( bOp1 )	// CID
		{
		case	USB_CID_VAC_ReadThermoCCGComp:		// 0x94
			bType	= 1;
			break;
		case	USB_CID_VAC_ReadSelExtSwitchState:	// 0x98
			bType	= 2;
			break;
		case	USB_CID_SPR_ReadExternalSwitch:		// 0x92
			bType	= 3;
			break;
		case	USB_CID_SPR_PowerStatus_Readback:	// 0x80
			bType	= 4;
			break;
		case	USB_CID_SCN_ReadBoardID:			// 0xE1
			bType	= 5;
			break;
		case	USB_CID_VAC_INT_Source_Register:	// 0xA4: Read INT source register on VAC & SPR board
			bType	= 6;
			break;
		default:
			break;
		}
		if( bType == 0 )
			return;
		m_pSPlist[m_nListIndex].nType	= bType;
		m_pSPlist[m_nListIndex].nNum	= 0;
		m_pSPlist[m_nListIndex].nMID	= bOp2 & 0x0F;
		m_pSPlist[m_nListIndex].nChn	= 0;
		m_pSPlist[m_nListIndex].bAnalyze= TRUE;
		m_nListIndex++;
		if( m_nListIndex > 127 )
			m_nListIndex = 0;
	}
	else
	{
		// 向MCI发送的命令
		if( bOp1 != USB_Read_Pwr )
			return;
		m_pSPlist[m_nListIndex].nType	= 7;	// MCI Pwr
		m_pSPlist[m_nListIndex].nNum	= 0;
		m_pSPlist[m_nListIndex].nMID	= 0;
		m_pSPlist[m_nListIndex].nChn	= 0;
		m_pSPlist[m_nListIndex].bAnalyze= TRUE;
		m_nListIndex++;
		if( m_nListIndex > 127 )
			m_nListIndex = 0;
	}
}