// Vac.cpp : implementation file
//

#include "stdafx.h"
#include "Vac.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CVac
IMPLEMENT_DYNCREATE(CVac, CWinThread)

CVac::CVac()
{
	m_bAutoDelete	= FALSE;

	// kill event starts out in the signaled state
	m_hEventKill	= CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventDead	= CreateEvent(NULL, TRUE, FALSE, NULL);

	m_hTimeDelay	= CreateEvent( NULL, FALSE, TRUE, "TimeDelay" );

	m_bReady		= FALSE;
	m_lastSys		= 0;
	m_lastTC		= 0;
	m_lastTCamp		= 0;
	m_lastTCdelay	= 0;
	m_lastRecord	= 0;
	m_lastVent		= 0;
	m_nErrReads		= 0;

	m_hMain			= NULL;

	m_pnReadData	= new unsigned short[ ReadBack_DataSize +256 ];
	m_hFileVAC		= NULL;
}

CVac::~CVac()
{
	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);
	Close();

	if( m_pnReadData != NULL )
	{
		delete [] m_pnReadData;
		m_pnReadData = NULL;
	}
}

BOOL CVac::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	// loop but check for kill notification
	while( WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT )
	{
		MainLoop();
		Sleep(1);
	}

	// avoid entering standard message loop by returning FALSE
	return FALSE;	// TRUE;
}

int CVac::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CVac, CWinThread)
	//{{AFX_MSG_MAP(CVac)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVac message handlers

void CVac::Delete()
{
	// calling the base here won't do anything but it is a good habit
	CWinThread::Delete();

	// acknowledge receipt of kill notification
	VERIFY(SetEvent(m_hEventDead));
}

void CVac::KillThread()
{
	// Note: this function is called in the context of other threads,
	//  not the thread itself.

	// reset the m_hEventKill which signals the thread to shutdown
	VERIFY(SetEvent(m_hEventKill));

	// allow thread to run at higher priority during kill process
	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	WaitForSingleObject(m_hEventDead, INFINITE);
	WaitForSingleObject(m_hThread, INFINITE);

	// now delete CWinThread object since no longer necessary
	delete this;
}

BOOL CVac::Close()
{
	m_bReady	= FALSE;
	m_hMain		= NULL;

	if( m_hFileVAC != NULL )
		CloseHandle( m_hFileVAC );
	m_hFileVAC	= NULL;

	return TRUE;
}

BOOL CVac::Reset( CWnd* pWnd )
{
	Close();
	m_hMain = pWnd->m_hWnd;

	Init();

	return TRUE;
}

void CVac::Init()
{
	// 各真空参数Reset
	m_VacParamSM.bPower		= FALSE;
	m_VacParamSM.bChamb		= FALSE;
	m_VacParamSM.bGun		= FALSE;
	
	m_VacParamSM.dwTCs		= 0xFFFF;
	m_VacParamSM.dwExtSwi	= 0x0000;

	m_VacParamSM.bTCf		= FALSE;
	m_VacParamSM.bTCc		= FALSE;
	
	m_VacParamSM.bChambEn	= FALSE;
	m_VacParamSM.bGunEn		= FALSE;
	
	m_VacParamSM.bVf		= FALSE;
	m_VacParamSM.bVc		= FALSE;
	m_VacParamSM.bVa		= FALSE;
	m_VacParamSM.bV1		= FALSE;
	
	m_VacParamSM.bTMP		= FALSE;
	m_VacParamSM.bMP		= FALSE;
	m_VacParamSM.bHVInt		= FALSE;
	
	m_VacParamMain.bPower	= FALSE;
	m_VacParamMain.bChamb	= FALSE;
	m_VacParamMain.bGun		= FALSE;
	
	m_VacParamMain.dwTCs	= 0xFFFF;
	m_VacParamMain.dwExtSwi	= 0x0000;

	m_VacParamMain.bTCf		= FALSE;
	m_VacParamMain.bTCc		= FALSE;
	
	m_VacParamMain.bChambEn	= FALSE;
	m_VacParamMain.bGunEn	= FALSE;
	
	m_VacParamMain.bVf		= FALSE;
	m_VacParamMain.bVc		= FALSE;
	m_VacParamMain.bVa		= FALSE;
	m_VacParamMain.bV1		= FALSE;
	
	m_VacParamMain.bTMP		= FALSE;
	m_VacParamMain.bMP		= FALSE;
	m_VacParamMain.bHVInt	= FALSE;

	m_nErrReads	= 0;
	m_nState	= 0;	// All OFF
	m_bDelay	= FALSE;
	m_bChanged	= FALSE;
	m_nMsgType	= -1;
	m_bDebug	= FALSE;
	m_bScanning	= FALSE;
	m_bPause	= TRUE;
	m_bV1CtrlMode	= FALSE;

	if( m_pnReadData != NULL )
		ZeroMemory( m_pnReadData, sizeof(m_pnReadData) );
	m_TCAmp.dTCfAmp	= 0.0;
	m_TCAmp.dTCcAmp	= 0.0;

	m_hFileVAC	= NULL;
	m_nLoopsVAC	= 0;

	m_dCCGvac		= 0.0;
//	m_dTMPThreshold	= 1.3E-1;
	m_dThreshold[0]	= 1.1E-1;
	m_dThreshold[1]	= 1.1E-1;
	m_dThreshold[2]	= 1.1E-1;
	m_dThreshold[3]	= 1.1E-1;
	m_nVacSrc		= 0;
	/////////////////////////////////////////////
	// 设置日志
	CString strTemp, str;
	TCHAR exeFullPath[255];
	::GetModuleFileName( NULL, exeFullPath, 255 );
	char *ptemp = strrchr( exeFullPath, '\\');
	ptemp[1] = 0;
	
	CTime	timeCurrent = CTime::GetCurrentTime();
	strTemp = timeCurrent.Format("%Y%m%d%H%M");
	
	// 设置VAC日志文件
	str.Format( "%s\\Log\\VAC%s.csv", exeFullPath, strTemp );
	if( m_hFileVAC != NULL )
		CloseHandle( m_hFileVAC );
	m_hFileVAC	= NULL;
	m_hFileVAC = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	m_nLoopsVAC = 1;
	// 写VAC日志文件头
	if( m_hFileVAC != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
//		str.Format( "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s", "Time", "bPower", "bChamb", "bGun", "bVf", "bVc", "bTMP", "bVa", "bTCf", "bTCc", "dwTCs", "dwExtSwi", "TCf", "TCc", "CCG" );
		str.Format( "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s", "Time", "bPower", "bChamb", "bGun", "bVf", "bVc", "bTMP", "bVa", "bTCf", "bTCc", "dwTCs", "dwExtSwi", "TCf", "TCc", "CCG" );
		sprintf( Buf, "%s\r\n", str );
		WriteFile( m_hFileVAC, Buf, strlen(Buf), &cbRet, NULL );
	}
}

void CVac::Delay( int msecond )
{
	WaitForSingleObject( m_hTimeDelay, msecond );
}

void CVac::USB_CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 )
{
	if( !CMP::Instance().USB_isReady() )
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

BOOL CVac::USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData )
{
	if( !CMP::Instance().USB_isReady() )
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

BOOL CVac::SetParams( int nType, DWORD dwParam )
{
	// 测试时使用，从测试程序发模拟命令
	switch( nType )
	{
	case	VACIndex_Power:
		m_VacParamMain.bPower	= (BOOL)dwParam;
		if( m_VacParamMain.bPower )
			m_bPause = FALSE;
		break;
	case	VACIndex_Chamb:
		m_VacParamMain.bChamb	= (BOOL)dwParam;
		break;
	case	VACIndex_Gun:
		m_VacParamMain.bGun		= (BOOL)dwParam;
		break;
	case	VACIndex_MP:
		m_VacParamMain.bMP		= (BOOL)dwParam;
		break;
	case	VACIndex_V1:
		m_VacParamMain.bV1		= (BOOL)dwParam;
		break;
	case	VACIndex_Vf:
		m_VacParamMain.bVf		= (BOOL)dwParam;
		break;
	case	VACIndex_Vc:
		m_VacParamMain.bVc		= (BOOL)dwParam;
		USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, m_VacParamMain.bVc );
		break;
	case	VACIndex_TMP:
		m_VacParamMain.bTMP		= (BOOL)dwParam;
		break;
	case	VACIndex_Va:
		m_VacParamMain.bVa		= (BOOL)dwParam;
		break;
	case	VACIndex_TCf:
		m_VacParamMain.bTCf		= (BOOL)dwParam;
		break;
	case	VACIndex_TCc:
		m_VacParamMain.bTCc		= (BOOL)dwParam;
		break;
	case	11:	// bHVInt
		m_VacParamMain.bHVInt	= (BOOL)dwParam;
		break;
	case	12:	// dwTCs
		m_VacParamSM.dwTCs		= dwParam;
		break;
	case	13:	// dwExtSwi
		m_VacParamSM.dwExtSwi	= dwParam;
		break;
	case	14:	// Test LEDs
		USB_CommToBoards2(	USB_CID_SCN_Test_LED,
							USB_TM_SCN_Write_08,
							0x0,
							(BYTE)dwParam );
		USB_CommToBoards2(	USB_CID_LNS_Test_LED,
							USB_TM_LNS_Write_08,
							0x0,
							(BYTE)dwParam );
		USB_CommToBoards2(	USB_CID_VAC_Test_LED,
							USB_TM_VAC_Write_08,
							0x0,
							(BYTE)dwParam );
		USB_CommToBoards2(	USB_CID_SPR_Test_LED,
							USB_TM_SPR_Write_08,
							0x0,
							(BYTE)dwParam );
		break;
	case	15:	// Reset MCI
		USB_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
		break;

	case	20:
		m_bDebug	= (BOOL)dwParam;
		break;
	case	21:
		m_bScanning	= (BOOL)dwParam;
		break;
	case	22:
		m_bPause	= (BOOL)dwParam;
		break;
	case	23:	// 16.07 V1阀控制方式
		m_bV1CtrlMode	= (BOOL)dwParam;
		break;
	case	24:	// 16.07 V1阀控制方式
		USB_CommToBoards2(	RelayEnAddr_V1, USB_TM_VAC_Write_08, 0x0, (BYTE)dwParam );
		break;
	}

	m_bChanged	= TRUE;

	return TRUE;
}

