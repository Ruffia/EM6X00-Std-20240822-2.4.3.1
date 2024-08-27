// CommVince.cpp : implementation file
//

#include "stdafx.h"
#include "CommVince.h"

#include "Serial.h"
#include <afxmt.h>
#include <Comm_Vince.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCriticalSection		m_mutexVince;

#define nVince_Sleep		50
#define	nRet_OK				100
#define nRet_Ready			101
#define nRet_E01			102
#define nRet_E02			103

DWORD WINAPI CommThreadVince(LPVOID lpParam);

HANDLE	g_hThreadVince;
DWORD	g_dwThreadVince;
HANDLE	g_hEventWorkVince		= CreateEvent(NULL,FALSE, FALSE, _T("WORKVince"));
HANDLE	g_hEventTeminateVince	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEVince"));

HWND	g_hMain;
CSerial	g_Vince;
BYTE	g_dataVinceSaved[1024];
int		g_nSavedCounts = 0;

HANDLE	g_hFileVince = NULL;
HANDLE	g_hFileTest	= NULL;
int		g_nLoopsVince, g_nLoopsTestVince;
DWORD	g_lastMonVince	= 0;

BOOL	g_bVinceOpen		= FALSE;	// �Ƿ�������һ���豸��ͨ

float	g_fSpeed = 0.0;
//float	g_fPos = 0.000;
//int		g_nPos = 0;
Vince_Param_Status g_Status;
Vince_Params_Sys g_ParamsSys;
Vince_Params g_Params;
/////////////////////////////////////////////////////////////////////////////
// CCommVince

CCommVince::CCommVince()
{
	g_hMain	= NULL;
	ZeroMemory(&g_Status, sizeof(g_Status));
	ZeroMemory(&g_ParamsSys, sizeof(g_ParamsSys));
	ZeroMemory(&g_Params, sizeof(g_Params));
}

CCommVince::~CCommVince()
{
}

int Vince_WriteToDevice( int nSleep, char* data )
{
	if( !g_bVinceOpen )
		return -1;
	int nLength = strlen(data);
	int nLenSend = g_Vince.SendData(data, nLength);
	Sleep(nSleep);
	return nLenSend;
}

int Vince_ReadFromDevice( int nSleep, BYTE* data )
{
	if( !g_bVinceOpen )
		return -1;
	
	if( g_nSavedCounts > 0 )	// ƴ����
		memcpy( data, g_dataVinceSaved, g_nSavedCounts );

	int nLenRecv = g_Vince.ReadData( data +g_nSavedCounts, 200 );
	nLenRecv += g_nSavedCounts;
	return nLenRecv;
}


/////////////////////////////////////////////////////////////////////
char* Float2Hex(const float f) 
{
	unsigned char *hex = (unsigned char *)&f;
	char* sHex = new char[9];
	memset(sHex,0,9);
	sprintf(sHex,"%02X%02X%02X%02X",hex[0],hex[1],hex[2],hex[3]);
	return sHex;
}

