
#ifndef __PMSKV_H
#define __PMSKV_H

#define WM_USER_SPELLMAN_PMSKV_SETPARAM		(WM_USER	+75)	// Status byte from the controller is passed to user via the wParam argument
#define WM_USER_SPELLMAN_PMSKV_UpdateSet	(WM_USER	+76)
#define WM_USER_SPELLMAN_PMSKV_UpdateMon	(WM_USER	+77)
#define WM_USER_SPELLMAN_PMSKV_UpdateStatus	(WM_USER	+78)

///////////////////////////////////////////////////////////////////////////////
// FEG 10KV
///////////////////////////////////////////////////////////////////////////////

typedef struct tagPMSKVcfg{
	short	nPort;	// COM�˿ں�
	BOOL	bReady;	// �˿��Ƿ�������

	short	nID;	// unit address
	int		nBD;	// baud rate
	short	nMode;	// <ARG> 1 = Remote, 0 = Local in ASCII format
	short	nEn;	// <ARG> 1 = HV-ON, 0 = HV-OFF in ASCII format
	float	fRangeV;// full V range
	float	fRangeI;// full I range
	float	fOutV;	// request present value of output voltage
	float	fMonV;	// voltage monitor
	float	fMonI;	// current monitor
} CfgPMSKV;
///////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// RS232
const WORD WSM30KV_SendToDevice_DataSize	= 17;
const WORD WSM30KV_ReadFromDevice_DataSize	= 513;

typedef struct tagWSM30KV_SEND_MESSAGE
{
	unsigned short	nLength;			// Avialable cData Length
	char	cData[ WSM30KV_SendToDevice_DataSize ];
} WSM30KV_Send_Msg;

typedef struct tagWSM30KV_RECV_MESSAGE
{
	unsigned short	nLength;			// Avialable bData Length
	char	cData[ WSM30KV_ReadFromDevice_DataSize ];
} WSM30KV_Recv_Msg;
/////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// SEM 30KV
///////////////////////////////////////////////////////////////////////////////

typedef struct tagWSM30KVcfg{
	short	nPort;	// COM�˿ں�
	BOOL	bReady;	// �˿��Ƿ�������
} Cfg_WSM30KV;

// Auto State

typedef enum tagWSM30KV_AutoState
{
	WSM30KVAS_Rise_ACL1		= 0,
	WSM30KVAS_Rise_ACL2		= 1,
	WSM30KVAS_Rise_FIL		= 2,
	WSM30KVAS_Rise_EMS		= 3,
	WSM30KVAS_NULL			= 4,
}WSM30KV_AutoState;

typedef enum tagWSM30KV2_AutoState
{
	WSM30KV2AS_Rise_FIL		= 0,
	WSM30KV2AS_Rise_ACL1	= 1,
	WSM30KV2AS_Rise_ACL2	= 2,
	WSM30KV2AS_Rise_EMS		= 3,
	WSM30KV2AS_NULL			= 4,
}WSM30KV2_AutoState;

// ����Ʒ����ModbusЭ�飬����������·��RS232��RS485���ϵ�ASCII����ģʽ
// �Լ�TCP/IP�ϵĴ��䣬�μ���ModbusЭ�顷��֧�����¹����룺
const	BYTE	byteWSM30KV_SnBroadcast	= 0x00;	// �㲥
const	BYTE	byteWSM30KV_Sn01		= 0x01;	// �ӻ���ַ power#1
const	BYTE	byteWSM30KV_Sn25		= 0x19;	// �ӻ���ַ power#25
const	BYTE	byteWSM30KV_FuncCodeR	= 0x03;	// �����ּĴ���
const	BYTE	byteWSM30KV_FuncCodeWRm	= 0x10;	// д����Ĵ���
const	BYTE	byteWSM30KV_FuncCodeWR	= 0x06;	// д�����Ĵ���
const	BYTE	byteWSM30KV_FuncCodeWC	= 0x05;	// д������Ȧ
const	BYTE	byteWSM30KV_FuncCodeS	= 0x16;	// ����д�Ĵ���
const	BYTE	byteWSM30KV_FuncCodeRWm	= 0x17;	// ��/д����Ĵ�����ʵ��һ����������һ��д���������

const	short	byteWSM30KV_LenReqRecv	= 7;	// �ض�����������7���ַ�
const	short	byteWSM30KV_Offset		= 1000;	// ƫ����

const	short	nWSM30KVCount_Registers	= 66;	// �ܼ�66���Ĵ���

// �Ĵ�����ַ power#1 ͨ��
const	short	AddrWSM30KVR01_INTStatus		= 1000;	// INT״̬
const	short	AddrWSM30KVR01_OutputStatus1	= 1001;	// ���״̬1
const	short	AddrWSM30KVR01_ProtectStatusB	= 1003;	// ��������״̬
const	short	AddrWSM30KVR01_ProtectStatusV	= 1004;	// ��ѹ����״̬
const	short	AddrWSM30KVR01_ProtectStatusI	= 1005;	// ��������״̬
const	short	AddrWSM30KVR01_InquireStatus1	= 1040;	// ����״̬1
const	short	AddrWSM30KVR01_InquireStatus2	= 1041;	// ����״̬2
const	short	AddrWSM30KVR01_InquireStatus3	= 1042;	// ����״̬3

const	short	AddrWSM30KVR01_FB_P3V3			= 1023;	// 3.3V���:0~54600 ��Ӧ 0 �� 3.3V.
const	short	AddrWSM30KVR01_FB_P5V			= 1024;	// 5V���:	0~54600 ��Ӧ 0 �� 5V.
const	short	AddrWSM30KVR01_FB_P15V			= 1025;	// 15V���:	0~54600 ��Ӧ 0 �� 15V.
const	short	AddrWSM30KVR01_FB_N15V			= 1026;	// -15V���:0~54600 ��Ӧ 0 �� -15V.
const	short	AddrWSM30KVR01_FB_P24V			= 1027;	// 24V���:	0~54600 ��Ӧ 0 �� 24V.
const	short	AddrWSM30KVR01_FB_Temp			= 1028;	// �¶ȼ��:0~54600 ��Ӧ 0 �� 250 ��.

