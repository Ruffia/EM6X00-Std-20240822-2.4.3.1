#include "stdafx.h"

// IonPump & Cold-cathode Gauge

//::CreateWindow(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
//	m_ghThread = AfxBeginThread((AFX_THREADPROC)ThreadProc, NULL, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

#include <VACobtain.h>
#include "SerialPort.h"

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

UINT	/*WINAPI*/	ThreadProcIPG(LPVOID lpParam);
UINT	/*WINAPI*/	ThreadProcIPC(LPVOID lpParam);
UINT	/*WINAPI*/	ThreadProcCCG(LPVOID lpParam);
int	APIENTRY	MainWndProcIPG(HWND hDlg, int uMsg, WPARAM wParam, LPARAM lParam);
int	APIENTRY	MainWndProcIPC(HWND hDlg, int uMsg, WPARAM wParam, LPARAM lParam);
int	APIENTRY	MainWndProcCCG(HWND hDlg, int uMsg, WPARAM wParam, LPARAM lParam);

HWND		ghMain, ghThreadWinIPG, ghThreadWinIPC, ghThreadWinCCG;
CWinThread*	gpThreadIPG, gpThreadIPC, gpThreadCCG;

HANDLE ghIPGThreadTerminated = CreateEvent( NULL, TRUE, TRUE, "IPGThreadTerminated" );
HANDLE ghIPCThreadTerminated = CreateEvent( NULL, TRUE, TRUE, "IPGThreadTerminated" );
HANDLE ghCCGThreadTerminated = CreateEvent( NULL, TRUE, TRUE, "IPGThreadTerminated" );

LPCSTR AppNameIPG	= TEXT("IPG");
LPCSTR AppHdrIPG	= TEXT("IPGWindow");
LPCSTR AppNameIPC	= TEXT("IPC");
LPCSTR AppHdrIPC	= TEXT("IPCWindow");
LPCSTR AppNameCCG	= TEXT("CCG");
LPCSTR AppHdrCCG	= TEXT("CCGWindow");

CfgVAC_IP_CCG	g_cfgIPG, g_cfgIPC, g_cfgCCG;
CSerialPort		g_spIPG, g_spIPC, g_spCCG;
CString			g_strDataIPG, g_strDataIPC, g_strDataCCG;
CString			g_strDataIPGSaved, g_strDataIPCSaved, g_strDataCCGSaved;

HANDLE	g_hFileIPG = NULL;
HANDLE	g_hFileIPC = NULL;
HANDLE	g_hFileCCG = NULL;
int		g_nLoopsIPG, g_nLoopsIPC, g_nLoopsCCG;

UINT	InitIPG();
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

double	ScientificToDouble(CString strVal);
double	ScientificToDouble(CString strVal, double& dValue, int& nExp);

void	ReceiveText( CString& strData );
CString	ConvertReceiveData( CString strData );
CString	ConvertComData( CString strData, BOOL bIonP1 = TRUE );

int		ConvertReceiveData2( BYTE* rxdata, int nLen, CString& strOut );
CString	ConvertReceiveData2( CString strIn );
CString ConvertComData2( CString strIn, CByteArray& baData );
int		ConvertComData22( CString strIn, char* data );

CString ConvertCommData_Win( CString strWin, char cCmd, char cType, CString strData, CByteArray& baData );
CString	ConvertReceiveData_Win( CString strIn );
int		GetFieldLength_Win( CString strWin );