DWORD Float2DWORD(const float f) 
{
	unsigned char *hex = (unsigned char *)&f;
	char* sHex = new char[2*8 + 1];
	memset(sHex,0,17);
	sprintf(sHex,"%02X%02X%02X%02X",hex[3],hex[2],hex[1],hex[0]);	// ��λ��ǰ
	DWORD dValue = (DWORD)strtoul(sHex, NULL, 16);
	delete sHex;
	sHex = NULL;
	return dValue;
}
/*
��ʵ����������˱�����ת�͵��ֶΣ�
����float��DWORD����4���ֽڳ��ȣ�
ͨ�������ֶ�ת�ͣ����ᶪʧ�����Ƶ����ݣ�����Ҳ�Ͳ��ᶪʧԭʼ���ݡ�

*((DWORD*)&f1)�����Ȼ�ȡһ��floatָ�룬
Ȼ����ǿ��ת��ΪDWORD���͵�ָ�룬�������ã���ȡ����һ��DWORD���͵�ֵ��
*((float*)&dw)����ͬ��  


�����bai��Ľ����Ҫdu������һ��˼��zhi��
����Ҫת������ֵ�ĵ�ַȡ����
��ֵ��һ������Ҫת���ɵġ��������͵ĵ�ַ��Ҳ����ָ�룩��
Ȼ��ȡ��Ҫ���ڴ浥Ԫ�е����ݾͿ�����

�����һ������ֵ��ʮ�����Ƶı���n����Ҫת����float�͵ģ�
������ȡ���ַ&n������һ��float�͵�ָ�룬Ҳ����(float*)&n��
Ȼ��ȡָ�������*(float)&n�Ϳ����ˡ�

��Ҫע����ǣ��������ֻ������
����ת�����������ڴ�����ռ��С
����Ҫת���ɵ������������ڴ�����ռ��С��ͬ�����
(��VC6�У�float������ռ4���ֽڣ�
��ÿ���ֽ�����λʮ����������4���ֽ�Ҳ���ǰ�λʮ����������
���õ�������Ҫת����ʮ����������λ�����ʴ˷�������)
ϣ���ҵĻش��ܰﵽ��~��
���¸��ϲο������:

#include <stdio.h>
float Hex_To_Decimal(unsigned char *Byte,int num)//ʮ�����Ƶ�������
{
	// char cByte[4];//����һ
	// for (int i=0;i<num;i++)
	// {
	// cByte[i] = Byte[i];
	// }
	//
	// float pfValue=*(float*)&cByte;
	//return pfValue;
	return *((float*)Byte);//������
}

long FloatTohex(float HEX)//��������ʮ������ת��1
{
	return *( long *)&HEX;
}

void FloatToByte(float floatNum,unsigned char* byteArry)////��������ʮ������ת��2
{
	char* pchar=(char*)&floatNum;
	for(int i=0;i<sizeof(float);i++)
	{
		*byteArry=*pchar;
		pchar++;
		byteArry++;
	}
}

void main()
{
	unsigned char floatToHex[4];
	unsigned char hexbyte[4]={0xd0,0x0f,0x49,0x40};//��������Ϊ3d cc cc cd,0xcd,0xCC,0xCC,0x3D,40490fd0
	float Hdecimal=0.0;
	float flh=0.4;
	Hdecimal=Hex_To_Decimal(hexbyte,sizeof(hexbyte));//ʮ������ת��Ϊ������
	printf("\n ������Ϊ��\n %f\n",Hdecimal);
	long hX=FloatTohex(Hdecimal);//������ת��Ϊʮ������һ
	printf("\n����ʮ������ֵ��\n %f=%X \n",Hdecimal,hX);//������ʾ
	FloatToByte(Hdecimal,floatToHex);//������תΪʮ�����ƶ�
	printf("\n����ʮ�����ƣ�\n%f=%x %x %x %x\n",Hdecimal,floatToHex[0],floatToHex[1],floatToHex[2],floatToHex[3] );//������ʾ
}
*/
/////////////////////////////////////////////////////////////////////


const BYTE	DataHead	= 0xFF;
const BYTE	DataEnd		= 0xFE;
const BYTE	DataLength	= 21;	// head +CID +type +5bytes +5bytes +5bytes +BH +BL +end

/////////////////////////////////////////////////////////////////////
// BCCУ�飺BCC�����У�鷨
//			У�����ý��յ���У�����ÿһ����У��������ٴν���������
//			����õ��Ľ����0����У��ɹ�������У��ʧ��
//
// У���뱾��Ҳ��Ҫ����0~127��Χ�Ĺ���
// ������Ҫ��1�ֽڵ�У����任��2�ֽڵ�У������֮���ڽ��д���
//
// ������Ϣͷ����Ϣβ֮��������������ֽڶ�����У��
/////////////////////////////////////////////////////////////////////

BYTE bcc_checksum( BYTE* data, BYTE seed, int size )
{
	BYTE sum = seed;
	for( int i=0; i<size; i++ )
		sum ^= data[i];
	return sum;
}

