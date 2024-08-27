// CommTMCM.cpp : implementation file
//

#include "stdafx.h"
#include "CommTMCM.h"

#include <afxmt.h>
#include <math.h>

#include <Serial.h>
#include <Comm_TMCM.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Opcodes of all TMCL commands that can be used in direct mode

#define TMCL_MST 3		//ֹͣ����˶�
#define TMCL_MVP 4		//�ƶ���λ��ֵ�����/���ԣ�
#define TMCL_SAP 5		//��������� -�ϵ�ʧЧ
#define TMCL_GAP 6		//��ȡ�����

#define TMCL_SGP 9		//����ȫ�ֲ���
#define TMCL_GGP 10		//��ȡȫ�ֲ���
#define TMCL_STGP 11	//�洢ȫ�ֲ���
#define TMCL_RSGP 12	//�ָ�ȫ�ֲ���
#define TMCL_RFS 13		//�ο�����-״̬��ѯ
#define TMCL_SCO 30		//��������
#define TMCL_GCO 31		//��ȡ����
#define TMCL_CCO 32		//��������

//Opcodes of TMCL control functions (to be used to run or abort a TMCL program in the module)
#define TMCL_APPL_STOP		128
#define TMCL_APPL_RUN		129
#define TMCL_APPL_RESET		131

//Options for MVP commandds
#define MVP_ABS		0
#define MVP_REL		1
#define MVP_COORD	2

//Options for RFS command
#define RFS_START	0
#define RFS_STOP	1
#define RFS_STATUS	2

#define FALSE	0
#define TRUE	1

//Result codes for GetResult
#define TMCL_RESULT_OK				0
#define TMCL_RESULT_NOT_READY		1
#define TMCL_RESULT_CHECKSUM_ERROR	2

#define TMCM_Target_address		1

static CCriticalSection			m_mutexTMCM;

#define	nTMCM_Sleep				50
const UINT	nStageTMCM_Elapse	= 300;

DWORD WINAPI InitThreadTMCM(LPVOID lpParam);
//DWORD WINAPI StatuThreadTMCM(LPVOID lpParam);

HANDLE	g_hInitThreadTMCM;
DWORD	g_dwInitThreadTMCM;
HANDLE	g_hEventWorkTMCM		= CreateEvent(NULL,FALSE, FALSE, _T("WORKTMCM"));
HANDLE	g_hEventTeminateTMCM	= CreateEvent(NULL,FALSE, FALSE, _T("TERMINATETMCM"));
//HANDLE	g_hStatuThreadTMCM;
//DWORD		g_dwstatuThreadTMCM;

HWND	g_hMain;
CSerial	g_TMCM;
BOOL	g_bTMCMOpen		= FALSE;		// �Ƿ�������һ���豸��ͨ
BOOL    g_bTMCMOpenElec[5];
int     g_nAsynCmdFormat;
BOOL    g_bCurPosSetting	= FALSE;	//��ǰλ�û�ȡ-����
UCHAR	g_bUcharbuff[4]		= {0};

TMCM_Params_Current			g_ParamCurrent;
TMCM_Params_Speed			g_ParamSpeed;
TMCM_Params_Bl				g_ParamBl;
TMCM_Params_Elec            g_ParamElec;
TMCM_Params_Style			g_ParamStyle;
TMCM_Params_LimitParam		g_paramLimitParam;
TMCM_Params_LimitStatus		g_paramLimitStatus;

BOOL	g_bBacklash[4];
int		g_nStatus[2] = {1};

void	SendtarPosW(BYTE* SRecvdata, short anxi);
void	Enable(short anxi);
void	PosSpeed(short anxi,const float* data);
//void	BackOff( unsigned char ucAxis );


/////////////////////////////////////////////////////////////////////////////
// CStageTMCMManager
CStageTMCMManager& CStageTMCMManager::Instance()
{
	static CStageTMCMManager Inst;
	return Inst;
}

CStageTMCMManager::CStageTMCMManager()
{
	m_vecCmdTMCM.clear();
	for( int i=0; i<5; i++ )
		m_bIsMoving[i] = TRUE;
}
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// CCommTMCM

CCommTMCM::CCommTMCM()
{
	for( int i=0; i<5; i++ )
	{
		g_ParamSpeed.fSpeedPos[i]			= 0.0;
		g_ParamSpeed.fSpeedAcc[i]			= 0.0;
		g_ParamSpeed.fSpeedDec[i]			= 0.0;
		g_ParamSpeed.fSpeedHome[i]			= 0.0;
		g_ParamCurrent.fSteps[i]			= 0.0;
		g_ParamCurrent.fOrigin[i]			= 0.0;
		g_paramLimitStatus.nL[i]			= 1;
		g_paramLimitStatus.nR[i]			= 1;
		g_paramLimitStatus.nGL				= 0;	
		g_ParamElec.fAxisCurSpeed[i]		= 0.0;
		g_ParamElec.fAxisStandbySpeed[i]	= 0.0;
		g_bTMCMOpenElec[i]					= FALSE;
	}
}

