#include "StdAfx.h"
#include "CCGStatusDetector.h"

#define TryCount 3

CCCGStatusDetector& CCCGStatusDetector::Instance()
{
	static CCCGStatusDetector Inst;
	return Inst;
}

CCCGStatusDetector::CCCGStatusDetector(void)
{
	m_hWnd = NULL;
	m_strCCG = "";
	m_strCCGSaved = "";

	//m_bPauseCommCCG	= FALSE;	// ��տ��ư��ϵ�CCG��Դ�غ�PC��CCG��ӿ�������ͨѶ�Ͷ��ˣ�����PC��CCG��ͨѶ�赥������

	m_hFileCCG = NULL;
	m_nLoopsCCG = 0;

	ZeroMemory( &m_cfgCCG, sizeof(CfgVAC_IP_CCG) );
	m_cfgCCG.nType	= 3;

	m_dwSleep = 250;
}

void CCCGStatusDetector::Init( HWND hWnd,int nPort )
{
	m_hWnd = hWnd;
	m_cfgCCG.nPort = nPort;
}

void CCCGStatusDetector::SetPause( BOOL bPause )
{
	if( bPause )
	{
//		if( m_cfgCCG.bReady )
			Release();
	}
	else
	{
//		if( !(m_cfgCCG.bReady) )
			Start();
	}
}

void CCCGStatusDetector::InitThreshold(double dThreshhold1,double dThreshhold2)
{
	m_cfgCCG.dThreshold1 = dThreshhold1;
	m_cfgCCG.dThreshold2 = dThreshhold2;
}

BOOL CCCGStatusDetector::Start()
{
	char sz[512] = {0};
	// �򿪶˿�
	BOOL bCommOpen = m_CCG.Open( m_cfgCCG.nPort );
	if (!bCommOpen)
	{
		PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 3, bCommOpen );
		ZeroMemory(sz,512);
		sprintf_s(sz,512,"CCG::Start(), bCommOpen (COM %d) failed.\r\n", m_cfgCCG.nPort);
		OutputDebugString(sz);
		return FALSE;
	}

	InitRecordFile();
	_ReqUnit();
	if (FALSE == m_cfgCCG.bReady)
	{
		PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 3, m_cfgCCG.bReady );
		ZeroMemory(sz,512);
		sprintf_s(sz,512,"CCG::Start(), _ReqUnit failed.\r\n");
		OutputDebugString(sz);
	}

	if( m_cfgCCG.bReady )
	{
		_ReqSP1();
		if (FALSE == m_cfgCCG.bReady)
		{
			PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 3, m_cfgCCG.bReady );
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"CCG::Start(), _ReqSP1 failed.\r\n");
			OutputDebugString(sz);
		}
	}

	if( m_cfgCCG.bReady )
	{
		_ReqPR1();
		if (FALSE == m_cfgCCG.bReady)
		{
			PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 3, m_cfgCCG.bReady );
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"CCG::Start(), _ReqPR1 failed.\r\n");
			OutputDebugString(sz);
		}
	}

	BOOL bStart = FALSE;
	if( m_cfgCCG.bReady )
		bStart = __super::Start();
	else
	{
		if( m_hFileCCG != NULL )
			CloseHandle( m_hFileCCG );
		m_hFileCCG = NULL;
	}
	return bStart;
}

void CCCGStatusDetector::Release()
{
	m_CCG.Close();
	m_cfgCCG.bReady	= FALSE;	// m_CCG.IsOpened();
	m_cfgCCG.nPort	= -1;
	if( m_hFileCCG != NULL )
		CloseHandle( m_hFileCCG );
	m_hFileCCG	= NULL;

	__super::Release();
}

DWORD CCCGStatusDetector::DoJob()
{
	// 1����ͨѶһ�Σ���һ�����ֵ
	CommWithCCG(100);

	return 0L;
}

void CCCGStatusDetector::CCG_ConvertSendData( const CString strOutput, short nCom )
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