BYTE Parse8( BYTE* data )
{
	// �����豸�õ���2�ֽ����ݲ�ֺϲ���1�ֽ�8λ����
	// �� byte2 : xxxxxxx? x???????
	// ��ΪBYTE1:          ????????
	BYTE bData = ((data[0] & 0x01) << 7) | (data[1] & 0x7F);
	return bData;
}

DWORD Parse32( BYTE* data )
{
	// �����豸�õ���5�ֽ����ݲ�ֺϲ���4�ֽ�32λ����
	// �� byte5 : xxxx???? x??????? x??????? x??????? x???????
	// ��ΪBYTE4:          ???????? ???????? ???????? ????????
	BYTE bData[4];
	bData[0] = ((data[0] & 0x0F) << 4) | ((data[1] & 0x78) >> 3);
	bData[1] = ((data[1] & 0x07) << 5) | ((data[2] & 0x7C) >> 2);
	bData[2] = ((data[2] & 0x03) << 6) | ((data[3] & 0x7E) >> 1);
	bData[3] = ((data[3] & 0x01) << 7) | (data[4] & 0x7F);
	DWORD dwData = (bData[0] << 24) | (bData[1] << 16) | (bData[2] << 8) | bData[3];
	return dwData;
}

int Vince_ConvertReceiveData( BYTE* data, int nLenRecv )
{
	if( nLenRecv < DataLength )	
	{
		// ���ݲ���һ���ֽ������������ݣ�����һ��һ�����
		return -1;	// ����һ֡����
	}

//	CString strTemp, str = "";
	int i, nIndex = 0;
/*
	str = "";
	for( i=nIndex; i<nLenRecv; i++ )
	{
		strTemp.Format( "%02X ", data[i] );
		str += strTemp;
		if( i % 100 == 0 )
		{
			TRACE(str);
			str = "";
		}
	}
	str += "\r\n";
	TRACE(str);
*/
	while( nLenRecv > DataLength -1 )
	{
		BYTE bHead = data[nIndex];
		if( bHead != DataHead )
		{
			// �ҵ���Ϣͷ
			nLenRecv--;
			nIndex++;
			continue;
		}
		if( nLenRecv > nIndex +DataLength -1)
		{
			BYTE bCID = data[nIndex +1];
			BYTE bType = data[nIndex +2];
			if( bType != 0x02 )	// 0x01 ��0x03
			{
				// ���յ����ַ�����
				int i, nLen;
				for( i=nIndex +3; i<nLenRecv; i++ )
				{
					if( data[i] != 0xFE )
						continue;
					else
						break;
				}
				nLen = i -1;
				if( data[nLen+1] != 0xFE )
					break;	// ����βҲû���ҵ���Ϣβ
// 				str = "";
// 				for( i=nIndex +3; i<nLen; i++ )
// 				{
// 					strTemp.Format("%c", data[i]);
// 					str += strTemp;
// 					if( i % 100 == 0 )
// 					{
// 						TRACE(str);
// 						str = "";
// 					}
// 				}
// 				str += "\r\n";
// 				TRACE(str);
				nLenRecv -= (nLen +1);
				nIndex += (nLen +1);
			}
			else	// 0x02
			{
				// ���յĶ����ֽ���
				// У����
				int nIndexCRC = 2 +5 +5 +5 +1;
				BYTE bChkC = bcc_checksum( data +nIndex +1, data[nIndex +1 +nIndexCRC], nIndexCRC );
				if( bChkC != 0x00 )
				{
					nLenRecv -= DataLength;
					nIndex += DataLength;
					continue;
				}
// 				BYTE bData[2 +4 +4 +4];
// 				bData[0] = bCID;
// 				bData[1] = bType;
				DWORD dwSpeed = Parse32( data +nIndex +3 );//, bData +2 );
				DWORD dwPos = Parse32( data +nIndex +3 +5 );//, bData +2 +4 );
				DWORD dwStatus = Parse32( data +nIndex +3 +5 +5 );//, bData +2 +4 +4 );

				// �ٶ���32λ������
				g_fSpeed = *((float*)&dwSpeed);
				g_Params.speed = dwSpeed;
				// λ��Ϊ32λ����������
				if( (dwPos & 0x80000000) != 0 )	// ���λΪ1���Ǹ���
					g_Params.pos = -(int)(0xFFFFFFFF -dwPos +1);
				else
					g_Params.pos = (int)(dwPos & 0x7FFFFFFF);
				// ״̬Ϊ32λ�޷�������
				g_Status.dwStatus = dwStatus;
// 				str.Format( "Speed = 0x%08X (%.2f), %d, 0x%08X\n", dwSpeed, g_fSpeed, g_Params.pos, dwStatus );
// 				TRACE(str);
				nLenRecv -= DataLength;
				nIndex += DataLength;
			}
		}
		else
			break;
	}

	// ���ݲ���һ���ֽ������������ݣ�����һ��һ�����
	g_nSavedCounts = nLenRecv;
	if( nLenRecv > 0 )
	{
		for( i=nIndex; i<nIndex+nLenRecv; i++ )
		{
			g_dataVinceSaved[i -nIndex] = data[i];
		}
	}

	return 0;
}

