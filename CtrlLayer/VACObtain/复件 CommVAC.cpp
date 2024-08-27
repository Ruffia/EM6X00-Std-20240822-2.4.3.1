// CommVAC.cpp : implementation file
//

#include "stdafx.h"
#include "CommVAC.h"

#include "Serial.h"
#include <VACobtain.h>

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD WINAPI CommThread(LPVOID lpParam);
DWORD WINAPI CommThread2(LPVOID lpParam);

HANDLE	g_hThread, g_hThread2;
DWORD	g_dwThread, g_dwThread2;
HANDLE	g_hEventWork		= CreateEvent(NULL,FALSE, FALSE, _T("WORKic"));
HANDLE	g_hEventTeminate	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEic"));
HANDLE	g_hEventWork2		= CreateEvent(NULL,FALSE, FALSE, _T("WORKboard"));
HANDLE	g_hEventTeminate2	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEboard"));

HWND	g_hMain;
CSerial	g_IPG, g_IPC, g_CCG, g_board;
CfgVAC_IP_CCG	g_cfgIPG, g_cfgIPC, g_cfgCCG, g_cfgBoard;
VACboardStatusSet	g_boardStatusSetServer, g_boardStatusSetMain;

CString g_strIPG, g_strIPC, g_strCCG, g_strBoard;
CString g_strIPGSaved, g_strIPCSaved, g_strCCGSaved, g_strBoardSaved;

HANDLE	g_hFileIPG	= NULL;
HANDLE	g_hFileIPC	= NULL;
HANDLE	g_hFileCCG	= NULL;
HANDLE	g_hFileBoard= NULL;
int		g_nLoopsIPG, g_nLoopsIPC, g_nLoopsCCG, g_nLoopsBoard;

BOOL	g_bReady	= FALSE;		// 是否至少有一个设备联通
BOOL	g_bPauseComm	= FALSE;	// 是否暂停与设备通讯
BOOL	g_bPausePost	= FALSE;	// 是否暂停上送监测值（但不停止与设备通讯）

/////////////////////////////////////////////////////////////////////////////
// CCommVAC

CCommVAC::CCommVAC()
{
	g_hMain	= NULL;
	
	g_cfgIPG.bReady	= FALSE;
	g_cfgIPC.bReady	= FALSE;
	g_cfgCCG.bReady	= FALSE;
	g_cfgIPG.nType	= 1;
	g_cfgIPC.nType	= 2;
	g_cfgCCG.nType	= 3;
	g_cfgIPG.dVac	= 0.0;
	g_cfgIPC.dVac	= 0.0;
	g_cfgCCG.dVac	= 0.0;

	g_cfgBoard.bReady	= FALSE;
	g_cfgBoard.nType	= 4;
	g_cfgBoard.dVac		= 0.0;
	g_boardStatusSetServer.bV1		= 0x02;	// close
	g_boardStatusSetServer.bVent	= 0x03;	// pump
	g_boardStatusSetMain.bV1		= 0x02;	// close
	g_boardStatusSetMain.bVent		= 0x03;	// pump
}

CCommVAC::~CCommVAC()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCommVAC message handlers

double ScientificToDouble(CString strVal)
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

int ConvertCommData_Win2( CString strWin, char cType, CString strData, char* data )
{
	CString str, strOut = "";

	// 计算消息长度
	int nLen = 1 +1 +3 +1 +1 +2;	// 1STX +1ADDR +3WIN +1COM +1ETX +2CRC;
/*	int nLenData = 0;
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
*/
	BYTE chk	= 0x02;	// STX:  0x02
	data[0]		= chk;
	chk	= 0x80;			// ADDR: 0x80 for RS232
	data[1]		= chk;		
	for( int i=0; i<3; i++ )	// WIN:  a string of 3 numerics indicating the window number
		data[i +2]= (BYTE)strWin[i];
	if( cType == 'R' )			// COM:  0x30 to read the window, 0x31 to write in the window
		data[5]	= 0x30;
	else
	{
		data[5]	= 0x31;
		// DATA: an alphanumeric ASCII string with the data to be written in the window.
		// In the case of a read command, this field is not present.
		// The field length varies according to the data type
// 		int nLenIn = strData.GetLength();
// 		int nZeroNum = nLenData -nLenIn;
// 		for( i=0; i<nZeroNum; i++ )
// 			baData[i +6] = 0x30;
// 		for( i=0; i<nLenIn; i++ )
// 			baData[ nZeroNum +6 +i ] = (BYTE)strData[i];
	}
	data[nLen -3]	= 0x03;		// EXT:	 0x03

	// CRC: XOR of all characters following <STX> and including the <ETX> terminator.
	// The value is hexadecimal coded and represented by two ASCII characters.
	chk = 0x00;
	for( i=1; i<nLen -2; i++ )
		chk ^= data[i];
	str.Format( "%02X", chk );
	data[nLen -2]	= (BYTE)str[0];
	data[nLen -1]	= (BYTE)str[1];

	for( i=0; i<nLen; i++ )
	{
		str.Format( "%c ", data[i] );
		strOut += str;
	}
	return nLen;
}

