// IonPump.cpp : implementation file
//

#include "stdafx.h"
#include "IonPump.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIonPump

IMPLEMENT_DYNCREATE(CIonPump, CWinThread)

CIonPump::CIonPump()
{
	m_bAutoDelete	= FALSE;

	// kill event starts out in the signaled state
	m_hEventKill	= CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventDead	= CreateEvent(NULL, TRUE, FALSE, NULL);

	m_cfg.bReady	= FALSE;

	m_hMain			= NULL;
	m_hFil			= NULL;
}

CIonPump::~CIonPump()
{
	Close();
	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);
}

BOOL CIonPump::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	// loop but check for kill notification
	while( WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT )
		MainLoop();

	// avoid entering standard message loop by returning FALSE
	return FALSE;	// TRUE;
}

int CIonPump::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CIonPump, CWinThread)
	//{{AFX_MSG_MAP(CIonPump)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIonPump message handlers

void CIonPump::Delete()
{
	// calling the base here won't do anything but it is a good habit
	CWinThread::Delete();
	
	// acknowledge receipt of kill notification
	VERIFY(SetEvent(m_hEventDead));
}

void CIonPump::KillThread()
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

BOOL CIonPump::Close()
{
	BOOL bReady = m_cfg.bReady;	// 保存当前状态
	m_cfg.bReady = FALSE;		// 告诉Mainloop不要执行了
	if( bReady )
	{
		if( m_IP.IsOpened() )
			m_IP.Close();
	}

	if( m_hFil != NULL )
		CloseHandle( m_hFil );
	m_hFil	= NULL;
	return TRUE;
}

BOOL CIonPump::Reset( CWnd* pWnd )
{
	if( m_cfg.bReady )
		Close();

	m_hMain = pWnd->m_hWnd;
//	Init();

	return TRUE;
}

BOOL CIonPump::Init()
{
	m_cfg.bReady	= FALSE;
	m_cfg.dVac		= 0.0;

	m_lastTicks	= 0;
	// 串口端口设置
	if( m_IP.Open( m_cfg.nPort) )
	{
		m_cfg.bReady	= TRUE;
		m_lastTicks		= GetTickCount();
	}
	PostMessage( m_hMain, WM_USER_VAC_UpdateStatus, m_cfg.nType, m_cfg.bReady );

	if( !m_IP.IsOpened() )
		return FALSE;

	/////////////////////////////////////////////
	// 日志
	if( m_hFil != NULL )
		CloseHandle( m_hFil );
	m_hFil	= NULL;
	// 设置日志
	TCHAR exeFullPath[255];
	::GetModuleFileName( NULL, exeFullPath, 255 );
	char *ptemp = strrchr( exeFullPath, '\\');
	ptemp[1] = 0;

	CString str, strTemp;
	CTime	timeCurrent = CTime::GetCurrentTime();
	strTemp = timeCurrent.Format("%Y%m%d%H%M");
	// 设置IP日志文件
	if( m_cfg.nType == 1 )
		str.Format( "%szIPG%s.txt", exeFullPath, strTemp );
	else
		str.Format( "%szIPC%s.txt", exeFullPath, strTemp );
	m_hFil = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	m_nLoops = 1;
	// 写IP日志文件头
	if( m_hFil != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		if( m_cfg.nType == 1 )
			str.Format( "\t%s", "IPG" );
		else
			str.Format( "\t%s", "IPC" );
		sprintf( Buf, "%s\n", str );
		WriteFile( m_hFil, Buf, strlen(Buf), &cbRet, NULL );
	}
	/////////////////////////////////////////////
	m_strData		= "";
	m_strDataSaved	= "";

	return TRUE;
}

void CIonPump::ClearDevice()
{

}

void CIonPump::SetParam( int nIndex, int nValue )
{
	switch( nIndex )
	{
	case	0:
		m_cfg.nPort	= nValue;
		break;
	case	1:
		m_cfg.nType	= nValue;
		break;
	}
}