int		AnalyzeRecvData( CString strIn, CString& strOut, double& dValue, int& nExp );
void	LogIPs_CCG( int nType, CString strVacuum );

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
UINT InitIPG()
{
	// 注册窗口类window class
	WNDCLASS wc;
	// default values
	memset(&wc, 0, sizeof(wc));
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)MainWndProcIPG;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;
	wc.hIcon = NULL;
	wc.hCursor = 0;
	wc.hbrBackground = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = AppNameIPG;
	RegisterClass(&wc);
	
	// 生成一个窗口
	ghThreadWinIPG = CreateWindow(
		AppNameIPG,
		AppHdrIPG,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		100,
		100,
		NULL,
		NULL,
		NULL,
		NULL);
	if(! ghThreadWinIPG) 
		return 0;
	
	// 配置并打开串口
	g_cfgIPG.bReady = FALSE;
	if (g_spIPG.InitPort(CWnd::FromHandle(ghThreadWinIPG), g_cfgIPG.nPort, 9600, 'N', 8, 0))
	{
		g_spIPG.StartMonitoring();
		
		g_cfgIPG.nType	= 1;
		g_cfgIPG.bReady	= TRUE;
		g_cfgIPG.dVac		= 0.0;
		g_strDataIPG		= "";
		g_strDataIPGSaved	= "";
	}
	PostMessage( ghMain, WM_USER_VAC_UpdateStatus, g_cfgIPG.nType, g_cfgIPG.bReady );
	
	if( g_cfgIPG.bReady )
	{
		/////////////////////////////////////////////
		// 日志
		if( g_hFileIPG != NULL )
			CloseHandle( g_hFileIPG );
		g_hFileIPG	= NULL;
		// 设置日志
		TCHAR exeFullPath[255];
		::GetModuleFileName( NULL, exeFullPath, 255 );
		char *ptemp = strrchr( exeFullPath, '\\');
		ptemp[1] = 0;
		
		CString str, strTemp;
		CTime	timeCurrent = CTime::GetCurrentTime();
		strTemp = timeCurrent.Format("%Y%m%d%H%M");
		// 设置IP日志文件
		str.Format( "%szIPG%s.txt", exeFullPath, strTemp );
		g_hFileIPG = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		g_nLoopsIPG = 1;
		// 写IP日志文件头
		if( g_hFileIPG != NULL )
		{
			unsigned long cbRet;
			char Buf[255];
			str.Format( "\t%s", "IPG" );
			sprintf( Buf, "%s\n", str );
			WriteFile( g_hFileIPG, Buf, strlen(Buf), &cbRet, NULL );
		}
		/////////////////////////////////////////////
		
	}
	return 0;
}