BOOL CVac::SetParams2( int nType, double dParam )
{
	switch( nType )
	{
	case	0:
		m_dCCGvac		= dParam;
		break;
	case	1:
		m_dThreshold[0] = dParam;
		break;
	case	2:
		m_dThreshold[1] = dParam;
		break;
	case	3:
		m_dThreshold[2] = dParam;
		break;
	case	4:
		m_dThreshold[3] = dParam;
		break;
	case	5:
		{
			CString str;
			m_nVacSrc		= (int)dParam;
			str.Format( "TC3 Amp = [%.3f / %.3f]; TC4 Amp = [%.3f / %.3f]", m_dThreshold[0], m_dThreshold[1], m_dThreshold[2], m_dThreshold[3] );
			Record2(str);
			str.Format( "VacSrc = %d", m_nVacSrc );
			Record2(str);
		}
		break;
	}
	return TRUE;
}

void CVac::OnlySendReadSelectedADCcmd( int nADCchan )
{
	BYTE bOP1, bOP2, bOP3, bOP4;
	DWORD dwWdata = 0x0;

	bOP1	= USB_CID_VAC_ADC0_Read;	// Circuit ID 0xC6
	bOP2	= 0xC0 | (BYTE)USB_MID_VAC;	// Upper nibble of OP2 = Transfer Type (C for 32 bit Read)
										// OR the target Module ID into lower nibble of OP2
	// 0xB0 for S.E. measurement
	// 0x90 = 144, which takes ~73mS to complete conversion (not including transfer latencies)
	switch( nADCchan )
	{
	case	2:
//		bChanNib	= 1;
		dwWdata	= 0xB190;	// (0xB0 *256) | (1 *256) | 0x90;
		break;
	case	3:
//		bChanNib	= 9;
		dwWdata = 0xB990;	// (0xB0 *256) | (9 *256) | 0x90;
		break;
	default:
		break;
	}
	bOP3	= (BYTE)(dwWdata & 0xFF);
	bOP4	= (BYTE)((dwWdata /256) & 0xFF);// Shift data down 8 bits before assigning to OP4

	USB_CommToBoards2( bOP1, bOP2, bOP3, bOP4 );
}

BOOL CVac::CheckValidity( int nType, DWORD dwData )
{
	BOOL bRet = FALSE;
	// 检查回读数据有效性
	if( nType < 1 )
	{
		// TC翻转值
		// 首先是其它位的有效性
		if( ((dwData & 0xFF0F) == 0x8F0F) || ((dwData & 0xFF0F) == 0x850C) )
		{
			BYTE bTC34 = (BYTE)((dwData & 0x00F0) >> 4);
			switch( bTC34 )
			{				// TC4	TC3
			case	0xA:	// 10	10	都好
			case	0xB:	// 10	11	TC3不太好，TC4好
			case	0x9:	// 10	01	TC3不好，TC4号
	
			case	0xE:	// 11	10
			case	0xF:	// 11	11
			case	0xD:	// 11	01
	
			case	0x6:	// 01	10
			case	0x7:	// 01	11
			case	0x5:	// 01	01
				bRet	= TRUE;
				break;
			default:
				break;
			}
		}
	}
	else
	{
		// V1回读值
		if( (dwData == 0x0002) || (dwData == 0x0003) )
			bRet = TRUE;
	}
	return bRet;
}

BOOL CVac::ReadStates2()
{
	// 根据CCG值判断是否TC3好

	// 读取TC翻转状态和V1开关状态
	BOOL bRet = FALSE;
	///////////////////////////////////////////////////
	// 读外部开关状态
	if( CUsbDI16A::Instance().USB_GetSwitchStatus(m_pnReadData) )
	{
		if( CheckValidity( 1, m_pnReadData[0] ) )
		{
			m_VacParamSM.dwExtSwi	= m_pnReadData[0];
			m_VacParamMain.dwExtSwi	= m_pnReadData[0];
			// 更新状态机的外部开关参数
			// V1
			if( (m_VacParamSM.dwExtSwi & 0x0001) == 0x0001 )
				m_VacParamSM.bV1	= TRUE;
			else
				m_VacParamSM.bV1	= FALSE;
			// MP
// 			if( (m_VacParamSM.dwExtSwi & 0x0002) == 0x0002 )
// 				m_VacParamSM.bMP	= TRUE;
// 			else
// 				m_VacParamSM.bMP	= FALSE;
			m_VacParamSM.bMP	= TRUE;	// 先默认开，以后通过读取外部开关来更新状态
			bRet = TRUE;
		}
	}
	return bRet;
}

BOOL CVac::ReadStates()
{
	BOOL bRet = FALSE;
	///////////////////////////////////////////////////
	// 读一次FIFO
	unsigned long lBytesTransferred = 0;
	if( !(CUsbDI16A::Instance().USB_LoopbackWords2( 1, ReadBack_DataSize, m_pnReadData, &lBytesTransferred, -1 )) )
	{
//		unsigned long lCount = m_pnReadData[0];
//		if( lCount == 12 || lCount == 10 )	// 得到的数据长度正确2*6或者2*5
		unsigned long lCount = lBytesTransferred /2;
		if( lCount == 8 )	// 得到的数据长度正确2*4
		{
			BOOL bFound = FALSE;
			unsigned long i=0;
			for( i=0; i<lCount; i++ )
			{
				// 直到找到0xD22D才开始计数，前边的数据忽略
				if( m_pnReadData[i] == 0xD22D )
					bFound = TRUE;
				if( !bFound )
					continue;
				// 0xD22D后面的数就是需要的状态数据
				if( CheckValidity( 0, m_pnReadData[i+1] ) )
				{
					m_VacParamSM.dwTCs	= m_pnReadData[i+1];
					m_VacParamMain.dwTCs= m_pnReadData[i+1];
					// 更新状态机的TC参数
					// TCFore
					if( !m_VacParamSM.bTCf && ((m_VacParamSM.dwTCs & 0x0010) == 0x0000) )		// 高于高点
						m_VacParamSM.bTCf	= TRUE;
	 				else if( m_VacParamSM.bTCf && ((m_VacParamSM.dwTCs & 0x0020) == 0x0000) )	// 低于低点(真空施密特)
//					else if( m_VacParamSM.bTCf && ((m_VacParamSM.dwTCs & 0x0010) == 0x0010) )	// 实验时用
						m_VacParamSM.bTCf	= FALSE;
					// TCColumn
					if( !m_VacParamSM.bTCc && ((m_VacParamSM.dwTCs & 0x0040) == 0x0000) )		// 高于高点
						m_VacParamSM.bTCc	= TRUE;
					else if( m_VacParamSM.bTCc && ((m_VacParamSM.dwTCs & 0x0080) == 0x0000) )	// 低于低点
						m_VacParamSM.bTCc	= FALSE;
				}
				break;
			}

			unsigned long j = i +2;
			bFound = FALSE;
			for( i=j; i<lCount; i++ )
			{
				// 直到找到0xD22D才开始计数，前边的数据忽略
				if( m_pnReadData[i] == 0xD22D )
					bFound = TRUE;
				if( !bFound )
					continue;
				// 0xD22D后面的数就是需要的状态数据
				if( CheckValidity( 1, m_pnReadData[i+1] ) )
				{
					m_VacParamSM.dwExtSwi	= m_pnReadData[i+1];
					m_VacParamMain.dwExtSwi	= m_pnReadData[i+1];
					// 更新状态机的外部开关参数
					// V1
					if( (m_VacParamSM.dwExtSwi & 0x0001) == 0x0001 )
						m_VacParamSM.bV1	= TRUE;
					else
						m_VacParamSM.bV1	= FALSE;
					// MP
// 					if( (m_VacParamSM.dwExtSwi & 0x0002) == 0x0002 )
// 						m_VacParamSM.bMP	= TRUE;
// 					else
// 						m_VacParamSM.bMP	= FALSE;
					m_VacParamSM.bMP	= TRUE;	// 先默认开，以后通过读取外部开关来更新状态
					bRet = TRUE;
				}
				break;
			}
		}
	}
	return bRet;
}