CCommTMCM::~CCommTMCM()
{
}

/////////////////////////////////////////////////////////////////////////////
// CCommTMCM message handlers

//��д����ʵ��
union
{
	unsigned char data[4];	//uint8_t data[4];
	float value;
} fpx;

//�����i��Ӧ
short SerialNum(unsigned char ucMotor)
{
	short motor = 6;
	switch (ucMotor)
	{
	case 'X':
		motor = 0;
		break;
	case 'Y':
		motor = 1;
		break;
    case 'Z':
		motor = 2;
		break;
	case 'R':
		motor = 3;
		break;
	case 'T':
		motor = 4;
		break;
	}
	return motor;
}

//i�������Ӧ
unsigned char SerialNum(int motor)
{
	unsigned char ucMotor;
	switch (motor)
	{
	case 0:
		ucMotor = 'X';
		break;
	case 1:
		ucMotor = 'Y';
		break;
    case 2:
		ucMotor = 'Z';
		break;
	case 3:
		ucMotor = 'R';
		break;
	case 4:
		ucMotor = 'T';
		break;
	}
	return ucMotor;
}

short MotorConversionW(short motor)
{
	short motorval;
	if(motor >3 )
		motorval = 4;
	else
		motorval = motor;
	return motorval;
}

short MotorConversionR(short motor)
{
	short motorval;
	if(motor > 5 )
		motorval = 4;
	else
		motorval = motor;
	return motorval;
}

//ʮ����תʮ������
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

void ConvertStagePos(float Actualoc, short motor)
{
	//motor = MotorConversionR(motor);
	g_ParamCurrent.fSteps[motor] = Actualoc;
}

//ʮ������תʮ����
float ConvertReadData(BYTE* lbuff, short motor)
{
	float location;
	unsigned char highByte, lowByte;
	char p[8]={0};
	//16����ת�ַ���
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

	int Pos;
	sscanf(p, "%08x", &Pos);
	location = Pos;
	//ConvertStagePos(location, motor);
	return location;
}

//�򲽽��������
int ConveretSendDataW(UCHAR Address, UCHAR Command, UCHAR Type, UCHAR Motor, INT Value )
{
	if( !g_bTMCMOpen )
		return -1;
	UCHAR* TxBuffer = new UCHAR[9];
	int i;

	TxBuffer[0] = Address;
	TxBuffer[1] = Command;
	TxBuffer[2] = Type;
	TxBuffer[3] = Motor;
	TxBuffer[4] = Value >> 24;
	TxBuffer[5] = Value >> 16;
	TxBuffer[6] = Value >> 8;
	TxBuffer[7] = Value & 0xff;
	TxBuffer[8] = 0;
	for (i = 0; i < 8; i++)
		TxBuffer[8] += TxBuffer[i];

	CStageTMCMManager::Instance().m_vecCmdTMCM.push_back(TxBuffer);
	return 0;
}

float CCommTMCM::GetCurPosition(short motor )
{
	//motor = MotorConversionW(motor);
	ConveretSendDataW(TMCM_Target_address,TMCL_GAP,1,motor,0);
	//motor = MotorConversionR(motor);
	float fSteps = g_ParamCurrent.fSteps[motor];
	return fSteps;
}

void CCommTMCM::CurrPosR(short motor)
{
	//motor = MotorConversionW(motor);
	float fVal;
	fVal = ConveretSendDataW(TMCM_Target_address,TMCL_GAP,1,motor,0);
	Sleep(200);
	ConveretSendDataW(TMCM_Target_address,TMCL_MVP,0,motor,fVal);
}

//�ӵ�״̬��ѯ
int getCurGndStatus()
{
	g_paramLimitStatus.nGL = 0;
	UCHAR GBuffer[9],GBuf[4]={0};
	BYTE Recvdata[200] = {0};
	float Actualoca = 1;
	int i;

	GBuffer[0] = TMCM_Target_address;
	GBuffer[1] = TMCL_GAP;
	GBuffer[2] = 11;
	GBuffer[3] = 5;
	GBuffer[4] = 0 >> 24;
	GBuffer[5] = 0 >> 16;
	GBuffer[6] = 0 >> 8;
	GBuffer[7] = 0 & 0xff;
	GBuffer[8] = 0;
	for (i = 0; i < 8; i++)
		GBuffer[8] += GBuffer[i];

	int nlenr2 = sizeof(GBuffer);
	int nLenSend= g_TMCM.SendDataEx(GBuffer, nlenr2);
	Sleep(nTMCM_Sleep);
	int nlenRecv = g_TMCM.ReadData((BYTE*)Recvdata, 200);
	//��Ҫ��ȡValue����ת��Ϊʮ����
	GBuf[0] = Recvdata[4]; GBuf[1] = Recvdata[5]; 
	GBuf[2] = Recvdata[6]; GBuf[3] = Recvdata[7];
	BYTE* Temlocation;
	Temlocation = GBuf;
	int nGndStatus = ConvertReadData(Temlocation,5);
	g_paramLimitStatus.nGL = nGndStatus;
	return nGndStatus;
}