int Vince_SendCmd( int nSleep, int nCID, char* cmd )
{
	if( !g_bVinceOpen )
		return -1;

	int nRet = -1;
	m_mutexVince.Lock();
	char dataSend[255];
	sprintf( dataSend, "%d %s\n", nCID, cmd );
	Vince_WriteToDevice( nSleep, dataSend );

	BYTE dataRecv[255];
	int nLength = Vince_ReadFromDevice( nSleep, dataRecv );
	if( nLength > 0 )
	{
		nRet = Vince_ConvertReceiveData( dataRecv, nLength );
// 		if( nRet > 110 && nRet < 115 )	// ��������λ����Ϣ��˵���˶���ֹͣ
// 			Vince_AfterStop( nRet );
	}
	m_mutexVince.Unlock();
	return nRet;
}

void InitVinceParams( int nSleep )
{
	Vince_SendCmd( nVince_Sleep, 1, "ena" );
}

void SyncVinceParams( int nSleep )
{
	// ��ȡ������
}

void LoadKYKYdefault( int nSleep )
{
	if( !g_bVinceOpen )
		return;
}

int CommWithVince( int nSleep )
{
	if( !g_bVinceOpen )
		return -1;

	int nRet = 0;
//	m_mutexVince.Lock();
	nRet = Vince_SendCmd( nVince_Sleep, 1, "sts" );
/*	BYTE dataRecv[100];
	int nLength = Vince_ReadFromDevice( nSleep, dataRecv );
	if( nLength > 0 )
	{
		nRet = Vince_ConvertReceiveData( dataRecv, nLength );
// 		if( nRet > 110 && nRet < 115)	// ��������λ����Ϣ��˵���˶���ֹͣ
// 			Vince_AfterStop( nRet );
	}
	m_mutexVince.Unlock();
*/	return nRet;
}

