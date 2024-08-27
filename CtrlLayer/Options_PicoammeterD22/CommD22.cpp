// CommD22.cpp : implementation file
//

#include "stdafx.h"
#include "CommD22.h"

#include <math.h>
#include <Serial.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �������ò���
#define	nD22_Sleep		100

HWND	g_hMain;
CSerial	g_D22;
BOOL	g_bD22Open		= FALSE;
BOOL	g_bPauseComm	= FALSE;

/////////////////////////////////////////////////////////////////////////////
// CCommD22

CCommD22::CCommD22()
{
	g_hMain = NULL;
}

CCommD22::~CCommD22()
{
}

void CCommD22::InitHWND( HWND hWnd )
{
	g_hMain = hWnd;
}

/////////////////////////////////////////////////////////////////////////////
// CCommD22 message handlers

//��д����ʵ��
//IEEE754 ʮ������תʮ����CbuffR
float ConvertReadData(BYTE* lbuff)
{
	float flocation;
	unsigned char highByte, lowByte;
	char Beam[8]={0};

	//16����ת�ַ���
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

//CRCУ�����
void CRCheck(BYTE* DataCode)
{
	//����CRCУ��λ
	unsigned short tmp = 0xffff;
	unsigned short ret1 = 0;
	for(int n = 0; n < 6; n++)
	{
		//�˴���6 -- ҪУ���λ��Ϊ6��
		tmp = DataCode[n] ^ tmp;
		for(int i=0; i<8; i++ )
		{  
			//�˴���8 -- ָÿһ��char������8bit��ÿbit��Ҫ����
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

	//��CRCУ��ĸߵ�λ�Ի�λ��
	ret1 = tmp >> 8;
	ret1 = ret1 | (tmp << 8); 
	DataCode[6]= tmp;	// ��8λ
	DataCode[7]= ret1;	// ��8λ
}

float CCommD22::GetCurrentBeam()
{
	BYTE CbuffR[8] = {0},buff[4]={0};
	BYTE ORecvdata[255]= {0};
	CbuffR[0] = 0xff;
	CbuffR[1] = 0x03;
	CbuffR[2] = 0x00;
	CbuffR[3] = 0x01;
	CbuffR[4] = 0x00;
	CbuffR[5] = 0x01;
	//����CRCУ��λ
	CRCheck( CbuffR );
	int nlensr1 = sizeof(CbuffR);
	int nLenSend= g_D22.SendDataEx(CbuffR, nlensr1);
	Sleep(nD22_Sleep);
	int nlenRecvRH = g_D22.ReadData((BYTE*)ORecvdata, 255);
	buff[0] = ORecvdata[6]; buff[1] = ORecvdata[5];
	buff[2] = ORecvdata[4]; buff[3] = ORecvdata[3];
	/*int nBeamVal = (int)(ConvertReadData(buff)+0.5);*/
	float fBeamVal = ConvertReadData(buff);
	return fBeamVal;
}

//�汾��ѯ
BOOL CCommD22::GetVersion()
{
	BYTE Version[8] = {0},buff[4] = {0};
	BYTE VRecvdata[200]= {0};
	Version[0] = 0Xff;
	Version[1] = 0x02;
	Version[2] = 0x00;
	Version[3] = 0x03;
	Version[4] = 0x00;
	Version[5] = 0x01;
	//����CRCУ��λ
	CRCheck( Version );
	int nlensr = sizeof(Version);
	int nLenSend= g_D22.SendDataEx(Version, nlensr);
	Sleep(nD22_Sleep);
	BOOL bRet = g_D22.ReadData((BYTE*)VRecvdata, 255);
	return bRet;
}

BOOL CCommD22::Open(int nPort)
{
	//�򿪶˿�
	g_bD22Open = g_D22.Open( nPort,CBR_9600 );
	if(!g_bD22Open)
		g_bD22Open = g_D22.Open( nPort,CBR_9600 );
	Sleep(nD22_Sleep);
	if(g_bD22Open)
	{
		if( !GetVersion() )
			g_bD22Open = FALSE;
	}
	PostMessage( g_hMain, WM_USER_D22_UpdateValue, 0, g_bD22Open );	
	return TRUE;
}

