#include "StdAfx.h"
#include "PLCvacBrdStatusDetector.h"
#include <math.h>
#include <afxsock.h>

CPLCvacBrdStatusDetector& CPLCvacBrdStatusDetector::Instance()
{
	static CPLCvacBrdStatusDetector Inst;
	return Inst;
}

CPLCvacBrdStatusDetector::CPLCvacBrdStatusDetector(void)
{
	m_hWnd			= NULL;
	m_dwSleep		= 200;

	ZeroMemory( &m_cfgPLCvacBrd, sizeof(Cfg_PLCvacBrd) );
	ZeroMemory( &m_StatusPLCvacBrd, sizeof(Status_PLCvacBrd) );
}

void CPLCvacBrdStatusDetector::Release()
{
	Close();

	__super::Release();
}

BOOL CPLCvacBrdStatusDetector::Init( HWND hWnd, int nGaugeType )
{
	m_hWnd = hWnd;
	m_nGaugeType = nGaugeType;

	// ����socket
	AfxSocketInit();
	return TRUE;

	m_sockPLCvacBrd = socket(AF_INET,SOCK_STREAM,0);
	if( m_sockPLCvacBrd < 0 )
	{
		// Socket�����쳣...
		m_cfgPLCvacBrd.bCreate = FALSE;
		PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, 0, 11 );
		shutdown(m_sockPLCvacBrd,2);
		closesocket(m_sockPLCvacBrd);
		return FALSE;
	}
	else
	{
		m_cfgPLCvacBrd.bCreate = TRUE;
		char szPortPLC[5], szIP[16];

		int nNetTimeout = 1000;	//1��
		//����ʱ��
		setsockopt(m_sockPLCvacBrd, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout,sizeof(int));
		//����ʱ��
		setsockopt(m_sockPLCvacBrd, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout,sizeof(int));

		sprintf_s( szPortPLC, "%d", PLCvacBoard_PortPLC );
		sprintf_s( szIP, "192.168.2.1" );
		remote_addr.sin_family = AF_INET;
		remote_addr.sin_addr.s_addr = inet_addr(szIP);
		remote_addr.sin_port = htons(_ttoi(szPortPLC));

		if( connect(m_sockPLCvacBrd,(struct sockaddr*)&remote_addr, sizeof(struct sockaddr_in)) < 0 )
		{
			// �����쳣
			m_cfgPLCvacBrd.bConnect = FALSE;
			PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, 0, 13 );
			shutdown(m_sockPLCvacBrd,2);
			closesocket(m_sockPLCvacBrd);
			return FALSE;
		}
		else
		{
			m_cfgPLCvacBrd.bConnect = TRUE;
			return TRUE;
		}
	}
}
/*
BOOL InitTest( HWND hWnd )
{
	g_hMain = hWnd;

	// ����socket����socket����Ϊ������ģʽ
	// ����connect���ӣ���������������򷵻�0�������������ӷ���-1��
	//		����Ǻ��жϴ������Ƿ��ʾ��ʱ������ɣ��ǵĻ�������һ��
	// ���ŵ���select����ָ��ʱ���ڼ��socket�Ƿ��д��
	// �����д����connect���ӳɹ���0��ʾ��ʱ��-1��ʾ�����˴���

	// ��ʼ��
	WSADATA wsaData;
	if( WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR )
	{
		TRACE( "PLC: WSAStartup function failed\n" );
		return FALSE;
	}

	int nParam = 0;
	m_sockPLCvacBrd = INVALID_SOCKET;
	do
	{
		// ����socket
		m_sockPLCvacBrd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if( m_sockPLCvacBrd == INVALID_SOCKET )
		{
			TRACE( "PLC: socket function failed with error: %ld\n", WSAGetLastError() );
			nParam = 11;
			break;
		}
		// socket����Ϊ������
		unsigned long on = 1;
		if( ioctlsocket(m_sockPLCvacBrd, FIONBIO, &on) < 0 )
		{
			TRACE( "PLC: ioctlsocket failed\n" );
			nParam = 12;
			break;
		}

		g_cfgPLCvacBrd.bCreate = TRUE;
		/////////////////////////////////////////
		// ��������
		char szPortPLC[5], szIP[16];
		sprintf_s( szPortPLC, "%d", PLCvacBoard_PortPLC );
		sprintf_s( szIP, "192.168.2.1" );
		remote_addr.sin_family = AF_INET;
		remote_addr.sin_addr.s_addr = inet_addr(szIP);
		remote_addr.sin_port = htons(_ttoi(szPortPLC));

		int nNetTimeout = 1000;	//1��
		//����ʱ�޺ͽ���ʱ��
		setsockopt(m_sockPLCvacBrd, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout,sizeof(int));
		setsockopt(m_sockPLCvacBrd, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout,sizeof(int));

		int nRet = connect(m_sockPLCvacBrd,(struct sockaddr*)&remote_addr, sizeof(struct sockaddr_in));
		if( nRet == 0 )
		{
			TRACE( "PLC: connect success1\n" );
			g_cfgPLCvacBrd.bConnect = TRUE;
			nParam = 1;
		}

		// ��Ϊ�Ƿ������ģ����ʱ�������Ӧ����WSAEWOULDBLOCK
		if( nRet < 0 && WSAGetLastError() != WSAEWOULDBLOCK )
		{
			TRACE( "PLC: Connect failed with error: %ld\n", WSAGetLastError() );
			nParam = 13;
			break;	// return FALSE;
		}

		fd_set writeset;
		FD_ZERO(&writeset);
		FD_SET(m_sockPLCvacBrd, &writeset);
		timeval tv;
		tv.tv_sec = 2;
		tv.tv_usec = 0;
		nRet = select(m_sockPLCvacBrd +1, NULL, &writeset, NULL, &tv);
		if( nRet == 0 )
		{
			TRACE( "PLC: connect timeout\n" );
			nParam = 14;
		}
		else if( nRet < 0 )
		{
			TRACE( "PLC: connect failed with error: %ld\n", WSAGetLastError() );
			nParam = 15;
		}
		else
		{
			TRACE( "PLC: connect success2\n" );
			g_cfgPLCvacBrd.bConnect = TRUE;
			nParam = 2;
		}
		if( !(g_cfgPLCvacBrd.bConnect) )
			break;

		// ��������
		g_cfgPLCvacBrd.bConnect = TRUE;
		// �����߳�
		ResetEvent( g_hEventWorkPLCvacBoard );
		ResetEvent( g_hEventTeminatePLCvacBoard );
		g_hThreadPLCvacBrd = CreateThread(NULL, 0, CommThreadPLCvacBoard, NULL, 0, &g_dwThreadPLCvacBrd);
		if( g_hThreadPLCvacBrd == NULL )
		{
			TRACE( "PLC: CreateThread failed\n" );
			nParam = 16;
			break;
		}
		CloseHandle( g_hThreadPLCvacBrd );
	} while( false );

	PostMessage( g_hMain, WM_USER_PLCvacBrd_UpdateStatus, 0, nParam );
	if( nParam < 10 )
		return TRUE;
	else
	{
		// δ���ӳɹ�
		shutdown(m_sockPLCvacBrd,2);
		closesocket(m_sockPLCvacBrd);
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}
*/
BOOL CPLCvacBrdStatusDetector::SetCmd( BYTE bFunc, BYTE bOpr )
{
	if( SendCmd( bFunc, bOpr ) < 1 )
		return FALSE;
	return TRUE;
}