const	short	AddrWSM30KVR01_FB_SCI_V			= 1007;	// SCI ��ѹ����: 0~54600 ��Ӧ 0 ����������ѹֵ ,��λ: V.
const	short	AddrWSM30KVR01_FB_SCI_I			= 1008;	// SCI ��������: 0~54600 ��Ӧ 0 ������������ֵ ,��λ: mA.
const	short	AddrWSM30KVR01_FB_ACL_V			= 1009;	// ACL ��ѹ����: 0~54600 ��Ӧ 0 ����������ѹֵ ,��λ: V.
const	short	AddrWSM30KVR01_FB_ACL_I			= 1010;	// ACL ��������: 0~54600 ��Ӧ 0 ������������ֵ ,��λ: mA.
const	short	AddrWSM30KVR01_FB_COL_V			= 1015;	// COL ��ѹ����: 0~54600 ��Ӧ 0 ����������ѹֵ ,��λ: V.
const	short	AddrWSM30KVR01_FB_COL_I			= 1016;	// COL ��������: 0~54600 ��Ӧ 0 ������������ֵ ,��λ: mA.
const	short	AddrWSM30KVR01_FB_PMT_V			= 1017;	// PMT ��ѹ����: 0~54600 ��Ӧ 0 ����������ѹֵ ,��λ: V.
const	short	AddrWSM30KVR01_FB_PMT_I			= 1018;	// PMT ��������: 0~54600 ��Ӧ 0 ������������ֵ ,��λ: mA.

const	short	AddrWSM30KVR01_Given_SCI_V		= 1032;	// SCI��ѹ������DAC ���ֵ: 0~65535 ��Ӧ 0 �����DAC ���ֵ, ��λ: V.
const	short	AddrWSM30KVR01_Given_ACL_V		= 1033;	// ACL��ѹ������DAC ���ֵ: 0~65535 ��Ӧ 0 �����DAC ���ֵ, ��λ: V.
const	short	AddrWSM30KVR01_Given_ACL_I		= 1034;	// ACL����������DAC ���ֵ: 0~65535 ��Ӧ 0 �����DAC ���ֵ, ��λ: mA.
const	short	AddrWSM30KVR01_Given_COL_V		= 1036;	// COL��ѹ������DAC ���ֵ: 0~65535 ��Ӧ 0 �����DAC ���ֵ, ��λ: V.
const	short	AddrWSM30KVR01_Given_PMT_V		= 1037;	// PMT��ѹ������DAC ���ֵ: 0~65535 ��Ӧ 0 �����DAC ���ֵ, ��λ: V.

const	short	AddrWSM30KVR01_Given_SCI_V_M	= 1043;	// SCI��ѹ��Ҫ������0~65535 ��Ӧ 0 ������ѹ����ֵ, ��λ:V.
const	short	AddrWSM30KVR01_Given_SCI_V_A	= 1044;	// SCI��ѹ����������0~255 ��Ӧ��������������΢����ѹ����ֵ,��λ:V.
const	short	AddrWSM30KVR01_Given_ACL_V_M	= 1047;	// ACL��ѹ��Ҫ������0~65535 ��Ӧ 0 ������ѹ����ֵ, ��λ:V.
const	short	AddrWSM30KVR01_Given_ACL_V_A	= 1048;	// ACL��ѹ����������0~255 ��Ӧ��������������΢����ѹ����ֵ,��λ:V.
const	short	AddrWSM30KVR01_Given_ACL_I_M	= 1051;	// ACL������Ҫ������0~65535 ��Ӧ 0 ������������ֵ, ��λ:mA.
const	short	AddrWSM30KVR01_Given_ACL_I_A	= 1052;	// ACL��������������0~255 ��Ӧ��������������΢����������ֵ,��λ:mA.
const	short	AddrWSM30KVR01_Given_COL_V_M	= 1059;	// COL��ѹ��Ҫ������0~65535 ��Ӧ 0 ������ѹ����ֵ, ��λ:V.
const	short	AddrWSM30KVR01_Given_COL_V_A	= 1060;	// COL��ѹ����������0~255 ��Ӧ��������������΢����ѹ����ֵ,��λ:V.
const	short	AddrWSM30KVR01_Given_PMT_V_M	= 1063;	// PMT��ѹ��Ҫ������0~65535 ��Ӧ 0 ������ѹ����ֵ, ��λ:V.
const	short	AddrWSM30KVR01_Given_PMT_V_A	= 1064;	// PMT��ѹ����������0~255 ��Ӧ��������������΢����ѹ����ֵ,��λ:V.

// �Ĵ�����ַ power#25 ͨ��
const	short	AddrWSM30KVR25_INTStatus		= 1000;	// INT״̬
const	short	AddrWSM30KVR25_OutputStatus1	= 1001;	// ���״̬1
const	short	AddrWSM30KVR25_ProtectStatusB	= 1003;	// ��������״̬
const	short	AddrWSM30KVR25_ProtectStatusV	= 1004;	// ��ѹ����״̬
const	short	AddrWSM30KVR25_ProtectStatusI	= 1005;	// ��������״̬
const	short	AddrWSM30KVR25_ProtectStatusArc	= 1006;	// ARC����״̬
const	short	AddrWSM30KVR25_InquireStatus1	= 1040;	// ����״̬1
const	short	AddrWSM30KVR25_InquireStatus2	= 1041;	// ����״̬2
const	short	AddrWSM30KVR25_InquireStatus3	= 1042;	// ����״̬3

const	short	AddrWSM30KVR25_FB_P3V3			= 1023;	// 3.3V��� : 0~54600 ��Ӧ 0 �� 3.3V.
const	short	AddrWSM30KVR25_FB_P5V			= 1024;	// 5V��� : 0~54600 ��Ӧ 0 �� 5V.
const	short	AddrWSM30KVR25_FB_P15V			= 1025;	// 15V��� : 0~54600 ��Ӧ 0 �� 15V.
const	short	AddrWSM30KVR25_FB_N15V			= 1026;	// -15V��� : 0~54600 ��Ӧ0 �� -15V.
const	short	AddrWSM30KVR25_FB_P24V			= 1027;	// 24V��� : 0~54600 ��Ӧ 0 �� 24V.
const	short	AddrWSM30KVR25_FB_Temp			= 1028;	// �¶ȼ�� : 0~54600 ��Ӧ 0 �� 250 ��.