int ConvertReceiveData( CString strData, CString& strOut )
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

int ConvertReceiveData2( CString strIn, CString& strOut )
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

int ConvertReceiveData_Win( CString strIn, CString& strOut )
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
	if( (bt < 0x80) || (bt > 0x80+31) )
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
	chk = 0x00;
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
	strOut = str.Mid( 6+i, nLen -(6+i) -3 );
	strOut.Format( "L=%d, Cmd=%s, Chn=%s, Data=%s",	nLen, str.Mid(2,3), str.Mid(5,1), str.Mid(6+i, nLen -(6+i) -3) );
	return 0;
}

int AnalyzeRecvData( CString strIn, CString& strOut, double& dValue, int& nExp )
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
//		m_strDataSaved = strIn;
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

	if( bt != 0x02 )
	{
		CString strCmd = str.Mid(nPosCmd+4, 2);
		if( strCmd == "U0" )
		{
			// 真空值，用于监测、比较
			int nLen = str.GetLength();
			strOut = str.Right( nLen -nPosData -5 );
			ScientificToDouble( strOut, dValue, nExp );
			return 0;
		}
	}
	else
	{
		CString strCmd = str.Mid(nPosCmd+4, 3);
		if( (strCmd == "812") || (strCmd == "822") )
		{
			// 真空值，用于监测、比较
			int nLen = str.GetLength();
			strOut = str.Right( nLen -nPosData -5 );
			ScientificToDouble( strOut, dValue, nExp );
			return 0;
		}
	}

	return 5;	// 不是真空值
}

void RecordVacuum( int nType, CString strVacuum )
{
	if( nType == 1 && g_hFileIPG != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		CString str;

		CTime	timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
		str.Format( "[%s]\t%s", csCurrentTime, strVacuum );

		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFileIPG, Buf, strlen(Buf), &cbRet, NULL);

		g_nLoopsIPG++;
		if( g_nLoopsIPG > 10000 )
		{
			CloseHandle( g_hFileIPG );
			// 设置日志文件
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			char *ptemp = strrchr( exeFullPath, '\\');
			ptemp[1] = 0;
			CTime	timeCurrent = CTime::GetCurrentTime();
			csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
			str.Format( "%szIPG%s.txt", exeFullPath, csCurrentTime );
			g_hFileIPG = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoopsIPG = 1;

			str.Format( "\t%s\r\n", "IPG" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFileIPG, Buf, strlen(Buf), &cbRet, NULL );
		}
	}
	else if( nType == 2 && g_hFileIPC != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		CString str;

		CTime	timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
		str.Format( "[%s]\t%s", csCurrentTime, strVacuum );

		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFileIPC, Buf, strlen(Buf), &cbRet, NULL);

		g_nLoopsIPC++;
		if( g_nLoopsIPC > 10000 )
		{
			CloseHandle( g_hFileIPC );
			// 设置日志文件
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			char *ptemp = strrchr( exeFullPath, '\\');
			ptemp[1] = 0;
			CTime	timeCurrent = CTime::GetCurrentTime();
			csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
			str.Format( "%szIPC%s.txt", exeFullPath, csCurrentTime );
			g_hFileIPC = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoopsIPC = 1;

			str.Format( "\t%s\r\n", "IPC" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFileIPC, Buf, strlen(Buf), &cbRet, NULL );
		}
	}
	else if( nType == 3 && g_hFileCCG != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		CString str;

		CTime	timeCurrent = CTime::GetCurrentTime();
		CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
		str.Format( "[%s]\t%s", csCurrentTime, strVacuum );

		sprintf(Buf, "%s\r\n", str );
		WriteFile(g_hFileCCG, Buf, strlen(Buf), &cbRet, NULL);

		g_nLoopsCCG++;
		if( g_nLoopsCCG > 10000 )
		{
			CloseHandle( g_hFileCCG );
			// 设置日志文件
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			char *ptemp = strrchr( exeFullPath, '\\');
			ptemp[1] = 0;
			CTime	timeCurrent = CTime::GetCurrentTime();
			csCurrentTime = timeCurrent.Format("%Y%m%d%H%M");
			str.Format( "%szCCG%s.txt", exeFullPath, csCurrentTime );
			g_hFileCCG = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoopsCCG = 1;

			str.Format( "\t%s\r\n", "CCG" );
			sprintf( Buf, "%s\r\n", str );
			WriteFile( g_hFileCCG, Buf, strlen(Buf), &cbRet, NULL );
		}
	}
}