DWORD WINAPI CommThreadVince(LPVOID lpParam)
{
	// �򿪶˿�
	g_bVinceOpen	= g_Vince.Open( (short)lpParam, 9600 );

	if( g_bVinceOpen )
	{
		int nRet = Vince_SendCmd( 1000, 1, "dev" );
		Sleep( 1000 );
		if( nRet != 0 )
			g_bVinceOpen = FALSE;
		else
		{
			/////////////////////////////////////////////
/*			// ������־
			TCHAR exeFullPath[255];
			::GetModuleFileName( NULL, exeFullPath, 255 );
			char *ptemp = strrchr( exeFullPath, '\\');
			ptemp[1] = 0;

			CString str, strTemp;
			CTime	timeCurrent = CTime::GetCurrentTime();
			strTemp = timeCurrent.Format("%Y%m%d%H%M");

			// ����Box��־�ļ�
			str.Format( "%sLog\\Box%s.csv", exeFullPath, strTemp );
			if( g_hFileBox != NULL )
				CloseHandle( g_hFileBox );
			g_hFileBox	= NULL;
			g_hFileBox = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			g_nLoopsBox = 1;
			// дBox��־�ļ�ͷ
			if( g_hFileBox != NULL )
			{
				unsigned long cbRet;
				char Buf[255];
				str.Format( "\t%s\r\n", "Box" );
				sprintf( Buf, "%s\r\n", str );
				WriteFile( g_hFileBox, Buf, strlen(Buf), &cbRet, NULL );
			}
*/		}
		InitVinceParams(nVince_Sleep);
		SyncVinceParams(nVince_Sleep);
	}
	PostMessage( g_hMain, WM_USER_Vince_UpdateValue, 0, g_bVinceOpen );
	if( g_bVinceOpen )
	{
// 		PostMessage( g_hMain, WM_USER_Vince_UpdateValue, 1, (LPARAM)(&g_ParamSys) );
// 		PostMessage( g_hMain, WM_USER_Vince_UpdateValue, 2, (LPARAM)(&g_ParamSpeed) );
// 		PostMessage( g_hMain, WM_USER_Vince_UpdateValue, 3, (LPARAM)(&g_ParamPos) );
	}

	while( g_bVinceOpen && WaitForSingleObject(g_hEventTeminateVince,500) != WAIT_OBJECT_0 )
	{
//		if( !g_bPauseComm )
		{
			CommWithVince(nVince_Sleep);
		}
		Sleep( 1 );
	}

	g_Vince.Close();
	g_bVinceOpen	= g_Vince.IsOpened();
	if( g_hFileVince != NULL )
		CloseHandle( g_hFileVince );
	g_hFileVince	= NULL;

	SetEvent( g_hEventWorkVince );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCommVince message handlers
void CCommVince::InitHWND( HWND hWnd )
{
	g_hMain = hWnd;
}

BOOL CCommVince::Open( short port )
{
	/////////////////////////////////////////////////////////////////
	ResetEvent( g_hEventWorkVince );
	ResetEvent( g_hEventTeminateVince );

	g_hThreadVince	= CreateThread( NULL, 0, CommThreadVince, (LPVOID)(port), 0, &g_dwThreadVince );
	if( g_hThreadVince == NULL )
		return FALSE;

	CloseHandle(g_hThreadVince);
	return TRUE;
}

BOOL CCommVince::Close()
{
	if( g_bVinceOpen )
	{
		SetEvent( g_hEventTeminateVince );
		WaitForSingleObject( g_hEventWorkVince, INFINITE );
		g_bVinceOpen	= FALSE;
	}
	return TRUE;
}

BOOL CCommVince::GetConnectState()
{
	return g_bVinceOpen;
}

void CCommVince::SetCfgParams( short nIndex, short nCID, float fValue )
{
	char cmd[50];
	switch( nIndex )
	{
	case	Cfg_brd:
		sprintf( cmd, "cfg bdr=%d", (int)fValue );
		break;
	case	Cfg_cid:
		sprintf( cmd, "cfg cid=%d", (int)fValue );
		break;
	case	Cfg_mcs:
		sprintf( cmd, "cfg mcs=%d", (int)fValue );
		break;
	case	Cfg_spd:
		sprintf( cmd, "cfg spd=%d", (int)fValue );
		break;
	case	Cfg_acc:
		sprintf( cmd, "cfg acc=%ld", (long)fValue );
		break;
	case	Cfg_dec:
		sprintf( cmd, "cfg dec=%d", (long)fValue );
		break;
	case	Cfg_cra:
		sprintf( cmd, "cfg cra=%.1f", fValue );
		break;
	case	Cfg_crn:
		sprintf( cmd, "cfg crn=%.1f", fValue );
		break;
	case	Cfg_crh:
		sprintf( cmd, "cfg crh=%.1f", fValue );
		break;
	case	Cfg_s1f:
		sprintf( cmd, "cfg s1f=%d", (int)fValue );
		break;
	case	Cfg_s1r:
		sprintf( cmd, "cfg s1r=%d", (int)fValue );
		break;
	case	Cfg_s2f:
		sprintf( cmd, "cfg s2f=%d", (int)fValue );
		break;
	case	Cfg_s2r:
		sprintf( cmd, "cfg s2r=%d", (int)fValue );
		break;
	case	Cfg_s3f:
		sprintf( cmd, "cfg s3f=%d", (int)fValue );
		break;
	case	Cfg_s3r:
		sprintf( cmd, "cfg s3r=%d", (int)fValue );
		break;
	case	Cfg_s4f:
		sprintf( cmd, "cfg s4f=%d", (int)fValue );
		break;
	case	Cfg_s4r:
		sprintf( cmd, "cfg s4r=%d", (int)fValue );
		break;
	case	Cfg_s5f:
		sprintf( cmd, "cfg s5f=%d", (int)fValue );
		break;
	case	Cfg_s5r:
		sprintf( cmd, "cfg s5r=%d", (int)fValue );
		break;
	case	Cfg_s6f:
		sprintf( cmd, "cfg s6f=%d", (int)fValue );
		break;
	case	Cfg_s6r:
		sprintf( cmd, "cfg s6r=%d", (int)fValue );
		break;
	case	Cfg_s3:
		sprintf( cmd, "cfg s3=%d", (int)fValue );
		break;
	case	Cfg_s4:
		sprintf( cmd, "cfg s4=%d", (int)fValue );
		break;
	case	Cfg_s5:
		sprintf( cmd, "cfg s5=%d", (int)fValue );
		break;
	case	Cfg_s6:
		sprintf( cmd, "cfg s6=%d", (int)fValue );
		break;
	case	Cfg_zmd:
		sprintf( cmd, "cfg zmd=%d", (int)fValue );
		break;
	case	Cfg_snr:
		sprintf( cmd, "cfg snr=%d", (int)fValue );
		break;
	case	Cfg_osv:
		sprintf( cmd, "cfg osv=%d", (int)fValue );
		break;
	case	Cfg_zsd:
		sprintf( cmd, "cfg zsd=%d", (int)fValue );
		break;
	case	Cfg_zsp:
		sprintf( cmd, "cfg zsp=%ld", (long)fValue );
		break;
	case	Cfg_sds:
		sprintf( cmd, "cfg sds=%d", (int)fValue );
		break;
	case	Cfg_zcr:
		sprintf( cmd, "cfg zcr=%.1f", fValue );
		break;
	case	Cfg_dmd:
		sprintf( cmd, "cfg dmd=%d", (int)fValue );
		break;
	case	Cfg_dar:
		sprintf( cmd, "cfg dar=%d", (int)fValue );
		break;
	case	Cfg_msr:
		sprintf( cmd, "cfg msr=%d", (int)fValue );
		break;
	case	Cfg_msv:
		sprintf( cmd, "cfg msv=%d", (int)fValue );
		break;
	case	Cfg_psr:
		sprintf( cmd, "cfg psr=%d", (int)fValue );
		break;
	case	Cfg_psv:
		sprintf( cmd, "cfg psv=%d", (int)fValue );
		break;
	case	Cfg_pae:
		sprintf( cmd, "cfg pae=%d", (int)fValue );
		break;
	case	Cfg_zar:
		sprintf( cmd, "cfg zar=%d", (int)fValue );
		break;
	}
	Vince_SendCmd(nVince_Sleep, nCID, cmd);
}

void CCommVince::Move( short nIndex, short nCID, long lValue )
{
	char cmd[50];
	switch( nIndex )
	{
	case	0:
		Vince_SendCmd( nVince_Sleep, nCID, "mov" );
		break;
	case	1:
		sprintf( cmd, "pos %ld", lValue );
		Vince_SendCmd( nVince_Sleep, nCID, cmd );
		break;
	case	2:
		sprintf( cmd, "rmv %ld", lValue );
		Vince_SendCmd( nVince_Sleep, nCID, cmd );
		break;
	case	3:
		sprintf( cmd, "pps %ld", lValue );
		Vince_SendCmd( nVince_Sleep, nCID, cmd );
		break;
	}
}

void CCommVince::SetOrg( short nCID )
{
	Vince_SendCmd( nVince_Sleep, nCID, "org" );
}

void CCommVince::MotorEn( short nCID, BOOL bEn )
{
	if( bEn )
		Vince_SendCmd( nVince_Sleep, nCID, "ena" );
	else
		Vince_SendCmd( nVince_Sleep, nCID, "off" );
}

void CCommVince::MotorStop( short nCID, short nValue )
{
	char cmd[50];
	sprintf( cmd, "stp %d", nValue );
	Vince_SendCmd( nVince_Sleep, nCID, cmd );
}

void CCommVince::BackZero( short nCID, BOOL bStart )
{
	if( bStart )
		Vince_SendCmd( nVince_Sleep, nCID, "zero start" );
	else
		Vince_SendCmd( nVince_Sleep, nCID, "zero stop" );
}

int CCommVince::GetPosition( short nCID )
{
	int nValue = 0;
	switch( nCID )
	{
	case	1:
		nValue = g_Params.pos;
		break;
	}
	return nValue;
}

float CCommVince::GetSpeed( short nIndex, short nCID )
{
	float fValue = 0.0;
	switch( nIndex )
	{
	case	0:	// speed
		break;
	case	1:	// acc
		break;
	case	2:	// dec
		break;
	}
	return fValue;
}

Vince_Param_Status CCommVince::GetStatus( short nCID )
{
	Vince_Param_Status sp = g_Status;
	return sp;
}

void CCommVince::LoadDefault( short nCID )
{
	// �ָ�Ĭ������
	Vince_SendCmd(nVince_Sleep, nCID, "bdr=9600");
	Vince_SendCmd(nVince_Sleep, nCID, "mcs=5");
	Vince_SendCmd(nVince_Sleep, nCID, "spd=6400");
	Vince_SendCmd(nVince_Sleep, nCID, "acc=120000");
	Vince_SendCmd(nVince_Sleep, nCID, "dec=120000");
	Vince_SendCmd(nVince_Sleep, nCID, "cra=1.0");
	Vince_SendCmd(nVince_Sleep, nCID, "crn=1.0");
	Vince_SendCmd(nVince_Sleep, nCID, "crh=0.4");

	Vince_SendCmd(nVince_Sleep, nCID, "s1f=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s1r=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s2f=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s2r=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s3f=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s3r=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s4f=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s4r=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s5f=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s5r=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s6f=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s6r=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s3=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s4=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s5=0");
	Vince_SendCmd(nVince_Sleep, nCID, "s6=0");

	Vince_SendCmd(nVince_Sleep, nCID, "zmd=2");
	Vince_SendCmd(nVince_Sleep, nCID, "osv=1");
	Vince_SendCmd(nVince_Sleep, nCID, "snr=5");
	Vince_SendCmd(nVince_Sleep, nCID, "zsd=-640");
	Vince_SendCmd(nVince_Sleep, nCID, "zsp=4800");
	Vince_SendCmd(nVince_Sleep, nCID, "zcr=0.6");
	Vince_SendCmd(nVince_Sleep, nCID, "sds=32");
	Vince_SendCmd(nVince_Sleep, nCID, "msr=6");
	Vince_SendCmd(nVince_Sleep, nCID, "msv=0");
	Vince_SendCmd(nVince_Sleep, nCID, "psr=5");
	Vince_SendCmd(nVince_Sleep, nCID, "psv=0");
	Vince_SendCmd(nVince_Sleep, nCID, "dmd=0");
	Vince_SendCmd(nVince_Sleep, nCID, "dar=0");

	Vince_SendCmd(nVince_Sleep, nCID, "cfg");
}