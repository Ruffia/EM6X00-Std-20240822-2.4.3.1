#include "StdAfx.h"
#include "BoardStatusDetector.h"
#include "CommonVACFunction.h"
#include <math.h>

CBoardStatusDetector& CBoardStatusDetector::Instance()
{
	static CBoardStatusDetector Inst;
	return Inst;
}


CBoardStatusDetector::CBoardStatusDetector(void)
{
	m_hWnd = NULL;

	ZeroMemory( &m_cfgBoard, sizeof(CfgVAC_IP_CCG) );
	m_cfgBoard.nType = 4;

	m_strBoard = "";
	m_strBoardSaved = "";
	m_strWinBoard = "";

	m_hFileBoard	= NULL;
	m_nLoopsBoard = 0;

	m_nIPtype		= 1;		// 离子泵控制器是老式还是新式（影响通讯协议），默认为新式
	m_bIsDual		= FALSE;	// 离子泵控制器是否是双通道的
	m_nIs1845       = -1;
	m_bPauseInquire	= FALSE;

	ZeroMemory( &m_boardStatusServer, sizeof(StatusFEGBrdVAC_board) );
	ZeroMemory( &m_boardStatusMain, sizeof(StatusFEGBrdVAC_board) );

	m_dwSleep = 250;
}


void CBoardStatusDetector::Init( HWND hWnd,int nPort, BOOL bIntype, BOOL bIsDual)
{
	m_hWnd = hWnd;
	m_cfgBoard.nPort = nPort;
	m_nIPtype = bIntype;
	m_bIsDual = bIsDual;
}

BOOL CBoardStatusDetector::Start()
{
	// 打开端口
	BOOL bCommOpen	= m_Board.Open( m_cfgBoard.nPort );
	if (!bCommOpen)
	{
		OutputDebugString("CIPGStatusDetector---------- BOOL bCommOpen = m_IPG.Open( m_cfgIPG.nPort ); Failed.\r\n");
		PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 4, bCommOpen );
		return FALSE;
	}

	Board_SendCmd( byte_FEGBrdopcStatus, byte_FEGBrdopcStatus );		// 常规查询状态
	if( CommWithBoard(100) > 0 )
		m_cfgBoard.bReady = FALSE;
	else
	{
		/////////////////////////////////////////////
		// 设置日志
		TCHAR exeFullPath[255];
		::GetModuleFileName( NULL, exeFullPath, 255 );
		char *ptemp = strrchr( exeFullPath, '\\');
		ptemp[1] = 0;

		CString str, strTemp;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d%H%M");
		// 设置Board日志文件
		str.Format( "%s\\Log\\Board%s.csv", exeFullPath, strTemp );
		if( m_hFileBoard != NULL )
			CloseHandle( m_hFileBoard );
		m_hFileBoard	= NULL;
		m_hFileBoard	= CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		m_nLoopsBoard	= 1;
		// 写Board日志文件头
		if( m_hFileBoard != NULL )
		{
			unsigned long cbRet;
			char Buf[255];
			str.Format( ",%s\r\n", "Board: CCG, TMP, MP, Vvent2, Vvent1, V1, X, M" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( m_hFileBoard, Buf, strlen(Buf), &cbRet, NULL );
		}

		if( m_nIs1845 > 1 )		// CCG由Board读取
		{
			/////////////////////////////////////////////
			// 设置日志
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			char *ptemp = strrchr( exeFullPath, '\\');
			ptemp[1] = 0;

			CString str, strTemp;
			CTime	timeCurrent = CTime::GetCurrentTime();
			strTemp = timeCurrent.Format("%Y%m%d%H%M");

			// 设置CCG日志文件
			str.Format( "%s\\Log\\CCG%s.txt", exeFullPath, strTemp );
			if( m_hFileBoard != NULL )
				CloseHandle( m_hFileBoard );
			m_hFileBoard	= NULL;
			m_hFileBoard = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);

			// 写CCG日志文件头
			if( m_hFileBoard != NULL )
			{
				unsigned long cbRet;
				char Buf[255];
				str.Format( "\t%s\r\n", "CCG" );
				sprintf( Buf, "%s\r\n", str );
				WriteFile( m_hFileBoard, Buf, strlen(Buf), &cbRet, NULL );
				/////////////////////////////////////////////
			}
			m_cfgBoard.bReady = TRUE;
			PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 3, m_cfgBoard.bReady );
			PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 3+5, (LPARAM)( &m_cfgBoard ) );
		}
		else if( m_nIs1845 > 0 )	// 1845的Board由Board读取
		{
			/////////////////////////////////////////////
			// 设置日志
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			char *ptemp = strrchr( exeFullPath, '\\');
			ptemp[1] = 0;

			CString str, strTemp;
			CTime	timeCurrent = CTime::GetCurrentTime();
			strTemp = timeCurrent.Format("%Y%m%d%H%M");

			// 设置Board日志文件
			str.Format( "%s\\Log\\Board%s.txt", exeFullPath, strTemp );
			if( m_hFileBoard != NULL )
				CloseHandle( m_hFileBoard );
			m_hFileBoard	= NULL;
			m_hFileBoard = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			m_nLoopsBoard = 1;
			// 写Board日志文件头
			if( m_hFileBoard != NULL )
			{
				unsigned long cbRet;
				char Buf[255];
				str.Format( "\t%s\r\n", "Board" );
				sprintf( Buf, "%s\r\n", str );
				WriteFile( m_hFileBoard, Buf, strlen(Buf), &cbRet, NULL );
			}
			m_cfgBoard.bReady = TRUE;
			PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 1, m_cfgBoard.bReady );
		}
	}

	PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 4, m_cfgBoard.bReady );

	BOOL bStart = __super::Start();
	return bStart;
}