BOOL CPLCvacBrdStatusDetector::SetThresholds( double dVacPre, double dVacOK, double dVacNotOK )
{
	WORD wData = 0x0;
	wData = (WORD)(dVacPre *pow(10.0,3));
	SendThresholds( byte_PLCvacBRDopr_VacPre, wData );
	wData = (WORD)(dVacOK *pow(10.0,7));
	SendThresholds( byte_PLCvacBRDopr_VacOK, wData );
	wData = (WORD)(dVacNotOK *pow(10.0,7));
	SendThresholds( byte_PLCvacBRDopr_VacNotOK, wData );
	return TRUE;
}

int CPLCvacBrdStatusDetector::CommTest( int nSleep )
{
	int nRet = SendCmd( byte_PLCvacBRDopc_InquireStatus, byte_PLCvacBRDopr_Default );
	return nRet;
}

BOOL CPLCvacBrdStatusDetector::Start()
{
	OutputDebugString( "PLC: into CommThreadPLCvacBoard" );
	Connect_Init();

	return __super::Start();
}

DWORD CPLCvacBrdStatusDetector::Connect_Init()
{
	// ��ͨ�Ѷϣ��Զ�������������
	m_sockPLCvacBrd = socket(AF_INET,SOCK_STREAM,0);
	if( m_sockPLCvacBrd < 0 )
	{
		// Socket�����쳣...
		shutdown(m_sockPLCvacBrd,2);
		closesocket(m_sockPLCvacBrd);
		return -1;
	}
	else
	{
		m_cfgPLCvacBrd.bCreate = TRUE;
		char szPortPLC[5], szIP[16];

		int nNetTimeout = 1000;	//1��
		//����ʱ��
		setsockopt(m_sockPLCvacBrd, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout,sizeof(int));
		//����ʱ��
		setsockopt(m_sockPLCvacBrd, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout,sizeof(int));

		sprintf_s( szPortPLC, "%d", PLCvacBoard_PortPLC );
		sprintf_s( szIP, "192.168.2.1" );
		remote_addr.sin_family = AF_INET;
		remote_addr.sin_addr.s_addr = inet_addr(szIP);
		remote_addr.sin_port = htons(_ttoi(szPortPLC));

		int nConnect = connect(m_sockPLCvacBrd,(struct sockaddr*)&remote_addr, sizeof(struct sockaddr_in));
		if( SOCKET_ERROR == nConnect )
		{
			DWORD dwErr = GetLastError();
			closesocket(m_sockPLCvacBrd);
			m_sockPLCvacBrd = INVALID_SOCKET;
			Sleep(1000);
			return -1;
		}

		m_cfgPLCvacBrd.bConnect = TRUE;
		// �Ȳ���ͨѶ�Ƿ�����
		OutputDebugString( "PLC: Start to CommTest" );
		if( CommTest(10) > 0 )
			m_cfgPLCvacBrd.bReady = FALSE;
		else
		{
			m_cfgPLCvacBrd.bReady = TRUE;
			RecordInit();
		}
		PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, 1, m_cfgPLCvacBrd.bReady );

		if( m_cfgPLCvacBrd.bReady )
		{
			// �л��ٵ�˿
			SendCmd( byte_PLCvacBRDopc_PumpVent_SEM, byte_PLCvacBRDopr_SEMpvSwitch );
			// ������ռ�����
			SendCmd( byte_PLCvacBRDopc_GaugeType, (BYTE)(m_nGaugeType +1) );
			// �Զ�ģʽ
			SendCmd( byte_PLCvacBRDopc_SwitchMode, byte_PLCvacBRDopr_ModeAuto );
			// ��ȡ��ǰ��ֵ
			SendCmd( byte_PLCvacBRDopc_InquireVACthreshold, byte_PLCvacBRDopr_Default );
		}
	}
	return 0;
}