int CommWithIPG( int nSleep )
{
	if( !g_cfgIPG.bReady )
		return 1;

	char dataSend[255];
//	int nLenSend = ConvertComData22( "U01?", dataSend );
//	g_IPG.SendData(dataSend, nLenSend);	//发送数据
	int nLenSend = ConvertCommData_Win2( "812", 'R', "", dataSend );
	g_IPG.SendData( dataSend, nLenSend );

	Sleep(nSleep);

	char dataRecv[255];
	int nLenRecv = g_IPG.ReadData( dataRecv, 19 );	// nLenSend要根据实际情况改
	if( nLenRecv < 2 )
	{
		Sleep( 1 );
		return 2;
	}

	CString str, strOut, strTemp = "";
	for( int i=0; i<nLenRecv; i++ )
	{
		str.Format( "%c", dataRecv[i] );
		strTemp += str;
	}
	g_strIPG = g_strIPGSaved + strTemp;

	// 分析数据
	double dValue = 0.0;
	int nExp = 0;
	int n = AnalyzeRecvData( g_strIPG, strOut, dValue, nExp );
	if( n < 1 )
	{
		RecordVacuum( 1, strOut );
		if( !g_bPausePost )
			PostMessage( g_hMain, WM_USER_VAC_UpdateIPGValue, (WPARAM)(dValue *10), nExp );
		g_strIPG		= "";
		g_strIPGSaved	= "";
		return 0;
	}
	else
	{
		if( n == 2 )
			g_strIPGSaved = g_strIPG;
//		PostMessage( g_hMain, WM_USER_VAC_UpdateIPGValue, 72, -10 );
		g_strIPGSaved = g_strIPG;
		if( g_strIPG.GetLength() > 6 )
			g_strIPGSaved = "";
		return 3;
	}
}

int CommWithIPC( int nSleep )
{
	if( !g_cfgIPC.bReady )
		return 1;

	char dataSend[255];
	int nLenSend = ConvertComData22( "U01?", dataSend );
	g_IPC.SendData(dataSend, nLenSend);	//发送数据

	Sleep(nSleep);

	char dataRecv[255];
	int nLenRecv = g_IPC.ReadData( dataRecv, 15 );	// nLenSend要根据实际情况改
	if( nLenRecv < 2 )
	{
		Sleep( 1 );
		return 2;
	}

	CString str, strOut, strTemp = "";
	for( int i=0; i<nLenRecv; i++ )
	{
		str.Format( "%c", dataRecv[i] );
		strTemp += str;
	}
	g_strIPC = g_strIPCSaved + strTemp;

	// 分析数据
	double dValue = 0.0;
	int nExp = 0;
	int n = AnalyzeRecvData( g_strIPC, strOut, dValue, nExp );
	if( n < 1 )
	{
		RecordVacuum( 2, strOut );
		if( !g_bPausePost )
			PostMessage( g_hMain, WM_USER_VAC_UpdateIPCValue, (WPARAM)(dValue *10), nExp );
		g_strIPC		= "";
		g_strIPCSaved	= "";
		return 0;
	}
	else
	{
		if( n == 2 )
			g_strIPCSaved = g_strIPC;
//		PostMessage( g_hMain, WM_USER_VAC_UpdateIPCValue, 13, -8 );
		g_strIPCSaved = g_strIPC;
		if( g_strIPC.GetLength() > 6 )
			g_strIPCSaved = "";
		return 3;
	}
}

