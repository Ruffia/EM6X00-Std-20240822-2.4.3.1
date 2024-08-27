// CommMC6600.cpp : implementation file
//

#include "stdafx.h"
#include "CommMC6600.h"

#include <afxmt.h>
#include <math.h>

#include <Serial.h>
#include <Comm_MC6600.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCriticalSection		m_mutexMC6600;

#define	nMC6600_Sleep			50
const UINT	nStageMC6600_Elapse	= 300;

DWORD WINAPI InitThreadMC6600(LPVOID lpParam);
//DWORD WINAPI StatuThreadMC6600(LPVOID lpParam);

HANDLE	g_hInitThreadMC6600;
DWORD	g_dwinitThreadMC6600;
HANDLE	g_hEventWorkMC6600		= CreateEvent(NULL,FALSE, FALSE, _T("WORKMC6600"));
HANDLE	g_hEventTeminateMC6600	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATEMC6600"));
//HANDLE   g_hStatuThreadMC6600;
//DWORD    g_dwstatuThreadMC6600;

HWND	g_hMain;
CSerial	g_MC6600;
BOOL	g_bMC6600Open		= FALSE;	// 是否至少有一个设备联通
BOOL    g_bMC6600OpenElec[5];
int     g_nAsynCmdFormat;
BOOL    g_bCurPosSetting	= FALSE;   //当前位置获取-设置
BYTE	g_bytebuff[4]	= {0};

MC6600_Params_Current		g_ParamCurrent;
MC6600_Params_Speed			g_ParamSpeed;
MC6600_Params_Bl			g_ParamBl;
MC6600_Params_AxialAddr		g_ParamAddr;
MC6600_Params_Style			g_ParamStyle;
MC6600_Params_PID			g_ParamCurPID;

BOOL	g_bBacklash[4];
//BYTE	g_bSlaceAddr[5];
int		g_nDirX, g_nDirY;
int		m_nTimerID =0; //定时器ID
void	SendtarPosW(BYTE* SRecvdata, short anxi);
void	Enable(short anxi);
void	PosSpeed(short anxi,const float* data);
//void	BackOff( unsigned char ucAxis );


/////////////////////////////////////////////////////////////////////////////
// CStageMC6600Manager
CStageMC6600Manager& CStageMC6600Manager::Instance()
{
	static CStageMC6600Manager Inst;
	return Inst;
}

CStageMC6600Manager::CStageMC6600Manager()
{
	m_vecCmdMC6600.clear();
	for( int i=0; i<5; i++ )
		m_bIsMoving[i] = TRUE;
}
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// CCommMC6600

CCommMC6600::CCommMC6600()
{
	for( int i=0; i<5; i++ )
	{
		g_ParamSpeed.fSpeed[i]		= 0.0;
		g_ParamSpeed.fSpeedInit[i]	= 0.0;
		g_ParamSpeed.fSpeedAcc[i]	= 0.0;
		g_ParamSpeed.fSpeedHome[i]	= 0.0;
		g_ParamCurrent.fSteps[i]	= 0.0;
		g_ParamCurrent.fOrigin[i]	= 0.0;
		g_ParamCurrent.nStatus[i]	= 0;
		g_bMC6600OpenElec[i]		= FALSE;
	}
}

CCommMC6600::~CCommMC6600()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCommMC6600 message handlers

//编写函数实现
union
{
	unsigned char data[4];	//uint8_t data[4];
	float value;
} fpx;

//地址值与i对应
short SerialNum(unsigned char ucAxis)
{
	short anxi = 6;
	switch (ucAxis)
	{
	case 'X':
		anxi = 0;
		break;
	case 'Y':
		anxi = 1;
		break;
    case 'Z':
		anxi = 2;
		break;
	case 'R':
		anxi = 3;
		break;
	case 'T':
		anxi = 4;
		break;
	}
	return anxi;
}

//i与轴向对应
unsigned char SerialNum(short anxi)
{
	unsigned char ucAxis = 'X';
	switch (anxi)
	{
	case 0:
		ucAxis = 'X';
		break;
	case 1:
		ucAxis = 'Y';
		break;
    case 2:
		ucAxis = 'Z';
		break;
	case 3:
		ucAxis = 'R';
		break;
	case 4:
		ucAxis = 'T';
		break;
	}
	return ucAxis;
}

//CRC校验计算
void CRCheck(BYTE* DataCode)
{
	//计算CRC校验位
	unsigned short tmp = 0xffff;
	unsigned short ret1 = 0;
	for(int n = 0; n < 6; n++)
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
	//CRC校验后的值
	//printf("%X\n",tmp);
	//将CRC校验的高低位对换位置
	ret1 = tmp >> 8;
	ret1 = ret1 | (tmp << 8); 
	DataCode[6]= tmp;	// 低8位
	DataCode[7]= ret1;	// 高8位
// 	DataCode[6]=0x01;
// 	DataCode[7]=0x01;
}

//十进制转十六进制
void dec2hex(BYTE* DataCode, int SpeedVal)
{
	int y = 0, Sval1, Sval2;
//	string s, target;
	CString s, target;
	while(SpeedVal > 0)
	{
		y = SpeedVal %16;
		if(y < 10)
			s = char('0' +y) +s;
		else
			s = char('A'-10 +y) +s;
		SpeedVal = SpeedVal /16;
	}
	int len = s.GetLength();	//.length();
	if(len == 4)
	{
		target = s.Mid(0,2);
		const char* str1 = target.GetBuffer(0);	//.GetBuffer(0);
		sscanf(str1, "%02x", &Sval1);
		DataCode[4] = Sval1;

		target = s.Mid(2,2);
		const char* str2 = target.GetBuffer(0);
		sscanf(str2, "%02x", &Sval2);
		DataCode[5] = Sval2;
	}
	else if(len == 3)
	{
		target = s.Mid(0,1);
		const char* str1 = target.GetBuffer(0);
		sscanf(str1, "%02x", &Sval1);
		DataCode[4] = Sval1;

		target = s.Mid(1,2);
		const char* str2 = target.GetBuffer(0);
		sscanf(str2, "%02x", &Sval2);
		DataCode[5] = Sval2;
	}
	else if(len == 2)
	{
		target = s.Mid(0,2);
		const char* str2 = target.GetBuffer(0);
		sscanf(str2, "%02x", &Sval2);
		DataCode[5] = Sval2;
		DataCode[4] = 0x00;
	}
	else if(len == 1)
	{
		target = s.Mid(0,1);
		const char* str2 = target.GetBuffer(0);
		sscanf(str2, "%02x", &Sval2);
		DataCode[5] = Sval2;
		DataCode[4] = 0x00;
	}
}

//IEEE754 十进制转十六进制
void data2hex(const float data, char* hex)
{
	fpx.value = data;
	unsigned char i = 0, merchant, remainder;
	for(; i<4; ++i)
	{
		merchant	= fpx.data[i] / 16;
		remainder	= fpx.data[i] % 16;
		hex[7-i*2-1]= merchant +(merchant >=10 ? 55:48);
		hex[7-i*2]	= remainder +(remainder >=10 ? 55:48);
	}
}

//IEEE754 十六进制转十进制
void hex2data(const char* hex, float& data)
{
	unsigned char multiplier,residual;
	int i=3;
	for (; i>=0; i--)
	{
		residual		= hex[i*2+1] -(hex[i*2+1] >= 'A' ? 55:48);
		multiplier		= hex[i*2] -(hex[i*2] >= 'A' ? 55:48);
		fpx.data[3-i]	= multiplier * 16 + residual;
	}
	data = fpx.value;
}

void ConvertStagePos(float Actualoc, unsigned char ucAxis)
{
	short anxi = SerialNum( ucAxis );
	g_ParamCurrent.fSteps[anxi] = Actualoc;
}

//IEEE754 十六进制转十进制
float ConvertReadData(BYTE* lbuff, unsigned char ucAxis)
{
	float location;
//	BYTE lbuff[4];
	unsigned char highByte, lowByte;
	char p[8]={0};
//	lbuff[0]=LocationH[4];lbuff[1]=LocationH[5];lbuff[2]=LocationL[4];lbuff[3]=LocationL[5];
	//16进制转字符串
	for (int i = 0; i < 4; i++)
	{
		highByte	= lbuff[i] >> 4;
		lowByte		= lbuff[i] & 0x0f ;

		highByte	+= 0x30;
		if (highByte > 0x39)
		    p[i * 2] = highByte + 0x07;
		else
			p[i * 2] = highByte;

		lowByte += 0x30;
		if (lowByte > 0x39)
			p[i * 2 + 1] = lowByte + 0x07;
	    else
		    p[i * 2 + 1] = lowByte;
	}

//	const char* n;
//	n = p;
	int Pos;
	sscanf(p, "%08x", &Pos);
	float* loca = (float*)&Pos;
	printf("%f\n", *loca);
//	loc = f;
//	printf("%f\n",Actualoca);
	location = *loca;
	//ConvertStagePos(location, ucAxis);
// 	GetCurPosition(Axial);
	return location;
}