const	short	AddrWSM30KVR25_FB_FIL_V			= 1009;	// FIL ��ѹ����: 0~54600 ��Ӧ 0 ����������ѹֵ ,��λ: V.
const	short	AddrWSM30KVR25_FB_FIL_I			= 1010;	// FIL ��������: 0~54600 ��Ӧ 0 ������������ֵ ,��λ: mA.
const	short	AddrWSM30KVR25_Given_FIL_I		= 1033;	// FIL�������DAC ���ֵ: 0~65535 ��Ӧ 0 �����DAC ���ֵ, ��λ:A.
const	short	AddrWSM30KVR25_Given_FIL_I_M	= 1047;	// FIL��Ҫ������0~65535 ��Ӧ 0������������ֵ, ��λ:A.
const	short	AddrWSM30KVR25_Given_FIL_I_A	= 1048;	// FIL����������0~255 ��Ӧ��������������΢����������ֵ, ��λ:mA.

// ��Ȧ��ַ power#1 ͨ��
const	short	AddrWSM30KVC01_ReqOn_Power1		= 1084;	// �����Դ SCI ��ѹ��
const	short	AddrWSM30KVC01_ReqOn_Power2		= 1085;	// �����Դ ACL ��ѹ��
const	short	AddrWSM30KVC01_ReqOn_Power3		= 1086;	// �����Դ EMS ������
const	short	AddrWSM30KVC01_ReqOn_Power5		= 1088;	// �����Դ COL ��ѹ��
const	short	AddrWSM30KVC01_ReqOn_Power6		= 1089;	// �����Դ PMT ��ѹ��
const	short	AddrWSM30KVC01_ReqPause_Power1	= 1092;	// �����Դ SCI ��ѹ ��ͣ����
const	short	AddrWSM30KVC01_ReqPause_Power2	= 1093;	// �����Դ ACL ��ѹ ��ͣ����
const	short	AddrWSM30KVC01_ReqPause_Power3	= 1094;	// �����Դ EMS ���� ��ͣ����
const	short	AddrWSM30KVC01_ReqPause_Power5	= 1096;	// �����Դ COL ��ѹ ��ͣ����
const	short	AddrWSM30KVC01_ReqPause_Power6	= 1097;	// �����Դ PMT ��ѹ ��ͣ����
const	short	AddrWSM30KVC01_ReqReset_Power1	= 1108;	// �����Դ SCI ������λ
const	short	AddrWSM30KVC01_ReqReset_Power2	= 1109;	// �����Դ ACL ������λ
const	short	AddrWSM30KVC01_ReqReset_Power3	= 1110;	// �����Դ EMS ������λ
const	short	AddrWSM30KVC01_ReqReset_Power5	= 1112;	// �����Դ COL ������λ
const	short	AddrWSM30KVC01_ReqReset_Power6	= 1113;	// �����Դ PMT ������λ
const	short	AddrWSM30KVC01_ReqReset			= 1117;	// ����ȫ����λ

const	short	AddrWSM30KVC25_ReqOn_Power2		= 1085;	// �����Դ FIL ��˿��
const	short	AddrWSM30KVC25_ReqPause_Power2	= 1093;	// �����Դ FIL ��ͣ����
const	short	AddrWSM30KVC25_ReqReset_Power2	= 1109;	// �����Դ FIL ������λ
const	short	AddrWSM30KVC25_ReqReset			= 1117;	// ����ȫ����λ

// ���ֵ
const	WORD	g_wWSM30KVFS_1		= 54600;
const	WORD	g_wWSM30KVFS_2		= 65535;
const	BYTE	g_bWSM30KVFS_3		= 255;

const	float	g_fWSM30KVFS_SCI_mon_V	= 10000.0;	// V
const	float	g_fWSM30KVFS_SCI_mon_I	= 250.0;	// uA
const	float	g_fWSM30KVFS_ACL_mon_V	= 30000.0;	// V
const	float	g_fWSM30KVFS_ACL_mon_I	= 400.0;	// uA
const	float	g_fWSM30KVFS_COL_mon_V	= 800.0;	// ?????
const	float	g_fWSM30KVFS_COL_mon_I	= 100.0;	// uA
const	float	g_fWSM30KVFS_PMT_mon_V	= 1300.0;	// V
const	float	g_fWSM30KVFS_PMT_mon_I	= 1.2f;		// mA
const	float	g_fWSM30KVFS_FIL_mon_V	= 5.0f;		// V
const	float	g_fWSM30KVFS_FIL_mon_I	= 4.0f;		// A


/////////////////////////////////////////////////
// INT״̬��1000��λ˵��
typedef struct tagWSM30KVStatus_INTStatus_BitDefine{
	WORD			: 9;	// δ��
	WORD	ACL_FIL	: 1;	//
	WORD	SCI		: 1;	//
	WORD	PMT		: 1;	//
	WORD	COL		: 1;	//
	WORD			: 3;	// δ��
}StatusWSM30KV_INTStatus_BitDefine;

typedef union tagWSM30KVStatus_INTStatus
{
	WORD	wStatus;
	StatusWSM30KV_INTStatus_BitDefine	status;	
}StatusWSM30KV_INTStatus;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ���״̬1��1001��λ˵��
typedef struct tagWSM30KVStatus_Output1Status_BitDefine{
	WORD	SCI		: 1;	// 00000000 0000000?��SCI -- ��SCI ON����ʱ,��λ����ź�1.(1 ʹ��)
	WORD	ACL_FIL	: 1;	// 00000000 000000?0��ACL/FIL -- ��ACL/FIL ON����ʱ,��λ����ź�1.(1 ʹ��)
	WORD	EMS		: 1;	// 00000000 00000?00��EMS -- ��EMS ON����ʱ,��λ����ź�1.(1 ʹ��)
	WORD			: 1;	// 00000000 0000?000��δ��
	WORD	COL		: 1;	// 00000000 000?0000��COL -- ��COL ON����ʱ,��λ����ź�1.(1 ʹ��)
	WORD	PMT		: 1;	// 00000000 00?00000��PMT -- ��PMT ON����ʱ,��λ����ź�1.(1 ʹ��)
	WORD			: 10;	// ???????? ??000000��δ��
}StatusWSM30KV_Output1Status_BitDefine;