void CCG_ConvertSendData( const CString strOutput, short nCom )
{
	CString str;
	switch( nCom )
	{
	case	0:
/*		str = strOutput + "\r\n";
		m_commEVR116.SetOutBufferCount( 0 );
		m_commEVR116.SetOutput( COleVariant(str) );
*/		break;
	case	1:
		str = strOutput + "\r";
		break;
	case	2:	// <ENQ>
		str.Format( "\005" );
		break;
	}
}

int CCG_ConvertRequestMeasureData( CString strIn, char* data )
{
	int nRetLen = strIn.GetLength() +2;
	// e.g. PR1<CR><LF>
	CString str, strEnd;
	strEnd.Format( "\015\012" );
	str.Format( "%s%s", strIn, strEnd );
	int nLen = str.GetLength();
	for( int i=0; i<nLen; i++ )
	{
		char ch = str[i];
		data[i] = ch;
	}
	return nRetLen;
}

void CCG_ReceiveText( CString& strData )
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

CString CCG_ConvertReceiveData( CString strData )
{
	CString str, strStatus, strTemp, strOut;
	int nPos;
	//	double dValue;
	
	// 使用PR1命令，返回值格式为x,sx.xxxxEsxx<CR><LF>
	str = strData;
	strOut = "";
//	strRepSignal.Format( "\015\012" );
//	if( strTemp.Right( 2 ) == strRepSignal )
	str.Left( str.GetLength() -2 );
//	MessageVGC401( str, FALSE );
	strStatus = str.Left( 1 );
	if( atoi( strStatus ) == 0 )	// Measurement data okay
	{
		strTemp = str.Right( str.GetLength() - 2 );
		nPos = strTemp.Find( 'E', 0 );
		strOut.Format( "%.2f", atof( strTemp.Left(nPos) ) );
		strOut += strTemp.Mid( nPos, 4 );
	}
	return strOut;
}

void CommWithCCG()
{
	if( !g_cfgCCG.bReady )
		return;

	// Request for data transmission: <ENQ>
	CString str;
	str.Format( "\005" );
	char dataSend[25], dataRecv[255];
	dataSend[0] = str[0];
	g_CCG.SendData(dataSend, 1);
	// Status and pressure: e.g. 0,8.3400E-03<CR><LF>
	Sleep(1);
	CString strOut, strTemp = "";
	int nLenRecv = g_CCG.ReadData( dataRecv, 15 );
	for( int i=0; i<nLenRecv; i++ )
	{
		str.Format( "%c", dataRecv[i] );
		strTemp += str;
	}
	if( nLenRecv != 15 )
		g_strCCG = g_strCCGSaved + strTemp;
	else
	{
		g_strCCG = strTemp;
		// 分析数据
		strOut = CCG_ConvertReceiveData( g_strCCG );
		double dValue = 0.0;
		int nExp = 0;
		ScientificToDouble( strOut, dValue, nExp );
		RecordVacuum( 3, strOut );
		if( !g_bPausePost )
			PostMessage( g_hMain, WM_USER_VAC_UpdateCCGValue, (WPARAM)(dValue*100), nExp );
		g_strCCG		= "";
		g_strCCGSaved	= "";
	}
	// 如果第一位不是0，则重新发送请求: PR1<CR>[<LF>]--><ACK><CR><LF>--><ENQ>-->status and pressure
}