void CPLCvacBrdStatusDetector::Close()
{
	// �쳣���ر�socket
	shutdown(m_sockPLCvacBrd,2);
	closesocket(m_sockPLCvacBrd);
	m_sockPLCvacBrd = INVALID_SOCKET;
	if( m_hFilePLCvacBrd != NULL )
		CloseHandle( m_hFilePLCvacBrd );
	m_hFilePLCvacBrd	= NULL;
	ZeroMemory( &m_cfgPLCvacBrd, sizeof(Cfg_PLCvacBrd) );
	ZeroMemory( &m_StatusPLCvacBrd, sizeof(Status_PLCvacBrd) );
}

DWORD CPLCvacBrdStatusDetector::DoJob()
{
	if( !(m_cfgPLCvacBrd.bReady) )
	{
		Connect_Init();
	}

	while(true)
	{
		int nRet = SendCmd( byte_PLCvacBRDopc_InquireStatus, byte_PLCvacBRDopr_Default );
		if( SOCKET_ERROR == nRet )
		{
			Close();
			PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, 1, 0 );
			break;
		}
		char szBuf[255];
		sprintf_s( szBuf, "%.2e,%ld,%ld,%ld,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			m_StatusPLCvacBrd.dPccg, m_StatusPLCvacBrd.tmp.wR, m_StatusPLCvacBrd.tmp.wD, m_StatusPLCvacBrd.tmp.wT,
			m_StatusPLCvacBrd.statusValve.valve.V1, m_StatusPLCvacBrd.statusValve.valve.V2,
			m_StatusPLCvacBrd.statusValve.valve.V3, m_StatusPLCvacBrd.statusValve.valve.V4,
			m_StatusPLCvacBrd.statusValve.valve.V5, m_StatusPLCvacBrd.statusValve.valve.MP,
			m_StatusPLCvacBrd.statusValve.valve.TMP, m_StatusPLCvacBrd.statusValve.valve.CCD,
			m_StatusPLCvacBrd.statusValve.valve.mode, m_StatusPLCvacBrd.statusValve.valve.V1_SEM,
			m_StatusPLCvacBrd.statusVac.status.XX, m_StatusPLCvacBrd.statusVac.status.VacChambOK,
			m_StatusPLCvacBrd.statusVac.status.VacErr,m_StatusPLCvacBrd.statusVac.status.TMPErr,
			m_StatusPLCvacBrd.statusVac.status.TMPFail, m_StatusPLCvacBrd.statusVac.status.TMPLink,
			m_StatusPLCvacBrd.statusVac.status.MPErr, m_StatusPLCvacBrd.statusVac.status.CCGErr );
		Record( szBuf );
		Sleep(m_dwSleep);
	}
	return 0L;
}