typedef union tagWSM30KVStatus_Output1
{
	WORD	wStatus;
	StatusWSM30KV_Output1Status_BitDefine	status;	
}StatusWSM30KV_Output1Status;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ��������״̬��1003��λ˵��
typedef struct tagWSM30KVStatus_ProtectStatusB_BitDefine{
	WORD	P3V3	: 1;	// 0000000?��+3V3 ����(0 ������ 1 ����)
	WORD	P5V		: 1;	// 000000?0��+5V ����(0 ������ 1 ����)
	WORD	P15V	: 1;	// 00000?00��+15V ����(0 ������ 1 ����)
	WORD	N15V	: 1;	// 0000?000��-15V ����(0 ������ 1 ����)
	WORD	P24V	: 1;	// 000?0000��+24V ����(0 ������ 1 ����)
	WORD	Temp	: 1;	// 00?00000���¶�1 ����(0 ������ 1 ����)
	WORD			: 10;	// δ��
}StatusWSM30KV_ProtectStatusB_BitDefine;

typedef union tagWSM30KVStatus_ProtectStatusB
{
	WORD	wStatus;
	StatusWSM30KV_ProtectStatusB_BitDefine	status;	
}StatusWSM30KV_ProtectStatusB;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ��ѹ����״̬��1004��λ˵��
typedef struct tagWSM30KVStatus_ProtectStatusV_BitDefine{
	WORD	SCI		: 1;	// 00000000 0000000?��SCI ��ѹ����(0 ������ 1 ����)
	WORD	ACL_FIL	: 1;	// 00000000 000000?0��ACL/FIL ��ѹ����(0 ������ 1 ����)
	WORD			: 2;	// 00000000 0000??00��δ��
	WORD	COL		: 1;	// 00000000 000?0000��COL ��ѹ����(0 ������ 1 ����)
	WORD	PMT		: 1;	// 00000000 00?00000��PMT ��ѹ����(0 ������ 1 ����)
	WORD			: 10;	// ???????? ??000000��δ��
}StatusWSM30KV_ProtectStatusV_BitDefine;

typedef union tagWSM30KVStatus_ProtectStatusV
{
	WORD	wStatus;
	StatusWSM30KV_ProtectStatusV_BitDefine	status;	
}StatusWSM30KV_ProtectStatusV;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ��������״̬��1005��λ˵��
typedef struct tagWSM30KVStatus_ProtectStatusI_BitDefine{
	WORD	SCI		: 1;	// 00000000 0000000?��SCI ��������(0 ������ 1 ����)
	WORD	ACL_FIL	: 1;	// 00000000 000000?0��ACL/FIL ��������(0 ������ 1 ����)
	WORD			: 2;	// 00000000 0000??00��δ��
	WORD	COL		: 1;	// 00000000 000?0000��COL ��������(0 ������ 1 ����)
	WORD	PMT		: 1;	// 00000000 00?00000��PMT ��������(0 ������ 1 ����)
	WORD			: 10;	// ???????? ??000000��δ��
}StatusWSM30KV_ProtectStatusI_BitDefine;

typedef union tagWSM30KVStatus_ProtectStatusI
{
	WORD	wStatus;
	StatusWSM30KV_ProtectStatusI_BitDefine	status;	
}StatusWSM30KV_ProtectStatusI;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ARC����״̬��1006��λ˵��
typedef struct tagWSM30KVStatus_ProtectStatusArc_BitDefine{
	WORD			: 1;	// 0000000?��δ��
	WORD	ACL		: 1;	// 000000?0��ACL ��������(0 ������ 1 ����)
	WORD			: 14;	// ??????00��δ��
}StatusWSM30KV_ProtectStatusArc_BitDefine;

typedef union tagWSM30KVStatus_ProtectStatusArc
{
	WORD	wStatus;
	StatusWSM30KV_ProtectStatusArc_BitDefine	status;	
}StatusWSM30KV_ProtectStatusArc;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ����״̬1��1040��λ˵��
typedef struct tagWSM30KVStatus_InquireStatus1_BitDefine{
	WORD	SCI		: 1;	// 0000000?������ SCI ��ѹ����1 ����
	WORD	ACL_FIL	: 1;	// 000000?0������ ACL/FIL ��ѹ����1 ����
	WORD	EMS		: 1;	// 00000?00������ EMS ��ѹ����1 ����
	WORD			: 1;	// 0000?000��δ��
	WORD	COL		: 1;	// 000?0000������ COL ��ѹ����1 ����
	WORD	PMT		: 1;	// 00?00000������ PMT ��ѹ����1 ����
	WORD			: 10;	// ??000000��δ��
}StatusWSM30KV_InquireStatus1_BitDefine;

typedef union tagWSM30KVStatus_InquireStatus1
{
	WORD	wStatus;
	StatusWSM30KV_InquireStatus1_BitDefine	status;	
}StatusWSM30KV_InquireStatus1;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ����״̬2��1041��λ˵��
typedef struct tagWSM30KVStatus_InquireStatus2_BitDefine{
	WORD			: 8;
	WORD	SCI		: 1;	// 0000000?������ SCI ��λ��1 ����
	WORD	ACL_FIL	: 1;	// 000000?0������ ACL/FIL ��λ��1 ����
	WORD	EMS		: 1;	// 00000?00������ EMS ��λ��1 ����
	WORD			: 1;	// 0000?000��δ��
	WORD	COL		: 1;	// 000?0000������ COL ��λ��1 ����
	WORD	PMT		: 1;	// 00?00000������ PMT ��λ��1 ����
	WORD			: 2;	// ??000000��δ��
}StatusWSM30KV_InquireStatus2_BitDefine;

typedef union tagWSM30KVStatus_InquireStatus2
{
	WORD	wStatus;
	StatusWSM30KV_InquireStatus2_BitDefine	status;	
}StatusWSM30KV_InquireStatus2;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ����״̬3��1042��λ˵��
typedef struct tagWSM30KVStatus_InquireStatus3_BitDefine{
	WORD			: 1;	// 0000000?��δ��
	WORD	Reset	: 1;	// 000000?0������ȫ����λ��1 ����
	WORD			: 14;	// ??????00��δ��
}StatusWSM30KV_InquireStatus3_BitDefine;

typedef union tagWSM30KVStatus_InquireStatus3
{
	WORD	wStatus;
	StatusWSM30KV_InquireStatus3_BitDefine	status;	
}StatusWSM30KV_InquireStatus3;
/////////////////////////////////////////////////