BYTE JudgeAxis(unsigned char ucAxis)
{
	short anxi = SerialNum( ucAxis );
	BYTE bf = g_ParamAddr.byteAddr[anxi];
	return bf;
}

float converet(BYTE* Value )
{
	unsigned char highByte, lowByte;
	char Pcon[4]={0};
// 	lbuff[0]=LocationH[4];lbuff[1]=LocationH[5];lbuff[2]=LocationL[4];lbuff[3]=LocationL[5];
	//16进制转字符串
	for (int i = 0; i < 2; i++)
	{
		highByte = Value[i] >> 4;
		lowByte =  Value[i] & 0x0f ;
		highByte += 0x30;

		if (highByte > 0x39)
			Pcon[i * 2] = highByte + 0x07;
		else
			Pcon[i * 2] = highByte;

		lowByte += 0x30;
		if (lowByte > 0x39)
			Pcon[i * 2 + 1] = lowByte + 0x07;
		else
			Pcon[i * 2 + 1] = lowByte;
	}

	int Pos;
	sscanf(Pcon,"%x",&Pos);
	float loca =  (float)(Pos / 100.0);
	return loca;
}

//停止
void StopW(unsigned char ucAxis)
{
	BYTE StbuffR[8] = {0};
	BYTE Recvdata[200]= {0};

	StbuffR[0] = JudgeAxis(ucAxis);
	StbuffR[1] = 0x06;
	StbuffR[2] = 0x00;
	StbuffR[3] = 0x18;
	StbuffR[4] = 0x00;
	StbuffR[5] = 0x00;

	//计算CRC校验位
	CRCheck( StbuffR );
	int nlen = sizeof(StbuffR);
	int nLenSend= g_MC6600.SendDataEx(StbuffR, nlen);
	Sleep(nMC6600_Sleep);
	int nlenRecvRH = g_MC6600.ReadData((BYTE*)Recvdata, 200);
}

BOOL allZero()
{
	int total = 0;
	for(int i=0;i<5;i++)
		if(g_bMC6600OpenElec[i] == 0)
			total++;
	if(total == 5)	
		return TRUE;
	return FALSE;
}

//限位是否解除
void CCommMC6600::LimRelease(int nID, unsigned char ucAxis)
{
	if( !g_bMC6600Open )
		return;
	BYTE* LimRel = new BYTE[8];
	ZeroMemory(LimRel,8);
	BYTE RecvdataLR[200];
	LimRel[0] = JudgeAxis(ucAxis);
	LimRel[1] = 0x06;
	LimRel[2] = 0x00;
	if( g_ParamStyle.bOld && (ucAxis == g_ParamStyle.ucAxis) )
		LimRel[3] = 0x3A;
	else
		LimRel[3] = 0x44;
	LimRel[4] = 0x00;
	switch(nID)
	{
	case 0:
		//限位有效
		LimRel[5] = 0x00;
		break;
	case 1:
		//限位无效
		LimRel[5] = 0x01;
		break;
	}
	CRCheck(LimRel);
	CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(LimRel);
}