void CBoardStatusDetector::Release()
{
	m_Board.Close();
	m_cfgBoard.bReady	= m_Board.IsOpened();
	if( m_hFileBoard != NULL )
		CloseHandle( m_hFileBoard );
	m_hFileBoard	= NULL;

	__super::Release();
}


DWORD CBoardStatusDetector::DoJob()
{
	if( !m_bPauseComm )
	{
		// 1秒钟通讯一次，读一次真空值
		CommWithBoard(100);
		if( !m_bPauseInquire )
		{
			Board_SendCmd( byte_FEGBrdopcStatus, byte_FEGBrdopcStatus );		// 常规查询状态
			if( m_nIs1845 > 0  )
				Board_SendCmd( byte_FEGBrdopcIPG, byte_FEGBrdopcIPG );			// 查询IPG状态
		}
	}
	Sleep( 1 );

	return 0L;
}


int CBoardStatusDetector::CommWithBoard( int nSleep )
{
	char dataSend[5] = {0};
	if( m_boardStatusServer.byteV1 != m_boardStatusMain.byteV1 )
	{
		Board_ConvertSendData( byte_FEGBrdopcV1, m_boardStatusMain.byteV1, dataSend );
		m_boardStatusServer.byteV1 = m_boardStatusMain.byteV1;
		m_Board.SendData(dataSend, byte_FEGBrdLenSnd);
		Sleep(nSleep);
		RecordVacuum( "V1状态改变" );
	}
	if( m_boardStatusServer.byteVent != m_boardStatusMain.byteVent )
	{
		if( m_boardStatusMain.byteVent == 0x01 )	// 放气
		{
			Board_ConvertSendData( byte_FEGBrdopcVent, byte_FEGBrdopcVent, dataSend );
			RecordVacuum( "发送放气命令" );
		}
		if( m_boardStatusMain.byteVent == 0x03 )	// 抽气
		{
			Board_ConvertSendData( byte_FEGBrdopcPump, byte_FEGBrdopcPump, dataSend );
			RecordVacuum( "发送抽气命令" );
		}
		m_boardStatusServer.byteVent = m_boardStatusMain.byteVent;
		m_Board.SendData(dataSend, byte_FEGBrdLenSnd);
		Sleep(nSleep);
	}

	BYTE dataRecv[255];
	int nLenRecv = 0;
	int nCount = 0;
	int nRet = -1;
	do{
		Sleep(nSleep);
		nLenRecv = m_Board.ReadData( dataRecv, 255 );
		if( nLenRecv < 1 )
		{
			// 数据不完整
			Sleep(nSleep);
		}
		else
		{
			nRet = Board_ConvertReceiveData( nLenRecv, dataRecv );
			if( nRet == 0 )
				break;
		}
		nCount++;
	} while( (nLenRecv < 1) && (nCount < 5) );
	if( nCount == 5 )
		return 2;
	if( nRet > 0 )
		return 3;

	CString str, strOut, strTemp = "";
	for(int i=0; i<nLenRecv; i++ )
	{
		str.Format( "%c", dataRecv[i] );
		strTemp += str;
	}
	m_strBoard = m_strBoardSaved + strTemp;
	return 0;
}