typedef struct tagWSM30KVStatus{
	StatusWSM30KV_INTStatus			statusINT01;
	StatusWSM30KV_Output1Status		statusO01;
	StatusWSM30KV_ProtectStatusB	statusB01;
	StatusWSM30KV_ProtectStatusV	statusV01;
	StatusWSM30KV_ProtectStatusI	statusI01;
	StatusWSM30KV_InquireStatus1	status101;
	StatusWSM30KV_InquireStatus2	status201;
	StatusWSM30KV_InquireStatus3	status301;

	StatusWSM30KV_INTStatus			statusINT25;
	StatusWSM30KV_Output1Status		statusO25;
	StatusWSM30KV_ProtectStatusB	statusB25;
	StatusWSM30KV_ProtectStatusV	statusV25;
	StatusWSM30KV_ProtectStatusI	statusI25;
	StatusWSM30KV_ProtectStatusArc	statusA25;
	StatusWSM30KV_InquireStatus1	status125;
	StatusWSM30KV_InquireStatus2	status225;
	StatusWSM30KV_InquireStatus3	status325;
}Status_WSM30KV;

typedef struct tagWSM30KVMon_VoltTemp{
	float	fP3V3[2];
	float	fP5V[2];
	float	fP15V[2];
	float	fN15V[2];
	float	fP24V[2];
	float	fTemp[2];
}MonVT_WSM30KV;

typedef struct tagWSM30KVMon_DACOutput{
	float	fSCIv;
	float	fACLv;
	float	fACLi;
	float	fCOLv;
	float	fPMTv;
	float	fFILi;
}MonDACOutput_WSM30KV;

typedef struct tagWSM30KVMon_feedback{
	float	fSCIv;
	float	fSCIi;
	float	fACLv;
	float	fACLi;
	float	fCOLv;
	float	fCOLi;
	float	fPMTv;
	float	fPMTi;
	float	fFILv;
	float	fFILi;
}MonFeedback_WSM30KV;

typedef struct tagWSM30KVMon{
	MonVT_WSM30KV			monVT;
	MonDACOutput_WSM30KV	monOP;
	MonFeedback_WSM30KV		monFB;
}Mon_WSM30KV;

typedef struct tagWSM30KVSetting{
	float	fSCIv;
	float	fACLv;
	float	fACLi;
	float	fCOLv;
	float	fPMTv;
	float	fFILi;
}Setting_WSM30KV;
///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
// SEM 30KV: SEM30N6X7605������ʹ�õ�30KV��˿ǹ��ѹ��Դ��
///////////////////////////////////////////////////////////////////////////////
/*
typedef struct tagWSM30KV2cfg{
	short	nPort;	// COM�˿ں�
	BOOL	bReady;	// �˿��Ƿ�������
} Cfg_WSM30KV2;

// Auto State
typedef enum tagWSM30KV2_AutoState
{
	WSM30KV2AS_Rise_ACL1	= 0,
	WSM30KV2AS_Rise_ACL2	= 1,
	WSM30KV2AS_Rise_FIL		= 2,
	WSM30KV2AS_Rise_EMS		= 3,
	WSM30KV2AS_NULL			= 4,
}WSM30KV2_AutoState;
*/
// ����Ʒ����ModbusЭ�飬����������·��RS232��RS485���ϵ�ASCII����ģʽ
// �Լ�TCP/IP�ϵĴ��䣬�μ���ModbusЭ�顷��֧�����¹����룺
const	BYTE	byteWSM30KV2_SnBroadcast	= 0x00;	// �㲥
const	BYTE	byteWSM30KV2_Sn				= 0x01;	// �ӻ���ַ power#1
const	BYTE	byteWSM30KV2_FuncCodeR		= 0x03;	// �����ּĴ���
const	BYTE	byteWSM30KV2_FuncCodeWRm	= 0x10;	// д����Ĵ���
const	BYTE	byteWSM30KV2_FuncCodeWR		= 0x06;	// д�����Ĵ���
const	BYTE	byteWSM30KV2_FuncCodeWC		= 0x05;	// д������Ȧ
const	BYTE	byteWSM30KV2_FuncCodeS		= 0x16;	// ����д�Ĵ���
const	BYTE	byteWSM30KV2_FuncCodeRWm	= 0x17;	// ��/д����Ĵ�����ʵ��һ����������һ��д���������

const	short	byteWSM30KV2_LenReqRecv		= 7;	// �ض�����������7���ַ�
const	short	byteWSM30KV2_Offset			= 1000;	// ƫ����

const	short	nWSM30KV2Count_Registers	= 68;	// �ܼ�68���Ĵ���

// �Ĵ�����ַ power#1 ͨ��
//const	short	AddrWSM30KV2R_INTStatus			= 1000;	// INT״̬
const	short	AddrWSM30KV2R_OutputStatus1		= 1001;	// ���״̬1
const	short	AddrWSM30KV2R_ProtectStatusB	= 1003;	// ��������״̬
const	short	AddrWSM30KV2R_ProtectStatusV	= 1004;	// ��ѹ����״̬
const	short	AddrWSM30KV2R_ProtectStatusI	= 1005;	// ��������״̬
const	short	AddrWSM30KV2R_ProtectStatusA	= 1006;	// ARC����״̬
const	short	AddrWSM30KV2R_InquireStatus1	= 1040;	// ����״̬1
const	short	AddrWSM30KV2R_InquireStatus2	= 1041;	// ����״̬2
const	short	AddrWSM30KV2R_InquireStatus3	= 1042;	// ����״̬3

const	short	AddrWSM30KV2R_FB_P3V3			= 1023;	// 3.3V���:0~54600 ��Ӧ 0 �� 3.3V.
const	short	AddrWSM30KV2R_FB_P5V			= 1024;	// 5V���:	0~54600 ��Ӧ 0 �� 5V.
const	short	AddrWSM30KV2R_FB_P15V			= 1025;	// 15V���:	0~54600 ��Ӧ 0 �� 15V.
const	short	AddrWSM30KV2R_FB_N15V			= 1026;	// -15V���:0~54600 ��Ӧ 0 �� -15V.
const	short	AddrWSM30KV2R_FB_P24V			= 1027;	// 24V���:	0~54600 ��Ӧ 0 �� 24V.
const	short	AddrWSM30KV2R_FB_Temp			= 1028;	// �¶ȼ��:0~54600 ��Ӧ 0 �� 250 ��.