void CPLCvacBrdStatusDetector::RecordInit()
{
	if( m_hFilePLCvacBrd != NULL )
		CloseHandle( m_hFilePLCvacBrd );
	m_hFilePLCvacBrd = NULL;
	// ������־�ļ�
	TCHAR exeFullPath[255];
	::GetModuleFileName( NULL, exeFullPath, 255 );
	char *ptemp = strrchr( exeFullPath, '\\');
	ptemp[1] = 0;

	CString csCurrentTime, str;
	CTime	timeCurrent = CTime::GetCurrentTime();
	csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
	str.Format( "%s\\Log\\PLCvacBrd%s.csv", exeFullPath, csCurrentTime );
	m_hFilePLCvacBrd = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	m_nLoopsPLCvacBrd = 1;

	if( m_hFilePLCvacBrd != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		str.Format( "\t%s\r\n", "PLCvacBrd" );
		sprintf_s( Buf, "%s\r\n", str );
		WriteFile( m_hFilePLCvacBrd, Buf, strlen(Buf), &cbRet, NULL );
		sprintf_s( Buf, "Time, CCG, TMP speed, TMP temperature, TMP time, V1, V2, V3, V4, V5, TMP, MP, CCD, mode, V1_SEM, XX, ChambVacOK, VacErr, TMPErr, TMPfail, TMPlink, MPErr, CCGErr\r\n" );
		WriteFile( m_hFilePLCvacBrd, Buf, strlen(Buf), &cbRet, NULL );
	}
}