//查询回读解析
BOOL  Readanalysis(unsigned char ucAxis,BYTE *Recvdata)
{
	BYTE RecvBuff[7] = {0};
	for(int i=0;i<5;i++)
		RecvBuff[i] = Recvdata[i];
	if(RecvBuff[0] != JudgeAxis(ucAxis))
		return FALSE;
	//计算CRC校验位
	unsigned short tmp = 0xffff;
	unsigned short ret1 = 0;
	for(int n = 0; n < 5; n++)
	{
		//此处的6 -- 要校验的位数为6个
		tmp = RecvBuff[n] ^ tmp;
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
	//CRC校验后的值
	//printf("%X\n",tmp);
	//将CRC校验的高低位对换位置
	ret1 = tmp >> 8;
	ret1 = ret1 | (tmp << 8); 
	RecvBuff[5]= tmp;	// 低8位
	RecvBuff[6]= ret1;	// 高8位
	if(RecvBuff[5] == Recvdata[5] && RecvBuff[6] == Recvdata[6])
		return TRUE;
	else
		return FALSE;
}

//向伺服电机发送移动的编码
int ConveretSendDataW( const float* data ,  unsigned char ucAxis )
{
	if( !g_bMC6600Open )
		return 1;
//	unsigned char buff[8] = {0};
	for (int h=0; h<2; h++)
	{
		if(!h)
		{
			BYTE* buffW = new BYTE[8];
			ZeroMemory(buffW,8);
	
			BYTE Recvdata[200];
			buffW[0] = JudgeAxis(ucAxis);
			buffW[2] = 0x00;
			char* hexW = new char[8];
		//	float result =0;
			data2hex(*data, hexW);

			CString s,tarhigh;
			s = hexW;
			int nPos1,nPos2,nPos3,nPos4;

			buffW[1] = 0x06;

			//目标位置IEEE 754换算
			buffW[3]=0x2A;
			tarhigh = s.Mid(0,2);
			const char* str1 = tarhigh.GetBuffer(0);
			sscanf(str1,"%02x",&nPos1);
			buffW[4] = nPos1;

			tarhigh = s.Mid(2,2);
			const char* str2 = tarhigh.GetBuffer(0);
			sscanf(str2,"%02x",&nPos2);
			buffW[5] = nPos2;
			
			//计算CRC校验位
			CRCheck( buffW );
			CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(buffW);
			delete [] hexW;
		}
		else
		{
			BYTE* buffW = new BYTE[8];
			ZeroMemory(buffW,8);
	
			BYTE Recvdata[200];
			buffW[0] = JudgeAxis(ucAxis);
			buffW[2] = 0x00;

			int j =0;
			char* hexW = new char[8];
			data2hex(*data, hexW);

			CString s,tarhigh;
			s = hexW;
			int nPos1,nPos2,nPos3,nPos4;

			buffW[1] = 0x06;
			buffW[3]=0x2B;
			tarhigh = s.Mid(4,2);
			const char* str3 = tarhigh.GetBuffer(0);
			sscanf(str3,"%02x",&nPos3);
			buffW[4] = nPos3;

			tarhigh = s.Mid(6,2);
			const char* str4 = tarhigh.GetBuffer(0);
			sscanf(str4,"%02x",&nPos4);
			buffW[5] = nPos4;
			//计算CRC校验位
			CRCheck( buffW );
			CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(buffW);
			delete [] hexW;
		}
	}
	return 0;
}

//向伺服电机发送读取移动位置的编码
float ConveretSendDataR( unsigned char ucAxis )
{
	BYTE buffR[8] = {0}, buff[4] = {0};
	BYTE Recvdata[200] = {0};
	float Actualoca;
	buffR[0] = JudgeAxis(ucAxis);
	for (int h=0;h < 2;h++)
	{
	  	//buffR[0] = 0x02;
	  	buffR[2] = 0x00;
    	buffR[1] = 0x03;
    	buffR[4] = 0x00;
	    buffR[5] = 0x01;
		if(!h)
		{
			buffR[3]=0x24;
			//计算CRC校验位
			CRCheck( buffR );
			int nlenr1 = sizeof(buffR);
			int nLenSend= g_MC6600.SendDataEx(buffR, nlenr1);
			Sleep(nMC6600_Sleep);
		} 
		else
		{
			buffR[3]=0x25;
			//计算CRC校验位
			CRCheck( buffR );
			int nlenr2 = sizeof(buffR);
			int nLenSend= g_MC6600.SendDataEx(buffR, nlenr2);
			Sleep(nMC6600_Sleep);
			int nlenRecv = g_MC6600.ReadData((BYTE*)Recvdata, 200);
		}
	}
	buff[0] = Recvdata[3]; buff[1] = Recvdata[4]; buff[2] = Recvdata[10]; buff[3] = Recvdata[11];
	BYTE* Temlocation;
	Temlocation = buff;
	Actualoca = ConvertReadData(Temlocation,ucAxis);

	return Actualoca;
}

//目标位置发送以实现停止
void SendtarPosW(BYTE* SRecvdata, short anxi)
{
	if( !g_bMC6600Open )
		return;
	for (int h=0;h < 2;h++)
	{
		if(!h)
		{
			BYTE* SbuffW = new BYTE[8];
			ZeroMemory(SbuffW,8);

			SbuffW[0] = g_ParamAddr.byteAddr[anxi];
			SbuffW[1] = 0x06;
			SbuffW[2] = 0x00;
			SbuffW[3] =	0x2A;
			SbuffW[4] = SRecvdata[0];
			SbuffW[5] = SRecvdata[1];
			//计算CRC校验位
			CRCheck( SbuffW );
			CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(SbuffW);
		}
		else
		{
			BYTE* SbuffW = new BYTE[8];
			ZeroMemory(SbuffW,8);

			SbuffW[0] = g_ParamAddr.byteAddr[anxi];
			SbuffW[1] = 0x06;
			SbuffW[2] = 0x00;
			SbuffW[3] =	0x2B;
			SbuffW[4] = SRecvdata[2];
			SbuffW[5] = SRecvdata[3];
			//计算CRC校验位
			CRCheck( SbuffW );
			CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(SbuffW);
		}
	}
}

//当前位置读取
float getCurPosition(unsigned char ucAxis )
{
	float fSteps = ConveretSendDataR(ucAxis);
	ConvertStagePos(fSteps,ucAxis);
	return fSteps;
}

//状态读取
int statusReadInit( unsigned char ucAxis)
{
	BYTE SbuffR[8] = {0x00};
	BYTE Recvdata[200]= {0};

	SbuffR[0] = JudgeAxis(ucAxis);
	SbuffR[1] = 0x03;
	SbuffR[2] = 0x00;
	if( g_ParamStyle.bOld && (ucAxis == g_ParamStyle.ucAxis) )
		SbuffR[3] = 0x2F;
	else
		SbuffR[3] = 0x30;
	SbuffR[4] = 0x00;
	SbuffR[5] = 0x01;

	//计算CRC校验位
	CRCheck( SbuffR );
	int nlens = sizeof(SbuffR);
	int nLenSend= g_MC6600.SendDataEx(SbuffR, nlens);
	Sleep(nMC6600_Sleep);
	int nlenRecv = g_MC6600.ReadData((BYTE*)Recvdata, 200);
	//十六进制字节型转十进制int型
	BYTE sl[1];
	sl[0] = Recvdata[4];
//	sl[0] = Recvdata[4];

	unsigned char highByte, lowByte;
	char p[2]={0};
	for (int i = 0; i < 1; i++)
	{
		highByte = sl[i] >> 4;
		lowByte =  sl[i] & 0x0f ;

		highByte += 0x30;
		if (highByte > 0x39)
			p[i * 2] = highByte + 0x07;
		else
			p[i * 2] = highByte;

		lowByte += 0x30;
		if (lowByte > 0x39)
			p[i * 2 + 1] = lowByte + 0x07;
		else
			p[i * 2 + 1] = lowByte;
	}

	float value,status;
	value = p[1];
	if( value > 47 && value < 58)
		status = value-48;
	if( value > 64)
		status = value-55;
//	status = 10;
	short anxi = SerialNum(ucAxis);
	g_ParamCurrent.nStatus[anxi] = status;
	return  status;
}

void getCurStatus(unsigned char ucAxis )
{
	if( !g_bMC6600Open )
		return;
	BYTE* SbuffR = new BYTE[8];

	SbuffR[0] = JudgeAxis(ucAxis);
	SbuffR[1] = 0x03;
	SbuffR[2] = 0x00;
	if( g_ParamStyle.bOld && (ucAxis == g_ParamStyle.ucAxis) )
		SbuffR[3] = 0x2F;
	else
		SbuffR[3] = 0x30;
	SbuffR[4] = 0x00;
	SbuffR[5] = 0x01;

	//计算CRC校验位
	CRCheck( SbuffR );
	CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(SbuffR);
}

//使能状态
void enable(short anxi)
{
	BYTE Enable[8] = {0};
	BYTE ERecvdata[200]={0};
	Enable[0]= g_ParamAddr.byteAddr[anxi];
	Enable[1]= 0x06;
	Enable[2]= 0x00;
	Enable[3]= 0x18;
	Enable[4]= 0x00;
	Enable[5]= 0x01;
	CRCheck(Enable);
	int nlenEn = sizeof(Enable);
	int nLenSendE = g_MC6600.SendDataEx(Enable, nlenEn);
	Sleep(nMC6600_Sleep);
	int nReadValue = 0;
	nReadValue = g_MC6600.ReadData((BYTE*)ERecvdata, 200);
	if(nReadValue>0)
		g_bMC6600OpenElec[anxi] = TRUE;
	else
		g_bMC6600OpenElec[anxi] = FALSE;
}

void CCommMC6600::Enable(short anxi)
{
	if( !g_bMC6600Open )
		return;
	BYTE* Enable = new BYTE[8];
	ZeroMemory(Enable,8);

	Enable[0]= g_ParamAddr.byteAddr[anxi];
	Enable[1]= 0x06;
	Enable[2]= 0x00;
	Enable[3]= 0x18;
	Enable[4]= 0x00;
	Enable[5]= 0x01;
	CRCheck(Enable);
	CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(Enable);
}

//位置清零
void CCommMC6600::Reset(short anxi)
{
	if( !g_bMC6600Open )
		return;
	BYTE*  Reset = new BYTE[8];
	ZeroMemory(Reset,8);
	Reset[0]= g_ParamAddr.byteAddr[anxi];
	Reset[1]= 0x06;
	Reset[2]= 0x00;
	Reset[3]= 0x29;
	Reset[4]= 0x00;
	Reset[5]= 0x01;
	CRCheck(Reset);
	CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(Reset);
}

void CCommMC6600::LocaZero(short anxi)
{
	if( !g_bMC6600Open )
		return;
	//目标值为0-高位
	BYTE* ZeroA = new BYTE[8];
	ZeroMemory(ZeroA,8);
	ZeroA[0]= g_ParamAddr.byteAddr[anxi];
	ZeroA[1]= 0x06;
	ZeroA[2]= 0x00;
	ZeroA[3]= 0x2A;
	ZeroA[4]= 0x00;
	ZeroA[5]= 0x00;
	CRCheck(ZeroA);
	CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(ZeroA);

	//目标值为0-低位
	BYTE* ZeroB = new BYTE[8];
	ZeroMemory(ZeroB,8);
	ZeroB[0]= g_ParamAddr.byteAddr[anxi];
	ZeroB[1]= 0x06;
	ZeroB[2]= 0x00;
	ZeroB[3]= 0x2B;
	ZeroB[4]= 0x00;
	ZeroB[5]= 0x00;
	CRCheck(ZeroB);
	CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(ZeroB);
}

//回退操作
float CCommMC6600::BackOff( unsigned char ucAxis, float LimDirID )
{
	short anxi = SerialNum(ucAxis);
	float fStep = g_ParamCurrent.fSteps[anxi];

	if(LimDirID ==1)
	{
		switch (ucAxis)
		{
			case   'X':
				fStep = fStep-0.25f;	//0.25mm
				break;
			case 'Y':
				fStep = fStep-0.25f;	//0.25mm
				break;
			case 'Z':
				fStep = fStep-0.5;	//0.25mm
				break;
			case 'R':
				fStep = fStep-0.1f;	//0.5度
				break;
			case 'T':
				fStep = fStep-4/9.f;//1度
				break;
		}
	}	
	else
	{
		switch (ucAxis)
		{
			case   'X':
				fStep = fStep+0.25f;	//0.25mm
				break;
			case 'Y':
				fStep = fStep+0.25f;	//0.25mm
				break;
			case 'Z':
				fStep = fStep+0.5;	//0.25mm
				break;
			case 'R':
				fStep = fStep+0.1f;	//0.5度
				break;
			case 'T':
				fStep = fStep+4/9.f;//1度
				break;
		}
	}

	const float* cmd;
	cmd = &fStep;
	SendCmd(cmd , ucAxis );
/*	float Sbase=0;
	float SActualoc=0;
	while(SActualoc != Sbase || abs(SActualoc-fStep)>2 )
	{
		Sbase = SActualoc;
		SActualoc = ConveretSendDataR(ucAxis);
		Sleep(nMC6600_Sleep);
	}
	anxi = SerialNum(ucAxis);
	Enable(anxi);*/
	return fStep;
}

//设置向导
void  CCommMC6600::SetWizard(  unsigned char ucAxis ,BYTE M )
{
	BYTE  MType[8],Recvdata[200]={0};
	MType[0]= JudgeAxis(ucAxis);
	MType[1]= 0x06;
	MType[2]= 0x00;
	MType[3]= 0x28;
	MType[4]= 0x00;
	MType[5]= M;
	CRCheck(MType);
	int nlenE = sizeof(MType);
	int nLenSendMT = g_MC6600.SendDataEx(MType, nlenE);
	Sleep(100);
	int nlenRecvMT = g_MC6600.ReadData((BYTE*)Recvdata, 200);
}

void CCommMC6600::SetEncoderLines(  unsigned char ucAxis ,int M )
{
	BYTE  EncodLine[8],Recvdata[200]={0};
	EncodLine[0]= JudgeAxis(ucAxis);
	EncodLine[1]= 0x06;
	EncodLine[2]= 0x00;
	if( g_ParamStyle.bOld && (ucAxis == g_ParamStyle.ucAxis) )
		EncodLine[3]= 0x39;
	else
		EncodLine[3]= 0x43;
	dec2hex(EncodLine,M);
	CRCheck(EncodLine);
	int nlenEL = sizeof(EncodLine);
	int nLenSendE = g_MC6600.SendDataEx(EncodLine, nlenEL);
	Sleep(nMC6600_Sleep);
	int nlenRecvEL = g_MC6600.ReadData(Recvdata, 200);
}

void CCommMC6600::SetControlSignal(  unsigned char ucAxis ,BYTE M )
{
	BYTE  CSignal[8],Recvdata[200]={0};
	CSignal[0]= JudgeAxis(ucAxis);
	CSignal[1]= 0x06;
	CSignal[2]= 0x00;
	CSignal[3]= 0x15;
	CSignal[4]= 0x00;
	CSignal[5]= M;
	CRCheck(CSignal);
	int nlenCS = sizeof(CSignal);
	int nLenSendCS = g_MC6600.SendDataEx(CSignal, nlenCS);
	Sleep(nMC6600_Sleep);
	int nlenRecvCS = g_MC6600.ReadData(Recvdata, 200);
}

void CCommMC6600::SetControlMode(  unsigned char ucAxis ,BYTE M )
{
	BYTE  CMode[8],Recvdata[200]={0};
	CMode[0]= JudgeAxis(ucAxis);
	CMode[1]= 0x06;
	CMode[2]= 0x00;
	CMode[3]= 0x16;
	CMode[4]= 0x00;
	CMode[5]= M;
	CRCheck(CMode);
	int nlenCM = sizeof(CMode);
	int nLenSendCM = g_MC6600.SendDataEx(CMode, nlenCM);
	Sleep(nMC6600_Sleep);
	int nlenRecvCM = g_MC6600.ReadData(Recvdata, 200);
}

void CCommMC6600::SetMaxCurrent(  unsigned char ucAxis ,int M )
{
	BYTE  MaxCur[8],OutPercentW[8],Recvdata[200]={0};
/*
	OutPercentR[0]=  JudgeAxis(ucAxis);OutPercentR[1]= 0x03;
	OutPercentR[2]= 0x00;OutPercentR[3]= 0x1C;
	OutPercentR[4]= 0x00;OutPercentR[5]= 0x01;
	CRCheck(OutPercentR);
	int nlenOPR = sizeof(OutPercentR);
	int nLenSendOPR = g_MC6600.SendDataEx(OutPercentR, nlenOPR);
	Sleep(nMC6600_Sleep);
	int nlenRecvOPR = g_MC6600.ReadData(Recvdata, 200);
*/
	OutPercentW[4]= Recvdata[3];
	OutPercentW[5]= Recvdata[4];

	MaxCur[0]= JudgeAxis(ucAxis);
	MaxCur[1]= 0x06;
	MaxCur[2]= 0x00;
	MaxCur[3]= 0x1B;
	dec2hex(MaxCur,M);
	CRCheck(MaxCur);
	int nlenMC = sizeof(MaxCur);
	int nLenSendMC = g_MC6600.SendDataEx(MaxCur, nlenMC);
	Sleep(nMC6600_Sleep);
	int nlenRecvMC = g_MC6600.ReadData(Recvdata, 200);
	Sleep(nMC6600_Sleep);

	if( !(g_ParamStyle.bOld) || (g_ParamStyle.ucAxis != ucAxis) )
	{
		OutPercentW[0]= JudgeAxis(ucAxis);
		OutPercentW[1]= 0x06;
		OutPercentW[2]= 0x00;
		OutPercentW[3]= 0x1C;
		OutPercentW[4]= 0x00;
		OutPercentW[5]= 0x5A;
		CRCheck(OutPercentW);
		int nlenOPW = sizeof(OutPercentW);
		int nLenSendOPW = g_MC6600.SendDataEx(OutPercentW, nlenOPW);
		Sleep(nMC6600_Sleep);
		int nlenRecvOPW = g_MC6600.ReadData(Recvdata, 200);
	}
}

void CCommMC6600::SetSpeed( unsigned char ucAxis, float fSpeed )
{
/*	BYTE SpeedSet[8];
	SpeedSet[0]= JudgeAxis(ucAxis);SpeedSet[1]= 0x06;
	SpeedSet[2]= 0x00;SpeedSet[3]= 0x2C;
	dec2hex(SpeedSet,fSpeed);
//	Pospeed[4]= 0x01;Pospeed[5]= 0xF4;
	CRCheck(SpeedSet);
	int nlenss = sizeof(SpeedSet);
	int nLenSendP = g_MC6600.SendDataEx(SpeedSet, nlenss);
*/	
	const float* cmd;
	cmd  = &fSpeed;
	short anxi = SerialNum(ucAxis);
	PosSpeed(anxi,cmd);
	Sleep(nMC6600_Sleep);
}

//PID值查询
void PIDQuery(int ID, short anxi)  //ID=1\2\3 ->P I D
{
	if( !g_bMC6600Open )
		return;
	for (int h=0;h < 2;h++)
	{
		if(!h)
		{
			BYTE* PIDQ = new BYTE[8];
			ZeroMemory(PIDQ,8);
			PIDQ[0] = g_ParamAddr.byteAddr[anxi];
			PIDQ[1] = 0x03;
			PIDQ[2] = 0x00;
			PIDQ[4] = 0x00;
			PIDQ[5] = 0x01;
			if(ID ==1)
				PIDQ[3]=0x3D;
			else if(ID ==2)
				PIDQ[3]=0x3F;
			else if(ID ==3)
				PIDQ[3]=0x41;
			//计算CRC校验位
			CRCheck( PIDQ );
			CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(PIDQ);
// 			CurrentlocH = RecvdataH;
		} 
		else
		{
			BYTE* PIDQ = new BYTE[8];
			ZeroMemory(PIDQ,8);
			PIDQ[0] = g_ParamAddr.byteAddr[anxi];
			PIDQ[1] = 0x03;
			PIDQ[2] = 0x00;
			PIDQ[4] = 0x00;
			PIDQ[5] = 0x01;
			if(ID ==1)
				PIDQ[3]=0x3E;
			else if(ID ==2)
				PIDQ[3]=0x40;
			else if(ID ==3)
				PIDQ[3]=0x42;
			//计算CRC校验位
			CRCheck( PIDQ );
			CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(PIDQ);
		}  
	}
}

/*float CCommMC6600::PtoQuery(int Ai)
{
	//P值查询
	BYTE  P[8],PRecvdata[200]={0};
	P[0]= g_ParamAddr.byteAddr[Ai];
	P[1]= 0x03;
	P[2]= 0x00;
	P[3]= 0x36;
	P[4]= 0x00;
	P[5]= 0x01;
	CRCheck(P);
	int nlenpq = sizeof(P);
	int nLenSendE = g_MC6600.SendDataEx(P, nlenpq);
	Sleep(nMC6600_Sleep);
	int nlenRecv = g_MC6600.ReadData((BYTE*)PRecvdata, 200);

	BYTE Value[2];
// 	if(PRecvdata[1] == 0x06 )
// 	{
// 		Value[0] = PRecvdata[19];
// 		Value[1] = PRecvdata[20];
// 	}
// 	else 
// 	{
// 		Value[0] = PRecvdata[3];
// 		Value[1] = PRecvdata[4];
// 	}
	Value[0] = PRecvdata[3];
	Value[1] = PRecvdata[4];
	float value = converet(Value);
	return value;
} */

void ItoQuery(int Ai)
{
	if( !g_bMC6600Open )
		return;
	//I值查询
	BYTE*  I = new BYTE[8];
	I[0]= g_ParamAddr.byteAddr[Ai];
	I[1]= 0x03;
	I[2]= 0x00;
	I[3]= 0x37;
	I[4]= 0x00;
	I[5]= 0x01;
	CRCheck(I);
	CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(I);
}

void DtoQuery(int Ai)
{
	if( !g_bMC6600Open )
		return;
	//D值查询
	BYTE*  D = new BYTE[8];
	ZeroMemory(D,8);
	D[0]= g_ParamAddr.byteAddr[Ai];
	D[1]= 0x03;
	D[2]= 0x00;
	D[3]= 0x38;
	D[4]= 0x00;
	D[5]= 0x01;
	CRCheck(D);
    CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(D);
}

void CCommMC6600::PIDModify(int ID, unsigned char ucAxis, float Pv)
{
	if( !g_bMC6600Open )
		return;
	for (int h=0;h < 2;h++)
	{
		if(!h)
		{
			BYTE* PIDM = new BYTE[8];
			ZeroMemory(PIDM,8);
			PIDM[0] = JudgeAxis(ucAxis);
			const float* cmd;
			cmd = &Pv;
			char* hexW = new char[8];
			data2hex(*cmd, hexW);
			CString s,tarhigh;
			s = hexW;
			int nPos1,nPos2;
			PIDM[1] = 0x06;
			PIDM[2] = 0x00;
			if(ID ==1)
				PIDM[3]=0x3D;
			else if(ID ==2)
				PIDM[3]=0x3F;
			else if(ID ==3)
				PIDM[3]=0x41;
			//目标位置IEEE 754换算
			tarhigh = s.Mid(0,2);
			const char* str1=tarhigh.GetBuffer(0);
			sscanf(str1,"%02x",&nPos1);
			PIDM[4] = nPos1;

			tarhigh = s.Mid(2,2);
			const char* str2=tarhigh.GetBuffer(0);
			sscanf(str2,"%02x",&nPos2);
			PIDM[5] = nPos2;

			//计算CRC校验位
			CRCheck( PIDM );
			CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(PIDM);
			delete [] hexW;
		}
		else
		{
			BYTE* PIDM = new BYTE[8];
			ZeroMemory(PIDM,8);
			PIDM[0] = JudgeAxis(ucAxis);
			const float* cmd;
			cmd = &Pv;
			char* hexW = new char[8];
			data2hex(*cmd, hexW);
			CString s,tarhigh;
			s = hexW;
			int nPos1,nPos2;
			PIDM[1] = 0x06;
			PIDM[2] = 0x00;
			if(ID ==1)
				PIDM[3]=0x3E;
			else if(ID ==2)
				PIDM[3]=0x40;
			else if(ID ==3)
				PIDM[3]=0x42;
			//目标位置IEEE 754换算
			tarhigh = s.Mid(4,2);
			const char* str1=tarhigh.GetBuffer(0);
			sscanf(str1,"%02x",&nPos1);
			PIDM[4] = nPos1;

			tarhigh = s.Mid(6,2);
			const char* str2=tarhigh.GetBuffer(0);
			sscanf(str2,"%02x",&nPos2);
			PIDM[5] = nPos2;

			//计算CRC校验位
			CRCheck( PIDM );
			CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(PIDM);
			delete [] hexW;
		}  
	}
}
/*
void CCommMC6600::PtoModify(unsigned char ucAxis, float Pv)
{
	const float* data = &Pv;

	BYTE buffP[8] = {0};
	BYTE PRecvdata[200];
	int PPos1,PPos2,iPv;
	iPv = Pv;
	buffP[0] = JudgeAxis(ucAxis);
	buffP[1] = 0x06;
	buffP[2] = 0x00;
	buffP[3] = 0x36;

	char temporary[5];
	sprintf(temporary,"%x",iPv);

	CString s, tarhigh;
	s = temporary;
	int length = s.GetLength();
	if(length == 4)
	{
		tarhigh = s.Mid(0,2);
		const char* str1=tarhigh.GetBuffer(0);
		sscanf(str1,"%02x",&PPos1);
		buffP[4] = PPos1;

		tarhigh = s.Mid(2,2);
		const char* str2=tarhigh.GetBuffer(0);
		sscanf(str2,"%02x",&PPos2);
		buffP[5] = PPos2;
	}
	else if(length == 3)
	{
		tarhigh = s.Mid(0,1);
		const char* str1=tarhigh.GetBuffer(0);
		sscanf(str1,"%02x",&PPos1);
		buffP[4] = PPos1;

		tarhigh = s.Mid(1,3);
		const char* str2=tarhigh.GetBuffer(0);
		sscanf(str2,"%02x",&PPos2);
		buffP[5] = PPos2;
	}
	else if(length == 2)
	{
		tarhigh = s.Mid(0,2);
		const char* str1=tarhigh.GetBuffer(0);
		sscanf(str1,"%02x",&PPos2);
		buffP[5] = PPos2;
		buffP[4] = 0x00; 
	}
	else if(length == 1)
	{
		tarhigh = s.Mid(0,1);
		const char* str1=tarhigh.GetBuffer(0);
		sscanf(str1,"%02x",&PPos2);
		buffP[5] = PPos2;
		buffP[4] = 0x00; 
	}

	//计算CRC校验位
	CRCheck( buffP );
	int nLenSend= g_MC6600.SendDataEx(buffP, sizeof(buffP));
	int nlenRecvSH = g_MC6600.ReadData(PRecvdata, 200);
	Sleep(nMC6600_Sleep);
}
*/
void CCommMC6600::ItoModify(unsigned char ucAxis, float Iv)
{
	if( !g_bMC6600Open )
		return;
	const float* data = &Iv;

	BYTE* buffI = new BYTE[8];
	ZeroMemory(buffI,8);
	int IPos1,IPos2,iIv;
	iIv = Iv;
	buffI[0] = JudgeAxis(ucAxis);
	buffI[1] = 0x06;
	buffI[2] = 0x00;
	buffI[3] = 0x37;

	char temporary[5];
	sprintf(temporary,"%x",iIv);

	CString s,tarhigh;
	s =temporary;
	int length = s.GetLength();
	if(length == 4)
	{
		tarhigh = s.Mid(0,2);
		const char* str1=tarhigh.GetBuffer(0);
		sscanf(str1,"%02x",&IPos1);
		buffI[4] = IPos1;

		tarhigh = s.Mid(2,2);
		const char* str2=tarhigh.GetBuffer(0);
		sscanf(str2,"%02x",&IPos2);
		buffI[5] = IPos2;
	}
	else if(length == 3)
	{
		tarhigh = s.Mid(0,1);
		const char* str1=tarhigh.GetBuffer(0);
		sscanf(str1,"%02x",&IPos1);
		buffI[4] = IPos1;

		tarhigh = s.Mid(1,3);
		const char* str2=tarhigh.GetBuffer(0);
		sscanf(str2,"%02x",&IPos2);
		buffI[5] = IPos2;
	}
	else if(length == 2)
	{
		tarhigh = s.Mid(0,2);
		const char* str1=tarhigh.GetBuffer(0);
		sscanf(str1,"%02x",&IPos2);
		buffI[5] = IPos2;
		buffI[4] = 0x00; 
	}
	else if(length == 1)
	{
		tarhigh = s.Mid(0,1);
		const char* str1=tarhigh.GetBuffer(0);
		sscanf(str1,"%02x",&IPos2);
		buffI[5] = IPos2;
		buffI[4] = 0x00; 
	}

	//计算CRC校验位
	CRCheck( buffI );
	CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(buffI);
}

void CCommMC6600::DtoModify(unsigned char ucAxis, float Dv)
{
	if( !g_bMC6600Open )
		return;
	const float* data = &Dv;

	BYTE* buffD = new BYTE[8];
	ZeroMemory(buffD,8);
	int DPos1,DPos2,iDv;
	iDv = Dv;
	buffD[0] = JudgeAxis(ucAxis);
	buffD[1] = 0x06;
	buffD[2] = 0x00;
	buffD[3] = 0x38;

	char temporary[5];
	sprintf(temporary,"%x",iDv);

	CString s,tarhigh;
	s =temporary;
	int length = s.GetLength();
	if(length == 4)
	{
		tarhigh = s.Mid(0,2);
		const char* str1=tarhigh.GetBuffer(0);
		sscanf(str1,"%02x",&DPos1);
		buffD[4] = DPos1;

		tarhigh = s.Mid(2,2);
		const char* str2=tarhigh.GetBuffer(0);
		sscanf(str2,"%02x",&DPos2);
		buffD[5] = DPos2;
	}
	else if(length == 3)
	{
		tarhigh = s.Mid(0,1);
		const char* str1=tarhigh.GetBuffer(0);
		sscanf(str1,"%02x",&DPos1);
		buffD[4] = DPos1;

		tarhigh = s.Mid(1,3);
		const char* str2=tarhigh.GetBuffer(0);
		sscanf(str2,"%02x",&DPos2);
		buffD[5] = DPos2;
	}
	else if(length == 2)
	{
		tarhigh = s.Mid(0,2);
		const char* str2=tarhigh.GetBuffer(0);
		sscanf(str2,"%02x",&DPos2);
		buffD[5] = DPos2;
		buffD[4] = 0x00; 
	}
	else if(length == 1)
	{
		tarhigh = s.Mid(0,1);
		const char* str2=tarhigh.GetBuffer(0);
		sscanf(str2,"%02x",&DPos2);
		buffD[5] = DPos2;
		buffD[4] = 0x00; 
	}

	//计算CRC校验位
	CRCheck( buffD );
	CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(buffD);
}

//void CCommMC6600::LimitChanged(int nID)
//{
//	BYTE LimRel[8]={0};
//	BYTE RecvdataLC[200];
//	switch(nID)
//	{
//	case 0:
//		//限位有效
//		LimRel[5] = 0x00;
//		break;
//	case 1:
//		//限位无效
//		LimRel[5] = 0x01;
//		break;
//	}
//	LimRel[0] = 0x04;
//	LimRel[1] = 0x06;
//	LimRel[2] = 0x00;
//	LimRel[3] = 0x44;
//	LimRel[4] = 0x00;
//	CRCheck(LimRel);
//	int nlenLC = sizeof(LimRel);
//	int nLenSendLC = g_MC6600.SendDataEx(LimRel, nlenLC);
//	Sleep(nMC6600_Sleep);
//	int nlenRecv = g_MC6600.ReadData((BYTE*)RecvdataLC, 200);
//}

//转速、使能初始化等
void InitMC6600Params( int nSleep )
{
	if( !g_bMC6600Open )
		return;
	BYTE  PosSpeed[8],PSRecvdata[200]={0};
	//位置转速设定- 寄存器为2个
	short i = 0;
	for(i=0;i<5;i++)
	{
		if( g_ParamStyle.bOld && (i == g_ParamStyle.nAxis) )
			continue;
		Sleep(nSleep);
		for (int h=0;h < 2;h++)
		{
			if(!h)
			{
				BYTE* PosSpeed = new BYTE[8];
				ZeroMemory(PosSpeed,8);
				const float* data;
				data  = &g_ParamSpeed.fSpeedInit[i];

				PosSpeed[0] = g_ParamAddr.byteAddr[i];
				PosSpeed[1] = 0x06;
				PosSpeed[2] = 0x00;

				char* hexW = new char[8];
				//float result =0;
				data2hex(*data, hexW);

				CString s,tarhigh;
				s = hexW;
				int nPos1,nPos2,nPos3,nPos4;

				unsigned short tmp = 0xffff;
				unsigned short ret1 = 0;
				PosSpeed[3]=0x2C;
				tarhigh = s.Mid(0,2);
				const char* str1=tarhigh.GetBuffer(0);
				sscanf(str1,"%02x",&nPos1);
				PosSpeed[4] = nPos1;

				tarhigh = s.Mid(2,2);
				const char* str2=tarhigh.GetBuffer(0);
				sscanf(str2,"%02x",&nPos2);
				PosSpeed[5] = nPos2;
			
				//计算CRC校验位
				CRCheck( PosSpeed );
				CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(PosSpeed);
				delete [] hexW;
			}
			else
			{
				BYTE* PosSpeed = new BYTE[8];
				ZeroMemory(PosSpeed,8);

				const float* data;
				data  = &g_ParamSpeed.fSpeedInit[i];

				PosSpeed[0] = g_ParamAddr.byteAddr[i];
				PosSpeed[1] = 0x06;
				PosSpeed[2] = 0x00;

				char* hexW = new char[8];
				//float result =0;
				data2hex(*data, hexW);

				CString s,tarhigh;
				s = hexW;
				int nPos1,nPos2,nPos3,nPos4;

				PosSpeed[3]=0x2D;
				tarhigh = s.Mid(4,2);
				const char* str3=tarhigh.GetBuffer(0);
				sscanf(str3,"%02x",&nPos3);
				PosSpeed[4] = nPos3;
			
				tarhigh = s.Mid(6,2);
				const char* str4=tarhigh.GetBuffer(0);
				sscanf(str4,"%02x",&nPos4);
				PosSpeed[5] = nPos4;

				//计算CRC校验位
				CRCheck( PosSpeed );
				CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(PosSpeed);
				delete [] hexW;
			}
		}
	}
	
	//转速设置 寄存器为1个-T轴为旧版驱动器
	if( g_ParamStyle.bOld )
	{
		BYTE* PosSpeed = new BYTE[8];
		ZeroMemory(PosSpeed,8);
		PosSpeed[0]= g_ParamAddr.byteAddr[g_ParamStyle.nAxis];
		PosSpeed[1]= 0x06;
		PosSpeed[2]= 0x00;
		PosSpeed[3]= 0x2C;
		dec2hex(PosSpeed, (int)(g_ParamSpeed.fSpeedInit[g_ParamStyle.nAxis]*100));
		//PosSpeed[4]= 0x01;
		//PosSpeed[5]= 0xF4;
		CRCheck(PosSpeed);
		CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(PosSpeed);
	}
	//PID查询
	for(i=0;i<5;i++)
	{
		if(g_ParamStyle.bOld && (i==g_ParamStyle.nAxis))
		{
			g_ParamCurPID.fP[i] = 10000.0f;
			ItoQuery(i);
			DtoQuery(i);
		}
		else
		{
			PIDQuery(1,i);
			PIDQuery(2,i);
			PIDQuery(3,i);
		}
	}
	//当前位置查询
	for(i=0; i<5; i++ )
	{
		getCurPosition(SerialNum(i));
		Sleep(100);
	}
	//当前状态查询
	for(i=0;i<5;i++)
	{
		statusReadInit(SerialNum(i));
		Sleep(100);
	}
}
	
//数据下发，数据读取
int CCommMC6600::SendCmd( const float* cmd , unsigned char ucAxis )
{
	ConveretSendDataW(cmd , ucAxis );
// 	Actualoc = ConveretSendDataR(ucAxis);
	return 0;
}

//状态读取
int CCommMC6600::StatusRead( unsigned char ucAxis)
{
	getCurStatus( ucAxis );
	return 0;
}

void CommWithBox6600( int nSleep )
{
	// 常规查询当前位置、状态值
	short i = 0;
	for( i=0; i<5; i++ )
	{
		if(CStageMC6600Manager::Instance().m_bIsMoving[i])
		{
			getCurPosition(SerialNum(i));
			Sleep(100);
			statusReadInit(SerialNum(i));
		}
	}
}

DWORD WINAPI InitThreadMC6600(LPVOID lpParam)
{
	//打开端口
	g_bMC6600Open = g_MC6600.Open( (short)lpParam,115200 );
	if(g_bMC6600Open)
	{
		//通讯是否正常
		for(int i=0;i<5;i++)
			enable(i);
		if( allZero() )
		{
			g_bMC6600Open = FALSE;
			PostMessage( g_hMain, WM_USER_MC6600_UpdateValue, 0, g_bMC6600Open );
			PostMessage( g_hMain, WM_USER_MC6600_UpdateValue, 7, (LPARAM)(&g_bMC6600OpenElec) );
		}
		else
		{
			InitMC6600Params(200);
			PostMessage( g_hMain, WM_USER_MC6600_UpdateValue, 7, (LPARAM)(&g_bMC6600OpenElec) );
			PostMessage( g_hMain, WM_USER_MC6600_UpdateValue, 0, g_bMC6600Open );
			PostMessage( g_hMain, WM_USER_MC6600_UpdateValue, 6, (LPARAM)(&g_ParamCurrent) );
		}
	}
	else
	{
		PostMessage( g_hMain, WM_USER_MC6600_UpdateValue, 0, g_bMC6600Open );
		PostMessage( g_hMain, WM_USER_MC6600_UpdateValue, 7, (LPARAM)(&g_bMC6600OpenElec)  );
	}
	while( g_bMC6600Open && WaitForSingleObject(g_hEventTeminateMC6600,300) != WAIT_OBJECT_0 )
	{
		CommWithBox6600(nMC6600_Sleep);
		PostMessage( g_hMain, WM_USER_MC6600_UpdateValue, 11, (LPARAM)(&g_ParamCurrent) );
		for(int i = 0; i <CStageMC6600Manager::Instance().m_vecCmdMC6600.size();i++ )
		{
			BYTE* pCmd = CStageMC6600Manager::Instance().m_vecCmdMC6600[i];
			BYTE Reset[8] = {0x00};
			memcpy(Reset,pCmd,8);
			delete pCmd;
			pCmd = NULL;
			/*CString strKey = _T("");
			for(int i = 0 ; i < 8; i++)
			{
				CString sByte = _T("");
				sByte.Format(_T("%02X "), Reset[i]);
				strKey = strKey + sByte;
			}*/
			BYTE Recvdata[200] = {0};
			int nlenre = sizeof(Reset);
			int nLenSendE = g_MC6600.SendDataEx(Reset, nlenre);
			Sleep(nMC6600_Sleep);
			int nlenRecv = g_MC6600.ReadData((BYTE*)Recvdata, 200);
			//判断是否下发成功
			while(!nlenRecv)
			{
				nLenSendE = g_MC6600.SendDataEx(Reset, nlenre);
				Sleep(nMC6600_Sleep);
				nlenRecv = g_MC6600.ReadData((BYTE*)Recvdata, 200);
			}
			short anxi = Reset[0];
			anxi = anxi-1;
			unsigned short ucAxis = SerialNum(anxi);
			switch( Reset[3] )
			{
			case	0x2F:
			case	0x30:
				{
					//状态查询
					//十六进制字节型转十进制int型
					BYTE sl[1];
					sl[0] = Recvdata[4];
					unsigned char highByte, lowByte;
					char p[2]={0};
					for (int i = 0; i < 1; i++)
					{
						highByte = sl[i] >> 4;
						lowByte =  sl[i] & 0x0f ;

						highByte += 0x30;
						if (highByte > 0x39)
							p[i * 2] = highByte + 0x07;
						else
							p[i * 2] = highByte;

						lowByte += 0x30;
						if (lowByte > 0x39)
							p[i * 2 + 1] = lowByte + 0x07;
						else
							p[i * 2 + 1] = lowByte;
					}

					float value,status;
					value = p[1];
					if( value > 47 && value < 58)
						status = value-48;
					if( value > 64)
						status = value-55;

					g_ParamCurrent.nStatus[anxi] = status;
				}
				break;

			case	0x24:
				{
					//位置查询第1条
					g_bytebuff[0] = Recvdata[3];
					g_bytebuff[1] = Recvdata[4];
				}
				break;
			case	0x25:
				{
					//位置查询第2条
					g_bytebuff[2] = Recvdata[3];
					g_bytebuff[3] = Recvdata[4];
					ConvertReadData(g_bytebuff,ucAxis);
				}
				break;

			case	0x3D:
			case	0x3F:
			case	0x41:
				{
					//PID设置第1条
					g_bytebuff[0] = Recvdata[3];
					g_bytebuff[1] = Recvdata[4];
				}
				break;
			case	0x3E:
			case	0x40:
			case	0x42:
				{
					//PID设置第2条
					g_bytebuff[2] = Recvdata[3];
					g_bytebuff[3] = Recvdata[4];
					switch (Reset[3])
					{
					case 0x3E:
						g_ParamCurPID.fP[anxi] = ConvertReadData(g_bytebuff,ucAxis);
						break;
					case 0x40:
						g_ParamCurPID.fI[anxi] = ConvertReadData(g_bytebuff,ucAxis);
						break;
					case 0x42:
						g_ParamCurPID.fD[anxi] = ConvertReadData(g_bytebuff,ucAxis);
						break;
					}
					PostMessage( g_hMain, WM_USER_MC6600_UpdateValue, 12,(LPARAM)(&g_ParamCurPID));
				}
				break;

			case	0x37:
			case	0x38:
				{
					//旧版-PID
					BYTE Value[2] = {0};
					Value[0] = Recvdata[3];
					Value[1] = Recvdata[4];
					switch(Reset[3])
					{
					case 0x37:
						g_ParamCurPID.fI[anxi] = converet(Value);
                        g_ParamCurPID.fI[anxi] = g_ParamCurPID.fI[anxi]/10.0f;
						break;
					case 0x38:
						g_ParamCurPID.fD[anxi] = converet(Value);
						g_ParamCurPID.fD[anxi] = g_ParamCurPID.fD[anxi]/10.0f;
						break;
					}
					PostMessage( g_hMain, WM_USER_MC6600_UpdateValue, 12,(LPARAM)(&g_ParamCurPID));
				}
				break;
			}
		}
		CStageMC6600Manager::Instance().m_vecCmdMC6600.clear();
		Sleep( 1 );
	}
	g_MC6600.Close();
	g_bMC6600Open = g_MC6600.IsOpened();

	SetEvent( g_hEventWorkMC6600 );
	return 0;
}

void CCommMC6600::InitHWND( HWND hWnd )
{
	g_hMain = hWnd;
}

BOOL CCommMC6600::Open( short port )
{
	ResetEvent( g_hEventWorkMC6600 );
	ResetEvent( g_hEventTeminateMC6600 );

	g_hInitThreadMC6600 = CreateThread(NULL, 0, InitThreadMC6600, (LPVOID)(port), 0, &g_dwinitThreadMC6600);
	if( g_hInitThreadMC6600 == NULL )
		return FALSE;

	CloseHandle(g_hInitThreadMC6600);
/*	char m = 'X';
	g_hStatuThreadMC6600 = CreateThread(NULL,0,StatuThreadMC6600,(LPVOID)(m),0,&g_dwstatuThreadMC6600);
	if( g_hStatuThreadMC6600 == NULL )
		return FALSE;
	CloseHandle(g_hStatuThreadMC6600);
*/	return TRUE;
}

BOOL CCommMC6600::Close()
{
	if( g_bMC6600Open )
	{
		SetEvent( g_hEventTeminateMC6600 );
		WaitForSingleObject( g_hEventWorkMC6600, INFINITE );
		g_bMC6600Open	= FALSE;
	}

	return TRUE;
}

BOOL CCommMC6600::close(short anxi)
{
	if( !g_bMC6600Open )
		return FALSE;

	BYTE*  Disability = new BYTE[8];
	ZeroMemory(Disability,8);
		
	BYTE StRecvdata[200] = {0};

	Disability[0]= g_ParamAddr.byteAddr[anxi];
	Disability[1]= 0x06;
	Disability[2]= 0x00;
	Disability[3]= 0x18;
	Disability[4]= 0x00;
	Disability[5]= 0x00;
	CRCheck(Disability);

	CString strKey = _T("");
	for(int i = 0 ; i < 8; i++)
	{
		CString sByte = _T("");
	    sByte.Format(_T("%02X "), Disability[i]);
		strKey = strKey + sByte;
	}
	CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(Disability);

	return FALSE;
}

void DefauParam()
{
}

void CCommMC6600::GetCurPosition(unsigned char ucAxis )
{
	getCurPosition(ucAxis);
}

//当前位置读取 //GetCurPosition?
void CCommMC6600::CurrPosR(short anxi)
{
	if( !g_bMC6600Open )
		return;
	g_bCurPosSetting = TRUE;
	for (int h=0;h < 2;h++)
	{
		if(!h)
		{
			BYTE* CbuffR = new BYTE[8];
			ZeroMemory(CbuffR,8);

			CbuffR[0] = g_ParamAddr.byteAddr[anxi];
			CbuffR[1] = 0x03;
			CbuffR[2] = 0x00;
			CbuffR[4] = 0x00;
			CbuffR[5] = 0x01;
			CbuffR[3]=  0x24;
			//计算CRC校验位
			CRCheck( CbuffR );
			CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(CbuffR);
		} 
		else
		{
			BYTE* CbuffR = new BYTE[8];
			ZeroMemory(CbuffR,8);

			CbuffR[0] = g_ParamAddr.byteAddr[anxi];
			CbuffR[1] = 0x03;
			CbuffR[2] = 0x00;
			CbuffR[4] = 0x00;
			CbuffR[5] = 0x01;
			CbuffR[3] =	0x25;
			//计算CRC校验位
			CRCheck( CbuffR );
			CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(CbuffR);
		}
	}
}

//扫描初始位置读取
float CCommMC6600::CurrPosO(unsigned char ucAxis)
{
	BYTE CbuffR[8] = {0},buff[4]={0};
	BYTE ORecvdata[200]= {0};
	CbuffR[0] =JudgeAxis(ucAxis);

	for (int h=0;h < 2;h++)
	{
//		CbuffR[0] = 0x01;
		CbuffR[1] = 0x03;
		CbuffR[2] = 0x00;
		CbuffR[4] = 0x00;
		CbuffR[5] = 0x01;
		if(!h)
		{
			CbuffR[3]=0x24;
			//计算CRC校验位
			CRCheck( CbuffR );
			int nlensr1 = sizeof(CbuffR);
			int nLenSend= g_MC6600.SendDataEx(CbuffR, nlensr1);
			Sleep(nMC6600_Sleep);
			int nlenRecvRH = g_MC6600.ReadData((BYTE*)ORecvdata, 40);
			if(buff[0] == ORecvdata[0])
				buff[0] = ORecvdata[3]; buff[1] = ORecvdata[4];
//			CurrentlocH = RecvdataH;
		}
		else
		{
			CbuffR[3]=0x25;
			//计算CRC校验位
			CRCheck( CbuffR );
			int nlensr2 = sizeof(CbuffR);
			int nLenSend= g_MC6600.SendDataEx(CbuffR, nlensr2);
//			int nlenRecvRL = g_MC6600.ReadData((BYTE*)RecvdataL, 40);
			Sleep(nMC6600_Sleep);
			int nlenRecv = g_MC6600.ReadData((BYTE*)ORecvdata, 200);
			if(buff[0] == ORecvdata[0])
				buff[2] = ORecvdata[3]; buff[3] = ORecvdata[4];
//			CurrentlocL = RecvdataL;
		}
	}

	float Actualoca = ConvertReadData(buff,ucAxis);
	return Actualoca;
}

//转速初始化
void CCommMC6600::Pospeed(float* pnSpeed)
{
	for(int i=0;i<5;i++)
	{
		g_ParamSpeed.fSpeedInit[i] = pnSpeed[i];
		g_ParamSpeed.fSpeed[i] = pnSpeed[i];
	}
}

//位置转速设置
void CCommMC6600::PosSpeed(short anxi, const float* data)
{
	if( !g_bMC6600Open )
		return;
	if( anxi == g_ParamStyle.nAxis )
	{
		BYTE* PosSpeed = new BYTE[8];
		ZeroMemory(PosSpeed,8);
		PosSpeed[0] = g_ParamAddr.byteAddr[anxi];
		PosSpeed[1]= 0x06;
		PosSpeed[2]= 0x00;
		PosSpeed[3]= 0x2C;
		dec2hex(PosSpeed, (int)(g_ParamSpeed.fSpeedInit[anxi]*100));
		CRCheck(PosSpeed);
		CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(PosSpeed);
	}
	for (int h=0;h < 2;h++)
	{
		unsigned short tmp = 0xffff;
		unsigned short ret1 = 0;
		if(!h)
		{
			BYTE* PosSpeed = new BYTE[8];
			ZeroMemory(PosSpeed,8);
			PosSpeed[0] = g_ParamAddr.byteAddr[anxi];
			PosSpeed[1] = 0x06;
			PosSpeed[2] = 0x00;
			char* hexW = new char[8];
			data2hex(*data, hexW);

			CString s,tarhigh;
			s = hexW;
			int nPos1,nPos2,nPos3,nPos4;
			//目标位置IEEE 754换算
			PosSpeed[3] =	0x2C;
			tarhigh = s.Mid(0,2);
			const char* str1=tarhigh.GetBuffer(0);
			sscanf(str1,"%02x",&nPos1);
			PosSpeed[4] = nPos1;

			tarhigh = s.Mid(2,2);
			const char* str2=tarhigh.GetBuffer(0);
			sscanf(str2,"%02x",&nPos2);
			PosSpeed[5] = nPos2;

			//计算CRC校验位
			CRCheck( PosSpeed );
			CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(PosSpeed);
			delete [] hexW;
		}
		else
		{
			BYTE* PosSpeed = new BYTE[8];
			ZeroMemory(PosSpeed,8);
			PosSpeed[0] = g_ParamAddr.byteAddr[anxi];
			PosSpeed[1] = 0x06;
			PosSpeed[2] = 0x00;
			char* hexW = new char[8];
			data2hex(*data, hexW);

			CString s,tarhigh;
			s = hexW;
			int nPos1,nPos2,nPos3,nPos4;
			PosSpeed[3] = 0x2D;
			tarhigh = s.Mid(4,2);
			const char* str3=tarhigh.GetBuffer(0);
			sscanf(str3,"%02x",&nPos3);
			PosSpeed[4] = nPos3;

			tarhigh = s.Mid(6,2);
			const char* str4=tarhigh.GetBuffer(0);
			sscanf(str4,"%02x",&nPos4);
			PosSpeed[5] = nPos4;
			//计算CRC校验位
			CRCheck( PosSpeed );
			CStageMC6600Manager::Instance().m_vecCmdMC6600.push_back(PosSpeed);
			delete [] hexW;
		}
	}
}

//从站地址设置
void CCommMC6600::SlaveAddrSet(BYTE* pbyteAdd, int nLength)
{
	int nLenSendSA= g_MC6600.SendDataEx(pbyteAdd, nLength);
}

//从站地址传递
void CCommMC6600::SlaveAddrSend(BYTE* pbyteAddr)
{
	for(int i=0; i<5; i++)
	{
		g_ParamAddr.byteAddr[i] = pbyteAddr[i];
	}
}

float CCommMC6600::MoveDir(unsigned char ucAxis, float fSteps, BOOL bBacklash )
{
// 	float lsVal = lSteps;
	if( bBacklash )
		CoherenceCheck_Backlash( ucAxis, fSteps, FALSE );

//	float Val;
	const float* cmd;
	cmd = &fSteps;
	SendCmd(cmd,ucAxis);

	return fSteps;
}

void CCommMC6600::MoveLimit(unsigned char ucAxis, float fSteps)
{
	const float* cmd;
	cmd = &fSteps;
	SendCmd(cmd,ucAxis);
}

void CCommMC6600::GoOrigion(unsigned char ucAxis, float fCurPos)
{
	const float* cmd;
	cmd = &fCurPos;
	SendCmd(cmd,ucAxis);
}

void CCommMC6600::GoHome(unsigned char ucAxis, float fSteps )
{
	const float* cmd;
	cmd = &fSteps;
	SendCmd(cmd,ucAxis);
}
void CCommMC6600::SetBacklashParams(short nIndex,short nDir, float fCount) //nDir:方向   nCount：间隙量
{
	// 每次向特定方向移动时，要多移动一个间隙量，然后向反方向走回一个间隙量
	g_ParamBl.nDir[nIndex]		= nDir;
	g_ParamBl.fCount[nIndex]	= fCount;
}
	
void CCommMC6600::Move( unsigned char ucAxis, float fSteps, BOOL bRelative , BOOL bBacklash)
{
	const float* cmd = &fSteps;
	SendCmd(cmd , ucAxis);
}

float CCommMC6600::CoherenceCheck_Backlash( unsigned char ucAxis, float& fSteps, BOOL bRelative )
{
 	short anxi = SerialNum( ucAxis );
	float lThreshold = 0;	// 运行相对位置，只需看相对位置值的正负即可（与0比较）
	if( !bRelative )
		// 运行绝对位置，要先判断相对于当前位置是要向哪个方向运动
		lThreshold = g_ParamCurrent.fSteps[anxi];
	if( fSteps < lThreshold && g_ParamBl.nDir[anxi] < 0 )
	{
		g_bBacklash[anxi] = TRUE;
		fSteps -= g_ParamBl.fCount[anxi];
	}
	else if( fSteps > lThreshold && g_ParamBl.nDir[anxi] > 0 )
	{
		g_bBacklash[anxi] = TRUE;
		fSteps += g_ParamBl.fCount[anxi];
	}
	else
		g_bBacklash[anxi] = FALSE;
	return fSteps;
}
/*
int CCommMC6600::GetDir( unsigned char ucAxis )
{
	int nDir = 0;
	if( ucAxis == 'X' )
		nDir = g_nDirX;
	else if( ucAxis == 'Y' )
		nDir = g_nDirY;
	return nDir;
}
*/
/*void CCommMC6600::LocaInit( unsigned char ucAxis, short anxi)
{
	float Dis;
	float state=0;

	LimRelease(1,ucAxis);
	Sleep(1000);
	GoHome(ucAxis, -100 );

	Sleep(1000);
	do{
		state = StatusRead(ucAxis);
	}while(state ==0);

	state=0;
	close(anxi);

	Reset(anxi);

	Sleep(500);
//	m_StageMC6600.LocaZero(i);
	GoHome(ucAxis, 100 );
	Sleep(1000);
	Enable(anxi);

	Sleep(1000);
	do{
		state = StatusRead(ucAxis);
	}while(state ==0);

	state =0;

	close(anxi);
	ConveretSendDataR(ucAxis);

	Dis = ConveretSendDataR(ucAxis);
	Dis = Dis/2;
	GoHome(ucAxis, Dis );
	Enable(anxi);
	Sleep(100);
	m_nTimerID = SetTimer( NULL,NULL, nStageMC6600_Elapse, NULL );
	float CurP;
	do
	{
		CurP = ConveretSendDataR(ucAxis);
	}while(fabs(Dis - CurP)>0.001);

	close(anxi);

	Reset(anxi);
	Sleep(500);

	LocaZero(anxi);
	Sleep(100);
	Enable(anxi);
	LimRelease(0,ucAxis);

	Sleep(50);
}
*/
void CCommMC6600::SetOldStyle( BOOL bOldStyle, unsigned char ucAxis )
{
	g_ParamStyle.bOld = bOldStyle;
	g_ParamStyle.ucAxis = ucAxis;
	g_ParamStyle.nAxis = SerialNum( ucAxis );
}