const	short	AddrWSM30KV2R_FB_BIAS_V			= 1007;	// BIAS ��ѹ����: 0~54600 ��Ӧ 0 �� -2000V �����ѹֵ ,��λ: V.
const	short	AddrWSM30KV2R_FB_FIL_I			= 1010;	// FIL ��������: 0~54600 ��Ӧ 0 �� 3A �������ֵ ,��λ: mA.
const	short	AddrWSM30KV2R_FB_ACL_V			= 1011;	// ACL ��ѹ����: 0~54600 ��Ӧ 0 �� -30000V �����ѹֵ ,��λ: V.
const	short	AddrWSM30KV2R_FB_ACL_I			= 1012;	// ACL ����������������: 0~54600 ��Ӧ 0 �� 240uA �������ֵ ,��λ: uA.
const	short	AddrWSM30KV2R_FB_SCI_V			= 1013;	// SCI ��ѹ����: 0~54600 ��Ӧ 0 �� 10000V �������ֵ ,��λ: V.
const	short	AddrWSM30KV2R_FB_SCI_I			= 1014;	// SCI ��������: 0~54600 ��Ӧ 0 �� 250uA �������ֵ ,��λ: uA.
const	short	AddrWSM30KV2R_FB_PMT_V			= 1015;	// PMT ��ѹ����: 0~54600 ��Ӧ 0 �� -1300V �����ѹֵ ,��λ: V.
const	short	AddrWSM30KV2R_FB_PMT_I			= 1016;	// PMT ��������: 0~54600 ��Ӧ 0 �� 1.2mA �������ֵ ,��λ: mA.
const	short	AddrWSM30KV2R_FB_COL_V			= 1017;	// COL ��ѹ����: 0~54600 ��Ӧ 0 �� 400V �����ѹֵ ,��λ: V.
const	short	AddrWSM30KV2R_FB_COL_I			= 1018;	// COL ��������: 0~54600 ��Ӧ 0 �� 5mA �������ֵ ,��λ: mA.

const	short	AddrWSM30KV2R_Given_BIAS_V		= 1043;	// BIAS��ѹ��Ҫ������0~65535 ��Ӧ 0 �� -2000V ��ѹ����ֵ,��λ:V.
const	short	AddrWSM30KV2R_Given_FIL_I		= 1047;	// FIL������Ҫ������0~65535 ��Ӧ 0 �� 3A ��������ֵ,��λ:A.
const	short	AddrWSM30KV2R_Given_ACL_V		= 1051;	// ACL��ѹ��Ҫ������0~65535 ��Ӧ 0 �� -30000V ��ѹ����ֵ, ��λ:V.
const	short	AddrWSM30KV2R_Given_SCI_V		= 1055;	// SCI��ѹ��Ҫ������0~65535 ��Ӧ 0 �� 10000V ��ѹ����ֵ, ��λ:V.
const	short	AddrWSM30KV2R_Given_ACL_I		= 1059;	// ACL��������������Ҫ������0~65535 ��Ӧ 0 �� 240uA ��������ֵ, ��λ:mA.
const	short	AddrWSM30KV2R_Given_COL_V		= 1063;	// COL��ѹ��Ҫ������0~65535 ��Ӧ 0 �� 400V ��ѹ����ֵ, ��λ:V.
const	short	AddrWSM30KV2R_Given_PMT_V		= 1067;	// PMT��ѹ��Ҫ������0~65535 ��Ӧ 0 �� -1300V ��ѹ����ֵ, ��λ:V.

// ��Ȧ��ַ power#1 ͨ��
const	short	AddrWSM30KV2C_ReqOn_Power1		= 1084;	// �����Դ BIAS ��ѹ��
const	short	AddrWSM30KV2C_ReqOn_Power2		= 1085;	// �����Դ FIL ��˿��
const	short	AddrWSM30KV2C_ReqOn_Power3		= 1086;	// �����Դ ACL ��ѹ��
const	short	AddrWSM30KV2C_ReqOn_Power4		= 1087;	// �����Դ SCI ��ѹ��
const	short	AddrWSM30KV2C_ReqOn_Power5		= 1088;	// �����Դ EMS(ACL����)��
const	short	AddrWSM30KV2C_ReqOn_Power6		= 1089;	// �����Դ COL ��ѹ��
const	short	AddrWSM30KV2C_ReqOn_Power7		= 1090;	// �����Դ PMT ��ѹ��
const	short	AddrWSM30KV2C_ReqPause_Power1	= 1092;	// �����Դ BIAS ��ѹ ��ͣ����
const	short	AddrWSM30KV2C_ReqPause_Power2	= 1093;	// �����Դ FIL ��˿ ��ͣ����
const	short	AddrWSM30KV2C_ReqPause_Power3	= 1094;	// �����Դ ACL ��ѹ ��ͣ����
const	short	AddrWSM30KV2C_ReqPause_Power4	= 1095;	// �����Դ SCI ��ѹ ��ͣ����
const	short	AddrWSM30KV2C_ReqPause_Power5	= 1096;	// �����Դ EMS(ACL����) ��ͣ����
const	short	AddrWSM30KV2C_ReqPause_Power6	= 1097;	// �����Դ COL ��ѹ ��ͣ����
const	short	AddrWSM30KV2C_ReqPause_Power7	= 1098;	// �����Դ PMT ��ѹ ��ͣ����
const	short	AddrWSM30KV2C_ReqReset_Power1	= 1108;	// �����Դ BIAS ������λ
const	short	AddrWSM30KV2C_ReqReset_Power2	= 1109;	// �����Դ FIL ������λ
const	short	AddrWSM30KV2C_ReqReset_Power3	= 1110;	// �����Դ ACL ������λ
const	short	AddrWSM30KV2C_ReqReset_Power4	= 1111;	// �����Դ SCI ������λ
const	short	AddrWSM30KV2C_ReqReset_Power5	= 1112;	// �����Դ EMS(ACL����) ������λ
const	short	AddrWSM30KV2C_ReqReset_Power6	= 1113;	// �����Դ COL ������λ
const	short	AddrWSM30KV2C_ReqReset_Power7	= 1114;	// �����Դ PMT ������λ
const	short	AddrWSM30KV2C_ReqReset			= 1117;	// ����ȫ����λ

// ���ֵ
const	WORD	g_wWSM30KV2FS_1		= 54600;
const	WORD	g_wWSM30KV2FS_2		= 65535;