void CPLCvacBrdStatusDetector::Record( LPCTSTR lpszRcd )
{
	if( m_hFilePLCvacBrd == NULL )
		return;

	unsigned long cbRet;
	char Buf[255];
	CString str;

	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
	str.Format( "[%s],%s", csCurrentTime, lpszRcd );

	sprintf_s(Buf, "%s\r\n", str );
	WriteFile(m_hFilePLCvacBrd, Buf, strlen(Buf), &cbRet, NULL);

	m_nLoopsPLCvacBrd++;
	if( m_nLoopsPLCvacBrd > 10000 )
		RecordInit();
}

int CPLCvacBrdStatusDetector::ReadFrom( char FAR * buf, int len )
{
	int flags = 0;
	int nRet = recv(m_sockPLCvacBrd, buf, len, flags);

	char szRecv[2048] = {0}, temp[10] = {0};
	sprintf_s(szRecv, "PLCvacBrd: recv = ");
	for(int i=0; i<nRet; i++)
	{
		sprintf_s(temp, "0x%02X ", (unsigned char)buf[i]);
		strcat_s(szRecv, temp);
	}
//	Record(szRecv);
	return nRet;
}

int CPLCvacBrdStatusDetector::WriteTo( const char FAR * buf, int len )
{
	char szSend[255] = {0}, temp[10] = {0};
	sprintf_s(szSend, "PLCvacBrd: send = ");
	for(int i = 0; i < len; i++)
	{
		sprintf_s(temp, "0x%02X ", (unsigned char)buf[i]);
		strcat_s(szSend, temp);
	}
//	Record(szSend);

	int flags = 0;
	return send(m_sockPLCvacBrd, buf, len, flags);
}

int CPLCvacBrdStatusDetector::SendCmd( BYTE byteOpc, BYTE byteOpr )
{
// 	char dataSend[byte_PLCvacBRDLenSend1 +1];
// 	sprintf_s( dataSend, "%c%c%c\r", byte_PLCvacBRDHead, byteOpc, byteOpr );
// 	return PLC_WriteTo( dataSend, strlen(dataSend) );

	// Send
	char dataSend[byte_PLCvacBRDLenSend1 +1];
	sprintf_s( dataSend, "%c%c%c\r", byte_PLCvacBRDHead, byteOpc, byteOpr );
	int nLenSend = WriteTo( dataSend, strlen(dataSend) );
	if( SOCKET_ERROR == nLenSend )
		return SOCKET_ERROR;
	else if( nLenSend < 2 )
		return 2;
//	sprintf_s( dataSend, "Send (%d) = 0x%02X, %02X, %02X, %02X", nLenSend, byte_PLCvacBRDHead, byteOpc, byteOpr, 0x0D );
//	OutputDebugString( dataSend );
	Sleep(10);

	// Receive
	char dataRecv[255] = {0};
	int nLenRecv = ReadFrom( dataRecv, 255 );
	if( SOCKET_ERROR == nLenRecv )
		return SOCKET_ERROR;

//	sprintf_s( dataSend, "PLC: Recv (%d)", nLenRecv );
//	OutputDebugString( dataSend );

	// Analyze
	int nRet = 3;
	if( nLenRecv > 0 )
		nRet = ConvertReceiveData( nLenRecv, dataRecv );
	return nRet;
}

int CPLCvacBrdStatusDetector::SendThresholds( BYTE byteOpc, WORD wOpr )
{
	char dataSend[255] = {0};
	sprintf_s( dataSend, "%c%c%c%c%c\r", byte_PLCvacBRDHead, byte_PLCvacBRDopc_SetThreshold, byteOpc, (BYTE)((wOpr & 0xFF00) >> 8), (BYTE)(wOpr & 0x00FF) );
	int nLenSend = WriteTo( dataSend, 6 );	// strlen(dataSend) );
	if( nLenSend < 1 )
		return 2;
	sprintf_s( dataSend, "Send Thresholds (%d) = 0x%02X, %02X, %02X, %04X, %02X", nLenSend, byte_PLCvacBRDHead, byte_PLCvacBRDopc_SetThreshold, byteOpc, wOpr, 0x0D);
	Record( dataSend );
	Sleep(10);

	// Receive
	char dataRecv[255] = {0};
	int nLenRecv = ReadFrom( dataRecv, 255 );
//	sprintf_s( dataSend, "PLC: Recv (%d)", nLenRecv );
//	OutputDebugString( dataSend );

	// Analyze
	int nRet = 3;
	if( nLenRecv > 0 )
		nRet = ConvertReceiveData( nLenRecv, dataRecv );
	return nRet;
}