BOOL CVac::ReadTCsAmp()
{
	// 读取TC Amp
	///////////////////////////////////////////////////
	// 先发送读取命令
	int msecond = 100;
	// TCf
	OnlySendReadSelectedADCcmd(2);
	OnlySendReadSelectedADCcmd(2);
	Delay( msecond );
	OnlySendReadSelectedADCcmd(2);
	Delay( msecond );
	// TCc
	OnlySendReadSelectedADCcmd(3);
	OnlySendReadSelectedADCcmd(3);
	Delay( msecond );
	OnlySendReadSelectedADCcmd(3);
	Delay( msecond );

	BOOL bRet = FALSE;
	// 读一次FIFO
	unsigned long lBytesTransferred = 0;
	if( !(CUsbDI16A::Instance().USB_LoopbackWords( ReadBack_DataSize, m_pnReadData, &lBytesTransferred, -1 )) )
	{
//		unsigned long lCount = m_pnReadData[0];
//		if( lCount == 36 )	// 得到的数据长度正确6*6
		unsigned long lCount = lBytesTransferred /2;
		if( lCount == 24 )	// 得到的数据长度正确6*4
		{
			int nD22DCount = 0;
			unsigned long i=0;
			for( i=0; i<lCount; i++ )
			{
				if( m_pnReadData[i] == 0xD22D )
					nD22DCount++;
				if( nD22DCount == 3 )
				{
					nD22DCount = 0;
					if( (m_pnReadData[i+1] != 0x0) && (m_pnReadData[i+2] != 0x0) )
					{
						if( ADCvconv2( m_pnReadData[i+1], m_pnReadData[i+2], m_TCAmp.dTCfAmp ) )
							PostMessage( m_hMain, WM_USER_VAC_TCAmp, (WPARAM)( &m_TCAmp ), (LPARAM)0 );
						break;
					}
				}
			}
			unsigned long j = i +2;
			for( i=j; i<lCount; i++ )
			{
				if( m_pnReadData[i] == 0xD22D )
					nD22DCount++;
				if( nD22DCount == 3 )
				{
					if( (m_pnReadData[i+1] != 0x0) && (m_pnReadData[i+2] != 0x0) )
					{
						if( ADCvconv2( m_pnReadData[i+1], m_pnReadData[i+2], m_TCAmp.dTCcAmp ) )
						{
							PostMessage( m_hMain, WM_USER_VAC_TCAmp, (WPARAM)( &m_TCAmp ), (LPARAM)1 );
							bRet = TRUE;
						}
						break;
					}
				}
			}
		}
		else
		{
			CString str, strMsg = "";
			str.Format( "lCount = %d: ", lCount );
			TRACE(str);
/*			if( lCount < 64 )	// 512 /8 = 64（TRACE语句最多输出512个字符）
			{
				for( unsigned long i=0; i<lCount; i++ )
				{
					str.Format( "0x%04X, ", m_pnReadData[i] );	// 一次输出8个字符
					strMsg += str;
				}
				TRACE( "%s\n", strMsg );
			}*/
		}
	}
	if( bRet && m_nVacSrc > 0 )	// 用TC Amp判断TC3/4好坏
	{
		// TCFore
		if( !m_VacParamSM.bTCf && (m_TCAmp.dTCfAmp > m_dThreshold[0]) )		// 高于高点
			m_VacParamSM.bTCf	= TRUE;
		else if( m_VacParamSM.bTCf && (m_TCAmp.dTCfAmp < m_dThreshold[1]) )	// 低于低点(真空施密特)
			m_VacParamSM.bTCf	= FALSE;
		// TCColumn
		if( !m_VacParamSM.bTCc && (m_TCAmp.dTCcAmp > m_dThreshold[2]) )		// 高于高点
			m_VacParamSM.bTCc	= TRUE;
		else if( m_VacParamSM.bTCc && (m_TCAmp.dTCcAmp < m_dThreshold[3]) )	// 低于低点
			m_VacParamSM.bTCc	= FALSE;
	}
	return bRet;
}

void CVac::Record()
{
	unsigned long cbRet;
	char Buf[255];
	CString str, strVAC;

//	strVAC.Format("%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t0x%04X,\t0x%04X,\t%.3f,\t%.3f,\t%.2e",
	strVAC.Format("%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t0x%04X,\t0x%04X,\t%.3f,\t%.3f,\t%.2e",
					m_VacParamSM.bPower, m_VacParamSM.bChamb, m_VacParamSM.bGun,
					m_VacParamSM.bVf, m_VacParamSM.bVc, m_VacParamSM.bTMP, m_VacParamSM.bVa,
					m_VacParamSM.bTCf, m_VacParamSM.bTCc, m_VacParamSM.dwTCs, m_VacParamSM.dwExtSwi,
					m_TCAmp.dTCfAmp, m_TCAmp.dTCcAmp, m_dCCGvac );

	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%Y/%m/%d %H:%M:%S");
	str.Format( "[%s],%s", csCurrentTime, strVAC );

	sprintf(Buf, "%s\r\n", str );
	WriteFile(m_hFileVAC, Buf, strlen(Buf), &cbRet, NULL);

	m_nLoopsVAC++;
	if( m_nLoopsVAC > 10000 )
	{
		CloseHandle( m_hFileVAC );
		// 设置日志文件
		TCHAR exeFullPath[255];
		::GetModuleFileName( NULL, exeFullPath, 255 );
		char *ptemp = strrchr( exeFullPath, '\\');
		ptemp[1] = 0;
		CTime	timeCurrent = CTime::GetCurrentTime();
		csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
		str.Format( "%s\\Log\\VAC%s.csv", exeFullPath, csCurrentTime );
		m_hFileVAC = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		m_nLoopsVAC = 1;

//		str.Format( "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s", "Time", "bPower", "bChamb", "bGun", "bVf", "bVc", "bTMP", "bVa", "bTCf", "bTCc", "dwTCs", "dwExtSwi", "TCf", "TCc", "CCG" );
		str.Format( "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s", "Time", "bPower", "bChamb", "bGun", "bVf", "bVc", "bTMP", "bVa", "bTCf", "bTCc", "dwTCs", "dwExtSwi", "TCf", "TCc", "CCG" );
		sprintf( Buf, "%s\r\n", str );
		WriteFile( m_hFileVAC, Buf, strlen(Buf), &cbRet, NULL );
	}
}

void CVac::Record2( CString strRcd )
{
	if( m_hFileVAC != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		CString str;
		CTime	timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime = timeCurrent.Format("%Y/%m/%d %H:%M:%S");
		str.Format( "[%s],%s", csCurrentTime, strRcd );
		sprintf(Buf, "%s\r\n", str );
		WriteFile(m_hFileVAC, Buf, strlen(Buf), &cbRet, NULL);
	}
}

