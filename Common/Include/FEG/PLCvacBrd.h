
#ifndef __PLCvacBrd_H
#define __PLCvacBrd_H

#define WM_USER_PLCvacBrd_SendMsg		(WM_USER	+110)
#define WM_USER_PLCvacBrd_RecvMsg		(WM_USER	+111)
#define WM_USER_PLCvacBrd_UpdateStatus	(WM_USER	+112)

#define PLCvacBoard_PortPLC		2000
#define PLCvacBoard_PortPC		5000

typedef struct tagCfg_PLCvacBrd{
	double			dThreshold_PreVac;	// ��ֵ1
	double			dThreshold_VacOK;	// ��ֵ2
	double			dThreshold_VacNotOK;// ��ֵ3

	BOOL			bCreate;	// socket����
	BOOL			bConnect;	// socket����
	BOOL			bReady;		// ͨѶ��������
} Cfg_PLCvacBrd;


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

const	BYTE	byte_PLCvacBRDHead			= 0x25;
const	BYTE	byte_PLCvacBRDTail			= 0x0D;
const	BYTE	byte_PLCvacBRDLenSend1		= 0x04;	// ������
const	BYTE	byte_PLCvacBRDLenSend2		= 0x06;	// ��ֵ
const	BYTE	byte_PLCvacBRDLenRecv1		= 0x04;	// ������
const	BYTE	byte_PLCvacBRDLenRecv2		= 0x09;	// ��ֵ

const	BYTE	byte_PLCvacBoard_Offset		= 0x20;

/////////////////////////////////////////////////////////////////////
// 1. PLC��PC��ͨѶЭ��֡��ʽ
//	֡ͷByte1	��������Byte2	������Byte3 ~ Byte(N-1)		֡βByte N
//	25H			opcXXX			XX...XX						0DH

// 2. PC����PLC���������ơ���Ӧֵ�Ͳ�����
const	BYTE	byte_PLCvacBRDopr_Default				= 0x01;		// oprand Ĭ��ֵ
const	BYTE	byte_PLCvacBRDopr_Open					= 0x01;		// oprand = 0x01������
const	BYTE	byte_PLCvacBRDopr_Close					= 0x02;		// oprand = 0x02���ر�

const	BYTE	byte_PLCvacBRDopc_SwitchMode			= 0x01;		// �л��ֶ�/�Զ�ģʽ
		const	BYTE	byte_PLCvacBRDopr_ModeManual	= 0x01;		// oprand = 0x01���ֶ�
		const	BYTE	byte_PLCvacBRDopr_ModeAuto		= 0x02;		// oprand = 0x02���Զ���Ĭ�ϣ�

const	BYTE	byte_PLCvacBRDopc_PumpVent_SEM			= 0x02;		// ��˿ǹ����/����
		const	BYTE	byte_PLCvacBRDopr_SEMpvPump		= 0x01;		// oprand = 0x01������
		const	BYTE	byte_PLCvacBRDopr_SEMpvVentChamb= 0x02;		// oprand = 0x02����Ʒ�ҷ���
		const	BYTE	byte_PLCvacBRDopr_SEMpvVentGun	= 0x03;		// oprand = 0x03��ǹ�ҷ���
		const	BYTE	byte_PLCvacBRDopr_SEMpvSwitch	= 0x06;		// oprand = 0x06���л��ٵ�˿
		const	BYTE	byte_PLCvacBRDopr_SEMpvStandby	= 0x07;		// oprand = 0x07������

const	BYTE	byte_PLCvacBRDopc_GaugeType				= 0x03;		// ��ռ�����
		const	BYTE	byte_PLCvacBRDopr_GaugeInficon	= 0x01;		// oprand = 0x01��INFICON MPG400
		const	BYTE	byte_PLCvacBRDopr_GaugeWPC		= 0x02;		// oprand = 0x02����׼ WPC400

const	BYTE	byte_PLCvacBRDopc_PumpVent_FEM			= 0x04;		// ��ǹ����/����
		const	BYTE	byte_PLCvacBRDopr_FEMpvPump		= 0x01;		// oprand = 0x01������
		const	BYTE	byte_PLCvacBRDopr_FEMpvVent		= 0x02;		// oprand = 0x02����Ʒ�ҷ���
		const	BYTE	byte_PLCvacBRDopr_FEMpvStandby	= 0x07;		// oprand = 0x07������
		const	BYTE	byte_PLCvacBRDopr_FEMpvSwitch	= 0x08;		// oprand = 0x08���л���ǹ