void CBoardStatusDetector::RecordVacuum( CString strVacuum )
{
	unsigned long cbRet;
	char Buf[255] = {0};
	CString str;

	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
	str.Format( "[%s],%s", csCurrentTime, strVacuum );

	sprintf(Buf, "%s\r\n", str );
	WriteFile(m_hFileBoard, Buf, strlen(Buf), &cbRet, NULL);

	m_nLoopsBoard++;
	if( m_nLoopsBoard > 10000 )
	{
		CloseHandle( m_hFileBoard );
		// 设置日志文件
		TCHAR exeFullPath[255];
		::GetModuleFileName( NULL, exeFullPath, 255 );
		char *ptemp = strrchr( exeFullPath, '\\');
		ptemp[1] = 0;
		CTime	timeCurrent = CTime::GetCurrentTime();
		csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
		str.Format( "%s\\Log\\Board%s.csv", exeFullPath, csCurrentTime );
		m_hFileBoard = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		m_nLoopsBoard = 1;

		str.Format( ",%s\r\n", "Board: CCG, TMP, MP, Vvent2, Vvent1, V1, X, M" );
		sprintf( Buf, "%s\r\n", str );
		WriteFile( m_hFileBoard, Buf, strlen(Buf), &cbRet, NULL );
	}
}


void CBoardStatusDetector::Board_SendCmd( BYTE byteOpc, BYTE byteOpr )
{
	char dataSend[5] = {0};
	Board_ConvertSendData( byteOpc, byteOpr, dataSend );
	m_Board.SendData(dataSend, byte_FEGBrdLenSnd);
}


void CBoardStatusDetector::Board_ConvertSendData( BYTE byteOpc, BYTE byteOpr, char* data )
{
	data[0] = byte_FEGBrdHead;
	data[1]	= byteOpc;
	data[2]	= byteOpr;
	data[3]	= data[1] ^ data[2];
	data[4]	= byte_FEGBrdTail;
}