void CCommTMCM::GroundStatusRead()
{
	g_paramLimitStatus.nGL = 0;
	ConveretSendDataW(TMCM_Target_address,TMCL_GAP,11,5,0);
}

int* getCurLimitStatus(short motor)
{
	ConveretSendDataW(TMCM_Target_address,TMCL_GAP,11,motor,0);
	ConveretSendDataW(TMCM_Target_address,TMCL_GAP,10,motor,0);
	g_nStatus[0] = g_paramLimitStatus.nL[motor];
	g_nStatus[1] = g_paramLimitStatus.nR[motor];
	return g_nStatus;
}

//��λ״̬��ȡ
void CCommTMCM::LimitStatusRead( short motor)
{
	ConveretSendDataW(TMCM_Target_address,TMCL_GAP,11,motor,0);
	ConveretSendDataW(TMCM_Target_address,TMCL_GAP,10,motor,0);
}

int* statusReadInit(short motor)
{
	UCHAR SBuffer[9] = {0x00},SBuf[4]={0x00};
	BYTE Recvdata[200] = {0x00};
	float Actualoca = 1;
	int i;

	SBuffer[0] = TMCM_Target_address;
	SBuffer[1] = TMCL_GAP;
	SBuffer[2] = 11;
	SBuffer[3] = motor;
	SBuffer[4] = 0 >> 24;
	SBuffer[5] = 0 >> 16;
	SBuffer[6] = 0 >> 8;
	SBuffer[7] = 0 & 0xff;
	SBuffer[8] = 0;
	for (i = 0; i < 8; i++)
		SBuffer[8] += SBuffer[i];

	int nlenr2 = sizeof(SBuffer);
	int nLenSend= g_TMCM.SendDataEx(SBuffer, nlenr2);
	Sleep(nTMCM_Sleep);
	int nlenRecv = g_TMCM.ReadData((BYTE*)Recvdata, 200);
	//��Ҫ��ȡValue����ת��Ϊʮ����
	SBuf[0] = Recvdata[4]; SBuf[1] = Recvdata[5]; 
	SBuf[2] = Recvdata[6]; SBuf[3] = Recvdata[7];
	BYTE* Temlocation;
	Temlocation = SBuf;
	g_nStatus[0] = ConvertReadData(Temlocation,motor);
	g_paramLimitStatus.nL[motor] = g_nStatus[0];

	Sleep(nTMCM_Sleep);

	SBuffer[2] = 10;
	SBuffer[8] = 0;
	for (i = 0; i < 8; i++)
		SBuffer[8] += SBuffer[i];
	nlenr2 = sizeof(SBuffer);
	nLenSend= g_TMCM.SendDataEx(SBuffer, nlenr2);
	Sleep(nTMCM_Sleep);
	nlenRecv = g_TMCM.ReadData((BYTE*)Recvdata, 200);
	//��Ҫ��ȡValue����ת��Ϊʮ����
	SBuf[0] = Recvdata[4]; SBuf[1] = Recvdata[5]; 
	SBuf[2] = Recvdata[6]; SBuf[3] = Recvdata[7];

	Temlocation = NULL;
	Temlocation = SBuf;
	g_nStatus[1] = ConvertReadData(Temlocation,motor);
	g_paramLimitStatus.nR[motor] = g_nStatus[1];

	return g_nStatus;
}

//ɨ���ʼλ�ö�ȡ
float CCommTMCM::CurrPosO(short motor)
{
	//motor = MotorConversionW(motor);
	ConveretSendDataW(TMCM_Target_address,TMCL_GAP,1,motor,0);
	//motor = MotorConversionR(motor);
	float fSteps = g_ParamCurrent.fSteps[motor];
	return fSteps;
}