const	BYTE	byte_PLCvacBRDopc_V1					= 0x05;		// �Զ�ģʽ����V1��
const	BYTE	byte_PLCvacBRDopc_CCD					= 0x06;		// ����CCD��Դ

const	BYTE	byte_PLCvacBRDopc_SetThreshold			= 0x07;		// ������ֵ��������������Ӧ�ı������㣩
		const	BYTE	byte_PLCvacBRDopr_VacPre		= 0x01;		// oprand = 0x01 XXXX��PreVac	*1E+3
		const	BYTE	byte_PLCvacBRDopr_VacOK			= 0x02;		// oprand = 0x02 XXXX��VacOK	*1E+7
		const	BYTE	byte_PLCvacBRDopr_VacNotOK		= 0x03;		// oprand = 0x03 XXXX��VacNotOK	*1E+7

const	BYTE	byte_PLCvacBRDopc_SetVentInterval		= 0x08;		// ���÷��������ʱ�䣨���ã�
const	BYTE	byte_PLCvacBRDopc_WarnReset				= 0x09;		// ������λ���oprand = 0x01��������λ

// �ֶ�ģʽ
const	BYTE	byte_PLCvacBRDopc_Manual_V1				= 0x11;		// �ֶ�ģʽ����V1��
const	BYTE	byte_PLCvacBRDopc_Manual_V2				= 0x12;		// �ֶ�ģʽ����V2��
const	BYTE	byte_PLCvacBRDopc_Manual_V3				= 0x13;		// �ֶ�ģʽ����V3��
const	BYTE	byte_PLCvacBRDopc_Manual_V4				= 0x14;		// �ֶ�ģʽ����V4��
const	BYTE	byte_PLCvacBRDopc_Manual_TMP			= 0x15;		// �ֶ�ģʽ����TMP
const	BYTE	byte_PLCvacBRDopc_Manual_MP				= 0x16;		// �ֶ�ģʽ����MP

const	BYTE	byte_PLCvacBRDopc_NAV					= 0x19;		// ����������NAV
const	BYTE	byte_PLCvacBRDopc_BSE					= 0x1D;		// ����BSE��oprand = 0x01�����룻oprand = 0x02���Ƴ�
const	BYTE	byte_PLCvacBRDopc_BeamDetect			= 0x1F;		// ���󿪹������������

// ��ȡ
const	BYTE	byte_PLCvacBRDopc_InquireVACstatus		= 0x21;	// ��ȡ���״̬
const	BYTE	byte_PLCvacBRDopc_InquireVACvalve		= 0x22;	// ��ȡ����״̬
const	BYTE	byte_PLCvacBRDopc_InquireVACthreshold	= 0x27;	// ��ȡ��ǰ��ֵ

const	BYTE	byte_PLCvacBRDopc_InquireCCG			= 0x31;	// ��ȡCCGֵ
const	BYTE	byte_PLCvacBRDopc_InquireTMPr			= 0x32;	// ��ȡ���ӱ�ת��rotational speed
const	BYTE	byte_PLCvacBRDopc_InquireTMPd			= 0x33;	// ��ȡ���ӱ��¶�degrees
const	BYTE	byte_PLCvacBRDopc_InquireTMPt			= 0x34;	// ��ȡ���ӱ�����ʱ��time

const	BYTE	byte_PLCvacBRDopc_InquireStatus			= 0x51;	// ��ȡ����״ֵ̬


/////////////////////////////////////////////////////////////////////
// 3. PLC����PC����Ӧ