UINT /*WINAPI*/ ThreadProcIPG(LPVOID lpParam)
{
	InitIPG();
	ResetEvent( ghIPGThreadTerminated );
	// 消息循环
	MSG msg;
/*	BOOL bRet;
	while( (bRet = GetMessage(&msg, ghThreadWinIPG, 0, 0) ) != 0)
	{
		if( bRet == -1 )
		{
			TRACE0( "HANDLE THE ERROR AND POSSIBLY EXIT");
		}
		else
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
*/
	while ( TRUE )
	{
		if( PeekMessage( &msg, ghThreadWinIPG, 0, 0, PM_NOREMOVE ) )
		{
			if( !GetMessage(&msg, ghThreadWinIPG, 0, 0) )
				break;
/*			if( msg.message == MM_ADDBLOCKDATA )
			{
			}
			else
*/			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
			WaitMessage();
    }

	// 关闭串口
	if( g_cfgIPG.bReady )
	{
		KillTimer(ghThreadWinIPG, 1);
		g_spIPG.ClosePort();
		g_spIPG.StopMonitoring();

		if( g_hFileIPG != NULL )
			CloseHandle( g_hFileIPG );
		g_hFileIPG	= NULL;
	}

	SetEvent( ghIPGThreadTerminated );
	AfxEndThread(0);
	return 0;
}

int APIENTRY MainWndProcIPG(HWND hDlg, int uMsg, WPARAM wParam, LPARAM lParam)
{
	char dataSend[255];

	CString str, strOut;
	int nExp = 0, n = 0;
	double dValue = 0.0;

	switch(uMsg)
	{
	case	WM_TIMER:
		n = ConvertComData22( "U01?", dataSend );
		g_spIPG.WriteToPort(dataSend, n);	//发送数据
		break;

	case	WM_COMM_RXCHAR:		// 接收数据
		str.Format( "%c", wParam );
		g_strDataIPG = g_strDataIPGSaved +str;
		TRACE( "%s\n", g_strDataIPG );
		// 分析数据
		n = AnalyzeRecvData( g_strDataIPG, strOut, dValue, nExp );
		if( n < 1 )
		{
			LogIPs_CCG( 1, strOut );
//			PostMessage( ghMain, WM_USER_VAC_UpdateIPGValue, (WPARAM)(dValue *10), nExp );
			PostMessage( ghMain, WM_USER_VAC_UpdateValue, (WPARAM)&strOut, g_cfgIPG.nType );
		}
		else
		{
			PostMessage( ghMain, WM_USER_VAC_UpdateValue, (WPARAM)&g_strDataIPG, g_cfgIPG.nType );
			g_strDataIPGSaved = g_strDataIPG;
			if( g_strDataIPG.GetLength() > 8 )
				g_strDataIPGSaved = str;
		}
		break;

	case	WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		//break;

	default:
		break;
	}
	return DefWindowProc(hDlg, uMsg,  wParam,  lParam);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

int AnalyzeRecvData( CString strIn, CString& strOut, double& dValue, int& nExp )
{
	CString str;
	strOut = "";

	BYTE bt = strIn[0];
	BOOL bIsRecvData = TRUE;
	switch( bt )
	{
	case	0x24:
		str	= ConvertReceiveData( strIn );
		break;
	case	0x01:
		str	= ConvertReceiveData2( strIn );
		break;
	case	0x02:
		str	= ConvertReceiveData_Win( strIn );
		break;
	default:
		bIsRecvData	= FALSE;
		break;
	}
	if( !bIsRecvData )
		return 1;	// 不是符合协议的数据

	int nLen = str.GetLength();
	if( nLen < 2 )
	{
		// 数据包不完整的时候先保存
		g_strDataIPGSaved = strIn;
		return 2;
	}
	if( nLen > 20 )
	{
		// 发生check error
		return 3;
	}

	int nPosCmd		= str.Find( "Cmd=" );
	int nPosData	= str.Find( "Data=" );
	if( nPosCmd < 0 || nPosData < 0 )
		return 4;	// 没有找到相关标志

	str	= str.Mid(nPosCmd+4, 2);
	if( str == "U0" )
	{
		// 真空值，用于监测、比较
		strOut = str.Right( nLen -nPosData -5 );
		g_cfgIPG.dVac = ScientificToDouble( strOut, dValue, nExp );
		return 0;
	}

	return 4;	// 不是真空值
}


void LogIPs_CCG( int nType, CString strVacuum )
{
	unsigned long cbRet;
	char Buf[255];
	CString str;

	if( nType == 1 )
	{
		if( g_hFileIPG != NULL )
		{
			CTime	timeCurrent = CTime::GetCurrentTime();
			CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
			str.Format( "[%s]\t%s", csCurrentTime, strVacuum );

			sprintf(Buf, "%s\n", str );
			WriteFile(g_hFileIPG, Buf, strlen(Buf), &cbRet, NULL);

			g_nLoopsIPG++;
			if( g_nLoopsIPG > 3600 )
			{
				CloseHandle( g_hFileIPG );
				// 设置日志文件
				TCHAR exeFullPath[255];
				::GetModuleFileName( NULL, exeFullPath, 255 );
				CTime	timeCurrent = CTime::GetCurrentTime();
				csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
				str.Format( "%szIPG%s.txt", exeFullPath, csCurrentTime );
				g_hFileIPG = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
				g_nLoopsIPG = 1;

				str.Format( "\t%s", "IPG" );
				sprintf( Buf, "%s\n", str );
				WriteFile( g_hFileIPG, Buf, strlen(Buf), &cbRet, NULL );
			}
		}
	}
	else if( nType == 2 )
	{
		if( g_hFileIPC != NULL )
		{
			CTime	timeCurrent = CTime::GetCurrentTime();
			CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
			str.Format( "[%s]\t%s", csCurrentTime, strVacuum );

			sprintf(Buf, "%s\n", str );
			WriteFile(g_hFileIPC, Buf, strlen(Buf), &cbRet, NULL);

			g_nLoopsIPC++;
			if( g_nLoopsIPC > 3600 )
			{
				CloseHandle( g_hFileIPC );
				// 设置日志文件
				TCHAR exeFullPath[255];
				::GetModuleFileName( NULL, exeFullPath, 255 );
				CTime	timeCurrent = CTime::GetCurrentTime();
				csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
				str.Format( "%szIPC%s.txt", exeFullPath, csCurrentTime );
				g_hFileIPC = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
				g_nLoopsIPC = 1;

				str.Format( "\t%s", "IPC" );
				sprintf( Buf, "%s\n", str );
				WriteFile( g_hFileIPC, Buf, strlen(Buf), &cbRet, NULL );
			}
		}
	}
	else
	{
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <math.h>

double ScientificToDouble(CString strVal)
{
	int		nIndexE = -1;	//'e'所在位置
	int		nCount = 0;		//'e'右侧的数值
	double	dSvl = 0;		//'e'左侧的数值
	
	nIndexE	= strVal.Find('E');
	dSvl	= atof(strVal.Left(nIndexE));
	
	if( "-" == strVal.Mid(nIndexE + 1, 1) )
	{
		nCount	= atoi(strVal.Mid(nIndexE + 2, strVal.GetLength()));
		dSvl	= dSvl / pow(10, nCount);
	}
	else
	{
		nCount	= atoi(strVal.Mid(nIndexE + 1, strVal.GetLength()));
		dSvl	= dSvl * pow(10, nCount);
	}
	
	return dSvl;
}

double ScientificToDouble(CString strVal, double& dValue, int& nExp)
{
	int		nIndexE = -1;	//'e'所在位置
	nExp	= 0;			//'e'右侧的数值（指数）
	dValue	= 0.0;			//'e'左侧的数值
	double dSvl = 0.0;
	
	nIndexE	= strVal.Find('E');
	dValue	= atof(strVal.Left(nIndexE));
	
	if( "-" == strVal.Mid(nIndexE + 1, 1) )
	{
		nExp	= atoi(strVal.Mid(nIndexE + 2, strVal.GetLength()));
		dSvl	= dValue / pow(10, nExp);
	}
	else
	{
		nExp	= atoi(strVal.Mid(nIndexE + 1, strVal.GetLength()));
		dSvl	= dValue * pow(10, nExp);
	}
	
	return dSvl;
}

CString ConvertComData( CString strData, BOOL bIonP1 )
{
	CString str, strTemp;
	int i,nLen;
	char cBuf[1];
	nLen = 0;
	strTemp.Format( "%02d", strData.GetLength() );
	str = "@" + strTemp + strData;
	for( i=0; i<str.GetLength(); i++ )
	{
		memcpy( cBuf, str.Mid( i, 1 ), 1 );
		nLen += cBuf[0];
	}
	strTemp.Format( "%04d", nLen );
	str += strTemp;
	return str;
}

CString ConvertComData2( CString strIn, CByteArray& baData )
{
	CString str, strTemp, strOut = "";

	int nLen = strIn.GetLength();
	baData.SetSize( 1 + 2 + nLen + 1 );	// 1head + 2Length + nData + 1Checksum
	baData[0] = 0x81;	// head
	// 2Length + nData
	str.Format( "%02d%s", nLen, strIn );
	
	nLen = str.GetLength();
	BYTE chk = 0x81;	// head
	strOut.Format( "%02c", chk );
	for( int i=0; i<nLen; i++ )
	{
		char ch = str[i];
		baData[i+1] = (BYTE)ch;
		chk ^= ch;		// 更新checksum
		strTemp.Format( "%c", baData[i+1] );
		strOut += strTemp;
	}
	baData[i+1] = chk & 0x7F;
	strTemp.Format( "%02c", baData[i+1] );
	strOut += strTemp;
	return strOut;
}

int ConvertComData22( CString strIn, char* data )
{
	CString str, strTemp, strOut = "";

	int nLen = strIn.GetLength();
	int nRetLen = ( 1 + 2 + nLen + 1 );	// 1head + 2Length + nData + 1Checksum
	// 2Length + nData
	str.Format( "%02d%s", nLen, strIn );
	
	nLen = str.GetLength();
	BYTE chk = 0x81;	// head
	strOut.Format( "%02c", chk );
	data[0] = chk;//0x81;	// head
	for( int i=0; i<nLen; i++ )
	{
		char ch = str[i];
		data[i+1] = ch;
		chk ^= ch;		// 更新checksum
		strTemp.Format( "%c", data[i+1] );
		strOut += strTemp;
	}
	data[i+1] = chk & 0x7F;
	strTemp.Format( "%02c", data[i+1] );
	strOut += strTemp;
	return nRetLen;
}

CString ConvertCommData_Win( CString strWin, char cCmd, char cType, CString strData, CByteArray& baData )
{
	CString str, strOut = "";

	// 计算消息长度
	int nLen = 1 +1 +3 +1 +1 +2;	// 1STX +1ADDR +3WIN +1COM +1ETX +2CRC;
	int nLenData = 0;
	switch( cType )
	{
	case	'L':
		nLenData	= 1;
		break;
	case	'N':
		nLenData	= 6;
		break;
	case	'A':
		nLenData	= 10;
		break;
	}
	nLen += nLenData;
	baData.SetSize( nLen );

	baData[0]		= 0x02;		// STX:  0x02
	baData[1]		= 0x80;		// ADDR: 0x80 for RS232
	for( int i=0; i<3; i++ )	// WIN:  a string of 3 numerics indicating the window number
		baData[i +2]= (BYTE)strWin[i];
	if( cType == 'R' )			// COM:  0x30 to read the window, 0x31 to write in the window
		baData[5]	= 0x30;
	else
	{
		baData[5]	= 0x31;
		// DATA: an alphanumeric ASCII string with the data to be written in the window.
		// In the case of a read command, this field is not present.
		// The field length varies according to the data type
		int nLenIn = strData.GetLength();
		int nZeroNum = nLenData -nLenIn;
		for( i=0; i<nZeroNum; i++ )
			baData[i +6] = 0x30;
		for( i=0; i<nLenIn; i++ )
			baData[ nZeroNum +6 +i ] = (BYTE)strData[i];
	}
	baData[nLen -3]	= 0x03;		// EXT:	 0x03

	// CRC: XOR of all characters following <STX> and including the <ETX> terminator.
	// The value is hexadecimal coded and represented by two ASCII characters.
	BYTE chk = 0x0;
	for( i=1; i<nLen -2; i++ )
		chk ^= baData[i];
	str.Format( "%02X", chk );
	baData[nLen -2] = (BYTE)str[0];
	baData[nLen -1]	= (BYTE)str[1];

	for( i=0; i<nLen; i++ )
	{
		str.Format( "%02Xh ", baData[i] );
		strOut += str;
	}
	return strOut;
}

CString ConvertReceiveData( CString strData )
{
	CString str, strOut = "";
	// 返回值格式为[header response1][length2][command2][channel1][datan][checksum4]
	str = strData;
	
	if( str.Left(1) != "$" )
		strOut = "Data header error!";
	else
	{
		char	cBuf[1];
		int		nLen=0;
		if( str.GetLength() < 3 )
			return strOut;	// 数据包不完整
		int		nDataPackLen	= atoi( str.Mid(1,2) );
		int		nSumOfAllBytes	= 1 +2 +nDataPackLen;	// header1 + length2 + data packet length (cmd +chn +dataField)
		if( nSumOfAllBytes +4 > str.GetLength() )
			return strOut;	// 数据包不完整
		for( int i=0; i<nSumOfAllBytes; i++ )
		{
			memcpy( cBuf, str.Mid(i, 1), 1);
			nLen += cBuf[0];
		}
		int nChecksum = atoi( str.Mid(nSumOfAllBytes, 4) );

		if( nChecksum != nLen )
			strOut.Format( "Data Checksum error! Sum = %04d, != %04d", nChecksum, nLen );
		else
			strOut.Format( "L=%d, Cmd=%s, Chn=%s, Data=%s",	nDataPackLen, str.Mid(3,2), str.Mid(5,1), str.Mid(6, nDataPackLen -3) );
	}

	return strOut;
}

int ConvertReceiveData2( BYTE* rxdata, int nLen, CString& strOut )
{
	// 返回值格式为[header response1][length2][command2][channel1][datan][checksum1]
	CString str, strTemp;
	BYTE bt, chk;

	strOut ="";
	// head
	bt = *(char*)rxdata;
	if( bt != 0x01 )
	{
		strOut = "Data header error!";
		return 0;	// 数据包头错了
	}

	if( nLen < 3 )
		return 1;	// 数据包不完整：不知道长度

	strTemp.Format( "%c%c", rxdata[1], rxdata[2] );
	int	nDataPackLen	= atoi( strTemp );
	int	nSumOfAllBytes	= 1 +2 +nDataPackLen;	// header1 + length2 + data packet length (cmd +chn +dataField)
	if( nSumOfAllBytes +1 > nLen )
		return 2;	// 数据包不完整：数据长度不够

	// checksum
	chk = 0x0;
	for( int i=0; i<nLen -1; i++ )
	{
		chk ^= rxdata[i];
	}
	chk &= 0x7F;
	if( chk != rxdata[nLen -1] )
	{
		strOut.Format("Data Checksum error! SumCal = %02X, != %02X", chk, rxdata[nLen -1] );
		return 3;	// 校验位不正确
	}

	// strOut = U01 5.6E-10
	for( i=3; i<nLen -1; i++ )	// 取cmd +chn +DataField
	{
		strTemp.Format( "%c", rxdata[i] );
		strOut += strTemp;
		if( i == 5 )
			strOut += " ";
	}
	return 2;
}

CString ConvertReceiveData2( CString strIn )
{
	// 返回值格式为[header response1][length2][command2][channel1][datan][checksum1]
	CString str, strTemp, strOut;
	BYTE bt, chk;

	strOut ="";
	str = strIn;
	// head
	bt = (BYTE)str[0];
	if( bt != 0x01 )
	{
		strOut = "Data header error!";
		return strOut;	// 数据包头错了
	}

	int nLen = str.GetLength();
	if( nLen < 3 )
		return strOut;	// 数据包不完整：不知道长度

	strTemp.Format( "%c%c", str[1], str[2] );
	int	nDataPackLen	= atoi( strTemp );
	int	nSumOfAllBytes	= 1 +2 +nDataPackLen;	// header1 + length2 + data packet length (cmd +chn +dataField)
	if( nSumOfAllBytes +1 > nLen )
		return strOut;	// 数据包不完整：数据长度不够

	// checksum
	chk = 0x0;
	for( int i=0; i<nLen -1; i++ )
	{
		chk ^= (BYTE)str[i];
	}
	chk &= 0x7F;
	if( chk != (BYTE)(str[nLen -1]) )
	{
		strOut.Format("Data Checksum error! SumCal = %02X, != %02X", chk, (BYTE)(str[nLen -1]) );
		return strOut;	// 校验位不正确
	}

	// strOut = U01 5.6E-10
// 	for( i=3; i<nLen -1; i++ )	// 取cmd +chn +DataField
// 	{
// 		strTemp.Format( "%c", rxdata[i] );
// 		strOut += strTemp;
// 		if( i == 5 )
// 			strOut += " ";
// 	}
	strOut.Format( "L=%d, Cmd=%s, Chn=%s, Data=%s",	nDataPackLen, str.Mid(3,2), str.Mid(5,1), str.Mid(6, nDataPackLen -3) );
	return strOut;
}

int GetFieldLength_Win( CString strWin )
{
	int nLen = 0;
	int nWin = atoi( strWin );
	switch( nWin )
	{
	case	11:
	case	12:
	case	13:
	case	14:
	case	504:
		nLen = 1;	// L: Logic
		break;
	case	108:
	case	205:
	case	206:
	case	503:
	case	505:
	case	600:
	case	610:
	case	612:
	case	613:
	case	614:
	case	620:
	case	622:
	case	623:
	case	624:
	case	630:
	case	632:
	case	633:
	case	634:
	case	640:
	case	642:
	case	643:
	case	644:
	case	800:
	case	801:
	case	802:
	case	808:
	case	809:
	case	810:
	case	812:
	case	820:
	case	822:
	case	830:
	case	832:
	case	840:
	case	842:
		nLen = 6;	// N: Numeric
		break;
	case	601:
	case	602:
	case	603:
	case	615:
	case	625:
	case	635:
	case	645:
	case	803:
	case	804:
	case	811:
	case	821:
	case	831:
	case	841:
		nLen = 10;	// A: Alphanumeric
		break;
	}
	return nLen;
}

CString ConvertReceiveData_Win( CString strIn )
{
	// 返回值格式为<STX1>+<ADDR1>+<WIN3>+<COM1>+<DATAn>+<ETX1>+<CRC2>
	CString str, strTemp, strOut;
	BYTE bt, chk;

	strOut ="";
	str = strIn;
	int nLen = str.GetLength();
	if( nLen < 6 )	// <STX1>+<ADDR1>+<WIN3>+<COM1>
		return strOut;
	// STX
	bt = (BYTE)str[0];
	if( bt != 0x02 )
	{
		strOut = "Data header error!";
		return strOut;	// 数据包头错了
	}
	// ADDR
	bt = (BYTE)str[1];
	if( bt != 0x80 )
	{
		strOut = "Data header error!";
		return strOut;	// 数据包头错了
	}
	if( nLen == 6 )
	{
		strTemp.Format( "%c%c%c%c%c%c", 0x02, 0x80, 0x06, 0x03, 0x38, 0x35 );
//		if( strTemp == str )
		if( (BYTE)str[2] == 0x06
			&& (BYTE)str[3] == 0x03
			&& (BYTE)str[4] == 0x38
			&& (BYTE)str[5] == 0x35 )
		{
			strOut = "ACK";
			return strOut;
		}
	}
	// WIN
	BYTE bCmd = (BYTE)str[5];
	int nDataLen = GetFieldLength_Win( str.Mid(2,3) );
	if( bCmd == 0x30 && nLen < 6 +nDataLen + 3 )
		return strOut;	// 数据包不完整：数据长度不够
	else if( nDataLen < 1 )
		return strOut;	// 非数据型消息

	// CRC
	for( int i=1; i<nLen -2; i++ )
		chk ^= (BYTE)str[i];
	strTemp.Format( "%02X", chk );
	if( strTemp != str.Right(2) )
	{
		strOut.Format("Data Checksum error! SumCal = %s, != %s", strTemp, str.Right(2) );
		return strOut;	// 校验位不正确
	}

	// DATA
	BOOL bNum = FALSE;	// Alphanumeric type
	if( nDataLen < 10 )
		bNum = TRUE;	// Numeric type
	BOOL bFound = FALSE;
	for( i=0; i<nDataLen; i++ )
	{
		bt = (BYTE)str[6 +i];
		if( bNum && bt>0x30 )
			bFound = TRUE;
		else if( !bNum && bt>0x20 )
			bFound = TRUE;
		if( !bFound  )
			continue;	// 忽略数据开始部分的0或者空格

		break;
	}
	strOut = str.Mid( 6+i, nDataLen -i );
	strOut.Format( "L=%d, Cmd=%s, Chn=%s, Data=%s",	nDataLen, str.Mid(3,2), str.Mid(5,1), str.Mid(6, nDataLen -3) );
	return strOut;
}

void ReceiveText( CString& strData )
{
	CString str, strASC1, strASC2;
	// These are declared Static so that
	// they will persist between function calls:
	static BOOL bCRflag;
	static CString strUse;
	strASC1.Format( "\015" );	// <CR>
	strASC2.Format( "\012" );	// <LF>

	for( int i=0; i<strData.GetLength()+1; i++ )
	{
		str = strData.Mid( i, 1 );
		if( str == strASC1 )
		{
			bCRflag = TRUE;
			str = "";
			strUse.TrimLeft();
			strUse = "";
		}
		else if( str == strASC2 )
		{
			str = "";
			bCRflag = FALSE;
		}

		if( !bCRflag )
		{
			strUse += str;
			TRACE( str );
		}
	}
}