const	float	g_fWSM30KV2FS_BIAS_mon_V= 2000.0;	// V
const	float	g_fWSM30KV2FS_FIL_mon_I	= 3.0f;		// A
const	float	g_fWSM30KV2FS_ACL_mon_V	= 30000.0;	// V
const	float	g_fWSM30KV2FS_ACL_mon_I	= 240.0;	// uA
const	float	g_fWSM30KV2FS_SCI_mon_V	= 10000.0;	// V
const	float	g_fWSM30KV2FS_SCI_mon_I	= 250.0;	// uA
const	float	g_fWSM30KV2FS_PMT_mon_V	= 1300.0;	// V
const	float	g_fWSM30KV2FS_PMT_mon_I	= 1.2f;		// mA
const	float	g_fWSM30KV2FS_COL_mon_V	= 400.0;	// V
const	float	g_fWSM30KV2FS_COL_mon_I	= 5.0;		// mA

/*
/////////////////////////////////////////////////
// INT״̬��1000��λ˵��
typedef struct tagWSM30KV2Status_INTStatus_BitDefine{
	WORD			: 9;	// δ��
	WORD	ACL_FIL	: 1;	//
	WORD	SCI		: 1;	//
	WORD	PMT		: 1;	//
	WORD	COL		: 1;	//
	WORD			: 3;	// δ��
}StatusWSM30KV2_INTStatus_BitDefine;

typedef union tagWSM30KV2Status_INTStatus
{
	WORD	wStatus;
	StatusWSM30KV2_INTStatus_BitDefine	status;	
}StatusWSM30KV2_INTStatus;
/////////////////////////////////////////////////
*/
/////////////////////////////////////////////////
// ���״̬1��1001��λ˵��
typedef struct tagWSM30KV2Status_Output1Status_BitDefine{
	WORD	BIAS	: 1;	// 00000000 0000000?��BIAS -- ��BIAS ON����ʱ,��λ����ź�1.(1 ʹ��)
	WORD	FIL		: 1;	// 00000000 000000?0��FIL -- ��FIL ON����ʱ,��λ����ź�1.(1 ʹ��)
	WORD	ACL		: 1;	// 00000000 00000?00��ACL -- ��ACL ON����ʱ,��λ����ź�1.(1 ʹ��)
	WORD	SCI		: 1;	// 00000000 0000?000��SCI -- ��SCI ON����ʱ,��λ����ź�1.(1 ʹ��)
	WORD	EMS		: 1;	// 00000000 000?0000��EMS -- ��EMS ON(ACL��������)����ʱ,��λ����ź�1.(1 ʹ��)
	WORD	COL		: 1;	// 00000000 00?00000��COL -- ��COL ON����ʱ,��λ����ź�1.(1 ʹ��)
	WORD	PMT		: 1;	// 00000000 0?000000��PMT -- ��PMT ON����ʱ,��λ����ź�1.(1 ʹ��)
	WORD			: 1;	// 00000000 ?0000000��δ��
	WORD	INTLK	: 1;	// 0000000? 00000000��INTLK -- ��λ�����ź�Ϊ1�������պ�
	WORD			: 7;	// ???????0 00000000��δ��
}StatusWSM30KV2_Output1Status_BitDefine;

typedef union tagWSM30KV2Status_Output1
{
	WORD	wStatus;
	StatusWSM30KV2_Output1Status_BitDefine	status;	
}StatusWSM30KV2_Output1Status;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ��������״̬��1003��λ˵��
typedef struct tagWSM30KV2Status_ProtectStatusB_BitDefine{
	WORD	P3V3	: 1;	// 0000000?��+3V3 ����(0 ������ 1 ����)
	WORD	P5V		: 1;	// 000000?0��+5V ����(0 ������ 1 ����)
	WORD	P15V	: 1;	// 00000?00��+15V ����(0 ������ 1 ����)
	WORD	N15V	: 1;	// 0000?000��-15V ����(0 ������ 1 ����)
	WORD	P24V	: 1;	// 000?0000��+24V ����(0 ������ 1 ����)
	WORD			: 1;	// 00?00000��δ��
	WORD	Temp	: 1;	// 0?000000���¶� ����(0 ������ 1 ����)
	WORD			: 9;	// δ��
}StatusWSM30KV2_ProtectStatusB_BitDefine;

typedef union tagWSM30KV2Status_ProtectStatusB
{
	WORD	wStatus;
	StatusWSM30KV2_ProtectStatusB_BitDefine	status;	
}StatusWSM30KV2_ProtectStatusB;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ��ѹ����״̬��1004��λ˵��
typedef struct tagWSM30KV2Status_ProtectStatusV_BitDefine{
	WORD	BIAS	: 1;	// 00000000 0000000?��BIAS ��ѹ����(0 ������ 1 ����)
	WORD			: 1;	// 00000000 000000?0��δ��
	WORD	ACL		: 1;	// 00000000 00000?00��ACL ��ѹ����(0 ������ 1 ����)
	WORD	SCI		: 1;	// 00000000 0000?000��SCI ��ѹ����(0 ������ 1 ����)
	WORD	PMT		: 1;	// 00000000 000?0000��PMT ��ѹ����(0 ������ 1 ����)
	WORD	COL		: 1;	// 00000000 00?00000��COL ��ѹ����(0 ������ 1 ����)
	WORD			: 10;	// ???????? ??000000��δ��
}StatusWSM30KV2_ProtectStatusV_BitDefine;

typedef union tagWSM30KV2Status_ProtectStatusV
{
	WORD	wStatus;
	StatusWSM30KV2_ProtectStatusV_BitDefine	status;	
}StatusWSM30KV2_ProtectStatusV;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ��������״̬��1005��λ˵��
typedef struct tagWSM30KV2Status_ProtectStatusI_BitDefine{
	WORD			: 1;	// 00000000 0000000?��δ��
	WORD	FIL		: 1;	// 00000000 000000?0��FIL ��������(0 ������ 1 ����)
	WORD	ACL		: 1;	// 00000000 00000?00��ACL ��������(0 ������ 1 ����)
	WORD	SCI		: 1;	// 00000000 0000?000��SCI ��������(0 ������ 1 ����)
	WORD	PMT		: 1;	// 00000000 000?0000��PMT ��������(0 ������ 1 ����)
	WORD	COL		: 1;	// 00000000 00?00000��COL ��������(0 ������ 1 ����)
	WORD			: 10;	// ???????? ??000000��δ��
}StatusWSM30KV2_ProtectStatusI_BitDefine;