DWORD WINAPI CommThread(LPVOID lpParam)
{
	// 打开端口
	g_cfgIPG.bReady	= g_IPG.Open( g_cfgIPG.nPort );
	g_cfgIPC.bReady	= g_IPC.Open( g_cfgIPC.nPort );
	g_cfgCCG.bReady	= g_CCG.Open( g_cfgCCG.nPort );

	/////////////////////////////////////////////
	// 设置日志
	TCHAR exeFullPath[255];
	::GetModuleFileName( NULL, exeFullPath, 255 );
	char *ptemp = strrchr( exeFullPath, '\\');
	ptemp[1] = 0;
	
	CString str, strTemp;
	CTime	timeCurrent = CTime::GetCurrentTime();
	strTemp = timeCurrent.Format("%Y%m%d%H%M");

	if( g_cfgIPG.bReady )
	{
		if( CommWithIPG( 200 ) > 1 )
			g_cfgIPG.bReady = FALSE;
		else
		{
			// 设置IPG日志文件
			str.Format( "%szIPG%s.txt", exeFullPath, strTemp );
			if( g_hFileIPG != NULL )
				CloseHandle( g_hFileIPG );
			g_hFileIPG	= NULL;
			g_hFileIPG = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoopsIPG = 1;
			// 写IPG日志文件头
			if( g_hFileIPG != NULL )
			{
				unsigned long cbRet;
				char Buf[255];
				str.Format( "\t%s\r\n", "IPG" );
				sprintf( Buf, "%s\r\n", str );
				WriteFile( g_hFileIPG, Buf, strlen(Buf), &cbRet, NULL );
			}
		}
	}
	if( g_cfgIPC.bReady )
	{
		if( CommWithIPC( 200 ) > 1 )
			g_cfgIPC.bReady = FALSE;
		else
		{
			// 设置IPC日志文件
			str.Format( "%szIPC%s.txt", exeFullPath, strTemp );
			if( g_hFileIPC != NULL )
				CloseHandle( g_hFileIPC );
			g_hFileIPC	= NULL;
			g_hFileIPC = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoopsIPC = 1;
			// 写IPC日志文件头
			if( g_hFileIPC != NULL )
			{
				unsigned long cbRet;
				char Buf[255];
				str.Format( "\t%s\r\n", "IPC" );
				sprintf( Buf, "%s\r\n", str );
				WriteFile( g_hFileIPC, Buf, strlen(Buf), &cbRet, NULL );
			}
		}
	}
	if( g_cfgCCG.bReady )
	{
		/////////////////////////////////////////////////////////
		// Request for measurement data: PR1<CR>[<LF>]
		char dataSend[255], dataRecv[255];
		CString strReq = "PR1";
		CString strAck;
		strAck.Format( "\006\015\012" );	// <ACK><CR><LF>
		strTemp = "";
		int nLenSend, nLenRecv, nTimes = 0;
		nLenSend= CCG_ConvertRequestMeasureData( strReq, dataSend );
		do
		{
			g_CCG.SendData(dataSend, nLenSend);	// 发送数据
			Sleep(1);
			// Positive acknowledgement: <ACK><CR><LF>
			nLenRecv = g_CCG.ReadData( dataRecv, 3);
			for( int i=0; i<nLenRecv; i++ )
			{
				str.Format( "%c", dataRecv[i] );
				strTemp += str;
			}
			if( strTemp.GetLength() == 3 )
			{
				if( strTemp == strAck )
					break;
			}
			nTimes++;
		} while( nTimes < 10 );
		/////////////////////////////////////////////////////////
		if( nTimes == 10 )
			g_cfgCCG.bReady = FALSE;
		else
		{
			// 设置CCG日志文件
			str.Format( "%szCCG%s.txt", exeFullPath, strTemp );
			if( g_hFileCCG != NULL )
				CloseHandle( g_hFileCCG );
			g_hFileCCG	= NULL;
			g_hFileCCG = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoopsCCG = 1;
			// 写CCG日志文件头
			if( g_hFileCCG != NULL )
			{
				unsigned long cbRet;
				char Buf[255];
				str.Format( "\t%s\r\n", "CCG" );
				sprintf( Buf, "%s\r\n", str );
				WriteFile( g_hFileCCG, Buf, strlen(Buf), &cbRet, NULL );
			}
		}
		/////////////////////////////////////////////
	}
	PostMessage( g_hMain, WM_USER_VAC_UpdateStatus, 1, g_cfgIPG.bReady );
	PostMessage( g_hMain, WM_USER_VAC_UpdateStatus, 2, g_cfgIPC.bReady );
	PostMessage( g_hMain, WM_USER_VAC_UpdateStatus, 3, g_cfgCCG.bReady );

	if( g_cfgIPG.bReady || g_cfgIPC.bReady || g_cfgCCG.bReady )
		g_bReady = TRUE;
	else
		g_bReady = FALSE;

	while( g_bReady && WaitForSingleObject(g_hEventTeminate,1000) != WAIT_OBJECT_0 )
	{
		if( !g_bPauseComm )
		{
			// 1秒钟通讯一次，读一次真空值
			CommWithIPG(1);
			CommWithIPC(1);
			CommWithCCG();
		}
		Sleep( 1 );
	}

	g_IPG.Close();
	g_IPC.Close();
	g_CCG.Close();
	g_cfgIPG.bReady	= g_IPG.IsOpened();
	g_cfgIPC.bReady	= g_IPC.IsOpened();
	g_cfgCCG.bReady	= g_CCG.IsOpened();
	if( g_hFileIPG != NULL )
		CloseHandle( g_hFileIPG );
	g_hFileIPG	= NULL;
	if( g_hFileIPC != NULL )
		CloseHandle( g_hFileIPC );
	g_hFileIPC	= NULL;
	if( g_hFileCCG != NULL )
		CloseHandle( g_hFileCCG );
	g_hFileCCG	= NULL;

	SetEvent( g_hEventWork );
	return 0;
}