void CVac::MainLoop()
{
	DWORD thisTickCount;

	if( m_bPause )
	{
		Sleep( 1 );
		return;
	}

	// 真空电源关，即使正在扫描，也可以关真空电源
	if( m_VacParamMain.bPower != m_VacParamSM.bPower && !(m_VacParamMain.bPower) )
	{
		FuncPowerClose( &m_nState );
		if( m_bChanged || m_nMsgType > 0 )
		{
			PostMessage( m_hMain, WM_USER_VAC_MSG, (WPARAM)( &m_VacParamMain ), (LPARAM)m_nMsgType );
			m_bChanged = FALSE;
			m_nMsgType = -1;
		}
	}

	// 查询并更新状态
	thisTickCount = GetTickCount();
	if( thisTickCount - m_lastSys < 50 )	// 50ms
	{
		Sleep( 1 );
		return;
	}
	m_lastSys = thisTickCount;

	if( !(m_VacParamMain.bPower) )	// 未开真空电源，返回
	{
		if( m_VacParamMain.bV1 && m_nMsgType < 0 )
		{
			m_nMsgType	= 2;
//			PostMessage( m_hMain, WM_USER_VAC_MSG, (WPARAM)( &m_VacParamMain ), (LPARAM)m_nMsgType );	// 提示用户关V1
			m_nMsgType	= 0;
		}
		return;
	}
	// 真空电源开
	if( m_VacParamMain.bPower != m_VacParamSM.bPower && m_VacParamMain.bPower )
	{
		// 读一次ADC，防止想读真正的ADC时读到错误的极值
		BYTE bOp1, bOp2, bOp3, bOp4;
		bOp1	= USB_CID_VAC_ADC0_Read;
		bOp2	= USB_TM_VAC_Read_32;
		bOp3	= 0x90;
		bOp4	= 0xB1;
		USB_CommToBoards2( bOp1, bOp2, bOp3, bOp4 );
		Delay(10);
		// 读一次FIFO
		unsigned long lBytesTransferred = 0;
		CUsbDI16A::Instance().USB_LoopbackWords( ReadBack_DataSize, m_pnReadData, &lBytesTransferred, -1 );
	}

	// 1. 回读并更新，包括TCs和外部开关（如V1和MP）
	if( m_bDebug )
	{
		// 调试模式下不连下位机不回读，直接判断并更新
		// 更新状态机的TC参数
		// TCFore
		if( !m_VacParamSM.bTCf && ((m_VacParamSM.dwTCs & 0x0010) == 0x0000) )		// 高于高点
			m_VacParamSM.bTCf	= TRUE;
		else if( m_VacParamSM.bTCf && ((m_VacParamSM.dwTCs & 0x0020) == 0x0000) )	// 低于低点(真空施密特)
//		else if( m_VacParamSM.bTCf && ((m_VacParamSM.dwTCs & 0x0010) == 0x0010) )	// 实验时用
			m_VacParamSM.bTCf	= FALSE;
		// TCColumn
		if( !m_VacParamSM.bTCc && ((m_VacParamSM.dwTCs & 0x0040) == 0x0000) )		// 高于高点
			m_VacParamSM.bTCc	= TRUE;
		else if( m_VacParamSM.bTCc && ((m_VacParamSM.dwTCs & 0x0080) == 0x0000) )	// 低于低点
			m_VacParamSM.bTCc	= FALSE;

		// 更新状态机的外部开关参数
		// V1
		if( (m_VacParamSM.dwExtSwi & 0x0001) == 0x0001 )
			m_VacParamSM.bV1	= TRUE;
		else
			m_VacParamSM.bV1	= FALSE;
		// MP
// 		if( (m_VacParamSM.dwExtSwi & 0x0002) == 0x0002 )
// 			m_VacParamSM.bMP	= TRUE;
// 		else
// 			m_VacParamSM.bMP	= FALSE;
		m_VacParamSM.bMP	= TRUE;	// 先默认开，以后通过读取外部开关来更新状态
	}
	else
	{
		BOOL bRead1, bRead2;
		bRead1 = bRead2 = TRUE;
		// TCs gauge comparators
		if( thisTickCount -m_lastTC > 1000 )
		{
			if( m_nVacSrc < 1 )
				bRead1 = ReadStates();
			else
				bRead1 = ReadStates2();
			m_lastTC = GetTickCount();
		}
		///////////////////////////////////////////////////

		///////////////////////////////////////////////////
		// TCs Amp readback
		if( thisTickCount -m_lastTCamp > 1000 )
		{
			bRead2 = ReadTCsAmp();
			m_lastTCamp = GetTickCount();
		}
		///////////////////////////////////////////////////
		if( !bRead1 || !bRead2 )
			m_nErrReads++;
		if( m_nErrReads > 10 )
		{
			CUsbDI16A::Instance().USB_WriteToBoard( USB_MCI_COMM_RESET, FALSE, 0 );
			TRACE( "Reset sub boards from VACStateMachine\n" );
			m_nErrReads = 0;
			PostMessage( m_hMain, WM_USER_VAC_MSG, 0, 51 );
		}
	}

	// 2. 判断事件，转至相应函数处理
	// 使用if...else...，表示有优先级
	// 真空电源开
	if( m_VacParamMain.bPower != m_VacParamSM.bPower && m_VacParamMain.bPower )
		FuncPowerOpen( &m_nState );

	// V1开
	else if( m_VacParamMain.bV1 != m_VacParamSM.bV1 && m_VacParamSM.bV1 )
		FuncV1Open( &m_nState );
	// V1关
	else if( m_VacParamMain.bV1 != m_VacParamSM.bV1 && !(m_VacParamSM.bV1) )
		FuncV1Close( &m_nState );

	// TCFore变好
	if( m_VacParamMain.bTCf != m_VacParamSM.bTCf && m_VacParamSM.bTCf )
		FuncTCForelOK( &m_nState );
	// TCFore变坏
	else if( m_VacParamMain.bTCf != m_VacParamSM.bTCf && !(m_VacParamSM.bTCf) )
		FuncTCForelBad( &m_nState );

	// TCColumn变好
	else if( m_VacParamMain.bTCc != m_VacParamSM.bTCc && m_VacParamSM.bTCc )
		FuncTCColumnOK( &m_nState );
	// TCColumn变坏
	else if( m_VacParamMain.bTCc != m_VacParamSM.bTCc && !(m_VacParamSM.bTCc) )
		FuncTCColumnBad( &m_nState );

	// 样品室放气开
	else if( m_VacParamMain.bChamb != m_VacParamSM.bChamb && m_VacParamMain.bChamb )
		FuncVentOpen( &m_nState );
	// 样品室放气关
	else if( m_VacParamMain.bChamb != m_VacParamSM.bChamb && !(m_VacParamMain.bChamb) )
		FuncVentClose( &m_nState );

	// 电子枪放气开
	else if( m_VacParamMain.bGun != m_VacParamSM.bGun && m_VacParamMain.bGun )
		FuncVentOpen( &m_nState );
	// 电子枪放气关
	else if( m_VacParamMain.bGun != m_VacParamSM.bGun && !(m_VacParamMain.bGun) )
		FuncVentClose( &m_nState );

	// 强制TMP开
	else if( m_VacParamMain.bTMP != m_VacParamSM.bTMP && m_VacParamMain.bTMP )
		FuncTMPOpen( &m_nState );
	// 强制TMP关
	else if( m_VacParamMain.bTMP != m_VacParamSM.bTMP && !(m_VacParamMain.bTMP) )
		FuncTMPClose( &m_nState );

	// 强制开前级阀
	else if( m_VacParamMain.bVf != m_VacParamSM.bVf && m_VacParamMain.bVf )
		FuncVfOpen( &m_nState );
	// 强制关前级阀
	else if( m_VacParamMain.bVf != m_VacParamSM.bVf && !(m_VacParamMain.bVf) )
		FuncVfClose( &m_nState );

	// 放气阀开后开始计时，若5分钟内未有主动抽气，则主动关闭放气阀，其它不变
	// 强制关放气阀
	if( m_VacParamSM.bVa && m_lastVent > 0 
		&& GetTickCount() - m_lastVent > 300000 )	// 1000*60*5
	{
		USB_CommToBoards2(	RelayEnAddr_Va, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		m_VacParamMain.bVa		= FALSE;
		m_VacParamSM.bVa		= FALSE;
		m_lastVent = 0;
		m_nMsgType	= 13;
		m_bChanged	= TRUE;
	}

	if( m_bChanged || m_nMsgType > 0 )
	{
		PostMessage( m_hMain, WM_USER_VAC_MSG, (WPARAM)( &m_VacParamMain ), (LPARAM)m_nMsgType );
		m_bChanged = FALSE;
		m_nMsgType = -1;
	}

	if( thisTickCount -m_lastRecord > 500 )
	{
		Record();
		m_lastRecord = GetTickCount();
	}

	Sleep( 1 );
}

void CVac::VAC_InitCtrls( BOOL bCtrlBySoft ) 
{
	if( bCtrlBySoft )
	{
		// "0 (Rly Tst A), 29 (Rly Tst B)" = High
		BYTE bytePos = 0x80 | 0x0;	// 0
		USB_CommToBoards2( USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x1D;		// 29
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		// "Test Relays 0,1,2,3,4,5,6,7" = check on
		bytePos = 0x80 | 0x01;		// 1 (Test Rly 0)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x02;		// 2 (Test Rly 1)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x03;		// 3 (Test Rly 2)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x04;		// 4 (Test Rly 3)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x05;		// 6 (Test Rly 4)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x06;		// 6 (Test Rly 5)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x07;		// 7 (Test Rly 6)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		if( m_bV1CtrlMode )			// V1阀控制方式为气动时使用
		{
			bytePos = 0x80 | 0x08;	// 8 (Test Rly 7)
			USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		}
		// "En TC0,1,2,3" = High (enables TC1,2,3,4 on VAC diagram)
		bytePos = 0x80 | 0x13;		// 19 (En TC1 Ref.)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x14;		// 20 (En TC2 Ref.)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x15;		// 21 (En TC3 Ref.)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x80 | 0x16;		// 22 (En TC4 Ref.)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
	}
	else
	{
		// "En TC0,1,2,3" = Low (enables TC1,2,3,4 on VAC diagram)
		BYTE bytePos = 0x40 | 0x13;	// 19 (En TC1 Ref.)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x14;		// 20 (En TC2 Ref.)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x15;		// 21 (En TC3 Ref.)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x16;		// 22 (En TC4 Ref.)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		// "Test Relays 0,1,2,3,5,6,7" = check off
		bytePos = 0x40 | 0x01;		// 1 (Test Rly 0)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x02;		// 2 (Test Rly 1)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x03;		// 3 (Test Rly 2)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x04;		// 4 (Test Rly 3)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x05;		// 6 (Test Rly 4)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x06;		// 6 (Test Rly 5)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x07;		// 7 (Test Rly 6)
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x08;		// 8 (Test Rly 7)，V1阀控制方式为气动时使用
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		// "0 (Rly Tst A), 29 (Rly Tst B)" = Low
		bytePos = 0x40 | 0x0;		// 0
		USB_CommToBoards2( USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
		bytePos = 0x40 | 0x1D;		// 29
		USB_CommToBoards2(	USB_CID_VAC_ExpanderChip, USB_TM_VAC_Write_08, 0x0, bytePos );
	}
}

void CVac::FuncPowerOpen( int * nxt_state )
{
	int cur_state = *nxt_state;

	switch( cur_state )
	{
	case	vacAllOFF:
		// 执行动作
		VAC_InitCtrls( TRUE );
		// 开前级阀
		if( !m_VacParamSM.bVf )
			USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
		// 更新状态
		m_VacParamMain.bMP	= TRUE;
		m_VacParamSM.bMP	= TRUE;
		m_VacParamMain.bVf	= TRUE;
		m_VacParamSM.bVf	= TRUE;
		m_VacParamSM.bPower	= m_VacParamMain.bPower;
		m_VacParamMain.bVf	= m_VacParamSM.bVf;
		m_VacParamMain.bVc	= m_VacParamSM.bVc;
		m_VacParamMain.bTMP	= m_VacParamSM.bTMP;
		m_VacParamMain.bV1	= m_VacParamSM.bV1;
		m_nMsgType	= -1;
		m_bChanged	= TRUE;
		*nxt_state	= vacMP;
		break;
	}
}

void CVac::FuncPowerClose( int * nxt_state )
{
	int cur_state = *nxt_state;

	// 执行动作
	// 关闭所有阀
	if( m_VacParamSM.bVa )
		USB_CommToBoards2(	RelayEnAddr_Va, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
	if( m_VacParamSM.bTMP )
	{
		USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
	}
	if( m_VacParamSM.bVc )
		USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
	if( m_VacParamSM.bVf )
		USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
//	VAC_InitCtrls( FALSE );
	// 更新状态
	BOOL bV1 = m_VacParamSM.bV1;
	Init();
	m_VacParamSM.bV1	= bV1;
	m_VacParamMain.bV1	= bV1;

	m_bChanged	= TRUE;
	*nxt_state	= vacAllOFF;
}

void CVac::FuncVentOpen( int * nxt_state )
{
	int cur_state = *nxt_state;

	// 执行动作
	if( m_bDelay )
	{
		if( GetTickCount() -m_lastTCdelay < 3000 )
			return;						// 延时未到
		if( m_VacParamMain.bGun )
		{
			// 电子枪放气时要先开枪阀
			if( !m_VacParamSM.bVc )
				USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			m_VacParamMain.bVc	= TRUE;
			m_VacParamSM.bVc	= TRUE;
			m_VacParamSM.bGun	= TRUE;
		}
		else
		{
			m_VacParamMain.bChamb	= TRUE;
			m_VacParamSM.bChamb		= TRUE;
		}
		// 开放气阀
		if( !m_VacParamSM.bVa )
			USB_CommToBoards2(	RelayEnAddr_Va, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
		m_VacParamMain.bVa		= TRUE;
		m_VacParamSM.bVa		= TRUE;
		m_bDelay	= FALSE;
		m_bChanged	= TRUE;
		*nxt_state	= vacA;
		// 放气阀开后开始计时，若5分钟内未有主动抽气，则主动关闭放气阀，其它阀状态不变
		m_lastVent	= GetTickCount();
	}
	else
	{
		// 关前级阀、分子泵；若为电子枪放气且枪阀已开，则枪阀可不关
		if( m_VacParamSM.bTMP )
		{
			USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		}
		// 只有在样品室放气且枪阀已开时，才需关枪阀
		if( !m_VacParamMain.bGun && m_VacParamSM.bVc )
		{
			USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			m_VacParamSM.bVc = FALSE;
		}
		if( m_VacParamSM.bVf )
			USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		// 更新状态
		m_VacParamMain.bTMP	= FALSE;
		m_VacParamMain.bVf	= FALSE;
		m_VacParamSM.bTMP	= FALSE;
		m_VacParamSM.bVf	= FALSE;
		m_VacParamMain.bVc	= m_VacParamSM.bVc;

		m_lastTCdelay	= GetTickCount();
		if( m_VacParamMain.bGun )
			m_nMsgType	= 12;
		else
			m_nMsgType	= 11;
		m_bDelay		= TRUE;
		m_bChanged		= TRUE;
	}
}

void CVac::FuncVentClose( int * nxt_state )
{
	int cur_state = *nxt_state;

	switch( cur_state )
	{
	case	vacA:
		// 关枪阀、放气阀、分子泵，开前级阀
		if( m_VacParamSM.bGun )
		{
			// 电子枪放气后，两个TC都变坏；
			// 需开前级阀、枪阀（枪阀实际上是保持开的）
			if( m_VacParamSM.bTMP )
			{
				USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
				USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			}
			if( m_VacParamSM.bVa )
				USB_CommToBoards2(	RelayEnAddr_Va, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			if( !m_VacParamSM.bVf )
				USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			if( !m_VacParamSM.bVc )
				USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			// 更新状态
			m_VacParamMain.bTMP		= FALSE;
			m_VacParamMain.bVa		= FALSE;
			m_VacParamMain.bVc		= TRUE;
			m_VacParamMain.bVf		= TRUE;
			m_VacParamSM.bTMP		= FALSE;
			m_VacParamSM.bVa		= FALSE;
			m_VacParamSM.bVc		= TRUE;
			m_VacParamSM.bVf		= TRUE;

			m_VacParamSM.bGun		= FALSE;
			m_VacParamMain.bGun		= FALSE;
			m_nMsgType	= 13;
			m_bChanged	= TRUE;
			*nxt_state	= vacFC;
		}
		else
		{
			// 样品室放气后，枪室真空维持放气前的状态，样品室前级真空变坏；
			// 只需开前级阀，待样品室前级真空变好后即可开枪阀及机械泵
			if( m_VacParamSM.bTMP )
			{
				USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
				USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			}
			if( m_VacParamSM.bVc )
				USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			if( m_VacParamSM.bVa )
				USB_CommToBoards2(	RelayEnAddr_Va, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			if( !m_VacParamSM.bVf )
				USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			// 更新状态
			m_VacParamMain.bTMP		= FALSE;
			m_VacParamMain.bVc		= FALSE;
			m_VacParamMain.bVa		= FALSE;
			m_VacParamMain.bVf		= TRUE;
			m_VacParamSM.bTMP		= FALSE;
			m_VacParamSM.bVc		= FALSE;
			m_VacParamSM.bVa		= FALSE;
			m_VacParamSM.bVf		= TRUE;

			if( m_VacParamSM.bTCf )
			{
				// 只要样品室前级真空好，则启动延时，3s后开枪阀
				// 若样品室前级真空和枪室真空都好，则3s后开枪阀和分子泵
				m_nMsgType		= 14;
				m_lastTCdelay	= GetTickCount();
				m_bDelay		= TRUE;
			}
			else
			{
				// 其它真空情况
				m_VacParamMain.bChamb	= FALSE;
				m_VacParamSM.bChamb		= FALSE;
				m_nMsgType	= 13;
			}
			m_bChanged	= TRUE;
			*nxt_state	= vacF;
		}
		m_lastVent = 0;
		break;

	case	vacF:
		if( m_bDelay )
		{
			if( GetTickCount() -m_lastTCdelay < 3000 )
				break;						// 延时未到
			if( m_VacParamSM.bTCf && m_VacParamSM.bTCc )
			{
				// 开枪阀和分子泵
				if( !m_VacParamSM.bVc )
					USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				if( !m_VacParamSM.bTMP )
				{
					USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
					USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				}
				m_VacParamMain.bVc		= TRUE;
				m_VacParamMain.bTMP		= TRUE;
				m_VacParamSM.bVc		= TRUE;
				m_VacParamSM.bTMP		= TRUE;
				m_VacParamMain.bChamb	= FALSE;
				m_VacParamSM.bChamb		= FALSE;
				m_VacParamMain.bGun		= FALSE;
				m_VacParamSM.bGun		= FALSE;
				m_bChanged	= TRUE;
				m_bDelay	= FALSE;
				*nxt_state	= vacFCT;
				break;
			}
			else if( m_VacParamSM.bTCf )
			{
				// 开枪阀
				if( !m_VacParamSM.bVc )
					USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				m_VacParamMain.bVc		= TRUE;
				m_VacParamSM.bVc		= TRUE;
				m_VacParamMain.bChamb	= FALSE;
				m_VacParamSM.bChamb		= FALSE;
				m_VacParamMain.bGun		= FALSE;
				m_VacParamSM.bGun		= FALSE;
				m_bChanged	= TRUE;
				m_bDelay	= FALSE;
				*nxt_state	= vacFC;
				break;
			}
		}
		break;
	}
}

void CVac::FuncV1Open( int * nxt_state )
{
	int cur_state = *nxt_state;

	switch( cur_state )
	{
	case	vacAllOFF:
	case	vacA:
	case	vacF:
	case	vacFC:
		// 执行动作：前级阀和枪阀可继续保持当前状态继续抽真空，不受影响；
		// 只是高压连锁信号不给，不允许用户加高压
/*		if( m_VacParamSM.bTMP )
		{
			USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		}
		if( m_VacParamSM.bVc )
			USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		if( m_VacParamSM.bVf )
			USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		// 提示用户“V1不应开”
		// 更新状态
		m_VacParamMain.bTMP	= FALSE;
		m_VacParamMain.bVc	= FALSE;
		m_VacParamMain.bVf	= FALSE;
		m_VacParamSM.bTMP	= FALSE;
		m_VacParamSM.bVc	= FALSE;
		m_VacParamSM.bVf	= FALSE;
*/		m_VacParamMain.bV1	= m_VacParamSM.bV1;
		m_nMsgType	= 1;
//		m_bChanged	= TRUE;
//		*nxt_state	= vacMP;
		break;

	case	vacMP:
		// 开前级阀
		if( !m_VacParamSM.bVf )
			USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
		m_VacParamMain.bVf	= TRUE;
		m_VacParamSM.bVf	= TRUE;
		m_VacParamMain.bV1	= m_VacParamSM.bV1;
		if( !m_VacParamSM.bTCf || !m_VacParamSM.bTCc )
			m_nMsgType	= 1;
		m_bChanged	= TRUE;
		*nxt_state	= vacF;
		break;

	case	vacFCT:
		// 提示：高压联锁信号有效，禁止放气
		m_VacParamMain.bV1	= TRUE;
		m_VacParamSM.bV1	= TRUE;
		m_nMsgType	= 5;
		m_bChanged	= TRUE;
		*nxt_state	= vacFCT1;
		break;
	}
}

void CVac::FuncV1Close( int * nxt_state )
{
	int cur_state = *nxt_state;

	switch( cur_state )
	{
	case	vacAllOFF:
	case	vacMP:
		// 更新状态
		m_VacParamSM.dwExtSwi &= 0xFFFE;
		m_VacParamMain.bV1	= FALSE;
		m_VacParamSM.bV1	= FALSE;
		m_nMsgType	= 6;
		m_bChanged	= TRUE;
		break;

	case	vacF:
		// 执行动作
		if( m_bDelay )
		{
			if( GetTickCount() -m_lastTCdelay < 3000 )
				break;						// 延时未到
			if( m_VacParamSM.bTCc )			// TCs are both good
			{
				// 延时3s后，两个TC都好
				if( !m_VacParamSM.bVc )		// 开枪阀
					USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				if( !m_VacParamSM.bTMP )	// 开分子泵
				{
					USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
					USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				}
				// 更新状态
				m_VacParamMain.bVc	= TRUE;
				m_VacParamSM.bVc	= TRUE;
				m_VacParamMain.bTMP	= TRUE;
				m_VacParamSM.bTMP	= TRUE;
				m_VacParamMain.bTCf	= m_VacParamSM.bTCf;
				m_VacParamMain.bTCc	= m_VacParamSM.bTCc;
				m_bChanged	= TRUE;
				*nxt_state	= vacFCT;
			}
			else
			{
				// 延时3s后，只有TCf好
				if( !m_VacParamSM.bVc )		// 开枪阀
					USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				// 更新状态
				m_VacParamMain.bVc	= TRUE;
				m_VacParamSM.bVc	= TRUE;
				m_VacParamMain.bTCf	= m_VacParamSM.bTCf;
				m_bChanged	= TRUE;
				*nxt_state	= vacFC;
			}
			m_bDelay = FALSE;
			m_VacParamSM.dwExtSwi &= 0xFFFE;
			m_VacParamMain.bV1	= FALSE;
			m_VacParamSM.bV1	= FALSE;
		}
		else
		{
			if( m_VacParamSM.bTCf )
			{
				// 如果之前就有真空已经好了，则启动延时，3秒后开启相应阀
				m_nMsgType		= 23;
				m_lastTCdelay	= GetTickCount();
				m_bDelay		= TRUE;
				m_nMsgType		= 46;
				m_bChanged		= TRUE;
			}
			else
			{
				// 更新状态
				m_VacParamSM.dwExtSwi &= 0xFFFE;
				m_VacParamMain.bV1	= FALSE;
				m_VacParamSM.bV1	= FALSE;
				m_nMsgType	= 6;
				m_bChanged	= TRUE;
			}
		}
		break;

	case	vacFC:
		// 执行动作
		if( m_VacParamSM.bTCf && m_VacParamSM.bTCc 
			&& m_VacParamSM.bVf && m_VacParamSM.bVc )
		{
			// 如果样品室前级和枪室真空都好，且两阀均已开，则立刻开TMP无延迟
			if( !m_VacParamSM.bTMP )
			{
				USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			}
			m_VacParamMain.bTMP	= TRUE;
			m_VacParamSM.bTMP	= TRUE;
			*nxt_state	= vacFCT;
		}
		// 更新状态
		m_VacParamSM.dwExtSwi &= 0xFFFE;
		m_VacParamMain.bV1	= FALSE;
		m_VacParamSM.bV1	= FALSE;
		m_nMsgType	= 6;
		m_bChanged	= TRUE;
		break;

	case	vacFCT:
	case	vacFCT1:
		m_VacParamSM.dwExtSwi &= 0xFFFE;
		m_VacParamMain.bV1	= FALSE;
		m_VacParamSM.bV1	= FALSE;
		m_nMsgType	= 6;
		m_bChanged	= TRUE;
		*nxt_state	= vacFCT;
		break;
	}
}

void CVac::FuncTCForelOK( int * nxt_state )
{
	int cur_state = *nxt_state;

	switch( cur_state )
	{
	case	vacMP:
		// 执行动作
		if( m_bDelay )
		{
			if( GetTickCount() -m_lastTCdelay < 3000 )
				break;						// 延时未到
			if( m_VacParamSM.bTCc )			// TCs are both good
			{
				// 延时3s后，两个TC都好
				if( !m_VacParamSM.bVc )		// 开枪阀
					USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				if( !m_VacParamSM.bTMP )	// 开分子泵
				{
					USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
					USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				}
				// 更新状态
				m_VacParamMain.bVf	= TRUE;
				m_VacParamMain.bVc	= TRUE;
				m_VacParamSM.bVc	= TRUE;
				m_VacParamMain.bTMP	= TRUE;
				m_VacParamSM.bTMP	= TRUE;
				m_VacParamMain.bTCf	= m_VacParamSM.bTCf;
				m_VacParamMain.bTCc	= m_VacParamSM.bTCc;
				m_bChanged	= TRUE;
				*nxt_state	= vacFCT;
			}
			else
			{
				// 延时3s后，只有TCf好
				if( !m_VacParamSM.bVc )		// 开枪阀
					USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				// 更新状态
				m_VacParamMain.bVf	= TRUE;
				m_VacParamMain.bVc	= TRUE;
				m_VacParamSM.bVc	= TRUE;
				m_VacParamMain.bTCf	= m_VacParamSM.bTCf;
				m_bChanged	= TRUE;
				*nxt_state	= vacFC;
			}
			m_bDelay = FALSE;
		}
		else
		{
			// 无论TCf好还是两个TC都好，都得先开前级阀
			if( !m_VacParamSM.bVf )			// 开前级阀
				USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			// 然后启动3s延时
//			m_VacParamMain.bVf	= TRUE;
			m_VacParamSM.bVf	= TRUE;
			m_nMsgType			= 23;
			m_bChanged			= TRUE;
			m_lastTCdelay		= GetTickCount();
			m_bDelay			= TRUE;
		}
		break;

	case	vacF:
		if( m_bDelay )
		{
			if( GetTickCount() -m_lastTCdelay < 3000 )
				break;						// 延时未到
			if( !m_VacParamSM.bVc )			// 开枪阀
				USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			m_VacParamMain.bVc	= TRUE;
			m_VacParamSM.bVc	= TRUE;
			m_VacParamMain.bTCf	= m_VacParamSM.bTCf;
			if( m_VacParamSM.bTCc )
			{
				// 两个TC均好，开分子泵
				if( !m_VacParamSM.bTMP )
				{
					USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
					USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				}
				m_VacParamMain.bTMP	= TRUE;
				m_VacParamSM.bTMP	= TRUE;
				m_VacParamMain.bTCc	= m_VacParamSM.bTCc;
				*nxt_state	= vacFCT;
			}
			else
				*nxt_state	= vacFC;
			// 更新状态
			m_bChanged	= TRUE;
			m_bDelay	= FALSE;
		}
		else
		{
			// 前级阀已开，只需启动3s延时，之后开枪阀
			m_nMsgType		= 23;
			m_lastTCdelay	= GetTickCount();
			m_bDelay		= TRUE;
		}
		break;

	case	vacFC:
		// 当TC全部由好变坏，即状态由vacFC仍然保持vacFC；若之后TCf变好，即执行下列程序
		if( m_bDelay )
		{
			if( GetTickCount() -m_lastTCdelay < 3000 )
				break;						// 延时未到
			if( m_VacParamSM.bTCc )
			{
				// 如果两个TC都好了，开枪阀、分子泵
				if( !m_VacParamSM.bVc )
					USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				if( !m_VacParamSM.bTMP )
				{
					USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
					USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				}
				// 更新状态
				m_VacParamMain.bVc	= TRUE;
				m_VacParamSM.bVc	= TRUE;
				m_VacParamMain.bTMP	= TRUE;
				m_VacParamSM.bTMP	= TRUE;
				m_VacParamMain.bTCc	= TRUE;
				m_VacParamSM.bTCc	= TRUE;
				*nxt_state	= vacFCT;
			}
			m_VacParamMain.bTCf		= TRUE;
			m_VacParamSM.bTCf		= TRUE;
			m_bChanged	= TRUE;
			m_bDelay	= FALSE;
		}
		else
		{
			if( m_VacParamSM.bTCc )
			{
				// 如果枪室真空也是好的，则启动延时
				m_nMsgType		= 26;
				m_lastTCdelay	= GetTickCount();
				m_bDelay		= TRUE;
			}
			else
			{
				// 只更新TCf，状态不变
				m_VacParamMain.bTCf	= TRUE;
				m_nMsgType		= 27;
			}
		}
		break;
	}
}

void CVac::FuncTCForelBad( int * nxt_state )
{
	int cur_state = *nxt_state;

	switch( cur_state )
	{
	case	vacF:
		m_VacParamMain.bTCf	= FALSE;
		m_VacParamSM.bTCf	= FALSE;
		m_nMsgType	= 21;
		break;

	case	vacFC:
		// 执行动作
		if( m_VacParamSM.bTCc )
		{
			// 如果TCc好，关枪阀
			if( m_VacParamSM.bVc )
				USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			// 更新状态
			m_VacParamMain.bVc	= FALSE;
			m_VacParamSM.bVc	= FALSE;
			m_VacParamMain.bTCf	= m_VacParamSM.bTCf;
			m_VacParamMain.bTCc	= m_VacParamSM.bTCc;
			m_nMsgType	= 21;
			m_bChanged	= TRUE;
			*nxt_state	= vacF;
		}
		else
		{
			// 如果两个TC全变坏，仍保持当前阀状态，即前级阀和枪阀都开，一起抽样品室前级和枪室
			// 更新TC状态
			m_VacParamMain.bTCf	= FALSE;
			m_VacParamMain.bTCc	= FALSE;
			m_nMsgType	= 25;
		}
		break;

	case	vacFCT:
		// 关TMP
		if( m_VacParamSM.bTMP )
		{
			USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		}
		m_VacParamMain.bTMP	= FALSE;
		m_VacParamSM.bTMP	= FALSE;
		// 如果只是样品室前级真空变坏，同时开前级阀和枪阀一起抽
		if( m_VacParamSM.bTCc )
		{
// 			if( m_VacParamSM.bVc )
// 				USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			// 更新状态
// 			m_VacParamMain.bVc	= FALSE;
// 			m_VacParamSM.bVc	= FALSE;
			m_nMsgType			= 21;
			*nxt_state			= vacFC;
		}
		else
		{
			m_nMsgType	= 25;
			*nxt_state	= vacFC;
		}
		// 更新状态
		m_VacParamMain.bTCf	= m_VacParamSM.bTCf;
		m_VacParamMain.bTCc	= m_VacParamSM.bTCc;
		m_bChanged			= TRUE;
		break;

	case	vacFCT1:
		// 执行动作
		// 处于看像工作状态时，任一个TC变坏，都需将TMP关，保险起见Vc也应关，Vf开着
		if( m_VacParamSM.bTMP )
		{
			USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		}
		if( m_VacParamSM.bVc )
			USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		if( !m_VacParamSM.bVf )
			USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
		// 更新状态
		m_VacParamMain.bTMP	= FALSE;
		m_VacParamSM.bTMP	= FALSE;
		m_VacParamMain.bVc	= FALSE;
		m_VacParamSM.bVc	= FALSE;
		m_VacParamMain.bVf	= TRUE;
		m_VacParamSM.bVf	= TRUE;
		m_VacParamMain.bTCf	= FALSE;
		m_VacParamSM.bTCf	= FALSE;
		m_VacParamMain.bTCc	= FALSE;
		m_nMsgType	= 1;	// 真空变差，提示用户关V1阀
		m_bChanged	= TRUE;
		*nxt_state	= vacF;
		break;

	case	vacA:
		m_VacParamMain.bTCf	= FALSE;
		m_VacParamSM.bTCf	= FALSE;
		m_nMsgType	= 21;
		break;
	}
}

void CVac::FuncTCColumnOK( int * nxt_state )
{
	int cur_state = *nxt_state;

	switch( cur_state )
	{
	case	vacMP:
		// 执行动作
		if( !m_VacParamSM.bVf )			// 开前级阀
			USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
		m_VacParamMain.bVf	= TRUE;
		m_VacParamSM.bVf	= TRUE;
		m_VacParamMain.bTCc	= m_VacParamSM.bTCc;
		m_nMsgType	= 28;
		m_bChanged	= TRUE;
		*nxt_state	= vacF;
		break;

	case	vacFC:
		// 执行动作
		if( m_bDelay )
		{
			if( GetTickCount() -m_lastTCdelay < 3000 )
				break;						// 延时未到
			if( !m_VacParamSM.bV1 )		// 如果V1阀关着，才可以开分子泵
			{
				if( !m_VacParamSM.bTMP )
				{
					USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
					USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				}
				m_VacParamMain.bTMP	= TRUE;
				m_VacParamSM.bTMP	= TRUE;
				*nxt_state	= vacFCT;
			}
			// 更新状态
			m_VacParamMain.bTCc	= m_VacParamSM.bTCc;
			m_bChanged	= TRUE;
			m_bDelay	= FALSE;
		}
		else
		{
			
			if( m_VacParamSM.bTCf && m_VacParamSM.bTCc )
			{
				if( m_VacParamSM.bV1 )
				{
					// 如果前级阀和枪阀都开了，且真空都好，且V1阀开着，则关枪阀
					if( m_VacParamSM.bVc )
						USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
					m_VacParamMain.bVc	= FALSE;
					m_VacParamSM.bVc	= FALSE;
					m_VacParamMain.bTCc	= TRUE;
					m_VacParamSM.bTCc	= TRUE;
					m_bChanged	= TRUE;
					*nxt_state	= vacF;
				}
				else
				{
					// 如果前级阀和枪阀都开了，且真空都好，且V1阀关着，则启动3s延时
					m_nMsgType		= 24;
					m_lastTCdelay	= GetTickCount();
					m_bDelay		= TRUE;
				}
			}
			else if( !m_VacParamSM.bTCf && m_VacParamSM.bTCc )
			{
				if( fabs(m_TCAmp.dTCfAmp -m_TCAmp.dTCcAmp) < 0.1 )
				{
					// 如果样品室前级真空未好，而枪室真空好，但差值较小
					// 仍保持当前阀状态，即前级阀和枪阀都开，一起抽样品室前级和枪室
					// 更新状态
					m_VacParamMain.bTCc	= m_VacParamSM.bTCc;
					m_nMsgType	= 29;
				}
				else
				{
					// 如果样品室前级真空未好，而枪室真空好，但差值较大，需关枪阀
					if( m_VacParamSM.bVc )
						USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
					m_VacParamMain.bVc	= FALSE;
					m_VacParamSM.bVc	= FALSE;
					m_VacParamMain.bTCf	= m_VacParamSM.bTCf;
					m_VacParamMain.bTCc	= TRUE;
					m_VacParamSM.bTCc	= TRUE;
					m_nMsgType		= 29;
					m_bChanged		= TRUE;
					*nxt_state		= vacF;
				}
			}
			else
				m_nMsgType		= 28;
		}
		break;
	}
}

void CVac::FuncTCColumnBad( int * nxt_state )
{
	int cur_state = *nxt_state;

	switch( cur_state )
	{
	case	vacFCT1:
		// 执行动作
		// 处于看像工作状态时，任一个TC变坏，都需将TMP关，保险起见Vc也应关，Vf开着
		if( m_VacParamSM.bTMP )
		{
			USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		}
		if( m_VacParamSM.bVc )
			USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		if( !m_VacParamSM.bVf )
			USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
		// 更新状态
		m_VacParamMain.bTMP	= FALSE;
		m_VacParamSM.bTMP	= FALSE;
		m_VacParamMain.bVc	= FALSE;
		m_VacParamSM.bVc	= FALSE;
		m_VacParamMain.bVf	= TRUE;
		m_VacParamSM.bVf	= TRUE;
		m_VacParamMain.bTCc	= FALSE;
		m_VacParamSM.bTCc	= FALSE;
		m_VacParamMain.bTCf	= FALSE;
		m_nMsgType	= 1;
		m_bChanged	= TRUE;
		*nxt_state	= vacF;
		break;

	case	vacFCT:
		// 执行动作
		if( m_VacParamSM.bTMP )
		{
			USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		}
		// 更新状态
		m_VacParamMain.bTMP	= FALSE;
		m_VacParamSM.bTMP	= FALSE;
		m_VacParamMain.bTCc	= m_VacParamSM.bTCc;
		m_nMsgType	= 22;
		m_bChanged	= TRUE;
		*nxt_state	= vacFC;
		break;

	case	vacFC:
		m_VacParamMain.bTCc	= m_VacParamSM.bTCc;
		m_nMsgType	= 22;
		m_bChanged	= TRUE;
		*nxt_state	= vacFC;
		break;

	case	vacF:
		m_VacParamMain.bTCc	= m_VacParamSM.bTCc;
		m_nMsgType	= 22;
		m_bChanged	= TRUE;
		*nxt_state	= vacF;
		break;

	case	vacMP:
	case	vacA:
		m_VacParamMain.bTCc	= FALSE;
		m_VacParamSM.bTCc	= FALSE;
		m_nMsgType	= 22;
		break;
	}
}

void CVac::FuncTMPOpen( int * nxt_state )
{
	int cur_state = *nxt_state;

	switch( cur_state )
	{
	case	vacFC:
		if( m_VacParamSM.bTCf && m_VacParamSM.bTCc )
		{
			// 如果前级阀和枪阀都开着，且两真空都好
			// 则直接开TMP
			// 开TMP
			if( !m_VacParamSM.bTMP )
			{
				USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			}
			m_VacParamSM.bTMP	= TRUE;
			m_nMsgType	= 44;	// 前级阀和枪阀都开着，且两真空都好；强开TMP；则直接开TMP
			m_bChanged	= TRUE;
			*nxt_state	= vacFCT;
		}
		else if( m_VacParamSM.bTCf && !m_VacParamSM.bTCc )
		{
			// 如果前级阀和枪阀都开着，但只有样品室前级真空好
			// 此时强行开TMP，则需关枪阀
			// 关枪阀
			if( m_VacParamSM.bVc )
				USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			m_VacParamMain.bVc	= FALSE;
			m_VacParamSM.bVc	= FALSE;
			// 开TMP
			if( !m_VacParamSM.bTMP )
			{
				USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			}
			m_VacParamSM.bTMP	= TRUE;
			m_nMsgType	= 40;
			m_bChanged	= TRUE;
			*nxt_state	= vacNotN_FT;
		}
		break;

	case	vacA:
		// 样品室放气、放气阀已开时，强开TMP
		// 则开TMP，关放气阀
		if( m_VacParamSM.bVa )
			USB_CommToBoards2(	RelayEnAddr_Va, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		m_VacParamMain.bVa	= FALSE;
		m_VacParamSM.bVa	= FALSE;
		if( !m_VacParamSM.bTMP )
		{
			USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
		}
		m_VacParamSM.bTMP	= TRUE;
		m_VacParamSM.bTMP	= TRUE;
		m_bChanged	= TRUE;
		*nxt_state	= vacNotN;
		break;
	}
}

void CVac::FuncTMPClose( int * nxt_state )
{
	int cur_state = *nxt_state;

	switch( cur_state )
	{
	case	vacNotN:
		// 执行动作
		if( m_bDelay )
		{
			if( GetTickCount() -m_lastTCdelay < 3000 )
				break;						// 延时未到
			// 开放气阀
			if( !m_VacParamSM.bVa )
				USB_CommToBoards2(	RelayEnAddr_Va, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			m_VacParamMain.bVa	= TRUE;
			m_VacParamSM.bVa	= TRUE;
			*nxt_state	= vacA;
			// 更新状态
			m_VacParamSM.bTMP	= FALSE;
			m_bChanged	= TRUE;
			m_bDelay	= FALSE;
		}
		else
		{
			if( m_VacParamSM.bTMP )
			{
				USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
				USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			}
			// 如果之前样品室放气后强开了TMP
			// 那么现在手动关闭TMP后，要延时3秒后开放气阀
			if( m_VacParamSM.bChamb )
			{
				m_lastTCdelay	= GetTickCount();
				m_bDelay		= TRUE;
			}
			else
				m_VacParamSM.bTMP	= FALSE;
		}
		break;

	case	vacNotN_FT:
		// 关TMP
		if( m_VacParamSM.bTMP )
		{
			USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		}
		m_VacParamSM.bTMP	= FALSE;
		if( m_VacParamSM.bTCf )
		{
			// 立刻开枪阀
			if( !m_VacParamSM.bVc )
				USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			m_VacParamMain.bVc	= TRUE;
			m_VacParamSM.bVc	= TRUE;
		}
		m_nMsgType	= 41;
		m_bChanged	= TRUE;
		*nxt_state	= vacFC;
		break;

	case	vacFCT:
		// 关TMP
		if( m_VacParamSM.bTMP )
		{
			USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		}
		m_VacParamSM.bTMP	= FALSE;
		m_nMsgType	= 43;	// 真空都好阀都开时，强关TMP；则保持前级、枪阀开
		m_bChanged	= TRUE;
		*nxt_state	= vacFC;
		break;
	}
}

void CVac::FuncVfOpen( int * nxt_state )
{
	int cur_state = *nxt_state;
	
	switch( cur_state )
	{
	case	vacMP:
		// 执行动作
		if( m_bDelay )
		{
			if( GetTickCount() -m_lastTCdelay < 3000 )
				break;						// 延时未到
			// 开枪阀
			if( !m_VacParamSM.bVc )
				USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			m_VacParamMain.bVc	= TRUE;
			m_VacParamSM.bVc	= TRUE;
			*nxt_state	= vacFC;
			if( m_VacParamSM.bTCc )
			{
				if( !m_VacParamSM.bTMP )
				{
					USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
					USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
				}
				m_VacParamMain.bTMP	= TRUE;
				m_VacParamSM.bTMP	= TRUE;
				*nxt_state	= vacFCT;
			}
			// 更新状态
			m_VacParamSM.bVf	= TRUE;
			m_VacParamMain.bTCc	= m_VacParamSM.bTCc;
			m_bChanged	= TRUE;
			m_bDelay	= FALSE;
		}
		else
		{
			// 开前级阀
			USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			// 如果样品室前级和枪室真空都好，则启动3s延时
			if( m_VacParamSM.bTCf )
			{
				m_nMsgType		= 23;
				m_lastTCdelay	= GetTickCount();
				m_bDelay		= TRUE;
			}
			else
			{
				m_VacParamSM.bVf	= TRUE;
				m_bChanged			= TRUE;
			}
		}
		break;

	case	vacA:
		// 样品室放气、放气阀已开时，强开前级阀
		// 则开前级阀，关放气阀
		if( m_VacParamSM.bVa )
			USB_CommToBoards2(	RelayEnAddr_Va, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		m_VacParamMain.bVa	= FALSE;
		m_VacParamSM.bVa	= FALSE;
		if( !m_VacParamSM.bVf )
			USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
		m_VacParamMain.bVf	= TRUE;
		m_VacParamSM.bVf	= TRUE;
		m_bChanged	= TRUE;
		*nxt_state	= vacF;
		break;
	}
}

void CVac::FuncVfClose( int * nxt_state )
{
	int cur_state = *nxt_state;
	
	switch( cur_state )
	{
	case	vacFCT:
		// 关前级阀
		USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		m_VacParamSM.bVf	= FALSE;
		// 关TMP
		if( m_VacParamSM.bTMP )
		{
			USB_CommToBoards2(	RelayEnAddr_TMP, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			USB_CommToBoards2(	RelayEnAddr_TMP+1, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		}
		m_VacParamMain.bTMP	= FALSE;
		m_VacParamSM.bTMP	= FALSE;
		// 关枪阀
		if( m_VacParamSM.bVc )
			USB_CommToBoards2(	RelayEnAddr_Vc, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
		m_VacParamMain.bVc	= FALSE;
		m_VacParamSM.bVc	= FALSE;
		m_nMsgType	= 45;	// 强关前级阀，则需关TMP、枪阀
		m_bChanged	= TRUE;
		*nxt_state	= vacMP;
		break;

	case	vacF:
		// 执行动作
		if( m_bDelay )
		{
			if( GetTickCount() -m_lastTCdelay < 3000 )
				break;						// 延时未到
			// 开放气阀
			if( !m_VacParamSM.bVa )
				USB_CommToBoards2(	RelayEnAddr_Va, USB_TM_VAC_Write_08, 0x0, RelayEn_Open );
			m_VacParamMain.bVa	= TRUE;
			m_VacParamSM.bVa	= TRUE;
			*nxt_state	= vacA;
			// 更新状态
			m_VacParamSM.bVf	= FALSE;
			m_bChanged	= TRUE;
			m_bDelay	= FALSE;
		}
		else
		{
			// 关前级阀
			USB_CommToBoards2(	RelayEnAddr_Vf, USB_TM_VAC_Write_08, 0x0, RelayEn_Close );
			// 如果之前样品室放气后强开了前级阀
			// 那么现在手动关闭前级阀后，要延时3秒后开放气阀
			if( m_VacParamSM.bChamb )
			{
				m_lastTCdelay	= GetTickCount();
				m_bDelay		= TRUE;
			}
			else
				m_VacParamSM.bVf	= FALSE;
		}
		break;
	}
}

BOOL CVac::ADCvconv2( DWORD dw1, DWORD dw2, double& dValue )
{
	// Converts raw read data to voltage (and also to current if applicable)
	DWORD dwADCreadRaw, dwADCread24, dwADCreadSignBit, dwSIGNmask;
	BOOL bADCvalSignBit;
	double dADCreadVrefFrac, dADCreadVrefPct;
	
	double dADCvref			= 4;
	double dADCCOMoffset	= 1.607;
	// 2432 bits, LTC2448
	dwSIGNmask = 0x20000000;		// from page 10 on datasheet of LTC2448, SIG is the bit 29
	dwADCreadRaw = (dw1<<16) | dw2;
	if( dwADCreadRaw == 0 )
		return FALSE;
	dwADCread24 = (dwADCreadRaw & 0x1FFFFFFF) /32;	// Bit 28 --- Bit0, and shift down 5 bits
	
	dwADCreadSignBit = dwADCreadRaw & dwSIGNmask;
	if( dwADCreadSignBit > 0 )
		bADCvalSignBit	= TRUE;
	else
		bADCvalSignBit	= FALSE;
	// If ADCvalSignBit = 1, then reading is a positive voltage in the range of 0V to Vref/2 minus 1LSB
	// If ADCvalSignBit = 0, then reading is a negative voltage in the range of 0V to -Vref/2
	if( bADCvalSignBit )
		dADCreadVrefFrac = 1.0 * dwADCread24 / 0x7FFFFF;	// Fractional value of full range of 0 to +Vref/2
	else
		dADCreadVrefFrac = 1.0 - 1.0*(dwADCread24 - 0x800000) / 0x7FFFFF;	// Fractional value of range of 0 to -Vref/2
	
	dADCreadVrefPct = dADCreadVrefFrac * 100;	// Percentage of full range of Vref(+) - Vref(-)
	if( bADCvalSignBit )
	{
		dValue = dADCreadVrefFrac * dADCvref /2 +dADCCOMoffset;
	}
	else
	{
		dValue = dADCCOMoffset - dADCreadVrefFrac * dADCvref /2;
	}
	return TRUE;
}