double CIonPump::ScientificToDouble(CString strVal)
{
	int		nIndexE = -1;	//'e'所在位置
	int		nCount = 0;		//'e'右侧的数值
	double	dSvl = 0;		//'e'左侧的数值
	CString	strReturn = "";
	
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

double CIonPump::ScientificToDouble(CString strVal, double& dValue, int& nExp)
{
	int		nIndexE = -1;	//'e'所在位置
	nExp	= 0;			//'e'右侧的数值（指数）
	dValue	= 0.0;			//'e'左侧的数值
	double dSvl = 0.0;
	
	nIndexE	= strVal.Find('E');
	dValue	= atof(strVal.Left(nIndexE));
	
	if( "-" == strVal.Mid(nIndexE + 1, 1) )
	{
		nExp	= -atoi(strVal.Mid(nIndexE + 2, strVal.GetLength()));
		dSvl	= dValue / pow(10, nExp);
	}
	else
	{
		nExp	= atoi(strVal.Mid(nIndexE + 1, strVal.GetLength()));
		dSvl	= dValue * pow(10, nExp);
	}
	
	return dSvl;
}

CString CIonPump::ConvertComData( CString strData, BOOL bIonP1 )
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

CString CIonPump::ConvertComData2( CString strIn, CByteArray& baData )
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

int CIonPump::ConvertComData22( CString strIn, char* data )
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

CString CIonPump::ConvertCommData_Win( CString strWin, char cCmd, char cType, CString strData, CByteArray& baData )
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

int CIonPump::ConvertReceiveData( CString strData, CString& strOut )
{
	CString str;
	strOut = "";
	// 返回值格式为[header response1][length2][command2][channel1][datan][checksum4]
	str = strData;
	
	if( str.Left(1) != "$" )
	{
		strOut = "Data header error!";
		return 1;
	}

	char	cBuf[1];
	int		nLen=0;
	if( str.GetLength() < 3 )
		return 2;	// 数据包不完整
	int		nDataPackLen	= atoi( str.Mid(1,2) );
	int		nSumOfAllBytes	= 1 +2 +nDataPackLen;	// header1 + length2 + data packet length (cmd +chn +dataField)
	if( nSumOfAllBytes +4 > str.GetLength() )
		return 3;	// 数据包不完整
	for( int i=0; i<nSumOfAllBytes; i++ )
	{
		memcpy( cBuf, str.Mid(i, 1), 1);
		nLen += cBuf[0];
	}
	int nChecksum = atoi( str.Mid(nSumOfAllBytes, 4) );

	if( nChecksum != nLen )
	{
		strOut.Format( "Data Checksum error! Sum = %04d, != %04d", nChecksum, nLen );
		return 4;
	}

	strOut.Format( "L=%d, Cmd=%s, Chn=%s, Data=%s",	nDataPackLen, str.Mid(3,2), str.Mid(5,1), str.Mid(6, nDataPackLen -3) );
	return 0;
}

int CIonPump::ConvertReceiveData2( BYTE* rxdata, int nLen, CString& strOut )
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
		return 1;	// 数据包头错了
	}

	if( nLen < 3 )
		return 2;	// 数据包不完整：不知道长度

	strTemp.Format( "%c%c", rxdata[1], rxdata[2] );
	int	nDataPackLen	= atoi( strTemp );
	int	nSumOfAllBytes	= 1 +2 +nDataPackLen;	// header1 + length2 + data packet length (cmd +chn +dataField)
	if( nSumOfAllBytes +1 > nLen )
		return 3;	// 数据包不完整：数据长度不够

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
		return 4;	// 校验位不正确
	}

	// strOut = U01 5.6E-10
	for( i=3; i<nLen -1; i++ )	// 取cmd +chn +DataField
	{
		strTemp.Format( "%c", rxdata[i] );
		strOut += strTemp;
		if( i == 5 )
			strOut += " ";
	}
	return 0;
}