void Board_ConvertSendData( BYTE byteOpc, BYTE byteOpr, char* data )
{
	data[0] = byte_Head;
	data[1]	= byteOpc;
	data[2]	= byteOpr;
	data[3]	= data[1] ^ data[2];
	data[4]	= byte_Tail;
}

int Board_ConvertReceiveData( int nLen, char* data )
{
	if( nLen < byte_LenRec )
		return 1;	// 不够一帧数据
	if( data[0] != byte_Head )
		return 2;	// 帧头不对
	if( data[5] != byte_Tail )
		return 3;	// 帧尾不对

	char chk = data[1] ^ data[2];
	chk ^= data[3];
	if( chk != data[4] )
		return 4;	// 校验码不对

	BYTE bOpc = data[1];
	if( bOpc == byte_opcStatus )	// 状态
	{
		WORD wStatus = (data[2] << 8) | data[3];
		PostMessage( g_hMain, WM_USER_VAC_UpdateStatus, byte_opcStatus, (LPARAM)wStatus );
	}
	else if( bOpc == byte_opcIPG )	// IPG
	{
		WORD wIPG = (data[2] << 8) | data[3];
		PostMessage( g_hMain, WM_USER_VAC_UpdateStatus, byte_opcIPG, (LPARAM)wIPG );
	}
	else if( bOpc == byte_opcClr )
	{
		// Clr和Warning的命令值是一样的
		if( (data[2] == 0x00) && (data[3] == 0x00) )
//			TRACE( "Gun error has been cleaned\n");
			PostMessage( g_hMain, WM_USER_VAC_UpdateStatus, byte_opcClr, 0 );
		else if( data[2] == 0x91 )	// warning
		{
			if( data[3] == 0x01 )
				TRACE( "Gun error!\n" );
			else if( data[3] == 0x00 )
				TRACE( "TMP error!\n" );
			PostMessage( g_hMain, WM_USER_VAC_UpdateStatus, byte_opcWarning+1, (LPARAM)(data[3]) );
		}
	}
	return 0;
}

void Board_SendCmd( BYTE byteOpc, BYTE byteOpr )
{
	char dataSend[5];
	Board_ConvertSendData( byteOpc, byteOpr, dataSend );
	g_board.SendData(dataSend, byte_LenSnd);
}

int CommWithBoard( int nSleep )
{
	if( !g_cfgBoard.bReady )
		return 1;

	char dataSend[5];
	if( g_boardStatusSetServer.bVent != g_boardStatusSetMain.bVent )
	{
		if( g_boardStatusSetMain.bVent == 0x01 )	// 放气
			Board_ConvertSendData( byte_opcVent, byte_opcVent, dataSend );
		if( g_boardStatusSetMain.bVent == 0x03 )	// 抽气
			Board_ConvertSendData( byte_opcPump, byte_opcPump, dataSend );
		g_boardStatusSetServer.bVent = g_boardStatusSetMain.bVent;
		g_board.SendData(dataSend, byte_LenSnd);
	}
	if( g_boardStatusSetServer.bV1 != g_boardStatusSetMain.bV1 )
	{
		Board_ConvertSendData( byte_opcV1, g_boardStatusSetMain.bV1, dataSend );
		g_boardStatusSetServer.bV1 = g_boardStatusSetMain.bV1;
		g_board.SendData(dataSend, byte_LenSnd);
	}

	Sleep(nSleep);

	char dataRecv[255];
	int nLenRecv = 0;
	int nCount = 0;
	do{
		Board_SendCmd( byte_opcStatus, byte_opcStatus );		// 常规查询状态
		Sleep(nSleep);
		nLenRecv = g_board.ReadData( dataRecv, byte_LenRec );	// 上传数据一帧都是6个字节
		if( nLenRecv < 1 )
			Sleep(nSleep);
		Board_ConvertReceiveData( nLenRecv, dataRecv );
		nCount++;
	} while( (nLenRecv < 1) && (nCount < 5) );
	if( nCount == 5 )
		return 2;

	CString str, strOut, strTemp = "";
	for( int i=0; i<nLenRecv; i++ )
	{
		str.Format( "%c", dataRecv[i] );
		strTemp += str;
	}
	g_strIPG = g_strIPGSaved + strTemp;
	return 0;
}

