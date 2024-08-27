#include "stdafx.h"
#include "CommonVACFunction.h"
#include <math.h>
#include "IPCStatusDetector.h"

CCommonVACFunction& CCommonVACFunction::Instance()
{
	static CCommonVACFunction Inst;
	return Inst;
}


CCommonVACFunction::CCommonVACFunction()
{
}



int CCommonVACFunction::ConvertComData22( CString strIn, char* data )
{
	CString str, strTemp, strOut = "";

	int i, nLen = strIn.GetLength();
	int nRetLen = ( 1 + 2 + nLen + 1 );	// 1head + 2Length + nData + 1Checksum
	// 2Length + nData
	str.Format( "%02d%s", nLen, strIn );

	nLen = str.GetLength();
	BYTE chk = 0x81;	// head
	strOut.Format( "%02c", chk );
	data[0] = chk;//0x81;	// head
	for( i=0; i<nLen; i++ )
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


int CCommonVACFunction::ConvertCommData_Win2( CString strWin, char cType, CString strData, char* data )
{
	CString str, strOut = "";

	// 计算消息长度
	int i, nLen = 1 +1 +3 +1 +1 +2;	// 1STX +1ADDR +3WIN +1COM +1ETX +2CRC;
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
	for( i=0; i<3; i++ )	// WIN:  a string of 3 numerics indicating the window number
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


int CCommonVACFunction::AnalyzeRecvData( CString strIn, BOOL bSameChn, CString& strOut, double& dValue, int& nExp )
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
		if( (strCmd == "812") || (strCmd == "822")  || (strCmd == "832") || (strCmd == "842"))
		{
			// 真空值，用于监测、比较
			int nLen = str.GetLength();
			strOut = str.Right( nLen -nPosData -5 );
			//ScientificToDouble( strOut, dValue, nExp );
			double dValue = atof(strOut);
			if (bSameChn)
			{
				return 0;
			}
			//if( strCmd == g_strWinIPG )
			//{
			//	RecordVacuum( 1, strOut );
			//	if( !g_bPausePost )
			//		PostMessage( g_hMain, WM_USER_VAC_UpdateIPGValue, (WPARAM)(dValue *10), nExp );
			//}
			//else if( strCmd == g_strWinIPC )
			//{
			//	RecordVacuum( 2, strOut );
			//	if( !g_bPausePost )
			//		PostMessage( g_hMain, WM_USER_VAC_UpdateIPCValue, (WPARAM)(dValue *10), nExp );
			//}

			return 0;
		}
	}

	return 5;	// 不是真空值
}



double CCommonVACFunction::ScientificToDouble(CString strVal)
{
	int		nIndexE = -1;	//'e'所在位置
	int		nCount = 0;		//'e'右侧的数值
	double	dSvl = 0;		//'e'左侧的数值

	strVal.MakeLower();
	nIndexE	= strVal.Find('e');
	dSvl	= atof(strVal.Left(nIndexE));
	nCount	= atoi(strVal.Mid(nIndexE + 1, strVal.GetLength()));
	dSvl	= dSvl * pow((double)10, (int)nCount);
/*
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
*/
	return dSvl;
}


double CCommonVACFunction::ScientificToDouble(CString strVal, double& dValue, int& nExp)
{
	int		nIndexE = -1;	//'e'所在位置
	nExp	= 0;			//'e'右侧的数值（指数）
	dValue	= 0.0;			//'e'左侧的数值
	double dSvl = 0.0;

	nIndexE	= strVal.Find('E');
	dValue	= atof(strVal.Left(nIndexE));

	if( "-" == strVal.Mid(nIndexE + 1, 1) )
		nExp	= -atoi(strVal.Mid(nIndexE + 2, strVal.GetLength()));
	else
		nExp	= atoi(strVal.Mid(nIndexE + 1, strVal.GetLength()));
	dSvl	= dValue * pow((double)10, (int)nExp);

	return dSvl;
}


int CCommonVACFunction::ConvertReceiveData( CString strData, CString& strOut )
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
	int		i, nLen=0;
	if( str.GetLength() < 3 )
		return 2;	// 数据包不完整
	int		nDataPackLen	= atoi( str.Mid(1,2) );
	int		nSumOfAllBytes	= 1 +2 +nDataPackLen;	// header1 + length2 + data packet length (cmd +chn +dataField)
	if( nSumOfAllBytes +4 > str.GetLength() )
		return 3;	// 数据包不完整
	for( i=0; i<nSumOfAllBytes; i++ )
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


int CCommonVACFunction::ConvertReceiveData2( CString strIn, CString& strOut )
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
	for(int i=0; i<nLen -1; i++ )
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


int CCommonVACFunction::ConvertReceiveData_Win( CString strIn, CString& strOut )
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
	int i=0;
	for(i=0; i<nDataLen; i++ )
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


int CCommonVACFunction::GetFieldLength_Win( CString strWin )
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