void Version(int motor)
{
	//motor = MotorConversionW(motor);
	UCHAR VBuffer[9]={0x00};
	BYTE Recvdata[200] = {0};

	VBuffer[0] = TMCM_Target_address;
	VBuffer[1] = 136;
	VBuffer[2] = 1;
	VBuffer[3] = motor;
	VBuffer[4] = 0 >> 24;
	VBuffer[5] = 0 >> 16;
	VBuffer[6] = 0 >> 8;
	VBuffer[7] = 0 & 0xff;
	VBuffer[8] = 0;
	for (int i = 0; i < 8; i++)
		VBuffer[8] += VBuffer[i];
	int nlenr2 = sizeof(VBuffer);
	int nLenSend= g_TMCM.SendDataEx(VBuffer, nlenr2);
	Sleep(nTMCM_Sleep);
	int nReadValue = g_TMCM.ReadData((BYTE*)Recvdata, 200);
	//int nLenth = ConveretSendDataW(TMCM_Target_address,136,1,motor,0);
	if( nReadValue)
		g_bTMCMOpenElec[motor] = TRUE;
	else
		g_bTMCMOpenElec[motor] = FALSE;
}

//λ������
void CCommTMCM::Reset(short motor)
{
	//motor = MotorConversionW(motor);
	ConveretSendDataW(TMCM_Target_address,TMCL_SAP,1,motor,0);
}

void CCommTMCM::LocaZero(short motor)
{
	//motor = MotorConversionW(motor);
	ConveretSendDataW(TMCM_Target_address,TMCL_MVP,0,motor,0);
}

void CCommTMCM::Stop(short motor)
{
	//motor = MotorConversionW(motor);
	ConveretSendDataW(TMCM_Target_address,TMCL_MST,0,motor,0);
}

//���˲���
float CCommTMCM::BackOff( unsigned char ucMotor, float LimDirID )
{
	short motor;
	motor = SerialNum(ucMotor);
	float fStep = GetCurPosition(motor);

	if(LimDirID ==1)
	{
		switch (ucMotor)
		{
			case   'X':
				fStep = fStep-3200;	//1mm
				break;
			case 'Y':
				fStep = fStep-3200;	//1mm
				break;
			case 'Z':
				fStep = fStep-1600;	//0.5mm
				break;
			case 'R':
				fStep = fStep-1600;	//1��
				break;
			case 'T':
				fStep = fStep-1600;//1��
				break;
		}
	}	
	else
	{
		switch (ucMotor)
		{
			case   'X':
				fStep = fStep+3200;	//1mm
				break;
			case 'Y':
				fStep = fStep+3200;	//1mm
				break;
			case 'Z':
				fStep = fStep+1600;	//0.5mm
				break;
			case 'R':
				fStep = fStep+1600;	//1��
				break;
			case 'T':
				fStep = fStep+1600;//1��
				break;
		}
	}
	//motor = MotorConversionW(motor);
	const float* cmd;
	cmd = &fStep;
	SendCmd(cmd,motor);

	return fStep;
}

void CCommTMCM::SetSpeed( unsigned char ucMotor, float fSpeed )
{
	const float* cmd;
	cmd  = &fSpeed;
	short motor = SerialNum(ucMotor);
	//motor = MotorConversionW(motor);
	ConveretSendDataW(TMCM_Target_address,TMCL_SAP,4,motor,fSpeed);
}

//��λ״̬-��/����
void CCommTMCM::LimitStatus( short ID,short motor,int nable )
{
	//motor = MotorConversionW(motor);
	switch(ID)
	{
	case 12://����λ
		ConveretSendDataW(TMCM_Target_address,TMCL_SAP,12,motor,nable);
		g_paramLimitParam.nRightLimit[motor] = nable;
		break;
	case 13://����λ
		ConveretSendDataW(TMCM_Target_address,TMCL_SAP,13,motor,nable);	  
		g_paramLimitParam.nLeftLimit[motor] = nable;
		break;
	}
}

//�ߵ͵�ƽ����
void CCommTMCM::LimitLevel( short ID,short motor,int nLimitLevel )
{
	switch(ID)
	{
	case 24://����λ
		ConveretSendDataW(TMCM_Target_address,TMCL_SAP,24,motor,nLimitLevel);
		g_paramLimitParam.nRightLimit[motor] = nLimitLevel;
		break;
	case 25://����λ
		ConveretSendDataW(TMCM_Target_address,TMCL_SAP,25,motor,nLimitLevel);	  
		g_paramLimitParam.nLeftLimit[motor] = nLimitLevel;
		break;
	}
}

//��λ״̬-����
void CCommTMCM::LimitExchange( short motor,BOOL bExchangeLimit)
{
	//motor = MotorConversionW(motor);
	ConveretSendDataW(TMCM_Target_address,TMCL_SAP,14,motor,bExchangeLimit);
	g_paramLimitParam.nExchangeLimit[motor] = bExchangeLimit;
}

//�����·������ݶ�ȡ
int CCommTMCM::SendCmd( const float* cmd , short motor )
{
	//motor = MotorConversionW(motor);
	int Val =*cmd;
	ConveretSendDataW(TMCM_Target_address,TMCL_MVP,0,motor,Val);
	return 0;
}