// �л���/�Զ�ģʽ����			0x01	0x0X���յ����Xֵͬ����
// 6000��Ʒ�ҳ����/��������	0x02	0x0X���յ����Xֵͬ����
// Standby						0x03	����
// 8000�����/��������			0x04	0x0X���յ����Xֵͬ����
// ����V1������					0x05	0x0X���յ����Xֵͬ����
// ����CCD��Դ����				0x06	0x0X���յ����Xֵͬ����
// ������ֵ����					0x07	0x0X���յ����Xֵͬ����
// ���÷��������ʱ������		0x08	����
// ������λ����					0x09	0x0X���յ����Xֵͬ����
// �ֶ�ģʽ����V1������			0x11	0x0X���յ����Xֵͬ����
// �ֶ�ģʽ����V2������			0x12	0x0X���յ����Xֵͬ����
// �ֶ�ģʽ����V3������			0x13	0x0X���յ����Xֵͬ����
// �ֶ�ģʽ����V4������			0x14	0x0X���յ����Xֵͬ����
// �ֶ�ģʽ����TMP����			0x15	0x0X���յ����Xֵͬ����
// ��ȡ���״̬����				0x21	0xXX XX���������״̬
// ��ȡ����״̬����				0x22	0xXX XX�����ͷ���״̬
// ��ȡ��ǰ��ֵ���Unit��Torr��
// ��������������Ӧ�ı������㣩	0x27	0xXXXX XXXX XXXX������������ֵ
//										PreVac    /1E+3
//										VacOK     /1E+7
//										VacNotOK  /1E+7
// ��ȡCCGֵ����				0x31	0xXX XX������CCGֵ
// ��ȡ���ӱ�ת������			0x32	0xXX XX�����ͷ��ӱ�ת��ֵ
// ��ȡ���ӱ��¶�����			0x33	0xXX XX�����ͷ��ӱ��¶�ֵ
// ��ȡ���ӱ�����ʱ������		0x34	0xXX XX�����ͷ��ӱ�����ʱ��ֵ
// ��ȡ��״̬����				0x51	0xXX��XX��������״ֵ̬


/////////////////////////////////////////////////////////////////////
// 4. PLC����PC����Ӧ֡�������ơ���Ӧֵ�Ͳ���
// const	BYTE	byte_PLCvacBRDopc_InquireVACstatus		= 0x21;	// �������״̬������״̬
//				����1��0xXX���Զ�����״̬��
//						0 �ֶ���1 �Զ�ֹͣ��2 �ֳ�״̬��3 ����գ�4 �����
//						5 TMP����������6 TMP������ת�٣�7 TMP����VacOK
//						10 ׼��������11 ���ٷ�����12 ���ٷ���
//						19 ����״̬��20 ������д���
//				����2��0b ABCD EFGH ��1Ϊ�б�����0Ϊ�ޱ�����
//						A������
//						B��CCG���ϣ�C��MP����D��TMP���ӣ�E��TMP Fail��F��TMP����
//						G����մ���H����Ʒ����մ��

// const	BYTE	byte_PLCvacBRDopc_InquireVACvalve		= 0x22;	// ���ط���״̬
//				����1��0b ABCD EFGH ��1Ϊ����0Ϊ�أ�
//						A��CCD��B��TMP��C��CCG��D��V5��E��V4��F��V3��G��V2��H��V1
//						G����մ���H����Ʒ����մ��
//				����2��0b ABCD EFGH ��1Ϊ����0Ϊ�أ�
//						A��B��C��D��E��F������
//						G����˿ǹV1��״̬
//						H��1Ϊ�Զ�ģʽ��0Ϊ�ֶ�ģʽ


// const	BYTE	byte_PLCvacBRDopc_InquireCCG			= 0x31;	// ����CCGֵ
//				����1��2����Ϊ0xXX����Ӧ16λADCֵ����ת��Ϊ���ֵ����λTorr��
// U = (ADC/27648.0) *10.0	// ?????
// P = 10 ^ (1.667 *U -d)	// d = 11.46
// valid in the range: 3.8E-9 Torr < P < 750 Torr
// Measurement range: 1.82 ... 8.6V

// const	BYTE	byte_PLCvacBRDopc_InquireTMPr			= 0x32;	// ��ȡ���ӱ�ת��rotational speed
// const	BYTE	byte_PLCvacBRDopc_InquireTMPd			= 0x33;	// ��ȡ���ӱ��¶�degrees
// const	BYTE	byte_PLCvacBRDopc_InquireTMPt			= 0x34;	// ��ȡ���ӱ�����ʱ��time
//				����1��2����Ϊ0xXX����Ӧ16λADCֵ����ת��Ϊת�٣�r/S�����¶ȣ����϶ȣ�������ʱ�䣨Сʱ��

// const	BYTE	byte_PLCvacBRDopc_InquireStatus			= 0x51;	// ��ȡ����״ֵ̬
//				����1,2��	���״̬
//				����3,4��	����״̬
//				����5,6��	CCGֵ
//				����7,8��	���ӱ�ת��ֵ
//				����9,10��	���ӱ��¶�ֵ
//				����11,12��	���ӱ�����ʱ��ֵ
//				����13��	01Ϊ�ٵ�˿��02Ϊ��ǹ
//				����14��	01Ϊ������NAV

/////////////////////////////////////////////////////////////////////
// Thresholds:
// SEM:
const	double	d_PLCvacBRD_ThresholdSEM_VacPre1	= 1.0;
const	double	d_PLCvacBRD_ThresholdSEM_VacPre2	= 0.1;
const	double	d_PLCvacBRD_ThresholdSEM_VacOK1		= 1E-4;
const	double	d_PLCvacBRD_ThresholdSEM_VacOK2		= 8E-5;
const	double	d_PLCvacBRD_ThresholdSEM_VacNotOK1	= 1E-4;
const	double	d_PLCvacBRD_ThresholdSEM_VacNotOK2	= 8E-5;
// FEM:
const	double	d_PLCvacBRD_ThresholdFEM_VacPre1	= 3.75;
const	double	d_PLCvacBRD_ThresholdFEM_VacPre2	= 0.01;
const	double	d_PLCvacBRD_ThresholdFEM_VacOK1		= 9E-5;
const	double	d_PLCvacBRD_ThresholdFEM_VacOK2		= 1E-7;
const	double	d_PLCvacBRD_ThresholdFEM_VacNotOK1	= 9E-5;
const	double	d_PLCvacBRD_ThresholdFEM_VacNotOK2	= 1E-7;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

typedef struct tagStatus_PLCvacBrd_vac{
	WORD	XX			: 8;	// 00000000 ????????
	WORD	VacChambOK	: 1;	// 0000000? 00000000
	WORD	VacErr		: 1;	// 000000?0 00000000
	WORD	TMPErr		: 1;	// 00000?00 00000000
	WORD	TMPFail		: 1;	// 0000?000 00000000
	WORD	TMPLink		: 1;	// 000?0000 00000000
	WORD	MPErr		: 1;	// 00?00000 00000000
	WORD	CCGErr		: 1;	// 0?000000 00000000
	WORD	rev			: 1;	// ?0000000 00000000
}Status_PLCvacBrd_vac;

typedef union tagParam_PLCvacBrd_vac
{
	WORD		wStatus;
	Status_PLCvacBrd_vac	status;	
}Param_PLCvacBrd_vac;

typedef struct tagStatus_PLCvacBrd_valve{
	WORD	V1			: 1;	// 00000000 0000000?
	WORD	V2			: 1;	// 00000000 000000?0
	WORD	V3			: 1;	// 00000000 00000?00
	WORD	V4			: 1;	// 00000000 0000?000
	WORD	V5			: 1;	// 00000000 000?0000
	WORD	MP			: 1;	// 00000000 00?00000
	WORD	TMP			: 1;	// 00000000 0?000000
	WORD	CCD			: 1;	// 00000000 ?0000000
	WORD	mode		: 1;	// 0000000? 00000000
	WORD	V1_SEM		: 1;	// 000000?0 00000000
	WORD	REV			: 6;	// ??????00 00000000
}Status_PLCvacBrd_valve;

typedef union tagParam_PLCvacBrd_valve
{
	WORD		wValve;
	Status_PLCvacBrd_valve	valve;	
}Param_PLCvacBrd_valve;

typedef struct tagParam_PLCvacBrd_TMP
{
	WORD	wR;	// rotate speed
	WORD	wD;	// degree
	WORD	wT;	// time
}Param_PLCvacBrd_TMP;

typedef struct tagStatus_PLCvacBrd{
	double	dPccg;
	int		nType;	// 01Ϊ��˿ǹ��02Ϊ��ǹ

	Param_PLCvacBrd_vac		statusVac;
	Param_PLCvacBrd_valve	statusValve;
	Param_PLCvacBrd_TMP		tmp;
} Status_PLCvacBrd;

#endif