/////////////////////////////////////////////////////////////////////
int CPLCvacBrdStatusDetector::Recv_VACstatus( BYTE* data )
{
	// ���ز�����Ϊ��0xXXXX
	int i = 0;
	WORD wStatus = (data[i +2] << 8) | data[i +3];
	if( wStatus != m_StatusPLCvacBrd.statusVac.wStatus )
	{
		m_StatusPLCvacBrd.statusVac.wStatus = wStatus;
		PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_InquireVACstatus, (LPARAM)(&(m_StatusPLCvacBrd.statusVac)) );
	}
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_VACvalve( BYTE* data )
{
	// ���ز�����Ϊ��0xXXXX
	int i = 0;
	WORD wStatus = (data[i +2] << 8) | data[i +3];
	if( wStatus != m_StatusPLCvacBrd.statusValve.wValve )
	{
		m_StatusPLCvacBrd.statusValve.wValve = wStatus;
		PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_InquireVACvalve, (LPARAM)(&(m_StatusPLCvacBrd.statusValve)) );
	}
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_VACthreshold( BYTE* data )
{
	// ���ز�����Ϊ��0xXXXX XXXX XXXX
	int i = 0;
	WORD wData1 = (data[i +2] << 8) | data[i +3];
	WORD wData2 = (data[i +4] << 8) | data[i +5];
	WORD wData3 = (data[i +6] << 8) | data[i +7];
	m_cfgPLCvacBrd.dThreshold_PreVac = 1.0 *wData1 / pow(10.0,3);
	m_cfgPLCvacBrd.dThreshold_VacOK = 1.0 *wData2 / pow(10.0,7);
	m_cfgPLCvacBrd.dThreshold_VacNotOK = 1.0 *wData3 / pow(10.0,7);
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_InquireVACthreshold, (LPARAM)(&m_cfgPLCvacBrd) );

	char szBuf[255] = {0};
	sprintf_s( szBuf, "Recv thresholds: 0x%04X (%.1e), 0x%04X (%.1e), 0x%04X (%.1e)", wData1, m_cfgPLCvacBrd.dThreshold_PreVac, wData2, m_cfgPLCvacBrd.dThreshold_VacOK, wData3, m_cfgPLCvacBrd.dThreshold_VacNotOK );
	Record( szBuf );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_Status( BYTE* data )
{
	// ���ز�����Ϊ��
	//				����1,2��	���״̬
	//				����3,4��	����״̬
	//				����5,6��	CCGֵ
	//				����7,8��	���ӱ�ת��ֵ
	//				����9,10��	���ӱ��¶�ֵ
	//				����11,12��	���ӱ�����ʱ��ֵ
	//				����13��		01Ϊ�ٵ�˿��02Ϊ��ǹ
	//				����14��		����
	int i = 0;
	m_StatusPLCvacBrd.statusVac.wStatus = (data[i +3] << 8) | data[i +2];
	m_StatusPLCvacBrd.statusValve.wValve = (data[i +5] << 8) | data[i +4];

	WORD wData = (data[i +6] << 8) | data[i +7];
	// U = (ADC/27648.0) *10.0	// ?????
	double dU = 10.0 *wData /27648;
	if( m_nGaugeType > 0 )
	{
		// ��׼��ռƣ�P = 10 ^ 1.222*(U -7.647) (Torr)
		m_StatusPLCvacBrd.dPccg = pow( 10, 1.222 * (dU -7.647) );
	}
	else
	{
		// Inficon��ռƣ�P = 10 ^ (1.667 *U -d)	// d = 11.46
		m_StatusPLCvacBrd.dPccg = pow( 10, 1.667 * dU -11.46 );
	}

	m_StatusPLCvacBrd.tmp.wR = (data[i +8] << 8) | data[i +9];
	m_StatusPLCvacBrd.tmp.wD = (data[i +10] << 8) | data[i +11];
	m_StatusPLCvacBrd.tmp.wT = (data[i +12] << 8) | data[i +13];

	m_StatusPLCvacBrd.nType = data[i +14];

	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_InquireStatus, (LPARAM)(&m_StatusPLCvacBrd) );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_SwitchMode( BYTE* data )
{
	int nMode = 1;	// �Զ�ģʽ
	if( data[2] == 0x01 )
		nMode = 0;
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_SwitchMode, (LPARAM)(&nMode) );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_PumpVent_SEM( BYTE* data )
{
	int nPumpVent_SEM = data[2];
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_PumpVent_SEM, (LPARAM)(&nPumpVent_SEM) );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_PumpVent_FEM( BYTE* data )
{
	int nPumpVent_FEM = data[2];
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_PumpVent_FEM, (LPARAM)(&nPumpVent_FEM) );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_V1( BYTE* data )
{
	int nV1 = data[2];
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_V1, (LPARAM)(&nV1) );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_Manual_V1( BYTE* data )
{
	int nManual_V1 = data[2];
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_Manual_V1, (LPARAM)(&nManual_V1) );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_Manual_V2( BYTE* data )
{
	int nManual_V2 = data[2];
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_Manual_V2, (LPARAM)(&nManual_V2) );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_Manual_V3( BYTE* data )
{
	int nManual_V3 = data[2];
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_Manual_V3, (LPARAM)(&nManual_V3) );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_Manual_V4( BYTE* data )
{
	int nManual_V4 = data[2];
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_Manual_V4, (LPARAM)(&nManual_V4) );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_Manual_TMP( BYTE* data )
{
	int nManual_TMP = data[2];
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_Manual_TMP, (LPARAM)(&nManual_TMP) );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_CCD( BYTE* data )
{
	int nV1 = data[2];
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_CCD, (LPARAM)(&nV1) );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_WarnReset( BYTE* data )
{
	int nWarnReset = data[2];
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_WarnReset, (LPARAM)(&nWarnReset) );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_NAV( BYTE* data )
{
	int nNAV = data[2];
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_NAV, (LPARAM)(&nNAV) );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_BeamDetect( BYTE* data )
{
	int nBeamDetect = data[2];
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_BeamDetect, (LPARAM)(&nBeamDetect) );
	return 0;
}