//����ϵ�/�ϵ�
void CCommTMCM::PowerOfforOn(short ID ,short motor)
{
	switch(ID)
	{
	case 0: //����ϵ�
		ConveretSendDataW(TMCM_Target_address,TMCL_SAP,167,motor,3);
		break;
	case 1://����ϵ�
		ConveretSendDataW(TMCM_Target_address,TMCL_SAP,167,motor,0);
		break;
	}
}

BOOL allZero()
{
	int total = 0;
	for(int i=0;i<5;i++)
		if(g_bTMCMOpenElec[i] == 0)
			total++;
	if(total == 5)	
		return TRUE;
	return FALSE;
}

//��ǰλ�ö�ȡ
float ConveretSendDataR(UCHAR Address, UCHAR Command, UCHAR Type, UCHAR Motor, INT Value)
{
	UCHAR TxBuffer[9],TxBuf[4]={0};
	BYTE Recvdata[200] = {0};
	float Actualoca = 1;
	int i;

	TxBuffer[0] = Address;
	TxBuffer[1] = Command;
	TxBuffer[2] = Type;
	TxBuffer[3] = Motor;
	TxBuffer[4] = Value >> 24;
	TxBuffer[5] = Value >> 16;
	TxBuffer[6] = Value >> 8;
	TxBuffer[7] = Value & 0xff;
	TxBuffer[8] = 0;
	for (i = 0; i < 8; i++)
		TxBuffer[8] += TxBuffer[i];

	int nlenr2 = sizeof(TxBuffer);
	int nLenSend= g_TMCM.SendDataEx(TxBuffer, nlenr2);
	Sleep(nTMCM_Sleep);
	int nlenRecv = g_TMCM.ReadData((BYTE*)Recvdata, 200);
	//��Ҫ��ȡValue����ת��Ϊʮ����
	if( Recvdata[2] == 0x04)
		return Actualoca;
	else
	{
		TxBuf[0] = Recvdata[4]; TxBuf[1] = Recvdata[5]; 
		TxBuf[2] = Recvdata[6]; TxBuf[3] = Recvdata[7];
		BYTE* Temlocation;
		Temlocation = TxBuf;
		Actualoca = ConvertReadData(Temlocation,Motor);
		if(TxBuffer[1] == 0x06 && TxBuffer[2] == 0x01)
			ConvertStagePos(Actualoca, Motor);
		return Actualoca;
	}
}

float getCurPosition(UCHAR Address, UCHAR Command, UCHAR Type, UCHAR Motor, INT Value)
{
	float fSteps = ConveretSendDataR(Address,Command,Type,Motor,Value);
	//ConvertStagePos(fSteps, Motor);
	return fSteps;
}

void CommWithBoxTMCM(int nSleep)
{
	short i =0;
	for(i = 0;i<5;i++)
	{
		if(CStageTMCMManager::Instance().m_bIsMoving[i])
		{
			getCurPosition(TMCM_Target_address,TMCL_GAP,1,i,0);
			Sleep(50);
			statusReadInit(i);
			Sleep(50);

		}
	}
	//�ӵ�״̬��ѯ
	getCurGndStatus();
}