typedef union tagWSM30KV2Status_ProtectStatusI
{
	WORD	wStatus;
	StatusWSM30KV2_ProtectStatusI_BitDefine	status;	
}StatusWSM30KV2_ProtectStatusI;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ARC����״̬��1006��λ˵��
typedef struct tagWSM30KV2Status_ProtectStatusArc_BitDefine{
	WORD			: 2;	// 000000??��δ��
	WORD	ACL		: 1;	// 00000?00��ACL ��������(0 ������ 1 ����)
	WORD			: 13;	// ?????000��δ��
}StatusWSM30KV2_ProtectStatusArc_BitDefine;

typedef union tagWSM30KV2Status_ProtectStatusArc
{
	WORD	wStatus;
	StatusWSM30KV2_ProtectStatusArc_BitDefine	status;	
}StatusWSM30KV2_ProtectStatusArc;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ����״̬1��1040��λ˵��
typedef struct tagWSM30KV2Status_InquireStatus1_BitDefine{
	WORD	ON_BIAS		: 1;	// 00000000 0000000?������ BIAS ��ѹ����1 ����
	WORD	ON_FIL		: 1;	// 00000000 000000?0������ FIL ��˿����1 ����
	WORD	ON_ACL		: 1;	// 00000000 00000?00������ ACL ��ѹ����1 ����
	WORD	ON_SCI		: 1;	// 00000000 0000?000������ SCI ��ѹ����1 ����
	WORD	ON_EMS		: 1;	// 00000000 000?0000������ EMS(ACL����)����1 ����
	WORD	ON_COL		: 1;	// 00000000 00?00000������ COL ��ѹ����1 ����
	WORD	ON_PMT		: 1;	// 00000000 0?000000������ PMT ��ѹ����1 ����
	WORD				: 1;	// 00000000 ?0000000��δ��
	WORD	PAUSE_BIAS	: 1;	// 0000000? 00000000������ BIAS ��ͣ������1 ����
	WORD	PAUSE_FIL	: 1;	// 000000?0 00000000������ FIL ��ͣ������1 ����
	WORD	PAUSE_ACL	: 1;	// 00000?00 00000000������ ACL ��ͣ������1 ����
	WORD	PAUSE_SCI	: 1;	// 0000?000 00000000������ SCI ��ͣ������1 ����
	WORD	PAUSE_EMS	: 1;	// 000?0000 00000000������ EMS(ACL����)��ͣ������1 ����
	WORD	PAUSE_COL	: 1;	// 00?00000 00000000������ COL ��ͣ������1 ����
	WORD	PAUSE_PMT	: 1;	// 0?000000 00000000������ PMT ��ͣ������1 ����
	WORD				: 1;	// ?0000000 00000000��δ��
}StatusWSM30KV2_InquireStatus1_BitDefine;

typedef union tagWSM30KV2Status_InquireStatus1
{
	WORD	wStatus;
	StatusWSM30KV2_InquireStatus1_BitDefine	status;	
}StatusWSM30KV2_InquireStatus1;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ����״̬2��1041��λ˵��
typedef struct tagWSM30KV2Status_InquireStatus2_BitDefine{
	WORD			: 8;
	WORD	BIAS	: 1;	// 0000000?������ BIAS ��λ��1 ����
	WORD	FIL		: 1;	// 000000?0������ FIL ��λ��1 ����
	WORD	ACL		: 1;	// 00000?00������ ACL ��λ��1 ����
	WORD	SCI		: 1;	// 0000?000������ SCI ��λ��1 ����
	WORD	EMS		: 1;	// 000?0000������ EMS(ACL����) ��λ��1 ����
	WORD	COL		: 1;	// 00?00000������ COL ��λ��1 ����
	WORD	PMT		: 1;	// 0?000000������ PMT ��λ��1 ����
	WORD			: 1;	// ?0000000��δ��
}StatusWSM30KV2_InquireStatus2_BitDefine;

typedef union tagWSM30KV2Status_InquireStatus2
{
	WORD	wStatus;
	StatusWSM30KV2_InquireStatus2_BitDefine	status;	
}StatusWSM30KV2_InquireStatus2;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ����״̬3��1042��λ˵��
typedef struct tagWSM30KV2Status_InquireStatus3_BitDefine{
	WORD			: 1;	// 0000000?��δ��
	WORD	Reset	: 1;	// 000000?0������ȫ����λ��1 ����
	WORD			: 14;	// ??????00��δ��
}StatusWSM30KV2_InquireStatus3_BitDefine;

typedef union tagWSM30KV2Status_InquireStatus3
{
	WORD	wStatus;
	StatusWSM30KV2_InquireStatus3_BitDefine	status;	
}StatusWSM30KV2_InquireStatus3;
/////////////////////////////////////////////////



typedef struct tagWSM30KV2Status{
	StatusWSM30KV2_Output1Status	statusO;
	StatusWSM30KV2_ProtectStatusB	statusB;
	StatusWSM30KV2_ProtectStatusV	statusV;
	StatusWSM30KV2_ProtectStatusI	statusI;
	StatusWSM30KV2_ProtectStatusArc	statusA;
	StatusWSM30KV2_InquireStatus1	status1;
	StatusWSM30KV2_InquireStatus2	status2;
	StatusWSM30KV2_InquireStatus3	status3;
}Status_WSM30KV2;

typedef struct tagWSM30KV2Mon_VoltTemp{
	float	fP3V3;
	float	fP5V;
	float	fP15V;
	float	fN15V;
	float	fP24V;
	float	fTemp;
}MonVT_WSM30KV2;

typedef struct tagWSM30KV2Mon_DACOutput{
	float	fBIASv;
	float	fFILi;
	float	fACLv;
	float	fSCIv;
	float	fACLi;
	float	fCOLv;
	float	fPMTv;
}MonDACOutput_WSM30KV2;

typedef struct tagWSM30KV2Mon_feedback{
	float	fBIASv;
	float	fFILi;
	float	fACLv;
	float	fACLi;
	float	fSCIv;
	float	fSCIi;
	float	fPMTv;
	float	fPMTi;
	float	fCOLv;
	float	fCOLi;
}MonFeedback_WSM30KV2;

typedef struct tagWSM30KV2Mon{
	MonVT_WSM30KV2			monVT;
	MonDACOutput_WSM30KV2	monOP;
	MonFeedback_WSM30KV2	monFB;
}Mon_WSM30KV2;

typedef struct tagWSM30KV2Setting{
	float	fBIASv;
	float	fFILi;
	float	fACLv;
	float	fSCIv;
	float	fACLi;
	float	fCOLv;
	float	fPMTv;
}Setting_WSM30KV2;
///////////////////////////////////////////////////////////////////////////////


#endif