int CPLCvacBrdStatusDetector::Recv_BSE( BYTE* data )
{
	int nBSE = data[2];
	PostMessage( m_hWnd, WM_USER_PLCvacBrd_UpdateStatus, byte_PLCvacBoard_Offset +byte_PLCvacBRDopc_BSE, (LPARAM)(&nBSE) );
	return 0;
}
/////////////////////////////////////////////////////////////////////

int CPLCvacBrdStatusDetector::GetFrameLength( BYTE bFunc )
{
	int nRet = 4;
	switch( bFunc )
	{
	case	byte_PLCvacBRDopc_SwitchMode:
	case	byte_PLCvacBRDopc_PumpVent_SEM:
	case	byte_PLCvacBRDopc_V1:
	case	byte_PLCvacBRDopc_Manual_V1:
	case	byte_PLCvacBRDopc_Manual_V2:
	case	byte_PLCvacBRDopc_Manual_V3:
	case	byte_PLCvacBRDopc_Manual_V4:
	case	byte_PLCvacBRDopc_Manual_TMP:
	case	byte_PLCvacBRDopc_CCD:
	case	byte_PLCvacBRDopc_NAV:
	case	byte_PLCvacBRDopc_WarnReset:
	case	byte_PLCvacBRDopc_BeamDetect:
	case	byte_PLCvacBRDopc_BSE:
		nRet = 4;
		break;

	case	byte_PLCvacBRDopc_InquireVACstatus:
	case	byte_PLCvacBRDopc_InquireVACvalve:
		nRet = 5;
		break;
	case	byte_PLCvacBRDopc_InquireVACthreshold:
		nRet = 9;
		break;
	case	byte_PLCvacBRDopc_InquireStatus:
		nRet = 17;
		break;

	case	byte_PLCvacBRDopc_InquireCCG:
	case	byte_PLCvacBRDopc_InquireTMPr:
	case	byte_PLCvacBRDopc_InquireTMPd:
	case	byte_PLCvacBRDopc_InquireTMPt:
		nRet = 5;
		break;
	}
	return nRet;
}