int CCCGStatusDetector::CCG_ConvertRequestMeasureData( CString strIn, char* data )
{
	int nRetLen = strIn.GetLength() +2;
	// e.g. PR1<CR><LF>
	CString str, strEnd;
	strEnd.Format( "\015\012" );
	str.Format( "%s%s", strIn, strEnd );
	int nLen = str.GetLength();
	for(int i=0; i<nLen; i++ )
	{
		char ch = str[i];
		data[i] = ch;
	}
	return nRetLen;
}

void CCCGStatusDetector::CCG_ReceiveText( CString& strData )
{
	CString str, strASC1, strASC2;
	// These are declared Static so that
	// they will persist between function calls:
	static BOOL bCRflag;
	static CString strUse;
	strASC1.Format( "\015" );	// <CR>
	strASC2.Format( "\012" );	// <LF>

	for(int i=0; i<strData.GetLength()+1; i++ )
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

CString CCCGStatusDetector::CCG_ConvertReceiveData( CString strData )
{
	CString str, strStatus, strTemp, strOut, strRepSignal;
	int nPos, nLength;
	//	double dValue;
	
	// ʹ��PR1�������ֵ��ʽΪx,sx.xxxxEsxx<CR><LF>
	str = strData;
	strOut = "";
	strRepSignal.Format( "\015\012" );
	nPos = str.Find( strRepSignal );
	nLength = str.GetLength();
	if( nPos < nLength -2 )
	{
		// �س����ַ����м�
		m_strCCGSaved = str.Right( nLength -nPos -2 );
		str = str.Left( nPos );
	}
	else
		m_strCCGSaved = "";
	if( str.GetLength() < 13 )
		return strOut;
	strStatus = str.Left( 1 );
//	if( atoi( strStatus ) == 0 )	// Measurement data okay
	{
		strTemp = str.Right( str.GetLength() - 2 );
		nPos = strTemp.Find( 'E', 0 );
		strOut.Format( "%.2f", atof( strTemp.Left(nPos) ) );
		strOut += strTemp.Mid( nPos, 4 );
	}
	return strOut;
}

void CCCGStatusDetector::CommWithCCG( int nSleep )
{
	// Request for data transmission: <ENQ>
	CString str;
	str.Format( "\005" );
	char dataSend[25] = {0}, dataRecv[255] = {0};
	dataSend[0] = str[0];
	m_CCG.SendData(dataSend, 1);
	// Status and pressure: e.g. 0,8.3400E-03<CR><LF>
	Sleep(nSleep);
	CString strOut, strOutPut, strTemp = "";
	int nLenRecv = m_CCG.ReadData( dataRecv, 15 );
	for(int i=0; i<nLenRecv; i++ )
	{
		str.Format( "%c", dataRecv[i] );
		strTemp += str;

		CString strHex;
		strHex.Format( "%02X ", dataRecv[i] );
		strOutPut += strHex;
	}

	char sz[512] = {0};

	m_strCCG = m_strCCGSaved + strTemp;
	if( m_strCCG.GetLength() < 15 )
	{
		m_strCCGSaved = m_strCCG;
		return;		// ����һ��������
	}

	//OutputDebugString(m_strCCG);
	// ��������
	strOut = CCG_ConvertReceiveData( m_strCCG );
	RecordVacuum( strOut );
	m_cfgCCG.dVac = atof( strOut );
	//if( !m_bPausePost )

	PostMessage( m_hWnd, WM_USER_VAC_UpdateCCGValue, 0, (LPARAM)(&m_cfgCCG) );
	m_strCCG = "";
	// �����һλ����0�������·�������: PR1<CR>[<LF>]--><ACK><CR><LF>--><ENQ>-->status and pressure
}

void CCCGStatusDetector::InitRecordFile()
{
	if( m_hFileCCG != NULL )
		CloseHandle( m_hFileCCG );
	m_hFileCCG	= NULL;

	// ������־
	TCHAR exeFullPath[255];
	::GetModuleFileName( NULL, exeFullPath, 255 );
	char *ptemp = strrchr( exeFullPath, '\\');
	ptemp[1] = 0;

	CString str, strTemp;
	CTime	timeCurrent = CTime::GetCurrentTime();
	strTemp = timeCurrent.Format("%Y%m%d%H%M");

	// ����IPG��־�ļ�
	str.Format( "%s\\Log\\CCG%s.csv", exeFullPath, strTemp );
	m_hFileCCG = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	m_nLoopsCCG = 1;
	// дIPs��־�ļ�ͷ
	if( m_hFileCCG != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		str.Format( "\t%s\r\n", "CCG" );
		sprintf_s( Buf, "%s\r\n", str );
		WriteFile( m_hFileCCG, Buf, strlen(Buf), &cbRet, NULL );
	}
}

void CCCGStatusDetector::RecordVacuum( CString strVacuum )
{
	unsigned long cbRet;
	char Buf[255];
	CString str;

	CTime	timeCurrent = CTime::GetCurrentTime();
	CString csCurrentTime = timeCurrent.Format("%H:%M:%S");
	str.Format( "[%s],%s", csCurrentTime, strVacuum );

	sprintf(Buf, "%s\r\n", str );
	WriteFile(m_hFileCCG, Buf, strlen(Buf), &cbRet, NULL);

	m_nLoopsCCG++;
	if( m_nLoopsCCG > 10000 )
		InitRecordFile();
}


BOOL CCCGStatusDetector::_ReqUnit() 
{
	CString str, strOut, strTemp, strAck;
	char dataSend[255], dataRecv[255];
	int i, nUnit, nLenSend, nLenRecv, nTimes = 0;
	strAck.Format( "\006\015\012" );	// <ACK><CR><LF>
	BOOL bSucceed;

	int nTimesCmd1	= 0;
	int nTimesCmd2	= 0;
	// ��ѯ��նȵ�λ
	CString strReq = "UNI";
	nLenSend = CCG_ConvertRequestMeasureData( strReq, dataSend );
	do{
		strTemp = "";
		bSucceed = FALSE;
		do{
			// �ȷ���������õ�ACK����
			m_CCG.SendDataEx2(dataSend, nLenSend);
			Sleep(100);
			nLenRecv = m_CCG.ReadData(dataRecv, 3);
			for( i=0; i<nLenRecv; i++ )
			{
				str.Format( "%c", dataRecv[i] );
				strTemp += str;
			}
			if( strTemp.GetLength() == 3 )
			{
				if( strTemp == strAck )
				{
					bSucceed = TRUE;
					break;
				}
			}
			nTimesCmd1++;
		} while( nTimesCmd1 < 3 );

		if( !bSucceed )
		{
			nTimes++;
			str.Format( "CCG::UNI: strTemp = %s, strAck = %s, Times = %d, nTimesCmd1 = %d",
				strTemp, strAck, nTimes, nTimesCmd1 );
			RecordVacuum(str);
			OutputDebugString(str);
			continue;	// break;
		}

		str.Format( "\005" );
		dataSend[0] = str[0];
		strTemp = "";
		bSucceed = FALSE;
		// �ٷ���ENQ�����õ���Ҫ������
		do{
			m_CCG.SendData(dataSend, 1);
			Sleep(100);
			// Measurement unit: e.g. 1<CR><LF>
			nLenRecv = m_CCG.ReadData( dataRecv, 3 );
			for( i=0; i<nLenRecv; i++ )
			{
				str.Format( "%c", dataRecv[i] );
				strTemp += str;
			}
			if( strTemp.GetLength() == 3 )
			{
				nUnit = atoi( strTemp.Left(1) );
				if( (nUnit > -1) && (nUnit < 4) )
				{
					bSucceed = TRUE;
					break;
				}
			}
			nTimesCmd2++;
		} while( !bSucceed && (nTimesCmd2 < 3) );
		nTimes++;
	} while( !bSucceed && (nTimes < 3) );

	if( !bSucceed )	// nTimes == 3 || !(g_cfgCCG.bReady) )
	{
		m_cfgCCG.bReady = FALSE;
		str.Format( "CCG::UNI: strTemp = %s, nUnit = %d, Times = %d, nTimesCmd2 = %d",
			strTemp, nUnit, nTimes, nTimesCmd2 );
		RecordVacuum(str);
		OutputDebugString(str);
	}
	else
	{
		m_cfgCCG.bReady = TRUE;
		switch( nUnit )
		{
		case	0:
			str.Format( "Unit = mbar" );
			break;
		case	1:
			str.Format( "Unit = Torr" );
			break;
		case	2:
			str.Format( "Unit = Pascal" );
			break;
		case	3:
			str.Format( "Unit = Micron" );
			break;
		}
		RecordVacuum( str );
	}
	RecordVacuum("End of Query_UNI\r\n");
	return bSucceed;
}

BOOL CCCGStatusDetector::_ReqSP1() 
{
	CString str, strOut, strTemp, strAck;
	char dataSend[255], dataRecv[255];
	int i, nLenSend, nLenRecv, nTimes = 0;
	strAck.Format( "\006\015\012" );	// <ACK><CR><LF>
	BOOL bSucceed;

	int nTimesCmd1	= 0;
	int nTimesCmd2	= 0;
	nTimes = 0;
	// ����ֵ������־�м�¼��ֵ
	CString strReq = "SP1";
	nLenSend = CCG_ConvertRequestMeasureData( strReq, dataSend );
	do{
		strTemp = "";
		bSucceed = FALSE;
		do{
			// �ȷ���������õ�ACK����
			m_CCG.SendDataEx2(dataSend, nLenSend);
			Sleep(100);
			nLenRecv = m_CCG.ReadData(dataRecv, 3);
			for( i=0; i<nLenRecv; i++ )
			{
				str.Format( "%c", dataRecv[i] );
				strTemp += str;
			}
			if( strTemp.GetLength() == 3 )
			{
				if( strTemp == strAck )
				{
					bSucceed = TRUE;
					break;
				}
			}
			nTimesCmd1++;
		} while( nTimesCmd1 < 3 );

		if( !bSucceed )
		{
			nTimes++;
			str.Format( "SP1: strTemp = %s, strAck = %s, Times = %d, nTimesCmd1 = %d",
				strTemp, strAck, nTimes, nTimesCmd1 );
			RecordVacuum(str);
			continue;	// break;
		}

		m_strCCG = "";
		m_strCCGSaved = "";
		str.Format( "\005" );
		dataSend[0] = str[0];
		strTemp = "";
		bSucceed = FALSE;
		// �ٷ���ENQ�����õ���Ҫ������
		do{
			m_CCG.SendData(dataSend, 1);
			Sleep(100);
			// Threshold value setting: x.xxxxEsxx,x.xxxxEsxx<CR><LF>
			nLenRecv = m_CCG.ReadData( dataRecv, 24 );
			strTemp = "";
			for( i=0; i<nLenRecv; i++ )
			{
				str.Format( "%c", dataRecv[i] );
				strTemp += str;
			}
			m_strCCG = m_strCCGSaved + strTemp;
			if( m_strCCG.GetLength() > 22 )
			{
				// ��������
				str = m_strCCG;
				strOut = "";
				CString strRepSignal;
				strRepSignal.Format( "\015\012" );
				int nPos = str.Find( strRepSignal );
				int nLength = str.GetLength();
				if( nPos < nLength -2 && nPos > 0 )
				{
					// �س����ַ����м�
					m_strCCGSaved = str.Right( nLength -nPos -2 );
					str = str.Left( nPos );
				}
				else
					m_strCCGSaved = "";
				if( str.GetLength() > 22 )
				{
					nPos = str.Find( ',', 3 );
					CString strLower	= str.Left( nPos );
					CString strUpper	= str.Right( str.GetLength() - nPos -1 );
					nPos = str.Find( ',', 0 );
					if( nPos > 0 )
						strLower = str.Right( str.GetLength() - nPos -1 );
					nPos = strLower.Find( 'E', 0 );
					strOut.Format( "%.2f", atof( strLower.Left(nPos) ) );
					strOut += strLower.Mid( nPos, 4 );
					m_cfgCCG.dThreshold2 = atof(strOut);
					nPos = strUpper.Find( 'E', 0 );
					strOut.Format( "%.2f", atof( strUpper.Left(nPos) ) );
					strOut += strUpper.Mid( nPos, 4 );
					m_cfgCCG.dThreshold1 = atof(strOut);

					bSucceed = TRUE;
					break;
				}
			}
			else 
				m_strCCGSaved = m_strCCG;
			nTimesCmd2++;
		} while( !bSucceed && (nTimesCmd2 < 3) );
		nTimes++;
	} while( !bSucceed && (nTimes < 3) );

	if( !bSucceed )	// nTimes == 3 || !(g_cfgCCG.bReady) )
	{
		m_cfgCCG.bReady = FALSE;
		str.Format( "SP1: strTemp = %s, Times = %d, nTimesCmd2 = %d",
			strTemp, nTimes, nTimesCmd2 );
		RecordVacuum(str);
	}
	else
	{
		m_cfgCCG.bReady = TRUE;
		// ��¼��ֵ����־
		str.Format( "Threshold Upper = %.2e, Threshold Lower = %.2e", m_cfgCCG.dThreshold1, m_cfgCCG.dThreshold2 );
		RecordVacuum( str );
		PostMessage( m_hWnd, WM_USER_VAC_UpdateStatus, 3+5, (LPARAM)( &m_cfgCCG ) );
	}
	RecordVacuum("End of Query_SP1\r\n");
	return bSucceed;
}

BOOL CCCGStatusDetector::_ReqPR1() 
{
	CString str, strOut, strTemp, strAck;
	char dataSend[255], dataRecv[255];
	int i, nLenSend, nLenRecv, nTimes = 0;
	strAck.Format( "\006\015\012" );	// <ACK><CR><LF>
	BOOL bSucceed;

	RecordVacuum("Begin of Query_PR1");
	// ׼����ȡ���ֵ
	/////////////////////////////////////////////////////////
	// Request for measurement data: PR1<CR>[<LF>]
	CString strReq = "PR1";
	nLenSend= CCG_ConvertRequestMeasureData( strReq, dataSend );
	int nTimesCmd1	= 0;
	do
	{
		strTemp = "";
		bSucceed = FALSE;
		m_CCG.SendDataEx2(dataSend, nLenSend);	// ��������
		Sleep(100);
		// Positive acknowledgement: <ACK><CR><LF>
		do{
			nLenRecv = m_CCG.ReadData( dataRecv, 3);
			///////////////////////////////////////////////
			str.Format( "The %d times to ReadData [%d].", nTimesCmd1, nLenRecv );
			RecordVacuum(str);
			strOut = "";
			for( i=0; i<nLenRecv; i++ )
			{
				str.Format( "%02X,", dataRecv[i] );
				strOut += str;
			}
			str.Format( "Recv data [%d]: 0x%s", strTemp.GetLength(), strOut );
			RecordVacuum(str);
			///////////////////////////////////////////////
			for( i=0; i<nLenRecv; i++ )
			{
				str.Format( "%c", dataRecv[i] );
				strTemp += str;
			}
//			if( strTemp.GetLength() == 3 )
//			{
//				if( strTemp == strAck )
			if( strTemp.GetLength() > 2 )
			{
				if( strTemp.Find(strAck) > -1 )
				{
					bSucceed = TRUE;
					break;
				}
			}
			nTimesCmd1++;
		} while( !bSucceed && (nTimesCmd1 < 3) );
		nTimes++;
	} while( !bSucceed && (nTimes < 3) );

	if( bSucceed )
	{
		m_cfgCCG.bReady = TRUE;
		RecordVacuum("PR1 ready!" );
	}
	else
	{
		m_cfgCCG.bReady = FALSE;
		str.Format( "PR1 cmd recv times = %d [%d], strTemp = %s", nTimesCmd1, nTimes, strTemp );
		RecordVacuum( str );
	}
	RecordVacuum("End of Query_PR1");
	return bSucceed;
}