int CIonPump::ConvertReceiveData2( CString strIn, CString& strOut )
{
	// 返回值格式为[header response1][length2][command2][channel1][datan][checksum1]
	CString str, strTemp;
	BYTE bt, chk;

	strOut ="";
	str = strIn;
	// head
	bt = (BYTE)str[0];
	if( bt != 0x01 )
	{
		strOut = "Data header error!";
		return 1;	// 数据包头错了
	}

	int nLen = str.GetLength();
	if( nLen < 3 )
		return 2;	// 数据包不完整：不知道长度

	strTemp.Format( "%c%c", str[1], str[2] );
	int	nDataPackLen	= atoi( strTemp );
	int	nSumOfAllBytes	= 1 +2 +nDataPackLen;	// header1 + length2 + data packet length (cmd +chn +dataField)
	if( nSumOfAllBytes +1 > nLen )
		return 3;	// 数据包不完整：数据长度不够

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
		return 4;	// 校验位不正确
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
	return 0;
}

int CIonPump::GetFieldLength_Win( CString strWin )
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

int CIonPump::ConvertReceiveData_Win( CString strIn, CString& strOut )
{
	// 返回值格式为<STX1>+<ADDR1>+<WIN3>+<COM1>+<DATAn>+<ETX1>+<CRC2>
	CString str, strTemp;
	BYTE bt, chk;

	strOut ="";
	str = strIn;
	int nLen = str.GetLength();
	if( nLen < 6 )	// <STX1>+<ADDR1>+<WIN3>+<COM1>
		return 1;
	// STX
	bt = (BYTE)str[0];
	if( bt != 0x02 )
	{
		strOut = "Data header error!";
		return 2;	// 数据包头错了
	}
	// ADDR
	bt = (BYTE)str[1];
	if( bt != 0x80 )
	{
		strOut = "Data header error!";
		return 3;	// 数据包头错了
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
			return 4;
		}
	}
	// WIN
	BYTE bCmd = (BYTE)str[5];
	int nDataLen = GetFieldLength_Win( str.Mid(2,3) );
	if( bCmd == 0x30 && nLen < 6 +nDataLen + 3 )
		return 5;	// 数据包不完整：数据长度不够
	else if( nDataLen < 1 )
		return 6;	// 非数据型消息

	// CRC
	for( int i=1; i<nLen -2; i++ )
		chk ^= (BYTE)str[i];
	strTemp.Format( "%02X", chk );
	if( strTemp != str.Right(2) )
	{
		strOut.Format("Data Checksum error! SumCal = %s, != %s", strTemp, str.Right(2) );
		return 7;	// 校验位不正确
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
	return 0;
}

void CIonPump::ReceiveText( CString& strData )
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

int CIonPump::AnalyzeRecvData( CString strIn, CString& strOut, double& dValue, int& nExp )
{
	CString str;
	strOut = "";
	
	BYTE bt = strIn[0];
	BOOL bIsRecvData = TRUE;
	int nRet = 0;
	switch( bt )
	{
	case	0x24:
		nRet	= ConvertReceiveData( strIn, str );
		break;
	case	0x01:
		nRet	= ConvertReceiveData2( strIn, str );
		break;
	case	0x02:
		nRet	= ConvertReceiveData_Win( strIn, str );
		break;
	default:
		bIsRecvData	= FALSE;
		break;
	}
	if( !bIsRecvData )
		return 1;	// 不是符合协议的数据
	
	if( nRet == 2 || nRet == 3 )
	{
		// 数据包不完整的时候先保存
		m_strDataSaved = strIn;
		return 2;
	}
	if( nRet == 4 )
	{
		// 发生check error
		return 3;
	}
	
	int nPosCmd		= str.Find( "Cmd=" );
	int nPosData	= str.Find( "Data=" );
	if( nPosCmd < 0 || nPosData < 0 )
		return 4;	// 没有找到相关标志

	CString strCmd = str.Mid(nPosCmd+4, 2);
	if( strCmd == "U0" )
	{
		// 真空值，用于监测、比较
		int nLen = str.GetLength();
		strOut = str.Right( nLen -nPosData -5 );
		m_cfg.dVac = ScientificToDouble( strOut, dValue, nExp );
		return 0;
	}
	
	return 5;	// 不是真空值
}

