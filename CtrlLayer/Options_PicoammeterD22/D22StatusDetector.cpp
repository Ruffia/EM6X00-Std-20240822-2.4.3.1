#include "StdAfx.h"
#include "D22StatusDetector.h"
#include <math.h>

#define	nD22_Sleep		100

CD22StatusDetector& CD22StatusDetector::Instance()
{
	static CD22StatusDetector Inst;
	return Inst;
}

CD22StatusDetector::CD22StatusDetector(void)
{
	m_hWnd			= NULL;
	m_nPortD22		= -1;

	m_strD22		= "";
	m_strD22Saved	= "";

	m_hFileD22		= NULL;
	m_nLoopsD22		= 0;

	m_bPauseComm	= FALSE;
	m_bPauseInquire	= TRUE;	// FALSE;

	m_dwSleep		= 250;
}

void CD22StatusDetector::Init( HWND hWnd, int nPort )
{
	m_hWnd = hWnd;
	m_nPortD22 = nPort;
}

void CD22StatusDetector::SetParam( int nIndex, int nValue )
{
	switch( nIndex )
	{
	case	11:
		m_bPauseComm	= (BOOL)nValue;
		break;
	case	12:
//		m_bPausePost	= (BOOL)nValue;
		break;
	case	13:
		m_bPauseInquire	= (BOOL)nValue;
		break;
	}
}

BOOL CD22StatusDetector::Start()
{
	m_fBeam = -20.867;
//	PostMessage( m_hWnd, WM_USER_D22_UpdateValue, 11, (LPARAM)(&m_fBeam) );
	// 打开端口
	BOOL bCommOpen	= m_D22.Open( m_nPortD22 );
	if (!bCommOpen)
	{
		char szBuf[255] = {0};
		sprintf( szBuf, "CD22StatusDetector open failed! port = %d.\r\n", m_nPortD22 );
		OutputDebugString(szBuf);
		PostMessage( m_hWnd, WM_USER_D22_UpdateValue, 0, bCommOpen );
		return FALSE;
	}

	BOOL bCommReady = FALSE;
	if( GetVersion() )
	{
		bCommReady = TRUE;
		InitRecordFile();
	}
	PostMessage( m_hWnd, WM_USER_D22_UpdateValue, 1, (LPARAM)bCommReady );

	BOOL bStart = FALSE;
	if( bCommReady )
		bStart = __super::Start();
	return bStart;
}

void CD22StatusDetector::Release()
{
	m_D22.Close();
	m_nPortD22	= -1;

	if( m_hFileD22 != NULL )
		CloseHandle( m_hFileD22 );
	m_hFileD22	= NULL;

	__super::Release();
}

DWORD CD22StatusDetector::DoJob()
{
	if( !m_bPauseInquire )
	{
		m_fBeam = GetCurrentBeam();
		char szBuf[50];
		sprintf( szBuf, "%.5f", m_fBeam );
		Record( szBuf );
		if( m_fBeam < 9999.0 )
			PostMessage( m_hWnd, WM_USER_D22_UpdateValue, 11, (LPARAM)(&m_fBeam) );
	}
	Sleep( 1 );

	return 0L;
}

void CD22StatusDetector::InitRecordFile()
{
	if( m_hFileD22 != NULL )
		CloseHandle( m_hFileD22 );
	m_hFileD22	= NULL;

	// 设置日志文件
	TCHAR exeFullPath[255];
	::GetModuleFileName( NULL, exeFullPath, 255 );
	char *ptemp = strrchr( exeFullPath, '\\');
	ptemp[1] = 0;

	CString str, strTemp;
	CTime	timeCurrent = CTime::GetCurrentTime();
	strTemp = timeCurrent.Format("%Y%m%d%H%M");

	str.Format( "%s\\Log\\D22%s.csv", exeFullPath, strTemp );
	m_hFileD22 = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	m_nLoopsD22 = 1;

	if( m_hFileD22 != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		str.Format( "\t%s\r\n", "D22: Current Beam" );
		sprintf_s( Buf, "%s\r\n", str );
		WriteFile( m_hFileD22, Buf, strlen(Buf), &cbRet, NULL );
	}
}

void CD22StatusDetector::Record( LPCTSTR lpszRcd )
{
	if( m_hFileD22 == NULL )
		return;
	unsigned long cbRet;
	char Buf[255] = {0};
	CString str;

	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
	str.Format( "[%s],%s", csCurrentTime, lpszRcd );

	sprintf_s(Buf, "%s\r\n", str );
	WriteFile(m_hFileD22, Buf, strlen(Buf), &cbRet, NULL);

	m_nLoopsD22++;
	if( m_nLoopsD22 > 10000 )
		InitRecordFile();
}

//编写函数实现
//IEEE754 十六进制转十进制CbuffR
float CD22StatusDetector::ConvertReadData(BYTE* lbuff)
{
	float flocation;
	unsigned char highByte, lowByte;
	char Beam[8]={0};

	//16进制转字符串
	for (int i = 0; i < 4; i++)
	{
		highByte	= lbuff[i] >> 4;
		lowByte		= lbuff[i] & 0x0f ;

		highByte	+= 0x30;
		if (highByte > 0x39)
			Beam[i * 2] = highByte + 0x07;
		else
			Beam[i * 2] = highByte;

		lowByte += 0x30;
		if (lowByte > 0x39)
			Beam[i * 2 + 1] = lowByte + 0x07;
		else
			Beam[i * 2 + 1] = lowByte;
	}

	int nPos;
	sscanf_s(Beam, "%15x", &nPos);
	float* loca = (float*)&nPos;
	flocation = *loca;
	flocation = flocation * pow(10.0f,12);
	return flocation;
}