DWORD WINAPI CommThread2(LPVOID lpParam)
{
	// 打开端口
	g_cfgBoard.bReady = g_board.Open( g_cfgBoard.nPort );
	if( g_cfgBoard.bReady )
	{
		if( CommWithBoard(100) > 0 )
			g_cfgBoard.bReady = FALSE;
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
			str.Format( "%szBoard%s.txt", exeFullPath, strTemp );
			if( g_hFileBoard != NULL )
				CloseHandle( g_hFileBoard );
			g_hFileBoard	= NULL;
			g_hFileBoard	= CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoopsBoard	= 1;
			// 写Board日志文件头
			if( g_hFileBoard != NULL )
			{
				unsigned long cbRet;
				char Buf[255];
				str.Format( "\t%s\r\n", "Board" );
				sprintf( Buf, "%s\r\n", str );
				WriteFile( g_hFileIPC, Buf, strlen(Buf), &cbRet, NULL );
			}
		}
	}
	PostMessage( g_hMain, WM_USER_VAC_UpdateStatus, 4, g_cfgBoard.bReady );

	while( g_cfgBoard.bReady && WaitForSingleObject(g_hEventTeminate2,1000) != WAIT_OBJECT_0 )
	{
		if( !g_bPauseComm )
		{
			// 1秒钟通讯一次
			CommWithBoard(1);
		}
		Sleep( 1 );
	}

	g_board.Close();
	g_cfgBoard.bReady	= g_board.IsOpened();
	if( g_hFileBoard != NULL )
		CloseHandle( g_hFileBoard );
	g_hFileBoard	= NULL;

	SetEvent( g_hEventWork2 );
	return 0;
}

void CCommVAC::SetParam( int nIndex, int nValue )
{
	switch( nIndex )
	{
	case	1:
		g_cfgIPG.nPort	= nValue;
		break;
	case	2:
		g_cfgIPC.nPort	= nValue;
		break;
	case	3:
		g_cfgCCG.nPort	= nValue;
		break;
	case	4:
		g_cfgBoard.nPort= nValue;
		break;

	case	11:
		g_bPauseComm	= (BOOL)nValue;
		break;
	case	12:
		g_bPausePost	= (BOOL)nValue;
		break;

	case	101:
	case	103:
		g_boardStatusSetMain.bVent	= (BYTE)nValue;
		break;
	case	102:
		g_boardStatusSetMain.bV1	= (BYTE)nValue;
		break;

	case	(100 +0x11):
	case	(100 +0x21):
	case	(100 +0x91):
		Board_SendCmd( (BYTE)(nIndex-100), (BYTE)nValue );
		break;
	}
}

BOOL CCommVAC::Init( CWnd* pWnd )
{
	g_hMain = pWnd->m_hWnd;
	g_strIPG	= "";
	g_strIPC	= "";
	g_strCCG	= "";
	g_strBoard	= "";
	g_strIPGSaved	= "";
	g_strIPCSaved	= "";
	g_strCCGSaved	= "";
	g_strBoardSaved	= "";

	/////////////////////////////////////////////////////////////////
	ResetEvent( g_hEventWork );
	ResetEvent( g_hEventTeminate );

	g_hThread	= CreateThread( NULL, 0, CommThread, NULL, 0, &g_dwThread );
	if( g_hThread == NULL )
		return FALSE;

	CloseHandle(g_hThread);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	ResetEvent( g_hEventWork2 );
	ResetEvent( g_hEventTeminate2 );

	g_hThread2	= CreateThread( NULL, 0, CommThread2, NULL, 0, &g_dwThread2 );
	if( g_hThread2 == NULL )
		return FALSE;

	CloseHandle(g_hThread2);
	/////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL CCommVAC::Close()
{
	if( g_bReady )
	{
		SetEvent( g_hEventTeminate );
		WaitForSingleObject( g_hEventWork, INFINITE );
		g_bReady = FALSE;
	}
	if( g_cfgBoard.bReady )
	{
		SetEvent( g_hEventTeminate2 );
		WaitForSingleObject( g_hEventWork2, INFINITE );
		g_cfgBoard.bReady = FALSE;
	}

	return TRUE;
}