int CBoardStatusDetector::Board_ConvertReceiveData( int nLen, BYTE* data )
{
	if( nLen < byte_FEGBrdLenRec )
		return 1;	// 不够一帧数据

	int nCount = nLen /byte_FEGBrdLenRec;	// 一次收到多少帧数据
	int i = 0;
	for( int j=0; j<nCount; j++ )
	{
		i = j*byte_FEGBrdLenRec;
		if( data[i] != byte_FEGBrdHead )
		{
			if( j < nCount )
				continue;
			else
				return 2;	// 帧头不对
		}
		if( data[i+5] != byte_FEGBrdTail )
		{
			if( j < nCount )
				continue;
			else
				return 3;	// 帧尾不对
		}

		BYTE chk = data[i+1] ^ data[i+2];
		chk ^= data[i+3];
		if( chk != data[i+4] )
		{
			if( j < nCount )
				continue;
			else
				return 4;	// 校验码不对
		}

		BYTE bOpc = data[i+1];
		if( bOpc == byte_FEGBrdopcStatus )	// 状态
		{
			WORD wStatus = (data[i+2] << 8) | data[i+3];
			if( wStatus != m_boardStatusServer.wStatus )
			{
				m_boardStatusServer.wStatus = wStatus;

				/////////////////////////////////////////////////////
				// 分析状态
				//const	BYTE	byte_FEGBrdopcStatus	= 0x11;		// 返回真空状态和各个开关量的状态
				// 参数1：0xXY
				//	X：	0 初始化状态；1 样品室大气状态；2 抽真空状态
				//		3 粗真空状态；4 TMP运行状态；5 TMP运行错误状态
				//	Y：	0bMKQP
				//		M：1 枪真空错误状态；0 枪真空正常状态
				//		K：1 样品室真空错误状态；0 样品室真空正常状态
				//		QP 保留
				//	参数2：0b ABCD EFGH （1为开，0为关）
				//		A、B：保留；C：CCG电源；D：TMP；E：MP；F：Vvent2；G：Vvent1；H：V1
				BYTE status1 = (BYTE)((wStatus & 0xFF00 ) >> 8);
				BYTE status2 = (BYTE)(wStatus & 0x00FF);
				// 分析参数1
				m_boardStatusServer.bX = (status1 & 0x70) >> 4;			// 高4位中的低3位表示X
//				m_boardStatusServer.bY = ( (status1 & 0x08) == 0x08 );	// 低4位表示Y，其中最高位为1表示枪真空错误状态
				m_boardStatusServer.bY = status1 & 0x0F;				// 低4位表示Y
				// 分析参数2
				m_boardStatusServer.bCCG	= ( (status2 & 0x20) == 0x20 );
				m_boardStatusServer.bTMP	= ( (status2 & 0x10) == 0x10 );
				m_boardStatusServer.bMP		= ( (status2 & 0x08) == 0x08 );
				m_boardStatusServer.bVvent2	= ( (status2 & 0x04) == 0x04 );
				m_boardStatusServer.bVvent1	= ( (status2 & 0x02) == 0x02 );
				m_boardStatusServer.bV1		= ( (status2 & 0x01) == 0x01 );
				/////////////////////////////////////////////////////

				PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, byte_FEGBrdopcStatus, (LPARAM)( &m_boardStatusServer ) );
			}
			CString str;
			str.Format( "%d, %d, %d, %d, %d, %d, 0x%02X, 0x%02X", m_boardStatusServer.bCCG, m_boardStatusServer.bTMP, m_boardStatusServer.bMP,
				m_boardStatusServer.bVvent2, m_boardStatusServer.bVvent1, m_boardStatusServer.bV1,
				m_boardStatusServer.bX, m_boardStatusServer.bY );
			RecordVacuum(str );
		}
		else if( bOpc == byte_FEGBrdopcIPG )	// IPG
		{
			WORD wIPG = (data[i+2] << 8) | data[i+3];
			double dU = 1.0 * wIPG /0xFFFF *10;						// U = D /65535 *10 (V)
			m_boardStatusServer.dPipg = pow( 10, (dU -18.5) /2 );	// U = 2log10(P) +18.5, P = 10 ^ ((U-18.5) /2) (Torr)
			PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, byte_FEGBrdopcIPG, (LPARAM)( &m_boardStatusServer ) );
			CString str;
			str.Format( "%.1e", m_boardStatusServer.dPipg );
			RecordVacuum( str );
		}
		else if( bOpc == byte_FEGBrdopcClr )
		{
			PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, byte_FEGBrdopcClr, 0 );
			RecordVacuum("接收到Clr命令" );
		}
		else if( bOpc == byte_FEGBrdopcWarning )
		{
			PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, byte_FEGBrdopcWarning, (LPARAM)(data[i+3]) );
			RecordVacuum("接收到warning" );
		}
	}
	return 0;
}