//CRC校验计算
unsigned short CD22StatusDetector::CRCcheck(BYTE* DataCode, int nLen)
{
	//计算CRC校验位
	unsigned short tmp = 0xffff;
//	unsigned short ret1 = 0;
	for(int n = 0; n < nLen; n++)
	{
		//此处的6 -- 要校验的位数为6个
		tmp = DataCode[n] ^ tmp;
		for(int i=0; i<8; i++ )
		{  
			//此处的8 -- 指每一个char类型又8bit，每bit都要处理
			if(tmp & 0x01)
			{
				tmp = tmp >> 1;
				tmp = tmp ^ 0xa001;
			}   
			else
			{
				tmp = tmp >> 1;
			}   
		}   
	}   

	return tmp;
/*	//将CRC校验的高低位对换位置
	ret1 = tmp >> 8;
	ret1 = ret1 | (tmp << 8); 
	DataCode[6]= tmp;	// 低8位
	DataCode[7]= ret1;	// 高8位*/
}

int CD22StatusDetector::SendCmd( BYTE bFunc, BYTE bK, BYTE bM )
{
	BYTE data[8] = {0};
	data[0] = 0xFF;
	data[1] = bFunc;
	data[2] = 0x00;
	data[3] = bK;
	data[4] = 0x00;
	data[5] = bM;
	// 计算CRC校验位
	unsigned short crc = CRCcheck( data, 6 );
	data[6] = (BYTE)(crc & 0x00FF);
	data[7] = (BYTE)((crc & 0xFF00) >> 8);

	int nLenSend= m_D22.SendDataEx(data, sizeof(data));
	if( nLenSend < 1 )
	{
		char szBuf[255] = {0};
		sprintf( szBuf, "D22: SendDataEx failed, LenSend = %d.\r\n", nLenSend );
		TRACE( szBuf );
	}
	return nLenSend;
}

BOOL CD22StatusDetector::ReadFromDevice( BYTE* data )
{
	char szBuf[255] = {0};

	int nLenTotal = 0;
	int nLenRecv = 0;
	int nTimes = 0;
	do{
		nLenRecv = m_D22.ReadData( data +nLenTotal, 255 );
		if( nLenRecv > 8 )
			break;
		nLenTotal += nLenRecv;
		nTimes++;
		Sleep(10);
	} while( nTimes < 50 );

	if( nLenRecv < 9 )
	{
		sprintf( szBuf, "D22: Read %d data, not enough!\r\n", nLenTotal );
		TRACE( szBuf );
		return FALSE;
	}

	unsigned crc = CRCcheck( data, 7 );
	BYTE crc1 = (BYTE)((crc & 0xFF00) >> 8);
	BYTE crc2 = (BYTE)(crc & 0x00FF);
	if( crc1 != data[7] || crc2 != data[8] )
	{
		sprintf( szBuf, "D22: CRC is not correct! calc_crc = 0x%04X, recv_crc = 0x%02X%02X\r\n", crc, data[7], data[8] );
		TRACE( szBuf );
		return FALSE;
	}
	return TRUE;
}

float CD22StatusDetector::GetCurrentBeam()
{
	BYTE buff[4]={0};
	int nLenSend = SendCmd( 0x03, 0x01, 0x01 );
	Sleep(nD22_Sleep);

	BYTE dataRecv[255];
	BOOL bValid = ReadFromDevice( dataRecv );
	if( !bValid )
		return 10000.0f;

	buff[0] = dataRecv[6]; buff[1] = dataRecv[5];
	buff[2] = dataRecv[4]; buff[3] = dataRecv[3];
	/*int nBeamVal = (int)(ConvertReadData(buff)+0.5);*/
	float fBeamVal = ConvertReadData(buff);
	return fBeamVal;
}

//版本查询
BOOL CD22StatusDetector::GetVersion()
{
	BYTE buff[4] = {0};
	int nLenSend = SendCmd( 0x02, 0x03, 0x01 );
	Sleep(nD22_Sleep);

	BYTE data[8] = {0};
	data[0] = 0xFF;
	data[1] = 0x02;
	data[2] = 0x00;
	data[3] = 0x03;
	data[4] = 0x00;
	data[5] = 0x01;
	// 计算CRC校验位
	unsigned short crc = CRCcheck( data, 6 );
	data[6] = (BYTE)(crc & 0x00FF);
	data[7] = (BYTE)((crc & 0xFF00) >> 8);
	char szBuf[255] = {0};
	sprintf( szBuf, "D22 GetVersion: send [%d], 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\r\n",
			nLenSend, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7] );
	OutputDebugString(szBuf);

	BYTE dataRecv[255] = {0};
	BOOL bValid = ReadFromDevice( dataRecv );
	sprintf( szBuf, "D22 GetVersion: recv = 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\r\n",
		dataRecv[0], dataRecv[1], dataRecv[2], dataRecv[3], dataRecv[4], dataRecv[5], dataRecv[6], dataRecv[7], dataRecv[8] );
	OutputDebugString(szBuf);

	return bValid;
}