int CPLCvacBrdStatusDetector::ConvertReceiveData( int nLen, char* data )
{
	if( nLen < 4 )
		return 11;	// ����һ֡����

	int i, nFrameLen = 0;
	char szBuf[255], szTemp[10] = {0};
	sprintf_s( szBuf, "PLC: Recv = 0x" );
	BYTE* pbData = new BYTE[ nLen +1];
	for( i=0; i<nLen; i++ )
	{
		pbData[i] = data[i];
//		sprintf_s( szTemp, "%02X, ", pbData[i] );
//		strcat( szBuf, szTemp );
	}
//	OutputDebugString( szBuf );

	i = 0;
	BYTE bFunc = 0x00;
	do{
		bFunc = pbData[i +1];
		nFrameLen = GetFrameLength( bFunc );
		switch( bFunc )
		{
		case	byte_PLCvacBRDopc_InquireVACstatus:
			Recv_VACstatus( pbData +i );
			break;
		case	byte_PLCvacBRDopc_InquireVACvalve:
			Recv_VACvalve( pbData +i );
			break;
		case	byte_PLCvacBRDopc_InquireVACthreshold:
			Recv_VACthreshold( pbData +i );
			break;
		case	byte_PLCvacBRDopc_InquireStatus:
			Recv_Status( pbData +i );
			break;

		case	byte_PLCvacBRDopc_SwitchMode:
			Recv_SwitchMode( pbData +i );
			break;
		case	byte_PLCvacBRDopc_PumpVent_SEM:
			Recv_PumpVent_SEM( pbData +i );
			break;
		case	byte_PLCvacBRDopc_V1:
			Recv_V1( pbData +i );
			break;

		case	byte_PLCvacBRDopc_Manual_V1:
			Recv_Manual_V1( pbData +i );
			break;
		case	byte_PLCvacBRDopc_Manual_V2:
			Recv_Manual_V2( pbData +i );
			break;
		case	byte_PLCvacBRDopc_Manual_V3:
			Recv_Manual_V3( pbData +i );
			break;
		case	byte_PLCvacBRDopc_Manual_V4:
			Recv_Manual_V4( pbData +i );
			break;
		case	byte_PLCvacBRDopc_Manual_TMP:
			Recv_Manual_TMP( pbData +i );
			break;

		case	byte_PLCvacBRDopc_CCD:
			Recv_CCD( pbData +i );
			break;
		case	byte_PLCvacBRDopc_WarnReset:
			Recv_WarnReset( pbData +i );
			break;

		case	byte_PLCvacBRDopc_NAV:
			Recv_NAV( pbData +i );
			break;
		case	byte_PLCvacBRDopc_BeamDetect:
			Recv_BeamDetect( pbData +i );
			break;
		case	byte_PLCvacBRDopc_BSE:
			Recv_BSE( pbData +i );
			break;
		}
		i += nFrameLen;
	} while( nLen -i > 0 );
	delete [] pbData;
	return 0;
}