//��ʼ���������������õ�
void InitTMCMParams( int nSleep )
{
	for(int i=0;i<5;i++)
	{
		//short motor = MotorConversionW(i);
		short motor = i;
		//���е���
		ConveretSendDataR(TMCM_Target_address,TMCL_SAP,6,motor,g_ParamElec.fAxisCurSpeed[i]);
		//��������
		ConveretSendDataR(TMCM_Target_address,TMCL_SAP,7,motor,g_ParamElec.fAxisStandbySpeed[i]);
		//��λ�ٶ�
		ConveretSendDataR(TMCM_Target_address,TMCL_SAP,4,motor,g_ParamSpeed.fSpeedPos[i]);
		//���ٶ�
		ConveretSendDataR(TMCM_Target_address,TMCL_SAP,5,motor,g_ParamSpeed.fSpeedAcc[i]);
		//���ٶ�
		ConveretSendDataR(TMCM_Target_address,TMCL_SAP,17,motor,g_ParamSpeed.fSpeedDec[i]);
		//ϸ��
		ConveretSendDataR(TMCM_Target_address,TMCL_SAP,140,motor,5);

		if(g_ParamStyle.nVersion<1)
		{
			if(i<2 )
			{
				//��λ����
				ConveretSendDataR(TMCM_Target_address,TMCL_SAP,12,motor,0);
				ConveretSendDataR(TMCM_Target_address,TMCL_SAP,13,motor,0);
				//��λ����
				ConveretSendDataR(TMCM_Target_address,TMCL_SAP,14,motor,1);
			}
			else if(i<3)
			{
				//��λ����
				ConveretSendDataR(TMCM_Target_address,TMCL_SAP,12,motor,0);
				ConveretSendDataR(TMCM_Target_address,TMCL_SAP,13,motor,0);
			}
			else if(i>3)
			{
				//����λ���ã�����λ����
				ConveretSendDataR(TMCM_Target_address,TMCL_SAP,12,motor,0);
				ConveretSendDataR(TMCM_Target_address,TMCL_SAP,13,motor,1);
			}
			else
			{
				ConveretSendDataR(TMCM_Target_address,TMCL_SAP,12,motor,1);
				ConveretSendDataR(TMCM_Target_address,TMCL_SAP,13,motor,1);	 
			}
		}
		else
		{
			//if(i<3 )
			//{
			//	//��λ����
			//	ConveretSendDataW(TMCM_Target_address,TMCL_SAP,14,motor,g_paramLimitParam.nExchangeLimit[i]);
			//	//��ֹͣ������ֹͣ����
			//	//ConveretSendDataW(TMCM_Target_address,TMCL_SAP,26,motor,g_paramLimitParam.nExchangeLimit[i]);
			//}
			//else if(i>3)
			//{
			//	//�ߵ͵�ƽ����
			//	ConveretSendDataW(TMCM_Target_address,TMCL_SAP,24,motor,1);
			//	ConveretSendDataW(TMCM_Target_address,TMCL_SAP,25,motor,1);
			//	Sleep(nTMCM_Sleep);
			//}
			////��λ��/����
			//ConveretSendDataW(TMCM_Target_address,TMCL_SAP,12,motor,g_paramLimitParam.nRightLimit[i]);
			//ConveretSendDataW(TMCM_Target_address,TMCL_SAP,13,motor,g_paramLimitParam.nLeftLimit[i]);

			//20240116��HY
			//��λ��/����
			ConveretSendDataR(TMCM_Target_address,TMCL_SAP,12,motor,g_paramLimitParam.nRightLimit[i]);
			ConveretSendDataR(TMCM_Target_address,TMCL_SAP,13,motor,g_paramLimitParam.nLeftLimit[i]);

			//��λ����
			if(g_paramLimitParam.nExchangeLimit[i])
				ConveretSendDataR(TMCM_Target_address,TMCL_SAP,14,motor,g_paramLimitParam.nExchangeLimit[i]);

			//�ߵ͵�ƽ����-��
			if(g_paramLimitParam.nLeftLevelReversal[i])
				ConveretSendDataR(TMCM_Target_address,TMCL_SAP,25,motor,g_paramLimitParam.nLeftLevelReversal[i]);
			//�ߵ͵�ƽ����-��
			if(g_paramLimitParam.nRightLevelReversal[i])
				ConveretSendDataR(TMCM_Target_address,TMCL_SAP,24,motor,g_paramLimitParam.nRightLevelReversal[i]);
		}
	}
	////�ӵظߵ͵�ƽ����
	//ConveretSendDataW(TMCM_Target_address,TMCL_SAP,24,5,1);
	//ConveretSendDataW(TMCM_Target_address,TMCL_SAP,25,5,1);
	Sleep(nTMCM_Sleep);
	 //�ӵ���λ����
	ConveretSendDataR(TMCM_Target_address,TMCL_SAP,12,5,0);
	ConveretSendDataR(TMCM_Target_address,TMCL_SAP,13,5,0);
}