void CIonPump::LogIPG_IPC( int nType, CString strVacuum )
{
	if( m_hFil != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		CString str;

		CTime	timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
		str.Format( "[%s]\t%s", csCurrentTime, strVacuum );

		sprintf(Buf, "%s\n", str );
		WriteFile(m_hFil, Buf, strlen(Buf), &cbRet, NULL);

		m_nLoops++;
		if( m_nLoops > 3600 )
		{
			CloseHandle( m_hFil );
			// 设置日志文件
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			CTime	timeCurrent = CTime::GetCurrentTime();
			csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
			if( m_cfg.nType == 1 )
				str.Format( "%szIPG%s.txt", exeFullPath, csCurrentTime );
			else
				str.Format( "%szIPC%s.txt", exeFullPath, csCurrentTime );
			m_hFil = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			m_nLoops = 1;

			if( m_cfg.nType == 1 )
				str.Format( "\t%s", "IPG" );
			else
				str.Format( "\t%s", "IPC" );
			sprintf( Buf, "%s\n", str );
			WriteFile( m_hFil, Buf, strlen(Buf), &cbRet, NULL );
		}
	}
}

void CIonPump::MainLoop()
{
	if( !m_cfg.bReady )
	{
		Sleep( 1 );
		return;
	}

	DWORD thisTickCount = GetTickCount();
	///////////////////////////////////////////////////////
	// 常规监测
	///////////////////////////////////////////////////////
	if( m_lastTicks > 0 && thisTickCount - m_lastTicks > 2000 )
	{
// 		CByteArray baData;
// 		CString strSend = ConvertComData2( "U01?", baData );
// 		m_IP.WriteToPort((LPCTSTR)strSend);	//发送数据
		char dataSend[255];
		int nLenSend = ConvertComData22( "U01?", dataSend );
		m_IP.SendData(dataSend, nLenSend);	//发送数据

		Sleep(1);

		char dataRecv[255];
		int nLenRecv = m_IP.ReadData( dataRecv, 15 );	// nLenSend要根据实际情况改
		if( nLenRecv < 2 )
		{
			Sleep( 1 );
			return;
		}

//		PostMessage( m_hMain, WM_USER_VAC_UpdateValue, m_cfg.nType, (LPARAM)&dataRecv );
		CString str, strOut, strTemp = "";
		for( int i=0; i<nLenRecv; i++ )
		{
			str.Format( "%c", dataRecv[i] );
			strTemp += str;
		}
//		PostMessage( m_hMain, WM_USER_VAC_UpdateValue, m_cfg.nType, (LPARAM)&strOut );

		m_strData = m_strDataSaved + strTemp;
		TRACE( "Data = %s\n", m_strData );

		// 分析数据
		double dValue = 0.0;
		int nExp = 0;
		int n = AnalyzeRecvData( m_strData, strOut, dValue, nExp );
		if( n < 1 )
		{
			LogIPG_IPC( m_cfg.nType, strOut );
			if( m_cfg.nType == 1 )
				PostMessage( m_hMain, WM_USER_VAC_UpdateIPGValue, (WPARAM)(dValue *10), nExp );
			else
				PostMessage( m_hMain, WM_USER_VAC_UpdateIPCValue, (WPARAM)(dValue *10), nExp );
			m_strData		= "";
			m_strDataSaved	= "";
		}
		else
		{
			if( m_cfg.nType == 1 )
				PostMessage( m_hMain, WM_USER_VAC_UpdateIPGValue, 72, -10 );
			else
				PostMessage( m_hMain, WM_USER_VAC_UpdateIPCValue, 13, -9 );
			m_strDataSaved = m_strData;
			if( m_strData.GetLength() > 6 )
				m_strDataSaved = "";
		}

		m_lastTicks = GetTickCount();
	}

	Sleep( 1 );
}