DWORD WINAPI InitThreadTMCM(LPVOID lpParam )
{
	//�򿪶˿�
	g_bTMCMOpen = g_TMCM.Open( (short)lpParam,9600 );

	if(g_bTMCMOpen)
	{
		//ͨѶ�Ƿ�����
		for(int i=0;i<5;i++)
			Version(i);
		if(allZero())
		{
			g_bTMCMOpen = FALSE;
			PostMessage( g_hMain, WM_USER_TMCM_UpdateValue, 7, (LPARAM)(&g_bTMCMOpenElec) );
			PostMessage( g_hMain, WM_USER_TMCM_UpdateValue, 0, g_bTMCMOpen );
		}
		else
		{
			InitTMCMParams(200);
			PostMessage( g_hMain, WM_USER_TMCM_UpdateValue, 7, (LPARAM)(&g_bTMCMOpenElec) );
			PostMessage( g_hMain, WM_USER_TMCM_UpdateValue, 0, g_bTMCMOpen );
			PostMessage( g_hMain, WM_USER_TMCM_UpdateValue, 6, (LPARAM)(&g_ParamCurrent) );
		}
	}
	else
	{
		PostMessage( g_hMain, WM_USER_TMCM_UpdateValue, 0, g_bTMCMOpen );
		PostMessage( g_hMain, WM_USER_TMCM_UpdateValue, 7, (LPARAM)(&g_bTMCMOpenElec) );
	}
	while(g_bTMCMOpen && WaitForSingleObject(g_hEventTeminateTMCM,300) != WAIT_OBJECT_0)
	{
		CommWithBoxTMCM(nTMCM_Sleep);
		PostMessage( g_hMain, WM_USER_TMCM_UpdateValue, 12, (LPARAM)(&g_paramLimitStatus) );
		PostMessage( g_hMain, WM_USER_TMCM_UpdateValue, 11, (LPARAM)(&g_ParamCurrent) );
		for(int i = 0;i < CStageTMCMManager::Instance().m_vecCmdTMCM.size();i++)
		{
			UCHAR* pCmd = CStageTMCMManager::Instance().m_vecCmdTMCM[i];
			UCHAR Reset[9] = {0x00};
			memcpy(Reset,pCmd,9);
			delete pCmd;
			pCmd = NULL;
			BYTE Recvdata[200] = {0};
			int nlenre = sizeof(Reset);
			int nLenSendE = g_TMCM.SendDataEx(Reset, nlenre);
			Sleep(nTMCM_Sleep);
			int nlenRecv = g_TMCM.ReadData((BYTE*)Recvdata, 200);
			//�ж��·��Ƿ�ɹ�
			while(!nlenRecv)
			{
				nLenSendE = g_TMCM.SendDataEx(Reset,nlenre);
				Sleep(nTMCM_Sleep);
				nlenRecv = g_TMCM.ReadData((BYTE*)Recvdata,200);
			}
			////����·�����
			//fout.open(file,ios::out|ios::app);
			//fout << "�·�����" << ":";
			//for(int i=0;i<9;i++)
			//	fout << hex << ((Reset[i] & 0xf0)>>4) << ((Reset[i] & 0x0f)) << "";
			//fout << endl;
			//for(int i=0;i<9;i++)
			//	fout << hex << ((Recvdata[i] & 0xf0)>>4) << ((Recvdata[i] & 0x0f)) << "";
			//fout << "-" << nlenRecv << endl;
			//fout.close();
			//�ӵ�
			switch(Reset[3])
			{
				BYTE* Temlocation;
			case 0x05:
				{
					g_bUcharbuff[0] = Recvdata[4]; g_bUcharbuff[1] = Recvdata[5]; 
					g_bUcharbuff[2] = Recvdata[6]; g_bUcharbuff[3] = Recvdata[7];
					Temlocation = g_bUcharbuff;
					g_paramLimitStatus.nGL = ConvertReadData(Temlocation,5);
					break;
				}
			case 0x11:
				{
					short motor = Reset[3];
					g_bUcharbuff[0] = Recvdata[4]; g_bUcharbuff[1] = Recvdata[5]; 
					g_bUcharbuff[2] = Recvdata[6]; g_bUcharbuff[3] = Recvdata[7];
					Temlocation = g_bUcharbuff;
					g_paramLimitStatus.nL[motor] = ConvertReadData(Temlocation,motor);
					break;
				}
			case 0x10:
				{
					short motor = Reset[3];
					g_bUcharbuff[0] = Recvdata[4]; g_bUcharbuff[1] = Recvdata[5]; 
					g_bUcharbuff[2] = Recvdata[6]; g_bUcharbuff[3] = Recvdata[7];
					Temlocation = g_bUcharbuff;
					g_paramLimitStatus.nR[motor] = ConvertReadData(Temlocation,motor);
					break;
				}
			}
			PostMessage( g_hMain, WM_USER_TMCM_UpdateValue, 12, (LPARAM)(&g_paramLimitStatus) ); 
		}
		CStageTMCMManager::Instance().m_vecCmdTMCM.clear();
		Sleep( 1 );
	}
	g_TMCM.Close();
	g_bTMCMOpen = g_TMCM.IsOpened();
	SetEvent(g_hEventWorkTMCM);
// 	g_TMCM.Close();
	return 0;
}

void CCommTMCM::InitHWND( HWND hWnd )
{
	g_hMain = hWnd;
}

BOOL CCommTMCM::Open( short port )
{
	ResetEvent(g_hEventWorkTMCM);
	ResetEvent(g_hEventTeminateTMCM);
	g_hInitThreadTMCM = CreateThread(NULL, 0, InitThreadTMCM, (LPVOID)(port), 0, &g_dwInitThreadTMCM);
	if( g_hInitThreadTMCM == NULL )
		return FALSE;

	CloseHandle(g_hInitThreadTMCM);
/*	char m = 'X';
	g_hStatuThreadTMCM = CreateThread(NULL,0,StatuThreadTMCM,(LPVOID)(m),0,&g_dwstatuThreadTMCM);
	if( g_hStatuThreadTMCM == NULL )
		return FALSE;
	CloseHandle(g_hStatuThreadTMCM);
*/	return TRUE;
}

BOOL CCommTMCM::Close()
{
	if( g_bTMCMOpen )
	{
		SetEvent( g_hEventTeminateTMCM );
		WaitForSingleObject( g_hEventWorkTMCM, INFINITE );
		g_bTMCMOpen	= FALSE;
	}

	return TRUE;
}

//ת�ٳ�ʼ��
void CCommTMCM::Pospeed(float* fPosSpeed,float* fAccSpeed, float* fDecSpeed)
{
	for(int i=0;i<5;i++)
	{
		g_ParamSpeed.fSpeedPos[i] = fPosSpeed[i];
		g_ParamSpeed.fSpeedAcc[i] = fAccSpeed[i];
		g_ParamSpeed.fSpeedDec[i] = fDecSpeed[i];
	}
}

//��λ��ʼ��
void CCommTMCM::LimitParam(int* nLeftLimit,int* nRightParam, int* nExchangeLimit)
{
	for(int i=0;i<5;i++)
	{
		g_paramLimitParam.nLeftLimit[i] = nLeftLimit[i];
		g_paramLimitParam.nRightLimit[i] = nRightParam[i];
		g_paramLimitParam.nExchangeLimit[i] = nExchangeLimit[i];
	}
}

//��ƽ��ʼ��
void CCommTMCM::LimitLevelParam(int* nLeftLimitLevel,int* nRightLimitLevel)
{
	for(int i=0;i<5;i++)
	{
		g_paramLimitParam.nLeftLevelReversal[i] = nLeftLimitLevel[i];
		g_paramLimitParam.nRightLevelReversal[i] = nRightLimitLevel[i];
	}
}

//����������ʼ��
void CCommTMCM::Current(float* fCurSpeed,float* fStandbySpeed)
{
	for(int i=0;i<5;i++)
	{
		g_ParamElec.fAxisCurSpeed[i] = fCurSpeed[i];
		g_ParamElec.fAxisStandbySpeed[i] = fStandbySpeed[i];
	}
}

float CCommTMCM::MoveDir(short motor, float fSteps, BOOL bBacklash )
{
// 	float lsVal = lSteps;
	if( bBacklash )
		CoherenceCheck_Backlash( motor, fSteps, FALSE );

//	float Val;
	const float* cmd;
	cmd = &fSteps;

	//motor = MotorConversionW(motor);

	SendCmd(cmd,motor);

	return fSteps;
}

void CCommTMCM::MoveLimit(short motor, float fSteps)
{
	//motor = MotorConversionW(motor);
	const float* cmd;
	cmd = &fSteps;
	SendCmd(cmd,motor);
}

void CCommTMCM::GoOrigion(short motor, float fCurPos)
{
	//motor = MotorConversionW(motor);
	const float* cmd;
	cmd = &fCurPos;
	SendCmd(cmd,motor);
}

void CCommTMCM::GoHome(short motor, float fSteps )
{
	//motor = MotorConversionW(motor);
	const float* cmd;
	cmd = &fSteps;
	SendCmd(cmd,motor);
}

void CCommTMCM::SetBacklashParams(short nIndex,short nDir, float fCount) //nDir:����   nCount����϶��
{
	// ÿ�����ض������ƶ�ʱ��Ҫ���ƶ�һ����϶����Ȼ���򷴷����߻�һ����϶��
	g_ParamBl.nDir[nIndex]		= nDir;
	g_ParamBl.fCount[nIndex]	= fCount;
}
	
void CCommTMCM::Move(  short motor, float fSteps, BOOL bRelative , BOOL bBacklash)
{
	//motor = MotorConversionW(motor);
	const float* cmd = &fSteps;
	SendCmd(cmd , motor);
}

float CCommTMCM::CoherenceCheck_Backlash( short motor, float& fSteps, BOOL bRelative )
{
	float lThreshold = 0;	// �������λ�ã�ֻ�迴���λ��ֵ���������ɣ���0�Ƚϣ�
	if( !bRelative )
		// ���о���λ�ã�Ҫ���ж�����ڵ�ǰλ����Ҫ���ĸ������˶�
		lThreshold = g_ParamCurrent.fSteps[motor];
	if( fSteps < lThreshold && g_ParamBl.nDir[motor] < 0 )
	{
		g_bBacklash[motor] = TRUE;
		fSteps -= g_ParamBl.fCount[motor];
	}
	else if( fSteps > lThreshold && g_ParamBl.nDir[motor] > 0 )
	{
		g_bBacklash[motor] = TRUE;
		fSteps += g_ParamBl.fCount[motor];
	}
	else
		g_bBacklash[motor] = FALSE;
	return fSteps;
}

void CCommTMCM::SetVersion( int nVersion )
{
	g_ParamStyle.nVersion = nVersion;
}
